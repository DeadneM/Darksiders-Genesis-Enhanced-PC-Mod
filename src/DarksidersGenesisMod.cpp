#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include <MinHook.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <array>
#include <atomic>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <string>

namespace {

constexpr const char* kBuild = "0.8A-action-enabled-move-diagnostic";
constexpr const wchar_t* kIniName = L"DarksidersGenesisMod.ini";
constexpr const wchar_t* kLogName = L"DarksidersGenesisMod.log";

HMODULE g_module = nullptr;
std::wstring g_iniPath;
std::wstring g_logPath;

using PresentFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
using ResizeBuffersFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
using HudHiddenGetterFn = bool(*)();
using CharacterGetMaxSpeedFn = float(*)(void*);
using ActionGateFn = bool(*)(void*, unsigned char);
using DashVoidFn = void(*)(void*);
using DashTickFn = void(*)(void*, float);
using InputSuppressNotifyFn = void(*)(void*, void*, void*);
using AbilityInterruptEnabledFn = bool(*)(void*, unsigned char);
using AbilityActionEnabledFn = bool(*)(void*, unsigned char);

PresentFn g_originalPresent = nullptr;
ResizeBuffersFn g_originalResizeBuffers = nullptr;
HudHiddenGetterFn g_originalHudHiddenGetter = nullptr;
CharacterGetMaxSpeedFn g_originalCharacterGetMaxSpeed = nullptr;
ActionGateFn g_originalActionGate = nullptr;
DashVoidFn g_originalDashStart = nullptr;
DashTickFn g_originalDashTick = nullptr;
DashVoidFn g_originalDashFinish = nullptr;
InputSuppressNotifyFn g_originalInputSuppressBegin = nullptr;
InputSuppressNotifyFn g_originalInputSuppressEnd = nullptr;
AbilityInterruptEnabledFn g_originalAbilityInterruptEnabled = nullptr;
AbilityActionEnabledFn g_originalAbilityActionEnabled = nullptr;

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
std::atomic<void*> g_activeDashAbility{nullptr};
std::atomic<float> g_dashElapsedSeconds{0.0f};
std::atomic<float> g_lastNativeDashDuration{0.0f};
std::atomic_bool g_dashEarlyUnlockApplied{false};
std::atomic_int g_bypassedInputSuppressWindows{0};
std::atomic_int g_moveInterruptQueries{0};
std::atomic_int g_moveInterruptNativeBlocked{0};
std::atomic_int g_moveInterruptForced{0};
std::atomic<void*> g_lastMoveQueryAbility{nullptr};
std::atomic_int g_lastMoveQueryState{-1};
std::atomic<float> g_lastMoveQueryElapsed{0.0f};
std::atomic<void*> g_localPlayerCharacter{nullptr};
std::atomic_int g_actionMoveQueries{0};
std::atomic_int g_actionMoveLocalQueries{0};
std::atomic_int g_actionMoveNativeBlocked{0};
std::atomic_int g_actionMoveForced{0};
std::atomic<void*> g_lastActionMoveAbility{nullptr};
std::atomic_int g_lastActionMoveState{-1};
std::atomic<float> g_lastActionMoveElapsed{0.0f};
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

struct Config {
    bool overlayEnabled = true;
    int menuKey = VK_INSERT;

    // Requested default policy: every planned feature is enabled by default.
    bool toggleHudEnabled = true;
    bool movementSpeedEnabled = true;
    bool actionRecoveryEnabled = true;
    bool skipIntroEnabled = true;
    bool thirdPersonEnabled = true;

    // Tentative user-facing tuning values. They are inert until their gameplay
    // hooks are implemented and validated.
    float movementSpeedMultiplier = 1.15f;
    float actionRecoveryMultiplier = 2.00f; // legacy V0.5A value
    float dodgeEarlyUnlockMs = 100.0f;

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
        movementSpeedMultiplier = 1.15f;
        actionRecoveryMultiplier = 2.00f;
        dodgeEarlyUnlockMs = 100.0f;

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

        movementSpeedMultiplier = ReadFloat(L"Values", L"MovementSpeedMultiplier", 1.15f, g_iniPath);
        actionRecoveryMultiplier = ReadFloat(L"Values", L"ActionRecoveryMultiplier", 2.00f, g_iniPath);
        dodgeEarlyUnlockMs = ReadFloat(L"Values", L"DodgeEarlyUnlockMs", 100.0f, g_iniPath);

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

