#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include <MinHook.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "HorseFeature.h"
#include "PlayerIdentity.h"
#include "TargetValidator.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <array>
#include <atomic>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <cmath>
#include <string>
#include <cstdlib>

namespace {

constexpr const char* kBuild = "0.17-core-cleanup-test";
constexpr const wchar_t* kIniName = L"DarksidersGenesisMod.ini";
constexpr const wchar_t* kLogName = L"DarksidersGenesisMod.log";

HMODULE g_module = nullptr;
std::wstring g_iniPath;
std::wstring g_logPath;

using PresentFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
using ResizeBuffersFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
using HudHiddenGetterFn = bool(*)();
using CharacterGetMaxSpeedFn = float(*)(void*);
using AbilityActionEnabledFn = bool(*)(void*, unsigned char);
using AddJuiceFn = void(*)(void*, float);
using FilterOutgoingDamageFn = void(*)(void*, void*);

PresentFn g_originalPresent = nullptr;
ResizeBuffersFn g_originalResizeBuffers = nullptr;
HudHiddenGetterFn g_originalHudHiddenGetter = nullptr;
CharacterGetMaxSpeedFn g_originalCharacterGetMaxSpeed = nullptr;
void* g_playerGetMaxSpeedTarget = nullptr;
AbilityActionEnabledFn g_originalAbilityActionEnabled = nullptr;
AddJuiceFn g_originalAddJuice = nullptr;
FilterOutgoingDamageFn g_originalFilterOutgoingDamage = nullptr;

ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;
IDXGISwapChain* g_gameSwapChain = nullptr;
HWND g_hwnd = nullptr;
WNDPROC g_originalWndProc = nullptr;

std::atomic_bool g_imguiReady{false};
std::atomic_bool g_overlayVisible{false};
std::atomic_bool g_captureMenuKey{false};
std::atomic_int g_capturedMenuKey{0};
std::atomic_bool g_hudHidden{false};
std::atomic_bool g_hudHookReady{false};
std::atomic_bool g_movementHookReady{false};
std::atomic_bool g_recoveryHookReady{false};
std::atomic_bool g_skipIntroReady{false};
std::atomic_bool g_shuttingDown{false};
std::atomic_bool g_gameplayHooksAllowed{false};
dg::target::ValidationResult g_targetValidation{};
LONG** g_skipIntroDataSlot = nullptr;
LONG* g_skipIntroData = nullptr;
LONG g_skipIntroOriginalValue = 1;
std::atomic_bool g_hotstreakHookReady{false};
std::atomic_int g_hotstreakBoostCalls{0};
std::atomic<float> g_lastNativeJuiceGain{0.0f};
std::atomic<float> g_lastBoostedJuiceGain{0.0f};
std::atomic_int g_pistolDamageBoostCalls{0};
std::atomic<float> g_lastNativePistolDamage{0.0f};
std::atomic<float> g_lastBoostedPistolDamage{0.0f};
std::atomic<float> g_lastPistolBaseJuice{0.0f};
std::atomic_int g_meleeDamageBoostCalls{0};
std::atomic<float> g_lastNativeBaseDamage{0.0f};
std::atomic<float> g_lastBoostedBaseDamage{0.0f};
std::atomic_bool g_finalOutgoingDamageHookReady{false};
std::atomic_uint g_lastOutgoingScaleType{0};
std::atomic_int g_lastOutgoingTagCount{0};
std::atomic<void*> g_localPlayerCharacter{nullptr};
std::atomic_int g_actionMoveQueries{0};
std::atomic_int g_actionMoveLocalQueries{0};
std::atomic_int g_actionMoveNativeBlocked{0};
std::atomic_int g_actionMoveForced{0};
std::atomic<void*> g_lastActionMoveAbility{nullptr};
std::atomic_int g_lastActionMoveState{-1};
std::atomic<float> g_lastActionMoveElapsed{0.0f};
std::atomic<void*> g_recoveryTailAbility{nullptr};
std::atomic<float> g_recoveryTailStartElapsed{0.0f};

std::atomic_int g_movementCaptureRejectLogBudget{12};

struct PlayerMovementTuningState {
    void* component = nullptr;
    float jumpZVelocity = 0.0f;
    float doubleJumpZVelocity = 0.0f;
    float glideDurationSeconds = 0.0f;
};


SRWLOCK g_tuningLock = SRWLOCK_INIT;
std::array<PlayerMovementTuningState, 4> g_playerMovementStates{};

std::array<bool, 256> g_keyDown{};
std::string g_lastAction = "None";

enum class Action : int {
    None = 0,
    ToggleHUD,
    MovementSpeed,
    ActionRecovery,
    SkipIntroVideos,
    ThirdPerson,
    Count
};

constexpr std::array<const char*, static_cast<size_t>(Action::Count)> kActionLabels = {
    "None",
    "Toggle HUD",
    "Movement Speed",
    "Action Recovery",
    "Skip Intro Videos",
    "Third Person"
};

constexpr std::array<const wchar_t*, static_cast<size_t>(Action::Count)> kActionTokens = {
    L"None",
    L"ToggleHUD",
    L"MovementSpeed",
    L"ActionRecovery",
    L"SkipIntroVideos",
    L"ThirdPerson"
};

const char* ActionLabel(Action action) {
    const int i = static_cast<int>(action);
    if (i < 0 || i >= static_cast<int>(Action::Count)) {
        return "None";
    }
    return kActionLabels[static_cast<size_t>(i)];
}

const wchar_t* ActionToken(Action action) {
    const int i = static_cast<int>(action);
    if (i < 0 || i >= static_cast<int>(Action::Count)) {
        return L"None";
    }
    return kActionTokens[static_cast<size_t>(i)];
}

Action ParseAction(const wchar_t* text) {
    if (!text) {
        return Action::None;
    }
    for (int i = 0; i < static_cast<int>(Action::Count); ++i) {
        if (_wcsicmp(text, kActionTokens[static_cast<size_t>(i)]) == 0) {
            return static_cast<Action>(i);
        }
    }
    return Action::None;
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

std::string KeyDisplayName(int vk) {
    if (vk <= 0 || vk >= 256) {
        return "Unbound";
    }

    UINT scan = MapVirtualKeyA(static_cast<UINT>(vk), MAPVK_VK_TO_VSC);
    LONG keyData = static_cast<LONG>(scan << 16);
    if (IsExtendedVirtualKey(vk)) {
        keyData |= (1 << 24);
    }

    char name[64]{};
    if (GetKeyNameTextA(keyData, name, static_cast<int>(sizeof(name))) > 0) {
        return name;
    }

    char fallback[16]{};
    sprintf_s(fallback, sizeof(fallback), "VK_%02X", vk & 0xFF);
    return fallback;
}

std::wstring KeyTokenFromVK(int vk) {
    switch (vk) {
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

    if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z')) {
        wchar_t text[2]{ static_cast<wchar_t>(vk), L'\0' };
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

    struct NamedKey { const wchar_t* name; int vk; };
    constexpr NamedKey named[] = {
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
        if (ch >= L'a' && ch <= L'z') ch = static_cast<wchar_t>(ch - L'a' + L'A');
        if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z')) {
            return static_cast<int>(ch);
        }
    }

    if ((_wcsnicmp(text, L"VK_", 3) == 0) && text[3]) {
        wchar_t* end = nullptr;
        const long value = wcstol(text + 3, &end, 16);
        if (end != text + 3 && value > 0 && value < 256) {
            return static_cast<int>(value);
        }
    }

    return fallback;
}

void InitializePaths() {
    wchar_t path[MAX_PATH]{};
    if (!GetModuleFileNameW(g_module, path, MAX_PATH)) {
        return;
    }

    wchar_t* slash = wcsrchr(path, L'\\');
    if (!slash) {
        return;
    }
    *(slash + 1) = L'\0';

    g_iniPath = path;
    g_iniPath += kIniName;

    g_logPath = path;
    g_logPath += kLogName;
}

void ResetLogFile() {
    if (g_logPath.empty()) {
        return;
    }

    // Logs are intentionally per-session. Truncate the previous run before
    // writing the first line so diagnostics never become cumulative.
    HANDLE file = CreateFileW(
        g_logPath.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file != INVALID_HANDLE_VALUE) {
        CloseHandle(file);
    }
}

void Log(const char* format, ...) {
    if (g_logPath.empty()) {
        return;
    }

    char message[2048]{};
    va_list args;
    va_start(args, format);
    vsnprintf_s(message, sizeof(message), _TRUNCATE, format, args);
    va_end(args);

    SYSTEMTIME st{};
    GetLocalTime(&st);

    char line[2300]{};
    sprintf_s(
        line,
        sizeof(line),
        "[%04u-%02u-%02u %02u:%02u:%02u.%03u] %s\r\n",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds,
        message
    );

    HANDLE file = CreateFileW(
        g_logPath.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        WriteFile(file, line, static_cast<DWORD>(strlen(line)), &written, nullptr);
        CloseHandle(file);
    }
}

void FeatureLog(const char* message) {
    if (message && *message) {
        Log("%s", message);
    }
}

struct Config {
    bool overlayEnabled = true;
    int menuKey = VK_INSERT;

    // Requested default policy: every planned feature is enabled by default.
    bool toggleHudEnabled = true;
    bool movementSpeedEnabled = true;
    bool actionRecoveryEnabled = true;
    bool skipIntroEnabled = true;
    bool thirdPersonEnabled = true;
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool fovEnabled = true;
    bool hotstreakChargeEnabled = true;

    // User-facing tuning values.
    float movementSpeedMultiplier = 1.50f;
    float actionRecoveryDelayMs = 0.0f;
    float actionRecoveryMultiplier = 2.00f; // legacy compatibility, not used by V0.8B
    float dodgeEarlyUnlockMs = 100.0f;      // legacy compatibility, not used by V0.8B
    float pistolDamageMultiplier = 2.00f;
    float meleeDamageMultiplier = 2.00f;
    float jumpHeightMultiplier = 1.25f;
    float glideDurationMultiplier = 10.00f;
    float horseSpeedMultiplier = 1.25f;
    float horseSprintSpeedMultiplier = 1.25f;
    float horseSprintDurationMultiplier = 2.00f;
    float fovDegrees = 90.0f;
    float thirdPersonDistanceMultiplier = 1.00f;
    float hotstreakChargeMultiplier = 2.00f;

    std::array<Action, 12> hotkeys{};

    Config() {
        ResetDefaults(false);
    }

    static bool ReadBool(const wchar_t* section, const wchar_t* key, bool fallback, const std::wstring& path) {
        return GetPrivateProfileIntW(section, key, fallback ? 1 : 0, path.c_str()) != 0;
    }

    static float ReadFloat(const wchar_t* section, const wchar_t* key, float fallback, const std::wstring& path) {
        wchar_t buffer[64]{};
        wchar_t fallbackText[64]{};
        swprintf_s(fallbackText, L"%.3f", fallback);
        GetPrivateProfileStringW(section, key, fallbackText, buffer, 64, path.c_str());
        wchar_t* end = nullptr;
        const float value = wcstof(buffer, &end);
        return (end && end != buffer) ? value : fallback;
    }

    static void WriteBool(const wchar_t* section, const wchar_t* key, bool value, const std::wstring& path) {
        WritePrivateProfileStringW(section, key, value ? L"1" : L"0", path.c_str());
    }

    static void WriteFloat(const wchar_t* section, const wchar_t* key, float value, const std::wstring& path) {
        wchar_t buffer[64]{};
        swprintf_s(buffer, L"%.3f", value);
        WritePrivateProfileStringW(section, key, buffer, path.c_str());
    }

    void ResetDefaults(bool save) {
        overlayEnabled = true;
        menuKey = VK_INSERT;
        toggleHudEnabled = true;
        movementSpeedEnabled = true;
        actionRecoveryEnabled = true;
        skipIntroEnabled = true;
        thirdPersonEnabled = true;
        pistolDamageEnabled = true;
        meleeDamageEnabled = true;
        jumpHeightEnabled = true;
        glideDurationEnabled = true;
        horseSpeedEnabled = true;
        horseSprintSpeedEnabled = true;
        horseSprintDurationEnabled = true;
        fovEnabled = true;
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
        horseSprintDurationMultiplier = 2.00f;
        fovDegrees = 90.0f;
        thirdPersonDistanceMultiplier = 1.00f;
        hotstreakChargeMultiplier = 2.00f;

        hotkeys.fill(Action::None);
        hotkeys[0] = Action::ToggleHUD;
        hotkeys[1] = Action::MovementSpeed;
        hotkeys[2] = Action::ActionRecovery;
        hotkeys[3] = Action::SkipIntroVideos;
        hotkeys[4] = Action::ThirdPerson;

        if (save) {
            Save();
        }
    }

    void Load() {
        if (g_iniPath.empty()) {
            return;
        }

        if (GetFileAttributesW(g_iniPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            ResetDefaults(true);
            Log("INI not found -> wrote authoritative defaults");
            return;
        }

        overlayEnabled = ReadBool(L"Overlay", L"Enabled", true, g_iniPath);

        wchar_t menuKeyText[64]{};
        GetPrivateProfileStringW(L"Overlay", L"MenuKey", L"Insert", menuKeyText, 64, g_iniPath.c_str());
        menuKey = ParseKeyToken(menuKeyText, VK_INSERT);

        toggleHudEnabled = ReadBool(L"Features", L"ToggleHUD", true, g_iniPath);
        movementSpeedEnabled = ReadBool(L"Features", L"MovementSpeed", true, g_iniPath);
        actionRecoveryEnabled = ReadBool(L"Features", L"ActionRecovery", true, g_iniPath);
        skipIntroEnabled = ReadBool(L"Features", L"SkipIntroVideos", true, g_iniPath);
        thirdPersonEnabled = ReadBool(L"Features", L"ThirdPerson", true, g_iniPath);
        pistolDamageEnabled = ReadBool(L"Features", L"PistolDamage", true, g_iniPath);
        meleeDamageEnabled = ReadBool(L"Features", L"MeleeDamage", true, g_iniPath);
        jumpHeightEnabled = ReadBool(L"Features", L"JumpHeight", true, g_iniPath);
        glideDurationEnabled = ReadBool(L"Features", L"GlideDuration", true, g_iniPath);
        horseSpeedEnabled = ReadBool(L"Features", L"HorseSpeed", true, g_iniPath);
        horseSprintSpeedEnabled = ReadBool(L"Features", L"HorseSprintSpeed", true, g_iniPath);
        horseSprintDurationEnabled = ReadBool(L"Features", L"HorseSprintDuration", true, g_iniPath);
        fovEnabled = ReadBool(L"Features", L"FOV", true, g_iniPath);
        hotstreakChargeEnabled = ReadBool(L"Features", L"HotstreakCharge", true, g_iniPath);

        movementSpeedMultiplier = ReadFloat(L"Values", L"MovementSpeedMultiplier", 1.50f, g_iniPath);
        actionRecoveryDelayMs = ReadFloat(L"Values", L"ActionRecoveryDelayMs", 0.0f, g_iniPath);
        actionRecoveryMultiplier = ReadFloat(L"Values", L"ActionRecoveryMultiplier", 2.00f, g_iniPath);
        dodgeEarlyUnlockMs = ReadFloat(L"Values", L"DodgeEarlyUnlockMs", 100.0f, g_iniPath);
        pistolDamageMultiplier = ReadFloat(L"Values", L"PistolDamageMultiplier", 2.00f, g_iniPath);
        meleeDamageMultiplier = ReadFloat(L"Values", L"MeleeDamageMultiplier", 2.00f, g_iniPath);
        jumpHeightMultiplier = ReadFloat(L"Values", L"JumpHeightMultiplier", 1.25f, g_iniPath);
        glideDurationMultiplier = ReadFloat(L"Values", L"GlideDurationMultiplier", 10.00f, g_iniPath);
        horseSpeedMultiplier = ReadFloat(L"Values", L"HorseSpeedMultiplier", 1.25f, g_iniPath);
        horseSprintSpeedMultiplier = ReadFloat(L"Values", L"HorseSprintSpeedMultiplier", 1.25f, g_iniPath);
        horseSprintDurationMultiplier = ReadFloat(L"Values", L"HorseSprintDurationMultiplier", 2.00f, g_iniPath);
        fovDegrees = ReadFloat(L"Values", L"FOVDegrees", 90.0f, g_iniPath);
        thirdPersonDistanceMultiplier = ReadFloat(L"Values", L"ThirdPersonDistanceMultiplier", 1.00f, g_iniPath);
        hotstreakChargeMultiplier = ReadFloat(L"Values", L"HotstreakChargeMultiplier", 2.00f, g_iniPath);

        for (int i = 0; i < 12; ++i) {
            wchar_t key[8]{};
            swprintf_s(key, L"F%d", i + 1);

            wchar_t value[64]{};
            GetPrivateProfileStringW(
                L"Hotkeys",
                key,
                ActionToken(hotkeys[static_cast<size_t>(i)]),
                value,
                64,
                g_iniPath.c_str()
            );
            hotkeys[static_cast<size_t>(i)] = ParseAction(value);
        }

        Log("INI loaded");
    }

    void Save() const {
        if (g_iniPath.empty()) {
            return;
        }

        WriteBool(L"Overlay", L"Enabled", overlayEnabled, g_iniPath);
        const std::wstring menuKeyToken = KeyTokenFromVK(menuKey);
        WritePrivateProfileStringW(L"Overlay", L"MenuKey", menuKeyToken.c_str(), g_iniPath.c_str());

        WriteBool(L"Features", L"ToggleHUD", toggleHudEnabled, g_iniPath);
        WriteBool(L"Features", L"MovementSpeed", movementSpeedEnabled, g_iniPath);
        WriteBool(L"Features", L"ActionRecovery", actionRecoveryEnabled, g_iniPath);
        WriteBool(L"Features", L"SkipIntroVideos", skipIntroEnabled, g_iniPath);
        WriteBool(L"Features", L"ThirdPerson", thirdPersonEnabled, g_iniPath);
        WriteBool(L"Features", L"PistolDamage", pistolDamageEnabled, g_iniPath);
        WriteBool(L"Features", L"MeleeDamage", meleeDamageEnabled, g_iniPath);
        WriteBool(L"Features", L"JumpHeight", jumpHeightEnabled, g_iniPath);
        WriteBool(L"Features", L"GlideDuration", glideDurationEnabled, g_iniPath);
        WriteBool(L"Features", L"HorseSpeed", horseSpeedEnabled, g_iniPath);
        WriteBool(L"Features", L"HorseSprintSpeed", horseSprintSpeedEnabled, g_iniPath);
        WriteBool(L"Features", L"HorseSprintDuration", horseSprintDurationEnabled, g_iniPath);
        WriteBool(L"Features", L"FOV", fovEnabled, g_iniPath);
        WriteBool(L"Features", L"HotstreakCharge", hotstreakChargeEnabled, g_iniPath);

        WriteFloat(L"Values", L"MovementSpeedMultiplier", movementSpeedMultiplier, g_iniPath);
        WriteFloat(L"Values", L"ActionRecoveryDelayMs", actionRecoveryDelayMs, g_iniPath);
        WriteFloat(L"Values", L"ActionRecoveryMultiplier", actionRecoveryMultiplier, g_iniPath);
        WriteFloat(L"Values", L"DodgeEarlyUnlockMs", dodgeEarlyUnlockMs, g_iniPath);
        WriteFloat(L"Values", L"PistolDamageMultiplier", pistolDamageMultiplier, g_iniPath);
        WriteFloat(L"Values", L"MeleeDamageMultiplier", meleeDamageMultiplier, g_iniPath);
        WriteFloat(L"Values", L"JumpHeightMultiplier", jumpHeightMultiplier, g_iniPath);
        WriteFloat(L"Values", L"GlideDurationMultiplier", glideDurationMultiplier, g_iniPath);
        WriteFloat(L"Values", L"HorseSpeedMultiplier", horseSpeedMultiplier, g_iniPath);
        WriteFloat(L"Values", L"HorseSprintSpeedMultiplier", horseSprintSpeedMultiplier, g_iniPath);
        WriteFloat(L"Values", L"HorseSprintDurationMultiplier", horseSprintDurationMultiplier, g_iniPath);
        WriteFloat(L"Values", L"FOVDegrees", fovDegrees, g_iniPath);
        WriteFloat(L"Values", L"ThirdPersonDistanceMultiplier", thirdPersonDistanceMultiplier, g_iniPath);
        WriteFloat(L"Values", L"HotstreakChargeMultiplier", hotstreakChargeMultiplier, g_iniPath);

        for (int i = 0; i < 12; ++i) {
            wchar_t key[8]{};
            swprintf_s(key, L"F%d", i + 1);
            WritePrivateProfileStringW(
                L"Hotkeys",
                key,
                ActionToken(hotkeys[static_cast<size_t>(i)]),
                g_iniPath.c_str()
            );
        }

        Log("INI saved");
    }
};

Config g_config;

struct PeSectionView {
    BYTE* begin = nullptr;
    size_t size = 0;
};

bool GetMainModuleSection(const char* sectionName, PeSectionView& out) {
    out = {};

    HMODULE module = GetModuleHandleW(nullptr);
    if (!module || !sectionName) {
        return false;
    }

    BYTE* base = reinterpret_cast<BYTE*>(module);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        return false;
    }

    const IMAGE_SECTION_HEADER* sections = IMAGE_FIRST_SECTION(nt);
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        char name[9]{};
        memcpy(name, sections[i].Name, 8);
        if (strcmp(name, sectionName) == 0) {
            out.begin = base + sections[i].VirtualAddress;
            out.size = static_cast<size_t>(sections[i].Misc.VirtualSize);
            return out.begin != nullptr && out.size != 0;
        }
    }

    return false;
}

BYTE* FindBytes(const PeSectionView& section, const BYTE* bytes, size_t length) {
    if (!section.begin || !bytes || length == 0 || section.size < length) {
        return nullptr;
    }

    for (size_t i = 0; i <= section.size - length; ++i) {
        if (memcmp(section.begin + i, bytes, length) == 0) {
            return section.begin + i;
        }
    }

    return nullptr;
}

BYTE* FindUniquePattern(
    const PeSectionView& section,
    const int* pattern,
    size_t patternLength,
    size_t* outCount = nullptr
) {
    if (outCount) {
        *outCount = 0;
    }

    if (!section.begin || !pattern || patternLength == 0 || section.size < patternLength) {
        return nullptr;
    }

    BYTE* match = nullptr;
    size_t count = 0;

    for (size_t i = 0; i <= section.size - patternLength; ++i) {
        bool ok = true;
        for (size_t j = 0; j < patternLength; ++j) {
            if (pattern[j] >= 0 &&
                section.begin[i + j] != static_cast<BYTE>(pattern[j])) {
                ok = false;
                break;
            }
        }

        if (ok) {
            match = section.begin + i;
            ++count;
        }
    }

    if (outCount) {
        *outCount = count;
    }

    return count == 1 ? match : nullptr;
}

BYTE* FindWideString(const PeSectionView& section, const wchar_t* text) {
    if (!text) {
        return nullptr;
    }

    const size_t bytes = (wcslen(text) + 1) * sizeof(wchar_t);
    return FindBytes(section, reinterpret_cast<const BYTE*>(text), bytes);
}

BYTE* FindRipRelativeLeaTo(const PeSectionView& text, BYTE* target) {
    if (!text.begin || !target || text.size < 7) {
        return nullptr;
    }

    BYTE* match = nullptr;
    size_t count = 0;

    for (size_t i = 0; i <= text.size - 7; ++i) {
        BYTE* p = text.begin + i;

        // lea rdx,[rip+disp32] is the exact registration reference used by
        // ui.HideHud in the audited Darksiders Genesis executable.
        if (p[0] != 0x48 || p[1] != 0x8D || p[2] != 0x15) {
            continue;
        }

        const int32_t disp = *reinterpret_cast<const int32_t*>(p + 3);
        BYTE* resolved = p + 7 + disp;
        if (resolved == target) {
            match = p;
            ++count;
        }
    }

    return count == 1 ? match : nullptr;
}


LONG** ResolveSkipIntroCVarDataSlot() {
    PeSectionView text{};
    PeSectionView rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        Log("Skip Intro: failed to enumerate PE sections");
        return nullptr;
    }

