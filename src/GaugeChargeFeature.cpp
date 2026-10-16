#include "GaugeChargeFeature.h"
#include "ManualGaugeFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <windows.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <intrin.h>

namespace dg::gauges {
namespace {
// Audited SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54:
// UE4 native function table proves:
// Wrath/Ultimate natural gains are handled by ResourceGainFeature (validated).
//  ::SetCurrentSynergy wrapper RVA 0x7C5240 -> native RVA 0x583040(this,float);
//  native GetCurrentSynergy 0x56D430: [this+0x5A4] float;
//  native GetMaxSynergy 0x56FEC0: [this+0x498] float.
// Fail closed on any signature mismatch and on unrecognized local ownership.
// IMPORTANT V1.06: exact shipping EXE caller RVA 0x7C52C2 reads xmm0
// after calling SetCurrentSynergy RVA 0x583040. Its original return is FLOAT,
// not void; V1.05 had accidentally changed the detour ABI.
using SynergyFn=float(*)(void*,float); // XMM0 return consumed by Blueprint wrapper 0x7C52C2
// RVA 0x65A1A0: (manager, synergy owner, factorA, factorB, factorC).
// Native computes current + A*B*C and tail-jumps to SetCurrentSynergy.
using SynergyGainFn=float(*)(void*,void*,float,float,float);
SynergyFn originalSynergy=nullptr;
SynergyGainFn originalSynergyGain=nullptr;
LogFn logger=nullptr;
std::atomic<void*>* localPlayer=nullptr;
std::atomic_bool synergyReady{false},synergyGainReady{false};
std::atomic_uint32_t synergyEvents{0};
std::atomic_uint32_t synergyBoosts{0};
std::atomic_uint32_t synergyLocal{0},synergyPositive{0};
std::atomic_uint32_t synergyGainEvents{0},synergyGainLocal{0};
// Audit only: these counters do NOT affect native resource values.
std::atomic_uint32_t synergyAddPositive{0},synergyAddZero{0},
    synergyAddNegative{0},synergyAddInvalid{0},synergyPositiveSkipped{0};
std::atomic_uint32_t synergySetterIncreaseFromAdd{0},synergySetterIncreaseDirect{0},
    synergySetterUnchanged{0},synergySetterDecrease{0},synergySetterInvalid{0};
thread_local unsigned synergyAddDepth=0;
struct ScopedSynergyAdd {
    ScopedSynergyAdd(){++synergyAddDepth;}
    ~ScopedSynergyAdd(){--synergyAddDepth;}
};
std::atomic_uint32_t synergyNativeOwnerMatches{0},synergyOuterReferences{0},synergyNoVerifiedOwner{0};
void Log(const char* text);
std::atomic<void*> synergyLastActor{nullptr},synergyLastHolder{nullptr};
std::atomic_int synergyLastRelation{0};
// Native UObjects only: the Synergy container is NOT the character.
// V1.06: verify exact native holder->character relation at +0x5C0 before boost.
// Never infer local ownership from the presence of a single active gauge.
bool Readable(const void* ptr,size_t bytes){
    if(!ptr||!bytes)return false;
    MEMORY_BASIC_INFORMATION region{};
    if(!VirtualQuery(ptr,&region,sizeof(region))||
       region.State!=MEM_COMMIT||(region.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const DWORD access=region.Protect&0xFF;
    if(access!=PAGE_READONLY && access!=PAGE_READWRITE &&
       access!=PAGE_WRITECOPY && access!=PAGE_EXECUTE_READ &&
       access!=PAGE_EXECUTE_READWRITE && access!=PAGE_EXECUTE_WRITECOPY)return false;
    const uintptr_t addr=reinterpret_cast<uintptr_t>(ptr);
    const uintptr_t base=reinterpret_cast<uintptr_t>(region.BaseAddress);
    return addr>=base && size_t(addr-base)<=region.RegionSize &&
           bytes<=region.RegionSize-size_t(addr-base);
}
bool PointerFieldEquals(const void* object,size_t offset,const void* expected){
    if(!object||!expected)return false;
    const uintptr_t addr=reinterpret_cast<uintptr_t>(object);
    if(addr>UINTPTR_MAX-offset)return false;
    const void* address=reinterpret_cast<const void*>(addr+offset);
    if(!Readable(address,sizeof(void*)))return false;
    void* found=nullptr;
    std::memcpy(&found,address,sizeof(found));
    return found==expected;
}
int SynergyLocalRelation(void* holder,void* actor){
    if(!holder||!actor)return 0;
    if(holder==actor)return 1;
    if(synergyLastActor.load(std::memory_order_acquire)==actor &&
       synergyLastHolder.load(std::memory_order_acquire)==holder)
        return synergyLastRelation.load(std::memory_order_acquire);
    int relation=0;
    // VERIFIED IN EXACT EXE: RVA 0x56BE70 is a native inline getter
    //   mov rax, [rcx+0x5C0] ; ret.
    // At RVA 0x67E944..0x67E95B the engine obtains the same Synergy
    // owner (rdi) and then calls 0x56BE70(rdi) to obtain its character;
    // RVA 0x67EA9F and 0x67EAD7 subsequently pass rdi to the native
    // SetCurrentSynergy gain path. Exact owner+0x5C0 is the safe scope.
    if(PointerFieldEquals(holder,0x5C0,actor))relation=4;
    // UE4 UObject::OuterPrivate = +0x20. Traverse only live, readable
    // UObject headers; never retain/dereference them outside native callback.
    void* outer=holder;
    for(int depth=0;!relation&&depth<5 && outer;depth++){
        void* next=nullptr;
        if(!Readable(reinterpret_cast<const unsigned char*>(outer)+0x20,sizeof(next)))break;
        std::memcpy(&next,reinterpret_cast<const unsigned char*>(outer)+0x20,sizeof(next));
        if(next==actor){relation=2;break;}
        if(next==outer||!Readable(next,0x28))break;
        outer=next;
    }
    char line[220]{};
    sprintf_s(line,"Gauge V1.06 SYNERGY relation: holder=%p localActor=%p kind=%d (4=native holder+0x5C0 owner 2=Outer 1=same 0=unverified)",
        holder,actor,relation);
    Log(line);
    synergyLastRelation.store(relation,std::memory_order_release);
    synergyLastHolder.store(holder,std::memory_order_release);
    synergyLastActor.store(actor,std::memory_order_release);
    return relation;
}
void Log(const char* text){if(logger)logger(text);}
void Probe(const char* gauge,unsigned n,void* owner,float amount,float current,float limit,bool match){
    if(!logger||(n>2 && n%1000!=0))return;
    char line[250]{};
    sprintf_s(line,"Gauge V1.05 AUDIT %s n=%u owner=%p local=%p match=%d amount=%.3f current=%.3f max=%.3f",
        gauge,n,owner,localPlayer?localPlayer->load():nullptr,match?1:0,amount,current,limit);
    Log(line);
}
void Event(const char* name,unsigned count,float native,float modified){
    if(!logger||(count>3&&count%500!=0))return;
    char msg[240]{};
    sprintf_s(msg,"Gauge V1.03: %s LOCAL gain %.3f -> %.3f n=%u",name,native,modified,count);
    Log(msg);
}



// Inspect native callers only; no change to resource reward arithmetic.
uintptr_t CallerRva(void* address){
    const uintptr_t base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const uintptr_t caller=reinterpret_cast<uintptr_t>(address);
    return base && caller>=base && caller-base<0x4000000ull?caller-base:0;
}


// V1.06 native additive path. The game uses a real helper at RVA 0x65A1A0:
//   GetCurrentSynergy(owner) + A*B*C -> SetCurrentSynergy(owner,absolute).
// Modify only one positive factor while preserving the native setter's role.
float HookSynergyGain(void* manager,void* owner,float a,float b,float c){
    const uintptr_t caller=CallerRva(_ReturnAddress());
    if(!originalSynergyGain)return 0.0f;
    const unsigned n=++synergyGainEvents;
    void* actor=localPlayer?localPlayer->load(std::memory_order_acquire):nullptr;
    const int relation=SynergyLocalRelation(owner,actor);
    if(relation>0)++synergyGainLocal;
    const float nativeGain=(a*b)*c;
    const bool validGain=std::isfinite(nativeGain);
    const bool positiveGain=validGain && nativeGain>0.0f;
    const bool negativeGain=validGain && nativeGain<0.0f;
    if(logger && relation>0 && positiveGain && (n<=35||n%500==0)){
        char msg[230]{};
        sprintf_s(msg,"Gauge V1.25 SYNERGY_GAIN_CALLER n=%u RVA=0x%llX nativeGain=%.4f factors=(%.3f,%.3f,%.3f)",
            n,static_cast<unsigned long long>(caller),nativeGain,a,b,c);
        Log(msg);
    }
    if(relation>0 && positiveGain)
        dg::runtime::Get().lastNativeSynergyPositiveTick.store(
            GetTickCount64(),std::memory_order_release);
    const unsigned categoryCount = !validGain ? ++synergyAddInvalid
        : positiveGain ? ++synergyAddPositive
        : negativeGain ? ++synergyAddNegative : ++synergyAddZero;
    if(logger && (categoryCount <= (positiveGain ? 12u : 5u)
        || (categoryCount % 1000u)==0u)){
        char probe[256]{};
        sprintf_s(probe,
            "Gauge V1.15 ADD_CLASS n=%u class=%s categoryN=%u owner=%d factors=(%.9g,%.9g,%.9g) gain=%.9g",
            n, !validGain ? "INVALID" : positiveGain ? "POSITIVE" : negativeGain ? "NEGATIVE" : "ZERO",
            categoryCount,relation>0?1:0,a,b,c,nativeGain);
        Log(probe);
    }
    float effectiveC=c;
    auto& rt=dg::runtime::Get();
    if(relation>0&&rt.synergyChargeEnabled.load(std::memory_order_relaxed)&&
       std::isfinite(nativeGain)&&nativeGain>0.0f&&nativeGain<100000.0f){
        const float mult=rt.synergyChargeMultiplier.load(std::memory_order_relaxed);
        if(std::isfinite(mult)&&mult>=0.0f&&mult<=100.0f&&mult!=1.0f&&
           std::isfinite(c)&&std::isfinite(c*mult)&&std::fabs(c*mult)<100000.0f){
            effectiveC=c*mult;
            Event("Synergy Native Add",++synergyBoosts,nativeGain,(a*b)*effectiveC);
        }
    }
    if(positiveGain && effectiveC==c) ++synergyPositiveSkipped;
    if(logger&&(n<=2||n%1000==0)){
        char msg[250]{};
        sprintf_s(msg,"Gauge V1.06 ADD_SYNERGY n=%u localRelation=%d factors=(%.3f,%.3f,%.3f) native=%.3f applied=%.3f",
            n,relation,a,b,c,nativeGain,(a*b)*effectiveC);
        Log(msg);
    }
    // The game's additive wrapper calls the absolute setter. Tag this stack
    // only while it executes to distinguish gem/combat gain paths from
    // possible direct SetCurrentSynergy writes. No gameplay change.
    ScopedSynergyAdd scope;
    return originalSynergyGain(manager,owner,a,b,effectiveC);
}
// Natural gain classification is unchanged. Only the explicit Unlimited option
// can hold this verified local player's absolute value at its native maximum.
float HookSynergy(void* player,float requested){
    if(!originalSynergy)return 0.0f;
    const unsigned n=++synergyEvents;
    const void* local=localPlayer?localPlayer->load(std::memory_order_acquire):nullptr;
    const int relation=SynergyLocalRelation(player,const_cast<void*>(local));
    const bool match=relation>0;
    if(match)++synergyLocal;
    if(relation==2)++synergyOuterReferences;
    if(relation==4)++synergyNativeOwnerMatches;
    if(!match)++synergyNoVerifiedOwner;
    float previous=0.0f,maximum=0.0f;
    if(player){
        std::memcpy(&previous,static_cast<const unsigned char*>(player)+0x5A4,sizeof(previous));
        std::memcpy(&maximum,static_cast<const unsigned char*>(player)+0x498,sizeof(maximum));
    }
    if(std::isfinite(previous)&&std::isfinite(requested)){
        const float delta=requested-previous;
        const bool fromAdd=synergyAddDepth>0;
        unsigned category=0;
        const char* label=nullptr;
        if(delta>0.0f){
            ++synergyPositive;
            category=fromAdd?++synergySetterIncreaseFromAdd:++synergySetterIncreaseDirect;
            label=fromAdd?"GAIN_FROM_ADD":"GAIN_DIRECT_SET";
        }else if(delta<0.0f){
            category=++synergySetterDecrease;
            label="SPEND_OR_DECREASE";
        }else{
            category=++synergySetterUnchanged;
            label="UNCHANGED";
        }
        if(logger && (category <= (delta>0?12u:4u) || category%1000u==0u)){
            char sample[256]{};
            sprintf_s(sample,
                "Gauge V1.15 SET_CLASS n=%u class=%s classN=%u local=%u before=%.9g after=%.9g delta=%.9g cap=%.9g",
                n,label,category,match?1u:0u,previous,requested,delta,maximum);
            Log(sample);
        }
    }else{
        ++synergySetterInvalid;
    }
    Probe("SYNERGY SetCurrentSynergy",n,player,requested,previous,maximum,match);
    return originalSynergy(player,dg::manual_gauges::UnlimitedRequest(
        dg::manual_gauges::Synergy,player,requested));
}
bool InstallNative(uintptr_t base,uintptr_t rva,const unsigned char* sig,size_t count,
                   void* hook,void** original,std::atomic_bool& ready,const char* name){
    auto* target=reinterpret_cast<unsigned char*>(base+rva);
    if(std::memcmp(target,sig,count)!=0){
        char msg[160]{};sprintf_s(msg,"Gauge V1.03 %s SIGNATURE MISMATCH, native untouched",name);
        Log(msg);return false;
    }
    auto status=MH_CreateHook(target,hook,original);
    if(status!=MH_OK){
        char msg[160]{};sprintf_s(msg,"Gauge V1.03 %s CreateHook status=%d",name,int(status));
        Log(msg);return false;
    }
    status=MH_EnableHook(target);
    if(status!=MH_OK&&status!=MH_ERROR_ENABLED){
        MH_RemoveHook(target);
        char msg[160]{};sprintf_s(msg,"Gauge V1.03 %s EnableHook status=%d",name,int(status));
        Log(msg);return false;
    }
    ready.store(true);
    char msg[180]{};sprintf_s(msg,"Gauge V1.03 %s READY RVA=0x%zX",name,size_t(rva));
    Log(msg);return true;
}
}
void Initialize(LogFn callback,std::atomic<void*>* local,bool exactExecutable){
    logger=callback;localPlayer=local;
    if(!exactExecutable){Log("Gauge V1.03: unsupported EXE, all hooks OFF");return;}
    auto status=MH_Initialize();
    if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED){
        Log("Gauge V1.03: MinHook initialization failed");return;
    }
    const auto base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!base)return;
    constexpr unsigned char synergySig[]={0x40,0x53,0x48,0x83,0xEC,0x30,0xF3,0x0F,0x10,0x81};
    InstallNative(base,0x583040,synergySig,sizeof(synergySig),reinterpret_cast<void*>(&HookSynergy),
                  reinterpret_cast<void**>(&originalSynergy),synergyReady,"SetCurrentSynergy");
    constexpr unsigned char gainSig[]={0x40,0x53,0x48,0x83,0xEC,0x40,0x0F,0x29,0x74,0x24,0x30};
    InstallNative(base,0x65A1A0,gainSig,sizeof(gainSig),reinterpret_cast<void*>(&HookSynergyGain),
                  reinterpret_cast<void**>(&originalSynergyGain),synergyGainReady,"AddSynergyNative");
}
Telemetry GetTelemetry(){
    return {synergyReady.load()&&synergyGainReady.load(),synergyBoosts.load(),synergyEvents.load()};
}
void Shutdown(){
    char msg[256]{};
    sprintf_s(msg,"Gauge Synergy totals: observed=%u local=%u boosted=%u nativeOwner=%u outer=%u unverified=%u",
        synergyEvents.load(),synergyLocal.load(),synergyBoosts.load(),
        synergyNativeOwnerMatches.load(),synergyOuterReferences.load(),synergyNoVerifiedOwner.load());
    Log(msg);
    sprintf_s(msg,"Gauge V1.06 additive totals: ready=%u observed=%u verifiedLocal=%u boosted=%u (setter passthrough)",
        synergyGainReady.load()?1u:0u,synergyGainEvents.load(),synergyGainLocal.load(),synergyBoosts.load());
    Log(msg);
    sprintf_s(msg,
        "Gauge V1.15 additive classification: positive=%u zero=%u negative=%u invalid=%u positiveNotBoosted=%u",
        synergyAddPositive.load(),synergyAddZero.load(),synergyAddNegative.load(),
        synergyAddInvalid.load(),synergyPositiveSkipped.load());
    Log(msg);
    sprintf_s(msg,
        "Gauge V1.15 setter classification: gainFromAdd=%u gainDirectSet=%u unchanged=%u decreased=%u invalid=%u",
        synergySetterIncreaseFromAdd.load(),synergySetterIncreaseDirect.load(),
        synergySetterUnchanged.load(),synergySetterDecrease.load(),synergySetterInvalid.load());
    Log(msg);
}
}
