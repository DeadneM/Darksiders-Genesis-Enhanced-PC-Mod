#include "ActionRecoveryFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "DashCombatPolicy.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <array>
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
dash_combat::AimResume g_resume[2]{};
using NotifyFn=void(*)(void*,void*,void*);
NotifyFn g_notify=nullptr;
std::atomic_bool g_combatReady{false};
std::atomic_uint32_t g_resumed{0},g_suppressed{0},g_unavailable{0};
using ActionQueryFn=unsigned char(*)(void*,unsigned char);
using StartAimFn=void(*)(void*);
ActionQueryFn g_actionQuery=nullptr;
StartAimFn g_startAim=nullptr;
thread_local bool g_resuming=false;
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
bool CombatAction(void* actor,bool& active){
    // Native GetActionState thunk 770D00 uses this exact action interface.
    // ECharacterActions: ranged one=18, ranged two=20, AIM=31. Native dash
    // code also queries 18/20 here. These are actions, never physical keys.
    void* table=nullptr;uintptr_t query=0;
    if(!Read(actor,0x8D0,table)||!Read(table,0x18,query)||!query)return false;
    using State=unsigned char(*)(void*,unsigned char);
    const auto state=reinterpret_cast<State>(query);
    void* actionInterface=static_cast<unsigned char*>(actor)+0x8D0;
    active=state(actionInterface,18)!=0||state(actionInterface,20)!=0||state(actionInterface,31)!=0;
    return true;
}
template<size_t N> bool NameIs(void* object,const char (&expected)[N]){
    int index=-1,number=-1;uintptr_t names=0,chunk=0,entry=0;uint32_t flags=0;
    if(!Read(object,0x18,index)||!Read(object,0x1C,number)||number||index<0||index>=2000000||
       !Read(reinterpret_cast<void*>(g_base+0x3955198),0,names)||!names||
       !Read(reinterpret_cast<void*>(names),size_t(index/16384)*8,chunk)||!chunk||
       !Read(reinterpret_cast<void*>(chunk),size_t(index%16384)*8,entry)||!entry||
       !Read(reinterpret_cast<void*>(entry),8,flags))return false;
    if(flags&1){
        std::array<wchar_t,N> text{};if(!Read(reinterpret_cast<void*>(entry),0xC,text))return false;
        for(size_t i=0;i<N;++i)if(text[i]!=wchar_t(expected[i]))return false;
        return true;
    }
    std::array<char,N> text{};
    return Read(reinterpret_cast<void*>(entry),0xC,text)&&std::memcmp(text.data(),expected,N)==0;
}
template<size_t N> bool ClassNameIs(void* object,const char (&expected)[N]){
    void* cls=nullptr;return Read(object,0x10,cls)&&NameIs(cls,expected);
}
bool CombatOwner(void* actor,local_players::Context& owner){
    unsigned char type=0,role=0;
    return actor&&local_players::ResolveActor(actor,owner)&&owner.index>=0&&owner.index<2&&
        owner.character==reinterpret_cast<uintptr_t>(actor)&&owner.kind==local_players::PoseKind::Foot&&
        Read(actor,0xC18,type)&&type==1&&Read(actor,0x118,role)&&role==3&&Alive(actor)&&
        !native_tps::IsWorldPaused(actor)&&!native_tps::CinematicActive(unsigned(owner.index));
}
bool AbilityArray(void* actor,void*& array,int& count){
    void* component=nullptr;void* reciprocal=nullptr;int capacity=0;
    return Read(actor,0xC70,component)&&component&&Read(component,0xE8,reciprocal)&&reciprocal==actor&&
        Read(component,0xF8,array)&&Read(component,0x100,count)&&Read(component,0x104,capacity)&&
        count>=0&&count<=capacity&&capacity<=64&&(!count||array);
}
bool AimActions(void* actor,bool& aim,bool& primary,bool& secondary){
    void* table=nullptr;uintptr_t query=0;
    // Recorded Strife action interface; 669D30 -> 635100 reads the native
    // action-state array. No XInput mask, key, axis or synthetic release.
    if(!g_actionQuery||!Read(actor,0x8D0,table)||!Read(table,0x18,query)||
       query!=reinterpret_cast<uintptr_t>(g_actionQuery))return false;
    auto fn=g_actionQuery;
    void* iface=static_cast<unsigned char*>(actor)+0x8D0;
    aim=fn(iface,31)!=0;primary=fn(iface,18)!=0;secondary=fn(iface,20)!=0;return true;
}
bool GroundDashActive(void* actor){
    void* array=nullptr;int count=0;
    if(!AbilityArray(actor,array,count))return false;
    for(int i=0;i<count;++i){
        void* ability=nullptr;void* table=nullptr;void* instigator=nullptr;void* character=nullptr;
        void* helper=nullptr;uintptr_t physics=0,cleanup=0;unsigned char state=0;
        if(Read(array,size_t(i)*8,ability)&&Typed(ability,0x3810F90)&&Read(ability,0,table)&&
           Read(table,0x2E0,physics)&&physics==g_base+0x5B93F0&&
           Read(table,0x2A8,cleanup)&&cleanup==g_base+0x59C110&&
           Read(ability,0xD8,state)&&state==2&&Read(ability,0x1D8,helper)&&helper&&
           Read(ability,0x48,instigator)&&instigator==actor&&
           Read(ability,0x1B0,character)&&character==actor)return true;
    }
    return false;
}
void* MainPistols(void* actor){
    // The action-context ranged getter 66A7F0 returns null when its E08
    // context stack is empty. General AIM instead belongs to DualPistolsBP:
    // reflected Strife member 1038, also registered in native weapon slot 5.
    void* component=nullptr;void* reciprocal=nullptr;void* array=nullptr;
    void* weapon=nullptr;void* reflected=nullptr;void* aimClass=nullptr;
    int count=0,capacity=0;
    if(!Read(actor,0xC78,component)||!Read(component,0xE8,reciprocal)||reciprocal!=actor||
       !Read(component,0x108,array)||!array||!Read(component,0x110,count)||
       !Read(component,0x114,capacity)||count<6||count>capacity||capacity>32||
       !Read(array,5*8,weapon)||!Read(actor,0x1038,reflected)||weapon!=reflected)return nullptr;
    return Typed(weapon,0x3811D20)&&Read(weapon,0xE8,reciprocal)&&reciprocal==actor&&
        ClassNameIs(weapon,"MayhemWeapon_DualPistols_Blueprint_C")&&
        Read(weapon,0x618,aimClass)&&
        NameIs(aimClass,"MayhemAbility_Strife_DualPistols_Aim_Start_Ability_C")?weapon:nullptr;
}
dash_combat::Ammo EquippedAmmo(void* actor,unsigned slot){
    void* component=nullptr;void* array=nullptr;void* weapon=nullptr;void* reciprocal=nullptr;
    int count=0,capacity=0;
    if(!Read(actor,0xC78,component)||!Read(component,0x108,array)||!array||
       !Read(component,0x110,count)||!Read(component,0x114,capacity)||count<=int(slot)||
       count>capacity||capacity>32||!Read(array,size_t(slot)*8,weapon)||
       !Typed(weapon,0x3811D20)||!Read(weapon,0xE8,reciprocal)||reciprocal!=actor)
        return dash_combat::Ammo::Unknown;
    if(ClassNameIs(weapon,"MayhemStandardShot_Blueprint_C"))return dash_combat::Ammo::Standard;
    const bool special=ClassNameIs(weapon,"MayhemStaticShot_Blueprint_C")||
        ClassNameIs(weapon,"MayhemChargeShot_Blueprint_C")||ClassNameIs(weapon,"MayhemGravityShot_Blueprint_C")||
        ClassNameIs(weapon,"MayhemLavaShot_Blueprint_C")||ClassNameIs(weapon,"MayhemNatureShot_Blueprint_C")||
        ClassNameIs(weapon,"MayhemWorldEnder_Blueprint_C");
    return special?dash_combat::Ammo::Special:dash_combat::Ammo::Unknown;
}
void HookSpawnNotify(void* notify,void* mesh,void* animation){
    bool suppress=false;
    if(g_combatReady.load(std::memory_order_acquire)&&native_tps::OnGameThread()){
        void* actor=nullptr;void* outer=nullptr;void* forced=nullptr;local_players::Context owner{};
        bool aim=false,primary=false,secondary=false;
        const bool local=Read(mesh,0xE8,actor)&&CombatOwner(actor,owner);
        const bool sequence=local&&(NameIs(animation,"Strife_Aim_Dash_Fire_F")||NameIs(animation,"Strife_Aim_Dash_Fire_B")||
            NameIs(animation,"Strife_Aim_Dash_Fire_L")||NameIs(animation,"Strife_Aim_Dash_Fire_R"));
        if(sequence&&Typed(notify,0x3807E38)&&Read(notify,0x20,outer)&&outer==animation&&
           Read(notify,0x50,forced)&&NameIs(forced,"MayhemProjectile_Standard_Blueprint_C")&&
           GroundDashActive(actor)&&AimActions(actor,aim,primary,secondary)){
            suppress=dash_combat::SuppressForcedStandard(true,true,primary,EquippedAmmo(actor,2),secondary,EquippedAmmo(actor,3));
            if(suppress){const unsigned n=++g_suppressed;if(g_log&&(n<=8||n%1000==0)){
                char line[200]{};sprintf_s(line,"Dash combat V1.56 FORCED_STANDARD_SKIPPED player=%d n=%u selectedSpecialResumesNatively=1 gameplay=EXPERIMENTAL",owner.index+1,n);g_log(line);}}
        }
    }
    if(!suppress&&g_notify)g_notify(notify,mesh,animation);
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
    float elapsedDash=0;
    if(Read(ability,0xDC,elapsedDash)&&std::isfinite(elapsedDash)&&elapsedDash>=0)
        g_resume[owner.index].Observe(owner.localPlayer,owner.character,
            reinterpret_cast<uintptr_t>(ability),elapsedDash,now);
    if(!rt.actionRecoveryEnabled.load()){g_phase[0]={};g_phase[1]={};return;}
    auto& phase=g_phase[owner.index];
    bool combat=false;
    if(!CombatAction(reinterpret_cast<void*>(owner.character),combat)||combat){phase={};return;}
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
    const Guard combatGuards[]={
        {0x5D8130,{0x48,0x89,0x6C,0x24,0x18,0x57,0x48,0x83,0xEC,0x40,0x48,0x8B,0xBA,0xE8,0x00,0x00}},
        {0x69FBD0,{0x40,0x53,0x48,0x83,0xEC,0x50,0x48,0x83,0xB9,0x18,0x06,0x00,0x00,0x00,0x48,0x8B}},
        {0x7CE080,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x92,0x3C,0x04,0x03,0x48,0x85}},
        {0x669D30,{0x48,0x81,0xC1,0x40,0x05,0x00,0x00,0xE9,0xC4,0xB3,0xFC,0xFF,0xCC,0xCC,0xCC,0xCC}},
        {0x742030,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xFA,0x5D,0x0C,0x03,0x48,0x85}}
    };
    for(const auto& guard:combatGuards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Dash combat V1.56 signature mismatch; native combat retained");return;
    }
    void* notify=reinterpret_cast<void*>(g_base+0x5D8130);
    if(MH_CreateHook(notify,reinterpret_cast<void*>(&HookSpawnNotify),reinterpret_cast<void**>(&g_notify))!=MH_OK)return;
    if(MH_EnableHook(notify)!=MH_OK){MH_RemoveHook(notify);g_notify=nullptr;return;}
    g_actionQuery=reinterpret_cast<ActionQueryFn>(g_base+0x669D30);
    g_startAim=reinterpret_cast<StartAimFn>(g_base+0x69FBD0);
    g_combatReady.store(true,std::memory_order_release);
    if(log)log("Dash combat V1.56 READY: live-player held-AIM resume via registered DualPistolsBP after idle ground dash; V1.55 forced-standard scope retained; TPS/Recovery independent; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void Shutdown(){g_combatReady.store(false,std::memory_order_release);g_ready.store(false,std::memory_order_release);}
void OnLivePlayer(void* actor){
    if(!g_ready.load()||!g_combatReady.load()||!g_startAim||g_resuming||!native_tps::OnGameThread())return;
    const auto now=GetTickCount64();bool pending=false;
    for(auto& resume:g_resume){
        if(resume.seen&&(now<resume.seen||now-resume.seen>3000))resume={};
        pending=pending||(resume.seen&&!resume.requested);
    }
    if(!pending)return;
    local_players::Context owner{};
    if(!CombatOwner(actor,owner)){
        if(owner.index>=0&&owner.index<2&&g_resume[owner.index].actor==reinterpret_cast<uintptr_t>(actor))
            g_resume[owner.index]={};
        return;
    }
    auto& resume=g_resume[owner.index];
    if(!resume.seen||resume.requested)return;
    if(now<resume.seen||now-resume.seen>3000||resume.local!=owner.localPlayer||
       resume.actor!=owner.character){resume={};return;}
    void* array=nullptr;int count=0;void* movement=nullptr;unsigned char mode=0;
    bool aim=false,primary=false,secondary=false;
    if(!AbilityArray(actor,array,count)||count||!Read(actor,0x388,movement)||
       !Read(movement,0x1B0,mode)||(mode!=1&&mode!=2)||!AimActions(actor,aim,primary,secondary))return;
    if(!aim){resume={};return;}
    // The native player-camera callback runs even without an aim ability.
    // A nested callback cannot request a second ability.
    struct Scope {Scope(){g_resuming=true;}~Scope(){g_resuming=false;}} scope;
    void* pistols=MainPistols(actor);
    if(!pistols){
        resume={}; // Unknown contract: stop, never retry every frame.
        const unsigned n=++g_unavailable;
        if(g_log&&n<=8)g_log("Dash combat V1.56 AIM_WEAPON_UNAVAILABLE; native action retained, no retry for this dash");
        return;
    }
    if(!resume.Take(owner.localPlayer,owner.character,now,true,true,aim))return;
    g_startAim(pistols);
    const unsigned n=++g_resumed;
    if(g_log&&(n<=8||n%1000==0)){char line[190]{};
        sprintf_s(line,"Dash combat V1.56 AIM_RESTART_REQUEST player=%d n=%u nativeHeldAim=1 oncePerDash=1 gameplay=EXPERIMENTAL",owner.index+1,n);g_log(line);}
}
void Report(){if(g_log){char line[260]{};sprintf_s(line,"Recovery/Dash V1.56 totals positivePhasesP1=%u P2=%u nativeFinishRequestsP1=%u P2=%u aimRestartRequests=%u forcedStandardSkipped=%u gameplayValidation=USER_REQUIRED",g_armed[0].load(),g_armed[1].load(),g_requested[0].load(),g_requested[1].load(),g_resumed.load(),g_suppressed.load());g_log(line);}}
}
