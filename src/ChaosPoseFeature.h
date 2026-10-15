#pragma once
#include <cstdint>

namespace dg::chaos_pose {
using LogFn=void(*)(const char*);
struct Pose { float x=0,y=0,z=0,yaw=0; };
void Initialize(LogFn logger,bool exactExecutable);
// Synchronous game-thread lookup using the live native camera as world context.
bool Sample(void* liveCamera,Pose& pose);
bool ActiveNow();
void Shutdown();
}
