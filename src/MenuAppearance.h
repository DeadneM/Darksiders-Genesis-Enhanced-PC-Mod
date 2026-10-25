#pragma once
#include <algorithm>
#include <cmath>
namespace dg::menu {
inline constexpr float DefaultFontSize=13.f,MinFontSize=10.f,MaxFontSize=32.f;
inline float FontSize(float value){return std::isfinite(value)?std::clamp(value,MinFontSize,MaxFontSize):DefaultFontSize;}
}
