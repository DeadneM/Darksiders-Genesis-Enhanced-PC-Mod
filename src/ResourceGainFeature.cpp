#include "ResourceGainFeature.h"
#include "ResourceGainPolicy.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include "ManualGaugeFeature.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <initializer_list>
#include <cstdio>
#include <cstring>
namespace dg::resource_gain {
namespace {
using Setter=float(*)(void*,float);
Setter g_original[2]{};
std::atomic_bool g_ready[2]{};
std::atomic_uint32_t g_observed[2]{},g_positive[2]{},g_boosted[2]{};
LogFn g_log=nullptr;
uintptr_t g_base=0;
std::atomic<void*>* g_local=nullptr;
// Written/read on the sampled game thread only. Identities are compared;
// cached actor/resource pointers are NEVER dereferenced.
uintptr_t g_actor=0,g_resource=0;
uint32_t g_generation=0;
uint64_t g_observedAt=0;
bool Readable(const void* p,size_t count){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const DWORD access=m.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&
       access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&count<=m.RegionSize-offset;
}
template<class T> bool Read(const void* object,size_t offset,T& value){
    const auto at=reinterpret_cast<uintptr_t>(object);
    if(!object||at>UINTPTR_MAX-offset)return false;
    const void* p=reinterpret_cast<const void*>(at+offset);
    if(!Readable(p,sizeof(T)))return false;
    std::memcpy(&value,p,sizeof(T));return true;
}
bool ResourceType(void* resource){
    void* cls=nullptr;void* actual=nullptr;void* array=nullptr;void* entry=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+0x3810268),0,cls)&&cls&&
        Read(resource,0x10,actual)&&actual&&Read(cls,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*sizeof(void*),entry)&&entry==static_cast<unsigned char*>(cls)+0x30;
}
float Set(unsigned gauge,void* resource,float requested,uintptr_t caller){
    if(!g_original[gauge])return requested;
    ++g_observed[gauge];
    float before=0,maximum=0,bonus=0;
    float effective=requested;
    bool eligible=false;
    const auto now=GetTickCount64();
    auto& rt=dg::runtime::Get();
    // Native reward callsites only. This categorically excludes manual
    // refill and the old Add hooks, preventing both double boosts and any
    // change to the user's validated explicit gauge percentages.
    if(RewardCaller(gauge,caller)&&dg::native_tps::OnGameThread()&&
       now>=g_observedAt&&now-g_observedAt<500&&g_actor&&
       g_local&&reinterpret_cast<uintptr_t>(g_local->load(std::memory_order_acquire))==g_actor&&
       rt.tpsActorGeneration.load()==g_generation&&
       reinterpret_cast<uintptr_t>(resource)==g_resource&&ResourceType(resource)&&
       !dg::native_tps::CinematicActive()){
        unsigned char role=0;
        const size_t current=gauge==0?0x3A0:0x3A8,cap=gauge==0?0x3A4:0x3AC;
        eligible=Read(resource,0x118,role)&&role==3&&Read(resource,current,before)&&
            Read(resource,cap,maximum)&&(gauge!=0||Read(resource,0x44C,bonus));
        maximum+=bonus;
        if(eligible){
            const bool enabled=gauge==0?rt.wrathChargeEnabled.load():rt.ultimateChargeEnabled.load();
            const float multiplier=gauge==0?rt.wrathChargeMultiplier.load():rt.ultimateChargeMultiplier.load();
            effective=RewardRequest(before,requested,maximum,enabled,multiplier);
        }
    }
    const bool positive=eligible&&std::isfinite(before)&&std::isfinite(requested)&&requested>before;
    const unsigned n=positive?++g_positive[gauge]:0;
    if(effective!=requested&&std::isfinite(effective))++g_boosted[gauge];
    effective=dg::manual_gauges::UnlimitedRequest(gauge==0?dg::manual_gauges::Wrath:
        dg::manual_gauges::Ultimate,resource,effective);
    const float result=g_original[gauge](resource,effective); // retain float XMM0 ABI, clamp and RPC
    if(g_log&&positive&&(n<=12||n%250==0)){
        char line[360]{};
        sprintf_s(line,"Gauge V1.37 NATIVE_REWARD gauge=%s n=%u callerRVA=0x%llX before=%.4f nativeRequested=%.4f effective=%.4f actual=%.4f max=%.4f actor=%p resource=%p manual=0 gameplay=EXPERIMENTAL",
            gauge==0?"WRATH":"ULTIMATE",n,static_cast<unsigned long long>(caller),
            before,requested,effective,result,maximum,reinterpret_cast<void*>(g_actor),resource);
        g_log(line);
    }
    return result;
}
float HookWrath(void* resource,float requested){
    return Set(0,resource,requested,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
float HookChaos(void* resource,float requested){
    return Set(1,resource,requested,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
}
void Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable){
    g_log=logger;g_local=localPlayer;
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const unsigned char entry[]={0x40,0x53,0x48,0x83,0xEC,0x40,0x0F,0x29,0x74,0x24,0x30,0x48,0x8B,0xD9,0x0F,0x29};
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x7AFB10,{0x48,0x83,0xEC,0x28,0x48,0x8B,0x05,0x4D,0x07,0x06,0x03,0x48,0x85,0xC0,0x75,0x1A}},
        {0x6BA52E,{0xF3,0x0F,0x10,0xB1,0x4C,0x04,0x00,0x00,0xF3,0x0F,0x58,0xB1,0xA4,0x03,0x00,0x00}},
        {0x6BA2BE,{0xF3,0x0F,0x10,0xB1,0xAC,0x03,0x00,0x00,0xF3,0x0F,0x5D,0xF7,0xF3,0x0F,0x10,0x81}},
        {0x6EA9B4,{0xF3,0x41,0x0F,0x10,0x8D,0xA0,0x03,0x00,0x00,0xF3,0x0F,0x58,0xCA,0x49,0x8B,0xCD}},
        {0x6EAB1A,{0xF3,0x0F,0x10,0x8B,0xA0,0x03,0x00,0x00,0xF3,0x0F,0x58,0xC8,0x48,0x8B,0xCB,0xE8}},
        {0x67EB47,{0xF3,0x0F,0x58,0x8D,0xA8,0x03,0x00,0x00,0xE8,0x4C,0xB7,0x03,0x00,0x0F,0xB6,0x43}},
        {0x67EB7F,{0xF3,0x0F,0x58,0x8D,0xA8,0x03,0x00,0x00,0xE8,0x14,0xB7,0x03,0x00,0x48,0x8D,0x8E}}
    };
    for(const auto& guard:guards)
        if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
            if(g_log)g_log("Gauge V1.37 native reward signature mismatch; module OFF");return;
        }
    for(const auto call:{uintptr_t(0x6EA9C4),uintptr_t(0x6EAB29),uintptr_t(0x67EB4F),uintptr_t(0x67EB87)}){
        const auto* code=reinterpret_cast<const unsigned char*>(g_base+call);
        int32_t displacement=0;std::memcpy(&displacement,code+1,4);
        const auto target=static_cast<uintptr_t>(static_cast<intptr_t>(call+5)+displacement);
        if(code[0]!=0xE8||target!=((call==0x6EA9C4||call==0x6EAB29)?0x6BA510:0x6BA2A0)){
            if(g_log)g_log("Gauge V1.37 native reward call target mismatch; module OFF");return;
        }
    }
    const auto status=MH_Initialize();
    if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED)return;
    const uintptr_t rvas[2]={0x6BA510,0x6BA2A0};
    void* hooks[2]={reinterpret_cast<void*>(&HookWrath),reinterpret_cast<void*>(&HookChaos)};
    for(unsigned i=0;i<2;++i){
        void* target=reinterpret_cast<void*>(g_base+rvas[i]);
        if(std::memcmp(target,entry,sizeof(entry)))continue;
        if(MH_CreateHook(target,hooks[i],reinterpret_cast<void**>(&g_original[i]))!=MH_OK)continue;
        const auto enabled=MH_EnableHook(target);
        if(enabled!=MH_OK&&enabled!=MH_ERROR_ENABLED){MH_RemoveHook(target);g_original[i]=nullptr;continue;}
        g_ready[i].store(true);
    }
    if(g_log){char line[260]{};
        sprintf_s(line,"Gauge V1.37 native reward ready Wrath=%d Ultimate=%d; pickup/combat callsite+live local resource+authority; manual/spends/upgrades excluded; EXPERIMENTAL",
            g_ready[0].load()?1:0,g_ready[1].load()?1:0);g_log(line);}
}
void OnLivePlayer(void* actor,uint32_t generation){
    if((!g_ready[0].load()&&!g_ready[1].load())||!actor||!g_local||
       g_local->load()!=actor||!dg::native_tps::OnGameThread())return;
    void* resource=nullptr;
    if(Read(actor,0xEA0,resource)&&ResourceType(resource)){
        g_actor=reinterpret_cast<uintptr_t>(actor);g_resource=reinterpret_cast<uintptr_t>(resource);
        g_generation=generation;g_observedAt=GetTickCount64();
    }else{g_actor=0;g_resource=0;g_observedAt=0;}
}
Telemetry GetTelemetry(){
    Telemetry t{};
    for(unsigned i=0;i<2;++i){t.ready[i]=g_ready[i].load();t.observed[i]=g_observed[i].load();
        t.positive[i]=g_positive[i].load();t.boosted[i]=g_boosted[i].load();}
    return t;
}
void Shutdown(){
    if(g_log){const auto t=GetTelemetry();char line[260]{};
        sprintf_s(line,"Gauge V1.37 NATIVE_REWARD totals Wrath(setter=%u positive=%u modified=%u) Ultimate(setter=%u positive=%u modified=%u) manualExcluded=1 gameplayValidation=USER_REQUIRED",
            t.observed[0],t.positive[0],t.boosted[0],t.observed[1],t.positive[1],t.boosted[1]);g_log(line);}
}
}
