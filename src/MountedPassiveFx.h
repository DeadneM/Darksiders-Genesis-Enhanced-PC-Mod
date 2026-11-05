#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::mounted_fx {
void Initialize(local_players::LogFn,bool exactExecutable);
void OnLivePlayer(const local_players::Context&);
void Shutdown();
}
