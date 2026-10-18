#include "GrappleRangeFeature.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "LocalPlayerOwnership.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace dg::grapple {
namespace {
using InitFn=void(*)(void*);
InitFn g_target=nullptr,g_pull=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
unsigned g_changes=0;
template<class T> bool Read(void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
bool Eligible(void* ability,uintptr_t table,local_players::Context& owner){
    uintptr_t vt=0;void* actor=nullptr;uint32_t flags=0;unsigned char role=0;
    // Both playable Blueprint subclasses use these native method tables in
    // the supported executable. CDOs, foreign actors and other tools pass through.
    return g_ready.load()&&native_tps::OnGameThread()&&
        Read(ability,0,vt)&&vt==g_base+table&&Read(ability,8,flags)&&!(flags&0x30u)&&
        Read(ability,0x48,actor)&&actor&&Read(actor,0x118,role)&&role==3&&
        local_players::ResolveActor(actor,owner)&&owner.kind==local_players::PoseKind::Foot&&
        owner.character==reinterpret_cast<uintptr_t>(actor)&&!native_tps::CinematicActive(unsigned(owner.index))&&
        !native_tps::IsWorldPaused(actor);
}
float Factor(){
    const auto& rt=runtime::Get();const float factor=rt.grappleRangeMultiplier.load();
    return rt.grappleRangeEnabled.load()&&std::isfinite(factor)?std::clamp(factor,1.0f,10.0f):1.0f;
}
void Apply(void* ability,InitFn original,uintptr_t table,const size_t* offsets,unsigned count){
    if(!original)return;
    local_players::Context owner{};const float factor=Factor();float before[2]{},after[2]{};
    bool change=factor!=1&&Eligible(ability,table,owner);
    for(unsigned i=0;change&&i<count;++i){
        change=Read(ability,offsets[i],before[i])&&std::isfinite(before[i])&&before[i]>0&&before[i]<=100000;
        after[i]=before[i]*factor;
    }
    if(!change){original(ability);return;}
    for(unsigned i=0;i<count;++i)std::memcpy(static_cast<unsigned char*>(ability)+offsets[i],&after[i],4);
    // Native initialization copies the two search radii into owned finders,
    // and computes the pull endpoint from its distance. No CDO is changed.
    original(ability);
    for(unsigned i=0;i<count;++i){
        float current=0;
        if(Read(ability,offsets[i],current)&&current==after[i])
            std::memcpy(static_cast<unsigned char*>(ability)+offsets[i],&before[i],4);
    }
    if(g_log&&++g_changes<=12){char line[180]{};
        sprintf_s(line,"Grapple V1.62 INIT player=%d nativeTable=%llX factor=%.2f scopedFields=%u EXPERIMENTAL",
            owner.index+1,static_cast<unsigned long long>(table),factor,count);g_log(line);}
}
void HookTarget(void* ability){constexpr size_t offsets[]{0x1E0,0x220};Apply(ability,g_target,0x27A32B0,offsets,2);}
void HookPull(void* ability){constexpr size_t offsets[]{0x240};Apply(ability,g_pull,0x27A2BA0,offsets,1);}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]{
        {0x5A3C40,{0x48,0x89,0x5C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x50,0x48}},
        {0x5A3A10,{0x48,0x89,0x5C,0x24,0x18,0x57,0x48,0x81,0xEC,0x90,0x00,0x00,0x00,0x48,0x8B,0xF9}},
        {0x6AD9DF,{0x48,0x89,0x51,0x58,0x49,0x8D,0x58,0x20,0x41,0x8B,0x00,0x48,0x8B,0xF1,0x89,0x41}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16))return;
    uintptr_t target=0,pull=0;Read(reinterpret_cast<void*>(g_base+0x27A32B0),0x248,target);Read(reinterpret_cast<void*>(g_base+0x27A2BA0),0x248,pull);
    if(target!=g_base+0x5A3C40||pull!=g_base+0x5A3A10)return;
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* a=reinterpret_cast<void*>(target);void* b=reinterpret_cast<void*>(pull);
    const bool first=MH_CreateHook(a,reinterpret_cast<void*>(&HookTarget),reinterpret_cast<void**>(&g_target))==MH_OK;
    const bool second=first&&MH_CreateHook(b,reinterpret_cast<void*>(&HookPull),reinterpret_cast<void**>(&g_pull))==MH_OK;
    if(second&&MH_EnableHook(a)==MH_OK&&MH_EnableHook(b)==MH_OK){
        g_ready.store(true,std::memory_order_release);
        if(log)log("Grapple V1.62 READY: local native acquisition/relay/pull initialization; settings apply on next use; EXPERIMENTAL");return;
    }
    if(first){MH_DisableHook(a);MH_RemoveHook(a);}if(second){MH_DisableHook(b);MH_RemoveHook(b);}g_target=nullptr;g_pull=nullptr;
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
}
