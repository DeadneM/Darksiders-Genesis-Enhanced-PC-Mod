#include "ForeignRampageFeature.h"
#include "ForeignMontage.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
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
struct Cast {native_objects::Identity actor{},ability{};uintptr_t world=0;};Cast g_cast[2]{};
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
    out.sequence=g_idle.address;out.rootIndex=root.index;out.rootNumber=root.number;out.slotIndex=slot.index;out.slotNumber=slot.number;return true;
}
bool Recent(const local_players::Context& c){const auto* mark=PlayerCast(c.index);return mark&&mark->world==c.world&&mark->actor.address==c.character&&Alive(mark->actor);}
void Stop(void* ability,int phase,const local_players::Context& c,const char* why="motion source unavailable"){
    auto* mark=PlayerCast(c.index);if(!mark)return;
    g_failed=true;*mark={};
    // These are native Blueprint abort branches. A normal start finish would
    // otherwise launch another War phase after a failed private montage.
    g_finish(ability,phase==0?28:2);
    if(g_log&&g_events++<12){char line[256]{};sprintf_s(line,"Rampage V1.104 stopped phase=%d: %s; native abort, transfer disabled for session; select Native",phase,why);g_log(line);}
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
    if(log)log("Rampage V1.104 READY: private native motion and lifecycle; unused live-graph slot required; Strife locomotion pose; EXPERIMENTAL");
}
bool AssetsReady(){for(const auto& id:g_phases)if(!Alive(id))return false;return Alive(g_idle);}
bool InvalidateAssets(){
    if(!g_ready||!native_tps::OnGameThread())return false;bool changed=false;
    for(auto& id:g_phases)if(id&&!Alive(id)){id={};changed=true;}
    if(g_idle&&!Alive(g_idle)){g_idle={};changed=true;}return changed;
}
void ObserveObject(uintptr_t p){
    if(!g_ready||!native_tps::OnGameThread())return;
    for(int i=0;i<5;++i)if(!Alive(g_phases[i])&&Class(p,i))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_phases[i]);
    uintptr_t cls=0,package=0;
    if(!Alive(g_idle)&&Read(p,0x10,cls)&&Is(cls,"AnimSequence")&&Is(p,"Strife_Idle")&&Read(p,0x20,package)&&
       Is(package,"/Game/Characters/Heroes/Hero_Strife/Animations/Strife_Idle"))native_objects::CaptureLive(g_base,reinterpret_cast<void*>(p),g_idle);
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
        if(g_log&&g_refusal!=rejected&&g_refusals<12){++g_refusals;char line[200]{};sprintf_s(line,"Rampage V1.104 PREPARE refused: %s",rejected);g_log(line);}
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
    if(Companion(cls)&&Recent(c)){*mark={};g_finish(ability,0);return true;}
    const int phase=Kind(cls);if(phase<0)return false;
    if(phase&&!Recent(c)){Stop(ability,phase,c,"orphan finish phase");return true;}
    if(g_handling==ability)return true;
    struct Guard{void* old;~Guard(){g_handling=old;}} guard{g_handling};g_handling=ability;
    native_objects::Identity abi{},pawn{};foreign_montage::Source source{};
    if(!native_objects::CaptureLive(g_base,ability,abi)||!native_objects::CaptureLive(g_base,reinterpret_cast<void*>(actor),pawn)){Stop(ability,phase,c,"live identity capture failed");return true;}
    if(g_failed||!std::isfinite(rate)||rate<=0||rate>10||section.index||section.number||!Source(c,source)){Stop(ability,phase,c);return true;}
    auto* replacement=foreign_montage::RampageMotion(reinterpret_cast<uintptr_t>(montage),source);
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    if(!replacement){Stop(ability,phase,c,"private motion montage construction/shape failed");return true;}
    *mark={pawn,abi,c.world};
    native(ability,replacement,rate,section);
    if(!Alive(abi)||!Alive(pawn)||!Read(p,0x48,character)||character!=actor)return true;
    uintptr_t instance=0;
    if(!Read(p,0x1D8,instance)||!instance||!g_playing(reinterpret_cast<void*>(instance))){Stop(ability,phase,c,"native montage playback not active");return true;}
    if(g_log&&g_events++<12){char line[180]{};sprintf_s(line,"Rampage V1.104 CAST player=%d phase=%d: native motion/loop/collision/finish; own locomotion pose; EXPERIMENTAL",c.index+1,phase);g_log(line);}
    return true;
}
void Observe(const local_players::Context& c){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return;
    auto* mark=PlayerCast(c.index);if(!mark)return;unsigned char state=0;
    if(mark->actor&&(mark->world!=c.world||mark->actor.address!=c.character||!Alive(mark->actor)||!Alive(mark->ability)||
       !Read(mark->ability.address,0xD8,state)||(state!=1&&state!=2)))*mark={};
}
void Shutdown(){g_ready=false;for(auto& c:g_cast)c={};}
}
