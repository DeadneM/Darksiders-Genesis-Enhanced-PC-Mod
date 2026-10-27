#include "ForeignCaltropsFeature.h"
#include "ForeignMontage.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include <atomic>
#include <cmath>

namespace dg::foreign_caltrops {
namespace {
using Create=void*(*)(void*,Name,float,float,float,int,float,float);
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
// 7AF460 decodes a subclass and an eight-byte FName, then calls 5AE2B0.
// That native helper uses this ability's Character, authority and spawn path.
using Spawn=void(*)(void*,void*,Name);
Create g_create=nullptr;Finish g_finish=nullptr;Playing g_playing=nullptr;Spawn g_spawn=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false},g_failed{false};
native_objects::Identity g_power{},g_idle{};unsigned g_events=0;
thread_local void* g_handling=nullptr;
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Source {uintptr_t mesh=0,sequence=0,skeleton=0;Name pelvis{};float length=0;};
template<class T> bool Read(uintptr_t p,size_t o,T& value){native_objects::MemoryReader r;return r.Read(p,o,value);}
template<size_t N> bool Is(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
bool Alive(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Class(uintptr_t p){
    uintptr_t cls=0,package=0;
    return Read(p,0x10,cls)&&Is(cls,"BlueprintGeneratedClass")&&Is(p,"MayhemAbility_SpawnCaltrops_Blueprint_C")&&
        Read(p,0x20,package)&&Is(package,"/Game/Blueprints/Abilities/Caltrops/MayhemAbility_SpawnCaltrops_Blueprint");
}
bool Idle(uintptr_t p){
    uintptr_t cls=0,package=0;float length=0,rate=0;Array events{};
    return Read(p,0x10,cls)&&Is(cls,"AnimSequence")&&Is(p,"War_Idle")&&Read(p,0x20,package)&&
        Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Idle")&&Read(p,0x90,length)&&
        std::isfinite(length)&&std::fabs(length-5.3333334923f)<.001f&&Read(p,0x94,rate)&&rate==1&&
        Read(p,0x80,events)&&events.count==0&&events.capacity>=0&&events.capacity<=128;
}
bool Owner(void* live,local_players::Context& c){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0,movement=0,owner=0;unsigned char type=0,role=0,mode=0;
    return local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&
        c.kind==local_players::PoseKind::Foot&&c.pawn==actor&&c.character==actor&&
        Read(actor,0x10,cls)&&Is(cls,"War_Blueprint_C")&&Read(actor,0xC18,type)&&type==2&&Read(actor,0x118,role)&&role==3&&
        Read(actor,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==actor&&
        Read(actor,0x388,movement)&&Read(movement,0xE8,owner)&&owner==actor&&Read(movement,0x1B0,mode)&&mode==1;
}
bool Animation(const local_players::Context& c,Source& out){
    uintptr_t anim=0,cls=0,owner=0,asset=0,skeleton=0;Array bones{};struct Bone {Name name;int parent=0;} pelvis{};
    if(!Alive(g_idle)||!Idle(g_idle.address))return false;
    out.sequence=g_idle.address;
    return Read(c.character,0x380,out.mesh)&&Read(out.mesh,0xE8,owner)&&owner==c.character&&
        Read(out.mesh,0x718,anim)&&Read(anim,0x10,cls)&&Is(cls,"WarAnim_Blueprint_C")&&
        Read(anim,0x28,skeleton)&&Read(out.sequence,0x38,out.skeleton)&&skeleton==out.skeleton&&Is(skeleton,"War_Mesh_Skeleton")&&
        Read(out.mesh,0x5A0,asset)&&Read(asset,0x190,bones)&&bones.data&&bones.count>1&&bones.count<=bones.capacity&&bones.capacity<=512&&
        Read(bones.data,12,pelvis)&&pelvis.parent==0&&native_objects::NameValueIs(g_base,pelvis.name.index,pelvis.name.number,"Bone_War_Pelvis")&&
        Read(out.sequence,0x90,out.length)&&((out.pelvis=pelvis.name),true);
}
bool Projectile(uintptr_t ability,uintptr_t& cls){
    uintptr_t kind=0,package=0;
    return Read(ability,0x1E0,cls)&&Read(cls,0x10,kind)&&Is(kind,"BlueprintGeneratedClass")&&
        Is(cls,"MayhemCaltropSpawnerProjectile_Base_Blueprint_C")&&Read(cls,0x20,package)&&
        Is(package,"/Game/Blueprints/Projectiles/MayhemCaltropSpawnerProjectile_Base_Blueprint");
}
bool Montage(void* live,Name& slot,float& duration){
    const auto p=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,package=0,skeleton=0;float length=0,rate=0;Array tracks{};
    return Read(p,0x10,cls)&&Is(cls,"AnimMontage")&&Read(p,0x20,package)&&
        ((Is(p,"Strife_Caltrops_Ground_F_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Caltrops_Ground_F_Montage"))||
         (Is(p,"Strife_Caltrops_Ground_B_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Caltrops_Ground_B_Montage")))&&
        Read(p,0x38,skeleton)&&Is(skeleton,"Strife_Skeleton")&&Read(p,0x90,length)&&std::isfinite(length)&&std::fabs(length-2.3333332539f)<.001f&&
        Read(p,0x94,rate)&&rate==1&&Read(p,0x158,tracks)&&tracks.data&&tracks.count==1&&tracks.capacity>=1&&tracks.capacity<=4&&
        Read(tracks.data,0,slot)&&native_objects::NameValueIs(g_base,slot.index,slot.number,"TPS")&&((duration=length),true);
}
void Stop(void* ability){
    g_failed=true;g_finish(ability,0);
    if(g_log&&g_events++<12)g_log("Caltrops V1.93 stopped: owned War cast contract unavailable; native finish requested, select Native; no repeated constructor attempt");
}
}
void Initialize(LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char spawn[]={0x48,0x89,0x5C,0x24,0x18,0x48,0x89,0x74,0x24,0x20,0x48,0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xEC,0x30,0x48,0x8B,0x81,0xB0};
    constexpr unsigned char create[]={0x48,0x8B,0xC4,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D};
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x5AE2B0),spawn,sizeof(spawn))||std::memcmp(reinterpret_cast<void*>(g_base+0x168A410),create,sizeof(create))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))||std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing)))return;
    g_create=reinterpret_cast<Create>(g_base+0x168A410);g_finish=reinterpret_cast<Finish>(g_base+0x59B080);
    g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_spawn=reinterpret_cast<Spawn>(g_base+0x5AE2B0);g_ready=true;
    if(log)log("Caltrops V1.93 READY: War ground-only adapter, own pelvis and native projectile factory; no new hook or timer; EXPERIMENTAL UNTESTED IN GAME");
}
int Status(){return !g_ready||g_failed?0:(AssetsReady()?2:1);}
bool IsClass(uintptr_t p){return g_ready&&native_tps::OnGameThread()&&p==g_power.address&&Alive(g_power)&&Class(p);}
bool AssetsReady(){return Alive(g_power)&&Alive(g_idle);}
bool InvalidateAssets(){
    if(!g_ready||!native_tps::OnGameThread())return false;bool changed=false;
    for(auto* id:{&g_power,&g_idle})if(*id&&!Alive(*id)){*id={};changed=true;}return changed;
}
void ObserveObject(uintptr_t object,unsigned kind){
    if(!g_ready||!native_tps::OnGameThread())return;
    if(kind==1&&!g_power&&Class(object))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_power);
    if(kind==3&&!g_idle&&Idle(object))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_idle);
}
bool Prepare(void* actor,uintptr_t& abilityClass){
    if(!g_ready||g_failed||!native_tps::OnGameThread()||!IsClass(g_power.address))return false;
    local_players::Context c{};Source source{};uintptr_t cdo=0,cls=0,projectile=0;
    if(!Owner(actor,c)||!Animation(c,source)||!Read(g_power.address,0xF8,cdo)||!Read(cdo,0x10,cls)||cls!=g_power.address||!Projectile(cdo,projectile))return false;
    abilityClass=g_power.address;return true;
}
bool HandlePlay(PlayFn native,void* ability,void* montage,float rate,Name section){
    if(!g_ready||!native||!native_tps::OnGameThread())return false;
    const auto p=reinterpret_cast<uintptr_t>(ability);uintptr_t cls=0,actor=0,character=0;uint32_t flags=0;unsigned char state=0;
    if(!Read(p,0x10,cls)||!IsClass(cls)||!Read(p,8,flags)||(flags&0x30u)||!Read(p,0x48,actor)||!Read(p,0x1B0,character)||actor!=character||
       !Read(p,0xD8,state)||(state!=1&&state!=2))return false;
    local_players::Context c{};if(!Owner(reinterpret_cast<void*>(actor),c))return false;
    if(g_handling==ability)return true;
    struct Scope {void* previous;explicit Scope(void* live):previous(g_handling){g_handling=live;}~Scope(){g_handling=previous;}} scope{ability};
    Source source{};Name slot{};float duration=0;uintptr_t projectile=0;native_objects::Identity cast{},pawn{};
    if(g_failed||!std::isfinite(rate)||rate<=0||rate>10||section.index||section.number||!Animation(c,source)||!Montage(montage,slot,duration)||
       !Projectile(p,projectile)||!native_objects::CaptureLive(g_base,ability,cast)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),pawn)){Stop(ability);return true;}
    const auto grounded=foreign_montage::CaltropsGesture(source.skeleton,source.length);
    if(grounded){source.sequence=grounded;duration=source.length;}
    else source.sequence=foreign_montage::CastingSequence(source.sequence,source.skeleton,source.length);
    auto* replacement=g_create(reinterpret_cast<void*>(source.sequence),slot,.1f,.18f,1,1,-1,0);
    if(!Alive(cast)||!Alive(pawn))return true;
    const auto clone=reinterpret_cast<uintptr_t>(replacement);uintptr_t skeleton=0;float length=0;
    if(!clone||clone==reinterpret_cast<uintptr_t>(montage)||!Read(clone,0x10,cls)||!Is(cls,"AnimMontage")||!Read(clone,0x38,skeleton)||skeleton!=source.skeleton||
       !Read(clone,0x90,length)||!std::isfinite(length)||std::fabs(length-source.length)>.001f){Stop(ability);return true;}
    const float adjusted=rate*length/duration;
    if(!std::isfinite(adjusted)||adjusted<=.01f||adjusted>=100){Stop(ability);return true;}
    foreign_montage::CopyLaterSounds(reinterpret_cast<uintptr_t>(montage),replacement);
    native(ability,replacement,adjusted,section);
    uintptr_t instance=0;
    if(!Alive(cast)||!Alive(pawn))return true;
    uintptr_t ownerAfter=0,characterAfter=0,projectileAfter=0;
    if(!Read(p,0x48,ownerAfter)||ownerAfter!=actor||!Read(p,0x1B0,characterAfter)||characterAfter!=actor||
       !Projectile(p,projectileAfter)||projectileAfter!=projectile){g_failed=true;return true;}
    if(!Read(p,0x1D8,instance)||!instance||!g_playing(reinterpret_cast<void*>(instance))||!Owner(reinterpret_cast<void*>(actor),c)){Stop(ability);return true;}
    // Deliberate ground prototype: request the native base spawner at cast
    // start, not a transplanted notification. Native creation keeps ownership,
    // upgrades and damage. No Strife socket, shared component, timer or CDO edit.
    g_spawn(ability,reinterpret_cast<void*>(projectile),source.pelvis);
    if(Alive(cast)&&Alive(pawn))foreign_montage::StartSounds(reinterpret_cast<uintptr_t>(montage),ability);
    if(g_log&&g_events++<12)g_log(grounded?
        "Caltrops V1.122: single complete grounded War gesture at authored speed, no Start/End splice; native spawner; EXPERIMENTAL":
        "Caltrops V1.122: grounded gesture unavailable; V1.121 original-span fallback retained; EXPERIMENTAL");
    return true;
}
void Shutdown(){g_ready=false;}
}
