#include <windows.h>
#include <cwchar>
#include <atomic>

#include <MinHook.h>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_skipLogosOnce = INIT_ONCE_STATIC_INIT;

using CreateFileWFn = HANDLE (WINAPI*)(
    LPCWSTR,
    DWORD,
    DWORD,
    LPSECURITY_ATTRIBUTES,
    DWORD,
    DWORD,
    HANDLE
);

static CreateFileWFn g_originalCreateFileW = nullptr;
static std::atomic_bool g_skipLogosInstalled{false};
static std::atomic_bool g_skipLogosEnabled{true};
static std::atomic_long g_createFileCalls{0};
static std::atomic_long g_mp4Calls{0};
static std::atomic_long g_blockedCalls{0};

static const wchar_t* BaseName(const wchar_t* path) {
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

static bool EndsWithMp4(const wchar_t* path) {
    const wchar_t* base = BaseName(path);
    if (!base) {
        return false;
    }

    const size_t length = wcslen(base);
    return
        length >= 4 &&
        _wcsicmp(base + length - 4, L".mp4") == 0;
}

static bool IsTargetLogo(const wchar_t* path) {
    const wchar_t* base = BaseName(path);
    if (!base) {
        return false;
    }

    return
        _wcsicmp(base, L"THQ_LogoBasic.mp4") == 0 ||
        _wcsicmp(base, L"AS_LogoBasic.mp4") == 0;
}

static HANDLE WINAPI HookCreateFileW(
    LPCWSTR fileName,
    DWORD desiredAccess,
    DWORD shareMode,
    LPSECURITY_ATTRIBUTES securityAttributes,
    DWORD creationDisposition,
    DWORD flagsAndAttributes,
    HANDLE templateFile
) {
    g_createFileCalls.fetch_add(1, std::memory_order_relaxed);

    if (EndsWithMp4(fileName)) {
        g_mp4Calls.fetch_add(1, std::memory_order_relaxed);
    }

    if (g_skipLogosEnabled.load(std::memory_order_relaxed) &&
        IsTargetLogo(fileName)) {
        g_blockedCalls.fetch_add(1, std::memory_order_relaxed);
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
            templateFile
        )
        : INVALID_HANDLE_VALUE;
}

static bool ReadSkipLogosEnabledFromIni() {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self ||
        !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) {
        return true;
    }

    wchar_t* slash = wcsrchr(modulePath, L'\\');
    if (!slash) {
        return true;
    }
    *(slash + 1) = L'\0';

    wchar_t iniPath[MAX_PATH]{};
    lstrcpyW(iniPath, modulePath);
    lstrcatW(iniPath, L"DarksidersGenesisMod.ini");

    return GetPrivateProfileIntW(
        L"Features",
        L"SkipLogos",
        1,
        iniPath
    ) != 0;
}

static BOOL CALLBACK InstallEarlySkipLogos(
    PINIT_ONCE,
    PVOID,
    PVOID*
) {
    g_skipLogosEnabled.store(
        ReadSkipLogosEnabledFromIni(),
        std::memory_order_relaxed
    );

    const MH_STATUS init = MH_Initialize();
    if (init != MH_OK &&
        init != MH_ERROR_ALREADY_INITIALIZED) {
        return TRUE;
    }

    HMODULE kernelBase = GetModuleHandleW(L"KernelBase.dll");
    if (!kernelBase) {
        kernelBase = LoadLibraryW(L"KernelBase.dll");
    }

    void* target = kernelBase
        ? reinterpret_cast<void*>(
            GetProcAddress(kernelBase, "CreateFileW")
        )
        : nullptr;

    if (!target) {
        HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
        target = kernel32
            ? reinterpret_cast<void*>(
                GetProcAddress(kernel32, "CreateFileW")
            )
            : nullptr;
    }

    if (!target) {
        return TRUE;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<void*>(&HookCreateFileW),
        reinterpret_cast<void**>(&g_originalCreateFileW)
    );

    if (status != MH_OK &&
        status != MH_ERROR_ALREADY_CREATED) {
        return TRUE;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK &&
        status != MH_ERROR_ENABLED) {
        return TRUE;
    }

    g_skipLogosInstalled.store(
        true,
        std::memory_order_release
    );

    return TRUE;
}

static void EnsureEarlySkipLogos() {
    InitOnceExecuteOnce(
        &g_skipLogosOnce,
        InstallEarlySkipLogos,
        nullptr,
        nullptr
    );
}

static BOOL CALLBACK InitRealDxgi(PINIT_ONCE, PVOID, PVOID*) {
    wchar_t systemDir[MAX_PATH]{};
    if (!GetSystemDirectoryW(systemDir, MAX_PATH)) {
        return FALSE;
    }

    wchar_t path[MAX_PATH]{};
    lstrcpyW(path, systemDir);
    lstrcatW(path, L"\\dxgi.dll");
    g_realDxgi = LoadLibraryW(path);
    return g_realDxgi != nullptr;
}

