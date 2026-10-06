#pragma once

#include <cstdint>

namespace dg::horse {

using LogFn = void(*)(const char*);

struct Settings {
    bool speedEnabled = true;
    float speedMultiplier = 1.25f;
    bool sprintDurationEnabled = true;
    float sprintDurationMultiplier = 2.0f;
};

struct Telemetry {
    bool mounted = false;
    bool movementValidated = false;
    bool staminaReady = false;
    void* horseOwner = nullptr;
    void* horseMovement = nullptr;
    float nativeMaxWalkSpeed = 0.0f;
    float appliedMaxWalkSpeed = 0.0f;
    float nativeMaxAcceleration = 0.0f;
    float appliedMaxAcceleration = 0.0f;
    float nativeSprintDrain = 0.0f;
    float appliedSprintDrain = 0.0f;
    std::uint32_t candidateChecks = 0;
    std::uint32_t candidateMatches = 0;
};

void Initialize(LogFn logger);
void SetSettings(const Settings& settings);
void OnGetMaxSpeed(
    void* movementComponent,
    void* characterOwner,
    void* localPlayer,
    float nativeGetMaxSpeed);
Telemetry GetTelemetry();
void Shutdown();

} // namespace dg::horse
