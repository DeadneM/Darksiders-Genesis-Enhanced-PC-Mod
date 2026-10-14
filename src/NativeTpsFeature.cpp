#include "NativeTpsFeature.h"
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
using CinemaFn=void(*)(void*,bool,bool);
using PausedFn=bool(*)(void*);
Aim2Fn g_aim2=nullptr;
Aim3Fn g_aim3=nullptr;
DeprojectFn g_deproject=nullptr;
CinemaFn g_cinema=nullptr;
std::atomic<PausedFn> g_paused{nullptr};
RayProvider g_provider=nullptr;
LogFn g_log=nullptr;
uintptr_t g_base=0;
std::atomic_bool g_ready{false},g_cinematic{false};
std::atomic_uint32_t g_foot{0},g_horse{0},g_hits{0},g_misses{0},g_rays{0};
std::atomic_uint32_t g_native2{0},g_native3{0},g_rejected{0};
struct RayScope {
    void* controller;
    MouseRay ray;
    RayScope* previous;
    bool used=false;
};
thread_local RayScope* g_scope=nullptr;
struct ScopedRay {
    RayScope scope;
    ScopedRay(void* c,const MouseRay& r):scope{c,r,g_scope}{g_scope=&scope;}
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
void Observe(const MouseRay& r,const Vec3& target,bool hit,const char* path){
    const unsigned n=r.mounted?++g_horse:++g_foot;
    if(hit)++g_hits;else ++g_misses;
    // Separate budgets: mounted observations cannot be exhausted on foot.
    if(g_log&&(n<=12||n==100||n==1000)){
        char line[410]{};
        sprintf_s(line,"TPS V1.32 NATIVE_MOUSE_TARGET source=%s path=%s n=%u nativeHit=%d camera=(%.1f,%.1f,%.1f) ray=(%.4f,%.4f,%.4f) target=(%.1f,%.1f,%.1f) muzzleConsumer=NATIVE projectileImpactUnmeasured=1",
            r.mounted?"HORSE":"FOOT",path,n,hit?1:0,
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
Vec3* HookAim3(void* controller,Vec3* out,void* actor,NativeTarget* cache,
               bool debug,void* extra){
    ++g_native3;
    MouseRay ray{};
    if(!controller||!out||!cache||!Context(actor,ray)){
        ++g_rejected;
        return g_aim3(controller,out,actor,cache,debug,extra);
    }
    // A stack-local cache prevents old world/old screen targets from being
    // reused after a miss. Copy back only a native collision HIT, following
    // the original helper's existing !debug cache-write contract.
    NativeTarget fresh{};
    ScopedRay scope(controller,ray);
    Vec3 native{};
    g_aim3(controller,&native,actor,&fresh,false,extra);
    if(!scope.scope.used){ // native helper could not obtain a viewport
        return g_aim3(controller,out,actor,cache,debug,extra);
    }
    const bool hit=fresh.valid&&Finite(fresh.point);
    *out=hit?fresh.point:FarTarget(ray);
    if(hit&&!debug)*cache=fresh;
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
    NativeTarget fresh{};
    Vec3 target{};
    ScopedRay scope(controller,ray);
    // Reuse the game's actual 3D collision helper with its existing world,
    // query channel and hit result. Do not call the old plane intersection.
    g_aim3(controller,&target,actor,&fresh,false,extra);
    if(!scope.scope.used)
        return g_aim2(controller,out,actor,reference,flag,extra);
    const bool hit=fresh.valid&&Finite(fresh.point);
    target=hit?fresh.point:FarTarget(ray);
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
    bool ok=created==4;
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
    g_ready.store(true,std::memory_order_release);
    if(log)log("TPS V1.32 native target hooks READY: local physical mouse only; scoped deprojection; native collision; native cinematic observer; gameplay EXPERIMENTAL");
    return true;
}
bool TargetReady(){return g_ready.load(std::memory_order_acquire);}
bool CinematicActive(){return g_cinematic.load(std::memory_order_acquire);}
bool IsWorldPaused(void* context){
    const auto fn=g_paused.load(std::memory_order_acquire);
    return fn&&context?fn(context):false;
}
void Report(){
    if(!g_log)return;
    char line[280]{};
    sprintf_s(line,"TPS V1.32 native targeting totals: ready=%d helper2=%u helper3=%u scopedRays=%u foot=%u horse=%u nativeHits=%u noHitForward=%u nativePass=%u actualProjectileImpacts=UNMEASURED",
        TargetReady()?1:0,g_native2.load(),g_native3.load(),g_rays.load(),
        g_foot.load(),g_horse.load(),g_hits.load(),g_misses.load(),g_rejected.load());
    g_log(line);
}
}
