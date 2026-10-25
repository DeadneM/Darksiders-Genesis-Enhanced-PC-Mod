#pragma once
#include "ForeignWrathFeature.h"
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
void Shutdown();
}
