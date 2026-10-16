#include "AmmoConsumptionFeature.h"
#include "LocalPlayerOwnership.h"
#include "AmmoConsumptionPolicy.h"
#include "RuntimeSettings.h"
#include "NativeTpsFeature.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <cstdio>
#include <cstring>
namespace dg::ammo {
namespace {
using Setter=int(*)(void*,int);
using Maximum=int(*)(void*);
Setter g_original=nullptr;
Maximum g_maximum=nullptr;
LogFn g_log=nullptr;
uintptr_t g_base=0;

std::atomic_bool g_ready{false};
std::atomic_uint32_t g_observed{0},g_spends{0},g_modified{0},g_topups{0};
// Sampled game thread only. Stored UObject identities are compared, never read.
struct Entry {uintptr_t holder=0;Credit credit{};};
struct PlayerAmmo {
    uintptr_t actor=0,resource=0;uint32_t generation=0;uint64_t tick=0;
    Entry entries[16]{};
};
PlayerAmmo g_players[2]{};
struct PublishedReserve {refill::Request request{};Reserve view{};};
PublishedReserve g_reserves[2]{};
SRWLOCK g_refillLock=SRWLOCK_INIT;
bool Readable(const void* p,size_t size){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!size||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto access=m.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&size<=m.RegionSize-offset;
}
template<class T> bool Read(const void* p,size_t offset,T& value){
    const auto address=reinterpret_cast<uintptr_t>(p);
    if(!p||address>UINTPTR_MAX-offset)return false;
    const void* at=reinterpret_cast<const void*>(address+offset);
    if(!Readable(at,sizeof(T)))return false;
    std::memcpy(&value,at,sizeof(T));return true;
}
bool Instance(void* object,uintptr_t slot){
    void* expected=nullptr;void* cls=nullptr;void* array=nullptr;void* entry=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+slot),0,expected)&&expected&&
        Read(object,0x10,cls)&&cls&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(cls,0x38,last)&&last>=index&&Read(cls,0x30,array)&&array&&
        Read(array,size_t(index)*8,entry)&&entry==static_cast<unsigned char*>(expected)+0x30;
}
bool Authority(void* object){unsigned char role=0;return Read(object,0x118,role)&&role==3;}
int Set(void* proxy,int requested,uintptr_t caller){
    ++g_observed;
    auto& rt=dg::runtime::Get();
    const bool reduce=rt.reduceAmmoEnabled.load(),unlimited=rt.unlimitedAmmoEnabled.load();
    if(!g_original)return requested;
    if(!dg::native_tps::OnGameThread())return g_original(proxy,requested);
    if(!reduce&&!unlimited){
        for(auto& player:g_players)for(auto& entry:player.entries)entry.credit={};
        return g_original(proxy,requested);
    }
    PlayerAmmo* selected=nullptr;unsigned playerIndex=0;
    for(unsigned i=0;i<2&&!selected;++i){
        auto& candidate=g_players[i];
        if(!dg::local_players::Matches(i,candidate.actor,candidate.generation))continue;
        for(const auto& entry:candidate.entries)
            if(entry.holder==reinterpret_cast<uintptr_t>(proxy)){selected=&candidate;playerIndex=i;break;}
    }
    if(!selected)return g_original(proxy,requested);
    auto& player=*selected;
    const uint64_t now=GetTickCount64();
    const auto local=player.actor;
    // Exact native weapon AUTH_SetAmmo -> proxy setter. Excludes ammo rewards,
    // auto recharge, initialization and manual top-ups from spend arithmetic.
    if(caller!=0x697D92||!local||!player.tick||now<player.tick||now-player.tick>=500||
       dg::native_tps::CinematicActive(playerIndex))
        return g_original(proxy,requested);
    Entry* state=nullptr;
    for(auto& entry:player.entries)if(entry.holder==reinterpret_cast<uintptr_t>(proxy)){state=&entry;break;}
    uintptr_t owner=0;int before=0;
    if(!state||!Instance(proxy,0x3811D40)||!Authority(proxy)||
       !Read(proxy,0x108,owner)||owner!=player.resource||!Read(proxy,0x34C,before))
        return g_original(proxy,requested);
    const int effective=ConsumptionRequest(state->credit,local,reinterpret_cast<uintptr_t>(proxy),
        player.generation,before,requested,reduce,rt.ammoConsumptionDivisor.load(),unlimited);
    const bool spend=before>=0&&requested>=0&&requested<before;
    const unsigned n=spend?++g_spends:0;
    if(effective!=requested)++g_modified;
    const int actual=g_original(proxy,effective); // native cap, authority and integer RAX return
    if(g_log&&spend&&(n<=12||n%250==0)){
        char line[300]{};
        sprintf_s(line,"Ammo V1.39 SPEND n=%u before=%d nativeRequested=%d effective=%d actual=%d divisor=%.3f reduce=%d unlimited=%d proxy=%p generation=%u gameplay=EXPERIMENTAL",
            n,before,requested,effective,actual,rt.ammoConsumptionDivisor.load(),reduce?1:0,
            unlimited?1:0,proxy,player.generation);g_log(line);
    }
    return actual;
}
int HookSet(void* proxy,int requested){
    return Set(proxy,requested,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);
}
}
void Initialize(LogFn logger,bool exactExecutable){
    g_log=logger;
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x697E70,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x80,0xB9,0x18,0x01,0x00,0x00}},
        {0x6A90D0,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x30,0x48,0x8B,0x99,0x08,0x01,0x00}},
        {0x7CBC20,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x12,0x61,0x04,0x03,0x48,0x85}},
        {0x697D88,{0x8B,0xD6,0x48,0x8B,0xCF,0xE8,0xDE,0x00,0x00,0x00,0x48,0x8B,0x03,0x48,0x8B,0xCB}},
        {0x6B82A0,{0x48,0x8B,0xBB,0x28,0x04,0x00,0x00,0x48,0x63,0x83,0x30,0x04,0x00,0x00,0x48,0x8D}},
        {0x7AFA60,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x02,0x08,0x06,0x03,0x48,0x85}}
    };
    for(const auto& guard:guards)
        if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
            if(g_log)g_log("Ammo V1.39 signature mismatch; native consumption retained");return;
        }
    int32_t displacement=0;Read(reinterpret_cast<void*>(g_base+0x697D8D),1,displacement);
    if(intptr_t(0x697D92)+displacement!=0x697E70)return;
    const auto init=MH_Initialize();
    if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x697E70);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookSet),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    const auto status=MH_EnableHook(target);
    if(status!=MH_OK&&status!=MH_ERROR_ENABLED){MH_RemoveHook(target);g_original=nullptr;return;}
    g_maximum=reinterpret_cast<Maximum>(g_base+0x6A90D0);g_ready.store(true,std::memory_order_release);
    if(g_log)g_log("Ammo V1.39 ready; exact native weapon spend caller, typed live local owned proxies, native finite maximum; OFF=original; EXPERIMENTAL");
}
void OnLivePlayer(void* actor,uint32_t generation){
    if(!g_ready.load()||!actor||!dg::native_tps::OnGameThread())return;
    dg::local_players::Slot slot{};
    const int index=dg::local_players::FindCharacter(actor,&slot);
    if(index<0||slot.generation!=generation)return;
    auto& player=g_players[index];
    const auto now=GetTickCount64();
    const uintptr_t identity=reinterpret_cast<uintptr_t>(actor);
    AcquireSRWLockShared(&g_refillLock);
    const bool pending=g_reserves[index].request.requestedActor!=0;
    ReleaseSRWLockShared(&g_refillLock);
    if(!pending&&identity==player.actor&&generation==player.generation&&player.tick&&now>=player.tick&&now-player.tick<100)return;
    void* resource=nullptr;void* array=nullptr;int count=0;
    if(!Read(actor,0xEA0,resource)||!Instance(resource,0x3810268)||!Authority(resource)||
       !Read(resource,0x428,array)||!Read(resource,0x430,count)||count<0||count>16||
       (count&&!Readable(array,size_t(count)*8))){
        player.tick=0;AcquireSRWLockExclusive(&g_refillLock);
        g_reserves[index].request.Observe(identity,generation,now,false);
        ReleaseSRWLockExclusive(&g_refillLock);return;
    }
    if(identity!=player.actor||generation!=player.generation||reinterpret_cast<uintptr_t>(resource)!=player.resource)
        for(auto& entry:player.entries)entry={};
    player.actor=identity;player.resource=reinterpret_cast<uintptr_t>(resource);player.generation=generation;player.tick=now;
    const bool gameplay=!dg::native_tps::CinematicActive(unsigned(index))&&!dg::native_tps::IsWorldPaused(actor);
    AcquireSRWLockExclusive(&g_refillLock);
    auto& published=g_reserves[index];
    published.request.Observe(identity,generation,now,true);
    const bool manual=published.request.Take(identity,generation,now,gameplay);
    ReleaseSRWLockExclusive(&g_refillLock);
    const bool fill=gameplay&&(manual||dg::runtime::Get().unlimitedAmmoEnabled.load());
    unsigned finite=0;bool complete=true;
    Entry next[16]{};
    for(int i=0;i<count;++i){
        void* proxy=nullptr;uintptr_t owner=0;int current=-1;
        if(!Read(array,size_t(i)*8,proxy)||!Instance(proxy,0x3811D40)||!Authority(proxy)||
           !Read(proxy,0x108,owner)||owner!=player.resource||!Read(proxy,0x34C,current))continue;
        next[i].holder=reinterpret_cast<uintptr_t>(proxy);
        for(const auto& old:player.entries)if(old.holder==next[i].holder){next[i].credit=old.credit;break;}
        if(current<0)continue; // native infinite/default ammunition stays native
        const int maximum=g_maximum(proxy);
        if(maximum<=0||maximum>1000000)continue;
        ++finite;
        if(!fill||current>=maximum)continue;
        const int after=g_original(proxy,maximum);
        int readback=-1;
        if(!Read(proxy,0x34C,readback)||readback!=maximum||after!=maximum)complete=false;
        const unsigned n=++g_topups;
        if(g_log&&(n<=8||n%1000==0)){char line[220]{};
            sprintf_s(line,"Ammo V1.39 UNLIMITED_TOPUP n=%u before=%d after=%d nativeMax=%d proxy=%p generation=%u readbackMatches=%d",
                n,current,after,maximum,proxy,generation,after==maximum?1:0);g_log(line);}
    }
    for(unsigned i=0;i<16;++i)player.entries[i]=next[i];
    AcquireSRWLockExclusive(&g_refillLock);
    published.request.available=finite!=0;published.view.finiteReserves=finite;
    if(manual){published.request.Complete(finite!=0&&complete);if(finite&&complete)++published.view.refills;}
    ReleaseSRWLockExclusive(&g_refillLock);
    if(g_log&&manual){char line[200]{};
        sprintf_s(line,"Ammo V1.48 REFILL player=%d finiteReserves=%u matches=%d generation=%u",index+1,finite,finite&&complete?1:0,generation);g_log(line);}
}
Telemetry GetTelemetry(){return {g_ready.load(),g_observed.load(),g_spends.load(),g_modified.load(),g_topups.load()};}
bool RequestRefill(){
    bool queued=false;const auto now=GetTickCount64();AcquireSRWLockExclusive(&g_refillLock);
    for(unsigned i=0;i<2;++i){auto& request=g_reserves[i].request;
        if(g_ready.load()&&dg::local_players::Matches(i,request.actor,request.generation))queued=request.Queue(now)||queued;
    }
    ReleaseSRWLockExclusive(&g_refillLock);return queued;
}
Reserve GetReserve(unsigned index){
    Reserve view{};AcquireSRWLockExclusive(&g_refillLock);
    if(index<2){auto& published=g_reserves[index];published.request.Expire(GetTickCount64());view=published.view;
        const bool matches=dg::local_players::Matches(index,published.request.actor,published.request.generation);
        if(!matches&&published.request.requestedActor)published.request.Cancel();
        view.available=g_ready.load()&&published.request.Fresh(GetTickCount64())&&matches;
        view.pending=published.request.requestedActor!=0;view.result=published.request.result;}
    ReleaseSRWLockExclusive(&g_refillLock);return view;
}
void Shutdown(){
    AcquireSRWLockExclusive(&g_refillLock);for(auto& published:g_reserves)published.request.Cancel();ReleaseSRWLockExclusive(&g_refillLock);
    if(g_log){const auto t=GetTelemetry();char line[220]{};
    sprintf_s(line,"Ammo V1.39 totals ready=%d setters=%u spends=%u modified=%u topups=%u gameplayValidation=USER_REQUIRED",
        t.ready?1:0,t.observed,t.spends,t.modified,t.topups);g_log(line);}}
}
