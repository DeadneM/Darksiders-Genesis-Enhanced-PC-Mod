#include "ForeignMontage.h"
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
uintptr_t g_base=0;std::atomic_bool g_ready{false};
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
        ++out.count;if(particle)++out.particles;else ++out.sounds;
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
    if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char construct[]={0x40,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x40};
    constexpr unsigned char package[]={0x48,0x8B,0x05,0x59,0xCD,0xED,0x02,0xC3};
    constexpr unsigned char skeleton[]={0x48,0x85,0xD2,0x74,0x15,0x48,0x3B,0x51,0x38,0x74,0x0F,0x48,0x89,0x51,0x38,0x0F};
    constexpr unsigned char reserve[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x63,0xDA,0x48,0x8B,0xF9,0x85,0xD2,0x74,0x3A,0x48,0x69,0xCB,0xB8};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0xAC57A0),construct,sizeof(construct))||
       std::memcmp(reinterpret_cast<void*>(g_base+0xA7B1B0),package,sizeof(package))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x16C7130),skeleton,sizeof(skeleton))||
       std::memcmp(reinterpret_cast<void*>(g_base+0x2C42C0),reserve,sizeof(reserve)))return;
    g_construct=reinterpret_cast<Construct>(g_base+0xAC57A0);g_package=reinterpret_cast<Package>(g_base+0xA7B1B0);
    g_skeleton=reinterpret_cast<Skeleton>(g_base+0x16C7130);g_reserve=reinterpret_cast<Reserve>(g_base+0x2C42C0);g_ready=true;
    if(log)log("Foreign montage V1.88 READY: exact native NewObject template ABI, private arrays only; no timer or additional hook; EXPERIMENTAL");
}
bool Ready(){return g_ready.load();}
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
    Segment segment=after.segment;segment.sequence=source.sequence;segment.last=length;segment.rate=length/after.length;
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
        if(!Store(at,0,presentation.events[i])||!Link(at,montage,source.sequence,time,after.length)||
           !Link(at+0x60,montage,source.sequence,time,after.length))return nullptr;
    }
    if(!Store(after.segments.data,0,segment)||!Link(after.sections.data,montage,source.sequence,zero,after.length)||
       !Store(montage,0x88,presentation.count))return nullptr;
    return created;
}
namespace {
bool Named(uintptr_t p,const char* text){int index=0,number=0;return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(g_base,index,number,text);}
bool TransferMontage(uintptr_t p){
    uintptr_t package=0;if(!Read(p,0x20,package))return false;
    constexpr const char* strife[]={"Strife_SpawnClone_Back_Montage","Strife_SpawnClone_F_Montage","Strife_Aim_WorldEnder_Montage"};
    constexpr const char* war[]={"War_Atk_Rampage_Start_Montage","War_Atk_Rampage_Land_Montage","War_Atk_Rampage_Strike_Level1_Montage","War_Atk_Rampage_Strike_Level2_Montage","War_Atk_Rampage_Strike_Level3_Montage"};
    char path[200]{};
    for(const auto* name:strife)if(Named(p,name)){sprintf_s(path,"/Game/Characters/Heroes/Hero_Strife/Animations/%s",name);return Named(package,path);}
    for(const auto* name:war)if(Named(p,name)){sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",name);return Named(package,path);}
    return false;
}
bool RootProperty(uintptr_t notify,uintptr_t cls,size_t& offset,Name& socket){
    if(Is(cls,"AnimNotify_PlayParticleEffect")){offset=0x84;return Read(notify,offset,socket);}
    offset=0;socket={};
    for(unsigned depth=0;cls&&depth<16;++depth){
        uintptr_t field=0;if(!Read(cls,0x48,field))return false;
        for(unsigned n=0;field&&n<256;++n){
            uintptr_t type=0,next=0;int size=0,at=0;
            if(Read(field,0x10,type)&&Is(type,"NameProperty")&&Is(field,"SpawnSocket")&&Read(field,0x34,size)&&size==8&&
               Read(field,0x44,at)&&at>=0x30&&at<=0x200){offset=size_t(at);return Read(notify,offset,socket);}
            if(!Read(field,0x28,next)||next==field)return false;field=next;
        }
        uintptr_t parent=0;if(!Read(cls,0x40,parent)||parent==cls)return false;cls=parent;
    }
    return false;
}
bool Gather(uintptr_t asset,const Array& list,float first,float last,float start,float rate,Presentation& plan){
    for(int i=0;i<list.count;++i){
        const auto at=list.data+size_t(i)*sizeof(Event);uintptr_t notify=0,cls=0,state=0;float time=0,duration=0;unsigned char tick=255;
        if(!Read(at,0x48,notify))return false;if(!notify)continue;
        if(!Read(notify,0x10,cls))return false;
        const bool callback=Is(cls,"AnimNotify_CallAbilityFunction"),particle=Is(cls,"AnimNotify_PlayParticleEffect"),blast=Is(cls,"AnimNotify_SpawnBlast");
        if(!callback&&!particle&&!blast&&!Is(cls,"AnimNotify_PlaySound"))continue;
        if(!Read(at,0x24,time)||!std::isfinite(time))return false;
        if(time<first||time>=last)continue;
        size_t socketOffset=0;Name socket{};
        if((particle||blast)&&!RootProperty(notify,cls,socketOffset,socket))return false;
        if((socket.index||socket.number)&&!native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Root")&&
           !native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_Strife_Root")&&
           !native_objects::NameValueIs(g_base,socket.index,socket.number,"VFX_Root")&&
           !(blast&&native_objects::NameValueIs(g_base,socket.index,socket.number,"Bone_War_Hand_L")))continue;
        uintptr_t owner=0;
        if(plan.count<0||plan.count>=int(plan.events.size())||!Read(notify,0x20,owner)||owner!=asset||!Read(at,0x50,state)||state||
           !Read(at,0x58,duration)||duration!=0||!Read(at,0x91,tick)||tick>1||!Read(at,0,plan.events[plan.count]))return false;
        const float mapped=start+(time-first)/rate;
        std::memcpy(plan.events[plan.count].data()+0x24,&mapped,sizeof(mapped));plan.events[plan.count][0x91]=0;++plan.count;
    }
    return true;
}
bool Rebind(Event& event,uintptr_t montage,const Source& source){
    uintptr_t notify=0,cls=0;std::memcpy(&notify,event.data()+0x48,sizeof(notify));
    if(!Read(notify,0x10,cls))return false;
    if(!Is(cls,"AnimNotify_PlayParticleEffect")&&!Is(cls,"AnimNotify_SpawnBlast"))return true;
    size_t offset=0;Name socket{};if(!RootProperty(notify,cls,offset,socket))return false;
    if(!socket.index&&!socket.number)return true;
    const auto copy=reinterpret_cast<uintptr_t>(g_construct(reinterpret_cast<void*>(cls),reinterpret_cast<void*>(montage),{},0x40u,0,reinterpret_cast<void*>(notify),false,nullptr,false));
    uintptr_t actual=0,outer=0;uint32_t flags=0;
    if(!copy||copy==notify||!Read(copy,0x10,actual)||actual!=cls||!Read(copy,0x20,outer)||outer!=montage||
       !Read(copy,8,flags)||(flags&0x30u)||!(flags&0x40u))return false;
    if(Is(cls,"AnimNotify_PlayParticleEffect")){
        std::array<unsigned char,0x54> payload{};if(!Read(notify,0x38,payload)||!Store(copy,0x38,payload))return false;
    }
    if(!offset||!Store(copy,offset,Name{source.rootIndex,source.rootNumber}))return false;
    std::memcpy(event.data()+0x48,&copy,sizeof(copy));return true;
}
}
void* Compatible(uintptr_t original,const Source& source){
    if(!g_ready||!g_construct||!g_package||!g_skeleton||!g_reserve||!native_tps::OnGameThread()||!TransferMontage(original))return nullptr;
    uintptr_t cls=0,skeleton=0;Array tracks{},segments{},events{},sections{},idleEvents{};float length=0,rate=0,idleLength=0;
    if(!Read(source.sequence,0x10,cls)||!Is(cls,"AnimSequence")||(!Is(source.sequence,"Strife_Idle")&&!Is(source.sequence,"War_Idle"))||
       !Read(source.sequence,0x38,skeleton)||skeleton!=source.skeleton||!Read(source.sequence,0x90,idleLength)||!std::isfinite(idleLength)||idleLength<=.1f||idleLength>=10||
       !List(source.sequence,0x80,idleEvents,128)||idleEvents.count||
       (!native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_War_Root")&&!native_objects::NameValueIs(g_base,source.rootIndex,source.rootNumber,"Bone_Strife_Root"))||
       !Read(original,0x10,cls)||!Is(cls,"AnimMontage")||!Read(original,0x90,length)||!std::isfinite(length)||length<=.1f||length>=20||
       !Read(original,0x94,rate)||rate!=1||!List(original,0x158,tracks,4)||tracks.count!=1||!List(tracks.data,8,segments,8)||!segments.count||
       !List(original,0x80,events,128)||!List(original,0x148,sections,16)||!sections.count||!Empty(original,0x168)||!Empty(original,0x128)||!Empty(original,0x138))return nullptr;
    Name slot{};if(!Read(tracks.data,0,slot)||slot.index!=source.slotIndex||slot.number!=source.slotNumber)return nullptr;
    Presentation plan{};
    for(int i=0;i<segments.count;++i){
        Segment segment{};Array notifies{};
        if(!Read(segments.data,size_t(i)*sizeof(Segment),segment)||!segment.valid||segment.loops<1||segment.loops>10||
           !std::isfinite(segment.first)||!std::isfinite(segment.last)||!std::isfinite(segment.start)||!std::isfinite(segment.rate)||
           segment.first<0||segment.last<=segment.first||segment.rate<=0||segment.rate>10||segment.start<0||
           !Read(segment.sequence,0x10,cls)||!Is(cls,"AnimSequence")||!List(segment.sequence,0x80,notifies,128))return nullptr;
        const float duration=(segment.last-segment.first)/segment.rate;
        for(int loop=0;loop<segment.loops;++loop)if(!Gather(segment.sequence,notifies,segment.first,segment.last,segment.start+float(loop)*duration,segment.rate,plan))return nullptr;
    }
    if(!Gather(original,events,0,length,0,1,plan))return nullptr;
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
    if(plan.count>privateEvents.capacity)g_reserve(reinterpret_cast<void*>(copy+0x80),plan.count);
    if(!List(copy,0x80,privateEvents,512)||privateEvents.capacity<plan.count||(events.data&&privateEvents.data==events.data)||
       (plan.count&&!Writable(privateEvents.data,size_t(plan.count)*sizeof(Event))))return nullptr;
    g_skeleton(created,reinterpret_cast<void*>(source.skeleton));
    for(int i=0;i<segments.count;++i){
        Segment s{};if(!Read(segments.data,size_t(i)*sizeof(Segment),s))return nullptr;
        const float span=(s.last-s.first)/s.rate;s.sequence=source.sequence;s.first=0;s.last=idleLength;s.rate=idleLength/span;
        if(!std::isfinite(s.rate)||s.rate<=.01f||s.rate>100||!Store(privateSegments.data,size_t(i)*sizeof(Segment),s))return nullptr;
    }
    for(int i=0;i<plan.count;++i){
        const auto at=privateEvents.data+size_t(i)*sizeof(Event);float time=0;std::memcpy(&time,plan.events[i].data()+0x24,4);
        if(!std::isfinite(time)||time<0||time>=length||!Store(at,0,plan.events[i])||!Link(at,copy,source.sequence,time,length)||!Link(at+0x60,copy,source.sequence,time,length))return nullptr;
    }
    for(int i=0;i<sections.count;++i){
        float time=0;const auto at=privateSections.data+size_t(i)*88;
        if(!Read(at,0x24,time)||!std::isfinite(time)||time<0||time>=length||!Link(at,copy,source.sequence,time,length))return nullptr;
    }
    const int zero=0;
    if(!Store(copy,0x88,plan.count)||!Store(copy,0x188,zero)||!Store(copy,0x198,zero))return nullptr;
    return created;
}
}
