#include "CameraTraceFeature.h"
#include "RuntimeSettings.h"
#include <windows.h>
#include <MinHook.h>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace dg::camera_trace {
namespace {
// Exact native targets, not Unreal script thunks. The main executable
// SHA256 is verified before Install() by the ASI core.
constexpr uintptr_t kViewRva = 0x16F9790;
constexpr uintptr_t kArmRva = 0x6F57B0;
constexpr unsigned char kViewBytes[] = {
  0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0xA8,0x18,0xFD,0xFF,0xFF
};
constexpr unsigned char kArmBytes[] = {
  0x48,0x8B,0xC4,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xA8,0x98,0xFD,0xFF,0xFF
};
using ViewFn = void (*)(void*, float, void*);
using ArmFn = void (*)(void*, bool, bool, bool, float);
ViewFn g_view = nullptr;
ArmFn g_arm = nullptr;
LogFn g_log = nullptr;
std::atomic<bool> g_viewReady{false}, g_armReady{false};
std::atomic<uint32_t> g_viewCount{0}, g_armCount{0};
std::atomic<float> g_nativeFov{0.0f}, g_appliedFov{0.0f};
std::atomic<float> g_nativePitch{0.0f}, g_appliedPitch{0.0f};
std::atomic<float> g_nativeYaw{0.0f}, g_appliedYaw{0.0f};
std::atomic<float> g_nativeHeight{0.0f}, g_appliedHeight{0.0f};
std::atomic<float> g_nativeDistance{0.0f}, g_appliedDistance{0.0f};
thread_local bool g_inArm = false;
void Write(const char* s) { if (g_log && s) g_log(s); }
bool Sample(uint32_t n) { return n <= 12 || (n % 5000) == 0; }

void HookView(void* camera, float dt, void* outView) {
    if (g_view) g_view(camera, dt, outView);
    if (!outView) return;
    // Transient FMinimalViewInfo: FVector Location +0, FRotator Rotation +12,
    // FOV +24. Never write a camera UObject or retain its pointer.
    auto* p = static_cast<unsigned char*>(outView);
    float nativeFov = 0, nativePitch = 0, nativeHeight = 0, nativeYaw = 0;
    std::memcpy(&nativeFov, p + 0x18, sizeof(float));
    std::memcpy(&nativePitch, p + 0x0C, sizeof(float));
    std::memcpy(&nativeYaw, p + 0x10, sizeof(float));
    std::memcpy(&nativeHeight, p + 0x08, sizeof(float));
    if (!std::isfinite(nativeFov) || nativeFov < 1 || nativeFov > 179) return;
    if (!std::isfinite(nativePitch) || std::fabs(nativePitch) > 360) return;
    if (!std::isfinite(nativeHeight) || std::fabs(nativeHeight) > 10000000.0f) return;
    if (!std::isfinite(nativeYaw) || std::fabs(nativeYaw) > 100000.0f) return;

    float appliedFov = nativeFov, appliedPitch = nativePitch;
    float appliedHeight = nativeHeight, appliedYaw = nativeYaw;
    auto& settings = dg::runtime::Get();
    if (settings.fovEnabled.load(std::memory_order_relaxed)) {
        const float target = settings.fovDegrees.load(std::memory_order_relaxed);
        if (std::isfinite(target) && target >= 40.0f && target <= 140.0f) {
            appliedFov = target;
            std::memcpy(p + 0x18, &appliedFov, sizeof(float));
        }
    }
    const float offset = settings.cameraPitchDegrees.load(std::memory_order_relaxed);
    if (std::isfinite(offset) && offset >= -35.0f && offset <= 35.0f && offset != 0.0f) {
        appliedPitch = std::clamp(nativePitch + offset, -89.0f, 89.0f);
        std::memcpy(p + 0x0C, &appliedPitch, sizeof(float));
    }
    const float yawOffset = settings.cameraYawDegrees.load(std::memory_order_relaxed);
    if (std::isfinite(yawOffset) && yawOffset >= -180.0f && yawOffset <= 180.0f &&
        yawOffset != 0.0f) {
        // Transient FMinimalViewInfo::Rotation.Yaw (+0x10).
        // Never modify the camera UObject or retain pointers across frames.
        appliedYaw = std::remainder(nativeYaw + yawOffset, 360.0f);
        std::memcpy(p + 0x10, &appliedYaw, sizeof(float));
    }
    g_nativeYaw.store(nativeYaw, std::memory_order_relaxed);
    g_appliedYaw.store(appliedYaw, std::memory_order_relaxed);
    const float heightOffset = settings.cameraHeightOffset.load(std::memory_order_relaxed);
    if (std::isfinite(heightOffset) && heightOffset >= -1500.0f &&
        heightOffset <= 1500.0f && heightOffset != 0.0f) {
        appliedHeight = nativeHeight + heightOffset;
        std::memcpy(p + 0x08, &appliedHeight, sizeof(float));
    }
    // Third Person V0.54: modify only transient FMinimalViewInfo,
    // never the UObject's persistent transform. Enabled only by user.
    if (settings.thirdPersonEnabled.load(std::memory_order_relaxed)) {
        const float dist=g_nativeDistance.load(std::memory_order_relaxed);
        const float mult=settings.thirdPersonDistanceMultiplier.load(std::memory_order_relaxed);
        float x=0.0f,y=0.0f;
        std::memcpy(&x,p,sizeof(float)); std::memcpy(&y,p+4,sizeof(float));
        if (std::isfinite(x)&&std::isfinite(y)&&dist>=100.0f&&dist<=10000.0f&&
            std::isfinite(mult)&&mult>=0.25f&&mult<=3.0f&&
            nativePitch>=-89.0f&&nativePitch<=-15.0f) {
            constexpr float rad=0.01745329251994329577f;
            const float np=nativePitch*rad,ny=nativeYaw*rad;
            const float dp=-12.0f*rad,dy=appliedYaw*rad;
            const float distance=std::clamp(dist*mult,120.0f,12000.0f);
            const float pivotX=x+std::cos(np)*std::cos(ny)*dist;
            const float pivotY=y+std::cos(np)*std::sin(ny)*dist;
            const float pivotZ=nativeHeight+std::sin(np)*dist;
            const float newX=pivotX-std::cos(dp)*std::cos(dy)*distance;
            const float newY=pivotY-std::cos(dp)*std::sin(dy)*distance;
            const float newZ=pivotZ-std::sin(dp)*distance+60.0f;
            if (std::isfinite(newX)&&std::isfinite(newY)&&std::isfinite(newZ)&&
                std::fabs(newX)<1e7f&&std::fabs(newY)<1e7f&&std::fabs(newZ)<1e7f) {
                const float pitch=-12.0f;
                std::memcpy(p,&newX,sizeof(float));
                std::memcpy(p+4,&newY,sizeof(float));
                std::memcpy(p+8,&newZ,sizeof(float));
                std::memcpy(p+12,&pitch,sizeof(float));
                appliedPitch=pitch; appliedHeight=newZ;
            }
        }
    }
    g_nativeHeight.store(nativeHeight, std::memory_order_relaxed);
    g_appliedHeight.store(appliedHeight, std::memory_order_relaxed);
    g_nativeFov.store(nativeFov, std::memory_order_relaxed);
    g_appliedFov.store(appliedFov, std::memory_order_relaxed);
    g_nativePitch.store(nativePitch, std::memory_order_relaxed);
    g_appliedPitch.store(appliedPitch, std::memory_order_relaxed);
    const uint32_t calls = g_viewCount.fetch_add(1, std::memory_order_relaxed) + 1;
    if (Sample(calls)) {
        char line[230]{};
        sprintf_s(line,"Camera V0.38: native View calls=%u camera=%p FOV=%.2f->%.2f pitch=%.2f->%.2f yaw=%.2f->%.2f height=%.1f->%.1f",
            calls,camera,nativeFov,appliedFov,nativePitch,appliedPitch,nativeYaw,appliedYaw,nativeHeight,appliedHeight);
        Write(line);
    }
}

void HookArm(void* arm, bool trace, bool locationLag, bool rotationLag, float dt) {
    if (!g_arm) return;
    if (!arm || g_inArm) {
        g_arm(arm, trace, locationLag, rotationLag, dt);
        return;
    }
    // Scope strictly the live native callback, never a cached UObject.
    auto* distance = static_cast<unsigned char*>(arm) + 0x258;
    float native = 0;
    std::memcpy(&native, distance, sizeof(float));
    const float zoom = dg::runtime::Get().cameraZoomPercent.load(std::memory_order_relaxed);
    const bool valid = std::isfinite(native) && native >= 25 && native <= 25000 &&
                       std::isfinite(zoom) && zoom >= -75 && zoom <= 200;
    float applied = native;
    if (valid && zoom != 0) {
        applied = native * (100.0f / (100.0f + zoom));
        if (applied >= 20.0f && applied <= 30000.0f)
            std::memcpy(distance, &applied, sizeof(float));
        else
            applied = native;
    }
    g_inArm = true;
    g_arm(arm, trace, locationLag, rotationLag, dt);
    if (applied != native) std::memcpy(distance, &native, sizeof(float));
    g_inArm = false;
    if (valid) {
        g_nativeDistance.store(native, std::memory_order_relaxed);
        g_appliedDistance.store(applied, std::memory_order_relaxed);
    }
    const uint32_t calls = g_armCount.fetch_add(1, std::memory_order_relaxed) + 1;
    if (Sample(calls)) {
        char line[220]{};
        sprintf_s(line,"Camera V0.38: native SpringArm calls=%u arm=%p length=%.1f->%.1f zoom=%+.1f%%",
                  calls,arm,native,applied,zoom);
        Write(line);
    }
}

bool InstallOne(uintptr_t base, uintptr_t rva, const unsigned char* signature,
                size_t length, void* detour, void** original, const char* label) {
    auto* target = reinterpret_cast<unsigned char*>(base + rva);
    if (std::memcmp(target, signature, length) != 0) {
        char line[160]{};
        sprintf_s(line,"Camera V0.38: %s signature mismatch; SKIPPED",label);
        Write(line);
        return false;
    }
    const MH_STATUS created = MH_CreateHook(target,detour,original);
    if (created != MH_OK) {
        char line[160]{};
        sprintf_s(line,"Camera V0.38: %s CreateHook failed=%d",label,int(created));
        Write(line);
        return false;
    }
    const MH_STATUS enabled = MH_EnableHook(target);
    if (enabled != MH_OK && enabled != MH_ERROR_ENABLED) {
        MH_RemoveHook(target);
        char line[160]{};
        sprintf_s(line,"Camera V0.38: %s EnableHook failed=%d",label,int(enabled));
        Write(line);
        return false;
    }
    char line[160]{};
    sprintf_s(line,"Camera V0.38: %s READY nativeRVA=0x%zX",label,size_t(rva));
    Write(line);
    return true;
}
}
void Install(LogFn log) {
    g_log = log;
    const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    if (!base) return;
    const MH_STATUS status = MH_Initialize();
    if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED) {
        Write("Camera V0.38: MinHook initialization failed; camera untouched");
        return;
    }
    const bool v = InstallOne(base,kViewRva,kViewBytes,sizeof(kViewBytes),
       reinterpret_cast<void*>(&HookView),reinterpret_cast<void**>(&g_view),"View");
    const bool a = InstallOne(base,kArmRva,kArmBytes,sizeof(kArmBytes),
       reinterpret_cast<void*>(&HookArm),reinterpret_cast<void**>(&g_arm),"SpringArm");
    g_viewReady.store(v);
    g_armReady.store(a);
    char line[140]{};
    sprintf_s(line,"Camera V0.38: installed view=%d arm=%d (in-game test required)",v?1:0,a?1:0);
    Write(line);
}
Telemetry GetTelemetry() {
    Telemetry t{};
    t.viewReady = g_viewReady.load();
    t.armReady = g_armReady.load();
    t.viewCalls = g_viewCount.load();
    t.armCalls = g_armCount.load();
    t.nativeFov = g_nativeFov.load();
    t.appliedFov = g_appliedFov.load();
    t.nativePitch = g_nativePitch.load();
    t.appliedPitch = g_appliedPitch.load();
    t.nativeYaw = g_nativeYaw.load();
    t.appliedYaw = g_appliedYaw.load();
    t.nativeHeight = g_nativeHeight.load();
    t.appliedHeight = g_appliedHeight.load();
    t.nativeArmLength = g_nativeDistance.load();
    t.appliedArmLength = g_appliedDistance.load();
    return t;
}
}
