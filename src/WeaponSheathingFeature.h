#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::sheathing {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& context,uint32_t generation);
bool RequestSheathe();
bool Ready();
void Shutdown();
}
