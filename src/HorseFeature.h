#pragma once

#include <cstdint>

namespace dg::horse {

using LogFn = void(*)(const char*);

struct Settings {
    bool speedEnabled = true;
    float speedMultiplier = 1.25f;
    bool sprintSpeedEnabled = true;
    float sprintSpeedMultiplier = 1.25f;
    bool sprintDurationEnabled = true;
    float sprintDurationMultiplier = 2.0f;
};

struct Telemetry {
    bool validated = false;
    bool staminaReady = false;
    bool sprinting = false;
    void* playerOwner = nullptr;
    void* horseOwner = nullptr;
    void* horseMovement = nullptr;
    float nativeGetMaxSpeed = 0.0f;
    float appliedGetMaxSpeed = 0.0f;
    float nativeMaxWalkSpeed = 0.0f;
    float appliedMaxWalkSpeed = 0.0f;
    float nativeMaxAcceleration = 0.0f;
    float appliedMaxAcceleration = 0.0f;
    float nativeSprintingMaxSpeed = 0.0f;
    float appliedSprintingMaxSpeed = 0.0f;
    float nativeSprintDrain = 0.0f;
    float appliedSprintDrain = 0.0f;
    std::uint32_t uniqueCandidatesLogged = 0;
    std::uint32_t candidateMatches = 0;
};

void Initialize(LogFn logger);
void SetSettings(const Settings& settings);
void PollDirectHorse(void* knownLocalPlayer);
void ObserveMovement(
    void* movementComponent,
    void* characterOwner,
    void* knownLocalPlayer,
    float nativeGetMaxSpeed);
float AdjustSpeedResult(
    void* movementComponent,
    float nativeGetMaxSpeed);
void Tick();
Telemetry GetTelemetry();
bool IsValidatedMovement(void* movementComponent);
void Shutdown();

} // namespace dg::horse
