#pragma once
namespace dg::recovery {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
// Game-thread callback after the original native look getter. Repairs a
// missing held-AIM lifecycle after ground dash, independent of Recovery/TPS.
void OnNativeLook(void* actor);
void Report();
}
