#include "AirDashFeature.h"
#include "AirDashGesture.h"
#include "AirDashScriptContract.h"
#include "DashCombatFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <array>
#include <atomic>
#include <cmath>

namespace dg::air_dash {
namespace {
using Id=native_objects::Identity;
uintptr_t g_base=0;std::atomic_bool g_ready{false};bool g_dispatching=false;LogFn g_log=nullptr;Dispatch g_dispatch=nullptr;
Id g_source{},g_air{},g_finish{};bool g_contract=false;unsigned g_events=0;
struct Saved {Id ability{};float fall=0,forward=0,timeout=0,oursTimeout=0;};
struct Player {
    local_players::Context context{};uint32_t generation=0;Id actor{};
    Press press{};bool allowed=false,strife=false,await=false;uint64_t observed=0,until=0;
    Gesture request=Gesture::Native;Id gestureAbility{};
    Saved saved{};
};
std::array<Player,2> g_players{};
template<class T>bool Read(uintptr_t p,size_t at,T& out){native_objects::MemoryReader r;return r.Read(p,at,out);}
template<size_t N>bool Is(uintptr_t p,const char(&s)[N]){return native_objects::NameIs(g_base,p,s);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
bool Writable(uintptr_t p,size_t size){
    MEMORY_BASIC_INFORMATION m{};if(!p||!VirtualQuery(reinterpret_cast<void*>(p),&m,sizeof(m))||m.State!=MEM_COMMIT||(m.Protect&PAGE_GUARD))return false;
    const auto access=m.Protect&255;const auto at=p-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return (access==PAGE_READWRITE||access==PAGE_WRITECOPY)&&at<=m.RegionSize&&size<=m.RegionSize-at;
}
void Float(uintptr_t p,size_t at,float value){std::memcpy(reinterpret_cast<void*>(p+at),&value,sizeof(value));}
void Restore(Saved& saved){
    if(Alive(saved.ability)){
        const auto p=saved.ability.address;float value=0;
        if(Writable(p+0x1F8,8)){
            if(Read(p,0x1F8,value)&&value==0)Float(p,0x1F8,saved.fall);
            if(Read(p,0x1FC,value)&&value==3000)Float(p,0x1FC,saved.forward);
        }
        if(Writable(p+0x44,4)&&Read(p,0x44,value)&&value==saved.oursTimeout)Float(p,0x44,saved.timeout);
    }
    saved={};
}
uintptr_t Function(uintptr_t cls,const char* wanted){
    uintptr_t f=0;if(!Read(cls,0x48,f))return 0;
    for(unsigned n=0;f&&n<1024;++n){int name=-1,number=-1;uintptr_t type=0,next=0;
        if(!Read(f,0x18,name)||!Read(f,0x1C,number))return 0;
        if(native_objects::NameValueIdentifierEquals(g_base,name,number,wanted))
            return Read(f,0x10,type)&&Is(type,"Function")?f:0;
        if(!Read(f,0x28,next)||next==f)return 0;f=next;
    }return 0;
}
bool Reference(const unsigned char* code,const ranged_script::Block& b){
    const auto address=reinterpret_cast<uintptr_t>(code);int name=-1,number=-1;
    if(b.width==8){
        uintptr_t p=0;if(!Read(address,b.offset,p))return false;
        // EX_Let can carry an explicitly null UProperty (cooked index None).
        // A null pointer is admissible only at that exact declared reference.
        if(std::strcmp(b.name,"None")==0)return p==0;
        if(!Read(p,0x18,name)||!Read(p,0x1C,number))return false;
    }else if(b.width==12){if(!Read(address,b.offset,name)||!Read(address,b.offset+8,number))return false;}
    else return false;
    return native_objects::NameValueIdentifierEquals(g_base,name,number,b.name);
}
template<unsigned Size,size_t N>bool Script(uintptr_t function,uint64_t hash,const std::array<ranged_script::Block,N>& blocks){
    struct Array {uintptr_t data=0;int count=0,capacity=0;} a;std::array<unsigned char,Size> code{};
    if(!Read(function,0x58,a)||a.count!=int(Size)||a.capacity<a.count||a.capacity>65536||!Read(a.data,0,code))return false;
    return ranged_script::Matches(code,Size,hash,blocks,[&](const auto& b){return Reference(code.data(),b);});
}
bool Contract(uintptr_t actor){
    uintptr_t cls=0;if(!Read(actor,0x10,cls))return false;
    if(Alive(g_source)&&g_source.address==cls)return g_contract&&Alive(g_air);
    g_contract=false;g_air={};g_finish={};g_source={};if(!Capture(cls,g_source))return false;
    using namespace air_dash_contract;
    const auto perform=Function(cls,"PerformDash");uintptr_t code=0,air=0,package=0,cdo=0,table=0,physics=0,cancel=0,cleanup=0;int fall=-1,forward=-1,timeout=-1;
    if(!perform||!Script<PerformSize>(perform,PerformHash,PerformBlocks)||!Read(perform,0x58,code))return false;
    for(const auto& b:PerformBlocks)if(std::strcmp(b.name,"MayhemAbility_Strife_AirDash_Blueprint_C")==0){if(!Read(code,b.offset,air))return false;break;}
    const auto finish=Function(air,"OnFinish"),graph=Function(air,"ExecuteUbergraph_MayhemAbility_Strife_AirDash_Blueprint");
    uintptr_t defaultClass=0,parameter=0,parameterOwner=0,next=0;uint32_t defaultFlags=0;int causeAt=-1;
    uint16_t parameterBytes=0;
    const bool valid=air&&Is(air,"MayhemAbility_Strife_AirDash_Blueprint_C")&&Read(air,0x20,package)&&
        Is(package,"/Game/Blueprints/Abilities/Dash/MayhemAbility_Strife_AirDash_Blueprint")&&
        finish&&graph&&Script<FinishSize>(finish,FinishHash,FinishBlocks)&&Script<GraphSize>(graph,GraphHash,GraphBlocks)&&
        Read(finish,0x9E,parameterBytes)&&parameterBytes==1&& // ProcessEvent copies exactly this size.
        native_objects::PropertyField(g_base,finish,"Cause","EnumProperty",1,causeAt)&&causeAt==0&&
        Read(finish,0x48,parameter)&&Read(parameter,0x20,parameterOwner)&&parameterOwner==finish&&Read(parameter,0x28,next)&&!next&&
        Read(air,0xF8,cdo)&&Read(cdo,0x10,defaultClass)&&defaultClass==air&&Read(cdo,8,defaultFlags)&&(defaultFlags&0x10u)&&
        Read(cdo,0,table)&&Read(table,0x2E0,physics)&&physics==g_base+0x5B9180&&
        Read(table,0x290,cancel)&&cancel==g_base+0x59C660&&Read(table,0x2A8,cleanup)&&cleanup==g_base+0x59BF80&&
        native_objects::PropertyField(g_base,air,"FallSpeed","FloatProperty",4,fall)&&fall==0x1F8&&
        native_objects::PropertyField(g_base,air,"ForwardSpeed","FloatProperty",4,forward)&&forward==0x1FC&&
        native_objects::PropertyField(g_base,air,"AbilityTimeout","FloatProperty",4,timeout)&&timeout==0x44&&Capture(air,g_air)&&Capture(finish,g_finish);
    g_contract=valid;
    if(g_log)g_log(valid?"Air dash V1.152: complete native selection/finish scripts and native timeout/physics verified; tap/hold EXPERIMENTAL":
        "Air dash V1.152: contract unavailable; immediate native action retained");
    return valid;
}
bool Gameplay(const Player& p){
    const auto& c=p.context;uintptr_t controller=0,pawn=0,resource=0;float health=0;unsigned char role=0;
    return p.allowed&&p.strife&&c.localPlayer&&c.controller&&c.world&&c.kind==local_players::PoseKind::Foot&&c.pawn==c.character&&Alive(p.actor)&&
        local_players::Matches(unsigned(c.index),c.character,p.generation)&&Read(c.character,0x348,controller)&&controller==c.controller&&
        Read(controller,0x350,pawn)&&pawn==c.character&&Read(c.character,0x118,role)&&role==3&&
        Read(c.character,0xEA0,resource)&&Read(resource,0x37C,health)&&std::isfinite(health)&&health>0&&
        !native_tps::IsWorldPaused(reinterpret_cast<void*>(c.character))&&!native_tps::CinematicActive(unsigned(c.index))&&!native_tps::RadialActive(unsigned(c.index));
}
bool Airborne(const Player& p){
    uintptr_t movement=0;unsigned char mode=0,custom=0;
    return Read(p.context.character,0x388,movement)&&Read(movement,0x1B0,mode)&&Read(movement,0x1B1,custom)&&
        (mode==3||(mode==6&&custom==2)); // the same falling/gliding branch as native PerformDash
}
void Send(Player& p,Gesture gesture,uint64_t now){
    p.await=true;p.request=gesture;p.until=now+250;
    struct Guard{Guard(){g_dispatching=true;}~Guard(){g_dispatching=false;}} guard;
    g_dispatch(reinterpret_cast<void*>(p.context.character+0x8D0),14);
}
}
void Initialize(LogFn log,bool exactExecutable,bool finishBoundaryReady){
    g_log=log;if(!exactExecutable||!finishBoundaryReady)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard{uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x5B46C0,{0x40,0x53,0x48,0x83,0xEC,0x30,0xF3,0x0F,0x10,0x51,0x44,0x0F,0x57,0xC0,0x0F,0x2F}},
        {0x5B9180,{0x48,0x8B,0xC4,0x55,0x48,0x8D,0x68,0xE8,0x48,0x81,0xEC,0x10,0x01,0x00,0x00,0x48}},
        {0x59C660,{0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48}}
    };
    for(const auto& g:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+g.rva),g.bytes,16))return;
    g_ready=true;
}
void OnLivePlayer(const local_players::Context& c,uint32_t generation,bool gameplayAllowed){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>=2||g_dispatching)return;
    auto& p=g_players[unsigned(c.index)];const auto now=GetTickCount64();
    if(p.context.character!=c.character||p.context.world!=c.world||p.context.controller!=c.controller||p.generation!=generation||!Alive(p.actor)){
        Restore(p.saved);p={};p.context=c;p.generation=generation;uintptr_t cls=0,package=0;
        p.strife=c.character&&Capture(c.character,p.actor)&&Read(c.character,0x10,cls)&&Is(cls,"Strife_Blueprint_C")&&
            Read(cls,0x20,package)&&Is(package,"/Game/Blueprints/Strife_Blueprint");
    }
    p.context=c;p.allowed=gameplayAllowed;p.observed=now;
    if(p.await&&now>p.until)p.await=false;
    if(p.saved.ability){unsigned char state=0;
        if(!Alive(p.saved.ability)||!Read(p.saved.ability.address,0xD8,state)||state!=2)Restore(p.saved);
    }
    if(p.gestureAbility&&!Alive(p.gestureAbility))p.gestureAbility={};
    if(!runtime::Get().airDashReworkEnabled.load())p.press={};
    if(p.press.down){
        const bool safe=Gameplay(p);
        if(!safe)p.press={}; // focus/menu loss must not swallow the next real press
        else if(p.press.pending){const auto action=p.press.Update(now,Airborne(p));if(action==Gesture::Forward&&g_dispatch)Send(p,action,now);}
    }
}
bool ProcessAction(Dispatch native,void* iface,unsigned char action){
    // ABILITY_ONE / ABILITY_ONE_RELEASE route to Strife's native PerformDash.
    // No physical key, XInput bit, camera mode or movement-axis test is used.
    if((action!=14&&action!=15)||!g_ready||!native||!native_tps::OnGameThread()||g_dispatching||!dash_combat::TickReady())return false;
    if(!runtime::Get().airDashReworkEnabled.load()){
        for(auto& player:g_players)player.press={};
        return false;
    }
    const auto now=GetTickCount64();
    for(auto& p:g_players){
        if(!p.strife||reinterpret_cast<uintptr_t>(iface)!=p.context.character+0x8D0)continue;
        g_dispatch=native;const bool allowed=now>=p.observed&&now-p.observed<=100&&Gameplay(p)&&Airborne(p);
        const auto result=action==14?p.press.Begin(now,allowed&&Contract(p.context.character)):p.press.Release(now,allowed);
        if(result==Gesture::Down||result==Gesture::Forward){Send(p,result,now);return false;} // original release follows our one native press
        return result==Gesture::Swallow;
    }return false;
}
void BeforeTick(void* liveAbility){
    if(!g_ready||!native_tps::OnGameThread()||(!g_players[0].await&&!g_players[1].await)||!Alive(g_air))return;
    const auto ability=reinterpret_cast<uintptr_t>(liveAbility);uintptr_t cls=0,actor=0,character=0;
    if(!Read(ability,0x10,cls)||cls!=g_air.address||!Read(ability,0x48,actor)||!Read(ability,0x1B0,character)||actor!=character)return;
    const auto now=GetTickCount64();
    for(auto& p:g_players){
        if(!p.await||now>p.until||(p.request!=Gesture::Down&&p.request!=Gesture::Forward)||p.context.character!=actor||p.gestureAbility||!Gameplay(p))continue;
        uintptr_t component=0,owner=0,array=0;int count=-1,capacity=0;bool active=false;unsigned char state=0;float elapsed=0,fall=0,forward=0,timeout=0;
        if(!Read(actor,0xC70,component)||!Read(component,0xE8,owner)||owner!=actor||!Read(component,0xF8,array)||
           !Read(component,0x100,count)||!Read(component,0x104,capacity)||count<=0||count>capacity||capacity>64)continue;
        for(int i=0;i<count;++i){uintptr_t item=0;if(Read(array,size_t(i)*8,item)&&item==ability)active=true;}
        if(!active||!Read(ability,0xD8,state)||state!=2||!Read(ability,0xDC,elapsed)||!std::isfinite(elapsed)||elapsed<0||elapsed>.25f||
           !Read(ability,0x1F8,fall)||fall!=5000||!Read(ability,0x1FC,forward)||forward!=0||!Read(ability,0x44,timeout)||timeout!=3||
           !Writable(ability+0x1F8,8)||!Writable(ability+0x44,4)||!Capture(ability,p.gestureAbility))continue;
        p.await=false;
        if(p.request==Gesture::Down)return; // untouched native descending physics
        p.saved={p.gestureAbility,fall,forward,timeout,elapsed+.24f};
        // Only this live instance: native physics and native TIMEOUT (47)
        // terminate it. No CDO, velocity, movement mode or montage is edited.
        Float(ability,0x1F8,0);Float(ability,0x1FC,3000);Float(ability,0x44,p.saved.oursTimeout);
        if(g_log&&g_events++<8)g_log("Air dash V1.152: held action -> native forward physics, 0.24s native timeout; original parameters restored at finish; EXPERIMENTAL");
        return;
    }
}
void AfterTick(void* ability){
    if(!g_ready||!native_tps::OnGameThread())return;
    for(auto& p:g_players)if(p.saved.ability.address==reinterpret_cast<uintptr_t>(ability)){
        unsigned char state=0;if(!Alive(p.saved.ability)||!Read(p.saved.ability.address,0xD8,state)||state!=2)Restore(p.saved);
    }
}
bool FinishParameters(void* ability,void* function,void* parameters,unsigned char& cause){
    if(!g_ready||!native_tps::OnGameThread()||reinterpret_cast<uintptr_t>(function)!=g_finish.address||!Alive(g_finish))return false;
    for(auto& p:g_players)if(p.gestureAbility.address==reinterpret_cast<uintptr_t>(ability)&&Alive(p.gestureAbility)&&
        local_players::Matches(unsigned(p.context.index),p.context.character,p.generation)){
        unsigned char state=0,original=0;
        if(!Read(p.gestureAbility.address,0xD8,state)||state!=5||
           !Read(reinterpret_cast<uintptr_t>(parameters),0,original))return false;
        p.gestureAbility={};Restore(p.saved);
        if(original!=38)return false;
        // Native cleanup and completion already ran. This one Blueprint
        // branch would add a second ground dash after the chosen gesture.
        // Keep its verified OnFinish call, with a private non-LAND cause.
        cause=0;return true;
    }return false;
}
void Shutdown(){
    g_ready=false;
    if(native_tps::OnGameThread()){
        for(auto& p:g_players)Restore(p.saved);
        g_players={};g_air={};g_finish={};g_source={};g_contract=false;g_dispatch=nullptr;
    } // Process-exit worker never dereferences or edits live gameplay state.
}
bool Ready(){return g_ready.load()&&dash_combat::TickReady();}
}
