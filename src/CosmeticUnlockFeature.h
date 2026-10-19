#pragma once
namespace dg::cosmetics {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
}
