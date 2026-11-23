#pragma once
#include <algorithm>
#include <cmath>
namespace dg::hdr {
// -1 Native, 0 SDR, 1/2 HDR10 1000/2000 nits, 3/4 scRGB 1000/2000 nits.
struct Settings {
    int mode=-1;
    float uiBrightness=-1;
    int buffer=0;
    bool dxgiSignaling=true,customMetadata=false;
    float peakNits=1000,minNits=.001f,maxCLL=1000,maxFALL=400;
};
inline constexpr bool PQ(int mode){return mode==1||mode==2;}
// Buffer choices: match the profile, native, RGB10, or float16.
inline constexpr bool CompatibleBuffer(const Settings& s){
    return s.mode<=0||s.buffer<2||s.buffer==(PQ(s.mode)?2:3);
}
inline constexpr int OutputDevice(int mode){return mode>0?mode+2:0;}
inline constexpr int Gamut(int mode){return PQ(mode)?2:0;}
struct EngineProfile {
    int enabled=-1, outputDevice=-1, gamut=-1;

    constexpr bool Matches(int mode) const {
        return mode>0&&mode<=4&&enabled==1&&outputDevice==OutputDevice(mode)&&gamut==Gamut(mode);
    }
};
inline float Bound(float v,float low,float high,float fallback){return std::isfinite(v)?std::clamp(v,low,high):std::clamp(fallback,low,high);}
inline Settings Normalize(Settings s){
    if(s.mode < -1 || s.mode > 4)s.mode=-1;
    s.uiBrightness=std::isfinite(s.uiBrightness)&&s.uiBrightness>=0?std::clamp(s.uiBrightness,0.0f,4.0f):-1.0f;
    if(s.buffer<0||s.buffer>3)s.buffer=0;
    s.peakNits=Bound(s.peakNits,80,10000,1000);
    s.minNits=Bound(s.minNits,0,20,.001f);
    s.maxCLL=Bound(s.maxCLL,1,10000,1000);
    s.maxFALL=Bound(s.maxFALL,1,s.maxCLL,400);
    return s;
}
}
