#pragma once

namespace dg::engine_ini {

using LogFn = void(*)(const char*);

void Initialize(LogFn logger);

// Queues the embedded helper. It writes the final saved choice to Engine.ini
// only after the game exits; valid values remain 0..4.
bool QueueGraphicsAdapter(int adapter);


} // namespace dg::engine_ini
