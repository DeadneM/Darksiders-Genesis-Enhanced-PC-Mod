#pragma once
#include <cstdint>
namespace dg::local_players {struct Context;}
namespace dg::ranged_state {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& owner,uint32_t generation,bool gameplayAllowed);
void Shutdown();
}
