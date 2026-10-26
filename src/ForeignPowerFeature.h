#pragma once
#include "ForeignWrathFeature.h"
namespace dg::foreign_power {
using PlayFn=void(*)(void*,void*,float,foreign_wrath::Name);
void Initialize(foreign_wrath::LogFn,bool exactExecutable);
int Status(int choice);
bool AssetsReady();
bool InvalidateAssets();
void ObserveObject(uintptr_t,unsigned kind);
bool Prepare(void* actor,uintptr_t& abilityClass,int choice);
bool IsClass(uintptr_t);
bool HandlePlay(PlayFn,void* ability,void* montage,float rate,foreign_wrath::Name);
void Shutdown();
}
