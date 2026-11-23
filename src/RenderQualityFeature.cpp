#include "RenderQualityFeature.h"
#include "RenderQualitySettings.h"
#include "NativeMemoryReader.h"
#include "NativeTpsFeature.h"
#include "RuntimeSettings.h"
#include <atomic>
#include <bit>
#include <charconv>
#include <cstdio>

namespace dg::render_quality {
namespace {
constexpr uint32_t PriorityMask=0xFF000000;
constexpr uint32_t ReadOnlyOrUnregistered=0x0C;
using SetFn=void(*)(void*,const wchar_t*,unsigned);

struct Channel {
    uintptr_t slot,vtable,setter;
    uint32_t requiredFlags;
    float lastRequest=-1;
    uintptr_t object=0;
    LiveOverride owned{};
};

Channel g_resolution{0x381DEB8,0x28224E0,0x81E690,0x40};
Channel g_reflections{0x39EACB8,0x2822458,0x81E5F0,0x60};
std::atomic_uint g_ready{0};
uintptr_t g_base=0;
LogFn g_log=nullptr;

template<size_t N>
bool Match(uintptr_t rva,const unsigned char (&bytes)[N]){
    return std::memcmp(reinterpret_cast<const void*>(g_base+rva),bytes,N)==0;
}

bool VerifySetter(const Channel& channel){
    constexpr unsigned char setter[]={0x48,0x89,0x6C,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xEC,0x20};
    uintptr_t slot=0;
    native_objects::MemoryReader reader;
    return reader.Read(g_base,channel.vtable+0x60,slot)&&slot==g_base+channel.setter&&Match(channel.setter,setter);
}

bool ReadValue(const Channel& channel,uintptr_t object,uintptr_t data,ConsoleValue& output){
    uint32_t flags=0;
    native_objects::MemoryReader reader;
    if(!reader.Read(object,0x18,flags)||(flags&ReadOnlyOrUnregistered)||
       (flags&0x60)!=channel.requiredFlags||!reader.Read(data,0,output.bits))return false;
    output.priority=flags&PriorityMask;
    return output.priority<=0x09000000;
}

template<class T>
bool SetValue(const Channel& channel,uintptr_t object,uintptr_t data,ConsoleValue value){
    const T native=std::bit_cast<T>(value.bits);
    if(!std::isfinite(native))return false;
    char narrow[64]{};
    const auto converted=std::to_chars(narrow,narrow+sizeof(narrow)-1,native);
    if(converted.ec!=std::errc{})return false;
    wchar_t text[64]{};
    for(size_t i=0;i<static_cast<size_t>(converted.ptr-narrow);++i)text[i]=wchar_t(narrow[i]);
    reinterpret_cast<SetFn>(g_base+channel.setter)(reinterpret_cast<void*>(object),text,value.priority);
    ConsoleValue observed{};
    return ReadValue(channel,object,data,observed)&&observed==value;
}

template<class T>
void Apply(Channel& channel,size_t index,float requested){
    if(requested==channel.lastRequest)return;
    const float desired=Normalize(index,requested);
    native_objects::MemoryReader reader;
    uintptr_t object=0,vt=0,data=0;
    ConsoleValue current{};
    if(!reader.Read(g_base,channel.slot,object)||!reader.Read(object,0,vt)||vt!=g_base+channel.vtable||
       !reader.Read(g_base,channel.slot+8,data)||data!=object+0x38||!ReadValue(channel,object,data,current))return;
    if(channel.object!=object){channel.object=object;channel.owned={};}
    const auto wanted=desired<0?std::nullopt:std::optional<int32_t>(std::bit_cast<int32_t>(static_cast<T>(desired)));
    const auto write=[&](ConsoleValue value){
        const bool changed=SetValue<T>(channel,object,data,value);
        if(g_log){
            char line[160]{};
            sprintf_s(line,"Rendering quality live %s request=%.3f verified=%d",Specs[index].key,double(desired),int(changed));
            g_log(line);
        }
        return changed;
    };
    channel.owned.Update(current,wanted,write);
    channel.lastRequest=desired;
}
}

void Initialize(LogFn log,bool exactExecutable){
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!g_base)return;
    g_log=log;
    constexpr unsigned char resolutionRegister[]={0x48,0x8D,0x15,0x72,0x0D,0x6F,0x02,0xFF,0x50,0x08};
    constexpr unsigned char resolutionSlot[]={0x48,0x89,0x05,0x52,0x9E,0x6F,0x03};
    constexpr unsigned char resolutionRead[]={0xF3,0x0F,0x10,0x0C,0x9F,0xF3,0x0F,0x59,0x0D,0x9D,0x0F,0xD3,0x00};
    constexpr unsigned char reflectionsRegister[]={0x48,0x8D,0x15,0x1C,0x43,0x7F,0x02,0xFF,0x50,0x10};
    constexpr unsigned char reflectionsSlot[]={0x48,0x89,0x05,0x54,0x69,0x85,0x03};
    constexpr unsigned char reflectionsRead[]={0x48,0x8B,0x05,0x30,0x0A,0x9A,0x02,0x83,0x78,0x04,0x00,0x7E};
    unsigned ready=0;
    if(Match(0x12403F,resolutionRegister)&&Match(0x124067,resolutionSlot)&&
       Match(0x19082CA,resolutionRead)&&VerifySetter(g_resolution))ready|=1u<<ResolutionIndex;
    if(Match(0x19433D,reflectionsRegister)&&Match(0x194365,reflectionsSlot)&&
       Match(0x104A289,reflectionsRead)&&VerifySetter(g_reflections))ready|=1u<<ReflectionsIndex;
    g_ready.store(ready,std::memory_order_release);
}
bool Ready(size_t index){
    return IsLiveOption(index)&&(g_ready.load(std::memory_order_acquire)&(1u<<index));
}
void ApplyOnGameThread(){
    const unsigned ready=g_ready.load(std::memory_order_acquire);
    if(!ready||!native_tps::OnGameThread())return;
    const auto& runtime=dg::runtime::Get();
    if(ready&(1u<<ResolutionIndex))Apply<float>(g_resolution,ResolutionIndex,runtime.renderResolution.load(std::memory_order_relaxed));
    if(ready&(1u<<ReflectionsIndex))Apply<int32_t>(g_reflections,ReflectionsIndex,runtime.renderReflections.load(std::memory_order_relaxed));
}
}
