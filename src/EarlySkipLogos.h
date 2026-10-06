#pragma once

#include <windows.h>

namespace dg::skip_logos {

bool InstallEarlyIatHooks();
const wchar_t* Status();
LONG BlockCount();

} // namespace dg::skip_logos
