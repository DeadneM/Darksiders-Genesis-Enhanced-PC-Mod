#pragma once
namespace dg::recovery {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
// Bounded read-only observation of the original native getter after a dash.
void ObserveControllerInput(unsigned id,short lx,short ly,short rx,short ry,bool swapKnown,bool swap);
void ObserveNativeLook(void* actor,float x,float y);
void Report();
}
