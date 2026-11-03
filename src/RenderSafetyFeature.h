#pragma once
#include <cstdint>
namespace dg::render_safety {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
uint64_t AvoidedClears();
void Report(); // Deferred reporting from Present; never log from the RHI hook.
}
