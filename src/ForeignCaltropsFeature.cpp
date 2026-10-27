#include "ForeignCaltropsFeature.h"
#include "ForeignMontage.h"
#include "DashCombatFeature.h"
#include "NativeObjectName.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include <MinHook.h>
#include <atomic>
#include <array>
#include <cmath>

namespace dg::foreign_caltrops {
namespace {
using Create=void*(*)(void*,Name,float,float,float,int,float,float);
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
using Notify=void(*)(void*,void*,void*);
using Event=void(*)(void*,void*,void*);
// 7AF460 decodes a subclass and an eight-byte FName, then calls 5AE2B0.
// That native helper uses this ability's Character, authority and spawn path.
using Spawn=void(*)(void*,void*,Name);
Create g_create=nullptr;Finish g_finish=nullptr;Playing g_playing=nullptr;Spawn g_spawn=nullptr;
std::array<Notify,3> g_donors{};
Event g_event=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false},g_failed{false};
native_objects::Identity g_power{},g_stomp{};unsigned g_events=0;
struct Vec {float x=0,y=0,z=0;};
struct Ground {native_objects::Identity movement{},root{};Vec position{};bool pending=false;};
struct Cue {native_objects::Identity actor{},ability{},mesh{},montage{},sequence{};Name bone{};uintptr_t world=0;bool spawned=false;Ground ground{};};
std::array<Cue,2> g_cues{};
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
bool Stomp(uintptr_t p){
    uintptr_t cls=0,package=0;float length=0,rate=0;Array events{};
    return Read(p,0x10,cls)&&Is(cls,"AnimSequence")&&Is(p,"War_Atk_Stoneskin")&&Read(p,0x20,package)&&
        Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Atk_Stoneskin")&&Read(p,0x90,length)&&
        std::isfinite(length)&&std::fabs(length-1.8333333731f)<.00001f&&Read(p,0x94,rate)&&rate==1&&
        Read(p,0x80,events)&&events.data&&events.count>=2&&events.count<=events.capacity&&events.capacity<=64;
}
bool Owner(void* live,local_players::Context& c,bool grounded=true){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0,movement=0,owner=0;unsigned char type=0,role=0,mode=0;
    return local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&
        c.kind==local_players::PoseKind::Foot&&c.pawn==actor&&c.character==actor&&
        Read(actor,0x10,cls)&&Is(cls,"War_Blueprint_C")&&Read(actor,0xC18,type)&&type==2&&Read(actor,0x118,role)&&role==3&&
        Read(actor,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==actor&&
        Read(actor,0x388,movement)&&Read(movement,0xE8,owner)&&owner==actor&&(!grounded||(Read(movement,0x1B0,mode)&&mode==1));
}
template<class T> bool Field(uintptr_t object,const char* name,const char* kind,T& value){
    uintptr_t cls=0;int at=-1;return Read(object,0x10,cls)&&native_objects::PropertyField(g_base,cls,name,kind,int(sizeof(T)),at)&&Read(object,size_t(at),value);
}
uintptr_t MovementFunction(uintptr_t movement){
    uintptr_t cls=0;if(!Read(movement,0x10,cls)||!Is(cls,"MayhemPlayerCharacterMovementComponent"))return 0;
    for(int level=0;cls&&level<16;++level){uintptr_t field=0;if(!Read(cls,0x48,field))return 0;
        for(int i=0;field&&i<512;++i){
            if(Is(field,"SetMovementMode")){uintptr_t kind=0,outer=0,entry=0;uint32_t flags=0;uint16_t bytes=0;int mode=-1,custom=-1;
                return Is(cls,"CharacterMovementComponent")&&Read(field,0x10,kind)&&Is(kind,"Function")&&Read(field,0x20,outer)&&outer==cls&&
                    Read(field,0xC0,entry)&&entry==g_base+0x1CF1600&&Read(field,0x98,flags)&&(flags&0x2400u)==0x400u&&Read(field,0x9E,bytes)&&bytes==2&&
                    native_objects::PropertyField(g_base,field,"NewMovementMode","ByteProperty",1,mode)&&mode==0&&
                    native_objects::PropertyField(g_base,field,"NewCustomMode","ByteProperty",1,custom)&&custom==1?field:0;
            }
            uintptr_t next=0;if(!Read(field,0x28,next)||next==field)return 0;field=next;
        }
        if(field||!Read(cls,0x40,cls))return 0;
    }return 0;
}
bool Position(const Vec& v){return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
void CaptureGround(const local_players::Context& c,Ground& out){
    uintptr_t movement=0,root=0,updated=0,owner=0;Vec velocity{},position{};unsigned char mode=0,custom=0;
    if(!g_event||!Read(c.character,0x388,movement)||!Read(movement,0xE8,owner)||owner!=c.character||
       !Field(movement,"CharacterOwner","ObjectProperty",owner)||owner!=c.character||!Read(movement,0x1B0,mode)||mode!=1||
       !Read(movement,0x1B1,custom)||custom||!Field(c.character,"RootComponent","ObjectProperty",root)||!Read(root,0xE8,owner)||owner!=c.character||
       !Field(movement,"UpdatedComponent","ObjectProperty",updated)||updated!=root||!Read(root,0x1A0,position)||!Position(position)||
       !Field(movement,"Velocity","StructProperty",velocity)||!Position(velocity)||std::fabs(velocity.z)>1||!MovementFunction(movement))return;
    Ground next{};if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(movement),next.movement)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(root),next.root))return;
    next.position=position;next.pending=true;out=next;
}
bool Animation(const local_players::Context& c,Source& out){
    uintptr_t anim=0,cls=0,owner=0,asset=0,skeleton=0;Array bones{};struct Bone {Name name;int parent=0;} pelvis{};
    if(!Alive(g_stomp)||!Stomp(g_stomp.address))return false;
    out.sequence=g_stomp.address;
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
bool Montage(void* live,Name& slot){
    const auto p=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,package=0,skeleton=0;float length=0,rate=0;Array tracks{};
    return Read(p,0x10,cls)&&Is(cls,"AnimMontage")&&Read(p,0x20,package)&&
        ((Is(p,"Strife_Caltrops_Ground_F_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Caltrops_Ground_F_Montage"))||
         (Is(p,"Strife_Caltrops_Ground_B_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Caltrops_Ground_B_Montage")))&&
        Read(p,0x38,skeleton)&&Is(skeleton,"Strife_Skeleton")&&Read(p,0x90,length)&&std::isfinite(length)&&std::fabs(length-2.3333332539f)<.001f&&
        Read(p,0x94,rate)&&rate==1&&Read(p,0x158,tracks)&&tracks.data&&tracks.count==1&&tracks.capacity>=1&&tracks.capacity<=4&&
        Read(tracks.data,0,slot)&&native_objects::NameValueIs(g_base,slot.index,slot.number,"TPS");
}
void Stop(void* ability){
    g_failed=true;g_finish(ability,0);
    if(g_log&&g_events++<12)g_log("Caltrops V1.93 stopped: owned War cast contract unavailable; native finish requested, select Native; no repeated constructor attempt");
}
void Donor(size_t which,void* notify,void* mesh,void* animation){
    if(!HandleNotify(notify,mesh,animation)&&g_donors[which])g_donors[which](notify,mesh,animation);
}
void SkinNotify(void* notify,void* mesh,void* animation){Donor(0,notify,mesh,animation);}
void ActorNotify(void* notify,void* mesh,void* animation){Donor(1,notify,mesh,animation);}
void BlastNotify(void* notify,void* mesh,void* animation){Donor(2,notify,mesh,animation);}
}
void Initialize(LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    g_event=reinterpret_cast<Event>(g_base+0xAC0630);
    constexpr unsigned char spawn[]={0x48,0x89,0x5C,0x24,0x18,0x48,0x89,0x74,0x24,0x20,0x48,0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xEC,0x30,0x48,0x8B,0x81,0xB0};
    constexpr unsigned char create[]={0x48,0x8B,0xC4,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x8D};
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x5AE2B0),spawn,sizeof(spawn))||std::memcmp(reinterpret_cast<void*>(g_base+0x168A410),create,sizeof(create))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))||std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing)))return;
    g_create=reinterpret_cast<Create>(g_base+0x168A410);g_finish=reinterpret_cast<Finish>(g_base+0x59B080);
    g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_spawn=reinterpret_cast<Spawn>(g_base+0x5AE2B0);
    // Read-only capture verified these three entries in the source sequence's
    // notify vtables (+0x240), including the Blueprint coating notification.
    constexpr uintptr_t donorRvas[]={0x1697780,0x5D7C00,0x5D7DF0};
    constexpr unsigned char donorBytes[3][16]={
        {0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0x59,0x30,0x48,0x8B},
        {0x48,0x89,0x5C,0x24,0x18,0x48,0x89,0x54,0x24,0x10,0x55,0x48,0x8B,0xEC,0x48,0x83},
        {0x48,0x89,0x74,0x24,0x18,0x48,0x89,0x7C,0x24,0x20,0x41,0x56,0x48,0x83,0xEC,0x50}};
    for(size_t i=0;i<3;++i)if(std::memcmp(reinterpret_cast<void*>(g_base+donorRvas[i]),donorBytes[i],16))return;
    const auto initialized=MH_Initialize();if(initialized!=MH_OK&&initialized!=MH_ERROR_ALREADY_INITIALIZED)return;
    const Notify detours[]={SkinNotify,ActorNotify,BlastNotify};size_t created=0;
    for(;created<3;++created)if(MH_CreateHook(reinterpret_cast<void*>(g_base+donorRvas[created]),reinterpret_cast<void*>(detours[created]),reinterpret_cast<void**>(&g_donors[created]))!=MH_OK)break;
    bool enabled=created==3;
    if(enabled)for(size_t i=0;i<3;++i)if(MH_EnableHook(reinterpret_cast<void*>(g_base+donorRvas[i]))!=MH_OK){enabled=false;break;}
    if(!enabled){for(size_t i=0;i<created;++i){MH_DisableHook(reinterpret_cast<void*>(g_base+donorRvas[i]));MH_RemoveHook(reinterpret_cast<void*>(g_base+donorRvas[i]));}g_donors={};return;}
    g_ready=true;
    if(log)log("Caltrops V1.123 READY: complete own War stomp, three signature-checked donor guards; no timer or physics/VM write; EXPERIMENTAL UNTESTED IN GAME");
}
int Status(){return !g_ready||g_failed?0:(AssetsReady()?2:1);}
bool IsClass(uintptr_t p){return g_ready&&native_tps::OnGameThread()&&p==g_power.address&&Alive(g_power)&&Class(p);}
bool AssetsReady(){return Alive(g_power)&&Alive(g_stomp);}
bool InvalidateAssets(){
    if(!g_ready||!native_tps::OnGameThread())return false;bool changed=false;
    for(auto* id:{&g_power,&g_stomp})if(*id&&!Alive(*id)){*id={};changed=true;}return changed;
}
void ObserveObject(uintptr_t object,unsigned kind){
    if(!g_ready||!native_tps::OnGameThread())return;
    if(kind==1&&!g_power&&Class(object))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_power);
    if(kind==3&&!g_stomp&&Stomp(object))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(object),g_stomp);
}
bool Prepare(void* actor,uintptr_t& abilityClass){
    if(!g_ready||g_failed||!native_tps::OnGameThread()||!IsClass(g_power.address)||!dash_combat::PresentationReady())return false;
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
    Source source{};Name slot{};uintptr_t projectile=0;native_objects::Identity cast{},pawn{};
    if(g_failed||!dash_combat::PresentationReady()||!std::isfinite(rate)||rate<=0||rate>10||section.index||section.number||!Animation(c,source)||!Montage(montage,slot)||
       !Projectile(p,projectile)||!native_objects::CaptureLive(g_base,ability,cast)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),pawn)){Stop(ability);return true;}
    Ground ground{};CaptureGround(c,ground);
    auto* replacement=g_create(reinterpret_cast<void*>(source.sequence),slot,.1f,.18f,1,1,-1,0);
    if(!Alive(cast)||!Alive(pawn))return true;
    const auto clone=reinterpret_cast<uintptr_t>(replacement);uintptr_t skeleton=0;float length=0;
    if(!clone||clone==reinterpret_cast<uintptr_t>(montage)||!Read(clone,0x10,cls)||!Is(cls,"AnimMontage")||!Read(clone,0x38,skeleton)||skeleton!=source.skeleton||
       !Read(clone,0x90,length)||!std::isfinite(length)||std::fabs(length-source.length)>.001f){Stop(ability);return true;}
    const float adjusted=rate;
    if(!std::isfinite(adjusted)||adjusted<=.01f||adjusted>=100){Stop(ability);return true;}
    foreign_montage::CopyLaterSounds(reinterpret_cast<uintptr_t>(montage),replacement);
    native(ability,replacement,adjusted,section);
    uintptr_t instance=0;
    if(!Alive(cast)||!Alive(pawn))return true;
    uintptr_t ownerAfter=0,characterAfter=0,projectileAfter=0;
    if(!Read(p,0x48,ownerAfter)||ownerAfter!=actor||!Read(p,0x1B0,characterAfter)||characterAfter!=actor||
       !Projectile(p,projectileAfter)||projectileAfter!=projectile){g_failed=true;return true;}
    if(!Read(p,0x1D8,instance)||!instance||!g_playing(reinterpret_cast<void*>(instance))||!Owner(reinterpret_cast<void*>(actor),c)){Stop(ability);return true;}
    Cue next{};next.actor=pawn;next.ability=cast;next.sequence=g_stomp;next.bone=source.pelvis;next.world=c.world;next.ground=ground;
    if(!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(source.mesh),next.mesh)||
       !native_objects::CaptureLive(g_base,replacement,next.montage)){Stop(ability);return true;}
    if(c.index==0)g_cues[0]=next;
    else if(c.index==1)g_cues[1]=next;
    else {Stop(ability);return true;}
    if(Alive(cast)&&Alive(pawn))foreign_montage::StartSounds(reinterpret_cast<uintptr_t>(montage),ability);
    if(g_log&&g_events++<12)g_log("Caltrops V1.123: complete War Stoneskin gesture; native spawner awaits foot stomp; donor coating/actor/blast guards active; no timer or physics/VM write; EXPERIMENTAL");
    return true;
}
bool HandleNotify(void* live,void* mesh,void* animation){
    if(!g_ready||!native_tps::OnGameThread()||(!g_cues[0].actor&&!g_cues[1].actor))return false;
    const auto notify=reinterpret_cast<uintptr_t>(live),component=reinterpret_cast<uintptr_t>(mesh),asset=reinterpret_cast<uintptr_t>(animation);
    uintptr_t actor=0,outer=0;if(!Read(component,0xE8,actor)||!Read(notify,0x20,outer)||outer!=asset)return false;
    for(auto& cue:g_cues){
        if(cue.actor.address!=actor||cue.mesh.address!=component||asset!=cue.sequence.address||!Alive(cue.actor)||!Alive(cue.ability)||
           !Alive(cue.mesh)||!Alive(cue.montage)||!Alive(cue.sequence))continue;
        local_players::Context c{};uintptr_t owner=0,character=0,instance=0,current=0,abilities=0,list=0,last=0;int count=0,capacity=0;
        if(!Read(actor,0x380,owner)||owner!=component||!Read(cue.ability.address,0x48,owner)||owner!=actor||!Read(cue.ability.address,0x1B0,character)||character!=actor||
           !Read(cue.ability.address,0x1D8,instance)||!Read(instance,0x28,current)||current!=cue.montage.address||!g_playing(reinterpret_cast<void*>(instance))||
           !Read(actor,0xC70,abilities)||!Read(abilities,0xE8,owner)||owner!=actor||!Read(abilities,0xF8,list)||
           !Read(abilities,0x100,count)||!Read(abilities,0x104,capacity)||count<=0||count>capacity||capacity>64||
           !Read(list,size_t(count-1)*8,last)||last!=cue.ability.address)continue;
        // A still-playing private cast never receives donor Stoneskin
        // cosmetics after ownership/form loss, but cannot spawn caltrops.
        if(!Owner(reinterpret_cast<void*>(actor),c,false)||c.world!=cue.world||c.index<0||c.index>1||&g_cues[size_t(c.index)]!=&cue)return true;
        uintptr_t cls=0,effect=0;unsigned char state=0;
        const bool stomp=Read(notify,0x10,cls)&&Is(cls,"AnimNotify_PlayParticleEffect")&&Read(notify,0x38,effect)&&
            (Is(effect,"P_WarStoneSkin_Base")||Is(effect,"P_WarStoneSkin_Base_NoDust"));
        if(stomp&&!cue.spawned&&Read(cue.ability.address,0xD8,state)&&(state==1||state==2)){
            uintptr_t projectile=0;if(!Projectile(cue.ability.address,projectile))return true;
            cue.spawned=true;g_spawn(reinterpret_cast<void*>(cue.ability.address),reinterpret_cast<void*>(projectile),cue.bone);
            if(g_log&&g_events++<16)g_log("Caltrops V1.123 STOMP: one native spawner on own War pelvis; donor Stoneskin presentation suppressed");
        }
        // Narrow hooks consume donor particles/BP visual notifies and
        // Stinger actor spawn only in this exact current private cast.
        return true;
    }return false;
}
void Observe(const local_players::Context& c){
    if(!g_ready||!g_event||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto& cue=g_cues[size_t(c.index)];auto& ground=cue.ground;if(!ground.pending)return;
    // One early request for this grounded foreign cast, never a held mode.
    ground.pending=false;
    if(!c.localPlayer||!c.world||c.character!=cue.actor.address||c.pawn!=c.character||c.world!=cue.world||c.kind!=local_players::PoseKind::Foot||
       !Alive(cue.actor)||!Alive(cue.ability)||!Alive(cue.mesh)||!Alive(cue.montage)||!Alive(ground.movement)||!Alive(ground.root)||
       native_tps::IsWorldPaused(reinterpret_cast<void*>(c.character))||native_tps::CinematicActive(unsigned(c.index))||native_tps::RadialActive(unsigned(c.index)))return;
    local_players::Context actual{};uintptr_t owner=0,movement=0,root=0,updated=0,helper=0,montage=0,mesh=0,component=0,list=0,last=0;int count=0,capacity=0;
    unsigned char state=0,mode=0,custom=0;float age=0;Vec position{},velocity{};
    if(!Owner(reinterpret_cast<void*>(c.character),actual,false)||actual.index!=c.index||actual.world!=c.world||actual.controller!=c.controller||actual.localPlayer!=c.localPlayer||
       !Read(c.character,0x380,mesh)||mesh!=cue.mesh.address||!Read(mesh,0xE8,owner)||owner!=c.character||
       !Read(cue.ability.address,0x48,owner)||owner!=c.character||!Read(cue.ability.address,0x1B0,owner)||owner!=c.character||
       !Read(cue.ability.address,0xD8,state)||state!=2||!Read(cue.ability.address,0xDC,age)||!std::isfinite(age)||age<0||age>.15f||
       !Read(cue.ability.address,0x1D8,helper)||!Read(helper,0x28,montage)||montage!=cue.montage.address||!g_playing(reinterpret_cast<void*>(helper))||
       !Read(c.character,0xC70,component)||!Read(component,0xE8,owner)||owner!=c.character||!Read(component,0xF8,list)||
       !Read(component,0x100,count)||!Read(component,0x104,capacity)||count<=0||count>capacity||capacity>64||!Read(list,size_t(count-1)*8,last)||last!=cue.ability.address||
       !Read(c.character,0x388,movement)||movement!=ground.movement.address||!Read(movement,0xE8,owner)||owner!=c.character||
       !Field(movement,"CharacterOwner","ObjectProperty",owner)||owner!=c.character||!Field(c.character,"RootComponent","ObjectProperty",root)||root!=ground.root.address||
       !Read(root,0xE8,owner)||owner!=c.character||!Field(movement,"UpdatedComponent","ObjectProperty",updated)||updated!=root||
       !Read(movement,0x1B0,mode)||mode!=5||!Read(movement,0x1B1,custom)||custom||!Read(root,0x1A0,position)||!Position(position)||
       !Field(movement,"Velocity","StructProperty",velocity)||!Position(velocity)||std::fabs(velocity.z)>1)return;
    const float dx=position.x-ground.position.x,dy=position.y-ground.position.y;
    if(!std::isfinite(dx)||!std::isfinite(dy)||std::fabs(position.z-ground.position.z)>1.5f||dx*dx+dy*dy>625.f)return;
    const auto function=MovementFunction(movement);if(!function)return;
    struct Args {unsigned char mode=1,custom=0;} args;static_assert(sizeof(Args)==2);
    // Explicitly authorized public API; engine owns floor/gravity changes.
    // No mode/velocity/animation field or VM instruction is written here.
    g_event(reinterpret_cast<void*>(movement),reinterpret_cast<void*>(function),&args);
    const bool walking=Alive(ground.movement)&&Read(movement,0x1B0,mode)&&mode==1;
    if(g_log&&g_events++<20)g_log(walking?
        "Caltrops V1.124 GROUND_REQUEST: one public walking request, walking readback; native floor/gravity retained; hop EXPERIMENTAL":
        "Caltrops V1.124 GROUND_REQUEST: one public request, native result not walking or unavailable; no retry; hop EXPERIMENTAL");
}
void Shutdown(){g_ready=false;if(native_tps::OnGameThread())g_cues={};}
}
