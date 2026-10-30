#pragma once
#include <array>
#include <algorithm>
#include <cmath>

namespace dg::aim_assist {
inline constexpr unsigned Count=7;
inline constexpr std::array<const char*,Count> Names={"Standard Shot","Charge Shot","Graviton Shot","Lava Shot","Beam Shot","Nature Shot","Static Shot"};
inline constexpr std::array<const char*,Count> Classes={"MayhemStandardShot_Blueprint_C","MayhemChargeShot_Blueprint_C","MayhemGravityShot_Blueprint_C","MayhemLavaShot_Blueprint_C","MayhemBeamShot_Blueprint_C","MayhemNatureShot_Blueprint_C","MayhemStaticShot_Blueprint_C"};
inline constexpr std::array<const char*,Count> Packages={"/Game/Blueprints/Player/MayhemStandardShot_Blueprint","/Game/Blueprints/Player/MayhemChargeShot_Blueprint","/Game/Blueprints/Player/MayhemGravityShot_Blueprint","/Game/Blueprints/Player/MayhemLavaShot_Blueprint","/Game/Blueprints/Player/MayhemBeamShot_Blueprint","/Game/Blueprints/Player/MayhemNatureShot_Blueprint","/Game/Blueprints/Player/MayhemStaticShot_Blueprint"};
inline constexpr std::array<const wchar_t*,Count> Keys={L"TPSNativeAimAssistStandard",L"TPSNativeAimAssistCharge",L"TPSNativeAimAssistGraviton",L"TPSNativeAimAssistLava",L"TPSNativeAimAssistBeam",L"TPSNativeAimAssistNature",L"TPSNativeAimAssistStatic"};
inline constexpr std::array<float,Count> Defaults={100,100,100,100,100,100,100};
inline float Strength(float v){return std::isfinite(v)?std::clamp(v,0.0f,200.0f):100.0f;}
inline bool Angles(float v,float h,float percent,float& vertical,float& horizontal){
    if(!std::isfinite(v)||!std::isfinite(h)||v<0||h<0||v>180||h>180)return false;
    const float scale=Strength(percent)*.01f;vertical=std::min(180.0f,v*scale);horizontal=std::min(180.0f,h*scale);return true;
}
}
