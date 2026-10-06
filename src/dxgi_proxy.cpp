#include <windows.h>
#include <cwchar>
#include <atomic>
#include <cstring>

#include <MinHook.h>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_skipLogosOnce = INIT_ONCE_STATIC_INIT;

constexpr DWORD kSupportedSizeOfImage = 0x03DDF000;
constexpr uintptr_t kSetupLoadingScreenFromIniRva = 0x0160BC50;

using SetupLoadingScreenFromIniFn = void(*)(void* self);
static SetupLoadingScreenFromIniFn g_originalSetupLoadingScreenFromIni = nullptr;

static std::atomic_bool g_skipLogosTargetValid{false};
static std::atomic_bool g_skipLogosInstalled{false};
static std::atomic_bool g_skipLogosEnabled{true};
static std::atomic_long g_setupCalls{0};
static std::atomic_long g_skippedCalls{0};

static bool ReadSkipLogosEnabledFromIni() {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self || !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) {
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
        L"Meta", L"ConfigRevision", 0, iniPath
    );

    if (revision < 1806) {
        return true;
    }

    return GetPrivateProfileIntW(
        L"Features", L"SkipLogos", 1, iniPath
    ) != 0;
}

static bool ValidateNativeTarget(BYTE*& outTarget) {
    outTarget = nullptr;

    HMODULE mainModule = GetModuleHandleW(nullptr);
    if (!mainModule) {
        return false;
    }

    BYTE* base = reinterpret_cast<BYTE*>(mainModule);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(
        base + dos->e_lfanew
    );
    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
        nt->OptionalHeader.SizeOfImage != kSupportedSizeOfImage) {
        return false;
    }

    static constexpr BYTE kExpectedPrefix[] = {
        0x48,0x8B,0xC4,0x55,0x57,0x41,0x54,0x41,
        0x56,0x41,0x57,0x48,0x8D,0x68,0xA1,0x48,
        0x81,0xEC,0xB0,0x00,0x00,0x00,0x48,0xC7,
        0x45,0xB7,0xFE,0xFF,0xFF,0xFF,0x48,0x89
    };

    BYTE* target = base + kSetupLoadingScreenFromIniRva;
    if (std::memcmp(
            target,
            kExpectedPrefix,
            sizeof(kExpectedPrefix)
        ) != 0) {
        return false;
    }

    outTarget = target;
    return true;
}

static void HookSetupLoadingScreenFromIni(void* self) {
    g_setupCalls.fetch_add(1, std::memory_order_relaxed);

    if (g_skipLogosEnabled.load(std::memory_order_relaxed)) {
        g_skippedCalls.fetch_add(1, std::memory_order_relaxed);
        return;
    }

    if (g_originalSetupLoadingScreenFromIni) {
        g_originalSetupLoadingScreenFromIni(self);
    }
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

    BYTE* target = nullptr;
    if (!ValidateNativeTarget(target)) {
        return TRUE;
    }

    g_skipLogosTargetValid.store(true, std::memory_order_release);

    const MH_STATUS init = MH_Initialize();
    if (init != MH_OK &&
        init != MH_ERROR_ALREADY_INITIALIZED) {
        return TRUE;
    }

    MH_STATUS status = MH_CreateHook(
        target,
        reinterpret_cast<void*>(&HookSetupLoadingScreenFromIni),
        reinterpret_cast<void**>(&g_originalSetupLoadingScreenFromIni)
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

    g_skipLogosInstalled.store(true, std::memory_order_release);
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
    InitOnceExecuteOnce(&g_dxgiOnce, InitRealDxgi, nullptr, nullptr);
    return g_realDxgi;
}

static BOOL CALLBACK LoadAsiPlugins(PINIT_ONCE, PVOID, PVOID*) {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self || !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) {
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
    InitOnceExecuteOnce(&g_asiOnce, LoadAsiPlugins, nullptr, nullptr);
}

template <typename T>
static T Resolve(const char* name) {
    HMODULE real = RealDxgi();
    if (!real) {
        return nullptr;
    }
    return reinterpret_cast<T>(GetProcAddress(real, name));
}

extern "C" __declspec(dllexport)
BOOL WINAPI DGSkipLogosTargetValid() {
    return g_skipLogosTargetValid.load(std::memory_order_acquire) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
BOOL WINAPI DGSkipLogosInstalled() {
    return g_skipLogosInstalled.load(std::memory_order_acquire) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
BOOL WINAPI DGSkipLogosEnabled() {
    return g_skipLogosEnabled.load(std::memory_order_relaxed) ? TRUE : FALSE;
}

extern "C" __declspec(dllexport)
void WINAPI DGSetSkipLogosEnabled(BOOL enabled) {
    g_skipLogosEnabled.store(enabled != FALSE, std::memory_order_relaxed);
}

extern "C" __declspec(dllexport)
LONG WINAPI DGSkipLogosSetupCalls() {
    return g_setupCalls.load(std::memory_order_relaxed);
}

extern "C" __declspec(dllexport)
LONG WINAPI DGSkipLogosSkippedCalls() {
    return g_skippedCalls.load(std::memory_order_relaxed);
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory(REFIID riid, void** ppFactory) {
    EnsureEarlySkipLogos();
    EnsureAsisLoaded();
    using Fn = HRESULT(WINAPI*)(REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory");
    return fn ? fn(riid, ppFactory) : E_FAIL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory1(REFIID riid, void** ppFactory) {
    EnsureEarlySkipLogos();
    EnsureAsisLoaded();
    using Fn = HRESULT(WINAPI*)(REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory1");
    return fn ? fn(riid, ppFactory) : E_FAIL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI CreateDXGIFactory2(UINT flags, REFIID riid, void** ppFactory) {
    EnsureEarlySkipLogos();
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
    }
    return TRUE;
}
