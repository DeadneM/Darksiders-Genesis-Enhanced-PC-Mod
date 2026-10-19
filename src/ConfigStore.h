#pragma once

#include "DefaultSettings.h"
#include <array>
#include <atomic>
#include <string>

namespace dg::config {
enum class CameraAction : int { HeightUp = 0, HeightDown, ZoomOut, ZoomIn, PitchDown, PitchUp, YawLeft, YawRight, Count };
// New V0.92 TPS keyboard actions have the same semantics as the existing
// controller actions. OFF (0) is intentional for legacy compatibility.
enum class TpsKeyboardAction : int { Horse = 0, Toggle, Recenter, Suspend, WheelG, WheelTab, Count };
inline constexpr int kSheathKeyboardBindingIndex=int(CameraAction::Count)+int(TpsKeyboardAction::Count);
inline constexpr std::array<const char*, 6> kTpsKeyboardLabels = {
    "Summon / dismount horse","Long press TPS ON/OFF",
    "Recenter TPS camera","Hold temporary vanilla view",
    "Equipment wheel (G)","Ammunition / abilities wheel (Tab)"
};
inline constexpr std::array<const wchar_t*, 6> kTpsKeyboardTokens = {
    L"Horse",L"ToggleTPS",L"RecenterTPS",L"TemporaryView",L"RadialG",L"RadialTab"
};
inline constexpr std::array<const char*, 8> kCameraLabels = {
  "Raise Height", "Lower Height", "Zoom Out", "Zoom In", "Tilt Down", "Tilt Up",
  "Rotate Left", "Rotate Right"
};
inline constexpr std::array<const wchar_t*, 8> kCameraTokens = {
  L"HeightUp", L"HeightDown", L"ZoomOut", L"ZoomIn", L"PitchDown", L"PitchUp",
  L"YawLeft", L"YawRight"
};

enum class Action : int {
    None = 0,
    ToggleHUD,
    MovementSpeed,
    ActionRecovery,
    SkipIntroVideos,
    ThirdPerson,
    SkipLogos,
    SkipWarning,
    FOV,
    PistolDamage,
    MeleeDamage,
    JumpHeight,
    GlideDuration,
    HorseSpeed,
    HorseSprintSpeed,
    HorseSprintDuration,
    HotstreakCharge,
    CameraHeightUp,
    CameraHeightDown,
    CameraZoomOut,
    CameraZoomIn,
    CameraPitchDown,
    CameraPitchUp,
    CameraYawLeft,
    CameraYawRight,
    CameraReset,
    TpsCameraReset,
    RefillWrath,
    RefillUltimate,
    RefillSynergy,
    RefillAllGauges,
    UnlimitedWrath,
    UnlimitedUltimate,
    UnlimitedSynergy,
    UnlimitedAllGauges,
    ReduceAmmo,
    UnlimitedAmmo,
    RefillPotions,
    PotionRecharge,
    UnlimitedPotions,
    RefillHealth,
    UnlimitedHealth,
    UnlimitedGlide,
    UnlimitedHorseSprint,
    ReduceHotstreak,
    UnlimitedHotstreak,
    RefillHotstreak,
    RefillAmmo,
    SheatheWeapons,
    FiniteStandardAmmo,
    GrappleRange,
    InfiniteDoubleJump,
    UltimateDecreaseRate,
    Count
};

inline constexpr std::array<const char*, static_cast<std::size_t>(Action::Count)>
kActionLabels = {
    "None",
    "Toggle HUD",
    "Movement Speed",
    "Action Recovery (retired)",
    "Skip Intro Videos",
    "Third Person",
    "Skip Logos",
    "Skip Warning",
    "FOV Override",
    "Pistol Damage",
    "Melee Damage",
    "Jump Height",
    "Glide Duration",
    "Horse Speed",
    "Horse Sprint Speed",
    "Horse Sprint Duration",
    "Hotstreak Charge",
    "Camera Height Up",
    "Camera Height Down",
    "Camera Zoom Out",
    "Camera Zoom In",
    "Camera Pitch Down",
    "Camera Pitch Up",
    "Camera Rotate Left",
    "Camera Rotate Right",
    "Camera Values Default",
    "TPS Camera Values Default",
    "Fill Wrath (100%)",
    "Fill Ultimate (100%)",
    "Fill Synergy (100%)",
    "Fill all three gauges (100%)",
    "Toggle Unlimited Wrath",
    "Toggle Unlimited Ultimate",
    "Toggle Unlimited Synergy",
    "Toggle Unlimited all gauges",
    "Toggle reduced ammo consumption",
    "Toggle Unlimited ammo",
    "Fill health potions",
    "Toggle potion recharge rate",
    "Toggle Unlimited potions",
    "Fill health",
    "Toggle Unlimited health",
    "Toggle Unlimited glide",
    "Toggle Unlimited horse sprint",
    "Toggle reduced Hotstreak decrease",
    "Toggle Unlimited Hotstreak",
    "Fill Hotstreak",
    "Fill ammunition",
    "Sheathe weapons",
    "Toggle limited Standard ammunition",
    "Toggle grapple range",
    "Cycle infinite double jump",
    "Toggle Ultimate decrease rate"
};

inline constexpr std::array<const wchar_t*, static_cast<std::size_t>(Action::Count)>
kActionTokens = {
    L"None",
    L"ToggleHUD",
    L"MovementSpeed",
    L"ActionRecovery",
    L"SkipIntroVideos",
    L"ThirdPerson",
    L"SkipLogos",
    L"SkipWarning",
    L"FOV",
    L"PistolDamage",
    L"MeleeDamage",
    L"JumpHeight",
    L"GlideDuration",
    L"HorseSpeed",
    L"HorseSprintSpeed",
    L"HorseSprintDuration",
    L"HotstreakCharge",
    L"CameraHeightUp",
    L"CameraHeightDown",
    L"CameraZoomOut",
    L"CameraZoomIn",
    L"CameraPitchDown",
    L"CameraPitchUp",
    L"CameraYawLeft",
    L"CameraYawRight",
    L"CameraReset",
    L"TpsCameraReset",
    L"RefillWrath",
    L"RefillUltimate",
    L"RefillSynergy",
    L"RefillAllGauges",
    L"UnlimitedWrath",
    L"UnlimitedUltimate",
    L"UnlimitedSynergy",
    L"UnlimitedAllGauges",
    L"ReduceAmmo",
    L"UnlimitedAmmo",
    L"RefillPotions",
    L"PotionRecharge",
    L"UnlimitedPotions",
    L"RefillHealth",
    L"UnlimitedHealth",
    L"UnlimitedGlide",
    L"UnlimitedHorseSprint",
    L"ReduceHotstreak",
    L"UnlimitedHotstreak",
    L"RefillHotstreak",
    L"RefillAmmo",
    L"SheatheWeapons",
    L"FiniteStandardAmmo",
    L"GrappleRange",
    L"InfiniteDoubleJump",
    L"UltimateDecreaseRate"
};

// Instant actions share the validated one-shot manual refill queue.
// Values match the manual gauge mask, without including native game code here.
constexpr unsigned GaugeRefillMask(Action action){
    switch(action){
    case Action::RefillWrath:return 1;
    case Action::RefillUltimate:return 2;
    case Action::RefillSynergy:return 4;
    case Action::RefillAllGauges:return 7;
    default:return 0;
    }
}

constexpr unsigned UnlimitedMask(Action action){
    switch(action){
    case Action::UnlimitedWrath:return 1;
    case Action::UnlimitedUltimate:return 2;
    case Action::UnlimitedSynergy:return 4;
    case Action::UnlimitedAllGauges:return 7;
    default:return 0;
    }
}
inline constexpr std::array<int, 8> kDefaultCameraKeys{{
    0x26,0x28,0x21,0x22,0x24,0x23,0x25,0x27
}};
inline constexpr std::array<int, 6> kDefaultTpsKeyboardKeys{{
    'H','Z','Z','E','G',0x09
}};
inline constexpr std::array<Action, 12> kDefaultHotkeys{{
    Action::RefillAllGauges,Action::RefillUltimate,Action::RefillSynergy,
    Action::RefillWrath,Action::UnlimitedAllGauges,Action::UnlimitedUltimate,
    Action::UnlimitedSynergy,Action::UnlimitedWrath,Action::UnlimitedAmmo,
    Action::None,Action::None,Action::ToggleHUD
}};
const char* ActionLabel(Action action);
const wchar_t* ActionToken(Action action);
Action ParseAction(const wchar_t* text);

std::string KeyDisplayName(int vk);
std::wstring KeyTokenFromVK(int vk);
int ParseKeyToken(const wchar_t* text, int fallback);

using LogFn = void(*)(const char*);

class Store {
public:
    bool overlayEnabled = true;
    int menuKey = defaults::MenuKey;
    int graphicsAdapter = 0;
    bool updaterAutoCheck = false;
    bool updaterBeta = defaults::UpdaterBeta; // OFF=main stable, ON=debug beta

