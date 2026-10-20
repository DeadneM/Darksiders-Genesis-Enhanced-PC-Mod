#pragma once
#include <atomic>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <algorithm>

namespace dg::camera_focus {
// Exact retail FMinimalViewInfo/PostProcessSettings layout, gated before the
// camera hook is installed. Only a native per-frame output is ever changed.
inline std::atomic_bool layoutReady{false};
inline std::atomic_bool lensLayoutReady{false};
inline bool Ready(){return layoutReady.load(std::memory_order_acquire);}
inline bool LensReady(){return lensLayoutReady.load(std::memory_order_acquire);}
struct Point {float x=0,y=0,z=0;};
struct Bounds {Point center{},extent{};bool valid=false;};
// Read only a live owner's mesh. Bounds are copied values, never a retained
// component pointer. A missing/unexpected mesh cannot change the focal region.
template<class Reader> Bounds ReadBounds(Reader& reader,uintptr_t actor){
    uintptr_t mesh=0,owner=0;float native[7]{};
    if(!actor||!reader.Read(actor,0x380,mesh)||!mesh||
       !reader.Read(mesh,0xE8,owner)||owner!=actor||!reader.Read(mesh,0x148,native))return {};
    for(float v:native)if(!std::isfinite(v))return {};
    if(native[3]<=0||native[4]<=0||native[5]<=0||native[6]<=0||native[6]>20000||
       native[3]>native[6]||native[4]>native[6]||native[5]>native[6])return {};
    return {{native[0],native[1],native[2]},{native[3],native[4],native[5]},true};
}
inline Bounds Merge(Bounds a,Bounds b){
    if(!a.valid)return b;if(!b.valid)return a;
    const Point low{std::min(a.center.x-a.extent.x,b.center.x-b.extent.x),
        std::min(a.center.y-a.extent.y,b.center.y-b.extent.y),std::min(a.center.z-a.extent.z,b.center.z-b.extent.z)};
    const Point high{std::max(a.center.x+a.extent.x,b.center.x+b.extent.x),
        std::max(a.center.y+a.extent.y,b.center.y+b.extent.y),std::max(a.center.z+a.extent.z,b.center.z+b.extent.z)};
    return {{(low.x+high.x)*.5f,(low.y+high.y)*.5f,(low.z+high.z)*.5f},
        {(high.x-low.x)*.5f,(high.y-low.y)*.5f,(high.z-low.z)*.5f},true};
}
inline bool Compensate(void* output,Point target,bool lens=false,Bounds bounds={}){
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
    // FMinimalViewInfo.PP=0x40; FocalDistance=0x454; override +0x16 mask 0x08 (native blend at 1B2F693).
    // Quality, aperture and blend weights remain authored.
    // No UObject, level volume or persistent render setting is patched.
    std::memcpy(p+0x40+0x454,&focus,4);
    p[0x40+0x16]|=0x08;
    // Keep the level's method and distant blur. For Gaussian DOF the focal
    // region starts AFTER the focus plane; the portion in front of the pivot
    // used to blur at close TPS distances. NearBlurSize=0 protects that side;
    // the projected mesh bounds protect the portion behind the pivot.
    // No CircleDOF override: its wide-FOV lens made distant scenery too sharp.
    if(lens&&LensReady()&&p[0x40+0x23]<=2){
        const float zero=0;
        // Native blend 1B2F87E tests the sign of +17 for NearBlurSize.
        // +18 mask01 is FarBlurSize and must remain untouched.
        std::memcpy(p+0x40+0x474,&zero,4);p[0x40+0x17]|=0x80;
        if(bounds.valid){
            const double fx=std::cos(pitch)*std::cos(yaw),fy=std::cos(pitch)*std::sin(yaw),fz=std::sin(pitch);
            const double radius=std::fabs(fx)*bounds.extent.x+std::fabs(fy)*bounds.extent.y+std::fabs(fz)*bounds.extent.z;
            const double centerDepth=(double(bounds.center.x)-view[0])*fx+(double(bounds.center.y)-view[1])*fy+(double(bounds.center.z)-view[2])*fz;
            // Reject stale/far-away bounds during an unproved owner transition.
            const double dx=double(bounds.center.x)-target.x,dy=double(bounds.center.y)-target.y,dz=double(bounds.center.z)-target.z;
            const double extent2=double(bounds.extent.x)*bounds.extent.x+double(bounds.extent.y)*bounds.extent.y+double(bounds.extent.z)*bounds.extent.z;
            if(std::isfinite(radius)&&std::isfinite(centerDepth)&&radius>0&&radius<=20000&&
               dx*dx+dy*dy+dz*dz<=extent2){
                float region=static_cast<float>(std::clamp(centerDepth+radius-depth,0.0,20000.0));
                float authored=0;std::memcpy(&authored,p+0x40+0x460,4);
                if((p[0x40+0x17]&0x04)&&std::isfinite(authored)&&authored>region)region=authored;
                std::memcpy(p+0x40+0x460,&region,4);p[0x40+0x17]|=0x04;
            }
        }
    }
    return true;
}
}