    BYTE* cvarName = FindWideString(rdata, L"g.PlayIntroCinematicOnBoot");
    if (!cvarName) {
        Log("Skip Intro: g.PlayIntroCinematicOnBoot string not found");
        return nullptr;
    }

    BYTE* nameXref = FindRipRelativeLeaTo(text, cvarName);
    if (!nameXref) {
        Log("Skip Intro: unique CVar registration xref not found");
        return nullptr;
    }

    // Same UE4 TAutoConsoleVariable registration layout as ui.HideHud:
    //   lea rdx,[rip+CVarName]
    //   call qword ptr [rax+10h]
    //   ...
    //   call qword ptr [rdx+38h]
    //   mov [rip+CVarDataSlot],rax
    //
    // In this executable the final data-slot store starts +40 bytes after the
    // name LEA for g.PlayIntroCinematicOnBoot.
    BYTE* dataStore = nameXref + 40;
    if (dataStore + 7 > text.begin + text.size ||
        dataStore[0] != 0x48 ||
        dataStore[1] != 0x89 ||
        dataStore[2] != 0x05) {
        Log("Skip Intro: CVar registration layout mismatch");
        return nullptr;
    }

    const int32_t slotDisp = *reinterpret_cast<const int32_t*>(dataStore + 3);
    BYTE* dataSlot = dataStore + 7 + slotDisp;

