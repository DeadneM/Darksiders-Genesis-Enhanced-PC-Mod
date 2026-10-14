#pragma once
#include <cstdint>
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
    float nativeArmLength = 0.0f;
    float appliedArmLength = 0.0f;
};
void Install(LogFn log);
Telemetry GetTelemetry();
bool RecenterOnPlayer();
}
