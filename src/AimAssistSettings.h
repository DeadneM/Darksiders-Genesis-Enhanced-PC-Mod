#pragma once
#include <array>
#include <algorithm>
#include <cmath>

namespace dg::aim_assist {
inline constexpr unsigned Count=7;
inline constexpr std::array<const char*,Count> Names={"Standard Shot","Charge Shot","Graviton Shot","Lava Shot","Beam Shot","Nature Shot","Static Shot"};
inline constexpr std::array<const char*,Count> Classes={"MayhemStandardShot_Blueprint_C","MayhemChargeShot_Blueprint_C","MayhemGravityShot_Blueprint_C","MayhemLavaShot_Blueprint_C","MayhemBeamShot_Blueprint_C","MayhemNatureShot_Blueprint_C","MayhemStaticShot_Blueprint_C"};
inline constexpr std::array<const char*,Count> Packages={"/Game/Blueprints/Player/MayhemStandardShot_Blueprint","/Game/Blueprints/Player/MayhemChargeShot_Blueprint","/Game/Blueprints/Player/MayhemGravityShot_Blueprint","/Game/Blueprints/Player/MayhemLavaShot_Blueprint","/Game/Blueprints/Player/MayhemBeamShot_Blueprint","/Game/Blueprints/Player/MayhemNatureShot_Blueprint","/Game/Blueprints/Player/MayhemStaticShot_Blueprint"};
inline constexpr std::array<const wchar_t*,Count> Keys={L"TPSAimAssistStandard",L"TPSAimAssistCharge",L"TPSAimAssistGraviton",L"TPSAimAssistLava",L"TPSAimAssistBeam",L"TPSAimAssistNature",L"TPSAimAssistStatic"};
inline float Strength(float value){return std::isfinite(value)?std::clamp(value,0.0f,100.0f):0.0f;}
// The native Projectile trace ignores/overlaps character collision profiles.
// Visibility therefore means no earlier blocking surface, not a mandatory
// blocking hit on the selected actor. A hit on that actor's near surface is
// also valid, even though its root/aim point lies farther along the ray.
template<class Ray,class Vec>bool ClearPath(const Ray& ray,const Vec& candidate,const Vec& point,bool hit,bool sameActor){
    if(!hit)return true;
    if(!std::isfinite(point.x)||!std::isfinite(point.y)||!std::isfinite(point.z))return false;
    if(sameActor)return true;
    const float depth=(candidate.x-ray.origin.x)*ray.direction.x+(candidate.y-ray.origin.y)*ray.direction.y+(candidate.z-ray.origin.z)*ray.direction.z;
    const float surface=(point.x-ray.origin.x)*ray.direction.x+(point.y-ray.origin.y)*ray.direction.y+(point.z-ray.origin.z)*ray.direction.z;
    return std::isfinite(depth)&&depth>0&&std::isfinite(surface)&&surface>=depth-1;
}
// New added assistance, NOT a percentage of the native game's own settings.
// Project onto the ray at the target's depth before interpolating, so a sky
// endpoint at100000 units cannot dilute the requested strength.
template<class Ray,class Vec>bool Pull(const Ray& ray,const Vec& candidate,const Vec& obstruction,bool blocked,float percent,Ray& result){
    const float weight=Strength(percent)*.01f;if(weight<=0)return false;
    const float x=candidate.x-ray.origin.x,y=candidate.y-ray.origin.y,z=candidate.z-ray.origin.z;
    const float depth=x*ray.direction.x+y*ray.direction.y+z*ray.direction.z;
    const float distance=std::hypot(std::hypot(x,y),z);
    if(!std::isfinite(distance)||distance<1||distance>100000||!std::isfinite(depth)||depth<=0||depth/distance<.9945218954f)return false; //6 degrees
    const float wall=(obstruction.x-ray.origin.x)*ray.direction.x+(obstruction.y-ray.origin.y)*ray.direction.y+(obstruction.z-ray.origin.z)*ray.direction.z;
    if(blocked&&(!std::isfinite(wall)||wall<depth-1))return false;
    const float dx=ray.direction.x*depth*(1-weight)+x*weight;
    const float dy=ray.direction.y*depth*(1-weight)+y*weight;
    const float dz=ray.direction.z*depth*(1-weight)+z*weight;
    const float length=std::hypot(std::hypot(dx,dy),dz);if(!std::isfinite(length)||length<1)return false;
    result=ray;result.direction={dx/length,dy/length,dz/length};return true;
}
}
