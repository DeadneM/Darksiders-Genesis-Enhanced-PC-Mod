#pragma once

#include <windows.h>

namespace dg::skip_logos {

// V0.18B boot-only feature: install before UE4 startup and block only the two exact
// company-logo movie basenames. No runtime fallback path is intentionally kept.

bool InstallEarlyIatHooks(bool enabled);
const wchar_t* Status();
LONG BlockCount();
bool BootEnabled();

} // namespace dg::skip_logos
