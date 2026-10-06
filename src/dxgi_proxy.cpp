#include <windows.h>
#include <cwchar>
#include <atomic>
#include <cstring>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;

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

        // V0.19C: let ProjectMayhem build SStartupScreens, then bypass the
        // block that attaches it to the engine MoviePlayer.
        const bool enabled =
            ReadSkipLogosEnabledFromIni();
        ApplySkipLogosPatch(enabled);
    }

    return TRUE;
}
