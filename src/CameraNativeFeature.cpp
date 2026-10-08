#include "CameraNativeFeature.h"
#include "RuntimeSettings.h"

#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdint>

namespace dg::camera_native {
namespace {

constexpr uintptr_t kGetCameraViewRva = 0x16F9790;
constexpr uintptr_t kSpringArmUpdateRva = 0x6F57B0;

// Verified against the exact March 2020 retail PE64. Do not substitute
// script thunks at 0x1CE7D30/0x1DB6F10: those are not native frame rendering.
constexpr unsigned char kGetCameraViewBytes[] = {
    0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0xA8,0x18,0xFD,0xFF,0xFF
};
constexpr unsigned char kSpringArmUpdateBytes[] = {
    0x48,0x8B,0xC4,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xA8,0x98,0xFD,0xFF,0xFF
};

// Win64 native C++ member ABI.
//   UCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& OutView)
//   USpringArmComponent::UpdateDesiredArmLocation(bool,bool,bool,float)
using GetCameraViewFn = void(*)(void*, float, void*);
using SpringArmUpdateFn = void(*)(void*, bool, bool, bool, float);

GetCameraViewFn g_getViewOriginal = nullptr;
SpringArmUpdateFn g_springOriginal = nullptr;
LogFn g_log = nullptr;
std::atomic_bool g_fovReady{false};
std::atomic_bool g_zoomReady{false};
std::atomic_uint32_t g_fovCalls{0};
std::atomic_uint32_t g_zoomCalls{0};
std::atomic<float> g_nativeFov{0.0f};
std::atomic<float> g_appliedFov{0.0f};
std::atomic<float> g_nativeDistance{0.0f};
std::atomic<float> g_appliedDistance{0.0f};
thread_local bool t_springNested = false;

void WriteLog(const char* msg) {
    if (g_log && msg) g_log(msg);
}

void HookGetCameraView(void* camera, float deltaTime, void* desiredView) {
    // Always let the native camera calculate location, rotation, projection,
    // postprocessing and any script overrides before changing only the FOV.
    if (g_getViewOriginal)
        g_getViewOriginal(camera, deltaTime, desiredView);

    if (!camera || !desiredView) return;
    auto& runtime = dg::runtime::Get();
    const bool enabled = runtime.fovEnabled.load(std::memory_order_relaxed);
    auto* fov = reinterpret_cast<float*>(
        static_cast<unsigned char*>(desiredView) + 0x18);
    const float native = *fov;
    const uint32_t hits = g_fovCalls.fetch_add(1, std::memory_order_relaxed) + 1;

    float applied = native;
    if (enabled && std::isfinite(native) && native >= 10.0f &&
        native <= 179.0f) {
        const float target = runtime.fovDegrees.load(std::memory_order_relaxed);
        if (std::isfinite(target) && target >= 40.0f && target <= 140.0f) {
            *fov = target; // output FMinimalViewInfo only; NO UObject write
            applied = target;
        }
    }
    g_nativeFov.store(native, std::memory_order_relaxed);
    g_appliedFov.store(applied, std::memory_order_relaxed);

    if (hits <= 8 || hits % 5000 == 0) {
        char line[210]{};
        sprintf_s(line,
            "CameraNative V0.36: GetCameraView hits=%u camera=%p "
            "FOV native=%.2f applied=%.2f enabled=%d",
            hits, camera, native, applied, enabled ? 1 : 0);
        WriteLog(line);
    }
}

void HookSpringArmUpdate(void* springArm, bool doTrace,
                         bool doLocationLag, bool doRotationLag,
                         float deltaTime) {
    if (!g_springOriginal) return;
    if (!springArm || t_springNested) {
        g_springOriginal(springArm, doTrace, doLocationLag,
                         doRotationLag, deltaTime);
        return;
    }

    auto& runtime = dg::runtime::Get();
    int zoom = runtime.cameraZoomPercent.load(std::memory_order_relaxed);
    if (zoom < -75) zoom = -75;
    if (zoom > 200) zoom = 200;

    auto* distance = reinterpret_cast<float*>(
        static_cast<unsigned char*>(springArm) + 0x258);
    const float native = *distance;
    float applied = native;
    const bool canScale = zoom != 0 && std::isfinite(native) &&
                          native >= 25.0f && native <= 25000.0f;
    if (canScale)
        applied = native * (100.0f / (100.0f + static_cast<float>(zoom)));

    const uint32_t hits = g_zoomCalls.fetch_add(1, std::memory_order_relaxed) + 1;
    g_nativeDistance.store(native, std::memory_order_relaxed);
    g_appliedDistance.store(applied, std::memory_order_relaxed);

    // V0.31 lifetime invariant: only write inside a live, native callback.
    // There is no cached SpringArm/UObject pointer or deferred restoration.
    t_springNested = true;
    if (canScale) *distance = applied;
    g_springOriginal(springArm, doTrace, doLocationLag,
                     doRotationLag, deltaTime);
    if (canScale) *distance = native;
    t_springNested = false;

    if (hits <= 8 || hits % 5000 == 0) {
        char line[245]{};
        sprintf_s(line,
            "CameraNative V0.36: SpringArmUpdate hits=%u arm=%p "
            "distance native=%.1f applied=%.1f zoom=%+d%%",
            hits, springArm, native, applied, zoom);
        WriteLog(line);
    }
}

bool InstallOne(uintptr_t base, uintptr_t rva,
                const unsigned char* prefix, size_t prefixSize,
                void* detour, void** original,
                const char* name) {
    auto* target = reinterpret_cast<unsigned char*>(base + rva);
    if (std::memcmp(target, prefix, prefixSize) != 0) {
        char buf[180]{};
        sprintf_s(buf,"CameraNative V0.36: %s code signature mismatch, skipped",name);
        WriteLog(buf);
        return false;
    }
    const MH_STATUS created = MH_CreateHook(target, detour, original);
    if (created != MH_OK) {
        char buf[180]{};
        sprintf_s(buf,"CameraNative V0.36: %s MH_CreateHook failed=%d",
                  name,static_cast<int>(created));
        WriteLog(buf);
        return false;
    }
    const MH_STATUS enabled = MH_EnableHook(target);
    if (enabled != MH_OK && enabled != MH_ERROR_ENABLED) {
        char buf[180]{};
        sprintf_s(buf,"CameraNative V0.36: %s MH_EnableHook failed=%d",
                  name,static_cast<int>(enabled));
        WriteLog(buf);
        return false;
    }
    char buf[180]{};
    sprintf_s(buf,"CameraNative V0.36: %s READY RVA=0x%zX",
              name,static_cast<size_t>(rva));
    WriteLog(buf);
    return true;
}
} // namespace

void Install(LogFn logger) {
    g_log = logger;
    const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (!base) return;

    const MH_STATUS initialized = MH_Initialize();
    if (initialized != MH_OK &&
        initialized != MH_ERROR_ALREADY_INITIALIZED) {
        WriteLog("CameraNative V0.36: MinHook unavailable, camera untouched");
        return;
    }

    const bool fov = InstallOne(base,kGetCameraViewRva,
        kGetCameraViewBytes,sizeof(kGetCameraViewBytes),
        reinterpret_cast<void*>(&HookGetCameraView),
        reinterpret_cast<void**>(&g_getViewOriginal),
        "UCameraComponent::GetCameraView");
    g_fovReady.store(fov,std::memory_order_relaxed);

    const bool zoom = InstallOne(base,kSpringArmUpdateRva,
        kSpringArmUpdateBytes,sizeof(kSpringArmUpdateBytes),
        reinterpret_cast<void*>(&HookSpringArmUpdate),
        reinterpret_cast<void**>(&g_springOriginal),
        "USpringArmComponent::UpdateDesiredArmLocation");
    g_zoomReady.store(zoom,std::memory_order_relaxed);
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.fovHookReady = g_fovReady.load(std::memory_order_relaxed);
    t.zoomHookReady = g_zoomReady.load(std::memory_order_relaxed);
    t.fovCalls = g_fovCalls.load(std::memory_order_relaxed);
    t.zoomCalls = g_zoomCalls.load(std::memory_order_relaxed);
    t.nativeFov = g_nativeFov.load(std::memory_order_relaxed);
    t.appliedFov = g_appliedFov.load(std::memory_order_relaxed);
    t.nativeDistance = g_nativeDistance.load(std::memory_order_relaxed);
    t.appliedDistance = g_appliedDistance.load(std::memory_order_relaxed);
    return t;
}
} // namespace dg::camera_native
