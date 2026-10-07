#include <windows.h>

#include "HorseFeature.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdarg>
#include <cstdio>

namespace dg::horse {
namespace {

constexpr std::size_t kPlayerHorseMountOffset = 0xE70;
constexpr std::size_t kHorseMovementOffset = 0x390;
constexpr std::size_t kHorseIsSprintingOffset = 0x8D4;
constexpr std::size_t kHorseSprintingMaxSpeedOffset = 0x760;

constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1E0;
constexpr std::size_t kMaxAccelerationOffset = 0x1F4;
constexpr std::size_t kBrakingFrictionFactorOffset = 0x1FC;

constexpr std::size_t kStaminaRecoveryOffset = 0x910;
constexpr std::size_t kStaminaTotalRecoveryOffset = 0x914;
constexpr std::size_t kStaminaSprintDrainOffset = 0x920;

constexpr ULONGLONG kHorseLostTimeoutMs = 1500;

struct HorseState {
    void* player = nullptr;
    void* horse = nullptr;
    void* movement = nullptr;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float brakingFrictionFactor = 0.0f;
    float sprintingMaxSpeed = 0.0f;
    float sprintDrain = 0.0f;
    bool captured = false;
};

SRWLOCK g_lock = SRWLOCK_INIT;
HorseState g_horse{};

std::atomic_bool g_speedEnabled{true};
std::atomic<float> g_speedMultiplier{1.25f};
std::atomic_bool g_sprintSpeedEnabled{true};
std::atomic<float> g_sprintSpeedMultiplier{1.25f};
std::atomic_bool g_sprintDurationEnabled{true};
std::atomic<float> g_sprintDurationMultiplier{2.0f};

std::atomic_bool g_validated{false};
std::atomic_bool g_staminaReady{false};
std::atomic_bool g_sprinting{false};
std::atomic<void*> g_player{nullptr};
std::atomic<void*> g_owner{nullptr};
std::atomic<void*> g_movement{nullptr};

std::atomic<float> g_nativeGetMaxSpeed{0.0f};
std::atomic<float> g_appliedGetMaxSpeed{0.0f};
std::atomic<float> g_nativeMaxWalkSpeed{0.0f};
std::atomic<float> g_appliedMaxWalkSpeed{0.0f};
std::atomic<float> g_nativeMaxAcceleration{0.0f};
std::atomic<float> g_appliedMaxAcceleration{0.0f};
std::atomic<float> g_nativeSprintingMaxSpeed{0.0f};
std::atomic<float> g_appliedSprintingMaxSpeed{0.0f};
std::atomic<float> g_nativeSprintDrain{0.0f};
std::atomic<float> g_appliedSprintDrain{0.0f};

std::atomic_uint32_t g_directChainsLogged{0};
std::atomic_uint32_t g_directMatches{0};
std::atomic_ullong g_lastSeenTick{0};

LogFn g_logger = nullptr;

void FeatureLog(const char* fmt, ...) {
    if (!g_logger) return;
    char buffer[768]{};
    va_list args;
    va_start(args, fmt);
    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    va_end(args);
    g_logger(buffer);
}

bool Readable(const void* address, std::size_t bytes) {
    if (!address || bytes == 0) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(address, &mbi, sizeof(mbi))) return false;
    if (mbi.State != MEM_COMMIT ||
        (mbi.Protect & PAGE_GUARD) ||
        (mbi.Protect & PAGE_NOACCESS)) return false;

    const auto begin = reinterpret_cast<std::uintptr_t>(address);
    const auto regionBegin = reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
    const auto regionEnd = regionBegin + mbi.RegionSize;
    return begin >= regionBegin &&
           begin + bytes >= begin &&
           begin + bytes <= regionEnd;
}

bool Writable(void* address, std::size_t bytes) {
    if (!Readable(address, bytes)) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(address, &mbi, sizeof(mbi))) return false;
    const DWORD protect = mbi.Protect & 0xFF;
    return protect == PAGE_READWRITE ||
           protect == PAGE_WRITECOPY ||
           protect == PAGE_EXECUTE_READWRITE ||
           protect == PAGE_EXECUTE_WRITECOPY;
}

template <typename T>
bool ReadAt(const void* base, std::size_t offset, T& out) {
    if (!base) return false;
    const auto* p = reinterpret_cast<const unsigned char*>(base) + offset;
    if (!Readable(p, sizeof(T))) return false;
    out = *reinterpret_cast<const T*>(p);
    return true;
}