static HMODULE RealDxgi() {
    InitOnceExecuteOnce(
        &g_dxgiOnce,
        InitRealDxgi,
        nullptr,
        nullptr
    );
    return g_realDxgi;
}

static BOOL CALLBACK LoadAsiPlugins(PINIT_ONCE, PVOID, PVOID*) {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self ||
        !GetModuleFileNameW(
            g_self,
            modulePath,
            MAX_PATH)) {
        return TRUE;
    }

    wchar_t* slash = wcsrchr(modulePath, L'\\');
    if (!slash) {
        return TRUE;
    }
    *(slash + 1) = L'\0';

    wchar_t pattern[MAX_PATH]{};
    lstrcpyW(pattern, modulePath);
    lstrcatW(pattern, L"*.asi");

    WIN32_FIND_DATAW fd{};
    HANDLE find = FindFirstFileW(pattern, &fd);
    if (find == INVALID_HANDLE_VALUE) {
        return TRUE;
    }

    do {
        if ((fd.dwFileAttributes &
             FILE_ATTRIBUTE_DIRECTORY) != 0) {
            continue;
        }

        wchar_t asiPath[MAX_PATH]{};
        lstrcpyW(asiPath, modulePath);
        lstrcatW(asiPath, fd.cFileName);
        LoadLibraryW(asiPath);
    } while (FindNextFileW(find, &fd));

    FindClose(find);
    return TRUE;
}

static void EnsureAsisLoaded() {
    InitOnceExecuteOnce(
        &g_asiOnce,
        LoadAsiPlugins,
        nullptr,
        nullptr
    );
}

template <typename T>
static T Resolve(const char* name) {
    HMODULE real = RealDxgi();
    if (!real) {
        return nullptr;
    }
    return reinterpret_cast<T>(
        GetProcAddress(real, name)
    );
}

extern "C" __declspec(dllexport)
BOOL WINAPI DGSkipLogosInstalled() {
    return g_skipLogosInstalled.load(
        std::memory_order_acquire
    ) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
BOOL WINAPI DGSkipLogosEnabled() {
    return g_skipLogosEnabled.load(
        std::memory_order_relaxed
    ) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
void WINAPI DGSetSkipLogosEnabled(BOOL enabled) {
    g_skipLogosEnabled.store(
        enabled != FALSE,
        std::memory_order_relaxed
    );
}

extern "C" __declspec(dllexport)
LONG WINAPI DGSkipLogosCreateFileCalls() {
    return g_createFileCalls.load(
        std::memory_order_relaxed
    );
}

extern "C" __declspec(dllexport)
LONG WINAPI DGSkipLogosMp4Calls() {
    return g_mp4Calls.load(
        std::memory_order_relaxed
    );
}

extern "C" __declspec(dllexport)
LONG WINAPI DGSkipLogosBlockedCalls() {
    return g_blockedCalls.load(
        std::memory_order_relaxed
    );
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory(
    REFIID riid,
    void** ppFactory
) {
    EnsureEarlySkipLogos();
    EnsureAsisLoaded();

    using Fn = HRESULT(WINAPI*)(REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory");
    return fn ? fn(riid, ppFactory) : E_FAIL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory1(
    REFIID riid,
    void** ppFactory
) {
    EnsureEarlySkipLogos();
    EnsureAsisLoaded();

    using Fn = HRESULT(WINAPI*)(REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory1");
    return fn ? fn(riid, ppFactory) : E_FAIL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory2(
    UINT flags,
    REFIID riid,
    void** ppFactory
) {
    EnsureEarlySkipLogos();
    EnsureAsisLoaded();

    using Fn = HRESULT(
        WINAPI*)(UINT, REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory2");
    return fn
        ? fn(flags, riid, ppFactory)
        : E_NOTIMPL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIGetDebugInterface1(
    UINT flags,
    REFIID riid,
    void** ppDebug
) {
    using Fn = HRESULT(
        WINAPI*)(UINT, REFIID, void**);
    Fn fn = Resolve<Fn>("DXGIGetDebugInterface1");
    return fn
        ? fn(flags, riid, ppDebug)
        : E_NOINTERFACE;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIDeclareAdapterRemovalSupport() {
    using Fn = HRESULT(WINAPI*)();
    Fn fn = Resolve<Fn>(
        "DXGIDeclareAdapterRemovalSupport"
    );
    return fn ? fn() : E_NOTIMPL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIDisableVBlankVirtualization() {
    using Fn = HRESULT(WINAPI*)();
    Fn fn = Resolve<Fn>(
        "DXGIDisableVBlankVirtualization"
    );
    return fn ? fn() : E_NOTIMPL;
}

BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD reason,
    LPVOID
) {
    if (reason == DLL_PROCESS_ATTACH) {
        g_self = module;
        DisableThreadLibraryCalls(module);
    }
    return TRUE;
}
