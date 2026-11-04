#include "RangedStateFeature.h"
#include "RangedStatePolicy.h"
#include "LocalPlayerOwnership.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "RangedScriptContract.h"
#include <array>
#include <atomic>
#include <cmath>

namespace dg::ranged_state {
namespace {
using Id=native_objects::Identity;
using Query=unsigned char(*)(void*,unsigned char);
using Event=void(*)(void*,void*,void*,void*);
uintptr_t g_base=0;LogFn g_log=nullptr;Query g_query=nullptr;Event g_event=nullptr;
std::atomic_bool g_ready{false};bool g_servicing=false;unsigned g_requests=0;
Id g_class{},g_setter{};int g_mode=-1;
struct Player {Id actor{};uintptr_t local=0,controller=0,world=0;uint32_t generation=0;Policy policy{};};
std::array<Player,2> g_players{};
template<class T>bool Read(uintptr_t p,size_t at,T& out){native_objects::MemoryReader r;return r.Read(p,at,out);}
template<size_t N>bool Is(uintptr_t p,const char(&name)[N]){return native_objects::NameIs(g_base,p,name);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
bool Script(uintptr_t function){
    using namespace ranged_script;
    struct Array {uintptr_t data=0;int count=0,capacity=0;} code;
    std::array<unsigned char,ReleasedAimSize> bytes{};
    if(!Read(function,0x58,code)||code.count!=int(ReleasedAimSize)||code.capacity<code.count||code.capacity>65536||!Read(code.data,0,bytes))return false;
    return Matches(bytes,ReleasedAimSize,ReleasedAimHash,ReleasedAimBlocks,[&](const auto& b){
        uintptr_t property=0;int name=-1,number=-1;
        return b.width==8&&Read(reinterpret_cast<uintptr_t>(bytes.data()),b.offset,property)&&
            Read(property,0x18,name)&&Read(property,0x1C,number)&&native_objects::NameValueIdentifierEquals(g_base,name,number,b.name);
    });
}
bool Contract(uintptr_t cls){
    if(Alive(g_class)&&g_class.address==cls)return g_mode>=0&&Alive(g_setter);
    g_class={};g_setter={};g_mode=-1;
    if(!Capture(cls,g_class))return false;
    uintptr_t field=0,function=0;if(!Read(cls,0x48,field))return false;
    for(unsigned n=0;field&&n<1024;++n){
        int name=-1,number=-1;
        if(!Read(field,0x18,name)||!Read(field,0x1C,number))return false;
        if(native_objects::NameValueIdentifierEquals(g_base,name,number,"SetAimMode")){if(function)return false;function=field;}
        uintptr_t next=0;if(!Read(field,0x28,next)||next==field)return false;field=next;
    }
    int at=-1,param=-1,propertyOffset=-1;uintptr_t outer=0,code=0,property=0,other=0,parameter=0,next=0,type=0;
    const bool valid=!field&&function&&Read(function,0x10,type)&&Is(type,"Function")&&Read(function,0x20,outer)&&outer==cls&&
        native_objects::PropertyField(g_base,cls,"AimMode","ByteProperty",1,at)&&
        native_objects::PropertyField(g_base,function,"Mode","ByteProperty",1,param)&&param==0&&Script(function)&&
        Read(function,0x58,code)&&Read(code,1,property)&&Read(code,10,other)&&property==other&&
        Read(property,0x20,outer)&&outer==cls&&Read(property,0x44,propertyOffset)&&propertyOffset==at&&
        Read(code,19,parameter)&&Read(parameter,0x20,outer)&&outer==function&&
        Read(function,0x48,other)&&other==parameter&&Read(parameter,0x28,next)&&!next&&Capture(function,g_setter);
    if(valid)g_mode=at;
    if(g_log)g_log(valid?"Ranged state V1.152: complete native setter verified, identifier case resolved; interruption source independent":
        "Ranged state V1.152: setter contract unavailable, native retained without per-frame retry");
    return valid;
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const unsigned char query[]={0x48,0x81,0xC1,0x40,0x05,0x00,0x00,0xE9,0xC4,0xB3,0xFC,0xFF};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x669D30),query,sizeof(query)))return;
    g_query=reinterpret_cast<Query>(g_base+0x669D30);g_event=reinterpret_cast<Event>(g_base+0xAC0630);g_ready=true;
}
void OnLivePlayer(const local_players::Context& c,uint32_t generation,bool gameplayAllowed){
    if(!g_ready.load()||!native_tps::OnGameThread()||g_servicing||c.index<0||c.index>=2)return;
    auto& state=g_players[unsigned(c.index)];
    if(c.kind!=local_players::PoseKind::Foot||c.character!=c.pawn||!c.character||!c.world||!c.localPlayer||!c.controller){state={};return;}
    uintptr_t cls=0,package=0,controller=0,pawn=0,resource=0;unsigned char role=0;float health=0;
    if(!Read(c.character,0x10,cls)||!Is(cls,"Strife_Blueprint_C")||!Read(cls,0x20,package)||!Is(package,"/Game/Blueprints/Strife_Blueprint")||
       !Read(c.character,0x348,controller)||controller!=c.controller||!Read(controller,0x350,pawn)||pawn!=c.character||
       !Read(c.character,0x118,role)||role!=3||!Read(c.character,0xEA0,resource)||!Read(resource,0x37C,health)||!std::isfinite(health)||health<=0){state={};return;}
    if(state.actor.address!=c.character||state.local!=c.localPlayer||state.controller!=c.controller||state.world!=c.world||state.generation!=generation||!Alive(state.actor)){
        state={};if(!Capture(c.character,state.actor))return;
        state.local=c.localPlayer;state.controller=c.controller;state.world=c.world;state.generation=generation;
    }
    if(!Contract(cls))return;
    unsigned char mode=0;if(!Read(c.character,size_t(g_mode),mode))return;
    if(mode!=1){state.policy={};return;}
    uintptr_t table=0,query=0,component=0,owner=0,movement=0;int count=-1,capacity=0,lockIndex=0;unsigned char move=0;
    if(!Read(c.character,0x8D0,table)||!Read(table,0x18,query)||query!=reinterpret_cast<uintptr_t>(g_query)||
       !Read(c.character,0xC70,component)||!Read(component,0xE8,owner)||owner!=c.character||
       !Read(component,0x100,count)||!Read(component,0x104,capacity)||count<0||count>capacity||capacity>64||
       !Read(c.character,0x388,movement)||!Read(movement,0x1B0,move)||!Read(c.character,0xE90,lockIndex))return;
    void* iface=reinterpret_cast<void*>(c.character+0x8D0);
    const bool held=g_query(iface,31)||g_query(iface,18)||g_query(iface,20);
    const bool allowed=gameplayAllowed&&(move==1||move==2)&&lockIndex==-1&&
        !native_tps::IsWorldPaused(reinterpret_cast<void*>(c.character))&&!native_tps::CinematicActive(unsigned(c.index))&&!native_tps::RadialActive(unsigned(c.index));
    if(!state.policy.Recover(mode,held,count!=0,allowed))return;
    struct Scope {Scope(){g_servicing=true;}~Scope(){g_servicing=false;}} scope;
    unsigned char released=0;
    g_event(reinterpret_cast<void*>(c.character),reinterpret_cast<void*>(g_setter.address),&released,nullptr);
    if(g_log&&++g_requests<=12)g_log("Ranged state V1.152: orphan AIM mode after interruption; one native release, no ability/input/montage change; EXPERIMENTAL");
}
void Shutdown(){g_ready=false;g_players={};g_class={};g_setter={};g_mode=-1;g_query=nullptr;g_event=nullptr;}
}