bool ReadFloat(const void* base, std::size_t offset, float& out) {
    return ReadAt(base, offset, out) && std::isfinite(out);
}

bool WriteFloat(void* base, std::size_t offset, float value) {
    if (!base || !std::isfinite(value)) return false;
    auto* p = reinterpret_cast<unsigned char*>(base) + offset;
    if (!Writable(p, sizeof(float))) return false;
    *reinterpret_cast<float*>(p) = value;
    return true;
}

float Clamp(float value, float low, float high) {
    return std::clamp(value, low, high);
}

void PublishCleared() {
    g_validated.store(false);
    g_staminaReady.store(false);
    g_sprinting.store(false);
    g_player.store(nullptr);
    g_owner.store(nullptr);
    g_movement.store(nullptr);
    g_nativeGetMaxSpeed.store(0.0f);
    g_appliedGetMaxSpeed.store(0.0f);
    g_nativeMaxWalkSpeed.store(0.0f);
    g_appliedMaxWalkSpeed.store(0.0f);
    g_nativeMaxAcceleration.store(0.0f);
    g_appliedMaxAcceleration.store(0.0f);
    g_nativeSprintingMaxSpeed.store(0.0f);
    g_appliedSprintingMaxSpeed.store(0.0f);
    g_nativeSprintDrain.store(0.0f);
    g_appliedSprintDrain.store(0.0f);
    g_lastSeenTick.store(0);
}

void RestoreLocked() {
    if (!g_horse.captured) return;

    void* liveMovement = nullptr;
    if (!ReadAt(g_horse.horse, kHorseMovementOffset, liveMovement) ||
        liveMovement != g_horse.movement) {
        return;
    }

    WriteFloat(g_horse.movement, kMaxWalkSpeedOffset, g_horse.maxWalkSpeed);
    WriteFloat(g_horse.movement, kMaxAccelerationOffset, g_horse.maxAcceleration);
    WriteFloat(g_horse.movement, kBrakingFrictionFactorOffset, g_horse.brakingFrictionFactor);
    WriteFloat(g_horse.horse, kHorseSprintingMaxSpeedOffset, g_horse.sprintingMaxSpeed);
    WriteFloat(g_horse.horse, kStaminaSprintDrainOffset, g_horse.sprintDrain);
}

void ClearHorse(const char* reason) {
    AcquireSRWLockExclusive(&g_lock);
    if (g_horse.captured) {
        RestoreLocked();
        if (reason) {
            FeatureLog("HorseFeature V0.23B: released direct horse (%s)", reason);
        }
    }
    g_horse = {};
    PublishCleared();
    ReleaseSRWLockExclusive(&g_lock);
}

void ApplyFieldsLocked() {
    if (!g_horse.captured) return;

    const float speedMultiplier =
        Clamp(g_speedMultiplier.load(), 0.0f, 3.0f);
    const float sprintMultiplier =
        Clamp(g_sprintSpeedMultiplier.load(), 0.0f, 3.0f);
    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 10.0f);

    const float targetWalk =
        g_speedEnabled.load()
            ? 1500.0f
            : g_horse.maxWalkSpeed;
    const float targetAcceleration =
        g_speedEnabled.load()
            ? 700.0f
            : g_horse.maxAcceleration;
    const float targetBraking =
        g_speedEnabled.load()
            ? 2.0f
            : g_horse.brakingFrictionFactor;
    const float targetSprintMax =
        g_sprintSpeedEnabled.load()
            ? g_horse.sprintingMaxSpeed * sprintMultiplier
            : g_horse.sprintingMaxSpeed;

    float targetDrain = g_horse.sprintDrain;
    if (g_sprintDurationEnabled.load()) {
        targetDrain =
            durationMultiplier <= 0.0001f
                ? 100000.0f
                : g_horse.sprintDrain / durationMultiplier;
    }

    if (WriteFloat(g_horse.movement, kMaxWalkSpeedOffset, targetWalk)) {
        g_appliedMaxWalkSpeed.store(targetWalk);
    }
    if (WriteFloat(g_horse.movement, kMaxAccelerationOffset, targetAcceleration)) {
        g_appliedMaxAcceleration.store(targetAcceleration);
    }
    WriteFloat(
        g_horse.movement,
        kBrakingFrictionFactorOffset,
        targetBraking
    );
    if (WriteFloat(g_horse.horse, kHorseSprintingMaxSpeedOffset, targetSprintMax)) {
        g_appliedSprintingMaxSpeed.store(targetSprintMax);
    }
    if (WriteFloat(g_horse.horse, kStaminaSprintDrainOffset, targetDrain)) {
        g_appliedSprintDrain.store(targetDrain);
    }
}

