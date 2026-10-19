#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::mounted_holsters {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& owner);
bool Ready();
void Shutdown();
}
