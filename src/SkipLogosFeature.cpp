#include "SkipLogosFeature.h"

#include "RuntimeSettings.h"

#include <windows.h>
#include <cstdio>

namespace dg::skip_logos {
namespace {

using BoolFn = BOOL (WINAPI*)();
using SetEnabledFn = void (WINAPI*)(BOOL);
using CountFn = LONG (WINAPI*)();

BoolFn g_targetValidFn = nullptr;
BoolFn g_installedFn = nullptr;
BoolFn g_enabledFn = nullptr;
SetEnabledFn g_setEnabledFn = nullptr;
CountFn g_setupCallsFn = nullptr;
CountFn g_skippedCallsFn = nullptr;

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
    g_installedFn =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosInstalled")
        );
    g_enabledFn =
        reinterpret_cast<BoolFn>(
            GetProcAddress(proxy, "DGSkipLogosEnabled")
        );
    g_setEnabledFn =
        reinterpret_cast<SetEnabledFn>(
            GetProcAddress(proxy, "DGSetSkipLogosEnabled")
        );
    g_setupCallsFn =
        reinterpret_cast<CountFn>(
            GetProcAddress(proxy, "DGSkipLogosSetupCalls")
        );
    g_skippedCallsFn =
        reinterpret_cast<CountFn>(
            GetProcAddress(proxy, "DGSkipLogosSkippedCalls")
        );

    return
        g_targetValidFn &&
        g_installedFn &&
        g_enabledFn &&
        g_setEnabledFn &&
        g_setupCallsFn &&
        g_skippedCallsFn;
}

} // namespace

bool Initialize(LogFn logger) {
    g_logger = logger;

    if (!ResolveProxyExports()) {
        LogText("Skip Logos NATIVE: proxy exports unavailable");
        return false;
    }

    const bool enabled =
        dg::runtime::Get().skipLogosEnabled.load(
            std::memory_order_relaxed
        );
    g_setEnabledFn(enabled ? TRUE : FALSE);

    const Telemetry t = GetTelemetry();

    char message[256]{};
    sprintf_s(
        message,
        "Skip Logos NATIVE: proxy=%d target=%d installed=%d enabled=%d setupCalls=%ld skipped=%ld RVA=0x160BC50",
        t.proxyAvailable ? 1 : 0,
        t.targetValid ? 1 : 0,
        t.installed ? 1 : 0,
        t.enabled ? 1 : 0,
        t.setupCalls,
        t.skippedCalls
    );
    LogText(message);

    return t.proxyAvailable && t.targetValid && t.installed;
}

bool Apply(bool enabled) {
    if (!g_setEnabledFn && !ResolveProxyExports()) {
        return false;
    }

    g_setEnabledFn(enabled ? TRUE : FALSE);
    return true;
}

Telemetry GetTelemetry() {
    Telemetry t{};

    const bool available =
        g_targetValidFn &&
        g_installedFn &&
        g_enabledFn &&
        g_setEnabledFn &&
        g_setupCallsFn &&
        g_skippedCallsFn;

    t.proxyAvailable = available;
    if (!available) {
        return t;
    }

    t.targetValid = g_targetValidFn() != FALSE;
    t.installed = g_installedFn() != FALSE;
    t.enabled = g_enabledFn() != FALSE;
    t.setupCalls = g_setupCallsFn();
    t.skippedCalls = g_skippedCallsFn();
    return t;
}

void Shutdown() {
    // Native proxy hook remains installed until process exit.
}

} // namespace dg::skip_logos
