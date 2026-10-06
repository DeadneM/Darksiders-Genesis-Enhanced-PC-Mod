#include <windows.h>
#include <bcrypt.h>

#include "TargetValidator.h"

#include <array>
#include <cstdio>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

namespace dg::target {
namespace {

constexpr std::uint64_t kExpectedSize = 62113280ull;
constexpr const char* kExpectedSha256 =
    "9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54";

std::string HexLower(const unsigned char* bytes, std::size_t count) {
    static constexpr char kHex[] = "0123456789abcdef";
    std::string out;
    out.resize(count * 2);

    for (std::size_t i = 0; i < count; ++i) {
        out[i * 2] = kHex[(bytes[i] >> 4) & 0x0F];
        out[i * 2 + 1] = kHex[bytes[i] & 0x0F];
    }

    return out;
}

bool HashFileSha256(const wchar_t* path, std::string& outHash) {
    HANDLE file = CreateFileW(
        path,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
        nullptr
    );
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }

    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    std::vector<unsigned char> hashObject;
    std::array<unsigned char, 32> digest{};

    bool ok = false;

    do {
        if (BCryptOpenAlgorithmProvider(
                &algorithm,
                BCRYPT_SHA256_ALGORITHM,
                nullptr,
                0) < 0) {
            break;
        }

        DWORD objectSize = 0;
        DWORD resultSize = 0;
        if (BCryptGetProperty(
                algorithm,
                BCRYPT_OBJECT_LENGTH,
                reinterpret_cast<PUCHAR>(&objectSize),
                sizeof(objectSize),
                &resultSize,
                0) < 0) {
            break;
        }

        hashObject.resize(objectSize);

        if (BCryptCreateHash(
                algorithm,
                &hash,
                hashObject.data(),
                static_cast<ULONG>(hashObject.size()),
                nullptr,
                0,
                0) < 0) {
            break;
        }

        std::vector<unsigned char> buffer(1024 * 1024);
        for (;;) {
            DWORD read = 0;
            if (!ReadFile(
                    file,
                    buffer.data(),
                    static_cast<DWORD>(buffer.size()),
                    &read,
                    nullptr)) {
                break;
            }

            if (read == 0) {
                if (BCryptFinishHash(
                        hash,
                        digest.data(),
                        static_cast<ULONG>(digest.size()),
                        0) >= 0) {
                    outHash = HexLower(digest.data(), digest.size());
                    ok = true;
                }
                break;
            }

            if (BCryptHashData(hash, buffer.data(), read, 0) < 0) {
                break;
            }
        }
    } while (false);

    if (hash) {
        BCryptDestroyHash(hash);
    }
    if (algorithm) {
        BCryptCloseAlgorithmProvider(algorithm, 0);
    }

    CloseHandle(file);
    return ok;
}

} // namespace

ValidationResult ValidateCurrentExecutable() {
    ValidationResult result{};

    wchar_t path[MAX_PATH]{};
    if (!GetModuleFileNameW(nullptr, path, MAX_PATH)) {
        result.reason = "GetModuleFileNameW failed";
        return result;
    }

    WIN32_FILE_ATTRIBUTE_DATA data{};
    if (!GetFileAttributesExW(path, GetFileExInfoStandard, &data)) {
        result.reason = "GetFileAttributesExW failed";
        return result;
    }

    result.fileSize =
        (static_cast<std::uint64_t>(data.nFileSizeHigh) << 32) |
        static_cast<std::uint64_t>(data.nFileSizeLow);

    if (result.fileSize != kExpectedSize) {
        char text[160]{};
        sprintf_s(
            text,
            "size mismatch: got %llu expected %llu",
            static_cast<unsigned long long>(result.fileSize),
            static_cast<unsigned long long>(kExpectedSize)
        );
        result.reason = text;
        return result;
    }

    if (!HashFileSha256(path, result.sha256)) {
        result.reason = "SHA-256 calculation failed";
        return result;
    }

    if (result.sha256 != kExpectedSha256) {
        result.reason = "SHA-256 mismatch";
        return result;
    }

    result.exact = true;
    result.reason = "exact supported executable";
    return result;
}

} // namespace dg::target
