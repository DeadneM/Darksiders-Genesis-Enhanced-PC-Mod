#include "ExecutionCameraFeature.h"
#include "ExecutionCameraPolicy.h"
#include "NativeBonePoint.h"
#include "NativeTpsFeature.h"
#include <atomic>
#include <cstdio>

namespace dg::execution_camera {
namespace {
uintptr_t g_base=0;native_bone::LocationFn g_location=nullptr;LogFn g_log=nullptr;
std::atomic_bool g_ready{false};Follow g_players[2]{};unsigned g_events=0;
template<class T> bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
bool Finisher(uintptr_t ability,uintptr_t actor){
    uintptr_t instigator=0,character=0,cls=0;
    if(!Read(ability,0x48,instigator)||instigator!=actor||!Read(ability,0x1B0,character)||character!=actor||!Read(ability,0x10,cls))return false;
    for(unsigned n=0;cls&&n<12;++n){
        uintptr_t package=0;
        if(native_objects::NameIs(g_base,cls,"MayhemPlayerFinisherAbility")&&Read(cls,0x20,package)&&
           native_objects::NameIs(g_base,package,"/Script/DarksidersGenesis"))return true;
        if(!Read(cls,0x40,cls))break;
    }
    return false;
}
bool Executing(uintptr_t actor,bool& active){
    uintptr_t component=0,owner=0;native_bone::Array abilities{};
    if(!Read(actor,0xC70,component)||!Read(component,0xE8,owner)||owner!=actor||!Read(component,0xF8,abilities)||
       abilities.count<0||abilities.count>abilities.capacity||abilities.capacity>64||(!abilities.data&&abilities.count))return false;
    active=false;
    for(int i=0;i<abilities.count;++i){uintptr_t ability=0;
        if(!Read(abilities.data,size_t(i)*8,ability)||!ability)return false;
        if(Finisher(ability,actor))active=true;
    }
    return true;
}
}
void Initialize(LogFn log,bool exactExecutable){
    g_log=log;if(!exactExecutable)return;g_base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if(!native_bone::Boundary(g_base))return;
    g_location=reinterpret_cast<native_bone::LocationFn>(g_base+0x17A2380);g_ready=true;
    if(log)log("Execution camera V1.82 READY: local native finisher type, animated pelvis rise only; no new camera/input hook; EXPERIMENTAL");
}
float Offset(const local_players::Context& c,float rootZ){
    if(!g_ready||!native_tps::OnGameThread()||c.index<0||c.index>1)return 0;
    auto& state=g_players[c.index];uintptr_t cls=0,controller=0,pawn=0,mesh=0;unsigned char type=0,role=0;
    if(c.kind!=local_players::PoseKind::Foot||!c.localPlayer||c.pawn!=c.character||!c.world||
       !Read(c.pawn,0x118,role)||role!=3||!Read(c.pawn,0xC18,type)||(type!=1&&type!=2)||
       !Read(c.pawn,0x348,controller)||controller!=c.controller||!Read(controller,0x350,pawn)||pawn!=c.pawn||
       !Read(c.pawn,0x10,cls)||!(type==1?native_objects::NameIs(g_base,cls,"Strife_Blueprint_C"):native_objects::NameIs(g_base,cls,"War_Blueprint_C"))||
       !Read(c.pawn,0x380,mesh)){state={};return 0;}
    const auto slot=local_players::PublishedSlot(unsigned(c.index));
    if(slot.localPlayer!=c.localPlayer||slot.character!=c.character||!slot.generation){state={};return 0;}
    native_objects::MemoryReader reader;native_bone::Point pelvis{};bool executing=false;
    const bool bone=type==1?native_bone::Location(reader,g_base,g_location,mesh,c.pawn,1,"Bone_Strife_Pelvis",pelvis):
        native_bone::Location(reader,g_base,g_location,mesh,c.pawn,1,"Bone_War_Pelvis",pelvis);
    if(!bone||!Executing(c.pawn,executing)){state={};return 0;}
    const bool before=state.active;
    const float offset=state.Update(c.localPlayer,c.pawn,c.world,mesh,rootZ,pelvis.z,executing,GetTickCount64(),slot.generation);
    if(g_log&&executing&&!before&&++g_events<=12){char line[200]{};sprintf_s(line,
        "Execution camera V1.82: player=%d finisher=1 bodyDelta=%.1f baselineReady=%d; vertical follow EXPERIMENTAL",c.index+1,pelvis.z-rootZ,state.sampled?1:0);g_log(line);}
    return offset;
}
void Shutdown(){g_ready=false;}
}
