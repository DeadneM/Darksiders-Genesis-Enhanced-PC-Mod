#pragma once
#include "ForeignWrathFeature.h"
namespace dg::foreign_beam {
using PlayFn=void(*)(void*,void*,float,foreign_wrath::Name);
void Initialize(foreign_wrath::LogFn,bool exactExecutable);
void ObserveObject(uintptr_t);
bool AssetsReady();
bool InvalidateAssets();
int Status();
bool IsClass(uintptr_t);
bool Prepare(void* actor,uintptr_t& abilityClass);
bool HandlePlay(PlayFn,void* ability,void* montage,float rate,foreign_wrath::Name);
void Shutdown();
}
