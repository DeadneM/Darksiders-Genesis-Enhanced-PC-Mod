#pragma once
#include "NativeBonePoint.h"
#include <cmath>
namespace dg::melee_effect {
enum class Side {None,Right,Left,Shared};
inline unsigned Mask(int choice){return choice==1?2u:choice==2?1u:choice==3?3u:0u;}
inline bool Near(float a,float b){return std::isfinite(a)&&std::abs(a-b)<.05f;}
inline Side Classify(unsigned sequence,native_bone::Point offset){
    if(!native_bone::Finite(offset))return Side::None;
    if(sequence==5&&Near(offset.x,0)&&((Near(offset.y,-100)&&Near(offset.z,125))||
       (Near(offset.y,-150)&&Near(offset.z,90))||(Near(offset.y,0)&&Near(offset.z,125))))return Side::Shared;
    if(!Near(offset.z,100))return Side::None;
    if(sequence==1){
        if(Near(offset.x,0)&&Near(offset.y,-100))return Side::Left;
        if(Near(offset.x,-30)&&Near(offset.y,-60))return Side::Right;
    }
    if(sequence==2){
        if(Near(offset.x,0)&&Near(offset.y,-100))return Side::Left;
        if(Near(offset.x,50)&&Near(offset.y,-180))return Side::Right;
    }
    if(sequence==3&&Near(offset.x,0)&&(Near(offset.y,-100)||Near(offset.y,-200)))return Side::Shared;
    if(sequence==4&&Near(offset.x,0)&&(Near(offset.y,-70)||Near(offset.y,-100)))return Side::Shared;
    return Side::None;
}
inline bool Selected(Side side,unsigned mask){
    return side==Side::Right?(mask&1)!=0:side==Side::Left?(mask&2)!=0:side==Side::Shared?mask==3:false;
}
struct alignas(16) Transform {float rotation[4]{},position[4]{},scale[4]{};};
static_assert(sizeof(Transform)==48);
inline bool Adjust(const Transform& source,Side side,unsigned mask,native_bone::Point right,native_bone::Point left,Transform& result){
    if(!Selected(side,mask)||!native_bone::Finite(right)||!native_bone::Finite(left))return false;
    float norm=0;for(float q:source.rotation){if(!std::isfinite(q))return false;norm+=q*q;}
    if(std::abs(norm-1)>.02f)return false;
    for(unsigned i=0;i<3;++i)if(!std::isfinite(source.position[i])||std::abs(source.position[i])>1e7f||
       !std::isfinite(source.scale[i])||std::abs(source.scale[i])>1000)return false;
    auto hand=side==Side::Left?left:right;
    if(side==Side::Shared){
        auto distance=[&](native_bone::Point p){const double x=p.x-source.position[0],y=p.y-source.position[1],z=p.z-source.position[2];return x*x+y*y+z*z;};
        if(distance(left)<distance(right))hand=left;
    }
    // These authored root-centred arcs cover a blade sweep. Keep their
    // orientation and lifetime; a bare hand gets a compact 40% visual arc.
    result=source;result.position[0]=hand.x;result.position[1]=hand.y;result.position[2]=hand.z;
    for(unsigned i=0;i<3;++i)result.scale[i]*=.4f;
    return true;
}
}
