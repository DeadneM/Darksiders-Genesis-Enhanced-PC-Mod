#include "ActionRecoveryFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::recovery {
namespace {
using TickFn=void(*)(void*,float);
using FinishFn=void(*)(void*,bool);
TickFn g_tick=nullptr;
FinishFn g_finish=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_requested[2]{};
std::atomic_uint32_t g_armed[2]{};
// Value-only history, used exclusively by the native game thread. No cached
// UObject is dereferenced later. A new/expired activation must arm again.
struct Phase {
    uintptr_t local=0,character=0,ability=0;
    float elapsed=0,duration=0;
    ULONGLONG tick=0;
    bool armed=false,requested=false;
};
Phase g_phase[2]{};
struct AimWatch {uintptr_t local=0,character=0;ULONGLONG dash=0,sample=0;unsigned count[4]{};};
AimWatch g_aimWatch[2]{};
struct PadSample {short lx=0,ly=0,rx=0,ry=0;bool known=false,swap=false;ULONGLONG tick=0;};
thread_local PadSample g_physical[4]{};
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
template<class T> bool Read(const void* object,size_t offset,T& value){
    if(!object||reinterpret_cast<uintptr_t>(object)>UINTPTR_MAX-offset)return false;
    const auto* p=static_cast<const unsigned char*>(object)+offset;
    if(!Readable(p,sizeof(value)))return false;
    std::memcpy(&value,p,sizeof(value));return true;
}
bool Typed(void* object,uintptr_t classSlot){
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;void* item=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+classSlot),0,expected)&&expected&&
        Read(object,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
bool Alive(void* actor){
    void* resource=nullptr;float health=0;unsigned char role=0;
    return Read(actor,0xEA0,resource)&&Typed(resource,0x3810268)&&
        Read(resource,0x118,role)&&role==3&&Read(resource,0x37C,health)&&
        std::isfinite(health)&&health>0;
}
bool ActiveComponent(void* ability,void* actor){
    void* component=nullptr;void* instigator=nullptr;void* owner=nullptr;void* array=nullptr;
    int count=0,capacity=0;
    // Reflected Instigator is an ACTOR, also confirmed in the recorded dash
    // captures. It is not the actor's ability component at C70.
    if(!Read(actor,0xC70,component)||!component||!Read(ability,0x48,instigator)||instigator!=actor||
       !Read(component,0xE8,owner)||owner!=actor||!Read(component,0xF8,array)||!array||
       !Read(component,0x100,count)||!Read(component,0x104,capacity)||
       count<=0||count>capacity||capacity>64)return false;
    for(int i=0;i<count;++i){void* item=nullptr;if(!Read(array,size_t(i)*8,item))return false;if(item==ability)return true;}
    return false;
}
bool Owner(void* ability,local_players::Context& owner){
    void* actor=nullptr;void* table=nullptr;uintptr_t movementTick=0,cleanup=0;
    unsigned char state=0,role=0,type=0;void* helper=nullptr;void* movement=nullptr;
    unsigned char mode=0,custom=0;
    // The base dash class also has other subclasses. Only the observed
    // decelerating dash implementation and its complete native cleanup fit
    // the fields below. Chests, mounted/Ultimate and unknown variants pass.
    return Typed(ability,0x3810F90)&&Read(ability,0,table)&&
        Read(table,0x2E0,movementTick)&&movementTick==g_base+0x5B93F0&&
        Read(table,0x2A8,cleanup)&&cleanup==g_base+0x59C110&&
        Read(ability,0xD8,state)&&state==2&&Read(ability,0x1D8,helper)&&helper&&
        Read(ability,0x1B0,actor)&&actor&&local_players::ResolveActor(actor,owner)&&
        owner.index>=0&&owner.index<2&&owner.character==reinterpret_cast<uintptr_t>(actor)&&
        owner.kind==local_players::PoseKind::Foot&&
        Read(actor,0x118,role)&&role==3&&Read(actor,0xC18,type)&&(type==1||type==2)&&Alive(actor)&&
        Read(actor,0x388,movement)&&movement&&Read(movement,0x1B0,mode)&&mode==6&&
        Read(movement,0x1B1,custom)&&custom==5&&ActiveComponent(ability,actor);
}
void Tick(void* ability,float dt,ULONGLONG now){
    // Native input, montage notifies and the FULL physics step always run
    // once at the original dt, before evaluating recovery.
    if(g_tick)g_tick(ability,dt);
    if(!native_tps::OnGameThread())return;
    const auto& rt=runtime::Get();
    if(!g_ready.load(std::memory_order_acquire)||!g_finish){
        g_phase[0]={};g_phase[1]={};return;
    }
    if(!std::isfinite(dt)||dt<=0||dt>0.1f)return;
    local_players::Context owner{};
    if(!Owner(ability,owner))return;
    auto& watch=g_aimWatch[owner.index];
    watch.local=owner.localPlayer;watch.character=owner.character;watch.dash=now;
    if(!rt.actionRecoveryEnabled.load()){g_phase[0]={};g_phase[1]={};return;}
    auto& phase=g_phase[owner.index];
    if(native_tps::IsWorldPaused(reinterpret_cast<void*>(owner.character))||
       native_tps::CinematicActive(unsigned(owner.index))){phase={};return;}
    float remaining=0,duration=0,elapsed=0;
    const float delay=rt.actionRecoveryDelayMs.load();
    if(!Read(ability,0x224,remaining)||!Read(ability,0x228,duration)||!Read(ability,0xDC,elapsed)||
       !std::isfinite(remaining)||!std::isfinite(duration)||!std::isfinite(elapsed)||
       !std::isfinite(delay)||duration<=0||duration>5||remaining>duration+0.001f||remaining < -5||
       elapsed<0||elapsed>15||delay<0||delay>300){phase={};return;}
    const bool same=phase.local==owner.localPlayer&&phase.character==owner.character&&
        phase.ability==reinterpret_cast<uintptr_t>(ability)&&phase.tick&&now>=phase.tick&&
        now-phase.tick<=250&&elapsed>=phase.elapsed&&duration==phase.duration;
    if(!same)phase={owner.localPlayer,owner.character,reinterpret_cast<uintptr_t>(ability),elapsed,duration,now,false,false};
    phase.elapsed=elapsed;phase.tick=now;
    // 5B0F40 explicitly starts deceleration by setting BOTH 224 and 228.
    // Zero-initialized or stale negative clocks never authorize completion.
    if(remaining>0){
        if(!phase.armed&&!phase.requested){
            const unsigned n=++g_armed[owner.index];
            if(g_log&&n<=8){char line[180]{};
                sprintf_s(line,"Recovery V1.49 DASH_DECELERATION player=%d n=%u elapsed=%.3f duration=%.3f remaining=%.4f",
                    owner.index+1,n,elapsed,duration,remaining);g_log(line);}
        }
        phase.armed=true;return;
    }
    if(!phase.armed||phase.requested||-remaining+0.000001f<delay*0.001f)return;
    phase.requested=true;phase.armed=false;
    const unsigned n=++g_requested[owner.index];
    if(g_log&&(n<=8||n%1000==0)){
        char line[220]{};
        sprintf_s(line,"Recovery V1.49 DASH_FINISH_REQUEST player=%d n=%u elapsed=%.3f deceleration=%.3f remaining=%.4f delayMs=%.1f nativeCleanup=1 gameplayValidation=USER_REQUIRED",
            owner.index+1,n,elapsed,duration,remaining,delay);g_log(line);
    }
    // The same EndAbility(false) used by the native dash Tick. 59B2D0
    // dispatches 59C110: stops the montage, clears temporary state, restores
    // movement and clears dash velocity. No MOVE flag or actor write here.
    // Do not touch the ability again: completion can detach/recycle it.
    g_finish(ability,false);
}
void HookTick(void* ability,float dt){Tick(ability,dt,GetTickCount64());}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x5B72B0,{0x40,0x53,0x48,0x83,0xEC,0x30,0x0F,0x29,0x74,0x24,0x20,0x48,0x8B,0xD9,0x0F,0x28}},
        {0x5B0F40,{0x40,0x53,0x48,0x83,0xEC,0x30,0x48,0x8B,0xD9,0x0F,0x29,0x74,0x24,0x20,0x48,0x8B}},
        {0x5B93F0,{0x48,0x8B,0xC4,0x48,0x89,0x58,0x10,0x48,0x89,0x78,0x18,0x55,0x48,0x8D,0x68,0xA1}},
        {0x59B080,{0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48}},
        {0x59C110,{0x40,0x57,0x48,0x83,0xEC,0x60,0x48,0xC7,0x44,0x24,0x30,0xFE,0xFF,0xFF,0xFF,0x48}},
        {0x7BBFF0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x92,0x4F,0x05,0x03,0x48,0x85}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Recovery V1.49 signature mismatch; native dash retained");return;
    }
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x5B72B0);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookTick),reinterpret_cast<void**>(&g_tick))!=MH_OK)return;
    g_finish=reinterpret_cast<FinishFn>(g_base+0x59B080);
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_tick=nullptr;g_finish=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(log)log("Recovery V1.49 READY: observed completed dash deceleration only; native finish, P1/P2, original physics dt; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void Shutdown(){g_ready.store(false,std::memory_order_release);}
void ObserveControllerInput(unsigned id,short lx,short ly,short rx,short ry,bool known,bool swap){
    if(id<4&&g_ready.load()&&native_tps::OnGameThread())g_physical[id]={lx,ly,rx,ry,known,swap,GetTickCount64()};
}
void ObserveNativeLook(void* actor,float x,float y){
    if(!g_log||!g_ready.load()||!native_tps::OnGameThread())return;
    local_players::Context owner{};
    if(!local_players::ResolveActor(actor,owner)||owner.index<0||owner.index>1||
       owner.character!=reinterpret_cast<uintptr_t>(actor))return;
    auto& watch=g_aimWatch[owner.index];const auto now=GetTickCount64();const auto& rt=runtime::Get();
    const unsigned mode=(rt.thirdPersonEnabled.load()?2u:0u)|(rt.actionRecoveryEnabled.load()?1u:0u);
    if(watch.local!=owner.localPlayer||watch.character!=owner.character||!watch.dash||
       now<watch.dash||now-watch.dash>3000||watch.count[mode]>=48||
       (watch.sample&&now>=watch.sample&&now-watch.sample<250))return;
    watch.sample=now;++watch.count[mode];
    float rawX=0,rawY=0;void* movement=nullptr;void* component=nullptr;void* array=nullptr;
    unsigned char mode=0,custom=0;int count=0,capacity=0;uintptr_t action=0,table=0;
    Read(actor,0xD24,rawX);Read(actor,0xD28,rawY);
    if(Read(actor,0x388,movement)){Read(movement,0x1B0,mode);Read(movement,0x1B1,custom);}
    if(Read(actor,0xC70,component)&&Read(component,0xF8,array)&&
       Read(component,0x100,count)&&Read(component,0x104,capacity)&&count>0&&count<=capacity&&capacity<=64&&
       Read(array,0,action))Read(reinterpret_cast<void*>(action),0,table);
    PadSample pad{};
    if(owner.controllerId>=0&&owner.controllerId<4)pad=g_physical[owner.controllerId];
    const bool physicalFresh=pad.tick&&now>=pad.tick&&now-pad.tick<=250;
    char line[450]{};
    sprintf_s(line,"Recovery V1.52 AIM_AFTER_DASH player=%d recovery=%d tps=%d ageMs=%llu physicalFresh=%d rawPad=(%d,%d;%d,%d) nativeSwap=%d rawWorldLook=(%.3f,%.3f) nativeGetter=(%.3f,%.3f) movement=%u/%u activeCount=%d firstActionVtableRva=%llX readOnly=1",
        owner.index+1,rt.actionRecoveryEnabled.load()?1:0,rt.thirdPersonEnabled.load()?1:0,now-watch.dash,physicalFresh?1:0,pad.lx,pad.ly,pad.rx,pad.ry,pad.known?int(pad.swap):-1,rawX,rawY,x,y,unsigned(mode),unsigned(custom),count,table>=g_base?table-g_base:0);g_log(line);
}
void Report(){if(g_log){char line[210]{};sprintf_s(line,"Recovery V1.49 totals positivePhasesP1=%u P2=%u nativeFinishRequestsP1=%u P2=%u gameplayValidation=USER_REQUIRED",g_armed[0].load(),g_armed[1].load(),g_requested[0].load(),g_requested[1].load());g_log(line);}}
}
