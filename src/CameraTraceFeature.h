#pragma once
#include <cstdint>
#include <array>
#include <cstddef>
namespace dg::camera_trace {
using LogFn = void (*)(const char*);
struct Telemetry {
    bool viewReady = false;
    bool armReady = false;
    uint32_t viewCalls = 0;
    uint32_t armCalls = 0;
    uint32_t tpsAttachedFrames = 0;
    uint32_t nativeZoneOverridesIgnored = 0;
    uint32_t tpsAttachDeferred = 0;
    uint32_t tpsFovSamples = 0;
    uint32_t cinematicEntered = 0;
    uint32_t cinematicExited = 0;
    uint32_t cinematicCandidateBegins = 0;
    uint32_t cinematicMissingActorSamples = 0;
    uint32_t cinematicFarViewSamples = 0;
    uint32_t mountedFireRequested = 0;
    uint32_t mountedFireHorseSelected = 0;
    uint32_t mountedFireOverrodeFoot = 0;
    uint32_t mountedFireHorseUnavailable = 0;
    uint32_t mountedFireVerticalSmoothed = 0;
    // V1.27: camera read-only diagnostics, not native shot trajectory proof.
    uint32_t mountedMouseViewFrames = 0;
    uint32_t mountedMouseHorsePoseHeld = 0;
    uint32_t facingFootSamples = 0;
    uint32_t facingFootOffAxis = 0;
    uint32_t facingHorseSamples = 0;
    uint32_t facingHorseOffAxis = 0;
    float mountedFireLastVerticalCorrection = 0.0f;
    bool mountedFireCurrentlyActive = false;
    float nativeFov = 0.0f;
    float appliedFov = 0.0f;
    float nativePitch = 0.0f;
    float appliedPitch = 0.0f;
    float nativeYaw = 0.0f;
    float appliedYaw = 0.0f;
    float nativeHeight = 0.0f;
    float appliedHeight = 0.0f;
    // V1.30: actual OUTGOING third-person camera view, world-space.
    // Telemetry only; never alters native projectile/aim state.
    float appliedWorldX = 0.0f;
    float appliedWorldY = 0.0f;
    float appliedWorldZ = 0.0f;
    uint64_t appliedWorldTickMs = 0;
    float nativeArmLength = 0.0f;
    float appliedArmLength = 0.0f;
};
// V1.28 TEMPORARY CUTSCENE DEBUG: events represent a camera-heuristic
// classification. They are NOT game cutscene asset/sequence names.
enum class CinematicDebugPhase : uint8_t {
    Candidate, Detected, Returned, Rejected, BriefCut, ManualMark, Disabled
};
struct CinematicDebugEvent {
    CinematicDebugPhase phase = CinematicDebugPhase::Candidate;
    uint32_t serial = 0;
    uint32_t candidateId = 0;
    uint64_t tickMs = 0;
    uint64_t durationMs = 0;
    bool actorValid = false;
    bool nearPawn = false;
    bool normalAngles = false;
    bool gameplayCamera = false;
    bool autoSuspend = false;
    bool distanceExceeded = false;
    bool noActorStatic = false;
    float nativeViewShift = 0.0f;
    float gapXY = 0.0f;
    float gapZ = 0.0f;
    float gap3D = 0.0f;
    float distanceBaseline = 0.0f;
    float selectedTpsDistance = 0.0f;
    float candidateDistanceThreshold = 0.0f;
    float nativePitch = 0.0f;
    float nativeYaw = 0.0f;
    float nativeFov = 0.0f;
};
constexpr std::size_t kCinematicDebugHistory = 24;
struct CinematicDebugSnapshot {
    std::array<CinematicDebugEvent,kCinematicDebugHistory> events{};
    uint32_t count = 0;
    uint32_t activeCandidateId = 0;
    bool currentlySuspended = false;
    bool hasLastNativeSample = false;
    uint64_t lastNativeSampleTickMs = 0;
    CinematicDebugEvent lastNativeSample{};
};
CinematicDebugSnapshot GetCinematicDebugSnapshot();
bool MarkCurrentCinematicForDebug(); // user annotation only, no camera writes
const char* CinematicDebugPhaseLabel(CinematicDebugPhase phase);
void Install(LogFn log);
Telemetry GetTelemetry();
bool RecenterOnPlayer();
}
