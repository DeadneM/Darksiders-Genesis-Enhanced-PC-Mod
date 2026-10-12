#include "GaugeChargeFeature.h"
#include "RuntimeSettings.h"
#include <MinHook.h>
#include <windows.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdint>

namespace dg::gauges {
namespace {
// Audited SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54:
// UE4 native function table proves:
//  UMayhem*::AddWrath wrapper RVA 0x7ABD90 -> native RVA 0x660570(this,float).
//  ::AddChaos wrapper RVA 0x76FC60 -> native RVA 0x62E060(this,float).
//  ::SetCurrentSynergy wrapper RVA 0x7C5240 -> native RVA 0x583040(this,float);
//  native GetCurrentSynergy 0x56D430: [this+0x5A4] float;
//  native GetMaxSynergy 0x56FEC0: [this+0x498] float.
// Fail closed on any signature mismatch and on unrecognized local ownership.
// IMPORTANT V1.06: exact shipping EXE caller RVA 0x7C52C2 reads xmm0
// after calling SetCurrentSynergy RVA 0x583040. Its original return is FLOAT,
// not void; V1.05 had accidentally changed the detour ABI.
using WrathFn=void(*)(void*,float);
using ChaosFn=float(*)(void*,float);
using SynergyFn=float(*)(void*,float); // XMM0 return consumed by Blueprint wrapper 0x7C52C2
WrathFn originalWrath=nullptr;
ChaosFn originalChaos=nullptr;
SynergyFn originalSynergy=nullptr;
LogFn logger=nullptr;
std::atomic<void*>* localPlayer=nullptr;
std::atomic_bool wrathReady{false},chaosReady{false},synergyReady{false};
std::atomic_uint32_t wrathEvents{0},chaosEvents{0},synergyEvents{0};
std::atomic_uint32_t wrathBoosts{0},chaosBoosts{0},synergyBoosts{0};
std::atomic_uint32_t wrathLocal{0},chaosLocal{0},synergyLocal{0},synergyPositive{0},synergyRejected{0};
std::atomic_uint32_t synergyActorReferences{0},synergyOuterReferences{0},synergyNoVerifiedOwner{0};
std::atomic<void*> synergyLastActor{nullptr},synergyLastHolder{nullptr};
std::atomic_int synergyLastRelation{0};
// Native UObjects only: the actual Synergy container is NOT the Character
// pointer (validated V1.05 log). Derive ownership through an exact pointer
// identity; never infer ownership from a shared gauge or a single active actor.
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
    // UE4 UObject::OuterPrivate = +0x20. Traverse only live, readable
    // UObject headers; never retain/dereference them outside native callback.
    void* outer=holder;
    for(int depth=0;depth<5 && outer;depth++){
        void* next=nullptr;
        if(!Readable(reinterpret_cast<const unsigned char*>(outer)+0x20,sizeof(next)))break;
        std::memcpy(&next,reinterpret_cast<const unsigned char*>(outer)+0x20,sizeof(next));
        if(next==actor){relation=2;break;}
        if(next==outer||!Readable(next,0x28))break;
        outer=next;
    }
    // Native player characters can own the synergy state as a referenced
    // subobject or PlayerState. Match the *exact* pointer in the live local
    // player's first 0x800 bytes. Never accept holder->actor alone, because
    // a remote enemy may reference the local actor as a target.
    if(!relation){
        for(size_t off=0x28;off<0x800;off+=sizeof(void*)){
            if(PointerFieldEquals(actor,off,holder)){
                relation=3;
                char line[180]{};
                sprintf_s(line,"Gauge V1.06 SYNERGY: verified character+0x%zX references holder; local owner linked",off);
                Log(line);
                break;
            }
        }
    }
    char line[220]{};
    sprintf_s(line,"Gauge V1.06 SYNERGY relation: holder=%p localActor=%p kind=%d (1=same 2=Outer 3=player-field; 0=no proof)",
        holder,actor,relation);
    Log(line);
    synergyLastRelation.store(relation,std::memory_order_release);
    synergyLastHolder.store(holder,std::memory_order_release);
    synergyLastActor.store(actor,std::memory_order_release);
    return relation;
}
void Log(const char* text){if(logger)logger(text);}
void Probe(const char* gauge,unsigned n,void* owner,float amount,float current,float limit,bool match){
    if(!logger||(n>12 && n!=25 && n!=100 && n%2000!=0))return;
    char line[250]{};
    sprintf_s(line,"Gauge V1.05 AUDIT %s n=%u owner=%p local=%p match=%d amount=%.3f current=%.3f max=%.3f",
        gauge,n,owner,localPlayer?localPlayer->load():nullptr,match?1:0,amount,current,limit);
    Log(line);
}
void Event(const char* name,unsigned count,float native,float modified){
    if(!logger||(count>12&&count%250!=0))return;
    char msg[240]{};
    sprintf_s(msg,"Gauge V1.03: %s LOCAL gain %.3f -> %.3f n=%u",name,native,modified,count);
    Log(msg);
}
bool Local(void* object){
    return object&&localPlayer&&localPlayer->load(std::memory_order_acquire)==object;
}
bool LocalOwner(void* component){
    if(!component||!localPlayer)return false;
    void* owner=nullptr;
    // The native AddChaos reads its live component owner at this offset too.
    std::memcpy(&owner,static_cast<const unsigned char*>(component)+0xE8,sizeof(owner));
    return Local(owner);
}
float Boost(float value,bool enabled,float factor){
    if(!enabled||!std::isfinite(value)||value<=0.0f||value>=100000.0f||
       !std::isfinite(factor)||factor<1.0f||factor>25.0f)
        return value;
    return std::clamp(value*factor,value,100000.0f);
}
void HookWrath(void* character,float amount){
    if(!originalWrath)return;
    const unsigned n=++wrathEvents;
    float effective=amount;
    auto& rt=dg::runtime::Get();
    const bool match=Local(character);
    if(match)++wrathLocal;
    Probe("WRATH AddWrath",n,character,amount,0.0f,0.0f,match);
    if(match){
        effective=Boost(amount,rt.wrathChargeEnabled.load(),rt.wrathChargeMultiplier.load());
        if(effective>amount)Event("Wrath",++wrathBoosts,amount,effective);
    }
    originalWrath(character,effective);
}
float HookChaos(void* chaosComponent,float amount){
    if(!originalChaos)return 0.0f;
    const unsigned n=++chaosEvents;
    float effective=amount;
    auto& rt=dg::runtime::Get();
    void* owner=nullptr;
    if(chaosComponent)std::memcpy(&owner,static_cast<unsigned char*>(chaosComponent)+0xE8,sizeof(owner));
    const bool match=Local(owner);
    if(match)++chaosLocal;
    Probe("CHAOS AddChaos",n,owner,amount,0.0f,0.0f,match);
    if(match){
        effective=Boost(amount,rt.ultimateChargeEnabled.load(),rt.ultimateChargeMultiplier.load());
        if(effective>amount)Event("Chaos/Ultimate",++chaosBoosts,amount,effective);
    }
    return originalChaos(chaosComponent,effective);
}
float HookSynergy(void* player,float requested){
    if(!originalSynergy)return 0.0f;
    const unsigned n=++synergyEvents;
    float effective=requested;
    auto& rt=dg::runtime::Get();
    const void* local=localPlayer?localPlayer->load(std::memory_order_acquire):nullptr;
    const int relation=SynergyLocalRelation(player,const_cast<void*>(local));
    const bool match=relation>0;
    if(match)++synergyLocal;
    if(relation==2)++synergyOuterReferences;
    if(relation==3)++synergyActorReferences;
    if(!match)++synergyNoVerifiedOwner;
    float previous=0.0f,maximum=0.0f;
    if(player){
        std::memcpy(&previous,static_cast<const unsigned char*>(player)+0x5A4,sizeof(previous));
        std::memcpy(&maximum,static_cast<const unsigned char*>(player)+0x498,sizeof(maximum));
    }
    if(std::isfinite(previous)&&std::isfinite(requested)&&requested>previous)
        ++synergyPositive;
    Probe("SYNERGY SetCurrentSynergy",n,player,requested,previous,maximum,match);
    if(match&&rt.synergyChargeEnabled.load()&&
       std::isfinite(requested)&&requested>=0.0f&&requested<100000.0f){
        if(std::isfinite(previous)&&previous>=0.0f&&previous<requested&&
           std::isfinite(maximum)&&maximum>=previous&&maximum>0.0f&&maximum<100000.0f){
            const float gain=Boost(requested-previous,true,rt.synergyChargeMultiplier.load());
            effective=std::clamp(previous+gain,previous,maximum);
            if(effective>requested)Event("Synergy",++synergyBoosts,requested-previous,effective-previous);
        }else ++synergyRejected;
    }
    // Caller uses returned XMM0. Forward float return unmodified.
    return originalSynergy(player,effective);
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
    constexpr unsigned char wrathSig[]={0x80,0xB9,0x18,0x01,0x00,0x00,0x03};
    constexpr unsigned char chaosSig[]={0x40,0x53,0x48,0x83,0xEC,0x30};
    constexpr unsigned char synergySig[]={0x40,0x53,0x48,0x83,0xEC,0x30,0xF3,0x0F,0x10,0x81};
    InstallNative(base,0x660570,wrathSig,sizeof(wrathSig),reinterpret_cast<void*>(&HookWrath),
                  reinterpret_cast<void**>(&originalWrath),wrathReady,"AddWrath");
    InstallNative(base,0x62E060,chaosSig,sizeof(chaosSig),reinterpret_cast<void*>(&HookChaos),
                  reinterpret_cast<void**>(&originalChaos),chaosReady,"AddChaos");
    InstallNative(base,0x583040,synergySig,sizeof(synergySig),reinterpret_cast<void*>(&HookSynergy),
                  reinterpret_cast<void**>(&originalSynergy),synergyReady,"SetCurrentSynergy");
}
Telemetry GetTelemetry(){
    return {wrathReady.load(),chaosReady.load(),synergyReady.load(),
        wrathBoosts.load(),chaosBoosts.load(),synergyBoosts.load(),
        wrathEvents.load(),chaosEvents.load(),synergyEvents.load()};
}
void Shutdown(){
    char msg[256]{};sprintf_s(msg,
        "Gauge V1.03 totals observed Wrath=%u Chaos=%u Synergy=%u boosted Wrath=%u Chaos=%u Synergy=%u",
        wrathEvents.load(),chaosEvents.load(),synergyEvents.load(),
        wrathBoosts.load(),chaosBoosts.load(),synergyBoosts.load());
    Log(msg);
    sprintf_s(msg,"Gauge V1.06 owner totals: WrathLocal=%u ChaosOwnerLocal=%u SynergyLocal=%u (actorField=%u outer=%u noVerifiedOwner=%u) positive=%u rejected=%u",
        wrathLocal.load(),chaosLocal.load(),synergyLocal.load(),
        synergyActorReferences.load(),synergyOuterReferences.load(),synergyNoVerifiedOwner.load(),
        synergyPositive.load(),synergyRejected.load());
    Log(msg);
}
}
