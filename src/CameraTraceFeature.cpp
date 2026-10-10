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
std::atomic<float> g_idlePivotDX{0.0f},g_idlePivotDY{0.0f};
std::atomic_ullong g_idlePivotTick{0};
std::atomic_uint32_t g_idlePivotGeneration{UINT32_MAX};
std::atomic<uint32_t> g_pivotCompensations{0};
// V0.64: independent TPS FOV, player attachment and zone camera lockdown.
std::atomic<uint32_t> g_tpsAttachedFrames{0},g_zoneCameraIgnored{0},g_tpsAttachSkipped{0};
std::atomic<uint32_t> g_tpsFovOverrides{0};
std::atomic<uint32_t> g_tpsCameraRejected{0};
std::atomic_uint32_t g_zoneActorGeneration{UINT32_MAX};
std::atomic<float> g_zoneReferenceYaw{0.0f},g_zoneReferenceArm{0.0f};
std::atomic_bool g_zoneReferenceActive{false};
void ResetZoneReference() {
    g_zoneReferenceActive.store(false,std::memory_order_relaxed);
    g_zoneReferenceArm.store(0.0f,std::memory_order_relaxed);
}

void Write(const char* s) { if (g_log && s) g_log(s); }
bool Sample(uint32_t n) { return n <= 12 || (n % 5000) == 0; }

