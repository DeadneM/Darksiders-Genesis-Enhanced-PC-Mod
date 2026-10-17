#pragma once
#include "LocalPlayerOwnership.h"
#include "NativeTpsFeature.h"
namespace dg::projectile_aim {
using LogFn=void(*)(const char*);
using RayProvider=bool(*)(const local_players::Context&,native_tps::MouseRay&);
void Initialize(LogFn log,bool exactExecutable,RayProvider provider);
void Report();
}
