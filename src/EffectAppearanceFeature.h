#pragma once
#include "LocalPlayerOwnership.h"
#include "EffectAppearanceSettings.h"
namespace dg::effect_appearance {
using LogFn=void(*)(const char*);
void Initialize(LogFn,bool exactExecutable);
void OnLivePlayer(const local_players::Context&);
bool Ready();
unsigned LoadedMask(unsigned family);
void* ShieldAsset(int choice,bool requestLoad);
bool ShieldLoaderReady();
bool ShieldLoadFailed(int choice);
bool SelectionLoadFailed(unsigned family);
void Shutdown();
}
