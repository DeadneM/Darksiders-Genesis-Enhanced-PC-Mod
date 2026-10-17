#pragma once
#include <algorithm>

namespace dg::tps_camera {
struct Pitch { float boom, view; };
// Framing puts the camera above/behind the character. It is not an aiming
// depression angle. In 3D Camera Forward the actual rendered view and ray
// share orbit pitch: recenter is level, independently of the framing value.
// Simple/Vanilla Aim retains the complete previous camera pose.
inline Pitch CameraPitch(float framing, float orbit, bool aim3D, int aimMode) {
    const float boom=std::clamp(framing+orbit,-75.0f,65.0f);
    return {boom,aim3D&&aimMode==0?std::clamp(orbit,-75.0f,65.0f):boom};
}
}
