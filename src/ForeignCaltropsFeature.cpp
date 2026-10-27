#include "ForeignCaltropsFeature.h"
#include "ForeignMontage.h"
#include "NativeObjectName.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include <array>
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
struct Continuity {native_objects::Identity actor{},ability{},mesh{},anim{};uintptr_t world=0;int flag=-1;};
std::array<Continuity,2> g_continuity{};unsigned g_continuityEvents=0;
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
bool Owner(void* live,local_players::Context& c,bool grounded=true){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0,movement=0,owner=0;unsigned char type=0,role=0,mode=0;
    return local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&
        c.kind==local_players::PoseKind::Foot&&c.pawn==actor&&c.character==actor&&
        Read(actor,0x10,cls)&&Is(cls,"War_Blueprint_C")&&Read(actor,0xC18,type)&&type==2&&Read(actor,0x118,role)&&role==3&&
        Read(actor,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==actor&&
        Read(actor,0x388,movement)&&Read(movement,0xE8,owner)&&owner==actor&&(!grounded||(Read(movement,0x1B0,mode)&&mode==1));
}
template<size_t N>bool FieldObject(uintptr_t cls,const char(&name)[N],uintptr_t& result){
    for(int level=0;cls&&level<16;++level){uintptr_t field=0;if(!Read(cls,0x48,field))return false;
        for(int i=0;field&&i<512;++i){if(Is(field,name)){result=field;return true;}uintptr_t next=0;if(!Read(field,0x28,next)||next==field)return false;field=next;}
        if(field||!Read(cls,0x40,cls))return false;
    }return false;
}
bool SlotFlag(uintptr_t anim,int& at){
    constexpr char slot[]="AnimGraphNode_Slot_E905C13C481BC22BAA7A96B6C1C1C9C3";
    uintptr_t cls=0,field=0,structure=0,type=0,boolField=0;int node=-1,nameAt=-1,flagAt=-1;Name name{};std::array<unsigned char,4> boolean{};
    if(!Read(anim,0x10,cls)||!Is(cls,"WarAnim_Blueprint_C")||
       !native_objects::PropertyField(g_base,cls,slot,"StructProperty",72,node)||!FieldObject(cls,slot,field)||
       !Read(field,0x70,structure)||!Is(structure,"AnimNode_Slot")||!Read(structure,0x10,type)||!Is(type,"ScriptStruct")||
       !native_objects::PropertyField(g_base,structure,"SlotName","NameProperty",8,nameAt)||nameAt!=32||
       !native_objects::PropertyField(g_base,structure,"bAlwaysUpdateSourcePose","BoolProperty",1,flagAt)||flagAt!=40||
       !FieldObject(structure,"bAlwaysUpdateSourcePose",boolField)||!Read(boolField,0x70,boolean)||
       boolean!=std::array<unsigned char,4>{1,0,1,255}||!Read(anim,size_t(node+nameAt),name)||
       !native_objects::NameValueIs(g_base,name.index,name.number,"TPS"))return false;
    at=node+flagAt;return true;
}
bool StoreFlag(uintptr_t anim,int at,unsigned char value){
    if(at<0||at>65535||value>1||anim>UINTPTR_MAX-size_t(at))return false;const auto p=anim+size_t(at);MEMORY_BASIC_INFORMATION m{};
    if(!VirtualQuery(reinterpret_cast<void*>(p),&m,sizeof(m))||m.State!=MEM_COMMIT||(m.Protect&(PAGE_GUARD|PAGE_NOACCESS))||
       ((m.Protect&0xFF)!=PAGE_READWRITE&&(m.Protect&0xFF)!=PAGE_EXECUTE_READWRITE))return false;
    const auto delta=p-reinterpret_cast<uintptr_t>(m.BaseAddress);if(delta>=m.RegionSize)return false;
    std::memcpy(reinterpret_cast<void*>(p),&value,1);return true;
}
void Restore(Continuity& saved){
    uintptr_t actor=0,anim=0;int at=-1;unsigned char value=0;
    if(saved.flag>=0&&Alive(saved.actor)&&Alive(saved.mesh)&&Alive(saved.anim)&&Read(saved.mesh.address,0xE8,actor)&&actor==saved.actor.address&&
       Read(saved.mesh.address,0x718,anim)&&anim==saved.anim.address&&SlotFlag(anim,at)&&at==saved.flag&&Read(anim,size_t(at),value)&&value==1)
        StoreFlag(anim,at,0);
    saved={};
}
void BeginContinuity(const local_players::Context& c,void* ability,uintptr_t mesh){
    if(c.index<0||c.index>1)return;auto& prior=g_continuity[size_t(c.index)];Restore(prior);
    Continuity saved{};local_players::Context owned{};uintptr_t anim=0,owner=0;int at=-1;unsigned char value=255,state=0;
    if(!Owner(reinterpret_cast<void*>(c.character),owned,false)||owned.index!=c.index||owned.world!=c.world||
       !Read(reinterpret_cast<uintptr_t>(ability),0x48,owner)||owner!=c.character||!Read(reinterpret_cast<uintptr_t>(ability),0x1B0,owner)||owner!=c.character||
       !Read(reinterpret_cast<uintptr_t>(ability),0xD8,state)||(state!=1&&state!=2)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),saved.actor)||!native_objects::CaptureLive(g_base,ability,saved.ability)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(mesh),saved.mesh)||!Read(mesh,0xE8,owner)||owner!=c.character||
       !Read(mesh,0x718,anim)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(anim),saved.anim)||!SlotFlag(anim,at)||
       !Read(anim,size_t(at),value)||value>1){if(g_log&&g_continuityEvents++<8)g_log("Caltrops V1.120 pose continuity unavailable; previous presentation retained");return;}
    if(value)return; // A native true value is never owned or reset by the mod.
    if(!StoreFlag(anim,at,1))return;saved.world=c.world;saved.flag=at;prior=saved;
    if(g_log&&g_continuityEvents++<8)g_log("Caltrops V1.120 source pose kept updating on owned War TPS slot; restored at cast end; EXPERIMENTAL visual continuity");
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
    source.sequence=foreign_montage::CastingSequence(source.sequence,source.skeleton,source.length);
    auto* replacement=g_create(reinterpret_cast<void*>(source.sequence),slot,.1f,.18f,1,1,-1,0);
    if(!Alive(cast)||!Alive(pawn))return true;
    const auto clone=reinterpret_cast<uintptr_t>(replacement);uintptr_t skeleton=0;float length=0;
    if(!clone||clone==reinterpret_cast<uintptr_t>(montage)||!Read(clone,0x10,cls)||!Is(cls,"AnimMontage")||!Read(clone,0x38,skeleton)||skeleton!=source.skeleton||
       !Read(clone,0x90,length)||!std::isfinite(length)||std::fabs(length-source.length)>.001f){Stop(ability);return true;}
    const float adjusted=rate*(source.sequence==g_idle.address?length/duration:1.f); // own gesture at native speed; accepted idle fallback
    if(!std::isfinite(adjusted)||adjusted<=.01f||adjusted>=100){Stop(ability);return true;}
    foreign_montage::CompleteGroundCast(replacement,source.skeleton,.55f); // same complete return as the user-validated War Clone
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
    if(Alive(cast)&&Alive(pawn))BeginContinuity(c,ability,source.mesh);
    if(Alive(cast)&&Alive(pawn))foreign_montage::StartSounds(reinterpret_cast<uintptr_t>(montage),ability);
    if(g_log&&g_events++<12)g_log("Caltrops V1.98: one native spawner, own War gesture at native speed; own War casting gesture when loaded, idle fallback; animation EXPERIMENTAL");
    return true;
}
void Observe(const local_players::Context& c){
    if(!native_tps::OnGameThread()||c.index<0||c.index>1)return;auto& saved=g_continuity[size_t(c.index)];if(!saved.actor)return;
    local_players::Context owned{};uintptr_t actor=0,mesh=0,anim=0,cls=0;unsigned char state=0;
    if(!g_ready||!Alive(saved.actor)||!Alive(saved.ability)||!Alive(saved.mesh)||!Alive(saved.anim)||
       c.world!=saved.world||c.character!=saved.actor.address||c.kind!=local_players::PoseKind::Foot||c.pawn!=c.character||
       !Owner(reinterpret_cast<void*>(saved.actor.address),owned,false)||owned.index!=c.index||owned.world!=saved.world||
       !Read(saved.ability.address,0x10,cls)||!IsClass(cls)||!Read(saved.ability.address,0x48,actor)||actor!=c.character||
       !Read(saved.ability.address,0x1B0,actor)||actor!=c.character||!Read(saved.ability.address,0xD8,state)||(state!=1&&state!=2)||
       !Read(c.character,0x380,mesh)||mesh!=saved.mesh.address||!Read(mesh,0x718,anim)||anim!=saved.anim.address)Restore(saved);
}
void Shutdown(){if(native_tps::OnGameThread())for(auto& saved:g_continuity)Restore(saved);g_ready=false;}
}
