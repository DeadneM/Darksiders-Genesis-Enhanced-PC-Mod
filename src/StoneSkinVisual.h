#pragma once
#include "ForeignWrathFeature.h"
namespace dg::stone_visual {
void Initialize(foreign_wrath::LogFn,bool exactExecutable);
void ObserveObject(uintptr_t);
bool NeedsLoaderContext();
void Cast(const local_players::Context&);
void Observe(const local_players::Context&);
bool AssetsReady();
bool NeedsAssetScan(const local_players::Context&);
bool InvalidateAssets();
void Shutdown();
}
