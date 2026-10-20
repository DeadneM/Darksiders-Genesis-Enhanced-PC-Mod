#pragma once
#include "LocalPlayerOwnership.h"
namespace dg::cosmetics {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
// Bounded diagnostics only while the existing bonus-mount option is enabled.
void ObserveSelection(const local_players::Context& owner);
}
