#include "SkipLogosFeature.h"

#include "RuntimeSettings.h"

#include <windows.h>

#include <string>

namespace dg::skip_logos {
namespace {

constexpr const wchar_t* kBeginMarker =
    L"; BEGIN DarksidersGenesisEnhanced SkipLogos";
constexpr const wchar_t* kEndMarker =
    L"; END DarksidersGenesisEnhanced SkipLogos";

constexpr const wchar_t* kOverrideBlock =
    L"; BEGIN DarksidersGenesisEnhanced SkipLogos\r\n"
    L"[/Script/MoviePlayer.MoviePlayerSettings]\r\n"
    L"-StartupMovies=THQ_LogoBasic\r\n"
    L"-StartupMovies=AS_LogoBasic\r\n"
    L"; END DarksidersGenesisEnhanced SkipLogos\r\n";

LogFn g_logger = nullptr;
Telemetry g_telemetry{};
std::wstring g_gameIniPath;

void LogText(const char* text) {
    if (g_logger && text) {
        g_logger(text);
    }
}

bool BuildGameIniPath(std::wstring& out) {
    wchar_t localAppData[MAX_PATH]{};
    const DWORD len = GetEnvironmentVariableW(
        L"LOCALAPPDATA",
        localAppData,
        MAX_PATH
    );

    if (len == 0 || len >= MAX_PATH) {
        return false;
    }

    out = localAppData;
    out +=
        L"\\THQ Nordic\\Darksiders Genesis"
        L"\\Saved\\Config\\WindowsNoEditor\\Game.ini";
    return true;
}

bool ReadWholeFile(
    const std::wstring& path,
    std::wstring& out
) {
    out.clear();

    HANDLE file = CreateFileW(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            return true;
        }
        return false;
    }

    LARGE_INTEGER size{};
    if (!GetFileSizeEx(file, &size) ||
        size.QuadPart < 0 ||
        size.QuadPart > 4 * 1024 * 1024) {
        CloseHandle(file);
        return false;
    }

    std::string bytes(
        static_cast<std::size_t>(size.QuadPart),
        '\0'
    );

    DWORD read = 0;
    bool ok = true;
    if (!bytes.empty()) {
        ok = ReadFile(
            file,
            bytes.data(),
            static_cast<DWORD>(bytes.size()),
            &read,
            nullptr
        ) != FALSE;
        bytes.resize(read);
    }

    CloseHandle(file);

    if (!ok) {
        return false;
    }

    if (!bytes.empty() &&
        bytes.size() >= 2 &&
        static_cast<unsigned char>(bytes[0]) == 0xFF &&
        static_cast<unsigned char>(bytes[1]) == 0xFE) {
        const wchar_t* wide =
            reinterpret_cast<const wchar_t*>(
                bytes.data() + 2
            );
        const std::size_t count =
            (bytes.size() - 2) / sizeof(wchar_t);
        out.assign(wide, wide + count);
        return true;
    }

    if (bytes.empty()) {
        return true;
    }

    const int needed = MultiByteToWideChar(
        CP_UTF8,
        0,
        bytes.data(),
        static_cast<int>(bytes.size()),
        nullptr,
        0
    );

    if (needed <= 0) {
        return false;
    }

