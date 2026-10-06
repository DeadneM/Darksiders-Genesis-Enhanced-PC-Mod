#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <MinHook.h>

#include "StartupMoviesFeature.h"

#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cwchar>

namespace dg::startup_movies {
namespace {

using CreateFileWFn = HANDLE (WINAPI*)(
    LPCWSTR,
    DWORD,
    DWORD,
    LPSECURITY_ATTRIBUTES,
    DWORD,
    DWORD,
    HANDLE);
using GetFileAttributesWFn = DWORD (WINAPI*)(LPCWSTR);

CreateFileWFn g_originalCreateFileW = nullptr;
GetFileAttributesWFn g_originalGetFileAttributesW = nullptr;

std::atomic_bool g_installed{false};
std::atomic_bool g_enabled{false};
std::atomic_uint32_t g_blockedAttributeChecks{0};
std::atomic_uint32_t g_blockedOpens{0};

LogFn g_logger = nullptr;

void FeatureLog(const char* fmt, ...) {
    if (!g_logger) {
        return;
    }

    char buffer[512]{};
    va_list args;
    va_start(args, fmt);
    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    va_end(args);
    g_logger(buffer);
}

const wchar_t* BaseName(const wchar_t* path) {
    if (!path) {
        return nullptr;
    }

    const wchar_t* slash = wcsrchr(path, L'\\');
    const wchar_t* forward = wcsrchr(path, L'/');

    const wchar_t* last = slash;
    if (!last || (forward && forward > last)) {
        last = forward;
    }

    return last ? last + 1 : path;
}

bool IsStartupLogoPath(const wchar_t* path) {
    const wchar_t* base = BaseName(path);
    if (!base || !*base) {
        return false;
    }

    return _wcsicmp(base, L"THQ_LogoBasic.mp4") == 0 ||
           _wcsicmp(base, L"AS_LogoBasic.mp4") == 0;
}

HANDLE WINAPI HookCreateFileW(
    LPCWSTR fileName,
    DWORD desiredAccess,
    DWORD shareMode,
    LPSECURITY_ATTRIBUTES securityAttributes,
    DWORD creationDisposition,
    DWORD flagsAndAttributes,
    HANDLE templateFile
) {
    if (g_enabled.load() && IsStartupLogoPath(fileName)) {
        const std::uint32_t count = g_blockedOpens.fetch_add(1) + 1;
        if (count <= 2) {
            FeatureLog(
                "SkipLogos: blocked CreateFileW for %ls",
                BaseName(fileName)
            );
        }
        SetLastError(ERROR_FILE_NOT_FOUND);
        return INVALID_HANDLE_VALUE;
    }

    return g_originalCreateFileW
        ? g_originalCreateFileW(
              fileName,
              desiredAccess,
              shareMode,
              securityAttributes,
              creationDisposition,
              flagsAndAttributes,
              templateFile)
        : INVALID_HANDLE_VALUE;
}

DWORD WINAPI HookGetFileAttributesW(LPCWSTR fileName) {
    if (g_enabled.load() && IsStartupLogoPath(fileName)) {
        const std::uint32_t count = g_blockedAttributeChecks.fetch_add(1) + 1;
        if (count <= 2) {
            FeatureLog(
                "SkipLogos: hid file attributes for %ls",
                BaseName(fileName)
            );
        }
        SetLastError(ERROR_FILE_NOT_FOUND);
        return INVALID_FILE_ATTRIBUTES;
    }

    return g_originalGetFileAttributesW
        ? g_originalGetFileAttributesW(fileName)
        : INVALID_FILE_ATTRIBUTES;
}

FARPROC ResolveKernelFileApi(const char* name) {
    if (!name) {
        return nullptr;
    }

    if (HMODULE kernelBase = GetModuleHandleW(L"KernelBase.dll")) {
        if (FARPROC p = GetProcAddress(kernelBase, name)) {
            return p;
        }
    }

    if (HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll")) {
        return GetProcAddress(kernel32, name);
    }

    return nullptr;
}

bool InstallOne(
    const char* name,
    LPVOID hook,
    LPVOID* original
) {
    FARPROC target = ResolveKernelFileApi(name);
    if (!target) {
        FeatureLog("SkipLogos: %s target not found", name);
        return false;
    }

    const MH_STATUS createStatus = MH_CreateHook(
        reinterpret_cast<LPVOID>(target),
        hook,
        original
    );
    if (createStatus != MH_OK && createStatus != MH_ERROR_ALREADY_CREATED) {
        FeatureLog(
            "SkipLogos: %s create hook failed status=%d",
            name,
            static_cast<int>(createStatus)
        );
        return false;
    }

    const MH_STATUS enableStatus = MH_EnableHook(
        reinterpret_cast<LPVOID>(target)
    );
    if (enableStatus != MH_OK && enableStatus != MH_ERROR_ENABLED) {
        FeatureLog(
            "SkipLogos: %s enable hook failed status=%d",
            name,
            static_cast<int>(enableStatus)
        );
        return false;
    }

    return true;
}

} // namespace

bool Install(bool enabled, LogFn logger) {
    g_logger = logger;
    g_enabled.store(enabled);

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        FeatureLog(
            "SkipLogos: MinHook initialize failed status=%d",
            static_cast<int>(initStatus)
        );
        return false;
    }

    const bool attrReady = InstallOne(
        "GetFileAttributesW",
        reinterpret_cast<LPVOID>(&HookGetFileAttributesW),
        reinterpret_cast<LPVOID*>(&g_originalGetFileAttributesW)
    );

    const bool openReady = InstallOne(
        "CreateFileW",
        reinterpret_cast<LPVOID>(&HookCreateFileW),
        reinterpret_cast<LPVOID*>(&g_originalCreateFileW)
    );

    const bool ready = attrReady && openReady;
    g_installed.store(ready);

    FeatureLog(
        "SkipLogos: %s enabled=%d targets=THQ_LogoBasic.mp4,AS_LogoBasic.mp4",
        ready ? "READY" : "partial/unavailable",
        enabled ? 1 : 0
    );

    return ready;
}

void SetEnabled(bool enabled) {
    g_enabled.store(enabled);
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.installed = g_installed.load();
    t.enabled = g_enabled.load();
    t.blockedAttributeChecks = g_blockedAttributeChecks.load();
    t.blockedOpens = g_blockedOpens.load();
    return t;
}

} // namespace dg::startup_movies
