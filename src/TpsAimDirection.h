#pragma once
#include <algorithm>
#include <cmath>

namespace dg::tps_aim {
// One camera-forward ray in both modes. Vertical aiming changes only its
// elevation. It never changes the camera pose, yaw, or input ownership.
template<class Vector>
bool Direction(float cameraPitch,float yaw,bool vertical,float offset,Vector& out){
    if(!std::isfinite(cameraPitch)||!std::isfinite(yaw)||!std::isfinite(offset)||
       std::fabs(offset)>45.0f)return false;
    constexpr float rad=0.01745329251994329577f;
    const float pitch=vertical?std::clamp(cameraPitch+offset,-85.0f,85.0f):0.0f;
    const float az=yaw*rad,el=pitch*rad;
    out={std::cos(el)*std::cos(az),std::cos(el)*std::sin(az),std::sin(el)};
    return true;
}
}
