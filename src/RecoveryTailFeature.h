#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::recovery_tail {
using LogFn=void(*)(const char*);
void Initialize(LogFn,bool exactExecutable);
bool Ready();
void OnLivePlayer(const local_players::Context&,uint32_t generation,bool focusedGameplay);
void Shutdown();
}
