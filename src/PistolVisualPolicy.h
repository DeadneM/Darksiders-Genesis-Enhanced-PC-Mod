#pragma once
#include <array>
#include <cmath>
#include <cstdint>

namespace dg::pistol_visual {
// Row-vector matrices, matching the native CPU ReferenceToLocal FMatrix array.
// These helpers consume copies; no component pose or socket is ever changed.
struct Matrix {float m[4][4]{};};
struct Transform {float q[4]{},t[4]{},s[4]{};};
static_assert(sizeof(Matrix)==64&&sizeof(Transform)==48);
inline bool Finite(const Matrix& a){
    for(const auto& row:a.m)for(float v:row)if(!std::isfinite(v)||std::abs(v)>1000000)return false;
    return true;
}
inline Matrix Multiply(const Matrix& a,const Matrix& b){
    Matrix c{};
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j)
        for(unsigned k=0;k<4;++k)c.m[i][j]+=a.m[i][k]*b.m[k][j];
    return c;
}
inline bool Inverse(const Matrix& a,Matrix& inverse){
    if(!Finite(a))return false;
    double v[4][8]{};
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j){v[i][j]=a.m[i][j];v[i][j+4]=i==j?1.0:0.0;}
    for(unsigned i=0;i<4;++i){
        unsigned pivot=i;
        for(unsigned j=i+1;j<4;++j)if(std::abs(v[j][i])>std::abs(v[pivot][i]))pivot=j;
        if(std::abs(v[pivot][i])<1e-8)return false;
        if(pivot!=i)for(unsigned k=0;k<8;++k){const double tmp=v[i][k];v[i][k]=v[pivot][k];v[pivot][k]=tmp;}
        const double divisor=v[i][i];for(double& x:v[i])x/=divisor;
        for(unsigned j=0;j<4;++j)if(j!=i){const double scale=v[j][i];for(unsigned k=0;k<8;++k)v[j][k]-=scale*v[i][k];}
    }
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j)inverse.m[i][j]=float(v[i][j+4]);
    return Finite(inverse);
}
inline bool ToMatrix(const Transform& p,Matrix& result){
    float norm=0;
    for(float f:p.q){if(!std::isfinite(f))return false;norm+=f*f;}
    if(norm<0.98f||norm>1.02f)return false;
    for(unsigned i=0;i<3;++i)if(!std::isfinite(p.t[i])||std::abs(p.t[i])>10000||
        !std::isfinite(p.s[i])||p.s[i]<0.01f||p.s[i]>100)return false;
    const float x=p.q[0],y=p.q[1],z=p.q[2],w=p.q[3];
    Matrix a{};
    a.m[0][0]=(1-2*(y*y+z*z))*p.s[0];a.m[0][1]=2*(x*y+w*z)*p.s[0];a.m[0][2]=2*(x*z-w*y)*p.s[0];
    a.m[1][0]=2*(x*y-w*z)*p.s[1];a.m[1][1]=(1-2*(x*x+z*z))*p.s[1];a.m[1][2]=2*(y*z+w*x)*p.s[1];
    a.m[2][0]=2*(x*z+w*y)*p.s[2];a.m[2][1]=2*(y*z-w*x)*p.s[2];a.m[2][2]=(1-2*(x*x+y*y))*p.s[2];
    for(unsigned i=0;i<3;++i)a.m[3][i]=p.t[i];a.m[3][3]=1;
    if(!Finite(a))return false;result=a;return true;
}
inline constexpr unsigned BoneCount=80,ArmBones=22;
inline constexpr unsigned Upper[2]={41,18},Clavicle[2]={40,17}; // R, L
using Matrices=std::array<Matrix,BoneCount>;
struct ArmPose {
    std::array<std::array<Matrix,ArmBones>,2> relative{};
    std::array<std::array<Matrix,ArmBones>,2> previous{};
    Matrix rightClavicle{},previousRightClavicle{};
    bool ready=false;unsigned previousMask=0;uint64_t previousTick=0;
    bool Capture(const Matrices& native,const std::array<Matrix,2>& anchors){
        decltype(relative) next{};
        for(unsigned side=0;side<2;++side){
            Matrix inverse{};if(!Inverse(anchors[side],inverse))return false;
            for(unsigned j=0;j<ArmBones;++j){
                const auto& source=native[Upper[side]+j];if(!Finite(source))return false;
                next[side][j]=Multiply(source,inverse);if(!Finite(next[side][j]))return false;
            }
        }
        Matrix spineInverse{};
        if(!Inverse(anchors[0],spineInverse)||!Finite(native[Clavicle[0]]))return false;
        const auto shoulder=Multiply(native[Clavicle[0]],spineInverse);
        if(!Finite(shoulder))return false;
        rightClavicle=shoulder;relative=next;ready=true;previousMask=0;return true;
    }
    // All-or-nothing on invalid input. Return matrices for current and previous
    // render packets only; untouched bones and the opposite hand stay native.
    bool Apply(unsigned mask,const std::array<Matrix,2>& anchors,uint64_t now,
               Matrices& current,Matrices* old){
        if(!ready||!mask||(mask&~3u))return false;
        decltype(relative) next{};
        for(unsigned side=0;side<2;++side)if(mask&(1u<<side)){
            if(!Finite(anchors[side]))return false;
            for(unsigned j=0;j<ArmBones;++j){
                next[side][j]=Multiply(relative[side][j],anchors[side]);
                if(!Finite(next[side][j]))return false;
            }
        }
        Matrix shoulder{};
        if(mask&1u){
            shoulder=Multiply(rightClavicle,anchors[0]);if(!Finite(shoulder))return false;
        }
        for(unsigned side=0;side<2;++side)if(mask&(1u<<side)){
            const bool recent=(previousMask&(1u<<side))&&now>=previousTick&&now-previousTick<=100;
            for(unsigned j=0;j<ArmBones;++j){
                current[Upper[side]+j]=next[side][j];
                if(old)(*old)[Upper[side]+j]=recent?previous[side][j]:next[side][j];
            }
        }
        if(mask&1u){
            const bool recent=(previousMask&1u)&&now>=previousTick&&now-previousTick<=100;
            current[Clavicle[0]]=shoulder;
            if(old)(*old)[Clavicle[0]]=recent?previousRightClavicle:shoulder;
            previousRightClavicle=shoulder;
        }
        previous=next;previousMask=mask;previousTick=now;return true;
    }
};
// The right branch also includes its aiming clavicle, anchored at Spine2.
// The validated left branch keeps its original clavicle-relative behavior.
inline constexpr unsigned PoseAnchor[2]={14,17};
// These are leaf bones in the verified normal Strife mesh. Collapse only
// their rendered triangles; native component poses and firing sockets stay live.
inline bool HideHolsters(unsigned mask,Matrices& current,Matrices* old){
    if(!mask||(mask&~3u))return false;
    for(unsigned side=0;side<2;++side)if(mask&(1u<<side))
        if(!Finite(current[78+side])||(old&&!Finite((*old)[78+side])))return false;
    for(unsigned side=0;side<2;++side)if(mask&(1u<<side)){
        for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j){
            current[78+side].m[i][j]=0;if(old)(*old)[78+side].m[i][j]=0;
        }
    }
    return true;
}
enum class EffectSide { Unknown,Right,Left,Shared };
inline bool HideEffect(EffectSide side,unsigned mask){
    return side==EffectSide::Right?(mask&1u)!=0:side==EffectSide::Left?(mask&2u)!=0:
        side==EffectSide::Shared?mask==3:false;
}
}
