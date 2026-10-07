#include <windows.h>

#include "HorseFeature.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdarg>
#include <cmath>
#include <cstdio>

namespace dg::horse {
namespace {

constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kJumpZOffset = 0x1A0;
constexpr std::size_t kMovementModeOffset = 0x1B0;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1DC;
constexpr std::size_t kMaxAccelerationOffset = 0x1F0;

constexpr std::size_t kStaminaRecoveryOffset = 0x910;
constexpr std::size_t kStaminaTotalRecoveryOffset = 0x914;
constexpr std::size_t kStaminaSprintDrainOffset = 0x918;

constexpr float kHorseWalkSpeed = 1300.0f;
constexpr float kHorseAcceleration = 600.0f;
constexpr float kHorseRecovery = 15.0f;
constexpr float kHorseTotalRecovery = 40.0f;
constexpr float kHorseSprintDrain = 25.0f;

constexpr ULONGLONG kHorseLostTimeoutMs = 1500;
constexpr std::size_t kMaxCandidateSnapshots = 8;

struct HorseState {
    void* owner = nullptr;
    void* movement = nullptr;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float sprintDrain = 0.0f;
    bool captured = false;
};

SRWLOCK g_lock = SRWLOCK_INIT;
HorseState g_horse{};

std::atomic_bool g_speedEnabled{true};
std::atomic<float> g_speedMultiplier{1.25f};
std::atomic_bool g_sprintDurationEnabled{true};
std::atomic<float> g_sprintDurationMultiplier{2.0f};

std::atomic_bool g_validated{false};
std::atomic_bool g_staminaReady{false};
std::atomic<void*> g_owner{nullptr};
std::atomic<void*> g_movement{nullptr};
std::atomic<float> g_nativeMaxWalkSpeed{0.0f};
std::atomic<float> g_appliedMaxWalkSpeed{0.0f};
std::atomic<float> g_nativeMaxAcceleration{0.0f};
std::atomic<float> g_appliedMaxAcceleration{0.0f};
std::atomic<float> g_nativeSprintDrain{0.0f};
std::atomic<float> g_appliedSprintDrain{0.0f};
std::atomic_uint32_t g_uniqueCandidatesLogged{0};
std::atomic_uint32_t g_candidateMatches{0};
std::atomic_ullong g_lastSeenTick{0};

std::array<void*, kMaxCandidateSnapshots> g_loggedCandidates{};
SRWLOCK g_candidateLogLock = SRWLOCK_INIT;

LogFn g_logger = nullptr;

void FeatureLog(const char* fmt, ...) {
    if (!g_logger) {
        return;
    }

    char buffer[768]{};
    va_list args;
    va_start(args, fmt);
    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    va_end(args);
    g_logger(buffer);
}

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

bool Writable(void* address, std::size_t bytes) {
    if (!Readable(address, bytes)) {
        return false;
    }

    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(address, &mbi, sizeof(mbi))) {
        return false;
    }

