#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::caltrops_appearance {
using LogFn=void(*)(const char*);
void Initialize(LogFn,bool exactExecutable);
void OnLivePlayer(const local_players::Context&);
bool Ready();
unsigned LoadedMask();
void Shutdown();
}
