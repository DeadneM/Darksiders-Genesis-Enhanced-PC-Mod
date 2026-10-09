#include <windows.h>
#include <cwchar>
#include <atomic>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <limits>
#include <climits>
#include <initializer_list>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;
static std::atomic_bool g_loaderLogInitialized{false};

// V0.47: single non-cumulative DarksidersGenesisMod.log shared by DXGI and ASI.
// Keep early loader diagnostics even if the ASI cannot start.
static void LogLoader(const wchar_t* message) {
    if (!g_self || !message) return;
    wchar_t path[MAX_PATH]{};
    if (!GetModuleFileNameW(g_self, path, MAX_PATH)) return;
    wchar_t* slash = wcsrchr(path, L'\\');
    if (!slash) return;
    *(slash + 1) = L'\0';
    if (wcslen(path) + 28 >= MAX_PATH) return;
    // V0.48: preserve any old Loader.log file as-is. Never create it,
    // open it, truncate it, or delete it. Only the unified Mod.log is used.
    const bool first = !g_loaderLogInitialized.exchange(true);
    wcscat_s(path, L"DarksidersGenesisMod.log");

    // Suppress routine loader internals; retain errors and Skip Logos state.
    if (wcsstr(message, L"factory export reached") ||
        wcsstr(message, L"ASI scan complete") ||
        wcsstr(message, L"Proxy DLL loaded:") ||
        wcsstr(message, L"Real dxgi.dll:")) {
        // Still CREATE_ALWAYS the log at the beginning of the session,
        // so old runs never linger if the ASI fails before initialization.
        if (first) {
            HANDLE empty = CreateFileW(path, GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (empty != INVALID_HANDLE_VALUE) CloseHandle(empty);
        }
        return;
    }

    HANDLE file = CreateFileW(path, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
        first ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    SetFilePointer(file, 0, nullptr, FILE_END);

    SYSTEMTIME st{};
    GetLocalTime(&st);
    wchar_t line[1200]{};
    swprintf_s(line, L"[%02u:%02u:%02u] [Loader] %s\r\n",
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

// V0.49: inspect 2-entry playlist safely; only filter if a third movie survives.
// Never bypass SStartupScreens / MoviePlayer / Slate initialization.
constexpr DWORD kSupportedSizeOfImage = 0x03DDF000;
constexpr uintptr_t kMoviePlayerAttachBlockRva = 0x00260244;
constexpr uintptr_t kStartupCopyLoadRva = 0x0025FF31;
static constexpr BYTE kOriginalCopyLoad[7] = {
    0x44,0x8B,0x76,0x08,  // mov r14d,[rsi+8] (TArray count)
    0x48,0x8B,0x36       // mov rsi,[rsi]    (FString data)
};
static constexpr BYTE kOriginalAttach[19] = {
    0xE8,0x47,0x89,0x3A,0x01,0x4C,0x8B,0x08,
    0x48,0x8D,0x55,0x88,0x48,0x8B,0xC8,0x41,0xFF,0x51,0x20
};

static std::atomic_bool g_skipLogosTargetValid{false};
static std::atomic_bool g_skipLogosEnabled{false};
static std::atomic_bool g_skipLogosPatched{false};
static std::atomic_bool g_skipLogosHookReady{false};
static std::atomic_uint32_t g_startupFilterCalls{0};

struct FStringView {
    const wchar_t* data;
    int32_t size;
    int32_t capacity;
};
static_assert(sizeof(FStringView) == 16, "UE4 FString must be 16 bytes");

static bool IsReadableSpan(const void* p, size_t size) {
    if (!p || !size) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(p, &mbi, sizeof(mbi)) ||
        mbi.State != MEM_COMMIT ||
        (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))) return false;
    const uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    const uintptr_t base = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
    if (addr < base) return false;
    const size_t start = static_cast<size_t>(addr - base);
    return start < mbi.RegionSize && size <= mbi.RegionSize - start;
}

// Never make assumptions about playlist length/order without checking names.
// count==2 is allowed for DIAGNOSTIC NAME READS only, not for removal.
// Old V0.19B proved forcing a zero-length movie playlist also skipped intro.
static bool IsMovieName(const FStringView& entry, const wchar_t* expected) {
    if (!entry.data || entry.size < 2 || entry.size > 180 ||
        entry.capacity < entry.size || entry.capacity > 512 ||
        !IsReadableSpan(entry.data, static_cast<size_t>(entry.size) * sizeof(wchar_t)))
        return false;
    const int length = entry.data[entry.size - 1] == L'\0'
        ? entry.size - 1 : entry.size;
    const int expectedLength = static_cast<int>(wcslen(expected));
    if (length == expectedLength &&
        CompareStringOrdinal(entry.data, length, expected, expectedLength, TRUE) == CSTR_EQUAL)
        return true;
    if (length == expectedLength + 4 &&
        CompareStringOrdinal(entry.data, expectedLength, expected, expectedLength, TRUE) == CSTR_EQUAL &&
        CompareStringOrdinal(entry.data + expectedLength, 4, L".mp4", 4, TRUE) == CSTR_EQUAL)
        return true;
    return false;
}

static void LogMovieName(const FStringView& e, unsigned index) {
    wchar_t line[300]{};
    if (!e.data || e.size < 1 || e.size > 180 ||
        e.capacity < e.size || e.capacity > 512 ||
        !IsReadableSpan(e.data, static_cast<size_t>(e.size) * sizeof(wchar_t))) {
        swprintf_s(line, L"Skip Logos V0.49: entry[%u] invalid size=%d cap=%d",
            index, e.size, e.capacity);
        LogLoader(line);
        return;
    }
    wchar_t name[190]{};
    int n = e.size;
    if (n && e.data[n-1] == L'\0') --n;
    std::memcpy(name, e.data, static_cast<size_t>(n) * sizeof(wchar_t));
    name[n] = 0;
    for (int i=0; i<n; ++i) if (name[i] < 32) name[i] = L'_';
    swprintf_s(line, L"Skip Logos V0.49: entry[%u] '%s'",index,name);
    LogLoader(line);
}

// Called on the original game's startup thread AFTER settings are constructed,
// not from DllMain. Result only controls which FString descriptors are copied;
// the original loading-screen setup and all cleanup still execute normally.
static bool __cdecl ShouldFilterLogoPrefix(const FStringView* movies, int32_t count) {
    const unsigned calls = ++g_startupFilterCalls;
    if (calls > 2 || !g_skipLogosEnabled.load(std::memory_order_relaxed))
        return false;
    wchar_t line[220]{};
    swprintf_s(line, L"Skip Logos V0.49: native StartupMovies count=%d validPtr=%d",
        count, IsReadableSpan(movies, sizeof(FStringView)) ? 1 : 0);
    LogLoader(line);
    if (count < 2 || count > 64 ||
        !IsReadableSpan(movies, static_cast<size_t>(count) * sizeof(FStringView))) {
        LogLoader(L"Skip Logos V0.49: FAIL OPEN, playlist unreadable or count outside [2,64]");
        return false;
    }
    FStringView first{}, second{};
    std::memcpy(&first, movies, sizeof(first));
    std::memcpy(&second, movies + 1, sizeof(second));
    LogMovieName(first, 0);
    LogMovieName(second, 1);
    if (count > 2) {
        FStringView third{};
        std::memcpy(&third, movies + 2, sizeof(third));
        LogMovieName(third, 2);
    }
    const bool thqFirst = IsMovieName(first, L"THQ_LogoBasic");
    const bool thqSecond = IsMovieName(second, L"THQ_LogoBasic");
    const bool asFirst = IsMovieName(first, L"AS_LogoBasic");
    const bool asSecond = IsMovieName(second, L"AS_LogoBasic");
    if (!((thqFirst && asSecond) || (asFirst && thqSecond))) {
        LogLoader(L"Skip Logos V0.49: FAIL OPEN, prefix is not the exact THQ/AS pair");
        return false;
    }
    // CRITICAL: in the user's retail game playlist is exactly TWO entries.
    // Earlier count-zero tests disabled the intro too. Do NOT repeat
    // this known regression, even when the prefix is the exact pair.
    // Still record the exact names for a later lifecycle-safe approach.
    if (count == 2) {
        LogLoader(L"Skip Logos V0.49: TWO_LOGOS_CONFIRMED; no removal: a zero-movie list previously removed intro. Native MoviePlayer remains unchanged.");
        return false;
    }
    LogLoader(L"Skip Logos V0.49: MATCH, omitting only first 2 logo names; remaining movies preserved");
    return true;
}

static bool HasSupportedNativeCode() {
    HMODULE game = GetModuleHandleW(nullptr);
    if (!game) return false;
    const BYTE* base = reinterpret_cast<const BYTE*>(game);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0x40 ||
        dos->e_lfanew > 0x2000) return false;
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
        nt->OptionalHeader.SizeOfImage != kSupportedSizeOfImage) return false;
    return std::memcmp(base + kMoviePlayerAttachBlockRva,
                       kOriginalAttach, sizeof(kOriginalAttach)) == 0 &&
           std::memcmp(base + kStartupCopyLoadRva,
                       kOriginalCopyLoad, sizeof(kOriginalCopyLoad)) == 0;
}

