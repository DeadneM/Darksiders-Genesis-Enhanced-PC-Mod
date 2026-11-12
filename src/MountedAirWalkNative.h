#pragma once
#include "MountedAirWalkPolicy.h"
#include "MountedAirWalkFloor.h"
#include "LocalPlayerOwnership.h"
#include "NativePropertyField.h"
#include "NativeTpsFeature.h"
namespace dg::mounted_air {
using Id=native_objects::Identity;
using Event=void(*)(void*,void*,void*);
struct Lease {Id horse{},movement{},controller{},capsule{};uintptr_t world=0;Plane plane{};bool active=false;};
struct State {Policy policy{};Lease lease{};};
struct Api {Id cls{},mode{},capsuleClass{},height{},floor{},hit{};};
inline Api api{};
inline bool Alive(uintptr_t base,const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,base,id);}
template<class T>bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
inline bool Is(uintptr_t base,uintptr_t p,const char* text){int i=0,n=0;return Read(p,0x18,i)&&Read(p,0x1C,n)&&native_objects::NameValueIdentifierEquals(base,i,n,text);}
inline uintptr_t Function(uintptr_t base,uintptr_t cls,const char* name,const char* declaring,uintptr_t entry,unsigned size){
    for(unsigned depth=0;cls&&depth<16;++depth){
        uintptr_t field=0;if(!Read(cls,0x48,field))return 0;
        for(unsigned n=0;field&&n<512;++n){
            if(Is(base,field,name)){
                uintptr_t kind=0,outer=0,package=0,native=0;uint32_t flags=0;uint16_t bytes=0;
                if(!Is(base,cls,declaring)||!Read(cls,0x20,package)||!Is(base,package,"/Script/Engine")||
                   !Read(field,0x10,kind)||!Is(base,kind,"Function")||!Read(field,0x20,outer)||outer!=cls||
                   !Read(field,0xC0,native)||native!=base+entry||!Read(field,0x98,flags)||(flags&0x2400u)!=0x400u||
                   !Read(field,0x9E,bytes)||bytes!=size)return 0;
                return field;
            }
            uintptr_t next=0;if(!Read(field,0x28,next)||next==field)return 0;field=next;
        }
        if(field||!Read(cls,0x40,cls))return 0;
    }return 0;
}
inline uintptr_t Property(uintptr_t base,uintptr_t cls,const char* name){
    for(unsigned depth=0;cls&&depth<16;++depth){
        uintptr_t field=0;if(!Read(cls,0x48,field))return 0;
        for(unsigned i=0;field&&i<512;++i){
            if(Is(base,field,name))return field;
            uintptr_t next=0;if(!Read(field,0x28,next)||next==field)return 0;field=next;
        }
        if(field||!Read(cls,0x40,cls))return 0;
    }return 0;
}
inline bool At(uintptr_t base,uintptr_t cls,const char* name,const char* kind,int size,int at){
    int actual=-1;return native_objects::PropertyField(base,cls,name,kind,size,actual)&&actual==at;
}
inline bool Bit(uintptr_t base,uintptr_t cls,const char* name,unsigned char mask){
    const auto field=Property(base,cls,name);uintptr_t kind=0;int at=-1,bytes=0;unsigned char layout[4]{};
    return field&&Read(field,0x10,kind)&&Is(base,kind,"BoolProperty")&&Read(field,0x44,at)&&!at&&Read(field,0x34,bytes)&&
        (bytes==1||bytes==4)&&Read(field,0x70,layout)&&(layout[0]==1||layout[0]==4)&&!layout[1]&&layout[2]==mask&&layout[3]==mask;
}
inline bool Struct(uintptr_t base,uintptr_t cls,const char* name,int bytes,int& at,uintptr_t& out){
    uintptr_t kind=0,package=0;
    const auto field=Property(base,cls,name);
    return field&&native_objects::PropertyField(base,cls,name,"StructProperty",bytes,at)&&Read(field,0x70,out)&&
        Read(out,0x10,kind)&&Is(base,kind,"ScriptStruct")&&Read(out,0x20,package)&&Is(base,package,"/Script/Engine");
}
inline bool FloorSchema(uintptr_t base,uintptr_t movementClass){
    if(Alive(base,api.floor)&&Alive(base,api.hit))return true;
    uintptr_t floor=0,hit=0;int at=-1;
    if(!Struct(base,movementClass,"CurrentFloor",148,at,floor)||!Is(base,floor,"FindFloorResult")||
       !At(base,floor,"FloorDist","FloatProperty",4,4)||!At(base,floor,"LineDist","FloatProperty",4,8)||
       !Bit(base,floor,"bBlockingHit",1)||!Bit(base,floor,"bWalkableFloor",2)||!Bit(base,floor,"bLineTrace",4)||
       !Struct(base,floor,"HitResult",136,at,hit)||at!=12||!Is(base,hit,"HitResult")||
       !Bit(base,hit,"bBlockingHit",1)||!Bit(base,hit,"bStartPenetrating",2))return false;
    struct Field {const char* name;const char* kind;int bytes,offset;};
    constexpr Field fields[]={
        {"FaceIndex","IntProperty",4,4},{"Time","FloatProperty",4,8},{"Distance","FloatProperty",4,12},
        {"Location","StructProperty",12,16},{"ImpactPoint","StructProperty",12,28},{"Normal","StructProperty",12,40},
        {"ImpactNormal","StructProperty",12,52},{"TraceStart","StructProperty",12,64},{"TraceEnd","StructProperty",12,76},
        {"PenetrationDepth","FloatProperty",4,88},{"Item","IntProperty",4,92},
        {"PhysMaterial","WeakObjectProperty",8,96},{"Actor","WeakObjectProperty",8,104},{"Component","WeakObjectProperty",8,112},
        {"BoneName","NameProperty",8,120},{"MyBoneName","NameProperty",8,128}};
    for(const auto& f:fields)if(!At(base,hit,f.name,f.kind,f.bytes,f.offset))return false;
    return native_objects::CaptureLive(base,reinterpret_cast<void*>(floor),api.floor)&&native_objects::CaptureLive(base,reinterpret_cast<void*>(hit),api.hit);
}
inline bool Methods(uintptr_t base,uintptr_t movement){
    uintptr_t cls=0;if(!Read(movement,0x10,cls))return false;
    if(api.cls.address==cls&&Alive(base,api.cls)&&Alive(base,api.mode))return true;
    api={};const auto mode=Function(base,cls,"SetMovementMode","CharacterMovementComponent",0x1CF1600,2);
    if(!mode||!At(base,mode,"NewMovementMode","ByteProperty",1,0)||!At(base,mode,"NewCustomMode","ByteProperty",1,1))return false;
    return native_objects::CaptureLive(base,reinterpret_cast<void*>(cls),api.cls)&&native_objects::CaptureLive(base,reinterpret_cast<void*>(mode),api.mode);
}
inline bool SetMode(uintptr_t base,Event event,uintptr_t movement,unsigned char mode){
    if(!event||!native_tps::OnGameThread()||!Methods(base,movement))return false;
    struct Args {unsigned char mode=0,custom=0;} args{mode};static_assert(sizeof(Args)==2);
    event(reinterpret_cast<void*>(movement),reinterpret_cast<void*>(api.mode.address),&args);
    unsigned char actual=0,custom=0;return Read(movement,0x1B0,actual)&&actual==mode&&Read(movement,0x1B1,custom)&&!custom;
}
inline bool Possessed(uintptr_t base,const Lease& lease){
    uintptr_t owner=0,controller=0,pawn=0,root=0,updated=0;unsigned char role=0,despawning=0;
    return Alive(base,lease.horse)&&Alive(base,lease.movement)&&Alive(base,lease.controller)&&Alive(base,lease.capsule)&&
        Read(lease.movement.address,0xE8,owner)&&owner==lease.horse.address&&
        Read(owner,0x118,role)&&role==3&&Read(owner,0xA66,despawning)&&!despawning&&
        Read(owner,0x348,controller)&&controller==lease.controller.address&&Read(controller,0x350,pawn)&&pawn==owner&&
        Read(owner,0x158,root)&&root==lease.capsule.address&&Read(lease.movement.address,0xF8,updated)&&updated==root;
}
inline void Release(uintptr_t base,Event event,State& s,uintptr_t currentWorld){
    const auto saved=s.lease;s={}; // remove support before requesting native falling
    if(!saved.active||!currentWorld||saved.world!=currentWorld||!Alive(base,saved.horse)||!Alive(base,saved.movement))return;
    uintptr_t owner=0,controller=0;unsigned char role=0,despawning=0,mode=0,custom=0;
    if(!Read(saved.movement.address,0xE8,owner)||owner!=saved.horse.address||!Read(owner,0x118,role)||role!=3||
       !Read(owner,0xA66,despawning)||despawning||!Read(owner,0x348,controller)||
       (controller&&(controller!=saved.controller.address||!Alive(base,saved.controller)))||
       !Read(saved.movement.address,0x1B0,mode)||mode!=1||!Read(saved.movement.address,0x1B1,custom)||custom)return;
    SetMode(base,event,saved.movement.address,3);
}
inline bool AcquireFloor(uintptr_t base,Event event,const local_players::Context& c,uintptr_t movement,State& s){
    unsigned char before=0;uintptr_t root=0,cls=0,rootOwner=0;
    if(!event||!c.world||!native_tps::OnGameThread()||!Read(movement,0x1B0,before)||before!=3||!Methods(base,movement)||
       !FloorSchema(base,api.cls.address)||!Read(c.pawn,0x158,root)||!Read(root,0xE8,rootOwner)||rootOwner!=c.pawn||!Read(root,0x10,cls))return false;
    if(api.capsuleClass.address!=cls||!Alive(base,api.capsuleClass)||!Alive(base,api.height)){
        const auto height=Function(base,cls,"GetScaledCapsuleHalfHeight","CapsuleComponent",0x1CED210,4);
        if(!height||!At(base,height,"ReturnValue","FloatProperty",4,0)||
           !native_objects::CaptureLive(base,reinterpret_cast<void*>(cls),api.capsuleClass)||
           !native_objects::CaptureLive(base,reinterpret_cast<void*>(height),api.height))return false;
    }
    Lease next{};next.world=c.world;
    if(!native_objects::CaptureLive(base,reinterpret_cast<void*>(c.pawn),next.horse)||
       !native_objects::CaptureLive(base,reinterpret_cast<void*>(movement),next.movement)||
       !native_objects::CaptureLive(base,reinterpret_cast<void*>(c.controller),next.controller)||
       !native_objects::CaptureLive(base,reinterpret_cast<void*>(root),next.capsule)||!Possessed(base,next))return false;
    float halfHeight=0;event(reinterpret_cast<void*>(root),reinterpret_cast<void*>(api.height.address),&halfHeight);
    Vector position{};
    if(!Possessed(base,next)||!std::isfinite(halfHeight)||halfHeight<=0||halfHeight>10000||!Read(root,0x1A0,position)||!Finite(position))return false;
    next.plane={position.z-halfHeight,halfHeight};next.active=true;s.lease=next;
    // Walking asks the native floor solver immediately. The lease must already
    // expose its private plane during this callback; no fake Flying state.
    if(!SetMode(base,event,movement,1)||!Possessed(base,next)){
        Release(base,event,s,c.world);return false;
    }
    s.policy={};return true;
}
}
