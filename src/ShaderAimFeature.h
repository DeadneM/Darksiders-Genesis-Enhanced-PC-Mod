#pragma once
#include <d3d11.h>
#include <cstdint>

namespace dg::aim_shader {
using LogFn = void(*)(const char*, ...);
struct Telemetry {
    bool hooksReady = false;
    unsigned matchedShaders = 0;
    unsigned interceptedDraws = 0;
    unsigned blockedDraws = 0;
};
// Install on the EXISTING early D3D11 probe vtables, before the real game
// creates its pixel shaders. Do not initialize ImGui, touch aim or game data.
bool Install(ID3D11Device* device, ID3D11DeviceContext* context, LogFn log);
Telemetry GetTelemetry();
void Shutdown();
}