    // Validate against the native boot-time read:
    //   mov rax,[rip+CVarDataSlot]
    //   cmp dword ptr [rax],0
    //   je ...
    BYTE* bootCheck = nullptr;
    size_t checkCount = 0;

    for (size_t i = 0; i + 12 <= text.size; ++i) {
        BYTE* p = text.begin + i;
        if (p[0] != 0x48 || p[1] != 0x8B || p[2] != 0x05) {
            continue;
        }

        const int32_t disp = *reinterpret_cast<const int32_t*>(p + 3);
        BYTE* resolved = p + 7 + disp;
        if (resolved != dataSlot) {
            continue;
        }

        if (p[7] == 0x83 &&
            p[8] == 0x38 &&
            p[9] == 0x00 &&
            p[10] == 0x74) {
            bootCheck = p;
            ++checkCount;
        }
    }

    if (!bootCheck || checkCount != 1) {
        Log("Skip Intro: native boot-check match count=%zu", checkCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Skip Intro: CVar resolved nameRVA=0x%zX slotRVA=0x%zX bootCheckRVA=0x%zX",
        static_cast<size_t>(cvarName - base),
        static_cast<size_t>(dataSlot - base),
        static_cast<size_t>(bootCheck - base)
    );

    return reinterpret_cast<LONG**>(dataSlot);
}

bool ApplySkipIntroSetting(bool logChange) {
    if (!g_skipIntroDataSlot) {
        return false;
    }

    LONG* currentData = *g_skipIntroDataSlot;
    if (!currentData) {
        return false;
    }

    if (g_skipIntroData != currentData) {
        g_skipIntroData = currentData;
        g_skipIntroOriginalValue = *currentData;
        Log(
            "Skip Intro: captured native g.PlayIntroCinematicOnBoot=%ld data=%p",
            g_skipIntroOriginalValue,
            g_skipIntroData
        );
    }

    const LONG desired =
        g_config.skipIntroEnabled ? 0 : g_skipIntroOriginalValue;

    const LONG current = *g_skipIntroData;
    if (current != desired) {
        InterlockedExchange(
            reinterpret_cast<volatile LONG*>(g_skipIntroData),
            desired
        );

        if (logChange) {
            Log(
                "Skip Intro: g.PlayIntroCinematicOnBoot %ld -> %ld (%s)",
                current,
                desired,
                g_config.skipIntroEnabled ? "SKIP" : "VANILLA"
            );
        }
    }

    g_skipIntroReady.store(true);
    return true;
}

bool InstallSkipIntroControl() {
    g_skipIntroDataSlot = ResolveSkipIntroCVarDataSlot();
    if (!g_skipIntroDataSlot) {
        Log("Skip Intro: resolver failed; feature remains fail-open");
        return false;
    }

    // The DXGI proxy can load the ASI before the executable's static CVar
    // constructors finish. Wait briefly on the ASI worker thread so the default
    // ON setting is applied before the game reaches its boot cinematic check.
    for (int i = 0; i < 5000; ++i) {
        if (ApplySkipIntroSetting(i == 0)) {
            Log(
                "Skip Intro: READY nativeCVar=%ld requested=%s",
                *g_skipIntroData,
                g_config.skipIntroEnabled ? "SKIP" : "VANILLA"
            );
            return true;
        }
        Sleep(1);
    }

    Log("Skip Intro: CVar data pointer did not initialize within startup window");
    return false;
}

BYTE* ResolveHudHiddenGetter() {
    PeSectionView text{};
    PeSectionView rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        Log("HUD hook: failed to enumerate PE sections");
        return nullptr;
    }

    BYTE* cvarName = FindWideString(rdata, L"ui.HideHud");
    if (!cvarName) {
        Log("HUD hook: ui.HideHud string not found");
        return nullptr;
    }

    BYTE* nameXref = FindRipRelativeLeaTo(text, cvarName);
    if (!nameXref) {
        Log("HUD hook: unique ui.HideHud registration xref not found");
        return nullptr;
    }

    // Audited registration sequence:
    //   lea rdx,[rip+ui.HideHud]
    //   call qword ptr [rax+10h]
    //   mov [rip+ConsoleVariableObject],rax
    //   ...
    //   call qword ptr [rdx+38h]
    //   mov [rip+ConsoleVariableData],rax
    //
    // The final MOV begins exactly 40 bytes after the LEA in this executable.
    BYTE* dataStore = nameXref + 40;
    if (dataStore + 7 > text.begin + text.size ||
        dataStore[0] != 0x48 ||
        dataStore[1] != 0x89 ||
        dataStore[2] != 0x05) {
        Log("HUD hook: ui.HideHud registration layout mismatch");
        return nullptr;
    }

    const int32_t slotDisp = *reinterpret_cast<const int32_t*>(dataStore + 3);
    BYTE* dataSlot = dataStore + 7 + slotDisp;

    BYTE* getter = nullptr;
    size_t getterCount = 0;

    for (size_t i = 0; i + 14 <= text.size; ++i) {
        BYTE* p = text.begin + i;
        if (p[0] != 0x48 || p[1] != 0x8B || p[2] != 0x05) {
            continue;
        }

        const int32_t disp = *reinterpret_cast<const int32_t*>(p + 3);
        BYTE* resolved = p + 7 + disp;
        if (resolved != dataSlot) {
            continue;
        }

        static constexpr BYTE tail[] = {
            0x83, 0x38, 0x00,
            0x0F, 0x95, 0xC0,
            0xC3
        };

        if (memcmp(p + 7, tail, sizeof(tail)) == 0) {
            getter = p;
            ++getterCount;
        }
    }

