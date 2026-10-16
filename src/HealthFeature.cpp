#include "HealthFeature.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dg::health {
namespace {
using Setter=float(*)(void*,float);
using Maximum=float(*)(void*,bool);
Setter g_original=nullptr;
Maximum g_maximum=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_holds{0};
SRWLOCK g_lock=SRWLOCK_INIT;
struct Player {
    uintptr_t actor=0,resource=0,requestActor=0;
    uint32_t generation=0,requestGeneration=0;
    uint64_t tick=0,requestTick=0;
    Telemetry view{};
};
Player g_players[2]{};
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
template<class T> bool Read(void* object,size_t offset,T& value){
    if(!object||reinterpret_cast<uintptr_t>(object)>UINTPTR_MAX-offset)return false;
    const auto* p=static_cast<unsigned char*>(object)+offset;
    if(!Readable(p,sizeof(value)))return false;
    std::memcpy(&value,p,sizeof(value));return true;
}
bool Resource(void* object){
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;void* entry=nullptr;
    int index=-1,last=-1;unsigned char role=0;
    return Read(reinterpret_cast<void*>(g_base+0x3810268),0,expected)&&expected&&
        Read(object,0x10,actual)&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&last<=512&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,entry)&&entry==static_cast<unsigned char*>(expected)+0x30&&
        Read(object,0x118,role)&&role==3;
}
bool Values(void* resource,float& current,float& maximum){
    if(!Resource(resource)||!Read(resource,0x37C,current)||!std::isfinite(current)||current<=0)return false;
    maximum=g_maximum(resource,false); // same native cap used by SetCurrentHealth
    return std::isfinite(maximum)&&maximum>0&&maximum<=1e9f&&current<=maximum+0.01f;
}
float HookSet(void* resource,float requested){
    if(!g_original)return requested;
    if(!g_ready.load()||!runtime::Get().unlimitedHealthEnabled.load()||
       !native_tps::OnGameThread()||!std::isfinite(requested))return g_original(resource,requested);
    const auto now=GetTickCount64();
    for(unsigned i=0;i<2;++i){
        const auto& player=g_players[i];
        if(player.resource!=reinterpret_cast<uintptr_t>(resource)||!player.tick||now<player.tick||now-player.tick>=500||
           !local_players::Matches(i,player.actor,player.generation)||native_tps::CinematicActive(i))continue;
        float current=0,maximum=0;
        if(Values(resource,current,maximum)&&requested<current){
            requested=maximum;const auto n=++g_holds;
            if(g_log&&(n<=6||n%1000==0)){char line[180]{};
                sprintf_s(line,"Health V1.46 UNLIMITED player=%u n=%u current=%.2f nativeMax=%.2f holder=%p",i+1,n,current,maximum,resource);g_log(line);}
        }
        break;
    }
    return g_original(resource,requested);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x6BA320,{0x40,0x53,0x48,0x83,0xEC,0x50,0x0F,0x29,0x74,0x24,0x40,0x33,0xD2,0x0F,0x29,0x7C}},
        {0x6A9200,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x30,0x0F,0xB6,0xFA,0x48,0x8B,0xD9}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Health V1.46 signatures mismatch; module OFF");return;
    }
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x6BA320);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookSet),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    g_maximum=reinterpret_cast<Maximum>(g_base+0x6A9200);g_ready.store(true,std::memory_order_release);
    if(log)log("Health V1.46 READY: two typed local resources, native setter/cap, living players only; EXPERIMENTAL");
}
void OnLivePlayer(void* actor,uint32_t generation){
    if(!g_ready.load()||!actor||!native_tps::OnGameThread())return;
    local_players::Slot slot{};const int index=local_players::FindCharacter(actor,&slot);
    if(index<0||slot.generation!=generation)return;
    static thread_local bool processing=false;if(processing)return;processing=true;
    struct Done {bool& value;~Done(){value=false;}}done{processing};
    auto& player=g_players[index];const auto now=GetTickCount64();
    bool pending=false;AcquireSRWLockShared(&g_lock);pending=player.view.pending;ReleaseSRWLockShared(&g_lock);
    if(!pending&&player.actor==reinterpret_cast<uintptr_t>(actor)&&player.generation==generation&&
       player.tick&&now>=player.tick&&now-player.tick<100)return;
    void* resource=nullptr;float current=0,maximum=0;
    if(!Read(actor,0xEA0,resource)||!Values(resource,current,maximum)){
        AcquireSRWLockExclusive(&g_lock);player.tick=0;player.view.available=false;
        player.view.pending=false;player.requestActor=0;ReleaseSRWLockExclusive(&g_lock);return;
    }
    const bool gameplay=!native_tps::CinematicActive(unsigned(index))&&!native_tps::IsWorldPaused(actor);
    bool refill=false;
    AcquireSRWLockExclusive(&g_lock);
    if(player.requestActor){
        if(player.requestActor!=reinterpret_cast<uintptr_t>(actor)||player.requestGeneration!=generation||
           now<player.requestTick||now-player.requestTick>5000){player.requestActor=0;player.view.pending=false;}
        else if(gameplay){refill=true;player.requestActor=0;player.view.pending=false;}
    }
    player.actor=reinterpret_cast<uintptr_t>(actor);player.resource=reinterpret_cast<uintptr_t>(resource);
    player.generation=generation;player.tick=now;
    ReleaseSRWLockExclusive(&g_lock);
    bool applied=false;
    if(gameplay&&(refill||runtime::Get().unlimitedHealthEnabled.load())&&current<maximum){
        const float after=g_original(resource,maximum);
        applied=std::isfinite(after)&&std::fabs(after-maximum)<0.01f;
        if(applied)current=after;
    }else if(refill)applied=current==maximum;
    AcquireSRWLockExclusive(&g_lock);
    player.view.supported=true;player.view.available=true;player.view.current=current;player.view.maximum=maximum;
    if(refill&&applied)++player.view.refills;
    ReleaseSRWLockExclusive(&g_lock);
    if(refill&&g_log){char line[180]{};sprintf_s(line,"Health V1.46 REFILL player=%d actor=%p current=%.2f max=%.2f nativeReadback=%d",index+1,actor,current,maximum,applied?1:0);g_log(line);}
}
bool RequestRefill(){
    const auto now=GetTickCount64();bool queued=false;
    AcquireSRWLockExclusive(&g_lock);
    for(unsigned i=0;i<2;++i){auto& p=g_players[i];
        if(p.view.available&&!p.requestActor&&p.tick&&now>=p.tick&&now-p.tick<1500&&local_players::Matches(i,p.actor,p.generation)){
            p.requestActor=p.actor;p.requestGeneration=p.generation;p.requestTick=now;p.view.pending=true;queued=true;
        }
    }
    ReleaseSRWLockExclusive(&g_lock);return queued;
}
Telemetry GetTelemetry(unsigned index){
    if(index>1)return {};
    AcquireSRWLockShared(&g_lock);const auto p=g_players[index];ReleaseSRWLockShared(&g_lock);
    auto view=p.view;view.supported=g_ready.load();const auto now=GetTickCount64();
    view.available=view.available&&p.tick&&now>=p.tick&&now-p.tick<1500&&local_players::Matches(index,p.actor,p.generation);
    return view;
}
void Shutdown(){
    g_ready=false;if(g_log){char line[150]{};sprintf_s(line,"Health V1.46 totals refills=%u holds=%u gameplayValidation=USER_REQUIRED",GetTelemetry(0).refills+GetTelemetry(1).refills,g_holds.load());g_log(line);}
}
}
