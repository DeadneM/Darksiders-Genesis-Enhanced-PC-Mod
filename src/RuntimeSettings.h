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
    bool cameraOrbitInputEnabled = true; // derived: mouse OR controller
    bool cameraControllerOrbitEnabled = true;
    bool tpsCutsceneAutoSuspend = true;
    // V0.75: native TPS mouse firing automatically uses camera heading.
    bool tpsHideGroundAimFx = true;
    int aimIndicatorHideMode = 2;
    bool tpsHorseStableAimCamera = true;
    int tpsMountedAimMode = 0; // camera free/aim follows view, native mode=1
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
    bool horseEverywhereEnabled = true;
    int horseAreaMode = 2; // restricted-only by default; native predicate classification
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool hotstreakChargeEnabled = true;
    bool wrathChargeEnabled = false;
    bool ultimateChargeEnabled = false;
    bool synergyChargeEnabled = false;

    float fovDegrees = 90.0f;
    float thirdPersonFovDegrees = 110.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 50.0f;
    float thirdPersonDistanceMultiplier = 0.15f;
    float thirdPersonPitchDegrees = -12.0f;
    float thirdPersonHeightOffset = 0.0f;
    float tpsFootAnchorOffset = 0.0f; // estimated capsule center-to-foot, configurable
    float cameraStickSpeed = 180.0f;
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
    float wrathChargeMultiplier = 2.00f;
    float ultimateChargeMultiplier = 2.00f;
    float synergyChargeMultiplier = 2.00f;
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
    std::atomic_bool cameraOrbitInputEnabled{true};
    std::atomic_bool cameraControllerOrbitEnabled{true};
    std::atomic_bool tpsCutsceneAutoSuspend{true};
    std::atomic_bool tpsCutsceneSuspended{false};
    std::atomic_bool tpsTemporarilySuspended{false};
    std::atomic_bool tpsAbilityWheelHeld{false}; // LB/keyboard wheels; R3 is USE equipment, not wheel opener
    std::atomic_ullong tpsAbilityWheelTick{0}; // LB/native keyboard selection, R3 passthrough retained for use
    std::atomic_bool tpsHideGroundAimFx{true};
    std::atomic_int aimIndicatorHideMode{2};
    std::atomic_bool tpsHorseStableAimCamera{true};
    std::atomic_int tpsMountedAimMode{0};
    std::atomic_bool tpsMountedViewActive{false};
    std::atomic_ullong tpsMountedViewTick{0};
    std::atomic_ullong tpsLastHorseSummonInputTick{0}; // V0.88 audit only
    std::atomic_bool tpsAimActive{false};
    std::atomic<float> tpsActorWorldX{0.0f};
    std::atomic<float> tpsActorWorldY{0.0f};
    std::atomic<float> tpsActorWorldZ{0.0f};
    std::atomic<float> tpsActorYawDegrees{0.0f};
    std::atomic_ullong tpsActorLocationTick{0};
    std::atomic_uint32_t tpsActorGeneration{0};
    // Pose sampled from a LIVE HorseCharacter native callback only; never
    // dereference a cached horse pointer in camera/Present threads.
    std::atomic<float> tpsHorseWorldX{0.0f};
    std::atomic<float> tpsHorseWorldY{0.0f};
    std::atomic<float> tpsHorseWorldZ{0.0f};
    std::atomic<float> tpsHorseYawDegrees{0.0f};
    std::atomic_ullong tpsHorsePoseTick{0};
    std::atomic_uintptr_t tpsHorseOwnerIdentity{0};

    std::atomic_bool pistolDamageEnabled{true};
    std::atomic_bool meleeDamageEnabled{true};
    std::atomic_bool jumpHeightEnabled{true};
    std::atomic_bool glideDurationEnabled{true};
    std::atomic_bool horseEverywhereEnabled{true};
    std::atomic_int horseAreaMode{2};
    std::atomic_ullong horseSummonIntentTick{0};
    std::atomic_bool horseSpeedEnabled{true};
    std::atomic_bool horseSprintSpeedEnabled{true};
    std::atomic_bool horseSprintDurationEnabled{true};
    std::atomic_bool hotstreakChargeEnabled{true};
    std::atomic_bool wrathChargeEnabled{false};
    std::atomic_bool ultimateChargeEnabled{false};
    std::atomic_bool synergyChargeEnabled{false};

    std::atomic<float> fovDegrees{90.0f};
    std::atomic<float> thirdPersonFovDegrees{110.0f};
    std::atomic<float> cameraZoomPercent{0.0f};
    std::atomic<float> cameraPitchDegrees{0.0f};
    std::atomic<float> cameraYawDegrees{0.0f};
    std::atomic<float> cameraHeightOffset{50.0f};
    std::atomic<float> thirdPersonDistanceMultiplier{0.15f};
    std::atomic<float> thirdPersonPitchDegrees{-12.0f};
    std::atomic<float> thirdPersonHeightOffset{0.0f};
    std::atomic<float> tpsFootAnchorOffset{0.0f};
    std::atomic<float> cameraStickSpeed{180.0f};
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
    std::atomic<float> wrathChargeMultiplier{2.00f};
    std::atomic<float> ultimateChargeMultiplier{2.00f};
    std::atomic<float> synergyChargeMultiplier{2.00f};
};

State& Get();
void Publish(const Snapshot& snapshot);

} // namespace dg::runtime
