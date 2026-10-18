#pragma once
namespace dg::interaction_hud {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
void Shutdown();
bool Ready();
}
