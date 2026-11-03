#pragma once
#include "EffectTransformSettings.h"
#include "NativePropertyField.h"
#include <cstring>
namespace dg::owned_particle {
using Id=native_objects::Identity;
struct Vector {float x=0,y=0,z=0;};
inline bool Finite(Vector v){return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
inline bool Same(Vector a,Vector b){return std::abs(a.x-b.x)<.0001f&&std::abs(a.y-b.y)<.0001f&&std::abs(a.z-b.z)<.0001f;}
struct State {
    Id component{},cls{},scaleFunction{},locationFunction{};
    int scaleOffset=-1,locationOffset=-1;
    Vector baseScale{},baseLocation{},lastScale{},lastLocation{};
    bool captured=false,applied=false;
};
using Event=void(*)(void*,void*,void*);
template<class T>inline bool Read(uintptr_t p,size_t at,T& v){native_objects::MemoryReader r;return r.Read(p,at,v);}
inline bool Alive(uintptr_t base,const Id& id){native_objects::MemoryReader r;return native_objects::Alive(r,base,id);}
inline uintptr_t Function(uintptr_t base,uintptr_t cls,const char* name,uintptr_t entry){
    for(unsigned level=0;cls&&level<16;++level){
        uintptr_t field=0;if(!Read(cls,0x48,field))return 0;
        for(unsigned n=0;field&&n<512;++n){
            uintptr_t kind=0,fn=0,next=0;int index=0,number=0;
            if(!Read(field,0x10,kind)||!Read(field,0x28,next)||next==field)return 0;
            if(!Read(field,0x18,index)||!Read(field,0x1C,number))return 0;
            if(native_objects::NameValueEquals(base,index,number,name))return native_objects::NameIs(base,kind,"Function")&&Read(field,0xC0,fn)&&fn==base+entry?field:0;
            field=next;
        }
        if(field||!Read(cls,0x40,cls))return 0;
    }
    return 0;
}
inline bool Parameter(uintptr_t base,uintptr_t f,const char* name,const char* kind,int bytes,int expected){
    int at=-1;return native_objects::PropertyField(base,f,name,kind,bytes,at)&&at==expected;
}
inline bool Contract(uintptr_t base,const Id& component,State& s){
    uintptr_t cls=0;if(!Alive(base,component)||!Read(component.address,0x10,cls)||!native_objects::NameIs(base,cls,"ParticleSystemComponent"))return false;
    if(s.component==component&&s.cls.address==cls&&Alive(base,s.cls)&&Alive(base,s.scaleFunction)&&Alive(base,s.locationFunction))return true;
    s={};const auto scale=Function(base,cls,"SetRelativeScale3D",0x1DD9E30),location=Function(base,cls,"K2_SetRelativeLocation",0x1DD8A90);
    if(!scale||!location||!Parameter(base,scale,"NewScale3D","StructProperty",12,0)||
       !Parameter(base,location,"NewLocation","StructProperty",12,0)||!Parameter(base,location,"bSweep","BoolProperty",1,12)||
       !Parameter(base,location,"SweepHitResult","StructProperty",136,16)||!Parameter(base,location,"bTeleport","BoolProperty",1,152)||
       !native_objects::PropertyField(base,cls,"RelativeScale3D","StructProperty",12,s.scaleOffset)||
       !native_objects::PropertyField(base,cls,"RelativeLocation","StructProperty",12,s.locationOffset)||
       !native_objects::CaptureLive(base,reinterpret_cast<void*>(cls),s.cls)||
       !native_objects::CaptureLive(base,reinterpret_cast<void*>(scale),s.scaleFunction)||
       !native_objects::CaptureLive(base,reinterpret_cast<void*>(location),s.locationFunction))return false;
    s.component=component;return true;
}
// Callers must prove this is their private PSC with the expected template and
// local owner, on the game thread. No mesh, actor, shared template or material edits.
inline bool Apply(uintptr_t base,const Id& component,effect_transform::Values values,float preset,State& s,Event event=nullptr){
    if(!Contract(base,component,s))return false;
    Vector scale{},location{};
    if(!Read(component.address,size_t(s.scaleOffset),scale)||!Read(component.address,size_t(s.locationOffset),location)||!Finite(scale)||!Finite(location))return false;
    if(!s.captured){s.baseScale=scale;s.baseLocation=location;s.captured=true;}
    else if(s.applied){
        // Native attachment updates win; never multiply our previous output again.
        if(!Same(scale,s.lastScale))s.baseScale=scale;
        if(!Same(location,s.lastLocation))s.baseLocation=location;
    }
    values=effect_transform::Normalize(values);const float factor=values.size*preset;
    Vector wantedScale{s.baseScale.x*factor,s.baseScale.y*factor,s.baseScale.z*factor};
    const Vector wantedLocation{s.baseLocation.x+values.x,s.baseLocation.y+values.y,s.baseLocation.z+values.z};
    if(!Finite(wantedScale)||!Finite(wantedLocation)||factor<0||factor>25)return false;
    if(!event)event=reinterpret_cast<Event>(base+0xAC0630);
    if(!Same(scale,wantedScale))event(reinterpret_cast<void*>(component.address),reinterpret_cast<void*>(s.scaleFunction.address),&wantedScale);
    if(!Same(location,wantedLocation)){
        struct Args {Vector location;unsigned char sweep=0,pad[3]{};unsigned char hit[136]{};unsigned char teleport=1,pad1[3]{};} args{wantedLocation};
        static_assert(sizeof(Args)==156);
        event(reinterpret_cast<void*>(component.address),reinterpret_cast<void*>(s.locationFunction.address),&args);
    }
    s.lastScale=wantedScale;s.lastLocation=wantedLocation;s.applied=true;return true;
}
}
