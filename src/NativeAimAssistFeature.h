#pragma once
#include "ProjectileAimFeature.h"
namespace dg::native_assist {
void Initialize(projectile_aim::LogFn,bool exactExecutable,projectile_aim::RayProvider);
// Observe only a point actually consumed inside this synchronous native shot.
// No target identity or point survives the original call that produced it.
struct ShotScope {
    native_tps::Vec3 point{};
    bool used=false;
    ShotScope();
    ~ShotScope();
    void Close();
    ShotScope(const ShotScope&)=delete;
    ShotScope& operator=(const ShotScope&)=delete;
private:
    ShotScope* previous_=nullptr;
    bool open_=true;
};
void Report();
}
