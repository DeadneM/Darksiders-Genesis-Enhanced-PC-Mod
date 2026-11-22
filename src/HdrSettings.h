#pragma once
#include <algorithm>
#include <cmath>
namespace dg::hdr {
// -1 Native, 0 SDR, 1/2 HDR10 1000/2000 nits, 3/4 scRGB 1000/2000 nits.
// These are engine requests, not proof of active HDR transport.
struct Settings {int mode=-1;float uiBrightness=-1;};
inline Settings Normalize(Settings s){
    if(s.mode < -1 || s.mode > 4)s.mode=-1;
    s.uiBrightness=std::isfinite(s.uiBrightness)&&s.uiBrightness>=0?std::clamp(s.uiBrightness,0.0f,4.0f):-1.0f;
    return s;
}
}
