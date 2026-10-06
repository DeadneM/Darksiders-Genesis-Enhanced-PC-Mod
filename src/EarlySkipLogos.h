#pragma once

#include <windows.h>

namespace dg::skip_logos {

// Boot-only feature: install before UE4 startup and block only the two exact
// company-logo movie basenames. No runtime fallback path is intentionally kept.

bool InstallEarlyIatHooks();
const wchar_t* Status();
LONG BlockCount();

} // namespace dg::skip_logos
