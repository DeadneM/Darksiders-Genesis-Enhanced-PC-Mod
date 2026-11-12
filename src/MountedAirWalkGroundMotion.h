#pragma once
#include "MountedAirWalkNative.h"
namespace dg::mounted_air {
// Read the current horse's ground values, including existing speed tunings.
// No PhysicsVolume, MovementMode, velocity or persistent property writes.
struct GroundFields {Id cls{};int friction=-1,brake=-1,walk=-1;};
inline GroundFields groundFields{};
struct Ground {float friction=0,brake=0,walk=0;};
inline bool GroundValues(uintptr_t base,uintptr_t movement,Ground& out){
    uintptr_t cls=0;if(!Read(movement,0x10,cls))return false;
    auto& f=groundFields;
    if(f.cls.address!=cls||!Alive(base,f.cls)){
        f={};
        if(!native_objects::PropertyField(base,cls,"GroundFriction","FloatProperty",4,f.friction)||
           !native_objects::PropertyField(base,cls,"BrakingDecelerationWalking","FloatProperty",4,f.brake)||
           !native_objects::PropertyField(base,cls,"MaxWalkSpeed","FloatProperty",4,f.walk)||
           !native_objects::CaptureLive(base,reinterpret_cast<void*>(cls),f.cls)){f={};return false;}
    }
    return Read(movement,size_t(f.friction),out.friction)&&Read(movement,size_t(f.brake),out.brake)&&Read(movement,size_t(f.walk),out.walk)&&
        std::isfinite(out.friction)&&out.friction>=0&&out.friction<=10000&&
        std::isfinite(out.brake)&&out.brake>=0&&out.brake<=10000000&&std::isfinite(out.walk)&&out.walk>=0&&out.walk<=10000000;
}
}
