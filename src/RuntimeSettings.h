#pragma once

#include "DefaultSettings.h"
#include "CameraBlurSettings.h"
#include <atomic>

namespace dg::runtime {

struct Snapshot {
    int overlayMenuKey = defaults::MenuKey;
    bool toggleHudEnabled = true;
    bool interactionHudScaleEnabled = true;
    float interactionHudScale = 0.65f;
    bool mountedHolstersEnabled = false;
    int pistolRightModel = 0;
    int pistolLeftModel = 0;
    bool bonusMountSkinsEnabled = false;
    bool progressionSkinsEnabled = false;
    bool leviathanVisualEnabled = false;
    int leviathanVisualMode = 0;
    bool weaponSheathingEnabled = false;
    bool sheathNeverEnabled = true;
    float sheathDelaySeconds = 10.0f;
    bool sheathStrifeMeleeNeverEnabled = false;
    float sheathStrifeMeleeDelaySeconds = 0.0f;
    bool sheathWarNeverEnabled = true;
    float sheathWarDelaySeconds = 10.0f;
    bool fovEnabled = false;
    bool dashMovementRecoveryEnabled = false;
    bool movementSpeedEnabled = true;
    bool skipLogosEnabled = true;
    bool skipIntroEnabled = true;
    bool skipWarningEnabled = true;
    bool thirdPersonEnabled = true;
    bool thirdPersonFovEnabled = true;
    bool tpsLensFocusEnabled = true;
    camera_blur::Profile nativeBlur{},tpsBlur{};
    bool cameraOrbitInputEnabled = true; // derived: mouse OR controller
    bool cameraControllerOrbitEnabled = true;
    int tpsCameraStickMode = 2;
    int tpsMousePrimaryButton = 1;
    int tpsMouseSecondaryButton = 2;
    int tpsFirePrimaryMask = 131072;
    int tpsFireSecondaryMask = 512;
    int tpsWheelPadMask = 256;
    int tpsGrenadeButtonMask = 128;
    int horsePadButtonA = 256;
    int horsePadButtonB = 512;

    bool tpsCutsceneAutoSuspend = true;
    int aimIndicatorHideMode = 2;
    bool tps3DAimEnabled = true;
    float tpsAimPitchOffsetDegrees = 12.0f;
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
    bool wrathChargeEnabled = true;
    bool ultimateChargeEnabled = true;
    bool ultimateDecreaseEnabled = false;
    bool ultimateInfiniteDurationEnabled = false;
    float ultimateDurationMultiplier = 2.0f;
    bool synergyChargeEnabled = true;
    bool unlimitedWrathEnabled = false;
    bool unlimitedUltimateEnabled = false;
    bool unlimitedSynergyEnabled = false;
    bool finiteStandardAmmoEnabled = false;
    int standardAmmoCapacity = 200;
    bool grappleRangeEnabled = false;
    float grappleRangeMultiplier = 2.0f;
    int infiniteDoubleJumpMode = 0; // 0 Off, 1 rearm after each glide, 2 Always
    bool reduceAmmoEnabled = defaults::ReduceAmmo;
    bool unlimitedAmmoEnabled = false;
    float ammoConsumptionDivisor = 2.0f;
    bool reduceHotstreakEnabled = false;
    bool unlimitedHotstreakEnabled = false;
    float hotstreakDecreaseDivisor = 2.0f;
    bool potionRechargeEnabled = false;
    bool unlimitedPotionsEnabled = false;
    bool unlimitedHealthEnabled = false;
    bool unlimitedGlideEnabled = false;
    bool unlimitedHorseSprintEnabled = false;
    float potionRechargeMultiplier = 2.0f;

