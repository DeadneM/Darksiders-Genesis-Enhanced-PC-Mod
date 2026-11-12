#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
namespace dg::mounted_air {
struct Vector {float x=0,y=0,z=0;};
struct Weak {int32_t index=-1,serial=0;};
struct Name {int32_t index=0,number=0;};
// Exact retail layouts, checked again against reflected structs before use.
struct Hit {
    uint32_t flags=0;int32_t face=-1;float time=1,distance=0;
    Vector location{},impact{},normal{},impactNormal{},start{},end{};
    float penetration=0;int32_t item=-1;
    Weak material{},actor{},component{};Name bone{},myBone{};
};
struct Floor {uint32_t flags=0;float distance=0,lineDistance=0;Hit hit{};};
static_assert(sizeof(Hit)==136&&alignof(Hit)==4&&offsetof(Hit,time)==8&&offsetof(Hit,impact)==28&&offsetof(Hit,component)==112);
static_assert(sizeof(Floor)==148&&alignof(Floor)==4&&offsetof(Floor,hit)==12);
struct Plane {float z=0,halfHeight=0;};
inline bool Finite(const Vector& v){return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
inline bool Walkable(const Floor& f){return (f.flags&3)==3&&(f.hit.flags&3)==1;}
inline bool ApplyFloor(Plane& plane,const Vector& location,float line,float sweep,float radius,Floor& floor){
    if(!Finite(location)||!std::isfinite(plane.z)||!std::isfinite(plane.halfHeight)||plane.halfHeight<=0||
       !std::isfinite(line)||line<0||!std::isfinite(sweep)||sweep<=0||!std::isfinite(radius)||radius<=0)return false;
    const float distance=location.z-plane.halfHeight-plane.z;
    if(!std::isfinite(distance)||distance< -5||distance>sweep)return false;
    const float nativeDistance=(floor.flags&4)?floor.lineDistance:floor.distance;
    if(Walkable(floor)&&std::isfinite(nativeDistance)&&nativeDistance<=distance+.001f){
        // A real higher surface wins; leaving it keeps support at that height.
        if(floor.hit.component.index>=0&&Finite(floor.hit.impact))plane.z=std::max(plane.z,floor.hit.impact.z);
        return false;
    }
    Floor next{};next.flags=3;next.distance=next.lineDistance=distance;
    auto& hit=next.hit;hit.flags=1;hit.time=std::clamp(distance/sweep,0.f,1.f);hit.distance=std::max(distance,0.f);
    hit.location={location.x,location.y,plane.z+plane.halfHeight};hit.impact={location.x,location.y,plane.z};
    hit.normal=hit.impactNormal={0,0,1};hit.start=location;hit.end={location.x,location.y,location.z-sweep};
    floor=next;return true;
}
}
