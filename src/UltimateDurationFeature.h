#pragma once
namespace dg::ultimate_duration {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
}
