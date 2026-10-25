#include "ForeignPowerFeature.h"
#include "ForeignMontage.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::foreign_power {
namespace {
using Name=foreign_wrath::Name;
using Construct=void*(*)(void*,void*,Name,uint32_t,uint32_t,void*,bool,void*,bool);
using Register=void(*)(void*,void*);
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
Construct g_construct=nullptr;Register g_register=nullptr;Finish g_finish=nullptr;Playing g_playing=nullptr;
uintptr_t g_base=0;foreign_wrath::LogFn g_log=nullptr;std::atomic_bool g_ready{false};
std::atomic_uint g_requested{0},g_failed{0};int g_cursor=0,g_count=0;uint64_t g_scanTick=0;unsigned g_events=0;
native_objects::Identity g_power[1]{},g_helper[1]{},g_idle[1]{};
struct OwnedHelpers {native_objects::Identity actor{},component{};};OwnedHelpers g_owned[2]{};
struct Array {uintptr_t data=0;int count=0,capacity=0;};
thread_local void* g_handling=nullptr;
constexpr const char* PowerNames[]={"MayhemAbility_ShadowStrife_Blueprint_C"};
constexpr const char* PowerPackages[]={"/Game/Blueprints/Abilities/ShadowStrife/MayhemAbility_ShadowStrife_Blueprint"};
template<class T> bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
bool Named(uintptr_t p,const char* text){int index=0,number=0;return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(g_base,index,number,text);}
bool Alive(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
unsigned Bit(int choice){return choice==1?1u:0u;}
bool Class(uintptr_t p,const char* name,const char* package,bool native=false){
    uintptr_t cls=0,outer=0;return Read(p,0x10,cls)&&Named(cls,native?"Class":"BlueprintGeneratedClass")&&Named(p,name)&&Read(p,0x20,outer)&&Named(outer,package);
}
int Kind(uintptr_t cls){for(int i=0;i<1;++i)if(cls==g_power[i].address&&Alive(g_power[i])&&Class(cls,PowerNames[i],PowerPackages[i]))return i;return -1;}
bool Owner(void* live,local_players::Context& c,bool war,bool grounded){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0,movement=0,owner=0;unsigned char role=0,type=0,mode=0;
    return native_tps::OnGameThread()&&local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&
        c.kind==local_players::PoseKind::Foot&&c.character==actor&&c.pawn==actor&&Read(actor,0x10,cls)&&Named(cls,war?"War_Blueprint_C":"Strife_Blueprint_C")&&
        Read(actor,0xC18,type)&&type==(war?2:1)&&Read(actor,0x118,role)&&role==3&&Read(actor,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==actor&&
        (!grounded||(Read(actor,0x388,movement)&&Read(movement,0xE8,owner)&&owner==actor&&Read(movement,0x1B0,mode)&&mode==1));
}
bool Source(const local_players::Context& c,bool war,foreign_montage::Source& out){
    const auto& idle=g_idle[0];uintptr_t mesh=0,owner=0,anim=0,cls=0,skeleton=0,asset=0;Array bones{},events{};Name root{};float length=0,rate=0;
    if(!Alive(idle))return false;
    return Read(c.character,0x380,mesh)&&Read(mesh,0xE8,owner)&&owner==c.character&&Read(mesh,0x718,anim)&&Read(anim,0x10,cls)&&Named(cls,war?"WarAnim_Blueprint_C":"StrifeAnim_Blueprint_C")&&
        Read(anim,0x28,skeleton)&&Read(idle.address,0x38,out.skeleton)&&skeleton==out.skeleton&&Read(mesh,0x5A0,asset)&&Read(asset,0x190,bones)&&bones.data&&bones.count>0&&bones.count<=bones.capacity&&bones.capacity<=1000&&
        Read(bones.data,0,root)&&native_objects::NameValueEquals(g_base,root.index,root.number,war?"Bone_War_Root":"Bone_Strife_Root")&&
        Read(idle.address,0x90,length)&&std::isfinite(length)&&length>.1f&&length<10&&Read(idle.address,0x94,rate)&&rate==1&&Read(idle.address,0x80,events)&&!events.count&&
        ((out.sequence=idle.address),(out.rootIndex=root.index),(out.rootNumber=root.number),true);
}
bool ReadyChoice(int choice){return choice==1&&Alive(g_power[0])&&Alive(g_idle[0])&&Alive(g_helper[0]);}
void Scan(){
    for(auto& id:g_power)if(id&&!Alive(id)){id={};g_cursor=0;}
    for(auto& id:g_helper)if(id&&!Alive(id)){id={};g_cursor=0;}
    for(auto& id:g_idle)if(id&&!Alive(id)){id={};g_cursor=0;}
    bool complete=true;for(int choice:{1})if((g_requested&Bit(choice))&&!ReadyChoice(choice))complete=false;if(complete)return;
    uintptr_t chunks=0;int count=0;if(!Read(g_base,0x3959680,chunks)||!Read(g_base,0x3959694,count)||count<=0||count>2000000)return;
    if(count<g_count)g_cursor=0;g_count=count;
    for(unsigned n=0;n<512&&g_cursor<count;++n,++g_cursor){
        uintptr_t chunk=0,p=0,cls=0,outer=0;uint32_t flags=0;
        if(!Read(chunks,size_t(g_cursor/65536)*8,chunk)||!Read(chunk,size_t(g_cursor%65536)*24,p)||!p||!Read(chunk,size_t(g_cursor%65536)*24+8,flags)||(flags&0x30000000u)||!Read(p,0x10,cls))continue;
        if(Named(cls,"BlueprintGeneratedClass")){
            for(int i=0;i<1;++i)if(!g_power[i]&&Class(p,PowerNames[i],PowerPackages[i]))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_power[i]);
        }else if(Named(cls,"Class")){
            if(!g_helper[0]&&Class(p,"ShadowCharacterComponent","/Script/DarksidersGenesis",true))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_helper[0]);
        }else if(Named(cls,"AnimSequence")&&Read(p,0x20,outer)){
            if(!g_idle[0]&&Named(p,"War_Idle")&&Named(outer,"/Game/Characters/Heroes/Hero_War/Animation/War_Idle"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_idle[0]);
        }
    }
}
bool Helper(const local_players::Context& c){
    OwnedHelpers* state=nullptr;
    for(auto& item:g_owned)if(item.actor.address==c.character&&Alive(item.actor)){state=&item;break;}
    if(!state)for(auto& item:g_owned)if(!Alive(item.actor)){item={};state=&item;break;}
    if(!state)return false;
    auto& identity=state->component;uintptr_t owner=0,cls=0,outer=0;unsigned char registered=0;
    if(Alive(identity))return Read(identity.address,0xE8,owner)&&owner==c.character&&Read(identity.address,0xAC,registered)&&(registered&1);
    uintptr_t cdo=0;if(!Alive(g_helper[0])||!Read(g_helper[0].address,0xF8,cdo)||!Read(cdo,0x10,cls)||cls!=g_helper[0].address||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),state->actor))return false;
    const auto component=reinterpret_cast<uintptr_t>(g_construct(reinterpret_cast<void*>(g_helper[0].address),reinterpret_cast<void*>(c.character),Name{0,0},0x40u,0,reinterpret_cast<void*>(cdo),false,nullptr,false));
    uint32_t flags=0;
    if(!Alive(state->actor)||!component||component==cdo||!Read(component,0x10,cls)||cls!=g_helper[0].address||!Read(component,0x20,outer)||outer!=c.character||
       !Read(component,8,flags)||(flags&0x30u)||!(flags&0x40u)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(component),identity))return false;
    g_register(reinterpret_cast<void*>(component),reinterpret_cast<void*>(c.world));
    return Alive(state->actor)&&Alive(identity)&&Read(component,0xE8,owner)&&owner==c.character&&Read(component,0xAC,registered)&&(registered&1);
}
void Stop(void* ability,int choice){
    g_failed.fetch_or(Bit(choice));
    const auto p=reinterpret_cast<uintptr_t>(ability);uintptr_t actor=0,character=0,cls=0;unsigned char state=0;local_players::Context c{};native_objects::Identity live{};
    if(Read(p,0x10,cls)&&Kind(cls)>=0&&Read(p,0x48,actor)&&Read(p,0x1B0,character)&&actor==character&&
       Read(p,0xD8,state)&&(state==1||state==2)&&Owner(reinterpret_cast<void*>(actor),c,true,false)&&native_objects::CaptureLive(g_base,ability,live))g_finish(ability,0);
    if(g_log&&g_events++<16)g_log("Foreign powers V1.97: exact owned cast contract unavailable; native finish and this transfer stopped for the session; select Native");
}
}
void Initialize(foreign_wrath::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char ctor[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x40};
    constexpr unsigned char reg[]={0x40,0x56,0x57,0x41,0x56,0x48,0x83,0xEC,0x50,0x48,0xC7,0x44,0x24,0x30,0xFE,0xFF};
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0xAC57A0),ctor,sizeof(ctor))||std::memcmp(reinterpret_cast<void*>(g_base+0x1750880),reg,sizeof(reg))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))||std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing)))return;
    g_construct=reinterpret_cast<Construct>(g_base+0xAC57A0);g_register=reinterpret_cast<Register>(g_base+0x1750880);g_finish=reinterpret_cast<Finish>(g_base+0x59B080);g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_ready=true;
    if(log)log("Foreign powers V1.97 READY: Clone on War only; rejected World Ender/Rampage adapters removed; compatible idle pose, native callbacks");
}
int Status(int choice){const unsigned bit=Bit(choice);if(!bit||!g_ready||!foreign_montage::Ready()||(g_failed&bit))return 0;g_requested.fetch_or(bit);return ReadyChoice(choice)?2:1;}
void Observe(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||!c.localPlayer||!c.world||c.kind!=local_players::PoseKind::Foot)return;
    for(const auto& choices:runtime::Get().wrathSlots)for(const auto& choice:choices)if(Bit(choice.load()))g_requested.fetch_or(Bit(choice.load()));
    if(!g_requested)return;const auto now=GetTickCount64();if(now>=g_scanTick&&now-g_scanTick<15)return;g_scanTick=now;Scan();
}
bool IsClass(uintptr_t p){return g_ready&&native_tps::OnGameThread()&&Kind(p)==0;}
bool Prepare(void* actor,uintptr_t& abilityClass,int choice){
    if(Status(choice)!=2||!native_tps::OnGameThread())return false;local_players::Context c{};foreign_montage::Source source{};
    if(!Owner(actor,c,true,true)||!Source(c,true,source))return false;
    abilityClass=g_power[0].address;return true;
}
bool HandlePlay(PlayFn native,void* ability,void* montage,float rate,Name section){
    if(!g_ready||!native||!native_tps::OnGameThread())return false;
    const auto p=reinterpret_cast<uintptr_t>(ability);uintptr_t cls=0,actor=0,character=0;unsigned char state=0;uint32_t flags=0;
    if(!Read(p,0x10,cls)||!Read(p,8,flags)||(flags&0x30u)||!Read(p,0x48,actor)||!Read(p,0x1B0,character)||actor!=character||!Read(p,0xD8,state)||(state!=1&&state!=2))return false;
    const int kind=Kind(cls);local_players::Context c{};
    if(kind!=0||!Owner(reinterpret_cast<void*>(actor),c,true,false))return false;
    constexpr int choice=1;constexpr bool war=true;
    if(g_handling==ability)return true;
    struct Guard{void* old;~Guard(){g_handling=old;}} guard{g_handling};g_handling=ability;
    native_objects::Identity abi{},pawn{};foreign_montage::Source source{};
    if(!native_objects::CaptureLive(g_base,ability,abi)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),pawn)){g_failed.fetch_or(Bit(choice));return true;}
    if((g_failed&Bit(choice))||!std::isfinite(rate)||rate<=0||rate>10||section.index||section.number||!Source(c,war,source)){Stop(ability,choice);return true;}
    uintptr_t tracks=0;Name slot{};
    if(!Read(reinterpret_cast<uintptr_t>(montage),0x158,tracks)||!Read(tracks,0,slot)){Stop(ability,choice);return true;}
    source.slotIndex=slot.index;source.slotNumber=slot.number;
    if(!Helper(c)){if(Alive(abi)&&Alive(pawn))Stop(ability,choice);return true;}
    if(!Alive(abi)||!Alive(pawn))return true;
    uintptr_t ownerAfter=0;if(!Read(p,0x48,ownerAfter)||ownerAfter!=actor)return true;
    auto* replacement=foreign_montage::Compatible(reinterpret_cast<uintptr_t>(montage),source);
    if(!Alive(abi)||!Alive(pawn))return true;
    if(!Read(p,0x48,ownerAfter)||ownerAfter!=actor)return true;
    if(!replacement){Stop(ability,choice);return true;}
    native(ability,replacement,rate,section);
    if(!Alive(abi)||!Alive(pawn))return true;
    if(!Read(p,0x48,ownerAfter)||ownerAfter!=actor)return true;
    uintptr_t instance=0;if(!Read(p,0x1D8,instance)||!instance||!g_playing(reinterpret_cast<void*>(instance))){Stop(ability,choice);return true;}
    foreign_montage::StartSounds(reinterpret_cast<uintptr_t>(montage),ability);
    if(g_log&&g_events++<16){char line[180]{};sprintf_s(line,"Foreign powers V1.98 CAST choice=%d phase=%d player=%d; own gesture when loaded, idle fallback; native callbacks, animation EXPERIMENTAL",choice,kind,c.index+1);g_log(line);}
    return true;
}
void Shutdown(){g_ready=false;g_requested=0;for(auto& item:g_owned)item={};}
}
