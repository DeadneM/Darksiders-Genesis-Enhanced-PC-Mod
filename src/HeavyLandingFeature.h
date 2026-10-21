#pragma once
#include "LocalPlayerOwnership.h"
#include <cstdint>
namespace dg::heavy_landing {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void OnLivePlayer(const local_players::Context&,uint32_t generation,bool focusedGameplay);
void Shutdown();
}
