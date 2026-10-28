#pragma once
#include <algorithm>
#include <cmath>
namespace dg::recovery_settings {
inline float LandingDelay(float v){return std::isfinite(v)?std::clamp(v,.05f,.8f):.15f;}
inline float GroundTime(float v){return std::isfinite(v)?std::clamp(v,0.f,.25f):.05f;}
inline float TailDelay(float v){return std::isfinite(v)?std::clamp(v,0.f,1.f):0.f;}
}
