#include "LocalPlayerOwnership.h"
#include "LocalPlayerLayout.h"
#include "NativeTpsFeature.h"
#include <windows.h>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::local_players {
namespace {
uintptr_t g_base=0;
std::atomic_bool g_ready{false};
LogFn g_log=nullptr;
SRWLOCK g_lock=SRWLOCK_INIT;
Slot g_slots[2]{};
unsigned g_changes=0;
std::atomic<LiveObserver> g_observer{nullptr};
std::atomic<NativeView> g_views[2]{};
std::atomic_ullong g_viewTicks[2]{};
std::atomic_int g_localCount{0};
std::atomic_ullong g_localCountTick{0};
bool Readable(const void* p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(p,&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
    const auto access=m.Protect&0xFF;
    if(access!=PAGE_READONLY&&access!=PAGE_READWRITE&&access!=PAGE_WRITECOPY&&
       access!=PAGE_EXECUTE_READ&&access!=PAGE_EXECUTE_READWRITE&&
       access!=PAGE_EXECUTE_WRITECOPY)return false;
    const auto offset=reinterpret_cast<uintptr_t>(p)-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
struct Reader {
    template<class T> bool Read(uintptr_t object,size_t offset,T& value){
        if(!object||object>UINTPTR_MAX-offset)return false;
        const void* at=reinterpret_cast<void*>(object+offset);
        if(!Readable(at,sizeof(value)))return false;
        std::memcpy(&value,at,sizeof(value));return true;
    }
    bool Instance(uintptr_t object,uintptr_t slot){
        uintptr_t expected=0,actual=0,array=0,entry=0;
        int index=-1,last=-1;uint32_t flags=0;
        return Read(g_base+slot,0,expected)&&expected&&
            Read(object,8,flags)&&!(flags&0x30u)&&
            Read(object,0x10,actual)&&actual&&Read(expected,0x38,index)&&
            index>=0&&index<=512&&Read(actual,0x38,last)&&last>=index&&last<=512&&
            Read(actual,0x30,array)&&array&&Read(array,size_t(index)*8,entry)&&
            entry==expected+0x30;
    }
};
void Publish(const Context& context){
    if(context.index<0||context.index>1)return;
    AcquireSRWLockExclusive(&g_lock);
    auto& slot=g_slots[context.index];
    const bool changed=slot.localPlayer!=context.localPlayer||
        slot.character!=context.character||slot.controllerId!=context.controllerId;
    const bool identityChanged=slot.localPlayer!=context.localPlayer||
        slot.character!=context.character||slot.playerState!=context.playerState;
    auto generation=slot.generation+(identityChanged?1u:0u);
    if(!generation)generation=1;
    slot={context.localPlayer,context.character,context.controllerId,GetTickCount64(),generation,context.playerState};
    // A verified one-player world invalidates an old coop input lease.
    if(context.localCount==1){
        const auto previousCoopGeneration=g_slots[1].generation;
        g_slots[1]=Slot{};g_slots[1].generation=previousCoopGeneration;
    }
    g_localCount.store(context.localCount);
    g_localCountTick.store(GetTickCount64(),std::memory_order_release);
    if(changed&&g_log&&++g_changes<=80){
        char line[260]{};
        sprintf_s(line,"TPS V1.45 LOCAL_OWNER index=%d controllerId=%d localPlayer=%p controller=%p character=%p pawn=%p count=%d",
            context.index,context.controllerId,reinterpret_cast<void*>(context.localPlayer),
            reinterpret_cast<void*>(context.controller),reinterpret_cast<void*>(context.character),
            reinterpret_cast<void*>(context.pawn),context.localCount);
        g_log(line);
    }
    ReleaseSRWLockExclusive(&g_lock);
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;
    if(!exactExecutable)return;
    g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    // Prove class-slot getters against the retail EXE before using layouts.
    struct Guard{uintptr_t rva;unsigned char bytes[14];};
    const Guard guards[]={
        {0x7AAB80,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x92,0x4B,0x06,0x03}},
        {0x1D0CD00,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x8A,0x05,0xD5,0x01}},
        {0x1D7E4F0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x7A,0x29,0xCE,0x01}},
        {0x1E04BE0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x0A,0x34,0xC6,0x01}},
        {0x78FCE0,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0xCA,0xDC,0x07,0x03}},
        {0x1CE3310,{0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x78,0x48,0x8B,0x05,0x1A,0x7A,0xD7,0x01}}
    };
    for(const auto& guard:guards)
        if(std::memcmp(reinterpret_cast<void*>(g_base+guard.rva),guard.bytes,sizeof(guard.bytes))){
            if(g_log)g_log("TPS V1.45 LOCAL_OWNER signature mismatch; ownership changes disabled");
            return;
        }
    g_ready=true;
    if(g_log)g_log("TPS V1.45 LOCAL_OWNER ready; reciprocal native local-player chains; no new hooks");
}
bool ResolveActor(void* actor,Context& context){
    Reader r;
    if(!g_ready||!layout::Resolve(r,reinterpret_cast<uintptr_t>(actor),context))return false;
    Publish(context);return true;
}
bool Ready(){return g_ready.load(std::memory_order_acquire);}
void SetLiveObserver(LiveObserver observer){g_observer.store(observer,std::memory_order_release);}
void ObserveGameplay(const Context& context){
    if(!dg::native_tps::OnGameThread()||context.index<0||context.index>1)return;
    const auto slot=PublishedSlot(static_cast<unsigned>(context.index));
    const auto observer=g_observer.load(std::memory_order_acquire);
    if(observer&&slot.localPlayer==context.localPlayer&&slot.character==context.character)
        observer(context,slot.generation);
}
bool ResolveCamera(void* camera,Context& context){
    Reader r;
    if(!g_ready||!layout::Camera(r,reinterpret_cast<uintptr_t>(camera),context))return false;
    Publish(context);return true;
}
void ObserveCameraViews(void* camera){
    if(!g_ready||!dg::native_tps::OnGameThread())return;
    Reader reader;uintptr_t world=0;
    if(!reader.Read(reinterpret_cast<uintptr_t>(camera),0xF0,world)||
       !reader.Instance(world,layout::WorldClass))return;
    int count=0;
    const NativeView observed[]={layout::View(reader,reinterpret_cast<uintptr_t>(camera),0,&count),
        layout::View(reader,reinterpret_cast<uintptr_t>(camera),1)};
    if(count){g_localCount.store(count);g_localCountTick.store(GetTickCount64(),std::memory_order_release);}
    if(observed[0]==NativeView::Unknown&&observed[1]==NativeView::Unknown)return;
    for(unsigned index=0;index<2;++index){
        const auto view=observed[index];
        const auto prior=g_views[index].exchange(view);
        g_viewTicks[index].store(GetTickCount64(),std::memory_order_release);
        if(prior!=view&&g_log){
            char line[160]{};
            sprintf_s(line,"TPS V1.46 NATIVE_VIEW player=%u owner=%s source=ViewTarget+PendingViewTarget",
                index+1,view==NativeView::ExternalCamera?"EXTERNAL_CAMERA":
                view==NativeView::Gameplay?"GAMEPLAY":"UNKNOWN");
            g_log(line);
        }
    }
}
bool CameraCinematic(unsigned index){
    if(index>1)return false;
    const auto tick=g_viewTicks[index].load(std::memory_order_acquire),now=GetTickCount64();
    return tick&&now>=tick&&now-tick<500&&g_views[index].load()==NativeView::ExternalCamera;
}
bool ReadPose(const Context& context,Pose& pose){
    Reader r;uintptr_t root=0;
    if(!g_ready||!context.pawn||!r.Read(context.pawn,0x158,root)||!root||
       !r.Read(root,0x1A0,pose.x)||!r.Read(root,0x1A4,pose.y)||
       !r.Read(root,0x1A8,pose.z)||!r.Read(root,0x1F4,pose.yaw))return false;
    return std::isfinite(pose.x)&&std::isfinite(pose.y)&&std::isfinite(pose.z)&&
        std::isfinite(pose.yaw)&&std::fabs(pose.x)<1e7f&&std::fabs(pose.y)<1e7f&&
        std::fabs(pose.z)<1e7f&&std::fabs(pose.yaw)<36000.0f;
}
Slot PublishedSlot(unsigned index){
    if(index>1)return {};
    AcquireSRWLockShared(&g_lock);Slot result=g_slots[index];ReleaseSRWLockShared(&g_lock);
    const auto now=GetTickCount64();
    if(!result.tick||now<result.tick||now-result.tick>1500ull)return {};
    return result;
}
bool CoopActive(){
    const auto tick=g_localCountTick.load(std::memory_order_acquire),now=GetTickCount64();
    return tick&&now>=tick&&now-tick<1500&&g_localCount.load()>1;
}
int FindCharacter(const void* actor,Slot* matched){
    if(!actor)return -1;
    for(unsigned i=0;i<2;++i){
        const auto slot=PublishedSlot(i);
        if(slot.character==reinterpret_cast<uintptr_t>(actor)){
            if(matched)*matched=slot;
            return static_cast<int>(i);
        }
    }
    return -1;
}
bool Matches(unsigned index,uintptr_t actor,uint32_t generation){
    const auto slot=PublishedSlot(index);
    return actor&&slot.character==actor&&slot.generation==generation;
}
int PrimaryControllerId(){const auto s=PublishedSlot(0);return s.controllerId>=0?s.controllerId:0;}
}
