#pragma once
#include "ForeignWrathFeature.h"
namespace dg::pistol_visual {struct PoseContext;}
namespace dg::foreign_rampage {
using PlayFn=void(*)(void*,void*,float,foreign_wrath::Name);
void Initialize(foreign_wrath::LogFn,bool exactExecutable);
void ObserveObject(uintptr_t);
bool InvalidateAssets();
bool AssetsReady();
int Status();
bool IsClass(uintptr_t);
bool Prepare(void*,uintptr_t& abilityClass);
bool HandlePlay(PlayFn,void* ability,void* montage,float rate,foreign_wrath::Name section);
void Observe(const local_players::Context&);
// Reuses the existing owned animation root evaluation; no second update,
// root-motion application or animation notify dispatch.
bool Evaluate(void* node,pistol_visual::PoseContext*);
void Shutdown();
}
