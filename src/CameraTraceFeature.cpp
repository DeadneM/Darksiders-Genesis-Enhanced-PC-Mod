#include "CameraTraceFeature.h"
#include "LocalPlayerOwnership.h"
#include "CoopCameraFeature.h"
#include "CameraContinuity.h"
#include "TpsCameraFocus.h"
#include "NativeMemoryReader.h"
#include "DashCombatFeature.h"
#include "RuntimeSettings.h"
#include "NativeTpsFeature.h"
#include "ChaosPoseFeature.h"
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
std::atomic<float> g_nativeDistance{0.0f};
// V1.30: only the emitted TPS view; read-only projection telemetry.
std::atomic<float> g_tpsEmittedX{0.0f},g_tpsEmittedY{0.0f},g_tpsEmittedZ{0.0f};
std::atomic_ullong g_tpsEmittedTickMs{0};
SRWLOCK g_emittedLock=SRWLOCK_INIT;
AppliedView g_emittedView{};
// Values only: never retain a UObject for later dereferencing. The identity
// must match the current live callback and player generation before reuse.
struct HeldTpsView {
    uintptr_t camera=0;
    uint32_t generation=UINT32_MAX;
    float values[7]{};
    camera_focus::Point focus{};
    camera_focus::Bounds focusBounds{};
    bool focusValid=false;
    bool valid=false;
};
thread_local HeldTpsView g_heldView;
thread_local bool g_inArm = false;
// V0.64: independent TPS FOV, player attachment and zone camera lockdown.
std::atomic<uint32_t> g_tpsAttachedFrames{0},g_zoneCameraIgnored{0},g_tpsAttachSkipped{0};
std::atomic<uint32_t> g_tpsFovOverrides{0};
// V0.85: mounted-only firing camera stabilization telemetry. This is
// camera-output-only, never game actor / controller / weapon manipulation.
std::atomic_uint32_t g_mountedFireRequested{0};
std::atomic_uint32_t g_mountedFireHorseSelected{0};
std::atomic_uint32_t g_mountedFireOverrodeFoot{0};
std::atomic_uint32_t g_mountedFireHorseUnavailable{0};
// V1.27: separate actual mouse and native-pad mounted view observations.
std::atomic_uint32_t g_v127MountedMouseViewFrames{0};
std::atomic_uint32_t g_v127MountedMouseHorsePoseHeld{0};
std::atomic_uint32_t g_v127FacingFootSamples{0},g_v127FacingFootOffAxis{0},
    g_v127FacingHorseSamples{0},g_v127FacingHorseOffAxis{0};
std::atomic_ullong g_v127LastFacingLogTick{0};
std::atomic<float> g_zoneReferenceYaw{0.0f},g_zoneReferenceArm{0.0f};
std::atomic_bool g_zoneReferenceActive{false};
void ResetZoneReference() {
    g_zoneReferenceActive.store(false,std::memory_order_relaxed);
    g_zoneReferenceArm.store(0.0f,std::memory_order_relaxed);
}

void Write(const char* s) { if (g_log && s) g_log(s); }
bool Sample(uint32_t n) { return n <= 12 || (n % 5000) == 0; }

// Classification only. Do not alter or force the game's cinematic camera.
std::atomic_uint32_t g_cinematicEntered{0},g_cinematicExited{0},
    g_cinematicCandidateBegins{0},g_cinematicMissingActorSamples{0},
    g_cinematicFarViewSamples{0};

