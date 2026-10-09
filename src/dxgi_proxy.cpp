#include <windows.h>
#include <cwchar>
#include <atomic>
#include <cstring>
#include <cstdio>
#include <cstdint>

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
static std::atomic_bool g_skipLogosEnabled{false};
static std::atomic_bool g_skipLogosPatched{false};
static std::atomic_bool g_startupMovieProbeInstalled{false};
static std::atomic_uint32_t g_startupMovieProbeCalls{0};

static bool ReadSkipLogosEnabledFromIni() {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self || !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) return true;
    wchar_t* slash = wcsrchr(modulePath, L'\\');
    if (!slash) return true;
    *(slash + 1) = L'\0';
    wchar_t iniPath[MAX_PATH]{};
    lstrcpyW(iniPath, modulePath);
    lstrcatW(iniPath, L"DarksidersGenesisMod.ini");
    // DXGI DllMain executes before ASI can migrate earlier settings.
    // Fail closed on pre-V0.43 INIs: never apply legacy attachment bypass.
    const UINT revision = GetPrivateProfileIntW(L"Meta",L"ConfigRevision",0,iniPath);
    if (revision < 2102u) {
        LogLoader(L"V0.43: legacy INI found; StartupScreens bypass BLOCKED before startup");
        return false;
    }
    const bool enabled = GetPrivateProfileIntW(L"Features",L"SkipLogos",0,iniPath) != 0;
    LogLoader(enabled
        ? L"V0.43 WARNING: old SStartupScreens attach bypass explicitly ON (cursor bug suspected)"
        : L"V0.43 SAFE: native MoviePlayer startup attachment preserved");
    return enabled;
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


// V0.44: READ-ONLY snapshot of FLoadingScreenAttributes immediately before
// the ORIGINAL MoviePlayer vtable SetupLoadingScreen call. This is not a
// Skip Logos implementation. It preserves the native Slate/MoviePlayer
// lifecycle and never changes game UI/cursor/movie list state.
//
// Exact PE confirmed from user exe: size=62113280, SHA256=9f4702024df5...,
// image SizeOfImage=0x03DDF000; 19 native bytes at RVA 0x260244.
struct StartupFStringView {
    const wchar_t* text;
    int32_t count;
    int32_t capacity;
};
static_assert(sizeof(StartupFStringView) == 16, "Expected UE4 FString layout");
struct StartupMovieArrayView {
    const StartupFStringView* items;
    int32_t count;
    int32_t capacity;
};
static_assert(sizeof(StartupMovieArrayView) == 16, "Expected UE4 TArray layout");

static bool IsReadableRange(const void* p, size_t bytes) {
    if (!p || bytes == 0) return false;
    MEMORY_BASIC_INFORMATION region{};
    if (!VirtualQuery(p, &region, sizeof(region)) ||
        region.State != MEM_COMMIT ||
        (region.Protect & (PAGE_GUARD | PAGE_NOACCESS))) return false;
    const uintptr_t start = reinterpret_cast<uintptr_t>(p);
    const uintptr_t origin = reinterpret_cast<uintptr_t>(region.BaseAddress);
    if (start < origin) return false;
    const size_t offset = static_cast<size_t>(start - origin);
    return offset < region.RegionSize && bytes <= region.RegionSize - offset;
}

