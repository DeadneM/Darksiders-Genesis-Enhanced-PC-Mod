#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::shield_visual {
using LogFn=void(*)(const char*);
using VisibleFn=void(*)(void*,bool,unsigned char);
void Initialize(uintptr_t base,VisibleFn,LogFn);
bool OwnSpawnActive();
bool Ready();
void OnLivePlayer(const local_players::Context&);
void Shutdown();
}
