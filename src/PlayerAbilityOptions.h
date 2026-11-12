#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::player_abilities {
using LogFn=void(*)(const char*);
void Initialize(LogFn,bool exactExecutable,bool finishBoundaryReady);
bool CounterReady();
bool SynergyReady();
void OnLivePlayer(const local_players::Context&,uint32_t generation,bool commandsAllowed);
// Observe the existing ProcessEvent boundary before native dispatch. No script edits.
void ObserveFinish(void* ability,void* function);
void Shutdown();
}
