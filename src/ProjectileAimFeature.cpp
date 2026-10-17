#include "ProjectileAimFeature.h"
#include "NativeMemoryReader.h"
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
// Finish abilities and Lava use DIFFERENT native paths. The native action
// authorizes the correction, including on release. No actor rotation write.
using SpawnRotation=Rotator*(*)(void*,Rotator*,const void*);
using AbilityRotation=Rotator*(*)(void*,Rotator*);
using LavaUpdate=void(*)(void*);
using AimOrigin=Vec3*(*)(Vec3*,void*);
using ClassGetter=void*(*)();
using Gravity=float(*)(void*);
using ArcAngle=float(*)(float,float,float);
using ArcSpeed=float(*)(const Vec3*,const Vec3*,float,float);
SpawnRotation g_original=nullptr;
AbilityRotation g_finish=nullptr;
LavaUpdate g_lava=nullptr;
AimOrigin g_origin=nullptr;
ClassGetter g_movementClass=nullptr;
Gravity g_gravity=nullptr;
ArcAngle g_arcAngle=nullptr;
ArcSpeed g_arcSpeed=nullptr;
RayProvider g_provider=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_uint32_t g_calls{0},g_adjusted[2]{},g_finished[2]{},g_lobbed[2]{},g_lavaRejected{0};
template<class T> bool Read(const void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;
    return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
bool TypedClass(void* object,void* expected){
    void* actual=nullptr;void* array=nullptr;void* item=nullptr;uint32_t flags=0;
    int index=-1,last=-1;
    return expected&&Read(object,8,flags)&&!(flags&0x30u)&&
        Read(object,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
bool Typed(void* object,uintptr_t slot){void* cls=nullptr;return Read(reinterpret_cast<void*>(g_base+slot),0,cls)&&TypedClass(object,cls);}
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
bool Target(const local_players::Context& owner,Vec3& target,bool& hit){
    native_tps::MouseRay ray{};
    return g_provider&&g_provider(owner,ray)&&native_tps::TraceCameraTarget(owner,ray,target,hit);
}
bool AimAt(const local_players::Context& owner,Rotator& rotation){
    Vec3 target{},origin{};bool hit=false;
    if(!g_origin||!Target(owner,target,hit))return false;
    // The native aim-reference helper used by 663F80. This is NOT a claim
    // that every projectile spawns at this socket; near-parallax needs play.
    g_origin(&origin,reinterpret_cast<void*>(owner.pawn));
    return Direction(origin,target,rotation);
}
void Observe(const char* path,const local_players::Context& owner,unsigned n,const Rotator& result){
    if(g_log&&(n<=12||n%1000==0)){char line[230]{};
        sprintf_s(line,"TPS V1.50 TARGET3D path=%s player=%d n=%u pitch=%.2f yaw=%.2f nativeAction=1 actorRotationWritten=0 impactValidation=USER_REQUIRED",
            path,owner.index+1,n,result.pitch,result.yaw);g_log(line);}
}
Rotator* Rotation(void* weapon,Rotator* out,const void* input,uintptr_t caller){
    auto* result=g_original?g_original(weapon,out,input):out;++g_calls;
    if(!result||caller!=0x7CFA3B||!native_tps::OnGameThread())return result;
    void* actor=nullptr;local_players::Context owner{};
    if(!Read(weapon,0xE8,actor)||!Owner(weapon,actor,owner))return result;
    Rotator adjusted=*result;
    if(AimAt(owner,adjusted)){*result=adjusted;Observe("PISTOL",owner,++g_adjusted[owner.index],adjusted);}
    return result;
}
Rotator* HookRotation(void* weapon,Rotator* out,const void* input){
    return Rotation(weapon,out,input,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
Rotator* Finish(void* ability,Rotator* out){
    auto* result=g_finish?g_finish(ability,out):out;
    if(!result||!native_tps::OnGameThread())return result;
    local_players::Context owner{};
    if(!AbilityOwner(ability,0x3811260,owner))return result;
    Rotator adjusted=*result;
    if(AimAt(owner,adjusted)){*result=adjusted;Observe("PROJECTILE_FINISH",owner,++g_finished[owner.index],adjusted);}
    return result;
}
// The actual Lava preparation uses these native formulas and its own speed,
// world gravity*ProjectileGravityScale and angle bounds. Re-evaluate those
// against the camera hit. Sky/no-hit retains native free-aim speed/arc lift.
bool SolveLava(void* ability,const local_players::Context& owner,const Vec3& target,bool hit){
    void* projectileClass=nullptr;void* defaults=nullptr;void* vtable=nullptr;
    void* instigator=nullptr;void* root=nullptr;uintptr_t find=0;
    Vec3 origin{};float low=0,high=0,initial=0,scale=0;
    if(!g_movementClass||!g_gravity||!g_arcAngle||!g_arcSpeed||!owner.world||
       !Read(ability,0x1E0,projectileClass)||!Read(projectileClass,0xF8,defaults)||!defaults||
       !Read(defaults,0,vtable)||!Read(vtable,0x5E0,find)||!find||
       !Read(ability,0x48,instigator)||!Typed(instigator,0x3A58AB0)||
       !Read(instigator,0x158,root)||!Read(root,0x1A0,origin)||
       !Read(ability,0x204,low)||!Read(ability,0x208,high)||
       !std::isfinite(low)||!std::isfinite(high)||low>high||low<0||high>89)return false;
    using FindComponent=void*(*)(void*,void*);
    void* movementClass=g_movementClass();
    void* movement=reinterpret_cast<FindComponent>(find)(defaults,movementClass);
    if(!TypedClass(movement,movementClass)||!Read(movement,0x138,initial)||!Read(movement,0x154,scale)||
       !std::isfinite(initial)||initial<=0||initial>1e7f||!std::isfinite(scale)||scale<0||scale>100)return false;
    const float gravity=g_gravity(reinterpret_cast<void*>(owner.world))*scale;
    if(!std::isfinite(gravity)||gravity>0||gravity < -1e6f)return false;
    Rotator rotation{};
    if(!Read(ability,0x20C,rotation)||!std::isfinite(rotation.roll)||!Direction(origin,target,rotation))return false;
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
    Observe(hit?"LAVA_HIT_ARC":"LAVA_FREE_ARC",owner,++g_lobbed[owner.index],rotation);
    return true;
}
void UpdateLava(void* ability){
    if(g_lava)g_lava(ability); // native setup, CDO and charge execute first
    if(!native_tps::OnGameThread())return;
    local_players::Context owner{};Vec3 target{};bool hit=false;
    if(!AbilityOwner(ability,0x3811050,owner)||!Target(owner,target,hit))return;
    if(!SolveLava(ability,owner,target,hit)){
        const auto n=++g_lavaRejected;
        if(g_log&&n<=6)g_log("TPS V1.50 LAVA solve rejected; native paired rotation/speed retained");
    }
}
}
void Initialize(LogFn log,bool exactExecutable,RayProvider provider){
    g_log=log;g_provider=provider;if(!exactExecutable||!provider)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x6A9F20,{0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x50,0x48}},
        {0x59D280,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x40,0x80,0xB9,0xA4,0x01,0x00,0x00}},
        {0x5B7DC0,{0x48,0x89,0x5C,0x24,0x18,0x55,0x48,0x8B,0xEC,0x48,0x83,0xEC,0x60,0x80,0xB9,0xA4}},
        {0x66A3B0,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x70,0x48,0x8B,0xFA,0x48,0x8B,0xD9}},
        {0x1DCEE00,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xAA,0x63,0xC9,0x01,0x48,0x85}},
        {0x1C9BCB0,{0x48,0x83,0xEC,0x28,0x48,0x8B,0x49,0x30,0x48,0x85,0xC9,0x74,0x1D,0xB2,0x01,0xE8}},
        {0x6647A0,{0x48,0x83,0xEC,0x28,0x0F,0x57,0xDB,0x0F,0x2F,0xCB,0x73,0x3E,0xF3,0x0F,0x59,0xC0}},
        {0x664800,{0x48,0x83,0xEC,0x68,0x44,0x0F,0x29,0x4C,0x24,0x20,0x0F,0x57,0xC0,0x44,0x0F,0x28}},
        {0x7C1CE0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x72,0xF5,0x04,0x03,0x48,0x85}},
        {0x7BABA0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xA2,0x64,0x05,0x03,0x48,0x85}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("TPS V1.50 projectile signatures mismatch; native retained");return;
    }
    g_origin=reinterpret_cast<AimOrigin>(g_base+0x66A3B0);
    g_movementClass=reinterpret_cast<ClassGetter>(g_base+0x1DCEE00);
    g_gravity=reinterpret_cast<Gravity>(g_base+0x1C9BCB0);
    g_arcAngle=reinterpret_cast<ArcAngle>(g_base+0x6647A0);
    g_arcSpeed=reinterpret_cast<ArcSpeed>(g_base+0x664800);
    struct Hook {uintptr_t rva;void* detour;void** original;};
    const Hook hooks[]={
        {0x6A9F20,reinterpret_cast<void*>(&HookRotation),reinterpret_cast<void**>(&g_original)},
        {0x59D280,reinterpret_cast<void*>(&Finish),reinterpret_cast<void**>(&g_finish)},
        {0x5B7DC0,reinterpret_cast<void*>(&UpdateLava),reinterpret_cast<void**>(&g_lava)}
    };
    const auto status=MH_Initialize();if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED)return;
    unsigned created=0;bool ok=true;
    for(const auto& h:hooks){if(MH_CreateHook(reinterpret_cast<void*>(g_base+h.rva),h.detour,h.original)!=MH_OK){ok=false;break;}++created;}
    if(ok)for(const auto& h:hooks)if(MH_EnableHook(reinterpret_cast<void*>(g_base+h.rva))!=MH_OK){ok=false;break;}
    if(!ok){for(unsigned i=0;i<created;++i){MH_DisableHook(reinterpret_cast<void*>(g_base+hooks[i].rva));MH_RemoveHook(reinterpret_cast<void*>(g_base+hooks[i].rva));}
        g_original=nullptr;g_finish=nullptr;g_lava=nullptr;
        if(log)log("TPS V1.50 projectile hook transaction failed; native retained");return;}
    if(log)log("TPS V1.50 TARGET3D READY: local Strife pistols + native projectile Finish + Lava paired arc; P1/P2, native collision, release independent; EXPERIMENTAL");
}
void Report(){
    if(!g_log)return;char line[260]{};
    sprintf_s(line,"TPS V1.50 TARGET3D totals native=%u pistolP1=%u P2=%u finishP1=%u P2=%u lavaP1=%u P2=%u lavaRejected=%u impactValidation=USER_REQUIRED",
        g_calls.load(),g_adjusted[0].load(),g_adjusted[1].load(),g_finished[0].load(),g_finished[1].load(),g_lobbed[0].load(),g_lobbed[1].load(),g_lavaRejected.load());g_log(line);
}
}
