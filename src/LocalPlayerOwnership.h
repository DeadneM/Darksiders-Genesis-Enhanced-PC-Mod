#pragma once
#include <cstdint>

namespace dg::local_players {
enum class PoseKind { Foot, Horse, Ultimate };
struct Context {
    int index=-1,controllerId=-1,localCount=0;
    uintptr_t localPlayer=0,controller=0,character=0,pawn=0,world=0;
    PoseKind kind=PoseKind::Foot;
};
struct Pose { float x=0,y=0,z=0,yaw=0; };
struct Slot {
    uintptr_t localPlayer=0,character=0;
    int controllerId=-1;
    uint64_t tick=0;
};
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
// Synchronous reads from live native arguments only. Returned pointers must
// never be retained for a later dereference on the render/input threads.
bool ResolveActor(void* liveActor,Context& context);
bool ResolveCamera(void* liveCamera,Context& context);
bool ReadPose(const Context& context,Pose& pose);
Slot PublishedSlot(unsigned index);
bool CoopActive();
int PrimaryControllerId();
}
