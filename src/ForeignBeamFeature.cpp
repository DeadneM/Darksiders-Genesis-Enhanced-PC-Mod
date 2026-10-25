#include "ForeignBeamFeature.h"
#include "ForeignMontage.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::foreign_beam {
namespace {
using Name=foreign_wrath::Name;
using Construct=void*(*)(void*,void*,Name,uint32_t,uint32_t,void*,bool,void*,bool);
using Register=void(*)(void*,void*);
// SceneComponent K2_AttachToComponent (1DD86B0), not Actor's similarly named wrapper.
using Attach=bool(*)(void*,void*,Name,unsigned char,unsigned char,unsigned char,bool);
using Destroy=void(*)(void*,void*);
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
Construct g_construct=nullptr;Register g_register=nullptr;Attach g_attach=nullptr;Destroy g_destroy=nullptr;
Finish g_finish=nullptr;Playing g_playing=nullptr;
uintptr_t g_base=0;foreign_wrath::LogFn g_log=nullptr;unsigned g_events=0;
std::atomic_bool g_ready{false},g_failed{false};
native_objects::Identity g_power{},g_component{},g_idle{};
struct Owned {native_objects::Identity actor{},component{};};Owned g_owned[2]{};
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Source {foreign_montage::Source animation{};uintptr_t mesh=0,root=0;Name meshName{0,0},socket{0,0};};
thread_local void* g_handling=nullptr;
template<class T> bool Read(uintptr_t p,size_t at,T& value){native_objects::MemoryReader r;return r.Read(p,at,value);}
template<size_t N> bool Is(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
bool Alive(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
template<class T> bool Store(uintptr_t p,size_t at,const T& value){
    MEMORY_BASIC_INFORMATION m{};if(!p||p>UINTPTR_MAX-at)return false;
    const auto address=p+at;
    if(!VirtualQuery(reinterpret_cast<void*>(address),&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_GUARD|PAGE_NOACCESS))||((m.Protect&255)!=PAGE_READWRITE&&(m.Protect&255)!=PAGE_EXECUTE_READWRITE))return false;
    const auto offset=address-reinterpret_cast<uintptr_t>(m.BaseAddress);
    if(offset>m.RegionSize||sizeof(value)>m.RegionSize-offset)return false;
    std::memcpy(reinterpret_cast<void*>(address),&value,sizeof(value));return true;
}
bool Class(uintptr_t p,bool component){
    uintptr_t cls=0,package=0;
    return Read(p,0x10,cls)&&Is(cls,"BlueprintGeneratedClass")&&Read(p,0x20,package)&&
        (component?(Is(p,"MayhemWorldEnder_Blueprint_C")&&Is(package,"/Game/Blueprints/Player/MayhemWorldEnder_Blueprint")):
        (Is(p,"MayhemAbility_WorldEnder_Finish_Blueprint_C")&&Is(package,"/Game/Blueprints/Abilities/WorldEnder/MayhemAbility_WorldEnder_Finish_Blueprint")));
}
bool Owner(void* live,local_players::Context& c,bool grounded){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0,movement=0,owner=0;unsigned char role=0,type=0,mode=0;
    return local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&c.kind==local_players::PoseKind::Foot&&
        c.pawn==actor&&c.character==actor&&Read(actor,0x10,cls)&&Is(cls,"War_Blueprint_C")&&Read(actor,0xC18,type)&&type==2&&
        Read(actor,0x118,role)&&role==3&&Read(actor,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==actor&&
        (!grounded||(Read(actor,0x388,movement)&&Read(movement,0xE8,owner)&&owner==actor&&Read(movement,0x1B0,mode)&&mode==1));
}
bool Socket(uintptr_t object,Name& result){
    // Native SkeletalMesh/Skeleton Sockets property, followed through bounded reflection.
    uintptr_t cls=0;if(!Read(object,0x10,cls))return false;
    for(int depth=0;cls&&depth<16;++depth){
        uintptr_t field=0;if(!Read(cls,0x48,field))return false;
        for(int n=0;field&&n<128;++n){
            uintptr_t kind=0;int size=0,offset=0;
            if(Is(field,"Sockets")&&Read(field,0x10,kind)&&Is(kind,"ArrayProperty")&&Read(field,0x34,size)&&size==16&&
               Read(field,0x44,offset)&&offset>=0x28&&offset<0x1000){
                Array sockets{};if(!Read(object,size_t(offset),sockets)||sockets.count<0||sockets.count>sockets.capacity||sockets.capacity>512||(!sockets.data&&sockets.count))return false;
                for(int i=0;i<sockets.count;++i){
                    uintptr_t socket=0;Name name{0,0},bone{0,0};
                    if(Read(sockets.data,size_t(i)*8,socket)&&Read(socket,0x10,kind)&&Is(kind,"SkeletalMeshSocket")&&
                       Read(socket,0x28,name)&&native_objects::NameValueIs(g_base,name.index,name.number,"VFX_Aura")&&
                       Read(socket,0x30,bone)&&native_objects::NameValueIs(g_base,bone.index,bone.number,"Bone_War_Root")){result=name;return true;}
                }
            }
            if(!Read(field,0x28,field))return false;
        }
        if(!Read(cls,0x40,cls))return false;
    }
    return false;
}
bool Animation(const local_players::Context& c,Source& out){
    uintptr_t cls=0,anim=0,asset=0,skeleton=0,owner=0;Array bones{},events{};Name root{0,0};float length=0,rate=0;
    if(!Alive(g_idle)||!Read(c.character,0x380,out.mesh)||!Is(out.mesh,"CharacterMesh0")||!Read(out.mesh,0x18,out.meshName)||
       !Read(out.mesh,0xE8,owner)||owner!=c.character||!Read(c.character,0x158,out.root)||!Read(out.root,0xE8,owner)||owner!=c.character||
       !Read(out.mesh,0x718,anim)||!Read(anim,0x10,cls)||!Is(cls,"WarAnim_Blueprint_C")||!Read(anim,0x28,skeleton)||
       !Read(g_idle.address,0x38,out.animation.skeleton)||skeleton!=out.animation.skeleton||!Is(skeleton,"War_Mesh_Skeleton")||
       !Read(out.mesh,0x5A0,asset)||!Read(asset,0x190,bones)||!bones.data||bones.count<=0||bones.count>bones.capacity||bones.capacity>512||
       !Read(bones.data,0,root)||!native_objects::NameValueIs(g_base,root.index,root.number,"Bone_War_Root")||
       !Read(g_idle.address,0x90,length)||!std::isfinite(length)||length<=.1f||length>=10||!Read(g_idle.address,0x94,rate)||rate!=1||
       !Read(g_idle.address,0x80,events)||events.count||(!Socket(asset,out.socket)&&!Socket(skeleton,out.socket)))return false;
    out.animation.sequence=g_idle.address;out.animation.rootIndex=root.index;out.animation.rootNumber=root.number;return true;
}
bool Attached(const Owned& item,const local_players::Context& c,const Source& source){
    uintptr_t owner=0,parent=0;unsigned char registered=0;Name socket{0,0},meshName{0,0};
    return Alive(item.actor)&&item.actor.address==c.character&&Alive(item.component)&&
        Read(item.component.address,0xE8,owner)&&owner==c.character&&Read(item.component.address,0xAC,registered)&&(registered&1)&&
        Read(item.component.address,0x108,parent)&&parent==source.root&&Read(item.component.address,0x698,meshName)&&meshName==source.meshName&&
        Read(item.component.address,0x690,socket)&&socket==source.socket;
}
void Discard(Owned& item){
    uintptr_t owner=0;
    if(Alive(item.actor)&&Alive(item.component)&&Read(item.component.address,0xE8,owner)&&owner==item.actor.address)
        g_destroy(reinterpret_cast<void*>(owner),reinterpret_cast<void*>(item.component.address));
    item={};
}
bool Helper(const local_players::Context& c,const Source& source){
    Owned* state=nullptr;
    for(auto& item:g_owned)if(item.actor.address==c.character&&Alive(item.actor)){state=&item;break;}
    if(!state)for(auto& item:g_owned)if(!Alive(item.actor)){item={};state=&item;break;}
    if(!state)return false;
    if(Alive(state->component))return Attached(*state,c,source);
    uintptr_t cdo=0,cls=0,outer=0;uint32_t flags=0;
    if(!Alive(g_component)||!Read(g_component.address,0xF8,cdo)||!Read(cdo,0x10,cls)||cls!=g_component.address||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),state->actor))return false;
    const auto component=reinterpret_cast<uintptr_t>(g_construct(reinterpret_cast<void*>(cls),reinterpret_cast<void*>(c.character),Name{0,0},0x40u,0,reinterpret_cast<void*>(cdo),false,nullptr,false));
    if(!Alive(state->actor)||!component||component==cdo||!Read(component,0x10,cls)||cls!=g_component.address||
       !Read(component,0x20,outer)||outer!=c.character||!Read(component,8,flags)||(flags&0x30u)||!(flags&0x40u)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(component),state->component))return false;
    if(!Store(component,0x698,source.meshName)||!Store(component,0x690,source.socket)){Discard(*state);return false;}
    g_register(reinterpret_cast<void*>(component),reinterpret_cast<void*>(c.world));
    if(!Alive(state->actor)||!Alive(state->component))return false;
    uintptr_t owner=0,root=0,mesh=0;unsigned char registered=0;
    if(!Read(component,0xE8,owner)||owner!=c.character||!Read(component,0xAC,registered)||!(registered&1)||
       !Read(c.character,0x158,root)||root!=source.root||!Read(c.character,0x380,mesh)||mesh!=source.mesh){Discard(*state);return false;}
    // Snap all three rules to this War's capsule; native beam source is its own mesh/socket.
    if(!g_attach(reinterpret_cast<void*>(component),reinterpret_cast<void*>(source.root),Name{0,0},2,2,2,false)||!Attached(*state,c,source)){Discard(*state);return false;}
    return true;
}
void Stop(void* ability){
    g_failed=true;local_players::Context c{};uintptr_t actor=0,character=0,cls=0;unsigned char state=0;native_objects::Identity id{};
    const auto p=reinterpret_cast<uintptr_t>(ability);
    if(Read(p,0x10,cls)&&IsClass(cls)&&Read(p,0x48,actor)&&Read(p,0x1B0,character)&&character==actor&&
       Read(p,0xD8,state)&&(state==1||state==2)&&Owner(reinterpret_cast<void*>(actor),c,false)&&
       native_objects::CaptureLive(g_base,ability,id))g_finish(ability,0);
    if(g_log&&g_events++<12)g_log("World Ender V1.99 stopped: owned War contract unavailable; native finish, transfer disabled for session; select Native");
}
}
void Initialize(foreign_wrath::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char ctor[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x40};
    constexpr unsigned char reg[]={0x40,0x56,0x57,0x41,0x56,0x48,0x83,0xEC,0x50,0x48,0xC7,0x44,0x24,0x30,0xFE,0xFF};
    constexpr unsigned char attach[]={0x48,0x83,0xEC,0x28,0x0F,0xB6,0x44,0x24,0x50,0x88,0x44,0x24,0x49,0x0F,0xB6,0x44};
    constexpr unsigned char destroy[]={0x4C,0x8B,0xC2,0x48,0x85,0xD2,0x74,0x18,0x48,0x39,0x8A,0xE8,0x00,0x00,0x00,0x75};
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0xAC57A0),ctor,sizeof(ctor))||std::memcmp(reinterpret_cast<void*>(g_base+0x1750880),reg,sizeof(reg))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x17A7310),attach,sizeof(attach))||std::memcmp(reinterpret_cast<void*>(g_base+0x163EFA0),destroy,sizeof(destroy))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))||std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing)))return;
    g_construct=reinterpret_cast<Construct>(g_base+0xAC57A0);g_register=reinterpret_cast<Register>(g_base+0x1750880);
    g_attach=reinterpret_cast<Attach>(g_base+0x17A7310);g_destroy=reinterpret_cast<Destroy>(g_base+0x163EFA0);
    g_finish=reinterpret_cast<Finish>(g_base+0x59B080);g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_ready=true;
    if(log)log("World Ender V1.99 READY: War-owned scene component and authored VFX_Aura source; native beam lifecycle; EXPERIMENTAL");
}
bool AssetsReady(){return Alive(g_power)&&Alive(g_component)&&Alive(g_idle);}
bool InvalidateAssets(){
    if(!g_ready||!native_tps::OnGameThread())return false;
    bool changed=false;
    for(auto* id:{&g_power,&g_component,&g_idle})if(*id&&!Alive(*id)){*id={};changed=true;}
    return changed;
}
void ObserveObject(uintptr_t p){
    if(!g_ready||!native_tps::OnGameThread())return;
    if(!Alive(g_power)&&Class(p,false))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_power);
    if(!Alive(g_component)&&Class(p,true))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_component);
    uintptr_t cls=0,package=0;
    if(!Alive(g_idle)&&Read(p,0x10,cls)&&Is(cls,"AnimSequence")&&Is(p,"War_Idle")&&Read(p,0x20,package)&&
       Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Idle"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_idle);
}
int Status(){return !g_ready||g_failed||!foreign_montage::Ready()?0:(AssetsReady()?2:1);}
bool IsClass(uintptr_t cls){return g_ready&&native_tps::OnGameThread()&&cls==g_power.address&&Alive(g_power)&&Class(cls,false);}
bool Prepare(void* actor,uintptr_t& abilityClass){
    local_players::Context c{};Source source{};
    if(Status()!=2||!native_tps::OnGameThread()||!Owner(actor,c,true)||!Animation(c,source))return false;
    abilityClass=g_power.address;return true;
}
bool HandlePlay(PlayFn native,void* ability,void* montage,float rate,Name section){
    if(!g_ready||!native||!native_tps::OnGameThread())return false;
    const auto p=reinterpret_cast<uintptr_t>(ability);uintptr_t cls=0,actor=0,character=0;uint32_t flags=0;unsigned char state=0;local_players::Context c{};
    if(!Read(p,0x10,cls)||!IsClass(cls)||!Read(p,8,flags)||(flags&0x30u)||!Read(p,0x48,actor)||!Read(p,0x1B0,character)||character!=actor||
       !Read(p,0xD8,state)||(state!=1&&state!=2)||!Owner(reinterpret_cast<void*>(actor),c,false))return false;
    if(g_handling==ability)return true;
    struct Guard{void* old;~Guard(){g_handling=old;}} guard{g_handling};g_handling=ability;
    native_objects::Identity abi{},pawn{};Source source{};uintptr_t tracks=0;Name slot{0,0},meshName{0,0},socket{0,0};
    if(!native_objects::CaptureLive(g_base,ability,abi)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),pawn)){g_failed=true;return true;}
    if(g_failed||!std::isfinite(rate)||rate<=0||rate>10||section.index||section.number||!Animation(c,source)||
       !Read(reinterpret_cast<uintptr_t>(montage),0x158,tracks)||!Read(tracks,0,slot)||
       !Read(p,0x1F8,meshName)||!(meshName==Name{0,0})||!Read(p,0x200,socket)||!(socket==Name{0,0})){Stop(ability);return true;}
    source.animation.slotIndex=slot.index;source.animation.slotNumber=slot.number;
    // Build first: a rejected montage cannot leave a newly registered helper behind.
    auto* replacement=foreign_montage::WorldEnder(reinterpret_cast<uintptr_t>(montage),source.animation);
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    if(!replacement||!Helper(c,source)){if(Alive(abi)&&Alive(pawn))Stop(ability);return true;}
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    // Native None source names select the private component's defaults.
    // No ability fields, shared defaults or Strife component are rewritten.
    native(ability,replacement,rate,section);
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    uintptr_t instance=0;if(!Read(p,0x1D8,instance)||!instance||!g_playing(reinterpret_cast<void*>(instance))){Stop(ability);return true;}
    if(g_log&&g_events++<12){char text[180]{};sprintf_s(text,"World Ender V1.99 CAST player=%d: owned War mesh/VFX_Aura, compatible gesture; native StartBeam/DestroyBeam; EXPERIMENTAL",c.index+1);g_log(text);}
    return true;
}
void Shutdown(){g_ready=false;for(auto& item:g_owned)item={};}
}
