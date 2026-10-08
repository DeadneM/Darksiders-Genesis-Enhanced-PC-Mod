#pragma once

namespace dg::engine_ini {

using LogFn = void(*)(const char*);

void Initialize(LogFn logger);

// Writes [SystemSettings] r.GraphicsAdapter=N to the game's Engine.ini.
// Valid values are 0..4. Existing unrelated Engine.ini content is preserved.
bool ApplyGraphicsAdapter(int adapter);

int GetLastAppliedGraphicsAdapter();

} // namespace dg::engine_ini
