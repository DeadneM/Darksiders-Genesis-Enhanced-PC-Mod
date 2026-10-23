#pragma once
#include "LocalPlayerOwnership.h"

namespace dg::wrath_slots {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
int ForeignStatus(int choice=6);
void OnLivePlayer(const local_players::Context& owner);
void Shutdown();
}
