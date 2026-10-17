#include "SkipWarningFeature.h"
#include "WarningPresentationPolicy.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <intrin.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dg::skip_warning {
namespace {
using Advance=bool(*)(void*,double,float);
Advance g_original=nullptr;
uintptr_t g_base=0;
LogFn g_log=nullptr;
std::atomic_bool g_ready{false};
std::atomic_uint32_t g_observed{0},g_advanced{0},g_rejected{0};
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
struct Frame {
    void* durations=nullptr;
    int count=0,current=-1,target=-1;
    float alpha=0,elapsed=0,fade=0;
    unsigned char finished=0,cleaned=0;
};
bool Sample(void* widget,Frame& s){
    uintptr_t vtable=0;int children=0;
    return Read(widget,0,vtable)&&vtable==g_base+0x25D1AF0&&
        Read(widget,0x2C8,s.durations)&&Read(widget,0x2D0,s.count)&&
        Read(widget,0x2EC,children)&&s.count==children&&s.count>0&&s.count<=16&&
        Readable(s.durations,size_t(s.count)*sizeof(float))&&
        Read(widget,0x2F0,s.current)&&Read(widget,0x2F4,s.target)&&
        s.current>=-1&&s.current<s.count&&s.target>=-1&&s.target<=s.count&&
        Read(widget,0x2DC,s.alpha)&&std::isfinite(s.alpha)&&s.alpha>=0&&s.alpha<=1&&
        Read(widget,0x2E0,s.elapsed)&&std::isfinite(s.elapsed)&&s.elapsed>=0&&
        Read(widget,0x2E4,s.fade)&&std::isfinite(s.fade)&&s.fade>=0&&s.fade<=30&&
        Read(widget,0x2F8,s.finished)&&Read(widget,0x2F9,s.cleaned)&&!s.finished&&!s.cleaned;
}
struct Cache { uintptr_t widget=0;bool attempted=false,valid=false;uint32_t mask=0;int count=0;float durations[16]{}; };
struct FString { wchar_t* data=nullptr;int size=0,capacity=0; };
static_assert(sizeof(FString)==16);
bool KnownRecord(const unsigned char* record,unsigned mode){
    using ToString=FString*(*)(const void*,FString*);
    using Free=void(*)(void*);
    FString path{};
    const auto* result=reinterpret_cast<ToString>(g_base+0xAC84F0)(record+0x20,&path);
    bool known=false;
    if(result==&path&&path.size>0&&path.size<=261&&path.capacity>=path.size&&
       Readable(path.data,size_t(path.size)*sizeof(wchar_t))){
        const int length=path.size-(path.data[path.size-1]==0?1:0);
        known=IsKnownWarning(mode,std::wstring_view(path.data,size_t(length)));
    }
    // Same native ToString allocation and native free as AC87DE..AC87E8.
    if(path.data)reinterpret_cast<Free>(g_base+0x8312C0)(path.data);
    return known;
}
bool MapDefinitions(const Frame& frame,Cache& cache){
    using GetClass=void*(*)();
    void* owner=reinterpret_cast<GetClass>(g_base+0x2603F0)();
    void* settings=nullptr;void* records=nullptr;int count=0;
    if(!Read(owner,0xF8,settings)||!Read(settings,0x50,records)||
       !Read(settings,0x58,count)||count<1||count>16||
       !Readable(records,size_t(count)*0x40))return false;
    int active=0;
    for(int i=0;i<count;++i){
        auto* record=static_cast<unsigned char*>(records)+size_t(i)*0x40;
        unsigned char enabled=0,mode=0;
        float duration=0,actual=0;
        if(!Read(record,0x39,enabled)||!Read(record,0x38,mode))return false;
        if(!enabled)continue;
        if(active>=frame.count||!Read(record,0,duration)||!std::isfinite(duration)||
           duration<0||duration>120||!Read(frame.durations,size_t(active)*4,actual)||
           actual!=duration)return false;
        cache.durations[active]=duration;
        if(KnownRecord(record,mode))cache.mask|=1u<<active;
        ++active;
    }
    cache.count=active;
    return active==frame.count;
}
bool HookAdvance(void* widget,double currentTime,float delta){
    const auto caller=reinterpret_cast<uintptr_t>(_ReturnAddress());
    static thread_local bool inside=false;
    if(!g_original)return true;
    // All initialization, MoviePlayer readiness and ordinary timer work run
    // first. Non-Tick callbacks, including native movie-finished, are untouched.
    bool result=g_original(widget,currentTime,delta);
    if(inside||!result||caller!=g_base+0x2603CF||
       !dg::runtime::Get().skipWarningEnabled.load(std::memory_order_acquire)||
       !std::isfinite(currentTime)||!std::isfinite(delta)||delta<0)return result;
    inside=true;
    struct Done { bool& flag;~Done(){flag=false;} } done{inside};
    Frame frame{};
    if(!Sample(widget,frame))return result;
    static thread_local Cache cache{};
    if(cache.widget!=reinterpret_cast<uintptr_t>(widget)){
        cache={};cache.widget=reinterpret_cast<uintptr_t>(widget);
    }
    if(!cache.attempted){
        cache.attempted=true;cache.valid=MapDefinitions(frame,cache);++g_observed;
        if(!cache.valid)++g_rejected;
        if(g_log){char line[250]{};
            sprintf_s(line,"Skip Warning V1.38 PRESENTATION widget=%p children=%d knownMask=0x%X mappingVerified=%d nativeConstructionUnchanged=1 EXPERIMENTAL",
                widget,frame.count,cache.mask,cache.valid?1:0);g_log(line);}
    }
    if(!cache.valid||frame.count!=cache.count)return result;
    const int first=frame.current;
    unsigned advanced=0,calls=0;
    for(;calls<unsigned(cache.count*4+4)&&CanAdvanceWarning(frame.current,frame.target,frame.count,cache.mask);){
        const int index=frame.current>=0?frame.current:frame.target;
        if(index<0||index>=cache.count||index>=16)break;
        float duration=0;
        if(!Read(frame.durations,size_t(index)*4,duration)||duration!=cache.durations[index])break;
        const Frame before=frame;
        // Consume only this verified page's timer/fades through the original
        // function. No state writes, synthetic input or loading-loop bypass.
        ++calls;
        result=g_original(widget,currentTime,std::max(1.0f,duration+frame.fade+1.0f));
        if(!result){if(before.current>=0)++advanced;break;} // native cleanup can release children
        if(!Sample(widget,frame)||frame.count!=cache.count)break;
        if(frame.current!=before.current&&before.current>=0)++advanced;
        if(frame.current==before.current&&frame.target==before.target&&
           frame.alpha==before.alpha&&frame.elapsed==before.elapsed)break; // native gate still closed
    }
    if(advanced){
        g_advanced.fetch_add(advanced);
        if(g_log){char line[220]{};
            sprintf_s(line,"Skip Warning V1.38 NATIVE_ADVANCE first=%d last=%d knownPages=%u calls=%u nativeCleanup=%d gameplayValidation=USER_REQUIRED",
                first,frame.current,advanced,calls,result?0:1);g_log(line);}
    }
    return result;
}
}
void Initialize(LogFn logger,bool exactExecutable){
    g_log=logger;
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    struct Guard {uintptr_t rva;unsigned char bytes[16];};
    const Guard guards[]={
        {0x25F850,{0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x60,0x0F,0x29,0x74,0x24,0x50,0x48}},
        {0x260330,{0x48,0x89,0x5C,0x24,0x10,0x57,0x48,0x83,0xEC,0x40,0x0F,0x29,0x74,0x24,0x30,0x48}},
        {0x25D840,{0x41,0x80,0x7F,0x39,0x00,0x0F,0x84,0xCD,0x16,0x00,0x00,0x4C,0x8B,0x75,0x90,0x49}},
        {0x25D877,{0x49,0x8B,0x96,0xC8,0x02,0x00,0x00,0x41,0x8B,0x07,0x89,0x04,0xBA,0x49,0x8D,0x4F}},
        {0x25F87B,{0x48,0x63,0x83,0xF0,0x02,0x00,0x00,0x8B,0x93,0xF4,0x02,0x00,0x00,0xF3,0x0F,0x10}},
        {0x25FA4C,{0xF3,0x0F,0x58,0xB3,0xE0,0x02,0x00,0x00,0xF3,0x0F,0x11,0xB3,0xE0,0x02,0x00,0x00}},
        {0x2603F0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xBA,0x88,0x59,0x03,0x48,0x85}},
        {0xAC84F0,{0x48,0x89,0x4C,0x24,0x08,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57}},
        {0x8312C0,{0x48,0x85,0xC9,0x74,0x2E,0x53,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x8B,0x0D}}
    };
    for(const auto& guard:guards)
        if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,16)){
            if(g_log)g_log("Skip Warning V1.38 presentation signature mismatch; native behavior retained");return;
        }
    uintptr_t tick=0;int32_t displacement=0;
    Read(reinterpret_cast<void*>(g_base+0x25D1AF0),8,tick);
    Read(reinterpret_cast<void*>(g_base+0x2603CA),1,displacement);
    if(tick!=g_base+0x260330||*reinterpret_cast<unsigned char*>(g_base+0x2603CA)!=0xE8||
       intptr_t(0x2603CF)+displacement!=0x25F850)return;
    const auto init=MH_Initialize();
    if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return;
    void* target=reinterpret_cast<void*>(g_base+0x25F850);
    if(MH_CreateHook(target,reinterpret_cast<void*>(&HookAdvance),reinterpret_cast<void**>(&g_original))!=MH_OK)return;
    const auto status=MH_EnableHook(target);
    if(status!=MH_OK&&status!=MH_ERROR_ENABLED){MH_RemoveHook(target);g_original=nullptr;return;}
    g_ready.store(true,std::memory_order_release);
    if(g_log)g_log("Skip Warning V1.38 PRESENTATION ready=1 exact named pages only; native timers/fades/cleanup; asset loading and old loop untouched; EXPERIMENTAL");
}
bool Apply(bool enabled){dg::runtime::Get().skipWarningEnabled.store(enabled);return g_ready.load();}
Telemetry GetTelemetry(){return {g_ready.load(),g_observed.load(),g_advanced.load(),g_rejected.load()};}
void Shutdown(){
    const auto t=GetTelemetry();
    if(g_log){char line[200]{};sprintf_s(line,"Skip Warning V1.38 totals ready=%d mapped=%u advanced=%u rejected=%u gameplayValidation=USER_REQUIRED",
        t.ready?1:0,t.observed,t.advanced,t.rejected);g_log(line);}
}
}
