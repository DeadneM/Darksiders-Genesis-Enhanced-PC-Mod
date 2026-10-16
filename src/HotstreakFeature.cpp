#include "HotstreakFeature.h"
#include "HotstreakPolicy.h"
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <atomic>
#include <cstdio>
#include <cstring>
namespace dg::hotstreak {
namespace {
using RemoveJuice=void(*)(void*,float);
using Maximum=float(*)(void*);
RemoveJuice g_original=nullptr;
AddJuice g_add=nullptr;
Maximum g_maximum=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_modified{0},g_topups{0};
struct Player {refill::Request request{};Telemetry view{};};
Player g_players[2]{};
SRWLOCK g_lock=SRWLOCK_INIT;
bool Readable(const void* p,size_t size){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!size||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const DWORD access=m.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&size<=m.RegionSize-offset;
}
template<class T> bool Read(const void* object,size_t offset,T& value){
    const auto at=reinterpret_cast<uintptr_t>(object);
    if(!object||at>UINTPTR_MAX-offset)return false;
    const void* field=reinterpret_cast<void*>(at+offset);
    if(!Readable(field,sizeof(T)))return false;
    std::memcpy(&value,field,sizeof(T));return true;
}
bool Instance(void* object,uintptr_t slot){
    void* expected=nullptr;void* cls=nullptr;void* tree=nullptr;void* item=nullptr;
    int index=-1,last=-1;
    return Read(reinterpret_cast<void*>(g_base+slot),0,expected)&&expected&&
        Read(object,0x10,cls)&&cls&&Read(expected,0x38,index)&&index>=0&&index<=512&&
        Read(cls,0x38,last)&&last>=index&&last<=512&&Read(cls,0x30,tree)&&tree&&
        Read(tree,size_t(index)*8,item)&&item==static_cast<unsigned char*>(expected)+0x30;
}
bool Context(void* component,void*& actor,void*& resource,unsigned& index,uint32_t& generation){
    unsigned char type=0,role=0;void* reciprocal=nullptr;local_players::Slot slot{};
    if(!g_ready.load()||!native_tps::OnGameThread()||!Instance(component,0x380DB38)||
       !Read(component,0xE8,actor)||!Read(actor,0xCB8,reciprocal)||reciprocal!=component||
       !Read(actor,0xC18,type)||type!=1||!Read(actor,0x118,role)||role!=3||
       !Read(actor,0xEA0,resource)||!Instance(resource,0x3810268)||
       !Read(resource,0x118,role)||role!=3)return false;
    const int found=local_players::FindCharacter(actor,&slot);
    if(found<0||found>1)return false;
    index=unsigned(found);generation=slot.generation;return true;
}
void Remove(void* component,float amount,uintptr_t caller){
    float effective=amount;void* actor=nullptr;void* resource=nullptr;unsigned index=0;uint32_t generation=0;
    const auto& rt=runtime::Get();
    if(DepletionCaller(caller)&&(rt.reduceHotstreakEnabled.load()||rt.unlimitedHotstreakEnabled.load())&&
       Context(component,actor,resource,index,generation)&&!native_tps::CinematicActive(index)&&
       !native_tps::IsWorldPaused(actor))
        effective=DepletionAmount(amount,rt.reduceHotstreakEnabled.load(),rt.hotstreakDecreaseDivisor.load(),rt.unlimitedHotstreakEnabled.load());
    if(effective!=amount&&std::isfinite(effective)){
        const unsigned n=++g_modified;
        if(g_log&&(n<=8||n%1000==0)){char line[240]{};
            sprintf_s(line,"Hotstreak V1.48 DECREASE player=%u n=%u caller=%zX native=%.5f effective=%.5f generation=%u resetsPreserved=1",
                index+1,n,caller,amount,effective,generation);g_log(line);}
    }
    if(g_original)g_original(component,effective); // native depletion, end-of-streak and FX lifecycle
}
void HookRemove(void* component,float amount){Remove(component,amount,reinterpret_cast<uintptr_t>(_ReturnAddress())-g_base);}
}
void Initialize(LogFn log,bool exact,AddJuice nativeAdd){
    g_log=log;if(!exact||!nativeAdd)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x681A40,{0x48,0x89,0x5C,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xEC,0x30,0x48}},
        {0x68AFFC,{0x0F,0x28,0xCE,0x48,0x8B,0xCB,0xE8,0x39,0x6A,0xFF,0xFF,0x0F,0x28,0x74,0x24,0x20}},
        {0x68BA7E,{0x0F,0x28,0xCE,0x48,0x8B,0xCF,0xE8,0xB7,0x5F,0xFF,0xFF,0xB0,0x01,0xC7,0x87,0x24}},
        {0x67E811,{0xF3,0x0F,0x10,0x4B,0x44,0x48,0x8B,0xC8,0xE8,0x22,0x32,0x00,0x00,0x49,0x8B,0xCC}},
        {0x793578,{0x48,0x8B,0xCF,0xE8,0xC0,0xE4,0xEE,0xFF,0x48,0x8B,0x5C,0x24,0x30,0x48,0x83,0xC4}},
        {0x78F980,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xAA,0xE1,0x07,0x03,0x48,0x85}},
        {0x6A9280,{0xF3,0x0F,0x10,0x81,0x70,0x04,0x00,0x00,0xF3,0x0F,0x59,0x81,0x9C,0x03,0x00,0x00}}
    };
    for(const auto& guard:guards)if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
        if(log)log("Hotstreak V1.48 signature mismatch; decrease/refill unavailable");return;
    }
    const auto status=MH_Initialize();if(status!=MH_OK&&status!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x681A40);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookRemove),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    if(MH_EnableHook(target)!=MH_OK){MH_RemoveHook(target);g_original=nullptr;return;}
    g_add=nativeAdd;g_maximum=reinterpret_cast<Maximum>(g_base+0x6A9280);g_ready.store(true);
    if(log)log("Hotstreak V1.48 ready: native decay/spend callers; explicit resets native; typed local Strife; refill via original AddJuice; EXPERIMENTAL");
}
void OnLivePlayer(void* actor,uint32_t generation){
    if(!g_ready.load()||!actor||!native_tps::OnGameThread())return;
    local_players::Slot slot{};const int found=local_players::FindCharacter(actor,&slot);
    if(found<0||slot.generation!=generation)return;
    const unsigned index=unsigned(found);const auto now=GetTickCount64();
    AcquireSRWLockShared(&g_lock);
    const auto& previous=g_players[index].request;
    const bool recent=!previous.requestedActor&&previous.actor==reinterpret_cast<uintptr_t>(actor)&&
        previous.generation==generation&&previous.observed&&now>=previous.observed&&now-previous.observed<100;
    ReleaseSRWLockShared(&g_lock);
    if(recent)return;
    static thread_local bool processing=false;
    if(processing)return;
    processing=true;struct Done {bool& active;~Done(){active=false;}} done{processing};
    void* component=nullptr;void* resource=nullptr;void* owner=nullptr;unsigned checkedIndex=0;uint32_t checkedGeneration=0;
    float current=0,maximum=0,health=0;
    bool available=Read(actor,0xCB8,component)&&Context(component,owner,resource,checkedIndex,checkedGeneration)&&
        checkedIndex==index&&checkedGeneration==generation&&Read(resource,0x398,current)&&
        Read(resource,0x37C,health)&&std::isfinite(health)&&health>0;
    if(available){maximum=g_maximum(resource);available=std::isfinite(current)&&current>=0&&
        std::isfinite(maximum)&&maximum>0&&maximum<=1000000&&current<=maximum;}
    auto& player=g_players[index];bool fill=false;
    const bool gameplay=available&&!native_tps::CinematicActive(index)&&!native_tps::IsWorldPaused(actor);
    AcquireSRWLockExclusive(&g_lock);
    // Publish numbers and identities only; all native work stays on this thread.
    player.request.Observe(reinterpret_cast<uintptr_t>(actor),generation,now,available);
    fill=player.request.Take(reinterpret_cast<uintptr_t>(actor),generation,now,gameplay);
    ReleaseSRWLockExclusive(&g_lock);
    bool ok=false;
    if(gameplay&&(fill||runtime::Get().unlimitedHotstreakEnabled.load())){
        if(current<maximum){g_add(component,maximum-current);++g_topups;}
        float after=0;ok=Read(resource,0x398,after)&&std::isfinite(after)&&std::fabs(after-maximum)<=0.001f;
        if(ok)current=after;
        if(g_log&&fill){char line[200]{};sprintf_s(line,"Hotstreak V1.48 REFILL player=%u max=%.3f after=%.3f matches=%d generation=%u",index+1,maximum,after,ok?1:0,generation);g_log(line);}
    }
    AcquireSRWLockExclusive(&g_lock);
    player.view.current=current;player.view.maximum=maximum;
    if(fill){player.request.Complete(ok);if(ok)++player.view.refills;}
    ReleaseSRWLockExclusive(&g_lock);
}
bool RequestRefill(){
    const auto now=GetTickCount64();bool queued=false;AcquireSRWLockExclusive(&g_lock);
    for(unsigned i=0;i<2;++i){auto& request=g_players[i].request;
        if(g_ready.load()&&local_players::Matches(i,request.actor,request.generation))queued=request.Queue(now)||queued;
    }
    ReleaseSRWLockExclusive(&g_lock);return queued;
}
Telemetry GetTelemetry(unsigned index){
    Telemetry view{};AcquireSRWLockExclusive(&g_lock);
    if(index<2){auto& player=g_players[index];player.request.Expire(GetTickCount64());view=player.view;
        const bool matches=local_players::Matches(index,player.request.actor,player.request.generation);
        if(!matches&&player.request.requestedActor)player.request.Cancel();
        view.available=player.request.Fresh(GetTickCount64())&&matches;
        view.pending=player.request.requestedActor!=0;view.result=player.request.result;}
    ReleaseSRWLockExclusive(&g_lock);view.supported=g_ready.load();view.modified=g_modified.load();view.topups=g_topups.load();return view;
}
void Shutdown(){
    g_ready.store(false);AcquireSRWLockExclusive(&g_lock);for(auto& player:g_players)player.request.Cancel();ReleaseSRWLockExclusive(&g_lock);
    if(g_log){char line[160]{};sprintf_s(line,"Hotstreak V1.48 totals decreases=%u topups=%u gameplayValidation=USER_REQUIRED",g_modified.load(),g_topups.load());g_log(line);}
}
}
