#pragma once
#include "ForeignWrathFeature.h"
namespace dg::stone_visual {
void Initialize(foreign_wrath::LogFn,bool exactExecutable);
void ObserveObject(uintptr_t);
void Observe(const local_players::Context&);
bool AssetsReady();
bool InvalidateAssets();
void Shutdown();
}
