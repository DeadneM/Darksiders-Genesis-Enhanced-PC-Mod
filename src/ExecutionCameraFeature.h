#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::execution_camera {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
float Offset(const local_players::Context& liveOwner,float rootZ);
void Shutdown();
}
