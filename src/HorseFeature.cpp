#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "HorseFeature.h"

#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <cmath>
#include <cstdio>

namespace dg::horse {
namespace {

constexpr std::size_t kMountedSignalOffset = 0xE70;
constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kMovementModeOffset = 0x1B0;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1D4;
constexpr std::size_t kMaxAccelerationOffset = 0x1E8;

constexpr std::size_t kStaminaRecoveryOffset = 0x910;
constexpr std::size_t kStaminaTotalRecoveryOffset = 0x914;
constexpr std::size_t kStaminaSprintDrainOffset = 0x918;

constexpr float kReferenceMaxWalkSpeed = 1300.0f;
constexpr float kReferenceMaxAcceleration = 600.0f;

struct CapturedHorse {
    void* owner = nullptr;
    void* movement = nullptr;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float sprintDrain = 0.0f;
    bool staminaReady = false;
    bool captured = false;
};

SRWLOCK g_lock = SRWLOCK_INIT;
CapturedHorse g_horse{};

std::atomic_bool g_speedEnabled{true};
std::atomic<float> g_speedMultiplier{1.25f};
std::atomic_bool g_sprintDurationEnabled{true};
std::atomic<float> g_sprintDurationMultiplier{2.0f};

std::atomic_bool g_mounted{false};
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
std::atomic_uint32_t g_candidateChecks{0};
std::atomic_uint32_t g_candidateMatches{0};

LogFn g_logger = nullptr;

void FeatureLog(const char* fmt, ...) {
    if (!g_logger) {
        return;
    }

    char buffer[512]{};
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

    const auto* p = reinterpret_cast<const unsigned char*>(base) + offset;
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

bool IsMounted(void* localPlayer) {
    void* opaque = nullptr;
    return localPlayer &&
           ReadAt(localPlayer, kMountedSignalOffset, opaque) &&
           opaque != nullptr &&
           opaque != localPlayer;
}

bool LooksLikeReferenceHorseMovement(
    void* movement,
    void* owner,
    float nativeGetMaxSpeed,
    float& maxWalkSpeed,
    float& maxAcceleration
) {
    if (!movement || !owner || nativeGetMaxSpeed <= 0.0f ||
        !std::isfinite(nativeGetMaxSpeed)) {
        return false;
    }

    void* ownerBack = nullptr;
    if (!ReadAt(movement, kCharacterOwnerOffset, ownerBack) ||
        ownerBack != owner) {
        return false;
    }

    unsigned char movementMode = 0;
    if (!ReadAt(movement, kMovementModeOffset, movementMode) ||
        movementMode > 6) {
        return false;
    }

    if (!ReadFloat(movement, kMaxWalkSpeedOffset, maxWalkSpeed) ||
        !ReadFloat(movement, kMaxAccelerationOffset, maxAcceleration)) {
        return false;
    }

    // Reference PAK values are 1300 / 600. Keep this intentionally tight.
    // We want one recognizable horse component, not another actor scanner.
    const bool speedMatches =
        std::fabs(maxWalkSpeed - kReferenceMaxWalkSpeed) <= 250.0f;
    const bool accelerationMatches =
        std::fabs(maxAcceleration - kReferenceMaxAcceleration) <= 250.0f;
    const bool getMaxSpeedSane =
        nativeGetMaxSpeed >= 800.0f && nativeGetMaxSpeed <= 1800.0f;

    return speedMatches && accelerationMatches && getMaxSpeedSane;
}

bool CaptureStaminaIfValid(void* owner, float& sprintDrain) {
    float recovery = 0.0f;
    float totalRecovery = 0.0f;

    if (!ReadFloat(owner, kStaminaRecoveryOffset, recovery) ||
        !ReadFloat(owner, kStaminaTotalRecoveryOffset, totalRecovery) ||
        !ReadFloat(owner, kStaminaSprintDrainOffset, sprintDrain)) {
        return false;
    }

    return recovery >= 0.0f && recovery <= 100.0f &&
           totalRecovery >= 0.0f && totalRecovery <= 100.0f &&
           sprintDrain > 0.0f && sprintDrain <= 100.0f;
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
}

void RestoreLocked() {
    if (!g_horse.captured) {
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

    if (g_horse.staminaReady) {
        WriteFloat(
            g_horse.owner,
            kStaminaSprintDrainOffset,
            g_horse.sprintDrain
        );
    }
}

void ClearHorse(bool restore) {
    AcquireSRWLockExclusive(&g_lock);

    if (restore) {
        RestoreLocked();
    }

    g_horse = {};
    PublishCleared();

    ReleaseSRWLockExclusive(&g_lock);
}

void ApplyLocked() {
    if (!g_horse.captured) {
        return;
    }

    const float speedMultiplier = Clamp(g_speedMultiplier.load(), 0.0f, 3.0f);
    const float targetSpeed = g_speedEnabled.load()
        ? g_horse.maxWalkSpeed * speedMultiplier
        : g_horse.maxWalkSpeed;
    const float targetAcceleration = g_speedEnabled.load()
        ? g_horse.maxAcceleration * speedMultiplier
        : g_horse.maxAcceleration;

    const bool speedWritten =
        WriteFloat(g_horse.movement, kMaxWalkSpeedOffset, targetSpeed);
    const bool accelerationWritten =
        WriteFloat(g_horse.movement, kMaxAccelerationOffset, targetAcceleration);

    if (speedWritten && accelerationWritten) {
        g_appliedMaxWalkSpeed.store(targetSpeed);
        g_appliedMaxAcceleration.store(targetAcceleration);
    }

    if (!g_horse.staminaReady) {
        return;
    }

    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 10.0f);

    float targetDrain = g_horse.sprintDrain;
    if (g_sprintDurationEnabled.load()) {
        targetDrain = durationMultiplier <= 0.0001f
            ? 100000.0f
            : g_horse.sprintDrain / durationMultiplier;
    }

    if (WriteFloat(g_horse.owner, kStaminaSprintDrainOffset, targetDrain)) {
        g_appliedSprintDrain.store(targetDrain);
    }
}

bool TryCapture(
    void* movement,
    void* owner,
    float nativeGetMaxSpeed
) {
    if (g_validated.load()) {
        return movement == g_movement.load();
    }

    g_candidateChecks.fetch_add(1);

    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    if (!LooksLikeReferenceHorseMovement(
            movement,
            owner,
            nativeGetMaxSpeed,
            maxWalkSpeed,
            maxAcceleration)) {
        return false;
    }

    float sprintDrain = 0.0f;
    const bool staminaReady = CaptureStaminaIfValid(owner, sprintDrain);

    AcquireSRWLockExclusive(&g_lock);

    if (!g_horse.captured) {
        g_horse.owner = owner;
        g_horse.movement = movement;
        g_horse.maxWalkSpeed = maxWalkSpeed;
        g_horse.maxAcceleration = maxAcceleration;
        g_horse.sprintDrain = sprintDrain;
        g_horse.staminaReady = staminaReady;
        g_horse.captured = true;

        g_validated.store(true);
        g_staminaReady.store(staminaReady);
        g_owner.store(owner);
        g_movement.store(movement);
        g_nativeMaxWalkSpeed.store(maxWalkSpeed);
        g_appliedMaxWalkSpeed.store(maxWalkSpeed);
        g_nativeMaxAcceleration.store(maxAcceleration);
        g_appliedMaxAcceleration.store(maxAcceleration);
        g_nativeSprintDrain.store(staminaReady ? sprintDrain : 0.0f);
        g_appliedSprintDrain.store(staminaReady ? sprintDrain : 0.0f);
        g_candidateMatches.fetch_add(1);

        FeatureLog(
            "HorseFeature: VALIDATED movement=%p owner=%p "
            "MaxWalkSpeed=%.1f MaxAcceleration=%.1f stamina=%s drain=%.3f",
            movement,
            owner,
            maxWalkSpeed,
            maxAcceleration,
            staminaReady ? "READY" : "unavailable",
            sprintDrain
        );

        ApplyLocked();
    }

    const bool accepted = g_horse.movement == movement;
    ReleaseSRWLockExclusive(&g_lock);
    return accepted;
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;
    FeatureLog(
        "HorseFeature: clean movement-signature path armed "
        "reference MaxWalkSpeed=1300 MaxAcceleration=600"
    );
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintDurationEnabled.store(settings.sprintDurationEnabled);
    g_sprintDurationMultiplier.store(settings.sprintDurationMultiplier);
}

void OnGetMaxSpeed(
    void* movementComponent,
    void* characterOwner,
    void* localPlayer,
    float nativeGetMaxSpeed
) {
    if (!movementComponent || !characterOwner || !localPlayer) {
        return;
    }

    const bool mounted = IsMounted(localPlayer);
    const bool previousMounted = g_mounted.exchange(mounted);

    if (!mounted) {
        if (previousMounted || g_validated.load()) {
            FeatureLog("HorseFeature: unmounted -> restore captured values");
            ClearHorse(true);
        }
        return;
    }

    if (!previousMounted) {
        FeatureLog("HorseFeature: mounted signal ON");
    }

    if (characterOwner == localPlayer) {
        return;
    }

    if (g_validated.load()) {
        if (movementComponent != g_movement.load()) {
            return;
        }

        AcquireSRWLockExclusive(&g_lock);
        ApplyLocked();
        ReleaseSRWLockExclusive(&g_lock);
        return;
    }

    TryCapture(movementComponent, characterOwner, nativeGetMaxSpeed);
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.mounted = g_mounted.load();
    t.movementValidated = g_validated.load();
    t.staminaReady = g_staminaReady.load();
    t.horseOwner = g_owner.load();
    t.horseMovement = g_movement.load();
    t.nativeMaxWalkSpeed = g_nativeMaxWalkSpeed.load();
    t.appliedMaxWalkSpeed = g_appliedMaxWalkSpeed.load();
    t.nativeMaxAcceleration = g_nativeMaxAcceleration.load();
    t.appliedMaxAcceleration = g_appliedMaxAcceleration.load();
    t.nativeSprintDrain = g_nativeSprintDrain.load();
    t.appliedSprintDrain = g_appliedSprintDrain.load();
    t.candidateChecks = g_candidateChecks.load();
    t.candidateMatches = g_candidateMatches.load();
    return t;
}

void Shutdown() {
    ClearHorse(true);
    g_mounted.store(false);
    FeatureLog("HorseFeature: shutdown");
}

} // namespace dg::horse
