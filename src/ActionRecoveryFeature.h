#pragma once
namespace dg::recovery {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
// Existing live player-camera callback, also available when no native aim
// ability remains to call a look getter. Independent of Recovery/TPS.
void OnLivePlayer(void* actor);
void Report();
}
