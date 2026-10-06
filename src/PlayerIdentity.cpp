#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "PlayerIdentity.h"

#include <atomic>
#include <cmath>
#include <cstdint>

namespace dg::player {
namespace {

constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kJumpZOffset = 0x1A0;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1D4;
constexpr std::size_t kMaxAccelerationOffset = 0x1E8;
constexpr std::size_t kDoubleJumpZOffset = 0x85C;
constexpr std::size_t kGlideDurationOffset = 0x86C;
constexpr std::size_t kIsLocallyControlledVtableOffset = 0x680;

std::atomic_bool g_validated{false};
std::atomic<void*> g_character{nullptr};
std::atomic<void*> g_movement{nullptr};
std::atomic<float> g_walk{0.0f};
std::atomic<float> g_accel{0.0f};
std::atomic<float> g_jump{0.0f};
std::atomic<float> g_doubleJump{0.0f};
std::atomic<float> g_glide{0.0f};

bool Readable(const void* address, std::size_t bytes) {
    if (!address || bytes == 0) {
        return false;
    }

    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(address, &mbi, sizeof(mbi))) {
        return false;
    }

    if (mbi.State != MEM_COMMIT ||
        (mbi.Protect & PAGE_GUARD) ||
        (mbi.Protect & PAGE_NOACCESS)) {
        return false;
    }

    const auto begin = reinterpret_cast<std::uintptr_t>(address);
    const auto regionBegin = reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
    const auto regionEnd = regionBegin + mbi.RegionSize;

    return begin >= regionBegin &&
           begin + bytes >= begin &&
           begin + bytes <= regionEnd;
}

template <typename T>
bool ReadAt(const void* base, std::size_t offset, T& out) {
    if (!base) {
        return false;
    }

    const auto* address =
        reinterpret_cast<const unsigned char*>(base) + offset;
    if (!Readable(address, sizeof(T))) {
        return false;
    }

    out = *reinterpret_cast<const T*>(address);
    return true;
}

bool ReadFiniteFloat(
    const void* base,
    std::size_t offset,
    float& out) {
    if (!ReadAt(base, offset, out)) {
        return false;
    }
    return std::isfinite(out);
}

bool IsLocallyControlledPawn(void* character) {
    if (!character || !Readable(character, sizeof(void*))) {
        return false;
    }

    void** vtable = *reinterpret_cast<void***>(character);
    if (!vtable ||
        !Readable(
            reinterpret_cast<unsigned char*>(vtable) +
                kIsLocallyControlledVtableOffset,
            sizeof(void*))) {
        return false;
    }

    using Fn = bool(*)(void*);
    auto fn = reinterpret_cast<Fn>(
        vtable[kIsLocallyControlledVtableOffset / sizeof(void*)]
    );
    return fn ? fn(character) : false;
}

} // namespace

bool IsValidatedLocalPlayer(
    void* character,
    void* movementComponent) {
    if (!character ||
        !movementComponent ||
        !IsLocallyControlledPawn(character)) {
        return false;
    }

    void* ownerBack = nullptr;
    float walk = 0.0f;
    float accel = 0.0f;
    float jump = 0.0f;
    float doubleJump = 0.0f;
    float glide = 0.0f;

    if (!ReadAt(
            movementComponent,
            kCharacterOwnerOffset,
            ownerBack) ||
        ownerBack != character ||
        !ReadFiniteFloat(
            movementComponent,
            kMaxWalkSpeedOffset,
            walk) ||
        !ReadFiniteFloat(
            movementComponent,
            kMaxAccelerationOffset,
            accel) ||
        !ReadFiniteFloat(
            movementComponent,
            kJumpZOffset,
            jump) ||
        !ReadFiniteFloat(
            movementComponent,
            kDoubleJumpZOffset,
            doubleJump) ||
        !ReadFiniteFloat(
            movementComponent,
            kGlideDurationOffset,
            glide)) {
        return false;
    }

    // Proven Strife/War movement defaults from the supplied reference PAK
    // backups are ~950 MaxWalkSpeed and ~5000 MaxAcceleration. The broad
    // bounds tolerate game upgrades/tuning while excluding the horse
    // reference signature (1300 / 600) and ordinary non-player movement.
    const bool playerShape =
        walk >= 600.0f && walk <= 1250.0f &&
        accel >= 2500.0f && accel <= 8000.0f &&
        jump >= 100.0f && jump <= 10000.0f &&
        doubleJump >= 100.0f && doubleJump <= 10000.0f &&
        glide >= 0.05f && glide <= 60.0f;

    if (!playerShape) {
        return false;
    }

    g_validated.store(true);
    g_character.store(character);
    g_movement.store(movementComponent);
    g_walk.store(walk);
    g_accel.store(accel);
    g_jump.store(jump);
    g_doubleJump.store(doubleJump);
    g_glide.store(glide);
    return true;
}

IdentityTelemetry GetIdentityTelemetry() {
    IdentityTelemetry t{};
    t.validated = g_validated.load();
    t.character = g_character.load();
    t.movement = g_movement.load();
    t.maxWalkSpeed = g_walk.load();
    t.maxAcceleration = g_accel.load();
    t.jumpZ = g_jump.load();
    t.doubleJumpZ = g_doubleJump.load();
    t.glideDuration = g_glide.load();
    return t;
}

void ClearIdentity() {
    g_validated.store(false);
    g_character.store(nullptr);
    g_movement.store(nullptr);
}

} // namespace dg::player
