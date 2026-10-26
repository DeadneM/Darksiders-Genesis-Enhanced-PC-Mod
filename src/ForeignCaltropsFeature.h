#pragma once
#include "ForeignWrathFeature.h"

namespace dg::foreign_caltrops {
using Name=foreign_wrath::Name;
using LogFn=foreign_wrath::LogFn;
using PlayFn=void(*)(void*,void*,float,Name);
void Initialize(LogFn,bool exactExecutable);
int Status();
bool IsClass(uintptr_t);
bool AssetsReady();
bool InvalidateAssets();
void ObserveObject(uintptr_t,unsigned kind);
bool Prepare(void* actor,uintptr_t& abilityClass);
// True only when an identified, owned War cast was handled. Other native
// players/powers remain the caller's responsibility, without argument changes.
bool HandlePlay(PlayFn,void* ability,void* montage,float rate,Name section);
void Shutdown();
}
