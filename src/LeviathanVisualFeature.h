#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::leviathan_visual {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& owner);
bool Ready();
unsigned LoadedMask();
void Shutdown();
}