    out.resize(static_cast<std::size_t>(needed));
    return MultiByteToWideChar(
        CP_UTF8,
        0,
        bytes.data(),
        static_cast<int>(bytes.size()),
        out.data(),
        needed
    ) == needed;
}

bool EnsureParentDirectory() {
    wchar_t localAppData[MAX_PATH]{};
    const DWORD len = GetEnvironmentVariableW(
        L"LOCALAPPDATA",
        localAppData,
        MAX_PATH
    );

    if (len == 0 || len >= MAX_PATH) {
        return false;
    }

    std::wstring path = localAppData;
    const wchar_t* parts[] = {
        L"THQ Nordic",
        L"Darksiders Genesis",
        L"Saved",
        L"Config",
        L"WindowsNoEditor"
    };

    for (const wchar_t* part : parts) {
        path += L"\\";
        path += part;
        if (!CreateDirectoryW(path.c_str(), nullptr)) {
            const DWORD error = GetLastError();
            if (error != ERROR_ALREADY_EXISTS) {
                return false;
            }
        }
    }

    return true;
}

bool WriteUtf8File(
    const std::wstring& path,
    const std::wstring& text
) {
    const int needed = WideCharToMultiByte(
        CP_UTF8,
        0,
        text.data(),
        static_cast<int>(text.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    if (needed < 0) {
        return false;
    }

    std::string bytes(
        static_cast<std::size_t>(needed),
        '\0'
    );

    if (needed > 0) {
        if (WideCharToMultiByte(
                CP_UTF8,
                0,
                text.data(),
                static_cast<int>(text.size()),
                bytes.data(),
                needed,
                nullptr,
                nullptr) != needed) {
            return false;
        }
    }

    HANDLE file = CreateFileW(
        path.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written = 0;
    const bool ok =
        bytes.empty() ||
        WriteFile(
            file,
            bytes.data(),
            static_cast<DWORD>(bytes.size()),
            &written,
            nullptr
        ) != FALSE;

    CloseHandle(file);
    return ok &&
        written == static_cast<DWORD>(bytes.size());
}

bool RemoveManagedBlock(std::wstring& text) {
    const std::size_t begin = text.find(kBeginMarker);
    if (begin == std::wstring::npos) {
        return false;
    }

    const std::size_t endMarker =
        text.find(kEndMarker, begin);

    if (endMarker == std::wstring::npos) {
        return false;
    }

    std::size_t end =
        endMarker + wcslen(kEndMarker);

    while (end < text.size() &&
           (text[end] == L'\r' ||
            text[end] == L'\n')) {
        ++end;
    }

    text.erase(begin, end - begin);
    return true;
}

bool HasManagedBlock(const std::wstring& text) {
    return
        text.find(kBeginMarker) != std::wstring::npos &&
        text.find(kEndMarker) != std::wstring::npos;
}

} // namespace

bool Apply(bool enabled) {
    if (g_gameIniPath.empty() &&
        !BuildGameIniPath(g_gameIniPath)) {
        g_telemetry.lastApplyOk = false;
        LogText("Skip Logos UE4: failed to resolve Game.ini path");
        return false;
    }

    std::wstring text;
    if (!ReadWholeFile(g_gameIniPath, text)) {
        g_telemetry.lastApplyOk = false;
        LogText("Skip Logos UE4: failed to read Game.ini");
        return false;
    }

    RemoveManagedBlock(text);

    if (enabled) {
        if (!text.empty() &&
            text.back() != L'\n' &&
            text.back() != L'\r') {
            text += L"\r\n";
        }
        text += kOverrideBlock;
    }

    if (!EnsureParentDirectory() ||
        !WriteUtf8File(g_gameIniPath, text)) {
        g_telemetry.lastApplyOk = false;
        LogText("Skip Logos UE4: failed to write Game.ini");
        return false;
    }

    g_telemetry.initialized = true;
    g_telemetry.overridePresent = enabled;
    g_telemetry.lastApplyOk = true;
    g_telemetry.restartRequired = true;

    LogText(
        enabled
            ? "Skip Logos UE4: StartupMovies override installed; restart required"
            : "Skip Logos UE4: StartupMovies override removed; restart required"
    );

    return true;
}

bool Initialize(LogFn logger) {
    g_logger = logger;

    if (!BuildGameIniPath(g_gameIniPath)) {
        g_telemetry.initialized = false;
        LogText("Skip Logos UE4: Game.ini path unavailable");
        return false;
    }

    std::wstring text;
    if (!ReadWholeFile(g_gameIniPath, text)) {
        g_telemetry.initialized = false;
        LogText("Skip Logos UE4: Game.ini read failed");
        return false;
    }

    g_telemetry.initialized = true;
    g_telemetry.overridePresent =
        HasManagedBlock(text);

    const bool enabled =
        dg::runtime::Get().skipLogosEnabled.load(
            std::memory_order_relaxed
        );

    return Apply(enabled);
}

Telemetry GetTelemetry() {
    return g_telemetry;
}

void Shutdown() {
    // Persistent Game.ini override intentionally survives process exit.
}

} // namespace dg::skip_logos
