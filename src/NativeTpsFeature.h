#pragma once
namespace dg::native_tps {
struct Vec3 { float x=0,y=0,z=0; };
struct MouseRay { Vec3 origin,direction; bool mounted=false,fireHeld=false,ultimate=false; };
using RayProvider=bool(*)(void* actor,MouseRay& ray);
using LogFn=void(*)(const char*);
bool Install(RayProvider provider,LogFn log);
bool TargetReady();
bool CinematicActive();
bool IsWorldPaused(void* liveWorldContext);
// Called only from native game-thread callbacks, never from Present/XInput.
void SampleNativeStickSwap();
void ObserveUiEvent(void* liveObject);
bool NativeStickSwap();
bool NativeStickSwapKnown();
bool RadialActive();
bool OnGameThread();
void Report();
}