    const DWORD protect = mbi.Protect & 0xFF;
    return protect == PAGE_READWRITE ||
           protect == PAGE_WRITECOPY ||
           protect == PAGE_EXECUTE_READWRITE ||
           protect == PAGE_EXECUTE_WRITECOPY;
}

template <typename T>
bool ReadAt(const void* base, std::size_t offset, T& out) {
    if (!base) {
        return false;
    }

    const auto* p =
        reinterpret_cast<const unsigned char*>(base) + offset;
    if (!Readable(p, sizeof(T))) {
        return false;
    }

    out = *reinterpret_cast<const T*>(p);
    return true;
}

bool ReadFloat(const void* base, std::size_t offset, float& out) {
    if (!ReadAt(base, offset, out)) {
        return false;
    }

    return std::isfinite(out);
}

bool WriteFloat(void* base, std::size_t offset, float value) {
    if (!base || !std::isfinite(value)) {
        return false;
    }

    auto* p = reinterpret_cast<unsigned char*>(base) + offset;
    if (!Writable(p, sizeof(float))) {
        return false;
    }

    *reinterpret_cast<float*>(p) = value;
    return true;
}

float Clamp(float value, float low, float high) {
    return std::clamp(value, low, high);
}

bool IsHorseSignature(
    float maxWalkSpeed,
    float maxAcceleration,
    float recovery,
    float totalRecovery,
    float sprintDrain
) {
    return
        std::fabs(maxWalkSpeed - kHorseWalkSpeed) <= 150.0f &&
        std::fabs(maxAcceleration - kHorseAcceleration) <= 120.0f &&
        std::fabs(recovery - kHorseRecovery) <= 10.0f &&
        std::fabs(totalRecovery - kHorseTotalRecovery) <= 15.0f &&
        std::fabs(sprintDrain - kHorseSprintDrain) <= 10.0f;
}

bool MarkCandidateForSingleSnapshot(void* movement) {
    if (!movement) {
        return false;
    }

    AcquireSRWLockExclusive(&g_candidateLogLock);

    for (void* existing : g_loggedCandidates) {
        if (existing == movement) {
            ReleaseSRWLockExclusive(&g_candidateLogLock);
            return false;
        }
    }

    for (void*& slot : g_loggedCandidates) {
        if (!slot) {
            slot = movement;
            g_uniqueCandidatesLogged.fetch_add(1);
            ReleaseSRWLockExclusive(&g_candidateLogLock);
            return true;
        }
    }

    ReleaseSRWLockExclusive(&g_candidateLogLock);
    return false;
}

void LogCandidateOnce(
    void* movement,
    void* owner,
    float nativeGetMaxSpeed
) {
    float jumpZ = 0.0f;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float recovery = 0.0f;
    float totalRecovery = 0.0f;
    float sprintDrain = 0.0f;
    unsigned char movementMode = 0;

    const bool jumpOk =
        ReadFloat(movement, kJumpZOffset, jumpZ);
    const bool walkOk =
        ReadFloat(movement, kMaxWalkSpeedOffset, maxWalkSpeed);
    const bool accelerationOk =
        ReadFloat(movement, kMaxAccelerationOffset, maxAcceleration);
    const bool modeOk =
        ReadAt(movement, kMovementModeOffset, movementMode);
    const bool staminaOk =
        ReadFloat(owner, kStaminaRecoveryOffset, recovery) &&
        ReadFloat(owner, kStaminaTotalRecoveryOffset, totalRecovery) &&
        ReadFloat(owner, kStaminaSprintDrainOffset, sprintDrain);

    // Do not spend the finite diagnostic budget on unrelated movement
    // components. The broad range still comfortably contains the proven
    // horse defaults 1300 / 600.
    if (!walkOk ||
        !accelerationOk ||
        maxWalkSpeed < 800.0f ||
        maxWalkSpeed > 1800.0f ||
        maxAcceleration < 250.0f ||
        maxAcceleration > 1600.0f ||
        !MarkCandidateForSingleSnapshot(movement)) {
        return;
    }

    FeatureLog(
        "HorseFeature: candidate movement=%p owner=%p native=%.1f "
        "mode=%s%u jump=%s%.1f walk=%s%.1f accel=%s%.1f "
        "stamina=%s[%.1f,%.1f,%.1f]",
        movement,
        owner,
        nativeGetMaxSpeed,
        modeOk ? "" : "?",
        static_cast<unsigned>(movementMode),
        jumpOk ? "" : "?",
        jumpZ,
        walkOk ? "" : "?",
        maxWalkSpeed,
        accelerationOk ? "" : "?",
        maxAcceleration,
        staminaOk ? "" : "?",
        recovery,
        totalRecovery,
        sprintDrain
    );
}

void PublishCleared() {
    g_validated.store(false);
    g_staminaReady.store(false);
    g_owner.store(nullptr);
    g_movement.store(nullptr);
    g_nativeMaxWalkSpeed.store(0.0f);
    g_appliedMaxWalkSpeed.store(0.0f);
    g_nativeMaxAcceleration.store(0.0f);
    g_appliedMaxAcceleration.store(0.0f);
    g_nativeSprintDrain.store(0.0f);
    g_appliedSprintDrain.store(0.0f);
    g_lastSeenTick.store(0);
}

void RestoreLocked() {
    if (!g_horse.captured) {
        return;
    }

    void* ownerBack = nullptr;
    float currentWalk = 0.0f;
    float currentAcceleration = 0.0f;
    float currentDrain = 0.0f;

    const bool stillSameHorse =
        ReadAt(
            g_horse.movement,
            kCharacterOwnerOffset,
            ownerBack) &&
        ownerBack == g_horse.owner &&
        ReadFloat(
            g_horse.movement,
            kMaxWalkSpeedOffset,
            currentWalk) &&
        ReadFloat(
            g_horse.movement,
            kMaxAccelerationOffset,
            currentAcceleration) &&
        ReadFloat(
            g_horse.owner,
            kStaminaSprintDrainOffset,
            currentDrain);

    if (!stillSameHorse) {
        FeatureLog(
            "HorseFeature: restore skipped; captured object identity no longer valid"
        );
        return;
    }

    WriteFloat(
        g_horse.movement,
        kMaxWalkSpeedOffset,
        g_horse.maxWalkSpeed
    );
    WriteFloat(
        g_horse.movement,
        kMaxAccelerationOffset,
        g_horse.maxAcceleration
    );
    WriteFloat(
        g_horse.owner,
        kStaminaSprintDrainOffset,
        g_horse.sprintDrain
    );
}

void ClearHorse(const char* reason) {
    AcquireSRWLockExclusive(&g_lock);

    if (g_horse.captured) {
        RestoreLocked();
        if (reason) {
            FeatureLog(
                "HorseFeature: released validated horse (%s)",
                reason
            );
        }
    }

    g_horse = {};
    PublishCleared();

    ReleaseSRWLockExclusive(&g_lock);
}

void ApplyLocked() {
    if (!g_horse.captured) {
        return;
    }

    // V0.20C isolates the virtual speed-return path. Detection still uses
    // the corrected reflected horse fields, but direct speed-property writes
    // are intentionally disabled in this candidate.
    g_appliedMaxWalkSpeed.store(g_horse.maxWalkSpeed);
    g_appliedMaxAcceleration.store(g_horse.maxAcceleration);

    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 10.0f);

