#pragma once
#include <algorithm>
#include <cmath>
namespace dg::hdr {
// -1 Native, 0 SDR, 3/4 scRGB 1000/2000 nits. IDs 1/2 are legacy HDR10.
struct Settings {
    int mode=-1;
    float uiBrightness=-1;
    int buffer=0;
    bool dxgiSignaling=true;
};
struct EngineProfile {
    int enabled=-1, outputDevice=-1, gamut=-1;

    constexpr bool Matches(int mode) const {
        return (mode==3||mode==4)&&enabled==1&&outputDevice==mode+2&&gamut==0;
    }
};
inline Settings Normalize(Settings s){
    if(s.mode < -1 || s.mode > 4)s.mode=-1;
    // The retail DX11 HDR viewport supplies PF_FloatRGBA explicitly.
    if(s.mode==1||s.mode==2)s.mode+=2;
    s.uiBrightness=std::isfinite(s.uiBrightness)&&s.uiBrightness>=0?std::clamp(s.uiBrightness,0.0f,4.0f):-1.0f;
    if(s.buffer!=1)s.buffer=0;
    return s;
}
}
