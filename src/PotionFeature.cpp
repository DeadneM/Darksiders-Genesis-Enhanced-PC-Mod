#include "PotionFeature.h"
#include "LocalPlayerOwnership.h"
#include "PotionPolicy.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <cstdio>
#include <cstring>
namespace dg::potions {
namespace {
using Threshold=float(*)(void*);
using Maximum=int(*)(void*);
using Consume=int(*)(void*,int);
using Add=void(*)(void*,int);
Threshold g_threshold=nullptr;
Consume g_consume=nullptr;
Maximum g_maximum=nullptr;
Add g_add=nullptr;
LogFn g_log=nullptr;
uintptr_t g_base=0;
struct PlayerPotions {
    uintptr_t actor=0,holder=0,requestActor=0,viewActor=0;
    uint32_t generation=0,requestGeneration=0,viewGeneration=0;
    uint64_t observed=0,requestTime=0,viewTime=0;
    Telemetry view{};
};
PlayerPotions g_players[2]{};
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_holds{0},g_rates{0};
SRWLOCK g_lock=SRWLOCK_INIT;

bool Readable(const void* p,size_t n){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!n||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const DWORD access=m.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&n<=m.RegionSize-offset;
}
template<class T> bool Read(const void* p,size_t offset,T& value){
    const auto at=reinterpret_cast<uintptr_t>(p);
    if(!p||at>UINTPTR_MAX-offset)return false;
    const void* field=reinterpret_cast<const void*>(at+offset);
    if(!Readable(field,sizeof(T)))return false;
    std::memcpy(&value,field,sizeof(T));return true;
}
bool Instance(void* object,uintptr_t slot){
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;void* item=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+slot),0,expected)&&expected&&
        Read(object,0x10,actual)&&actual&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(actual,0x38,last)&&last>=index&&Read(actual,0x30,array)&&array&&
        Read(array,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
bool Holder(void* holder,void* actor){
    void* owner=nullptr;unsigned char role=0;
    return Instance(holder,0x38115E0)&&Read(holder,0x5C0,owner)&&owner==actor&&
        Read(holder,0x118,role)&&role==3;
}
bool Context(void* holder){
    if(!g_ready.load(std::memory_order_acquire)||!dg::native_tps::OnGameThread())return false;
    const auto now=GetTickCount64();
    for(unsigned i=0;i<2;++i){
        const auto& player=g_players[i];
        if(!dg::native_tps::CinematicActive(i)&&reinterpret_cast<uintptr_t>(holder)==player.holder&&
           dg::local_players::Matches(i,player.actor,player.generation)&&
           now>=player.observed&&now-player.observed<1500&&
           Holder(holder,reinterpret_cast<void*>(player.actor)))return true;
    }
    return false;
}
float HookThreshold(void* holder){
    const float native=g_threshold(holder);
    const auto& rt=dg::runtime::Get();
    if(!rt.potionRechargeEnabled.load()||!Context(holder))return native;
    const float result=RechargeThreshold(native,true,rt.potionRechargeMultiplier.load());
    if(result!=native){
        const unsigned n=++g_rates;
        if(g_log&&(n<=4||n%1000==0)){
            char line[200]{};sprintf_s(line,"Potion V1.44 RECHARGE n=%u nativeOverflowThreshold=%.4f effective=%.4f multiplier=%.2f noHealthGainChange=1",
                n,native,result,rt.potionRechargeMultiplier.load());g_log(line);
        }
    }
    return result;
}
int HookConsume(void* holder,int amount){
    const uintptr_t caller=reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base;
    const bool unlimited=dg::runtime::Get().unlimitedPotionsEnabled.load()&&
        (caller==0x65BF1B||caller==0x65D78D)&&Context(holder);
    const int effective=ConsumeAmount(amount,unlimited);
    if(effective!=amount){
        const unsigned n=++g_holds;
        if(g_log&&(n<=8||n%250==0)){
            char line[160]{};sprintf_s(line,"Potion V1.44 UNLIMITED n=%u nativeCost=%d effective=%d cooldownAndHealingNative=1",n,amount,effective);g_log(line);
        }
    }
    return g_consume(holder,effective);
}
}
void Initialize(LogFn logger,bool exact){
    g_log=logger;
    if(!exact)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x56DCF0,{0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x6C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57}},
        {0x566ED0,{0x40,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x85,0xD2,0x74,0x17,0xF7,0xDA,0xE8}},
        {0x580390,{0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x20,0x80,0xB9,0x18,0x01,0x00,0x00}},
        {0x56FAF0,{0x48,0x89,0x5C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x20,0x8B}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(g_log)g_log("Potion V1.44 signature mismatch; module OFF");return;
    }
    const auto init=MH_Initialize();if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    const uintptr_t rvas[]={0x56DCF0,0x566ED0};
    void* hooks[]={reinterpret_cast<void*>(&HookThreshold),reinterpret_cast<void*>(&HookConsume)};
    void** originals[]={reinterpret_cast<void**>(&g_threshold),reinterpret_cast<void**>(&g_consume)};
    unsigned installed=0;
    for(unsigned n=0;n<2;++n){
        void* target=reinterpret_cast<void*>(g_base+rvas[n]);
        if(MH_CreateHook(target,hooks[n],originals[n])!=MH_OK)break;
        if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);*originals[n]=nullptr;break;}
        ++installed;
    }
    if(installed!=2){
        for(unsigned n=0;n<installed;++n){void* target=reinterpret_cast<void*>(g_base+rvas[n]);MH_DisableHook(target);MH_RemoveHook(target);}
        if(g_log)g_log("Potion V1.44 hook installation incomplete; module OFF");return;
    }
    g_maximum=reinterpret_cast<Maximum>(g_base+0x56FAF0);
    g_add=reinterpret_cast<Add>(g_base+0x580390);
    g_ready.store(true,std::memory_order_release);
    if(g_log)g_log("Potion V1.44 ready: HEALTH reserve only; native cap/add/overflow threshold and cost; no key-specific patch; EXPERIMENTAL");
}
void OnLivePlayer(void* actor,uint32_t generation,void* livePlayerState){
    if(!g_ready.load(std::memory_order_acquire)||!actor||!dg::native_tps::OnGameThread())return;
    dg::local_players::Slot slot{};
    const int index=dg::local_players::FindCharacter(actor,&slot);
    if(index<0||slot.generation!=generation)return;
    auto& player=g_players[index];
    static thread_local bool processing=false;
    if(processing)return;
    processing=true;
    struct Done {bool& active;~Done(){active=false;}} done{processing};
    const auto now=GetTickCount64();
    if(player.actor==reinterpret_cast<uintptr_t>(actor)&&player.generation==generation&&now>=player.observed&&now-player.observed<100)return;
    void* holder=livePlayerState;void* data=nullptr;int current=0;
    if((!holder&&!Read(actor,0x330,holder))||!Holder(holder,actor)||!Read(holder,0x580,data)||
       !Read(data,0xC,current)||current<0||current>100)return;
    const int maximum=g_maximum(holder);
    if(maximum<=0||maximum>100)return;
    player.actor=reinterpret_cast<uintptr_t>(actor);player.holder=reinterpret_cast<uintptr_t>(holder);
    player.generation=generation;player.observed=now;
    const bool gameplay=!dg::native_tps::CinematicActive(unsigned(index))&&!dg::native_tps::IsWorldPaused(actor);
    bool refill=false;
    AcquireSRWLockExclusive(&g_lock);
    if(player.requestActor){
        if(player.requestActor!=player.actor||player.requestGeneration!=generation||now<player.requestTime||now-player.requestTime>5000){
            player.requestActor=0;player.view.pending=false;player.view.result=Result::Cancelled;
        }else if(gameplay){refill=true;player.requestActor=0;player.view.pending=false;}
    }
    ReleaseSRWLockExclusive(&g_lock);
    const bool unlimited=dg::runtime::Get().unlimitedPotionsEnabled.load();
    if(gameplay&&(refill||unlimited)){
        const int add=RefillAmount(current,maximum);
        if(add)g_add(holder,add);
        int after=-1;const bool ok=Read(data,0xC,after)&&after==maximum;
        if(g_log&&refill){
            char line[220]{};sprintf_s(line,"Potion V1.44 REFILL before=%d max=%d added=%d after=%d matches=%d generation=%u",current,maximum,add,after,ok?1:0,generation);g_log(line);
        }
        if(ok)current=after;
        if(refill){AcquireSRWLockExclusive(&g_lock);player.view.result=ok?Result::Applied:Result::Rejected;if(ok)++player.view.refills;ReleaseSRWLockExclusive(&g_lock);}
    }
    float overflow=0;Read(data,0x10,overflow);
    const float threshold=g_threshold(holder); // unmodified native scale for telemetry
    AcquireSRWLockExclusive(&g_lock);
    player.view.supported=true;player.view.available=true;player.view.current=current;player.view.maximum=maximum;
    player.view.overflow=overflow;player.view.nativeThreshold=threshold;player.viewTime=now;
    player.viewActor=player.actor;player.viewGeneration=generation;
    ReleaseSRWLockExclusive(&g_lock);
}
bool RequestRefill(){
    const auto now=GetTickCount64();bool queued=false;
    AcquireSRWLockExclusive(&g_lock);
    for(unsigned i=0;i<2;++i){
        auto& player=g_players[i];
        if(g_ready.load()&&player.view.available&&!player.requestActor&&player.viewTime&&
           now>=player.viewTime&&now-player.viewTime<1500&&
           dg::local_players::Matches(i,player.viewActor,player.viewGeneration)){
            player.requestActor=player.viewActor;player.requestGeneration=player.viewGeneration;player.requestTime=now;
            player.view.pending=true;player.view.result=Result::Queued;queued=true;
        }
    }
    ReleaseSRWLockExclusive(&g_lock);return queued;
}
void CancelRequest(){
    AcquireSRWLockExclusive(&g_lock);
    for(auto& player:g_players){player.requestActor=0;player.view.pending=false;player.view.result=Result::Cancelled;}
    ReleaseSRWLockExclusive(&g_lock);
}
Telemetry GetTelemetry(unsigned index){
    AcquireSRWLockShared(&g_lock);
    Telemetry t=index<2?g_players[index].view:Telemetry{};const auto now=GetTickCount64();
    t.supported=g_ready.load();
    t.available=index<2&&t.available&&now>=g_players[index].viewTime&&now-g_players[index].viewTime<1500&&
        dg::local_players::Matches(index,g_players[index].viewActor,g_players[index].viewGeneration);
    ReleaseSRWLockShared(&g_lock);t.holds=g_holds.load();t.rateCalls=g_rates.load();return t;
}
void Shutdown(){
    g_ready.store(false);CancelRequest();
    if(g_log){auto t=GetTelemetry(0);t.refills+=GetTelemetry(1).refills;char line[180]{};sprintf_s(line,"Potion V1.44 totals refills=%u unlimitedCosts=%u rechargeThresholds=%u gameplayValidation=USER_REQUIRED",t.refills,t.holds,t.rateCalls);g_log(line);}
}
}
