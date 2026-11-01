#pragma once
#include <atomic>
#include "DefaultSettings.h"

namespace dg::camera_blur {
inline constexpr float MaxDistanceMeters=5000.0f;
// A generic profile retains native rendering. User-approved menu defaults
// below customize each view separately; disabling always takes precedence.
struct Profile {
    bool disabled=false,custom=false;
    float distanceMeters=10.0f,amount=0.6f;
};
inline constexpr Profile NativeDefaults{false,defaults::CustomDOF,10.0f,defaults::NativeDOFAmount};
inline constexpr Profile TpsDefaults{false,defaults::CustomDOF,defaults::TPSDOFDistance,defaults::TPSDOFAmount};
struct SharedProfile {
    std::atomic_bool disabled{false},custom{false};
    std::atomic<float> distanceMeters{10.0f},amount{0.6f};
    SharedProfile()=default;
    explicit SharedProfile(const Profile& p):disabled(p.disabled),custom(p.custom),distanceMeters(p.distanceMeters),amount(p.amount){}
    Profile Get() const {return {disabled.load(),custom.load(),distanceMeters.load(),amount.load()};}
    void Publish(const Profile& p){
        distanceMeters.store(p.distanceMeters);amount.store(p.amount);
        custom.store(p.custom);disabled.store(p.disabled);
    }
};
}
