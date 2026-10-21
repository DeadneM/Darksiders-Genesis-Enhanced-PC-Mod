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
    std::uint64_t lastHorseNativeTick = 0;
    void* playerOwner = nullptr;
    void* horseOwner = nullptr;
    void* horseMovement = nullptr;
    float nativeMaxWalkSpeed = 0.0f;
    float nativeMaxAcceleration = 0.0f;
    float nativeSprintingMaxSpeed = 0.0f;
    std::uint32_t uniqueCandidatesLogged = 0;
};

void Initialize(LogFn logger);
void SetSettings(const Settings& settings);
void ObservePlayerCharacter(void* playerCharacter);
float AdjustSpeedResult(
    void* movementComponent,
    float nativeGetMaxSpeed);
Telemetry GetTelemetry();
bool IsValidatedMovement(void* movementComponent);
void Shutdown();

} // namespace dg::horse
