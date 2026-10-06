#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>

static HMODULE g_self = nullptr;
static HMODULE g_realDxgi = nullptr;
static INIT_ONCE g_dxgiOnce = INIT_ONCE_STATIC_INIT;
static INIT_ONCE g_asiOnce = INIT_ONCE_STATIC_INIT;

static wchar_t g_skipLogosStatus[160] = L"NOT_ATTEMPTED";

struct PeSectionView {
    BYTE* begin = nullptr;
    std::size_t size = 0;
    DWORD characteristics = 0;
};

static bool GetMainModuleSection(
    const char* sectionName,
    PeSectionView& out
) {
    out = {};

    BYTE* base = reinterpret_cast<BYTE*>(GetModuleHandleW(nullptr));
    if (!base) {
        return false;
    }

    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        return false;
    }

    auto* section = IMAGE_FIRST_SECTION(nt);
    for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        char name[9]{};
        memcpy(name, section[i].Name, 8);

        if (strcmp(name, sectionName) != 0) {
            continue;
        }

        out.begin = base + section[i].VirtualAddress;
        out.size = section[i].Misc.VirtualSize;
        out.characteristics = section[i].Characteristics;
        return true;
    }

    return false;
}

static bool AddressInSection(
    const PeSectionView& section,
    const BYTE* address
) {
    return address &&
           address >= section.begin &&
           address < section.begin + section.size;
}

static BYTE* FindBytes(
    const PeSectionView& section,
    const void* needle,
    std::size_t needleBytes
) {
    if (!section.begin || !needle || needleBytes == 0 ||
        needleBytes > section.size) {
        return nullptr;
    }

    const BYTE* bytes = reinterpret_cast<const BYTE*>(needle);
    for (std::size_t i = 0; i + needleBytes <= section.size; ++i) {
        BYTE* p = section.begin + i;
        if (memcmp(p, bytes, needleBytes) == 0) {
            return p;
        }
    }

    return nullptr;
}

static bool RipRelativeLeaTargets(
    const BYTE* instruction,
    const BYTE* target
) {
    if (!instruction || !target) {
        return false;
    }

    // REX.W + LEA reg, [RIP+disp32]
    if ((instruction[0] & 0xF0) != 0x40 ||
        instruction[1] != 0x8D ||
        (instruction[2] & 0xC7) != 0x05) {
        return false;
    }

    const std::int32_t rel =
        *reinterpret_cast<const std::int32_t*>(instruction + 3);
    const BYTE* resolved = instruction + 7 + rel;
    return resolved == target;
}

static bool CallResultTestsAl(
    const PeSectionView& text,
    BYTE* callSite
) {
    if (!callSite || callSite[0] != 0xE8) {
        return false;
    }

    for (std::size_t i = 5; i <= 14; ++i) {
        BYTE* p = callSite + i;
        if (!AddressInSection(text, p) ||
            !AddressInSection(text, p + 1)) {
            break;
        }

        // test al, al
        if (p[0] == 0x84 && p[1] == 0xC0) {
            return true;
        }
    }

    return false;
}

static bool PatchCallToTrue(BYTE* callSite) {
    if (!callSite || callSite[0] != 0xE8) {
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(
            callSite,
            5,
            PAGE_EXECUTE_READWRITE,
            &oldProtect)) {
        return false;
    }

    // mov al,1 ; nop ; nop ; nop
    const BYTE replacement[5] = {
        0xB0, 0x01, 0x90, 0x90, 0x90
    };
    memcpy(callSite, replacement, sizeof(replacement));
    FlushInstructionCache(
        GetCurrentProcess(),
        callSite,
        sizeof(replacement)
    );

    DWORD ignored = 0;
    VirtualProtect(callSite, 5, oldProtect, &ignored);
    return true;
}

