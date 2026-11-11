#pragma once
// V1.141 defaults explicitly approved by the user on 1 November 2026.
// Used by factory resets, missing-value fallbacks and individual Default buttons.
namespace dg::defaults {
inline constexpr int MenuKey=0x2D; // Insert, restored at the user's request
inline constexpr bool UpdaterBeta=true;
inline constexpr bool ReduceAmmo=true;
inline constexpr bool AutoCheckUpdates=true,MountedJump=true,MountedHolsters=true,BonusMountSkins=true;
inline constexpr bool WeaponSheathing=true,NeverAutoSheathe=false,HeavyLandingRecovery=true,RecoveryTail=true;
inline constexpr bool AirDashRework=true;
inline constexpr bool CustomDOF=true,AimAssistEnabled=true;
inline constexpr int MouseReticleHideMode=1;
inline constexpr int SheatheKeyboardKey='L',SheathePadButtonA=0x0100,SheathePadButtonB=0x0040;
inline constexpr int AimPadButtonA=0x0002,AimPadButtonB=0; // D-pad down alone; user-approved 11 Nov 2026
inline constexpr float InteractionHudScale=0.50f,TPSCoopFOV=80.0f,AimAssistGlobal=50.0f;
inline constexpr float NativeDOFAmount=0.5f,TPSDOFAmount=0.5f,TPSDOFDistance=250.0f;
inline constexpr float HotstreakGain=1.0f,UltimateDuration=1.0f,GrappleRange=1.0f;
inline constexpr float AmmoDivisor=1.0f,HotstreakDivisor=1.0f,PotionRecharge=1.0f;
inline constexpr float UltimateDistance=0.30f;
inline constexpr float UltimateHeight=80.0f;
inline constexpr float HorseDistance=0.25f;
inline constexpr float HorseHeight=40.0f;
inline constexpr float WrathGain=1.0f;
inline constexpr float UltimateGain=1.0f;
inline constexpr float SynergyGain=1.0f;
}