    bool toggleHudEnabled = true;
    bool interactionHudScaleEnabled = true;
    float interactionHudScale = 0.65f;
    bool weaponSheathingEnabled = false;
    bool sheathNeverEnabled = true;
    float sheathDelaySeconds = 10.0f;
    bool sheathStrifeMeleeNeverEnabled = false;
    float sheathStrifeMeleeDelaySeconds = 0.0f;
    bool sheathWarNeverEnabled = true;
    float sheathWarDelaySeconds = 10.0f;
    int sheathKeyboardKey = 0;
    int sheathPadButtonA = 0;
    int sheathPadButtonB = 0;
    bool movementSpeedEnabled = true;
    bool skipLogosEnabled = true;
    bool skipIntroEnabled = true;
    bool skipWarningEnabled = true;
    bool thirdPersonEnabled = true;
    bool thirdPersonFovEnabled = true;
    bool tpsLensFocusEnabled = true;
    bool cameraOrbitInputEnabled = true; // derived: controller-only TPS camera
    bool cameraControllerOrbitEnabled = true;
    int tpsCameraStickMode = 2; // 0 OFF, 1 left, 2 right
    bool tpsCutsceneAutoSuspend = true;
    bool tpsHoldSuspendEnabled = true;
    bool tpsLongPressToggleEnabled = true;
    bool tpsGrenadeHoldEnabled = true;
    int aimIndicatorHideMode = 2; // 0 Everywhere, 1 Default Only, 2 TPS Only
    bool tps3DAimEnabled = true;
    float tpsAimPitchOffsetDegrees = 12.0f;
    int tpsMountedAimMode = 0; // 0 camera-forward free, 1 native aim camera locked
    int tpsGrenadeButtonMask = 0x0080; // R3 USE gear (0=OFF)
    int tpsFirePrimaryMask = 0x20000;
    int tpsFireSecondaryMask = 0x0200;
    // TPS-only physical mouse bindings (Win32 virtual-key codes).
    int tpsMousePrimaryButton = 0x01;
    int tpsMouseSecondaryButton = 0x02;
    int tpsWheelPadMask = 0x0100;
    int tpsSuspendButtonMask = 0x0080; // R3, temporary vanilla view
    int tpsSuspendDelayMs = 300; // delay before R3 hold enables vanilla view, 0..3000
    int tpsToggleHoldButtonMask = 0x0040; // L3, long press TPS toggle
    int tpsToggleHoldMs = 500;
    int tpsRecenterButtonMask = 0x0040; // XINPUT_GAMEPAD_LEFT_THUMB
    int horsePadButtonA = 0x0100; // LB + RB are TWO independent simultaneous buttons
    int horsePadButtonB = 0x0200;
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
    bool horseEverywhereEnabled = true;
    int horseAreaMode = 2; // 0 Off, 1 all, 2 restricted-only (default), 3 banned-only
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool fovEnabled = false;
    bool hotstreakChargeEnabled = true;
    // V1.02 research controls. Fail-closed until three native charge owners are verified.
    bool wrathChargeEnabled = true;
    bool ultimateChargeEnabled = true;
    bool ultimateDecreaseEnabled = false;
    float ultimateDurationMultiplier = 2.0f;
    bool synergyChargeEnabled = true;
    bool unlimitedWrathEnabled = false;
    bool unlimitedUltimateEnabled = false;
    bool unlimitedSynergyEnabled = false;
    bool finiteStandardAmmoEnabled = false;
    int standardAmmoCapacity = 200;
    bool grappleRangeEnabled = false;
    float grappleRangeMultiplier = 2.0f;
    int infiniteDoubleJumpMode = 0; // 0 Off, 1 rearm after each glide, 2 Always
    bool reduceAmmoEnabled = defaults::ReduceAmmo;
    bool unlimitedAmmoEnabled = false;
    float ammoConsumptionDivisor = 2.0f;
    bool reduceHotstreakEnabled = false;
    bool unlimitedHotstreakEnabled = false;
    float hotstreakDecreaseDivisor = 2.0f;
    bool potionRechargeEnabled = false;
    bool unlimitedPotionsEnabled = false;
    bool unlimitedHealthEnabled = false;
    bool unlimitedGlideEnabled = false;
    bool unlimitedHorseSprintEnabled = false;
    float potionRechargeMultiplier = 2.0f;

