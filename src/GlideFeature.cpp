#include "GlideFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dg::glide {
namespace {
// Exact native PlayerCharacterMovement::TickComponent(float,ELevelTick,
// FActorComponentTickFunction*). Only the elapsed glide-budget increment
// at 6BDD72 is compensated. Native input, physics dt and landing run normally.
using Tick=void(*)(void*,float,int,void*);
Tick g_original=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_held{0};
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
template<class T> bool Read(void* obj,size_t offset,T& value){
    if(!obj||reinterpret_cast<uintptr_t>(obj)>UINTPTR_MAX-offset)return false;
    const void* p=static_cast<unsigned char*>(obj)+offset;
    if(!Readable(p,sizeof(value)))return false;
    std::memcpy(&value,p,sizeof(value));return true;
}
bool Movement(void* component){
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;void* item=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+0x380FFF0),0,expected)&&expected&&
        Read(component,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
void HookTick(void* component,float dt,int tickType,void* tickFunction){
    if(g_ready.load()&&runtime::Get().unlimitedGlideEnabled.load()&&native_tps::OnGameThread()&&
       std::isfinite(dt)&&dt>=0&&Movement(component)){
        void* actor=nullptr;unsigned char flags=0;float elapsed=0;
        local_players::Context owner{};
        if(Read(component,0x850,actor)&&local_players::ResolveActor(actor,owner)&&
           owner.kind==local_players::PoseKind::Foot&&
           Read(actor,0xD14,flags)&&(flags&1)&&Read(component,0x9F0,elapsed)&&
           std::isfinite(elapsed)&&elapsed>=0&&elapsed<1e9f){
            // 6BDD72 adds exactly this native dt once before state checks.
            // Hold the consumed budget, without changing the configured limit.
            const float held=std::max(0.0f,elapsed-dt);
            std::memcpy(static_cast<unsigned char*>(component)+0x9F0,&held,sizeof(held));
            ++g_held;
        }
    }
    if(g_original)g_original(component,dt,tickType,tickFunction);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const unsigned char tick[]{0x48,0x89,0x5C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x40,0x48};
    const unsigned char getter[]{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x22,0xF7,0x05,0x03,0x48,0x85};
    const unsigned char increment[]{0xF3,0x0F,0x58,0x83,0xF0,0x09,0x00,0x00};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x6BDC70),tick,sizeof(tick))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x7B08C0),getter,sizeof(getter))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x6BDD72),increment,sizeof(increment))){
        if(log)log("Glide V1.46 signatures mismatch; native retained");return;
    }
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x6BDC70);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookTick),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(log)log("Glide V1.46 READY: active local glide budget only; native physics/input/landing and OFF preserved; EXPERIMENTAL");
}
bool Ready(){return g_ready.load();}
void Report(){if(g_log){char line[140]{};sprintf_s(line,"Glide V1.46 totals heldBudgetTicks=%u gameplayValidation=USER_REQUIRED",g_held.load());g_log(line);}}
}
