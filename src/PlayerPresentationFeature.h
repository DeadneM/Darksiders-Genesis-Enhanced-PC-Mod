#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::presentation {
using LogFn=void(*)(const char*);
using FocusFn=bool(*)();
void Initialize(LogFn,bool exactExecutable,FocusFn);
void OnLivePlayer(const local_players::Context&);
bool Ready();
void Shutdown();
}
