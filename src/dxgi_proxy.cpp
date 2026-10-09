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

// V0.51: V0.49 known-working StartupMovies diagnostic restored.
// Keep V0.50's stronger single-log file-open checks and fallback.
// Early RVA 0x25FF31 hook remains TEST-ONLY; only named entries are read
// when count==2, and MoviePlayer setup/intro remains native.
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
    const bool first = !g_loaderLogInitialized.load(std::memory_order_acquire);
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
            if (empty != INVALID_HANDLE_VALUE) {
                CloseHandle(empty);
                g_loaderLogInitialized.store(true, std::memory_order_release);
            }
        }
        return;
    }

    HANDLE file = CreateFileW(path, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
        first ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    if (first) g_loaderLogInitialized.store(true, std::memory_order_release);
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

// V0.52: replace exact two logo names with two dummy paths, without zeroing the list.
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
// These names are intentionally NONEXISTENT movie paths. The UE4 FString
// descriptors remain nonempty and MoviePlayer::SetupLoadingScreen is
// always executed. The game-owned source array is NEVER modified.
// This tests whether the two startup logos can be suppressed while
// retaining the native startup/loading lifecycle and independent intro.
static constexpr wchar_t kSkippedTHQ[] = L"DG_Skipped_THQ_Logo";
static constexpr wchar_t kSkippedAS[]  = L"DG_Skipped_AS_Logo";
static const FStringView kSubstituteMovies[2] = {
    {kSkippedTHQ, static_cast<int32_t>(_countof(kSkippedTHQ)), static_cast<int32_t>(_countof(kSkippedTHQ))},
    {kSkippedAS, static_cast<int32_t>(_countof(kSkippedAS)), static_cast<int32_t>(_countof(kSkippedAS))}
};

// Return an array pointer to use for deep-copying into UE's
// FLoadingScreenAttributes. NULL means leave all original arguments.
// If count==2, replace with two dummy descriptor values instead of
// count-zero (old V0.19B suppressed intro) or bypassing Setup (old
// V0.19C caused cross-shaped cursor). With more than two entries,
// keep the existing validated skip-prefix logic (count decremented
// by two by the assembly island).
static const FStringView* __cdecl FilterStartupMovieNames(const FStringView* movies, int32_t count) {
    const unsigned calls = ++g_startupFilterCalls;
    if (calls > 2 || !g_skipLogosEnabled.load(std::memory_order_relaxed))
        return nullptr;
    wchar_t line[220]{};
    swprintf_s(line, L"Skip Logos V0.52: StartupMovies count=%d validPtr=%d",
        count, IsReadableSpan(movies, sizeof(FStringView)) ? 1 : 0);
    LogLoader(line);
    if (count < 2 || count > 64 ||
        !IsReadableSpan(movies, static_cast<size_t>(count) * sizeof(FStringView))) {
        LogLoader(L"Skip Logos V0.52: FAIL OPEN, movie array/count invalid");
        return nullptr;
    }
    FStringView first{}, second{};
    std::memcpy(&first, movies, sizeof(first));
    std::memcpy(&second, movies+1, sizeof(second));
    LogMovieName(first, 0);
    LogMovieName(second, 1);
    const bool match =
        (IsMovieName(first, L"THQ_LogoBasic") && IsMovieName(second, L"AS_LogoBasic")) ||
        (IsMovieName(first, L"AS_LogoBasic") && IsMovieName(second, L"THQ_LogoBasic"));
    if (!match) {
        LogLoader(L"Skip Logos V0.52: FAIL OPEN, two entries are not exactly THQ and AS logos");
        return nullptr;
    }
    if (count == 2) {
        LogLoader(L"Skip Logos V0.52: TWO LOGOS MATCH; substitute two nonexistent movie names, preserving nonempty playlist and native MoviePlayer");
        return kSubstituteMovies;
    }
    LogLoader(L"Skip Logos V0.52: LOGO PREFIX MATCH; exclude first two, keep remaining movie entries");
    return movies + 2;
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
    absolutePointer(reinterpret_cast<uintptr_t>(&FilterStartupMovieNames));
    bytes({0xFF,0xD0});                       // call helper
    bytes({0x48,0x83,0xC4,0x20});             // shadow space released
    // RAX = NULL (no change), kSubstituteMovies (two entries), or
    // original array + 2 (three or more entries).
    // The 13-byte conditional sequence is position-independent:
    //   test rax,rax    ; jz +13 ; mov rsi,rax ; cmp r14d,2
    //   je +4 ; sub r14d,2.
    // It preserves original MoviePlayer setup even with two logos.
    bytes({0x48,0x85,0xC0,0x74,0x0D});       // NULL -> retain original RSI/R14
    bytes({0x48,0x89,0xC6});                  // RSI = returned descriptor pointer
    bytes({0x41,0x83,0xFE,0x02,0x74,0x04}); // count==2 -> retain count
    bytes({0x41,0x83,0xEE,0x02});             // otherwise remove logo prefix
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
    LogLoader(L"Skip Logos V0.52: targeted 7-byte playlist hook installed; MoviePlayer attachment unchanged");
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
    LogLoader(L"V0.52: DXGI proxy and unified Mod.log active");
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

        // V0.51 restores the exact in-game-validated V0.49 diagnostic:
        // installed before the game's StartupScreens module runs.
        // If playlist count=2, report two movie names WITHOUT removing them.
        // Unlike old V0.19C, native MoviePlayer attachment is untouched.
        const bool enabled = ReadSkipLogosEnabledFromIni();
        g_skipLogosEnabled.store(enabled,std::memory_order_relaxed);
        const bool valid = HasSupportedNativeCode();
        g_skipLogosTargetValid.store(valid,std::memory_order_release);
        if (enabled && valid && !InstallSelectiveLogoHook())
            LogLoader(L"Skip Logos V0.51: diagnostic hook install failed; native startup unchanged");
        if (!enabled)
            LogLoader(L"Skip Logos V0.51: OFF; original startup preserved");
    }

    return TRUE;
}