    if (getterCount != 1 || !getter) {
        Log("HUD hook: ui.HideHud getter match count=%zu", getterCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "HUD hook: ui.HideHud resolved nameRVA=0x%zX slotRVA=0x%zX getterRVA=0x%zX",
        static_cast<size_t>(cvarName - base),
        static_cast<size_t>(dataSlot - base),
        static_cast<size_t>(getter - base)
    );

    return getter;
}

bool HookHudHiddenGetter() {
    const bool nativeHidden = g_originalHudHiddenGetter
        ? g_originalHudHiddenGetter()
        : false;

    if (!g_config.toggleHudEnabled) {
        return nativeHidden;
    }

    return nativeHidden || g_hudHidden.load();
}

BYTE* FindAsciiString(const PeSectionView& section, const char* text) {
    if (!text) {
        return nullptr;
    }

    const size_t bytes = strlen(text) + 1;
    return FindBytes(section, reinterpret_cast<const BYTE*>(text), bytes);
}

bool AddressInSection(const PeSectionView& section, const void* address) {
    const BYTE* p = reinterpret_cast<const BYTE*>(address);
    return section.begin && p >= section.begin && p < section.begin + section.size;
}


BYTE* ResolveAddJuiceNative() {
    PeSectionView text{};
    if (!GetMainModuleSection(".text", text)) {
        Log("Hotstreak hook: failed to enumerate .text");
        return nullptr;
    }

    // UMayhem Hotstreak AddJuice native body.
    //
    // Audited native RVA: 0x660260
    // The generated AddJuice exec wrapper passes Amount in XMM1 and calls this
    // native function. The signature below is unique in the target EXE.
    static constexpr int kPattern[] = {
        0x48, 0x89, 0x5C, 0x24, 0x10,
        0x48, 0x89, 0x6C, 0x24, 0x18,
        0x57,
        0x48, 0x81, 0xEC, 0xB0, 0x00, 0x00, 0x00,
        0x48, 0x8B, 0xB9, 0xE8, 0x00, 0x00, 0x00,
        0x48, 0x8B, 0xD9,
        0x0F, 0x29, 0xBC, 0x24, 0x90, 0x00, 0x00, 0x00,
        0x0F, 0x28, 0xF9
    };

    size_t matchCount = 0;
    BYTE* target = FindUniquePattern(
        text,
        kPattern,
        ARRAYSIZE(kPattern),
        &matchCount
    );

    if (!target) {
        Log("Hotstreak hook: AddJuice signature match count=%zu", matchCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Hotstreak hook: AddJuice resolved RVA=0x%zX",
        static_cast<size_t>(target - base)
    );

    return target;
}

void HookAddJuice(void* hotStreakComponent, float amount) {
    if (!g_originalAddJuice) {
        return;
    }

    float effectiveAmount = amount;

    if (g_config.hotstreakChargeEnabled &&
        hotStreakComponent &&
        amount > 0.0f &&
        amount < 100000.0f) {

        // The audited AddJuice body immediately reads component+0xE8 and treats
        // it as its owning player object. Compare it to the locally controlled
        // player captured by CharacterMovement. No arbitrary owner dereference
        // is required here.
        void* owner = *reinterpret_cast<void**>(
            reinterpret_cast<BYTE*>(hotStreakComponent) + 0xE8
        );
        void* localPlayer = g_localPlayerCharacter.load();

        if (localPlayer && owner == localPlayer) {
            float multiplier = g_config.hotstreakChargeMultiplier;
            if (multiplier < 0.0f) multiplier = 0.0f;
            if (multiplier > 25.0f) multiplier = 25.0f;

            effectiveAmount = amount * multiplier;
            if (effectiveAmount > 100000.0f) {
                effectiveAmount = 100000.0f;
            }

            g_lastNativeJuiceGain.store(amount);
            g_lastBoostedJuiceGain.store(effectiveAmount);

            const int count = g_hotstreakBoostCalls.fetch_add(1) + 1;
            if (count <= 30 || (count % 100) == 0) {
                Log(
                    "Hotstreak hook: AddJuice local gain %.3f -> %.3f (%.2fx) count=%d",
                    amount,
                    effectiveAmount,
                    multiplier,
                    count
                );
            }
        }
    }

    g_originalAddJuice(hotStreakComponent, effectiveAmount);
}

bool InstallHotstreakChargeHook() {
    BYTE* target = ResolveAddJuiceNative();
    if (!target) {
        Log("Hotstreak hook: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log(
            "Hotstreak hook: MinHook initialize FAILED status=%d",
            static_cast<int>(initStatus)
        );
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookAddJuice),
        reinterpret_cast<LPVOID*>(&g_originalAddJuice)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log(
            "Hotstreak hook: create FAILED status=%d",
            static_cast<int>(status)
        );
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log(
            "Hotstreak hook: enable FAILED status=%d",
            static_cast<int>(status)
        );
        return false;
    }

    g_hotstreakHookReady.store(true);
    Log(
        "Hotstreak hook: READY AddJuice positive local gains multiplier=%.3fx",
        g_config.hotstreakChargeMultiplier
    );
    return true;
}


BYTE* ResolveFinalOutgoingDamageFilter() {
    PeSectionView text{};
    if (!GetMainModuleSection(".text", text)) {
        Log("Final damage hook: failed to enumerate .text");
        return nullptr;
    }

    // Player outgoing-damage filter.
    //
    // Audited RVA: 0x668BE0
    //
    // The native function:
    //   - multiplies DamageRecord.Damage by d.PlayerOutgoingDamageMultiplier;
    //   - applies player outgoing-damage filters/status effects;
    //   - mutates the same FMayhemDamageEventRecord in place.
    //
    // V0.13A hooks the function and applies user multipliers only AFTER the
    // native function returns, putting us downstream of GetBaseDamage.
    static constexpr int kPattern[] = {
        0x48, 0x89, 0x5C, 0x24, 0x08,
        0x57,
        0x48, 0x83, 0xEC, 0x20,
        0x48, 0x8B, 0x05, -1, -1, -1, -1,
        0x48, 0x8B, 0xFA,
        0x48, 0x8B, 0xD9,
        0xF3, 0x0F, 0x10, 0x00,
        0xF3, 0x0F, 0x59, 0x42, 0x08,
        0xF3, 0x0F, 0x11, 0x42, 0x08,
        0xE8, -1, -1, -1, -1
    };

    size_t matchCount = 0;
    BYTE* target = FindUniquePattern(
        text,
        kPattern,
        ARRAYSIZE(kPattern),
        &matchCount
    );

    if (!target) {
        Log("Final damage hook: outgoing-filter signature match count=%zu", matchCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Final damage hook: player outgoing filter resolved RVA=0x%zX",
        static_cast<size_t>(target - base)
    );

    return target;
}

void HookFinalOutgoingDamage(void* playerCharacter, void* damageRecord) {
    if (!g_originalFilterOutgoingDamage) {
        return;
    }

    // Let the full native player-damage pipeline run first.
    g_originalFilterOutgoingDamage(playerCharacter, damageRecord);

    if (!playerCharacter || !damageRecord) {
        return;
    }

    const dg::player::IdentityTelemetry playerIdentity =
        dg::player::GetIdentityTelemetry();
    if (!playerIdentity.validated ||
        playerIdentity.character != playerCharacter) {
        return;
    }

    BYTE* record = reinterpret_cast<BYTE*>(damageRecord);
    float* damagePtr = reinterpret_cast<float*>(record + 0x08);
    const float nativeFinalDamage = *damagePtr;

    if (!(nativeFinalDamage >= 0.0f && nativeFinalDamage < 1000000.0f)) {
        return;
    }

    const uint32_t scaleType = *reinterpret_cast<uint32_t*>(record + 0x0C);
    const int32_t tagCount = *reinterpret_cast<int32_t*>(record + 0x20);
    const float baseJuice = *reinterpret_cast<float*>(record + 0x28);

    g_lastOutgoingScaleType.store(scaleType);
    g_lastOutgoingTagCount.store(tagCount);

    const bool pistolLike =
        baseJuice > 0.0001f &&
        baseJuice < 1000.0f;

    // Diagnostic classification:
    // supplied Strife projectile PAKs consistently carry BaseJuice > 0;
    // ordinary melee records are expected to carry BaseJuice == 0.
    //
    // The zero-juice branch is intentionally marked diagnostic because it may
    // also include non-pistol player abilities. ScaleType/tag telemetry is
    // logged so we can tighten the discriminator after one real test.
    float multiplier = 1.0f;
    const char* kind = nullptr;

    if (pistolLike && g_config.pistolDamageEnabled) {
        multiplier = g_config.pistolDamageMultiplier;
        if (multiplier < 0.0f) multiplier = 0.0f;
        if (multiplier > 100.0f) multiplier = 100.0f;
        kind = "PISTOL";

        g_lastNativePistolDamage.store(nativeFinalDamage);
        g_lastPistolBaseJuice.store(baseJuice);
    } else if (!pistolLike && g_config.meleeDamageEnabled) {
        multiplier = g_config.meleeDamageMultiplier;
        if (multiplier < 0.0f) multiplier = 0.0f;
        if (multiplier > 100.0f) multiplier = 100.0f;
        kind = "ZERO_JUICE_MELEE_DIAG";

        g_lastNativeBaseDamage.store(nativeFinalDamage);
    } else {
        return;
    }

    float boostedDamage = nativeFinalDamage * multiplier;
    if (boostedDamage > 1000000.0f) {
        boostedDamage = 1000000.0f;
    }

    *damagePtr = boostedDamage;

    int count = 0;
    if (pistolLike) {
        g_lastBoostedPistolDamage.store(boostedDamage);
        count = g_pistolDamageBoostCalls.fetch_add(1) + 1;
    } else {
        g_lastBoostedBaseDamage.store(boostedDamage);
        count = g_meleeDamageBoostCalls.fetch_add(1) + 1;
    }

    if (count <= 60 || (count % 100) == 0) {
        Log(
            "Final damage hook: %s final %.3f -> %.3f BaseJuice=%.3f ScaleType=%u Tags=%d mult=%.2fx count=%d",
            kind,
            nativeFinalDamage,
            boostedDamage,
            baseJuice,
            scaleType,
            tagCount,
            multiplier,
            count
        );
    }
}

bool InstallFinalOutgoingDamageHook() {
    BYTE* target = ResolveFinalOutgoingDamageFilter();
    if (!target) {
        Log("Final damage hook: resolver failed; Pistol/Melee remain fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log(
            "Final damage hook: MinHook initialize FAILED status=%d",
            static_cast<int>(initStatus)
        );
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookFinalOutgoingDamage),
        reinterpret_cast<LPVOID*>(&g_originalFilterOutgoingDamage)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log(
            "Final damage hook: create FAILED status=%d",
            static_cast<int>(status)
        );
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log(
            "Final damage hook: enable FAILED status=%d",
            static_cast<int>(status)
        );
        return false;
    }

    g_finalOutgoingDamageHookReady.store(true);

    Log(
        "Final damage hook: READY RVA=0x668BE0 pistol=BaseJuice>0 meleeDiag=BaseJuice==0"
    );
    return true;
}

BYTE* ResolveMovementComponentGetMaxSpeedOverride() {
    PeSectionView text{};
    if (!GetMainModuleSection(".text", text)) {
        Log("Movement hook: failed to enumerate .text");
        return nullptr;
    }

    // V0.4A mistake:
    // the old resolver hooked AMayhemCharacter::GetMaxSpeed, which only queries
    // the movement component and is not the virtual used by movement physics.
    //
    // V0.5B targets the real UMayhemCharacterMovementComponent::GetMaxSpeed
    // override. The base UCharacterMovementComponent virtual sits at vtable
    // +0x3D0; Mayhem replaces that slot with this unique implementation.
    //
    // Audited RVA: 0x56FBE0
    // UMayhemCharacterMovementComponent reflected object size: 0x850.
    static constexpr int kPattern[] = {
        0x4C, 0x8B, 0xDC,
        0x55,
        0x57,
        0x49, 0x8D, 0x6B, 0xA1,
        0x48, 0x81, 0xEC, 0xB8, 0x00, 0x00, 0x00,
        0x8B, 0x81, 0x80, 0x07, 0x00, 0x00,
        0x48, 0x8B, 0xF9,
        0x2B, 0x81, 0xAC, 0x07, 0x00, 0x00
    };

    size_t matchCount = 0;
    BYTE* target = FindUniquePattern(
        text,
        kPattern,
        ARRAYSIZE(kPattern),
        &matchCount
    );

    if (!target) {
        Log("Movement hook: virtual GetMaxSpeed signature match count=%zu", matchCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Movement hook: UMayhemCharacterMovementComponent::GetMaxSpeed resolved RVA=0x%zX vtableSlot=0x3D0",
        static_cast<size_t>(target - base)
    );

    return target;
}

float ClampFloat(float value, float minValue, float maxValue) {
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

bool IsReasonablePositiveFloat(float value, float minValue, float maxValue) {
    return value >= minValue && value <= maxValue;
}

PlayerMovementTuningState* FindOrCapturePlayerMovementState(void* movementComponent) {
    if (!movementComponent) {
        return nullptr;
    }

    for (auto& state : g_playerMovementStates) {
        if (state.component == movementComponent) {
            return &state;
        }
    }

    BYTE* component = reinterpret_cast<BYTE*>(movementComponent);

    // Offsets recovered directly from UE4 generated reflection property params
    // in the audited executable:
    // UCharacterMovementComponent::JumpZVelocity                 +0x1A0
    // UMayhemPlayerCharacterMovementComponent::DoubleJumpZVelocity +0x85C
    // UMayhemPlayerCharacterMovementComponent::GlideDurationSeconds +0x86C
    const float jumpZ = *reinterpret_cast<float*>(component + 0x1A0);
    const float doubleJumpZ = *reinterpret_cast<float*>(component + 0x85C);
    const float glideDuration = *reinterpret_cast<float*>(component + 0x86C);

    if (!IsReasonablePositiveFloat(jumpZ, 100.0f, 10000.0f) ||
        !IsReasonablePositiveFloat(doubleJumpZ, 100.0f, 10000.0f) ||
        !IsReasonablePositiveFloat(glideDuration, 0.05f, 60.0f)) {
        const int budget = g_movementCaptureRejectLogBudget.fetch_sub(1);
        if (budget > 0) {
            Log(
                "Runtime tuning: movement capture rejected component=%p jump=%.3f double=%.3f glide=%.3f",
                movementComponent,
                jumpZ,
                doubleJumpZ,
                glideDuration
            );
        }
        return nullptr;
    }

    PlayerMovementTuningState* slot = nullptr;
    for (auto& state : g_playerMovementStates) {
        if (!state.component) {
            slot = &state;
            break;
        }
    }
    if (!slot) {
        slot = &g_playerMovementStates[0];
    }

    slot->component = movementComponent;
    slot->jumpZVelocity = jumpZ;
    slot->doubleJumpZVelocity = doubleJumpZ;
    slot->glideDurationSeconds = glideDuration;

    Log(
        "Runtime tuning: captured movement component=%p JumpZ=%.3f DoubleJumpZ=%.3f GlideDuration=%.3f",
        movementComponent,
        jumpZ,
        doubleJumpZ,
        glideDuration
    );

    return slot;
}

void ApplyPlayerMovementTunings(void* movementComponent) {
    AcquireSRWLockExclusive(&g_tuningLock);

    PlayerMovementTuningState* state = FindOrCapturePlayerMovementState(movementComponent);
    if (!state) {
        ReleaseSRWLockExclusive(&g_tuningLock);
        return;
    }

    BYTE* component = reinterpret_cast<BYTE*>(movementComponent);

    float heightMultiplier = ClampFloat(g_config.jumpHeightMultiplier, 0.0f, 20.0f);
    // Jump apex height is approximately proportional to velocity squared when
    // gravity is unchanged, so use sqrt(multiplier) for a true height scalar.
    const float velocityMultiplier = sqrtf(heightMultiplier);

    *reinterpret_cast<float*>(component + 0x1A0) =
        g_config.jumpHeightEnabled
            ? state->jumpZVelocity * velocityMultiplier
            : state->jumpZVelocity;

    *reinterpret_cast<float*>(component + 0x85C) =
        g_config.jumpHeightEnabled
            ? state->doubleJumpZVelocity * velocityMultiplier
            : state->doubleJumpZVelocity;

    const float glideMultiplier = ClampFloat(g_config.glideDurationMultiplier, 0.0f, 100.0f);
    *reinterpret_cast<float*>(component + 0x86C) =
        g_config.glideDurationEnabled
            ? state->glideDurationSeconds * glideMultiplier
            : state->glideDurationSeconds;

    ReleaseSRWLockExclusive(&g_tuningLock);
}

float HookCharacterGetMaxSpeed(void* movementComponent) {
    const float nativeSpeed = g_originalCharacterGetMaxSpeed
        ? g_originalCharacterGetMaxSpeed(movementComponent)
        : 0.0f;

    if (!movementComponent) {
        return nativeSpeed;
    }

    BYTE* component = reinterpret_cast<BYTE*>(movementComponent);
    void* characterOwner = *reinterpret_cast<void**>(component + 0x190);
    if (!characterOwner) {
        return nativeSpeed;
    }

    dg::horse::Settings horseSettings{};
    horseSettings.speedEnabled = g_config.horseSpeedEnabled;
    horseSettings.speedMultiplier = g_config.horseSpeedMultiplier;
    horseSettings.sprintDurationEnabled = g_config.horseSprintDurationEnabled;
    horseSettings.sprintDurationMultiplier =
        g_config.horseSprintDurationMultiplier;
    dg::horse::SetSettings(horseSettings);
    dg::horse::Tick();

    // Observe every component already reaching this validated shared Mayhem
    // movement hook. Do this BEFORE APawn::IsLocallyControlled, because the
    // player's horse can itself be locally controlled. HorseFeature identifies
    // the mount only from the exact reference-PAK property signature.
    dg::horse::ObserveMovement(
        movementComponent,
        characterOwner,
        nativeSpeed
    );

    const bool isLocalPlayer =
        dg::player::IsValidatedLocalPlayer(
            characterOwner,
            movementComponent
        );

    if (!isLocalPlayer) {
        return nativeSpeed;
    }

    g_localPlayerCharacter.store(characterOwner);

    ApplyPlayerMovementTunings(movementComponent);

    if (!g_config.movementSpeedEnabled || nativeSpeed <= 0.0f) {
        return nativeSpeed;
    }

    // EMovementMode: 1 = Walking, 2 = NavWalking.
    const unsigned char movementMode = *(component + 0x1B0);
    if (movementMode != 1 && movementMode != 2) {
        return nativeSpeed;
    }

    const float multiplier =
        ClampFloat(g_config.movementSpeedMultiplier, 0.0f, 5.00f);
    return nativeSpeed * multiplier;
}

bool InstallMovementSpeedHook() {
    BYTE* target = ResolveMovementComponentGetMaxSpeedOverride();
    if (!target) {
        Log("Movement hook: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("Movement hook: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    g_playerGetMaxSpeedTarget = target;

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookCharacterGetMaxSpeed),
        reinterpret_cast<LPVOID*>(&g_originalCharacterGetMaxSpeed)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("Movement hook: create FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("Movement hook: enable FAILED status=%d", static_cast<int>(status));
        return false;
    }

    g_movementHookReady.store(true);
    Log(
        "Movement hook: READY multiplier=%.3fx player-only=1 walking-only=1 virtualSlot=0x3D0",
        g_config.movementSpeedMultiplier
    );
    return true;
}

const char* AbilityStateName(unsigned char state) {
    switch (state) {
    case 0: return "INITIALIZING";
    case 1: return "STARTING";
    case 2: return "RUNNING";
    case 3: return "SUSPENDED";
    case 4: return "AWAITING_FINISH";
    case 5: return "FINISHED";
    case 6: return "FINALIZED";
    default: return "UNKNOWN";
    }
}

BYTE* ResolveAbilityActionEnabledNative() {
    PeSectionView text{};
    PeSectionView rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        Log("Action Recovery V0.8: failed to enumerate PE sections");
        return nullptr;
    }

    BYTE* name = FindAsciiString(rdata, "IsActionEnabled");
    if (!name) {
        Log("Action Recovery V0.8: IsActionEnabled string not found");
        return nullptr;
    }

    static constexpr BYTE kNativePrefix[] = {
        0x44, 0x0F, 0xB6, 0xC2,
        0x41, 0x0F, 0xB6, 0xC0,
        0x49, 0xC1, 0xE8, 0x06,
        0x24, 0x3F,
        0x0F, 0xB6, 0xD0,
        0x4A, 0x8B, 0x84, 0xC1, 0x80, 0x00, 0x00, 0x00
    };

    const uintptr_t nameVA = reinterpret_cast<uintptr_t>(name);
    BYTE* native = nullptr;
    size_t nativeCount = 0;

    for (size_t i = 0; i + 16 <= rdata.size; i += sizeof(uintptr_t)) {
        BYTE* entry = rdata.begin + i;
        if (*reinterpret_cast<const uintptr_t*>(entry) != nameVA) {
            continue;
        }

        BYTE* wrapper = reinterpret_cast<BYTE*>(
            *reinterpret_cast<const uintptr_t*>(entry + sizeof(uintptr_t))
        );
        if (!AddressInSection(text, wrapper)) {
            continue;
        }

        for (size_t j = 0; j + 5 <= 0x100; ++j) {
            BYTE* p = wrapper + j;
            if (!AddressInSection(text, p) || p[0] != 0xE8) {
                continue;
            }

            const int32_t rel = *reinterpret_cast<const int32_t*>(p + 1);
            BYTE* target = p + 5 + rel;
            if (!AddressInSection(text, target)) {
                continue;
            }

            if (memcmp(target, kNativePrefix, sizeof(kNativePrefix)) == 0) {
                if (!native || native != target) {
                    native = target;
                    ++nativeCount;
                }
            }
        }
    }

    if (!native || nativeCount != 1) {
        Log("Action Recovery V0.8: native IsActionEnabled match count=%zu", nativeCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Action Recovery V0.8: IsActionEnabled resolved RVA=0x%zX actionBitset=ability+0x80 instigator=+0x48 state=+0xD8 elapsed=+0xDC",
        static_cast<size_t>(native - base)
    );

    return native;
}

bool HookAbilityActionEnabled(void* ability, unsigned char action) {
    const bool nativeEnabled = g_originalAbilityActionEnabled
        ? g_originalAbilityActionEnabled(ability, action)
        : false;

    // ECharacterActions::MOVE was independently observed in the native player
    // action gate as enum value 0x1D.
    constexpr unsigned char kMoveAction = 0x1D;

    if (!ability || action != kMoveAction) {
        return nativeEnabled;
    }

    g_actionMoveQueries.fetch_add(1);

    BYTE* object = reinterpret_cast<BYTE*>(ability);
    void* instigator = *reinterpret_cast<void**>(object + 0x48);
    void* localPlayer = g_localPlayerCharacter.load();

    if (!instigator || !localPlayer || instigator != localPlayer) {
        return nativeEnabled;
    }

    g_actionMoveLocalQueries.fetch_add(1);

    const unsigned char state = *(object + 0xD8);
    const float elapsed = *reinterpret_cast<float*>(object + 0xDC);
    g_lastActionMoveElapsed.store(elapsed);

    if (!nativeEnabled) {
        g_actionMoveNativeBlocked.fetch_add(1);
    }

    void* previousAbility = g_lastActionMoveAbility.load();
    const int previousState = g_lastActionMoveState.load();
    if (previousAbility != ability || previousState != static_cast<int>(state)) {
        g_lastActionMoveAbility.store(ability);
        g_lastActionMoveState.store(static_cast<int>(state));
        Log(
            "Action Recovery V0.8B: MOVE query ability=%p state=%s(%u) elapsed=%.3f native=%d",
            ability,
            AbilityStateName(state),
            static_cast<unsigned>(state),
            elapsed,
            nativeEnabled ? 1 : 0
        );
    }

    if (!g_config.actionRecoveryEnabled || nativeEnabled) {
        if (nativeEnabled || state != 4) {
            g_recoveryTailAbility.store(nullptr);
            g_recoveryTailStartElapsed.store(0.0f);
        }
        return nativeEnabled;
    }

    // V0.8A proved that AllowedActions/MOVE can affect the lock, but forcing
    // MOVE during RUNNING lets interactions (e.g. chest opening) slide without
    // their animation. V0.8B therefore preserves STARTING/RUNNING entirely and
    // only trims the common AWAITING_FINISH tail.
    if (state != 4) {
        g_recoveryTailAbility.store(nullptr);
        g_recoveryTailStartElapsed.store(0.0f);
        return nativeEnabled;
    }

    if (g_recoveryTailAbility.load() != ability) {
        g_recoveryTailAbility.store(ability);
        g_recoveryTailStartElapsed.store(elapsed);
        Log(
            "Action Recovery V0.8B: tail START ability=%p elapsed=%.3f",
            ability,
            elapsed
        );
    }

    float delayMs = g_config.actionRecoveryDelayMs;
    if (delayMs < 0.0f) delayMs = 0.0f;
    if (delayMs > 500.0f) delayMs = 500.0f;

    float tailElapsed = elapsed - g_recoveryTailStartElapsed.load();
    if (tailElapsed < 0.0f || tailElapsed > 10.0f) {
        g_recoveryTailStartElapsed.store(elapsed);
        tailElapsed = 0.0f;
    }

    if (tailElapsed * 1000.0f < delayMs) {
        return nativeEnabled;
    }

    const int forced = g_actionMoveForced.fetch_add(1) + 1;
    if (forced <= 30 || (forced % 100) == 0) {
        Log(
            "Action Recovery V0.8B: FORCE MOVE TAIL ability=%p tail=%.3f sec delay=%.0f ms forcedCount=%d",
            ability,
            tailElapsed,
            delayMs,
            forced
        );
    }

    return true;
}

bool InstallActionEnabledRecoveryDiagnostic() {
    BYTE* target = ResolveAbilityActionEnabledNative();
    if (!target) {
        Log("Action Recovery V0.8: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("Action Recovery V0.8: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookAbilityActionEnabled),
        reinterpret_cast<LPVOID*>(&g_originalAbilityActionEnabled)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("Action Recovery V0.8: create FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("Action Recovery V0.8: enable FAILED status=%d", static_cast<int>(status));
        return false;
    }

    g_recoveryHookReady.store(true);
    Log("Action Recovery V0.8B: READY policy=force local MOVE only in AWAITING_FINISH");
    return true;
}

bool InstallHudHook() {
    BYTE* getter = ResolveHudHiddenGetter();
    if (!getter) {
        Log("HUD hook: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("HUD hook: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        getter,
        reinterpret_cast<LPVOID>(&HookHudHiddenGetter),
        reinterpret_cast<LPVOID*>(&g_originalHudHiddenGetter)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("HUD hook: create FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(getter);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("HUD hook: enable FAILED status=%d", static_cast<int>(status));
        return false;
    }

    g_hudHookReady.store(true);
    Log("HUD hook: READY; mod state starts visible and preserves native hidden state");
    return true;
}

bool IsFeatureEnabled(Action action) {
    switch (action) {
    case Action::ToggleHUD:
        return g_config.toggleHudEnabled;
    case Action::MovementSpeed:
        return g_config.movementSpeedEnabled;
    case Action::ActionRecovery:
        return g_config.actionRecoveryEnabled;
    case Action::SkipIntroVideos:
        return g_config.skipIntroEnabled;
    case Action::ThirdPerson:
        return g_config.thirdPersonEnabled;
    default:
        return true;
    }
}

bool KeyPressed(int vk) {
    if (vk < 0 || vk >= static_cast<int>(g_keyDown.size())) {
        return false;
    }

    const bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
    const bool pressed = down && !g_keyDown[static_cast<size_t>(vk)];
    g_keyDown[static_cast<size_t>(vk)] = down;
    return pressed;
}

void TriggerAction(Action action, int functionKey) {
    if (action == Action::None) {
        return;
    }

    const char* label = ActionLabel(action);

    if (action == Action::ToggleHUD) {
        if (!g_config.toggleHudEnabled) {
            g_lastAction = "Toggle HUD disabled in config";
            Log("F%d -> Toggle HUD ignored (feature disabled)", functionKey);
            return;
        }

        if (!g_hudHookReady.load()) {
            g_lastAction = "Toggle HUD [hook unavailable]";
            Log("F%d -> Toggle HUD ignored (native hook unavailable)", functionKey);
            return;
        }

        const bool hidden = !g_hudHidden.load();
        g_hudHidden.store(hidden);
        g_lastAction = std::string("HUD ") + (hidden ? "hidden" : "visible");
        Log("F%d -> HUD %s", functionKey, hidden ? "HIDDEN" : "VISIBLE");
        return;
    }

    if (action == Action::MovementSpeed) {
        if (!g_movementHookReady.load()) {
            g_lastAction = "Movement Speed [hook unavailable]";
            Log("F%d -> Movement Speed ignored (native hook unavailable)", functionKey);
            return;
        }

        g_config.movementSpeedEnabled = !g_config.movementSpeedEnabled;
        g_config.Save();
        g_lastAction = std::string("Movement Speed ") +
            (g_config.movementSpeedEnabled ? "ON" : "OFF");
        Log(
            "F%d -> Movement Speed %s multiplier=%.3fx",
            functionKey,
            g_config.movementSpeedEnabled ? "ON" : "OFF",
            g_config.movementSpeedMultiplier
        );
        return;
    }

    if (action == Action::ActionRecovery) {
        if (!g_recoveryHookReady.load()) {
            g_lastAction = "Action Recovery [hook unavailable]";
            Log("F%d -> Action Recovery ignored (native hook unavailable)", functionKey);
            return;
        }

        g_config.actionRecoveryEnabled = !g_config.actionRecoveryEnabled;
        g_config.Save();
        g_lastAction = std::string("Action Recovery ") +
            (g_config.actionRecoveryEnabled ? "ON" : "OFF");
        Log(
            "F%d -> Action Recovery %s policy=AWAITING_FINISH tail only",
            functionKey,
            g_config.actionRecoveryEnabled ? "ON" : "OFF"
        );
        return;
    }

    if (action == Action::SkipIntroVideos) {
        g_config.skipIntroEnabled = !g_config.skipIntroEnabled;
        g_config.Save();

        const bool applied = ApplySkipIntroSetting(true);
        g_lastAction = std::string("Skip Intro ") +
            (g_config.skipIntroEnabled ? "ON" : "OFF");

        Log(
            "F%d -> Skip Intro %s nativeApply=%d (boot effect may require restart)",
            functionKey,
            g_config.skipIntroEnabled ? "ON" : "OFF",
            applied ? 1 : 0
        );
        return;
    }

    if (!IsFeatureEnabled(action)) {
        g_lastAction = std::string(label) + " disabled in config";
        Log("F%d -> %s ignored (feature disabled)", functionKey, label);
        return;
    }

    g_lastAction = std::string(label) + " [hook pending]";
    Log("F%d -> %s (input OK, gameplay hook pending)", functionKey, label);
}

void ProcessInput() {
    const int capturedMenuKey = g_capturedMenuKey.exchange(0);
    if (capturedMenuKey > 0 && capturedMenuKey < 256) {
        g_config.menuKey = capturedMenuKey;
        g_keyDown[static_cast<size_t>(capturedMenuKey)] = true;
        g_config.Save();

        const std::string keyName = KeyDisplayName(capturedMenuKey);
        g_lastAction = std::string("Menu key rebound to ") + keyName;
        Log("Menu key rebound to %s (VK=0x%02X)", keyName.c_str(), capturedMenuKey);
    }

    if (!g_captureMenuKey.load() &&
        g_config.menuKey > 0 &&
        g_config.menuKey < 256 &&
        KeyPressed(g_config.menuKey) &&
        g_config.overlayEnabled) {
        const bool newState = !g_overlayVisible.load();
        g_overlayVisible.store(newState);
        Log("Overlay %s by %s", newState ? "OPEN" : "CLOSED", KeyDisplayName(g_config.menuKey).c_str());
    }

    if (g_overlayVisible.load()) {
        return;
    }

    for (int i = 0; i < 12; ++i) {
        if (KeyPressed(VK_F1 + i)) {
            TriggerAction(g_config.hotkeys[static_cast<size_t>(i)], i + 1);
        }
    }
}

void CreateRenderTarget() {
    if (!g_gameSwapChain || !g_device || g_rtv) {
        return;
    }

    ID3D11Texture2D* backBuffer = nullptr;
    if (SUCCEEDED(g_gameSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))) && backBuffer) {
        g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
        backBuffer->Release();
    }
}

void ReleaseRenderTarget() {
    if (g_rtv) {
        g_rtv->Release();
        g_rtv = nullptr;
    }
}

LRESULT CALLBACK OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (g_imguiReady.load() && g_overlayVisible.load()) {
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

        if (g_captureMenuKey.load() && (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)) {
            const int vk = static_cast<int>(wParam & 0xFF);
            if (vk == VK_ESCAPE) {
                g_captureMenuKey.store(false);
                g_lastAction = "Menu key rebind cancelled";
                Log("Menu key rebind cancelled");
                return TRUE;
            }

            if (vk > 0 && vk < 256) {
                g_capturedMenuKey.store(vk);
                g_captureMenuKey.store(false);
                return TRUE;
            }
        }

        ImGuiIO& io = ImGui::GetIO();
        const bool mouseMessage =
            msg == WM_MOUSEMOVE ||
            msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_LBUTTONDBLCLK ||
            msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP || msg == WM_RBUTTONDBLCLK ||
            msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP || msg == WM_MBUTTONDBLCLK ||
            msg == WM_XBUTTONDOWN || msg == WM_XBUTTONUP || msg == WM_XBUTTONDBLCLK ||
            msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL ||
            msg == WM_SETCURSOR;

        const bool keyboardMessage =
            msg == WM_KEYDOWN || msg == WM_KEYUP ||
            msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP ||
            msg == WM_CHAR;

        if ((mouseMessage && io.WantCaptureMouse) ||
            (keyboardMessage && io.WantCaptureKeyboard) ||
            msg == WM_INPUT) {
            return TRUE;
        }
    }

    return g_originalWndProc
        ? CallWindowProcW(g_originalWndProc, hwnd, msg, wParam, lParam)
        : DefWindowProcW(hwnd, msg, wParam, lParam);
}

bool InitializeImGui(IDXGISwapChain* swapChain) {
    if (g_imguiReady.load()) {
        return true;
    }

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swapChain->GetDesc(&desc)) || !desc.OutputWindow) {
        return false;
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(desc.OutputWindow, &pid);
    if (pid != GetCurrentProcessId()) {
        return false;
    }

    ID3D11Device* device = nullptr;
    if (FAILED(swapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device))) || !device) {
        return false;
    }

    ID3D11DeviceContext* context = nullptr;
    device->GetImmediateContext(&context);
    if (!context) {
        device->Release();
        return false;
    }

    g_device = device;
    g_context = context;
    g_gameSwapChain = swapChain;
    g_hwnd = desc.OutputWindow;

    CreateRenderTarget();
    if (!g_rtv) {
        g_context->Release();
        g_context = nullptr;
        g_device->Release();
        g_device = nullptr;
        g_gameSwapChain = nullptr;
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 7.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 4.0f;
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(8.0f, 5.0f);
    style.ItemSpacing = ImVec2(8.0f, 7.0f);

    if (!ImGui_ImplWin32_Init(g_hwnd) || !ImGui_ImplDX11_Init(g_device, g_context)) {
        ImGui::DestroyContext();
        ReleaseRenderTarget();
        g_context->Release();
        g_context = nullptr;
        g_device->Release();
        g_device = nullptr;
        g_gameSwapChain = nullptr;
        Log("ImGui backend initialization FAILED");
        return false;
    }

    SetLastError(0);
    g_originalWndProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrW(g_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(OverlayWndProc))
    );

    if (!g_originalWndProc && GetLastError() != 0) {
        Log("WndProc hook FAILED error=%lu", GetLastError());
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        ReleaseRenderTarget();
        g_context->Release();
        g_context = nullptr;
        g_device->Release();
        g_device = nullptr;
        g_gameSwapChain = nullptr;
        return false;
    }

    g_imguiReady.store(true);
    Log("ImGui D3D11 overlay READY hwnd=%p", g_hwnd);
    return true;
}

void DrawTunableFeature(
    const char* label,
    const char* id,
    bool* enabled,
    float* value,
    float minValue,
    float maxValue,
    float defaultValue,
    const char* format,
    const char* note
) {
    if (ImGui::Checkbox(label, enabled)) {
        g_config.Save();
    }

    ImGui::SameLine(310.0f);
    ImGui::TextDisabled("%s", note);

    if (*enabled) {
        ImGui::Indent();

        ImGui::SetNextItemWidth(245.0f);
        std::string sliderLabel = std::string("Value##Slider_") + id;
        bool changed = ImGui::SliderFloat(
            sliderLabel.c_str(),
            value,
            minValue,
            maxValue,
            format
        );

        ImGui::SameLine();
        ImGui::SetNextItemWidth(110.0f);
        std::string inputLabel = std::string("Manual##Input_") + id;
        if (ImGui::InputFloat(
            inputLabel.c_str(),
            value,
            0.0f,
            0.0f,
            "%.3f"
        )) {
            changed = true;
        }

        ImGui::SameLine();
        std::string defaultLabel = std::string("Default##Reset_") + id;
        if (ImGui::Button(defaultLabel.c_str())) {
            *value = defaultValue;
            changed = true;
        }

        if (changed) {
            if (*value < minValue) *value = minValue;
            if (*value > maxValue) *value = maxValue;
            g_config.Save();
        }

        ImGui::Unindent();
    }
}

void DrawToggleFeature(const char* label, bool* enabled, const char* note) {
    if (ImGui::Checkbox(label, enabled)) {
        g_config.Save();
    }
    ImGui::SameLine(310.0f);
    ImGui::TextDisabled("%s", note);
}

void DrawSectionTitle(const char* title) {
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("%s", title);
    ImGui::Separator();
    ImGui::Spacing();
}

void DrawOverlay() {
    ImGui::SetNextWindowSize(ImVec2(840.0f, 720.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(80.0f, 80.0f), ImGuiCond_FirstUseEver);

    bool open = true;
    if (!ImGui::Begin("Darksiders Genesis Enhanced", &open, ImGuiWindowFlags_NoCollapse)) {
        ImGui::End();
        if (!open) {
            g_overlayVisible.store(false);
        }
        return;
    }

    ImGui::Text("ASI Overlay  v%s", kBuild);
    ImGui::SameLine();
    const std::string menuKeyName = KeyDisplayName(g_config.menuKey);
    ImGui::TextDisabled("| %s to close", menuKeyName.c_str());
    ImGui::Separator();

    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("Gameplay")) {
            ImGui::Spacing();

            DrawSectionTitle("Player");

            if (ImGui::Checkbox("Toggle HUD", &g_config.toggleHudEnabled)) {
                g_config.Save();
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s",
                g_hudHookReady.load()
                    ? "Native ui.HideHud getter hooked"
                    : "Native hook unavailable"
            );

            if (g_config.toggleHudEnabled) {
                bool hudHidden = g_hudHidden.load();
                ImGui::Indent();
                if (ImGui::Checkbox("HUD Hidden##RuntimeHUD", &hudHidden)) {
                    g_hudHidden.store(hudHidden);
                    g_lastAction = std::string("HUD ") + (hudHidden ? "hidden" : "visible");
                    Log("Overlay -> HUD %s", hudHidden ? "HIDDEN" : "VISIBLE");
                }
                ImGui::SameLine();
                ImGui::TextDisabled("F1 default");
                ImGui::Unindent();
            }

            DrawTunableFeature(
                "Movement Speed",
                "MovementSpeed",
                &g_config.movementSpeedEnabled,
                &g_config.movementSpeedMultiplier,
                0.00f,
                3.00f,
                1.50f,
                "%.2fx",
                g_movementHookReady.load()
                    ? "Runtime hook active"
                    : "Native hook unavailable"
            );

            if (ImGui::Checkbox("Action Recovery", &g_config.actionRecoveryEnabled)) {
                g_config.Save();
                g_lastAction = std::string("Action Recovery ") +
                    (g_config.actionRecoveryEnabled ? "ON" : "OFF");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s",
                g_recoveryHookReady.load()
                    ? "AllowedActions MOVE, tail-only safety"
                    : "Native hook unavailable"
            );

            if (g_config.actionRecoveryEnabled) {
                ImGui::Indent();
                ImGui::SetNextItemWidth(280.0f);
                bool recoveryChanged = ImGui::SliderFloat(
                    "Recovery Delay##ActionRecovery",
                    &g_config.actionRecoveryDelayMs,
                    0.0f,
                    500.0f,
                    "%.0f ms"
                );
                ImGui::SameLine();
                if (ImGui::Button("Default##Reset_ActionRecovery")) {
                    g_config.actionRecoveryDelayMs = 0.0f;
                    recoveryChanged = true;
                }
                if (recoveryChanged) {
                    g_config.Save();
                }
                ImGui::TextDisabled(
                    "MOVE is never forced during STARTING/RUNNING. Only AWAITING_FINISH is shortened."
                );
                ImGui::TextDisabled(
                    "Queries %d | Local %d | Blocked %d | Forced %d",
                    g_actionMoveQueries.load(),
                    g_actionMoveLocalQueries.load(),
                    g_actionMoveNativeBlocked.load(),
                    g_actionMoveForced.load()
                );
                const int state = g_lastActionMoveState.load();
                if (state >= 0) {
                    ImGui::TextDisabled(
                        "Last ability: %s (%d) | elapsed %.3f s",
                        AbilityStateName(static_cast<unsigned char>(state)),
                        state,
                        g_lastActionMoveElapsed.load()
                    );
                }
                ImGui::Unindent();
            }

            DrawTunableFeature(
                "Jump Height",
                "JumpHeight",
                &g_config.jumpHeightEnabled,
                &g_config.jumpHeightMultiplier,
                0.00f,
                20.00f,
                1.25f,
                "%.2fx",
                g_movementHookReady.load()
                    ? "Runtime property hook | JumpZ + DoubleJumpZ"
                    : "Native movement hook unavailable"
            );

            DrawTunableFeature(
                "Glide / Flight Duration",
                "GlideDuration",
                &g_config.glideDurationEnabled,
                &g_config.glideDurationMultiplier,
                0.00f,
                100.00f,
                10.00f,
                "%.2fx",
                g_movementHookReady.load()
                    ? "Runtime property hook | GlideDurationSeconds"
                    : "Native movement hook unavailable"
            );

            DrawSectionTitle("Combat");

            DrawTunableFeature(
                "Pistol Damage",
                "PistolDamage",
                &g_config.pistolDamageEnabled,
                &g_config.pistolDamageMultiplier,
                0.00f,
                100.00f,
                2.00f,
                "%.2fx",
                g_finalOutgoingDamageHookReady.load()
                    ? "Final outgoing-damage hook | BaseJuice > 0"
                    : "Final outgoing-damage hook unavailable"
            );

            if (g_finalOutgoingDamageHookReady.load()) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Pistol events: %d | Last final %.2f -> %.2f | BaseJuice %.2f",
                    g_pistolDamageBoostCalls.load(),
                    g_lastNativePistolDamage.load(),
                    g_lastBoostedPistolDamage.load(),
                    g_lastPistolBaseJuice.load()
                );
                ImGui::Unindent();
            }

