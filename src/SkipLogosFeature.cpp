#include "SkipLogosFeature.h"

#include "RuntimeSettings.h"

#include <MinHook.h>

#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cwchar>

namespace dg::skip_logos {
namespace {

using MFCreateSourceResolverFn =
    HRESULT (WINAPI*)(void** resolver);

using CreateObjectFromURLFn =
    HRESULT (STDMETHODCALLTYPE*)(
        void* resolver,
        LPCWSTR url,
        DWORD flags,
        void* propertyStore,
        int* objectType,
        IUnknown** object
    );

MFCreateSourceResolverFn g_originalCreateSourceResolver = nullptr;
CreateObjectFromURLFn g_originalCreateObjectFromURL = nullptr;

void* g_createSourceResolverTarget = nullptr;
void* g_createObjectFromUrlTarget = nullptr;

std::atomic_bool g_installed{false};
std::atomic_bool g_resolverMethodHooked{false};
std::atomic_long g_resolverCreateCalls{0};
std::atomic_long g_urlCalls{0};
std::atomic_long g_blocked{0};
std::atomic_long g_loggedUrls{0};

SRWLOCK g_hookLock = SRWLOCK_INIT;
LogFn g_logger = nullptr;

void LogText(const char* text) {
    if (g_logger && text) {
        g_logger(text);
    }
}

const wchar_t* BaseName(const wchar_t* path) {
    if (!path) {
        return nullptr;
    }

    const wchar_t* base = path;
    for (const wchar_t* p = path; *p; ++p) {
        if (*p == L'\\' || *p == L'/') {
            base = p + 1;
        }
    }

    return base;
}

bool IsTargetLogo(const wchar_t* url) {
    const wchar_t* base = BaseName(url);
    if (!base || !*base) {
        return false;
    }

    return
        _wcsicmp(base, L"THQ_LogoBasic.mp4") == 0 ||
        _wcsicmp(base, L"AS_LogoBasic.mp4") == 0;
}

void LogUrlBounded(LPCWSTR url, bool target) {
    const LONG index =
        g_loggedUrls.fetch_add(1, std::memory_order_relaxed);

    if (index >= 8) {
        return;
    }

    char urlUtf8[768]{};
    if (url) {
        WideCharToMultiByte(
            CP_UTF8,
            0,
            url,
            -1,
            urlUtf8,
            static_cast<int>(sizeof(urlUtf8)),
            nullptr,
            nullptr
        );
    }

    char message[960]{};
    sprintf_s(
        message,
        "Skip Logos MF: URL call=%ld target=%d url=%s",
        index + 1,
        target ? 1 : 0,
        urlUtf8[0] ? urlUtf8 : "(null)"
    );
    LogText(message);
}

HRESULT STDMETHODCALLTYPE HookCreateObjectFromURL(
    void* resolver,
    LPCWSTR url,
    DWORD flags,
    void* propertyStore,
    int* objectType,
    IUnknown** object
) {
    g_urlCalls.fetch_add(1, std::memory_order_relaxed);

    const bool target =
        dg::runtime::Get().skipLogosEnabled.load(
            std::memory_order_relaxed
        ) &&
        IsTargetLogo(url);

    LogUrlBounded(url, target);

    if (target) {
        g_blocked.fetch_add(1, std::memory_order_relaxed);

        if (objectType) {
            *objectType = 0;
        }
        if (object) {
            *object = nullptr;
        }

        LogText("Skip Logos MF: blocked startup logo URL");
        return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
    }

    return g_originalCreateObjectFromURL
        ? g_originalCreateObjectFromURL(
            resolver,
            url,
            flags,
            propertyStore,
            objectType,
            object
        )
        : E_FAIL;
}

bool HookResolverMethod(void* resolver) {
    if (!resolver) {
        return false;
    }

    if (g_resolverMethodHooked.load(std::memory_order_acquire)) {
        return true;
    }

    AcquireSRWLockExclusive(&g_hookLock);

    if (g_resolverMethodHooked.load(std::memory_order_relaxed)) {
        ReleaseSRWLockExclusive(&g_hookLock);
        return true;
    }

    void** vtable = *reinterpret_cast<void***>(resolver);
    if (!vtable) {
        ReleaseSRWLockExclusive(&g_hookLock);
        return false;
    }

    // IMFSourceResolver:
    // 0 QueryInterface, 1 AddRef, 2 Release, 3 CreateObjectFromURL.
    void* target = vtable[3];
    if (!target) {
        ReleaseSRWLockExclusive(&g_hookLock);
        return false;
    }

    const MH_STATUS createStatus =
        MH_CreateHook(
            target,
            reinterpret_cast<void*>(
                &HookCreateObjectFromURL
            ),
            reinterpret_cast<void**>(
                &g_originalCreateObjectFromURL
            )
        );

    if (createStatus != MH_OK &&
        createStatus != MH_ERROR_ALREADY_CREATED) {
        ReleaseSRWLockExclusive(&g_hookLock);
        return false;
    }

    const MH_STATUS enableStatus =
        MH_EnableHook(target);

    if (enableStatus != MH_OK &&
        enableStatus != MH_ERROR_ENABLED) {
        ReleaseSRWLockExclusive(&g_hookLock);
        return false;
    }

    g_createObjectFromUrlTarget = target;
    g_resolverMethodHooked.store(
        true,
        std::memory_order_release
    );

    ReleaseSRWLockExclusive(&g_hookLock);

    LogText(
        "Skip Logos MF: IMFSourceResolver::CreateObjectFromURL hook READY"
    );
    return true;
}

HRESULT WINAPI HookMFCreateSourceResolver(void** resolver) {
    if (!g_originalCreateSourceResolver) {
        return E_FAIL;
    }

    const HRESULT hr =
        g_originalCreateSourceResolver(resolver);

    if (SUCCEEDED(hr) && resolver && *resolver) {
        g_resolverCreateCalls.fetch_add(
            1,
            std::memory_order_relaxed
        );
        HookResolverMethod(*resolver);
    }

    return hr;
}

} // namespace

bool Initialize(LogFn logger) {
    g_logger = logger;

    HMODULE mfplat = GetModuleHandleW(L"mfplat.dll");
    if (!mfplat) {
        mfplat = LoadLibraryW(L"mfplat.dll");
    }

    if (!mfplat) {
        LogText("Skip Logos MF: mfplat.dll unavailable");
        return false;
    }

    void* target = reinterpret_cast<void*>(
        GetProcAddress(
            mfplat,
            "MFCreateSourceResolver"
        )
    );

    if (!target) {
        LogText(
            "Skip Logos MF: MFCreateSourceResolver export unavailable"
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        LogText("Skip Logos MF: MinHook initialization failed");
        return false;
    }

    const MH_STATUS createStatus =
        MH_CreateHook(
            target,
            reinterpret_cast<void*>(
                &HookMFCreateSourceResolver
            ),
            reinterpret_cast<void**>(
                &g_originalCreateSourceResolver
            )
        );

    if (createStatus != MH_OK &&
        createStatus != MH_ERROR_ALREADY_CREATED) {
        LogText(
            "Skip Logos MF: MFCreateSourceResolver hook creation failed"
        );
        return false;
    }

    const MH_STATUS enableStatus =
        MH_EnableHook(target);

    if (enableStatus != MH_OK &&
        enableStatus != MH_ERROR_ENABLED) {
        LogText(
            "Skip Logos MF: MFCreateSourceResolver hook enable failed"
        );
        return false;
    }

    g_createSourceResolverTarget = target;
    g_installed.store(true, std::memory_order_release);

    LogText(
        "Skip Logos MF: MFCreateSourceResolver hook READY"
    );
    return true;
}

Telemetry GetTelemetry() {
    Telemetry telemetry{};
    telemetry.installed =
        g_installed.load(std::memory_order_acquire);
    telemetry.resolverMethodHooked =
        g_resolverMethodHooked.load(
            std::memory_order_acquire
        );
    telemetry.resolverCreateCalls =
        g_resolverCreateCalls.load(
            std::memory_order_relaxed
        );
    telemetry.urlCalls =
        g_urlCalls.load(std::memory_order_relaxed);
    telemetry.blocked =
        g_blocked.load(std::memory_order_relaxed);
    return telemetry;
}

void Shutdown() {
    if (g_createObjectFromUrlTarget) {
        MH_DisableHook(g_createObjectFromUrlTarget);
    }

    if (g_createSourceResolverTarget) {
        MH_DisableHook(g_createSourceResolverTarget);
    }

    g_installed.store(false);
    g_resolverMethodHooked.store(false);
}

} // namespace dg::skip_logos
