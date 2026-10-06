#include "EarlySkipLogos.h"

#include <atomic>
#include <cstdint>
#include <cstring>
#include <cwchar>

namespace dg::skip_logos {
namespace {

using CreateFileWFn = HANDLE (WINAPI*)(
    LPCWSTR,
    DWORD,
    DWORD,
    LPSECURITY_ATTRIBUTES,
    DWORD,
    DWORD,
    HANDLE
);

using GetFileAttributesWFn = DWORD (WINAPI*)(LPCWSTR);

using GetFileAttributesExWFn = BOOL (WINAPI*)(
    LPCWSTR,
    GET_FILEEX_INFO_LEVELS,
    LPVOID
);

CreateFileWFn g_originalCreateFileW = nullptr;
GetFileAttributesWFn g_originalGetFileAttributesW = nullptr;
GetFileAttributesExWFn g_originalGetFileAttributesExW = nullptr;

std::atomic_long g_blockCount{0};
wchar_t g_status[192] = L"NOT_INSTALLED";

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

bool IsTargetLogo(const wchar_t* path) {
    const wchar_t* base = BaseName(path);
    if (!base || !*base) {
        return false;
    }

    return
        _wcsicmp(base, L"THQ_LogoBasic.mp4") == 0 ||
        _wcsicmp(base, L"AS_LogoBasic.mp4") == 0;
}

void Blocked(const wchar_t*) {
    g_blockCount.fetch_add(1, std::memory_order_relaxed);
    SetLastError(ERROR_FILE_NOT_FOUND);
}

HANDLE WINAPI HookCreateFileW(
    LPCWSTR fileName,
    DWORD desiredAccess,
    DWORD shareMode,
    LPSECURITY_ATTRIBUTES securityAttributes,
    DWORD creationDisposition,
    DWORD flagsAndAttributes,
    HANDLE templateFile
) {
    if (IsTargetLogo(fileName)) {
        Blocked(fileName);
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
            templateFile)
        : INVALID_HANDLE_VALUE;
}

DWORD WINAPI HookGetFileAttributesW(LPCWSTR fileName) {
    if (IsTargetLogo(fileName)) {
        Blocked(fileName);
        return INVALID_FILE_ATTRIBUTES;
    }

    return g_originalGetFileAttributesW
        ? g_originalGetFileAttributesW(fileName)
        : INVALID_FILE_ATTRIBUTES;
}

BOOL WINAPI HookGetFileAttributesExW(
    LPCWSTR fileName,
    GET_FILEEX_INFO_LEVELS infoLevel,
    LPVOID fileInformation
) {
    if (IsTargetLogo(fileName)) {
        Blocked(fileName);
        return FALSE;
    }

    return g_originalGetFileAttributesExW
        ? g_originalGetFileAttributesExW(
            fileName,
            infoLevel,
            fileInformation)
        : FALSE;
}

bool PatchImportByName(
    HMODULE module,
    const char* functionName,
    void* replacement,
    void** original
) {
    if (!module ||
        !functionName ||
        !replacement ||
        !original) {
        return false;
    }

    auto* base = reinterpret_cast<unsigned char*>(module);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(
        base + dos->e_lfanew
    );
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        return false;
    }

    const auto& directory =
        nt->OptionalHeader.DataDirectory[
            IMAGE_DIRECTORY_ENTRY_IMPORT
        ];

    if (!directory.VirtualAddress ||
        !directory.Size) {
        return false;
    }

    auto* descriptor =
        reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(
            base + directory.VirtualAddress
        );

    for (; descriptor->Name; ++descriptor) {
        if (!descriptor->FirstThunk) {
            continue;
        }

        auto* firstThunk =
            reinterpret_cast<IMAGE_THUNK_DATA64*>(
                base + descriptor->FirstThunk
            );

        IMAGE_THUNK_DATA64* nameThunk = nullptr;
        if (descriptor->OriginalFirstThunk) {
            nameThunk =
                reinterpret_cast<IMAGE_THUNK_DATA64*>(
                    base + descriptor->OriginalFirstThunk
                );
        }

        for (std::size_t index = 0;
             firstThunk[index].u1.Function;
             ++index) {
            bool match = false;

            if (nameThunk &&
                nameThunk[index].u1.AddressOfData &&
                !IMAGE_SNAP_BY_ORDINAL64(
                    nameThunk[index].u1.Ordinal)) {
                auto* byName =
                    reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(
                        base +
                        nameThunk[index].u1.AddressOfData
                    );

                match =
                    _stricmp(
                        reinterpret_cast<const char*>(
                            byName->Name),
                        functionName
                    ) == 0;
            }

            if (!match) {
                continue;
            }

            auto** slot =
                reinterpret_cast<void**>(
                    &firstThunk[index].u1.Function
                );

            DWORD oldProtect = 0;
            if (!VirtualProtect(
                    slot,
                    sizeof(void*),
                    PAGE_READWRITE,
                    &oldProtect)) {
                return false;
            }

            if (!*original) {
                *original = *slot;
            }

            *slot = replacement;

            DWORD ignored = 0;
            VirtualProtect(
                slot,
                sizeof(void*),
                oldProtect,
                &ignored
            );

            FlushInstructionCache(
                GetCurrentProcess(),
                slot,
                sizeof(void*)
            );

            return true;
        }
    }

    return false;
}

} // namespace

bool InstallEarlyIatHooks() {
    HMODULE executable = GetModuleHandleW(nullptr);
    if (!executable) {
        wcscpy_s(g_status, L"NO_MAIN_MODULE");
        return false;
    }

    const bool createPatched =
        PatchImportByName(
            executable,
            "CreateFileW",
            reinterpret_cast<void*>(&HookCreateFileW),
            reinterpret_cast<void**>(
                &g_originalCreateFileW)
        );

    const bool attributesPatched =
        PatchImportByName(
            executable,
            "GetFileAttributesW",
            reinterpret_cast<void*>(
                &HookGetFileAttributesW),
            reinterpret_cast<void**>(
                &g_originalGetFileAttributesW)
        );

    const bool attributesExPatched =
        PatchImportByName(
            executable,
            "GetFileAttributesExW",
            reinterpret_cast<void*>(
                &HookGetFileAttributesExW),
            reinterpret_cast<void**>(
                &g_originalGetFileAttributesExW)
        );

    swprintf_s(
        g_status,
        L"EARLY_IAT CreateFileW=%d GetFileAttributesW=%d GetFileAttributesExW=%d",
        createPatched ? 1 : 0,
        attributesPatched ? 1 : 0,
        attributesExPatched ? 1 : 0
    );

    return
        createPatched ||
        attributesPatched ||
        attributesExPatched;
}

const wchar_t* Status() {
    return g_status;
}

LONG BlockCount() {
    return g_blockCount.load(std::memory_order_relaxed);
}

} // namespace dg::skip_logos
