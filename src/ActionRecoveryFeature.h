#pragma once
namespace dg::recovery {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
// Bounded read-only observation of the original native getter after a dash.
void ObserveNativeLook(void* actor,float x,float y);
void Report();
}
