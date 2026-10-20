#include "ManualAimFeature.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <atomic>
#include <cstdio>

namespace dg::manual_aim {
namespace {
constexpr unsigned char Aim=31,ReleaseAim=32;
using DispatchFn=void(*)(void*,unsigned char);
using QueryFn=unsigned char(*)(void*,unsigned char);
DispatchFn g_dispatch=nullptr;QueryFn g_query=nullptr;
uintptr_t g_base=0;LogFn g_log=nullptr;std::atomic_bool g_ready{false};
struct Source {uintptr_t character=0;uint32_t generation=0;int controller=-1;bool held=false;uint64_t tick=0;};
Source g_sources[2]{};SRWLOCK g_sourceLock=SRWLOCK_INIT;
struct State {
    uintptr_t pawn=0,character=0,controller=0;uint32_t generation=0;
    native_objects::Identity identity{};
    bool seen=false,armed=true,owned=false,nativeHeld=false,pendingRelease=false;
    uint64_t retryAfter=0;
};
State g_states[2]{}; // game thread only
unsigned g_started=0,g_released=0;bool g_servicing=false;

template<class T> bool Read(uintptr_t object,size_t offset,T& value){
    native_objects::MemoryReader reader;return reader.Read(object,offset,value);
}
Source Snapshot(unsigned index){
    AcquireSRWLockShared(&g_sourceLock);const auto value=g_sources[index];
    ReleaseSRWLockShared(&g_sourceLock);return value;
}
bool Wanted(const Source& source,const State& state,int controller,uint64_t now){
    return source.held&&source.character==state.character&&source.generation==state.generation&&
        source.controller==controller&&source.tick&&now>=source.tick&&now-source.tick<=500;
}
bool Gameplay(uintptr_t pawn,unsigned index){
    return !native_tps::IsWorldPaused(reinterpret_cast<void*>(pawn))&&
        !native_tps::CinematicActive(index)&&!native_tps::RadialActive(index);
}
bool Boundary(uintptr_t pawn,uintptr_t controller){
    uintptr_t actual=0,table=0,event=0,query=0;unsigned char role=0;
    return pawn&&controller&&Read(pawn,0x118,role)&&role==3&&
        Read(pawn,0x348,actual)&&actual==controller&&
        Read(controller,0x350,actual)&&actual==pawn&&
        Read(pawn,0x8D0,table)&&Read(table,0x10,event)&&event==g_base+0x681D50&&
        Read(table,0x18,query)&&query==g_base+0x669D30;
}
bool Held(uintptr_t pawn){return g_query(reinterpret_cast<void*>(pawn+0x8D0),Aim)!=0;}
void Active(unsigned index,bool value){runtime::Get().manualAimHeld[index].store(value,std::memory_order_release);}
void Reset(unsigned index,State& state,bool armed){
    const auto pawn=state.pawn,character=state.character,controller=state.controller;
    const auto generation=state.generation;state={};state.seen=true;state.armed=armed;
    state.pawn=pawn;state.character=character;state.controller=controller;state.generation=generation;
    Active(index,false);
}
void HookDispatch(void* iface,unsigned char action){
    if(g_ready.load(std::memory_order_acquire)&&(action==Aim||action==ReleaseAim)&&native_tps::OnGameThread()){
        const auto address=reinterpret_cast<uintptr_t>(iface);
        native_objects::MemoryReader reader;
        for(unsigned index=0;index<2;++index){
            auto& state=g_states[index];
            if((!state.owned&&!state.pendingRelease)||address!=state.pawn+0x8D0||
               !local_players::Matches(index,state.character,state.generation)||
               !native_objects::Alive(reader,g_base,state.identity)||!Boundary(state.pawn,state.controller))continue;
            // Remember the native source separately. Our generated events
            // use the original trampoline and never enter this hook.
            state.nativeHeld=action==Aim;
            if(state.nativeHeld)state.pendingRelease=false;
            if(action==ReleaseAim&&state.owned&&
               Wanted(Snapshot(index),state,local_players::PublishedSlot(index).controllerId,GetTickCount64())&&
               Gameplay(state.pawn,index))return;
            break;
        }
    }
    if(g_dispatch)g_dispatch(iface,action);
}
void Service(const local_players::Context& owner,uint32_t generation,uint64_t now){
    if(!g_ready.load(std::memory_order_acquire)||!g_dispatch||!g_query||!native_tps::OnGameThread()||
       owner.index<0||owner.index>1||!owner.localPlayer||!owner.pawn||
       !local_players::Matches(unsigned(owner.index),owner.character,generation))return;
    if(g_servicing)return;
    struct Guard {Guard(){g_servicing=true;}~Guard(){g_servicing=false;}} guard;
    const unsigned index=unsigned(owner.index);auto& state=g_states[index];
    const auto source=Snapshot(index);
    const bool changed=state.pawn!=owner.pawn||state.character!=owner.character||
        state.controller!=owner.controller||state.generation!=generation;
    if(changed){
        const bool seen=state.seen;
        state.pawn=owner.pawn;state.character=owner.character;state.controller=owner.controller;state.generation=generation;
        Reset(index,state,!seen||!Wanted(source,state,owner.controllerId,now));
    }
    const bool wanted=Wanted(source,state,owner.controllerId,now);
    if(!wanted)state.armed=true;
    if(!wanted&&!state.owned&&!state.pendingRelease)return; // OFF: no native reads or calls
    native_objects::Identity identity{};
    if(!Boundary(owner.pawn,owner.controller)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(owner.pawn),identity)){
        Reset(index,state,!wanted);return;
    }
    if(state.identity&&state.identity!=identity){Reset(index,state,!wanted);return;}
    state.identity=identity;
    bool held=Held(owner.pawn);const bool allowed=Gameplay(owner.pawn,index);
    // A native cancellation wins; never restart an ability every frame.
    if(state.owned&&!held){state.owned=false;state.nativeHeld=false;state.pendingRelease=false;state.armed=false;}
    if(state.owned&&(!wanted||!allowed)){
        state.owned=false;state.pendingRelease=held&&!state.nativeHeld;
        if(wanted)state.armed=false;
    }
    if(state.pendingRelease){
        if(!held||state.nativeHeld)state.pendingRelease=false;
        else if(allowed&&now>=state.retryAfter){
            // The native dispatch may reject input while a page owns it.
            // Keep only this release pending; do not clear raw input bytes.
            state.retryAfter=now+64;
            g_dispatch(reinterpret_cast<void*>(owner.pawn+0x8D0),ReleaseAim);
            held=Held(owner.pawn);state.pendingRelease=held;
            if(!held&&g_log&&++g_released<=8)g_log("Manual aim V1.74: native AIM_RELEASE accepted; fire actions untouched; EXPERIMENTAL");
        }
    }
    if(wanted&&allowed&&state.armed&&!state.owned&&!state.pendingRelease){
        state.armed=false;state.nativeHeld=held;
        if(!held)g_dispatch(reinterpret_cast<void*>(owner.pawn+0x8D0),Aim);
        state.owned=Held(owner.pawn);
        if(g_log&&++g_started<=8){char line[180]{};sprintf_s(line,
            "Manual aim V1.74: player=%u nativeAIM=%d noFireActionGenerated=1 EXPERIMENTAL",index+1,state.owned?1:0);g_log(line);}
    }
    if(wanted&&!allowed)state.armed=false;
    Active(index,state.owned);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char eventSig[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x20,0x48};
    constexpr unsigned char querySig[]={0x48,0x81,0xC1,0x40,0x05,0x00,0x00};
    if(!g_base||std::memcmp(reinterpret_cast<void*>(g_base+0x681D50),eventSig,sizeof(eventSig))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x669D30),querySig,sizeof(querySig)))return;
    auto* target=reinterpret_cast<void*>(g_base+0x681D50);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookDispatch),reinterpret_cast<void**>(&g_dispatch))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_dispatch=nullptr;return;}
    g_query=reinterpret_cast<QueryFn>(g_base+0x669D30);g_ready=true;
    if(log)log("Manual aim V1.74 READY: per-player native AIM/AIM_RELEASE source; configurable controller hold; no keyboard or XInput injection; EXPERIMENTAL");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void Publish(unsigned index,uintptr_t character,uint32_t generation,int controllerId,bool held,uint64_t tick){
    if(index>1)return;AcquireSRWLockExclusive(&g_sourceLock);
    g_sources[index]={character,generation,controllerId,held,tick};ReleaseSRWLockExclusive(&g_sourceLock);
    if(!held)Active(index,false); // direction gate expires immediately on input/focus loss
}
void OnLivePlayer(const local_players::Context& owner,uint32_t generation){Service(owner,generation,GetTickCount64());}
void Shutdown(){g_ready=false;for(unsigned i=0;i<2;++i)Active(i,false);}
}
