#include "ProjectileAimFeature.h"
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dg::projectile_aim {
namespace {
using Vec3=native_tps::Vec3;
struct Rotator {float pitch,yaw,roll;};
static_assert(sizeof(Rotator)==12&&sizeof(Vec3)==12);
// Reflected pistol return: RCX=component, RDX=Rotator return, R8=input.
// One target/direction resolver. Native action authorizes both modes, even
// on release. Ability correction is AFTER 59C930 replaces the early yaw with
// Instigator yaw; the obsolete early 59D280 hook is removed. No actor write.
using SpawnRotation=Rotator*(*)(void*,Rotator*,const void*);
using AbilityRotation=Rotator*(*)(void*,Rotator*);
using LavaUpdate=void(*)(void*);
using ClassGetter=void*(*)();
using Gravity=float(*)(void*);
using ArcAngle=float(*)(float,float,float);
using ArcSpeed=float(*)(const Vec3*,const Vec3*,float,float);
using ForwardVector=Vec3*(*)(void*,Vec3*);
using SetTargetComponent=void(*)(void*,void*);
SpawnRotation g_original=nullptr;
AbilityRotation g_finish=nullptr;
LavaUpdate g_lava=nullptr;
ClassGetter g_movementClass=nullptr;
Gravity g_gravity=nullptr;
ArcAngle g_arcAngle=nullptr;
ArcSpeed g_arcSpeed=nullptr;
ForwardVector g_forward=nullptr;
SetTargetComponent g_setTarget=nullptr;std::atomic_bool g_staticReady{false};
std::atomic_uint32_t g_staticTargets[2]{};
RayProvider g_provider=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_uint32_t g_calls{0},g_adjusted[2]{},g_finished[2]{},g_lobbed[2]{},g_lavaRejected{0};
std::atomic_uint32_t g_beams[2]{};
template<class T> bool Read(const void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;
    return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
bool TypedClass(void* object,void* expected,bool allowDefaults=false){
    void* actual=nullptr;void* array=nullptr;void* item=nullptr;uint32_t flags=0;
    int index=-1,last=-1;
    return expected&&Read(object,8,flags)&&!(flags&0x18000u)&&(allowDefaults||!(flags&0x30u))&&
        Read(object,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
bool Typed(void* object,uintptr_t slot){void* cls=nullptr;return Read(reinterpret_cast<void*>(g_base+slot),0,cls)&&TypedClass(object,cls);}
bool WorldEnder(void* beam){
    void* actor=nullptr;void* weapon=nullptr;void* reciprocal=nullptr;void* projectile=nullptr;uintptr_t cls=0;
    // Provenance covers all native sizes/upgrades, including Ice Beam.
    // This ability's montage owns the muzzle axis; changing only the beam
    // toward the camera can point it opposite to the character and guns.
    return Read(beam,0x108,actor)&&Read(actor,0xF98,weapon)&&Read(weapon,0xE8,reciprocal)&&reciprocal==actor&&
        Read(weapon,0x10,cls)&&native_objects::NameIs(g_base,cls,"MayhemWorldEnder_Blueprint_C")&&
        Read(weapon,0x688,projectile)&&projectile==beam;
}
bool Owner(void* weapon,void* actor,local_players::Context& owner){
    unsigned char type=0;void* reciprocal=nullptr;
    return native_tps::OnGameThread()&&Typed(weapon,0x380C0E0)&&
        Read(weapon,0xE8,reciprocal)&&reciprocal==actor&&local_players::ResolveActor(actor,owner)&&
        owner.index>=0&&owner.index<2&&Read(reinterpret_cast<void*>(owner.character),0xC18,type)&&type==1&&
        !native_tps::CinematicActive(unsigned(owner.index))&&!native_tps::IsWorldPaused(actor)&&
        !native_tps::RadialActive(unsigned(owner.index));
}
bool AbilityOwner(void* ability,uintptr_t slot,local_players::Context& owner){
    void* actor=nullptr;void* weapon=nullptr;
    return Typed(ability,slot)&&Read(ability,0x1B0,actor)&&Read(ability,0x50,weapon)&&Owner(weapon,actor,owner);
}
bool Finite(const Vec3& value){return std::isfinite(value.x)&&std::isfinite(value.y)&&std::isfinite(value.z);}
bool Direction(const Vec3& origin,const Vec3& target,Rotator& rotation){
    const float x=target.x-origin.x,y=target.y-origin.y,z=target.z-origin.z;
    const float flat=std::hypot(x,y);
    if(!Finite(origin)||!Finite(target)||!std::isfinite(flat)||std::hypot(flat,z)<0.01f)return false;
    constexpr float deg=57.29577951308232f;
    rotation.pitch=std::atan2(z,flat)*deg;rotation.yaw=std::atan2(y,x)*deg;
    return std::isfinite(rotation.pitch)&&std::isfinite(rotation.yaw);
}
bool Target(const local_players::Context& owner,const Vec3& origin,Vec3& target,bool& hit){
    native_tps::MouseRay ray{};
    return g_provider&&g_provider(owner,ray)&&native_tps::TraceCameraTarget(owner,ray,target,hit,&origin);
}
bool AbilityOrigin(void* ability,Vec3& origin){
    void* instigator=nullptr;void* root=nullptr;
    return Read(ability,0x48,instigator)&&Typed(instigator,0x3A58AB0)&&
        Read(instigator,0x158,root)&&Read(root,0x1A0,origin)&&Finite(origin);
}
bool AimAt(const local_players::Context& owner,const Vec3& origin,Rotator& rotation,bool nativeVertical=false){
    Vec3 target{};bool hit=false;
    if(!Target(owner,origin,target,hit))return false;
    Rotator adjusted=rotation;
    if(!Direction(origin,target,adjusted))return false;
    // Shared yaw/release/target correction is NEVER disabled by the option.
    // Only vertical freedom differs. Native Lava pitch stays coupled to speed.
    if(nativeVertical||!runtime::Get().tps3DAimEnabled.load())adjusted.pitch=rotation.pitch;
    rotation=adjusted;return true;
}
void Observe(const char* path,const local_players::Context& owner,unsigned n,const Rotator& result){
    if(g_log&&(n<=12||n%1000==0)){char line[230]{};
        sprintf_s(line,"TPS V1.53 TARGET3D path=%s player=%d n=%u pitch=%.2f yaw=%.2f nativeAction=1 actorRotationWritten=0 impactValidation=USER_REQUIRED",
            path,owner.index+1,n,result.pitch,result.yaw);g_log(line);}
}
Rotator* Rotation(void* weapon,Rotator* out,const void* input,uintptr_t caller){
    auto* result=g_original?g_original(weapon,out,input):out;++g_calls;
    if(!result||caller!=0x7CFA3B||!native_tps::OnGameThread())return result;
    void* actor=nullptr;local_players::Context owner{};
    if(!Read(weapon,0xE8,actor)||!Owner(weapon,actor,owner))return result;
    // Reflected GetProjectileSpawnRotation has a FVector SpawnLocation
    // parameter (3640478); 7CF9B0 forwards that actual value as R8.
    Vec3 origin{};if(!Read(input,0,origin)||!Finite(origin))return result;
    Rotator adjusted=*result;
    if(AimAt(owner,origin,adjusted)){*result=adjusted;Observe("PISTOL",owner,owner.index==0?++g_adjusted[0]:++g_adjusted[1],adjusted);}
    return result;
}
Rotator* HookRotation(void* weapon,Rotator* out,const void* input){
    return Rotation(weapon,out,input,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
// Native 697FD0 creates the PRIMARY beam, stores it in weapon+688, then
// assigns the input interface's assisted target via 617B90 at 6981CB.
// This bypasses the free-fire forward-vector path. Suppress only this
// initial assignment for an owned Static Shot in TPS Camera Forward.
// Child beams, hit retargeting, other ammunition and native view stay native.
bool PrimaryStatic(void* beam,local_players::Context& owner){
    if(!g_staticReady.load()||!native_tps::OnGameThread()||!native_tps::TargetReady()||!Typed(beam,0x380A4B0))return false;
    void* actor=nullptr;void* root=nullptr;void* volume=nullptr;void* registry=nullptr;void* reciprocal=nullptr;void* list=nullptr;
    unsigned char type=0,role=0;int count=0,capacity=0;
    if(!Read(beam,0x108,actor)||!local_players::ResolveActor(actor,owner)||owner.index<0||owner.index>1||
       owner.kind!=local_players::PoseKind::Foot||owner.character!=reinterpret_cast<uintptr_t>(actor)||owner.pawn!=owner.character||
       !owner.localPlayer||!owner.controller||!Read(actor,0xC18,type)||type!=1||!Read(actor,0x118,role)||role!=3||
       native_tps::CinematicActive(unsigned(owner.index))||native_tps::IsWorldPaused(actor)||native_tps::RadialActive(unsigned(owner.index))||
       !Read(beam,0x158,root)||!Read(beam,0x5E8,volume)||!root||root!=volume||!Read(root,0xE8,reciprocal)||reciprocal!=beam||
       !Read(actor,0xC78,registry)||!Read(registry,0xE8,reciprocal)||reciprocal!=actor||!Read(registry,0x108,list)||!list||
       !Read(registry,0x110,count)||!Read(registry,0x114,capacity)||count<4||count>capacity||capacity>32)return false;
    for(unsigned slot:{2u,3u}){
        void* weapon=nullptr;void* primary=nullptr;uintptr_t cls=0,package=0;uint32_t flags=0;
        if(Read(list,slot*8,weapon)&&weapon&&Read(weapon,8,flags)&&!(flags&0x18030u)&&
           Read(weapon,0xE8,reciprocal)&&reciprocal==actor&&Read(weapon,0x688,primary)&&primary==beam&&
           Read(weapon,0x10,cls)&&native_objects::NameIs(g_base,cls,"MayhemStaticShot_Blueprint_C")&&
           Read(cls,0x20,package)&&native_objects::NameIs(g_base,package,"/Game/Blueprints/Player/MayhemStaticShot_Blueprint"))return true;
    }
    return false;
}
void AssignTarget(void* beam,void* component,uintptr_t caller){
    local_players::Context owner{};native_tps::MouseRay ray{};
    bool manual=false;
    if(component&&caller==0x6981D0&&PrimaryStatic(beam,owner)&&g_provider&&g_provider(owner,ray)){
        const float length=ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z;
        manual=Finite(ray.origin)&&Finite(ray.direction)&&std::isfinite(length)&&length>.99f&&length<1.01f;
    }
    // Forward the exact original call once. The native setter owns target
    // bookkeeping; no raw target/actor/rotation/range/damage field writes.
    if(g_setTarget)g_setTarget(beam,manual?nullptr:component);
    if(manual){const unsigned n=++g_staticTargets[owner.index];
        if(g_log&&n<=8){char line[210]{};sprintf_s(line,
            "TPS V1.129 STATIC_PRIMARY player=%d initial assisted target omitted; free ray and secondary chains native; impactValidation=USER_REQUIRED",owner.index+1);g_log(line);}}
}
void HookAssignTarget(void* beam,void* component){AssignTarget(beam,component,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);}
// 61D410 is MayhemBeamProjectile's native Tick. In its free-fire branch it reads
// the ROOT/SOCKET forward vector, bypassing the projectile SpawnRotation.
// Replace this one local beam consumer's return, never the scene transform,
// beam range, damage, secondary chains or every UE forward-vector call.
Vec3* BeamForward(void* component,Vec3* out,uintptr_t caller){
    auto* result=g_forward?g_forward(component,out):out;
    if(!result||caller!=0x61D5E5||!native_tps::OnGameThread())return result;
    void* beam=nullptr;void* root=nullptr;void* volume=nullptr;void* actor=nullptr;
    local_players::Context owner{};unsigned char type=0;Vec3 origin{};
    if(!Read(component,0xE8,beam)||!Typed(beam,0x380A4B0)||
       !Read(beam,0x158,root)||root!=component||!Read(beam,0x5E8,volume)||volume!=component||
       !Read(beam,0x108,actor)||!local_players::ResolveActor(actor,owner)||
       owner.index<0||owner.index>1||!Read(reinterpret_cast<void*>(owner.character),0xC18,type)||type!=1||
       WorldEnder(beam)||
       native_tps::CinematicActive(unsigned(owner.index))||native_tps::IsWorldPaused(actor)||
       native_tps::RadialActive(unsigned(owner.index))||!Read(component,0x1A0,origin)||!Finite(origin))return result;
    Rotator rotation{};
    // In simple mode preserve the native beam's vertical direction, just as
    // ordinary shots retain native pitch; common target/yaw correction stays.
    if(!Finite(*result)||!Direction(Vec3{},*result,rotation)||!AimAt(owner,origin,rotation))return result;
    constexpr float rad=0.01745329251994329577f;
    const float pitch=rotation.pitch*rad,yaw=rotation.yaw*rad;
    *result={std::cos(pitch)*std::cos(yaw),std::cos(pitch)*std::sin(yaw),std::sin(pitch)};
    Observe("BEAM_FORWARD",owner,owner.index==0?++g_beams[0]:++g_beams[1],rotation);
    return result;
}
Vec3* HookBeamForward(void* component,Vec3* out){
    return BeamForward(component,out,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
Rotator* Finish(void* ability,Rotator* out){
    auto* result=g_finish?g_finish(ability,out):out;
    if(!result||!native_tps::OnGameThread())return result;
    local_players::Context owner{};
    if(!AbilityOwner(ability,0x3811260,owner))return result;
    Vec3 origin{};if(!AbilityOrigin(ability,origin))return result;
    Rotator adjusted=*result;
    if(AimAt(owner,origin,adjusted,Typed(ability,0x3811050))){*result=adjusted;Observe("ABILITY_FINAL",owner,owner.index==0?++g_finished[0]:++g_finished[1],adjusted);}
    return result;
}
// The actual Lava preparation uses these native formulas and its own speed,
// world gravity*ProjectileGravityScale and angle bounds. Re-evaluate those
// against the camera hit. Sky/no-hit retains native free-aim speed/arc lift.
bool SolveLava(void* ability,const local_players::Context& owner,const Vec3& target,bool hit,const char*& rejected,uint32_t& defaultFlags){
    rejected="source/CDO/arc bounds";
    void* projectileClass=nullptr;void* defaults=nullptr;void* vtable=nullptr;
    uintptr_t find=0;
    Vec3 origin{};float low=0,high=0,initial=0,scale=0;
    if(!g_movementClass||!g_gravity||!g_arcAngle||!g_arcSpeed||!owner.world||
       !Read(ability,0x1E0,projectileClass)||!Read(projectileClass,0xF8,defaults)||!defaults||
       !Read(defaults,0,vtable)||!Read(vtable,0x5E0,find)||!find||
       !AbilityOrigin(ability,origin)||
       !Read(ability,0x204,low)||!Read(ability,0x208,high)||
       !std::isfinite(low)||!std::isfinite(high)||low>high||low < -89||high>89)return false;
    using FindComponent=void*(*)(void*,void*);
    void* movementClass=g_movementClass();
    void* movement=reinterpret_cast<FindComponent>(find)(defaults,movementClass);
    rejected="projectile movement defaults";
    // These are native CDO components, where ClassDefault/Archetype flags
    // are expected. The live weapon/ability guards still exclude defaults.
    if(!Read(movement,8,defaultFlags)||!TypedClass(movement,movementClass,true)||!Read(movement,0x138,initial)||!Read(movement,0x154,scale)||
       !std::isfinite(initial)||initial<=0||initial>1e7f||!std::isfinite(scale)||scale<0||scale>100)return false;
    rejected="world gravity";
    const float gravity=g_gravity(reinterpret_cast<void*>(owner.world))*scale;
    if(!std::isfinite(gravity)||gravity>0||gravity < -1e6f)return false;
    rejected="source/target direction";
    Rotator rotation{};
    if(!Read(ability,0x20C,rotation)||!std::isfinite(rotation.roll)||!Direction(origin,target,rotation))return false;
    rejected="native paired ballistic solve";
    float lift=(low+high)*0.5f,speed=initial;
    if(hit){
        const float distance=std::hypot(std::hypot(target.x-origin.x,target.y-origin.y),target.z-origin.z);
        const float angle=g_arcAngle(initial,gravity,distance);
        if(!std::isfinite(angle))return false;
        lift=std::clamp(angle,low,high);
    }
    rotation.pitch+=lift;
    if(hit)speed=g_arcSpeed(&origin,&target,rotation.pitch,gravity);
    if(!std::isfinite(rotation.pitch)||rotation.pitch < -89||rotation.pitch>89||
       !std::isfinite(speed)||speed<=0||speed>1e7f)return false;
    // Commit both coupled outputs only after the complete solve succeeds.
    std::memcpy(static_cast<unsigned char*>(ability)+0x20C,&rotation,sizeof(rotation));
    std::memcpy(static_cast<unsigned char*>(ability)+0x218,&speed,sizeof(speed));
    Observe(hit?"LAVA_HIT_ARC":"LAVA_FREE_ARC",owner,owner.index==0?++g_lobbed[0]:++g_lobbed[1],rotation);
    return true;
}
void UpdateLava(void* ability){
    if(g_lava)g_lava(ability); // native setup, CDO and charge execute first
    if(!native_tps::OnGameThread()||!runtime::Get().tps3DAimEnabled.load())return;
    local_players::Context owner{};Vec3 origin{},target{};bool hit=false;
    if(!AbilityOwner(ability,0x3811050,owner)||!AbilityOrigin(ability,origin)||!Target(owner,origin,target,hit))return;
    const char* rejected="unknown";uint32_t defaultFlags=0;
    if(!SolveLava(ability,owner,target,hit,rejected,defaultFlags)){
        const auto n=++g_lavaRejected;
        if(g_log&&(n<=8||n%1000==0)){char line[220]{};
            sprintf_s(line,"TPS V1.53 LAVA_REJECT player=%d n=%u stage=%s defaultFlags=0x%X nativePairRetained=1",owner.index+1,n,rejected,defaultFlags);g_log(line);}
    }
}
}
void Initialize(LogFn log,bool exactExecutable,RayProvider provider){
    g_log=log;g_provider=provider;if(!exactExecutable||!provider)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x6A9F20,{0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x50,0x48}},
        {0x59C930,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x50,0x48,0x8B,0x01,0x48,0x8B,0xFA}},
        {0x5B7DC0,{0x48,0x89,0x5C,0x24,0x18,0x55,0x48,0x8B,0xEC,0x48,0x83,0xEC,0x60,0x80,0xB9,0xA4}},
        {0x1DCEE00,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xAA,0x63,0xC9,0x01,0x48,0x85}},
        {0x1C9BCB0,{0x48,0x83,0xEC,0x28,0x48,0x8B,0x49,0x30,0x48,0x85,0xC9,0x74,0x1D,0xB2,0x01,0xE8}},
        {0x6647A0,{0x48,0x83,0xEC,0x28,0x0F,0x57,0xDB,0x0F,0x2F,0xCB,0x73,0x3E,0xF3,0x0F,0x59,0xC0}},
        {0x664800,{0x48,0x83,0xEC,0x68,0x44,0x0F,0x29,0x4C,0x24,0x20,0x0F,0x57,0xC0,0x44,0x0F,0x28}},
        {0x7C1CE0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x72,0xF5,0x04,0x03,0x48,0x85}},
        {0x7BABA0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xA2,0x64,0x05,0x03,0x48,0x85}},
        {0x1773600,{0x48,0x83,0xEC,0x18,0x0F,0x28,0x2D,0x05,0xB8,0xED,0x00,0x48,0x8B,0xC2,0x0F,0x29}},
        {0x61D410,{0x48,0x8B,0xC4,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D}},
        {0x61D5E0,{0xE8,0x1B,0x60,0x15,0x01,0x49,0x8B,0x8E,0xE8,0x05,0x00,0x00,0xF2,0x0F,0x10,0x00}},
        {0x763190,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x12,0x73,0x0A,0x03,0x48,0x85}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("TPS V1.53 projectile signatures mismatch; native retained");return;
    }
    g_movementClass=reinterpret_cast<ClassGetter>(g_base+0x1DCEE00);
    g_gravity=reinterpret_cast<Gravity>(g_base+0x1C9BCB0);
    g_arcAngle=reinterpret_cast<ArcAngle>(g_base+0x6647A0);
    g_arcSpeed=reinterpret_cast<ArcSpeed>(g_base+0x664800);
    struct Hook {uintptr_t rva;void* detour;void** original;};
    const Hook hooks[]={
        {0x6A9F20,reinterpret_cast<void*>(&HookRotation),reinterpret_cast<void**>(&g_original)},
        {0x59C930,reinterpret_cast<void*>(&Finish),reinterpret_cast<void**>(&g_finish)},
        {0x5B7DC0,reinterpret_cast<void*>(&UpdateLava),reinterpret_cast<void**>(&g_lava)},
        {0x1773600,reinterpret_cast<void*>(&HookBeamForward),reinterpret_cast<void**>(&g_forward)}
    };
    const auto status=MH_Initialize();if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED)return;
    unsigned created=0;bool ok=true;
    for(const auto& h:hooks){if(MH_CreateHook(reinterpret_cast<void*>(g_base+h.rva),h.detour,h.original)!=MH_OK){ok=false;break;}++created;}
    if(ok)for(const auto& h:hooks)if(MH_EnableHook(reinterpret_cast<void*>(g_base+h.rva))!=MH_OK){ok=false;break;}
    if(!ok){for(unsigned i=0;i<created;++i){MH_DisableHook(reinterpret_cast<void*>(g_base+hooks[i].rva));MH_RemoveHook(reinterpret_cast<void*>(g_base+hooks[i].rva));}
        g_original=nullptr;g_finish=nullptr;g_lava=nullptr;g_forward=nullptr;g_setTarget=nullptr;g_staticReady=false;
        if(log)log("TPS V1.53 projectile hook transaction failed; native retained");return;}
    if(log)log("TPS V1.53 TARGET3D READY: common target, native forward collision, scoped free-beam direction; original beam range/chains; EXPERIMENTAL");
    // The new optional boundary cannot disable the accepted aiming paths.
    const unsigned char setter[]={0x40,0x55,0x57,0x48,0x83,0xEC,0x28,0x48,0x83,0xB9,0x38,0x06,0x00,0x00,0x00,0x48};
    const unsigned char site[]={0xE8,0xC0,0xF9,0xF7,0xFF,0xEB,0x2D,0x48,0x8B,0x97,0xE8,0x00,0x00,0x00,0x48,0x8D};
    void* original=nullptr;void* target=reinterpret_cast<void*>(g_base+0x617B90);
    if(!std::memcmp(target,setter,sizeof(setter))&&!std::memcmp(reinterpret_cast<void*>(g_base+0x6981CB),site,sizeof(site))&&
       MH_CreateHook(target,reinterpret_cast<void*>(&HookAssignTarget),&original)==MH_OK){
        g_setTarget=reinterpret_cast<SetTargetComponent>(original);
        if(MH_EnableHook(target)==MH_OK)g_staticReady=true;
        else {MH_RemoveHook(target);g_setTarget=nullptr;}
    }
    if(log)log(g_staticReady?"TPS V1.129 STATIC_PRIMARY READY: initial Static assisted target scoped; secondary chains native; EXPERIMENTAL":"TPS V1.129 STATIC_PRIMARY unavailable; accepted aiming paths retained");
}
void Report(){
    if(g_log){char line[180]{};sprintf_s(line,"TPS V1.129 STATIC_PRIMARY totals P1=%u P2=%u initialAssignmentOnly=1 chainsNative=1 impactValidation=USER_REQUIRED",g_staticTargets[0].load(),g_staticTargets[1].load());g_log(line);}
    if(!g_log)return;char line[260]{};
    sprintf_s(line,"TPS V1.53 TARGET3D totals native=%u pistolP1=%u P2=%u finishP1=%u P2=%u lavaP1=%u P2=%u lavaRejected=%u impactValidation=USER_REQUIRED",
        g_calls.load(),g_adjusted[0].load(),g_adjusted[1].load(),g_finished[0].load(),g_finished[1].load(),g_lobbed[0].load(),g_lobbed[1].load(),g_lavaRejected.load());g_log(line);
    sprintf_s(line,"TPS V1.53 BEAM_FORWARD totals P1=%u P2=%u rootForwardConsumerOnly=1 socketAndTargetChainsNative=1 gameplayValidation=USER_REQUIRED",g_beams[0].load(),g_beams[1].load());g_log(line);
}
}
