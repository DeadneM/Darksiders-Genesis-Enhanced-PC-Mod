#pragma once
namespace dg::glide {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
void Report();
}
