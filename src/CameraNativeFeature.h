#pragma once
#include <cstdint>

namespace dg::camera_native {
using LogFn = void(*)(const char*);

struct Telemetry {
    bool fovHookReady = false;
    bool zoomHookReady = false;
    uint32_t fovCalls = 0;
    uint32_t zoomCalls = 0;
    float nativeFov = 0.0f;
    float appliedFov = 0.0f;
    float nativeDistance = 0.0f;
    float appliedDistance = 0.0f;
};

// Only call after exact supported retail EXE hash verification.
// Camera memory is accessed exclusively in the live native callback.
void Install(LogFn logger);
Telemetry GetTelemetry();
} // namespace dg::camera_native
