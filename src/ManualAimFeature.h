#pragma once
#include "LocalPlayerOwnership.h"
#include <cstdint>

namespace dg::manual_aim {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
// Physical controller sampling publishes values only. Native calls stay on
// the owning player's synchronous game-thread callback.
void Publish(unsigned index,uintptr_t character,uint32_t generation,
             int controllerId,bool held,uint64_t tick);
void OnLivePlayer(const local_players::Context& owner,uint32_t generation);
void Shutdown();
}