            DrawTunableFeature(
                "Melee Damage",
                "MeleeDamage",
                &g_config.meleeDamageEnabled,
                &g_config.meleeDamageMultiplier,
                0.00f,
                100.00f,
                2.00f,
                "%.2fx",
                g_finalOutgoingDamageHookReady.load()
                    ? "Final outgoing-damage hook | zero-juice diagnostic"
                    : "Final outgoing-damage hook unavailable"
            );

            if (g_finalOutgoingDamageHookReady.load()) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Melee-diag events: %d | Last final %.2f -> %.2f",
                    g_meleeDamageBoostCalls.load(),
                    g_lastNativeBaseDamage.load(),
                    g_lastBoostedBaseDamage.load()
                );
                ImGui::TextDisabled(
                    "Last DamageRecord: ScaleType %u | Tags %d",
                    g_lastOutgoingScaleType.load(),
                    g_lastOutgoingTagCount.load()
                );
                ImGui::Unindent();
            }

            DrawTunableFeature(
                "Hotstreak Charge",
                "HotstreakCharge",
                &g_config.hotstreakChargeEnabled,
                &g_config.hotstreakChargeMultiplier,
                0.00f,
                25.00f,
                2.00f,
                "%.2fx",
                g_hotstreakHookReady.load()
                    ? "Runtime AddJuice hook | local positive gains"
                    : "Native AddJuice hook unavailable"
            );

            if (g_hotstreakHookReady.load()) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Boost calls: %d | Last gain %.2f -> %.2f",
                    g_hotstreakBoostCalls.load(),
                    g_lastNativeJuiceGain.load(),
                    g_lastBoostedJuiceGain.load()
                );
                ImGui::Unindent();
            }

            DrawSectionTitle("Horse");

            const dg::horse::Telemetry horseTelemetry =
                dg::horse::GetTelemetry();

            DrawTunableFeature(
                "Horse Speed",
                "HorseSpeed",
                &g_config.horseSpeedEnabled,
                &g_config.horseSpeedMultiplier,
                0.00f,
                3.00f,
                1.25f,
                "%.2fx",
                horseTelemetry.validated
                    ? "Validated on existing shared GetMaxSpeed hook"
                    : "Waiting for exact horse signature 1300/600 + stamina"
            );

            DrawTunableFeature(
                "Horse Sprint Speed",
                "HorseSprintSpeed",
                &g_config.horseSprintSpeedEnabled,
                &g_config.horseSprintSpeedMultiplier,
                0.00f,
                3.00f,
                1.25f,
                "%.2fx",
                "Pending dedicated RunSpeed primitive"
            );

            DrawTunableFeature(
                "Horse Sprint Duration",
                "HorseSprintDuration",
                &g_config.horseSprintDurationEnabled,
                &g_config.horseSprintDurationMultiplier,
                0.00f,
                10.00f,
                2.00f,
                "%.2fx",
                horseTelemetry.staminaReady
                    ? "Validated StaminaSprintPercentageRate"
                    : "Waiting for exact horse stamina signature"
            );

            ImGui::Indent();
            ImGui::TextDisabled(
                "Horse: %s | candidate snapshots %u | matches %u",
                horseTelemetry.validated ? "VALIDATED" : "waiting",
                horseTelemetry.uniqueCandidatesLogged,
                horseTelemetry.candidateMatches
            );
            ImGui::TextDisabled(
                "MaxWalkSpeed %.1f -> %.1f | MaxAcceleration %.1f -> %.1f",
                horseTelemetry.nativeMaxWalkSpeed,
                horseTelemetry.appliedMaxWalkSpeed,
                horseTelemetry.nativeMaxAcceleration,
                horseTelemetry.appliedMaxAcceleration
            );
            ImGui::TextDisabled(
                "Sprint drain %.3f -> %.3f | owner %p | movement %p",
                horseTelemetry.nativeSprintDrain,
                horseTelemetry.appliedSprintDrain,
                horseTelemetry.horseOwner,
                horseTelemetry.horseMovement
            );
            ImGui::Unindent();

            DrawSectionTitle("Camera");

            DrawTunableFeature(
                "FOV",
                "FOV",
                &g_config.fovEnabled,
                &g_config.fovDegrees,
                60.0f,
                140.0f,
                90.0f,
                "%.0f deg",
                "Pending camera hook"
            );

            DrawTunableFeature(
                "Third Person",
                "ThirdPerson",
                &g_config.thirdPersonEnabled,
                &g_config.thirdPersonDistanceMultiplier,
                0.00f,
                3.00f,
                1.00f,
                "%.2fx",
                "Pending camera hook | distance"
            );

            DrawSectionTitle("System");

            if (ImGui::Checkbox("Skip Intro Videos", &g_config.skipIntroEnabled)) {
                g_config.Save();
                ApplySkipIntroSetting(true);
                g_lastAction = std::string("Skip Intro ") +
                    (g_config.skipIntroEnabled ? "ON" : "OFF");
            }
            ImGui::SameLine(310.0f);
            ImGui::TextDisabled(
                "%s",
                g_skipIntroReady.load()
                    ? "Native g.PlayIntroCinematicOnBoot control | restart applies boot state"
                    : "Native CVar control unavailable"
            );

            if (g_skipIntroReady.load() && g_skipIntroData) {
                ImGui::Indent();
                ImGui::TextDisabled(
                    "Native CVar now: %ld | vanilla captured: %ld",
                    *g_skipIntroData,
                    g_skipIntroOriginalValue
                );
                ImGui::Unindent();
            }

            ImGui::Spacing();
            ImGui::TextDisabled(
                "Checkboxes and values are authoritative and persisted to DarksidersGenesisMod.ini."
            );

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Hotkeys")) {
            ImGui::Spacing();

            ImGui::Text("Menu key");
            ImGui::Text("Open / close: %s", menuKeyName.c_str());
            ImGui::SameLine(280.0f);
            if (g_captureMenuKey.load()) {
                ImGui::TextDisabled("Press a key...  Esc = cancel");
            } else if (ImGui::Button("Rebind Menu Key", ImVec2(160.0f, 0.0f))) {
                g_captureMenuKey.store(true);
                g_lastAction = "Waiting for new menu key";
                Log("Menu key capture started");
            }

            ImGui::Spacing();
            if (ImGui::Button("Save", ImVec2(110.0f, 0.0f))) {
                g_config.Save();
                g_lastAction = "Configuration saved";
            }
            ImGui::SameLine();
            if (ImGui::Button("Reload", ImVec2(110.0f, 0.0f))) {
                g_config.Load();
                g_lastAction = "Configuration reloaded";
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset Defaults", ImVec2(140.0f, 0.0f))) {
                g_config.ResetDefaults(true);
                g_hudHidden.store(false);
                g_lastAction = "Defaults restored";
            }

            DrawSectionTitle("F1-F12");

            ImGui::TextWrapped(
                "Each physical F-key slot can be reassigned to any implemented mod action or None."
            );
            ImGui::Spacing();

            if (ImGui::BeginTable("HotkeyTable", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 90.0f);
                ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                for (int i = 0; i < 12; ++i) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("F%d", i + 1);

                    ImGui::TableSetColumnIndex(1);
                    char comboId[32]{};
                    sprintf_s(comboId, sizeof(comboId), "##HotkeyF%d", i + 1);

                    int current = static_cast<int>(g_config.hotkeys[static_cast<size_t>(i)]);
                    if (current < 0 || current >= static_cast<int>(Action::Count)) {
                        current = 0;
                    }

                    ImGui::SetNextItemWidth(-1.0f);
                    if (ImGui::BeginCombo(comboId, kActionLabels[static_cast<size_t>(current)])) {
                        for (int a = 0; a < static_cast<int>(Action::Count); ++a) {
                            const bool selected = current == a;
                            if (ImGui::Selectable(kActionLabels[static_cast<size_t>(a)], selected)) {
                                g_config.hotkeys[static_cast<size_t>(i)] = static_cast<Action>(a);
                                g_config.Save();
                            }
                            if (selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                }

                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::TextDisabled("Default: F1 HUD | F2 Movement | F3 Recovery | F4 Skip Intro | F5 Third Person");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("About")) {
            ImGui::Spacing();
            ImGui::Text("Darksiders Genesis Enhanced - experimental ASI core");
            ImGui::Spacing();
            ImGui::TextWrapped(
                "Target executable: DarksidersGenesis-Win64-Shipping.exe"
            );
            ImGui::Text(
                "Target validation: %s",
                g_targetValidation.exact ? "EXACT / gameplay enabled" : "MISMATCH / overlay-only"
            );
            ImGui::TextWrapped(
                "Runtime SHA-256: %s",
                g_targetValidation.sha256.empty()
                    ? "(not available)"
                    : g_targetValidation.sha256.c_str()
            );
            ImGui::TextWrapped(
                "Expected SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54"
            );
            ImGui::TextWrapped("Size: 62,113,280 bytes");
            ImGui::Spacing();
            ImGui::TextWrapped(
                "V0.8B keeps AllowedActions/MOVE as the proven recovery path but no longer "
                "forces MOVE during RUNNING abilities. The chest interaction regression from "
                "V0.8A is specifically protected by the tail-only policy."
            );
            ImGui::Spacing();
            ImGui::TextWrapped(
                "Reference PAK audits recovered concrete targets for jump, glide, horse stamina, "
                "projectile damage and Hotstreak/juice. Numeric options now expose both a slider "
                "and a manual input field; multiplier minima use 0. Pistol Damage V0.11A is rejected "
                "and remains pending a better filter."
            );
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();
    ImGui::Text("Last action: %s", g_lastAction.c_str());

    ImGui::End();

    if (!open) {
        g_overlayVisible.store(false);
    }
}

HRESULT __stdcall HookPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
    ApplySkipIntroSetting(false);
    ProcessInput();

    if (!g_imguiReady.load()) {
        InitializeImGui(swapChain);
    }

    if (g_imguiReady.load() && swapChain == g_gameSwapChain && g_overlayVisible.load()) {
        // UE4 can clip or hide the cursor during gameplay. Release clipping every
        // overlay frame and let ImGui draw its own pointer.
        ClipCursor(nullptr);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::GetIO().MouseDrawCursor = true;
        DrawOverlay();

        ImGui::Render();

        if (g_rtv) {
            g_context->OMSetRenderTargets(1, &g_rtv, nullptr);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    } else if (g_imguiReady.load()) {
        ImGui::GetIO().MouseDrawCursor = false;
    }

    return g_originalPresent
        ? g_originalPresent(swapChain, syncInterval, flags)
        : S_OK;
}

HRESULT __stdcall HookResizeBuffers(
    IDXGISwapChain* swapChain,
    UINT bufferCount,
    UINT width,
    UINT height,
    DXGI_FORMAT newFormat,
    UINT swapChainFlags
) {
    if (g_imguiReady.load() && swapChain == g_gameSwapChain) {
        ImGui_ImplDX11_InvalidateDeviceObjects();
        ReleaseRenderTarget();
    }

    const HRESULT hr = g_originalResizeBuffers
        ? g_originalResizeBuffers(swapChain, bufferCount, width, height, newFormat, swapChainFlags)
        : E_FAIL;

    if (SUCCEEDED(hr) && g_imguiReady.load() && swapChain == g_gameSwapChain) {
        CreateRenderTarget();
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    return hr;
}

LRESULT CALLBACK ProbeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

bool DiscoverAndHookD3D11() {
    const wchar_t* className = L"DarksidersGenesisModProbeWindow";

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = ProbeWndProc;
    wc.hInstance = g_module;
    wc.lpszClassName = className;

    RegisterClassExW(&wc);

    HWND window = CreateWindowExW(
        0,
        className,
        L"DG probe",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        100,
        100,
        nullptr,
        nullptr,
        g_module,
        nullptr
    );

    if (!window) {
        Log("Probe window creation FAILED error=%lu", GetLastError());
        UnregisterClassW(className, g_module);
        return false;
    }

    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 100;
    sd.BufferDesc.Height = 100;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL requested[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    IDXGISwapChain* probeSwap = nullptr;
    ID3D11Device* probeDevice = nullptr;
    ID3D11DeviceContext* probeContext = nullptr;
    D3D_FEATURE_LEVEL obtained{};

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        requested,
        ARRAYSIZE(requested),
        D3D11_SDK_VERSION,
        &sd,
        &probeSwap,
        &probeDevice,
        &obtained,
        &probeContext
    );

    if (FAILED(hr)) {
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            requested,
            ARRAYSIZE(requested),
            D3D11_SDK_VERSION,
            &sd,
            &probeSwap,
            &probeDevice,
            &obtained,
            &probeContext
        );
    }

    if (FAILED(hr) || !probeSwap) {
        Log("D3D11 probe creation FAILED hr=0x%08lX", static_cast<unsigned long>(hr));
        if (probeContext) probeContext->Release();
        if (probeDevice) probeDevice->Release();
        DestroyWindow(window);
        UnregisterClassW(className, g_module);
        return false;
    }

    void** vtable = *reinterpret_cast<void***>(probeSwap);
    void* presentAddress = vtable[8];
    void* resizeBuffersAddress = vtable[13];

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        probeSwap->Release();
        probeContext->Release();
        probeDevice->Release();
        DestroyWindow(window);
        UnregisterClassW(className, g_module);
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        presentAddress,
        reinterpret_cast<LPVOID>(&HookPresent),
        reinterpret_cast<LPVOID*>(&g_originalPresent)
    );
    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("Present hook creation FAILED status=%d", static_cast<int>(status));
        probeSwap->Release();
        probeContext->Release();
        probeDevice->Release();
        DestroyWindow(window);
        UnregisterClassW(className, g_module);
        return false;
    }

    status = MH_CreateHook(
        resizeBuffersAddress,
        reinterpret_cast<LPVOID>(&HookResizeBuffers),
        reinterpret_cast<LPVOID*>(&g_originalResizeBuffers)
    );
    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("ResizeBuffers hook creation FAILED status=%d", static_cast<int>(status));
        probeSwap->Release();
        probeContext->Release();
        probeDevice->Release();
        DestroyWindow(window);
        UnregisterClassW(className, g_module);
        return false;
    }

    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK) {
        Log("MinHook enable FAILED status=%d", static_cast<int>(status));
        probeSwap->Release();
        probeContext->Release();
        probeDevice->Release();
        DestroyWindow(window);
        UnregisterClassW(className, g_module);
        return false;
    }

    Log("D3D11 hooks installed Present=%p ResizeBuffers=%p", presentAddress, resizeBuffersAddress);

    probeSwap->Release();
    probeContext->Release();
    probeDevice->Release();
    DestroyWindow(window);
    UnregisterClassW(className, g_module);
    return true;
}