    float fovDegrees = 90.0f;
    float thirdPersonFovDegrees = 110.0f;
    float thirdPersonCoopFovDegrees = 110.0f;
    float cameraZoomPercent = 0.0f;
    float cameraPitchDegrees = 0.0f;
    float cameraYawDegrees = 0.0f;
    float cameraHeightOffset = 0.0f;
    float thirdPersonDistanceMultiplier = 0.15f;
    float thirdPersonPitchDegrees = -12.0f;
    float thirdPersonHeightOffset = 0.0f;
    float tpsUltimateDistanceMultiplier = defaults::UltimateDistance;
    float tpsUltimateHeightOffset = defaults::UltimateHeight;
    float tpsHorseDistanceMultiplier = defaults::HorseDistance;
    float tpsHorseHeightOffset = defaults::HorseHeight;
    float tpsFootAnchorOffset = 0.0f; // estimated capsule center-to-foot, configurable
    float cameraStickSpeed = 180.0f;
    float movementSpeedMultiplier = 1.50f;
    float pistolDamageMultiplier = 2.00f;
    float meleeDamageMultiplier = 2.00f;
    float jumpHeightMultiplier = 1.25f;
    float glideDurationMultiplier = 10.00f;
    float horseSpeedMultiplier = 1.25f;
    float horseSprintSpeedMultiplier = 1.25f;
    float horseSprintDurationMultiplier = 5.00f;
    float hotstreakChargeMultiplier = 2.00f;
    float wrathChargeMultiplier = defaults::WrathGain;
    float ultimateChargeMultiplier = defaults::UltimateGain;
    float synergyChargeMultiplier = defaults::SynergyGain;
};

struct State {
    std::atomic_int overlayMenuKey{defaults::MenuKey};
    std::atomic_bool toggleHudEnabled{true};
    std::atomic_bool interactionHudScaleEnabled{true};
    std::atomic<float> interactionHudScale{0.65f};
    std::atomic_bool mountedHolstersEnabled{false};
    std::atomic_int pistolRightModel{0};
    std::atomic_int pistolLeftModel{0};
    std::atomic_bool bonusMountSkinsEnabled{false};
    std::atomic_bool progressionSkinsEnabled{false};
    std::atomic_bool leviathanVisualEnabled{false};
    std::atomic_int leviathanVisualMode{0};
    std::atomic_bool weaponSheathingEnabled{false};
    std::atomic_bool sheathNeverEnabled{true};
    std::atomic<float> sheathDelaySeconds{10.0f};
    std::atomic_bool sheathStrifeMeleeNeverEnabled{false};
    std::atomic<float> sheathStrifeMeleeDelaySeconds{0.0f};
    std::atomic_bool sheathWarNeverEnabled{true};
    std::atomic<float> sheathWarDelaySeconds{10.0f};
    std::atomic_bool fovEnabled{false};
    std::atomic_bool dashMovementRecoveryEnabled{false};
    std::atomic_bool movementSpeedEnabled{true};
    std::atomic_bool skipLogosEnabled{true};
    std::atomic_bool skipIntroEnabled{true};
    std::atomic_bool skipWarningEnabled{true};
    std::atomic_bool thirdPersonEnabled{true};
    std::atomic_bool thirdPersonFovEnabled{true};
    std::atomic_bool tpsLensFocusEnabled{true};
    camera_blur::SharedProfile nativeBlur{},tpsBlur{};
    std::atomic_bool tpsViewGameplay{false};
    std::atomic_ullong tpsGameplayViewTick{0};
    std::atomic_bool cameraOrbitInputEnabled{true};
    std::atomic_bool cameraControllerOrbitEnabled{true};
    std::atomic_int tpsCameraStickMode{2};
    // Rebindable controls are published with other runtime settings. Native
    // callbacks must not race with the editable Store on the render thread.
    std::atomic_int tpsMousePrimaryButton{1};
    std::atomic_int tpsMouseSecondaryButton{2};
    std::atomic_int tpsFirePrimaryMask{131072};
    std::atomic_int tpsFireSecondaryMask{512};
    std::atomic_int tpsWheelPadMask{256};
    std::atomic_int tpsGrenadeButtonMask{128};
    std::atomic_int horsePadButtonA{256};
    std::atomic_int horsePadButtonB{512};

