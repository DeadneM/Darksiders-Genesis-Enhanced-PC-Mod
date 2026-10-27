#include "CaltropsAppearanceFeature.h"
#include "CaltropsAppearancePolicy.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <array>
#include <atomic>
#include <cstdio>

namespace dg::caltrops_appearance {
namespace {
using Id=native_objects::Identity;
// Exact retail common PSC factory. Native location (both overloads) and
// attached emitters call it with Template, UWorld, optional Outer, auto/pool.
using Create=void*(*)(void*,void*,void*,bool,unsigned char);
Create g_create=nullptr;uintptr_t g_base=0;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};std::atomic_uint g_loaded{0};
struct Asset {const char* name;const char* package;};
constexpr Asset Assets[]={
 {"P_Strife_Caltrop_Glyph_Init_1B","/Game/VFX/Particles/Characters/Strife/Caltrops/Strife_Caltrop_1B/P_Strife_Caltrop_Glyph_Init_1B"},
 {"P_Strife_Caltrop_Glyph_Init_Frost_01","/Game/VFX/Particles/Characters/Strife/Caltrops/Caltrop_Frost/Level_1/P_Strife_Caltrop_Glyph_Init_Frost_01"},
 {"P_Strife_Caltrop_Trigger_1B","/Game/VFX/Particles/Characters/Strife/Caltrops/Strife_Caltrop_1B/P_Strife_Caltrop_Trigger_1B"},
 {"P_Strife_Caltrop_Trigger_Frost_Lvl1","/Game/VFX/Particles/Characters/Strife/Caltrops/Caltrop_Frost/Level_1/P_Strife_Caltrop_Trigger_Frost_Lvl1"},
 {"P_Strife_Caltrop_Trigger_Frost_Lvl2","/Game/VFX/Particles/Characters/Strife/Caltrops/Caltrop_Frost/Level_2/P_Strife_Caltrop_Trigger_Frost_Lvl2"},
 {"P_Strife_Caltrop_Trigger_Frost_Lvl3","/Game/VFX/Particles/Characters/Strife/Caltrops/Caltrop_Frost/Level_3/P_Strife_Caltrop_Trigger_Frost_Lvl3"}
};
std::array<Id,6> g_assets{};
struct World {Id id{};uint64_t tick=0;};
std::array<World,2> g_worlds{};
int g_selection=0,g_cursor=0,g_count=0;bool g_complete=false;unsigned g_changes=0;uint64_t g_scanTick=0;
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Capture(uintptr_t p,Id& id){return native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),id);}
int Classify(uintptr_t p){
    uintptr_t cls=0,package=0;int index=0,number=0,pi=0,pn=0;
    if(!Read(p,0x10,cls)||!native_objects::NameIs(g_base,cls,"ParticleSystem")||!Read(p,0x20,package)||
       !Read(p,0x18,index)||!Read(p,0x1C,number)||!Read(package,0x18,pi)||!Read(package,0x1C,pn))return -1;
    for(unsigned i=0;i<g_assets.size();++i)if(native_objects::NameValueEquals(g_base,index,number,Assets[i].name)&&
       native_objects::NameValueEquals(g_base,pi,pn,Assets[i].package))return int(i);
    return -1;
}
void Scan(uint64_t now){
    bool invalid=false;unsigned ready=0,available=0;
    for(unsigned i=0;i<g_assets.size();++i){auto& id=g_assets[i];if(Alive(id))available|=1u<<i;else if(id){id={};invalid=true;}}
    if(invalid){g_cursor=0;g_complete=false;}
    if(available!=63&&now>=g_scanTick&&now-g_scanTick>=16){
        g_scanTick=now;uintptr_t chunks=0;int count=0;
        if(Read(g_base,0x3959680,chunks)&&Read(g_base,0x3959694,count)&&count>0&&count<=2000000){
            if(count<g_count)g_cursor=0;
            if(count!=g_count){g_count=count;g_complete=false;}
            // A bounded loaded-object pass only after an explicit non-Native
            // selection. New appended objects are visited, no synchronous load.
            if(!g_complete)for(unsigned n=0;n<512&&g_cursor<count;++n,++g_cursor){
                uintptr_t chunk=0,p=0;unsigned flags=0;
                if(!Read(chunks,size_t(g_cursor/65536)*8,chunk)||!Read(chunk,size_t(g_cursor%65536)*24,p)||
                   !Read(chunk,size_t(g_cursor%65536)*24+8,flags)||!p||(flags&0x30000000u))continue;
                const int k=Classify(p);if(k>=0&&!(available&(1u<<k))&&Capture(p,g_assets[k]))available|=1u<<k;
            }
            if(g_cursor>=count)g_complete=true;
        }
    }
    for(int choice=1;choice<=4;++choice)if((available&(1u<<Target(choice,0)))&&(available&(1u<<Target(choice,2))))ready|=1u<<choice;
    g_loaded.store(ready,std::memory_order_relaxed);
}
void* Resolve(void* asset,void* world){
    const int choice=Normalize(runtime::Get().caltropsAppearance.load(std::memory_order_relaxed));
    if(!choice||!g_ready.load(std::memory_order_relaxed)||!asset||!world||!native_tps::OnGameThread())return asset;
    // All Caltrops in the current local world share this cosmetic choice.
    // No unsafe actor/instigator guess after the engine resolved its UWorld.
    const auto now=GetTickCount64();bool current=false;
    for(const auto& w:g_worlds)if(w.id.address==reinterpret_cast<uintptr_t>(world)&&w.tick&&now>=w.tick&&now-w.tick<=500&&Alive(w.id)){current=true;break;}
    if(!current)return asset;
    int source=-1;for(unsigned i=0;i<g_assets.size();++i)if(g_assets[i].address==reinterpret_cast<uintptr_t>(asset)&&Alive(g_assets[i])){source=int(i);break;}
    if(source<0)return asset;const int target=Target(choice,source);
    if(target==source||!Alive(g_assets[target]))return asset;
    if(g_log&&g_changes++<8){char line[150]{};sprintf_s(line,"Caltrops appearance V1.115: role=%s choice=%d native emitter only; gameplay unchanged; EXPERIMENTAL",source<2?"initial glyph":"explosion",choice);g_log(line);}
    return reinterpret_cast<void*>(g_assets[target].address);
}
void* HookCreate(void* asset,void* world,void* outer,bool autoDestroy,unsigned char pooling){
    return g_create?g_create(Resolve(asset,world),world,outer,autoDestroy,pooling):nullptr;
}
}
void Initialize(LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char signature[]={0x48,0x89,0x5C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x60,0x41};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x187E600),signature,sizeof(signature)))return;
    auto* entry=reinterpret_cast<void*>(g_base+0x187E600);
    if(MH_CreateHook(entry,reinterpret_cast<void*>(&HookCreate),reinterpret_cast<void**>(&g_create))!=MH_OK)return;
    if(MH_EnableHook(entry)!=MH_OK){MH_RemoveHook(entry);g_create=nullptr;return;}
    g_ready=true;if(log)log("Caltrops appearance V1.115 READY: initial glyph/explosion templates only; Native default, no startup asset scan; loaded assets only; EXPERIMENTAL");
}
void OnLivePlayer(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    const int choice=Normalize(runtime::Get().caltropsAppearance.load());
    if(!choice){g_worlds={};g_selection=0;return;}
    auto& w=g_worlds[c.index];uintptr_t cls=0;
    if(!c.localPlayer||!c.controller||!c.pawn||!c.world||!Read(c.world,0x10,cls)||!native_objects::NameIs(g_base,cls,"World")){w={};return;}
    if(g_selection!=choice||w.id.address!=c.world||!Alive(w.id)){
        if(!Capture(c.world,w.id)){w={};return;}g_cursor=0;g_count=0;g_complete=false;g_selection=choice;
    }
    w.tick=GetTickCount64();Scan(w.tick);
}
bool Ready(){return g_ready.load(std::memory_order_relaxed);}
unsigned LoadedMask(){return g_loaded.load(std::memory_order_relaxed);}
void Shutdown(){g_ready=false;}
}