static bool ReadSkipLogosEnabledFromIni() {
    wchar_t modulePath[MAX_PATH]{};
    if (!g_self || !GetModuleFileNameW(g_self, modulePath, MAX_PATH)) return false;
    wchar_t* slash = wcsrchr(modulePath, L'\\');
    if (!slash) return false;
    *(slash + 1) = L'\0';
    wchar_t iniPath[MAX_PATH]{};
    lstrcpyW(iniPath, modulePath);
    lstrcatW(iniPath, L"DarksidersGenesisMod.ini");
    // ASI loads later, so retain V0.43 protection from old un-migrated INIs.
    const UINT rev = GetPrivateProfileIntW(L"Meta",L"ConfigRevision",0,iniPath);
    if (rev < 2102u) {
        LogLoader(L"Skip Logos V0.46: old INI revision; filtering OFF for safety");
        return false;
    }
    const bool enable = GetPrivateProfileIntW(L"Features",L"SkipLogos",0,iniPath) != 0;
    LogLoader(enable ? L"Skip Logos V0.46: exact-name prefix filter requested" :
        L"Skip Logos V0.46: OFF, original loading screens preserved");
    return enable;
}

// Allocate a small executable island within rel32 reach of RVA 0x25FF31.
// The island contains no exception-raising native calls of its own.
// It replays the exact 7 original bytes; saves flags and volatile regs;
// invokes the C++ validator with correct Windows x64 shadow-space alignment;
// adjusts only rsi/r14 count if matched; restores flags/regs; absolute-jumps
// to RVA 0x25FF38 *without clobbering RAX*. MoviePlayer remains untouched.
static BYTE* AllocateNearby(BYTE* target) {
    SYSTEM_INFO info{};
    GetSystemInfo(&info);
    const uintptr_t step = info.dwAllocationGranularity;
    if (step == 0) return nullptr;
    const uintptr_t pivot = reinterpret_cast<uintptr_t>(target);
    const uintptr_t gameBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const uintptr_t above = (gameBase + kSupportedSizeOfImage + step-1) & ~(step-1);
    for (size_t i=0; i<8192; ++i) {
        uintptr_t candidate = 0;
        if ((i & 1) == 0) {
            candidate = above + (i/2) * step;
        } else {
            const uintptr_t sub = (i/2 + 1) * step;
            candidate = gameBase > sub ? (gameBase - sub) : 0;
        }
        if (!candidate) continue;
        const int64_t delta = static_cast<int64_t>(candidate) -
            static_cast<int64_t>(pivot + 5);
        if (delta < INT32_MIN || delta > INT32_MAX) continue;
        BYTE* allocated = static_cast<BYTE*>(VirtualAlloc(
            reinterpret_cast<void*>(candidate), 4096,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        if (allocated) return allocated;
    }
    return nullptr;
}

static bool InstallSelectiveLogoHook() {
    if (!HasSupportedNativeCode()) {
        LogLoader(L"Skip Logos V0.46: FAIL CLOSED, unexpected native EXE bytes");
        return false;
    }
    BYTE* target = reinterpret_cast<BYTE*>(
        GetModuleHandleW(nullptr)) + kStartupCopyLoadRva;
    BYTE* island = AllocateNearby(target);
    if (!island) {
        LogLoader(L"Skip Logos V0.46: FAIL CLOSED, no nearby executable island");
        return false;
    }
    BYTE* p = island;
    auto bytes = [&](std::initializer_list<BYTE> v) {
        for (BYTE b : v) *p++=b;
    };
    auto absolutePointer = [&](uintptr_t address) {
        std::memcpy(p, &address, sizeof(address));
        p += sizeof(address);
    };

    bytes({0x44,0x8B,0x76,0x08,0x48,0x8B,0x36}); // replay 7 original bytes
    bytes({0x9C,0x50,0x51,0x52,0x41,0x50,0x41,0x51,0x41,0x52,0x41,0x53});
    bytes({0x48,0x83,0xEC,0x20});             // Win64 shadow space; RSP 16-aligned
    bytes({0x48,0x89,0xF1});                  // rcx = rsi (movie descriptors)
    bytes({0x44,0x89,0xF2});                  // edx = r14d (count)
    bytes({0x48,0xB8});
    absolutePointer(reinterpret_cast<uintptr_t>(&ShouldFilterLogoPrefix));
    bytes({0xFF,0xD0});                       // call helper
    bytes({0x48,0x83,0xC4,0x20});             // shadow space released
    bytes({0x84,0xC0,0x74,0x08});             // if (!match) do not shift
    bytes({0x48,0x83,0xC6,0x20});             // rsi += 2 FString descriptors
    bytes({0x41,0x83,0xEE,0x02});             // r14d -= 2
    bytes({0x41,0x5B,0x41,0x5A,0x41,0x59,0x41,0x58,
           0x5A,0x59,0x58,0x9D});             // restore non-volatile context
    bytes({0xFF,0x25,0x00,0x00,0x00,0x00});  // jmp qword ptr [rip]
    absolutePointer(reinterpret_cast<uintptr_t>(target + sizeof(kOriginalCopyLoad)));

    const size_t n = static_cast<size_t>(p - island);
    if (n > 256) {
        VirtualFree(island, 0, MEM_RELEASE);
        return false;
    }
    DWORD oldIsland = 0;
    if (!VirtualProtect(island, 4096, PAGE_EXECUTE_READ, &oldIsland)) {
        VirtualFree(island, 0, MEM_RELEASE);
        return false;
    }
    FlushInstructionCache(GetCurrentProcess(), island, n);
    const int64_t rel = static_cast<int64_t>(
        reinterpret_cast<uintptr_t>(island)) -
        static_cast<int64_t>(reinterpret_cast<uintptr_t>(target) + 5);
    if (rel < INT32_MIN || rel > INT32_MAX) {
        VirtualFree(island, 0, MEM_RELEASE);
        return false;
    }
    BYTE branch[sizeof(kOriginalCopyLoad)] = {0xE9,0,0,0,0,0x90,0x90};
    const int32_t displacement = static_cast<int32_t>(rel);
    std::memcpy(branch+1, &displacement, sizeof(displacement));
    DWORD oldProtect = 0;
    if (!VirtualProtect(target, sizeof(branch), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        VirtualFree(island,0,MEM_RELEASE);
        return false;
    }
    std::memcpy(target,branch,sizeof(branch));
    FlushInstructionCache(GetCurrentProcess(),target,sizeof(branch));
    DWORD ignored = 0;
    VirtualProtect(target,sizeof(branch),oldProtect,&ignored);
    g_skipLogosHookReady.store(true,std::memory_order_release);
    g_skipLogosPatched.store(true,std::memory_order_release);
    LogLoader(L"Skip Logos V0.46: exact 7-byte startup copy hook installed; native MoviePlayer untouched");
    return true;
}

static bool ApplySkipLogosPatch(bool enabled) {
    // Do not apply or unapply any code patch after the game's entry point.
    // Runtime checkbox only changes the requested setting for next boot.
    g_skipLogosEnabled.store(enabled,std::memory_order_relaxed);
    g_skipLogosTargetValid.store(HasSupportedNativeCode() ||
        g_skipLogosHookReady.load(std::memory_order_acquire),
        std::memory_order_release);
    g_skipLogosPatched.store(g_skipLogosHookReady.load(std::memory_order_acquire),
                            std::memory_order_release);
    return g_skipLogosTargetValid.load(std::memory_order_acquire);
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

// ASI uses this to preserve the already-created per-session loader lines.
extern "C" __declspec(dllexport)
BOOL WINAPI DGUnifiedLogActive() {
    return g_loaderLogInitialized.load(std::memory_order_acquire) ? TRUE : FALSE;
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

        // V0.46: a strictly verified, prefix-only filter of StartupMovies.
        // The old V0.19C hook at MoviePlayer attachment is NEVER applied.
        const bool enabled = ReadSkipLogosEnabledFromIni();
        g_skipLogosEnabled.store(enabled,std::memory_order_relaxed);
        const bool native = HasSupportedNativeCode();
        g_skipLogosTargetValid.store(native,std::memory_order_release);
        if (enabled && native && !InstallSelectiveLogoHook())
            LogLoader(L"Skip Logos V0.46: install failed; native startup untouched");
        if (!enabled)
            LogLoader(L"Skip Logos V0.46: no startup code modified");
    }

    return TRUE;
}
