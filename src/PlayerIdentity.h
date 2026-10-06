#pragma once

namespace dg::player {

struct IdentityTelemetry {
    bool validated = false;
    void* character = nullptr;
    void* movement = nullptr;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float jumpZ = 0.0f;
    float doubleJumpZ = 0.0f;
    float glideDuration = 0.0f;
};

bool IsValidatedLocalPlayer(
    void* character,
    void* movementComponent);

IdentityTelemetry GetIdentityTelemetry();
void ClearIdentity();

} // namespace dg::player
