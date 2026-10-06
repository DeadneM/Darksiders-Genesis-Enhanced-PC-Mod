#include "SkipLogosFeature.h"

#include "RuntimeSettings.h"

#include <windows.h>
#include <cstdio>

namespace dg::skip_logos {
namespace {

using InstalledFn = BOOL (WINAPI*)();
using EnabledFn = BOOL (WINAPI*)();
using SetEnabledFn = void (WINAPI*)(BOOL);
using CountFn = LONG (WINAPI*)();

InstalledFn g_installedFn = nullptr;
EnabledFn g_enabledFn = nullptr;
SetEnabledFn g_setEnabledFn = nullptr;
CountFn g_createFileCallsFn = nullptr;
CountFn g_mp4CallsFn = nullptr;
CountFn g_blockedCallsFn = nullptr;

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

    g_installedFn =
        reinterpret_cast<InstalledFn>(
            GetProcAddress(proxy, "DGSkipLogosInstalled")
        );
    g_enabledFn =
        reinterpret_cast<EnabledFn>(
            GetProcAddress(proxy, "DGSkipLogosEnabled")
        );
    g_setEnabledFn =
        reinterpret_cast<SetEnabledFn>(
            GetProcAddress(proxy, "DGSetSkipLogosEnabled")
        );
    g_createFileCallsFn =
        reinterpret_cast<CountFn>(
            GetProcAddress(proxy, "DGSkipLogosCreateFileCalls")
        );
    g_mp4CallsFn =
        reinterpret_cast<CountFn>(
            GetProcAddress(proxy, "DGSkipLogosMp4Calls")
        );
    g_blockedCallsFn =
        reinterpret_cast<CountFn>(
            GetProcAddress(proxy, "DGSkipLogosBlockedCalls")
        );

    return
        g_installedFn &&
        g_enabledFn &&
        g_setEnabledFn &&
        g_createFileCallsFn &&
        g_mp4CallsFn &&
        g_blockedCallsFn;
}

} // namespace

bool Initialize(LogFn logger) {
    g_logger = logger;

    if (!ResolveProxyExports()) {
        LogText("Skip Logos FILE: proxy exports unavailable");
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
        "Skip Logos FILE: proxy=%d installed=%d enabled=%d CreateFileW=%ld mp4=%ld blocked=%ld",
        t.proxyAvailable ? 1 : 0,
        t.installed ? 1 : 0,
        t.enabled ? 1 : 0,
        t.createFileCalls,
        t.mp4Calls,
        t.blocked
    );
    LogText(message);

    return t.proxyAvailable;
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
        g_installedFn &&
        g_enabledFn &&
        g_setEnabledFn &&
        g_createFileCallsFn &&
        g_mp4CallsFn &&
        g_blockedCallsFn;

    t.proxyAvailable = available;

    if (!available) {
        return t;
    }

    t.installed =
        g_installedFn() != FALSE;
    t.enabled =
        g_enabledFn() != FALSE;
    t.createFileCalls =
        g_createFileCallsFn();
    t.mp4Calls =
        g_mp4CallsFn();
    t.blocked =
        g_blockedCallsFn();

    return t;
}

void Shutdown() {
    // Proxy hook intentionally stays installed until process exit.
}

} // namespace dg::skip_logos
