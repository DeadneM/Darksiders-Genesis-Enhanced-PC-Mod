#include "CameraTraceFeature.h"
#include <windows.h>
#include <MinHook.h>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace dg::camera_trace {
namespace {
constexpr uintptr_t kViewRva = 0x16F9790;
constexpr uintptr_t kArmRva = 0x6F57B0;
constexpr unsigned char kViewBytes[] = {
  0x48,0x8B,0xC4,0x55,0x57,0x41,0x56,0x48,0x8D,0xA8,0x18,0xFD,0xFF,0xFF
};
constexpr unsigned char kArmBytes[] = {
  0x48,0x8B,0xC4,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xA8,0x98,0xFD,0xFF,0xFF
};
using ViewFn = void (*)(void*, float, void*);
using ArmFn = void (*)(void*, bool, bool, bool, float);
ViewFn g_view = nullptr;
ArmFn g_arm = nullptr;
LogFn g_log = nullptr;
std::atomic<uint32_t> g_viewCount{0}, g_armCount{0};
void Write(const char* s) { if (g_log && s) g_log(s); }
bool Sample(uint32_t n) { return n <= 12 || (n % 5000) == 0; }

void HookView(void* camera, float dt, void* outView) {
  if (g_view) g_view(camera, dt, outView);
  const uint32_t count = g_viewCount.fetch_add(1, std::memory_order_relaxed) + 1;
  if (!Sample(count)) return;
  float fov = -1.0f;
  if (outView) {
    float v = 0.0f;
    std::memcpy(&v, static_cast<const unsigned char*>(outView) + 0x18, sizeof(v));
    if (std::isfinite(v) && v > 1.0f && v < 179.0f) fov = v;
  }
  char line[220]{};
  sprintf_s(line, "CameraTrace V0.35: GetCameraView calls=%u camera=%p dt=%.5f FOV=%.2f READ_ONLY",
            count, camera, dt, fov);
  Write(line);
}
void HookArm(void* arm, bool trace, bool locLag, bool rotLag, float dt) {
  float distance = -1.0f;
  if (arm) {
    float v = 0.0f;
    std::memcpy(&v, static_cast<const unsigned char*>(arm) + 0x258, sizeof(v));
    if (std::isfinite(v) && v >= 0.0f && v < 100000.0f) distance = v;
  }
  if (g_arm) g_arm(arm, trace, locLag, rotLag, dt);
  const uint32_t count = g_armCount.fetch_add(1, std::memory_order_relaxed) + 1;
  if (!Sample(count)) return;
  char line[240]{};
  sprintf_s(line, "CameraTrace V0.35: UpdateDesiredArmLocation calls=%u arm=%p length=%.2f trace=%d lag=%d/%d dt=%.5f READ_ONLY",
            count, arm, distance, trace ? 1 : 0, locLag ? 1 : 0, rotLag ? 1 : 0, dt);
  Write(line);
}
bool InstallOne(uintptr_t base, uintptr_t rva, const unsigned char* signature,
                size_t length, void* detour, void** original, const char* label) {
  auto* target = reinterpret_cast<unsigned char*>(base + rva);
  if (std::memcmp(target, signature, length) != 0) {
    char s[160]{};
    sprintf_s(s, "CameraTrace V0.35: %s signature mismatch; SKIPPED", label);
    Write(s); return false;
  }
  const MH_STATUS created = MH_CreateHook(target, detour, original);
  if (created != MH_OK) {
    char s[160]{};
    sprintf_s(s, "CameraTrace V0.35: %s create failed=%d", label, int(created));
    Write(s); return false;
  }
  const MH_STATUS enabled = MH_EnableHook(target);
  if (enabled != MH_OK && enabled != MH_ERROR_ENABLED) {
    MH_RemoveHook(target);
    char s[160]{};
    sprintf_s(s, "CameraTrace V0.35: %s enable failed=%d", label, int(enabled));
    Write(s); return false;
  }
  char s[160]{};
  sprintf_s(s, "CameraTrace V0.35: %s READY RVA=0x%zX READ_ONLY", label, size_t(rva));
  Write(s); return true;
}
}
void Install(LogFn log) {
  g_log = log;
  // Caller first verifies exact executable SHA-256. No scan, no cached UObject.
  const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
  if (!base) return;
  const MH_STATUS status = MH_Initialize();
  if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED) {
    Write("CameraTrace V0.35: MinHook unavailable; no camera hooks");
    return;
  }
  const bool view = InstallOne(base, kViewRva, kViewBytes, sizeof(kViewBytes),
       reinterpret_cast<void*>(&HookView), reinterpret_cast<void**>(&g_view), "View");
  const bool arm = InstallOne(base, kArmRva, kArmBytes, sizeof(kArmBytes),
       reinterpret_cast<void*>(&HookArm), reinterpret_cast<void**>(&g_arm), "SpringArm");
  char s[160]{};
  sprintf_s(s, "CameraTrace V0.35: summary View=%d SpringArm=%d; no FOV/zoom/angle edits",
            view ? 1 : 0, arm ? 1 : 0);
  Write(s);
}
}
