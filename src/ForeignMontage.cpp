#include "ForeignMontage.h"
#include "NativePropertyField.h"
#include <cstdio>
#include "NativeObjectName.h"
#include "NativeTpsFeature.h"
#include <array>
#include <atomic>
#include <cmath>
#include <algorithm>

namespace dg::foreign_montage {
namespace {
struct Name {int index=0,number=0;};
struct Array {uintptr_t data=0;int count=0,capacity=0;};
struct Segment {uintptr_t sequence=0;float start=0,first=0,last=0,rate=0;int loops=0;unsigned char valid=0,pad[3]{};};
using Event=std::array<unsigned char,184>;
using Construct=void*(*)(void*,void*,Name,uint32_t,uint32_t,void*,bool,void*,bool);
using Package=void*(*)();
using Skeleton=void(*)(void*,void*);
using Reserve=void(*)(void*,int);
Construct g_construct=nullptr;Package g_package=nullptr;Skeleton g_skeleton=nullptr;Reserve g_reserve=nullptr;
using SoundNotify=void(*)(void*,void*,void*);
SoundNotify g_soundNotify=nullptr;LogFn g_log=nullptr;unsigned g_soundEvents=0,g_motionRefusals=0;
bool OpeningSound(uintptr_t notify,float time);
uintptr_t g_base=0;std::atomic_bool g_ready{false};
native_objects::Identity g_gestures[2]{};
static_assert(sizeof(Name)==8&&sizeof(Array)==16&&sizeof(Segment)==32);
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
template<size_t N> bool Is(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
bool Writable(uintptr_t p,size_t bytes){
    MEMORY_BASIC_INFORMATION m{};
    if(!p||!VirtualQuery(reinterpret_cast<void*>(p),&m,sizeof(m))||m.State!=MEM_COMMIT||
       (m.Protect&(PAGE_GUARD|PAGE_NOACCESS))||((m.Protect&0xFF)!=PAGE_READWRITE&&(m.Protect&0xFF)!=PAGE_EXECUTE_READWRITE))return false;
    const auto offset=p-reinterpret_cast<uintptr_t>(m.BaseAddress);
    return offset<=m.RegionSize&&bytes<=m.RegionSize-offset;
}
template<class T> bool Store(uintptr_t p,size_t offset,const T& value){
    if(p>UINTPTR_MAX-offset||!Writable(p+offset,sizeof(value)))return false;
    std::memcpy(reinterpret_cast<void*>(p+offset),&value,sizeof(value));return true;
}
bool List(uintptr_t p,size_t offset,Array& a,int max){
    return Read(p,offset,a)&&a.count>=0&&a.count<=a.capacity&&a.capacity<=max&&(a.data||!a.count);
}
bool Empty(uintptr_t p,size_t offset){Array a{};return List(p,offset,a,128)&&!a.count;}
bool Idle(const Source& s,float& length){
    uintptr_t cls=0,skeleton=0;float rate=0;std::array<unsigned char,16> guid{};
    return Read(s.sequence,0x10,cls)&&Is(cls,"AnimSequence")&&Is(s.sequence,"Strife_Idle")&&
        Read(s.sequence,0x38,skeleton)&&skeleton==s.skeleton&&Read(s.skeleton,0x10,cls)&&Is(cls,"Skeleton")&&Read(s.skeleton,0x160,guid)&&
        Read(s.sequence,0x90,length)&&std::isfinite(length)&&length>.1f&&length<10&&
        Read(s.sequence,0x94,rate)&&rate==1&&Empty(s.sequence,0x80)&&
        native_objects::NameValueIs(g_base,s.slotIndex,s.slotNumber,"TPS");
}
bool Gesture(uintptr_t p,int kind,float& length){
    uintptr_t cls=0,package=0,skeleton=0;float rate=0;unsigned char root=255;Array events{},tracks{};int firstBone=-1;
    if(kind<0||kind>1||!Read(p,0x10,cls)||!Is(cls,"AnimSequence")||!Read(p,0x20,package)||!Read(p,0x38,skeleton)||
       !Read(p,0x90,length)||!std::isfinite(length)||length<=0||length>2||!Read(p,0x94,rate)||rate!=1||!Read(p,0x1A1,root)||
       !List(p,0xB0,tracks,1024)||!tracks.count||!Read(tracks.data,0,firstBone)||firstBone!=0||
       !List(p,0x80,events,16)||events.count!=3)return false;
    if(kind==0){
        if(!Is(p,"Strife_Aether_Channel_Start")||!Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Aether_Channel_Start")||
           !Is(skeleton,"Strife_Skeleton")||root||length!=1)return false;
    }else{
        // Original cooked clip: root track maps to bone 0, one translation
        // and one rotation key. Root-motion flag is native; delta is zero.
        if(!Is(p,"War_Channel_Start")||!Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Channel_Start")||
           !Is(skeleton,"War_Mesh_Skeleton")||root!=1||std::fabs(length-.5666666627f)>.00001f)return false;
    }
    constexpr float times[2][3]={{0,0,.4308005273f},{0,.1327852011f,.3913668990f}};
    for(int i=0;i<events.count;++i){
        const auto at=events.data+size_t(i)*sizeof(Event);uintptr_t notify=0,state=0,owner=0;float time=0,duration=0;
        if(!Read(at,0x48,notify)||!Read(notify,0x10,cls)||!Read(notify,0x20,owner)||owner!=p||
           !Read(at,0x50,state)||state||!Read(at,0x58,duration)||duration||!Read(at,0x24,time)||
           !std::isfinite(time)||std::fabs(time-times[kind][i])>.00001f||
           !(kind==1&&i>0?Is(cls,"AnimNotify_PlayFootstep"):Is(cls,"AnimNotify_PlaySound")))return false;
    }
    return true;
}
bool Original(uintptr_t p){
    uintptr_t package=0;
    return Read(p,0x20,package)&&
        ((Is(p,"War_Wrath_BladeGeyser_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser_Montage"))||
         (Is(p,"War_Wrath_BladeGeyser_Level1_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser_Level1_Montage"))||
         (Is(p,"War_Wrath_BladeGeyser_Level2_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser_Level2_Montage"))||
         (Is(p,"War_Wrath_BladeGeyser_Level3_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser_Level3_Montage")));
}
struct Layout {uintptr_t cls=0;Array tracks{},segments{},events{},sections{};Segment segment{};float length=0;};
bool Shape(uintptr_t p,Layout& out){
    Name slot{};float rate=0,sectionTime=0;uintptr_t cls=0,package=0,skeleton=0,sequenceSkeleton=0;
    return Read(p,0x10,out.cls)&&Is(out.cls,"AnimMontage")&&Read(out.cls,0x10,cls)&&Is(cls,"Class")&&
        Read(p,0x90,out.length)&&std::isfinite(out.length)&&std::fabs(out.length-2.3666665554f)<.0001f&&
        Read(p,0x94,rate)&&rate==1&&List(p,0x158,out.tracks,4)&&out.tracks.count==1&&
        Read(out.tracks.data,0,slot)&&native_objects::NameValueIs(g_base,slot.index,slot.number,"TPS")&&
        List(out.tracks.data,8,out.segments,8)&&out.segments.count==1&&Read(out.segments.data,0,out.segment)&&
        Read(out.segment.sequence,0x10,cls)&&Is(cls,"AnimSequence")&&Is(out.segment.sequence,"War_Wrath_BladeGeyser")&&
        Read(out.segment.sequence,0x20,package)&&Is(package,"/Game/Characters/Heroes/Hero_War/Animation/War_Wrath_BladeGeyser")&&
        Read(p,0x38,skeleton)&&Read(out.segment.sequence,0x38,sequenceSkeleton)&&skeleton==sequenceSkeleton&&Is(skeleton,"War_Mesh_Skeleton")&&
        out.segment.start==0&&out.segment.first==0&&out.segment.last==out.length&&out.segment.rate==1&&out.segment.loops==1&&out.segment.valid==1&&
        List(p,0x80,out.events,128)&&out.events.count>=1&&List(p,0x148,out.sections,8)&&out.sections.count==1&&
        Read(out.sections.data,0x24,sectionTime)&&sectionTime==0&&
        Empty(p,0x168)&&Empty(p,0x180)&&Empty(p,0x190)&&Empty(p,0x128)&&Empty(p,0x138);
}
bool SpawnEvent(uintptr_t sequence,Event& event){
    Array a{};if(!List(sequence,0x80,a,64))return false;
    bool found=false;
    for(int i=0;i<a.count;++i){
        const auto at=a.data+size_t(i)*event.size();uintptr_t notify=0,cls=0,owner=0,vt=0,entry=0,state=0;Name function{};
        if(!Read(at,0x48,notify)||!Read(notify,0x10,cls)||!Is(cls,"AnimNotify_CallAbilityFunction")||
           !Read(notify,0x38,function)||!native_objects::NameValueIs(g_base,function.index,function.number,"SpawnBlast"))continue;
        unsigned char method=255,cached=255,tickType=255;float time=0,duration=0;
        if(found||!Read(notify,0x20,owner)||owner!=sequence||!Read(notify,0,vt)||!Read(vt,0x240,entry)||entry!=g_base+0x5A7C60||
           !Read(at,0x50,state)||state||!Read(at,0x58,duration)||duration!=0||
           !Read(at,0x18,method)||method||!Read(at,0x19,cached)||cached||!Read(at,0x91,tickType)||tickType||
           !Read(at,0x24,time)||!std::isfinite(time)||time<=0||time>=2.366667f||!Read(at,0,event))return false;
        found=true;
    }
    return found;
}
bool Link(uintptr_t element,uintptr_t montage,uintptr_t sequence,float time,float length){
    const int zero=0;const unsigned char absolute=0;const float start=0;
    return Store(element,8,montage)&&Store(element,0x10,zero)&&Store(element,0x14,zero)&&
        Store(element,0x18,absolute)&&Store(element,0x19,absolute)&&Store(element,0x1C,start)&&
        Store(element,0x20,length)&&Store(element,0x24,time)&&Store(element,0x28,sequence);
}
bool BladeEffect(uintptr_t asset){
    return Is(asset,"VFX_war_wrath_bladeGeyser_blade")||Is(asset,"VFX_war_wrath_bladeGeyser_bladeFar")||
        Is(asset,"VFX_war_wrath_bladeGeyser_nova")||Is(asset,"VFX_war_wrath_bladeGeyser_bladeFar_Lvl1")||
        Is(asset,"VFX_war_wrath_bladeGeyser_bladeFar_Lvl2")||Is(asset,"VFX_war_wrath_bladeGeyser_bladeFar_Lvl3");
}
struct Presentation {std::array<Event,64> events{};int count=0,sounds=0,particles=0,spawns=0;};
bool Collect(uintptr_t asset,const Array& events,Presentation& out,const Event* spawn=nullptr){
    uintptr_t callback=0;if(spawn)std::memcpy(&callback,spawn->data()+0x48,sizeof(callback));
    for(int i=0;i<events.count;++i){
        const auto at=events.data+size_t(i)*sizeof(Event);uintptr_t notify=0,cls=0,effect=0,owner=0,state=0;float time=0,duration=0;
        if(!Read(at,0x48,notify))return false;
        if(!notify)continue;
        if(notify==callback){
            if(out.count<0||out.count>=int(out.events.size()))return false;
            out.events[out.count++]=*spawn;++out.spawns;continue;
        }
        if(!Read(notify,0x10,cls))return false;
        const bool particle=Is(cls,"AnimNotify_PlayParticleEffect"),sound=Is(cls,"AnimNotify_PlaySound");
        if(!particle&&!sound)continue;
        if(!Read(notify,0x38,effect))return false;
        if(!effect||!(particle?BladeEffect(effect):Is(effect,"char_war_wrath_bladegeyser")))continue;
        unsigned char tickType=255;
        if(out.count<0||out.count>=int(out.events.size())||!Read(notify,0x20,owner)||owner!=asset||
           !Read(at,0x50,state)||state||!Read(at,0x58,duration)||duration!=0||!Read(at,0x91,tickType)||tickType||
           !Read(at,0x24,time)||!std::isfinite(time)||time<0||time>=2.366667f||!Read(at,0,out.events[out.count]))return false;
        if(sound){++out.sounds;if(OpeningSound(notify,time))continue;}else ++out.particles;
        ++out.count;
    }
    return true;
}
bool AdaptRoot(Event& event,uintptr_t montage,const Source& source){
    uintptr_t notify=0,cls=0;std::memcpy(&notify,event.data()+0x48,sizeof(notify));
    if(!Read(notify,0x10,cls))return false;
    if(!Is(cls,"AnimNotify_PlayParticleEffect"))return true;
    Name socket{};if(!Read(notify,0x84,socket))return false;
    if(!socket.index&&!socket.number)return true;
    if(!native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Root"))return false;
    const auto copy=reinterpret_cast<uintptr_t>(g_construct(reinterpret_cast<void*>(cls),reinterpret_cast<void*>(montage),{},0x40u,0,
        reinterpret_cast<void*>(notify),false,nullptr,false));
    uintptr_t actualClass=0,outer=0;uint32_t flags=0;std::array<unsigned char,0x54> payload{};
    if(!copy||copy==notify||!Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u)||
       !Read(copy,0x10,actualClass)||actualClass!=cls||!Read(copy,0x20,outer)||outer!=montage||
       !Read(notify,0x38,payload)||!Store(copy,0x38,payload)||!Store(copy,0x84,Name{source.rootIndex,source.rootNumber}))return false;
    std::memcpy(event.data()+0x48,&copy,sizeof(copy));return true;
}
}
void Initialize(LogFn log,bool exact){
    if(!exact)return;g_log=log;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char construct[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x40};
    constexpr unsigned char package[]={0x48,0x8B,0x05,0x59,0xCD,0xED,0x02,0xC3};
    constexpr unsigned char skeleton[]={0x48,0x85,0xD2,0x74,0x15,0x48,0x3B,0x51,0x38,0x74,0x0F,0x48,0x89,0x51,0x38,0x0F};
    constexpr unsigned char reserve[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x63,0xDA,0x48,0x8B,0xF9,0x85,0xD2,0x74,0x3A,0x48,0x69,0xCB,0xB8};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0xAC57A0),construct,sizeof(construct))||
       std::memcmp(reinterpret_cast<void*>(g_base+0xA7B1B0),package,sizeof(package))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x16C7130),skeleton,sizeof(skeleton))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x2C42C0),reserve,sizeof(reserve)))return;
    constexpr unsigned char sound[]={0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0x68,0xA1,0x48,0x81,0xEC,0xB0,0x00};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x16977D0),sound,sizeof(sound)))g_soundNotify=reinterpret_cast<SoundNotify>(g_base+0x16977D0);
    g_gestures[0]=g_gestures[1]={};
    g_construct=reinterpret_cast<Construct>(g_base+0xAC57A0);g_package=reinterpret_cast<Package>(g_base+0xA7B1B0);
    g_skeleton=reinterpret_cast<Skeleton>(g_base+0x16C7130);g_reserve=reinterpret_cast<Reserve>(g_base+0x2C42C0);g_ready=true;
    if(log)log(g_soundNotify?"Foreign montage V1.108 READY: private native montages and sound entry verified before shared detours":"Foreign montage V1.108 READY; opening sound boundary unavailable");
}
bool Ready(){return g_ready.load();}
void ObserveSequence(uintptr_t p){
    if(!g_ready||!native_tps::OnGameThread())return;
    const int kind=Is(p,"Strife_Aether_Channel_Start")?0:(Is(p,"War_Channel_Start")?1:-1);
    if(kind<0)return;
    native_objects::MemoryReader reader;
    if(native_objects::Alive(reader,g_base,g_gestures[kind]))return;
    g_gestures[kind]={};float length=0;
    if(Gesture(p,kind,length))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_gestures[kind]);
}
bool GesturesReady(){
    native_objects::MemoryReader reader;
    return native_objects::Alive(reader,g_base,g_gestures[0])&&native_objects::Alive(reader,g_base,g_gestures[1]);
}
uintptr_t CastingSequence(uintptr_t fallback,uintptr_t skeleton,float& length){
    if(!g_ready||!native_tps::OnGameThread())return fallback;
    for(int i=0;i<2;++i){
        native_objects::MemoryReader reader;uintptr_t actual=0;float duration=0;
        if(native_objects::Alive(reader,g_base,g_gestures[i])&&Gesture(g_gestures[i].address,i,duration)&&
           Read(g_gestures[i].address,0x38,actual)&&actual==skeleton){length=duration;return g_gestures[i].address;}
    }
    return fallback;
}
bool CanBuild(const Source& source){
    float length=0;uintptr_t cls=0;
    if(!g_ready||!g_construct||!g_package||!g_skeleton||!native_tps::OnGameThread()||!Idle(source,length))return false;
    const auto package=reinterpret_cast<uintptr_t>(g_package());
    return Read(package,0x10,cls)&&Is(cls,"Package")&&Is(package,"/Engine/Transient");
}
void* Geyser(uintptr_t original,const Source& source){
    Layout before{},after{};Event event{};float length=0,time=0;Array sequenceEvents{};Presentation presentation{};
    if(!g_reserve||!CanBuild(source)||!Idle(source,length)||
       !native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_Strife_Root")||
       !Original(original)||!Shape(original,before)||!SpawnEvent(before.segment.sequence,event)||
       !List(before.segment.sequence,0x80,sequenceEvents,64)||!Collect(before.segment.sequence,sequenceEvents,presentation,&event)||
       !Collect(original,before.events,presentation)||presentation.spawns!=1||presentation.sounds!=1||!presentation.particles)return nullptr;
    const auto playback=CastingSequence(source.sequence,source.skeleton,length);
    std::memcpy(&time,event.data()+0x24,sizeof(time));
    if(time>=before.length)return nullptr;
    std::stable_sort(presentation.events.begin(),presentation.events.begin()+presentation.count,[](const Event& a,const Event& b){
        float ta=0,tb=0;std::memcpy(&ta,a.data()+0x24,sizeof(ta));std::memcpy(&tb,b.data()+0x24,sizeof(tb));return ta<tb;
    });
    const auto outer=reinterpret_cast<uintptr_t>(g_package());
    auto* created=g_construct(reinterpret_cast<void*>(before.cls),reinterpret_cast<void*>(outer),{},0x40u,0,reinterpret_cast<void*>(original),false,nullptr,false);
    const auto montage=reinterpret_cast<uintptr_t>(created);uintptr_t actualOuter=0;uint32_t flags=0;
    if(!montage||montage==original||!Read(montage,8,flags)||(flags&0x30u)||!(flags&0x40u)||
       !Read(montage,0x20,actualOuter)||actualOuter!=outer||!Shape(montage,after)||after.cls!=before.cls||after.length!=before.length||
       after.events.count!=before.events.count||after.events.data==before.events.data||after.tracks.data==before.tracks.data||
       after.segments.data==before.segments.data||after.sections.data==before.sections.data||
       !Writable(after.events.data,event.size())||!Writable(after.segments.data,sizeof(Segment))||
       !Writable(after.sections.data,88)||!Writable(montage+0x80,sizeof(Array)))return nullptr;
    // All storage is native-owned and distinct from the source template.
    // Keep the single native callback and compatible presentation on its timeline.
    Segment segment=after.segment;segment.sequence=playback;segment.last=length;segment.rate=length/after.length;
    if(!std::isfinite(segment.rate)||segment.rate<=.01f||segment.rate>100)return nullptr;
    for(int i=0;i<presentation.count;++i)if(!AdaptRoot(presentation.events[i],montage,source))return nullptr;
    // Use the game's 184-byte array reserve helper, on the private clone only.
    // No CRT allocation is ever handed to Unreal's array destructor.
    if(presentation.count>after.events.capacity)g_reserve(reinterpret_cast<void*>(montage+0x80),presentation.count);
    Array storage{};
    if(!List(montage,0x80,storage,512)||storage.data==before.events.data||storage.capacity<presentation.count||
       !Writable(storage.data,size_t(presentation.count)*sizeof(Event)))return nullptr;
    g_skeleton(created,reinterpret_cast<void*>(source.skeleton));
    const float zero=0;
    for(int i=0;i<presentation.count;++i){
        const auto at=storage.data+size_t(i)*sizeof(Event);std::memcpy(&time,presentation.events[i].data()+0x24,sizeof(time));
        if(!Store(at,0,presentation.events[i])||!Link(at,montage,playback,time,after.length)||
           !Link(at+0x60,montage,playback,time,after.length))return nullptr;
    }
    if(!Store(after.segments.data,0,segment)||!Link(after.sections.data,montage,playback,zero,after.length)||
       !Store(montage,0x88,presentation.count))return nullptr;
    return created;
}
namespace {
bool Named(uintptr_t p,const char* text){int index=0,number=0;return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(g_base,index,number,text);}
bool TransferMontage(uintptr_t p,bool beam){
    uintptr_t package=0;if(!Read(p,0x20,package))return false;
    if(beam)return Is(p,"Strife_Aim_WorldEnder_Montage")&&Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Aim_WorldEnder_Montage");
    constexpr const char* names[]={"Strife_SpawnClone_Back_Montage","Strife_SpawnClone_F_Montage"};
    char path[200]{};
    for(const auto* name:names)if(Named(p,name)){sprintf_s(path,"/Game/Characters/Heroes/Hero_Strife/Animations/%s",name);return Named(package,path);}
    return false;
}
bool Gather(uintptr_t asset,const Array& list,float first,float last,float start,float rate,Presentation& plan){
    for(int i=0;i<list.count;++i){
        const auto at=list.data+size_t(i)*sizeof(Event);uintptr_t notify=0,cls=0,state=0;float time=0,duration=0;unsigned char tick=255;
        if(!Read(at,0x48,notify))return false;if(!notify)continue;
        if(!Read(notify,0x10,cls))return false;
        const bool callback=Is(cls,"AnimNotify_CallAbilityFunction"),particle=Is(cls,"AnimNotify_PlayParticleEffect");
        if(!callback&&!particle&&!Is(cls,"AnimNotify_PlaySound"))continue;
        if(!Read(at,0x24,time)||!std::isfinite(time))return false;
        if(time<first||time>=last)continue;
        Name socket{};
        if(particle&&!Read(notify,0x84,socket))return false;
        if((socket.index||socket.number)&&!native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Root")&&
           !native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_Strife_Root")&&
           !native_objects::NameValueIs(g_base,socket.index,socket.number,"VFX_Root"))continue;
        uintptr_t owner=0;
        if(plan.count<0||plan.count>=int(plan.events.size())||!Read(notify,0x20,owner)||owner!=asset||!Read(at,0x50,state)||state||
           !Read(at,0x58,duration)||duration!=0||!Read(at,0x91,tick)||tick>1||!Read(at,0,plan.events[plan.count]))return false;
        const float mapped=start+(time-first)/rate;
        if(OpeningSound(notify,mapped))continue;
        std::memcpy(plan.events[plan.count].data()+0x24,&mapped,sizeof(mapped));plan.events[plan.count][0x91]=0;++plan.count;
    }
    return true;
}
bool Interrupts(uintptr_t asset,const Array& events,float first,float last,float start,float rate,Presentation& plan){
    for(int i=0;i<events.count;++i){
        const auto at=events.data+size_t(i)*sizeof(Event);uintptr_t state=0,cls=0,owner=0,notify=0;float time=0,duration=0;
        if(!Read(at,0x50,state))return false;
        if(!state)continue;
        if(!Read(state,0x10,cls))return false;
        if(!Is(cls,"AnimNotify_EnableAbilityInterrupt")&&!Is(cls,"AnimNotify_SoundLoopRange"))continue;
        if(!Read(at,0x24,time)||!std::isfinite(time))return false;
        if(time<first||time>=last)continue;
        if(plan.count>=int(plan.events.size())||!Read(state,0x20,owner)||owner!=asset||!Read(at,0x48,notify)||notify||
           !Read(at,0x58,duration)||!std::isfinite(duration)||duration<=0||time+duration>last+.001f||!Read(at,0,plan.events[plan.count]))return false;
        const float mapped=start+(time-first)/rate,span=duration/rate;
        std::memcpy(plan.events[plan.count].data()+0x24,&mapped,4);std::memcpy(plan.events[plan.count].data()+0x58,&span,4);
        plan.events[plan.count][0x91]=0;++plan.count;
    }
    return true;
}
bool BeamCallbacks(const Presentation& plan){
    int starts=0,ends=0;
    for(int i=0;i<plan.count;++i){
        uintptr_t notify=0,cls=0;Name function{};float time=0;
        std::memcpy(&notify,plan.events[i].data()+0x48,8);if(!notify)continue;
        if(!Read(notify,0x10,cls))return false;
        if(!Is(cls,"AnimNotify_CallAbilityFunction"))continue;
        if(!Read(notify,0x38,function))return false;
        std::memcpy(&time,plan.events[i].data()+0x24,4);
        if(native_objects::NameValueIs(g_base,function.index,function.number,"StartBeam")&&std::fabs(time-.6564341187f)<.0001f)++starts;
        else if(native_objects::NameValueIs(g_base,function.index,function.number,"DestroyBeam")&&std::fabs(time-3.5871195793f)<.0001f)++ends;
        else return false;
    }
    return starts==1&&ends==1;
}
bool Rebind(Event& event,uintptr_t montage,const Source& source){
    uintptr_t notify=0,cls=0;std::memcpy(&notify,event.data()+0x48,sizeof(notify));
    if(!notify){uintptr_t state=0;return Read(reinterpret_cast<uintptr_t>(event.data()),0x50,state)&&Read(state,0x10,cls)&&
        (Is(cls,"AnimNotify_EnableAbilityInterrupt")||Is(cls,"AnimNotify_SoundLoopRange"));}
    if(!Read(notify,0x10,cls))return false;
    if(!Is(cls,"AnimNotify_PlayParticleEffect"))return true;
    constexpr size_t offset=0x84;Name socket{};if(!Read(notify,offset,socket))return false;
    if(!socket.index&&!socket.number)return true;
    const auto copy=reinterpret_cast<uintptr_t>(g_construct(reinterpret_cast<void*>(cls),reinterpret_cast<void*>(montage),{},0x40u,0,reinterpret_cast<void*>(notify),false,nullptr,false));
    uintptr_t actual=0,outer=0;uint32_t flags=0;
    if(!copy||copy==notify||!Read(copy,0x10,actual)||actual!=cls||!Read(copy,0x20,outer)||outer!=montage||
       !Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u))return false;
    std::array<unsigned char,0x54> payload{};if(!Read(notify,0x38,payload)||!Store(copy,0x38,payload))return false;
    if(!Store(copy,offset,Name{source.rootIndex,source.rootNumber}))return false;
    std::memcpy(event.data()+0x48,&copy,sizeof(copy));return true;
}
void* Retarget(uintptr_t original,const Source& source,bool beam){
    if(!g_ready||!g_construct||!g_package||!g_skeleton||!g_reserve||!native_tps::OnGameThread()||!TransferMontage(original,beam))return nullptr;
    uintptr_t cls=0,skeleton=0;Array tracks{},segments{},events{},sections{},idleEvents{};float length=0,rate=0,idleLength=0;
    if(!Read(source.sequence,0x10,cls)||!Is(cls,"AnimSequence")||(!Is(source.sequence,"Strife_Idle")&&!Is(source.sequence,"War_Idle"))||
       !Read(source.sequence,0x38,skeleton)||skeleton!=source.skeleton||!Read(source.sequence,0x90,idleLength)||!std::isfinite(idleLength)||idleLength<=.1f||idleLength>=10||
       !List(source.sequence,0x80,idleEvents,128)||idleEvents.count||
       (!native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_War_Root")&&!native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_Strife_Root"))||
       !Read(original,0x10,cls)||!Is(cls,"AnimMontage")||!Read(original,0x90,length)||!std::isfinite(length)||length<=.1f||length>=20||
       !Read(original,0x94,rate)||rate!=1||!List(original,0x158,tracks,4)||tracks.count!=1||!List(tracks.data,8,segments,8)||!segments.count||
       !List(original,0x80,events,128)||!List(original,0x148,sections,16)||!sections.count||!Empty(original,0x168)||!Empty(original,0x128)||!Empty(original,0x138))return nullptr;
    Name slot{};if(!Read(tracks.data,0,slot)||slot.index!=source.slotIndex||slot.number!=source.slotNumber)return nullptr;
    if(beam&&(segments.count!=3||std::fabs(length-4.8999996185f)>.0001f||
       !native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_War_Root")))return nullptr;
    const auto playback=CastingSequence(source.sequence,source.skeleton,idleLength);
    // Use War's own cast at authored speed. Preserve the complete beam's
    // damage interval; only its wind-up and return are shortened.
    const bool quick=playback!=source.sequence&&Is(source.skeleton,"War_Mesh_Skeleton");
    const float oldLength=length;
    const auto mapTime=[&](float time){
        if(!quick)return time;
        if(!beam)return time*idleLength/oldLength;
        constexpr float begin=.6564341187f,fire=3.5871195793f-begin;
        if(time<=begin)return time*idleLength/begin;
        if(time<=begin+fire)return idleLength+time-begin;
        return idleLength+fire+(time-begin-fire)*.2f/(oldLength-begin-fire);
    };
    Presentation plan{};
    for(int i=0;i<segments.count;++i){
        Segment segment{};Array notifies{};
        if(!Read(segments.data,size_t(i)*sizeof(Segment),segment)||!segment.valid||segment.loops<1||segment.loops>10||
           !std::isfinite(segment.first)||!std::isfinite(segment.last)||!std::isfinite(segment.start)||!std::isfinite(segment.rate)||
           segment.first<0||segment.last<=segment.first||segment.rate<=0||segment.rate>10||segment.start<0||
           !Read(segment.sequence,0x10,cls)||!Is(cls,"AnimSequence")||!List(segment.sequence,0x80,notifies,128))return nullptr;
        const float duration=(segment.last-segment.first)/segment.rate;
        for(int loop=0;loop<segment.loops;++loop){
            const float start=segment.start+float(loop)*duration;
            if(!Gather(segment.sequence,notifies,segment.first,segment.last,start,segment.rate,plan)||
               (beam&&!Interrupts(segment.sequence,notifies,segment.first,segment.last,start,segment.rate,plan)))return nullptr;
        }
    }
    if(!Gather(original,events,0,length,0,1,plan)||(beam&&(!Interrupts(original,events,0,length,0,1,plan)||!BeamCallbacks(plan))))return nullptr;
    std::stable_sort(plan.events.begin(),plan.events.begin()+plan.count,[](const Event& a,const Event& b){float x=0,y=0;std::memcpy(&x,a.data()+0x24,4);std::memcpy(&y,b.data()+0x24,4);return x<y;});
    const auto outer=reinterpret_cast<uintptr_t>(g_package());
    if(!Read(original,0x10,cls))return nullptr;
    auto* created=g_construct(reinterpret_cast<void*>(cls),reinterpret_cast<void*>(outer),{},0x40u,0,reinterpret_cast<void*>(original),false,nullptr,false);
    const auto copy=reinterpret_cast<uintptr_t>(created);uintptr_t actual=0,actualOuter=0;uint32_t flags=0;Array privateTracks{},privateSegments{},privateSections{},privateEvents{};
    if(!copy||copy==original||!Read(copy,0x10,actual)||actual!=cls||!Read(copy,0x20,actualOuter)||actualOuter!=outer||
       !Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u)||!List(copy,0x158,privateTracks,4)||privateTracks.count!=tracks.count||privateTracks.data==tracks.data||
       !List(privateTracks.data,8,privateSegments,8)||privateSegments.count!=segments.count||privateSegments.data==segments.data||
       !List(copy,0x148,privateSections,16)||privateSections.count!=sections.count||privateSections.data==sections.data||
       !List(copy,0x80,privateEvents,128)||(events.data&&privateEvents.data==events.data)||
       !Writable(copy+0x80,16)||!Writable(privateSegments.data,size_t(segments.count)*sizeof(Segment))||!Writable(privateSections.data,size_t(sections.count)*88))return nullptr;
    for(int i=0;i<plan.count;++i)if(!Rebind(plan.events[i],copy,source))return nullptr;
    length=mapTime(oldLength);
    if(quick&&!Store(copy,0x90,length))return nullptr;
    if(plan.count>privateEvents.capacity)g_reserve(reinterpret_cast<void*>(copy+0x80),plan.count);
    if(!List(copy,0x80,privateEvents,512)||privateEvents.capacity<plan.count||(events.data&&privateEvents.data==events.data)||
       (plan.count&&!Writable(privateEvents.data,size_t(plan.count)*sizeof(Event))))return nullptr;
    g_skeleton(created,reinterpret_cast<void*>(source.skeleton));
    for(int i=0;i<segments.count;++i){
        Segment s{};if(!Read(segments.data,size_t(i)*sizeof(Segment),s))return nullptr;
        const float span=(mapTime(s.start+(s.last-s.first)/s.rate*float(s.loops))-mapTime(s.start))/float(s.loops);
        s.start=mapTime(s.start);
        s.sequence=playback;s.first=0;s.last=idleLength;s.rate=idleLength/span;
        if(beam){
            // Start once, hold the last source frame during the complete beam,
            // then return to War's own idle. The full fire interval is retained.
            const float total=span*float(s.loops);s.loops=1;
            if(i==1)s.first=std::max(0.f,idleLength-1.f/30.f);
            if(i==2){s.sequence=source.sequence;s.first=0;if(!Read(source.sequence,0x90,s.last))return nullptr;}
            s.rate=(s.last-s.first)/total;
        }
        if(!std::isfinite(s.rate)||s.rate<=.01f||s.rate>100||!Store(privateSegments.data,size_t(i)*sizeof(Segment),s))return nullptr;
    }
    for(int i=0;i<plan.count;++i){
        const auto at=privateEvents.data+size_t(i)*sizeof(Event);float time=0;std::memcpy(&time,plan.events[i].data()+0x24,4);
        float duration=0;std::memcpy(&duration,plan.events[i].data()+0x58,4);
        const float end=mapTime(time+duration);time=mapTime(time);duration=end-time;
        std::memcpy(plan.events[i].data()+0x24,&time,4);std::memcpy(plan.events[i].data()+0x30,&time,4);std::memcpy(plan.events[i].data()+0x58,&duration,4);
        // Trigger offsets follow their local interval, including notify states.
        for(size_t offset:{size_t(0x34),size_t(0x38)}){float value=0;std::memcpy(&value,plan.events[i].data()+offset,4);
            if(!std::isfinite(value))return nullptr;value*=length/oldLength;std::memcpy(plan.events[i].data()+offset,&value,4);}
        if(!std::isfinite(time)||time<0||time>=length||!std::isfinite(duration)||duration<0||time+duration>length+.001f||
           !Store(at,0,plan.events[i])||!Link(at,copy,playback,time,length)||!Link(at+0x60,copy,playback,std::min(time+duration,length),length))return nullptr;
    }
    for(int i=0;i<sections.count;++i){
        float time=0;const auto at=privateSections.data+size_t(i)*88;
        if(!Read(at,0x24,time)||!std::isfinite(time)||time<0||time>=oldLength||!Link(at,copy,playback,mapTime(time),length))return nullptr;
    }
    const int zero=0;
    if(!Store(copy,0x88,plan.count)||!Store(copy,0x188,zero)||!Store(copy,0x198,zero))return nullptr;
    return created;
}
}
void* Compatible(uintptr_t original,const Source& source){return Retarget(original,source,false);}
void* WorldEnder(uintptr_t original,const Source& source){return Retarget(original,source,true);}
namespace {
struct MotionLayout {uintptr_t cls=0;Array tracks{},segments{},sections{},events{},markers{},states{};};
int RampageAsset(uintptr_t p){
    constexpr const char* names[]={"War_Atk_Rampage_Start_Montage","War_Atk_Rampage_Land_Montage",
        "War_Atk_Rampage_Strike_Level1_Montage","War_Atk_Rampage_Strike_Level2_Montage","War_Atk_Rampage_Strike_Level3_Montage"};
    uintptr_t package=0;if(!Read(p,0x20,package))return -1;
    for(int i=0;i<5;++i)if(Named(p,names[i])){char path[200]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",names[i]);return Named(package,path)?i:-1;}
    return -1;
}
bool MotionShape(uintptr_t p,int kind,MotionLayout& out){
    uintptr_t cls=0,skeleton=0;float length=0,rate=0;Name slot{};
    if(kind<0||kind>4||!Read(p,0x10,out.cls)||!Is(out.cls,"AnimMontage")||!Read(out.cls,0x10,cls)||!Is(cls,"Class")||
       !Read(p,0x38,skeleton)||!Is(skeleton,"War_Mesh_Skeleton")||!Read(p,0x90,length)||
       !std::isfinite(length)||std::fabs(length-(kind==0?1.4f:(kind==1?1.3666667f:1.2666667f)))>.00001f||
       !Read(p,0x94,rate)||rate!=(kind==0?1.5f:1.f)||!List(p,0x158,out.tracks,4)||out.tracks.count!=1||
       !Read(out.tracks.data,0,slot)||!native_objects::NameValueIs(g_base,slot.index,slot.number,"TPS")||
       !List(out.tracks.data,8,out.segments,4)||out.segments.count!=(kind==0?2:1)||
       !List(p,0x148,out.sections,4)||out.sections.count!=(kind==0?2:1)||!List(p,0x80,out.events,128)||
       !List(p,0x180,out.markers,256)||!List(p,0x190,out.states,128)||!Empty(p,0x128)||!Empty(p,0x138)||!Empty(p,0x168))return false;
    for(int i=0;i<out.segments.count;++i){
        Segment s{};uintptr_t sequenceSkeleton=0,package=0;unsigned char motion=0;char path[200]{};
        const char* name=kind==0?(i?"War_Atk_Rampage_Run":"War_Atk_Rampage_Start"):(kind==1?"War_Dash_F_Air_Land":"War_Atk_Rampage_Strike");
        sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",name);
        if(!Read(out.segments.data,size_t(i)*sizeof(Segment),s)||!Read(s.sequence,0x10,cls)||!Is(cls,"AnimSequence")||!Named(s.sequence,name)||
           !Read(s.sequence,0x20,package)||!Named(package,path)||!Read(s.sequence,0x38,sequenceSkeleton)||sequenceSkeleton!=skeleton||
           s.valid!=1||s.loops!=1||s.rate!=1||s.first!=0||!std::isfinite(s.last)||
           std::fabs(s.last-(kind==0?(i?1.0666667f:.33333334f):length))>.00001f||
           std::fabs(s.start-(i?.33333334f:0.f))>.00001f||
           (kind==0&&(!Read(s.sequence,0x1A1,motion)||motion!=1)))return false;
    }
    return true;
}
template<size_t Stride> bool Separate(const Array& original,const Array& copy,uintptr_t sourceOwner=0,uintptr_t privateOwner=0){
    if(copy.count!=original.count||(original.data&&copy.data==original.data))return false;
    if(!original.count)return true;
    const size_t bytes=size_t(original.count)*Stride;
    if(!Writable(copy.data,bytes))return false;
    // Read through the same bounded memory reader before comparing the arrays.
    for(size_t at=0;at<bytes;at+=Stride){
        std::array<unsigned char,Stride> a{},b{};if(!Read(original.data,at,a)||!Read(copy.data,at,b))return false;
        if constexpr(Stride==88){
            // UObject instancing may already relink a section to its clone.
            uintptr_t before=0,after=0;std::memcpy(&before,a.data()+8,8);std::memcpy(&after,b.data()+8,8);
            if(before==sourceOwner&&after==privateOwner)std::memcpy(b.data()+8,&before,8);
        }
        if(a!=b)return false;
    }
    return true;
}
bool PrivateEvents(const Array& original,const Array& copy){
    return original.count==copy.count&&(!original.data||copy.data!=original.data)&&
        (!original.count||Writable(copy.data,size_t(original.count)*sizeof(Event)));
}
// Source-sequence cues are queued by slot in this engine. DefaultSlot is
// deliberately absent from Strife's graph: promote only charge gameplay/FX
// onto the private montage timeline, which the engine dispatches independently.
bool ChargeEvents(const MotionLayout& layout,Presentation& plan){
    unsigned blasts=0,surfaces=0,shields=0,collisions=0;
    for(int segmentIndex=0;segmentIndex<layout.segments.count;++segmentIndex){
        Segment segment{};Array events{};
        if(!Read(layout.segments.data,size_t(segmentIndex)*sizeof(Segment),segment)||!List(segment.sequence,0x80,events,128))return false;
        for(int i=0;i<events.count;++i){
            const auto at=events.data+size_t(i)*sizeof(Event);uintptr_t notify=0,state=0,cls=0,owner=0;float time=0,duration=0;unsigned char tick=255;
            if(!Read(at,0x48,notify)||!Read(at,0x50,state)||(notify&&state)||!Read(at,0x24,time)||!std::isfinite(time))return false;
            if(time<segment.first||time>=segment.last)continue;
            const auto object=notify?notify:state;if(!object)continue;
            if(!Read(object,0x10,cls))return false;
            const bool blast=Is(cls,"AnimNotify_SpawnBlast"),surface=Is(cls,"AnimNotify_PlaySurfaceParticleEffect");
            const bool shield=Is(cls,"AnimNotifyState_TimedParticleEffect"),callback=Is(cls,"AnimNotify_CallAbilityFunction");
            if(!blast&&!surface&&!shield&&!callback)continue; // audio remains on its validated path
            Name function{};
            if(callback&&(!Read(object,0x38,function)||!native_objects::NameValueIs(g_base,function.index,function.number,"EnableStopOnCollision")))return false;
            if((shield?!state:!notify)||!Read(object,0x20,owner)||owner!=segment.sequence||
               !Read(at,0x58,duration)||!std::isfinite(duration)||duration<0||
               (shield?(duration<=0||time+duration>segment.last+.001f):duration!=0)||
               !Read(at,0x91,tick)||tick>1||plan.count>=int(plan.events.size())||!Read(at,0,plan.events[plan.count]))return false;
            const float mapped=segment.start+(time-segment.first)/segment.rate,span=duration/segment.rate;
            auto& event=plan.events[plan.count++];std::memcpy(event.data()+0x24,&mapped,4);std::memcpy(event.data()+0x58,&span,4);
            event[0x91]=0; // queued native notify/state lifecycle, including cancellation cleanup
            ++(blast?blasts:surface?surfaces:shield?shields:collisions);
        }
    }
    return blasts==1&&surfaces==6&&shields==1&&collisions==1;
}
uintptr_t RampageNotify(uintptr_t notify,uintptr_t outer,const Source& source);
bool MotionNotify(Event& event,uintptr_t montage,const Source& source){
    uintptr_t notify=0;size_t at=0x48;std::memcpy(&notify,event.data()+at,8);
    if(!notify){at=0x50;std::memcpy(&notify,event.data()+at,8);if(!notify)return true;}
    const auto copy=RampageNotify(notify,montage,source);if(!copy)return false;
    std::memcpy(event.data()+at,&copy,8);return true;
}
uintptr_t RampageNotify(uintptr_t notify,uintptr_t outer,const Source& source){
    if(!g_ready||!g_construct||!native_tps::OnGameThread())return 0;
    uintptr_t cls=0,blast=0;int socketAt=0,blastAt=0;Name socket{},replacement{};bool particle=false,timed=false;
    if(!Read(notify,0x10,cls))return 0;
    if(Is(cls,"AnimNotify_SpawnBlast")){
        if(!native_objects::PropertyField(g_base,cls,"SpawnSocket","NameProperty",8,socketAt)||
           !native_objects::PropertyField(g_base,cls,"BlastClass","ClassProperty",8,blastAt)||!Read(notify,size_t(blastAt),blast)||!blast)return 0;
    }else if(Is(cls,"AnimNotify_PlayParticleEffect")||Is(cls,"AnimNotify_PlaySurfaceParticleEffect")){
        particle=true;
        if(!native_objects::PropertyField(g_base,cls,"SocketName","NameProperty",8,socketAt)||socketAt!=0x84)return 0;
    }else if(Is(cls,"AnimNotifyState_TimedParticleEffect")){
        timed=true;uintptr_t effect=0,package=0;int templateAt=0;
        if(!native_objects::PropertyField(g_base,cls,"SocketName","NameProperty",8,socketAt)||
           !native_objects::PropertyField(g_base,cls,"PSTemplate","ObjectProperty",8,templateAt)||
           !Read(notify,size_t(templateAt),effect)||!Is(effect,"P_Rampage_Shield")||!Read(effect,0x20,package)||
           !Is(package,"/Game/VFX/Particles/Characters/War/Rampage/P_Rampage_Shield"))return 0;
    }else return notify;
    if(!Read(notify,size_t(socketAt),socket))return 0;
    if(native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Spine1")){
        if(!native_objects::NameValueIs(g_base,source.spineIndex,source.spineNumber,"Bone_Strife_Spine1"))return 0;
        replacement={source.spineIndex,source.spineNumber};
    }else if(native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Root")||
             native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Hand_L"))replacement={source.rootIndex,source.rootNumber};
    else if(timed&&native_objects::NameValueIs(g_base,socket.index,socket.number,"VFX_Root"))replacement={source.rootIndex,source.rootNumber};
    else return notify;
    const auto copy=reinterpret_cast<uintptr_t>(g_construct(reinterpret_cast<void*>(cls),reinterpret_cast<void*>(outer),{},0x40u,0,reinterpret_cast<void*>(notify),false,nullptr,false));
    uintptr_t actual=0,parent=0,copiedBlast=0;uint32_t flags=0;
    if(!copy||copy==notify||!Read(copy,0x10,actual)||actual!=cls||!Read(copy,0x20,parent)||parent!=outer||
       !Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u)||(blast&&(!Read(copy,size_t(blastAt),copiedBlast)||copiedBlast!=blast))||
       !Store(copy,size_t(socketAt),replacement))return 0;
    // Preserve the existing particle quaternion/cache as well as reflected
    // fields. NewObject need not copy non-UPROPERTY native cached values.
    if(particle){std::array<unsigned char,0x54> payload{};
        if(!Read(notify,0x38,payload)||!Store(copy,0x38,payload)||!Store(copy,size_t(socketAt),replacement))return 0;}
    return copy;
}
}
void* RampageMotion(uintptr_t original,const Source& source){
    if(!g_ready||!g_construct||!g_package||!g_skeleton||!native_tps::OnGameThread()||
       !native_objects::NameValueIs(g_base,source.slotIndex,source.slotNumber,"DefaultSlot")||
       !native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_Strife_Root")||!Is(source.skeleton,"Strife_Skeleton"))return nullptr;
    // CanBuild's shared idle contract expects TPS; the private output uses the
    // unused DefaultSlot, independently verified against the live Strife graph.
    uintptr_t cls=0,idleSkeleton=0;float length=0,rate=0;
    if(!Read(source.sequence,0x10,cls)||!Is(cls,"AnimSequence")||!Is(source.sequence,"Strife_Idle")||
       !Read(source.sequence,0x38,idleSkeleton)||idleSkeleton!=source.skeleton||!Read(source.sequence,0x90,length)||
       !std::isfinite(length)||length<=.1f||length>=10||!Read(source.sequence,0x94,rate)||rate!=1||!Empty(source.sequence,0x80))return nullptr;
    auto fail=[](const char* why)->void*{if(g_log&&g_motionRefusals++<8){char line[200]{};sprintf_s(line,"Rampage V1.105 montage refused: %s",why);g_log(line);}return nullptr;};
    const int kind=RampageAsset(original);MotionLayout before{},after{};
    const auto outer=reinterpret_cast<uintptr_t>(g_package());
    if(!Read(outer,0x10,cls)||!Is(cls,"Package")||!Is(outer,"/Engine/Transient")||!MotionShape(original,kind,before))return fail("source phase, native rate or layout");
    Presentation charge{};
    if(kind==0&&(!g_reserve||before.events.count||before.markers.count||before.states.count||!ChargeEvents(before,charge)))return fail("native charge cues or unused-slot timeline");
    auto* created=g_construct(reinterpret_cast<void*>(before.cls),reinterpret_cast<void*>(outer),{},0x40u,0,reinterpret_cast<void*>(original),false,nullptr,false);
    const auto copy=reinterpret_cast<uintptr_t>(created);uintptr_t actual=0;uint32_t flags=0;
    if(!copy||copy==original||!Read(copy,0x20,actual)||actual!=outer||!Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u)||
       !MotionShape(copy,kind,after)||after.cls!=before.cls||!Separate<32>(before.segments,after.segments)||
       !Separate<88>(before.sections,after.sections,original,copy)||!PrivateEvents(before.events,after.events)||
       !Separate<12>(before.markers,after.markers)||!Separate<4>(before.states,after.states)||
       after.tracks.data==before.tracks.data||!Writable(after.tracks.data,24))return fail("private clone identity, arrays or source equality");
    // Motion clips, section loops and native rates stay intact. No source
    // object is edited, no manual blast/FX call or extra detour is needed.
    if(kind==0){
        if(charge.count>after.events.capacity)g_reserve(reinterpret_cast<void*>(copy+0x80),charge.count);
        Array storage{};
        if(!List(copy,0x80,storage,128)||storage.data==before.events.data||storage.capacity<charge.count||
           !Writable(storage.data,size_t(charge.count)*sizeof(Event)))return fail("private charge storage");
        for(int i=0;i<charge.count;++i){
            auto event=charge.events[i];float time=0,duration=0;std::memcpy(&time,event.data()+0x24,4);std::memcpy(&duration,event.data()+0x58,4);
            const int index=time>=.33333334f?1:0;Segment segment{};
            if(!Read(before.segments.data,size_t(index)*sizeof(Segment),segment)||!MotionNotify(event,copy,source))return fail("private charge notify/socket");
            const auto at=storage.data+size_t(i)*sizeof(Event);
            if(!Store(at,0,event)||!Link(at,copy,segment.sequence,time,segment.last)||!Link(at+0x60,copy,segment.sequence,time+duration,segment.last)||
               !Store(at,0x14,index)||!Store(at,0x74,index)||!Store(at,0x1C,segment.start)||!Store(at,0x7C,segment.start))return nullptr;
        }
        if(!Store(copy,0x88,charge.count))return nullptr;
    }else for(int i=0;i<after.events.count;++i){
        Event event{};const auto at=after.events.data+size_t(i)*sizeof(Event);
        if(!Read(before.events.data,size_t(i)*sizeof(Event),event)||!MotionNotify(event,copy,source)||!Store(at,0,event)||!Store(at,8,copy)||!Store(at,0x68,copy))return fail("private event or impact socket");
    }
    for(int i=0;i<after.sections.count;++i)if(!Store(after.sections.data+size_t(i)*88,8,copy))return nullptr;
    if(!Store(after.tracks.data,0,Name{source.slotIndex,source.slotNumber}))return nullptr;
    g_skeleton(created,reinterpret_cast<void*>(source.skeleton));
    if(!Read(copy,0x38,actual)||actual!=source.skeleton)return nullptr;
    return created;
}
namespace {
bool SoundValid(uintptr_t notify){
    uintptr_t cls=0,sound=0,vt=0,entry=0;Name socket{};float volume=0,pitch=0;
    return g_soundNotify&&Read(notify,0x10,cls)&&Is(cls,"AnimNotify_PlaySound")&&
        Read(notify,0,vt)&&Read(vt,0x240,entry)&&entry==reinterpret_cast<uintptr_t>(g_soundNotify)&&
        Read(notify,0x38,sound)&&Read(sound,0x10,cls)&&(Is(cls,"SoundCue")||Is(cls,"SoundWave"))&&
        Read(notify,0x40,volume)&&std::isfinite(volume)&&volume>0&&volume<=10&&
        Read(notify,0x44,pitch)&&std::isfinite(pitch)&&pitch>.01f&&pitch<=4&&
        Read(notify,0x4C,socket)&&socket.index==0&&socket.number==0;
}
bool OpeningSound(uintptr_t notify,float time){return time==0&&SoundValid(notify);}
struct Cue {Event event{};uintptr_t notify=0,asset=0;float time=0,rate=1;};
struct Sounds {std::array<Cue,32> cues{};unsigned count=0;float length=0;};
bool AudioAsset(uintptr_t p){
    if(Original(p)||TransferMontage(p,false)||TransferMontage(p,true)||RampageAsset(p)>=0)return true;
    constexpr const char* names[]={"War_Atk_Stoneskin_Montage","War_Atk_Stoneskin_Molten_Montage",
        "Strife_Caltrops_Ground_F_Montage","Strife_Caltrops_Ground_B_Montage"};
    uintptr_t package=0;if(!Read(p,0x20,package))return false;
    for(unsigned i=0;i<4;++i)if(Named(p,names[i])){
        char path[200]{};sprintf_s(path,i<2?"/Game/Characters/Heroes/Hero_War/Animation/%s":"/Game/Characters/Heroes/Hero_Strife/Animations/%s",names[i]);
        return Named(package,path);
    }
    return false;
}
bool CueList(uintptr_t asset,float first,float last,float start,float rate,Sounds& out){
    Array events{};if(!List(asset,0x80,events,128))return false;
    for(int i=0;i<events.count;++i){
        const auto at=events.data+size_t(i)*sizeof(Event);uintptr_t notify=0,cls=0,owner=0,state=0;float time=0,duration=0;
        if(!Read(at,0x48,notify))return false;
        if(!notify)continue;
        if(!Read(notify,0x10,cls))return false;
        if(!Is(cls,"AnimNotify_PlaySound"))continue;
        if(!Read(at,0x24,time)||!std::isfinite(time))return false;
        if(time<first||time>=last)continue;
        if(out.count>=out.cues.size()||!SoundValid(notify)||!Read(notify,0x20,owner)||owner!=asset||
           !Read(at,0x50,state)||state||!Read(at,0x58,duration)||duration!=0)return false;
        const float mapped=start+(time-first)/rate;if(!std::isfinite(mapped)||mapped<0||mapped>=out.length)return false;
        auto& cue=out.cues[out.count];
        if(!Read(at,0,cue.event))return false;
        // A source notify represented in both timelines must not play twice.
        bool duplicate=false;for(unsigned j=0;j<out.count;++j)if(out.cues[j].notify==notify&&out.cues[j].time==mapped)duplicate=true;
        if(!duplicate){cue.notify=notify;cue.asset=asset;cue.time=mapped;cue.rate=rate;++out.count;}
    }
    return true;
}
bool SoundPlan(uintptr_t original,Sounds& out){
    uintptr_t cls=0;Array tracks{},segments{};
    if(!g_soundNotify||!AudioAsset(original)||!Read(original,0x10,cls)||!Is(cls,"AnimMontage")||
       !Read(original,0x90,out.length)||!std::isfinite(out.length)||out.length<=.1f||out.length>=20||
       !List(original,0x158,tracks,4)||tracks.count!=1||!List(tracks.data,8,segments,8)||!segments.count||
       !CueList(original,0,out.length,0,1,out))return false;
    for(int i=0;i<segments.count;++i){
        Segment s{};if(!Read(segments.data,size_t(i)*sizeof(Segment),s)||!s.valid||s.loops<1||s.loops>10||
           !std::isfinite(s.first)||!std::isfinite(s.last)||!std::isfinite(s.start)||!std::isfinite(s.rate)||
           s.first<0||s.last<=s.first||s.start<0||s.rate<=0||s.rate>10||!Read(s.sequence,0x10,cls)||!Is(cls,"AnimSequence"))return false;
        for(int loop=0;loop<s.loops;++loop)if(!CueList(s.sequence,s.first,s.last,s.start+float(loop)*(s.last-s.first)/s.rate,s.rate,out))return false;
    }
    return true;
}
}
void StartSounds(uintptr_t original,void* liveAbility){
    if(!g_ready||!g_soundNotify||!native_tps::OnGameThread())return;
    const auto ability=reinterpret_cast<uintptr_t>(liveAbility);uintptr_t actor=0,character=0,mesh=0,owner=0,controller=0,pawn=0,cls=0;unsigned char state=0,local=0,role=0;
    Sounds plan{};native_objects::Identity actorId{},abilityId{};
    if(!Read(ability,0xD8,state)||(state!=1&&state!=2)||!Read(ability,0x48,actor)||!Read(ability,0x1B0,character)||character!=actor||
       !Read(actor,0x10,cls)||(!Is(cls,"Strife_Blueprint_C")&&!Is(cls,"War_Blueprint_C"))||
       !Read(actor,0x118,role)||role!=3||!Read(actor,0x348,controller)||!Read(controller,0x350,pawn)||pawn!=actor||
       !Read(controller,0x654,local)||local!=1||!Read(actor,0x380,mesh)||!Read(mesh,0xE8,owner)||owner!=actor||
       !native_objects::CaptureLive(g_base,liveAbility,abilityId)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),actorId)||!SoundPlan(original,plan))return;
    native_objects::MemoryReader reader;unsigned count=0;
    for(unsigned i=0;i<plan.count;++i){
        const auto& cue=plan.cues[i];if(cue.time!=0)continue;
        if(!native_objects::Alive(reader,g_base,actorId)||!native_objects::Alive(reader,g_base,abilityId)||
           !Read(ability,0x48,owner)||owner!=actor||!Read(actor,0x380,owner)||owner!=mesh||!SoundValid(cue.notify))return;
        // Same native notify entry as an animation event: cue, mixer,
        // attenuation, volume, pitch and attachment remain engine-owned.
        g_soundNotify(reinterpret_cast<void*>(cue.notify),reinterpret_cast<void*>(mesh),reinterpret_cast<void*>(cue.asset));++count;
    }
    if(g_log&&g_soundEvents++<16){char line[180]{};sprintf_s(line,"Foreign audio V1.105 START cues=%u: native sound notify on receiver mesh, no timer or input; audibility unvalidated",count);g_log(line);}
}
bool CopyLaterSounds(uintptr_t original,void* privateMontage){
    if(!g_ready||!g_reserve||!native_tps::OnGameThread())return false;
    const auto copy=reinterpret_cast<uintptr_t>(privateMontage);uintptr_t cls=0,outer=0;uint32_t flags=0;float length=0;Array events{},tracks{},segments{};Sounds plan{};
    if(!copy||copy==original||!SoundPlan(original,plan)||!Read(copy,0x10,cls)||!Is(cls,"AnimMontage")||
       !Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u)||!Read(copy,0x20,outer)||!Is(outer,"/Engine/Transient")||
       !Read(copy,0x90,length)||!std::isfinite(length)||length<=.1f||length>=10||!List(copy,0x80,events,64)||events.count||
       !List(copy,0x158,tracks,4)||tracks.count!=1||!List(tracks.data,8,segments,4)||segments.count!=1)return false;
    Segment playback{};if(!Read(segments.data,0,playback))return false;
    unsigned later=0;const float scale=length/plan.length;
    for(unsigned i=0;i<plan.count;++i){
        auto cue=plan.cues[i];if(cue.time==0)continue;
        const float mapped=cue.time*scale;float offset=0,endOffset=0;
        std::memcpy(&offset,cue.event.data()+0x34,4);std::memcpy(&endOffset,cue.event.data()+0x38,4);
        if(!std::isfinite(offset)||!std::isfinite(endOffset)||mapped>=length)return false;
        offset*=scale/cue.rate;endOffset*=scale/cue.rate;
        std::memcpy(cue.event.data()+0x24,&mapped,4);std::memcpy(cue.event.data()+0x30,&mapped,4);
        std::memcpy(cue.event.data()+0x34,&offset,4);std::memcpy(cue.event.data()+0x38,&endOffset,4);cue.event[0x91]=0;
        cue.time=mapped;plan.cues[later++]=cue;
    }
    if(!later)return true;
    std::stable_sort(plan.cues.begin(),plan.cues.begin()+later,[](const Cue& a,const Cue& b){return a.time<b.time;});
    if(later>unsigned(events.capacity))g_reserve(reinterpret_cast<void*>(copy+0x80),int(later));
    if(!List(copy,0x80,events,512)||unsigned(events.capacity)<later||!Writable(events.data,size_t(later)*sizeof(Event))||!Writable(copy+0x88,4))return false;
    for(unsigned i=0;i<later;++i){
        const auto at=events.data+size_t(i)*sizeof(Event);const auto& cue=plan.cues[i];
        if(!Store(at,0,cue.event)||!Link(at,copy,playback.sequence,cue.time,length)||!Link(at+0x60,copy,playback.sequence,cue.time,length))return false;
    }
    return Store(copy,0x88,int(later));
}
}