static void __cdecl LogStartupMovieAttributes(const void* attributes) {
    const unsigned call = g_startupMovieProbeCalls.fetch_add(1) + 1;
    if (call > 4) return;
    if (!IsReadableRange(attributes, 0x30)) {
        LogLoader(L"StartupMovieProbe V0.44: WARNING attributes not readable");
        return;
    }
    const auto* bytes = static_cast<const BYTE*>(attributes);
    StartupMovieArrayView movies{};
    std::memcpy(&movies, bytes + 0x10, sizeof(movies));
    float minimumTime = 0.0f;
    std::memcpy(&minimumTime, bytes + 0x20, sizeof(minimumTime));
    wchar_t line[640]{};
    swprintf_s(line,
        L"StartupMovieProbe V0.44: call=%u attributes=%p movieCount=%d movieCapacity=%d minTime=%.3f items=%p [read only]",
        call, attributes, movies.count, movies.capacity, minimumTime, movies.items);
    LogLoader(line);
    if (movies.count < 0 || movies.count > 64 ||
        movies.capacity < movies.count || movies.capacity > 256) {
        LogLoader(L"StartupMovieProbe V0.44: invalid array bounds; no entries read");
        return;
    }
    if (!movies.count) {
        LogLoader(L"StartupMovieProbe V0.44: empty movie playlist (native setup still called)");
        return;
    }
    if (!IsReadableRange(movies.items,
        static_cast<size_t>(movies.count) * sizeof(StartupFStringView))) {
        LogLoader(L"StartupMovieProbe V0.44: movie array not readable");
        return;
    }
    for (int32_t i = 0; i < movies.count; ++i) {
        StartupFStringView entry{};
        std::memcpy(&entry, movies.items + i, sizeof(entry));
        wchar_t item[240]{};
        if (entry.count <= 0 || entry.count > 220 || entry.capacity < entry.count ||
            entry.capacity > 1024 ||
            !IsReadableRange(entry.text,
                static_cast<size_t>(entry.count) * sizeof(wchar_t))) {
            swprintf_s(item, L"StartupMovieProbe V0.44: item[%d] invalid length=%d capacity=%d ptr=%p",
                i, entry.count, entry.capacity, entry.text);
            LogLoader(item);
            continue;
        }
        wchar_t name[224]{};
        const size_t n = static_cast<size_t>(entry.count) - 1;
        std::memcpy(name, entry.text, n * sizeof(wchar_t));
        name[n] = L'\0';
        for (size_t j = 0; j < n; ++j) {
            if (name[j] < 32 || name[j] == 127) name[j] = L'_';
        }
        swprintf_s(item, L"StartupMovieProbe V0.44: item[%d] name='%s' len=%d",
            i, name, entry.count);
        LogLoader(item);
    }
}