    float targetDrain = g_horse.sprintDrain;
    if (g_sprintDurationEnabled.load()) {
        targetDrain =
            durationMultiplier <= 0.0001f
                ? 100000.0f
                : g_horse.sprintDrain / durationMultiplier;
    }

    if (WriteFloat(
            g_horse.owner,
            kStaminaSprintDrainOffset,
            targetDrain)) {
        g_appliedSprintDrain.store(targetDrain);
    }
}

bool TryCaptureHorse(
    void* movement,
    void* owner,
    float nativeGetMaxSpeed
) {
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float recovery = 0.0f;
    float totalRecovery = 0.0f;
    float sprintDrain = 0.0f;

    if (!ReadFloat(
            movement,
            kMaxWalkSpeedOffset,
            maxWalkSpeed) ||
        !ReadFloat(
            movement,
            kMaxAccelerationOffset,
            maxAcceleration) ||
        !ReadFloat(
            owner,
            kStaminaRecoveryOffset,
            recovery) ||
        !ReadFloat(
            owner,
            kStaminaTotalRecoveryOffset,
            totalRecovery) ||
        !ReadFloat(
            owner,
            kStaminaSprintDrainOffset,
            sprintDrain)) {
        return false;
    }

    if (!IsHorseSignature(
            maxWalkSpeed,
            maxAcceleration,
            recovery,
            totalRecovery,
            sprintDrain)) {
        return false;
    }

    AcquireSRWLockExclusive(&g_lock);

    if (!g_horse.captured) {
        g_horse.owner = owner;
        g_horse.movement = movement;
        g_horse.maxWalkSpeed = maxWalkSpeed;
        g_horse.maxAcceleration = maxAcceleration;
        g_horse.sprintDrain = sprintDrain;
        g_horse.captured = true;

        g_validated.store(true);
        g_staminaReady.store(true);
        g_owner.store(owner);
        g_movement.store(movement);
        g_nativeMaxWalkSpeed.store(maxWalkSpeed);
        g_appliedMaxWalkSpeed.store(maxWalkSpeed);
        g_nativeMaxAcceleration.store(maxAcceleration);
        g_appliedMaxAcceleration.store(maxAcceleration);
        g_nativeSprintDrain.store(sprintDrain);
        g_appliedSprintDrain.store(sprintDrain);
        g_candidateMatches.fetch_add(1);

        FeatureLog(
            "HorseFeature: VALIDATED shared-hook movement=%p owner=%p "
            "nativeGetMaxSpeed=%.1f walk=%.1f accel=%.1f "
            "stamina=[%.1f,%.1f,%.1f]",
            movement,
            owner,
            nativeGetMaxSpeed,
            maxWalkSpeed,
            maxAcceleration,
            recovery,
            totalRecovery,
            sprintDrain
        );
    }

    const bool accepted = g_horse.movement == movement;
    if (accepted) {
        g_lastSeenTick.store(GetTickCount64());
        ApplyLocked();
    }

    ReleaseSRWLockExclusive(&g_lock);
    return accepted;
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;
    FeatureLog(
        "HorseFeature V0.20C: corrected detection offsets armed "
        "walk=0x1DC accel=0x1F0; direct speed writes disabled"
    );
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintDurationEnabled.store(settings.sprintDurationEnabled);
    g_sprintDurationMultiplier.store(
        settings.sprintDurationMultiplier
    );
}

