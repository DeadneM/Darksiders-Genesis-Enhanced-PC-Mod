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
// Returns a shared TPS-toggle request; the main config owner applies it.
bool UpdateInput(const XINPUT_GAMEPAD* physical,bool focused,float dt,
    int recenterMask,int suspendMask,int suspendDelayMs,int toggleMask=0,int toggleHoldMs=500);
bool FilterPad(DWORD controllerId,XINPUT_GAMEPAD& pad,bool focusedGameplay=true);
bool AdjustLook(void* actor,float& a,float& b,float& c,bool rotation);
bool ShotRotation(const local_players::Context& owner,float& pitch,float& yaw);
}