// Mid-function trampoline (19-byte exact native sequence; no UObject pointer
// retained). The callback sees [rbp-0x78] in the original StartupModule
// stack frame, then the trampoline executes the ENTIRE native sequence:
// GetMoviePlayer; vtable+0x20(attributes); resume RVA 0x260257.
// x64 stack remains aligned; callback receives 32-byte shadow space.
static bool InstallStartupMovieProbe() {
    HMODULE game = GetModuleHandleW(nullptr);
    if (!game) return false;
    const uintptr_t gameBase = reinterpret_cast<uintptr_t>(game);
    auto* target = reinterpret_cast<BYTE*>(gameBase + kMoviePlayerAttachBlockRva);
    if (!ResolveValidatedAttachBlock() ||
        std::memcmp(target, kNativeAttachBlock, sizeof(kNativeAttachBlock)) != 0) {
        LogLoader(L"StartupMovieProbe V0.44: expected native bytes not found; fail closed");
        return false;
    }

    BYTE* trampoline = static_cast<BYTE*>(
        VirtualAlloc(nullptr, 128, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    if (!trampoline) {
        LogLoader(L"StartupMovieProbe V0.44: trampoline allocation failed");
        return false;
    }
    size_t n = 0;
    auto emit = [&](BYTE value) { trampoline[n++] = value; };
    auto ptr = [&](uintptr_t value) {
        std::memcpy(trampoline + n, &value, sizeof(value)); n += sizeof(value);
    };
    emit(0x48); emit(0x83); emit(0xEC); emit(0x20); // sub rsp, 20h
    emit(0x48); emit(0x8D); emit(0x4D); emit(0x88); // lea rcx,[rbp-78h]
    emit(0x48); emit(0xB8); ptr(reinterpret_cast<uintptr_t>(&LogStartupMovieAttributes));
    emit(0xFF); emit(0xD0);                     // call rax
    emit(0x48); emit(0x83); emit(0xC4); emit(0x20); // add rsp, 20h
    emit(0x48); emit(0xB8); ptr(gameBase + 0x1608B90u);
    emit(0xFF); emit(0xD0);                     // call GetMoviePlayer native
    emit(0x4C); emit(0x8B); emit(0x08);         // mov r9,[rax]
    emit(0x48); emit(0x8D); emit(0x55); emit(0x88); // lea rdx,[rbp-78h]
    emit(0x48); emit(0x8B); emit(0xC8);         // mov rcx,rax
    emit(0x41); emit(0xFF); emit(0x51); emit(0x20); // call [r9+20h]
    emit(0x48); emit(0xB8); ptr(gameBase + 0x260257u);
    emit(0xFF); emit(0xE0);                     // jmp RVA 0x260257
    if (n > 128) {
        VirtualFree(trampoline, 0, MEM_RELEASE);
        return false;
    }
    DWORD oldExec = 0;
    if (!VirtualProtect(trampoline, 128, PAGE_EXECUTE_READ, &oldExec)) {
        VirtualFree(trampoline, 0, MEM_RELEASE);
        return false;
    }
    FlushInstructionCache(GetCurrentProcess(), trampoline, n);

    BYTE branch[sizeof(kNativeAttachBlock)]{};
    std::memset(branch, 0x90, sizeof(branch));
    branch[0] = 0xFF; branch[1] = 0x25; // jmp qword ptr [rip]
    // branch[2..5] zero means the pointer immediately follows.
    const uintptr_t targetPtr = reinterpret_cast<uintptr_t>(trampoline);
    std::memcpy(branch + 6, &targetPtr, sizeof(targetPtr));
    DWORD previous = 0;
    if (!VirtualProtect(target, sizeof(branch), PAGE_EXECUTE_READWRITE, &previous)) {
        VirtualFree(trampoline, 0, MEM_RELEASE);
        return false;
    }
    std::memcpy(target, branch, sizeof(branch));
    FlushInstructionCache(GetCurrentProcess(), target, sizeof(branch));
    DWORD ignored = 0;
    VirtualProtect(target, sizeof(branch), previous, &ignored);
    g_startupMovieProbeInstalled.store(true, std::memory_order_release);
    g_skipLogosTargetValid.store(true, std::memory_order_release);
    g_skipLogosEnabled.store(false, std::memory_order_relaxed);
    g_skipLogosPatched.store(false, std::memory_order_release);
    LogLoader(L"StartupMovieProbe V0.44: native SetupLoadingScreen preserved; read-only trampoline ACTIVE");
    return true;
}

static bool ReadStartupMovieProbeEnabledFromIni() {
    wchar_t path[MAX_PATH]{};
    if (!g_self || !GetModuleFileNameW(g_self, path, MAX_PATH)) return false;
    wchar_t* slash = wcsrchr(path, L'\\');
    if (!slash) return false;
    *(slash + 1) = L'\0';
    wcscat_s(path, L"DarksidersGenesisMod.ini");
    // Probe remains experimental and OFF without an explicit opt-in.
    return GetPrivateProfileIntW(L"Diagnostics", L"StartupMovieProbe", 0, path) != 0;
}

static bool ApplySkipLogosPatch(bool enabled) {
    if (g_startupMovieProbeInstalled.load(std::memory_order_acquire)) {
        if (enabled) {
            LogLoader(L"V0.44: legacy Skip Logos cannot be activated while passive probe is running; restart after disabling probe");
            return false;
        }
        g_skipLogosTargetValid.store(true, std::memory_order_release);
        g_skipLogosEnabled.store(false, std::memory_order_relaxed);
        g_skipLogosPatched.store(false, std::memory_order_release);
        return true;
    }
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

        // V0.43: retain the native startup attachment unless the user
        // explicitly opts into the unsafe V0.19C diagnostic bypass.
        const bool enabled =
            ReadSkipLogosEnabledFromIni();
        ApplySkipLogosPatch(enabled);
        if (!enabled && ReadStartupMovieProbeEnabledFromIni()) {
            InstallStartupMovieProbe();
        }
    }

    return TRUE;
}
