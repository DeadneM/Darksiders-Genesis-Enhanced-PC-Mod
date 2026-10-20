#pragma once
#include <atomic>

namespace dg::camera_blur {
inline constexpr float MaxDistanceMeters=5000.0f;
// Both views default to the validated, authored effect. Stored numeric values
// take effect only when custom is selected; disabling takes precedence.
struct Profile {
    bool disabled=false,custom=false;
    float distanceMeters=10.0f,amount=0.6f;
};
struct SharedProfile {
    std::atomic_bool disabled{false},custom{false};
    std::atomic<float> distanceMeters{10.0f},amount{0.6f};
    Profile Get() const {return {disabled.load(),custom.load(),distanceMeters.load(),amount.load()};}
    void Publish(const Profile& p){
        distanceMeters.store(p.distanceMeters);amount.store(p.amount);
        custom.store(p.custom);disabled.store(p.disabled);
    }
};
}