bool CaptureDirectHorse(
    void* knownLocalPlayer,
    void* horse,
    void* movement
) {
    void* ownerBack = nullptr;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float brakingFriction = 0.0f;
    float sprintingMaxSpeed = 0.0f;
    float recovery = 0.0f;
    float totalRecovery = 0.0f;
    float sprintDrain = 0.0f;
    unsigned char sprinting = 0;

    const bool ok =
        ReadAt(movement, kCharacterOwnerOffset, ownerBack) &&
        ownerBack == horse &&
        ReadFloat(movement, kMaxWalkSpeedOffset, maxWalkSpeed) &&
        ReadFloat(movement, kMaxAccelerationOffset, maxAcceleration) &&
        ReadFloat(movement, kBrakingFrictionFactorOffset, brakingFriction) &&
        ReadFloat(horse, kHorseSprintingMaxSpeedOffset, sprintingMaxSpeed) &&
        ReadAt(horse, kHorseIsSprintingOffset, sprinting) &&
        ReadFloat(horse, kStaminaRecoveryOffset, recovery) &&
        ReadFloat(horse, kStaminaTotalRecoveryOffset, totalRecovery) &&
        ReadFloat(horse, kStaminaSprintDrainOffset, sprintDrain);

    if (!ok) {
        FeatureLog(
            "HorseFeature V0.23B: DIRECT CAPTURE REJECTED "
            "player=%p horse=%p movement=%p ownerBack=%p ownerMatch=%d",
            knownLocalPlayer,
            horse,
            movement,
            ownerBack,
            ownerBack == horse ? 1 : 0
        );
        return false;
    }

    if (maxWalkSpeed <= 0.0f ||
        maxAcceleration <= 0.0f ||
        sprintingMaxSpeed <= 0.0f ||
        sprintDrain < 0.0f) {
        FeatureLog(
            "HorseFeature V0.23B: DIRECT VALUES REJECTED "
            "walk=%.3f accel=%.3f sprintMax=%.3f drain=%.3f",
            maxWalkSpeed,
            maxAcceleration,
            sprintingMaxSpeed,
            sprintDrain
        );
        return false;
    }

    AcquireSRWLockExclusive(&g_lock);

    if (!g_horse.captured ||
        g_horse.horse != horse ||
        g_horse.movement != movement) {
        if (g_horse.captured) {
            RestoreLocked();
        }

        g_horse.player = knownLocalPlayer;
        g_horse.horse = horse;
        g_horse.movement = movement;
        g_horse.maxWalkSpeed = maxWalkSpeed;
        g_horse.maxAcceleration = maxAcceleration;
        g_horse.brakingFrictionFactor = brakingFriction;
        g_horse.sprintingMaxSpeed = sprintingMaxSpeed;
        g_horse.sprintDrain = sprintDrain;
        g_horse.captured = true;

        g_directChainsLogged.fetch_add(1);
        g_directMatches.fetch_add(1);

        FeatureLog(
            "HorseFeature V0.23B: DIRECT VALIDATED "
            "player=%p horse=%p movement=%p ownerMatch=1 "
            "walk=%.1f accel=%.1f brake=%.2f sprintMax=%.1f "
            "sprinting=%u stamina=[recovery %.1f total %.1f drain %.1f]",
            knownLocalPlayer,
            horse,
            movement,
            maxWalkSpeed,
            maxAcceleration,
            brakingFriction,
            sprintingMaxSpeed,
            static_cast<unsigned>(sprinting),
            recovery,
            totalRecovery,
            sprintDrain
        );
    }

    g_validated.store(true);
    g_staminaReady.store(true);
    g_sprinting.store(sprinting != 0);
    g_player.store(knownLocalPlayer);
    g_owner.store(horse);
    g_movement.store(movement);
    g_nativeMaxWalkSpeed.store(g_horse.maxWalkSpeed);
    g_nativeMaxAcceleration.store(g_horse.maxAcceleration);
    g_nativeSprintingMaxSpeed.store(g_horse.sprintingMaxSpeed);
    g_nativeSprintDrain.store(g_horse.sprintDrain);
    g_lastSeenTick.store(GetTickCount64());

    ApplyFieldsLocked();

    ReleaseSRWLockExclusive(&g_lock);
    return true;
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;
    FeatureLog(
        "HorseFeature V0.23B: reflected direct reference-PAK fields armed "
        "player+0xE70 -> horse, horse+0x390 -> CharacterMovement, walk=1500 accel=700 brake=2.0, "
        "movement+0x190 owner, horse+0x760 SprintingMaxSpeed, "
        "horse+0x8D4 bSprinting, stamina recovery/total/drain=0x910/0x914/0x920"
    );
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintSpeedEnabled.store(settings.sprintSpeedEnabled);
    g_sprintSpeedMultiplier.store(settings.sprintSpeedMultiplier);
    g_sprintDurationEnabled.store(settings.sprintDurationEnabled);
    g_sprintDurationMultiplier.store(settings.sprintDurationMultiplier);
}

