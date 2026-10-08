#include <windows.h>
#include <cwchar>
#include <atomic>
#include <cstring>
#include <cstdio>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;
static std::atomic_bool g_loaderLogInitialized{false};

// V0.39 loader-first log: intentionally independent of the ASI.
// If DarksidersGenesisMod.log does not begin with 0.39, inspect this file.
static void LogLoader(const wchar_t* message) {
    if (!g_self || !message) return;
    wchar_t path[MAX_PATH]{};
    if (!GetModuleFileNameW(g_self, path, MAX_PATH)) return;
    wchar_t* slash = wcsrchr(path, L'\\');
    if (!slash) return;
    *(slash + 1) = L'\0';
    if (wcslen(path) + 28 >= MAX_PATH) return;
    wcscat_s(path, L"DarksidersGenesisLoader.log");

    const bool first = !g_loaderLogInitialized.exchange(true);
    HANDLE file = CreateFileW(path, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
        first ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    SetFilePointer(file, 0, nullptr, FILE_END);

    SYSTEMTIME st{};
    GetLocalTime(&st);
    wchar_t line[1200]{};
    swprintf_s(line, L"[%02u:%02u:%02u] %s\r\n",
        st.wHour, st.wMinute, st.wSecond, message);
    char utf8[3600]{};
    const int bytes = WideCharToMultiByte(CP_UTF8, 0, line, -1,
        utf8, static_cast<int>(sizeof(utf8)), nullptr, nullptr);
    if (bytes > 1) {
        DWORD written = 0;
        WriteFile(file, utf8, static_cast<DWORD>(bytes - 1), &written, nullptr);
    }
    CloseHandle(file);
}

constexpr DWORD kSupportedSizeOfImage = 0x03DDF000;
constexpr uintptr_t kMoviePlayerAttachBlockRva = 0x00260244;

static constexpr BYTE kNativeAttachBlock[] = {
    0xE8,0x47,0x89,0x3A,0x01,
    0x4C,0x8B,0x08,
    0x48,0x8D,0x55,0x88,
    0x48,0x8B,0xC8,
    0x41,0xFF,0x51,0x20
};
static constexpr BYTE kSkipAttachBlock[] = { 0xE9,0x0E,0x00,0x00,0x00 };

static std::atomic_bool g_skipLogosTargetValid{false};
static std::atomic_bool g_skipLogosEnabled{true};
static std::atomic_bool g_skipLogosPatched{false};

static bool ReadSkipLogosEnabledFromIni() {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self || !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) return true;
    wchar_t* slash = wcsrchr(modulePath, L'\\');
    if (!slash) return true;
    *(slash + 1) = L'\0';
    wchar_t iniPath[MAX_PATH]{};
    lstrcpyW(iniPath, modulePath);
    lstrcatW(iniPath, L"DarksidersGenesisMod.ini");
    return GetPrivateProfileIntW(L"Features", L"SkipLogos", 1, iniPath) != 0;
}

static BYTE* ResolveValidatedAttachBlock() {
    HMODULE mainModule = GetModuleHandleW(nullptr);
    if (!mainModule) return nullptr;
    BYTE* base = reinterpret_cast<BYTE*>(mainModule);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
        nt->OptionalHeader.SizeOfImage != kSupportedSizeOfImage) return nullptr;
    BYTE* target = base + kMoviePlayerAttachBlockRva;
    const bool native = std::memcmp(target, kNativeAttachBlock, sizeof(kSkipAttachBlock)) == 0;
    const bool patched = std::memcmp(target, kSkipAttachBlock, sizeof(kSkipAttachBlock)) == 0;
    if ((!native && !patched) ||
        std::memcmp(target + sizeof(kSkipAttachBlock),
                    kNativeAttachBlock + sizeof(kSkipAttachBlock),
                    sizeof(kNativeAttachBlock) - sizeof(kSkipAttachBlock)) != 0) return nullptr;
    return target;
}

static bool ApplySkipLogosPatch(bool enabled) {
    BYTE* target = ResolveValidatedAttachBlock();
    if (!target) {
        g_skipLogosTargetValid.store(false, std::memory_order_release);
        g_skipLogosPatched.store(false, std::memory_order_release);
        return false;
    }
    g_skipLogosTargetValid.store(true, std::memory_order_release);
    const BYTE* desired = enabled ? kSkipAttachBlock : kNativeAttachBlock;
    if (std::memcmp(target, desired, sizeof(kSkipAttachBlock)) != 0) {
        DWORD oldProtect = 0;
        if (!VirtualProtect(target, sizeof(kSkipAttachBlock), PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
        std::memcpy(target, desired, sizeof(kSkipAttachBlock));
        FlushInstructionCache(GetCurrentProcess(), target, sizeof(kSkipAttachBlock));
        DWORD ignored = 0;
        VirtualProtect(target, sizeof(kSkipAttachBlock), oldProtect, &ignored);
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
    const DWORD loadError = g_realDxgi ? ERROR_SUCCESS : GetLastError();
    wchar_t detail[512]{};
    swprintf_s(detail, L"Real dxgi.dll: %s result=%p error=%lu",
        path, g_realDxgi, loadError);
    LogLoader(detail);
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
    LogLoader(L"DXGI proxy V0.39: factory export reached; scanning ASIs.");
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self ||
        !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) {
        LogLoader(L"ERROR: proxy module location unavailable.");
        return TRUE;
    }
    {
        wchar_t detail[512]{};
        swprintf_s(detail, L"Proxy DLL loaded: %s", modulePath);
        LogLoader(detail);
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
        wchar_t detail[512]{};
        swprintf_s(detail, L"ERROR: no ASI files found in %s", modulePath);
        LogLoader(detail);
        return TRUE;
    }

    do {
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            continue;
        }

        wchar_t asiPath[MAX_PATH]{};
        lstrcpyW(asiPath, modulePath);
        lstrcatW(asiPath, fd.cFileName);
        SetLastError(ERROR_SUCCESS);
        HMODULE asiModule = LoadLibraryW(asiPath);
        const DWORD loadError = asiModule ? ERROR_SUCCESS : GetLastError();
        wchar_t detail[600]{};
        swprintf_s(detail, L"ASI LoadLibrary: %s result=%p error=%lu",
            asiPath, asiModule, loadError);
        LogLoader(detail);
        if (!asiModule) {
            LogLoader(L"ERROR: ASI did not load. Check installed files, antivirus quarantine and Windows DLL load restrictions.");
        }
    } while (FindNextFileW(find, &fd));

    FindClose(find);
    LogLoader(L"DXGI proxy V0.39: ASI scan complete.");
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

        // V0.19C: let ProjectMayhem build SStartupScreens, then bypass the
        // block that attaches it to the engine MoviePlayer.
        const bool enabled =
            ReadSkipLogosEnabledFromIni();
        ApplySkipLogosPatch(enabled);
    }

    return TRUE;
}
