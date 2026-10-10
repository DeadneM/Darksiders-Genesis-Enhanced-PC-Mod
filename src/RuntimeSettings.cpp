#include "RuntimeSettings.h"

namespace dg::runtime {

namespace {
State g_state{};
}

State& Get() {
    return g_state;
}

void Publish(const Snapshot& s) {
    g_state.toggleHudEnabled.store(s.toggleHudEnabled, std::memory_order_relaxed);
    g_state.hideReticle.store(s.hideReticle, std::memory_order_relaxed);
    g_state.fovEnabled.store(s.fovEnabled, std::memory_order_relaxed);
    g_state.movementSpeedEnabled.store(s.movementSpeedEnabled, std::memory_order_relaxed);
    g_state.actionRecoveryEnabled.store(s.actionRecoveryEnabled, std::memory_order_relaxed);
    g_state.skipLogosEnabled.store(s.skipLogosEnabled, std::memory_order_relaxed);
    g_state.skipIntroEnabled.store(s.skipIntroEnabled, std::memory_order_relaxed);
    g_state.skipWarningEnabled.store(s.skipWarningEnabled, std::memory_order_relaxed);
    g_state.thirdPersonEnabled.store(s.thirdPersonEnabled, std::memory_order_relaxed);
    g_state.thirdPersonFovEnabled.store(s.thirdPersonFovEnabled);
    g_state.tpsFollowPlayer.store(s.tpsFollowPlayer);
    g_state.tpsSuppressNativeRightStick.store(s.tpsSuppressNativeRightStick);
    g_state.cameraOrbitInputEnabled.store(s.cameraOrbitInputEnabled, std::memory_order_relaxed);
    g_state.cameraMouseOrbitEnabled.store(s.cameraMouseOrbitEnabled);
    g_state.cameraControllerOrbitEnabled.store(s.cameraControllerOrbitEnabled);
    g_state.tpsMouseKeyboardStrafe.store(s.tpsMouseKeyboardStrafe);
    g_state.tpsControllerStrafe.store(s.tpsControllerStrafe);
    g_state.pistolDamageEnabled.store(s.pistolDamageEnabled, std::memory_order_relaxed);
    g_state.meleeDamageEnabled.store(s.meleeDamageEnabled, std::memory_order_relaxed);
    g_state.jumpHeightEnabled.store(s.jumpHeightEnabled, std::memory_order_relaxed);
    g_state.glideDurationEnabled.store(s.glideDurationEnabled, std::memory_order_relaxed);
    g_state.horseSpeedEnabled.store(s.horseSpeedEnabled, std::memory_order_relaxed);
    g_state.horseSprintSpeedEnabled.store(s.horseSprintSpeedEnabled, std::memory_order_relaxed);
    g_state.horseSprintDurationEnabled.store(s.horseSprintDurationEnabled, std::memory_order_relaxed);
    g_state.hotstreakChargeEnabled.store(s.hotstreakChargeEnabled, std::memory_order_relaxed);

    g_state.fovDegrees.store(s.fovDegrees, std::memory_order_relaxed);
    g_state.thirdPersonFovDegrees.store(s.thirdPersonFovDegrees);
    g_state.cameraZoomPercent.store(s.cameraZoomPercent, std::memory_order_relaxed);
    g_state.cameraPitchDegrees.store(s.cameraPitchDegrees, std::memory_order_relaxed);
    g_state.cameraYawDegrees.store(s.cameraYawDegrees, std::memory_order_relaxed);
    g_state.cameraHeightOffset.store(s.cameraHeightOffset, std::memory_order_relaxed);
    g_state.thirdPersonDistanceMultiplier.store(s.thirdPersonDistanceMultiplier, std::memory_order_relaxed);
    g_state.thirdPersonPitchDegrees.store(s.thirdPersonPitchDegrees, std::memory_order_relaxed);
    g_state.thirdPersonHeightOffset.store(s.thirdPersonHeightOffset, std::memory_order_relaxed);
    g_state.tpsFootAnchorOffset.store(s.tpsFootAnchorOffset);
    g_state.cameraMouseSensitivity.store(s.cameraMouseSensitivity, std::memory_order_relaxed);
    g_state.cameraStickSpeed.store(s.cameraStickSpeed, std::memory_order_relaxed);
    g_state.movementSpeedMultiplier.store(s.movementSpeedMultiplier, std::memory_order_relaxed);
    g_state.actionRecoveryDelayMs.store(s.actionRecoveryDelayMs, std::memory_order_relaxed);
    g_state.pistolDamageMultiplier.store(s.pistolDamageMultiplier, std::memory_order_relaxed);
    g_state.meleeDamageMultiplier.store(s.meleeDamageMultiplier, std::memory_order_relaxed);
    g_state.jumpHeightMultiplier.store(s.jumpHeightMultiplier, std::memory_order_relaxed);
    g_state.glideDurationMultiplier.store(s.glideDurationMultiplier, std::memory_order_relaxed);
    g_state.horseSpeedMultiplier.store(s.horseSpeedMultiplier, std::memory_order_relaxed);
    g_state.horseSprintSpeedMultiplier.store(s.horseSprintSpeedMultiplier, std::memory_order_relaxed);
    g_state.horseSprintDurationMultiplier.store(s.horseSprintDurationMultiplier, std::memory_order_relaxed);
    g_state.hotstreakChargeMultiplier.store(s.hotstreakChargeMultiplier, std::memory_order_relaxed);
}

} // namespace dg::runtime
