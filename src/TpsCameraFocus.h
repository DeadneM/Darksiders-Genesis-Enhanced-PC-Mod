#pragma once
#include <atomic>
#include <cmath>
#include <cstring>

namespace dg::camera_focus {
// Exact retail FMinimalViewInfo/PostProcessSettings layout, gated before the
// camera hook is installed. Only a native per-frame output is ever changed.
inline std::atomic_bool layoutReady{false};
inline bool Ready(){return layoutReady.load(std::memory_order_acquire);}
struct Point {float x=0,y=0,z=0;};
inline bool Compensate(void* output,Point target){
    if(!Ready()||!output)return false;
    auto* p=static_cast<unsigned char*>(output);
    float view[7]{},weight=0;
    std::memcpy(view,p,sizeof(view));std::memcpy(&weight,p+0x38,4);
    for(float v:view)if(!std::isfinite(v))return false;
    // Never increase the influence of other authored camera overrides.
    // The observed player cameras use 1. Partial/unknown blends stay native.
    if(weight!=1.0f||!std::isfinite(target.x)||!std::isfinite(target.y)||
       !std::isfinite(target.z)||view[6]<1||view[6]>179)return false;
    constexpr float rad=0.01745329251994329577f;
    const double pitch=double(view[3])*rad,yaw=double(view[4])*rad;
    // DOF uses forward depth, rather than the slant distance to the player.
    const double depth=(double(target.x)-view[0])*std::cos(pitch)*std::cos(yaw)+
        (double(target.y)-view[1])*std::cos(pitch)*std::sin(yaw)+
        (double(target.z)-view[2])*std::sin(pitch);
    if(!std::isfinite(depth)||depth<1||depth>20000)return false;
    const float focus=static_cast<float>(depth);
    // FMinimalViewInfo.PP=0x40; FocalDistance=0x454; override +0x16 mask 0x10.
    // Preserve quality, method, aperture, blur size/regions and every other
    // override/weight. No UObject, volume or persistent setting is patched.
    std::memcpy(p+0x40+0x454,&focus,4);
    p[0x40+0x16]|=0x10;
    return true;
}
}
