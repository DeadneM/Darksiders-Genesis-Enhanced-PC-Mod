#include "NativeTpsFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeMemoryReader.h"
#include "RadialSelectionState.h"
#include "TpsAimTarget.h"
#include <windows.h>
#include <intrin.h>
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>

namespace dg::native_tps {
namespace {
struct Vec2 {float x,y;};
struct NativeTarget {Vec3 point; unsigned char valid=0; unsigned char padding[3]{};};
static_assert(sizeof(Vec2)==8 && sizeof(Vec3)==12);
static_assert(offsetof(NativeTarget,valid)==12 && sizeof(NativeTarget)==16);
// Exact EXE: both helpers consume mouse pixels. Their discriminator is NOT
// established as a device type. All mutations require the separate provider.
using Aim2Fn=Vec2*(*)(void*,Vec2*,void*,const Vec3*,bool,void*);
using Aim3Fn=Vec3*(*)(void*,Vec3*,void*,NativeTarget*,bool,void*);
using DeprojectFn=bool(*)(void*,const Vec2*,Vec3*,Vec3*);
using MousePositionFn=bool(*)(void*,float*,float*);
using CinemaFn=void(*)(void*,bool,bool);
using PausedFn=bool(*)(void*);
using TraceFn=bool(*)(void*,void*,const Vec3*,const Vec3*,unsigned,const void*,const void*);
TraceFn g_trace=nullptr;std::atomic_bool g_contactReady{false};
Aim2Fn g_aim2=nullptr;
Aim3Fn g_aim3=nullptr;
DeprojectFn g_deproject=nullptr;
MousePositionFn g_mousePosition=nullptr;
CinemaFn g_cinema=nullptr;
std::atomic<PausedFn> g_paused{nullptr};
RayProvider g_provider=nullptr;
LogFn g_log=nullptr;
uintptr_t g_base=0;
std::atomic_bool g_ready{false},g_cinematic{false};
std::atomic_uint32_t g_foot{0},g_horse{0},g_hits{0},g_misses{0},g_rays{0};
std::atomic_uint32_t g_ultimate{0};
std::atomic_uint32_t g_native2{0},g_native3{0},g_rejected{0};
using SettingsFn=void*(*)();
using VisibleFn=bool(*)(void*);
SettingsFn g_settings=nullptr;
VisibleFn g_visible=nullptr;
std::atomic_bool g_swap{false},g_swapKnown{false};
std::atomic<DWORD> g_gameThread{0};
std::atomic_uint32_t g_radialEvents{0},g_idleMouseTargets{0};
using WidgetStateFn=void(*)(void*,unsigned char,bool);
using WidgetControllerFn=void*(*)(void*);
WidgetStateFn g_widgetState=nullptr;
WidgetControllerFn g_widgetController=nullptr;
struct RadialSample {
    native_objects::Identity identity{};
    uintptr_t local=0,character=0;
    uint32_t generation=0;
    unsigned player=2;
    RadialSelectionState selection{};
    bool active=false;
};
RadialSample g_radials[8]{};
SRWLOCK g_radialLock=SRWLOCK_INIT;
// Same native class-tree test used by the retail binary (e.g. 0x6D9AC0).
// The object and its class are live arguments, not cached UObject pointers.
bool IsInstanceOf(void* object,void* baseClass){
    if(!object||!baseClass)return false;
    auto* actual=*reinterpret_cast<unsigned char**>(
        static_cast<unsigned char*>(object)+0x10);
    if(!actual)return false;
    const auto* tree=static_cast<unsigned char*>(baseClass)+0x30;
    const int index=*reinterpret_cast<const int*>(tree+8);
    const int last=*reinterpret_cast<const int*>(actual+0x38);
    if(index<0||index>512||last<index)return false;
    auto* array=*reinterpret_cast<void* const* const*>(actual+0x30);
    return array&&array[index]==tree;
}
struct RayScope {
    void* controller;
    MouseRay ray;
    RayScope* previous;
    bool used=false,wantContact=false,contactSeen=false;native_objects::Identity contact{};
};
thread_local RayScope* g_scope=nullptr;
struct ScopedRay {
    RayScope scope;
    ScopedRay(void* c,const MouseRay& r,bool contact=false):scope{c,r,g_scope}{scope.wantContact=contact;g_scope=&scope;}
    ~ScopedRay(){g_scope=scope.previous;}
};
bool Finite(const Vec3& v){
    return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);
}
bool Context(void* actor,MouseRay& r){
    if(!g_ready.load()||!g_provider||!g_provider(actor,r))return false;
    const float n=r.direction.x*r.direction.x+r.direction.y*r.direction.y+
        r.direction.z*r.direction.z;
    return Finite(r.origin)&&Finite(r.direction)&&n>0.99f&&n<1.01f;
}
// UWorld::LineTraceSingleByChannel: seven arguments proved at 571F6D
// and 171E560. Observe only the result of OUR synchronous camera query.
// No query parameter, channel, collision result or return value is changed.
bool ObserveTrace(void* result,bool hit,uintptr_t caller){
    if(!g_scope||!g_scope->wantContact||caller!=0x571F72)return false;
    g_scope->contactSeen=true;g_scope->contact={};
    struct Weak {int index,serial;};Weak weak{};unsigned char flags=0;
    native_objects::MemoryReader reader;uintptr_t actor=0;int serial=0;uint32_t itemFlags=0;
    const auto at=reinterpret_cast<uintptr_t>(result);
    if(hit&&reader.Read(at,0,flags)&&(flags&1)&&reader.Read(at,0x68,weak)&&weak.serial>0&&
       native_objects::Item(reader,g_base,weak.index,actor,serial,itemFlags)&&serial==weak.serial)
        g_scope->contact={actor,weak.index,weak.serial};
    return true;
}
bool HookTrace(void* world,void* result,const Vec3* from,const Vec3* to,unsigned channel,const void* params,const void* response){
    const bool hit=g_trace&&g_trace(world,result,from,to,channel,params,response);
    ObserveTrace(result,hit,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);return hit;
}
bool HookDeproject(void* controller,const Vec2* pixel,Vec3* origin,Vec3* direction){
    // This is a shared UE helper. Only the exact game mouse collision-helper
    // callsite inside our synchronous, verified-local scope can be replaced.
    const uintptr_t ret=reinterpret_cast<uintptr_t>(_ReturnAddress());
    if(g_scope&&controller==g_scope->controller&&ret==g_base+0x571EB8&&
       origin&&direction){
        *origin=g_scope->ray.origin;
        *direction=g_scope->ray.direction;
        g_scope->used=true;
        ++g_rays;
        return true;
    }
    return g_deproject(controller,pixel,origin,direction);
}
bool HookMousePosition(void* controller,float* x,float* y){
    // The scoped 3D query supplies its own ray. A pad/second viewport does
    // not need a physical OS cursor inside the window to reach deprojection.
    if(g_scope&&controller==g_scope->controller&&x&&y&&
       reinterpret_cast<uintptr_t>(_ReturnAddress())==g_base+0x571E95){*x=0;*y=0;return true;}
    return g_mousePosition(controller,x,y);
}
void Observe(const MouseRay& r,const Vec3& target,bool hit,const char* path){
    const unsigned n=r.ultimate?++g_ultimate:r.mounted?++g_horse:++g_foot;
    if(hit)++g_hits;else ++g_misses;
    // Separate budgets: mounted observations cannot be exhausted on foot.
    const unsigned idle=r.fireHeld?0:++g_idleMouseTargets;
    if(g_log&&(n<=12||n==100||n==1000||(idle&&idle<=8))){
        char line[440]{};
        sprintf_s(line,"TPS V1.34 NATIVE_MOUSE_TARGET source=%s path=%s n=%u buttonHeld=%d nativeHit=%d camera=(%.1f,%.1f,%.1f) ray=(%.4f,%.4f,%.4f) target=(%.1f,%.1f,%.1f) muzzleConsumer=NATIVE projectileImpactUnmeasured=1",
            r.ultimate?"STRIFE_ULTIMATE":r.mounted?"HORSE":"FOOT",path,n,r.fireHeld?1:0,hit?1:0,
            r.origin.x,r.origin.y,r.origin.z,r.direction.x,r.direction.y,
            r.direction.z,target.x,target.y,target.z);
        g_log(line);
    }
}
Vec3 FarTarget(const MouseRay& r){
    // Native mouse helper traces 100000 UE units. Use a point AHEAD at the
    // same range for sky/no-hit, never a negative ground-plane intersection.
    return {r.origin.x+100000.0f*r.direction.x,
            r.origin.y+100000.0f*r.direction.y,
            r.origin.z+100000.0f*r.direction.z};
}
bool QueryTarget(void* controller,void* actor,const MouseRay& ray,const Vec3* source,
                 Vec3& target,bool& hit,void* extra,native_objects::Identity* contact=nullptr){
    return tps_aim::ForwardTarget(ray,source,target,hit,[&](const MouseRay& current,Vec3& point,bool& collision){
        NativeTarget fresh{};Vec3 native{};ScopedRay scope(controller,current,contact!=nullptr);
        g_aim3(controller,&native,actor,&fresh,false,extra);
        if(!scope.scope.used||(contact&&!scope.scope.contactSeen))return false;
        collision=fresh.valid&&Finite(fresh.point);point=collision?fresh.point:FarTarget(current);
        if(contact)*contact=collision?scope.scope.contact:native_objects::Identity{};
        return Finite(point);
    });
}
bool ActorSource(void* actor,Vec3& source){
    native_objects::MemoryReader reader;uintptr_t root=0;
    return reader.Read(reinterpret_cast<uintptr_t>(actor),0x158,root)&&root&&
        reader.Read(root,0x1A0,source)&&Finite(source);
}
Vec3* HookAim3(void* controller,Vec3* out,void* actor,NativeTarget* cache,
               bool debug,void* extra){
    ++g_native3;
    MouseRay ray{};
    if(!controller||!out||!cache||!Context(actor,ray)){
        ++g_rejected;
        return g_aim3(controller,out,actor,cache,debug,extra);
    }
    // A stack-local cache prevents old world/old screen targets from being
    // reused after a miss. Preserve !debug write ownership and explicitly
    // clear validity on a miss, including a rejected camera-side hit.
    Vec3 source{},target{};bool hit=false;
    if(!QueryTarget(controller,actor,ray,ActorSource(actor,source)?&source:nullptr,target,hit,extra)){
        return g_aim3(controller,out,actor,cache,debug,extra);
    }
    *out=target;
    if(!debug)*cache=NativeTarget{target,static_cast<unsigned char>(hit)};
    Observe(ray,*out,hit,"TARGET3D");
    return out;
}
Vec2* HookAim2(void* controller,Vec2* out,void* actor,const Vec3* reference,
               bool flag,void* extra){
    ++g_native2;
    MouseRay ray{};
    if(!controller||!out||!reference||!Finite(*reference)||!Context(actor,ray)){
        ++g_rejected;
        return g_aim2(controller,out,actor,reference,flag,extra);
    }
    Vec3 target{};bool hit=false;
    // Reuse the game's actual 3D collision helper with its existing world,
    // query channel and hit result. Do not call the old plane intersection.
    if(!QueryTarget(controller,actor,ray,reference,target,hit,extra))
        return g_aim2(controller,out,actor,reference,flag,extra);
    const float dx=target.x-reference->x,dy=target.y-reference->y;
    const float length=std::hypot(dx,dy);
    if(!std::isfinite(length)||length<0.01f)
        return g_aim2(controller,out,actor,reference,flag,extra);
    *out={dx/length,dy/length};
    Observe(ray,target,hit,"DIRECTION_FROM_TARGET3D");
    return out;
}
void HookCinema(void* instance,bool enabled,bool allPlayers){
    // Reflected ForceCinematicMode -> exact native game-instance method.
    // Forward EVERY argument. Mirror only the game's own lifecycle signal.
    g_cinema(instance,enabled,allPlayers);
    const bool changed=g_cinematic.exchange(enabled,std::memory_order_acq_rel)!=enabled;
    static std::atomic_uint32_t events{0};
    const unsigned n=++events;
    if(g_log&&(changed||n==1)){
        char line[180]{};
        sprintf_s(line,"TPS V1.32 NATIVE_CINEMATIC_MODE enabled=%d allPlayers=%d instance=%p tid=%lu",
            enabled?1:0,allPlayers?1:0,instance,GetCurrentThreadId());
        g_log(line);
    }
}
void HookWidgetState(void*,unsigned char,bool);
struct HookSpec {uintptr_t rva; unsigned char bytes[16]; void* detour; void** original;};
}
bool Install(RayProvider provider,LogFn log){
    g_provider=provider;g_log=log;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!g_base)return false;
    HookSpec hooks[]={
        {0x5719A0,{0x48,0x8B,0xC4,0x55,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xA8,0x38,0xFF},reinterpret_cast<void*>(&HookAim2),reinterpret_cast<void**>(&g_aim2)},
        {0x571E50,{0x40,0x55,0x53,0x56,0x57,0x41,0x56,0x48,0x8D,0x6C,0x24,0xD0,0x48,0x81,0xEC,0x30},reinterpret_cast<void*>(&HookAim3),reinterpret_cast<void**>(&g_aim3)},
        {0x187F0B0,{0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x81,0xEC,0x60,0x01},reinterpret_cast<void*>(&HookDeproject),reinterpret_cast<void**>(&g_deproject)},
        {0x1ACAA70,{0x48,0x89,0x5C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x20,0x48},reinterpret_cast<void*>(&HookMousePosition),reinterpret_cast<void**>(&g_mousePosition)},
        {0x648B20,{0x48,0x89,0x74,0x24,0x18,0x57,0x41,0x56,0x41,0x57,0x48,0x83,0xEC,0x20,0x45,0x0F},reinterpret_cast<void*>(&HookCinema),reinterpret_cast<void**>(&g_cinema)}
    };
    constexpr unsigned char pauseBytes[]={0x48,0x83,0xEC,0x28,0x48,0x8B,0xD1,0x41,0xB8,0x01,0x00,0x00,0x00,0x48,0x8B,0x0D};
    for(const auto& h:hooks){
        if(std::memcmp(reinterpret_cast<void*>(g_base+h.rva),h.bytes,16)){
            if(log)log("TPS V1.32 native signatures mismatch: all new paths disabled");
            return false;
        }
    }
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x188D940),pauseBytes,16))return false;
    unsigned created=0;
    for(const auto& h:hooks){
        if(MH_CreateHook(reinterpret_cast<void*>(g_base+h.rva),h.detour,h.original)!=MH_OK)break;
        ++created;
    }
    bool ok=created==sizeof(hooks)/sizeof(hooks[0]);
    if(ok){
        for(const auto& h:hooks)
            if(MH_EnableHook(reinterpret_cast<void*>(g_base+h.rva))!=MH_OK){ok=false;break;}
    }
    if(!ok){
        for(unsigned i=0;i<created;++i){
            MH_DisableHook(reinterpret_cast<void*>(g_base+hooks[i].rva));
            MH_RemoveHook(reinterpret_cast<void*>(g_base+hooks[i].rva));
        }
        if(log)log("TPS V1.32 native hook transaction failed: new paths disabled");
        return false;
    }
    g_paused.store(reinterpret_cast<PausedFn>(g_base+0x188D940),std::memory_order_release);
    constexpr unsigned char settingsBytes[]={0x48,0x8B,0x0D,0x51,0xFD,0x1C,0x02,0xE9,0x24,0x88,0x3B,0x00,0xCC,0xCC,0xCC,0xCC};
    constexpr unsigned char settingsClassBytes[]={0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x0A,0x09,0x0D,0x03,0x48,0x85};
    constexpr unsigned char visibleBytes[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x1885BE0),settingsBytes,16)&&
       !std::memcmp(reinterpret_cast<void*>(g_base+0x736620),settingsClassBytes,16))
        g_settings=reinterpret_cast<SettingsFn>(g_base+0x1885BE0);
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x14BEA00),visibleBytes,16))
        g_visible=reinterpret_cast<VisibleFn>(g_base+0x14BEA00);
    // HUD selector state and controller ownership, proved independently of
    // the visibility observer. Failure keeps the configurable input guard.
    constexpr unsigned char stateBytes[]={0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x20,0x0F};
    constexpr unsigned char controllerBytes[]={0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8D,0x99,0x18,0x02,0x00,0x00,0x48,0x8B,0xCB};
    if(g_visible&&!std::memcmp(reinterpret_cast<void*>(g_base+0x5E41B0),stateBytes,16)&&
       !std::memcmp(reinterpret_cast<void*>(g_base+0x14B5130),controllerBytes,16)){
        void* target=reinterpret_cast<void*>(g_base+0x5E41B0);
        if(MH_CreateHook(target,reinterpret_cast<void*>(&HookWidgetState),reinterpret_cast<void**>(&g_widgetState))==MH_OK){
            g_widgetController=reinterpret_cast<WidgetControllerFn>(g_base+0x14B5130);
            if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_widgetState=nullptr;g_widgetController=nullptr;}
        }
    }
    // Optional result observer. Its failure cannot disable accepted targeting.
    constexpr unsigned char traceBytes[]={0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x41,0x8B,0x41,0x08,0xF2,0x41,0x0F,0x10,0x01};
    constexpr unsigned char siteBytes[]={0xE8,0xEE,0xC5,0x1A,0x01};
    void* traceTarget=reinterpret_cast<void*>(g_base+0x171E560);void* traceOriginal=nullptr;
    if(!std::memcmp(traceTarget,traceBytes,sizeof(traceBytes))&&!std::memcmp(reinterpret_cast<void*>(g_base+0x571F6D),siteBytes,sizeof(siteBytes))&&
       MH_CreateHook(traceTarget,reinterpret_cast<void*>(&HookTrace),&traceOriginal)==MH_OK){
        g_trace=reinterpret_cast<TraceFn>(traceOriginal);
        if(MH_EnableHook(traceTarget)==MH_OK)g_contactReady=true;
        else {MH_RemoveHook(traceTarget);g_trace=nullptr;}
    }
    if(log)log(g_contactReady?"TPS V1.130 CONTACT READY: scoped native camera collision identity; parameters and result unchanged":"TPS V1.130 CONTACT unavailable: previous targeting retained");
    g_ready.store(true,std::memory_order_release);
    if(log)log("TPS V1.32 native target hooks READY: local physical mouse only; scoped deprojection; native collision; native cinematic observer; gameplay EXPERIMENTAL");
    return true;
}
bool TargetReady(){return g_ready.load(std::memory_order_acquire);}
bool CinematicActive(unsigned index){
    return dg::local_players::Ready()?dg::local_players::CameraCinematic(index):
        g_cinematic.load(std::memory_order_acquire);
}
bool IsWorldPaused(void* context){
    const auto fn=g_paused.load(std::memory_order_acquire);
    return fn&&context?fn(context):false;
}
void SampleNativeStickSwap(){
    DWORD unset=0;
    g_gameThread.compare_exchange_strong(unset,GetCurrentThreadId());
    if(!g_ready.load(std::memory_order_acquire)||!g_settings)return;
    // Native settings getter + exact class test, identical to game code.
    void* settings=g_settings();
    void* cls=*reinterpret_cast<void**>(g_base+0x3806F38);
    if(!IsInstanceOf(settings,cls))return;
    const bool swap=*(static_cast<const unsigned char*>(settings)+0x230)!=0;
    const bool changed=g_swap.exchange(swap)!=swap;
    const bool first=!g_swapKnown.exchange(true,std::memory_order_release);
    if(g_log&&(first||changed))g_log(swap?
        "TPS V1.34 NATIVE_STICK_SWAP=1 look=LEFT move=RIGHT (read only)":
        "TPS V1.34 NATIVE_STICK_SWAP=0 look=RIGHT move=LEFT (read only)");
}
bool NativeStickSwap(){return g_swap.load(std::memory_order_acquire);}
bool NativeStickSwapKnown(){return g_swapKnown.load(std::memory_order_acquire);}
bool OnGameThread(){
    const DWORD thread=g_gameThread.load(std::memory_order_acquire);
    return thread&&thread==GetCurrentThreadId();
}
namespace {
thread_local bool g_observingRadial=false;
void ObserveRadial(void* object,int request){
    if(!g_ready.load(std::memory_order_acquire)||!g_visible||!g_widgetController||
       !object||!OnGameThread()||g_observingRadial)return;
    void* cls=*reinterpret_cast<void**>(g_base+0x380E0B0);
    if(!IsInstanceOf(object,cls))return;
    native_objects::MemoryReader reader;
    native_objects::Identity identity{};
    if(!native_objects::CaptureLive(g_base,object,identity))return;
    g_observingRadial=true;
    void* controller=g_widgetController(object);
    uintptr_t state=0,actor=0;
    local_players::Context owner{};
    const bool owned=reader.Read(reinterpret_cast<uintptr_t>(controller),0x318,state)&&
        reader.Read(state,0x5C0,actor)&&local_players::ResolveActor(reinterpret_cast<void*>(actor),owner)&&
        owner.controller==reinterpret_cast<uintptr_t>(controller)&&owner.index>=0&&owner.index<2;
    const bool visible=owned&&g_visible(object);
    unsigned char nativeState=3;
    reader.Read(reinterpret_cast<uintptr_t>(object),0x254,nativeState);
    g_observingRadial=false;
    if(!owned)return;
    const auto slot=local_players::PublishedSlot(unsigned(owner.index));
    bool changed=false,active=false;
    AcquireSRWLockExclusive(&g_radialLock);
    RadialSample* chosen=nullptr;
    for(auto& sample:g_radials)if(sample.identity==identity){chosen=&sample;break;}
    if(!chosen&&request>=0){
        for(auto& sample:g_radials)if(!sample.identity||!native_objects::Alive(reader,g_base,sample.identity)){
            chosen=&sample;*chosen={};chosen->identity=identity;break;
        }
    }
    if(chosen){
        const bool same=chosen->local==owner.localPlayer&&chosen->character==owner.character&&
            chosen->generation==slot.generation&&chosen->player==unsigned(owner.index);
        if(!same)chosen->selection={};
        chosen->local=owner.localPlayer;chosen->character=owner.character;
        chosen->generation=slot.generation;chosen->player=unsigned(owner.index);
        if(request>=0)chosen->selection.Request(unsigned(request));
        active=chosen->selection.Selecting(nativeState,visible);
        changed=chosen->active!=active;chosen->active=active;
    }
    ReleaseSRWLockExclusive(&g_radialLock);
    if(changed){
        const auto n=++g_radialEvents;
        if(g_log&&n<=120){char line[220]{};
            sprintf_s(line,"TPS V1.50 NATIVE_RADIAL player=%d selecting=%d state=%u request=%d object=%p generation=%u bindingIndependent=1",
                owner.index+1,active?1:0,unsigned(nativeState),request,object,slot.generation);g_log(line);}
    }
}
void HookWidgetState(void* object,unsigned char state,bool update){
    // The native transition is authoritative; don't infer opening from a
    // constructed widget's default visibility or from a physical button.
    native_objects::Identity identity{};
    const bool live=OnGameThread()&&object&&IsInstanceOf(object,*reinterpret_cast<void**>(g_base+0x380E0B0))&&native_objects::CaptureLive(g_base,object,identity);
    g_widgetState(object,state,update);
    native_objects::MemoryReader reader;
    if(live&&native_objects::Alive(reader,g_base,identity))ObserveRadial(object,state);
}
}
native_objects::Identity CaptureUiEvent(void* object){
    native_objects::Identity identity{};
    if(g_ready.load(std::memory_order_acquire)&&OnGameThread()&&object&&
       IsInstanceOf(object,*reinterpret_cast<void**>(g_base+0x380E0B0)))
        native_objects::CaptureLive(g_base,object,identity);
    return identity;
}
void ObserveUiEvent(const native_objects::Identity& identity){
    native_objects::MemoryReader reader;
    if(OnGameThread()&&native_objects::Alive(reader,g_base,identity))
        ObserveRadial(reinterpret_cast<void*>(identity.address),-1);
}
bool RadialActive(unsigned index){
    if(index>1)return false;
    const auto owner=local_players::PublishedSlot(index);
    if(!owner.localPlayer)return false;
    native_objects::MemoryReader reader;
    bool active=false;
    AcquireSRWLockShared(&g_radialLock);
    for(const auto& sample:g_radials)
        if(sample.active&&sample.player==index&&sample.local==owner.localPlayer&&
           sample.character==owner.character&&sample.generation==owner.generation&&
           native_objects::Alive(reader,g_base,sample.identity)){active=true;break;}
    ReleaseSRWLockShared(&g_radialLock);
    return active;
}
bool TraceCameraTarget(const local_players::Context& owner,const MouseRay& ray,Vec3& target,bool& hit,const Vec3* source){
    if(!g_ready.load(std::memory_order_acquire)||!g_aim3||!OnGameThread()||
       owner.index<0||owner.index>1||!owner.controller||!owner.pawn||
       !Finite(ray.origin)||!Finite(ray.direction))return false;
    const float length=ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z;
    if(length<0.99f||length>1.01f)return false;
    return QueryTarget(reinterpret_cast<void*>(owner.controller),reinterpret_cast<void*>(owner.pawn),ray,source,target,hit,nullptr);
}
bool TraceCameraContact(const local_players::Context& owner,const MouseRay& ray,Vec3& target,bool& hit,native_objects::Identity& contact,const Vec3* source){
    contact={};
    if(!g_contactReady.load()||!g_ready.load()||!g_aim3||!OnGameThread()||owner.index<0||owner.index>1||!owner.controller||!owner.pawn||
       !Finite(ray.origin)||!Finite(ray.direction))return false;
    const float length=ray.direction.x*ray.direction.x+ray.direction.y*ray.direction.y+ray.direction.z*ray.direction.z;
    if(length<.99f||length>1.01f)return false;
    return QueryTarget(reinterpret_cast<void*>(owner.controller),reinterpret_cast<void*>(owner.pawn),ray,source,target,hit,nullptr,&contact);
}
void Report(){
    if(!g_log)return;
    char line[280]{};
    sprintf_s(line,"TPS V1.32 native targeting totals: ready=%d helper2=%u helper3=%u scopedRays=%u foot=%u horse=%u nativeHits=%u noHitForward=%u nativePass=%u actualProjectileImpacts=UNMEASURED",
        TargetReady()?1:0,g_native2.load(),g_native3.load(),g_rays.load(),
        g_foot.load(),g_horse.load(),g_hits.load(),g_misses.load(),g_rejected.load());
    g_log(line);
    sprintf_s(line,"TPS V1.37 native Strife Ultimate mouse targets=%u actualProjectileImpacts=UNMEASURED",g_ultimate.load());
    g_log(line);
    sprintf_s(line,"TPS V1.34 native action totals: radialEvents=%u nativeSwapKnown=%d nativeSwap=%d mouseTargetsWithoutHeldButton=%u wheelsCoverage=UNVALIDATED",
        g_radialEvents.load(),NativeStickSwapKnown()?1:0,NativeStickSwap()?1:0,g_idleMouseTargets.load());
    g_log(line);
}
}
