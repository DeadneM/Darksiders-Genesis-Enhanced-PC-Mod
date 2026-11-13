#pragma once
#include "TemporalAASettings.h"

namespace dg::temporal_aa {
using LogFn=void(*)(const char*);
void Initialize(LogFn log,bool exactExecutable);
bool Ready();
// Serviced by the existing Present callback, without a new detour or scan.
void Update(const Settings& settings);
void Restore();
}
