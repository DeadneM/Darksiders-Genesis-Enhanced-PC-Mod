#include "RenderSafetyFeature.h"
#include "RenderSafetyPolicy.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <cstdio>

namespace dg::render_safety {
namespace {
SetTargets g_original=nullptr;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
std::atomic_uint64_t g_avoided{0};
static_assert(std::atomic_uint64_t::is_always_lock_free);
uint64_t g_reported=0;
void HookSetTargets(void* context,const void* input){
    if(Forward(g_original,context,input,runtime::Get().molochBossCrashFixEnabled.load(std::memory_order_relaxed)))
        g_avoided.fetch_add(1,std::memory_order_relaxed);
}
bool Boundary(uintptr_t base){
    constexpr unsigned char entry[]={0x48,0x89,0x5C,0x24,0x18,0x48,0x89,0x74,0x24,0x20,0x55,0x57,0x41,0x56,0x48,0x8D};
    constexpr unsigned char uavs[]={0x48,0x63,0xBA,0x20,0x01,0,0};
    constexpr unsigned char flags[]={0x44,0x0F,0xB6,0x83,0xD8,0,0,0,0x0F,0xB6,0x93,0xD9,0,0,0};
    constexpr unsigned char fault[]={0x48,0x8B,0x83,0xC8,0,0,0,0x0F,0x10,0x40,0x20};
    return base&&!std::memcmp(reinterpret_cast<void*>(base+0xD5CDD0),entry,sizeof(entry))&&
        !std::memcmp(reinterpret_cast<void*>(base+0xD5CDF8),uavs,sizeof(uavs))&&
        !std::memcmp(reinterpret_cast<void*>(base+0xD5D082),flags,sizeof(flags))&&
        !std::memcmp(reinterpret_cast<void*>(base+0xD5D09A),fault,sizeof(fault));
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;
    if(g_ready.load())return;
    const auto base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!exactExecutable||!Boundary(base)){
        if(g_log)g_log("Moloch Boss Crash Fix V1.151: unavailable; unsupported executable or RHI boundary");
        return;
    }
    auto* target=reinterpret_cast<void*>(base+0xD5CDD0);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookSetTargets),reinterpret_cast<void**>(&g_original))!=MH_OK){
        if(g_log)g_log("Moloch Boss Crash Fix V1.151: hook creation failed; native path retained");
        return;
    }
    if(MH_EnableHook(target)!=MH_OK){
        MH_RemoveHook(target);g_original=nullptr;
        if(g_log)g_log("Moloch Boss Crash Fix V1.151: hook enable failed; native path retained");
        return;
    }
    g_ready.store(true,std::memory_order_release);
    if(g_log)g_log("Moloch Boss Crash Fix V1.151: ready; guard for captured stencil clear without depth target, toggle in Gameplay / System and startup");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
uint64_t AvoidedClears(){return g_avoided.load(std::memory_order_relaxed);}
void Report(){
    const auto count=AvoidedClears();
    if(count==g_reported)return;
    const bool report=g_reported==0||count<8||count/256!=g_reported/256;
    g_reported=count;
    if(report&&g_log){
        char line[192]{};
        sprintf_s(line,"Moloch Boss Crash Fix V1.151: skipped absent-target stencil clear; total=%llu (valid targets unchanged)",static_cast<unsigned long long>(count));
        g_log(line);
    }
}
}
