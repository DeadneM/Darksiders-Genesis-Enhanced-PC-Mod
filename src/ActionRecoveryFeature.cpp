#include "ActionRecoveryFeature.h"
#include "ActionRecoveryPolicy.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dg::recovery {
namespace {
using Tick=void(*)(void*,float);
using Resolve=void*(*)(void*);
using FindInstance=void*(*)(void*,void*);
using End=void(*)(void*,unsigned char);
using BlendCtor=void*(*)(void*,float);
using Stop=void(*)(void*,void*,unsigned char);
Tick g_original=nullptr;
Resolve g_weak=nullptr,g_anim=nullptr;
FindInstance g_find=nullptr;
End g_end=nullptr;
BlendCtor g_blend=nullptr;
Stop g_stop=nullptr;
LogFn g_log=nullptr;
std::atomic<void*>* g_local=nullptr;
uintptr_t g_base=0;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_ticks{0},g_locals{0},g_tails{0},g_completed{0};
TailWindow g_windows[16]{}; // game thread only, identities never dereferenced later
bool Readable(const void* p,size_t size){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&size<=m.RegionSize-offset;
}
template<class T> bool Read(void* p,size_t offset,T& value){
    const auto address=reinterpret_cast<uintptr_t>(p);
    if(!p||address>UINTPTR_MAX-offset)return false;
    const auto at=reinterpret_cast<void*>(address+offset);
    if(!Readable(at,sizeof(T)))return false;
    std::memcpy(&value,at,sizeof(T));return true;
}
bool MontageAbility(void* object){
    void* expected=nullptr;void* cls=nullptr;void* array=nullptr;void* entry=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+0x3811108),0,expected)&&expected&&
        Read(object,0x10,cls)&&cls&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(cls,0x38,last)&&last>=index&&Read(cls,0x30,array)&&array&&
        Read(array,size_t(index)*8,entry)&&entry==static_cast<unsigned char*>(expected)+0x30;
}
void HookTick(void* ability,float delta){
    ++g_ticks;
    if(g_original)g_original(ability,delta); // full native tick/notify path first
    auto& rt=dg::runtime::Get();
    if(!rt.actionRecoveryEnabled.load()||!dg::native_tps::OnGameThread()){
        if(dg::native_tps::OnGameThread())for(auto& window:g_windows)window={};
        return;
    }
    void* local=g_local?g_local->load(std::memory_order_acquire):nullptr;
    void* instigator=nullptr;void* player=nullptr;unsigned char state=0,role=0;
    if(!local||!Read(ability,0x48,instigator)||instigator!=local||
       !Read(ability,0x1B0,player)||player!=local||!MontageAbility(ability)||
       !Read(local,0x118,role)||role!=3||!Read(ability,0xD8,state))return;
    ++g_locals;
    const auto identity=reinterpret_cast<uintptr_t>(ability);
    TailWindow* window=nullptr;
    for(auto& item:g_windows)if(item.ability==identity){window=&item;break;}
    if(!window)for(auto& item:g_windows)if(!item.ability){window=&item;break;}
    if(!window)window=&g_windows[g_locals.load()%16];
    if(state!=2||dg::native_tps::CinematicActive()||dg::native_tps::IsWorldPaused(local)){
        *window={};return;
    }
    void* helper=nullptr;void* vtable=nullptr;void* montage=nullptr;
    if(!Read(ability,0x1D8,helper)||!Read(helper,0,vtable)||
       vtable!=reinterpret_cast<void*>(g_base+0x27BC3B0)||!Read(helper,0x28,montage)||!montage||
       g_weak(static_cast<unsigned char*>(helper)+0x30)!=local){*window={};return;}
    void* anim=g_anim(local);
    void* instance=anim?g_find(anim,montage):nullptr;
    float current=0,target=1,elapsed=0;unsigned char interrupted=1;
    void* instanceMontage=nullptr;
    // These are the same FAlphaBlend fields used by native IsPlaying (676620).
    // The target becomes zero only after UE has entered blend-out. Preserve all
    // normal RUNNING animation, root motion, rewards and notifies before that.
    const bool tail=Read(instance,0,instanceMontage)&&instanceMontage==montage&&
        Read(instance,0x148,current)&&Read(instance,0x150,target)&&
        Read(instance,0x160,interrupted)&&!interrupted&&std::isfinite(current)&&
        current>0&&current<=1.001f&&target==0&&Read(ability,0xDC,elapsed);
    if(!TailReady(*window,identity,reinterpret_cast<uintptr_t>(instance),rt.tpsActorGeneration.load(),
        tail,elapsed,delta,rt.actionRecoveryDelayMs.load()))return;
    const unsigned n=++g_tails;
    // FAlphaBlend is 0x28 bytes. Use the native zero-time constructor and Stop
    // with bInterrupted=false, retaining normal montage-end callback semantics.
    // Finish the ability normally so input/root-motion/interaction cleanup runs.
    alignas(16) unsigned char blend[0x30]{};
    g_blend(blend,0.0f);
    g_stop(instance,blend,0);
    if(Read(ability,0xD8,state)&&state==2)g_end(ability,0);
    Read(ability,0xD8,state);
    if(state==5||state==6)++g_completed;
    *window={};
    if(g_log&&(n<=30||n%100==0)){
        char line[320]{};
        sprintf_s(line,"Action Recovery V1.39 NATIVE_BLEND_END n=%u ability=%p montage=%p weight=%.4f stateAfter=%u delayMs=%.0f normalFinish=%d gameplay=EXPERIMENTAL",
            n,ability,montage,current,unsigned(state),rt.actionRecoveryDelayMs.load(),state==5||state==6?1:0);g_log(line);
    }
}
}
bool Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable){
    g_log=logger;g_local=localPlayer;if(!exactExecutable)return false;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x68D050,{0x40,0x53,0x48,0x83,0xEC,0x30,0x0F,0x29,0x74,0x24,0x20,0x48,0x8B,0xD9,0x0F,0x28}},
        {0x59B080,{0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48}},
        {0x1660F90,{0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8D,0x99,0x88,0x00,0x00,0x00,0x4C,0x8B,0xC2}},
        {0xAD3A50,{0x44,0x8B,0x41,0x04,0x45,0x85,0xC0,0x74,0x4E,0x8B,0x01,0x85,0xC0,0x78,0x48,0x3B}},
        {0x654180,{0x48,0x83,0xEC,0x28,0x48,0x8B,0x89,0x80,0x03,0x00,0x00,0x48,0x85,0xC9,0x74,0x41}},
        {0x169D4F0,{0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0x68,0xA1,0x48,0x81,0xEC,0x90,0x00}},
        {0x164C6A0,{0x40,0x53,0x48,0x83,0xEC,0x20,0x33,0xC0,0xF3,0x0F,0x11,0x49,0x08,0x48,0x89,0x01}},
        {0x67666E,{0xF3,0x0F,0x10,0x88,0x50,0x01,0x00,0x00,0x0F,0x57,0xC0,0x0F,0x2E,0xC8,0x75,0x09}},
        {0x7BC0A0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x5A,0x50,0x05,0x03,0x48,0x85}}
    };
    for(const auto& guard:guards)
        if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16))return false;
    // Additional Stop/constructor/weight-layout guards below are checked before
    // enabling the sole Tick hook; unknown builds retain native behavior.
    g_weak=reinterpret_cast<Resolve>(g_base+0xAD3A50);
    g_anim=reinterpret_cast<Resolve>(g_base+0x654180);
    g_find=reinterpret_cast<FindInstance>(g_base+0x1660F90);
    g_end=reinterpret_cast<End>(g_base+0x59B080);
    g_blend=reinterpret_cast<BlendCtor>(g_base+0x164C6A0);
    g_stop=reinterpret_cast<Stop>(g_base+0x169D4F0);
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return false;
    void* target=reinterpret_cast<void*>(g_base+0x68D050);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookTick),reinterpret_cast<void**>(&g_original))!=MH_OK)return false;
    const auto enabled=MH_EnableHook(target);
    if(enabled!=MH_OK&&enabled!=MH_ERROR_ENABLED){MH_RemoveHook(target);g_original=nullptr;return false;}
    g_ready.store(true);
    if(g_log)g_log("Action Recovery V1.39 READY: local authoritative player montages; natural blend-out only; native Stop(false)/EndAbility cleanup; no MOVE override; EXPERIMENTAL");
    return true;
}
Telemetry GetTelemetry(){return {g_ready.load(),g_ticks.load(),g_locals.load(),g_tails.load(),g_completed.load()};}
void Shutdown(){if(g_log){const auto t=GetTelemetry();char line[240]{};
    sprintf_s(line,"Action Recovery V1.39 totals ready=%d ticks=%u localMontages=%u blendTails=%u normalFinished=%u gameplayValidation=USER_REQUIRED",
        t.ready?1:0,t.ticks,t.local,t.tails,t.completed);g_log(line);}}
}