void PollDirectHorse(void* knownLocalPlayer) {
    if (!knownLocalPlayer) return;

    void* horse = nullptr;
    if (!ReadAt(knownLocalPlayer, kPlayerHorseMountOffset, horse) || !horse) {
        return;
    }

    void* horseMovement = nullptr;
    if (!ReadAt(horse, kHorseMovementOffset, horseMovement) || !horseMovement) {
        FeatureLog(
            "HorseFeature V0.23B: player+E70 horse=%p but horse+390 movement unavailable",
            horse
        );
        return;
    }

    CaptureDirectHorse(
        knownLocalPlayer,
        horse,
        horseMovement
    );
}

void ObserveMovement(
    void* movementComponent,
    void* characterOwner,
    void* knownLocalPlayer,
    float nativeGetMaxSpeed
) {
    PollDirectHorse(knownLocalPlayer);

    if (!g_validated.load() ||
        movementComponent != g_movement.load() ||
        characterOwner != g_owner.load()) {
        return;
    }

    unsigned char sprinting = 0;
    if (ReadAt(g_owner.load(), kHorseIsSprintingOffset, sprinting)) {
        g_sprinting.store(sprinting != 0);
    }

    if (nativeGetMaxSpeed > 0.0f) {
        g_nativeGetMaxSpeed.store(nativeGetMaxSpeed);
        g_appliedGetMaxSpeed.store(nativeGetMaxSpeed);
    }

    g_lastSeenTick.store(GetTickCount64());
}

float AdjustSpeedResult(
    void* movementComponent,
    float nativeGetMaxSpeed
) {
    if (movementComponent &&
        g_validated.load() &&
        movementComponent == g_movement.load()) {
        g_nativeGetMaxSpeed.store(nativeGetMaxSpeed);
        g_appliedGetMaxSpeed.store(nativeGetMaxSpeed);
    }
    return nativeGetMaxSpeed;
}

void Tick() {
    if (!g_validated.load()) return;

    const ULONGLONG lastSeen = g_lastSeenTick.load();
    const ULONGLONG now = GetTickCount64();
    if (lastSeen != 0 &&
        now - lastSeen > kHorseLostTimeoutMs) {
        ClearHorse("direct horse pointer no longer observed");
    }
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.validated = g_validated.load();
    t.staminaReady = g_staminaReady.load();
    t.sprinting = g_sprinting.load();
    t.playerOwner = g_player.load();
    t.horseOwner = g_owner.load();
    t.horseMovement = g_movement.load();
    t.nativeGetMaxSpeed = g_nativeGetMaxSpeed.load();
    t.appliedGetMaxSpeed = g_appliedGetMaxSpeed.load();
    t.nativeMaxWalkSpeed = g_nativeMaxWalkSpeed.load();
    t.appliedMaxWalkSpeed = g_appliedMaxWalkSpeed.load();
    t.nativeMaxAcceleration = g_nativeMaxAcceleration.load();
    t.appliedMaxAcceleration = g_appliedMaxAcceleration.load();
    t.nativeSprintingMaxSpeed = g_nativeSprintingMaxSpeed.load();
    t.appliedSprintingMaxSpeed = g_appliedSprintingMaxSpeed.load();
    t.nativeSprintDrain = g_nativeSprintDrain.load();
    t.appliedSprintDrain = g_appliedSprintDrain.load();
    t.uniqueCandidatesLogged = g_directChainsLogged.load();
    t.candidateMatches = g_directMatches.load();
    return t;
}

bool IsValidatedMovement(void* movementComponent) {
    return movementComponent &&
           g_validated.load() &&
           movementComponent == g_movement.load();
}

void Shutdown() {
    ClearHorse("shutdown");
}

} // namespace dg::horse
