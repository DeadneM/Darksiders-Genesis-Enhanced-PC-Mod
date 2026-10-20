#include "RuntimeSettings.h"

namespace dg::runtime {

namespace {
State g_state{};
}

State& Get() {
    return g_state;
}

void Publish(const Snapshot& s) {
    g_state.overlayMenuKey.store(s.overlayMenuKey,std::memory_order_relaxed);
    g_state.toggleHudEnabled.store(s.toggleHudEnabled, std::memory_order_relaxed);
    g_state.interactionHudScaleEnabled.store(s.interactionHudScaleEnabled);
    g_state.interactionHudScale.store(s.interactionHudScale);
    g_state.mountedHolstersEnabled.store(s.mountedHolstersEnabled);
    g_state.pistolRightModel.store(s.pistolRightModel);
    g_state.pistolLeftModel.store(s.pistolLeftModel);
    g_state.bonusMountSkinsEnabled.store(s.bonusMountSkinsEnabled);
    g_state.progressionSkinsEnabled.store(s.progressionSkinsEnabled);
    g_state.leviathanVisualEnabled.store(s.leviathanVisualEnabled);
    g_state.leviathanVisualMode.store(s.leviathanVisualMode);
    g_state.weaponSheathingEnabled.store(s.weaponSheathingEnabled);
    g_state.sheathNeverEnabled.store(s.sheathNeverEnabled);
    g_state.sheathDelaySeconds.store(s.sheathDelaySeconds);
    g_state.sheathStrifeMeleeNeverEnabled.store(s.sheathStrifeMeleeNeverEnabled);
    g_state.sheathStrifeMeleeDelaySeconds.store(s.sheathStrifeMeleeDelaySeconds);
    g_state.sheathWarNeverEnabled.store(s.sheathWarNeverEnabled);
    g_state.sheathWarDelaySeconds.store(s.sheathWarDelaySeconds);
    g_state.fovEnabled.store(s.fovEnabled, std::memory_order_relaxed);
    g_state.movementSpeedEnabled.store(s.movementSpeedEnabled, std::memory_order_relaxed);
    g_state.skipLogosEnabled.store(s.skipLogosEnabled, std::memory_order_relaxed);
    g_state.skipIntroEnabled.store(s.skipIntroEnabled, std::memory_order_relaxed);
    g_state.skipWarningEnabled.store(s.skipWarningEnabled, std::memory_order_relaxed);
    g_state.thirdPersonEnabled.store(s.thirdPersonEnabled, std::memory_order_relaxed);
    g_state.thirdPersonFovEnabled.store(s.thirdPersonFovEnabled);
    g_state.tpsLensFocusEnabled.store(s.tpsLensFocusEnabled);
    g_state.nativeBlur.Publish(s.nativeBlur);
    g_state.tpsBlur.Publish(s.tpsBlur);
    g_state.cameraOrbitInputEnabled.store(s.cameraOrbitInputEnabled, std::memory_order_relaxed);
    g_state.cameraControllerOrbitEnabled.store(s.cameraControllerOrbitEnabled);
    g_state.tpsCameraStickMode.store(s.tpsCameraStickMode);
    g_state.tpsMousePrimaryButton.store(s.tpsMousePrimaryButton,std::memory_order_relaxed);
    g_state.tpsMouseSecondaryButton.store(s.tpsMouseSecondaryButton,std::memory_order_relaxed);
    g_state.tpsFirePrimaryMask.store(s.tpsFirePrimaryMask,std::memory_order_relaxed);
    g_state.tpsFireSecondaryMask.store(s.tpsFireSecondaryMask,std::memory_order_relaxed);
    g_state.tpsWheelPadMask.store(s.tpsWheelPadMask,std::memory_order_relaxed);
    g_state.tpsGrenadeButtonMask.store(s.tpsGrenadeButtonMask,std::memory_order_relaxed);
    g_state.horsePadButtonA.store(s.horsePadButtonA,std::memory_order_relaxed);
    g_state.horsePadButtonB.store(s.horsePadButtonB,std::memory_order_relaxed);

    g_state.tpsCutsceneAutoSuspend.store(s.tpsCutsceneAutoSuspend);
    if(!s.tpsCutsceneAutoSuspend||!s.thirdPersonEnabled)
        g_state.tpsCutsceneSuspended.store(false);
    g_state.aimIndicatorHideMode.store(s.aimIndicatorHideMode, std::memory_order_relaxed);
    g_state.tps3DAimEnabled.store(s.tps3DAimEnabled,std::memory_order_relaxed);
    g_state.tpsAimPitchOffsetDegrees.store(s.tpsAimPitchOffsetDegrees,std::memory_order_relaxed);
    g_state.tpsMountedAimMode.store(s.tpsMountedAimMode,std::memory_order_relaxed);
    g_state.pistolDamageEnabled.store(s.pistolDamageEnabled, std::memory_order_relaxed);
    g_state.meleeDamageEnabled.store(s.meleeDamageEnabled, std::memory_order_relaxed);
    g_state.jumpHeightEnabled.store(s.jumpHeightEnabled, std::memory_order_relaxed);
    g_state.glideDurationEnabled.store(s.glideDurationEnabled, std::memory_order_relaxed);
    g_state.horseEverywhereEnabled.store(s.horseEverywhereEnabled, std::memory_order_relaxed);
    g_state.horseAreaMode.store(s.horseAreaMode,std::memory_order_relaxed);
    g_state.horseSpeedEnabled.store(s.horseSpeedEnabled, std::memory_order_relaxed);
    g_state.horseSprintSpeedEnabled.store(s.horseSprintSpeedEnabled, std::memory_order_relaxed);
    g_state.horseSprintDurationEnabled.store(s.horseSprintDurationEnabled, std::memory_order_relaxed);
    g_state.hotstreakChargeEnabled.store(s.hotstreakChargeEnabled, std::memory_order_relaxed);
    g_state.wrathChargeEnabled.store(s.wrathChargeEnabled, std::memory_order_relaxed);
    g_state.ultimateChargeEnabled.store(s.ultimateChargeEnabled, std::memory_order_relaxed);
    g_state.ultimateDecreaseEnabled.store(s.ultimateDecreaseEnabled);
    g_state.ultimateInfiniteDurationEnabled.store(s.ultimateInfiniteDurationEnabled);
    g_state.ultimateDurationMultiplier.store(s.ultimateDurationMultiplier);
    g_state.synergyChargeEnabled.store(s.synergyChargeEnabled, std::memory_order_relaxed);
    g_state.unlimitedWrathEnabled.store(s.unlimitedWrathEnabled);
    g_state.unlimitedUltimateEnabled.store(s.unlimitedUltimateEnabled);
    g_state.unlimitedSynergyEnabled.store(s.unlimitedSynergyEnabled);
    g_state.finiteStandardAmmoEnabled.store(s.finiteStandardAmmoEnabled);
    g_state.standardAmmoCapacity.store(s.standardAmmoCapacity);
    g_state.grappleRangeEnabled.store(s.grappleRangeEnabled);
    g_state.grappleRangeMultiplier.store(s.grappleRangeMultiplier);
    g_state.infiniteDoubleJumpMode.store(s.infiniteDoubleJumpMode);
    g_state.reduceAmmoEnabled.store(s.reduceAmmoEnabled);
    g_state.unlimitedAmmoEnabled.store(s.unlimitedAmmoEnabled);
    g_state.ammoConsumptionDivisor.store(s.ammoConsumptionDivisor);
    g_state.reduceHotstreakEnabled.store(s.reduceHotstreakEnabled);
    g_state.unlimitedHotstreakEnabled.store(s.unlimitedHotstreakEnabled);
    g_state.hotstreakDecreaseDivisor.store(s.hotstreakDecreaseDivisor);
    g_state.potionRechargeEnabled.store(s.potionRechargeEnabled);
    g_state.unlimitedPotionsEnabled.store(s.unlimitedPotionsEnabled);
    g_state.unlimitedHealthEnabled.store(s.unlimitedHealthEnabled);
    g_state.unlimitedGlideEnabled.store(s.unlimitedGlideEnabled);
    g_state.unlimitedHorseSprintEnabled.store(s.unlimitedHorseSprintEnabled);
    g_state.potionRechargeMultiplier.store(s.potionRechargeMultiplier);

    g_state.fovDegrees.store(s.fovDegrees, std::memory_order_relaxed);
    g_state.thirdPersonFovDegrees.store(s.thirdPersonFovDegrees);
    g_state.thirdPersonCoopFovDegrees.store(s.thirdPersonCoopFovDegrees);
    g_state.cameraZoomPercent.store(s.cameraZoomPercent, std::memory_order_relaxed);
    g_state.cameraPitchDegrees.store(s.cameraPitchDegrees, std::memory_order_relaxed);
    g_state.cameraYawDegrees.store(s.cameraYawDegrees, std::memory_order_relaxed);
    g_state.cameraHeightOffset.store(s.cameraHeightOffset, std::memory_order_relaxed);
    g_state.thirdPersonDistanceMultiplier.store(s.thirdPersonDistanceMultiplier, std::memory_order_relaxed);
    g_state.thirdPersonPitchDegrees.store(s.thirdPersonPitchDegrees, std::memory_order_relaxed);
    g_state.thirdPersonHeightOffset.store(s.thirdPersonHeightOffset, std::memory_order_relaxed);
    g_state.tpsUltimateDistanceMultiplier.store(s.tpsUltimateDistanceMultiplier);
    g_state.tpsUltimateHeightOffset.store(s.tpsUltimateHeightOffset);
    g_state.tpsHorseDistanceMultiplier.store(s.tpsHorseDistanceMultiplier);
    g_state.tpsHorseHeightOffset.store(s.tpsHorseHeightOffset);
    g_state.tpsFootAnchorOffset.store(s.tpsFootAnchorOffset);
    g_state.cameraStickSpeed.store(s.cameraStickSpeed, std::memory_order_relaxed);
    g_state.movementSpeedMultiplier.store(s.movementSpeedMultiplier, std::memory_order_relaxed);
    g_state.pistolDamageMultiplier.store(s.pistolDamageMultiplier, std::memory_order_relaxed);
    g_state.meleeDamageMultiplier.store(s.meleeDamageMultiplier, std::memory_order_relaxed);
    g_state.jumpHeightMultiplier.store(s.jumpHeightMultiplier, std::memory_order_relaxed);
    g_state.glideDurationMultiplier.store(s.glideDurationMultiplier, std::memory_order_relaxed);
    g_state.horseSpeedMultiplier.store(s.horseSpeedMultiplier, std::memory_order_relaxed);
    g_state.horseSprintSpeedMultiplier.store(s.horseSprintSpeedMultiplier, std::memory_order_relaxed);
    g_state.horseSprintDurationMultiplier.store(s.horseSprintDurationMultiplier, std::memory_order_relaxed);
    g_state.hotstreakChargeMultiplier.store(s.hotstreakChargeMultiplier, std::memory_order_relaxed);
    g_state.wrathChargeMultiplier.store(s.wrathChargeMultiplier, std::memory_order_relaxed);
    g_state.ultimateChargeMultiplier.store(s.ultimateChargeMultiplier, std::memory_order_relaxed);
    g_state.synergyChargeMultiplier.store(s.synergyChargeMultiplier, std::memory_order_relaxed);
}

} // namespace dg::runtime