void HookView(void* camera, float dt, void* outView) {
    if (g_view) g_view(camera, dt, outView);
    auto& settings=dg::runtime::Get();
    // V0.66: do not cancel a valid gameplay frame if GetCameraView
    // is called again for a spectator / UI camera in the same frame.
    // The Present crosshair check already requires a fresh successful tick.
    if(!settings.thirdPersonEnabled.load(std::memory_order_relaxed))
        settings.tpsViewGameplay.store(false,std::memory_order_release);
    if (!outView) return;
    auto* p=static_cast<unsigned char*>(outView);
    float x=0,y=0,z=0,nativePitch=0,nativeYaw=0,nativeFov=0;
    std::memcpy(&x,p,4); std::memcpy(&y,p+4,4); std::memcpy(&z,p+8,4);
    std::memcpy(&nativePitch,p+12,4);std::memcpy(&nativeYaw,p+16,4);
    std::memcpy(&nativeFov,p+24,4);
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(z)||
       !std::isfinite(nativePitch)||!std::isfinite(nativeYaw)||
       !std::isfinite(nativeFov)||nativeFov<1||nativeFov>179)return;
    const bool thirdPerson=settings.thirdPersonEnabled.load(std::memory_order_relaxed);
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG actorTick=settings.tpsActorLocationTick.load(std::memory_order_acquire);
    const bool live=actorTick&&now>=actorTick&&now-actorTick<300ull;
    const float ax=settings.tpsActorWorldX.load(),ay=settings.tpsActorWorldY.load(),
                az=settings.tpsActorWorldZ.load();
    const float actorYaw=settings.tpsActorYawDegrees.load();
    const bool actorValid=live&&std::isfinite(ax)&&std::isfinite(ay)&&
        std::isfinite(az)&&std::isfinite(actorYaw)&&
        std::fabs(ax)<1e7f&&std::fabs(ay)<1e7f&&std::fabs(az)<1e7f;
    // Conservative gameplay/cutscene heuristic. Fail OPEN for unusual
    // cinematic views. We do not patch persistent camera volumes or sequences.
    const float nativeDx=x-ax,nativeDy=y-ay,nativeDz=z-az;
    const float planar=std::hypot(nativeDx,nativeDy);
    const bool nearPawn=actorValid&&planar<6500.0f&&std::fabs(nativeDz)<7500.0f;
    // V0.67: strict proximity again. The previous 32,477-unit 'gap'
    // came exclusively from READING THE WRONG root translation offset.
    // Never override far-away/cinematic native views based only on a
    // matching pitch/FOV signature.
    const bool normalAngles=nativePitch>=-89.0f&&nativePitch<=-15.0f;
    const bool gameplayCamera=actorValid&&normalAngles&&nearPawn;
    if(!thirdPerson)ResetZoneReference();
    const uint32_t generation=settings.tpsActorGeneration.load();
    if(g_zoneActorGeneration.load()!=generation){
        g_zoneActorGeneration.store(generation);
        ResetZoneReference();
    }
    // Never override cinematics, menu cameras, remote cutaway cameras.
    // Heuristic may miss an in-place cinematic with a top-down-like pose.
    if(thirdPerson&&!gameplayCamera){
        const unsigned n=++g_tpsAttachSkipped;
        ++g_tpsCameraRejected;
        if(n<=3||n==3000){
            char line[390]{};
            sprintf_s(line,"TPS V0.67: camera REJECT n=%u actorValid=%d fresh=%d near=%d native=(%.0f,%.0f,%.0f) actor=(%.0f,%.0f,%.0f) gapXY=%.1f gapZ=%.1f pitch=%.1f fov=%.1f",
                n,actorValid?1:0,live?1:0,nearPawn?1:0,
                x,y,z,ax,ay,az,planar,nativeDz,nativePitch,nativeFov);
            Write(line);
        }
        return;
    }
    float appliedFov=nativeFov,appliedPitch=nativePitch,appliedYaw=nativeYaw,appliedHeight=z;
    const bool tpsFov=thirdPerson&&settings.thirdPersonFovEnabled.load();
    if(tpsFov||settings.fovEnabled.load()){
        const float target=tpsFov?settings.thirdPersonFovDegrees.load():settings.fovDegrees.load();
        if(std::isfinite(target)&&target>=40&&target<=140){
            appliedFov=target;
            std::memcpy(p+24,&appliedFov,4);
            if(tpsFov){
                const unsigned n=++g_tpsFovOverrides;
                if(n==1||n==10000){
                    char line[128]{};
                    sprintf_s(line,"TPS V0.67: FOV override n=%u native=%.1f target=%.1f",n,nativeFov,target);
                    Write(line);
                }
            }
        }
    }
    const float pitchOffset=settings.cameraPitchDegrees.load();
    const float yawOffset=settings.cameraYawDegrees.load();
    const float heightOffset=settings.cameraHeightOffset.load();
    if(!thirdPerson){
        if(std::isfinite(pitchOffset)&&pitchOffset>=-35&&pitchOffset<=35&&pitchOffset!=0){
            appliedPitch=std::clamp(nativePitch+pitchOffset,-89.0f,89.0f);
            std::memcpy(p+12,&appliedPitch,4);
        }
        if(std::isfinite(yawOffset)&&yawOffset>=-180&&yawOffset<=180&&yawOffset!=0){
            appliedYaw=std::remainder(nativeYaw+yawOffset,360.0f);
            std::memcpy(p+16,&appliedYaw,4);
        }
        if(std::isfinite(heightOffset)&&heightOffset>=-1500&&heightOffset<=1500&&heightOffset!=0){
            appliedHeight=z+heightOffset;
            std::memcpy(p+8,&appliedHeight,4);
        }
    }else{
        const bool attached=settings.tpsFollowPlayer.load();
        const bool lockZone=attached&&settings.tpsLockZoneCamera.load();
        const float nativeArm=g_nativeDistance.load();
        // Native SpringArm may not be sampled on the frame where the player
        // enters TPS. A missing value must not disable the entire TPS camera.
        // The locked camera uses a one-time fallback, not repeated zone data.
        const bool validNativeArm=std::isfinite(nativeArm)&&nativeArm>=100&&nativeArm<=10000;
        const float usableArm=validNativeArm?nativeArm:900.0f;
        if(lockZone&&!g_zoneReferenceActive.load()){
            // Align camera axis with the character on TPS activation.
            // Orbit afterwards is independent of the character's own yaw.
            g_zoneReferenceYaw.store(std::remainder(actorYaw,360.0f));
            g_zoneReferenceArm.store(usableArm);
            g_zoneReferenceActive.store(true);
            char line[170]{};
            sprintf_s(line,"TPS V0.67: camera locked to actor axis gen=%u actorYaw=%.1f distance=%.1f",
                      generation,actorYaw,usableArm);
            Write(line);
        }
        const float arm=lockZone&&g_zoneReferenceActive.load()
             ?g_zoneReferenceArm.load():usableArm;
        const float baseYaw=lockZone&&g_zoneReferenceActive.load()
             ?g_zoneReferenceYaw.load():nativeYaw;
        const bool orbit=settings.cameraOrbitInputEnabled.load();
        const float oy=orbit?settings.cameraOrbitYawDegrees.load():0.0f;
        const float op=orbit?settings.cameraOrbitPitchDegrees.load():0.0f;
        const float basePitch=settings.thirdPersonPitchDegrees.load();
        const float mult=settings.thirdPersonDistanceMultiplier.load();
        const float tpHeight=settings.thirdPersonHeightOffset.load();
        const float foot=settings.tpsFootAnchorOffset.load();
        if(std::isfinite(arm)&&arm>=100&&arm<=10000&&
           std::isfinite(basePitch)&&std::isfinite(mult)&&std::isfinite(tpHeight)&&
           std::isfinite(oy)&&std::isfinite(op)&&std::isfinite(foot)&&
           std::isfinite(yawOffset)&&std::isfinite(heightOffset)&&
           mult>=0&&mult<=3&&foot>=0&&foot<=200&&std::fabs(tpHeight)<=500){
            constexpr float radians=0.01745329251994329577f;
            appliedPitch=std::clamp(basePitch+op,-75.0f,65.0f);
            appliedYaw=std::remainder(baseYaw+yawOffset+oy,360.0f);
            const float distance=std::clamp(arm*mult,0.0f,12000.0f);
            // Foot-level player origin. RootComponent may be at capsule
            // center, so configurable vertical correction is approximate
            // until capsule geometry has been natively verified.
            const float pivotX=attached?ax:x+std::cos(nativePitch*radians)*
                std::cos(nativeYaw*radians)*arm;
            const float pivotY=attached?ay:y+std::cos(nativePitch*radians)*
                std::sin(nativeYaw*radians)*arm;
            const float pivotZ=attached?(az-foot):z+std::sin(nativePitch*radians)*arm;
            const float pitchRad=appliedPitch*radians,yawRad=appliedYaw*radians;
            const float outX=pivotX-std::cos(pitchRad)*std::cos(yawRad)*distance;
            const float outY=pivotY-std::cos(pitchRad)*std::sin(yawRad)*distance;
            const float outZ=pivotZ-std::sin(pitchRad)*distance+tpHeight+heightOffset;
            if(std::isfinite(outX)&&std::isfinite(outY)&&std::isfinite(outZ)&&
               std::fabs(outX)<1e7f&&std::fabs(outY)<1e7f&&std::fabs(outZ)<1e7f){
                std::memcpy(p,&outX,4);std::memcpy(p+4,&outY,4);std::memcpy(p+8,&outZ,4);
                std::memcpy(p+12,&appliedPitch,4);std::memcpy(p+16,&appliedYaw,4);
                appliedHeight=outZ;
                if(attached){
                    const unsigned n=++g_tpsAttachedFrames;
                    if(n==1||n==5000){
                        char line[230]{};
                        sprintf_s(line,"TPS V0.67: foot camera attached n=%u root=(%.0f,%.0f,%.0f) footZ=%.0f camera=(%.0f,%.0f,%.0f) yaw=%.1f",
                            n,ax,ay,az,pivotZ,outX,outY,outZ,appliedYaw);
                        Write(line);
                    }
                    if(lockZone&&g_zoneReferenceActive.load()&&
                        std::fabs(std::remainder(nativeYaw-baseYaw,360.0f))>5){
                        const unsigned n=++g_zoneCameraIgnored;
                        if(n==1||n==10000){
                            char line[170]{};
                            sprintf_s(line,"TPS V0.67: native camera zone yaw ignored n=%u native=%.1f locked=%.1f",
                                n,nativeYaw,baseYaw);
                            Write(line);
                        }
                    }
                }
                settings.tpsGameplayViewTick.store(now,std::memory_order_release);
                settings.tpsViewGameplay.store(true,std::memory_order_release);
            }
        }
    }
    g_nativeFov.store(nativeFov);g_appliedFov.store(appliedFov);
    g_nativePitch.store(nativePitch);g_appliedPitch.store(appliedPitch);
    g_nativeYaw.store(nativeYaw);g_appliedYaw.store(appliedYaw);
    g_nativeHeight.store(z);g_appliedHeight.store(appliedHeight);
    const unsigned n=++g_viewCount;
    if(Sample(n)){
        char line[220]{};
        sprintf_s(line,"Camera V0.67: view n=%u nativeFOV=%.1f outputFOV=%.1f nativeYaw=%.1f outputYaw=%.1f tp=%d",
            n,nativeFov,appliedFov,nativeYaw,appliedYaw,thirdPerson?1:0);
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
    t.tpsAttachedFrames = g_tpsAttachedFrames.load();
    t.nativeZoneOverridesIgnored = g_zoneCameraIgnored.load();
    t.tpsAttachDeferred = g_tpsAttachSkipped.load();
    t.tpsFovSamples = g_tpsFovOverrides.load();
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
