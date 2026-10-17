#pragma once
namespace dg::recovery {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Shutdown();
void Report();
}
