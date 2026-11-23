#pragma once
#include <cstddef>

namespace dg::render_quality {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready(size_t index);
void ApplyOnGameThread();
}
