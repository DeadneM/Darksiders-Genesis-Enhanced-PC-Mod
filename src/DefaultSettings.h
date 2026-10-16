#pragma once
// V1.43 defaults explicitly chosen by the user in the installed V1.42 INI.
// Used by factory resets, missing-value fallbacks and individual Default buttons.
namespace dg::defaults {
inline constexpr int MenuKey=0x2D; // Insert, restored at the user's request
inline constexpr bool UpdaterBeta=true;
inline constexpr bool ReduceAmmo=true;
inline constexpr float UltimateDistance=0.30f;
inline constexpr float UltimateHeight=80.0f;
inline constexpr float HorseDistance=0.25f;
inline constexpr float HorseHeight=40.0f;
inline constexpr float WrathGain=4.0f;
inline constexpr float UltimateGain=4.0f;
inline constexpr float SynergyGain=4.0f;
}
