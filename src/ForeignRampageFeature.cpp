#include "ForeignRampageFeature.h"
#include "ForeignMontage.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
#include "PistolAnimationPolicy.h"
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstdio>

namespace dg::foreign_rampage {
namespace {
using Name=foreign_wrath::Name;
using Finish=void(*)(void*,unsigned char);
using Playing=bool(*)(void*);
Finish g_finish=nullptr;Playing g_playing=nullptr;
uintptr_t g_base=0;foreign_wrath::LogFn g_log=nullptr;unsigned g_events=0,g_refusals=0;const char* g_refusal=nullptr;
std::atomic_bool g_ready{false},g_failed{false};
native_objects::Identity g_phases[5]{},g_idle{};
native_objects::Identity g_poses[2]{};
using Notify=void(*)(void*,void*,void*);
using Position=float(*)(void*,void*);
struct Extract {bool root=true;unsigned char pad[3]{};float time=0;};
static_assert(sizeof(Extract)==8&&offsetof(Extract,time)==4);
using Sample=void(*)(void*,pistol_visual::CompactPose*,pistol_visual::Curve*,const Extract*);
Notify g_blast=nullptr,g_surface=nullptr;Position g_position=nullptr;Sample g_sample=nullptr;
struct Visual {native_objects::Identity ability{},actor{},mesh{},anim{},skeleton{},montage{},sequence{};foreign_montage::Source source{};float position=0,length=0;uint64_t tick=0;};
Visual g_visual[2]{};SRWLOCK g_visualLock=SRWLOCK_INIT;std::atomic_uint g_visualMask{0};
Visual* PlayerVisual(int index){if(index==0)return &g_visual[0];if(index==1)return &g_visual[1];return nullptr;}
// One native Rampage spans different ability objects. The actor token
// survives Start's ordinary end and is consumed by the final companion.
struct Cast {native_objects::Identity actor{};uintptr_t world=0;int phase=-1;};Cast g_cast[2]{};
Cast* PlayerCast(int index){if(index==0)return &g_cast[0];if(index==1)return &g_cast[1];return nullptr;}
thread_local void* g_handling=nullptr;
constexpr const char* Names[]={"MayhemAbility_Rampage_Start_Blueprint_C","MayhemAbility_Rampage_Finish_Land_Blueprint_C",
    "MayhemAbility_Rampage_Finish_Slam_Level1_Blueprint_C","MayhemAbility_Rampage_Finish_Slam_Level2_Blueprint_C","MayhemAbility_Rampage_Finish_Slam_Level3_Blueprint_C"};
template<class T> bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
template<size_t N> bool Is(uintptr_t p,const char (&text)[N]){return native_objects::NameIs(g_base,p,text);}
bool Named(uintptr_t p,const char* text){int index=0,number=0;return Read(p,0x18,index)&&Read(p,0x1C,number)&&native_objects::NameValueEquals(g_base,index,number,text);}
bool Alive(const native_objects::Identity& id){native_objects::MemoryReader r;return native_objects::Alive(r,g_base,id);}
bool Class(uintptr_t p,int phase){
    uintptr_t cls=0,package=0;char path[200]{};
    if(phase<0||phase>4)return false;
    // Blueprint package is the class name without its generated _C suffix.
    sprintf_s(path,"/Game/Blueprints/Abilities/Rampage/%.*s",int(std::strlen(Names[phase])-2),Names[phase]);
    return Read(p,0x10,cls)&&Is(cls,"BlueprintGeneratedClass")&&Named(p,Names[phase])&&Read(p,0x20,package)&&Named(package,path);
}
int Kind(uintptr_t cls){for(int i=0;i<5;++i)if(cls==g_phases[i].address&&Alive(g_phases[i])&&Class(cls,i))return i;return -1;}
bool Owner(void* live,local_players::Context& c,bool grounded){
    const auto actor=reinterpret_cast<uintptr_t>(live);uintptr_t cls=0,controller=0,pawn=0,movement=0,owner=0;unsigned char role=0,type=0,mode=0;
    return local_players::ResolveActor(live,c)&&c.index>=0&&c.index<2&&c.localPlayer&&c.world&&c.kind==local_players::PoseKind::Foot&&
        c.character==actor&&c.pawn==actor&&Read(actor,0x10,cls)&&Is(cls,"Strife_Blueprint_C")&&Read(actor,0xC18,type)&&type==1&&
        Read(actor,0x118,role)&&role==3&&Read(actor,0x348,controller)&&controller==c.controller&&Read(controller,0x350,pawn)&&pawn==actor&&
        (!grounded||(Read(actor,0x388,movement)&&Read(movement,0xE8,owner)&&owner==actor&&Read(movement,0x1B0,mode)&&mode==1));
}
bool UnusedSlot(uintptr_t anim,uintptr_t cls,Name motionSlot){
    Name seen[5]{};unsigned slots=0;
    for(int level=0;cls&&level<16;++level){
        uintptr_t field=0;if(!Read(cls,0x48,field))return false;
        for(int n=0;field&&n<512;++n){
            uintptr_t type=0,next=0,structure=0;int at=0,size=0,slotAt=0;Name value{};
            if(!Read(field,0x10,type)||!Read(field,0x28,next)||next==field)return false;
            if(Is(type,"StructProperty")){
                if(!Read(field,0x70,structure))return false;
                if(Is(structure,"AnimNode_Slot")){
                    if(slots>=5||!Read(field,0x44,at)||at<0||at>65536||!Read(field,0x34,size)||size<=0||size>1024||
                       !native_objects::PropertyField(g_base,structure,"SlotName","NameProperty",8,slotAt)||slotAt>size-8||
                       !Read(anim,size_t(at+slotAt),value)||value.index<=0||value.index>=2000000||value.number||value==motionSlot)return false;
                    for(unsigned i=0;i<slots;++i)if(seen[i]==value)return false;
                    seen[slots++]=value; // compare identity, never FName display spelling
                }
            }
            field=next;
        }
        if(field||!Read(cls,0x40,cls))return false;
    }
    return !cls&&slots==5;
}
bool Source(const local_players::Context& c,foreign_montage::Source& out,const char** rejected=nullptr){
    auto fail=[&](const char* why){if(rejected)*rejected=why;return false;};
    uintptr_t mesh=0,owner=0,anim=0,cls=0,skeleton=0,asset=0;Name root{},slot{};unsigned char mode=0;
    struct Array {uintptr_t data;int count,capacity;} bones{};
    if(!Alive(g_idle))return fail("idle asset");
    if(!Read(c.character,0x380,mesh)||!Read(mesh,0xE8,owner)||owner!=c.character||
       !Read(mesh,0x718,anim)||!Read(anim,0x10,cls)||!Is(cls,"StrifeAnim_Blueprint_C"))return fail("own animation instance");
    // Exact engine: mode 3 extracts the root-motion montage without slot
    // weight. Mode 2 would blend it by a slot that this graph never evaluates.
    if(!Read(anim,0x30,mode)||mode!=3)return fail("RootMotionMode must be 3");
    if(!Read(g_base,0x3A32340,slot)||!native_objects::NameValueIs(g_base,slot.index,slot.number,"DefaultSlot")||
       !UnusedSlot(anim,cls,slot))return fail("motion slot rendered, duplicated or graph contract unavailable");
    if(!Read(anim,0x28,skeleton)||!Read(g_idle.address,0x38,out.skeleton)||skeleton!=out.skeleton||!Is(skeleton,"Strife_Skeleton"))return fail("own skeleton");
    if(!Read(mesh,0x5A0,asset)||!Read(asset,0x190,bones)||!bones.data||bones.count<=0||bones.count>bones.capacity||bones.capacity>512||
       !Read(bones.data,0,root)||!native_objects::NameValueIs(g_base,root.index,root.number,"Bone_Strife_Root"))return fail("own root bone");
    // Spine is used only by the opening native charge blast, never a made-up
    // offset. A missing spine leaves presentation adaptation unavailable.
    Name spine{};
    if(bones.count>13&&Read(bones.data,13*12,spine)&&native_objects::NameValueIs(g_base,spine.index,spine.number,"Bone_Strife_Spine1")){
        out.spineIndex=spine.index;out.spineNumber=spine.number;
    }
    out.sequence=g_idle.address;out.rootIndex=root.index;out.rootNumber=root.number;out.slotIndex=slot.index;out.slotNumber=slot.number;return true;
}
void ClearVisual(int index){
    auto* target=PlayerVisual(index);if(!target)return;
    const unsigned bit=target==&g_visual[0]?1u:2u;
    AcquireSRWLockExclusive(&g_visualLock);*target={};g_visualMask.fetch_and(~bit,std::memory_order_relaxed);ReleaseSRWLockExclusive(&g_visualLock);
}
void BeginVisual(const local_players::Context& c,void* ability,void* montage,int phase,const foreign_montage::Source& source){
    auto* target=PlayerVisual(c.index);if(!target)return;
    Visual v{};v.source=source;uintptr_t mesh=0,anim=0;
    if(!Read(c.character,0x380,mesh)||!Read(mesh,0x718,anim)||!native_objects::CaptureLive(g_base,ability,v.ability)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(c.character),v.actor)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(mesh),v.mesh)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(anim),v.anim)||
       !native_objects::CaptureLive(g_base,reinterpret_cast<void*>(source.skeleton),v.skeleton)||!native_objects::CaptureLive(g_base,montage,v.montage)||
       !Read(reinterpret_cast<uintptr_t>(montage),0x90,v.length))return;
    v.sequence=g_poses[phase?1:0];v.tick=GetTickCount64();
    AcquireSRWLockExclusive(&g_visualLock);*target=v;g_visualMask.fetch_or(target==&g_visual[0]?1u:2u,std::memory_order_relaxed);ReleaseSRWLockExclusive(&g_visualLock);
}
void MotionNotify(Notify native,void* notify,void* mesh,void* animation){
    if(!native)return;
    if(g_ready&&!g_failed&&native_tps::OnGameThread()){
        const auto m=reinterpret_cast<uintptr_t>(mesh),a=reinterpret_cast<uintptr_t>(animation);uintptr_t actor=0,owner=0,package=0;local_players::Context c{};
        if(Read(m,0xE8,actor)&&Owner(reinterpret_cast<void*>(actor),c,false)&&
           (Is(a,"War_Atk_Rampage_Start")||Is(a,"War_Atk_Rampage_Run"))&&Read(a,0x20,package)){
            char path[200]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_War/Animation/%s",Is(a,"War_Atk_Rampage_Start")?"War_Atk_Rampage_Start":"War_Atk_Rampage_Run");
            const auto* target=PlayerVisual(c.index);if(!target){native(notify,mesh,animation);return;}
            const auto& v=*target;unsigned char state=0;
            if(Named(package,path)&&Alive(v.ability)&&Alive(v.montage)&&v.actor.address==actor&&v.mesh.address==m&&Read(v.ability.address,0x48,owner)&&owner==actor&&
               Read(v.ability.address,0xD8,state)&&(state==1||state==2)){
                const auto adapted=foreign_montage::RampageNotify(reinterpret_cast<uintptr_t>(notify),v.montage.address,v.source);
                if(adapted){native(reinterpret_cast<void*>(adapted),mesh,animation);return;}
            }
        }
    }
    native(notify,mesh,animation);
}
void Blast(void* n,void* m,void* a){MotionNotify(g_blast,n,m,a);}
void Surface(void* n,void* m,void* a){MotionNotify(g_surface,n,m,a);}
bool Recent(const local_players::Context& c){const auto* mark=PlayerCast(c.index);return mark&&mark->world==c.world&&mark->actor.address==c.character&&Alive(mark->actor);}
void Stop(void* ability,int phase,const local_players::Context& c,const char* why="motion source unavailable"){
    auto* mark=PlayerCast(c.index);if(!mark)return;
    g_failed=true;*mark={};
    // These are native Blueprint abort branches. A normal start finish would
    // otherwise launch another War phase after a failed private montage.
    g_finish(ability,phase==0?28:2);
    if(g_log&&g_events++<12){char line[256]{};sprintf_s(line,"Rampage V1.107 stopped phase=%d: %s; native abort, transfer disabled for session; select Native",phase,why);g_log(line);}
}
bool Companion(uintptr_t cls){
    uintptr_t package=0;return Is(cls,"MayhemAbility_War_LaunchAttack_Blueprint_C")&&Read(cls,0x20,package)&&
        Is(package,"/Game/Blueprints/Abilities/WarMelee/MayhemAbility_War_LaunchAttack_Blueprint");
}
}
void Initialize(foreign_wrath::LogFn log,bool exact){
    g_log=log;if(!exact)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char finish[]={0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE,0xFF,0xFF,0xFF,0x48};
    constexpr unsigned char playing[]={0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x20,0x48,0x8B,0xD9,0x48,0x83,0xC1};
    if(std::memcmp(reinterpret_cast<void*>(g_base+0x59B080),finish,sizeof(finish))||std::memcmp(reinterpret_cast<void*>(g_base+0x676620),playing,sizeof(playing)))return;
    g_finish=reinterpret_cast<Finish>(g_base+0x59B080);g_playing=reinterpret_cast<Playing>(g_base+0x676620);g_ready=true;
    constexpr unsigned char position[]={0x48,0x83,0xEC,0x38,0x0F,0x29,0x74,0x24,0x20,0x0F,0x57,0xF6,0x48,0x85,0xD2,0x74};
    constexpr unsigned char sample[]={0x48,0x89,0x74,0x24,0x18,0x48,0x89,0x7C,0x24,0x20,0x55,0x41,0x54,0x41,0x56,0x48};
    if(!std::memcmp(reinterpret_cast<void*>(g_base+0x16778E0),position,sizeof(position))&&!std::memcmp(reinterpret_cast<void*>(g_base+0x1690BD0),sample,sizeof(sample))){
        g_position=reinterpret_cast<Position>(g_base+0x16778E0);g_sample=reinterpret_cast<Sample>(g_base+0x1690BD0);
    }
    constexpr unsigned char blast[]={0x48,0x89,0x74,0x24,0x18,0x48,0x89,0x7C,0x24,0x20,0x41,0x56,0x48,0x83,0xEC,0x50};
    constexpr unsigned char surface[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24};
    auto* b=reinterpret_cast<void*>(g_base+0x5D7DF0);auto* s=reinterpret_cast<void*>(g_base+0x5D7760);
    if(!std::memcmp(b,blast,sizeof(blast))&&!std::memcmp(s,surface,sizeof(surface))){
        const bool cb=MH_CreateHook(b,reinterpret_cast<void*>(&Blast),reinterpret_cast<void**>(&g_blast))==MH_OK;
        const bool cs=cb&&MH_CreateHook(s,reinterpret_cast<void*>(&Surface),reinterpret_cast<void**>(&g_surface))==MH_OK;
        if(!cs||MH_EnableHook(b)!=MH_OK||MH_EnableHook(s)!=MH_OK){
            if(cb){MH_DisableHook(b);MH_RemoveHook(b);}if(cs){MH_DisableHook(s);MH_RemoveHook(s);}g_blast=g_surface=nullptr;
        }
    }
    if(log){char line[220]{};sprintf_s(line,"Rampage V1.108 READY: native motion/phase token; own Dash/Devil Heel pose=%u; scoped native charge blast/surface=%u; EXPERIMENTAL",unsigned(g_position&&g_sample),unsigned(g_blast&&g_surface));log(line);}
}
bool AssetsReady(){for(const auto& id:g_phases)if(!Alive(id))return false;return Alive(g_idle)&&Alive(g_poses[0])&&Alive(g_poses[1]);}
bool InvalidateAssets(){
    if(!g_ready||!native_tps::OnGameThread())return false;bool changed=false;
    for(auto& id:g_phases)if(id&&!Alive(id)){id={};changed=true;}
    if(g_idle&&!Alive(g_idle)){g_idle={};changed=true;}
    for(auto& id:g_poses)if(id&&!Alive(id)){id={};changed=true;}return changed;
}
void ObserveObject(uintptr_t p){
    if(!g_ready||!native_tps::OnGameThread())return;
    for(int i=0;i<5;++i)if(!Alive(g_phases[i])&&Class(p,i))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_phases[i]);
    uintptr_t cls=0,package=0;
    if(!Alive(g_idle)&&Read(p,0x10,cls)&&Is(cls,"AnimSequence")&&Is(p,"Strife_Idle")&&Read(p,0x20,package)&&
       Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Idle"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_idle);
    if(Read(p,0x10,cls)&&Is(cls,"AnimSequence"))for(int i=0;i<2;++i){
        const char* name=i?"Strife_Air_Atk_Melee_Kick":"Strife_Dash";char path[200]{};sprintf_s(path,"/Game/Characters/Heroes/Hero_Strife/Animations/%s",name);
        if(!Alive(g_poses[i])&&Named(p,name)&&Read(p,0x20,package)&&Named(package,path))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_poses[i]);
    }
}
int Status(){return !g_ready||g_failed||!foreign_montage::Ready()?0:(AssetsReady()?2:1);}
bool IsClass(uintptr_t cls){return g_ready&&native_tps::OnGameThread()&&Kind(cls)>=0;}
bool Prepare(void* actor,uintptr_t& abilityClass){
    if(!native_tps::OnGameThread())return false;
    local_players::Context c{};foreign_montage::Source source{};const char* rejected=nullptr;
    if(Status()!=2)rejected="required assets not ready or session disabled";
    else if(!Owner(actor,c,true))rejected="local Strife ownership or grounded state";
    else Source(c,source,&rejected);
    if(rejected){
        if(g_log&&g_refusal!=rejected&&g_refusals<12){++g_refusals;char line[200]{};sprintf_s(line,"Rampage V1.107 PREPARE refused: %s",rejected);g_log(line);}
        g_refusal=rejected;return false;
    }
    g_refusal=nullptr;abilityClass=g_phases[0].address;return true;
}
bool HandlePlay(PlayFn native,void* ability,void* montage,float rate,Name section){
    if(!g_ready||!native||!native_tps::OnGameThread())return false;
    const auto p=reinterpret_cast<uintptr_t>(ability);uintptr_t cls=0,actor=0,character=0;uint32_t flags=0;unsigned char state=0;local_players::Context c{};
    if(!Read(p,0x10,cls)||!Read(p,8,flags)||(flags&0x30u)||!Read(p,0x48,actor)||!Read(p,0x1B0,character)||character!=actor||
       !Read(p,0xD8,state)||(state!=1&&state!=2)||!Owner(reinterpret_cast<void*>(actor),c,false))return false;
    auto* mark=PlayerCast(c.index);if(!mark)return false;
    if(Companion(cls)&&Recent(c)){
        if(mark->phase>0)*mark={}; // an early companion cannot steal Start's pending handoff
        g_finish(ability,0);return true;
    }
    const int phase=Kind(cls);if(phase<0)return false;
    if(phase&&(!Recent(c)||mark->phase!=0)){Stop(ability,phase,c,"finish has no pending own Start");return true;}
    if(g_handling==ability)return true;
    struct Guard{void* old;~Guard(){g_handling=old;}} guard{g_handling};g_handling=ability;
    native_objects::Identity abi{},pawn{};foreign_montage::Source source{};
    if(!native_objects::CaptureLive(g_base,ability,abi)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),pawn)){Stop(ability,phase,c,"live identity capture failed");return true;}
    if(g_failed||!std::isfinite(rate)||rate<=0||rate>10||section.index||section.number||!Source(c,source)){Stop(ability,phase,c);return true;}
    auto* replacement=foreign_montage::RampageMotion(reinterpret_cast<uintptr_t>(montage),source);
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    if(!replacement){Stop(ability,phase,c,"private motion montage construction/shape failed");return true;}
    *mark={pawn,c.world,phase};
    BeginVisual(c,ability,replacement,phase,source); // native first-frame notifies can now resolve their own actor
    native(ability,replacement,rate,section);
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    uintptr_t instance=0;
    if(!Read(p,0x1D8,instance)||!instance||!g_playing(reinterpret_cast<void*>(instance))){Stop(ability,phase,c,"native montage playback not active");return true;}
    foreign_montage::StartSounds(reinterpret_cast<uintptr_t>(montage),ability);
    if(g_log&&g_events++<12){char line[180]{};sprintf_s(line,"Rampage V1.108 CAST player=%d phase=%d: native motion/loop/collision/finish; own Dash/Devil Heel presentation; EXPERIMENTAL",c.index+1,phase);g_log(line);}
    return true;
}
void Observe(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto* mark=PlayerCast(c.index);if(!mark)return;
    // Start is already finished when native code creates Land/Slam; the
    // final companion can likewise follow its parent's ordinary completion.
    // No old ability is retained/read/called to authorize that handoff.
    if(mark->actor&&(mark->world!=c.world||mark->actor.address!=c.character||c.pawn!=c.character||
       c.kind!=local_players::PoseKind::Foot||!Alive(mark->actor)))*mark={};
    auto* target=PlayerVisual(c.index);if(!target)return;
    Visual next{};AcquireSRWLockShared(&g_visualLock);next=*target;ReleaseSRWLockShared(&g_visualLock);
    unsigned char state=0;uintptr_t actor=0,mesh=0,anim=0;
    if(!g_failed&&mark->actor&&Alive(next.ability)&&Alive(next.actor)&&Alive(next.anim)&&Alive(next.montage)&&
       next.actor.address==c.character&&Read(next.ability.address,0x48,actor)&&actor==c.character&&Read(actor,0x380,mesh)&&mesh==next.mesh.address&&
       Read(mesh,0x718,anim)&&anim==next.anim.address&&Read(next.ability.address,0xD8,state)&&(state==1||state==2)&&g_position){
        next.position=g_position(reinterpret_cast<void*>(anim),reinterpret_cast<void*>(next.montage.address));next.tick=GetTickCount64();
        AcquireSRWLockExclusive(&g_visualLock);*target=next;ReleaseSRWLockExclusive(&g_visualLock);
    }else ClearVisual(c.index);
}
bool Evaluate(void* node,pistol_visual::PoseContext* output){
    if(!g_visualMask.load(std::memory_order_relaxed)||!g_ready||g_failed||!g_sample||!output)return false;Visual v{};const auto now=GetTickCount64();
    AcquireSRWLockShared(&g_visualLock);for(const auto& p:g_visual)if(p.anim&&reinterpret_cast<uintptr_t>(node)==p.anim.address+0x318){v=p;break;}ReleaseSRWLockShared(&g_visualLock);
    unsigned char state=0;
    if(!v.anim||now<v.tick||now-v.tick>100||!Alive(v.ability)||!Alive(v.montage)||!Alive(v.actor)||!Alive(v.mesh)||!Alive(v.anim)||!Alive(v.sequence)||!Alive(v.skeleton)||
       !Read(v.ability.address,0xD8,state)||(state!=1&&state!=2)||
       !std::isfinite(v.position)||v.position<0||!std::isfinite(v.length)||v.length<=0||v.position>v.length+.001f)return false;
    struct Array {uintptr_t data=0;int count=0,capacity=0;};
    uintptr_t proxy=0,actual=0;uint64_t vt=0;float length=0;Array required{};pistol_visual::PoseContext context{};
    if(!Read(reinterpret_cast<uintptr_t>(node),0,vt)||vt!=g_base+0x2A77628||!Read(reinterpret_cast<uintptr_t>(output),0,context)||context.additive||
       !Read(v.anim.address,0x240,proxy)||context.proxy!=proxy||!Read(proxy,0xA0,actual)||actual!=v.anim.address||
       !Read(proxy,0xB8,actual)||actual!=v.mesh.address||!Read(proxy,0xB0,actual)||actual!=v.skeleton.address||
       context.pose.container!=proxy+0x450||context.pose.transforms.count!=80||context.pose.transforms.capacity<80||context.pose.transforms.capacity>1024||
       !context.pose.transforms.data||(context.pose.transforms.data&15u)||!Read(proxy,0x450,required)||!required.data||required.count!=80||required.capacity<80||required.capacity>1024||
       !Read(v.sequence.address,0x38,actual)||actual!=v.skeleton.address||!Read(v.sequence.address,0x90,length)||!std::isfinite(length)||length<=0||length>5)return false;
    std::array<uint16_t,80> indices{};if(!Read(required.data,0,indices))return false;for(unsigned i=0;i<80;++i)if(indices[i]!=i)return false;
    // Pure pose extraction in the native evaluation scope: no second motion,
    // notify, cost, damage, input, animation update or timer. Keep the current
    // root transform; War's original montage alone controls displacement.
    pistol_visual::LocalTransform root{};if(!Read(context.pose.transforms.data,0,root))return false;
    const Extract extract{true,{},std::min(length,v.position*length/v.length)};
    g_sample(reinterpret_cast<void*>(v.sequence.address),&output->pose,&output->curve,&extract);
    std::memcpy(reinterpret_cast<void*>(context.pose.transforms.data),&root,sizeof(root));return true;
}
void Shutdown(){g_ready=false;for(auto& c:g_cast)c={};ClearVisual(0);ClearVisual(1);}
}
