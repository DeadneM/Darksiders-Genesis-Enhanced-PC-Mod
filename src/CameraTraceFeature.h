#pragma once
namespace dg::camera_trace {
using LogFn = void (*)(const char*);
void Install(LogFn log);
}
