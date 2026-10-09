#include <windows.h>

#include "ConfigStore.h"
#include "RuntimeSettings.h"

#include <cwchar>
#include <algorithm>

namespace dg::config {
namespace {

constexpr int kConfigRevision = 2101;

bool ReadBool(
    const wchar_t* section,
    const wchar_t* key,
    bool fallback,
    const std::wstring& path) {
    return GetPrivateProfileIntW(
        section,
        key,
        fallback ? 1 : 0,
        path.c_str()
    ) != 0;
}

float ReadFloat(
    const wchar_t* section,
    const wchar_t* key,
    float fallback,
    const std::wstring& path) {
    wchar_t buffer[64]{};
    wchar_t fallbackText[64]{};
    swprintf_s(fallbackText, L"%.3f", fallback);

    GetPrivateProfileStringW(
        section,
        key,
        fallbackText,
        buffer,
        64,
        path.c_str()
    );

    wchar_t* end = nullptr;
    const float value = wcstof(buffer, &end);
    return (end && end != buffer) ? value : fallback;
}

void WriteBool(
    const wchar_t* section,
    const wchar_t* key,
    bool value,
    const std::wstring& path) {
    WritePrivateProfileStringW(
        section,
        key,
        value ? L"1" : L"0",
        path.c_str()
    );
}

void WriteFloat(
    const wchar_t* section,
    const wchar_t* key,
    float value,
    const std::wstring& path) {
    wchar_t buffer[64]{};
    swprintf_s(buffer, L"%.3f", value);
    WritePrivateProfileStringW(
        section,
        key,
        buffer,
        path.c_str()
    );
}

bool IsExtendedVirtualKey(int vk) {
    switch (vk) {
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_LEFT:
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
    case VK_DIVIDE:
    case VK_NUMLOCK:
        return true;
    default:
        return false;
    }
}

} // namespace

const char* ActionLabel(Action action) {
    const int i = static_cast<int>(action);
    if (i < 0 || i >= static_cast<int>(Action::Count)) {
        return "None";
    }
    return kActionLabels[static_cast<std::size_t>(i)];
}

const wchar_t* ActionToken(Action action) {
    const int i = static_cast<int>(action);
    if (i < 0 || i >= static_cast<int>(Action::Count)) {
        return L"None";
    }
    return kActionTokens[static_cast<std::size_t>(i)];
}

Action ParseAction(const wchar_t* text) {
    if (!text) {
        return Action::None;
    }

    for (int i = 0; i < static_cast<int>(Action::Count); ++i) {
        if (_wcsicmp(
                text,
                kActionTokens[static_cast<std::size_t>(i)]) == 0) {
            return static_cast<Action>(i);
        }
    }

    return Action::None;
}

std::string KeyDisplayName(int vk) {
    if (vk <= 0 || vk >= 256) {
        return "Unbound";
    }

    UINT scan = MapVirtualKeyA(
        static_cast<UINT>(vk),
        MAPVK_VK_TO_VSC
    );
    LONG keyData = static_cast<LONG>(scan << 16);
    if (IsExtendedVirtualKey(vk)) {
        keyData |= (1 << 24);
    }

    char name[64]{};
    if (GetKeyNameTextA(
            keyData,
            name,
            static_cast<int>(sizeof(name))) > 0) {
        return name;
    }

    char fallback[16]{};
    sprintf_s(fallback, sizeof(fallback), "VK_%02X", vk & 0xFF);
    return fallback;
}

std::wstring KeyTokenFromVK(int vk) {
    switch (vk) {
    case 0: return L"None";
    case VK_UP: return L"Up";
    case VK_DOWN: return L"Down";
    case VK_LEFT: return L"Left";
    case VK_RIGHT: return L"Right";
    case VK_INSERT: return L"Insert";
    case VK_DELETE: return L"Delete";
    case VK_HOME: return L"Home";
    case VK_END: return L"End";
    case VK_PRIOR: return L"PageUp";
    case VK_NEXT: return L"PageDown";
    case VK_TAB: return L"Tab";
    case VK_CAPITAL: return L"CapsLock";
    case VK_PAUSE: return L"Pause";
    case VK_SCROLL: return L"ScrollLock";
    case VK_SPACE: return L"Space";
    default:
        break;
    }

    if (vk >= VK_F1 && vk <= VK_F24) {
        wchar_t text[16]{};
        swprintf_s(text, L"F%d", (vk - VK_F1) + 1);
        return text;
    }

    if ((vk >= '0' && vk <= '9') ||
        (vk >= 'A' && vk <= 'Z')) {
        wchar_t text[2]{
            static_cast<wchar_t>(vk),
            L'\0'
        };
        return text;
    }

    wchar_t fallback[16]{};
    swprintf_s(fallback, L"VK_%02X", vk & 0xFF);
    return fallback;
}

int ParseKeyToken(const wchar_t* text, int fallback) {
    if (!text || !*text) {
        return fallback;
    }

    struct NamedKey {
        const wchar_t* name;
        int vk;
    };

    constexpr NamedKey named[] = {
        {L"None", 0},
        {L"Up", VK_UP}, {L"Down", VK_DOWN},
        {L"Left", VK_LEFT}, {L"Right", VK_RIGHT},
        {L"Insert", VK_INSERT},
        {L"Delete", VK_DELETE},
        {L"Home", VK_HOME},
        {L"End", VK_END},
        {L"PageUp", VK_PRIOR},
        {L"PageDown", VK_NEXT},
        {L"Tab", VK_TAB},
        {L"CapsLock", VK_CAPITAL},
        {L"Pause", VK_PAUSE},
        {L"ScrollLock", VK_SCROLL},
        {L"Space", VK_SPACE}
    };

    for (const auto& entry : named) {
        if (_wcsicmp(text, entry.name) == 0) {
            return entry.vk;
        }
    }

    if ((text[0] == L'F' || text[0] == L'f') && text[1]) {
        const int n = _wtoi(text + 1);
        if (n >= 1 && n <= 24) {
            return VK_F1 + (n - 1);
        }
    }

    if (text[0] && !text[1]) {
        wchar_t ch = text[0];
        if (ch >= L'a' && ch <= L'z') {
            ch = static_cast<wchar_t>(ch - L'a' + L'A');
        }
        if ((ch >= L'0' && ch <= L'9') ||
            (ch >= L'A' && ch <= L'Z')) {
            return static_cast<int>(ch);
        }
    }

    if (_wcsnicmp(text, L"VK_", 3) == 0 && text[3]) {
        wchar_t* end = nullptr;
        const long value = wcstol(text + 3, &end, 16);
        if (end != text + 3 && value > 0 && value < 256) {
            return static_cast<int>(value);
        }
    }

    return fallback;
}

Store::Store() {
    // Member initializers already carry the scalar defaults. Keep global
    // construction side-effect free and initialize only the hotkey table here.
    hotkeys.fill(Action::None);
    hotkeys[0] = Action::ToggleHUD;
    hotkeys[1] = Action::MovementSpeed;
    hotkeys[2] = Action::ActionRecovery;
    hotkeys[3] = Action::SkipIntroVideos;
    hotkeys[4] = Action::ReticleFocusTest;
    hotkeys[5] = Action::ToggleReticle;
    cameraKeys = {{VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0, 0, VK_NUMPAD4, VK_NUMPAD6}};
}

void Store::SetPath(const std::wstring& path) {
    path_ = path;
}

void Store::SetLogger(LogFn logger) {
    logger_ = logger;
}

void Store::Log(const char* text) const {
    if (logger_ && text) {
        logger_(text);
    }
}

void Store::ResetDefaults(bool persist) {
    overlayEnabled = true;
    menuKey = VK_INSERT;
    graphicsAdapter = 0;

    toggleHudEnabled = true;
    movementSpeedEnabled = true;
    actionRecoveryEnabled = true;
    skipLogosEnabled = true;
    skipIntroEnabled = true;
    thirdPersonEnabled = false;
    pistolDamageEnabled = true;
    meleeDamageEnabled = true;
    jumpHeightEnabled = true;
    glideDurationEnabled = true;
    horseSpeedEnabled = true;
    horseSprintSpeedEnabled = true;
    horseSprintDurationEnabled = true;
    fovEnabled = false;
    hideReticle = false;
    crossCursorTestMode = 0;
    hotstreakChargeEnabled = true;

    movementSpeedMultiplier = 1.50f;
    actionRecoveryDelayMs = 0.0f;
    actionRecoveryMultiplier = 2.00f;
    dodgeEarlyUnlockMs = 100.0f;
    pistolDamageMultiplier = 2.00f;
    meleeDamageMultiplier = 2.00f;
    jumpHeightMultiplier = 1.25f;
    glideDurationMultiplier = 10.00f;
    horseSpeedMultiplier = 1.25f;
    horseSprintSpeedMultiplier = 1.25f;
    horseSprintDurationMultiplier = 5.00f;
    fovDegrees = 90.0f;
    cameraZoomPercent = 0.0f;
    cameraPitchDegrees = 0.0f;
    cameraYawDegrees = 0.0f;
    cameraHeightOffset = 0.0f;
    thirdPersonDistanceMultiplier = 1.00f;
    hotstreakChargeMultiplier = 2.00f;

    hotkeys.fill(Action::None);
    hotkeys[0] = Action::ToggleHUD;
    hotkeys[1] = Action::MovementSpeed;
    hotkeys[2] = Action::ActionRecovery;
    hotkeys[3] = Action::SkipIntroVideos;
    hotkeys[4] = Action::ReticleFocusTest;
    hotkeys[5] = Action::ToggleReticle;
    cameraKeys = {{VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0, 0, VK_NUMPAD4, VK_NUMPAD6}};

    PublishRuntime();

    if (persist) {
        SaveNow();
    }
}

bool Store::Load() {
    if (path_.empty()) {
        return false;
    }

    if (GetFileAttributesW(path_.c_str()) ==
        INVALID_FILE_ATTRIBUTES) {
        ResetDefaults(false);
        SaveNow();
        Log("INI not found -> wrote authoritative defaults");
        return true;
    }

    overlayEnabled =
        ReadBool(L"Overlay", L"Enabled", true, path_);

    wchar_t menuKeyText[64]{};
    GetPrivateProfileStringW(
        L"Overlay",
        L"MenuKey",
        L"Insert",
        menuKeyText,
        64,
        path_.c_str()
    );
    menuKey = ParseKeyToken(menuKeyText, VK_INSERT);

    graphicsAdapter = GetPrivateProfileIntW(
        L"System",
        L"GraphicsAdapter",
        0,
        path_.c_str()
    );
    if (graphicsAdapter < 0) {
        graphicsAdapter = 0;
    }
    if (graphicsAdapter > 4) {
        graphicsAdapter = 4;
    }

    toggleHudEnabled =
        ReadBool(L"Features", L"ToggleHUD", true, path_);
    movementSpeedEnabled =
        ReadBool(L"Features", L"MovementSpeed", true, path_);
    actionRecoveryEnabled =
        ReadBool(L"Features", L"ActionRecovery", true, path_);
    skipLogosEnabled =
        ReadBool(L"Features", L"SkipLogos", true, path_);
    skipIntroEnabled =
        ReadBool(L"Features", L"SkipIntroVideos", true, path_);
    thirdPersonEnabled =
        ReadBool(L"Features", L"ThirdPerson", false, path_);
    pistolDamageEnabled =
        ReadBool(L"Features", L"PistolDamage", true, path_);
    meleeDamageEnabled =
        ReadBool(L"Features", L"MeleeDamage", true, path_);
    jumpHeightEnabled =
        ReadBool(L"Features", L"JumpHeight", true, path_);
    glideDurationEnabled =
        ReadBool(L"Features", L"GlideDuration", true, path_);
    horseSpeedEnabled =
        ReadBool(L"Features", L"HorseSpeed", true, path_);
    horseSprintSpeedEnabled =
        ReadBool(L"Features", L"HorseSprintSpeed", true, path_);
    horseSprintDurationEnabled =
        ReadBool(L"Features", L"HorseSprintDuration", true, path_);
    fovEnabled =
        ReadBool(L"Features", L"FOV", false, path_);
    hideReticle =
        ReadBool(L"Features", L"HideReticle", false, path_);
    crossCursorTestMode = std::clamp(
        static_cast<int>(GetPrivateProfileIntW(L"Features", L"CrossCursorTestMode", 0, path_.c_str())),0,2);
    hotstreakChargeEnabled =
        ReadBool(L"Features", L"HotstreakCharge", true, path_);

    movementSpeedMultiplier =
        ReadFloat(L"Values", L"MovementSpeedMultiplier", 1.50f, path_);
    actionRecoveryDelayMs =
        ReadFloat(L"Values", L"ActionRecoveryDelayMs", 0.0f, path_);
    actionRecoveryMultiplier =
        ReadFloat(L"Values", L"ActionRecoveryMultiplier", 2.0f, path_);
    dodgeEarlyUnlockMs =
        ReadFloat(L"Values", L"DodgeEarlyUnlockMs", 100.0f, path_);
    pistolDamageMultiplier =
        ReadFloat(L"Values", L"PistolDamageMultiplier", 2.0f, path_);
    meleeDamageMultiplier =
        ReadFloat(L"Values", L"MeleeDamageMultiplier", 2.0f, path_);
    jumpHeightMultiplier =
        ReadFloat(L"Values", L"JumpHeightMultiplier", 1.25f, path_);
    glideDurationMultiplier =
        ReadFloat(L"Values", L"GlideDurationMultiplier", 10.0f, path_);
    horseSpeedMultiplier =
        ReadFloat(L"Values", L"HorseSpeedMultiplier", 1.25f, path_);
    horseSprintSpeedMultiplier =
        ReadFloat(L"Values", L"HorseSprintSpeedMultiplier", 1.25f, path_);
    horseSprintDurationMultiplier =
        ReadFloat(L"Values", L"HorseSprintDurationMultiplier", 5.0f, path_);
    fovDegrees =
        ReadFloat(L"Values", L"FOVDegrees", 90.0f, path_);
    cameraZoomPercent =
        ReadFloat(L"Values", L"CameraZoomPercent", 0.0f, path_);
    cameraPitchDegrees =
        ReadFloat(L"Values", L"CameraPitchDegrees", 0.0f, path_);
    cameraYawDegrees =
        ReadFloat(L"Values", L"CameraYawDegrees", 0.0f, path_);
    cameraHeightOffset =
        ReadFloat(L"Values", L"CameraHeightOffset", 0.0f, path_);
    thirdPersonDistanceMultiplier =
        ReadFloat(L"Values", L"ThirdPersonDistanceMultiplier", 1.0f, path_);
    hotstreakChargeMultiplier =
        ReadFloat(L"Values", L"HotstreakChargeMultiplier", 2.0f, path_);

    for (int i = 0; i < 12; ++i) {
        wchar_t key[8]{};
        swprintf_s(key, L"F%d", i + 1);

        wchar_t value[64]{};
        GetPrivateProfileStringW(
            L"Hotkeys",
            key,
            ActionToken(hotkeys[static_cast<std::size_t>(i)]),
            value,
            64,
            path_.c_str()
        );

        hotkeys[static_cast<std::size_t>(i)] =
            ParseAction(value);
    }

    for (size_t i = 0; i < cameraKeys.size(); ++i) {
        wchar_t keyText[64]{};
        const std::wstring fallback = KeyTokenFromVK(cameraKeys[i]);
        GetPrivateProfileStringW(
            L"CameraHotkeys", kCameraTokens[i], fallback.c_str(),
            keyText, 64, path_.c_str());
        const int parsed = ParseKeyToken(keyText, cameraKeys[i]);
        cameraKeys[i] = (parsed >= 0 && parsed < 256) ? parsed : 0;
    }

    const int revision = GetPrivateProfileIntW(
        L"Meta",
        L"ConfigRevision",
        0,
        path_.c_str()
    );

    if (revision < kConfigRevision) {
        // V0.18D migration: previous test runs could persist Skip Intro OFF.
        // Reset only the two startup-skip defaults once, then preserve all
        // future user choices normally.
        skipLogosEnabled = true;
        skipIntroEnabled = true;
        horseSprintSpeedEnabled = true;
        SaveNow();
        Log("INI migrated V0.21 -> startup skips ON, Horse Sprint Speed ON");
        return true;
    }

    PublishRuntime();
    dirty_.store(false, std::memory_order_relaxed);
    Log("INI loaded");
    return true;
}

void Store::PublishRuntime() const {
    dg::runtime::Snapshot runtime{};
    runtime.toggleHudEnabled = toggleHudEnabled;
    runtime.hideReticle = hideReticle;
    runtime.crossCursorTestMode = crossCursorTestMode;
    runtime.fovEnabled = fovEnabled;
    runtime.movementSpeedEnabled = movementSpeedEnabled;
    runtime.actionRecoveryEnabled = actionRecoveryEnabled;
    runtime.skipLogosEnabled = skipLogosEnabled;
    runtime.skipIntroEnabled = skipIntroEnabled;
    runtime.pistolDamageEnabled = pistolDamageEnabled;
    runtime.meleeDamageEnabled = meleeDamageEnabled;
    runtime.jumpHeightEnabled = jumpHeightEnabled;
    runtime.glideDurationEnabled = glideDurationEnabled;
    runtime.horseSpeedEnabled = horseSpeedEnabled;
    runtime.horseSprintSpeedEnabled = horseSprintSpeedEnabled;
    runtime.horseSprintDurationEnabled = horseSprintDurationEnabled;
    runtime.hotstreakChargeEnabled = hotstreakChargeEnabled;

    runtime.fovDegrees = fovDegrees;
    runtime.cameraZoomPercent = cameraZoomPercent;
    runtime.cameraPitchDegrees = cameraPitchDegrees;
    runtime.cameraYawDegrees = cameraYawDegrees;
    runtime.cameraHeightOffset = cameraHeightOffset;
    runtime.movementSpeedMultiplier = movementSpeedMultiplier;
    runtime.actionRecoveryDelayMs = actionRecoveryDelayMs;
    runtime.pistolDamageMultiplier = pistolDamageMultiplier;
    runtime.meleeDamageMultiplier = meleeDamageMultiplier;
    runtime.jumpHeightMultiplier = jumpHeightMultiplier;
    runtime.glideDurationMultiplier = glideDurationMultiplier;
    runtime.horseSpeedMultiplier = horseSpeedMultiplier;
    runtime.horseSprintSpeedMultiplier = horseSprintSpeedMultiplier;
    runtime.horseSprintDurationMultiplier = horseSprintDurationMultiplier;
    runtime.hotstreakChargeMultiplier = hotstreakChargeMultiplier;

    dg::runtime::Publish(runtime);
}

void Store::MarkDirty() {
    dirty_.store(true, std::memory_order_relaxed);
    dirtyTick_.store(GetTickCount64(), std::memory_order_relaxed);
}

void Store::Save() {
    PublishRuntime();
    MarkDirty();
}

bool Store::SaveNow() {
    if (path_.empty()) {
        return false;
    }

    PublishRuntime();

    WritePrivateProfileStringW(
        L"Meta",
        L"ConfigRevision",
        L"2101",
        path_.c_str()
    );

    WriteBool(L"Overlay", L"Enabled", overlayEnabled, path_);
    const std::wstring menuKeyToken = KeyTokenFromVK(menuKey);
    WritePrivateProfileStringW(
        L"Overlay",
        L"MenuKey",
        menuKeyToken.c_str(),
        path_.c_str()
    );

    wchar_t graphicsAdapterText[8]{};
    swprintf_s(
        graphicsAdapterText,
        L"%d",
        graphicsAdapter
    );
    WritePrivateProfileStringW(
        L"System",
        L"GraphicsAdapter",
        graphicsAdapterText,
        path_.c_str()
    );

    WriteBool(L"Features", L"ToggleHUD", toggleHudEnabled, path_);
    WriteBool(L"Features", L"MovementSpeed", movementSpeedEnabled, path_);
    WriteBool(L"Features", L"ActionRecovery", actionRecoveryEnabled, path_);
    WriteBool(L"Features", L"SkipLogos", skipLogosEnabled, path_);
    WriteBool(L"Features", L"SkipIntroVideos", skipIntroEnabled, path_);
    WriteBool(L"Features", L"ThirdPerson", thirdPersonEnabled, path_);
    WriteBool(L"Features", L"PistolDamage", pistolDamageEnabled, path_);
    WriteBool(L"Features", L"MeleeDamage", meleeDamageEnabled, path_);
    WriteBool(L"Features", L"JumpHeight", jumpHeightEnabled, path_);
    WriteBool(L"Features", L"GlideDuration", glideDurationEnabled, path_);
    WriteBool(L"Features", L"HorseSpeed", horseSpeedEnabled, path_);
    WriteBool(L"Features", L"HorseSprintSpeed", horseSprintSpeedEnabled, path_);
    WriteBool(L"Features", L"HorseSprintDuration", horseSprintDurationEnabled, path_);
    WriteBool(L"Features", L"FOV", fovEnabled, path_);
    WriteBool(L"Features", L"HideReticle", hideReticle, path_);
    {
        wchar_t mode[8]{};
        swprintf_s(mode,L"%d",std::clamp(crossCursorTestMode,0,2));
        WritePrivateProfileStringW(L"Features",L"CrossCursorTestMode",mode,path_.c_str());
    }
    WriteBool(L"Features", L"HotstreakCharge", hotstreakChargeEnabled, path_);

    WriteFloat(L"Values", L"MovementSpeedMultiplier", movementSpeedMultiplier, path_);
    WriteFloat(L"Values", L"ActionRecoveryDelayMs", actionRecoveryDelayMs, path_);
    WriteFloat(L"Values", L"ActionRecoveryMultiplier", actionRecoveryMultiplier, path_);
    WriteFloat(L"Values", L"DodgeEarlyUnlockMs", dodgeEarlyUnlockMs, path_);
    WriteFloat(L"Values", L"PistolDamageMultiplier", pistolDamageMultiplier, path_);
    WriteFloat(L"Values", L"MeleeDamageMultiplier", meleeDamageMultiplier, path_);
    WriteFloat(L"Values", L"JumpHeightMultiplier", jumpHeightMultiplier, path_);
    WriteFloat(L"Values", L"GlideDurationMultiplier", glideDurationMultiplier, path_);
    WriteFloat(L"Values", L"HorseSpeedMultiplier", horseSpeedMultiplier, path_);
    WriteFloat(L"Values", L"HorseSprintSpeedMultiplier", horseSprintSpeedMultiplier, path_);
    WriteFloat(L"Values", L"HorseSprintDurationMultiplier", horseSprintDurationMultiplier, path_);
    WriteFloat(L"Values", L"FOVDegrees", fovDegrees, path_);
    WriteFloat(L"Values", L"CameraZoomPercent", cameraZoomPercent, path_);
    WriteFloat(L"Values", L"CameraPitchDegrees", cameraPitchDegrees, path_);
    WriteFloat(L"Values", L"CameraYawDegrees", cameraYawDegrees, path_);
    WriteFloat(L"Values", L"CameraHeightOffset", cameraHeightOffset, path_);
    WriteFloat(L"Values", L"ThirdPersonDistanceMultiplier", thirdPersonDistanceMultiplier, path_);
    WriteFloat(L"Values", L"HotstreakChargeMultiplier", hotstreakChargeMultiplier, path_);

    for (int i = 0; i < 12; ++i) {
        wchar_t key[8]{};
        swprintf_s(key, L"F%d", i + 1);
        WritePrivateProfileStringW(
            L"Hotkeys",
            key,
            ActionToken(hotkeys[static_cast<std::size_t>(i)]),
            path_.c_str()
        );
    }

    for (size_t i = 0; i < cameraKeys.size(); ++i) {
        const std::wstring value = KeyTokenFromVK(cameraKeys[i]);
        WritePrivateProfileStringW(
            L"CameraHotkeys", kCameraTokens[i], value.c_str(), path_.c_str());
    }

    dirty_.store(false, std::memory_order_relaxed);
    Log("INI saved");
    return true;
}

void Store::FlushIfDue(bool force) {
    if (!dirty_.load(std::memory_order_relaxed)) {
        return;
    }

    const ULONGLONG now = GetTickCount64();
    const ULONGLONG dirtyAt =
        dirtyTick_.load(std::memory_order_relaxed);

    if (!force && now - dirtyAt < 500) {
        return;
    }

    SaveNow();
}

bool Store::IsDirty() const {
    return dirty_.load(std::memory_order_relaxed);
}

} // namespace dg::config
