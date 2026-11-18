#include "CoreUiLevelFeature.h"
#include "CoreUiRefreshPolicy.h"
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <array>
#include <atomic>
#include <cstring>

namespace dg::core_ui {
namespace {
using Id=native_objects::Identity;
std::atomic_bool g_ready{false};
uintptr_t g_base=0,g_widgetMeta=0;
uint64_t g_rootName=0;
Id g_class{},g_tick{},g_refresh{};
bool g_bound=false;
std::array<uintptr_t,8> g_otherMeta{};
struct ClassKey {uintptr_t address=0;uint64_t name=0;};
std::array<ClassKey,128> g_otherWidgets{};
native_objects::Baselines<Warmup,4> g_roots;
LogFn g_log=nullptr;

template<class T>T Header(uintptr_t object,size_t at){
    // Only synchronous live ProcessEvent arguments/classes use this fast
    // path. Deferred work validates the weak object table before access.
    T value{};std::memcpy(&value,reinterpret_cast<void*>(object+at),sizeof(value));return value;
}
bool Alive(const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
template<size_t N>bool Is(uintptr_t object,const char(&text)[N]){return native_objects::NameIs(g_base,object,text);}
bool RootClass(uintptr_t cls){
    const auto pair=Header<uint64_t>(cls,0x18);
    if(g_rootName)return pair==g_rootName;
    const auto meta=Header<uintptr_t>(cls,0x10);
    if(meta!=g_widgetMeta){
        for(auto ignored:g_otherMeta)if(ignored==meta)return false;
        if(!Is(meta,"WidgetBlueprintGeneratedClass")){
            for(auto& ignored:g_otherMeta)if(!ignored){ignored=meta;break;}
            return false;
        }
        g_widgetMeta=meta;
    }
    for(auto ignored:g_otherWidgets)if(ignored.address==cls&&ignored.name==pair)return false;
    if(!Is(cls,"CharacterPage_CreatureCoresFullscreen_C")){
        for(auto& ignored:g_otherWidgets)if(!ignored.address){ignored={cls,pair};break;}
        return false;
    }
    g_rootName=pair;return true;
}
bool Bind(uintptr_t cls){
    if(g_bound&&g_class.address==cls&&Alive(g_class)&&Alive(g_tick)&&Alive(g_refresh))return true;
    g_bound=false;g_class={};g_tick={};g_refresh={};
    native_objects::MemoryReader r;uintptr_t package=0,field=0;
    if(!r.Read(cls,0x20,package)||!Is(package,"/Game/UI/CharacterScreens/CharacterPage_CreatureCoresFullscreen")||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(cls),g_class)||!r.Read(cls,0x48,field))return false;
    for(unsigned n=0;field&&n<1024;++n){
        uintptr_t type=0,outer=0,next=0;
        if(!r.Read(field,0x10,type)||!r.Read(field,0x20,outer)||!r.Read(field,0x28,next)||next==field)return false;
        if(outer==cls&&Is(type,"Function")){
            if(Is(field,"Tick"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(field),g_tick);
            if(Is(field,"RefreshNodes")){
                unsigned short bytes=1;
                if(r.Read(field,0x9E,bytes)&&bytes==0)
                    native_objects::CaptureLive(g_base,reinterpret_cast<void*>(field),g_refresh);
            }
        }
        field=next;
    }
    g_bound=!field&&g_tick&&g_refresh;return g_bound;
}
}
void Initialize(LogFn log,bool exactExecutable,bool observerInstalled){
    if(g_ready.load(std::memory_order_acquire)||!exactExecutable||!observerInstalled)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));g_log=log;
    g_ready.store(g_base!=0,std::memory_order_release);
    if(g_log)g_log("Core UI V1.170: scoped first-display refresh observer ready; no new detour or save changes");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
Id CaptureTick(void* object,void* function){
    if(!Ready()||!runtime::Get().coreLevelDisplayFixEnabled.load(std::memory_order_relaxed)||
       !object||!function||!native_tps::OnGameThread())return {};
    const auto cls=Header<uintptr_t>(reinterpret_cast<uintptr_t>(object),0x10);
    if(!cls||!RootClass(cls)||!Bind(cls)||reinterpret_cast<uintptr_t>(function)!=g_tick.address)return {};
    Id identity{};native_objects::MemoryReader r;
    native_objects::Capture(r,g_base,reinterpret_cast<uintptr_t>(object),identity);
    return identity;
}
void AfterTick(const Id& widget,Dispatch nativeDispatch){
    if(!widget||!nativeDispatch||!Ready()||!native_tps::OnGameThread()||
       !runtime::Get().coreLevelDisplayFixEnabled.load(std::memory_order_relaxed)||
       !Alive(widget)||!Alive(g_class)||!Alive(g_refresh))return;
    native_objects::MemoryReader r;uintptr_t cls=0,owner=0;unsigned short bytes=1;
    if(!r.Read(widget.address,0x10,cls)||cls!=g_class.address||
       !r.Read(g_refresh.address,0x20,owner)||owner!=cls||
       !r.Read(g_refresh.address,0x9E,bytes)||bytes!=0)return;
    auto* warmup=g_roots.Acquire(r,g_base,widget);
    if(!warmup||!warmup->Next())return;
    nativeDispatch(reinterpret_cast<void*>(widget.address),reinterpret_cast<void*>(g_refresh.address),nullptr,nullptr);
    if(g_log)g_log("Core UI V1.170: native RefreshNodes replayed once after widget warmup; inventory untouched");
}
}
