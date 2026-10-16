#pragma once
#include "LocalPlayerOwnership.h"
#include <cstddef>

namespace dg::local_players::layout {
constexpr uintptr_t PlayerClass=0x380FE78, HorseClass=0x380D9B8,
    FormClass=0x380AC80, StateClass=0x38115D8, ControllerClass=0x3810B60,
    LocalPlayerClass=0x3A61320, LevelClass=0x3A60E78,
    WorldClass=0x3A67FF8, GameInstanceClass=0x3A5D298,
    PlayerCameraClass=0x380F720, CameraClass=0x3A5AEB0,
    CameraActorClass=0x3A5AD38, ManagerClass=0x380F730;

// This is the actual resolver, also exercised with a bounded synthetic UE
// object graph in the tests. No name suffix, distance or callback order is
// used to decide ownership. All offsets are from the exact retail EXE.
template<class Reader>
bool Resolve(Reader& r,uintptr_t actor,Context& out) {
    if(!r.Instance(actor,PlayerClass)&&!r.Instance(actor,HorseClass)&&
       !r.Instance(actor,FormClass))return false;
    uintptr_t level=0,world=0,instance=0,array=0;
    int count=0,capacity=0;
    if(!r.Read(actor,0x20,level)||!r.Instance(level,LevelClass)||
       !r.Read(level,0x20,world)||!r.Instance(world,WorldClass)||
       !r.Read(world,0x160,instance)||!r.Instance(instance,GameInstanceClass)||
       !r.Read(instance,0x38,array)||!array||
       !r.Read(instance,0x40,count)||!r.Read(instance,0x44,capacity)||
       count<1||count>2||capacity<count||capacity>16)return false;
    uintptr_t locals[2]{};
    for(int i=0;i<count;++i){
        if(!r.Read(array,size_t(i)*sizeof(uintptr_t),locals[i])||
           !r.Instance(locals[i],LocalPlayerClass))return false;
        if(i&&locals[i]==locals[0])return false;
    }
    for(int index=0;index<count;++index){
        const uintptr_t local=locals[index];
        uintptr_t state=0,normal=0,controller=0,other=0,pawn=0;
        if(!r.Read(local,0x30,controller)||!r.Instance(controller,ControllerClass)||
           !r.Read(controller,0x398,other)||other!=local||
           !r.Read(controller,0x318,state)||!r.Instance(state,StateClass)||
           !r.Read(state,0x108,other)||other!=controller||
           !r.Read(state,0x5C0,normal)||!r.Instance(normal,PlayerClass)||
           !r.Read(normal,0x330,other)||(other&&other!=state)||
           !r.Read(controller,0x350,pawn)||!pawn||
           !r.Read(pawn,0x348,other)||other!=controller)continue;
        PoseKind kind=PoseKind::Foot;
        if(pawn!=normal){
            if(r.Instance(pawn,HorseClass))kind=PoseKind::Horse;
            else if(r.Instance(pawn,FormClass))kind=PoseKind::Ultimate;
            else continue;
        }
        // Possession may clear normal.PlayerState during horse/ultimate.
        // Ownership comes from the current native local controller/state,
        // rather than borrowing the most recently observed normal pawn.
        uintptr_t form=0,formState=0;
        if(pawn==normal&&r.Read(normal,0xE78,form)&&r.Instance(form,FormClass)&&
           r.Read(form,0x330,formState)&&formState==state){
            pawn=form;kind=PoseKind::Ultimate;
        }
        if(actor!=normal&&actor!=pawn)continue;
        int id=-1;
        if(!r.Read(local,0xE0,id)||id<0||id>3)id=-1;
        out={index,id,count,local,controller,normal,pawn,world,kind,state};
        return true;
    }
    return false;
}
template<class Reader>
bool Camera(Reader& r,uintptr_t camera,Context& out){
    uintptr_t owner=0,follow=0,split=0,world=0;
    if(!r.Read(camera,0x20,owner))return false;
    if(r.Instance(camera,PlayerCameraClass)&&r.Instance(owner,PlayerClass)&&Resolve(r,owner,out)&&
       r.Read(owner,0xBA0,follow)&&r.Read(owner,0xBA8,split)&&(camera==follow||camera==split)){
        return r.Read(camera,0xF0,world)&&world==out.world;
    }
    // A possessed mount or the player's reciprocal active form may own its
    // own native player-camera component. It does not use Player.BA0/BA8.
    if(r.Instance(camera,PlayerCameraClass)&&
       (r.Instance(owner,HorseClass)||r.Instance(owner,FormClass))&&
       Resolve(r,owner,out)&&owner==out.pawn)
        return r.Read(camera,0xF0,world)&&world==out.world;
    // UE's fallback CameraActor is also explicitly owned by a player camera
    // manager. This is an observed reciprocal chain, not a proximity fallback.
    uintptr_t manager=0,controller=0,state=0,normal=0,reciprocal=0;
    return r.Instance(camera,CameraClass)&&r.Instance(owner,CameraActorClass)&&
        r.Read(owner,0x108,manager)&&r.Instance(manager,ManagerClass)&&
        r.Read(manager,0x310,controller)&&r.Instance(controller,ControllerClass)&&
        r.Read(controller,0x3B8,reciprocal)&&reciprocal==manager&&
        r.Read(controller,0x318,state)&&r.Instance(state,StateClass)&&
        r.Read(state,0x5C0,normal)&&Resolve(r,normal,out)&&out.controller==controller&&
        r.Read(camera,0xF0,world)&&world==out.world;
}
template<class Reader>
NativeView View(Reader& r,uintptr_t camera,unsigned index,int* localCount=nullptr){
    uintptr_t world=0,instance=0,array=0,local=0,pc=0,manager=0,reciprocal=0;
    int count=0,capacity=0;
    if(index>1||!r.Instance(camera,CameraClass)||
       !r.Read(camera,0xF0,world)||!r.Instance(world,WorldClass)||
       !r.Read(world,0x160,instance)||!r.Instance(instance,GameInstanceClass)||
       !r.Read(instance,0x38,array)||!array||!r.Read(instance,0x40,count)||
       !r.Read(instance,0x44,capacity)||count<1||count>2||index>=unsigned(count)||
       capacity<count||capacity>16||!r.Read(array,size_t(index)*8,local)||
       !r.Instance(local,LocalPlayerClass)||!r.Read(local,0x30,pc)||
       !r.Instance(pc,ControllerClass)||!r.Read(pc,0x398,reciprocal)||reciprocal!=local||
       !r.Read(pc,0x3B8,manager)||!r.Instance(manager,ManagerClass)||
       !r.Read(manager,0x310,reciprocal)||reciprocal!=pc)return NativeView::Unknown;
    if(localCount)*localCount=count;
    uintptr_t state=0,normal=0,pawn=0,current=0,pending=0;
    if(!r.Read(pc,0x318,state)||!r.Instance(state,StateClass)||
       !r.Read(state,0x108,reciprocal)||reciprocal!=pc||
       !r.Read(state,0x5C0,normal)||!r.Instance(normal,PlayerClass)||
       !r.Read(pc,0x350,pawn)||!r.Read(manager,0xEA0,current)||
       !r.Read(manager,0x1440,pending))return NativeView::Unknown;
    const auto classify=[&](uintptr_t target){
        if(!target)return NativeView::Unknown;
        if(target==normal||target==pawn||target==pc)return NativeView::Gameplay;
        uintptr_t owner=0;
        if(!r.Instance(target,CameraActorClass))return NativeView::Unknown;
        // The manager's own UE fallback camera is gameplay. A separate native
        // CameraActor selected as view target is the actual camera takeover.
        if(!r.Read(target,0x108,owner))return NativeView::Unknown;
        return owner==manager?NativeView::Gameplay:NativeView::ExternalCamera;
    };
    const auto active=classify(current),next=classify(pending);
    if(active==NativeView::ExternalCamera||next==NativeView::ExternalCamera)
        return NativeView::ExternalCamera;
    return active==NativeView::Gameplay&&(!pending||next==NativeView::Gameplay)?
        NativeView::Gameplay:NativeView::Unknown;
}
}