void ObserveMovement(
    void* movementComponent,
    void* characterOwner,
    float nativeGetMaxSpeed
) {
    if (!movementComponent || !characterOwner) {
        return;
    }

    if (g_validated.load()) {
        if (movementComponent != g_movement.load()) {
            return;
        }

        g_lastSeenTick.store(GetTickCount64());

        AcquireSRWLockExclusive(&g_lock);
        ApplyLocked();
        ReleaseSRWLockExclusive(&g_lock);
        return;
    }

    LogCandidateOnce(
        movementComponent,
        characterOwner,
        nativeGetMaxSpeed
    );

    TryCaptureHorse(
        movementComponent,
        characterOwner,
        nativeGetMaxSpeed
    );
}

void Tick() {
    if (!g_validated.load()) {
        return;
    }

    const ULONGLONG lastSeen = g_lastSeenTick.load();
    const ULONGLONG now = GetTickCount64();

    if (lastSeen != 0 &&
        now - lastSeen > kHorseLostTimeoutMs) {
        ClearHorse("movement no longer observed");
    }
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.validated = g_validated.load();
    t.staminaReady = g_staminaReady.load();
    t.horseOwner = g_owner.load();
    t.horseMovement = g_movement.load();
    t.nativeMaxWalkSpeed = g_nativeMaxWalkSpeed.load();
    t.appliedMaxWalkSpeed = g_appliedMaxWalkSpeed.load();
    t.nativeMaxAcceleration = g_nativeMaxAcceleration.load();
    t.appliedMaxAcceleration =
        g_appliedMaxAcceleration.load();
    t.nativeSprintDrain = g_nativeSprintDrain.load();
    t.appliedSprintDrain = g_appliedSprintDrain.load();
    t.uniqueCandidatesLogged =
        g_uniqueCandidatesLogged.load();
    t.candidateMatches = g_candidateMatches.load();
    return t;
}

void Shutdown() {
    ClearHorse("shutdown");
}

} // namespace dg::horse

bool IsValidatedMovement(void* movementComponent) {
    return
        movementComponent != nullptr &&
        g_validated.load() &&
        movementComponent == g_movement.load();
}