// V1.28 temporary scene-detection HISTORY. A ring of 24 *events* rather
// than repeated per-frame reports; stack copies only and a tiny SRW lock.
// Names of actual game sequences are unknown, so none are fabricated.
SRWLOCK g_cinematicDebugLock=SRWLOCK_INIT;
std::array<CinematicDebugEvent,kCinematicDebugHistory> g_cinematicEvents{};
CinematicDebugEvent g_cinematicLastView{};
uint32_t g_cinematicNext=0,g_cinematicCount=0,g_cinematicSerial=0;
bool g_cinematicHasLastView=false;
std::atomic_uint32_t g_cinematicActiveCandidateId{0};
std::atomic_ullong g_cinematicLastViewSampleTick{0};
void StoreLastCinematicNativeView(const CinematicDebugEvent& observation){
    const uint64_t previous=g_cinematicLastViewSampleTick.load(std::memory_order_relaxed);
    if(previous && observation.tickMs>=previous && observation.tickMs-previous<180ull)return;
    g_cinematicLastViewSampleTick.store(observation.tickMs,std::memory_order_relaxed);
    AcquireSRWLockExclusive(&g_cinematicDebugLock);
    g_cinematicLastView=observation;
    g_cinematicHasLastView=true;
    ReleaseSRWLockExclusive(&g_cinematicDebugLock);
}
void RecordCinematicEvent(CinematicDebugPhase phase,uint32_t candidate,
                          uint64_t duration,CinematicDebugEvent observation){
    observation.phase=phase;
    observation.candidateId=candidate;
    observation.durationMs=duration;
    AcquireSRWLockExclusive(&g_cinematicDebugLock);
    observation.serial=++g_cinematicSerial;
    g_cinematicEvents[g_cinematicNext]=observation;
    g_cinematicNext=(g_cinematicNext+1)%kCinematicDebugHistory;
    if(g_cinematicCount<kCinematicDebugHistory)++g_cinematicCount;
    ReleaseSRWLockExclusive(&g_cinematicDebugLock);
    // Emit bounded event-level diagnostics; no per-frame logs.
    if(observation.serial<=120u || (observation.serial%100u)==0u){
        char line[360]{};
        const char* phaseName=CinematicDebugPhaseLabel(phase);
        const char* reason=observation.noActorStatic?"NO_ACTOR_STATIC_MENU_POSSIBLE":
            !observation.actorValid?"NO_ACTOR_MOVING_CAMERA":
            observation.distanceExceeded?"DISTANCE_OVER_TPS":
            !observation.nearPawn?"FAR_OR_HEIGHT":
            !observation.normalAngles?"PITCH_OUTSIDE_GAMEPLAY":"NORMAL_GAMEPLAY_SHAPE";
        sprintf_s(line,"TPS V1.31 CINEMATIC_DEBUG event=%u candidate=%u phase=%s reason=%s durationMs=%llu actorValid=%d near=%d angles=%d auto=%d gap3D=%.0f baseline=%.0f tpsDistance=%.0f threshold=%.0f nativePitch=%.1f nativeYaw=%.1f nativeFov=%.1f nativeViewShift=%.0f",
            observation.serial,candidate,phaseName,reason,
            static_cast<unsigned long long>(duration),
            observation.actorValid?1:0,observation.nearPawn?1:0,
            observation.normalAngles?1:0,observation.autoSuspend?1:0,
            observation.gap3D,observation.distanceBaseline,
            observation.selectedTpsDistance,observation.candidateDistanceThreshold,
            observation.nativePitch,observation.nativeYaw,observation.nativeFov,
            observation.nativeViewShift);
        Write(line);
    }
}
void HookView(void* camera, float dt, void* outView) {
    if (g_view) g_view(camera, dt, outView);
    auto& settings=dg::runtime::Get();
    dg::native_tps::SampleNativeStickSwap();
    dg::local_players::ObserveCameraViews(camera);
    // A cinematic may switch to a camera with no player component at all.
    // Mirror the native lifecycle before the player-camera ownership filter.
    if(dg::native_tps::TargetReady()){
        const bool cinema=settings.thirdPersonEnabled.load()&&
            !settings.tpsTemporarilySuspended.load()&&
            settings.tpsCutsceneAutoSuspend.load()&&dg::native_tps::CinematicActive();
        settings.tpsCutsceneSuspended.store(cinema,std::memory_order_release);
        if(cinema){
            settings.tpsViewGameplay.store(false,std::memory_order_release);
            settings.tpsMountedViewActive.store(false,std::memory_order_release);
        }
    }
    dg::local_players::Context viewOwner{};
    const bool ownedView=dg::local_players::ResolveCamera(camera,viewOwner);
    if(ownedView){
        dg::local_players::ObserveGameplay(viewOwner);
        // Runs in both native/TPS views even when an ended aim ability no
        // longer reads look input. Outside native movement/ability ticks.
        dg::dash_combat::OnLivePlayer(reinterpret_cast<void*>(viewOwner.character));
    }
    if(settings.thirdPersonEnabled.load()){
        // Coop requires a proved player component or manager-owned fallback
        // camera. Unknown solo camera paths retain the validated fallback.
        if(!ownedView&&dg::local_players::CoopActive())return;
        if(ownedView&&viewOwner.index==1){dg::coop_camera::ApplyView(camera,outView,viewOwner);return;}
        if(ownedView&&viewOwner.kind==dg::local_players::PoseKind::Horse){
            dg::local_players::Pose horse{};
            if(dg::local_players::ReadPose(viewOwner,horse)){
                settings.tpsHorseWorldX.store(horse.x);settings.tpsHorseWorldY.store(horse.y);
                settings.tpsHorseWorldZ.store(horse.z);settings.tpsHorseYawDegrees.store(horse.yaw);
                settings.tpsHorseOwnerIdentity.store(viewOwner.pawn);
                settings.tpsHorsePoseTick.store(GetTickCount64(),std::memory_order_release);
            }
        }
    }
    // V0.66: do not cancel a valid gameplay frame if GetCameraView
    // is called again for a spectator / UI camera in the same frame.
    // The Present crosshair check already requires a fresh successful tick.
    if(!settings.thirdPersonEnabled.load(std::memory_order_relaxed) ||
       settings.tpsTemporarilySuspended.load(std::memory_order_relaxed)){
        settings.tpsViewGameplay.store(false,std::memory_order_release);
        settings.tpsMountedViewActive.store(false,std::memory_order_release);
    }
    if (!outView) return;
    auto* p=static_cast<unsigned char*>(outView);
    float x=0,y=0,z=0,nativePitch=0,nativeYaw=0,nativeFov=0;
    std::memcpy(&x,p,4); std::memcpy(&y,p+4,4); std::memcpy(&z,p+8,4);
    std::memcpy(&nativePitch,p+12,4);std::memcpy(&nativeYaw,p+16,4);
    std::memcpy(&nativeFov,p+24,4);
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(z)||
       !std::isfinite(nativePitch)||!std::isfinite(nativeYaw)||
       !std::isfinite(nativeFov)||nativeFov<1||nativeFov>179)return;
    const bool thirdPerson=settings.thirdPersonEnabled.load(std::memory_order_relaxed) &&
        !settings.tpsTemporarilySuspended.load(std::memory_order_relaxed);
    const ULONGLONG now=GetTickCount64();
    const uint32_t liveGeneration=settings.tpsActorGeneration.load();
    const bool nativeCinema=dg::native_tps::CinematicActive();
    const bool nativePause=dg::native_tps::IsWorldPaused(camera);
    const auto wheelTick=settings.tpsAbilityWheelTick.load();
    const bool wheelInput=settings.tpsAbilityWheelHeld.load()&&wheelTick&&
        now>=wheelTick&&now-wheelTick<250ull;
    const bool nativeRadialObserved=dg::native_tps::RadialActive();
    const bool cinematicOwns=thirdPerson&&nativeCinema&&
        settings.tpsCutsceneAutoSuspend.load();
    static thread_local bool previousNativeCinema=false;
    if(cinematicOwns!=previousNativeCinema){
        previousNativeCinema=cinematicOwns;
        settings.tpsCutsceneSuspended.store(cinematicOwns,std::memory_order_release);
        Write(cinematicOwns?"TPS V1.32 CAMERA_OWNER=NATIVE_CINEMATIC":"TPS V1.32 CAMERA_OWNER=RETURN_FROM_NATIVE_CINEMATIC");
    }
    if(cinematicOwns){
        settings.tpsUiViewHeld.store(false,std::memory_order_release);
        settings.tpsViewGameplay.store(false,std::memory_order_release);
        settings.tpsMountedViewActive.store(false,std::memory_order_release);
        return; // original native view already produced by g_view
    }
    const bool uiHold=thirdPerson&&nativePause&&
        g_heldView.valid&&g_heldView.generation==liveGeneration&&
        g_heldView.camera==reinterpret_cast<uintptr_t>(camera);
    const bool priorHold=settings.tpsUiViewHeld.exchange(uiHold,std::memory_order_acq_rel);
    if(priorHold!=uiHold){
        char line[180]{};
        sprintf_s(line,"TPS V1.34 CAMERA_OWNER=%s worldPaused=%d nativeRadial=%d temporaryNative=%d",
            uiHold?"TPS_UI_HOLD":"GAMEPLAY_OR_EXPLICIT_NATIVE",nativePause?1:0,
            nativeRadialObserved?1:0,settings.tpsTemporarilySuspended.load()?1:0);
        Write(line);
    }
    if(uiHold){
        // Keep the last rendered TPS composition, including during stopped
        // actor callbacks. No fake actor timestamps, UI aim or synthetic input.
        std::memcpy(p,g_heldView.values,sizeof(g_heldView.values));
        if(g_heldView.focusValid)camera_focus::Compensate(p,g_heldView.focus,settings.tpsLensFocusEnabled.load(),g_heldView.focusBounds);
        camera_focus::ApplyBlur(p,settings.tpsBlur.Get(),dg::native_tps::CinematicActive(),g_heldView.focusValid?&g_heldView.focus:nullptr,g_heldView.focusBounds);
        settings.tpsViewGameplay.store(false,std::memory_order_release);
        settings.tpsMountedViewActive.store(false,std::memory_order_release);
        return;
    }
    const ULONGLONG actorTick=settings.tpsActorLocationTick.load(std::memory_order_acquire);
    const bool live=actorTick&&now>=actorTick&&now-actorTick<300ull;
    float ax=settings.tpsActorWorldX.load(),ay=settings.tpsActorWorldY.load(),
          az=settings.tpsActorWorldZ.load();
    float actorYaw=settings.tpsActorYawDegrees.load();
    bool actorValid=live&&std::isfinite(ax)&&std::isfinite(ay)&&
        std::isfinite(az)&&std::isfinite(actorYaw)&&
        std::fabs(ax)<1e7f&&std::fabs(ay)<1e7f&&std::fabs(az)<1e7f;
    // V0.70: on horseback GetMaxSpeed of the on-foot player no longer
    // runs. Use ONLY freshly sampled HorseCharacter poses, read during
    // native horse callbacks; never dereference cached horse pointers.
    // V0.76: when the existing native horse pose has ALREADY been used,
    // retain that validated source during shots instead of briefly switching
    // to an on-foot pose updated on a mount animation frame. No cached
    // UObject is dereferenced and unmounted camera behavior is untouched.
    static std::atomic_bool mountedAimPoseLatched{false};
    static std::atomic_bool lastHorse{false};
    // V0.86: the mode of mounted shooting is ALWAYS enforced when in TPS.
    // View ownership is independent of the mounted aiming mode.
    // V1.27: mouse shots also hold a VERIFIED recent horse camera pose,
    // just as controller fire has since V0.85. The physical mouse state is
    // published separately; this does not alter native projectile raycasts.
    const ULONGLONG mouseAt=settings.tpsMouseFireTick.load(std::memory_order_acquire);
    const bool mouseMountedFire=thirdPerson &&
        settings.tpsMouseFireHeld.load(std::memory_order_acquire) &&
        mouseAt && now>=mouseAt && now-mouseAt<180ull &&
        settings.tpsMountedViewActive.load(std::memory_order_acquire);
    const bool activeHorseAim=thirdPerson &&
        (settings.tpsAimActive.load(std::memory_order_acquire)||mouseMountedFire);
    if(mouseMountedFire)++g_v127MountedMouseViewFrames;
    if(activeHorseAim){
        ++g_mountedFireRequested;
        // The previous camera frame must have really used a fresh horse
        // pose before allowing the extra override of a transient foot pose.
        if(lastHorse.load(std::memory_order_relaxed))
            mountedAimPoseLatched.store(true,std::memory_order_relaxed);
    }else{
        mountedAimPoseLatched.store(false,std::memory_order_relaxed);
    }
    bool usingHorse=false;
    const bool holdHorsePose=activeHorseAim &&
        mountedAimPoseLatched.load(std::memory_order_relaxed);
    const bool footValidBeforeHorse=actorValid;
    const ULONGLONG horseTick=settings.tpsHorsePoseTick.load(std::memory_order_acquire);
    const float hx=settings.tpsHorseWorldX.load(),hy=settings.tpsHorseWorldY.load(),
                hz=settings.tpsHorseWorldZ.load();
    const float hyaw=settings.tpsHorseYawDegrees.load();
    const bool freshHorse=thirdPerson&&horseTick&&now>=horseTick&&
        now-horseTick<750ull&&std::isfinite(hx)&&std::isfinite(hy)&&
        std::isfinite(hz)&&std::isfinite(hyaw)&&
        std::fabs(hx)<1.0e7f&&std::fabs(hy)<1.0e7f&&
        std::fabs(hz)<1.0e7f;
    // V0.85: never invalidate a good on-foot pose merely because the
    // previous horse was active. In particular, a stale horse pose must
    // NOT cause camera rejection or a frozen view after dismounting.
    if(freshHorse&&(!actorValid||holdHorsePose)){
        ax=hx;ay=hy;az=hz;actorYaw=hyaw;
        actorValid=true;usingHorse=true;
        if(activeHorseAim){
            mountedAimPoseLatched.store(true,std::memory_order_relaxed);
            if(footValidBeforeHorse){
                const unsigned n=++g_mountedFireOverrodeFoot;
                if(n==1||n==100||n==1000){
                    char line[210]{};
                    sprintf_s(line,"TPS V0.85 MOUNTED AIM: ignored transient FOOT pose while firing n=%u horse=(%.0f,%.0f,%.0f)",
                        n,hx,hy,hz);
                    Write(line);
                }
            }
        }
    }else if(activeHorseAim){
        ++g_mountedFireHorseUnavailable;
        // No proof of a live horse: keep original foot camera unchanged.
    }
    dg::chaos_pose::Pose chaosPose{};
    const bool chaosValid=dg::chaos_pose::Sample(camera,chaosPose);
    const bool usingChaos=thirdPerson&&(ownedView?
        viewOwner.kind==dg::local_players::PoseKind::Ultimate:chaosValid);
    if(usingChaos&&chaosValid){
        // Fresh native form pose for THIS camera callback. Keep the normal
        // player's identity/generation and all horse sampling unchanged.
        ax=chaosPose.x;ay=chaosPose.y;az=chaosPose.z;actorYaw=chaosPose.yaw;
        actorValid=true;usingHorse=false;
    }
    if(ownedView){
        dg::local_players::Pose pose{};
        if(dg::local_players::ReadPose(viewOwner,pose)){
            ax=pose.x;ay=pose.y;az=pose.z;actorYaw=pose.yaw;actorValid=true;
            usingHorse=viewOwner.kind==dg::local_players::PoseKind::Horse;
        }
    }
    if(thirdPerson&&actorValid){
        if(lastHorse.exchange(usingHorse)!=usingHorse){
            char message[180]{};
            sprintf_s(message,"TPS V0.70: camera pose source = %s at (%.0f,%.0f,%.0f)",
                      usingHorse?"HORSE":"FOOT",ax,ay,az);
            Write(message);
            // V0.88: Observe a previously LIVE mounted TPS camera source
            // falling back to FOOT. It may be voluntary dismount, forced
            // no-mount volume, teleport or cinematic: never assume which.
            const auto previousHorse=!usingHorse;
            if(previousHorse){
                const ULONGLONG horseAge=horseTick&&now>=horseTick?
                    now-horseTick:0ull;
                const auto inputTick=settings.tpsLastHorseSummonInputTick.load(
                    std::memory_order_acquire);
                const ULONGLONG inputAge=inputTick&&now>=inputTick?
                    now-inputTick:999999ull;
                char zoneMsg[384]{};
                sprintf_s(zoneMsg,"Horse Everywhere V0.88 MOUNT EXIT: HORSE->FOOT player=(%.0f,%.0f,%.0f) lastHorsePoseAgeMs=%llu latestMountKeyAgeMs=%llu noRecentMountKey=%d configured=%d (possible forced-removal volume if no recent key; not proof)",
                    ax,ay,az,static_cast<unsigned long long>(horseAge),
                    static_cast<unsigned long long>(inputAge),
                    inputAge>2500ull?1:0,settings.horseEverywhereEnabled.load()?1:0);
                Write(zoneMsg);
            }
        }
    }
    // Conservative gameplay/cutscene heuristic. Fail OPEN for unusual
    // cinematic views. We do not patch persistent camera volumes or sequences.
    const float nativeDx=x-ax,nativeDy=y-ay,nativeDz=z-az;
    const float planar=std::hypot(nativeDx,nativeDy);
    const float gap3D=std::hypot(planar,nativeDz);
    const bool normalAngles=nativePitch>=-89.0f&&nativePitch<=-15.0f;
    const bool oldNearPawn=actorValid&&planar<6500.0f&&std::fabs(nativeDz)<7500.0f;
    // V1.29: distinguish native isometric camera offset from the user
    // selected TPS camera distance. Vanilla view is still ~2130 UE units
    // away when the TPS multiplier is 0.15 (~320 units). Comparing native
    // gap directly to 320 would misclassify *every* normal gameplay frame!
    // Instead classify a camera moving FURTHER THAN normal vanilla gap
    // BY the chosen TPS camera distance, plus a small anti-jitter guard.
    // Keep the previously validated hard 6500/7500 rejection as a failsafe.
    const bool autoSuspend=settings.tpsCutsceneAutoSuspend.load(std::memory_order_relaxed);
    const uint32_t currentGeneration=settings.tpsActorGeneration.load(std::memory_order_acquire);
    const float candidateNativeArm=g_nativeDistance.load(std::memory_order_relaxed);
    const bool nativeArmValid=std::isfinite(candidateNativeArm) &&
        candidateNativeArm>=100.0f && candidateNativeArm<=10000.0f;
    const float multiplier=usingChaos?settings.tpsUltimateDistanceMultiplier.load(std::memory_order_relaxed):
        usingHorse?settings.tpsHorseDistanceMultiplier.load(std::memory_order_relaxed):
        settings.thirdPersonDistanceMultiplier.load(std::memory_order_relaxed);
    const float selectedTpsDistance=std::isfinite(multiplier)&&multiplier>=0&&multiplier<=3
        ?std::clamp((nativeArmValid?candidateNativeArm:900.0f)*multiplier,0.0f,12000.0f)
        :0.0f;
    static uint32_t distanceBaselineGeneration=UINT32_MAX;
    static bool distanceBaselineHorse=false;
    static float distanceBaseline3D=0.0f;
    static unsigned distanceBaselineSamples=0;
    if(distanceBaselineGeneration!=currentGeneration ||
       distanceBaselineHorse!=usingHorse){
        distanceBaselineGeneration=currentGeneration;
        distanceBaselineHorse=usingHorse;
        distanceBaseline3D=0.0f;
        distanceBaselineSamples=0;
    }
    if(thirdPerson&&actorValid&&normalAngles&&oldNearPawn &&
       std::isfinite(gap3D)&&gap3D>=100.0f && gap3D<6500.0f &&
       distanceBaseline3D==0.0f){
        // First trusted native snapshot: start with an observed offset,
        // never with an invented TPS-camera distance. A scene that starts
        // on this very frame may still need authoritative native sequence
        // detection later.
        distanceBaseline3D=gap3D;
        distanceBaselineSamples=1;
    }
    // V1.30: user's selected TPS distance is the primary separation
    // threshold beyond the calibrated NORMAL native isometric camera gap.
    // A small 40-unit hysteresis is enough to reject idle springarm jitter;
    // no unrelated 300-unit minimum +100 padding from V1.29.
    const float extraAllowed=selectedTpsDistance+40.0f;
    const float candidateDistanceThreshold=distanceBaseline3D>0.0f
        ?distanceBaseline3D+extraAllowed:6500.0f;
    const bool distanceExceeded=thirdPerson&&autoSuspend&&
        !dg::native_tps::TargetReady()&&actorValid&&
        distanceBaseline3D>0.0f && oldNearPawn &&
        gap3D>candidateDistanceThreshold;
    const bool nearPawn=oldNearPawn&&!distanceExceeded;
    if(thirdPerson&&autoSuspend&&actorValid&&normalAngles&&oldNearPawn&&
       !distanceExceeded&&!settings.tpsCutsceneSuspended.load(
           std::memory_order_acquire) && distanceBaseline3D>0.0f){
        // Extremely slow learning only on accepted gameplay-shaped views;
        // ensures camera-spring wobble does not continually move the limit.
        distanceBaseline3D=std::clamp(
            0.9975f*distanceBaseline3D+0.0025f*gap3D,
            100.0f,6500.0f);
        ++distanceBaselineSamples;
    }
    // V0.67: strict proximity again. The previous 32,477-unit 'gap'
    // came exclusively from READING THE WRONG root translation offset.
    // Never override far-away/cinematic native views based only on a
    // matching pitch/FOV signature.
    // V0.98: the old speculative camera-wheel continuity override was
    // unvalidated and did not prevent the native aerial view on wheels.
    // Keep all validated native LB/R3 right-stick input handling in the
    // controller hooks; let the game's actual radial camera render.
    const auto lastTPS=settings.tpsGameplayViewTick.load(std::memory_order_acquire);
    // V1.02: no mouse-specific camera continuity bypass.
    const bool sameTpsCamera=g_heldView.valid&&
        g_heldView.generation==liveGeneration&&
        g_heldView.camera==reinterpret_cast<uintptr_t>(camera);
    if(thirdPerson&&wheelInput&&sameTpsCamera&&!actorValid){
        std::memcpy(p,g_heldView.values,sizeof(g_heldView.values));
        if(g_heldView.focusValid)camera_focus::Compensate(p,g_heldView.focus,settings.tpsLensFocusEnabled.load(),g_heldView.focusBounds);
        camera_focus::ApplyBlur(p,settings.tpsBlur.Get(),dg::native_tps::CinematicActive(),g_heldView.focusValid?&g_heldView.focus:nullptr,g_heldView.focusBounds);
        settings.tpsViewGameplay.store(false,std::memory_order_release);
        settings.tpsMountedViewActive.store(false,std::memory_order_release);
        return; // stopped pose callbacks only; no live follow/orbit is frozen
    }
    const bool gameplayCamera=actorValid&&(ownedView||(normalAngles&&nearPawn)||
        (wheelInput&&sameTpsCamera)||usingChaos);
    // V1.31: actor visibility is NOT a reliable pause/cutscene signal.
    // Compare the actual native camera against the LAST VERIFIED normal
    // native view. A static camera with no player is likely pause/menu,
    // while a large native jump is a short cutscene/level camera change.
    // Do not guess the engine's paused flag or change the game's pause.
    static bool hasLastGameplayNativeView=false;
    static float lastGameplayCamX=0.0f,lastGameplayCamY=0.0f,
        lastGameplayCamZ=0.0f,lastGameplayCamPitch=0.0f,
        lastGameplayCamFov=0.0f;
    const float nativeViewShift=hasLastGameplayNativeView?
        std::hypot(std::hypot(x-lastGameplayCamX,y-lastGameplayCamY),
                    z-lastGameplayCamZ):0.0f;
    const bool nativeViewPoseChanged=hasLastGameplayNativeView&&(
        std::fabs(nativePitch-lastGameplayCamPitch)>12.0f ||
        std::fabs(nativeFov-lastGameplayCamFov)>8.0f);
    const bool missingActorChangedView=!actorValid&&
        hasLastGameplayNativeView&&
        (nativeViewShift>selectedTpsDistance+40.0f||nativeViewPoseChanged);
    const bool staticMissingActor=!actorValid&&hasLastGameplayNativeView&&
        !missingActorChangedView;
    if(gameplayCamera){
        lastGameplayCamX=x;
        lastGameplayCamY=y;
        lastGameplayCamZ=z;
        lastGameplayCamPitch=nativePitch;
        lastGameplayCamFov=nativeFov;
        hasLastGameplayNativeView=true;
    }
    // Current native view is captured for the UI's manual "missed scene"
    // marker, including if the automatic heuristic thinks it is gameplay.
    CinematicDebugEvent viewSample{};
    viewSample.tickMs=now;
    viewSample.actorValid=actorValid;
    viewSample.nearPawn=nearPawn;
    viewSample.normalAngles=normalAngles;
    viewSample.gameplayCamera=gameplayCamera;
    viewSample.autoSuspend=settings.tpsCutsceneAutoSuspend.load(std::memory_order_relaxed);
    viewSample.gapXY=planar;
    viewSample.gapZ=nativeDz;
    viewSample.gap3D=gap3D;
    viewSample.distanceBaseline=distanceBaseline3D;
    viewSample.selectedTpsDistance=selectedTpsDistance;
    viewSample.candidateDistanceThreshold=candidateDistanceThreshold;
    viewSample.distanceExceeded=distanceExceeded;
    viewSample.noActorStatic=staticMissingActor;
    viewSample.nativeViewShift=nativeViewShift;
    viewSample.nativePitch=nativePitch;
    viewSample.nativeYaw=nativeYaw;
    viewSample.nativeFov=nativeFov;
    StoreLastCinematicNativeView(viewSample);
    // V0.97 automatic cutscene suspension: the engine's own untouched
    // camera view is the source of truth, not a fabricated cutscene flag.
    // Require a PREVIOUS valid TPS view, continuous rejection >= 500 ms,
    // and only treat native camera changes in a currently running TPS game
    // as cinematic candidates. A valid player camera clears the suspension
    // immediately; turning the option OFF keeps the old V0.96 heuristic.
    static ULONGLONG cinematicCandidateStart=0;
    static uint32_t cinematicCandidateId=0;
    static bool cinematicCandidateLargeJump=false;
    static unsigned cinematicTransitions=0;
    static unsigned staticMenuLikeSamples=0;
    if(thirdPerson&&!gameplayCamera){
        if(!actorValid)++g_cinematicMissingActorSamples;
        else if(!nearPawn)++g_cinematicFarViewSamples;
    }
    // Native lifecycle supersedes the geometric 500ms classifier. Retain
    // the old fallback only if the guarded native module could not install.
    const bool autoCutscene=autoSuspend&&!dg::native_tps::TargetReady();
    if(!thirdPerson||!autoCutscene){
        if(cinematicCandidateStart){
            RecordCinematicEvent(CinematicDebugPhase::Disabled,cinematicCandidateId,
                now>=cinematicCandidateStart?now-cinematicCandidateStart:0ull,viewSample);
        }
        cinematicCandidateStart=0;
        cinematicCandidateId=0;
        cinematicCandidateLargeJump=false;
        g_cinematicActiveCandidateId.store(0,std::memory_order_release);
        settings.tpsCutsceneSuspended.store(false,std::memory_order_release);
    }else if(gameplayCamera){
        if(cinematicCandidateStart){
            const bool wasSuspended=settings.tpsCutsceneSuspended.load(
                std::memory_order_relaxed);
            RecordCinematicEvent(
                wasSuspended?CinematicDebugPhase::Returned:
                    cinematicCandidateLargeJump?CinematicDebugPhase::BriefCut:
                    CinematicDebugPhase::Rejected,
                cinematicCandidateId,
                now>=cinematicCandidateStart?now-cinematicCandidateStart:0ull,viewSample);
        }
        cinematicCandidateStart=0;
        cinematicCandidateId=0;
        cinematicCandidateLargeJump=false;
        g_cinematicActiveCandidateId.store(0,std::memory_order_release);
        if(settings.tpsCutsceneSuspended.exchange(false,std::memory_order_acq_rel)){
            ++g_cinematicExited;
            if(++cinematicTransitions<=20){
                char line[240]{};
                sprintf_s(line,"TPS V1.20 CINEMATIC_EXIT #%u actorValid=%d closeToPawn=%d nativePitch=%.1f nativeFov=%.1f",
                    cinematicTransitions,actorValid?1:0,nearPawn?1:0,nativePitch,nativeFov);
                Write(line);
            }
        }
    }else if(staticMissingActor){
        // A paused menu may stop publishing the local actor for MANY seconds
        // while the native camera itself remains entirely unchanged. Do
        // NOT call this an active cinematic solely due to actor absence.
        if(!settings.tpsCutsceneSuspended.load(std::memory_order_acquire)){
            cinematicCandidateStart=0;
            cinematicCandidateId=0;
            cinematicCandidateLargeJump=false;
            g_cinematicActiveCandidateId.store(0,std::memory_order_release);
        }
        const unsigned n=++staticMenuLikeSamples;
        if(n<=3 || n==500 || n==3000)
            Write("TPS V1.31 STATIC_NO_ACTOR camera stays near native gameplay pose; no cinematic candidate (menu/pause possible, unverified)");
    }else{
        // Only reject truly changed native camera views. Loss of actor
        // by itself must not be considered proof of a cinematic.
        const bool observedTPS=lastTPS&&now>=lastTPS&&now-lastTPS<3500ull;
        if(observedTPS){
            if(!cinematicCandidateStart){
                cinematicCandidateStart=now;
                const unsigned candidate=++g_cinematicCandidateBegins;
                cinematicCandidateId=candidate;
                cinematicCandidateLargeJump=nativeViewShift>
                    std::max(1500.0f,selectedTpsDistance*3.0f);
                g_cinematicActiveCandidateId.store(candidate,std::memory_order_release);
                RecordCinematicEvent(CinematicDebugPhase::Candidate,candidate,0,viewSample);
                if(candidate<=24){
                    char line[330]{};
                    sprintf_s(line,"TPS V1.31 CAMERA_TRANSITION candidate=%u actorValid=%d normalAngles=%d nearPawn=%d reason=%s nativeShift=%.0f largeJump=%d gap3D=%.0f vanillaBaseline=%.0f selectedTPSDistance=%.0f candidateThreshold=%.0f nativePitch=%.1f nativeFov=%.1f",
                        candidate,actorValid?1:0,normalAngles?1:0,
                        nearPawn?1:0,distanceExceeded?"TPS_DISTANCE":
                            !actorValid?"ACTOR_MISSING_CAMERA_MOVED":"NATIVE_CAMERA_SHAPE",
                        nativeViewShift,cinematicCandidateLargeJump?1:0,
                        gap3D,distanceBaseline3D,selectedTpsDistance,
                        candidateDistanceThreshold,nativePitch,nativeFov);
                    Write(line);
                }
            }
            if(now>=cinematicCandidateStart&&now-cinematicCandidateStart>=500ull &&
               !settings.tpsCutsceneSuspended.exchange(true,std::memory_order_acq_rel)){
                ++g_cinematicEntered;
                RecordCinematicEvent(CinematicDebugPhase::Detected,cinematicCandidateId,
                    now-cinematicCandidateStart,viewSample);
                if(++cinematicTransitions<=20){
                    char line[256]{};
                    sprintf_s(line,"TPS V1.25 CINEMATIC_ENTER #%u actorValid=%d nearPawn=%d normalAngles=%d nativePitch=%.1f nativeFov=%.1f gapXY=%.0f durationMs=%llu (heuristic)",
                        cinematicTransitions,actorValid?1:0,nearPawn?1:0,normalAngles?1:0,
                        nativePitch,nativeFov,planar,static_cast<unsigned long long>(now-cinematicCandidateStart));
                    Write(line);
                }
            }
        }else if(!settings.tpsCutsceneSuspended.load(std::memory_order_acquire)){
            if(cinematicCandidateStart){
                RecordCinematicEvent(CinematicDebugPhase::Rejected,cinematicCandidateId,
                    now>=cinematicCandidateStart?now-cinematicCandidateStart:0ull,viewSample);
            }
            cinematicCandidateStart=0;
            cinematicCandidateId=0;
            cinematicCandidateLargeJump=false;
            g_cinematicActiveCandidateId.store(0,std::memory_order_release);
        }
    }
    // Temporary R3 vanilla view must not discard the previously chosen TPS orbit basis.
    // Only F5 / true TPS OFF resets the reference.
    if(!settings.thirdPersonEnabled.load(std::memory_order_relaxed))ResetZoneReference();
    const uint32_t generation=settings.tpsActorGeneration.load();
    static dg::camera_continuity::ReferenceTracker referenceIdentity{};
    dg::camera_continuity::Identity identity{};
    const bool ownedPrimary=ownedView&&viewOwner.index==0;
    if(ownedPrimary){
        unsigned char type=255;
        std::memcpy(&type,reinterpret_cast<unsigned char*>(viewOwner.character)+0xC18,1);
        identity={viewOwner.localPlayer,viewOwner.world,viewOwner.character,type};
    }
    const auto referenceChange=referenceIdentity.Observe(ownedPrimary?&identity:nullptr,generation,
        dg::local_players::Ready());
    if(referenceChange==dg::camera_continuity::ReferenceChange::Reset)ResetZoneReference();
    else if(referenceChange==dg::camera_continuity::ReferenceChange::KeepCharacterSwap){
        static unsigned swapRecords=0;
        if(++swapRecords<=24){char line[200]{};
            sprintf_s(line,"TPS V1.47 CHARACTER_SWAP_KEEP player=1 type=%u generation=%u basisReady=%d yaw=%.2f; native actor rotation unchanged",
                unsigned(identity.type),generation,g_zoneReferenceActive.load()?1:0,
                std::remainder(g_zoneReferenceYaw.load()+settings.cameraOrbitYawDegrees.load(),360.0f));Write(line);}
    }
    // Never override cinematics, menu cameras, remote cutaway cameras.
    // Heuristic may miss an in-place cinematic with a top-down-like pose.
    if(thirdPerson&&!gameplayCamera){
        const unsigned n=++g_tpsAttachSkipped;
        if(n<=3||n==3000){
            char line[390]{};
            sprintf_s(line,"TPS V0.68: camera REJECT n=%u actorValid=%d fresh=%d near=%d native=(%.0f,%.0f,%.0f) actor=(%.0f,%.0f,%.0f) gapXY=%.1f gapZ=%.1f pitch=%.1f fov=%.1f",
                n,actorValid?1:0,live?1:0,nearPawn?1:0,
                x,y,z,ax,ay,az,planar,nativeDz,nativePitch,nativeFov);
            Write(line);
        }
        return;
    }
    float appliedFov=nativeFov,appliedPitch=nativePitch,appliedYaw=nativeYaw,appliedHeight=z;
    const bool tpsFov=thirdPerson&&settings.thirdPersonFovEnabled.load();
    if(tpsFov||(!thirdPerson&&settings.fovEnabled.load())){
        const float target=tpsFov?(local_players::CoopActive()?
            settings.thirdPersonCoopFovDegrees.load():settings.thirdPersonFovDegrees.load()):settings.fovDegrees.load();
        if(std::isfinite(target)&&target>=40&&target<=140){
            appliedFov=target;
            std::memcpy(p+24,&appliedFov,4);
            if(tpsFov){
                const unsigned n=++g_tpsFovOverrides;
                if(n==1||n==10000){
                    char line[128]{};
                    sprintf_s(line,"TPS V0.68: FOV override n=%u native=%.1f target=%.1f",n,nativeFov,target);
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
        constexpr bool attached=true; // permanently follow validated live TPS player.
        // Single standalone TPS camera. No obsolete "ignore zone" toggle:
        // its player-yaw reference is naturally stable once acquired.
        const bool lockZone=attached;
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
            sprintf_s(line,"TPS V0.68: camera locked to actor axis gen=%u actorYaw=%.1f distance=%.1f",
                      generation,actorYaw,usableArm);
            Write(line);
        }
        const float arm=lockZone&&g_zoneReferenceActive.load()
             ?g_zoneReferenceArm.load():usableArm;
        const float baseYaw=lockZone&&g_zoneReferenceActive.load()
             ?g_zoneReferenceYaw.load():nativeYaw;
        // Camera joystick selection governs orbit on FOOT and HORSE.
        // Mounted firing policy is independent of this camera input option.
        const bool orbit=settings.cameraOrbitInputEnabled.load();
        const float oy=orbit?settings.cameraOrbitYawDegrees.load():0.0f;
        const float op=orbit?settings.cameraOrbitPitchDegrees.load():0.0f;
        const float basePitch=settings.thirdPersonPitchDegrees.load();
        const float mult=usingChaos?settings.tpsUltimateDistanceMultiplier.load():
            usingHorse?settings.tpsHorseDistanceMultiplier.load():
            settings.thirdPersonDistanceMultiplier.load();
        const float tpHeight=usingChaos?settings.tpsUltimateHeightOffset.load():
            usingHorse?settings.tpsHorseHeightOffset.load():
            settings.thirdPersonHeightOffset.load();
        const float foot=settings.tpsFootAnchorOffset.load();
        if(std::isfinite(arm)&&arm>=100&&arm<=10000&&
           std::isfinite(basePitch)&&std::isfinite(mult)&&std::isfinite(tpHeight)&&
           std::isfinite(oy)&&std::isfinite(op)&&std::isfinite(foot)&&
           true&& // vanilla camera yaw/height are not TPS parameters
           mult>=0&&mult<=3&&foot>=-500&&foot<=500&&std::fabs(tpHeight)<=1000){
            constexpr float radians=0.01745329251994329577f;
            appliedPitch=std::clamp(basePitch+op,-75.0f,65.0f);
            appliedYaw=std::remainder(baseYaw+oy,360.0f);
            const float distance=std::clamp(arm*mult,0.0f,12000.0f);
            // Foot-level player origin. RootComponent may be at capsule
            // center, so configurable vertical correction is approximate
            // until capsule geometry has been natively verified.
            const float pivotX=attached?ax:x+std::cos(nativePitch*radians)*
                std::cos(nativeYaw*radians)*arm;
            const float pivotY=attached?ay:y+std::cos(nativePitch*radians)*
                std::sin(nativeYaw*radians)*arm;
            const float rawPivotZ=attached?(az-foot):z+std::sin(nativePitch*radians)*arm;
            // V0.86: no vertical smoothing. Firing modes control actual
            // native aim + orbit, NOT mount bobbing or camera height.
            const float pivotZ=rawPivotZ;
            const float pitchRad=appliedPitch*radians,yawRad=appliedYaw*radians;
            const float outX=pivotX-std::cos(pitchRad)*std::cos(yawRad)*distance;
            const float outY=pivotY-std::cos(pitchRad)*std::sin(yawRad)*distance;
            // Preserve V0.98 vertical framing with a TPS-private baseline;
            // this does NOT read or alter the normal camera's height option.
            constexpr float kTpsBaseHeight=50.0f;
            const float outZ=pivotZ-std::sin(pitchRad)*distance+tpHeight+kTpsBaseHeight;
            if(std::isfinite(outX)&&std::isfinite(outY)&&std::isfinite(outZ)&&
               std::fabs(outX)<1e7f&&std::fabs(outY)<1e7f&&std::fabs(outZ)<1e7f){
                std::memcpy(p,&outX,4);std::memcpy(p+4,&outY,4);std::memcpy(p+8,&outZ,4);
                std::memcpy(p+12,&appliedPitch,4);std::memcpy(p+16,&appliedYaw,4);
                appliedHeight=outZ;
                // The same validated live position already supplies this TPS
                // geometry. A character swap can temporarily leave the camera
                // on the previous owner: do not drop focus while still emitting
                // the verified gameplay view. Cinematics return before here.
                g_heldView.focusValid=attached&&actorValid;
                if(g_heldView.focusValid){
                    g_heldView.focus={ax,ay,az};
                    native_objects::MemoryReader reader;
                    g_heldView.focusBounds=ownedView&&(settings.tpsLensFocusEnabled.load()||settings.tpsBlur.custom.load())&&camera_focus::LensReady()?camera_focus::ReadBounds(reader,viewOwner.pawn):camera_focus::Bounds{};
                    if(g_heldView.focusBounds.valid&&usingHorse)g_heldView.focusBounds=camera_focus::Merge(g_heldView.focusBounds,camera_focus::ReadBounds(reader,viewOwner.character));
                    camera_focus::Compensate(p,g_heldView.focus,settings.tpsLensFocusEnabled.load(),g_heldView.focusBounds);
                    camera_focus::ApplyBlur(p,settings.tpsBlur.Get(),dg::native_tps::CinematicActive(),g_heldView.focusValid?&g_heldView.focus:nullptr,g_heldView.focusBounds);
                }
                g_tpsEmittedX.store(outX,std::memory_order_relaxed);
                g_tpsEmittedY.store(outY,std::memory_order_relaxed);
                g_tpsEmittedZ.store(outZ,std::memory_order_relaxed);
                g_tpsEmittedTickMs.store(now,std::memory_order_release);
                const auto emittedOwner=dg::local_players::PublishedSlot(0);
                AcquireSRWLockExclusive(&g_emittedLock);
                g_emittedView={outX,outY,outZ,appliedPitch,appliedYaw,
                    emittedOwner.localPlayer,emittedOwner.character,emittedOwner.generation,now};
                ReleaseSRWLockExclusive(&g_emittedLock);
                if(attached){
                    const unsigned n=++g_tpsAttachedFrames;
                    if(n==1||n==5000){
                        char line[230]{};
                        sprintf_s(line,"TPS V0.68: foot camera attached n=%u root=(%.0f,%.0f,%.0f) footZ=%.0f camera=(%.0f,%.0f,%.0f) yaw=%.1f",
                            n,ax,ay,az,pivotZ,outX,outY,outZ,appliedYaw);
                        Write(line);
                    }
                    if(lockZone&&g_zoneReferenceActive.load()&&
                        std::fabs(std::remainder(nativeYaw-baseYaw,360.0f))>5){
                        const unsigned ignoredYawCount=++g_zoneCameraIgnored;
                        if(ignoredYawCount==1||ignoredYawCount==10000){
                            char line[170]{};
                            sprintf_s(line,"TPS V0.68: native camera yaw differs n=%u native=%.1f locked=%.1f",
                                ignoredYawCount,nativeYaw,baseYaw);
                            Write(line);
                        }
                    }
                }
                // Publish actual live camera source, not the mere existence
                // of a horse sample. Consumers also impose a 300ms timeout.
                settings.tpsMountedViewActive.store(usingHorse,std::memory_order_release);
                settings.tpsMountedViewTick.store(now,std::memory_order_release);
                if(usingHorse&&activeHorseAim){
                    ++g_mountedFireHorseSelected;
                    if(mouseMountedFire)++g_v127MountedMouseHorsePoseHeld;
                }
                // Read-only facing-angle classification for native player or
                // live horse pose. The horse yaw is NOT the rider's mesh yaw.
                // Never force a world transform, movement flag or control yaw.
                const float facingGap=std::fabs(std::remainder(
                    appliedYaw-actorYaw,360.0f));
                if(std::isfinite(facingGap)){
                    if(usingHorse){
                        ++g_v127FacingHorseSamples;
                        if(facingGap>75.0f)++g_v127FacingHorseOffAxis;
                    }else{
                        ++g_v127FacingFootSamples;
                        if(facingGap>75.0f)++g_v127FacingFootOffAxis;
                    }
                    ULONGLONG previousFacing=g_v127LastFacingLogTick.load();
                    if((!previousFacing || (now>=previousFacing&&
                        now-previousFacing>=4500ull))&&
                       g_v127LastFacingLogTick.compare_exchange_strong(
                           previousFacing,now)){
                        char line[330]{};
                        sprintf_s(line,"TPS V1.27 FACING_AUDIT source=%s yawNative=%.1f cameraYaw=%.1f gapDeg=%.1f mouseFire=%d padFire=%d horsePoseAgeMs=%llu (READ ONLY, not mesh animation)",
                            usingChaos?"ULTIMATE_FORM":usingHorse?"HORSE_POSE":"FOOT_ACTOR",
                            actorYaw,appliedYaw,facingGap,
                            mouseMountedFire?1:0,
                            settings.tpsAimActive.load()?1:0,
                            static_cast<unsigned long long>(
                                horseTick&&now>=horseTick?now-horseTick:0ull));
                        Write(line);
                    }
                }
                settings.tpsGameplayViewTick.store(now,std::memory_order_release);
                settings.tpsViewGameplay.store(true,std::memory_order_release);
            }
        }
    }
    if(thirdPerson&&settings.tpsViewGameplay.load()&&
       settings.tpsGameplayViewTick.load()==now){
        g_heldView.camera=reinterpret_cast<uintptr_t>(camera);
        g_heldView.generation=liveGeneration;
        std::memcpy(g_heldView.values,p,sizeof(g_heldView.values));
        g_heldView.valid=true;
    }
    if(!thirdPerson&&ownedView){
        const auto profile=settings.nativeBlur.Get();
        if(profile.disabled||profile.custom){
            local_players::Pose pose{};camera_focus::Point target{};camera_focus::Bounds bounds{};
            const bool valid=local_players::ReadPose(viewOwner,pose);
            if(valid){target={pose.x,pose.y,pose.z};
                if(profile.custom){native_objects::MemoryReader reader;
                    bounds=camera_focus::ReadBounds(reader,viewOwner.pawn);
                    if(viewOwner.kind==local_players::PoseKind::Horse)bounds=camera_focus::Merge(bounds,camera_focus::ReadBounds(reader,viewOwner.character));}}
            camera_focus::ApplyBlur(p,profile,native_tps::CinematicActive(static_cast<unsigned>(viewOwner.index)),valid?&target:nullptr,bounds);
        }
    }
    g_nativeFov.store(nativeFov);g_appliedFov.store(appliedFov);
    g_nativePitch.store(nativePitch);g_appliedPitch.store(appliedPitch);
    g_nativeYaw.store(nativeYaw);g_appliedYaw.store(appliedYaw);
    const unsigned n=++g_viewCount;
    if(Sample(n)){
        char line[220]{};
        sprintf_s(line,"Camera V0.68: view n=%u nativeFOV=%.1f outputFOV=%.1f nativeYaw=%.1f outputYaw=%.1f tp=%d",
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
    const auto& camRt=dg::runtime::Get();
    const float zoom = camRt.thirdPersonEnabled.load(std::memory_order_relaxed)
        ? 0.0f : camRt.cameraZoomPercent.load(std::memory_order_relaxed);
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
    const bool secondaryArm=valid&&dg::coop_camera::ObserveArm(arm,native);
    if (valid&&!secondaryArm) {
        g_nativeDistance.store(native, std::memory_order_relaxed);
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
// Recenter yaw behind the LIVE local player; do not modify player rotation,
// game inputs, native scene camera objects, or the stored TPS distance.
bool RecenterOnPlayer() {
    auto& rt=dg::runtime::Get();
    if(!rt.thirdPersonEnabled.load()||!g_zoneReferenceActive.load()) return false;
    const ULONGLONG tick=rt.tpsActorLocationTick.load();
    const ULONGLONG now=GetTickCount64();
    const bool onFoot=tick&&now>=tick&&now-tick<=350ull;
    const ULONGLONG horseTick=rt.tpsHorsePoseTick.load();
    const bool onHorse=!onFoot&&horseTick&&now>=horseTick&&now-horseTick<=750ull;
    float formYaw=0;
    const bool onUltimate=dg::chaos_pose::GetLiveYaw(formYaw);
    if(!onFoot&&!onHorse&&!onUltimate)return false;
    const float actorYaw=onUltimate?formYaw:onHorse?rt.tpsHorseYawDegrees.load():rt.tpsActorYawDegrees.load();
    const float base=g_zoneReferenceYaw.load();
    const float manual=0.0f; // vanilla yaw separated from TPS recenter.
    if(!std::isfinite(actorYaw)||!std::isfinite(base)||!std::isfinite(manual)) return false;
    const float delta=std::remainder(actorYaw-base-manual,360.0f);
    rt.cameraOrbitYawDegrees.store(delta);
    rt.cameraOrbitPitchDegrees.store(0.0f);
    char message[170]{};
    sprintf_s(message,"TPS V1.38 recenter source=%s actorYaw=%.1f initialYaw=%.1f orbitYaw=%.1f",
        onUltimate?"ULTIMATE":onHorse?"HORSE":"FOOT",actorYaw,base,delta);
    Write(message);
    return true;
}
float AppliedYaw() { return g_appliedYaw.load(); }
AppliedView GetAppliedView(){
    AcquireSRWLockShared(&g_emittedLock);const auto view=g_emittedView;
    ReleaseSRWLockShared(&g_emittedLock);return view;
}
Telemetry GetTelemetry() {
    Telemetry t{};
    t.viewReady = g_viewReady.load();
    t.armReady = g_armReady.load();
    t.viewCalls = g_viewCount.load();
    t.tpsAttachedFrames = g_tpsAttachedFrames.load();
    t.cinematicEntered=g_cinematicEntered.load();
    t.cinematicExited=g_cinematicExited.load();
    t.cinematicCandidateBegins=g_cinematicCandidateBegins.load();
    t.cinematicMissingActorSamples=g_cinematicMissingActorSamples.load();
    t.cinematicFarViewSamples=g_cinematicFarViewSamples.load();
    t.mountedMouseViewFrames = g_v127MountedMouseViewFrames.load();
    t.mountedMouseHorsePoseHeld = g_v127MountedMouseHorsePoseHeld.load();
    t.facingFootSamples = g_v127FacingFootSamples.load();
    t.facingFootOffAxis = g_v127FacingFootOffAxis.load();
    t.facingHorseSamples = g_v127FacingHorseSamples.load();
    t.facingHorseOffAxis = g_v127FacingHorseOffAxis.load();
    t.nativeFov = g_nativeFov.load();
    t.appliedFov = g_appliedFov.load();
    t.nativePitch = g_nativePitch.load();
    t.appliedPitch = g_appliedPitch.load();
    t.nativeYaw = g_nativeYaw.load();
    t.appliedYaw = g_appliedYaw.load();
    t.appliedWorldX=g_tpsEmittedX.load(std::memory_order_relaxed);
    t.appliedWorldY=g_tpsEmittedY.load(std::memory_order_relaxed);
    t.appliedWorldZ=g_tpsEmittedZ.load(std::memory_order_relaxed);
    t.appliedWorldTickMs=g_tpsEmittedTickMs.load(std::memory_order_acquire);
    return t;
}
const char* CinematicDebugPhaseLabel(CinematicDebugPhase phase){
    switch(phase){
        case CinematicDebugPhase::Candidate:return "CANDIDATE";
        case CinematicDebugPhase::Detected:return "ENTER";
        case CinematicDebugPhase::Returned:return "EXIT";
        case CinematicDebugPhase::Rejected:return "REJECTED";
        case CinematicDebugPhase::BriefCut:return "SHORT_CUT";
        case CinematicDebugPhase::ManualMark:return "USER_MARK";
        case CinematicDebugPhase::Disabled:return "RESET";
    }
    return "UNKNOWN";
}
CinematicDebugSnapshot GetCinematicDebugSnapshot(){
    CinematicDebugSnapshot result{};
    AcquireSRWLockShared(&g_cinematicDebugLock);
    result.count=g_cinematicCount;
    for(uint32_t i=0;i<result.count;++i){
        const uint32_t index=(g_cinematicNext+kCinematicDebugHistory-
                              result.count+i)%kCinematicDebugHistory;
        result.events[i]=g_cinematicEvents[index];
    }
    result.hasLastNativeSample=g_cinematicHasLastView;
    result.lastNativeSample=g_cinematicLastView;
    result.lastNativeSampleTickMs=g_cinematicLastView.tickMs;
    ReleaseSRWLockShared(&g_cinematicDebugLock);
    result.activeCandidateId=g_cinematicActiveCandidateId.load(
        std::memory_order_acquire);
    result.currentlySuspended=dg::runtime::Get().tpsCutsceneSuspended.load(
        std::memory_order_acquire);
    return result;
}
bool MarkCurrentCinematicForDebug(){
    CinematicDebugEvent observation{};
    bool hasSample=false;
    AcquireSRWLockShared(&g_cinematicDebugLock);
    observation=g_cinematicLastView;
    hasSample=g_cinematicHasLastView;
    ReleaseSRWLockShared(&g_cinematicDebugLock);
    if(!hasSample)return false;
    // Explicit user annotation, not automatic cinematic identity.
    const uint64_t now=GetTickCount64();
    const uint64_t age=now>=observation.tickMs?now-observation.tickMs:0ull;
    // Show when the user actually clicked, not the timestamp of the
    // possibly stale native view. Keep the source age in durationMs.
    observation.tickMs=now;
    RecordCinematicEvent(CinematicDebugPhase::ManualMark,
        g_cinematicActiveCandidateId.load(std::memory_order_acquire),
        age,observation);
    return true;
}

}
