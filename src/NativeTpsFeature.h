#pragma once
#include "LocalPlayerOwnership.h"
#include "NativeObjectIdentity.h"
namespace dg::native_tps {
struct Vec3 { float x=0,y=0,z=0; };
struct MouseRay { Vec3 origin,direction; bool mounted=false,fireHeld=false,ultimate=false; };
using RayProvider=bool(*)(void* actor,MouseRay& ray);
using LogFn=void(*)(const char*);
bool Install(RayProvider provider,LogFn log);
bool TargetReady();
bool CinematicActive(unsigned playerIndex=0);
bool IsWorldPaused(void* liveWorldContext);
// Called only from native game-thread callbacks, never from Present/XInput.
void SampleNativeStickSwap();
native_objects::Identity CaptureUiEvent(void* liveObject);
void ObserveUiEvent(const native_objects::Identity& identity);
bool NativeStickSwap();
bool NativeStickSwapKnown();
bool RadialActive(unsigned playerIndex=0);
// Synchronous native collision query using the exact existing scoped helper.
bool TraceCameraTarget(const local_players::Context&,const MouseRay&,Vec3& target,bool& hit,const Vec3* source=nullptr);
bool OnGameThread();
// Same query, with the observed native blocking actor identity. No new collision policy.
bool TraceCameraContact(const local_players::Context&,const MouseRay&,Vec3&,bool&,native_objects::Identity&,const Vec3* source=nullptr);
void Report();
}
