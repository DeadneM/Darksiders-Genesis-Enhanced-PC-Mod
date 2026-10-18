#include "InteractionHudFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "NativeMemoryReader.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::interaction_hud {
namespace {
struct Scale {float x=1,y=1,z=1;};
struct Baseline {
    Scale original{},applied{};
    bool captured=false,modified=false;
};
native_objects::Baselines<Baseline,16> g_baselines;
using FacingFn=void(*)(void*,float);
using OwnerFn=void(*)(void*,void*);
using ScaleFn=void(*)(void*,const Scale*);
FacingFn g_facing=nullptr;
OwnerFn g_owner=nullptr;
ScaleFn g_scale=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
unsigned g_ownerChanges=0,g_scaleChanges=0;
template<class T> bool Read(const void* object,size_t offset,T& value){
    native_objects::MemoryReader reader;
    return reader.Read(reinterpret_cast<uintptr_t>(object),offset,value);
}
bool Close(Scale a,Scale b){return std::fabs(a.x-b.x)<.00001f&&std::fabs(a.y-b.y)<.00001f&&std::fabs(a.z-b.z)<.00001f;}
bool Valid(Scale s){return std::isfinite(s.x)&&std::isfinite(s.y)&&std::isfinite(s.z)&&
    s.x>0&&s.y>0&&s.z>0&&s.x<=100&&s.y<=100&&s.z<=100;}
bool Resolve(void* component,local_players::Context& context){
    uintptr_t table=0;void* actor=nullptr;void* reciprocal=nullptr;void* widget=nullptr;
    // Both observed heroes inherit InteractHUD at EF8. Other native facing
    // widgets (enemy bars, finishers, shared world sprites) do not qualify.
    return g_ready.load(std::memory_order_acquire)&&native_tps::OnGameThread()&&
        Read(component,0,table)&&table==g_base+0x275B7D0&&
        Read(component,0xE8,actor)&&actor&&local_players::ResolveActor(actor,context)&&
        context.index>=0&&context.index<2&&context.character==reinterpret_cast<uintptr_t>(actor)&&
        Read(actor,0xEF8,reciprocal)&&reciprocal==component&&
        Read(component,0x610,widget)&&widget;
}
void ApplyScale(void* component,const local_players::Context& context){
    if(!g_scale)return;
    Scale current{};native_objects::Identity identity{};
    if(!Read(component,0x17C,current)||!Valid(current)||
       !native_objects::CaptureLive(g_base,component,identity))return;
    native_objects::MemoryReader reader;
    auto* saved=g_baselines.Acquire(reader,g_base,identity);
    if(!saved)return; // Never evict a live modified component.
    auto& b=*saved;
    if(!b.captured){
        b={current,current,true,false};
    }else if(!b.modified||!Close(current,b.applied)){
        // A new native scale wins. Never multiply our last output again.
        b.original=current;b.modified=false;
    }
    const auto& rt=runtime::Get();
    const float factor=rt.interactionHudScale.load();
    const bool enabled=rt.interactionHudScaleEnabled.load()&&std::isfinite(factor)&&factor>=.1f&&factor<=2;
    const Scale target=enabled?Scale{b.original.x*factor,b.original.y*factor,b.original.z*factor}:b.original;
    if(!Close(current,target)){
        g_scale(component,&target);
        if(g_log&&++g_scaleChanges<=8){char line[160]{};sprintf_s(line,
            "Interaction HUD V1.58 SCALE player=%d factor=%.3f nativeScalePreserved=1 EXPERIMENTAL",
            context.index+1,enabled?factor:1.0f);g_log(line);}
    }
    b.applied=target;b.modified=enabled&&!Close(target,b.original);
}
void HookFacing(void* component,float dt){
    local_players::Context context{};
    const bool owned=Resolve(component,context);
    if(owned&&g_owner){
        void* current=nullptr;
        if(Read(component,0x5E0,current)&&current!=reinterpret_cast<void*>(context.localPlayer)){
            // SetOwnerPlayer performs the engine's screen-registration cleanup;
            // the original facing function then reads this player's camera.
            g_owner(component,reinterpret_cast<void*>(context.localPlayer));
            if(g_log&&++g_ownerChanges<=8){char line[160]{};sprintf_s(line,
                "Interaction HUD V1.58 CAMERA_OWNER player=%d reciprocalCharacter=1 EXPERIMENTAL",context.index+1);g_log(line);}
        }
    }
    if(g_facing)g_facing(component,dt);
    if(owned)ApplyScale(component,context);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));if(!g_base)return;
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x5EA970,{0x40,0x57,0x48,0x81,0xEC,0xF0,0x05,0x00,0x00,0x48,0xC7,0x44,0x24,0x50,0xFE,0xFF}},
        {0x14EC0F0,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xFA,0x48,0x8B,0xD9}},
        {0x17B03D0,{0x48,0x8B,0xC4,0x57,0x48,0x83,0xEC,0x70,0x48,0xC7,0x40,0xB8,0xFE,0xFF,0xFF,0xFF}}
    };
    for(const auto& g:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+g.rva),g.bytes,16)){
        if(log)log("Interaction HUD V1.58 unsupported functions; native HUD retained");return;
    }
    uintptr_t setter=0;unsigned offset=0;
    if(!Read(reinterpret_cast<void*>(g_base+0x275B7D0),0x3C8,setter)||setter!=g_base+0x17B03D0)return;
    std::memcpy(&offset,reinterpret_cast<void*>(g_base+0x379E0D0+36),4);if(offset!=0x17C)return;
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x5EA970);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookFacing),reinterpret_cast<void**>(&g_facing))!=MH_OK)return;
    g_owner=reinterpret_cast<OwnerFn>(g_base+0x14EC0F0);g_scale=reinterpret_cast<ScaleFn>(setter);
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_facing=nullptr;g_owner=nullptr;g_scale=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(log)log("Interaction HUD V1.58 READY: owned interaction prompts only, per-player native camera and reversible relative scale; EXPERIMENTAL");
}
void Shutdown(){g_ready.store(false,std::memory_order_release);}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
}
