#pragma once
#include "ProjectileAimFeature.h"
namespace dg::native_assist {
void Initialize(projectile_aim::LogFn,bool exactExecutable,projectile_aim::RayProvider);
int AmmoKind(void* weapon);
bool Target(const local_players::Context&,void* weapon,const native_tps::MouseRay&,const native_tps::Vec3& origin,native_tps::Vec3& point);
void Report();
}
