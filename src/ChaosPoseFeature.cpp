#include "ChaosPoseFeature.h"
#include "NativeTpsFeature.h"
#include "ManualGaugeFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::chaos_pose {
namespace {
using PawnFn=void*(*)(void*,int);
using LocalFn=bool(*)(void*);
PawnFn g_pawn=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false},g_active{false};
std::atomic_ullong g_tick{0},g_poseTick{0};
std::atomic<float> g_poseYaw{0};
std::atomic_bool g_ranged{false};
std::atomic_uintptr_t g_formIdentity{0},g_ownerIdentity{0};
std::atomic_uint32_t g_samples{0},g_valid{0},g_changes{0};
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION r{};
    if(!p||!VirtualQuery(p,&r,sizeof(r))||r.State!=MEM_COMMIT||
       (r.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto access=r.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&
       access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(r.BaseAddress);
    return offset<=r.RegionSize&&bytes<=r.RegionSize-offset;
}
template<class T> bool Read(void* p,size_t offset,T& value){
    const auto address=reinterpret_cast<uintptr_t>(p);
    if(!p||address>UINTPTR_MAX-offset)return false;
    const void* field=reinterpret_cast<void*>(address+offset);
    if(!Readable(field,sizeof(value)))return false;
    std::memcpy(&value,field,sizeof(value));return true;
}
bool Instance(void* object,uintptr_t slot){
    void* cls=nullptr;void* actual=nullptr;void* array=nullptr;void* entry=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+slot),0,cls)&&cls&&
        Read(object,0x10,actual)&&actual&&Read(cls,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*sizeof(void*),entry)&&
        entry==static_cast<unsigned char*>(cls)+0x30;
}
bool LocalPawn(void* pawn){
    void* table=nullptr;void* fn=nullptr;
    // Exact APawn::IsLocallyControlled virtual slot, already used by the
    // validated player-movement path. Only call retail executable code.
    if(!Read(pawn,0,table)||!table||!Read(table,0x680,fn))return false;
    const auto address=reinterpret_cast<uintptr_t>(fn);
    return address>=g_base+0x1000&&address<g_base+0x2600000&&
        reinterpret_cast<LocalFn>(fn)(pawn);
}
}
void Initialize(LogFn logger,bool exactExecutable){
    g_log=logger;
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard { uintptr_t rva; unsigned char bytes[16]; };
    const Guard guards[]={
        {0x1886B70,{0x48,0x83,0xEC,0x28,0xE8,0x87,0xFD,0xFF,0xFF,0x48,0x85,0xC0,0x74,0x11,0x48,0x8B}},
        {0x1886900,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x30,0x8B,0xFA,0x41,0xB8,0x01,0x00}},
        {0x76C2E0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x92,0xE9,0x09,0x03,0x48,0x85}},
        {0x7A6350,{0x48,0x83,0xEC,0x28,0x48,0x8B,0x05,0x1D,0x9B,0x06,0x03,0x48,0x85,0xC0,0x75,0x1A}},
        {0x7ACC15,{0x48,0x8B,0x81,0x78,0x0E,0x00,0x00,0x49,0x89,0x00,0xC3,0x48,0x8B,0x42,0x20,0x45}},
        {0x7ACC35,{0x0F,0xB6,0x81,0x18,0x0C,0x00,0x00,0x41,0x88,0x00,0xC3,0x48,0x89,0x5C,0x24,0x08}},
        {0x7BFDA0,{0x48,0x83,0xEC,0x28,0x48,0x8B,0x05,0x2D,0x18,0x05,0x03,0x48,0x85,0xC0,0x75,0x1A}},
        {0x56BE70,{0x48,0x8B,0x81,0xC0,0x05,0x00,0x00,0xC3,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC}}
    };
    for(const auto& guard:guards)
        if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
            if(g_log)g_log("TPS V1.36 ULTIMATE_POSE signatures mismatch; feature disabled");
            return;
        }
    g_pawn=reinterpret_cast<PawnFn>(g_base+0x1886B70);
    g_ready.store(true,std::memory_order_release);
    if(g_log)g_log("TPS V1.36 ULTIMATE_POSE ready; native local pawn / owned form; no new hooks; EXPERIMENTAL");
}
bool Sample(void* camera,Pose& pose){
    if(!g_ready.load(std::memory_order_acquire)||!camera||!dg::native_tps::OnGameThread())return false;
    ++g_samples;
    void* pawn=g_pawn(camera,0);
    void* form=nullptr;
    const char* source="NONE";
    // Only a native locally controlled pawn may supply the form. No cached
    // player dereference, remote pawn or identity inferred from actor absence.
    const bool isForm=Instance(pawn,0x380AC80);
    const bool isPlayer=Instance(pawn,0x380FE78);
    if((isForm||isPlayer)&&LocalPawn(pawn)){
        if(isForm){form=pawn;source="POSSESSED_FORM";}
        else{
            void* owned=nullptr;
            // Native GetChaosForm / IsChaosFormActive -> player+0xE78.
            if(Read(pawn,0xE78,owned)&&Instance(owned,0x380AC80)){
                form=owned;source="OWNED_FORM";
            }
        }
    }
    // Exact APawn.PlayerState -> native holder.GetCharacter -> normal player.
    // Require the reciprocal native GetChaosForm relation, never a guessed
    // previous pawn. CharacterType=1 is Strife (native Strife_Chaos mapping).
    void* state=nullptr;void* owner=nullptr;void* reciprocal=nullptr;
    unsigned char type=0;
    const bool verifiedOwner=form&&Read(form,0x330,state)&&Instance(state,0x38115D8)&&
        Read(state,0x5C0,owner)&&Instance(owner,0x380FE78)&&
        Read(owner,0xE78,reciprocal)&&reciprocal==form&&Read(owner,0xC18,type);
    g_formIdentity.store(reinterpret_cast<uintptr_t>(form),std::memory_order_relaxed);
    g_ownerIdentity.store(verifiedOwner?reinterpret_cast<uintptr_t>(owner):0,std::memory_order_relaxed);
    g_ranged.store(verifiedOwner&&type==1,std::memory_order_release);
    Pose candidate{};
    void* root=nullptr;
    bool valid=form&&Read(form,0x158,root)&&root&&
        Read(root,0x1A0,candidate.x)&&Read(root,0x1A4,candidate.y)&&
        Read(root,0x1A8,candidate.z)&&Read(root,0x1F4,candidate.yaw)&&
        std::isfinite(candidate.x)&&std::isfinite(candidate.y)&&std::isfinite(candidate.z)&&
        std::isfinite(candidate.yaw)&&std::fabs(candidate.x)<1e7f&&
        std::fabs(candidate.y)<1e7f&&std::fabs(candidate.z)<1e7f&&
        std::fabs(candidate.yaw)<36000.0f;
    if(valid){pose=candidate;++g_valid;g_poseYaw.store(candidate.yaw,std::memory_order_relaxed);}
    g_poseTick.store(valid?GetTickCount64():0,std::memory_order_release);
    if(verifiedOwner)dg::manual_gauges::OnLivePlayer(owner,
        dg::runtime::Get().tpsActorGeneration.load(std::memory_order_acquire));
    const auto now=GetTickCount64();
    const bool changed=g_active.exchange(form!=nullptr,std::memory_order_acq_rel)!=(form!=nullptr);
    g_tick.store(now,std::memory_order_release);
    if(g_log&&(changed||g_samples.load()==1)){
        ++g_changes;
        char line[410]{};
        sprintf_s(line,"TPS V1.37 ULTIMATE_POSE active=%d source=%s pawn=%p form=%p poseValid=%d world=(%.1f,%.1f,%.1f) tid=%lu ownerVerified=%d owner=%p characterType=%u aiming=%s",
            form?1:0,source,pawn,form,valid?1:0,candidate.x,candidate.y,candidate.z,GetCurrentThreadId(),
            verifiedOwner?1:0,verifiedOwner?owner:nullptr,unsigned(type),
            verifiedOwner&&type==1?"STRIFE_CAMERA_FORWARD":"NATIVE");
        g_log(line);
    }
    return valid;
}
bool ActiveNow(){
    const auto tick=g_tick.load(std::memory_order_acquire),now=GetTickCount64();
    return g_active.load(std::memory_order_acquire)&&tick&&now>=tick&&now-tick<250ull;
}
bool GetLiveYaw(float& yaw){
    const auto tick=g_poseTick.load(std::memory_order_acquire),now=GetTickCount64();
    if(!ActiveNow()||!tick||now<tick||now-tick>=250)return false;
    yaw=g_poseYaw.load(std::memory_order_relaxed);return std::isfinite(yaw);
}
bool RangedActive(){return ActiveNow()&&g_ranged.load(std::memory_order_acquire);}
bool RangedActor(void* actor){
    const auto identity=reinterpret_cast<uintptr_t>(actor);
    return identity&&RangedActive()&&(identity==g_formIdentity.load(std::memory_order_acquire)||
        identity==g_ownerIdentity.load(std::memory_order_acquire));
}
bool RangedOwner(void* actor){
    return actor&&RangedActive()&&reinterpret_cast<uintptr_t>(actor)==g_ownerIdentity.load(std::memory_order_acquire);
}
void Shutdown(){
    g_ready.store(false,std::memory_order_release);g_active.store(false,std::memory_order_release);
    if(g_log){
        char line[180]{};
        sprintf_s(line,"TPS V1.36 ULTIMATE_POSE totals samples=%u validPoses=%u stateEvents=%u gameplayValidation=USER_REQUIRED",g_samples.load(),g_valid.load(),g_changes.load());
        g_log(line);
    }
}
}
