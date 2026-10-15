#include "ManualGaugeFeature.h"
#include "NativeTpsFeature.h"
#include <windows.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::manual_gauges {
namespace {
using Setter=float(*)(void*,float); // native setters return the new value in XMM0
Setter g_wrath=nullptr,g_chaos=nullptr,g_synergy=nullptr;
LogFn g_log=nullptr;
std::atomic<void*>* g_local=nullptr;
uintptr_t g_base=0;
std::atomic_bool g_enabled{false};
SRWLOCK g_lock=SRWLOCK_INIT;
Telemetry g_state{};
Request g_request{};

bool Readable(const void* ptr,size_t size){
    if(!ptr||!size)return false;
    MEMORY_BASIC_INFORMATION info{};
    if(!VirtualQuery(ptr,&info,sizeof(info))||info.State!=MEM_COMMIT||
       (info.Protect&(PAGE_NOACCESS|PAGE_GUARD)))return false;
    const auto access=info.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&
       access!=PAGE_EXECUTE_WRITECOPY)return false;
    const uintptr_t offset=reinterpret_cast<uintptr_t>(ptr)-
        reinterpret_cast<uintptr_t>(info.BaseAddress);
    return offset<=info.RegionSize&&size<=info.RegionSize-offset;
}
template<class T> bool Read(const void* object,size_t offset,T& value){
    const auto address=reinterpret_cast<uintptr_t>(object);
    if(!object||address>UINTPTR_MAX-offset)return false;
    const void* field=reinterpret_cast<const void*>(address+offset);
    if(!Readable(field,sizeof(T)))return false;
    std::memcpy(&value,field,sizeof(T));
    return true;
}
bool IsInstance(void* object,uintptr_t classSlot){
    void* expected=nullptr;void* actual=nullptr;void* array=nullptr;
    int index=-1,last=-1;
    if(!Read(reinterpret_cast<void*>(g_base+classSlot),0,expected)||!expected||
       !Read(object,0x10,actual)||!actual||
       !Read(expected,0x38,index)||index<0||index>512||
       !Read(actual,0x38,last)||last<index||
       !Read(actual,0x30,array)||!array)return false;
    void* item=nullptr;
    return Read(array,size_t(index)*sizeof(void*),item)&&
        item==static_cast<unsigned char*>(expected)+0x30;
}
bool Authority(void* object){
    unsigned char role=0;
    return Read(object,0x118,role)&&role==3;
}
bool Signature(uintptr_t rva,const unsigned char (&expected)[16]){
    return !std::memcmp(reinterpret_cast<void*>(g_base+rva),expected,16);
}
struct Sample {
    Telemetry view;
    void* objects[3]{}; // synchronous live references, never published or cached
};
Sample SamplePlayer(void* actor,uint32_t generation,uint64_t now){
    Sample sample{};
    sample.view.supported=true;
    sample.view.actor=reinterpret_cast<uintptr_t>(actor);
    sample.view.generation=generation;
    sample.view.observedAt=now;
    void* form=nullptr;
    if(Read(actor,0xE78,form))sample.view.formActive=form!=nullptr;
    void* resources=nullptr;
    // Native AddWrath/GetMaxWrath/AddChaos all use this exact live chain.
    if(Read(actor,0xEA0,resources)&&IsInstance(resources,0x3810268)){
        float wrath=0,baseMax=0,bonus=0,chaos=0,maxChaos=0,target=0;
        if(Read(resources,0x3A0,wrath)&&Read(resources,0x3A4,baseMax)&&
           Read(resources,0x44C,bonus)&&std::isfinite(wrath)&&
           TargetValue(baseMax+bonus,100,target)){
            sample.view.values[0]={wrath,target,true};
            sample.objects[0]=resources;
            if(g_wrath&&Authority(resources))sample.view.readyMask|=Wrath;
        }
        if(Read(resources,0x3A8,chaos)&&Read(resources,0x3AC,maxChaos)&&
           std::isfinite(chaos)&&TargetValue(maxChaos,100,target)){
            sample.view.values[1]={chaos,target,true};
            sample.objects[1]=resources;
            if(g_chaos&&Authority(resources))sample.view.readyMask|=Ultimate;
        }
    }
    void* holder=nullptr;void* owner=nullptr;
    // Exact APawn.PlayerState reflected property: 0x377BBC8, offset +0x330.
    // Require native MayhemPlayerState type AND live holder+0x5C0 == actor.
    // No discovery by scanning memory, outer guess, or cached holder pointer.
    if(Read(actor,0x330,holder)&&IsInstance(holder,0x38115D8)&&
       Read(holder,0x5C0,owner)&&owner==actor){
        float current=0,maximum=0,target=0;
        if(Read(holder,0x5A4,current)&&Read(holder,0x498,maximum)&&
           std::isfinite(current)&&TargetValue(maximum,100,target)){
            sample.view.values[2]={current,target,true};
            sample.objects[2]=holder;
            if(g_synergy&&Authority(holder))sample.view.readyMask|=Synergy;
        }
    }
    return sample;
}
void LogSample(const Sample& sample){
    if(!g_log)return;
    char line[440]{};
    const auto& v=sample.view;
    sprintf_s(line,"Gauge V1.35 MANUAL_CONTEXT actor=%p generation=%u readyMask=%u wrath=%.3f/%.3f chaos=%.3f/%.3f synergy=%.3f/%.3f chaosFormActive=%d tid=%lu (read only)",
        reinterpret_cast<void*>(v.actor),v.generation,v.readyMask,
        v.values[0].current,v.values[0].maximum,v.values[1].current,
        v.values[1].maximum,v.values[2].current,v.values[2].maximum,
        v.formActive?1:0,GetCurrentThreadId());
    g_log(line);
}
}
void Initialize(LogFn logger,std::atomic<void*>* localPlayer,bool exactExecutable){
    g_log=logger;g_local=localPlayer;
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!g_base)return;
    constexpr unsigned char setterEntry[]={0x40,0x53,0x48,0x83,0xEC,0x40,0x0F,0x29,0x74,0x24,0x30,0x48,0x8B,0xD9,0x0F,0x29};
    constexpr unsigned char wrathCap[]={0xF3,0x0F,0x10,0xB1,0x4C,0x04,0x00,0x00,0xF3,0x0F,0x58,0xB1,0xA4,0x03,0x00,0x00};
    constexpr unsigned char chaosCap[]={0xF3,0x0F,0x10,0xB1,0xAC,0x03,0x00,0x00,0xF3,0x0F,0x5D,0xF7,0xF3,0x0F,0x10,0x81};
    constexpr unsigned char resourceClass[]={0x48,0x83,0xEC,0x28,0x48,0x8B,0x05,0x4D,0x07,0x06,0x03,0x48,0x85,0xC0,0x75,0x1A};
    constexpr unsigned char synergyEntry[]={0x40,0x53,0x48,0x83,0xEC,0x30,0xF3,0x0F,0x10,0x81,0xA4,0x05,0x00,0x00,0x48,0x8B};
    constexpr unsigned char stateClass[]={0x48,0x83,0xEC,0x28,0x48,0x8B,0x05,0x2D,0x18,0x05,0x03,0x48,0x85,0xC0,0x75,0x1A};
    constexpr unsigned char formGetter[]={0x48,0x8B,0x42,0x20,0x45,0x33,0xC9,0x48,0x85,0xC0,0x41,0x0F,0x95,0xC1,0x4C,0x03};
    if(!Signature(0x7AD6D0,formGetter))return;
    if(Signature(0x7AFB10,resourceClass)){
        if(Signature(0x6BA510,setterEntry)&&Signature(0x6BA52E,wrathCap))
            g_wrath=reinterpret_cast<Setter>(g_base+0x6BA510);
        if(Signature(0x6BA2A0,setterEntry)&&Signature(0x6BA2BE,chaosCap))
            g_chaos=reinterpret_cast<Setter>(g_base+0x6BA2A0);
    }
    // Initialize BEFORE GaugeChargeFeature hooks the same Synergy entry.
    if(Signature(0x7BFDA0,stateClass)&&Signature(0x583040,synergyEntry))
        g_synergy=reinterpret_cast<Setter>(g_base+0x583040);
    AcquireSRWLockExclusive(&g_lock);
    g_state.supported=g_wrath||g_chaos||g_synergy;
    ReleaseSRWLockExclusive(&g_lock);
    g_enabled.store(g_wrath||g_chaos||g_synergy,std::memory_order_release);
    if(g_log){
        char line[200]{};
        sprintf_s(line,"Gauge V1.35 MANUAL_SET ready Wrath=%d Ultimate=%d Synergy=%d explicitRequestsOnly=1 noNewHooks=1 gameplay=EXPERIMENTAL",
            g_wrath?1:0,g_chaos?1:0,g_synergy?1:0);
        g_log(line);
    }
}
void OnLivePlayer(void* actor,uint32_t generation){
    if(!g_enabled.load(std::memory_order_acquire)||!actor||!g_local||
       g_local->load(std::memory_order_acquire)!=actor||!dg::native_tps::OnGameThread())return;
    static thread_local bool processing=false;
    if(processing)return;
    processing=true;
    struct Done { bool& value; ~Done(){value=false;} } done{processing};
    const uint64_t now=GetTickCount64();
    Request request{};
    bool consume=false;
    AcquireSRWLockExclusive(&g_lock);
    const bool changed=g_state.actor!=reinterpret_cast<uintptr_t>(actor)||g_state.generation!=generation;
    if(!changed&&!g_request.mask&&now>=g_state.observedAt&&now-g_state.observedAt<100){
        ReleaseSRWLockExclusive(&g_lock);return;
    }
    if(g_request.mask){
        const auto valid=ValidateRequest(g_request,reinterpret_cast<uintptr_t>(actor),generation,now);
        if(valid!=RequestValidity::Accept){
            g_state.result=valid==RequestValidity::Expired?Result::Expired:Result::Cancelled;
            g_request={};g_state.pending=false;
        }else if(!dg::native_tps::CinematicActive()){
            request=g_request;g_request={};g_state.pending=false;consume=true;
        }
    }
    const Telemetry previous=g_state;
    ReleaseSRWLockExclusive(&g_lock);
    Sample sample=SamplePlayer(actor,generation,now);
    if(changed||sample.view.readyMask!=previous.readyMask||sample.view.formActive!=previous.formActive)
        LogSample(sample);
    unsigned applied=0;
    if(consume){
        const Setter setters[3]={g_wrath,g_chaos,g_synergy};
        const size_t offsets[3]={0x3A0,0x3A8,0x5A4};
        const char* labels[3]={"WRATH","ULTIMATE","SYNERGY"};
        for(unsigned i=0;i<3;++i){
            const unsigned bit=1u<<i;
            if(!(request.mask&bit))continue;
            float target=0,after=0;
            const auto before=sample.view.values[i];
            bool ok=(sample.view.readyMask&bit)&&setters[i]&&sample.objects[i]&&
                TargetValue(before.maximum,request.percent,target);
            if(ok){
                setters[i](sample.objects[i],target);
                ok=Read(sample.objects[i],offsets[i],after)&&std::isfinite(after)&&
                    std::fabs(after-target)<=std::max(0.01f,before.maximum*0.0001f);
                if(ok){applied|=bit;sample.view.values[i].current=after;}
            }
            if(g_log){
                char line[300]{};
                sprintf_s(line,"Gauge V1.35 MANUAL_SET gauge=%s percent=%.1f before=%.4f requested=%.4f after=%.4f max=%.4f readbackMatches=%d actor=%p generation=%u tid=%lu hudAndActivationRequireUserTest=1",
                    labels[i],request.percent,before.current,target,after,before.maximum,
                    ok?1:0,actor,generation,GetCurrentThreadId());
                g_log(line);
            }
        }
    }
    AcquireSRWLockExclusive(&g_lock);
    sample.view.requests=g_state.requests;
    sample.view.applied=g_state.applied+(applied?1u:0u);
    sample.view.pending=g_state.pending;
    sample.view.result=consume?(applied==request.mask?Result::Applied:
        applied?Result::Partial:Result::Rejected):g_state.result;
    sample.view.lastAppliedMask=consume?applied:g_state.lastAppliedMask;
    g_state=sample.view;
    ReleaseSRWLockExclusive(&g_lock);
}
bool RequestSet(unsigned mask,float percent){
    if(!(mask&All)||(mask&~All)||!std::isfinite(percent)||percent<0||percent>100)return false;
    const auto now=GetTickCount64();
    AcquireSRWLockExclusive(&g_lock);
    const bool valid=g_enabled.load(std::memory_order_acquire)&&!g_request.mask&&
        g_state.actor&&g_state.readyMask&&(mask&g_state.readyMask)==mask&&
        now>=g_state.observedAt&&now-g_state.observedAt<1500;
    if(valid){
        g_request={mask,percent,g_state.actor,g_state.generation,now};
        g_state.pending=true;g_state.result=Result::Queued;++g_state.requests;
    }
    ReleaseSRWLockExclusive(&g_lock);
    return valid;
}
void CancelRequest(){
    AcquireSRWLockExclusive(&g_lock);
    if(g_request.mask){g_request={};g_state.pending=false;g_state.result=Result::Cancelled;}
    ReleaseSRWLockExclusive(&g_lock);
}
Telemetry GetTelemetry(){
    AcquireSRWLockShared(&g_lock);
    Telemetry result=g_state;
    const auto now=GetTickCount64();
    if(!result.observedAt||now<result.observedAt||now-result.observedAt>=1500){
        result.readyMask=0;
        for(auto& value:result.values)value.available=false;
    }
    ReleaseSRWLockShared(&g_lock);
    return result;
}
void Shutdown(){
    g_enabled.store(false,std::memory_order_release);
    CancelRequest();
    const auto t=GetTelemetry();
    if(g_log){
        char line[160]{};
        sprintf_s(line,"Gauge V1.35 MANUAL_SET totals requests=%u appliedReadback=%u gameplayValidation=USER_REQUIRED",t.requests,t.applied);
        g_log(line);
    }
}
}