    std::atomic_bool tpsCutsceneAutoSuspend{true};
    std::atomic_bool tpsCutsceneSuspended{false};
    std::atomic_bool tpsTemporarilySuspended{false};
    std::atomic_bool tpsUiViewHeld{false}; // native world-pause camera hold; live wheel camera follows normally
    std::atomic_bool tpsAbilityWheelHeld{false}; // LB/keyboard wheels; R3 is USE equipment, not wheel opener
    std::atomic_ullong tpsAbilityWheelTick{0}; // LB/native keyboard selection, R3 passthrough retained for use
    std::atomic_int aimIndicatorHideMode{2};
    std::atomic_bool tps3DAimEnabled{true};
    std::atomic<float> tpsAimPitchOffsetDegrees{12.0f};
    std::atomic_int tpsMountedAimMode{0};
    std::atomic_bool tpsMountedViewActive{false};
    std::atomic_ullong tpsMountedViewTick{0};
    std::atomic_ullong tpsLastHorseSummonInputTick{0}; // V0.88 audit only
    std::atomic_bool tpsAimActive{false}; // native gamepad shooting
    // V1.27: physical mouse shot is a separate source. Published from
    // focused TPS Present only, never synthesize controller/weapon input.
    std::atomic_bool tpsMouseFireHeld{false};
    std::atomic_ullong tpsMouseFireTick{0};
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
    std::atomic_bool wrathChargeEnabled{true};
    std::atomic_bool ultimateChargeEnabled{true};
    std::atomic_bool ultimateDecreaseEnabled{false};
    std::atomic_bool ultimateInfiniteDurationEnabled{false};
    std::atomic<float> ultimateDurationMultiplier{2.0f};
    std::atomic_bool synergyChargeEnabled{true};
    std::atomic_bool unlimitedWrathEnabled{false};
    std::atomic_bool unlimitedUltimateEnabled{false};
    std::atomic_bool unlimitedSynergyEnabled{false};
    std::atomic_bool finiteStandardAmmoEnabled{false};
    std::atomic_int standardAmmoCapacity{200};
    std::atomic_bool grappleRangeEnabled{false};
    std::atomic<float> grappleRangeMultiplier{2.0f};
    std::atomic_int infiniteDoubleJumpMode{0};
    std::atomic_bool reduceAmmoEnabled{defaults::ReduceAmmo};
    std::atomic_bool unlimitedAmmoEnabled{false};
    std::atomic<float> ammoConsumptionDivisor{2.0f};
    std::atomic_bool reduceHotstreakEnabled{false};
    std::atomic_bool unlimitedHotstreakEnabled{false};
    std::atomic<float> hotstreakDecreaseDivisor{2.0f};
    std::atomic_bool potionRechargeEnabled{false};
    std::atomic_bool unlimitedPotionsEnabled{false};
    std::atomic_bool unlimitedHealthEnabled{false};
    std::atomic_bool unlimitedGlideEnabled{false};
    std::atomic_bool unlimitedHorseSprintEnabled{false};
    std::atomic<float> potionRechargeMultiplier{2.0f};
    // V1.24 read-only time of a verified positive local Synergy additive gain.
    std::atomic_ullong lastNativeSynergyPositiveTick{0};

    std::atomic<float> fovDegrees{90.0f};
    std::atomic<float> thirdPersonFovDegrees{110.0f};
    std::atomic<float> thirdPersonCoopFovDegrees{110.0f};
    std::atomic<float> cameraZoomPercent{0.0f};
    std::atomic<float> cameraPitchDegrees{0.0f};
    std::atomic<float> cameraYawDegrees{0.0f};
    std::atomic<float> cameraHeightOffset{0.0f};
    std::atomic<float> thirdPersonDistanceMultiplier{0.15f};
    std::atomic<float> thirdPersonPitchDegrees{-12.0f};
    std::atomic<float> thirdPersonHeightOffset{0.0f};
    std::atomic<float> tpsUltimateDistanceMultiplier{defaults::UltimateDistance};
    std::atomic<float> tpsUltimateHeightOffset{defaults::UltimateHeight};
    std::atomic<float> tpsHorseDistanceMultiplier{defaults::HorseDistance};
    std::atomic<float> tpsHorseHeightOffset{defaults::HorseHeight};
    std::atomic<float> tpsFootAnchorOffset{0.0f};
    std::atomic<float> cameraStickSpeed{180.0f};
    std::atomic<float> cameraOrbitYawDegrees{0.0f};
    std::atomic<float> cameraOrbitPitchDegrees{0.0f};
    std::atomic<float> movementSpeedMultiplier{1.50f};
    std::atomic<float> pistolDamageMultiplier{2.00f};
    std::atomic<float> meleeDamageMultiplier{2.00f};
    std::atomic<float> jumpHeightMultiplier{1.25f};
    std::atomic<float> glideDurationMultiplier{10.00f};
    std::atomic<float> horseSpeedMultiplier{1.25f};
    std::atomic<float> horseSprintSpeedMultiplier{1.25f};
    std::atomic<float> horseSprintDurationMultiplier{5.00f};
    std::atomic<float> hotstreakChargeMultiplier{2.00f};
    std::atomic<float> wrathChargeMultiplier{defaults::WrathGain};
    std::atomic<float> ultimateChargeMultiplier{defaults::UltimateGain};
    std::atomic<float> synergyChargeMultiplier{defaults::SynergyGain};
};

State& Get();
void Publish(const Snapshot& snapshot);

} // namespace dg::runtime
