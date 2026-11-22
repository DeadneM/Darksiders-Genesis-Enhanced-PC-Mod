#pragma once
#include <algorithm>
#include <cmath>
namespace dg::hdr {
// -1 Native, 0 SDR, 1/2 HDR10 1000/2000 nits, 3/4 scRGB 1000/2000 nits.
// These are engine requests, not proof of active HDR transport.
struct Settings {
    int mode=-1;float uiBrightness=-1;
    int buffer=0;bool dxgiSignaling=true,customMetadata=false;
    float peakNits=1000,minNits=.001f,maxCLL=1000,maxFALL=400;
};
inline constexpr bool PQ(int mode){return mode==1||mode==2;}
inline constexpr int BufferValue(const Settings& s){return s.mode<=0||s.buffer==1?-1:s.buffer==2?4:s.buffer==3?3:PQ(s.mode)?4:3;}
inline float Bound(float v,float low,float high,float fallback){return std::isfinite(v)?std::clamp(v,low,high):std::clamp(fallback,low,high);}
inline Settings Normalize(Settings s){
    if(s.mode < -1 || s.mode > 4)s.mode=-1;
    s.uiBrightness=std::isfinite(s.uiBrightness)&&s.uiBrightness>=0?std::clamp(s.uiBrightness,0.0f,4.0f):-1.0f;
    if(s.buffer<0||s.buffer>3)s.buffer=0;
    s.peakNits=Bound(s.peakNits,80,10000,1000);s.minNits=Bound(s.minNits,0,20,.001f);
    s.maxCLL=Bound(s.maxCLL,1,10000,1000);s.maxFALL=Bound(s.maxFALL,1,s.maxCLL,400);
    return s;
}
}
