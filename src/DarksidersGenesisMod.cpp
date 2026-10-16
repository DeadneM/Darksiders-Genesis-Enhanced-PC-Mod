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
#include "LocalPlayerOwnership.h"
#include "CoopCameraFeature.h"
#include "ChaosPoseFeature.h"
#include "ConfigStore.h"
#include "EngineIniFeature.h"
#include "GaugeChargeFeature.h"
#include "ResourceGainFeature.h"
#include "ManualGaugeFeature.h"
#include "AmmoConsumptionFeature.h"
#include "HotstreakFeature.h"
#include "PotionFeature.h"
#include "BuildInfo.h"
#include "HorseFeature.h"
#include "NativeTpsFeature.h"
#include "TpsStickRoles.h"
#include "TpsPadActions.h"
#include "OverlayUi.h"
#include "RuntimeSettings.h"
#include "ShaderAimFeature.h"
#include "ProjectileAimFeature.h"
#include "HealthFeature.h"
#include "GlideFeature.h"
#include "SkipLogosFeature.h"
#include "SkipWarningFeature.h"
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
#include <vector>
#include <cstdlib>

namespace {

constexpr const char* kBuild = dg::build::Id;
constexpr const wchar_t* kIniName = L"DarksidersGenesisMod.ini";
constexpr const wchar_t* kLogName = L"DarksidersGenesisMod.log";

HMODULE g_module = nullptr;
std::wstring g_iniPath;
std::wstring g_logPath;

// Optional V1.10 startup-only bridge to the local DXGI proxy.
// No game startup interception, asset rewrite, or call to MoviePlayer.
void ReportStartupPhase(unsigned phase,unsigned value=0){
    using TraceFn=void(WINAPI*)(unsigned,unsigned);
    HMODULE proxy=GetModuleHandleW(L"dxgi.dll");
    if(!proxy)return;
    auto trace=reinterpret_cast<TraceFn>(GetProcAddress(proxy,"DGReportStartupPhase"));
    if(trace)trace(phase,value);
}

using PresentFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
using ResizeBuffersFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
using HudHiddenGetterFn = bool(*)();
using CharacterGetMaxSpeedFn = float(*)(void*);
using AddJuiceFn = void(*)(void*, float);
using FilterOutgoingDamageFn = void(*)(void*, void*);

PresentFn g_originalPresent = nullptr;
ResizeBuffersFn g_originalResizeBuffers = nullptr;
HudHiddenGetterFn g_originalHudHiddenGetter = nullptr;
CharacterGetMaxSpeedFn g_originalCharacterGetMaxSpeed = nullptr;
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

// V0.59: actor-heading telemetry sampled ONLY from a live validated player
// movement callback. Never dereference a cached UObject in Present/XInput.
std::atomic<float> g_tpsActorWorldYaw{0.0f};
std::atomic_ullong g_tpsActorYawTick{0};
std::atomic_uint32_t g_tpsActorYawSamples{0};
std::atomic_bool g_tpsFireHeld{false};
std::atomic_ullong g_tpsFireLastTick{0};
std::atomic_bool g_tpsChargeChordHeld{false};
std::atomic_ullong g_tpsChargeChordTick{0};
std::atomic_uint32_t g_tpsChargeChordSamples{0};

std::atomic_uint32_t g_tpsPlayerGeneration{0};
// V0.72: track potential Steam Input -> synthetic Windows mouse mapping.
std::atomic_ullong g_tpsPadCameraStickTick{0};
// V0.99: V0.95 cursor-warp counter removed; all mouse warps were removed in V0.97.
// V0.92 experimental: a virtual forward-facing right stick is passed ONLY to
// the game's XInput while shooting mounted in CAMERA_FORWARD mode; the camera
// continues reading unmodified physical stick data through its own trampoline.
std::atomic_uint32_t g_tpsForwardAnalogInjected{0};
std::atomic_uint32_t g_tpsMountedForwardAnalogInjected{0};
std::atomic_uint32_t g_tpsFootForwardAnalogInjected{0};
std::atomic_uint32_t g_horsePadChordRemapped{0};
std::atomic_uint32_t g_keyboardHorseInjected{0};
// V0.76: physical R3 and the virtual native grenade key are distinct.
// Do not modify the validated V0.75 native firing/aim callback logic.
std::atomic_uint32_t g_tpsHoldSuspendCount{0},g_tpsHoldToggleCount{0};
std::atomic_uint32_t g_tpsGrenadeDownCount{0},g_tpsGrenadeUpCount{0};
std::atomic_uint32_t g_tpsGrenadeReleasePulseDelivered{0};

SRWLOCK g_grenadeHoldLock=SRWLOCK_INIT;
using GrenadeHoldState=dg::tps_input::EquipmentHoldState;
GrenadeHoldState g_grenadeHoldState{};
// V0.99 observational-only mouse audit. No synthetic cursor events or
// native shot/aim edits. Bound every live log, preserve controller paths.
ULONGLONG g_orbitLastPresentTick=0;
std::atomic_bool g_overlayVisible{false};
std::atomic_bool g_captureMenuKey{false};
std::atomic_int g_lastForegroundState{-1};
std::atomic_uint32_t g_focusTransitionCount{0};
std::atomic_uint32_t g_resizeBuffersTraceCount{0};
std::atomic<void*> g_gameWindowTrace{nullptr};
// Manual F5-only fallback retained for comparison; no auto focus simulation.
std::atomic_int g_capturedMenuKey{0};
std::atomic_int g_captureCameraKeyIndex{-1};
std::atomic_int g_capturedCameraBinding{0};
std::array<bool, static_cast<size_t>(dg::config::CameraAction::Count)> g_cameraKeyHeld{};
std::array<ULONGLONG, static_cast<size_t>(dg::config::CameraAction::Count)> g_cameraNextRepeat{};
std::array<uint32_t, static_cast<size_t>(dg::config::CameraAction::Count)> g_cameraActionRepeatCount{};
std::atomic_bool g_hudHidden{false};
std::atomic_bool g_hudHookReady{false};
// V1.18 NEW baseline-only mouse study. Read-only WndProc messages. We never
// consume input, set a cursor, warp position, write camera values or simulate a pad.
std::atomic_uint32_t g_v118MouseMove{0},g_v118RawInputMessages{0},
    g_v118MouseLmb{0},g_v118MouseRmb{0},g_v118MouseWheel{0};
// New V1.19 mouse path: no recycled pointer hooks, virtual XInput, cursor
// recenter/warp or synthetic mouse messages. Raw input stays native.
std::atomic_int g_v119MouseDeltaX{0},g_v119MouseDeltaY{0};
std::atomic_ullong g_v119LastRawMouseTick{0};
std::atomic_uint32_t g_v119RawMouse{0},g_v119MouseMoveFallback{0},
    g_v119MouseCameraFrames{0};

// V1.20: preserve existing game registrations and legacy mouse messages.
std::atomic_bool g_v120RawRegistrationChecked{false},g_v120RawAddedByMod{false};
std::atomic_uint32_t g_v120ExistingRawMouse{0},g_v120RawRegistrationErrors{0},
    g_v120RawMousePackets{0},g_v120RawNonMousePackets{0},g_v120RawAtEdge{0};
// Only limited repositioning at window edge, not a reticle visibility hook.
std::atomic_ullong g_v123LastPhysicalMouseTick{0},g_v123LastRecenterTick{0};
std::atomic_int g_v123RecenterClientX{0},g_v123RecenterClientY{0};
std::atomic_uint32_t g_v123EdgeRecenters{0},g_v123SyntheticMovesIgnored{0},
    g_v123RecenterFailures{0};
// Native mouse button routing counts; original XInput path stays untouched.
std::atomic_uint32_t g_v124PrimaryRouted{0},g_v124SecondaryRouted{0},
    g_v124NativeButtonSuppressed{0};

// V1.25: monitor actual process-wide RawInput ownership, never overwrite a
// native registration; re-arm only when there is no mouse registration.
std::atomic_uint32_t g_v125RawChecks{0},g_v125RawMissing{0},
    g_v125RawRearmAttempts{0},g_v125RawRearmSuccess{0},
    g_v125RawForeignOwners{0},g_v125RawStreamStalls{0},
    g_v125RawWarpAvoided{0};
std::atomic_int g_v125RawOwnerState{-1}; // 0 missing, 1 our hwnd, 2 native other hwnd
std::atomic_ullong g_v125LastPollTick{0};
// V1.26: remove the whole-session retry ceiling (V1.25 reached it in 5m).
// Instead throttle missing-registration rearm attempts and observe recovery.
std::atomic_ullong g_v126LastRearmAttemptTick{0};
std::atomic_uint32_t g_v126RearmThrottled{0},g_v126RecoveredAfterEight{0};
std::atomic_uint32_t g_v125LastProbeRawCount{0},g_v125LastProbeMoveCount{0};

std::atomic_bool g_movementHookReady{false};
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
    if (std::strncmp(message, "Window focus trace #", 20) == 0 ||
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
    // R3 temporary vanilla camera also restores the native DOF state;
    // persistent camera zoom, if enabled, remains an independent reason
    // for disabling depth-of-field blur.
    const bool displaced=(camera.thirdPersonEnabled.load(std::memory_order_relaxed) &&
        !camera.tpsTemporarilySuspended.load(std::memory_order_relaxed)) ||
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
            ReportStartupPhase(2,static_cast<unsigned>(*g_skipIntroData));
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
        if (dg::local_players::FindCharacter(owner)>=0) {
            float multiplier = dg::runtime::Get().hotstreakChargeMultiplier.load(std::memory_order_relaxed);
            if (multiplier < 0.0f) multiplier = 0.0f;
            if (multiplier > 100.0f) multiplier = 100.0f;

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

    if (dg::local_players::FindCharacter(playerCharacter)<0) {
        return;
    }
    // This records the native outgoing damage callback only, not a pickup
    // or a verified Wrath source. Compare timestamp with gauge samples.

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

    if (count <= 3 || (count % 1000) == 0) {
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

// Called only from a synchronous native movement/camera callback. The
// context supplies this player's current native state during possession.
void ServiceLocalGameplay(const dg::local_players::Context& owner,uint32_t generation){
    void* character=reinterpret_cast<void*>(owner.character);
    if(owner.index==0){
        g_tpsPlayerGeneration.store(generation,std::memory_order_release);
        dg::runtime::Get().tpsActorGeneration.store(generation,std::memory_order_release);
        void* previous=g_localPlayerCharacter.exchange(character,std::memory_order_acq_rel);
        if(owner.kind==dg::local_players::PoseKind::Foot){
            CaptureLiveTPSActorYaw(character);
            dg::horse::ObservePlayerCharacter(character);
        }else{
            dg::runtime::Get().tpsActorLocationTick.store(0,std::memory_order_release);
        }
        if(previous!=character)Log("Player identity: active player=%p (native LocalPlayers[0])",character);
    }
    dg::resource_gain::OnLivePlayer(character,generation);
    dg::manual_gauges::OnLivePlayer(character,generation,reinterpret_cast<void*>(owner.playerState));
    dg::ammo::OnLivePlayer(character,generation);
    dg::hotstreak::OnLivePlayer(character,generation);
    dg::potions::OnLivePlayer(character,generation,reinterpret_cast<void*>(owner.playerState));
    dg::health::OnLivePlayer(character,generation);
}

// V0.75: use the game's verified UMayhemCharacter native look functions,
// rather than an interpolation callback that did not execute in V0.74.
// RVA 0x671A40 = GetPlayerLookInput, referenced by game movement, aiming,
// and shooting logic; RVA 0x671B00 = GetPlayerLookRotation. Both are
// non-virtual native methods (not Unreal script thunks). Signatures and
// calling convention derived from the exact supported shipping executable.
// The hooks alter RETURN values only in these live game callbacks, never
// player/root/MovementComponent memory or left-stick input.
struct NativeTPSRotator { float pitch, yaw, roll; };
using NativeLookFn=NativeTPSRotator* (*)(void*,NativeTPSRotator*);
NativeLookFn g_originalGetPlayerLookInput=nullptr;
NativeLookFn g_originalGetPlayerLookRotation=nullptr;
std::atomic_uint32_t g_tpsLookInputCalls{0},g_tpsLookRotationCalls{0};
std::atomic_uint32_t g_tpsNativeLookInputBlocks{0};
std::atomic_uint32_t g_tpsNativeLookInputPadAim{0};
std::atomic_uint32_t g_tpsPadStrafeApplied{0},g_tpsNativeLookRotationBlocks{0};
std::atomic_uint32_t g_tpsNativeLookSignatureFailures{0};
// Mounted Vanilla Aim still suppresses native stick look while idle.
std::atomic_uint32_t g_tpsMountedIdleNativeLookBlocks{0};
// Visual-only audit of 2 OTHER suspected native targeting UI classes.
// No event is suppressed: the unvalidated V0.82 blocking experiment
// is NOT merged into this cumulative gameplay branch.
using AimUiEventDispatchFn=void(*)(void*,void*,void*,void*);
AimUiEventDispatchFn g_originalAimUiEventDispatch=nullptr;
void* const* g_aimUiClassSlot=nullptr;
void* const* g_aimUiPositionClassSlot=nullptr;
std::atomic_uint32_t g_aimUiEvents{0},g_aimUiPositionEvents{0};
std::atomic_uint32_t g_aimUiEventsInTPS{0},g_aimUiEventsWhileFire{0};

// Actual mounted TPS camera source is written by HookView, with a short
// expiry so stale HorseCharacter callbacks cannot make foot aim mounted.
bool IsMountedTPSNow() {
    auto& rt=dg::runtime::Get();
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG tick=rt.tpsMountedViewTick.load(std::memory_order_acquire);
    return rt.thirdPersonEnabled.load(std::memory_order_relaxed) &&
        !rt.tpsTemporarilySuspended.load(std::memory_order_relaxed) &&
        rt.tpsMountedViewActive.load(std::memory_order_acquire) &&
        tick&&now>=tick&&now-tick<300ull;
}
bool IsTPSRadialInputActive() {
    const auto now=GetTickCount64();
    // V1.10: no charged-key exception to LB's native radial selection.
    const auto& rt=dg::runtime::Get();
    if(!rt.tpsAbilityWheelHeld.load(std::memory_order_acquire))return false;
    const auto tick=rt.tpsAbilityWheelTick.load(std::memory_order_acquire);
    return tick&&now>=tick&&now-tick<=180ull;
}

bool IsLiveTPSLookActor(void* actor) {
    if(!actor || !g_targetValidation.exact || g_shuttingDown.load() ||
        !dg::runtime::Get().thirdPersonEnabled.load() ||
        dg::runtime::Get().tpsTemporarilySuspended.load() ||
        dg::runtime::Get().tpsCutsceneSuspended.load() ||
        dg::runtime::Get().tpsUiViewHeld.load() ||
        (dg::chaos_pose::ActiveNow()&&!dg::chaos_pose::RangedOwner(actor)) ||
        g_overlayVisible.load())
        return false;
    const auto* player=g_localPlayerCharacter.load(std::memory_order_acquire);
    if(actor!=player || !player)return false;
    const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    if(!hwnd || GetForegroundWindow()!=hwnd || IsIconic(hwnd))return false;
    const ULONGLONG tick=dg::runtime::Get().tpsActorLocationTick.load(std::memory_order_acquire);
    const ULONGLONG now=GetTickCount64();
    return (tick&&now>=tick&&now-tick<250ull)||IsMountedTPSNow()||dg::chaos_pose::RangedOwner(actor);
}
bool MousePhysicalHeld(int vk){return vk && (GetAsyncKeyState(vk)&0x8000)!=0;}
bool MousePrimaryFireHeld(){return MousePhysicalHeld(dg::runtime::Get().tpsMousePrimaryButton.load(std::memory_order_relaxed));}
bool MouseSecondaryFireHeld(){return MousePhysicalHeld(dg::runtime::Get().tpsMouseSecondaryButton.load(std::memory_order_relaxed));}
bool MouseAnyFireHeld(){return MousePrimaryFireHeld()||MouseSecondaryFireHeld();}
// V1.33: camera-center forward ray, independent of physical cursor pixels.
// Keep V1.32 native collision/local-owner scopes; never simulate XInput.
bool ComputeNativeMouseRay(dg::native_tps::MouseRay& ray){
    const HWND hwnd=reinterpret_cast<HWND>(
        g_gameWindowTrace.load(std::memory_order_acquire));
    if(!hwnd||GetForegroundWindow()!=hwnd||IsIconic(hwnd)||
       g_overlayVisible.load(std::memory_order_relaxed))return false;
    const auto t=dg::camera_trace::GetTelemetry();
    const ULONGLONG now=GetTickCount64();
    if(!t.appliedWorldTickMs||now<t.appliedWorldTickMs||
       now-t.appliedWorldTickMs>250ull||
       !std::isfinite(t.appliedYaw)||!std::isfinite(t.appliedPitch))return false;
    constexpr float rad=0.01745329251994329577f;
    const float az=t.appliedYaw*rad,el=t.appliedPitch*rad;
    const float cy=std::cos(az),sy=std::sin(az),
        cp=std::cos(el),sp=std::sin(el);
    const float dx=cp*cy;
    const float dy=cp*sy;
    const float dz=sp;
    const float flat=std::hypot(dx,dy);
    if(!std::isfinite(flat)||flat<0.01f||
       !std::isfinite(dz))return false;
    const float length=std::hypot(flat,dz);
    if(!std::isfinite(length)||length<0.01f)return false;
    ray.origin={t.appliedWorldX,t.appliedWorldY,t.appliedWorldZ};
    ray.direction={dx/length,dy/length,dz/length};
    return true;
}
bool IsPadFiringNow() {
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG tick=g_tpsFireLastTick.load(std::memory_order_acquire);
    return g_tpsFireHeld.load(std::memory_order_acquire)&&tick&&now>=tick&&now-tick<180ull;
}
bool IsCameraStickActiveNow() {
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG stick=g_tpsPadCameraStickTick.load(std::memory_order_acquire);
    // Track the selected physical camera stick, not a hardcoded right axis.
    return stick && now>=stick && now-stick<150ull;
}
bool IsTPSLookViewFresh() {
    const auto& rt=dg::runtime::Get();
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG view=rt.tpsGameplayViewTick.load(std::memory_order_acquire);
    return rt.tpsViewGameplay.load(std::memory_order_acquire) &&
        view&&now>=view&&now-view<250ull;
}
bool ProvideNativeMouseRay(void* actor,dg::native_tps::MouseRay& ray){
    const auto& rt=dg::runtime::Get();
    if(!actor||!g_targetValidation.exact||g_shuttingDown.load()||
       !rt.thirdPersonEnabled.load()||rt.tpsTemporarilySuspended.load()||
       rt.tpsCutsceneSuspended.load()||rt.tpsUiViewHeld.load()||
       (dg::chaos_pose::ActiveNow()&&!dg::chaos_pose::RangedActor(actor))||
       g_overlayVisible.load()||IsTPSRadialInputActive()||
       IsPadFiringNow()||IsCameraStickActiveNow()||
       !IsTPSLookViewFresh())return false;
    const bool mounted=IsMountedTPSNow();
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG horseTick=rt.tpsHorsePoseTick.load(std::memory_order_acquire);
    const bool local=actor==g_localPlayerCharacter.load(std::memory_order_acquire);
    const bool horse=mounted&&horseTick&&now>=horseTick&&now-horseTick<250ull&&
        reinterpret_cast<uintptr_t>(actor)==rt.tpsHorseOwnerIdentity.load();
    if(!local&&!horse&&!dg::chaos_pose::RangedActor(actor))return false;
    if(!ComputeNativeMouseRay(ray))return false;
    ray.mounted=mounted;
    ray.ultimate=dg::chaos_pose::RangedActor(actor);
    ray.fireHeld=MouseAnyFireHeld(); // telemetry only, not the action gate
    return true;
}
bool ProvideProjectileRotation(const dg::local_players::Context& owner,float& pitch,float& yaw){
    if(owner.index==1)return dg::coop_camera::ShotRotation(owner,pitch,yaw);
    if(owner.index!=0||!IsLiveTPSLookActor(reinterpret_cast<void*>(owner.character))||
       !IsTPSLookViewFresh()||!IsPadFiringNow()||MouseAnyFireHeld()||IsTPSRadialInputActive()||
       dg::runtime::Get().tpsMountedAimMode.load()!=0)return false;
    const auto view=dg::camera_trace::GetTelemetry();
    const auto now=GetTickCount64();
    if(!view.appliedWorldTickMs||now<view.appliedWorldTickMs||now-view.appliedWorldTickMs>=250)return false;
    pitch=view.appliedPitch;yaw=view.appliedYaw;
    return std::isfinite(pitch)&&std::isfinite(yaw);
}
// Candidate Wrath native location from exact-EXE AddWrath disassembly:
 // character+0xEA0 -> resource component, component+0x3A0 -> float.
 // This is an observation ONLY within the validated local player's live
 // native look callback; no writes, cached UObject dereferences or hooks.

NativeTPSRotator* HookPlayerLookInput(void* actor,NativeTPSRotator* out) {
    auto original=g_originalGetPlayerLookInput;
    if(!original)return out;
    auto* result=original(actor,out);
    if(result&&dg::local_players::CoopActive()&&dg::coop_camera::AdjustLook(actor,result->pitch,result->yaw,result->roll,false))return result;
    const unsigned calls=++g_tpsLookInputCalls;
    if(!result||!IsLiveTPSLookActor(actor) ||
       !std::isfinite(result->pitch)||!std::isfinite(result->yaw)||
       !std::isfinite(result->roll))
        return result;
    // Diagnostic BEFORE native-vector modification: sample the actual
    // live-player look caller and resource state for ordinary and charged
    // input. This is not a projectile hook and never alters any result.
    if(IsTPSRadialInputActive())return result;
    auto& rt=dg::runtime::Get();
    const bool mouseFiring=MouseAnyFireHeld();
    const bool padFiring=IsPadFiringNow();
    const bool fire=padFiring||mouseFiring;
    // V1.32: mouse acquisition belongs to native 3D targeting.
    // This getter is not projectile authority. Preserve all pad branches.
    if(mouseFiring&&!padFiring)return result;
    // V1.19: mouse LMB and controller use the SAME Camera Forward
    // heading. No alternative mouse mode, no pad emulation.
    // This changes ONLY the verified local player native TPS look getter.
    // Joystick OFF disables orbit, never the independent forward-fire path.
    // Mode 1: native look is passed through for vanilla horse aiming.
    // V0.87: Vanilla Aim means NATIVE look ONLY WHILE FIRING.
    // Off-fire the right stick is camera-only, never simultaneously
    // turning the horseman's aim. This is independent of on-foot toggles.
    if(rt.tpsMountedAimMode.load()==1 && !mouseFiring){
        if(fire)return result;
        if(IsCameraStickActiveNow()){
            result->pitch=0.0f;result->yaw=0.0f;result->roll=0.0f;
            const unsigned n=++g_tpsMountedIdleNativeLookBlocks;
            if(n==1||n==200)
                Log("TPS V0.87 MOUNT: idle right-stick native look suppressed n=%u",n);
        }
        return result;
    }
    // TPS aim/movement are independent: the camera-vector look path is a
    // fixed internal rule while firing, not an optional "strafe" switch.
    const bool padStrafeFiring=fire;
    if(padStrafeFiring&&IsTPSLookViewFresh()){
        // GetPlayerLookInput returns the native character's world-space
        // X/Y look VECTOR (RVA 0x671A40 reads actor+0xD24/0xD28).
        // Feed world-facing vector corresponding to the mod TPS view only
        // WHILE firing. Unlike rejected XInput axis remapping this leaves
        // native left-stick player movement COMPLETELY untouched.
        const float yaw=dg::camera_trace::AppliedYaw();
        if(std::isfinite(yaw)&&std::fabs(yaw)<36000.0f){
            const float rad=yaw*0.01745329251994329577f;
            result->pitch=std::cos(rad);
            result->yaw=std::sin(rad);
            result->roll=0.0f;
            const unsigned n=++g_tpsNativeLookInputPadAim;

            if(n==1||n==120||n==10000)
                Log("TPS V1.02: controller LOOK VECTOR -> camera during fire n=%u yaw=%.1f calls=%u",
                    n,yaw,calls);
            return result;
        }
    }
    if(rt.tpsCameraStickMode.load()==0||!IsCameraStickActiveNow())
        return result;
    // Existing XInput block remains enabled. This is a SECOND, precise
    // in-game look-vector gate for direct controller/HID paths. Our TPS
    // camera keeps raw original XInput and can orbit freely while firing.
    result->pitch=0.0f;result->yaw=0.0f;result->roll=0.0f;
    const unsigned blocked=++g_tpsNativeLookInputBlocks;
    if(blocked==1||blocked==120||blocked==10000)
        Log("TPS V0.75: native gamepad look input neutralized n=%u calls=%u",blocked,calls);
    return result;
}
NativeTPSRotator* HookPlayerLookRotation(void* actor,NativeTPSRotator* out) {
    auto original=g_originalGetPlayerLookRotation;
    if(!original)return out;
    auto* result=original(actor,out);
    if(result&&dg::local_players::CoopActive()&&dg::coop_camera::AdjustLook(actor,result->pitch,result->yaw,result->roll,true))return result;
    const unsigned calls=++g_tpsLookRotationCalls;
    if(!result||!IsLiveTPSLookActor(actor)||!IsTPSLookViewFresh())
        return result;
    if(IsTPSRadialInputActive())return result;
    auto& rt=dg::runtime::Get();
    const bool mouseFire=MouseAnyFireHeld();
    const bool padFire=IsPadFiringNow();
    // V1.19: native mouse firing and controller share TPS Camera Forward.
    const bool playerStrafe=padFire||mouseFire; // controller path preserved
    if(mouseFire&&!padFire)return result; // V1.32: native target, no yaw patch
    // During mounted Vanilla Aim fire, native look rotation is untouched.
    // While idle, suppress right-stick-origin horseman aim.
    if(rt.tpsMountedAimMode.load()==1 && padFire && !mouseFire)return result;
    const bool nativeBlock=rt.tpsCameraStickMode.load()!=0&&IsCameraStickActiveNow();
    const float actorYaw=g_tpsActorWorldYaw.load(std::memory_order_acquire);
    const float cameraYaw=dg::camera_trace::AppliedYaw();
    if(!std::isfinite(actorYaw)||!std::isfinite(cameraYaw)||
       std::fabs(cameraYaw)>36000.0f)return result;
    // Pad strafe: aim toward the rendered camera heading, retain vanilla
    // player movement vector. Rotate while firing is CAMERA ONLY.
    if(playerStrafe){
        result->pitch=0.0f;
        result->yaw=std::remainder(cameraYaw,360.0f);
        result->roll=0.0f;
        const unsigned n=++g_tpsPadStrafeApplied;

        if(n==1||n==120||n==10000)
            Log("TPS V1.02: controller look rotation -> camera n=%u yaw=%.1f",
                n,result->yaw);
    } else if(nativeBlock) {
        // Right stick must not rotate the PLAYER through the native look
        // getter, even when engine input arrives by another XInput/HID path.
        // Camera movement remains independent and can continue during fire.
        result->pitch=0.0f;
        result->yaw=std::remainder(actorYaw,360.0f);
        result->roll=0.0f;
        const unsigned n=++g_tpsNativeLookRotationBlocks;
        if(n==1||n==120||n==10000)
            Log("TPS V0.75: native player look rotation held n=%u yaw=%.1f",n,result->yaw);
    }
    if(calls==1||calls==10000)
        Log("TPS V0.75: GetPlayerLookRotation calls=%u",calls);
    return result;
}
bool InstallNativeTPSRotationHook() {
    if(!g_targetValidation.exact)return false;
    const uintptr_t base=reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    constexpr unsigned char lookInputSignature[]={
        0x48,0x8B,0xC4,0x53,0x48,0x83,0xEC,0x40
    };
    constexpr unsigned char lookRotationSignature[]={
        0x48,0x89,0x5C,0x24,0x08,0x57,0x48,0x83,0xEC,0x60
    };
    auto* input=reinterpret_cast<unsigned char*>(base+0x671A40);
    auto* rotation=reinterpret_cast<unsigned char*>(base+0x671B00);
    if(std::memcmp(input,lookInputSignature,sizeof(lookInputSignature))||
       std::memcmp(rotation,lookRotationSignature,sizeof(lookRotationSignature))){
        ++g_tpsNativeLookSignatureFailures;
        Log("TPS V0.75: native look signature mismatch: both new hooks DISABLED");
        return false;
    }
    const MH_STATUS init=MH_Initialize();
    if(init!=MH_OK&&init!=MH_ERROR_ALREADY_INITIALIZED)return false;
    const MH_STATUS a=MH_CreateHook(input,reinterpret_cast<void*>(&HookPlayerLookInput),
        reinterpret_cast<void**>(&g_originalGetPlayerLookInput));
    if(a!=MH_OK){Log("TPS V0.75: GetPlayerLookInput CreateHook failed=%d",int(a));return false;}
    const MH_STATUS b=MH_CreateHook(rotation,reinterpret_cast<void*>(&HookPlayerLookRotation),
        reinterpret_cast<void**>(&g_originalGetPlayerLookRotation));
    if(b!=MH_OK){
        MH_RemoveHook(input);
        g_originalGetPlayerLookInput=nullptr;
        Log("TPS V0.75: GetPlayerLookRotation CreateHook failed=%d",int(b));
        return false;
    }
    const MH_STATUS enabledA=MH_EnableHook(input);
    const MH_STATUS enabledB=MH_EnableHook(rotation);
    if(enabledA!=MH_OK&&enabledA!=MH_ERROR_ENABLED ||
       enabledB!=MH_OK&&enabledB!=MH_ERROR_ENABLED){
        MH_DisableHook(input);MH_DisableHook(rotation);
        MH_RemoveHook(input);MH_RemoveHook(rotation);
        g_originalGetPlayerLookInput=nullptr;
        g_originalGetPlayerLookRotation=nullptr;
        Log("TPS V0.75: native look hook enabling FAILED; game left vanilla");
        return false;
    }
    Log("TPS V0.75: native game look input READY RVA=0x671A40 and look rotation READY RVA=0x671B00");
    return true;
}

// V1.45: retire the unproven persistent native ground-FX override.
// The validated line/arc shader filter alone implements visibility scopes.
// Native dash/targeting lifecycle receives all its own hide/show requests.

// V0.87 passive audit: do the native targeting UI components receive
// Unreal events when Strife's blue world-space line/arc are visible?
// All events ALWAYS execute the original game callback, no visual or
// gameplay changes. If counters stay zero the visuals bypass this route.
void HookAimUiEventDispatch(void* object,void* function,void* parameters,void* outParameters){
    if(object && function && g_aimUiClassSlot && g_aimUiPositionClassSlot){
        const void* actual=*reinterpret_cast<void* const*>(
            static_cast<const unsigned char*>(object)+0x10);
        if(actual){
            const bool targeting=actual==*g_aimUiClassSlot;
            const bool positioning=actual==*g_aimUiPositionClassSlot;
            if(targeting||positioning){
                const unsigned n=targeting?++g_aimUiEvents:++g_aimUiPositionEvents;
                auto& rt=dg::runtime::Get();
                const bool tps=rt.thirdPersonEnabled.load(std::memory_order_relaxed);
                const bool fire=rt.tpsAimActive.load(std::memory_order_relaxed) ||
                    (MouseAnyFireHeld());
                if(tps && targeting)++g_aimUiEventsInTPS;
                if(fire && targeting)++g_aimUiEventsWhileFire;
                if(n==1||n==100||n==1000)
                    Log("TPS V0.87 AIM UI PROBE: class=%s events=%u tps=%d firing=%d (PASS THROUGH)",
                        targeting?"UMayhemTargetingUIComponent":
                        "UMayhemTargetUiPositionComponent",n,tps?1:0,fire?1:0);
            }
        }
    }
    if(g_originalAimUiEventDispatch)
        g_originalAimUiEventDispatch(object,function,parameters,outParameters);
    dg::native_tps::ObserveUiEvent(object); // live object, after native action
}
void InstallAimUiEventProbe(){
    if(!g_targetValidation.exact)return;
    auto* base=reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr));
    auto* entry=base+0xAC0630; // exact retail native UObject::ProcessEvent
    constexpr unsigned char sig[]={
        0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,
        0x48,0x81,0xEC,0xF0,0x00,0x00,0x00
    };
    if(std::memcmp(entry,sig,sizeof(sig))!=0){
        Log("TPS V0.87 AIM UI PROBE: ProcessEvent signature mismatch, disabled");
        return;
    }
    g_aimUiClassSlot=reinterpret_cast<void* const*>(base+0x3812DD0);
    g_aimUiPositionClassSlot=reinterpret_cast<void* const*>(base+0x3812DE0);
    const auto made=MH_CreateHook(entry,
        reinterpret_cast<void*>(&HookAimUiEventDispatch),
        reinterpret_cast<void**>(&g_originalAimUiEventDispatch));
    if(made!=MH_OK){
        Log("TPS V0.87 AIM UI PROBE: hook unavailable status=%d",int(made));
        return;
    }
    const auto enabled=MH_EnableHook(entry);
    if(enabled!=MH_OK&&enabled!=MH_ERROR_ENABLED){
        MH_RemoveHook(entry);g_originalAimUiEventDispatch=nullptr;
        Log("TPS V0.87 AIM UI PROBE: enable failed=%d",int(enabled));
        return;
    }
    Log("TPS V0.87 AIM UI PROBE: READY, pass-through classes UMayhemTargetingUIComponent and UMayhemTargetUiPositionComponent");
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
    ApplyPlayerMovementTunings(movementComponent);

    dg::local_players::Context localOwner{};
    if(dg::local_players::ResolveActor(characterOwner,localOwner)){
        dg::local_players::ObserveGameplay(localOwner);
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
        return false; // Legacy action retained; ineffective native experiment retired.
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

    if(action==Action::RefillHealth||action==Action::UnlimitedHealth||
       action==Action::UnlimitedGlide||action==Action::UnlimitedHorseSprint){
        const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
        if(!hwnd||GetForegroundWindow()!=hwnd||IsIconic(hwnd)||g_overlayVisible.load()||g_shuttingDown.load())return;
        if(action==Action::RefillHealth){
            const bool queued=dg::health::RequestRefill();
            g_lastAction=std::string(label)+(queued?" [queued]":" [unavailable]");
        }else{
            bool& enabled=action==Action::UnlimitedHealth?g_config.unlimitedHealthEnabled:
                action==Action::UnlimitedGlide?g_config.unlimitedGlideEnabled:g_config.unlimitedHorseSprintEnabled;
            enabled=!enabled;g_config.Save();g_lastAction=std::string(label)+(enabled?" ON":" OFF");
        }
        Log("Sustain V1.46 HOTKEY F%d action=%s",functionKey,label);return;
    }
    if(action==Action::RefillPotions||action==Action::PotionRecharge||action==Action::UnlimitedPotions){
        const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
        if(!hwnd||GetForegroundWindow()!=hwnd||IsIconic(hwnd)||g_overlayVisible.load()||g_shuttingDown.load())return;
        if(action==Action::RefillPotions){
            const bool queued=dg::potions::RequestRefill();
            g_lastAction=std::string(label)+(queued?" [queued]":" [unavailable]");
            Log("Potion V1.44 HOTKEY F%d refillQueued=%d",functionKey,queued?1:0);
        }else{
            bool& enabled=action==Action::PotionRecharge?g_config.potionRechargeEnabled:g_config.unlimitedPotionsEnabled;
            enabled=!enabled;g_config.Save();
            g_lastAction=std::string(label)+(enabled?" ON":" OFF");
            Log("Potion V1.44 HOTKEY F%d action=%s enabled=%d",functionKey,label,enabled?1:0);
        }
        return;
    }
    if(action==Action::ReduceHotstreak||action==Action::UnlimitedHotstreak||
       action==Action::RefillHotstreak||action==Action::RefillAmmo){
        const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
        if(!hwnd||GetForegroundWindow()!=hwnd||IsIconic(hwnd)||g_overlayVisible.load()||g_shuttingDown.load())return;
        if(action==Action::RefillHotstreak||action==Action::RefillAmmo){
            const bool queued=action==Action::RefillAmmo?dg::ammo::RequestRefill():dg::hotstreak::RequestRefill();
            g_lastAction=std::string(label)+(queued?" [queued]":" [unavailable]");
        }else{
            bool& enabled=action==Action::ReduceHotstreak?g_config.reduceHotstreakEnabled:g_config.unlimitedHotstreakEnabled;
            enabled=!enabled;g_config.Save();g_lastAction=std::string(label)+(enabled?" ON":" OFF");
        }
        Log("Resources V1.48 HOTKEY F%d action=%s",functionKey,label);return;
    }
    if(action==Action::ReduceAmmo||action==Action::UnlimitedAmmo){
        bool& enabled=action==Action::ReduceAmmo?g_config.reduceAmmoEnabled:g_config.unlimitedAmmoEnabled;
        enabled=!enabled;g_config.Save();
        g_lastAction=std::string(label)+(enabled?" ON":" OFF");
        Log("Ammo V1.39 HOTKEY F%d action=%s enabled=%d divisor=%.3f",functionKey,label,enabled?1:0,g_config.ammoConsumptionDivisor);
        return;
    }
    if(const unsigned mask=dg::config::UnlimitedMask(action)){
        const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
          if(!hwnd||GetForegroundWindow()!=hwnd||IsIconic(hwnd)||g_overlayVisible.load()||g_shuttingDown.load())return;
        const bool enable=mask==7?!(g_config.unlimitedWrathEnabled&&
            g_config.unlimitedUltimateEnabled&&g_config.unlimitedSynergyEnabled):
            mask==1?!g_config.unlimitedWrathEnabled:
            mask==2?!g_config.unlimitedUltimateEnabled:!g_config.unlimitedSynergyEnabled;
        if(mask&1)g_config.unlimitedWrathEnabled=enable;
        if(mask&2)g_config.unlimitedUltimateEnabled=enable;
        if(mask&4)g_config.unlimitedSynergyEnabled=enable;
        g_config.Save();
        g_lastAction=std::string(label)+(enable?" ON":" OFF");
        Log("Gauge V1.38 HOTKEY F%d unlimitedMask=%u enabled=%d",functionKey,mask,enable?1:0);
        return;
    }
    if(const unsigned refill=dg::config::GaugeRefillMask(action)){
        const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
        const bool allowed=hwnd&&GetForegroundWindow()==hwnd&&!IsIconic(hwnd)&&
            !g_overlayVisible.load()&&!g_shuttingDown.load();
        const bool queued=allowed&&dg::manual_gauges::RequestSet(refill,100.0f);
        g_lastAction=std::string(label)+(queued?" requested":" unavailable here");
        Log("Gauge V1.36 HOTKEY F%d action=%s mask=%u percent=100 queued=%d focusedGameplay=%d",
            functionKey,label,refill,queued?1:0,allowed?1:0);
        return;
    }

    if (action == Action::ToggleHUD) {
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
        g_lastAction = "Action Recovery [retired: no effect]";
        Log("F%d -> Action Recovery ignored (ineffective experiment retired)", functionKey);
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
        if (action == Action::SkipWarning) accepted=dg::skip_warning::Apply(*toggle);
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
        if(g_config.thirdPersonEnabled)return; // Vanilla camera actions disabled in TPS
        *cameraValue=std::clamp(*cameraValue+cameraStep,cameraMinimum,cameraMaximum);
        g_config.Save();
        g_lastAction=std::string(label)+" = "+std::to_string(*cameraValue);
        Log("F%d -> %s value=%.2f",functionKey,label,*cameraValue);
        return;
    }
    if(action==Action::CameraReset){
        g_config.ResetVanillaCameraValues();
        g_lastAction="Camera values restored";
        Log("F%d -> Camera Values Default",functionKey);
        return;
    }
    if(action==Action::TpsCameraReset){
        g_config.ResetTpsCameraValues();
        g_lastAction="TPS camera values restored";
        Log("F%d -> TPS Camera Values Default",functionKey);
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
        if (action >= static_cast<int>(g_config.cameraKeys.size()) &&
            action < static_cast<int>(g_config.cameraKeys.size()+g_config.tpsKeyboardKeys.size())) {
            const size_t index=static_cast<size_t>(action)-g_config.cameraKeys.size();
            g_config.tpsKeyboardKeys[index]=vk;
            g_config.Save();
            Log("TPS V0.92: keyboard shortcut %s -> %s",
                dg::config::kTpsKeyboardLabels[index],KeyDisplayName(vk).c_str());
        } else if (action >= 0 && action < static_cast<int>(g_config.cameraKeys.size())) {
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
        // Discard refill edges while editing the menu, including held keys
        // across its close. Existing non-refill F-key behavior is preserved.
        for(int i=0;i<12;++i)
            if(dg::config::GaugeRefillMask(g_config.hotkeys[static_cast<size_t>(i)])||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::RefillPotions||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::PotionRecharge||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::UnlimitedPotions||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::RefillHealth||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::UnlimitedHealth||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::UnlimitedGlide||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::UnlimitedHorseSprint||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::ReduceHotstreak||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::UnlimitedHotstreak||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::RefillHotstreak||
               g_config.hotkeys[static_cast<size_t>(i)]==Action::RefillAmmo)
                KeyPressed(VK_F1+i);
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
    if(g_config.thirdPersonEnabled){
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
        !dg::runtime::Get().tpsTemporarilySuspended.load(std::memory_order_relaxed) &&
        !dg::runtime::Get().tpsCutsceneSuspended.load(std::memory_order_relaxed) &&
        !dg::runtime::Get().tpsUiViewHeld.load(std::memory_order_relaxed) &&
        !IsTPSRadialInputActive() &&
        IsTPSLookViewFresh() && (!dg::chaos_pose::ActiveNow()||dg::chaos_pose::RangedActive()) &&
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
// V0.84 grenade hold repair. Previously the game-facing R3 DOWN lasted
// only 65 ms even while physical R3 remained held. A game poll could miss
// the tap, or see an artificial UP while the player was still holding R3.
// Keep the PHYSICAL R3 held normally for the full press. After its release,
// produce one distinct, sufficiently long native-toggle DOWN pulse separated
// by a definite UP interval. All other controller bits stay exactly native.
// This code never changes TPS camera hold timing or native firing.
// Standard XInput buttons plus LT/RT virtual binding IDs. GUIDE button
// is not accessible through the standard XINPUT_GAMEPAD structure.
bool GamepadBindingDown(int mask,const XINPUT_GAMEPAD& pad) {
    if(mask==0)return false;
    if(mask==0x10000)return pad.bLeftTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    if(mask==0x20000)return pad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    return (pad.wButtons&static_cast<WORD>(mask))!=0;
}
// Native LB+Y remains fully game owned. The chord detector is telemetry only.
bool TPSChargeChordDown(const XINPUT_GAMEPAD& pad){
    return (pad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER)!=0 &&
        (pad.wButtons&XINPUT_GAMEPAD_Y)!=0;
}
bool TPSFireDown(const XINPUT_GAMEPAD& pad) {
    return GamepadBindingDown(dg::runtime::Get().tpsFirePrimaryMask.load(std::memory_order_relaxed),pad) ||
        GamepadBindingDown(dg::runtime::Get().tpsFireSecondaryMask.load(std::memory_order_relaxed),pad);
}
void ApplyHoldGrenadeVirtualR3(XINPUT_GAMEPAD& pad,int mask) {
    AcquireSRWLockExclusive(&g_grenadeHoldLock);
    const auto events=dg::tps_input::ApplyEquipmentHold(pad,mask,GetTickCount64(),g_grenadeHoldState);
    if(events.rebound)Log("TPS V0.86 GRENADE binding now 0x%X",mask);
    if(events.down){const unsigned n=++g_tpsGrenadeDownCount;
        if(n<=8)Log("TPS V0.84 GRENADE: physical DOWN n=%u; forward continuous hold",n);}
    if(events.up){const unsigned n=++g_tpsGrenadeUpCount;
        if(n<=8)Log("TPS V0.84 GRENADE: physical UP n=%u; stow pulse queued after gap",n);}
    if(events.stow){const unsigned n=++g_tpsGrenadeReleasePulseDelivered;
        if(n<=8)Log("TPS V0.84 GRENADE: second R3 pulse sent to native XInput n=%u",n);}
    ReleaseSRWLockExclusive(&g_grenadeHoldLock);
}

DWORD FilterNativeXInput(int slot,DWORD user,XINPUT_STATE* state) {
    const auto original=g_nativeXInputOriginal[slot];
    if(!original)return ERROR_DEVICE_NOT_CONNECTED;
    const DWORD realStatus=original(user,state);
    if(realStatus!=ERROR_SUCCESS||!state)return realStatus;
    // P2 filters only its verified native ControllerId, with its own view.
    // The existing primary path below keeps every validated button policy.
    const HWND coopWindow=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    const bool coopFocused=g_targetValidation.exact&&coopWindow&&GetForegroundWindow()==coopWindow&&
        !IsIconic(coopWindow)&&!g_overlayVisible.load()&&!g_shuttingDown.load();
    if(dg::coop_camera::FilterPad(user,state->Gamepad,coopFocused))return realStatus;
    if(user!=static_cast<DWORD>(dg::local_players::PrimaryControllerId()))return realStatus;
    const DWORD status=realStatus;
    if(dg::chaos_pose::ActiveNow()){
        g_tpsFireHeld.store(false,std::memory_order_release);
        auto& rt=dg::runtime::Get();
        rt.tpsAimActive.store(false,std::memory_order_release);
        // Only Strife, proved by fresh native form -> PlayerState -> owner
        // and reciprocal owner.GetChaosForm. War/unrecognized forms stay native.
        // Keep wheel/charge selection and all physical buttons/triggers intact.
        if(!dg::chaos_pose::RangedActive()||!ShouldSuppressNativeAim()||
           !dg::native_tps::NativeStickSwapKnown())return realStatus;
        auto& formPad=state->Gamepad;
        if(GamepadBindingDown(dg::runtime::Get().tpsWheelPadMask.load(std::memory_order_relaxed),formPad)||
           TPSChargeChordDown(formPad))return realStatus;
        const int cameraMode=rt.tpsCameraStickMode.load();
        const bool swap=dg::native_tps::NativeStickSwap();
        if(cameraMode!=0)dg::tps_input::MapGameplayRoles(formPad,cameraMode==1,swap);
        const bool fire=TPSFireDown(formPad);
        g_tpsFireHeld.store(fire,std::memory_order_release);
        if(fire)g_tpsFireLastTick.store(GetTickCount64(),std::memory_order_release);
        rt.tpsAimActive.store(fire,std::memory_order_release);
        if(rt.tpsMountedAimMode.load()==0&&fire){
            dg::tps_input::SetNativeLook(formPad,swap,0,32767);
            static std::atomic_uint32_t shots{0};
            const auto n=++shots;
            if(n==1||n==250)Log("TPS V1.37 ULTIMATE_AIM Strife native camera-forward pad samples=%u",n);
        }else if(cameraMode!=0&&(rt.tpsMountedAimMode.load()==0||!fire)){
            dg::tps_input::SetNativeLook(formPad,swap,0,0);
        }
        return realStatus;
    }
    auto& pad=state->Gamepad;
    // V0.92: user-selected pair must be pressed SIMULTANEOUSLY, never OR.
    // Map its held state to the game's original LB+RB summon/dismount chord.
    // This is active with or without TPS, leaving unrelated inputs native.
    const HWND bindWindow=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    const bool bindActive=g_targetValidation.exact && bindWindow &&
        GetForegroundWindow()==bindWindow && !IsIconic(bindWindow) &&
        !g_overlayVisible.load() && !g_shuttingDown.load();
    const bool validChord=bindActive && dg::runtime::Get().horsePadButtonA.load(std::memory_order_relaxed) &&
        dg::runtime::Get().horsePadButtonB.load(std::memory_order_relaxed) &&
        dg::runtime::Get().horsePadButtonA.load(std::memory_order_relaxed)!=dg::runtime::Get().horsePadButtonB.load(std::memory_order_relaxed) &&
        GamepadBindingDown(dg::runtime::Get().horsePadButtonA.load(std::memory_order_relaxed),pad) &&
        GamepadBindingDown(dg::runtime::Get().horsePadButtonB.load(std::memory_order_relaxed),pad);
    // The XInput hook executes before the game consumes the native chord:
    // arm region permission HERE instead of after Present (which can miss
    // the first native HorseRegion getter call in banned zones).
    static std::atomic_bool previousChord{false};
    const bool chordEdge=validChord&&!previousChord.exchange(validChord);
    if(chordEdge && dg::runtime::Get().horseEverywhereEnabled.load(std::memory_order_relaxed)){
        const auto now=GetTickCount64();
        auto& horseRt=dg::runtime::Get();
        horseRt.horseSummonIntentTick.store(now,std::memory_order_release);
        horseRt.tpsLastHorseSummonInputTick.store(now,std::memory_order_release);
        Log("Horse Everywhere V0.94: native XInput chord pre-armed at GetState, no extra summon command");
    }
    if(validChord){
        const bool alreadyNative=(pad.wButtons&
            (XINPUT_GAMEPAD_LEFT_SHOULDER|XINPUT_GAMEPAD_RIGHT_SHOULDER))==
            (XINPUT_GAMEPAD_LEFT_SHOULDER|XINPUT_GAMEPAD_RIGHT_SHOULDER);
        pad.wButtons|=XINPUT_GAMEPAD_LEFT_SHOULDER|XINPUT_GAMEPAD_RIGHT_SHOULDER;
        if(!alreadyNative)++g_horsePadChordRemapped;
    }
    // This is intentionally BEFORE the temporary R3 vanilla-view gate.
    // R3 itself is a grenade key: its physical hold is handled separately
    // by UpdateOrbitInput, while the game receives two native toggle edges.
    const HWND localHwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    const bool localGameplay=dg::runtime::Get().thirdPersonEnabled.load(std::memory_order_relaxed) &&
        localHwnd && GetForegroundWindow()==localHwnd &&
        !g_overlayVisible.load() && !IsIconic(localHwnd) &&
        !g_shuttingDown.load();
    // V1.08: LB alone opens the original wheel. LB+Y preserves the native
    // charged-shot chord, never moves the game-facing wheel stick.
    const bool wheelDown=localGameplay&&GamepadBindingDown(dg::runtime::Get().tpsWheelPadMask.load(std::memory_order_relaxed),pad);
    const bool chargedChord=localGameplay&&!validChord&&TPSChargeChordDown(pad);
    const auto chordNow=GetTickCount64();
    // V1.09: record the actual ORIGINAL pad state before any aim filter,
    // including R1/R2 compared to LB+Y and plain LB. Input unmodified.
    g_tpsChargeChordHeld.store(chargedChord,std::memory_order_release);
    if(chargedChord){
        g_tpsChargeChordTick.store(chordNow,std::memory_order_release);
        const unsigned n=++g_tpsChargeChordSamples;
        if(n==1)
            Log("TPS V1.11 LB+Y passes through native input (0 special writes)");
    }
    if(wheelDown){
        auto& wheelRt=dg::runtime::Get();
        wheelRt.tpsAbilityWheelHeld.store(true,std::memory_order_release);
        wheelRt.tpsAbilityWheelTick.store(chordNow,std::memory_order_release);
        return status; // retained configurable selection guard, original axes
    }
    // R3 is equipment USE; never treat it as opening a radial wheel.
    // The original V0.84 continuous physical hold/stow pulse stays unchanged.
    const int grenadeMask=dg::runtime::Get().tpsGrenadeButtonMask.load(std::memory_order_relaxed);
    if(localGameplay&&grenadeMask!=0)
        ApplyHoldGrenadeVirtualR3(pad,grenadeMask);
    if(!ShouldSuppressNativeAim())
        return status; // Preserve vanilla aim while temporary view is held.
    auto& rt=dg::runtime::Get();
    const int cameraMode=rt.tpsCameraStickMode.load(std::memory_order_relaxed);
    // If the exact native layout is not yet known, fail open. Never guess
    // which physical stick the in-game swap checkbox routes to native aim.
    if(!dg::native_tps::NativeStickSwapKnown())return status;
    const bool nativeSwap=dg::native_tps::NativeStickSwap();
    if(cameraMode!=0)
        dg::tps_input::MapGameplayRoles(pad,cameraMode==1,nativeSwap);
    // V1.04: camera joystick OFF must not abort gameplay aiming/forward injection.
    const bool fire=TPSFireDown(pad);
    g_tpsFireHeld.store(fire);
    if(fire) g_tpsFireLastTick.store(GetTickCount64());
    rt.tpsAimActive.store(fire);

    // V1.10: ONLY R1/R2 may receive synthetic stick-UP aim.
    // LB+Y exits unmodified by native LB-wheel passthrough above.
    const bool mountedForForward=IsMountedTPSNow();
    if(rt.tpsMountedAimMode.load()==0&&fire){
        dg::tps_input::SetNativeLook(pad,nativeSwap,0,32767);
        const unsigned n=++g_tpsForwardAnalogInjected;
        const unsigned scoped=mountedForForward?
            ++g_tpsMountedForwardAnalogInjected:++g_tpsFootForwardAnalogInjected;
        if(scoped==1||scoped==250||scoped==1000)
            Log("TPS V1.04 FORWARD: native right-stick UP injected scope=%s n=%u total=%u stickMode=%d",
                mountedForForward?"HORSE":"FOOT",scoped,n,rt.tpsCameraStickMode.load());
        return status;
    }
    if(rt.tpsCameraStickMode.load(std::memory_order_relaxed)==0)
        return status; // OFF permits idle native look, but retains forward fire.
    // V0.94: normal native right-stick isolation is a TPS invariant, not
    // a separate user option. Camera Forward injection above or Vanilla Aim
    // while firing are the only intentional exceptions.
    // V0.68: NEVER inject artificial XInput aim during RT/RB.
    // The same right-stick suppression applies while running, idle AND
    // firing. Our own camera separately reads the UNFILTERED trampoline.
    // This is input isolation, NOT yet a shot-direction fix.
    const bool vanillaAim=rt.tpsMountedAimMode.load()==1;
    const bool blockLook=!vanillaAim || !fire;
    if(blockLook && (dg::tps_input::CameraX(pad,nativeSwap)!=0 ||
                     dg::tps_input::CameraY(pad,nativeSwap)!=0)){
        dg::tps_input::SetNativeLook(pad,nativeSwap,0,0);
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
    // V1.02 controller-only TPS: all native mouse messages pass through.
    // No RAWINPUT reads, WM_MOUSEMOVE filter, mouse-wheel camera or fallback.
    // Never intercept focus/activation messages needed for Alt-Tab.
    const bool focusMessage = msg == WM_ACTIVATEAPP || msg == WM_ACTIVATE ||
        msg == WM_SETFOCUS || msg == WM_KILLFOCUS || msg == WM_MOUSEACTIVATE;
    if (focusMessage) {
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
    // V1.18: fresh source-only TPS mouse audit. We do not read WM_INPUT
    // payloads, suppress any message, alter camera input or reuse V0.98 orbit.
    // WM_INPUT may include NON-MOUSE devices and is counted as such.
    if((msg==WM_MOUSEMOVE || msg==WM_INPUT || msg==WM_LBUTTONDOWN ||
        msg==WM_RBUTTONDOWN || msg==WM_MOUSEWHEEL) &&
       !g_overlayVisible.load(std::memory_order_relaxed) &&
       GetForegroundWindow()==hwnd && !IsIconic(hwnd)){
        const auto& rt=dg::runtime::Get();
        const ULONGLONG now=GetTickCount64();
        const ULONGLONG view=rt.tpsGameplayViewTick.load(std::memory_order_acquire);
        const bool liveTps=rt.thirdPersonEnabled.load(std::memory_order_relaxed) &&
            rt.tpsViewGameplay.load(std::memory_order_relaxed) &&
            !rt.tpsCutsceneSuspended.load(std::memory_order_relaxed) &&
            !rt.tpsTemporarilySuspended.load(std::memory_order_relaxed) &&
            !rt.tpsAbilityWheelHeld.load(std::memory_order_relaxed) &&
            view && now>=view && now-view<250ull;
        if(liveTps){
            // V1.29: correlate the REAL physical reticle position with
            // erroneous native isometric shots, especially below horizon.
            // This is READ ONLY and does not change WM messages, targets,
            // WndProc behavior, camera deltas, game raycast or reticle.
            if(msg==WM_LBUTTONDOWN||msg==WM_RBUTTONDOWN){
                static std::atomic_uint32_t shotSamples{0};
                const unsigned shot=++shotSamples;
                if(shot<=24u){
                    POINT cursor{};
                    RECT rect{};
                    if(GetCursorPos(&cursor)&&ScreenToClient(hwnd,&cursor)&&
                       GetClientRect(hwnd,&rect)){
                        const float w=static_cast<float>(rect.right-rect.left);
                        const float h=static_cast<float>(rect.bottom-rect.top);
                        if(w>0.0f&&h>0.0f){
                            const auto cam=dg::camera_trace::GetTelemetry();
                            const float nx=2.0f*static_cast<float>(cursor.x)/w-1.0f;
                            const float ny=2.0f*static_cast<float>(cursor.y)/h-1.0f;
                            Log("TPS V1.29 CURSOR_AIM_AUDIT shot=%u button=%s relativeCursor=(%.3f,%.3f) belowMidline=%d mounted=%d camYaw=%.1f camPitch=%.1f camFOV=%.1f gameRaycastUnmodified=1",
                                shot,msg==WM_LBUTTONDOWN?"LMB":"RMB",nx,ny,ny>0.0f?1:0,
                                IsMountedTPSNow()?1:0,cam.appliedYaw,
                                cam.appliedPitch,cam.appliedFov);
                            // The cursor screen-plane fraction is NOT an
                            // actual physical horizon classification.
                            // V1.30 first step toward true aim: derive a
                            // normalized WORLD ray through that viewport
                            // pixel using the emitted TPS camera pose.
                            // This is pure math; no collision trace and
                            // absolutely NO changes to weapon projectiles.
                            constexpr float rad=0.01745329251994329577f;
                            const bool recentCam=cam.appliedWorldTickMs &&
                                now>=cam.appliedWorldTickMs &&
                                now-cam.appliedWorldTickMs<250ull;
                            const float aspect=w/h;
                            if(recentCam&&std::isfinite(cam.appliedFov)&&
                               cam.appliedFov>=40.0f&&cam.appliedFov<=140.0f&&
                               std::isfinite(cam.appliedYaw)&&
                               std::isfinite(cam.appliedPitch)&&
                               std::isfinite(aspect)&&aspect>0.1f){
                                const float az=cam.appliedYaw*rad;
                                const float el=cam.appliedPitch*rad;
                                const float cy=std::cos(az),sy=std::sin(az);
                                const float cp=std::cos(el),sp=std::sin(el);
                                const float hTan=std::tan(cam.appliedFov*rad*0.5f);
                                const float vTan=hTan/aspect;
                                // Unreal forward/right/up basis.
                                const float dx=cp*cy+nx*hTan*(-sy)+
                                    ny*vTan*(sp*cy);
                                const float dy=cp*sy+nx*hTan*cy+
                                    ny*vTan*(sp*sy);
                                const float dz=sp-ny*vTan*cp;
                                const float norm=std::sqrt(dx*dx+dy*dy+dz*dz);
                                if(std::isfinite(norm)&&norm>0.001f){
                                    const float ux=dx/norm,uy=dy/norm,uz=dz/norm;
                                    const float rayYaw=std::atan2(uy,ux)/rad;
                                    const float rayPitch=std::asin(std::clamp(uz,-1.0f,1.0f))/rad;
                                    const float forwardDot=ux*cp*cy+uy*cp*sy+uz*sp;
                                    Log("TPS V1.30 RETICLE_WORLD_RAY shot=%u button=%s mounted=%d cursor=(%.3f,%.3f) aspect=%.3f worldCam=(%.1f,%.1f,%.1f) direction=(%.4f,%.4f,%.4f) azimuth=%.1f elevation=%.1f yawOffset=%.1f camForwardDot=%.4f hasNativeHit=0 bulletsUnchanged=1",
                                        shot,msg==WM_LBUTTONDOWN?"LMB":"RMB",
                                        IsMountedTPSNow()?1:0,nx,ny,aspect,
                                        cam.appliedWorldX,cam.appliedWorldY,cam.appliedWorldZ,
                                        ux,uy,uz,rayYaw,rayPitch,
                                        std::remainder(rayYaw-cam.appliedYaw,360.0f),
                                        forwardDot);
                                }
                            }
                        }
                    }
                }
            }
            unsigned n=0;
            const char* kind=nullptr;
            switch(msg){
            case WM_MOUSEMOVE: n=++g_v118MouseMove;kind="WM_MOUSEMOVE";break;
            case WM_INPUT: n=++g_v118RawInputMessages;kind="WM_INPUT_UNDECODED";break;
            case WM_LBUTTONDOWN: n=++g_v118MouseLmb;kind="LMB_DOWN";break;
            case WM_RBUTTONDOWN: n=++g_v118MouseRmb;kind="RMB_DOWN";break;
            case WM_MOUSEWHEEL: n=++g_v118MouseWheel;kind="WHEEL";break;
            default:break;
            }
            if(kind && (n<=4 || (n%5000)==0))
                Log("TPS V1.18 MOUSE_READONLY kind=%s count=%u tps=1 wheelHeld=0 overlay=0 (native input unchanged)",
                    kind,n);
        }
    }
    // V1.19 fresh mouse TPS path. Read only, never swallow WM_INPUT and
    // never change cursor position or any physical/controller input event.
    // Mouse and pad both feed the EXISTING TPS camera orbit, so there is one
    // camera-forward shooting direction instead of competing mouse aim modes.
    if (msg==WM_INPUT || msg==WM_MOUSEMOVE) {
        static POINT lastPoint{};
        static bool lastPointValid=false;
        const auto& rt=dg::runtime::Get();
        const ULONGLONG now=GetTickCount64();
        const ULONGLONG view=rt.tpsGameplayViewTick.load(std::memory_order_acquire);
        const bool mouseTpsActive=rt.thirdPersonEnabled.load(std::memory_order_relaxed) &&
            rt.tpsViewGameplay.load(std::memory_order_relaxed) &&
            !rt.tpsCutsceneSuspended.load(std::memory_order_relaxed) &&
            !rt.tpsTemporarilySuspended.load(std::memory_order_relaxed) &&
            !rt.tpsAbilityWheelHeld.load(std::memory_order_relaxed) &&
            !g_overlayVisible.load(std::memory_order_relaxed) &&
            GetForegroundWindow()==hwnd && !IsIconic(hwnd) &&
            view && now>=view && now-view<250ull;
        if (!mouseTpsActive) {
            lastPointValid=false;
            g_v119MouseDeltaX.exchange(0);
            g_v119MouseDeltaY.exchange(0);
        } else {
            int dx=0,dy=0;
            if(msg==WM_INPUT) {
                RAWINPUT raw{};
                UINT bytes=sizeof(raw);
                const UINT copied=GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,&raw,&bytes,sizeof(RAWINPUTHEADER));
                if(copied!=UINT(-1) && copied>=sizeof(RAWINPUTHEADER)) {
                    if(raw.header.dwType==RIM_TYPEMOUSE &&
                       (raw.data.mouse.usFlags&MOUSE_MOVE_ABSOLUTE)==0) {
                        dx=static_cast<int>(raw.data.mouse.lLastX);
                        dy=static_cast<int>(raw.data.mouse.lLastY);
                        g_v119LastRawMouseTick.store(now,std::memory_order_relaxed);
                        ++g_v119RawMouse;
                        ++g_v120RawMousePackets;
                        RECT area{};
                        POINT point{};
                        if(GetClientRect(hwnd,&area)&&GetCursorPos(&point)&&ScreenToClient(hwnd,&point)&&
                           (point.x<=area.left+3||point.x>=area.right-4||
                            point.y<=area.top+3||point.y>=area.bottom-4))
                            ++g_v120RawAtEdge;
                    }else if(raw.header.dwType!=RIM_TYPEMOUSE)++g_v120RawNonMousePackets;
                }
            }else {
                const POINT next{
                    static_cast<LONG>(static_cast<short>(LOWORD(lParam))),
                    static_cast<LONG>(static_cast<short>(HIWORD(lParam)))};
                const ULONGLONG warpAt=g_v123LastRecenterTick.load(
                    std::memory_order_acquire);
                const bool artificialMove=warpAt && now>=warpAt &&
                    now-warpAt<250ull &&
                    std::abs(next.x-g_v123RecenterClientX.load(
                        std::memory_order_relaxed))<=3 &&
                    std::abs(next.y-g_v123RecenterClientY.load(
                        std::memory_order_relaxed))<=3;
                if(artificialMove) {
                    // A SetCursorPos message must never rotate the camera.
                    ++g_v123SyntheticMovesIgnored;
                }else if(lastPointValid &&
                         now-g_v119LastRawMouseTick.load(
                             std::memory_order_relaxed)>300ull) {
                    dx=static_cast<int>(next.x-lastPoint.x);
                    dy=static_cast<int>(next.y-lastPoint.y);
                    if(dx||dy)++g_v119MouseMoveFallback;
                }
                lastPoint=next;
                lastPointValid=true;
            }
            // Bounded deltas reject teleports, very old positions and invalid
            // raw packets. The original WndProc still gets EVERY message.
            if(std::abs(dx)<=160 && std::abs(dy)<=160) {
                if(dx||dy)g_v123LastPhysicalMouseTick.store(
                    now,std::memory_order_release);
                if(dx)g_v119MouseDeltaX.fetch_add(dx,std::memory_order_relaxed);
                if(dy)g_v119MouseDeltaY.fetch_add(dy,std::memory_order_relaxed);
            }
        }
    }
    // Keyboard summon H must authorize its native region checks BEFORE
    // dispatch to the game's WndProc. Present-based polling is a frame late
    // for the first keypress in banned zones. No synthetic second H needed.
    if(msg==WM_KEYDOWN && (wParam&0xFF)=='H' &&
       (lParam&(1LL<<30))==0 &&
       g_targetValidation.exact && g_config.horseEverywhereEnabled &&
       !g_overlayVisible.load() && GetForegroundWindow()==hwnd &&
       !IsIconic(hwnd) && !g_shuttingDown.load()){
        const auto now=GetTickCount64();
        auto& horseRt=dg::runtime::Get();
        horseRt.horseSummonIntentTick.store(now,std::memory_order_release);
        horseRt.tpsLastHorseSummonInputTick.store(now,std::memory_order_release);
        static unsigned earlyH=0;
        if(++earlyH<=8)
            Log("Horse Everywhere V0.94: native WM_KEYDOWN H pre-armed before original WndProc #%u",earlyH);
    }
    if (g_imguiReady.load() && g_overlayVisible.load()) {
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
        const int cameraCapture = g_captureCameraKeyIndex.load();
        if (cameraCapture >= 0 &&
            cameraCapture < static_cast<int>(g_config.cameraKeys.size()+g_config.tpsKeyboardKeys.size()) &&
            (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)) {
            const int vk = static_cast<int>(wParam & 0xFF);
            if (vk == VK_ESCAPE) {
                g_captureCameraKeyIndex.store(-1);
                g_lastAction = "Keyboard binding cancelled";
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

    // TPS-only Win32 fire translation before Unreal native WndProc:
    // no synthetic pad input, no cursor visibility hook, no game menu changes.
    if(ShouldSuppressNativeAim() && IsTPSLookViewFresh() &&
       !IsTPSRadialInputActive()){
        int physical=0;
        bool release=false, doubleClick=false;
        switch(msg){
        case WM_LBUTTONDOWN: physical=VK_LBUTTON;break;
        case WM_LBUTTONUP: physical=VK_LBUTTON;release=true;break;
        case WM_LBUTTONDBLCLK: physical=VK_LBUTTON;doubleClick=true;break;
        case WM_RBUTTONDOWN: physical=VK_RBUTTON;break;
        case WM_RBUTTONUP: physical=VK_RBUTTON;release=true;break;
        case WM_RBUTTONDBLCLK: physical=VK_RBUTTON;doubleClick=true;break;
        case WM_MBUTTONDOWN: physical=VK_MBUTTON;break;
        case WM_MBUTTONUP: physical=VK_MBUTTON;release=true;break;
        case WM_MBUTTONDBLCLK: physical=VK_MBUTTON;doubleClick=true;break;
        case WM_XBUTTONDOWN: physical=HIWORD(wParam)==XBUTTON1?VK_XBUTTON1:VK_XBUTTON2;break;
        case WM_XBUTTONUP: physical=HIWORD(wParam)==XBUTTON1?VK_XBUTTON1:VK_XBUTTON2;release=true;break;
        case WM_XBUTTONDBLCLK: physical=HIWORD(wParam)==XBUTTON1?VK_XBUTTON1:VK_XBUTTON2;doubleClick=true;break;
        default:break;
        }
        if(physical){
            const int target=physical==dg::runtime::Get().tpsMousePrimaryButton.load(std::memory_order_relaxed)
                ?VK_LBUTTON:physical==dg::runtime::Get().tpsMouseSecondaryButton.load(std::memory_order_relaxed)
                ?VK_RBUTTON:0;
            // Unassigned physical L/R clicks cannot activate native shots.
            // Unassigned middle/X retain their original non-fire behavior.
            if(!target && (physical==VK_LBUTTON||physical==VK_RBUTTON)){
                ++g_v124NativeButtonSuppressed;
                return 0;
            }
            if(target && target!=physical){
                const UINT routed=target==VK_LBUTTON
                    ?(release?WM_LBUTTONUP:doubleClick?WM_LBUTTONDBLCLK:WM_LBUTTONDOWN)
                    :(release?WM_RBUTTONUP:doubleClick?WM_RBUTTONDBLCLK:WM_RBUTTONDOWN);
                WORD modifiers=static_cast<WORD>(LOWORD(wParam)&
                    ~(MK_LBUTTON|MK_RBUTTON|MK_MBUTTON|MK_XBUTTON1|MK_XBUTTON2));
                if(MousePrimaryFireHeld()&&!release)modifiers|=MK_LBUTTON;
                if(MouseSecondaryFireHeld()&&!release)modifiers|=MK_RBUTTON;
                msg=routed;
                wParam=MAKEWPARAM(modifiers,0);
                if(target==VK_LBUTTON)++g_v124PrimaryRouted;
                else ++g_v124SecondaryRouted;
            }
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
    t.movementHookReady = g_movementHookReady.load();
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
    const auto warnings=dg::skip_warning::GetTelemetry();
    t.skipWarningReady=warnings.ready;
    t.skipWarningAttempted=warnings.observed>0;
    t.skipWarningApplied=warnings.advanced>0;

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

    t.aimUiTargetingEvents = g_aimUiEvents.load();
    t.aimUiPositionEvents = g_aimUiPositionEvents.load();
    t.mountedIdleLookBlocks = g_tpsMountedIdleNativeLookBlocks.load();
    t.grenadePhysicalDown = g_tpsGrenadeDownCount.load();
    t.grenadePhysicalUp = g_tpsGrenadeUpCount.load();
    t.grenadeNativeReleasePulses = g_tpsGrenadeReleasePulseDelivered.load();
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
    context.applySkipWarning = &dg::skip_warning::Apply;
    context.applySkipLogos =
        &dg::skip_logos::Apply;
    context.applyGraphicsAdapter =
        &dg::engine_ini::ApplyGraphicsAdapter;
    context.log = &Log;

    return context;
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

// V1.20: register *missing* relative raw mouse only; never override a
// registration installed by the game, suppress WM_MOUSEMOVE or warp cursor.
// V1.25: GetRegisteredRawInputDevices(nullptr) normally returns -1 with
// ERROR_INSUFFICIENT_BUFFER and count>0. Treat that as enumeration, NOT failure.
// Win32 only allows one mouse target per PROCESS, so never steal an existing
// native registration pointing elsewhere. We only re-arm an ABSENT registration.
void EnsureRelativeMouse(HWND hwnd) {
    if(!hwnd || !IsWindow(hwnd) || !g_targetValidation.exact ||
       !dg::runtime::Get().thirdPersonEnabled.load(std::memory_order_relaxed))
        return;
    const ULONGLONG now=GetTickCount64();
    const bool first=!g_v120RawRegistrationChecked.exchange(true,std::memory_order_acq_rel);
    const ULONGLONG last=g_v125LastPollTick.load(std::memory_order_acquire);
    if(!first && last && now>=last && now-last<1300ull)return;
    g_v125LastPollTick.store(now,std::memory_order_release);
    ++g_v125RawChecks;
    UINT count=0;
    SetLastError(0);
    const UINT query=GetRegisteredRawInputDevices(nullptr,&count,sizeof(RAWINPUTDEVICE));
    const DWORD queryError=GetLastError();
    if(query==UINT(-1) && queryError!=ERROR_INSUFFICIENT_BUFFER){
        ++g_v120RawRegistrationErrors;
        Log("TPS V1.25 RAW_OWNER query error=%lu count=%u (fail-open)",queryError,count);
        return;
    }
    if(count>64){
        ++g_v120RawRegistrationErrors;
        Log("TPS V1.25 RAW_OWNER count=%u too large; no registration changed",count);
        return;
    }
    bool mousePresent=false;
    HWND rawTarget=nullptr;
    DWORD rawFlags=0;
    if(count){
        std::vector<RAWINPUTDEVICE> devices(count);
        UINT capacity=count;
        const UINT got=GetRegisteredRawInputDevices(devices.data(),&capacity,sizeof(RAWINPUTDEVICE));
        if(got==UINT(-1)){
            ++g_v120RawRegistrationErrors;
            Log("TPS V1.25 RAW_OWNER enumerate error=%lu (fail-open)",GetLastError());
            return;
        }
        for(UINT i=0;i<got;++i){
            if(devices[i].usUsagePage==1 && devices[i].usUsage==2){
                mousePresent=true;
                rawTarget=devices[i].hwndTarget;
                rawFlags=devices[i].dwFlags;
                break;
            }
        }
    }
    const int owner=mousePresent?(rawTarget==hwnd?1:2):0;
    const int previous=g_v125RawOwnerState.exchange(owner);
    if(previous!=owner){
        Log("TPS V1.25 RAW_OWNER transition %d -> %d target=%p flags=0x%lX hwnd=%p count=%u",
            previous,owner,rawTarget,rawFlags,hwnd,count);
    }
    if(owner==2){
        const unsigned n=++g_v125RawForeignOwners;
        if(n<=3)
            Log("TPS V1.25 RAW_OWNER native mouse target preserved; no registration override n=%u",n);
        return;
    }
    const bool foreground=GetForegroundWindow()==hwnd && !IsIconic(hwnd) &&
        !g_overlayVisible.load(std::memory_order_relaxed);
    auto& rt=dg::runtime::Get();
    const bool gameplay=rt.tpsViewGameplay.load(std::memory_order_acquire) &&
        !rt.tpsCutsceneSuspended.load(std::memory_order_relaxed) &&
        !rt.tpsTemporarilySuspended.load(std::memory_order_relaxed);
    const unsigned rawCount=g_v120RawMousePackets.load(std::memory_order_relaxed);
    const unsigned moveCount=g_v118MouseMove.load(std::memory_order_relaxed);
    const unsigned oldRaw=g_v125LastProbeRawCount.exchange(rawCount);
    const unsigned oldMove=g_v125LastProbeMoveCount.exchange(moveCount);
    if(owner==1 && foreground && gameplay && moveCount>oldMove+20 && rawCount==oldRaw){
        const unsigned n=++g_v125RawStreamStalls;
        if(n<=8 || n%60==0)
            Log("TPS V1.25 RAW_STALL n=%u regTarget=%p flags=0x%lX WM_MOUSEMOVE_delta=%u WM_INPUT_delta=0 (registration still present; no unsafe override)",
                n,rawTarget,rawFlags,moveCount-oldMove);
    }
    if(owner!=0)return;
    ++g_v125RawMissing;
    // Initial registration is safe before first gameplay frame. Later only
    // repair in active focused TPS; game menus/cutscenes retain native behavior.
    if(!first && (!foreground || !gameplay))return;
    // V1.26: repeated native unregistration in the user log (8/8
    // successful and the session then ended with mouse owner=0).
    // A per-session cap strands long play sessions. Use a 2-second
    // minimum gap instead. Never override an existing native owner.
    constexpr ULONGLONG kMinRearmIntervalMs=2000ull;
    const ULONGLONG prevAttempt=g_v126LastRearmAttemptTick.load(
        std::memory_order_acquire);
    if(!first && prevAttempt && now>=prevAttempt &&
       now-prevAttempt<kMinRearmIntervalMs){
        ++g_v126RearmThrottled;
        return;
    }
    g_v126LastRearmAttemptTick.store(now,std::memory_order_release);
    const unsigned attempt=++g_v125RawRearmAttempts;
    RAWINPUTDEVICE mouse{};
    mouse.usUsagePage=1;
    mouse.usUsage=2;
    mouse.dwFlags=0; // preserve legacy WM_MOUSEMOVE and physical firing.
    mouse.hwndTarget=hwnd;
    if(RegisterRawInputDevices(&mouse,1,sizeof(mouse))){
        g_v120RawAddedByMod.store(true,std::memory_order_relaxed);
        ++g_v125RawRearmSuccess;
        if(attempt>8u)++g_v126RecoveredAfterEight;
        g_v125RawOwnerState.store(1,std::memory_order_release);
        if(attempt<=9u || attempt%25u==0u)
            Log("TPS V1.26 RAW_REARM success attempt=%u stage=%s hwnd=%p afterV125Ceiling=%d minIntervalMs=%llu",
                attempt,first?"STARTUP":"GAMEPLAY",hwnd,attempt>8u?1:0,
                static_cast<unsigned long long>(kMinRearmIntervalMs));
    }else{
        ++g_v120RawRegistrationErrors;
        if(attempt<=9u || attempt%25u==0u)
            Log("TPS V1.26 RAW_REARM fail attempt=%u error=%lu",
                attempt,GetLastError());
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
        Log("V1.03: game WndProc passthrough + ImGui/reticle input ready");
    } else if (GetLastError()!=0) {
        Log("Aim V0.57: native mouse input gate failed error=%lu",GetLastError());
    }
}
// V0.88: retired ineffective Ctrl WM_KEYDOWN bridge. Posting keyboard
// messages did not change native raycast/shots according to user V0.87.

// Native H / E input routing is never installed unless a shortcut is set.
bool EmitKeyboardVirtualKey(WORD vk,bool down){
    INPUT event{};
    event.type=INPUT_KEYBOARD;
    event.ki.wVk=vk;
    event.ki.dwFlags=down?0:KEYEVENTF_KEYUP;
    event.ki.dwExtraInfo=0x44475392;
    return SendInput(1,&event,sizeof(event))==1;
}
std::atomic_bool g_horseSyntheticHDown{false};
void UpdateOrbitInput() {
    static WORD lastButtons=0;
    auto& rt=dg::runtime::Get();
    const HWND hwnd=reinterpret_cast<HWND>(g_gameWindowTrace.load());
    const ULONGLONG now=GetTickCount64();
    const bool focused=hwnd && hwnd==GetForegroundWindow() &&
        !IsIconic(hwnd) && !g_overlayVisible.load();
    // Always sample unmodified physical buttons, including when TPS is OFF,
    // so a LONG L3 press can enable TPS without a keyboard/F5 toggle.
    XINPUT_STATE physical{};
    const bool padAvailable=focused &&
        GetCameraXInputState(dg::local_players::PrimaryControllerId(),&physical)==ERROR_SUCCESS;

    // Only LB / G / Tab open radial wheels; R3/E USE equipment.
    const auto wheelKey=[&](dg::config::TpsKeyboardAction action)->bool{
        const int vk=g_config.tpsKeyboardKeys[static_cast<size_t>(action)];
        return vk>0&&vk<256&&(GetAsyncKeyState(vk)&0x8000)!=0;
    };
    const bool chargedPhysical=padAvailable&&g_config.thirdPersonEnabled&&
        TPSChargeChordDown(physical.Gamepad);
    const bool wheelOpen=focused && (
        (padAvailable&&GamepadBindingDown(dg::runtime::Get().tpsWheelPadMask.load(std::memory_order_relaxed),physical.Gamepad)) ||
        wheelKey(dg::config::TpsKeyboardAction::WheelG) ||
        wheelKey(dg::config::TpsKeyboardAction::WheelTab));
    g_tpsChargeChordHeld.store(focused&&chargedPhysical,std::memory_order_release);
    if(focused&&chargedPhysical)g_tpsChargeChordTick.store(now,std::memory_order_release);
    rt.tpsAbilityWheelHeld.store(wheelOpen,std::memory_order_release);
    if(wheelOpen)rt.tpsAbilityWheelTick.store(now,std::memory_order_release);

    // V0.86: native horse summon INPUT evidence only. H and LB+RB are
    // validated user-facing controls. Never inject an artificial summon;
    // simply correlate each real attempt with the existing server hook.
    const auto KeyboardHeld=[&](dg::config::TpsKeyboardAction action)->bool{
        const int vk=g_config.tpsKeyboardKeys[static_cast<size_t>(action)];
        return focused && vk>0 && vk<256 && (GetAsyncKeyState(vk)&0x8000)!=0;
    };
    // Real local input still goes through game's own original H/LB+RB.
    const int selectedHorseVK=g_config.tpsKeyboardKeys[static_cast<size_t>(
        dg::config::TpsKeyboardAction::Horse)];
    const bool customHorseDown=selectedHorseVK!='H' &&
        KeyboardHeld(dg::config::TpsKeyboardAction::Horse);
    if(customHorseDown!=g_horseSyntheticHDown.load() &&
       EmitKeyboardVirtualKey('H',customHorseDown)){
        g_horseSyntheticHDown.store(customHorseDown);
        if(customHorseDown)++g_keyboardHorseInjected;
    }
    // V0.94: no speculative 1.8-second extra H press. Native Windows and
    // XInput events now pre-arm the banned-region gate BEFORE game dispatch.
    const bool hPressed=focused &&
        (((GetAsyncKeyState('H')&0x8000)!=0) ||
         KeyboardHeld(dg::config::TpsKeyboardAction::Horse));
    const bool shoulderChord=padAvailable &&
        dg::runtime::Get().horsePadButtonA.load(std::memory_order_relaxed) && dg::runtime::Get().horsePadButtonB.load(std::memory_order_relaxed) &&
        dg::runtime::Get().horsePadButtonA.load(std::memory_order_relaxed)!=dg::runtime::Get().horsePadButtonB.load(std::memory_order_relaxed) &&
        GamepadBindingDown(dg::runtime::Get().horsePadButtonA.load(std::memory_order_relaxed),physical.Gamepad) &&
        GamepadBindingDown(dg::runtime::Get().horsePadButtonB.load(std::memory_order_relaxed),physical.Gamepad);
    static bool wasHorseH=false,wasHorseChord=false;
    if((hPressed&&!wasHorseH)||(shoulderChord&&!wasHorseChord)){
        rt.tpsLastHorseSummonInputTick.store(now,std::memory_order_release);
        rt.horseSummonIntentTick.store(now,std::memory_order_release);
    }
    wasHorseH=hPressed;wasHorseChord=shoulderChord;
    // Keyboard equivalents of controller L3 long/short and R3 temporary
    // view. Controller behavior below is retained byte-for-byte.
    const int kbToggleVK=g_config.tpsKeyboardKeys[static_cast<size_t>(
        dg::config::TpsKeyboardAction::Toggle)];
    const int kbRecenterVK=g_config.tpsKeyboardKeys[static_cast<size_t>(
        dg::config::TpsKeyboardAction::Recenter)];
    const bool kbToggle=KeyboardHeld(dg::config::TpsKeyboardAction::Toggle);
    static bool kbTogglePrev=false,kbConsumed=false,kbRecenterPrev=false;
    static ULONGLONG kbToggleStart=0;
    if(kbToggle&&!kbTogglePrev){kbToggleStart=now;kbConsumed=false;}
    if(kbToggle&&!kbConsumed&&kbToggleStart&&now>=kbToggleStart&&
       now-kbToggleStart>=static_cast<ULONGLONG>(g_config.tpsToggleHoldMs)){
        kbConsumed=true;
        g_config.thirdPersonEnabled=!g_config.thirdPersonEnabled;
        g_config.Save();
        ++g_tpsHoldToggleCount;
        Log("TPS V0.92: keyboard long-press toggled TPS %s",
            g_config.thirdPersonEnabled?"ON":"OFF");
    }
    if(!kbToggle&&kbTogglePrev){
        if(!kbConsumed&&kbRecenterVK&&kbRecenterVK==kbToggleVK&&
           g_config.thirdPersonEnabled)
            dg::camera_trace::RecenterOnPlayer();
        kbToggleStart=0;kbConsumed=false;
    }
    kbTogglePrev=kbToggle;
    const bool kbRecenter=KeyboardHeld(dg::config::TpsKeyboardAction::Recenter);
    if(kbRecenterVK&&kbRecenterVK!=kbToggleVK&&kbRecenter&&
       !kbRecenterPrev&&g_config.thirdPersonEnabled)
        dg::camera_trace::RecenterOnPlayer();
    kbRecenterPrev=kbRecenter;

    // V0.94: native E handles keyboard gear and radial-wheel selection.
    // The synthetic keyboard grenade / E release-pulse has been retired.
    bool primaryToggledThisFrame=false;
    const int toggleMask=g_config.tpsToggleHoldButtonMask;
    static bool toggleHeld=false,toggleConsumed=false;
    static ULONGLONG toggleStarted=0;
    const bool toggling=toggleMask && padAvailable &&
        GamepadBindingDown(toggleMask,physical.Gamepad);
    if(toggling && !toggleHeld){
        toggleHeld=true;toggleConsumed=false;toggleStarted=now;
    }
    if(toggling && toggleHeld && !toggleConsumed &&
       now>=toggleStarted &&
       now-toggleStarted>=static_cast<ULONGLONG>(g_config.tpsToggleHoldMs)){
        toggleConsumed=true;primaryToggledThisFrame=true;
        g_config.thirdPersonEnabled=!g_config.thirdPersonEnabled;
        g_config.Save();
        const unsigned count=++g_tpsHoldToggleCount;
        Log("TPS V0.76: long-press toggle n=%u mask=0x%04X TPS=%d durationMs=%llu",
            count,unsigned(toggleMask),g_config.thirdPersonEnabled?1:0,
            static_cast<unsigned long long>(now-toggleStarted));
    }
    if(!toggling && toggleHeld){
        // Preserve short-L3 recenter, but do it on RELEASE so a long
        // press does not first recenter the camera unexpectedly.
        if(!toggleConsumed && g_config.thirdPersonEnabled &&
           g_config.tpsRecenterButtonMask==toggleMask){
            const bool ok=dg::camera_trace::RecenterOnPlayer();
            Log("TPS V0.76: short-press recenter mask=0x%04X success=%d",
                unsigned(toggleMask),ok?1:0);
        }
        toggleHeld=false;toggleConsumed=false;toggleStarted=0;
    }
    // V0.77: delay START of temporary vanilla view without delaying release.
    // This prevents a quick R3 grenade tap from making the camera flash.
    // Time only valid uninterrupted physical DOWN in a focused gameplay frame.
    // If the user rebinds the button mid-game, the timer restarts cleanly.
    static ULONGLONG suspendStarted=0;
    static int previousSuspendMask=0;
    const int suspendMask=g_config.tpsSuspendButtonMask;
    // V0.95: keyboard native equipment E and the mod's temporary-view
    // shortcut MUST be allowed on the SAME physical key, by design.
    // R3 also retains its already validated grenade/temporary-view sharing.
    // A different held LB radial wheel cannot accidentally trigger R3
    // temporary view: the user's explicit keyboard suspend key still can.
    const bool keyboardSuspend=KeyboardHeld(dg::config::TpsKeyboardAction::Suspend);
    const bool controllerSuspend=padAvailable && suspendMask &&
        GamepadBindingDown(suspendMask,physical.Gamepad) &&
        !IsTPSRadialInputActive();
    const bool holdInput=focused && g_config.thirdPersonEnabled &&
        (controllerSuspend||keyboardSuspend);
    if(!holdInput || previousSuspendMask!=suspendMask)
        suspendStarted=0;
    if(holdInput && !suspendStarted)
        suspendStarted=now;
    previousSuspendMask=suspendMask;
    const int delayMs=std::clamp(g_config.tpsSuspendDelayMs,0,3000);
    const bool temporaryVanilla=holdInput && suspendStarted &&
        now>=suspendStarted &&
        now-suspendStarted>=static_cast<ULONGLONG>(delayMs);
    const bool before=rt.tpsTemporarilySuspended.exchange(
        temporaryVanilla,std::memory_order_acq_rel);
    if(before!=temporaryVanilla){
        const unsigned n=++g_tpsHoldSuspendCount;
        Log("TPS V0.77: temporary vanilla-view %s n=%u button=0x%04X delay=%d ms held=%llu ms",
            temporaryVanilla?"ON":"OFF",n,unsigned(suspendMask),delayMs,
            static_cast<unsigned long long>(holdInput&&suspendStarted?now-suspendStarted:0ull));
    }
    // Mounted TPS modes own their camera independently of ON-FOOT orbit flags.
    // Mode 1 pauses only during fire; it resumes afterwards.
    // Joystick OFF means no camera orbit on foot or on horse.
    // Native Camera Forward firing remains independent in FilterNativeXInput.
    const bool active=g_config.thirdPersonEnabled && !temporaryVanilla &&
        !rt.tpsUiViewHeld.load(std::memory_order_relaxed) &&
        !rt.tpsCutsceneSuspended.load(std::memory_order_relaxed) &&
        focused;
    // V1.27: annotate real mouse fire separately from native gamepad fire.
    // No mouse movement, WndProc routing, aim vector or XInput writes here.
    const bool mouseHeld=active && !g_overlayVisible.load() &&
        !IsTPSRadialInputActive() && MouseAnyFireHeld();
    rt.tpsMouseFireHeld.store(mouseHeld,std::memory_order_release);
    if(mouseHeld)rt.tpsMouseFireTick.store(now,std::memory_order_release);
    const float dt=g_orbitLastPresentTick && now-g_orbitLastPresentTick<=100ull
        ? static_cast<float>(now-g_orbitLastPresentTick)*0.001f:0.0f;
    g_orbitLastPresentTick=now;
    XINPUT_STATE coopPhysical{};
    const auto coopSlot=dg::local_players::PublishedSlot(1);
    const bool coopConnected=focused&&coopSlot.controllerId>=0&&
        GetCameraXInputState(static_cast<DWORD>(coopSlot.controllerId),&coopPhysical)==ERROR_SUCCESS;
    const bool secondaryToggle=dg::coop_camera::UpdateInput(coopConnected?&coopPhysical.Gamepad:nullptr,focused,dt,
        g_config.tpsRecenterButtonMask,g_config.tpsSuspendButtonMask,g_config.tpsSuspendDelayMs,
        g_config.tpsToggleHoldButtonMask,g_config.tpsToggleHoldMs);
    if(secondaryToggle&&!primaryToggledThisFrame){
        g_config.thirdPersonEnabled=!g_config.thirdPersonEnabled;g_config.Save();
        Log("TPS V1.47 P2 long-press shared TPS toggle mask=0x%X TPS=%d",
            g_config.tpsToggleHoldButtonMask,g_config.thirdPersonEnabled?1:0);
    }
    if (!active||!g_config.thirdPersonEnabled) {
        lastButtons=0;
        if(!g_config.thirdPersonEnabled) {
            rt.cameraOrbitYawDegrees.store(0.0f);
            rt.cameraOrbitPitchDegrees.store(0.0f);
        }
        return;
    }
    // V1.25: prefer true RawInput relative deltas WITHOUT cursor warping.
    // The old V1.23 fallback remains only if RawInput has been silent,
    // so broken registrations cannot strand the camera at the edge.
    const ULONGLONG freshRaw=g_v119LastRawMouseTick.load(std::memory_order_acquire);
    const bool relativeStreamHealthy=freshRaw && now>=freshRaw && now-freshRaw<500ull;
    if(relativeStreamHealthy)++g_v125RawWarpAvoided;
    // V1.23: keep unbounded TPS yaw when the OS mouse pointer runs into
    // the client border. Never reinstall any hide-reticle/SetCursor hook.
    // Warp only at an edge after real mouse input, no more than every 350ms.
    // This block runs only in active focused TPS, not overlay/wheels/cutscene.
    const ULONGLONG physicalAt=g_v123LastPhysicalMouseTick.load(
        std::memory_order_acquire);
    const ULONGLONG recenteredAt=g_v123LastRecenterTick.load(
        std::memory_order_acquire);
    const ULONGLONG gameplayAt=rt.tpsGameplayViewTick.load(
        std::memory_order_acquire);
    if(!relativeStreamHealthy && physicalAt && now>=physicalAt && now-physicalAt<180ull &&
       (!recenteredAt || (now>=recenteredAt &&
                          now-recenteredAt>=350ull)) &&
       rt.tpsViewGameplay.load(std::memory_order_relaxed) &&
       !rt.tpsAbilityWheelHeld.load(std::memory_order_relaxed) &&
       gameplayAt && now>=gameplayAt && now-gameplayAt<250ull) {
        RECT client{};
        POINT pointer{};
        if(GetClientRect(hwnd,&client) && GetCursorPos(&pointer) &&
           ScreenToClient(hwnd,&pointer) &&
           client.right-client.left>=200 && client.bottom-client.top>=160 &&
           (pointer.x<=client.left+10 || pointer.x>=client.right-11 ||
            pointer.y<=client.top+10 || pointer.y>=client.bottom-11)) {
            POINT center{(client.left+client.right)/2,
                         (client.top+client.bottom)/2};
            POINT target=center;
            if(ClientToScreen(hwnd,&target)) {
                g_v123RecenterClientX.store(center.x,std::memory_order_relaxed);
                g_v123RecenterClientY.store(center.y,std::memory_order_relaxed);
                g_v123LastRecenterTick.store(now,std::memory_order_release);
                if(SetCursorPos(target.x,target.y)) {
                    const unsigned n=++g_v123EdgeRecenters;
                    if(n<=6 || (n%250u)==0u)
                        Log("TPS V1.23 EDGE_RECAPTURE #%u cursor=(%ld,%ld) center=(%ld,%ld) physicalAgeMs=%llu rawPackets=%u",
                            n,pointer.x,pointer.y,center.x,center.y,
                            static_cast<unsigned long long>(now-physicalAt),
                            g_v120RawMousePackets.load());
                }else{
                    ++g_v123RecenterFailures;
                }
            }
        }
    }
    // Configurable recenter input, default L3 (XINPUT_GAMEPAD_LEFT_THUMB).
    // Edge detection on the ORIGINAL XInput state, not the filtered
    // game-facing state. Recenter yaw toward live player facing.
    XINPUT_STATE primary{};
    WORD buttonBits=0;
    bool suspendControllerOrbitOnFire=false;
    if(GetCameraXInputState(dg::local_players::PrimaryControllerId(),&primary)==ERROR_SUCCESS) {
        buttonBits=primary.Gamepad.wButtons;
        // If native right-stick player aim is allowed, do not also orbit
        // our independent TPS camera from that same stick during RT/RB.
        // When block is ON, orbit remains active and native XInput is filtered.
        const bool padFiringForOrbit=TPSFireDown(primary.Gamepad);
        // V0.75: explicit *camera* rotate-while-firing option. Unlike
        // vanilla right-stick PLAYER blocking, this controls ONLY the
        // independent TPS camera orbit. The two switches must not overlap.
        // Mounted shooting overrides all on-foot rotate/strafe switches.
        // Mode 0 keeps camera orbit live; mode 1 freezes independent orbit
        // and leaves the native right stick to aim (see FilterNativeXInput).
        const bool mounted=IsMountedTPSNow();
        const bool nativeMountAim=rt.tpsMountedAimMode.load()==1;
        suspendControllerOrbitOnFire=nativeMountAim && padFiringForOrbit;
        // Bounded diagnostics: proves that mounted aiming mode is actually
        // selected while firing (rather than only saved in the INI).
        const int activeMountedMode=(mounted &&
            padFiringForOrbit)
            ? rt.tpsMountedAimMode.load() : -1;
        static int previousMountedMode=-2;
        static unsigned mountedModeTransitions=0;
        if(activeMountedMode!=previousMountedMode) {
            previousMountedMode=activeMountedMode;
            const unsigned n=++mountedModeTransitions;
            if(n<=24)Log("TPS V1.02 MOUNTED AIM mode=%s n=%u padFire=%d",
                activeMountedMode==0?"CAMERA_FORWARD":
                activeMountedMode==1?"VANILLA_AIM":"INACTIVE",
                n,padFiringForOrbit?1:0);
        }
        // Diagnostic/limited guard for controller software mapping the right
        // stick into Windows mouse movement IN ADDITION TO native XInput.
        // Never globally suppress real mouse input, and time out promptly.
        const bool cameraLeft=g_config.tpsCameraStickMode==1;
        const int cameraDeadzone=cameraLeft?XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE:
            XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
        const bool cameraMoving=g_config.tpsCameraStickMode!=0&&(
            std::abs(dg::tps_input::CameraX(primary.Gamepad,cameraLeft))>cameraDeadzone||
            std::abs(dg::tps_input::CameraY(primary.Gamepad,cameraLeft))>cameraDeadzone);
        if(cameraMoving&&!IsTPSRadialInputActive())
            g_tpsPadCameraStickTick.store(now,std::memory_order_release);
        static ULONGLONG lastProbe=0;
        static float probeYaw=0.0f;
        const auto& pad=primary.Gamepad;
        const int movementDeadzone=cameraLeft?XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE:
            XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        const bool movementIdle=
            std::abs(dg::tps_input::CameraX(pad,!cameraLeft))<movementDeadzone&&
            std::abs(dg::tps_input::CameraY(pad,!cameraLeft))<movementDeadzone;
        const ULONGLONG actorTick=g_tpsActorYawTick.load();
        if(!IsTPSRadialInputActive()&&cameraMoving&&movementIdle&&
           actorTick&&now>=actorTick&&now-actorTick<300ull) {
            const float yaw=g_tpsActorWorldYaw.load();
            if(lastProbe&&now>=lastProbe&&now-lastProbe>=650ull){
                const float gap=std::fabs(std::remainder(yaw-probeYaw,360.0f));
                static unsigned events=0;
                if(gap>8.0f&&++events<=12)
                    Log("TPS V0.70: yaw changed with CAMERA STICK ONLY while XInput gate ON delta=%.1f fire=%d (alternate input/game rotation)",
                        gap,(pad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD||
                           (buttonBits&XINPUT_GAMEPAD_RIGHT_SHOULDER))?1:0);
                lastProbe=now;probeYaw=yaw;
            }else if(!lastProbe||now<lastProbe){
                lastProbe=now;probeYaw=yaw;
            }
        }else{
            lastProbe=0;
        }
        const int mask=g_config.tpsRecenterButtonMask;
        // When short-recenter and long-TPS toggle share L3, the latter
        // handles release. All other recenter bindings remain edge-triggered.
        static bool previousRecenterPressed=false;
        static int previousRecenterBinding=-1;
        const bool pressed=GamepadBindingDown(mask,primary.Gamepad);
        if(previousRecenterBinding!=mask)previousRecenterPressed=false;
        if(mask && !(g_config.tpsToggleHoldButtonMask!=0 &&
            mask==g_config.tpsToggleHoldButtonMask) &&
            pressed && !previousRecenterPressed) {
            const bool success=dg::camera_trace::RecenterOnPlayer();
            Log("TPS V0.70: recenter button=0x%X successful=%d",
                unsigned(mask),success?1:0);
        }
        previousRecenterPressed=pressed;
        previousRecenterBinding=mask;
    }
    lastButtons=buttonBits;
    static bool priorSuspend=false;
    if(suspendControllerOrbitOnFire!=priorSuspend) {
        priorSuspend=suspendControllerOrbitOnFire;
        Log("TPS V0.69: controller camera orbit %s while native aim enabled",
            suspendControllerOrbitOnFire?"SUSPENDED (RT/RB)":"RESUMED");
    }
    float ax=0.0f,ay=0.0f;
    if(g_config.cameraControllerOrbitEnabled&&!suspendControllerOrbitOnFire&&!IsTPSRadialInputActive()){
        const DWORD primaryId=static_cast<DWORD>(dg::local_players::PrimaryControllerId());
        XINPUT_STATE state{};
        if(GetCameraXInputState(primaryId,&state)==ERROR_SUCCESS){
            // Preserve the validated configurable wheel-selection priority.
            const bool left=g_config.tpsCameraStickMode==1;
            const float x=static_cast<float>(dg::tps_input::CameraX(state.Gamepad,left))/32767.0f;
            const float y=static_cast<float>(dg::tps_input::CameraY(state.Gamepad,left))/32767.0f;
            const float len=std::sqrt(x*x+y*y);
            const float dead=static_cast<float>(left?XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE:
                XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)/32767.0f;
            if (len>dead) {
                const float m=std::min(1.0f,(len-dead)/(1.0f-dead));
                ax=x/len*m;ay=y/len*m;
            }
        }
    }
    // V1.19: single native TPS orbit for mouse and controller. The mouse
    // path has NO activation option, no cursor warping and no virtual pad.
    // Convert relative mouse pixels to view degrees, in this same Present
    // writer that already updates physical controller orbit.
    const int mx=std::clamp(g_v119MouseDeltaX.exchange(0),-300,300);
    const int my=std::clamp(g_v119MouseDeltaY.exchange(0),-300,300);
    constexpr float kMouseDegPerPixel=0.13f;
    const float stick=g_config.cameraStickSpeed;
    if (!std::isfinite(stick)) return;
    const float dyaw=ax*stick*dt + static_cast<float>(mx)*kMouseDegPerPixel;
    const float dpitch=ay*stick*dt - static_cast<float>(my)*kMouseDegPerPixel;
    if(mx||my) {
        const unsigned n=++g_v119MouseCameraFrames;
        if(n<=5 || n%5000==0)
            Log("TPS V1.19 MOUSE_CAMERA frame=%u delta=(%d,%d) orbitYaw=%.2f orbitPitch=%.2f firing=%d",
                n,mx,my,dyaw,dpitch,MouseAnyFireHeld()?1:0);
    }
    if (dyaw!=0.0f || dpitch!=0.0f) {
        rt.cameraOrbitYawDegrees.store(std::remainder(rt.cameraOrbitYawDegrees.load()+dyaw,360.0f));
        rt.cameraOrbitPitchDegrees.store(std::clamp(rt.cameraOrbitPitchDegrees.load()+dpitch,-60.0f,60.0f));
    }
}

// V1.19: fresh TPS mouse orbit uses original WndProc data, shared camera
// direction, and original native input dispatch. No mouse-mode toggle.

HRESULT __stdcall HookPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
    static std::atomic_bool once{false};
    if(!once.exchange(true))ReportStartupPhase(4,flags);
    ApplySkipIntroSetting(false);
    UpdateCameraDof();

    // V0.31 HorseFeature::Tick is intentionally a no-op. Horse writes are
    // allowed only while a native HorseCharacter callback proves the UObject
    // is alive; cached raw pointers are never written from Present().
    dg::horse::Tick();

    ProcessInput();
    g_config.FlushIfDue(false);

    TraceGameWindowState(swapChain);
    ProcessCameraInput();
    EnsureOrbitWndProc(swapChain);
    if(g_originalWndProc && g_hwnd)EnsureRelativeMouse(g_hwnd);
    EnsureNativeXInputAimHooks();
    UpdateOrbitInput();

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

    // Early D3D11 probe device exists before game shader creation. Aim-only
    // GPU interception is isolated and fail-open; gameplay hooks untouched.
    if (g_targetValidation.exact)
        dg::aim_shader::Install(probeDevice, probeContext, &Log);

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
    dg::native_tps::Report();
    dg::health::Shutdown();
    dg::glide::Report();
    // Release the supported synthetic horse key before teardown.
    if(g_horseSyntheticHDown.exchange(false))EmitKeyboardVirtualKey('H',false);
    g_config.FlushIfDue(true);
    dg::skip_logos::Shutdown();
    dg::skip_warning::Shutdown();
    dg::horse::Shutdown();
    dg::aim_shader::Shutdown();
    dg::chaos_pose::Shutdown();
    dg::manual_gauges::Shutdown();
    dg::ammo::Shutdown();
    dg::hotstreak::Shutdown();
    dg::potions::Shutdown();

    dg::resource_gain::Shutdown();
    dg::gauges::Shutdown();
    const auto scene=dg::camera_trace::GetTelemetry();
    // Print the UI's temporary cutscene history at shutdown as well, so
    // the user can send a log without screen captures of the debug menu.
    const auto cutsceneHistory=dg::camera_trace::GetCinematicDebugSnapshot();
    Log("TPS V1.28 CINEMATIC_LIST historyCount=%u activeCandidate=%u suspended=%d (HEURISTIC ONLY)",
        cutsceneHistory.count,cutsceneHistory.activeCandidateId,
        cutsceneHistory.currentlySuspended?1:0);
    for(uint32_t eventIndex=0;eventIndex<cutsceneHistory.count;++eventIndex){
        const auto& event=cutsceneHistory.events[eventIndex];
        Log("TPS V1.28 CINEMATIC_LIST item=%u id=%u event=%s sceneCandidate=%u durationMs=%llu actorValid=%d near=%d angles=%d gameplayShape=%d pitch=%.1f yaw=%.1f fov=%.1f xy=%.0f",
            eventIndex+1,event.serial,
            dg::camera_trace::CinematicDebugPhaseLabel(event.phase),
            event.candidateId,static_cast<unsigned long long>(event.durationMs),
            event.actorValid?1:0,event.nearPawn?1:0,
            event.normalAngles?1:0,event.gameplayCamera?1:0,
            event.nativePitch,event.nativeYaw,event.nativeFov,event.gapXY);
    }
    Log("TPS V1.27 FACING_CAMERA totals: footSamples=%u footGapOver75=%u horsePoseSamples=%u horsePoseGapOver75=%u mountedMouseViewFrames=%u mouseHorsePoseHeld=%u (native yaw only; no projectile redirection)",
        scene.facingFootSamples,scene.facingFootOffAxis,
        scene.facingHorseSamples,scene.facingHorseOffAxis,
        scene.mountedMouseViewFrames,scene.mountedMouseHorsePoseHeld);
    Log("TPS V1.32 AIM_STATE: native mouse target acquisition replaces V1.31 yaw edits; native collision reused; physical pad path retained; actual projectile impacts require in-game validation");
    Log("TPS V1.20 CINEMATICS (read-only) enters=%u exits=%u candidates=%u missingActorSamples=%u farCameraSamples=%u nativeCutsceneBehaviorUnchanged=1",
        scene.cinematicEntered,scene.cinematicExited,
        scene.cinematicCandidateBegins,scene.cinematicMissingActorSamples,
        scene.cinematicFarViewSamples);
    RestoreCameraDof();
    Log("TPS V0.69 totals: mouseInputNative=PASSTHROUGH rightStickReadsFiltered=%u whileFiring=%u",
        g_nativePadAimSuppressed.load(),g_nativePadAimSuppressedDuringFire.load());
    Log("TPS native charged-shot passthrough: samples=%u wheelMask=0x%X",
        g_tpsChargeChordSamples.load(),dg::runtime::Get().tpsWheelPadMask.load(std::memory_order_relaxed));

    Log("TPS V1.04 forward totals: all=%u horse=%u foot=%u mappedHorseChord=%u keyboardHorse=%u",
        g_tpsForwardAnalogInjected.load(),g_tpsMountedForwardAnalogInjected.load(),
        g_tpsFootForwardAnalogInjected.load(),g_horsePadChordRemapped.load(),g_keyboardHorseInjected.load());
    Log("TPS V1.03 controller-only: rightStickFiltered=%u no TPS mouse orbit/virtual pad",
        g_nativePadAimSuppressed.load());
    Log("TPS V1.18 MOUSE_READONLY totals: WM_MOUSEMOVE=%u WM_INPUT_UNDECODED=%u LMB=%u RMB=%u WHEEL=%u",
        g_v118MouseMove.load(),g_v118RawInputMessages.load(),
        g_v118MouseLmb.load(),g_v118MouseRmb.load(),g_v118MouseWheel.load());
    Log("TPS V1.19 MOUSE_FORWARD totals: rawMouse=%u moveFallback=%u cameraFrames=%u nativeLookForward=%u nativeRotationForward=%u fallbackEdgeRecenter=V1.23 syntheticPad=0",
        g_v119RawMouse.load(),g_v119MouseMoveFallback.load(),
        g_v119MouseCameraFrames.load(),0u,0u);

    Log("TPS V1.26 RAW_RECOVERY totals: checks=%u missing=%u rearmAttempts=%u rearmSuccess=%u foreignRegistrationProtected=%u stallsWithRegistration=%u warpsAvoided=%u throttled=%u recoveredAfterOldCap=%u lastOwnerState=%d",
        g_v125RawChecks.load(),g_v125RawMissing.load(),
        g_v125RawRearmAttempts.load(),g_v125RawRearmSuccess.load(),
        g_v125RawForeignOwners.load(),g_v125RawStreamStalls.load(),
        g_v125RawWarpAvoided.load(),g_v126RearmThrottled.load(),
        g_v126RecoveredAfterEight.load(),g_v125RawOwnerState.load());
    Log("TPS V1.24 MOUSE_FIRE totals primaryRouted=%u secondaryRouted=%u nativeSuppressed=%u secondaryForwardLook=%u primaryVK=%d secondaryVK=%d",
        g_v124PrimaryRouted.load(),g_v124SecondaryRouted.load(),
        g_v124NativeButtonSuppressed.load(),0u,
        dg::runtime::Get().tpsMousePrimaryButton.load(std::memory_order_relaxed),dg::runtime::Get().tpsMouseSecondaryButton.load(std::memory_order_relaxed));
    Log("TPS V1.23 EDGE_RECAPTURE totals: edgeRecenters=%u syntheticMovesIgnored=%u recenterFailures=%u cursorHooks=REMOVED",
        g_v123EdgeRecenters.load(),g_v123SyntheticMovesIgnored.load(),
        g_v123RecenterFailures.load());
    Log("TPS V1.20 RELATIVE_INPUT totals: registeredByMod=%d nativePreserved=%u registrationErrors=%u rawMouse=%u rawOther=%u rawAtScreenEdge=%u fallbackMoves=%u edgeOnlyRecenter=1",
        g_v120RawAddedByMod.load()?1:0,g_v120ExistingRawMouse.load(),
        g_v120RawRegistrationErrors.load(),g_v120RawMousePackets.load(),
        g_v120RawNonMousePackets.load(),g_v120RawAtEdge.load(),
        g_v119MouseMoveFallback.load());

    Log("TPS V0.87 AIM UI PROBE totals: targeting=%u targetingTPS=%u targetingFiring=%u positioning=%u (all events passed through)",
        g_aimUiEvents.load(),g_aimUiEventsInTPS.load(),
        g_aimUiEventsWhileFire.load(),g_aimUiPositionEvents.load());
    Log("TPS V0.84 grenade totals: physicalDown=%u physicalUp=%u nativeReleasePulses=%u (game pickup/stow must be verified visually)",
        g_tpsGrenadeDownCount.load(),g_tpsGrenadeUpCount.load(),
        g_tpsGrenadeReleasePulseDelivered.load());
    Log("TPS V1.45 native ground-FX override RETIRED; shader scope retained; top-down dash aim requires user retest");
    dg::projectile_aim::Report();
    Log("TPS configured view-input totals: holdSuspend=%u longToggle=%u",
        g_tpsHoldSuspendCount.load(),g_tpsHoldToggleCount.load());
    Log("TPS V1.02 controller aim totals: nativeLookInput=%u nativeLookRotation=%u suppressedLook=%u heldYaw=%u padStrafe=%u padLookAim=%u signaturesFailed=%u",
        g_tpsLookInputCalls.load(),g_tpsLookRotationCalls.load(),
        g_tpsNativeLookInputBlocks.load(),g_tpsNativeLookRotationBlocks.load(),
        g_tpsPadStrafeApplied.load(),g_tpsNativeLookInputPadAim.load(),
        g_tpsNativeLookSignatureFailures.load());

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
    ReportStartupPhase(1,0);

    std::atexit(&ShutdownMod);

    g_config.SetPath(g_iniPath);
    g_config.SetLogger(&FeatureLog);
    g_config.Load();
    Log("%s CONFIG loaded %ls: TPS=%d cursorHooks=REMOVED FOV=%.1f dist=%.3f pitch=%.2f height=%.1f foot=%.1f stickMode=%d mountedAim=%d normalHeight=%.1f SkipWarningRuntime=%d",
        dg::build::Version,g_iniPath.c_str(),g_config.thirdPersonEnabled?1:0,
        g_config.thirdPersonFovDegrees,
        g_config.thirdPersonDistanceMultiplier,g_config.thirdPersonPitchDegrees,
        g_config.thirdPersonHeightOffset,g_config.tpsFootAnchorOffset,
        g_config.tpsCameraStickMode,g_config.tpsMountedAimMode,g_config.cameraHeightOffset,
        g_config.skipWarningEnabled?1:0);
    Log("TPS %s COOP_FOV loaded solo=%.1f coop=%.1f (both split-screen views)",
        dg::build::Version,g_config.thirdPersonFovDegrees,g_config.thirdPersonCoopFovDegrees);
    Log("TPS V1.37 ULTIMATE_CAMERA loaded distance=%.3f verticalOffset=%.1f (separate from normal form)",
        g_config.tpsUltimateDistanceMultiplier,g_config.tpsUltimateHeightOffset);
    Log("TPS %s HORSE_CAMERA loaded distance=%.3f verticalOffset=%.1f (separate from foot and Ultimate)",
        dg::build::Version,g_config.tpsHorseDistanceMultiplier,g_config.tpsHorseHeightOffset);
    Log("V1.05 GAUGES loaded Wrath=%d x%.2f Ultimate=%d x%.2f Synergy=%d x%.2f",
        g_config.wrathChargeEnabled?1:0,g_config.wrathChargeMultiplier,
        g_config.ultimateChargeEnabled?1:0,g_config.ultimateChargeMultiplier,
        g_config.synergyChargeEnabled?1:0,g_config.synergyChargeMultiplier);

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

    ReportStartupPhase(5,g_targetValidation.exact?1u:0u);
    // Install early on this worker after exact executable validation, so the
    // original startup constructor/loaders remain wholly native.
    dg::skip_warning::Initialize(&FeatureLog,g_targetValidation.exact);

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
    dg::local_players::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::coop_camera::Initialize(&FeatureLog);
    dg::camera_trace::Install(&FeatureLog);
    dg::native_tps::Install(&ProvideNativeMouseRay,&FeatureLog);
    dg::chaos_pose::Initialize(&FeatureLog,g_targetValidation.exact);
    Log("TPS V1.33 mouse aim=CAMERA_CENTER: actual TPS yaw/pitch, native collision; UI look/XInput suppression disabled during native UI ownership; gameplay EXPERIMENTAL");
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
    if (!InstallMovementSpeedHook()) {
        Log("Movement Speed unavailable; other ASI features remain active.");
    }
    // Passive UI observation only; no native ground-target visibility detour.
    InstallAimUiEventProbe();
    if (!InstallNativeTPSRotationHook()) {
        Log("TPS V0.75 native look hooks unavailable; vanilla look path untouched.");
    }
    Log("Action Recovery retired: ineffective V1.39 experiment; native timing retained, legacy INI/actions preserved");
    if (!InstallHotstreakChargeHook()) {
        Log("Hotstreak Charge unavailable; other ASI features remain active.");
    }
    dg::manual_gauges::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::gauges::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::resource_gain::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::ammo::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::hotstreak::Initialize(&FeatureLog,g_targetValidation.exact,g_originalAddJuice);
    dg::potions::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::local_players::SetLiveObserver(&ServiceLocalGameplay);
    dg::projectile_aim::Initialize(&FeatureLog,g_targetValidation.exact,&ProvideProjectileRotation);
    dg::health::Initialize(&FeatureLog,g_targetValidation.exact);
    dg::glide::Initialize(&FeatureLog,g_targetValidation.exact);
    if (!InstallFinalOutgoingDamageHook()) {
        Log("Final Pistol/Melee Damage hook unavailable; other ASI features remain active.");
    }

    Log(
        "Core initialization complete. Press %s after the first game frame.",
        KeyDisplayName(g_config.menuKey).c_str()
    );
    ReportStartupPhase(3,0);
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
