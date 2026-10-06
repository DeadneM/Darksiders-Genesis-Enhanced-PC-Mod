#include <windows.h>
#include <cwchar>
#include <atomic>
#include <cstring>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;

constexpr DWORD kSupportedSizeOfImage = 0x03DDF000;
constexpr uintptr_t kStartupScreenSelectorRva = 0x00253546;

static constexpr BYTE kNativeSelectorBytes[] = {
    0xE8,0x45,0x56,0x3B,0x01,
    0x48,0x8B,0x10,0x48,0x8B,0xC8,0xFF,0x52,0x30,
    0x84,0xC0,0x74,0x50
};

static constexpr BYTE kSkipSelectorBytes[] = {
    0xE9,0xB2,0x00,0x00,0x00
};

static std::atomic_bool g_skipLogosTargetValid{false};
static std::atomic_bool g_skipLogosEnabled{true};
static std::atomic_bool g_skipLogosPatched{false};

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

    const int revision = GetPrivateProfileIntW(
        L"Meta",
        L"ConfigRevision",
        0,
        iniPath
    );

    if (revision < 1809) {
        return true;
    }

    return GetPrivateProfileIntW(
        L"Features",
        L"SkipLogos",
        1,
        iniPath
    ) != 0;
}

static BYTE* ResolveValidatedSelector() {
    HMODULE mainModule = GetModuleHandleW(nullptr);
    if (!mainModule) {
        return nullptr;
    }

    BYTE* base = reinterpret_cast<BYTE*>(mainModule);
    const auto* dos =
        reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return nullptr;
    }

    const auto* nt =
        reinterpret_cast<const IMAGE_NT_HEADERS64*>(
            base + dos->e_lfanew
        );

    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
        nt->OptionalHeader.SizeOfImage != kSupportedSizeOfImage) {
        return nullptr;
    }

    BYTE* selector = base + kStartupScreenSelectorRva;

    // This is the GetMoviePlayer() call immediately before the engine chooses
    // between EarlyStartupMovie and CustomSplashScreen. Accept either the
    // exact retail call or our five-byte JMP, while requiring the rest of the
    // audited selector block to remain byte-for-byte identical.
    const bool native =
        std::memcmp(
            selector,
            kNativeSelectorBytes,
            sizeof(kSkipSelectorBytes)
        ) == 0;
    const bool patched =
        std::memcmp(
            selector,
            kSkipSelectorBytes,
            sizeof(kSkipSelectorBytes)
        ) == 0;

    if ((!native && !patched) ||
        std::memcmp(
            selector + sizeof(kSkipSelectorBytes),
            kNativeSelectorBytes + sizeof(kSkipSelectorBytes),
            sizeof(kNativeSelectorBytes) - sizeof(kSkipSelectorBytes)
        ) != 0) {
        return nullptr;
    }

    return selector;
}

static bool ApplySkipLogosPatch(bool enabled) {
    BYTE* selector = ResolveValidatedSelector();
    if (!selector) {
        g_skipLogosTargetValid.store(false, std::memory_order_release);
        g_skipLogosPatched.store(false, std::memory_order_release);
        return false;
    }

    g_skipLogosTargetValid.store(true, std::memory_order_release);

    const BYTE* desired =
        enabled ? kSkipSelectorBytes : kNativeSelectorBytes;

    if (std::memcmp(
            selector,
            desired,
            sizeof(kSkipSelectorBytes)) != 0) {
        DWORD oldProtect = 0;
        if (!VirtualProtect(
                selector,
                sizeof(kSkipSelectorBytes),
                PAGE_EXECUTE_READWRITE,
                &oldProtect)) {
            return false;
        }

        std::memcpy(
            selector,
            desired,
            sizeof(kSkipSelectorBytes)
        );
        FlushInstructionCache(
            GetCurrentProcess(),
            selector,
            sizeof(kSkipSelectorBytes)
        );

        DWORD ignored = 0;
        VirtualProtect(
            selector,
            sizeof(kSkipSelectorBytes),
            oldProtect,
            &ignored
        );
    }

    g_skipLogosEnabled.store(enabled, std::memory_order_relaxed);
    g_skipLogosPatched.store(enabled, std::memory_order_release);
    return true;
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
        !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) {
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
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
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
BOOL WINAPI DGSkipLogosTargetValid() {
    return g_skipLogosTargetValid.load(
        std::memory_order_acquire
    ) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
BOOL WINAPI DGSkipLogosPatched() {
    return g_skipLogosPatched.load(
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
BOOL WINAPI DGSetSkipLogosEnabled(BOOL enabled) {
    return ApplySkipLogosPatch(
        enabled != FALSE
    ) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory(REFIID riid, void** ppFactory) {
    EnsureAsisLoaded();
    using Fn = HRESULT(WINAPI*)(REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory");
    return fn ? fn(riid, ppFactory) : E_FAIL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory1(REFIID riid, void** ppFactory) {
    EnsureAsisLoaded();
    using Fn = HRESULT(WINAPI*)(REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory1");
    return fn ? fn(riid, ppFactory) : E_FAIL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory2(UINT flags, REFIID riid, void** ppFactory) {
    EnsureAsisLoaded();
    using Fn = HRESULT(WINAPI*)(UINT, REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory2");
    return fn ? fn(flags, riid, ppFactory) : E_NOTIMPL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIGetDebugInterface1(UINT flags, REFIID riid, void** ppDebug) {
    using Fn = HRESULT(WINAPI*)(UINT, REFIID, void**);
    Fn fn = Resolve<Fn>("DXGIGetDebugInterface1");
    return fn ? fn(flags, riid, ppDebug) : E_NOINTERFACE;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIDeclareAdapterRemovalSupport() {
    using Fn = HRESULT(WINAPI*)();
    Fn fn = Resolve<Fn>("DXGIDeclareAdapterRemovalSupport");
    return fn ? fn() : E_NOTIMPL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIDisableVBlankVirtualization() {
    using Fn = HRESULT(WINAPI*)();
    Fn fn = Resolve<Fn>("DXGIDisableVBlankVirtualization");
    return fn ? fn() : E_NOTIMPL;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        g_self = module;
        DisableThreadLibraryCalls(module);

        // dxgi.dll is loaded before the game entry point. Skip only the
        // FEngineLoop selector that chooses EarlyStartupMovie or the fallback
        // CustomSplashScreen; normal cutscenes and Skip Intro remain separate.
        const bool enabled =
            ReadSkipLogosEnabledFromIni();
        ApplySkipLogosPatch(enabled);
    }

    return TRUE;
}