    float movementSpeedMultiplier = 1.50f;
    float pistolDamageMultiplier = 2.00f;
    float meleeDamageMultiplier = 2.00f;
    float jumpHeightMultiplier = 1.25f;
    float glideDurationMultiplier = 10.00f;
    float horseSpeedMultiplier = 1.25f;
    float horseSprintSpeedMultiplier = 1.25f;
    float horseSprintDurationMultiplier = 5.00f;
    float fovDegrees = 90.0f;
    float thirdPersonFovDegrees = 110.0f;
    float thirdPersonCoopFovDegrees = 110.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 0.0f;
    float thirdPersonDistanceMultiplier = 0.15f;
    float thirdPersonPitchDegrees = -12.0f;
    float thirdPersonHeightOffset = 0.0f;
    float tpsUltimateDistanceMultiplier = defaults::UltimateDistance;
    float tpsUltimateHeightOffset = defaults::UltimateHeight;
    float tpsHorseDistanceMultiplier = defaults::HorseDistance;
    float tpsHorseHeightOffset = defaults::HorseHeight;
    float tpsFootAnchorOffset = 0.0f; // adjustable +/-500 units
    float cameraStickSpeed = 180.0f;
    float hotstreakChargeMultiplier = 2.00f;
    float wrathChargeMultiplier = defaults::WrathGain;
    float ultimateChargeMultiplier = defaults::UltimateGain;
    float synergyChargeMultiplier = defaults::SynergyGain;

    std::array<Action, 12> hotkeys{};
    std::array<int, static_cast<size_t>(CameraAction::Count)> cameraKeys{kDefaultCameraKeys};
    std::array<int, static_cast<size_t>(TpsKeyboardAction::Count)> tpsKeyboardKeys{kDefaultTpsKeyboardKeys};

    Store();

    void SetPath(const std::wstring& path);
    void SetLogger(LogFn logger);

    void ResetDefaults(bool persist);
    // Preserve previous INI in timestamped backup, then generate new clean defaults.
    bool ResetIniFileToDefaults();
    void ResetVanillaCameraValues();
    void ResetTpsCameraValues();
    bool Load();

    // Publish immediately to runtime atomics and mark disk state dirty.
    void Save();

    // Persist the whole INI immediately.
    bool SaveNow();

    // Persist after a 500 ms quiet period, or immediately when force=true.
    void FlushIfDue(bool force = false);

    void PublishRuntime() const;
    bool IsDirty() const;

private:
    std::wstring path_;
    LogFn logger_ = nullptr;
    bool saveFailureLogged_ = false;
    std::atomic_bool dirty_{false};
    std::atomic_ullong dirtyTick_{0};

    void Log(const char* text) const;
    void MarkDirty();
};

} // namespace dg::config
