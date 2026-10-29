#pragma once
#include "LocalPlayerOwnership.h"
#include "NativeMemoryReader.h"
#include "NativeObjectName.h"
#include <array>

namespace dg::pistol_appearance {
inline constexpr std::array<size_t,8> Offsets={0x1040,0x1048,0xBA0,0xBA8,0xBF8,0xC00,0x1010,0x1018};
struct OwnedSlot {local_players::Context context{};native_objects::Identity actor{},body{},component{};unsigned slot=0;};
template<class T>bool Read(uintptr_t p,size_t offset,T& value){native_objects::MemoryReader r;return r.Read(p,offset,value);}
inline bool Controlled(uintptr_t actor){uintptr_t controller=0,pawn=0;return Read(actor,0x348,controller)&&controller&&Read(controller,0x350,pawn)&&pawn==actor;}
inline bool Component(uintptr_t base,uintptr_t actor,uintptr_t body,unsigned slot,uintptr_t& component,unsigned char& flags){
    if(slot>=Offsets.size())return false;uintptr_t vt=0,owner=0,parent=0;
    return Read(actor,Offsets[slot],component)&&component&&Read(component,0,vt)&&vt==base+(slot>=6?0x2724250:0x2A575C8)&&
        Read(component,0xE8,owner)&&owner==actor&&Read(component,0x108,parent)&&parent==body&&Read(component,0x1CC,flags)&&
        (slot<6||(slot==6?native_objects::NameIs(base,component,"Dagger_R"):native_objects::NameIs(base,component,"Dagger_L")));
}
// Resolve from a LIVE native setter argument. No foreground/menu/pause TTL:
// this is an appearance choice, not an animation or input permission.
inline bool Resolve(uintptr_t base,uintptr_t component,OwnedSlot& out){
    uintptr_t vt=0,actor=0,cls=0,controller=0,pawn=0,body=0,bodyOwner=0;unsigned char role=0,type=0;uint32_t flags=0;
    if(!Read(component,8,flags)||(flags&0x18030u)||!Read(component,0,vt)||(vt!=base+0x2A575C8&&vt!=base+0x2724250)||!Read(component,0xE8,actor)||!actor||
       !Read(actor,8,flags)||(flags&0x18030u)||!Read(actor,0x10,cls)||
       (!native_objects::NameIs(base,cls,"Strife_Blueprint_C")&&!native_objects::NameIs(base,cls,"Despair_Blueprint_C")))return false;
    local_players::Context c{};
    if(!local_players::ResolveActor(reinterpret_cast<void*>(actor),c)||c.index<0||c.index>1||!c.localPlayer||actor!=c.pawn||
       !Read(actor,0x118,role)||role!=3||!Read(actor,0x348,controller)||controller!=c.controller||!Read(controller,0x350,pawn)||pawn!=actor||
       !Read(c.character,0xC18,type)||type!=1||!Read(c.character,0x10,cls)||!native_objects::NameIs(base,cls,"Strife_Blueprint_C")||
       !Read(actor,0x380,body)||!body||!Read(body,8,flags)||(flags&0x18030u)||!Read(body,0xE8,bodyOwner)||bodyOwner!=actor)return false;
    const bool foot=c.kind==local_players::PoseKind::Foot&&c.character==c.pawn;
    const bool horse=c.kind==local_players::PoseKind::Horse&&c.character!=c.pawn&&Read(actor,0x10,cls)&&native_objects::NameIs(base,cls,"Despair_Blueprint_C");
    if(!foot&&!horse)return false;
    for(unsigned slot=0;slot<Offsets.size();++slot){
        if(foot?(slot>=2&&slot<6):(slot<2||slot>=6))continue;
        uintptr_t current=0;unsigned char componentFlags=0;if(!Component(base,actor,body,slot,current,componentFlags)||current!=component)continue;
        OwnedSlot result{};result.context=c;result.slot=slot;
        if(!native_objects::CaptureLive(base,reinterpret_cast<void*>(actor),result.actor)||
           !native_objects::CaptureLive(base,reinterpret_cast<void*>(body),result.body)||
           !native_objects::CaptureLive(base,reinterpret_cast<void*>(component),result.component))return false;
        out=result;return true;
    }
    return false;
}
}
