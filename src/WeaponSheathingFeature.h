#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::sheathing {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void OnLivePlayer(const local_players::Context& context,uint32_t generation);
bool RequestSheathe(int playerIndex=-1); // -1: all local players; 0/1: owning controller only
bool Ready();
void Shutdown();
}
