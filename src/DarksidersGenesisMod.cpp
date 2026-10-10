#include <windows.h>
#include <intrin.h>
#include <d3d11.h>
#include <dxgi.h>
#include <Xinput.h>

#include <MinHook.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "CameraTraceFeature.h"
#include "ConfigStore.h"
#include "EngineIniFeature.h"
#include "HorseFeature.h"
#include "OverlayUi.h"
#include "RuntimeSettings.h"
#include "SkipLogosFeature.h"
#include "TargetValidator.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <algorithm>
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

constexpr const char* kBuild = "0.73-native-player-physics-rotation-test";
constexpr const wchar_t* kIniName = L"DarksidersGenesisMod.ini";
constexpr const wchar_t* kLogName = L"DarksidersGenesisMod.log";

HMODULE g_module = nullptr;
std::wstring g_iniPath;
std::wstring g_logPath;

using PresentFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
using ResizeBuffersFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
using HudHiddenGetterFn = bool(*)();
using UiIsCursorVisibleFn = bool(*)(void*);
using SetCursorFn = HCURSOR (WINAPI*)(HCURSOR);
using CharacterGetMaxSpeedFn = float(*)(void*);
using AbilityActionEnabledFn = bool(*)(void*, unsigned char);
using AddJuiceFn = void(*)(void*, float);
using FilterOutgoingDamageFn = void(*)(void*, void*);

PresentFn g_originalPresent = nullptr;
ResizeBuffersFn g_originalResizeBuffers = nullptr;
HudHiddenGetterFn g_originalHudHiddenGetter = nullptr;
UiIsCursorVisibleFn g_originalUiIsCursorVisible = nullptr;
SetCursorFn g_originalSetCursor = nullptr;
CharacterGetMaxSpeedFn g_originalCharacterGetMaxSpeed = nullptr;
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
// Third Person V0.57: mouse and controller aim isolation.
std::atomic_uint32_t g_nativePadAimSuppressed{0};
std::atomic_uint32_t g_nativePadAimSuppressedDuringFire{0};
std::atomic_uint32_t g_tpsCombatAimSamples{0};
std::atomic_uint32_t g_tpsCombatFireSamples{0};
// V0.59: actor-heading telemetry sampled ONLY from a live validated player
// movement callback. Never dereference a cached UObject in Present/XInput.
std::atomic<float> g_tpsActorWorldYaw{0.0f};
std::atomic_ullong g_tpsActorYawTick{0};
std::atomic_uint32_t g_tpsActorYawSamples{0};
std::atomic_uint32_t g_tpsLeftPassthroughSamples{0};
std::atomic_uint32_t g_tpsMovingFireSamples{0};
std::atomic_ullong g_tpsLastAimTick{0};
std::atomic<float> g_tpsDesiredYawDegrees{0.0f};
std::atomic<float> g_tpsCommandYawDegrees{0.0f};
std::atomic_bool g_tpsAimCommandReady{false};
// V0.63: native cone lock, no forced aim while not attacking.
std::atomic_uint32_t g_tpsFacingReasserted{0};
std::atomic_uint32_t g_tpsConeInside{0},g_tpsConeOutside{0};
std::atomic_bool g_tpsFireHeld{false};
std::atomic_ullong g_tpsFireLastTick{0};
std::atomic_uint32_t g_tpsPlayerGeneration{0};
std::atomic_uint32_t g_tpsNativeFacingOff{0},g_tpsNativeFacingRestored{0};
std::atomic_uint32_t g_tpsStrafeSamples{0};
// V0.72: track potential Steam Input -> synthetic Windows mouse mapping.
std::atomic_ullong g_tpsPadRightStickTick{0},g_tpsRawMouseTick{0};
std::atomic_uint32_t g_tpsControllerMouseBlocks{0};
SRWLOCK g_tpsFacingLock=SRWLOCK_INIT;
void* g_tpsFacingCurrentComponent=nullptr;
bool g_tpsFacingOwned=false;
std::atomic_int g_orbitMouseDx{0},g_orbitMouseDy{0};
std::atomic_ullong g_lastOrbitRawTick{0};
ULONGLONG g_orbitLastPresentTick=0;
std::atomic_bool g_overlayVisible{false};
std::atomic_bool g_captureMenuKey{false};
std::atomic_int g_lastForegroundState{-1};
std::atomic_uint32_t g_focusTransitionCount{0};
std::atomic_uint32_t g_resizeBuffersTraceCount{0};
std::atomic<void*> g_gameWindowTrace{nullptr};
// Manual F5-only fallback retained for comparison; no auto focus simulation.
std::atomic_uint32_t g_cursorVisibilityCalls{0};
std::atomic_int g_capturedMenuKey{0};
std::atomic_int g_captureCameraKeyIndex{-1};
std::atomic_int g_capturedCameraBinding{0};
std::array<bool, static_cast<size_t>(dg::config::CameraAction::Count)> g_cameraKeyHeld{};
std::array<ULONGLONG, static_cast<size_t>(dg::config::CameraAction::Count)> g_cameraNextRepeat{};
std::array<uint32_t, static_cast<size_t>(dg::config::CameraAction::Count)> g_cameraActionRepeatCount{};
std::atomic_bool g_hudHidden{false};
std::atomic_bool g_hudHookReady{false};
std::atomic_bool g_cursorVisibilityHookReady{false};
std::atomic_bool g_setCursorHookReady{false};
std::atomic_bool g_cursorBlankApplied{false};
std::atomic_uint32_t g_reticleCursorIntercepts{0};
std::atomic<HCURSOR> g_lastRequestedGameCursor{nullptr};
std::atomic_uint32_t g_cursorProbeChanges{0};
std::atomic_int g_lastNativeCursorVisible{-1};
std::atomic<HCURSOR> g_lastObservedDesktopCursor{nullptr};
std::atomic_uint32_t g_lastObservedDesktopFlags{0xFFFFFFFFu};
std::atomic_uint32_t g_cursorSnapshotCount{0};
std::atomic_ullong g_cursorNextSnapshotTick{0};
std::atomic_bool g_lastCursorSnapshotForeground{false};
std::atomic_bool g_movementHookReady{false};
std::atomic_bool g_recoveryHookReady{false};
std::atomic_bool g_skipIntroReady{false};
std::atomic_bool g_shuttingDown{false};
dg::target::ValidationResult g_targetValidation{};
LONG** g_skipIntroDataSlot = nullptr;
LONG* g_skipIntroData = nullptr;
LONG g_skipIntroOriginalValue = 1;
LONG** g_dofSlot = nullptr;
LONG* g_dofData = nullptr;
LONG g_dofOriginal = 2;
bool g_dofCaptured = false;
bool g_dofDisabled = false;
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

using dg::config::Action;
using dg::config::ActionLabel;
using dg::config::KeyDisplayName;

dg::config::Store g_config;

void TraceDesktopCursorState(const char* reason, bool force);

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

    // V0.47: the proxy created/truncated this SAME log at process attach.
    // Keep its early Skip Logos failures, including when ASI loads later.
    using UnifiedLogActiveFn = BOOL (WINAPI*)();
    const HMODULE proxy = GetModuleHandleW(L"dxgi.dll");
    const auto active = proxy ? reinterpret_cast<UnifiedLogActiveFn>(
        GetProcAddress(proxy, "DGUnifiedLogActive")) : nullptr;
    // If proxy initialization reported success but disk I/O failed,
    // allow the ASI to create its own fresh per-session log.
    if (active && active() != FALSE &&
        GetFileAttributesW(g_logPath.c_str()) != INVALID_FILE_ATTRIBUTES)
        return;

    // Fallback when no compatible loader exists: reset once per ASI run.
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

    // V0.47: compact diagnostics, WITHOUT changing any hook or gameplay
    // behavior. Keep initialization, errors, settings, actions, and first
    // observed damage/juice sample, suppress repetitive trace spam.
    if (std::strncmp(message, "CursorProbe V0.42:", 18) == 0 ||
        std::strncmp(message, "Cursor visibility: call=", 24) == 0 ||
        std::strncmp(message, "Window focus trace #", 20) == 0 ||
        std::strncmp(message, "Focus V0.38: msg=", 17) == 0 ||
        std::strncmp(message, "ResizeBuffers trace:", 20) == 0 ||
        std::strncmp(message, "Runtime tuning: movement capture rejected", 41) == 0 ||
        std::strncmp(message, "HorseFeature V0.31: NATIVE HORSE CAPTURE", 39) == 0 ||
        std::strncmp(message, "Camera V0.38: native SpringArm calls=", 36) == 0 ||
        std::strncmp(message, "Camera V0.38: native View calls=", 31) == 0)
        return;
    static std::atomic_uint32_t damageExamples{0};
    static std::atomic_uint32_t juiceExamples{0};
    if (std::strncmp(message, "Final damage hook: PISTOL final", 31) == 0 &&
        damageExamples.fetch_add(1, std::memory_order_relaxed) >= 1) return;
    if (std::strncmp(message, "Hotstreak hook: AddJuice local gain", 35) == 0 &&
        juiceExamples.fetch_add(1, std::memory_order_relaxed) >= 1) return;

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
        dg::runtime::Get().skipIntroEnabled.load(std::memory_order_relaxed) ? 0 : g_skipIntroOriginalValue;

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
                dg::runtime::Get().skipIntroEnabled.load(std::memory_order_relaxed) ? "SKIP" : "VANILLA"
            );
        }
    }

    g_skipIntroReady.store(true);
    return true;
}

