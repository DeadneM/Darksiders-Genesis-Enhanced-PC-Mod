#pragma once
#include "PistolVisualPolicy.h"
#include <algorithm>
#include <cstddef>

namespace dg::pistol_visual {
// Supported retail ABI, checked against native BlendSpace evaluation. These
// buffers are private output, never the character's local/socket transforms.
struct alignas(16) LocalTransform {float q[4]{},t[4]{},s[4]{};};
struct NativeArray {uintptr_t data=0;int count=0,capacity=0;};
struct CompactPose {NativeArray transforms{};uintptr_t container=0;};
struct Curve {NativeArray values{};uintptr_t ids=0;uint16_t elements=0;uint8_t valid=0,pad[5]{};};
struct PoseContext {uintptr_t proxy=0;CompactPose pose{};Curve curve{};uint8_t additive=0,pad[7]{};};
struct BlendSample {int index=0,pad0=0;uint64_t pad1=0;float weight=0,time=0;uint8_t unused[40]{};};
static_assert(sizeof(LocalTransform)==48&&sizeof(BlendSample)==64);
static_assert(sizeof(PoseContext)==0x48&&offsetof(PoseContext,pose)==8&&offsetof(PoseContext,curve)==0x20&&offsetof(PoseContext,additive)==0x40);

inline bool LocalMatrix(const LocalTransform& t,Matrix& m){
    float norm=0;
    for(float v:t.q){if(!std::isfinite(v))return false;norm+=v*v;}
    if(std::abs(norm-1)>0.02f)return false;
    for(unsigned i=0;i<3;++i)if(!std::isfinite(t.t[i])||!std::isfinite(t.s[i])||std::abs(t.s[i])>100)return false;
    const float x=t.q[0],y=t.q[1],z=t.q[2],w=t.q[3];
    m={};
    m.m[0][0]=(1-2*(y*y+z*z))*t.s[0];m.m[0][1]=2*(x*y+w*z)*t.s[0];m.m[0][2]=2*(x*z-w*y)*t.s[0];
    m.m[1][0]=2*(x*y-w*z)*t.s[1];m.m[1][1]=(1-2*(x*x+z*z))*t.s[1];m.m[1][2]=2*(y*z+w*x)*t.s[1];
    m.m[2][0]=2*(x*z+w*y)*t.s[2];m.m[2][1]=2*(y*z-w*x)*t.s[2];m.m[2][2]=(1-2*(x*x+y*y))*t.s[2];
    for(unsigned i=0;i<3;++i)m.m[3][i]=t.t[i];m.m[3][3]=1;
    return Finite(m);
}
struct AnimationRig {
    std::array<int,BoneCount> parents{};
    Matrices inverseBind{};
    bool Skin(const std::array<LocalTransform,BoneCount>& local,Matrices& skin) const {
        Matrices component{},next{};
        for(unsigned i=0;i<BoneCount;++i){
            if(parents[i]<(i?0:-1)||parents[i]>=int(i)||!LocalMatrix(local[i],component[i]))return false;
            if(parents[i]>=0)component[i]=Multiply(component[i],component[size_t(parents[i])]);
            next[i]=Multiply(inverseBind[i],component[i]);if(!Finite(next[i]))return false;
        }
        skin=next;return true;
    }
};
// The native unarmed BlendSpace1D has Run=0, Idle=1, Walk=2. Armed and
// unarmed sample indices differ, so never reuse the armed node's sample cache.
inline std::array<float,3> UnarmedWeights(float speed){
    const float x=std::clamp(speed,0.0f,1.0f);
    if(x<=1.0f/3)return {0,1-3*x,3*x};
    const float run=(3*x-1)/2;return {run,0,1-run};
}
struct AnimationClock {
    float phase=0,speed=0,time=0;bool ready=false;
    bool Step(float now,float normalizedSpeed,float nativePhase,const std::array<float,3>& lengths){
        if(!std::isfinite(now)||!std::isfinite(normalizedSpeed)||!std::isfinite(nativePhase))return false;
        for(float length:lengths)if(!std::isfinite(length)||length<=0.01f||length>60)return false;
        const float dt=ready?now-time:0;
        const float target=std::clamp(normalizedSpeed,0.0f,1.0f);
        if(!ready||dt<0||dt>0.25f){
            phase=nativePhase-std::floor(nativePhase);speed=target;time=now;ready=true;return true;
        }
        // World time freezes on pause. Native clips provide the movement;
        // no sine-wave rotation or frozen pose is synthesized here.
        speed+=std::clamp(target-speed,-dt/0.4f,dt/0.4f);
        const auto weights=UnarmedWeights(speed);float duration=0;
        for(unsigned i=0;i<3;++i)duration+=weights[i]*lengths[i];
        phase+=dt/duration;phase-=std::floor(phase);time=now;return true;
    }
    unsigned Samples(const std::array<float,3>& lengths,std::array<BlendSample,2>& samples) const {
        const auto weights=UnarmedWeights(speed);unsigned n=0;samples={};
        for(unsigned i=0;i<3;++i)if(weights[i]>0){
            samples[n].index=int(i);samples[n].weight=weights[i];samples[n].time=phase*lengths[i];++n;
        }
        return n;
    }
};
}
