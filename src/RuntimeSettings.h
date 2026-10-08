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
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool hotstreakChargeEnabled = true;

    float fovDegrees = 90.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 0.0f;
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
    std::atomic_bool pistolDamageEnabled{true};
    std::atomic_bool meleeDamageEnabled{true};
    std::atomic_bool jumpHeightEnabled{true};
    std::atomic_bool glideDurationEnabled{true};
    std::atomic_bool horseSpeedEnabled{true};
    std::atomic_bool horseSprintSpeedEnabled{true};
    std::atomic_bool horseSprintDurationEnabled{true};
    std::atomic_bool hotstreakChargeEnabled{true};

    std::atomic<float> fovDegrees{90.0f};
    std::atomic<float> cameraZoomPercent{0.0f};
    std::atomic<float> cameraPitchDegrees{0.0f};
    std::atomic<float> cameraYawDegrees{0.0f};
    std::atomic<float> cameraHeightOffset{0.0f};
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
