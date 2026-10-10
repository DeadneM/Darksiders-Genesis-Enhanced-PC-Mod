#pragma once

#include <array>
#include <atomic>
#include <string>

namespace dg::config {
enum class CameraAction : int { HeightUp = 0, HeightDown, ZoomOut, ZoomIn, PitchDown, PitchUp, YawLeft, YawRight, Count };
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
    bool thirdPersonEnabled = false;
    bool thirdPersonFovEnabled = true;
    bool tpsFollowPlayer = true;
    bool tpsSuppressNativeRightStick = true;
    bool cameraOrbitInputEnabled = true; // derived: mouse OR controller
    bool cameraMouseOrbitEnabled = true;
    bool cameraControllerOrbitEnabled = true;
    // V0.71: independently gated experimental native strafe
    bool tpsMouseKeyboardStrafe = false;
    bool tpsControllerStrafe = false;
    int tpsRecenterButtonMask = 0x0040; // XINPUT_GAMEPAD_LEFT_THUMB
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
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
    float thirdPersonFovDegrees = 90.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 0.0f;
    float thirdPersonDistanceMultiplier = 0.50f;
    float thirdPersonPitchDegrees = -12.0f;
    float thirdPersonHeightOffset = 180.0f;
    float tpsFootAnchorOffset = 88.0f; // estimated capsule center-to-foot, configurable
    float cameraMouseSensitivity = 0.12f;
    float cameraStickSpeed = 135.0f;
    float hotstreakChargeMultiplier = 2.00f;

    std::array<Action, 12> hotkeys{};
    std::array<int, static_cast<size_t>(CameraAction::Count)> cameraKeys{{0x26,0x28,0x25,0x27,0,0,0x64,0x66}};

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
