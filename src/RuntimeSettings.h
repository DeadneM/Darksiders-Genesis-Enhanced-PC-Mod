#pragma once

#include <atomic>

namespace dg::runtime {

struct Snapshot {
    bool toggleHudEnabled = true;
    bool hideReticle = false;
    bool fovEnabled = false;
    bool movementSpeedEnabled = true;
    bool actionRecoveryEnabled = true;
    bool skipLogosEnabled = true;
    bool skipIntroEnabled = true;
    bool skipWarningEnabled = true;
    bool thirdPersonEnabled = false;
    bool thirdPersonFovEnabled = true;
    bool tpsFollowPlayer = true;
    bool tpsSuppressNativeRightStick = true;
    bool cameraOrbitInputEnabled = true;
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool hotstreakChargeEnabled = true;

    float fovDegrees = 90.0f;
    float thirdPersonFovDegrees = 90.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 0.0f;
    float thirdPersonDistanceMultiplier = 0.50f;
    float thirdPersonPitchDegrees = -12.0f;
    float thirdPersonHeightOffset = 180.0f;
    float tpsFootAnchorOffset = 88.0f; // estimated capsule center-to-foot, configurable
    float cameraMouseSensitivity = 0.12f;
    float cameraStickSpeed = 135.0f;
    float movementSpeedMultiplier = 1.50f;
    float actionRecoveryDelayMs = 0.0f;
    float pistolDamageMultiplier = 2.00f;
    float meleeDamageMultiplier = 2.00f;
    float jumpHeightMultiplier = 1.25f;
    float glideDurationMultiplier = 10.00f;
    float horseSpeedMultiplier = 1.25f;
    float horseSprintSpeedMultiplier = 1.25f;
    float horseSprintDurationMultiplier = 5.00f;
    float hotstreakChargeMultiplier = 2.00f;
};

struct State {
    std::atomic_bool toggleHudEnabled{true};
    std::atomic_bool hideReticle{false};
    std::atomic_bool fovEnabled{false};
    std::atomic_bool movementSpeedEnabled{true};
    std::atomic_bool actionRecoveryEnabled{true};
    std::atomic_bool skipLogosEnabled{true};
    std::atomic_bool skipIntroEnabled{true};
    std::atomic_bool skipWarningEnabled{true};
    std::atomic_bool thirdPersonEnabled{false};
    std::atomic_bool thirdPersonFovEnabled{true};
    std::atomic_bool tpsFollowPlayer{true};
    std::atomic_bool tpsViewGameplay{false};
    std::atomic_ullong tpsGameplayViewTick{0};
    std::atomic_bool tpsSuppressNativeRightStick{true};
    std::atomic_bool cameraOrbitInputEnabled{true};
    std::atomic_bool tpsAimActive{false};
    std::atomic<float> tpsActorWorldX{0.0f};
    std::atomic<float> tpsActorWorldY{0.0f};
    std::atomic<float> tpsActorWorldZ{0.0f};
    std::atomic<float> tpsActorYawDegrees{0.0f};
    std::atomic_ullong tpsActorLocationTick{0};
    std::atomic_uint32_t tpsActorGeneration{0};
    std::atomic_bool pistolDamageEnabled{true};
    std::atomic_bool meleeDamageEnabled{true};
    std::atomic_bool jumpHeightEnabled{true};
    std::atomic_bool glideDurationEnabled{true};
    std::atomic_bool horseSpeedEnabled{true};
    std::atomic_bool horseSprintSpeedEnabled{true};
    std::atomic_bool horseSprintDurationEnabled{true};
    std::atomic_bool hotstreakChargeEnabled{true};

    std::atomic<float> fovDegrees{90.0f};
    std::atomic<float> thirdPersonFovDegrees{90.0f};
    std::atomic<float> cameraZoomPercent{0.0f};
    std::atomic<float> cameraPitchDegrees{0.0f};
    std::atomic<float> cameraYawDegrees{0.0f};
    std::atomic<float> cameraHeightOffset{0.0f};
    std::atomic<float> thirdPersonDistanceMultiplier{0.50f};
    std::atomic<float> thirdPersonPitchDegrees{-12.0f};
    std::atomic<float> thirdPersonHeightOffset{180.0f};
    std::atomic<float> tpsFootAnchorOffset{88.0f};
    std::atomic<float> cameraMouseSensitivity{0.12f};
    std::atomic<float> cameraStickSpeed{135.0f};
    std::atomic<float> cameraOrbitYawDegrees{0.0f};
    std::atomic<float> cameraOrbitPitchDegrees{0.0f};
    std::atomic<float> movementSpeedMultiplier{1.50f};
    std::atomic<float> actionRecoveryDelayMs{0.0f};
    std::atomic<float> pistolDamageMultiplier{2.00f};
    std::atomic<float> meleeDamageMultiplier{2.00f};
    std::atomic<float> jumpHeightMultiplier{1.25f};
    std::atomic<float> glideDurationMultiplier{10.00f};
    std::atomic<float> horseSpeedMultiplier{1.25f};
    std::atomic<float> horseSprintSpeedMultiplier{1.25f};
    std::atomic<float> horseSprintDurationMultiplier{5.00f};
    std::atomic<float> hotstreakChargeMultiplier{2.00f};
};

State& Get();
void Publish(const Snapshot& snapshot);

} // namespace dg::runtime
