#pragma once

namespace dg::camera_audit {

// Read-only native PE reference audit. Never inspects or writes live UObject
// addresses. Invoked only after the exact supported EXE SHA-256 is verified.
using LogFn = void(*)(const char*);
void Run(LogFn log);

} // namespace dg::camera_audit
