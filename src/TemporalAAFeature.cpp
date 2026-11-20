#include "TemporalAAFeature.h"
#include <windows.h>
#include <atomic>
#include <cstring>
#include <mutex>

namespace dg::temporal_aa {
namespace {
std::atomic_bool g_ready{false};
std::mutex g_mutex;
std::atomic_bool g_active{false};
struct Channel {
    uintptr_t slotRva;
    LONG* data=nullptr;
    ShadowOverride state;
    bool active=false;
};
Channel g_weight{0x39CB138},g_sharpen{0x39CB168};
Channel g_motionBlur{0x3A47F80},g_colorFringe{0x3A47FB0};
uintptr_t g_base=0;

bool Writable(const LONG* data){
    if(!data||reinterpret_cast<uintptr_t>(data)%alignof(LONG))return false;
    MEMORY_BASIC_INFORMATION region{};
    if(!VirtualQuery(data,&region,sizeof(region))||region.State!=MEM_COMMIT||
       (region.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const auto protection=region.Protect&0xFF;
    const auto start=reinterpret_cast<uintptr_t>(data);
    const auto offset=start-reinterpret_cast<uintptr_t>(region.BaseAddress);
    return (protection==PAGE_READWRITE||protection==PAGE_WRITECOPY||
        protection==PAGE_EXECUTE_READWRITE||protection==PAGE_EXECUTE_WRITECOPY)&&
        offset<=region.RegionSize&&2*sizeof(LONG)<=region.RegionSize-offset;
}
struct Access {
    LONG* data;
    int32_t Read(unsigned i){return InterlockedCompareExchange(data+i,0,0);}
    int32_t ExchangeIf(unsigned i,int32_t value,int32_t expected){
        return InterlockedCompareExchange(data+i,value,expected);
    }
};
void Service(Channel& channel,bool enabled,float value,float minimum,float maximum){
    if(!enabled&&!channel.active)return;
    auto* data=*reinterpret_cast<LONG**>(g_base+channel.slotRva);
    if(data!=channel.data){channel.state.Forget();channel.data=data;}
    if(!Writable(data)){channel.state.Forget();channel.active=false;return;}
    Access access{data};channel.state.Update(access,enabled,value,minimum,maximum);
    channel.active=enabled;
}
void RestoreLocked(){
    if(!g_active.load(std::memory_order_relaxed))return;
    Service(g_weight,false,0,0,1);Service(g_sharpen,false,0,-1,10);
    Service(g_motionBlur,false,0,-1,10);Service(g_colorFringe,false,0,-2,10);
    g_active.store(false,std::memory_order_relaxed);
}
}
void Initialize(LogFn log,bool exactExecutable){
    if(g_ready.load(std::memory_order_acquire))return;
    if(!exactExecutable)return;
    const auto* base=reinterpret_cast<const unsigned char*>(GetModuleHandleW(nullptr));
    if(!base)return;
    // Exact float registration, render-thread-safe flags (0x60), data slot,
    // UTF-16 name and native render [+4] consumer in the supported EXE.
    constexpr unsigned char weightRegister[]={0x48,0x8D,0x15,0x1A,0xB4,0x7E,0x02,0xFF,0x50,0x08};
    constexpr unsigned char sharpenRegister[]={0x48,0x8D,0x15,0xAF,0xE5,0x7E,0x02,0xFF,0x50,0x08};
    constexpr unsigned char weightSlot[]={0x48,0x89,0x05,0x1A,0xCD,0x83,0x03};
    constexpr unsigned char sharpenSlot[]={0x48,0x89,0x05,0x4F,0xCA,0x83,0x03};
    constexpr unsigned char flags[]={0xC7,0x44,0x24,0x20,0x60,0,0,0};
    constexpr unsigned char weightRead[]={0x48,0x8B,0x05,0xA9,0xFC,0xA0,0x02,0xF3,0x0F,0x10,0x40,0x04};
    constexpr unsigned char sharpenRead[]={0x48,0x8B,0x05,0xED,0x2C,0xA1,0x02,0xF3,0x0F,0x10,0x48,0x04};
    constexpr unsigned char motionRegister[]={0x48,0x8D,0x15,0x22,0xC2,0xBB,0x02,0xFF,0x50,0x08};
    constexpr unsigned char fringeRegister[]={0x48,0x8D,0x15,0xE2,0xBF,0xBB,0x02,0xFF,0x50,0x08};
    constexpr unsigned char motionSlot[]={0x48,0x89,0x05,0x32,0x3F,0x85,0x03};
    constexpr unsigned char fringeSlot[]={0x48,0x89,0x05,0xE2,0x39,0x85,0x03};
    constexpr unsigned char motionRead[]={0x48,0x8B,0x05,0xDB,0x53,0xF2,0x01,0xF3,0x0F,0x10,0x08,0x0F,0x2F,0xCE,0x72,0x08};
    constexpr unsigned char fringeRead[]={0x48,0x8B,0x05,0xDB,0x53,0xF2,0x01,0xF3,0x0F,0x10,0x00,0x0F,0x2F,0xC6,0x72,0x12,0xF3,0x0F,0x5D,0x83,0x70,0x0F,0,0};
    const bool valid=!std::memcmp(base+0x18E3EF,weightRegister,sizeof(weightRegister))&&
        !std::memcmp(base+0x18E6EA,sharpenRegister,sizeof(sharpenRegister))&&
        !std::memcmp(base+0x18E417,weightSlot,sizeof(weightSlot))&&
        !std::memcmp(base+0x18E712,sharpenSlot,sizeof(sharpenSlot))&&
        !std::memcmp(base+0x18E3D8,flags,sizeof(flags))&&
        !std::memcmp(base+0x18E6D8,flags,sizeof(flags))&&
        !std::memcmp(base+0xFBB488,weightRead,sizeof(weightRead))&&
        !std::memcmp(base+0xFB8474,sharpenRead,sizeof(sharpenRead))&&
        !std::memcmp(base+0x2979810,L"r.TemporalAACurrentFrameWeight",sizeof(L"r.TemporalAACurrentFrameWeight"))&&
        !std::memcmp(base+0x297CCA0,L"r.Tonemapper.Sharpen",sizeof(L"r.Tonemapper.Sharpen"))&&
        !std::memcmp(base+0x1F401F,motionRegister,sizeof(motionRegister))&&
        !std::memcmp(base+0x1F459F,fringeRegister,sizeof(fringeRegister))&&
        !std::memcmp(base+0x1F4047,motionSlot,sizeof(motionSlot))&&
        !std::memcmp(base+0x1F45C7,fringeSlot,sizeof(fringeSlot))&&
        !std::memcmp(base+0x1F4008,flags,sizeof(flags))&&
        !std::memcmp(base+0x1F4588,flags,sizeof(flags))&&
        !std::memcmp(base+0x1B22B9E,motionRead,sizeof(motionRead))&&
        !std::memcmp(base+0x1B22BCE,fringeRead,sizeof(fringeRead))&&
        !std::memcmp(base+0x2DB0248,L"r.MotionBlur.Amount",sizeof(L"r.MotionBlur.Amount"))&&
        !std::memcmp(base+0x2DB0588,L"r.SceneColorFringe.Max",sizeof(L"r.SceneColorFringe.Max"));
    if(valid)g_base=reinterpret_cast<uintptr_t>(base);
    g_ready.store(valid,std::memory_order_release);
    if(log)log(valid?"Temporal AA: float registration/render consumers verified; optional settings, native by default":
        "Temporal AA: unsupported registration/render consumers; native output retained");
    if(log&&valid)log("Image effects V1.169: native motion blur amount/color fringe clamp verified; independent optional filters, no new hook");
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void Update(const Settings& requested){
    const bool any=requested.enabled||requested.motionBlurAmount>=0||requested.colorFringeMax>=0;
    if(!Ready()||(!any&&!g_active.load(std::memory_order_relaxed)))return;
    std::lock_guard<std::mutex> lock(g_mutex);
    if(!Ready())return;
    if(!any){RestoreLocked();return;}
    const auto settings=Normalize(requested);
    Service(g_weight,settings.enabled,settings.frameWeight,0,1);
    Service(g_sharpen,settings.enabled,settings.sharpen,-1,10);
    Service(g_motionBlur,settings.motionBlurAmount>=0,settings.motionBlurAmount,-1,10);
    Service(g_colorFringe,settings.colorFringeMax>=0,settings.colorFringeMax,-2,10);
    g_active.store(true,std::memory_order_relaxed);
}
void Restore(){
    if(!g_ready.exchange(false,std::memory_order_acq_rel))return;
    std::lock_guard<std::mutex> lock(g_mutex);RestoreLocked();
}
}
