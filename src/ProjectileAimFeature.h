#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::projectile_aim {
using LogFn=void(*)(const char*);
using RotationProvider=bool(*)(const local_players::Context&,float& pitch,float& yaw);
void Initialize(LogFn log,bool exactExecutable,RotationProvider provider);
void Report();
}