        WriteFloat(L"Values", L"MovementSpeedMultiplier", movementSpeedMultiplier, g_iniPath);
        WriteFloat(L"Values", L"ActionRecoveryMultiplier", actionRecoveryMultiplier, g_iniPath);
        WriteFloat(L"Values", L"DodgeEarlyUnlockMs", dodgeEarlyUnlockMs, g_iniPath);

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

bool IsLocallyControlledMayhemCharacter(void* character) {
    if (!character) {
        return false;
    }

    void** vtable = *reinterpret_cast<void***>(character);
    if (!vtable) {
        return false;
    }

    // APawn::IsLocallyControlled virtual slot in the audited UE4 build.
    using IsLocallyControlledFn = bool(*)(void*);
    auto fn = reinterpret_cast<IsLocallyControlledFn>(vtable[0x680 / sizeof(void*)]);
    if (!fn) {
        return false;
    }

    return fn(character);
}

float HookCharacterGetMaxSpeed(void* movementComponent) {
    const float nativeSpeed = g_originalCharacterGetMaxSpeed
        ? g_originalCharacterGetMaxSpeed(movementComponent)
        : 0.0f;

    if (nativeSpeed <= 0.0f || !movementComponent) {
        return nativeSpeed;
    }

    BYTE* component = reinterpret_cast<BYTE*>(movementComponent);

    // UCharacterMovementComponent::CharacterOwner reflection offset.
    void* characterOwner = *reinterpret_cast<void**>(component + 0x190);
    if (!characterOwner || !IsLocallyControlledMayhemCharacter(characterOwner)) {
        return nativeSpeed;
    }

    g_localPlayerCharacter.store(characterOwner);

    if (!g_config.movementSpeedEnabled) {
        return nativeSpeed;
    }

    // EMovementMode:
    // 1 = Walking, 2 = NavWalking. Do not modify falling/swimming/flying/custom.
    const unsigned char movementMode = *(component + 0x1B0);
    if (movementMode != 1 && movementMode != 2) {
        return nativeSpeed;
    }

    float multiplier = g_config.movementSpeedMultiplier;
    if (multiplier < 0.10f) multiplier = 0.10f;
    if (multiplier > 5.00f) multiplier = 5.00f;

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

BYTE* ResolveActionRecoveryGate() {
    PeSectionView text{};
    if (!GetMainModuleSection(".text", text)) {
        Log("Recovery hook: failed to enumerate .text");
        return nullptr;
    }

    // UMayhemPlayerAbilityComponent movement gate, audited against the supplied EXE.
    // ECharacterActions::MOVE == 0x1D.
    // The native function compares:
    //   MoveInterruptDelaySec [this+0x110]
    //   elapsed runtime timer [this+0x114]
    // and rejects MOVE while delay > elapsed.
    static constexpr int kPattern[] = {
        0x40, 0x57,
        0x48, 0x83, 0xEC, 0x20,
        0x0F, 0xB6, 0xFA,
        0x80, 0xFA, 0x1D,
        0x75, -1,
        0xF3, 0x0F, 0x10, 0x81, 0x10, 0x01, 0x00, 0x00,
        0x0F, 0x2F, 0x81, 0x14, 0x01, 0x00, 0x00,
        0x76, -1,
        0x32, 0xC0
    };

    size_t matchCount = 0;
    BYTE* target = FindUniquePattern(
        text,
        kPattern,
        ARRAYSIZE(kPattern),
        &matchCount
    );

    if (!target) {
        Log("Recovery hook: movement-gate signature match count=%zu", matchCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Recovery hook: movement gate resolved RVA=0x%zX",
        static_cast<size_t>(target - base)
    );

    return target;
}

bool HookActionGate(void* abilityComponent, unsigned char action) {
    if (!g_originalActionGate) {
        return false;
    }

    constexpr unsigned char kMoveAction = 0x1D;

    if (action != kMoveAction ||
        !g_config.actionRecoveryEnabled ||
        !abilityComponent) {
        return g_originalActionGate(abilityComponent, action);
    }

    float multiplier = g_config.actionRecoveryMultiplier;
    if (multiplier < 1.0f) multiplier = 1.0f;
    if (multiplier > 10.0f) multiplier = 10.0f;

    if (multiplier <= 1.0001f) {
        return g_originalActionGate(abilityComponent, action);
    }

    BYTE* object = reinterpret_cast<BYTE*>(abilityComponent);
    float* moveInterruptDelay = reinterpret_cast<float*>(object + 0x110);
    float* elapsedTimer = reinterpret_cast<float*>(object + 0x114);

    const float originalDelay = *moveInterruptDelay;
    const float elapsed = *elapsedTimer;

    // Reject obviously invalid/corrupt values and fall back to vanilla logic.
    if (!(originalDelay >= 0.0f && originalDelay < 60.0f) ||
        !(elapsed >= 0.0f && elapsed < 600.0f)) {
        return g_originalActionGate(abilityComponent, action);
    }

    const float effectiveDelay = originalDelay / multiplier;

    // Temporary, stack-scoped override only for this native gate evaluation.
    // The original object value is restored immediately after the game finishes
    // its full action checks.
    *moveInterruptDelay = effectiveDelay;
    const bool result = g_originalActionGate(abilityComponent, action);
    *moveInterruptDelay = originalDelay;

    return result;
}

bool InstallActionRecoveryHook() {
    BYTE* target = ResolveActionRecoveryGate();
    if (!target) {
        Log("Recovery hook: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("Recovery hook: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookActionGate),
        reinterpret_cast<LPVOID*>(&g_originalActionGate)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("Recovery hook: create FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("Recovery hook: enable FAILED status=%d", static_cast<int>(status));
        return false;
    }

    g_recoveryHookReady.store(true);
    Log(
        "Recovery hook: READY multiplier=%.3fx move-action-only=1 field-write=purely-temporary",
        g_config.actionRecoveryMultiplier
    );
    return true;
}


struct DashHookTargets {
    BYTE* start = nullptr;
    BYTE* tick = nullptr;
    BYTE* finish = nullptr;
};

DashHookTargets ResolveDashRecoveryTargets() {
    DashHookTargets out{};

    PeSectionView text{};
    PeSectionView rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        Log("Dodge recovery: failed to enumerate PE sections");
        return out;
    }

    static constexpr int kStartPattern[] = {
        0x40, 0x57,
        0x48, 0x83, 0xEC, 0x60,
        0x48, 0xC7, 0x44, 0x24, 0x30, 0xFE, 0xFF, 0xFF, 0xFF,
        0x48, 0x89, 0x5C, 0x24, 0x70,
        0x48, 0x89, 0x74, 0x24, 0x78,
        0x48, 0x8B, 0xF9,
        0xE8, -1, -1, -1, -1,
        0x48, 0x8B, 0x9F, 0xB0, 0x01, 0x00, 0x00,
        0x48, 0x85, 0xDB,
        0x0F, 0x84, -1, -1, -1, -1,
        0x80, 0xBF, 0xF1, 0x01, 0x00, 0x00, 0x00,
        0x74, 0x07,
        0xC6, 0x83, 0xAC, 0x0A, 0x00, 0x00, 0x01
    };

    static constexpr int kFinishPattern[] = {
        0x40, 0x57,
        0x48, 0x83, 0xEC, 0x60,
        0x48, 0xC7, 0x44, 0x24, 0x30, 0xFE, 0xFF, 0xFF, 0xFF,
        0x48, 0x89, 0x5C, 0x24, 0x70,
        0x48, 0x8B, 0xF9,
        0xE8, -1, -1, -1, -1,
        0x48, 0x8B, 0x8F, 0xD8, 0x01, 0x00, 0x00,
        0x48, 0x85, 0xC9,
        0x74, -1,
        0x33, 0xD2,
        0xE8, -1, -1, -1, -1,
        0x48, 0x8B, 0x9F, 0xB0, 0x01, 0x00, 0x00,
        0x48, 0x85, 0xDB,
        0x0F, 0x84, -1, -1, -1, -1,
        0xC6, 0x83, 0xAC, 0x0A, 0x00, 0x00, 0x00
    };

    size_t startCount = 0;
    size_t finishCount = 0;
    out.start = FindUniquePattern(text, kStartPattern, ARRAYSIZE(kStartPattern), &startCount);
    out.finish = FindUniquePattern(text, kFinishPattern, ARRAYSIZE(kFinishPattern), &finishCount);

    if (!out.start || !out.finish) {
        Log(
            "Dodge recovery: start/finish signature mismatch start=%zu finish=%zu",
            startCount,
            finishCount
        );
        return {};
    }

    BYTE* commonTick = nullptr;
    size_t relationCount = 0;

    for (size_t i = 0; i + 0x60 <= rdata.size; i += sizeof(uintptr_t)) {
        BYTE* p = rdata.begin + i;
        const uintptr_t startPtr = *reinterpret_cast<const uintptr_t*>(p);
        const uintptr_t finishPtr = *reinterpret_cast<const uintptr_t*>(p + 0x58);

        if (startPtr != reinterpret_cast<uintptr_t>(out.start) ||
            finishPtr != reinterpret_cast<uintptr_t>(out.finish)) {
            continue;
        }

        BYTE* tick = reinterpret_cast<BYTE*>(
            *reinterpret_cast<const uintptr_t*>(p + sizeof(uintptr_t))
        );

        if (!AddressInSection(text, tick)) {
            continue;
        }

        if (!commonTick) {
            commonTick = tick;
        } else if (commonTick != tick) {
            Log("Dodge recovery: vtable relation resolves conflicting tick targets");
            return {};
        }

        ++relationCount;
    }

    if (!commonTick || relationCount == 0) {
        Log("Dodge recovery: no start/tick/finish vtable relation found");
        return {};
    }

    out.tick = commonTick;

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Dodge recovery: targets start=0x%zX tick=0x%zX finish=0x%zX vtableRelations=%zu",
        static_cast<size_t>(out.start - base),
        static_cast<size_t>(out.tick - base),
        static_cast<size_t>(out.finish - base),
        relationCount
    );

    return out;
}

void HookDashStart(void* ability) {
    g_originalDashStart(ability);

    if (!ability) {
        return;
    }

    BYTE* object = reinterpret_cast<BYTE*>(ability);
    void* player = *reinterpret_cast<void**>(object + 0x1B0);
    if (!player) {
        return;
    }

    BYTE* movementLock = reinterpret_cast<BYTE*>(player) + 0xAAC;
    if (*movementLock == 0) {
        return;
    }

    g_activeDashAbility.store(ability);
    g_dashElapsedSeconds.store(0.0f);
    g_dashEarlyUnlockApplied.store(false);

    Log(
        "Dodge recovery: dash START ability=%p player=%p learnedNative=%.3f sec",
        ability,
        player,
        g_lastNativeDashDuration.load()
    );
}

void HookDashTick(void* ability, float deltaSeconds) {
    if (ability == g_activeDashAbility.load() &&
        deltaSeconds > 0.0f &&
        deltaSeconds < 0.25f) {
        g_dashElapsedSeconds.store(g_dashElapsedSeconds.load() + deltaSeconds);
    }

    g_originalDashTick(ability, deltaSeconds);

    if (!g_config.actionRecoveryEnabled ||
        ability != g_activeDashAbility.load() ||
        g_dashEarlyUnlockApplied.load()) {
        return;
    }

    const float nativeDuration = g_lastNativeDashDuration.load();
    if (nativeDuration <= 0.05f) {
        return;
    }

    float earlyMs = g_config.dodgeEarlyUnlockMs;
    if (earlyMs < 0.0f) earlyMs = 0.0f;
    if (earlyMs > 400.0f) earlyMs = 400.0f;

    float releaseAt = nativeDuration - (earlyMs / 1000.0f);
    if (releaseAt < 0.05f) {
        releaseAt = 0.05f;
    }

    const float elapsed = g_dashElapsedSeconds.load();
    if (elapsed < releaseAt) {
        return;
    }

    BYTE* object = reinterpret_cast<BYTE*>(ability);
    void* player = *reinterpret_cast<void**>(object + 0x1B0);
    if (!player) {
        return;
    }

    BYTE* movementLock = reinterpret_cast<BYTE*>(player) + 0xAAC;
    if (*movementLock != 0) {
        *movementLock = 0;
        g_dashEarlyUnlockApplied.store(true);
        Log(
            "Dodge recovery: EARLY UNLOCK at %.3f sec (native %.3f sec, early %.0f ms)",
            elapsed,
            nativeDuration,
            earlyMs
        );
    }
}

void HookDashFinish(void* ability) {
    if (ability && ability == g_activeDashAbility.load()) {
        const float elapsed = g_dashElapsedSeconds.load();
        if (elapsed > 0.05f && elapsed < 5.0f) {
            g_lastNativeDashDuration.store(elapsed);
        }

        Log(
            "Dodge recovery: dash FINISH nativeLifetime=%.3f sec earlyUnlock=%d",
            elapsed,
            g_dashEarlyUnlockApplied.load() ? 1 : 0
        );

        g_activeDashAbility.store(nullptr);
        g_dashElapsedSeconds.store(0.0f);
        g_dashEarlyUnlockApplied.store(false);
    }

    g_originalDashFinish(ability);
}

bool InstallDodgeRecoveryHooks() {
    const DashHookTargets targets = ResolveDashRecoveryTargets();
    if (!targets.start || !targets.tick || !targets.finish) {
        Log("Dodge recovery: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("Dodge recovery: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    struct HookSpec {
        BYTE* target;
        LPVOID detour;
        LPVOID* original;
        const char* name;
    };

    HookSpec specs[] = {
        { targets.start, reinterpret_cast<LPVOID>(&HookDashStart), reinterpret_cast<LPVOID*>(&g_originalDashStart), "start" },
        { targets.tick, reinterpret_cast<LPVOID>(&HookDashTick), reinterpret_cast<LPVOID*>(&g_originalDashTick), "tick" },
        { targets.finish, reinterpret_cast<LPVOID>(&HookDashFinish), reinterpret_cast<LPVOID*>(&g_originalDashFinish), "finish" }
    };

    for (const auto& spec : specs) {
        MH_STATUS status = MH_CreateHook(spec.target, spec.detour, spec.original);
        if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
            Log("Dodge recovery: create %s FAILED status=%d", spec.name, static_cast<int>(status));
            return false;
        }
    }

    for (const auto& spec : specs) {
        MH_STATUS status = MH_EnableHook(spec.target);
        if (status != MH_OK && status != MH_ERROR_ENABLED) {
            Log("Dodge recovery: enable %s FAILED status=%d", spec.name, static_cast<int>(status));
            return false;
        }
    }

    g_recoveryHookReady.store(true);
    Log(
        "Dodge recovery: READY earlyUnlock=%.0f ms firstDashLearnsNative=1",
        g_config.dodgeEarlyUnlockMs
    );
    return true;
}


struct InputSuppressWindowTargets {
    BYTE* begin = nullptr;
    BYTE* end = nullptr;
};

InputSuppressWindowTargets ResolveInputSuppressWindowTargets() {
    InputSuppressWindowTargets out{};

    PeSectionView text{};
    PeSectionView rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        Log("InputSuppressWindow: failed to enumerate PE sections");
        return out;
    }

    BYTE* displayNameString = FindWideString(rdata, L"Suppress Player Input Window");
    if (!displayNameString) {
        Log("InputSuppressWindow: display-name string not found");
        return out;
    }

    BYTE* nameXref = FindRipRelativeLeaTo(text, displayNameString);
    if (!nameXref) {
        Log("InputSuppressWindow: unique display-name xref not found");
        return out;
    }

    // UAnimNotify_InputSuppressWindow::GetNotifyName starts 9 bytes before
    // the audited LEA of "Suppress Player Input Window".
    BYTE* getNotifyName = nameXref - 9;
    static constexpr BYTE kGetNamePrefix[] = {
        0x40, 0x53,
        0x48, 0x83, 0xEC, 0x20,
        0x48, 0x8B, 0xDA
    };

    if (!AddressInSection(text, getNotifyName) ||
        memcmp(getNotifyName, kGetNamePrefix, sizeof(kGetNamePrefix)) != 0) {
        Log("InputSuppressWindow: GetNotifyName layout mismatch");
        return out;
    }

    BYTE* vtableSlot = nullptr;
    size_t slotCount = 0;

    for (size_t i = 0; i + 32 <= rdata.size; i += sizeof(uintptr_t)) {
        BYTE* p = rdata.begin + i;
        if (*reinterpret_cast<const uintptr_t*>(p) ==
            reinterpret_cast<uintptr_t>(getNotifyName)) {
            vtableSlot = p;
            ++slotCount;
        }
    }

    if (slotCount != 1 || !vtableSlot) {
        Log("InputSuppressWindow: GetNotifyName vtable slot count=%zu", slotCount);
        return {};
    }

    // In this UAnimNotifyState-derived vtable:
    //   +0x00 GetNotifyName
    //   +0x08 NotifyBegin override
    //   +0x10 inherited NotifyTick
    //   +0x18 NotifyEnd override
    out.begin = reinterpret_cast<BYTE*>(
        *reinterpret_cast<const uintptr_t*>(vtableSlot + 0x08)
    );
    out.end = reinterpret_cast<BYTE*>(
        *reinterpret_cast<const uintptr_t*>(vtableSlot + 0x18)
    );

    if (!AddressInSection(text, out.begin) ||
        !AddressInSection(text, out.end)) {
        Log("InputSuppressWindow: begin/end targets outside .text");
        return {};
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);

    Log(
        "InputSuppressWindow: resolved GetNotifyName=0x%zX Begin=0x%zX End=0x%zX",
        static_cast<size_t>(getNotifyName - base),
        static_cast<size_t>(out.begin - base),
        static_cast<size_t>(out.end - base)
    );

    return out;
}

void HookInputSuppressBegin(void* notifyState, void* meshComponent, void* eventData) {
    if (!g_config.actionRecoveryEnabled) {
        g_originalInputSuppressBegin(notifyState, meshComponent, eventData);
        return;
    }

    const int count = g_bypassedInputSuppressWindows.fetch_add(1) + 1;
    Log(
        "InputSuppressWindow: BEGIN BYPASSED notify=%p mesh=%p event=%p activeBypasses=%d",
        notifyState,
        meshComponent,
        eventData,
        count
    );
}

void HookInputSuppressEnd(void* notifyState, void* meshComponent, void* eventData) {
    int count = g_bypassedInputSuppressWindows.load();

    while (count > 0) {
        if (g_bypassedInputSuppressWindows.compare_exchange_weak(count, count - 1)) {
            Log(
                "InputSuppressWindow: END BYPASSED notify=%p mesh=%p event=%p remaining=%d",
                notifyState,
                meshComponent,
                eventData,
                count - 1
            );
            return;
        }
    }

    // If this window began before the mod feature was enabled, preserve the
    // native End so the game's suppression counter is balanced correctly.
    g_originalInputSuppressEnd(notifyState, meshComponent, eventData);
}

bool InstallInputSuppressWindowHooks() {
    const InputSuppressWindowTargets targets = ResolveInputSuppressWindowTargets();
    if (!targets.begin || !targets.end) {
        Log("InputSuppressWindow: resolver failed; diagnostic remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("InputSuppressWindow: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        targets.begin,
        reinterpret_cast<LPVOID>(&HookInputSuppressBegin),
        reinterpret_cast<LPVOID*>(&g_originalInputSuppressBegin)
    );
    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("InputSuppressWindow: create Begin FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_CreateHook(
        targets.end,
        reinterpret_cast<LPVOID>(&HookInputSuppressEnd),
        reinterpret_cast<LPVOID*>(&g_originalInputSuppressEnd)
    );
    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("InputSuppressWindow: create End FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(targets.begin);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("InputSuppressWindow: enable Begin FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(targets.end);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("InputSuppressWindow: enable End FAILED status=%d", static_cast<int>(status));
        return false;
    }

    g_recoveryHookReady.store(true);
    Log("InputSuppressWindow: DIAGNOSTIC BYPASS READY");
    return true;
}


BYTE* ResolveAbilityInterruptEnabledNative() {
    PeSectionView text{};
    PeSectionView rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        Log("Action Recovery: failed to enumerate PE sections");
        return nullptr;
    }

    BYTE* name = FindAsciiString(rdata, "IsInterruptEnabled");
    if (!name) {
        Log("Action Recovery: IsInterruptEnabled string not found");
        return nullptr;
    }

    static constexpr BYTE kNativePrefix[] = {
        0x44, 0x0F, 0xB6, 0xC2,
        0x41, 0x0F, 0xB6, 0xC0,
        0x49, 0xC1, 0xE8, 0x06,
        0x24, 0x3F,
        0x0F, 0xB6, 0xD0,
        0x4A, 0x8B, 0x84, 0xC1, 0xB0, 0x00, 0x00, 0x00
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
        Log("Action Recovery: native IsInterruptEnabled match count=%zu", nativeCount);
        return nullptr;
    }

    HMODULE module = GetModuleHandleW(nullptr);
    BYTE* base = reinterpret_cast<BYTE*>(module);
    Log(
        "Action Recovery: IsInterruptEnabled resolved RVA=0x%zX bitset=ability+0xB0 state=+0xD8 elapsed=+0xDC",
        static_cast<size_t>(native - base)
    );

    return native;
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

bool HookAbilityInterruptEnabled(void* ability, unsigned char interrupt) {
    const bool nativeEnabled = g_originalAbilityInterruptEnabled
        ? g_originalAbilityInterruptEnabled(ability, interrupt)
        : false;

    // EAbilityInterrupt::MOVE is enum value 1 in the audited executable.
    constexpr unsigned char kMoveInterrupt = 1;

    if (!ability || interrupt != kMoveInterrupt) {
        return nativeEnabled;
    }

    BYTE* object = reinterpret_cast<BYTE*>(ability);
    const unsigned char state = *(object + 0xD8);
    const float elapsed = *reinterpret_cast<float*>(object + 0xDC);

    g_moveInterruptQueries.fetch_add(1);
    g_lastMoveQueryElapsed.store(elapsed);

    if (!nativeEnabled) {
        g_moveInterruptNativeBlocked.fetch_add(1);
    }

    void* previousAbility = g_lastMoveQueryAbility.load();
    const int previousState = g_lastMoveQueryState.load();

    if (previousAbility != ability || previousState != static_cast<int>(state)) {
        g_lastMoveQueryAbility.store(ability);
        g_lastMoveQueryState.store(static_cast<int>(state));
        Log(
            "Action Recovery: MOVE query ability=%p state=%s(%u) elapsed=%.3f native=%d",
            ability,
            AbilityStateName(state),
            static_cast<unsigned>(state),
            elapsed,
            nativeEnabled ? 1 : 0
        );
    }

    if (!g_config.actionRecoveryEnabled || nativeEnabled) {
        return nativeEnabled;
    }

    // The user-observed problem is a dead tail after the visible action has
    // completed. AWAITING_FINISH is the common ability lifecycle state for that
    // tail. Do not allow MOVE during STARTING/RUNNING, so attacks and actions
    // cannot be cancelled prematurely.
    if (state == 4) {
        const int forced = g_moveInterruptForced.fetch_add(1) + 1;
        if (forced <= 20 || (forced % 100) == 0) {
            Log(
                "Action Recovery: FORCE MOVE ability=%p state=AWAITING_FINISH elapsed=%.3f forcedCount=%d",
                ability,
                elapsed,
                forced
            );
        }
        return true;
    }

    return nativeEnabled;
}

bool InstallCommonActionRecoveryHook() {
    BYTE* target = ResolveAbilityInterruptEnabledNative();
    if (!target) {
        Log("Action Recovery: resolver failed; feature remains fail-open");
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log("Action Recovery: MinHook initialize FAILED status=%d", static_cast<int>(initStatus));
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookAbilityInterruptEnabled),
        reinterpret_cast<LPVOID*>(&g_originalAbilityInterruptEnabled)
    );

    if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
        Log("Action Recovery: create FAILED status=%d", static_cast<int>(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK && status != MH_ERROR_ENABLED) {
        Log("Action Recovery: enable FAILED status=%d", static_cast<int>(status));
        return false;
    }

    g_recoveryHookReady.store(true);
    Log("Action Recovery: READY policy=force MOVE only in AWAITING_FINISH");
    return true;
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
            "Action Recovery V0.8: MOVE action query ability=%p state=%s(%u) elapsed=%.3f native=%d instigator=%p",
            ability,
            AbilityStateName(state),
            static_cast<unsigned>(state),
            elapsed,
            nativeEnabled ? 1 : 0,
            instigator
        );
    }

    if (!g_config.actionRecoveryEnabled || nativeEnabled) {
        return nativeEnabled;
    }

    // Diagnostic hammer: if the local player's active ability says MOVE is
    // disabled, return true. If this removes the dead recovery tail, the
    // AllowedActionsFlags path is proven and the next build will narrow the
    // timing using the state/elapsed telemetry gathered here.
    const int forced = g_actionMoveForced.fetch_add(1) + 1;
    if (forced <= 30 || (forced % 100) == 0) {
        Log(
            "Action Recovery V0.8: FORCE MOVE ACTION ability=%p state=%s(%u) elapsed=%.3f forcedCount=%d",
            ability,
            AbilityStateName(state),
            static_cast<unsigned>(state),
            elapsed,
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
    Log("Action Recovery V0.8: READY policy=force local-player IsActionEnabled(MOVE)");
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
            "F%d -> Action Recovery %s policy=force local IsActionEnabled(MOVE)",
            functionKey,
            g_config.actionRecoveryEnabled ? "ON" : "OFF"
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

void DrawFeatureRow(const char* label, bool* enabled, const char* note) {
    ImGui::Checkbox(label, enabled);
    ImGui::SameLine(260.0f);
    ImGui::TextDisabled("%s", note);
}

void DrawOverlay() {
    ImGui::SetNextWindowSize(ImVec2(780.0f, 570.0f), ImGuiCond_FirstUseEver);
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
        if (ImGui::BeginTabItem("General")) {
            ImGui::Spacing();
            ImGui::Text("Renderer");
            ImGui::BulletText("DXGI proxy loader: active");
            ImGui::BulletText("D3D11 Present hook: active");
            ImGui::BulletText("Mouse capture: active while menu is open");
            ImGui::BulletText("F1-F12 gameplay input: suppressed while menu is open");

            ImGui::Spacing();
            ImGui::Text("Menu");
            ImGui::Text("Open / close key: %s", menuKeyName.c_str());
            ImGui::SameLine(280.0f);
            if (g_captureMenuKey.load()) {
                ImGui::TextDisabled("Press a key...  Esc = cancel");
            } else if (ImGui::Button("Rebind Menu Key", ImVec2(150.0f, 0.0f))) {
                g_captureMenuKey.store(true);
                g_lastAction = "Waiting for new menu key";
                Log("Menu key capture started");
            }

            ImGui::Spacing();
            ImGui::Text("Configuration");
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

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::TextWrapped(
                "V0.5 is cumulative: overlay foundation, rebindable menu key, native Toggle HUD, "
                "player-only Movement Speed, plus Action Recovery. Recovery scales only the native "
                "MOVE interrupt delay and leaves animations and all other action checks untouched."
            );
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Features")) {
            ImGui::Spacing();
            ImGui::Text("Default policy: all requested features are enabled.");
            ImGui::Spacing();

            ImGui::Checkbox("Toggle HUD", &g_config.toggleHudEnabled);
            ImGui::SameLine(260.0f);
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
            if (ImGui::Checkbox("Movement Speed", &g_config.movementSpeedEnabled)) {
                g_config.Save();
                g_lastAction = std::string("Movement Speed ") +
                    (g_config.movementSpeedEnabled ? "ON" : "OFF");
            }
            ImGui::SameLine(260.0f);
            ImGui::TextDisabled(
                "%s",
                g_movementHookReady.load()
                    ? "MovementComponent virtual GetMaxSpeed hook"
                    : "Native hook unavailable"
            );

            if (g_config.movementSpeedEnabled) {
                ImGui::Indent();
                ImGui::SetNextItemWidth(260.0f);
                if (ImGui::SliderFloat(
                    "Multiplier##Movement",
                    &g_config.movementSpeedMultiplier,
                    1.00f,
                    2.50f,
                    "%.2fx"
                )) {
                    g_config.Save();
                }
                ImGui::SameLine();
                ImGui::TextDisabled("F2 toggles");
                ImGui::TextDisabled("Walking/NavWalking only; physics virtual slot 0x3D0.");
                ImGui::Unindent();
            }

            if (ImGui::Checkbox("Action Recovery", &g_config.actionRecoveryEnabled)) {
                g_config.Save();
                g_lastAction = std::string("Action Recovery ") +
                    (g_config.actionRecoveryEnabled ? "ON" : "OFF");
            }
            ImGui::SameLine(260.0f);
            ImGui::TextDisabled(
                "%s",
                g_recoveryHookReady.load()
                    ? "DIAGNOSTIC: force AllowedActions MOVE for local player"
                    : "Native hook unavailable"
            );

            if (g_config.actionRecoveryEnabled) {
                ImGui::Indent();
                ImGui::TextDisabled("F3 toggles");
                ImGui::TextWrapped(
                    "V0.8A forces ECharacterActions::MOVE when the local player's active ability "
                    "blocks it. This is intentionally aggressive to prove or reject AllowedActionsFlags."
                );
                ImGui::TextDisabled(
                    "MOVE queries: %d | Local: %d | Native blocked: %d | Forced: %d",
                    g_actionMoveQueries.load(),
                    g_actionMoveLocalQueries.load(),
                    g_actionMoveNativeBlocked.load(),
                    g_actionMoveForced.load()
                );
                const int state = g_lastActionMoveState.load();
                if (state >= 0) {
                    ImGui::TextDisabled(
                        "Last local ability: %s (%d) | elapsed %.3f s",
                        AbilityStateName(static_cast<unsigned char>(state)),
                        state,
                        g_lastActionMoveElapsed.load()
                    );
                } else if (!g_localPlayerCharacter.load()) {
                    ImGui::TextDisabled("Local player pointer: waiting for CharacterMovement...");
                }
                ImGui::Unindent();
            }

            DrawFeatureRow("Skip Intro Videos", &g_config.skipIntroEnabled, "UE4 MoviePlayer audit started");
            DrawFeatureRow("Third Person", &g_config.thirdPersonEnabled, "Camera hook pending");

            ImGui::Spacing();
            ImGui::TextDisabled(
                "V0.8A tests the AllowedActionsFlags path: local-player MOVE is forced when native code blocks it."
            );
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Hotkeys")) {
            ImGui::Spacing();
            ImGui::TextWrapped(
                "F1-F12 are fixed physical slots, Q Protocol style. "
                "Each slot can be reassigned to any mod action or None."
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
                "Target executable audited for this branch: DarksidersGenesis-Win64-Shipping.exe"
            );
            ImGui::TextWrapped(
                "SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54"
            );
            ImGui::TextWrapped("Size: 62,113,280 bytes");
            ImGui::Spacing();
            ImGui::TextWrapped(
                "Toggle HUD uses ui.HideHud. Movement Speed hooks the Mayhem CharacterMovement "
                "GetMaxSpeed override. V0.8 Action Recovery hooks UMayhemAbility::IsActionEnabled "
                "and diagnoses ECharacterActions::MOVE / AllowedActionsFlags for the local player."
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

DWORD WINAPI MainThread(LPVOID) {
    InitializePaths();
    Log("============================================================");
    Log("Darksiders Genesis Enhanced ASI %s starting", kBuild);
    Log("Target EXE audit SHA256=9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54");
    Log("Architecture: DXGI proxy -> ASI -> D3D11 Present/ResizeBuffers -> Dear ImGui");

    g_config.Load();

    if (!DiscoverAndHookD3D11()) {
        Log("Overlay hook setup FAILED. Mod stays fail-open; game should continue normally.");
        return 0;
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

    Log("Core initialization complete. Press %s after the first game frame.",
        KeyDisplayName(g_config.menuKey).c_str());
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
