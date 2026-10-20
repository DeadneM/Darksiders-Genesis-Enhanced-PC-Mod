#include "UltimateDurationFeature.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "LocalPlayerOwnership.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <intrin.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
namespace dg::ultimate_duration {
namespace {
// 64FD70 computes the native decay (dt, perks and form ability scale), then
// tail-jumps to 6470E0. Its caller is 68A4EC, inside active-form player tick.
// 6470E0 returns the native setter's FLOAT in XMM0, including Unlimited handling.
using ConsumeFn=float(*)(void*,float);
ConsumeFn g_original=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};unsigned g_changes=0;
template<class T> bool Read(void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
float Consume(void* component,float amount,uintptr_t caller){
    if(!g_original)return 0;
    float effective=amount;
    const auto& rt=runtime::Get();const float duration=rt.ultimateDurationMultiplier.load();
    const bool infinite=rt.ultimateInfiniteDurationEnabled.load();
    local_players::Context owner{};void* actor=nullptr;void* reciprocal=nullptr;void* form=nullptr;void* resource=nullptr;
    unsigned char role=0;float before=0,maximum=0;
    if(g_ready.load()&&caller==0x68A4EC&&(infinite||rt.ultimateDecreaseEnabled.load())&&native_tps::OnGameThread()&&
       std::isfinite(amount)&&amount>0&&(infinite||(std::isfinite(duration)&&duration>=0&&duration<=10&&duration!=1))&&
       Read(component,0xE8,actor)&&actor&&Read(actor,0xCC0,reciprocal)&&reciprocal==component&&
       Read(actor,0xE78,form)&&form&&Read(actor,0xEA0,resource)&&resource&&
       Read(actor,0x118,role)&&role==3&&Read(resource,0x118,role)&&role==3&&
       local_players::ResolveActor(actor,owner)&&owner.index>=0&&owner.index<2&&
       owner.character==reinterpret_cast<uintptr_t>(actor)&&!native_tps::CinematicActive(unsigned(owner.index))&&
       !native_tps::IsWorldPaused(actor)&&Read(resource,0x3A8,before)&&Read(resource,0x3AC,maximum)&&
       std::isfinite(before)&&std::isfinite(maximum)&&before>0&&before<=maximum&&maximum<=100000){
        // 0 ends at the next positive native decay tick; it never means free form.
        // Infinite duration freezes decay only: no refill, grant or activation bypass.
        effective=infinite?0.0f:duration==0?before:float(std::min(double(before),double(amount)/duration));
    }
    const float result=g_original(component,effective);
    if(effective!=amount&&g_log&&++g_changes<=12){char line[180]{};
        sprintf_s(line,"Ultimate duration V1.68 DECAY player=%d native=%.5f effective=%.5f duration=%.3fx infinite=%d return=%.5f EXPERIMENTAL",
            owner.index+1,amount,effective,duration,infinite?1:0,result);g_log(line);
    }
    return result;
}
float HookConsume(void* component,float amount){return Consume(component,amount,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]{
        {0x6470E0,{0x40,0x53,0x48,0x83,0xEC,0x30,0x48,0x8B,0x99,0xE8,0x00,0x00,0x00,0x0F,0x29,0x74}},
        {0x64FE80,{0xE9,0x5B,0x72,0xFF,0xFF,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC}},
        {0x68A4DD,{0x0F,0x28,0xCE,0x48,0x8B,0x8F,0xC0,0x0C,0x00,0x00,0xE8,0x84,0x58,0xFC,0xFF,0x48}},
        {0x64FE4F,{0xF3,0x0F,0x59,0xB7,0x04,0x01,0x00,0x00,0x48,0x8B,0xCF,0xF3,0x0F,0x59,0xF7,0xF3}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16))return;
    const auto status=MH_Initialize();if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x6470E0);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookConsume),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(log)log("Ultimate duration V1.68 READY: multiplier or infinite active-form duration; no activation/gain/refill change; native float return and Unlimited preserved; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
}