void ShutdownMod() {
    if (g_shuttingDown.exchange(true)) {
        return;
    }

    Log("Shutdown: begin");

    dg::horse::Shutdown();
    dg::player::ClearIdentity();

    if (g_skipIntroData) {
        *g_skipIntroData = g_skipIntroOriginalValue;
    }

    if (g_originalWndProc && g_hwnd && IsWindow(g_hwnd)) {
        SetWindowLongPtrW(
            g_hwnd,
            GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(g_originalWndProc)
        );
        g_originalWndProc = nullptr;
    }

    if (g_imguiReady.exchange(false)) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }
    }

    ReleaseRenderTarget();

    if (g_context) {
        g_context->Release();
        g_context = nullptr;
    }
    if (g_device) {
        g_device->Release();
        g_device = nullptr;
    }
    g_gameSwapChain = nullptr;
    g_hwnd = nullptr;

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    Log("Shutdown: complete");
}

DWORD WINAPI MainThread(LPVOID) {
    InitializePaths();
    ResetLogFile();
    Log("============================================================");
    Log("Darksiders Genesis Enhanced ASI %s starting", kBuild);
    Log("Architecture: DXGI proxy -> ASI -> D3D11 Present/ResizeBuffers -> Dear ImGui");

    std::atexit(&ShutdownMod);

    g_config.Load();

    g_targetValidation = dg::target::ValidateCurrentExecutable();
    g_gameplayHooksAllowed.store(g_targetValidation.exact);

    Log(
        "Target validation: exact=%d size=%llu sha256=%s reason=%s",
        g_targetValidation.exact ? 1 : 0,
        static_cast<unsigned long long>(g_targetValidation.fileSize),
        g_targetValidation.sha256.empty()
            ? "(unavailable)"
            : g_targetValidation.sha256.c_str(),
        g_targetValidation.reason.c_str()
    );

    // The overlay remains available on an unknown executable so users receive
    // a useful compatibility diagnostic. Gameplay hooks are fail-closed.
    if (!DiscoverAndHookD3D11()) {
        Log("Overlay hook setup FAILED. Mod stays fail-open; game should continue normally.");
        return 0;
    }

    if (!g_targetValidation.exact) {
        Log("Target mismatch: gameplay hooks DISABLED; overlay/log only.");
        return 0;
    }

    dg::horse::Initialize(&FeatureLog);

    if (!InstallSkipIntroControl()) {
        Log("Skip Intro unavailable; continuing with remaining ASI features.");
    }
    if (!InstallHudHook()) {
        Log("Toggle HUD unavailable; renderer/input core remains active.");
    }
    if (!InstallMovementSpeedHook()) {
        Log("Movement Speed unavailable; other ASI features remain active.");
    }
    if (!InstallActionEnabledRecoveryDiagnostic()) {
        Log("Action Recovery V0.8 unavailable; other ASI features remain active.");
    }
    if (!InstallHotstreakChargeHook()) {
        Log("Hotstreak Charge unavailable; other ASI features remain active.");
    }
    if (!InstallFinalOutgoingDamageHook()) {
        Log("Final Pistol/Melee Damage hook unavailable; other ASI features remain active.");
    }

    Log(
        "Core initialization complete. Press %s after the first game frame.",
        KeyDisplayName(g_config.menuKey).c_str()
    );
    return 0;
}

} // namespace

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        g_module = module;
        DisableThreadLibraryCalls(module);

        HANDLE thread = CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
        if (thread) {
            CloseHandle(thread);
        }
    }

    return TRUE;
}
