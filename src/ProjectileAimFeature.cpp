#include "ProjectileAimFeature.h"
#include "NativeTpsFeature.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dg::projectile_aim {
namespace {
struct Rotator {float pitch,yaw,roll;};
static_assert(sizeof(Rotator)==12);
// Reflected GetProjectileSpawnRotation 7CF9B0 calls vtable+8E8 with
// RCX=component, RDX=12-byte return buffer, R8=12-byte input struct.
// 6A9F20 obtains the BASE projectile rotation from the aiming interface.
// It neither rotates the actor nor applies projectile spread/physics.
using SpawnRotation=Rotator*(*)(void*,Rotator*,const void*);
SpawnRotation g_original=nullptr;
RotationProvider g_provider=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_uint32_t g_calls{0},g_adjusted[2]{};
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
template<class T> bool Read(void* object,size_t offset,T& value){
    if(!object||reinterpret_cast<uintptr_t>(object)>UINTPTR_MAX-offset)return false;
    const void* at=static_cast<unsigned char*>(object)+offset;
    if(!Readable(at,sizeof(value)))return false;
    std::memcpy(&value,at,sizeof(value));return true;
}
bool Pistols(void* weapon){
    // Live class trees prove all ordinary/special pistol components share
    // MayhemDualPistols. Ghost Hook and Void Grenade do not inherit it.
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;void* item=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+0x380C0E0),0,expected)&&expected&&
        Read(weapon,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
Rotator* Rotation(void* weapon,Rotator* out,const void* input,uintptr_t caller){
    auto* result=g_original?g_original(weapon,out,input):out;
    ++g_calls;
    if(!result||!g_provider||caller!=0x7CFA3B||!native_tps::OnGameThread()||
       !Pistols(weapon)||!std::isfinite(result->pitch)||!std::isfinite(result->yaw)||
       !std::isfinite(result->roll))return result;
    void* actor=nullptr;local_players::Context owner{};unsigned char type=0;
    if(!Read(weapon,0xE8,actor)||!local_players::ResolveActor(actor,owner)||
       owner.index<0||owner.index>1||
       !Read(reinterpret_cast<void*>(owner.character),0xC18,type)||type!=1||
       native_tps::CinematicActive(unsigned(owner.index))||native_tps::IsWorldPaused(actor))return result;
    float pitch=0,yaw=0;
    if(!g_provider(owner,pitch,yaw)||!std::isfinite(pitch)||!std::isfinite(yaw)||
       pitch < -75||pitch > 65||std::fabs(yaw)>36000)return result;
    const float nativeYaw=result->yaw;
    result->pitch=pitch;result->yaw=std::remainder(yaw,360.0f);
    const unsigned n=++g_adjusted[owner.index];
    if(g_log&&(n<=8||n%1000==0)){
        char line[240]{};
        sprintf_s(line,"TPS V1.46 PROJECTILE_BASE player=%d n=%u weapon=%p nativeYaw=%.2f cameraYaw=%.2f pitch=%.2f device=PAD actorRotationWritten=0 impactUnmeasured=1",
            owner.index+1,n,weapon,nativeYaw,result->yaw,pitch);g_log(line);
    }
    return result;
}
Rotator* HookRotation(void* weapon,Rotator* out,const void* input){
    return Rotation(weapon,out,input,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
}
void Initialize(LogFn log,bool exactExecutable,RotationProvider provider){
    g_log=log;g_provider=provider;
    if(!exactExecutable||!provider)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x6A9F20,{0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x50,0x48}},
        {0x77F4E0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xF2,0xCB,0x08,0x03,0x48,0x85}}
    };
    const unsigned char thunk[]{0xFF,0x90,0xE8,0x08,0x00,0x00};
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("TPS V1.46 projectile signatures mismatch; native retained");return;
    }
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x7CFA35),thunk,sizeof(thunk)))return;
    const auto status=MH_Initialize();if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x6A9F20);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookRotation),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    if(log)log("TPS V1.46 projectile base READY: typed local Strife pistols, PAD Camera Forward only; mouse/native-view/gear preserved; EXPERIMENTAL");
}
void Report(){
    if(!g_log)return;char line[180]{};
    sprintf_s(line,"TPS V1.46 PROJECTILE_BASE totals native=%u correctedP1=%u correctedP2=%u impactValidation=USER_REQUIRED",
        g_calls.load(),g_adjusted[0].load(),g_adjusted[1].load());g_log(line);
}
}
