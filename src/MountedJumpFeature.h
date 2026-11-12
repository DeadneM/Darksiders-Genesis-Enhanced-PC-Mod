#pragma once
#include "LocalPlayerOwnership.h"
#include <cstdint>
namespace dg::mounted_jump {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool focused,bool commandsAllowed=true);
bool Ready();
bool AirWalkReady();
unsigned SoundAssetsReady();
}
