#pragma once
#include "ForeignWrathFeature.h"
namespace dg::stone_fx {
using Event=void(*)(void*,void*,void*);
void Initialize(uintptr_t base,Event existingDispatcher);
void ObserveObject(uintptr_t);
bool AssetsReady();
bool Ready(int kind);
bool InvalidateAssets();
bool Spawn(const local_players::Context&,int kind); // base, molten, expiry
void Shutdown();
}
