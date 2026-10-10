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
