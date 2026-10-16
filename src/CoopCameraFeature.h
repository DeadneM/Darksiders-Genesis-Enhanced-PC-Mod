#pragma once
#include "LocalPlayerOwnership.h"
#include <windows.h>
#include <Xinput.h>

namespace dg::coop_camera {
using LogFn=void(*)(const char*);
void Initialize(LogFn log);
// All native object reads happen synchronously in these native callbacks.
void ApplyView(void* camera,void* outView,const local_players::Context& owner);
bool ObserveArm(void* liveArm,float nativeLength);
// Render/input thread: only published identities/values, no UObject reads.
void UpdateInput(const XINPUT_GAMEPAD* physical,bool focused,float dt,
    int recenterMask,int suspendMask,int suspendDelayMs);
bool FilterPad(DWORD controllerId,XINPUT_GAMEPAD& pad);
bool AdjustLook(void* actor,float& a,float& b,float& c,bool rotation);
bool ShotRotation(const local_players::Context& owner,float& pitch,float& yaw);
}
