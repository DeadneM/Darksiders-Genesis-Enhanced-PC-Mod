#include <windows.h>

#include "EngineIniFeature.h"

#include <atomic>
#include <cstdio>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

namespace dg::engine_ini {
namespace {

constexpr int kMinGraphicsAdapter = 0;
constexpr int kMaxGraphicsAdapter = 4;

LogFn g_logger = nullptr;
std::atomic_int g_lastApplied{-1};

void FeatureLog(const char* text) {
    if (g_logger && text && *text) {
        g_logger(text);
    }
}

std::string WideToUtf8(const std::wstring& value) {
    if (value.empty()) {
        return {};
    }

    const int bytes = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.c_str(),
        static_cast<int>(value.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    if (bytes <= 0) {
        return {};
    }

    std::string output(
        static_cast<std::size_t>(bytes),
        '\0'
    );

    WideCharToMultiByte(
        CP_UTF8,
        0,
        value.c_str(),
        static_cast<int>(value.size()),
        output.data(),
        bytes,
        nullptr,
        nullptr
    );

    return output;
}

std::wstring ResolveEngineIniPath() {
    const DWORD required =
        GetEnvironmentVariableW(
            L"LOCALAPPDATA",
            nullptr,
            0
        );

    if (required == 0) {
        return {};
    }

    std::vector<wchar_t> buffer(
        static_cast<std::size_t>(required) + 1,
        L'\0'
    );

    const DWORD written =
        GetEnvironmentVariableW(
            L"LOCALAPPDATA",
            buffer.data(),
            static_cast<DWORD>(buffer.size())
        );

    if (written == 0 ||
        written >= buffer.size()) {
        return {};
    }

    std::filesystem::path path(buffer.data());
    path /= L"THQ Nordic";
    path /= L"Darksiders Genesis";
    path /= L"Saved";
    path /= L"Config";
    path /= L"WindowsNoEditor";
    path /= L"Engine.ini";

    return path.wstring();
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;
}

bool ApplyGraphicsAdapter(int adapter) {
    if (adapter < kMinGraphicsAdapter ||
        adapter > kMaxGraphicsAdapter) {
        char line[160]{};
        sprintf_s(
            line,
            sizeof(line),
            "Engine.ini GraphicsAdapter: rejected invalid value %d (allowed 0..4)",
            adapter
        );
        FeatureLog(line);
        return false;
    }

    const std::wstring engineIniPath =
        ResolveEngineIniPath();

    if (engineIniPath.empty()) {
        FeatureLog(
            "Engine.ini GraphicsAdapter: LOCALAPPDATA unavailable"
        );
        return false;
    }

    const std::filesystem::path path(engineIniPath);
    std::error_code ec;
    std::filesystem::create_directories(
        path.parent_path(),
        ec
    );

    if (ec) {
        char line[512]{};
        const std::string utf8Path =
            WideToUtf8(engineIniPath);
        sprintf_s(
            line,
            sizeof(line),
            "Engine.ini GraphicsAdapter: failed to create config directory path=%s error=%d",
            utf8Path.c_str(),
            ec.value()
        );
        FeatureLog(line);
        return false;
    }

    wchar_t value[8]{};
    swprintf_s(value, L"%d", adapter);

    const BOOL writeOk =
        WritePrivateProfileStringW(
            L"SystemSettings",
            L"r.GraphicsAdapter",
            value,
            engineIniPath.c_str()
        );

    if (!writeOk) {
        char line[512]{};
        const std::string utf8Path =
            WideToUtf8(engineIniPath);
        sprintf_s(
            line,
            sizeof(line),
            "Engine.ini GraphicsAdapter: write FAILED value=%d path=%s winerr=%lu",
            adapter,
            utf8Path.c_str(),
            GetLastError()
        );
        FeatureLog(line);
        return false;
    }

    // Flush the Win32 profile cache for this physical INI file.
    WritePrivateProfileStringW(
        nullptr,
        nullptr,
        nullptr,
        engineIniPath.c_str()
    );

    const int verify =
        GetPrivateProfileIntW(
            L"SystemSettings",
            L"r.GraphicsAdapter",
            -1,
            engineIniPath.c_str()
        );

    if (verify != adapter) {
        char line[512]{};
        const std::string utf8Path =
            WideToUtf8(engineIniPath);
        sprintf_s(
            line,
            sizeof(line),
            "Engine.ini GraphicsAdapter: verification FAILED requested=%d readback=%d path=%s",
            adapter,
            verify,
            utf8Path.c_str()
        );
        FeatureLog(line);
        return false;
    }

    g_lastApplied.store(
        adapter,
        std::memory_order_relaxed
    );

    char line[512]{};
    const std::string utf8Path =
        WideToUtf8(engineIniPath);
    sprintf_s(
        line,
        sizeof(line),
        "Engine.ini GraphicsAdapter: applied r.GraphicsAdapter=%d path=%s (game restart required)",
        adapter,
        utf8Path.c_str()
    );
    FeatureLog(line);

    return true;
}

int GetLastAppliedGraphicsAdapter() {
    return g_lastApplied.load(
        std::memory_order_relaxed
    );
}

} // namespace dg::engine_ini