static int PatchNativeNoStartupMoviesQuery() {
    PeSectionView text{};
    PeSectionView rdata{};

    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        wcscpy_s(
            g_skipLogosStatus,
            L"NO_PE_SECTIONS"
        );
        return 0;
    }

    BYTE* stringTargets[4]{};
    int stringCount = 0;

    const wchar_t wideLower[] = L"nostartupmovies";
    const wchar_t wideCaps[] = L"NoStartupMovies";
    const char asciiLower[] = "nostartupmovies";
    const char asciiCaps[] = "NoStartupMovies";

    BYTE* candidate = FindBytes(
        rdata,
        wideLower,
        sizeof(wideLower)
    );
    if (candidate) {
        stringTargets[stringCount++] = candidate;
    }

    candidate = FindBytes(
        rdata,
        wideCaps,
        sizeof(wideCaps)
    );
    if (candidate && stringCount < 4) {
        stringTargets[stringCount++] = candidate;
    }

    candidate = FindBytes(
        rdata,
        asciiLower,
        sizeof(asciiLower)
    );
    if (candidate && stringCount < 4) {
        stringTargets[stringCount++] = candidate;
    }

    candidate = FindBytes(
        rdata,
        asciiCaps,
        sizeof(asciiCaps)
    );
    if (candidate && stringCount < 4) {
        stringTargets[stringCount++] = candidate;
    }

    if (stringCount == 0) {
        wcscpy_s(
            g_skipLogosStatus,
            L"STRING_NOT_FOUND"
        );
        return 0;
    }

    BYTE* callSites[8]{};
    int callCount = 0;
    int xrefCount = 0;

    for (std::size_t i = 0; i + 7 < text.size; ++i) {
        BYTE* instruction = text.begin + i;

        bool isXref = false;
        for (int s = 0; s < stringCount; ++s) {
            if (RipRelativeLeaTargets(
                    instruction,
                    stringTargets[s])) {
                isXref = true;
                break;
            }
        }

        if (!isXref) {
            continue;
        }

        ++xrefCount;

        for (std::size_t j = 7; j <= 0x50; ++j) {
            BYTE* p = instruction + j;
            if (!AddressInSection(text, p) ||
                !AddressInSection(text, p + 4)) {
                break;
            }

            if (p[0] != 0xE8 ||
                !CallResultTestsAl(text, p)) {
                continue;
            }

            bool duplicate = false;
            for (int c = 0; c < callCount; ++c) {
                if (callSites[c] == p) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate && callCount < 8) {
                callSites[callCount++] = p;
            }
            break;
        }
    }

    if (callCount == 0) {
        swprintf_s(
            g_skipLogosStatus,
            L"NO_CALL xrefs=%d strings=%d",
            xrefCount,
            stringCount
        );
        return 0;
    }

    int patched = 0;
    for (int i = 0; i < callCount; ++i) {
        if (PatchCallToTrue(callSites[i])) {
            ++patched;
        }
    }

    if (patched > 0) {
        swprintf_s(
            g_skipLogosStatus,
            L"PATCHED calls=%d xrefs=%d strings=%d",
            patched,
            xrefCount,
            stringCount
        );
    } else {
        swprintf_s(
            g_skipLogosStatus,
            L"PATCH_FAILED calls=%d xrefs=%d",
            callCount,
            xrefCount
        );
    }

    return patched;
}

extern "C" __declspec(dllexport)
const wchar_t* WINAPI DGGetSkipLogosStatus() {
    return g_skipLogosStatus;
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

static BOOL CALLBACK LoadAsiPlugins(
    PINIT_ONCE,
    PVOID,
    PVOID*
) {
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
HRESULT WINAPI CreateDXGIFactory(
    REFIID riid,
    void** ppFactory
) {
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
    EnsureAsisLoaded();
    using Fn = HRESULT(WINAPI*)(UINT, REFIID, void**);
    Fn fn = Resolve<Fn>("CreateDXGIFactory2");
    return fn ? fn(flags, riid, ppFactory) : E_NOTIMPL;
}

extern "C" __declspec(dllexport)
HRESULT WINAPI DXGIGetDebugInterface1(
    UINT flags,
    REFIID riid,
    void** ppDebug
) {
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

BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD reason,
    LPVOID
) {
    if (reason == DLL_PROCESS_ATTACH) {
        g_self = module;

        // Runs while the executable image is already mapped but before its
        // entry point. Only the exact UE4 nostartupmovies query call is
        // replaced, and only when the string/xref/call-result chain resolves.
        PatchNativeNoStartupMoviesQuery();

        DisableThreadLibraryCalls(module);
    }
    return TRUE;
}
