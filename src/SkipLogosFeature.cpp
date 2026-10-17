#include "SkipLogosFeature.h"

#include "RuntimeSettings.h"

#include <windows.h>
#include <cstdio>
#include <atomic>

namespace dg::skip_logos {
namespace {

using BoolFn = BOOL (WINAPI*)();
using SetEnabledFn = BOOL (WINAPI*)(BOOL);

std::atomic<BoolFn> g_targetValidFn{nullptr};
std::atomic<BoolFn> g_patchedFn{nullptr};
std::atomic<BoolFn> g_enabledFn{nullptr};
std::atomic<SetEnabledFn> g_setEnabledFn{nullptr};

LogFn g_logger = nullptr;

void LogText(const char* text) {
    if (g_logger && text) {
        g_logger(text);
    }
}

bool ResolveProxyExports() {
    HMODULE proxy = GetModuleHandleW(L"dxgi.dll");
    if (!proxy) {
        return false;
    }

    const auto targetValid =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosTargetValid")
        );
    const auto patched =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosPatched")
        );
    const auto currentEnabled =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosEnabled")
        );
    const auto setEnabled =
        reinterpret_cast<SetEnabledFn>(
            GetProcAddress(proxy, "DGSetSkipLogosEnabled")
        );

    if(!targetValid||!patched||!currentEnabled||!setEnabled)return false;
    // Never publish null exports, even if another caller retries resolution.
    g_targetValidFn.store(targetValid,std::memory_order_release);
    g_patchedFn.store(patched,std::memory_order_release);
    g_enabledFn.store(currentEnabled,std::memory_order_release);
    g_setEnabledFn.store(setEnabled,std::memory_order_release);
    return true;
}

} // namespace

bool Initialize(LogFn logger) {
    g_logger = logger;

    if (!ResolveProxyExports()) {
        LogText("Skip Logos STARTUPSCREENS_ATTACH_VALIDATED: proxy exports unavailable");
        return false;
    }

    const bool enabled =
        dg::runtime::Get().skipLogosEnabled.load(
            std::memory_order_relaxed
        );

    const bool applied = Apply(enabled);

    const Telemetry t = GetTelemetry();

    char message[256]{};
    sprintf_s(
        message,
        "Skip Logos V0.52 NAME_SUBSTITUTION: proxy=%d target=%d patched=%d enabled=%d copyRVA=0x25FF31 attachUNCHANGED=0x260244",
        t.proxyAvailable ? 1 : 0,
        t.targetValid ? 1 : 0,
        t.patched ? 1 : 0,
        t.enabled ? 1 : 0
    );
    LogText(message);
    LogText(enabled
        ? "Skip Logos V0.52: exact THQ + AS movie names substituted; startup effect on next launch"
        : "Skip Logos V0.52: native startup retained (feature OFF)");

    return
        applied &&
        t.proxyAvailable &&
        t.targetValid &&
        (t.patched == enabled);
}

bool Apply(bool enabled) {
    auto setEnabled=g_setEnabledFn.load(std::memory_order_acquire);
    if(!setEnabled){
        if(!ResolveProxyExports())return false;
        setEnabled=g_setEnabledFn.load(std::memory_order_acquire);
    }
    return setEnabled&&setEnabled(enabled?TRUE:FALSE)!=FALSE;
}



Telemetry GetTelemetry() {
    Telemetry t{};

    const auto targetValid=g_targetValidFn.load(std::memory_order_acquire);
    const auto patched=g_patchedFn.load(std::memory_order_acquire);
    const auto currentEnabled=g_enabledFn.load(std::memory_order_acquire);
    const auto setEnabled=g_setEnabledFn.load(std::memory_order_acquire);
    if(!targetValid||!patched||!currentEnabled||!setEnabled)return t;
    t.proxyAvailable=true;
    t.targetValid=targetValid()!=FALSE;
    t.patched=patched()!=FALSE;
    t.enabled=currentEnabled()!=FALSE;

    return t;
}

void Shutdown() {
    // The StartupScreens MoviePlayer attachment block is restored when disabled.
}

} // namespace dg::skip_logos
