#pragma once

#include <array>
#include <atomic>
#include <string>

namespace dg::config {
enum class CameraAction : int { HeightUp = 0, HeightDown, ZoomOut, ZoomIn, PitchDown, PitchUp, YawLeft, YawRight, Count };
// New V0.92 TPS keyboard actions have the same semantics as the existing
// controller actions. OFF (0) is intentional for legacy compatibility.
enum class TpsKeyboardAction : int { Horse = 0, Toggle, Recenter, Suspend, Count };
inline constexpr std::array<const char*, 4> kTpsKeyboardLabels = {
    "Summon / dismount horse", "Long press TPS ON/OFF",
    "Recenter TPS camera", "Hold temporary vanilla view"
};
inline constexpr std::array<const wchar_t*, 4> kTpsKeyboardTokens = {
    L"Horse", L"ToggleTPS", L"RecenterTPS", L"TemporaryView"
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
    ToggleReticle,
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
    Count
};

inline constexpr std::array<const char*, static_cast<std::size_t>(Action::Count)>
kActionLabels = {
    "None",
    "Toggle HUD",
    "Movement Speed",
    "Action Recovery",
    "Skip Intro Videos",
    "Third Person",
    "Toggle Reticle",
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
    "Camera Reset"
};

inline constexpr std::array<const wchar_t*, static_cast<std::size_t>(Action::Count)>
kActionTokens = {
    L"None",
    L"ToggleHUD",
    L"MovementSpeed",
    L"ActionRecovery",
    L"SkipIntroVideos",
    L"ThirdPerson",
    L"ToggleReticle",
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
    L"CameraReset"
};

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
    int menuKey = 0x2D; // VK_INSERT
    int graphicsAdapter = 0;

    bool toggleHudEnabled = true;
    bool movementSpeedEnabled = true;
    bool actionRecoveryEnabled = true;
    bool skipLogosEnabled = true;
    bool skipIntroEnabled = true;
    bool skipWarningEnabled = true;
    bool thirdPersonEnabled = true;
    bool thirdPersonFovEnabled = true;
    bool tpsFollowPlayer = true;
    bool cameraOrbitInputEnabled = true; // derived: mouse OR controller
    bool cameraMouseOrbitEnabled = true;
    bool cameraControllerOrbitEnabled = true;
    bool tpsMouseCenterAimEnabled = true; // V0.95 reversible mouse targeting test
    // V0.75: mouse TPS firing always faces the camera; controller has
    // independent movement-facing and camera orbit options.
    bool tpsHoldSuspendEnabled = true;
    bool tpsLongPressToggleEnabled = true;
    bool tpsGrenadeHoldEnabled = true;
    bool tpsHideGroundAimFx = true; // legacy diagnostic toggle, no longer in UI
    int aimIndicatorHideMode = 2; // 0 Everywhere, 1 Default Only, 2 TPS Only
    bool tpsHorseStableAimCamera = true; // legacy setting migrated to mounted aim mode
    int tpsMountedAimMode = 0; // 0 camera-forward free, 1 native aim camera locked
    int tpsGrenadeButtonMask = 0x0080; // R3 (0 = OFF)
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
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool fovEnabled = false;
    bool hideReticle = false;
    bool hotstreakChargeEnabled = true;

    float movementSpeedMultiplier = 1.50f;
    float actionRecoveryDelayMs = 0.0f;
    float actionRecoveryMultiplier = 2.00f;
    float dodgeEarlyUnlockMs = 100.0f;
    float pistolDamageMultiplier = 2.00f;
    float meleeDamageMultiplier = 2.00f;
    float jumpHeightMultiplier = 1.25f;
    float glideDurationMultiplier = 10.00f;
    float horseSpeedMultiplier = 1.25f;
    float horseSprintSpeedMultiplier = 1.25f;
    float horseSprintDurationMultiplier = 5.00f;
    float fovDegrees = 90.0f;
    float thirdPersonFovDegrees = 110.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 0.0f;
    float thirdPersonDistanceMultiplier = 0.15f;
    float thirdPersonPitchDegrees = 0.0f;
    float thirdPersonHeightOffset = 100.0f;
    float tpsFootAnchorOffset = 0.0f; // adjustable +/-500 units
    float cameraMouseSensitivity = 0.12f;
    float cameraStickSpeed = 180.0f;
    float hotstreakChargeMultiplier = 2.00f;

    std::array<Action, 12> hotkeys{};
    std::array<int, static_cast<size_t>(CameraAction::Count)> cameraKeys{{0x26,0x28,0x25,0x27,0,0,0x64,0x66}};
    std::array<int, static_cast<size_t>(TpsKeyboardAction::Count)> tpsKeyboardKeys{{'H','Z','Z','E'}};

    Store();

    void SetPath(const std::wstring& path);
    void SetLogger(LogFn logger);

    void ResetDefaults(bool persist);
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
    std::atomic_bool dirty_{false};
    std::atomic_ullong dirtyTick_{0};

    void Log(const char* text) const;
    void MarkDirty();
};

} // namespace dg::config