// V0.55: compensate for camera-distance blur without touching Engine.ini.
// The registered native r.DepthOfFieldQuality is restored when both
// Third Person and manual Zoom are off.
bool WritableDofData(const LONG* p) {
    if (!p) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(p,&mbi,sizeof(mbi)) || mbi.State!=MEM_COMMIT ||
        (mbi.Protect&(PAGE_GUARD|PAGE_NOACCESS))) return false;
    const DWORD prot=mbi.Protect&0xFF;
    return (prot==PAGE_READWRITE||prot==PAGE_WRITECOPY||
        prot==PAGE_EXECUTE_READWRITE||prot==PAGE_EXECUTE_WRITECOPY) &&
        reinterpret_cast<uintptr_t>(p)+sizeof(LONG) <=
        reinterpret_cast<uintptr_t>(mbi.BaseAddress)+mbi.RegionSize;
}
void InitializeCameraDof() {
    if (!g_targetValidation.exact) return;
    const BYTE* base=reinterpret_cast<const BYTE*>(GetModuleHandleW(nullptr));
    if (!base) return;
    const BYTE nameSig[7]={0x48,0x8D,0x15,0x2C,0x25,0x6F,0x02};
    const BYTE slotSig[7]={0x48,0x89,0x05,0xDC,0xB8,0x6F,0x03};
    if (std::memcmp(base+0x12259D,nameSig,7)!=0 ||
        std::memcmp(base+0x1225C5,slotSig,7)!=0) {
        Log("Camera DOF V0.55: unsupported CVar registration, original DOF retained");
        return;
    }
    g_dofSlot=reinterpret_cast<LONG**>(const_cast<BYTE*>(base)+0x381DEA8);
    Log("Camera DOF V0.55: r.DepthOfFieldQuality registration validated");
}
void UpdateCameraDof() {
    if (!g_dofSlot || !WritableDofData(*g_dofSlot)) return;
    LONG* data=*g_dofSlot;
    if (data!=g_dofData) {
        g_dofData=data; g_dofCaptured=false; g_dofDisabled=false;
    }
    const LONG current=*data;
    if (!g_dofCaptured) {
        if (current<0 || current>4) return;
        g_dofOriginal=current;g_dofCaptured=true;
        Log("Camera DOF V0.55: vanilla native quality=%ld",current);
    }
    const auto& camera=dg::runtime::Get();
    const float zoom=camera.cameraZoomPercent.load(std::memory_order_relaxed);
    const bool displaced=camera.thirdPersonEnabled.load(std::memory_order_relaxed)||
        (std::isfinite(zoom)&&std::fabs(zoom)>0.0001f);
    const LONG desired=displaced ? 0 : g_dofOriginal;
    if (current!=desired) {
        InterlockedExchange(reinterpret_cast<volatile LONG*>(data),desired);
        Log("Camera DOF V0.55: quality %ld -> %ld %s",current,desired,
            displaced?"camera-distance blur suppressed":"original restored");
    }
    g_dofDisabled=displaced;
}
void RestoreCameraDof() {
    if (g_dofDisabled && g_dofCaptured && WritableDofData(g_dofData) &&
        *g_dofData==0)
        InterlockedExchange(reinterpret_cast<volatile LONG*>(g_dofData),g_dofOriginal);
    g_dofDisabled=false;
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

    if (!dg::runtime::Get().toggleHudEnabled.load(std::memory_order_relaxed)) {
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

    if (dg::runtime::Get().hotstreakChargeEnabled.load(std::memory_order_relaxed) &&
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
            float multiplier = dg::runtime::Get().hotstreakChargeMultiplier.load(std::memory_order_relaxed);
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

    void* localPlayer = g_localPlayerCharacter.load();
    if (!localPlayer || localPlayer != playerCharacter) {
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

    if (pistolLike &&
        dg::runtime::Get().pistolDamageEnabled.load(std::memory_order_relaxed)) {
        multiplier = dg::runtime::Get().pistolDamageMultiplier.load(std::memory_order_relaxed);
        if (multiplier < 0.0f) multiplier = 0.0f;
        if (multiplier > 100.0f) multiplier = 100.0f;
        kind = "PISTOL";

        g_lastNativePistolDamage.store(nativeFinalDamage);
        g_lastPistolBaseJuice.store(baseJuice);
    } else if (!pistolLike &&
               dg::runtime::Get().meleeDamageEnabled.load(std::memory_order_relaxed)) {
        multiplier = dg::runtime::Get().meleeDamageMultiplier.load(std::memory_order_relaxed);
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

bool IsLocallyControlledMayhemCharacter(void* character) {
    if (!character) {
        return false;
    }

    void** vtable = *reinterpret_cast<void***>(character);
    if (!vtable) {
        return false;
    }

    // Proven V0.14F path: APawn::IsLocallyControlled in this audited UE4 build.
    using IsLocallyControlledFn = bool(*)(void*);
    auto fn = reinterpret_cast<IsLocallyControlledFn>(
        vtable[0x680 / sizeof(void*)]
    );

    return fn ? fn(character) : false;
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

bool ApplyPlayerMovementTunings(void* movementComponent) {
    AcquireSRWLockExclusive(&g_tuningLock);

    PlayerMovementTuningState* state = FindOrCapturePlayerMovementState(movementComponent);
    if (!state) {
        ReleaseSRWLockExclusive(&g_tuningLock);
        return false;
    }

    BYTE* component = reinterpret_cast<BYTE*>(movementComponent);

    auto& runtime = dg::runtime::Get();

    const float heightMultiplier = ClampFloat(
        runtime.jumpHeightMultiplier.load(std::memory_order_relaxed),
        0.0f,
        20.0f
    );
    // Jump apex height is approximately proportional to velocity squared when
    // gravity is unchanged, so use sqrt(multiplier) for a true height scalar.
    const float velocityMultiplier = sqrtf(heightMultiplier);
    const bool jumpEnabled =
        runtime.jumpHeightEnabled.load(std::memory_order_relaxed);

    *reinterpret_cast<float*>(component + 0x1A0) =
        jumpEnabled
            ? state->jumpZVelocity * velocityMultiplier
            : state->jumpZVelocity;

    *reinterpret_cast<float*>(component + 0x85C) =
        jumpEnabled
            ? state->doubleJumpZVelocity * velocityMultiplier
            : state->doubleJumpZVelocity;

    const float glideMultiplier = ClampFloat(
        runtime.glideDurationMultiplier.load(std::memory_order_relaxed),
        0.0f,
        100.0f
    );
    *reinterpret_cast<float*>(component + 0x86C) =
        runtime.glideDurationEnabled.load(std::memory_order_relaxed)
            ? state->glideDurationSeconds * glideMultiplier
            : state->glideDurationSeconds;

    ReleaseSRWLockExclusive(&g_tuningLock);
    return true;
}


void CaptureLiveTPSActorYaw(void* actor) {
    if (!actor || !g_targetValidation.exact ||
        !dg::runtime::Get().thirdPersonEnabled.load(std::memory_order_relaxed))
        return;
    // Confirmed in the supplied exact build: native GetNormalizedAimRotation
    // getter RVAs 0x638240/0x570260 read Actor.RootComponent at +0x158,
    // and compare cached root-component FRotator at +0x1F0..0x1F8
    // with live actor rotation at +0x900..0x904.
    // This is observation only, not a rotation or actor write.
    auto* bytes=static_cast<unsigned char*>(actor);
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(bytes+0x158,&mbi,sizeof(mbi)) || mbi.State!=MEM_COMMIT ||
        (mbi.Protect & (PAGE_NOACCESS|PAGE_GUARD))) return;
    void* root=nullptr;
    std::memcpy(&root,bytes+0x158,sizeof(root));
    if (!root) return;
    auto* yawPtr=static_cast<unsigned char*>(root)+0x1F4;
    if (!VirtualQuery(yawPtr,&mbi,sizeof(mbi)) || mbi.State!=MEM_COMMIT ||
        (mbi.Protect & (PAGE_NOACCESS|PAGE_GUARD))) return;
    float yaw=0.0f;
    std::memcpy(&yaw,yawPtr,sizeof(yaw));
    if (!std::isfinite(yaw) || std::fabs(yaw)>36000.0f) return;
    // V0.67: NATIVE EXE CONFIRMED (SHA-256 exact) at RVA 0x66976C:
    //   movups xmm1, XMMWORD PTR [rax+0x1A0]
    // after loading Actor.RootComponent [r12+0x158] at RVA 0x669728.
    // Native then extracts x/y/z from xmm1 by scalar + shufps.
    // +0x1D0, used in V0.64-0.66, was WRONG and yielded (0,0,0),
    // teleporting the mod's synthetic camera to the map origin.
    // Read only in validated live-player GetMaxSpeed callback.
    auto* worldPosition=static_cast<unsigned char*>(root)+0x1A0;
    MEMORY_BASIC_INFORMATION positionMemory{};
    if(!VirtualQuery(worldPosition,&positionMemory,sizeof(positionMemory)) ||
       positionMemory.State!=MEM_COMMIT ||
       (positionMemory.Protect&(PAGE_NOACCESS|PAGE_GUARD))) return;
    float px=0,py=0,pz=0;
    std::memcpy(&px,worldPosition+0x00,4);
    std::memcpy(&py,worldPosition+0x04,4);
    std::memcpy(&pz,worldPosition+0x08,4);
    if(std::isfinite(px)&&std::isfinite(py)&&std::isfinite(pz)&&
        std::fabs(px)<1e7f&&std::fabs(py)<1e7f&&std::fabs(pz)<1e7f) {
        auto& settings=dg::runtime::Get();
        settings.tpsActorWorldX.store(px);
        settings.tpsActorWorldY.store(py);
        settings.tpsActorWorldZ.store(pz);
        settings.tpsActorLocationTick.store(GetTickCount64());
        settings.tpsActorGeneration.store(g_tpsPlayerGeneration.load());
    }
    dg::runtime::Get().tpsActorYawDegrees.store(std::remainder(yaw,360.0f));
    g_tpsActorWorldYaw.store(std::remainder(yaw,360.0f));
    g_tpsActorYawTick.store(GetTickCount64());
    const unsigned count=g_tpsActorYawSamples.fetch_add(1)+1;
    if (count==1 || count==120 || count==1000 || count==10000)
        Log("TPS V0.67: live actor transform sample=%u actor=%p root=%p "
            "worldYaw=%.2f worldLocation=(%.1f,%.1f,%.1f) nativeOffset=0x1A0",
            count,actor,root,yaw,px,py,pz);
}

// bOrientRotationToMovement native reflected field validated in EXACT EXE:
// static bit setter at RVA 0x1CF0300: OR BYTE PTR [RCX+0x240], 0x10.
// Write ONLY within an active local player's live GetMaxSpeed callback.
// Legacy V0.61-0.66 movement-facing experiment retired. If this ASI ever
// owned the native flag earlier in the same session, restore it in a LIVE
// validated movement callback. Never force turn-to-move OFF in new builds.
void UpdateTPSFacingNative(void* component) {
    if(!component)return;
    auto* field=static_cast<unsigned char*>(component)+0x240;
    AcquireSRWLockExclusive(&g_tpsFacingLock);
    if(g_tpsFacingCurrentComponent!=component){
        g_tpsFacingCurrentComponent=component;
        g_tpsFacingOwned=false;
    }
    if(g_tpsFacingOwned){
        if(!(*field&0x10u))*field=static_cast<unsigned char>(*field|0x10u);
        g_tpsFacingOwned=false;
        ++g_tpsNativeFacingRestored;
    }
    ReleaseSRWLockExclusive(&g_tpsFacingLock);
}
// V0.73: player-specific desired yaw at the NATIVE UE4 rotation interpolation
// boundary, not the V0.61/V0.66/V0.71 orient-to-movement bit and NOT XInput
// axis remapping. Exact retail EXE only; MinHook signature is verified below.
// Disassembly: RVA 0x5B96BF calls RInterp at 0x8A1700 with CurrentRotation
// movement+0x22C and a transient TargetRotation. The result is written back
// into movement+0x22C. This callback is outside the D3D11 render thread.
struct NativeTpsRotator { float pitch, yaw, roll; };
using NativeRotationInterpFn = NativeTpsRotator* (*)(
    NativeTpsRotator*, const NativeTpsRotator*, const NativeTpsRotator*, float, float);
NativeRotationInterpFn g_originalNativeRotationInterp = nullptr;
std::atomic<void*> g_tpsLiveMovementForYaw{nullptr};
std::atomic_ullong g_tpsLiveMovementYawTick{0};
std::atomic_uint32_t g_tpsNativeYawOverrides{0}, g_tpsNativeYawSeen{0};
NativeTpsRotator* HookNativeRotationInterp(NativeTpsRotator* output,
        const NativeTpsRotator* current, const NativeTpsRotator* desired,
        float dt, float speed) {
    auto original = g_originalNativeRotationInterp;
    if (!original) return output;
    const uintptr_t exe = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const uintptr_t caller = reinterpret_cast<uintptr_t>(_ReturnAddress());
    // Narrow down the shared UE interpolator to exactly the game-specific
    // CharacterMovement physics rotation call, NOT camera / other actors.
    if (caller != exe + 0x5B96C4 || !current || !desired || !output)
        return original(output,current,desired,dt,speed);
    const unsigned seen = g_tpsNativeYawSeen.fetch_add(1) + 1;
    auto& rt=dg::runtime::Get();
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG movementTick=g_tpsLiveMovementYawTick.load(std::memory_order_acquire);
    auto* component = static_cast<unsigned char*>(g_tpsLiveMovementForYaw.load(std::memory_order_acquire));
    const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    const ULONGLONG viewTick=rt.tpsGameplayViewTick.load(std::memory_order_acquire);
    // Controller opt-in, correct local player movement component and a fresh
    // camera gameplay view. Native behavior everywhere else, including LB /
    // menu, unmounted checks, focus loss and third-person disabled.
    if (!rt.tpsControllerStrafe.load() || !rt.thirdPersonEnabled.load() ||
        g_overlayVisible.load() || g_shuttingDown.load() ||
        !hwnd || GetForegroundWindow()!=hwnd || IsIconic(hwnd) ||
        !component || reinterpret_cast<const unsigned char*>(current)!=component+0x22C ||
        !movementTick || now<movementTick || now-movementTick>300 ||
        !rt.tpsViewGameplay.load() || !viewTick ||
        now<viewTick || now-viewTick>250)
        return original(output,current,desired,dt,speed);
    const float yaw=dg::camera_trace::GetTelemetry().appliedYaw;
    if(!std::isfinite(yaw) || std::fabs(yaw)>36000.0f ||
       !std::isfinite(dt) || dt<0.0f || dt>0.25f)
        return original(output,current,desired,dt,speed);
    NativeTpsRotator facing=*desired;
    facing.yaw=std::remainder(yaw,360.0f);
    const unsigned n=g_tpsNativeYawOverrides.fetch_add(1)+1;
    if(n==1||n==120||n==1000||n==10000)
        Log("TPS V0.73: native physics desired yaw override n=%u viewedYaw=%.2f nativeTarget=%.2f nativeCurrent=%.2f dt=%.4f callsite=0x5B96BF",
            n,facing.yaw,desired->yaw,current->yaw,dt);
    return original(output,current,&facing,dt,speed);
}
bool InstallNativeTPSRotationHook() {
    if(!g_targetValidation.exact) return false;
    auto* target=reinterpret_cast<unsigned char*>(
        reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr))+0x8A1700);
    // Exact native UE4 FRotator interp signature, verified in original EXE.
    constexpr unsigned char signature[]={
        0x48,0x83,0xEC,0x78,0x0F,0x29,0x7C,0x24,0x50,0x0F,0x28,0xFB
    };
    if(std::memcmp(target,signature,sizeof(signature))!=0){
        Log("TPS V0.73: native physics rotation hook signature mismatch: fail open");
        return false;
    }
    const MH_STATUS init=MH_Initialize();
    if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED) return false;
    const MH_STATUS created=MH_CreateHook(
        target,reinterpret_cast<void*>(&HookNativeRotationInterp),
        reinterpret_cast<void**>(&g_originalNativeRotationInterp));
    if(created!=MH_OK){
        Log("TPS V0.73: CreateHook failed=%d",int(created));
        return false;
    }
    const MH_STATUS enabled=MH_EnableHook(target);
    if(enabled!=MH_OK&&enabled!=MH_ERROR_ENABLED){
        MH_RemoveHook(target);g_originalNativeRotationInterp=nullptr;
        Log("TPS V0.73: EnableHook failed=%d",int(enabled));
        return false;
    }
    Log("TPS V0.73: native physics rotation interpolator READY RVA=0x8A1700 callsite=0x5B96BF controllerOptIn=1");
    return true;
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

    auto& runtime = dg::runtime::Get();

    dg::horse::Settings horseSettings{};
    horseSettings.speedEnabled =
        runtime.horseSpeedEnabled.load(std::memory_order_relaxed);
    horseSettings.speedMultiplier =
        runtime.horseSpeedMultiplier.load(std::memory_order_relaxed);
    horseSettings.sprintSpeedEnabled =
        runtime.horseSprintSpeedEnabled.load(std::memory_order_relaxed);
    horseSettings.sprintSpeedMultiplier =
        runtime.horseSprintSpeedMultiplier.load(std::memory_order_relaxed);
    horseSettings.sprintDurationEnabled =
        runtime.horseSprintDurationEnabled.load(std::memory_order_relaxed);
    horseSettings.sprintDurationMultiplier =
        runtime.horseSprintDurationMultiplier.load(std::memory_order_relaxed);
    dg::horse::SetSettings(horseSettings);
    dg::horse::Tick();

    // V0.31: the generic movement hook never writes cached horse pointers.
    // It only asks HorseFeature whether this movement was very recently proven
    // by a live native HorseCharacter callback so player tuning can skip it.
    void* knownLocalPlayer =
        g_localPlayerCharacter.load(std::memory_order_relaxed);

    dg::horse::ObserveMovement(
        movementComponent,
        characterOwner,
        knownLocalPlayer,
        nativeSpeed
    );

    // V0.22A follows the game's direct mount chain instead of identifying
    // the horse from movement-property signatures. Once the exact horse
    // movement is proven, return its independently adjusted normal/sprint speed
    // before APawn::IsLocallyControlled can reclassify the mount as the player.
    if (dg::horse::IsValidatedMovement(movementComponent)) {
        return dg::horse::AdjustSpeedResult(
            movementComponent,
            nativeSpeed
        );
    }

    // Keep player identity exactly as the already validated V0.14F path:
    // locally controlled pawn -> local player.
    //
    // JumpZ / DoubleJumpZ / Glide validation protects only those property
    // writes. It must never decide whether Movement/Recovery/Damage are allowed.
    if (!IsLocallyControlledMayhemCharacter(characterOwner)) {
        return nativeSpeed;
    }

    // APawn::IsLocallyControlled is true for more than the actual player
    // during mount transitions. Refresh the persistent player pointer only when
    // this movement component also proves it is a real player movement object
    // through the validated Jump/DoubleJump/Glide property set.
    const bool playerMovementValidated =
        ApplyPlayerMovementTunings(movementComponent);

    if (playerMovementValidated) {
        // This stores only an identity and freshness timestamp; no pointer is
        // dereferenced from Present or after the movement callback expires.
        g_tpsLiveMovementForYaw.store(movementComponent,std::memory_order_release);
        g_tpsLiveMovementYawTick.store(GetTickCount64(),std::memory_order_release);
        CaptureLiveTPSActorYaw(characterOwner);
        UpdateTPSFacingNative(movementComponent);
        // Keep the existing single active-player pointer only for player-only
        // features. Horse discovery is fully independent in V0.26.
        void* previousPlayer =
            g_localPlayerCharacter.exchange(
                characterOwner,
                std::memory_order_relaxed
            );
        if (previousPlayer != characterOwner) {
            g_tpsPlayerGeneration.fetch_add(1);
            Log(
                "Player identity: active player=%p movement=%p (validated player movement)",
                characterOwner,
                movementComponent
            );
            // V0.33's automatic WM_SETCURSOR refresh was ineffective.
            // V0.34 deliberately makes the next test opt-in (F5).
        }
    }

    if (!runtime.movementSpeedEnabled.load(std::memory_order_relaxed) || nativeSpeed <= 0.0f) {
        return nativeSpeed;
    }

    // EMovementMode: 1 = Walking, 2 = NavWalking.
    const unsigned char movementMode = *(component + 0x1B0);
    if (movementMode != 1 && movementMode != 2) {
        return nativeSpeed;
    }

    const float multiplier =
        ClampFloat(runtime.movementSpeedMultiplier.load(std::memory_order_relaxed), 0.0f, 5.00f);
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

    if (!dg::runtime::Get().actionRecoveryEnabled.load(std::memory_order_relaxed) || nativeEnabled) {
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

    float delayMs = dg::runtime::Get().actionRecoveryDelayMs.load(std::memory_order_relaxed);
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

bool InstallActionRecoveryV08B() {
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

bool HookUiIsCursorVisible(void* uiManager) {
    const bool nativeVisible =
        g_originalUiIsCursorVisible
            ? g_originalUiIsCursorVisible(uiManager)
            : false;

    const auto& runtime = dg::runtime::Get();
    const ULONGLONG tick=runtime.tpsGameplayViewTick.load();
    const ULONGLONG now=GetTickCount64();
    const bool tpsLockedReticle =
        runtime.thirdPersonEnabled.load() &&
        runtime.tpsViewGameplay.load() &&
        tick&&now>=tick&&now-tick<250ull;
    const bool hiddenByMod =
        g_hudHidden.load(std::memory_order_relaxed) ||
        runtime.hideReticle.load(std::memory_order_relaxed) ||
        tpsLockedReticle;
    const int nativeState = nativeVisible ? 1 : 0;
    const int lastNative = g_lastNativeCursorVisible.exchange(nativeState);
    if (lastNative != nativeState)
        Log("CursorProbe V0.42: native UI visibility %d -> %d manager=%p hideReticle=%d hudHidden=%d",
            lastNative, nativeState, uiManager,
            runtime.hideReticle.load(std::memory_order_relaxed) ? 1 : 0,
            g_hudHidden.load(std::memory_order_relaxed) ? 1 : 0);

    const auto calls =
        g_cursorVisibilityCalls.fetch_add(1) + 1;

    if (calls <= 12 || (calls % 500) == 0) {
        Log(
            "Cursor visibility: call=%u manager=%p native=%d hideHud=%d result=%d",
            calls,
            uiManager,
            nativeVisible ? 1 : 0,
            hiddenByMod ? 1 : 0,
            hiddenByMod ? 0 : (nativeVisible ? 1 : 0)
        );
    }

    return hiddenByMod ? false : nativeVisible;
}

bool InstallCursorVisibilityHook() {
    HMODULE module = GetModuleHandleW(nullptr);
    if (!module) {
        Log("Cursor visibility hook: main module unavailable");
        return false;
    }

    BYTE* target =
        reinterpret_cast<BYTE*>(module) + 0x715800;

    static constexpr BYTE kPrefix[] = {
        0x40, 0x53,
        0x48, 0x83, 0xEC, 0x20,
        0x48, 0x8D, 0x99, 0xA8, 0x00, 0x00, 0x00
    };

    if (memcmp(target, kPrefix, sizeof(kPrefix)) != 0) {
        Log(
            "Cursor visibility hook: target validation FAILED RVA=0x715800"
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        Log(
            "Cursor visibility hook: MinHook init FAILED status=%d",
            static_cast<int>(initStatus)
        );
        return false;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(
            &HookUiIsCursorVisible),
        reinterpret_cast<LPVOID*>(
            &g_originalUiIsCursorVisible)
    );

    if (status != MH_OK &&
        status != MH_ERROR_ALREADY_CREATED) {
        Log(
            "Cursor visibility hook: create FAILED status=%d",
            static_cast<int>(status)
        );
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK &&
        status != MH_ERROR_ENABLED) {
        Log(
            "Cursor visibility hook: enable FAILED status=%d",
            static_cast<int>(status)
        );
        return false;
    }

    g_cursorVisibilityHookReady.store(true);
    Log(
        "Cursor visibility hook: READY UAirshipUIManager::IsCursorVisible RVA=0x715800 "
        "Hide HUD or Hide Reticle forces native UI cursor invisible"
    );
    return true;
}


// Hide Reticle is not Toggle HUD: the malformed cross can be an OS cursor.
// Keep this Win32 fallback gated to our actual foreground game window and
// disabled while the ImGui overlay is open. No mouse input is synthesized.
bool ShouldBlankGameCursor() {
    const HWND hwnd = reinterpret_cast<HWND>(
        g_gameWindowTrace.load(std::memory_order_relaxed));
    const auto& runtime = dg::runtime::Get();
    const ULONGLONG tick=runtime.tpsGameplayViewTick.load();
    const ULONGLONG now=GetTickCount64();
    const bool hideWindows = runtime.hideReticle.load() ||
        (runtime.thirdPersonEnabled.load() && runtime.tpsViewGameplay.load() &&
         tick&&now>=tick&&now-tick<250ull);
    return hwnd && GetForegroundWindow() == hwnd &&
        !g_overlayVisible.load(std::memory_order_relaxed) && hideWindows;
}

HCURSOR WINAPI HookSetCursor(HCURSOR requested) {
    if (!g_originalSetCursor) return nullptr;
    const HWND hwnd = reinterpret_cast<HWND>(
        g_gameWindowTrace.load(std::memory_order_relaxed));
    if (hwnd && GetForegroundWindow() == hwnd &&
        !g_overlayVisible.load(std::memory_order_relaxed)) {
        const HCURSOR previous = g_lastRequestedGameCursor.exchange(requested);
        if (previous != requested) {
            const uint32_t n = g_cursorProbeChanges.fetch_add(1) + 1;
            if (n <= 40) {
                // Read-only callsite evidence: relative RVA distinguishes a
                // game's cursor-request origin from a system/User32 origin.
                void* caller = _ReturnAddress();
                const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
                const uintptr_t addr = reinterpret_cast<uintptr_t>(caller);
                const uintptr_t rva = addr >= base && addr - base < 0x03DDF000u
                    ? addr - base : 0;
                Log("CursorProbe V0.42: SetCursor change=%u thread=%lu before=%p requested=%p caller=%p gameCallerRVA=0x%llX",
                    n,static_cast<unsigned long>(GetCurrentThreadId()),previous,
                    requested,caller,static_cast<unsigned long long>(rva));
            }
        }
    }
    if (!ShouldBlankGameCursor()) return g_originalSetCursor(requested);
    const unsigned calls = g_reticleCursorIntercepts.fetch_add(1) + 1;
    if (calls <= 8 || calls % 500 == 0) {
        Log("Reticle V0.42: SetCursor intercepted=%u requested=%p -> NULL",
            calls, requested);
    }
    return g_originalSetCursor(nullptr);
}

bool InstallSetCursorHook() {
    const MH_STATUS init = MH_Initialize();
    if (init != MH_OK && init != MH_ERROR_ALREADY_INITIALIZED) return false;
    const MH_STATUS create = MH_CreateHookApi(L"user32.dll", "SetCursor",
        reinterpret_cast<LPVOID>(&HookSetCursor),
        reinterpret_cast<LPVOID*>(&g_originalSetCursor));
    if (create != MH_OK) {
        Log("Reticle V0.36: SetCursor create failed=%d", int(create));
        return false;
    }
    const MH_STATUS enable = MH_EnableHook(
        reinterpret_cast<LPVOID>(&SetCursor));
    // Some systems expose a forwarding thunk; enable by exact exported address.
    if (enable != MH_OK && enable != MH_ERROR_ENABLED) {
        Log("Reticle V0.36: SetCursor enable failed=%d", int(enable));
        return false;
    }
    g_setCursorHookReady.store(true);
    Log("Reticle V0.36: native UI + Win32 SetCursor suppression READY");
    return true;
}

void RefreshReticleCursorState() {
    const bool blank = ShouldBlankGameCursor();
    if (g_cursorBlankApplied.exchange(blank) == blank) return;
    if (blank) {
        if (g_originalSetCursor) g_originalSetCursor(nullptr);
        else SetCursor(nullptr);
        Log("Reticle V0.42: OS cursor hidden by existing Hide Reticle");
    } else {
        const HWND hwnd = reinterpret_cast<HWND>(
            g_gameWindowTrace.load(std::memory_order_relaxed));
        const bool foreground = hwnd && GetForegroundWindow() == hwnd;
        // Alt-Tab hardening: old logic restored only when foreground=true,
        // leaving the cursor NULL after switching away from the game.
        const HCURSOR arrow = LoadCursorW(nullptr, IDC_ARROW);
        if (g_originalSetCursor) g_originalSetCursor(arrow);
        else SetCursor(arrow);
        if (!foreground) {
            ClipCursor(nullptr);
        } else {
            PostMessageW(hwnd, WM_SETCURSOR,
                reinterpret_cast<WPARAM>(hwnd), MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
        }
        Log("Reticle V0.42: suppression ended foreground=%d", foreground ? 1 : 0);
    }
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
        return true;
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
        if (!g_hudHookReady.load()) {
            g_lastAction = "Toggle HUD [hook unavailable]";
            Log("F%d -> Toggle HUD ignored (native hook unavailable)", functionKey);
            return;
        }

        const bool hidden = !g_hudHidden.load();
        g_hudHidden.store(hidden);
        // V0.33 WM_SETCURSOR repeats did not rebuild the reticle.
        g_lastAction = std::string("HUD ") + (hidden ? "hidden" : "visible");
        Log("F%d -> HUD %s", functionKey, hidden ? "HIDDEN" : "VISIBLE");
        return;
    }

    if (action == Action::ToggleReticle) {
        g_config.hideReticle = !g_config.hideReticle;
        g_config.Save();
        g_lastAction = g_config.hideReticle ? "Reticle hidden" : "Reticle visible";
        Log("F%d -> Hide Reticle %s", functionKey,
            g_config.hideReticle ? "ON" : "OFF");
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

    if (action == Action::ThirdPerson) {
        g_config.thirdPersonEnabled = !g_config.thirdPersonEnabled;
        g_config.Save();
        g_lastAction = g_config.thirdPersonEnabled ? "Third Person ON" : "Third Person OFF";
        Log("F%d -> Third Person %s", functionKey,
            g_config.thirdPersonEnabled ? "ON" : "OFF");
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


    // V0.55: every implemented switch is now assignable in F1-F12.
    // Startup-only features persist to INI and take effect on next boot.
    bool* toggle = nullptr;
    switch (action) {
    case Action::FOV: toggle=&g_config.fovEnabled; break;
    case Action::PistolDamage: toggle=&g_config.pistolDamageEnabled; break;
    case Action::MeleeDamage: toggle=&g_config.meleeDamageEnabled; break;
    case Action::JumpHeight: toggle=&g_config.jumpHeightEnabled; break;
    case Action::GlideDuration: toggle=&g_config.glideDurationEnabled; break;
    case Action::HorseSpeed: toggle=&g_config.horseSpeedEnabled; break;
    case Action::HorseSprintSpeed: toggle=&g_config.horseSprintSpeedEnabled; break;
    case Action::HorseSprintDuration: toggle=&g_config.horseSprintDurationEnabled; break;
    case Action::HotstreakCharge: toggle=&g_config.hotstreakChargeEnabled; break;
    case Action::SkipLogos: toggle=&g_config.skipLogosEnabled; break;
    case Action::SkipWarning: toggle=&g_config.skipWarningEnabled; break;
    default: break;
    }
    if (toggle) {
        *toggle = !*toggle;
        g_config.Save();
        bool accepted=true;
        if (action == Action::SkipLogos) accepted=dg::skip_logos::Apply(*toggle);
        if (action == Action::SkipWarning) accepted=dg::skip_logos::ApplyWarning(*toggle);
        const bool startup=action==Action::SkipLogos || action==Action::SkipWarning;
        g_lastAction=std::string(label)+(*toggle?" ON":" OFF")+
            (startup?" (restart required)":"");
        Log("F%d -> %s %s accepted=%d%s",functionKey,label,
            *toggle?"ON":"OFF",accepted?1:0,startup?" restart required":"");
        return;
    }

    // All eight existing camera operations can also be mapped to F-keys.
    float* cameraValue=nullptr;
    float cameraStep=0.0f, cameraMinimum=0.0f, cameraMaximum=0.0f;
    switch (action) {
    case Action::CameraHeightUp:
        cameraValue=&g_config.cameraHeightOffset; cameraStep=50; cameraMinimum=-1500; cameraMaximum=1500; break;
    case Action::CameraHeightDown:
        cameraValue=&g_config.cameraHeightOffset; cameraStep=-50; cameraMinimum=-1500; cameraMaximum=1500; break;
    case Action::CameraZoomOut:
        cameraValue=&g_config.cameraZoomPercent; cameraStep=-10; cameraMinimum=-75; cameraMaximum=200; break;
    case Action::CameraZoomIn:
        cameraValue=&g_config.cameraZoomPercent; cameraStep=10; cameraMinimum=-75; cameraMaximum=200; break;
    case Action::CameraPitchDown:
        cameraValue=&g_config.cameraPitchDegrees; cameraStep=-5; cameraMinimum=-35; cameraMaximum=35; break;
    case Action::CameraPitchUp:
        cameraValue=&g_config.cameraPitchDegrees; cameraStep=5; cameraMinimum=-35; cameraMaximum=35; break;
    case Action::CameraYawLeft:
        cameraValue=&g_config.cameraYawDegrees; cameraStep=-5; cameraMinimum=-180; cameraMaximum=180; break;
    case Action::CameraYawRight:
        cameraValue=&g_config.cameraYawDegrees; cameraStep=5; cameraMinimum=-180; cameraMaximum=180; break;
    default: break;
    }
    if (cameraValue) {
        *cameraValue=std::clamp(*cameraValue+cameraStep,cameraMinimum,cameraMaximum);
        g_config.Save();
        g_lastAction=std::string(label)+" = "+std::to_string(*cameraValue);
        Log("F%d -> %s value=%.2f",functionKey,label,*cameraValue);
        return;
    }
    if (action == Action::CameraReset) {
        g_config.cameraHeightOffset=0;
        g_config.cameraZoomPercent=0;
        g_config.cameraPitchDegrees=0;
        g_config.cameraYawDegrees=0;
        g_config.thirdPersonDistanceMultiplier=0.50f;
        g_config.thirdPersonPitchDegrees=-12.0f;
        g_config.thirdPersonHeightOffset=180.0f;
        g_config.tpsFootAnchorOffset=88.0f;
        dg::runtime::Get().cameraOrbitYawDegrees.store(0.0f);
        dg::runtime::Get().cameraOrbitPitchDegrees.store(0.0f);
        g_config.Save();
        g_lastAction="Camera values reset to vanilla";
        Log("F%d -> Camera Reset",functionKey);
        return;
    }

    if (!IsFeatureEnabled(action)) {
        g_lastAction = std::string(label) + " disabled in config";
        Log("F%d -> %s ignored (feature disabled)", functionKey, label);
        return;
    }

    g_lastAction = std::string(label) + " [not implemented]";
    Log("F%d -> %s (input OK, feature not implemented)", functionKey, label);
}

void ProcessInput() {
    const int capturedCamera = g_capturedCameraBinding.exchange(0);
    if (capturedCamera) {
        const int action = (capturedCamera >> 8) - 1;
        const int vk = capturedCamera & 255;
        if (action >= 0 && action < static_cast<int>(g_config.cameraKeys.size())) {
            for (size_t i = 0; i < g_config.cameraKeys.size(); ++i)
                if (i != static_cast<size_t>(action) && g_config.cameraKeys[i] == vk)
                    g_config.cameraKeys[i] = 0; // one camera action per key
            g_config.cameraKeys[static_cast<size_t>(action)] = vk;
            g_cameraKeyHeld.fill(false);
            g_cameraNextRepeat.fill(0);
            g_config.Save();
            g_lastAction = std::string("Camera ") +
                dg::config::kCameraLabels[static_cast<size_t>(action)] +
                " bound to " + KeyDisplayName(vk);
            Log("Camera V0.38: rebound %s to %s",
                dg::config::kCameraLabels[static_cast<size_t>(action)],
                KeyDisplayName(vk).c_str());
        }
    }
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
        g_captureCameraKeyIndex.load() < 0 &&
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


void ProcessCameraInput() {
    const HWND hwnd = reinterpret_cast<HWND>(
        g_gameWindowTrace.load(std::memory_order_relaxed));
    if (!hwnd || GetForegroundWindow() != hwnd ||
        g_overlayVisible.load() || g_captureCameraKeyIndex.load() >= 0) {
        g_cameraKeyHeld.fill(false);
        g_cameraNextRepeat.fill(0);
        return;
    }
    const ULONGLONG now = GetTickCount64();
    for (size_t i = 0; i < g_config.cameraKeys.size(); ++i) {
        const int vk = g_config.cameraKeys[i];
        if (vk < 1 || vk > 255 || vk == g_config.menuKey ||
            (vk >= VK_F1 && vk <= VK_F12)) continue;
        const bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
        if (!down) {
            g_cameraKeyHeld[i] = false;
            g_cameraNextRepeat[i] = 0;
            continue;
        }
        const bool initial = !g_cameraKeyHeld[i];
        if (!initial && now < g_cameraNextRepeat[i]) continue;
        g_cameraKeyHeld[i] = true;
        g_cameraNextRepeat[i] = now + (initial ? 290ull : 90ull);

        bool changed = false;
        float* value = nullptr;
        float delta = 0.0f, min = 0.0f, max = 0.0f;
        switch (static_cast<dg::config::CameraAction>(i)) {
        case dg::config::CameraAction::HeightUp:
            value=&g_config.cameraHeightOffset; delta=50.0f; min=-1500; max=1500; break;
        case dg::config::CameraAction::HeightDown:
            value=&g_config.cameraHeightOffset; delta=-50.0f; min=-1500; max=1500; break;
        case dg::config::CameraAction::ZoomOut:
            value=&g_config.cameraZoomPercent; delta=-10.0f; min=-75; max=200; break;
        case dg::config::CameraAction::ZoomIn:
            value=&g_config.cameraZoomPercent; delta=10.0f; min=-75; max=200; break;
        case dg::config::CameraAction::PitchDown:
            value=&g_config.cameraPitchDegrees; delta=-5.0f; min=-35; max=35; break;
        case dg::config::CameraAction::PitchUp:
            value=&g_config.cameraPitchDegrees; delta=5.0f; min=-35; max=35; break;
        case dg::config::CameraAction::YawLeft:
            value=&g_config.cameraYawDegrees; delta=-5.0f; min=-180; max=180; break;
        case dg::config::CameraAction::YawRight:
            value=&g_config.cameraYawDegrees; delta=5.0f; min=-180; max=180; break;
        default: break;
        }
        if (value) {
            const float previous=*value;
            *value=std::clamp(previous+delta,min,max);
            changed=*value != previous;
        }
        if (changed) {
            g_config.Save(); // update camera atomics now, defer INI writes
            g_lastAction=std::string("Camera ")+dg::config::kCameraLabels[i]+
                " = "+std::to_string(*value);
            const uint32_t count=++g_cameraActionRepeatCount[i];
            if (initial || count % 10 == 0)
                Log("Camera V0.38: key %s action=%s value=%.1f",
                    KeyDisplayName(vk).c_str(), dg::config::kCameraLabels[i], *value);
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


bool ShouldSuppressNativeAim() {
    // Fail open outside the exact supported game, on Alt-Tab and in the menu.
    const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load(std::memory_order_relaxed));
    return g_targetValidation.exact && !g_shuttingDown.load(std::memory_order_relaxed) &&
        dg::runtime::Get().thirdPersonEnabled.load(std::memory_order_relaxed) &&
        hwnd && hwnd==GetForegroundWindow() && !IsIconic(hwnd) &&
        !g_overlayVisible.load(std::memory_order_relaxed);
}

// Filtering happens only in the native controller API: our own camera
// polls the MinHook original trampoline to preserve unmodified stick data.
using NativeXInputGetStateFn=DWORD(WINAPI*)(DWORD, XINPUT_STATE*);
NativeXInputGetStateFn g_nativeXInputOriginal[5]{};
bool g_nativeXInputInstalled[5]{};
const wchar_t* const kNativeXInputLibraries[]={
    L"xinput1_4.dll",L"xinput1_3.dll",L"xinput9_1_0.dll",L"xinputuap.dll",L"xinput1_4.dll"
};
// V0.58: use the supported game API to steer its OWN native ranged aiming.
// Unlike changing arbitrary UObject transforms, this preserves game authority,
// animation, combat logic and movement-component lifetime safety.
// All heading changes are only active while RT/RB is held; the grenade/held
// item throw shares RT. LB radial / capability menus always receive vanilla.
DWORD FilterNativeXInput(int slot,DWORD user,XINPUT_STATE* state) {
    const auto original=g_nativeXInputOriginal[slot];
    if(!original)return ERROR_DEVICE_NOT_CONNECTED;
    const DWORD status=original(user,state);
    if(status!=ERROR_SUCCESS||!state||!ShouldSuppressNativeAim()||user!=0)
        return status; // Preserve vanilla menu, focus and co-op input.
    auto& pad=state->Gamepad;
    if(pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
        return status; // Ability wheel stays completely vanilla.

    auto& rt=dg::runtime::Get();
    const bool fire=pad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
        (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)!=0;
    g_tpsFireHeld.store(fire);
    if(fire) g_tpsFireLastTick.store(GetTickCount64());
    rt.tpsAimActive.store(fire);

    // V0.68: NEVER inject artificial XInput aim during RT/RB.
    // The same right-stick suppression applies while running, idle AND
    // firing. Our own camera separately reads the UNFILTERED trampoline.
    // This is input isolation, NOT yet a shot-direction fix.
    if(rt.tpsSuppressNativeRightStick.load() &&
       (pad.sThumbRX!=0 || pad.sThumbRY!=0)){
        pad.sThumbRX=0;pad.sThumbRY=0;
        const unsigned n=++g_nativePadAimSuppressed;
        if(fire){
            const unsigned count=++g_nativePadAimSuppressedDuringFire;
            if(count==1||count==10000)
                Log("TPS V0.69: native right-stick filtered DURING FIRE=%u slot=%d",count,slot);
        }
        if(n==1||n==10000)
            Log("TPS V0.69: native right-stick filtered=%u firing=%d",
                n,fire?1:0);
    }

    // Exposed as rebindable button: the gameplay action remains unchanged,
    // and the edge-triggered camera recenter uses RAW XInput in Present.
    return status;
}
DWORD WINAPI HookNativeXInput14(DWORD user,XINPUT_STATE* state) {return FilterNativeXInput(0,user,state);}
DWORD WINAPI HookNativeXInput13(DWORD user,XINPUT_STATE* state) {return FilterNativeXInput(1,user,state);}
DWORD WINAPI HookNativeXInput910(DWORD user,XINPUT_STATE* state) {return FilterNativeXInput(2,user,state);}
DWORD WINAPI HookNativeXInputUap(DWORD user,XINPUT_STATE* state) {return FilterNativeXInput(3,user,state);}
DWORD WINAPI HookNativeXInput14Ex(DWORD user,XINPUT_STATE* state) {return FilterNativeXInput(4,user,state);}
void EnsureNativeXInputAimHooks() {
    static ULONGLONG lastScan=0;
    const ULONGLONG now=GetTickCount64();
    if (lastScan && now-lastScan<2500ull) return;
    lastScan=now;
    void* const hooks[5]={
        reinterpret_cast<void*>(&HookNativeXInput14),
        reinterpret_cast<void*>(&HookNativeXInput13),
        reinterpret_cast<void*>(&HookNativeXInput910),
        reinterpret_cast<void*>(&HookNativeXInputUap),
        reinterpret_cast<void*>(&HookNativeXInput14Ex)
    };
    for (int i=0;i<5;++i) {
        if (g_nativeXInputInstalled[i]) continue;
        const HMODULE mod=GetModuleHandleW(kNativeXInputLibraries[i]);
        if (!mod) continue;
        FARPROC api=i==4
            ? GetProcAddress(mod,MAKEINTRESOURCEA(100)) // GetStateEx
            : GetProcAddress(mod,"XInputGetState");
        if (!api) continue;
        const MH_STATUS created=MH_CreateHook(
            reinterpret_cast<void*>(api),hooks[i],
            reinterpret_cast<void**>(&g_nativeXInputOriginal[i]));
        if (created!=MH_OK) continue;
        const MH_STATUS enabled=MH_EnableHook(reinterpret_cast<void*>(api));
        if (enabled!=MH_OK && enabled!=MH_ERROR_ENABLED) {
            MH_RemoveHook(reinterpret_cast<void*>(api));
            g_nativeXInputOriginal[i]=nullptr;
            continue;
        }
        g_nativeXInputInstalled[i]=true;
        Log("Aim V0.57: native XInput input gate READY library=%d export=%s",
            i,i==4?"GetStateEx":"GetState");
    }
}
// Always prefer the original unfiltered API for our camera.
// Controllers may not use XInput (Steam Input, HID); those paths need native tracing.
DWORD GetCameraXInputState(DWORD user,XINPUT_STATE* state) {
    if (!state) return ERROR_BAD_ARGUMENTS;
    for (int i=0;i<4;++i) {
        if (g_nativeXInputOriginal[i] &&
            g_nativeXInputOriginal[i](user,state)==ERROR_SUCCESS) return ERROR_SUCCESS;
    }
    HMODULE mod=GetModuleHandleW(L"xinput1_4.dll");
    if (!mod) mod=LoadLibraryW(L"xinput1_4.dll");
    if (!mod) mod=GetModuleHandleW(L"xinput1_3.dll");
    if (!mod) mod=GetModuleHandleW(L"xinput9_1_0.dll");
    if (!mod) return ERROR_DEVICE_NOT_CONNECTED;
    const auto api=reinterpret_cast<NativeXInputGetStateFn>(
        GetProcAddress(mod,"XInputGetState"));
    return api?api(user,state):ERROR_DEVICE_NOT_CONNECTED;
}

LRESULT CALLBACK OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // V0.72 controller-only synthetic WM_MOUSEMOVE guard. Previous blanket
    // WM_INPUT suppression broke mouse aiming, so RAWINPUT always passes.
    // Skip guard when a real RAW mouse input was observed very recently.
    if(msg==WM_MOUSEMOVE && ShouldSuppressNativeAim() &&
       dg::runtime::Get().tpsSuppressNativeRightStick.load()){
        const ULONGLONG now=GetTickCount64();
        const ULONGLONG stick=g_tpsPadRightStickTick.load(std::memory_order_acquire);
        const ULONGLONG raw=g_tpsRawMouseTick.load(std::memory_order_acquire);
        if(stick&&now>=stick&&now-stick<130ull&&
           (!raw||now<raw||now-raw>130ull)){
            const unsigned n=++g_tpsControllerMouseBlocks;
            if(n<=3||n==1000||n==10000)
                Log("TPS V0.72: controller-associated WM_MOUSEMOVE blocked=%u (possible Steam Input mouse emulation)",n);
            return 0;
        }
    }
    // Read mouse deltas without consuming native game input.
    static bool primed=false;
    static int oldX=0,oldY=0;
    const bool active=g_config.thirdPersonEnabled && g_config.cameraMouseOrbitEnabled &&
        !g_overlayVisible.load() && GetForegroundWindow()==hwnd;
    if (!active) primed=false;
    // V0.69: do not consume native WM_INPUT/WM_MOUSEMOVE.
    // The broken flat-mouse isolation froze the game's target coordinates
    // while moving only the mod camera, causing shots at stale points.
    // We observe deltas for camera orbit, then pass events to the game.
    if (msg==WM_INPUT && (active || ShouldSuppressNativeAim())) {
        RAWINPUT raw{};
        UINT size=sizeof(raw);
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),RID_INPUT,&raw,&size,
            sizeof(RAWINPUTHEADER))==size && raw.header.dwType==RIM_TYPEMOUSE) {
            g_tpsRawMouseTick.store(GetTickCount64(),std::memory_order_release);
            if (!(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)) {
                const LONG dx=raw.data.mouse.lLastX,dy=raw.data.mouse.lLastY;
                if (std::abs(dx)<=250 && std::abs(dy)<=250) {
                    g_orbitMouseDx.fetch_add(static_cast<int>(dx));
                    g_orbitMouseDy.fetch_add(static_cast<int>(dy));
                    g_lastOrbitRawTick.store(GetTickCount64());
                }
            }
        }
    }
    if (msg==WM_MOUSEMOVE) {
        const int x=static_cast<short>(LOWORD(lParam));
        const int y=static_cast<short>(HIWORD(lParam));
        if (active && primed && GetTickCount64()-g_lastOrbitRawTick.load()>1500ull) {
            const int dx=x-oldX,dy=y-oldY;
            if (std::abs(dx)<=180 && std::abs(dy)<=180) {
                g_orbitMouseDx.fetch_add(dx);
                g_orbitMouseDy.fetch_add(dy);
            }
        }
        oldX=x;oldY=y;primed=active;
    }
    // Never intercept focus/activation messages needed for Alt-Tab.
    const bool focusMessage = msg == WM_ACTIVATEAPP || msg == WM_ACTIVATE ||
        msg == WM_SETFOCUS || msg == WM_KILLFOCUS || msg == WM_MOUSEACTIVATE;
    if (focusMessage) {
        // Restore a cursor IMMEDIATELY on Alt-Tab instead of depending on
        // background Present calls, which some games stop issuing.
        if ((msg == WM_ACTIVATEAPP && wParam == FALSE) ||
            msg == WM_KILLFOCUS ||
            (msg == WM_ACTIVATE && LOWORD(wParam) == WA_INACTIVE)) {
            if (g_cursorBlankApplied.exchange(false)) {
                HCURSOR arrow = LoadCursorW(nullptr, IDC_ARROW);
                if (g_originalSetCursor) g_originalSetCursor(arrow);
                else SetCursor(arrow);
                ClipCursor(nullptr);
                Log("Focus V0.38: synchronous reticle cursor restore on deactivation");
            }
        }
        // Feed the ImGui backend the focus transition too, but never consume
        // it: otherwise its WantCaptureKeyboard state can become stale.
        if (g_imguiReady.load()) {
            ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
        }
        if (msg == WM_ACTIVATEAPP || msg == WM_ACTIVATE ||
            msg == WM_SETFOCUS || msg == WM_KILLFOCUS)
            Log("Focus V0.38: msg=0x%04X wParam=%zu overlay=%d",
                static_cast<unsigned>(msg), static_cast<size_t>(wParam),
                g_overlayVisible.load() ? 1 : 0);
        return g_originalWndProc
            ? CallWindowProcW(g_originalWndProc, hwnd, msg, wParam, lParam)
            : DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    if (g_imguiReady.load() && g_overlayVisible.load()) {
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
        const int cameraCapture = g_captureCameraKeyIndex.load();
        if (cameraCapture >= 0 && cameraCapture < static_cast<int>(g_config.cameraKeys.size()) &&
            (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)) {
            const int vk = static_cast<int>(wParam & 0xFF);
            if (vk == VK_ESCAPE) {
                g_captureCameraKeyIndex.store(-1);
                g_lastAction = "Camera binding cancelled";
                return TRUE;
            }
            if (vk != g_config.menuKey && vk != VK_ESCAPE &&
                (vk < VK_F1 || vk > VK_F12) && vk > 0 && vk < 256) {
                g_capturedCameraBinding.store(((cameraCapture + 1) << 8) | vk);
                g_captureCameraKeyIndex.store(-1);
                return TRUE;
            }
            return TRUE;
        }

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

        const bool altSystemKey =
            (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) &&
            ((GetKeyState(VK_MENU) & 0x8000) != 0 || (lParam & (1LL << 29)) != 0);
        const bool keyboardMessage =
            msg == WM_KEYDOWN || msg == WM_KEYUP ||
            ((msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) && !altSystemKey) ||
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

    if (!g_originalWndProc) {
        SetLastError(0);
        g_originalWndProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(g_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(OverlayWndProc))
        );
    }
    if (!g_originalWndProc) {
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

dg::overlay::Context BuildOverlayContext() {
    dg::overlay::Context context{};
    context.build = kBuild;
    context.config = &g_config;
    context.targetValidation = &g_targetValidation;

    context.overlayVisible = &g_overlayVisible;
    context.captureMenuKey = &g_captureMenuKey;
    context.captureCameraKeyIndex = &g_captureCameraKeyIndex;
    context.hudHidden = &g_hudHidden;
    context.lastAction = &g_lastAction;

    auto& t = context.telemetry;
    t.hudHookReady = g_hudHookReady.load();
    t.reticleCursorHookReady = g_setCursorHookReady.load();
    t.movementHookReady = g_movementHookReady.load();
    t.recoveryHookReady = g_recoveryHookReady.load();
    t.finalDamageHookReady =
        g_finalOutgoingDamageHookReady.load();
    t.hotstreakHookReady = g_hotstreakHookReady.load();
    t.skipIntroReady = g_skipIntroReady.load();

    const dg::skip_logos::Telemetry skipLogosTelemetry =
        dg::skip_logos::GetTelemetry();
    t.skipLogosProxyAvailable =
        skipLogosTelemetry.proxyAvailable;
    t.skipLogosTargetValid =
        skipLogosTelemetry.targetValid;
    t.skipLogosPatched =
        skipLogosTelemetry.patched;
    t.skipLogosEnabled =
        skipLogosTelemetry.enabled;
    t.skipWarningAttempted = skipLogosTelemetry.warningAttempted;
    t.skipWarningApplied = skipLogosTelemetry.warningApplied;

    t.actionMoveQueries = g_actionMoveQueries.load();
    t.actionMoveLocalQueries =
        g_actionMoveLocalQueries.load();
    t.actionMoveNativeBlocked =
        g_actionMoveNativeBlocked.load();
    t.actionMoveForced = g_actionMoveForced.load();
    t.lastActionMoveState =
        g_lastActionMoveState.load();
    t.lastActionMoveElapsed =
        g_lastActionMoveElapsed.load();

    t.pistolDamageBoostCalls =
        g_pistolDamageBoostCalls.load();
    t.lastNativePistolDamage =
        g_lastNativePistolDamage.load();
    t.lastBoostedPistolDamage =
        g_lastBoostedPistolDamage.load();
    t.lastPistolBaseJuice =
        g_lastPistolBaseJuice.load();

    t.meleeDamageBoostCalls =
        g_meleeDamageBoostCalls.load();
    t.lastNativeBaseDamage =
        g_lastNativeBaseDamage.load();
    t.lastBoostedBaseDamage =
        g_lastBoostedBaseDamage.load();
    t.lastOutgoingScaleType =
        g_lastOutgoingScaleType.load();
    t.lastOutgoingTagCount =
        g_lastOutgoingTagCount.load();

    t.hotstreakBoostCalls =
        g_hotstreakBoostCalls.load();
    t.lastNativeJuiceGain =
        g_lastNativeJuiceGain.load();
    t.lastBoostedJuiceGain =
        g_lastBoostedJuiceGain.load();

    t.skipIntroData = g_skipIntroData;
    t.skipIntroOriginalValue =
        g_skipIntroOriginalValue;

    context.applySkipIntro =
        &ApplySkipIntroSetting;
    context.applySkipWarning = &dg::skip_logos::ApplyWarning;
    context.applySkipLogos =
        &dg::skip_logos::Apply;
    context.applyGraphicsAdapter =
        &dg::engine_ini::ApplyGraphicsAdapter;
    context.abilityStateName =
        &AbilityStateName;
    context.log = &Log;

    return context;
}


void TraceDesktopCursorState(const char* reason, bool force) {
    const ULONGLONG now = GetTickCount64();
    const ULONGLONG due = g_cursorNextSnapshotTick.load(std::memory_order_relaxed);
    if (!force && now < due) return;
    g_cursorNextSnapshotTick.store(now + 350ull, std::memory_order_relaxed);

    const HWND hwnd = reinterpret_cast<HWND>(
        g_gameWindowTrace.load(std::memory_order_relaxed));
    if (!hwnd || !IsWindow(hwnd)) return;
    const bool foreground = GetForegroundWindow() == hwnd;
    CURSORINFO ci{};
    ci.cbSize = sizeof(ci);
    const BOOL ok = GetCursorInfo(&ci);
    if (!ok) return;
    const HCURSOR previous = g_lastObservedDesktopCursor.exchange(ci.hCursor);
    const DWORD oldFlags = g_lastObservedDesktopFlags.exchange(ci.flags);
    const bool oldForeground = g_lastCursorSnapshotForeground.exchange(foreground);
    if (!force && previous == ci.hCursor && oldFlags == ci.flags &&
        oldForeground == foreground) return;
    const unsigned count = g_cursorSnapshotCount.fetch_add(1) + 1;
    if (count > 70) return;

    // GetIconInfo allocates GDI bitmaps. Release BOTH on every successful call.
    ICONINFO icon{};
    const BOOL hasIcon = ci.hCursor && GetIconInfo(ci.hCursor, &icon);
    BITMAP mask{};
    BITMAP color{};
    if (hasIcon && icon.hbmMask) GetObjectW(icon.hbmMask, sizeof(mask), &mask);
    if (hasIcon && icon.hbmColor) GetObjectW(icon.hbmColor, sizeof(color), &color);
    if (hasIcon && icon.hbmMask) DeleteObject(icon.hbmMask);
    if (hasIcon && icon.hbmColor) DeleteObject(icon.hbmColor);

    RECT clip{};
    const BOOL clipped = GetClipCursor(&clip);
    Log("CursorProbe V0.42: snapshot #%u reason=%s gameForeground=%d overlay=%d hideReticle=%d desktopCursor=%p flags=0x%lX mouse=%ld,%ld "
        "iconInfo=%d hotspot=%lu,%lu mask=%ldx%ld color=%ldx%ld clipValid=%d clip=%ld,%ld,%ld,%ld lastRequest=%p nativeVisible=%d",
        count,reason,foreground ? 1 : 0,g_overlayVisible.load() ? 1 : 0,
        dg::runtime::Get().hideReticle.load(std::memory_order_relaxed) ? 1 : 0,
        ci.hCursor,static_cast<unsigned long>(ci.flags),
        ci.ptScreenPos.x,ci.ptScreenPos.y,hasIcon ? 1 : 0,
        hasIcon ? static_cast<unsigned long>(icon.xHotspot) : 0ul,
        hasIcon ? static_cast<unsigned long>(icon.yHotspot) : 0ul,
        mask.bmWidth,mask.bmHeight,color.bmWidth,color.bmHeight,
        clipped ? 1 : 0,clip.left,clip.top,clip.right,clip.bottom,
        g_lastRequestedGameCursor.load(std::memory_order_relaxed),
        g_lastNativeCursorVisible.load(std::memory_order_relaxed));
}

void TraceGameWindowState(IDXGISwapChain* swapChain) {
    if (!swapChain) {
        return;
    }

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swapChain->GetDesc(&desc)) ||
        !desc.OutputWindow) {
        return;
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(desc.OutputWindow, &pid);
    if (pid != GetCurrentProcessId()) {
        return;
    }

    g_gameWindowTrace.store(
        desc.OutputWindow,
        std::memory_order_relaxed
    );

    const bool foreground =
        GetForegroundWindow() == desc.OutputWindow;
    const int state = foreground ? 1 : 0;
    const int previous =
        g_lastForegroundState.exchange(state);

    if (previous != state) {
        RECT client{};
        GetClientRect(desc.OutputWindow, &client);

        const UINT clientWidth =
            client.right > client.left
                ? static_cast<UINT>(
                    client.right - client.left)
                : 0;
        const UINT clientHeight =
            client.bottom > client.top
                ? static_cast<UINT>(
                    client.bottom - client.top)
                : 0;

        CURSORINFO cursorInfo{};
        cursorInfo.cbSize = sizeof(cursorInfo);
        GetCursorInfo(&cursorInfo);

        const auto count =
            g_focusTransitionCount.fetch_add(1) + 1;

        Log(
            "Window focus trace #%u: foreground=%d hwnd=%p iconic=%d "
            "client=%ux%u swapDesc=%ux%u format=%u windowed=%d "
            "cursor=%p showing=%d pos=%ld,%ld",
            count,
            state,
            desc.OutputWindow,
            IsIconic(desc.OutputWindow) ? 1 : 0,
            clientWidth,
            clientHeight,
            desc.BufferDesc.Width,
            desc.BufferDesc.Height,
            static_cast<unsigned>(
                desc.BufferDesc.Format),
            desc.Windowed ? 1 : 0,
            cursorInfo.hCursor,
            (cursorInfo.flags & CURSOR_SHOWING) ? 1 : 0,
            cursorInfo.ptScreenPos.x,
            cursorInfo.ptScreenPos.y
        );

        // Focus events are evidence only. No synthetic recovery is queued.
    }
}


void EnsureOrbitWndProc(IDXGISwapChain* chain) {
    if (g_originalWndProc || !g_config.thirdPersonEnabled || !chain) return;
    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(chain->GetDesc(&desc)) || !desc.OutputWindow) return;
    DWORD pid=0;
    GetWindowThreadProcessId(desc.OutputWindow,&pid);
    if (pid!=GetCurrentProcessId()) return;
    SetLastError(0);
    auto previous=reinterpret_cast<WNDPROC>(
        SetWindowLongPtrW(desc.OutputWindow,GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(OverlayWndProc)));
    if (previous) {
        g_originalWndProc=previous;
        g_hwnd=desc.OutputWindow;
        Log("Aim V0.57: native mouse input gate installed without ImGui initialization");
    } else if (GetLastError()!=0) {
        Log("Aim V0.57: native mouse input gate failed error=%lu",GetLastError());
    }
}
void UpdateOrbitInput() {
    static WORD lastButtons=0;
    auto& rt=dg::runtime::Get();
    const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    const bool active=g_config.thirdPersonEnabled &&
        (g_config.cameraMouseOrbitEnabled||g_config.cameraControllerOrbitEnabled) &&
        hwnd && hwnd==GetForegroundWindow() && !IsIconic(hwnd) && !g_overlayVisible.load();
    const ULONGLONG now=GetTickCount64();
    const float dt=g_orbitLastPresentTick && now-g_orbitLastPresentTick<=100ull
        ? static_cast<float>(now-g_orbitLastPresentTick)*0.001f:0.0f;
    g_orbitLastPresentTick=now;
    const int dx=g_orbitMouseDx.exchange(0),dy=g_orbitMouseDy.exchange(0);
    if (!active) {
        lastButtons=0;
        if(!g_config.thirdPersonEnabled) {
            rt.cameraOrbitYawDegrees.store(0.0f);
            rt.cameraOrbitPitchDegrees.store(0.0f);
        }
        return;
    }
    // Configurable recenter input, default L3 (XINPUT_GAMEPAD_LEFT_THUMB).
    // Edge detection on the ORIGINAL XInput state, not the filtered
    // game-facing state. Recenter yaw toward live player facing.
    XINPUT_STATE primary{};
    WORD buttonBits=0;
    bool suspendControllerOrbitOnFire=false;
    if(GetCameraXInputState(0,&primary)==ERROR_SUCCESS) {
        buttonBits=primary.Gamepad.wButtons;
        // If native right-stick player aim is allowed, do not also orbit
        // our independent TPS camera from that same stick during RT/RB.
        // When block is ON, orbit remains active and native XInput is filtered.
        suspendControllerOrbitOnFire=!rt.tpsSuppressNativeRightStick.load() &&
            (primary.Gamepad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
             (buttonBits&XINPUT_GAMEPAD_RIGHT_SHOULDER)!=0);
        // Diagnostic/limited guard for controller software mapping the right
        // stick into Windows mouse movement IN ADDITION TO native XInput.
        // Never globally suppress real mouse input, and time out promptly.
        const bool rightActive=
            std::abs(int(primary.Gamepad.sThumbRX))>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE||
            std::abs(int(primary.Gamepad.sThumbRY))>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
        if(rt.tpsSuppressNativeRightStick.load()&&rightActive)
            g_tpsPadRightStickTick.store(now,std::memory_order_release);
        // Read-only probe: does the local pawn yaw change when ONLY the
        // right analog is moving, despite our native XInput suppression?
        // A positive result proves an additional game/input path must be
        // investigated, not that a second orientation write is safe.
        static ULONGLONG lastProbe=0;
        static float probeYaw=0.0f;
        const auto& pad=primary.Gamepad;
        const bool rightMoving=
            std::abs(int(pad.sThumbRX))>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
            std::abs(int(pad.sThumbRY))>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
        const bool leftIdle=
            std::abs(int(pad.sThumbLX))<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            std::abs(int(pad.sThumbLY))<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        const ULONGLONG actorTick=g_tpsActorYawTick.load();
        if(rt.tpsSuppressNativeRightStick.load()&&rightMoving&&leftIdle&&
           actorTick&&now>=actorTick&&now-actorTick<300ull) {
            const float yaw=g_tpsActorWorldYaw.load();
            if(lastProbe&&now>=lastProbe&&now-lastProbe>=650ull){
                const float gap=std::fabs(std::remainder(yaw-probeYaw,360.0f));
                static unsigned events=0;
                if(gap>8.0f&&++events<=12)
                    Log("TPS V0.70: yaw changed with RIGHT STICK ONLY while XInput gate ON delta=%.1f fire=%d (alternate input/game rotation)",
                        gap,(pad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD||
                           (buttonBits&XINPUT_GAMEPAD_RIGHT_SHOULDER))?1:0);
                lastProbe=now;probeYaw=yaw;
            }else if(!lastProbe||now<lastProbe){
                lastProbe=now;probeYaw=yaw;
            }
        }else{
            lastProbe=0;
        }
        const WORD mask=static_cast<WORD>(g_config.tpsRecenterButtonMask);
        if(mask && (buttonBits&mask) && !(lastButtons&mask)) {
            const bool success=dg::camera_trace::RecenterOnPlayer();
            Log("TPS V0.70: recenter button=0x%04X successful=%d",
                unsigned(mask),success?1:0);
        }
    }
    lastButtons=buttonBits;
    static bool priorSuspend=false;
    if(suspendControllerOrbitOnFire!=priorSuspend) {
        priorSuspend=suspendControllerOrbitOnFire;
        Log("TPS V0.69: controller camera orbit %s while native aim enabled",
            suspendControllerOrbitOnFire?"SUSPENDED (RT/RB)":"RESUMED");
    }
    float ax=0.0f,ay=0.0f;
    {
        for (DWORD i=0;i<XUSER_MAX_COUNT;++i) {
            if(!g_config.cameraControllerOrbitEnabled||suspendControllerOrbitOnFire) break;
            XINPUT_STATE state{};
            if (GetCameraXInputState(i,&state)!=ERROR_SUCCESS) continue;
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) continue;
            const float x=static_cast<float>(state.Gamepad.sThumbRX)/32767.0f;
            const float y=static_cast<float>(state.Gamepad.sThumbRY)/32767.0f;
            const float len=std::sqrt(x*x+y*y);
            const float dead=static_cast<float>(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)/32767.0f;
            if (len>dead) {
                const float m=std::min(1.0f,(len-dead)/(1.0f-dead));
                ax=x/len*m;ay=y/len*m;break;
            }
        }
    }
    const float mouse=g_config.cameraMouseSensitivity,stick=g_config.cameraStickSpeed;
    if (!std::isfinite(mouse)||!std::isfinite(stick)) return;
    const float dyaw=(g_config.cameraMouseOrbitEnabled?static_cast<float>(dx)*mouse:0.0f)+ax*stick*dt;
    const float dpitch=(g_config.cameraMouseOrbitEnabled?-static_cast<float>(dy)*mouse:0.0f)+ay*stick*dt;
    if (dyaw!=0.0f || dpitch!=0.0f) {
        rt.cameraOrbitYawDegrees.store(std::remainder(rt.cameraOrbitYawDegrees.load()+dyaw,360.0f));
        rt.cameraOrbitPitchDegrees.store(std::clamp(rt.cameraOrbitPitchDegrees.load()+dpitch,-60.0f,60.0f));
    }
}

HRESULT __stdcall HookPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
    ApplySkipIntroSetting(false);
    UpdateCameraDof();

    // V0.31 HorseFeature::Tick is intentionally a no-op. Horse writes are
    // allowed only while a native HorseCharacter callback proves the UObject
    // is alive; cached raw pointers are never written from Present().
    dg::horse::Tick();

    ProcessInput();
    g_config.FlushIfDue(false);

    TraceGameWindowState(swapChain);
    TraceDesktopCursorState("Present cursor state", false);
    ProcessCameraInput();
    EnsureOrbitWndProc(swapChain);
    EnsureNativeXInputAimHooks();
    UpdateOrbitInput();
    RefreshReticleCursorState();

    // V0.68: restore original V0.32 lazy overlay policy. No rendered TPS
    // crosshair and no needless ImGui frame during ordinary gameplay.
    if (!g_imguiReady.load() && g_overlayVisible.load()){
        Log("Overlay lazy-init requested on first menu open");
        InitializeImGui(swapChain);
    }
    if(g_imguiReady.load()&&swapChain==g_gameSwapChain&&g_overlayVisible.load()){
        if(GetForegroundWindow()==g_hwnd)ClipCursor(nullptr);
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::GetIO().MouseDrawCursor=true;
        auto context=BuildOverlayContext();
        dg::overlay::Draw(context);
        ImGui::Render();
        if(g_rtv){
            g_context->OMSetRenderTargets(1,&g_rtv,nullptr);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }else if(g_imguiReady.load()){
        ImGui::GetIO().MouseDrawCursor=false;
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
    DXGI_SWAP_CHAIN_DESC beforeDesc{};
    HWND traceHwnd = nullptr;
    bool processSwapChain = false;

    if (swapChain &&
        SUCCEEDED(swapChain->GetDesc(&beforeDesc)) &&
        beforeDesc.OutputWindow) {
        DWORD pid = 0;
        GetWindowThreadProcessId(
            beforeDesc.OutputWindow,
            &pid
        );
        if (pid == GetCurrentProcessId()) {
            processSwapChain = true;
            traceHwnd = beforeDesc.OutputWindow;

            const auto count =
                g_resizeBuffersTraceCount.fetch_add(1) + 1;

            Log(
                "ResizeBuffers trace #%u: BEFORE hwnd=%p "
                "requested=%ux%u count=%u format=%u flags=0x%X "
                "oldDesc=%ux%u oldFormat=%u foreground=%d",
                count,
                traceHwnd,
                width,
                height,
                bufferCount,
                static_cast<unsigned>(newFormat),
                swapChainFlags,
                beforeDesc.BufferDesc.Width,
                beforeDesc.BufferDesc.Height,
                static_cast<unsigned>(
                    beforeDesc.BufferDesc.Format),
                GetForegroundWindow() == traceHwnd ? 1 : 0
            );
        }
    }

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

    if (processSwapChain) {
        DXGI_SWAP_CHAIN_DESC afterDesc{};
        if (SUCCEEDED(swapChain->GetDesc(&afterDesc))) {
            Log(
                "ResizeBuffers trace: AFTER hwnd=%p hr=0x%08lX "
                "desc=%ux%u format=%u foreground=%d",
                traceHwnd,
                static_cast<unsigned long>(hr),
                afterDesc.BufferDesc.Width,
                afterDesc.BufferDesc.Height,
                static_cast<unsigned>(
                    afterDesc.BufferDesc.Format),
                GetForegroundWindow() == traceHwnd ? 1 : 0
            );
        } else {
            Log(
                "ResizeBuffers trace: AFTER hwnd=%p hr=0x%08lX desc=UNAVAILABLE",
                traceHwnd,
                static_cast<unsigned long>(hr)
            );
        }
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
    g_config.FlushIfDue(true);
    dg::skip_logos::Shutdown();
    dg::horse::Shutdown();
    RestoreCameraDof();
    Log("TPS V0.69 totals: mouseInputNative=PASSTHROUGH rightStickReadsFiltered=%u whileFiring=%u",
        g_nativePadAimSuppressed.load(),g_nativePadAimSuppressedDuringFire.load());
    Log("TPS V0.72 totals: controllerMappedMouseMessagesBlocked=%u rightStickFiltered=%u",
        g_tpsControllerMouseBlocks.load(),g_nativePadAimSuppressed.load());
    Log("TPS V0.73 totals: nativePhysicsCalls=%u yawOverrides=%u (experimental; actual strafe requires in-game check)",
        g_tpsNativeYawSeen.load(),g_tpsNativeYawOverrides.load());
    Log("TPS V0.63 totals: coneInside=%u coneOutside=%u reasserted=%u nativeLocks=%u restores=%u",
        g_tpsConeInside.load(),g_tpsConeOutside.load(),g_tpsFacingReasserted.load(),
        g_tpsNativeFacingOff.load(),g_tpsNativeFacingRestored.load());
    Log("TPS V0.63 totals: combatAim=%u fire=%u actorYaw=%u leftNative=%u movingFire=%u",
        g_tpsCombatAimSamples.load(),g_tpsCombatFireSamples.load(),g_tpsActorYawSamples.load(),
        g_tpsLeftPassthroughSamples.load(),g_tpsMovingFireSamples.load());

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

    g_config.SetPath(g_iniPath);
    g_config.SetLogger(&FeatureLog);
    g_config.Load();

    dg::engine_ini::Initialize(&FeatureLog);
    if (!dg::engine_ini::ApplyGraphicsAdapter(
            g_config.graphicsAdapter)) {
        Log(
            "Engine.ini GraphicsAdapter: startup apply failed; remaining mod features continue normally."
        );
    }

    g_targetValidation = dg::target::ValidateCurrentExecutable();
    Log(
        "Target validation: exact=%d size=%llu sha256=%s reason=%s",
        g_targetValidation.exact ? 1 : 0,
        static_cast<unsigned long long>(g_targetValidation.fileSize),
        g_targetValidation.sha256.empty()
            ? "(unavailable)"
            : g_targetValidation.sha256.c_str(),
        g_targetValidation.reason.c_str()
    );

    // Skip Logos bypasses the FEngineLoop selector that would play either the
    // EarlyStartupMovie path or its CustomSplashScreen fallback. The proxy
    // applies this before game entry; the ASI only binds control + telemetry.
    if (g_targetValidation.exact) {
        if (!dg::skip_logos::Initialize(&FeatureLog)) {
            Log("Skip Logos STARTUPSCREENS_ATTACH unavailable; remaining mod features continue normally.");
        }
    }

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

    // Native camera hook and scoped DOF compensation.
    InitializeCameraDof();
    dg::camera_trace::Install(&FeatureLog);
    dg::horse::Initialize(&FeatureLog);
    Log(
        "Player identity: exact V0.14F APawn::IsLocallyControlled path restored; Jump/Glide validation is non-blocking"
    );

    if (!InstallSkipIntroControl()) {
        Log("Skip Intro unavailable; continuing with remaining ASI features.");
    }
    if (!InstallHudHook()) {
        Log("Toggle HUD unavailable; renderer/input core remains active.");
    }
    if (!InstallSetCursorHook()) {
        Log("Reticle V0.36: Win32 hook unavailable, native UI layer still supported.");
    }
    if (!InstallCursorVisibilityHook()) {
        Log(
            "Cursor visibility hook unavailable; Hide HUD will keep native cursor behavior."
        );
    }
    if (!InstallMovementSpeedHook()) {
        Log("Movement Speed unavailable; other ASI features remain active.");
    }
    if (!InstallNativeTPSRotationHook()) {
        Log("TPS V0.73 native yaw hook unavailable; native rotation untouched.");
    }
    if (!InstallActionRecoveryV08B()) {
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
