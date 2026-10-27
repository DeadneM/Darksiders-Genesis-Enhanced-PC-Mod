#pragma once
#include "ForeignWrathFeature.h"
namespace dg::pistol_visual {struct PoseContext;}
namespace dg::geyser_static {
void Initialize(foreign_wrath::LogFn,bool exactExecutable);
void ObserveSequence(uintptr_t);
bool AssetsReady();
bool InvalidateAssets();
void Begin(const local_players::Context&,void* ability,void* montage);
void Observe(const local_players::Context&);
bool Evaluate(void*,pistol_visual::PoseContext*);
void Shutdown();
}
