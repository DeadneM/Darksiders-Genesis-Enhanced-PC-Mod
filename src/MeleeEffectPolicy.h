#pragma once
#include "NativeBonePoint.h"
#include <cmath>
namespace dg::melee_effect {
enum class Side {None,Right,Left,Shared,AuthoredHand};
inline unsigned Mask(int choice){return choice==1?2u:choice==2?1u:choice==3?3u:0u;}
inline bool Near(float a,float b){return std::isfinite(a)&&std::abs(a-b)<.05f;}
inline Side Classify(unsigned sequence,native_bone::Point offset){
    if(!native_bone::Finite(offset))return Side::None;
    if(sequence==7&&Near(offset.x,0)&&Near(offset.z,100)&&(Near(offset.y,60)||Near(offset.y,-60)))return Side::AuthoredHand;
    if(sequence==6&&Near(offset.x,100)&&Near(offset.y,0)&&Near(offset.z,160))return Side::Shared;
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
    return side==Side::Right?(mask&1)!=0:side==Side::Left?(mask&2)!=0:side==Side::Shared?mask==3:side==Side::AuthoredHand?mask!=0:false;
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
    if(side==Side::Shared||side==Side::AuthoredHand){
        auto distance=[&](native_bone::Point p){const double x=p.x-source.position[0],y=p.y-source.position[1],z=p.z-source.position[2];return x*x+y*y+z*z;};
        const bool useLeft=distance(left)<distance(right);
        if(side==Side::AuthoredHand&&!Selected(useLeft?Side::Left:Side::Right,mask))return false;
        if(useLeft)hand=left;
    }
    // These authored root-centred arcs cover a blade sweep. Keep their
    // orientation and lifetime; a bare hand gets a compact 40% visual arc.
    result=source;result.position[0]=hand.x;result.position[1]=hand.y;result.position[2]=hand.z;
    for(unsigned i=0;i<3;++i)result.scale[i]*=.4f;
    return true;
}
// Attached slash notifies use KeepRelativeOffset on Bone_Strife_Root.
// Express the hand in that same frame: retain parent, socket and rotation.
inline bool Attached(const Transform& root,native_bone::Point offset,native_bone::Point scale,
                     Side side,unsigned mask,native_bone::Point right,native_bone::Point left,
                     native_bone::Point& outOffset,native_bone::Point& outScale){
    if(!native_bone::Finite(offset)||!native_bone::Finite(scale))return false;
    Transform checked{};if(!Adjust(root,side,mask,right,left,checked))return false;
    for(unsigned i=0;i<3;++i)if(std::abs(root.scale[i])<1e-4f)return false;
    const auto rotate=[](native_bone::Point p,const float* q,float sign){
        const float x=q[0]*sign,y=q[1]*sign,z=q[2]*sign;
        const native_bone::Point t{2*(y*p.z-z*p.y),2*(z*p.x-x*p.z),2*(x*p.y-y*p.x)};
        return native_bone::Point{p.x+q[3]*t.x+y*t.z-z*t.y,
            p.y+q[3]*t.y+z*t.x-x*t.z,p.z+q[3]*t.z+x*t.y-y*t.x};
    };
    auto world=rotate({offset.x*root.scale[0],offset.y*root.scale[1],offset.z*root.scale[2]},root.rotation,1);
    Transform source=root;source.position[0]+=world.x;source.position[1]+=world.y;source.position[2]+=world.z;
    if(!Adjust(source,side,mask,right,left,checked))return false;
    auto local=rotate({checked.position[0]-root.position[0],checked.position[1]-root.position[1],
        checked.position[2]-root.position[2]},root.rotation,-1);
    outOffset={local.x/root.scale[0],local.y/root.scale[1],local.z/root.scale[2]};
    outScale={scale.x*.4f,scale.y*.4f,scale.z*.4f};
    return native_bone::Finite(outOffset)&&native_bone::Finite(outScale);
}
}
