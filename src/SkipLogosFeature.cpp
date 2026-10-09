#include "SkipLogosFeature.h"

#include "RuntimeSettings.h"

#include <windows.h>
#include <cstdio>

namespace dg::skip_logos {
namespace {

using BoolFn = BOOL (WINAPI*)();
using SetEnabledFn = BOOL (WINAPI*)(BOOL);

BoolFn g_targetValidFn = nullptr;
BoolFn g_patchedFn = nullptr;
BoolFn g_enabledFn = nullptr;
SetEnabledFn g_setEnabledFn = nullptr;
BoolFn g_warningAttemptedFn = nullptr;
BoolFn g_warningAppliedFn = nullptr;
SetEnabledFn g_setWarningFn = nullptr;

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

    g_targetValidFn =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosTargetValid")
        );
    g_patchedFn =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosPatched")
        );
    g_enabledFn =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosEnabled")
        );
    g_setEnabledFn =
        reinterpret_cast<SetEnabledFn>(
            GetProcAddress(proxy, "DGSetSkipLogosEnabled")
        );

    g_warningAttemptedFn =
        reinterpret_cast<BoolFn>(GetProcAddress(proxy, "DGSkipWarningAttempted"));
    g_warningAppliedFn =
        reinterpret_cast<BoolFn>(GetProcAddress(proxy, "DGSkipWarningApplied"));
    g_setWarningFn =
        reinterpret_cast<SetEnabledFn>(GetProcAddress(proxy, "DGSetSkipWarningEnabled"));
    return
        g_targetValidFn &&
        g_patchedFn &&
        g_enabledFn &&
        g_setEnabledFn;
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

    const bool applied =
        g_setEnabledFn(enabled ? TRUE : FALSE) != FALSE;

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
    if (!g_setEnabledFn && !ResolveProxyExports()) {
        return false;
    }

    return g_setEnabledFn(
        enabled ? TRUE : FALSE
    ) != FALSE;
}

bool ApplyWarning(bool enabled) {
    if (!g_setWarningFn && !ResolveProxyExports()) return false;
    return g_setWarningFn &&
        g_setWarningFn(enabled ? TRUE : FALSE) != FALSE;
}

Telemetry GetTelemetry() {
    Telemetry t{};

    const bool available =
        g_targetValidFn &&
        g_patchedFn &&
        g_enabledFn &&
        g_setEnabledFn;

    t.proxyAvailable = available;
    if (!available) {
        return t;
    }

    t.targetValid =
        g_targetValidFn() != FALSE;
    t.patched =
        g_patchedFn() != FALSE;
    t.enabled =
        g_enabledFn() != FALSE;
    if (g_warningAttemptedFn) t.warningAttempted = g_warningAttemptedFn() != FALSE;
    if (g_warningAppliedFn) t.warningApplied = g_warningAppliedFn() != FALSE;

    return t;
}

void Shutdown() {
    // The StartupScreens MoviePlayer attachment block is restored when disabled.
}

} // namespace dg::skip_logos
