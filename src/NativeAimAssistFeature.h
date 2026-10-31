#pragma once
#include "ProjectileAimFeature.h"
namespace dg::native_assist {
void Initialize(projectile_aim::LogFn,bool exactExecutable,projectile_aim::RayProvider);
void Report();
}
