#include <windows.h>

#include "HorseFeature.h"

#include <MinHook.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace dg::horse {
namespace {

constexpr std::size_t kPlayerHorseMountOffset = 0xE78;
constexpr std::size_t kHorseIsSprintingOffset = 0x8D4;
constexpr std::size_t kHorseSprintingMaxSpeedOffset = 0x760;

constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1E0;
constexpr std::size_t kMaxAccelerationOffset = 0x1F4;
constexpr std::size_t kBrakingFrictionFactorOffset = 0x1FC;

constexpr std::size_t kStaminaRecoveryOffset = 0x910;
constexpr std::size_t kStaminaTotalRecoveryOffset = 0x914;
constexpr std::size_t kStaminaSprintDrainOffset = 0x920;

constexpr std::uintptr_t kExecGetHorseMountRva = 0x7ACC00;
constexpr std::uintptr_t kExecIsHorseActiveRva = 0x7AD6D0;

constexpr std::array<unsigned char, 14> kExecWrapperPrefix{
    0x48, 0x8B, 0x42, 0x20,
    0x45, 0x33, 0xC9,
    0x48, 0x85, 0xC0,
    0x41, 0x0F, 0x95, 0xC1
};

constexpr std::size_t kMaxHorseSlots = 4;
constexpr std::size_t kMaxMovementSnapshots = 32;

struct HorseSlot {
    void* player = nullptr;
    void* horse = nullptr;
    void* movement = nullptr;

    bool movementValidated = false;
    bool speedFieldsReady = false;
    bool sprintFieldsReady = false;
    bool staminaReady = false;

    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float brakingFrictionFactor = 0.0f;
    float sprintingMaxSpeed = 0.0f;
    float sprintDrain = 0.0f;

    float nativeGetMaxSpeed = 0.0f;
    float appliedGetMaxSpeed = 0.0f;
};

SRWLOCK g_lock = SRWLOCK_INIT;
std::array<HorseSlot, kMaxHorseSlots> g_slots{};
std::array<void*, kMaxMovementSnapshots> g_seenMovement{};

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

std::atomic_uint32_t g_snapshotCount{0};
std::atomic_uint32_t g_horseCount{0};
std::atomic_uint32_t g_getHorseMountCalls{0};
std::atomic_uint32_t g_isHorseActiveCalls{0};

using ExecWrapperFn = void(*)(void*, void*, void*);
ExecWrapperFn g_originalGetHorseMount = nullptr;
ExecWrapperFn g_originalIsHorseActive = nullptr;
void* g_getHorseMountTarget = nullptr;
void* g_isHorseActiveTarget = nullptr;
bool g_getHorseMountHookOwned = false;
bool g_isHorseActiveHookOwned = false;

LogFn g_logger = nullptr;

void FeatureLog(const char* fmt, ...) {
    if (!g_logger) {
        return;
    }

    char buffer[1024]{};
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
    const auto regionBegin =
        reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
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

bool ExecutableAddress(const void* address) {
    if (!address) {
        return false;
    }

    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(address, &mbi, sizeof(mbi)) ||
        mbi.State != MEM_COMMIT ||
        (mbi.Protect & PAGE_GUARD) ||
        (mbi.Protect & PAGE_NOACCESS)) {
        return false;
    }

    const DWORD protect = mbi.Protect & 0xFF;
    return protect == PAGE_EXECUTE ||
           protect == PAGE_EXECUTE_READ ||
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
    return ReadAt(base, offset, out) && std::isfinite(out);
}

bool WriteFloat(void* base, std::size_t offset, float value) {
    if (!base || !std::isfinite(value)) {
        return false;
    }

    auto* p =
        reinterpret_cast<unsigned char*>(base) + offset;
    if (!Writable(p, sizeof(float))) {
        return false;
    }

    *reinterpret_cast<float*>(p) = value;
    return true;
}

float Clamp(float value, float low, float high) {
    return std::clamp(value, low, high);
}

bool Near(float value, float expected, float tolerance) {
    return std::isfinite(value) &&
           std::fabs(value - expected) <= tolerance;
}

bool LooksLikeUObject(void* object) {
    if (!object || !Readable(object, sizeof(void*))) {
        return false;
    }

    void* vtable = *reinterpret_cast<void**>(object);
    if (!vtable || !Readable(vtable, sizeof(void*))) {
        return false;
    }

    void* firstVirtual = *reinterpret_cast<void**>(vtable);
    return ExecutableAddress(firstVirtual);
}

HorseSlot* FindHorseLocked(void* horse) {
    for (auto& slot : g_slots) {
        if (slot.horse == horse && horse) {
            return &slot;
        }
    }
    return nullptr;
}

HorseSlot* FindMovementLocked(void* movement) {
    for (auto& slot : g_slots) {
        if (slot.movementValidated &&
            slot.movement == movement &&
            movement) {
            return &slot;
        }
    }
    return nullptr;
}

HorseSlot* AllocateHorseLocked(void* horse) {
    if (!horse) {
        return nullptr;
    }

    if (auto* existing = FindHorseLocked(horse)) {
        return existing;
    }

    for (auto& slot : g_slots) {
        if (!slot.horse) {
            slot.horse = horse;
            g_horseCount.fetch_add(1);
            return &slot;
        }
    }

    return nullptr;
}

bool MarkMovementSnapshotLocked(void* movement) {
    if (!movement) {
        return false;
    }

    for (void* seen : g_seenMovement) {
        if (seen == movement) {
            return false;
        }
    }

    for (auto& seen : g_seenMovement) {
        if (!seen) {
            seen = movement;
            g_snapshotCount.fetch_add(1);
            return true;
        }
    }

    return false;
}

void PublishSlotLocked(const HorseSlot& slot) {
    g_validated.store(slot.movementValidated);
    g_staminaReady.store(slot.staminaReady);
    g_player.store(slot.player);
    g_owner.store(slot.horse);
    g_movement.store(slot.movement);

    g_nativeGetMaxSpeed.store(slot.nativeGetMaxSpeed);
    g_appliedGetMaxSpeed.store(slot.appliedGetMaxSpeed);
    g_nativeMaxWalkSpeed.store(slot.maxWalkSpeed);
    g_nativeMaxAcceleration.store(slot.maxAcceleration);
    g_nativeSprintingMaxSpeed.store(slot.sprintingMaxSpeed);
    g_nativeSprintDrain.store(slot.sprintDrain);
}

void RestoreSlotLocked(HorseSlot& slot) {
    if (!slot.horse) {
        return;
    }

    if (slot.movementValidated &&
        slot.movement &&
        slot.speedFieldsReady) {
        WriteFloat(
            slot.movement,
            kMaxWalkSpeedOffset,
            slot.maxWalkSpeed
        );
        WriteFloat(
            slot.movement,
            kMaxAccelerationOffset,
            slot.maxAcceleration
        );
        WriteFloat(
            slot.movement,
            kBrakingFrictionFactorOffset,
            slot.brakingFrictionFactor
        );
    }

    if (slot.sprintFieldsReady) {
        WriteFloat(
            slot.horse,
            kHorseSprintingMaxSpeedOffset,
            slot.sprintingMaxSpeed
        );
    }

    if (slot.staminaReady) {
        WriteFloat(
            slot.horse,
            kStaminaSprintDrainOffset,
            slot.sprintDrain
        );
    }
}

void ApplySlotLocked(HorseSlot& slot) {
    if (!slot.horse || !slot.movementValidated) {
        return;
    }

    const float speedMultiplier =
        Clamp(g_speedMultiplier.load(), 0.0f, 3.0f);
    const float sprintMultiplier =
        Clamp(g_sprintSpeedMultiplier.load(), 0.0f, 3.0f);
    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 10.0f);

    if (slot.speedFieldsReady) {
        const float targetWalk =
            g_speedEnabled.load()
                ? slot.maxWalkSpeed * speedMultiplier
                : slot.maxWalkSpeed;
        const float targetAcceleration =
            g_speedEnabled.load()
                ? slot.maxAcceleration * speedMultiplier
                : slot.maxAcceleration;

        if (WriteFloat(
                slot.movement,
                kMaxWalkSpeedOffset,
                targetWalk)) {
            g_appliedMaxWalkSpeed.store(targetWalk);
        }

        if (WriteFloat(
                slot.movement,
                kMaxAccelerationOffset,
                targetAcceleration)) {
            g_appliedMaxAcceleration.store(targetAcceleration);
        }
    }

    if (slot.sprintFieldsReady) {
        const float targetSprint =
            g_sprintSpeedEnabled.load()
                ? slot.sprintingMaxSpeed * sprintMultiplier
                : slot.sprintingMaxSpeed;

        if (WriteFloat(
                slot.horse,
                kHorseSprintingMaxSpeedOffset,
                targetSprint)) {
            g_appliedSprintingMaxSpeed.store(targetSprint);
        }
    }

    if (slot.staminaReady) {
        float targetDrain = slot.sprintDrain;
        if (g_sprintDurationEnabled.load()) {
            targetDrain =
                durationMultiplier <= 0.0001f
                    ? 100000.0f
                    : slot.sprintDrain / durationMultiplier;
        }

        if (WriteFloat(
                slot.horse,
                kStaminaSprintDrainOffset,
                targetDrain)) {
            g_appliedSprintDrain.store(targetDrain);
        }
    }

    unsigned char sprinting = 0;
    if (ReadAt(
            slot.horse,
            kHorseIsSprintingOffset,
            sprinting)) {
        g_sprinting.store(sprinting != 0);
    }

    PublishSlotLocked(slot);
}

bool CaptureMovementLocked(
    HorseSlot& slot,
    void* movement,
    const char* source
) {
    if (!slot.horse || !movement) {
        return false;
    }

    void* ownerBack = nullptr;
    if (!ReadAt(
            movement,
            kCharacterOwnerOffset,
            ownerBack) ||
        ownerBack != slot.horse) {
        return false;
    }

    const bool newMovement =
        !slot.movementValidated ||
        slot.movement != movement;

    if (newMovement) {
        if (slot.movementValidated &&
            slot.movement &&
            slot.movement != movement) {
            RestoreSlotLocked(slot);
        }

        slot.movement = movement;
        slot.movementValidated = true;

        float walk = 0.0f;
        float acceleration = 0.0f;
        float braking = 0.0f;
        slot.speedFieldsReady =
            ReadFloat(movement, kMaxWalkSpeedOffset, walk) &&
            ReadFloat(
                movement,
                kMaxAccelerationOffset,
                acceleration) &&
            ReadFloat(
                movement,
                kBrakingFrictionFactorOffset,
                braking) &&
            walk > 0.0f &&
            acceleration > 0.0f;

        if (slot.speedFieldsReady) {
            slot.maxWalkSpeed = walk;
            slot.maxAcceleration = acceleration;
            slot.brakingFrictionFactor = braking;
        }

        float sprintMax = 0.0f;
        slot.sprintFieldsReady =
            ReadFloat(
                slot.horse,
                kHorseSprintingMaxSpeedOffset,
                sprintMax) &&
            sprintMax > 0.0f &&
            sprintMax < 10000.0f;
        if (slot.sprintFieldsReady) {
            slot.sprintingMaxSpeed = sprintMax;
        }

        float recovery = 0.0f;
        float totalRecovery = 0.0f;
        float sprintDrain = 0.0f;
        slot.staminaReady =
            ReadFloat(
                slot.horse,
                kStaminaRecoveryOffset,
                recovery) &&
            ReadFloat(
                slot.horse,
                kStaminaTotalRecoveryOffset,
                totalRecovery) &&
            ReadFloat(
                slot.horse,
                kStaminaSprintDrainOffset,
                sprintDrain) &&
            sprintDrain >= 0.0f &&
            sprintDrain < 1000.0f;

        if (slot.staminaReady) {
            slot.sprintDrain = sprintDrain;
        }

        FeatureLog(
            "HorseFeature V0.26: HORSE DETECTED source=%s "
            "player=%p horse=%p movement=%p ownerMatch=1 "
            "walk=%.1f accel=%.1f brake=%.2f "
            "sprintMax=%.1f stamina=[%.1f,%.1f,%.1f]",
            source ? source : "unknown",
            slot.player,
            slot.horse,
            movement,
            slot.maxWalkSpeed,
            slot.maxAcceleration,
            slot.brakingFrictionFactor,
            slot.sprintingMaxSpeed,
            recovery,
            totalRecovery,
            slot.sprintDrain
        );
    }

    ApplySlotLocked(slot);
    return true;
}

HorseSlot* RegisterHorseLocked(
    void* player,
    void* horse,
    const char* source
) {
    if (!horse || !LooksLikeUObject(horse)) {
        return nullptr;
    }

    HorseSlot* slot = AllocateHorseLocked(horse);
    if (!slot) {
        FeatureLog(
            "HorseFeature V0.26: horse registry FULL source=%s horse=%p",
            source ? source : "unknown",
            horse
        );
        return nullptr;
    }

    if (player && !slot->player) {
        slot->player = player;
    }

    if (!slot->movementValidated) {
        FeatureLog(
            "HorseFeature V0.26: HORSE REGISTERED source=%s player=%p horse=%p",
            source ? source : "unknown",
            slot->player,
            horse
        );
    }

    return slot;
}

bool ReadHorseSignature(
    void* movement,
    void* owner,
    float& walk,
    float& acceleration,
    float& braking,
    float& sprintMax,
    float& recovery,
    float& totalRecovery,
    float& sprintDrain,
    unsigned char& sprinting
) {
    if (!movement ||
        !owner ||
        !LooksLikeUObject(owner)) {
        return false;
    }

    if (!ReadFloat(movement, kMaxWalkSpeedOffset, walk) ||
        !ReadFloat(
            movement,
            kMaxAccelerationOffset,
            acceleration) ||
        !ReadFloat(
            movement,
            kBrakingFrictionFactorOffset,
            braking) ||
        !ReadFloat(
            owner,
            kHorseSprintingMaxSpeedOffset,
            sprintMax) ||
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
            sprintDrain) ||
        !ReadAt(
            owner,
            kHorseIsSprintingOffset,
            sprinting)) {
        return false;
    }

    return true;
}

bool MatchesVanillaHorseSignature(
    float walk,
    float acceleration,
    float braking,
    float sprintMax,
    float recovery,
    float totalRecovery,
    float sprintDrain
) {
    return
        Near(walk, 1300.0f, 25.0f) &&
        Near(acceleration, 600.0f, 25.0f) &&
        braking >= 0.0f &&
        braking <= 4.0f &&
        sprintMax > 0.0f &&
        sprintMax < 5000.0f &&
        Near(recovery, 15.0f, 0.5f) &&
        Near(totalRecovery, 40.0f, 0.5f) &&
        Near(sprintDrain, 25.0f, 0.5f);
}

void ObserveMovementOwnerLocked(
    void* movement,
    void* owner,
    float nativeGetMaxSpeed
) {
    if (!movement || !owner) {
        return;
    }

    if (auto* known = FindHorseLocked(owner)) {
        CaptureMovementLocked(
            *known,
            movement,
            "registered horse owner-match"
        );
        known->nativeGetMaxSpeed = nativeGetMaxSpeed;
        known->appliedGetMaxSpeed = nativeGetMaxSpeed;
        PublishSlotLocked(*known);
        return;
    }

    float walk = 0.0f;
    float acceleration = 0.0f;
    float braking = 0.0f;
    float sprintMax = 0.0f;
    float recovery = 0.0f;
    float totalRecovery = 0.0f;
    float sprintDrain = 0.0f;
    unsigned char sprinting = 0;

    const bool fieldsReadable =
        ReadHorseSignature(
            movement,
            owner,
            walk,
            acceleration,
            braking,
            sprintMax,
            recovery,
            totalRecovery,
            sprintDrain,
            sprinting);

    if (MarkMovementSnapshotLocked(movement)) {
        void* movementVtable = nullptr;
        void* ownerVtable = nullptr;
        if (Readable(movement, sizeof(void*))) {
            movementVtable =
                *reinterpret_cast<void**>(movement);
        }
        if (Readable(owner, sizeof(void*))) {
            ownerVtable =
                *reinterpret_cast<void**>(owner);
        }

        if (fieldsReadable) {
            FeatureLog(
                "HorseFeature V0.26: MOVEMENT SNAPSHOT movement=%p mvVt=%p "
                "owner=%p ownerVt=%p nativeSpeed=%.1f "
                "walk=%.1f accel=%.1f brake=%.2f sprintMax=%.1f "
                "sprinting=%u stamina=[%.1f,%.1f,%.1f]",
                movement,
                movementVtable,
                owner,
                ownerVtable,
                nativeGetMaxSpeed,
                walk,
                acceleration,
                braking,
                sprintMax,
                static_cast<unsigned>(sprinting),
                recovery,
                totalRecovery,
                sprintDrain
            );
        } else {
            FeatureLog(
                "HorseFeature V0.26: MOVEMENT SNAPSHOT movement=%p mvVt=%p "
                "owner=%p ownerVt=%p nativeSpeed=%.1f horseFields=UNREADABLE",
                movement,
                movementVtable,
                owner,
                ownerVtable,
                nativeGetMaxSpeed
            );
        }
    }

    if (!fieldsReadable ||
        !MatchesVanillaHorseSignature(
            walk,
            acceleration,
            braking,
            sprintMax,
            recovery,
            totalRecovery,
            sprintDrain)) {
        return;
    }

    HorseSlot* slot =
        RegisterHorseLocked(
            nullptr,
            owner,
            "movement signature 1300/600 + stamina 15/40/25"
        );
    if (!slot) {
        return;
    }

    CaptureMovementLocked(
        *slot,
        movement,
        "movement signature"
    );
    slot->nativeGetMaxSpeed = nativeGetMaxSpeed;
    slot->appliedGetMaxSpeed = nativeGetMaxSpeed;
    PublishSlotLocked(*slot);
}

void HookExecGetHorseMount(
    void* context,
    void* stack,
    void* result
) {
    if (g_originalGetHorseMount) {
        g_originalGetHorseMount(
            context,
            stack,
            result
        );
    }

    g_getHorseMountCalls.fetch_add(1);

    void* horse = nullptr;
    if (result &&
        Readable(result, sizeof(void*))) {
        horse = *reinterpret_cast<void**>(result);
    }

    if (!horse) {
        ReadAt(
            context,
            kPlayerHorseMountOffset,
            horse
        );
    }

    if (!horse) {
        return;
    }

    AcquireSRWLockExclusive(&g_lock);
    RegisterHorseLocked(
        context,
        horse,
        "Blueprint GetHorseMount"
    );
    ReleaseSRWLockExclusive(&g_lock);
}

void HookExecIsHorseActive(
    void* context,
    void* stack,
    void* result
) {
    if (g_originalIsHorseActive) {
        g_originalIsHorseActive(
            context,
            stack,
            result
        );
    }

    g_isHorseActiveCalls.fetch_add(1);

    void* horse = nullptr;
    if (!ReadAt(
            context,
            kPlayerHorseMountOffset,
            horse) ||
        !horse) {
        return;
    }

    AcquireSRWLockExclusive(&g_lock);
    RegisterHorseLocked(
        context,
        horse,
        "Blueprint IsHorseActive"
    );
    ReleaseSRWLockExclusive(&g_lock);
}

bool InstallExecHook(
    std::uintptr_t rva,
    void* detour,
    ExecWrapperFn& original,
    void*& targetOut,
    bool& ownedOut,
    const char* name
) {
    HMODULE module = GetModuleHandleW(nullptr);
    if (!module) {
        return false;
    }

    auto* target =
        reinterpret_cast<unsigned char*>(module) +
        rva;

    if (!Readable(
            target,
            kExecWrapperPrefix.size()) ||
        std::memcmp(
            target,
            kExecWrapperPrefix.data(),
            kExecWrapperPrefix.size()) != 0) {
        FeatureLog(
            "HorseFeature V0.26: %s target validation FAILED RVA=0x%llX",
            name,
            static_cast<unsigned long long>(rva)
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        FeatureLog(
            "HorseFeature V0.26: %s MinHook init FAILED status=%d",
            name,
            static_cast<int>(initStatus)
        );
        return false;
    }

    ExecWrapperFn nativeOriginal = nullptr;
    const MH_STATUS createStatus = MH_CreateHook(
        target,
        detour,
        reinterpret_cast<LPVOID*>(&nativeOriginal)
    );

    if (createStatus != MH_OK) {
        FeatureLog(
            "HorseFeature V0.26: %s hook create FAILED status=%d",
            name,
            static_cast<int>(createStatus)
        );
        return false;
    }

    const MH_STATUS enableStatus =
        MH_EnableHook(target);

    if (enableStatus != MH_OK &&
        enableStatus != MH_ERROR_ENABLED) {
        MH_RemoveHook(target);
        FeatureLog(
            "HorseFeature V0.26: %s hook enable FAILED status=%d",
            name,
            static_cast<int>(enableStatus)
        );
        return false;
    }

    original = nativeOriginal;
    targetOut = target;
    ownedOut = true;

    FeatureLog(
        "HorseFeature V0.26: %s hook READY RVA=0x%llX",
        name,
        static_cast<unsigned long long>(rva)
    );

    return true;
}

void ClearAllLocked(const char* reason) {
    for (auto& slot : g_slots) {
        if (slot.horse) {
            RestoreSlotLocked(slot);
        }
        slot = {};
    }

    g_validated.store(false);
    g_staminaReady.store(false);
    g_sprinting.store(false);
    g_player.store(nullptr);
    g_owner.store(nullptr);
    g_movement.store(nullptr);

    if (reason) {
        FeatureLog(
            "HorseFeature V0.26: registry cleared (%s)",
            reason
        );
    }
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;

    FeatureLog(
        "HorseFeature V0.26: multi-detector armed; "
        "NO player registry. Channels: Blueprint GetHorseMount, "
        "Blueprint IsHorseActive, movement-owner signature."
    );

    InstallExecHook(
        kExecGetHorseMountRva,
        reinterpret_cast<void*>(
            &HookExecGetHorseMount),
        g_originalGetHorseMount,
        g_getHorseMountTarget,
        g_getHorseMountHookOwned,
        "GetHorseMount"
    );

    InstallExecHook(
        kExecIsHorseActiveRva,
        reinterpret_cast<void*>(
            &HookExecIsHorseActive),
        g_originalIsHorseActive,
        g_isHorseActiveTarget,
        g_isHorseActiveHookOwned,
        "IsHorseActive"
    );
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintSpeedEnabled.store(settings.sprintSpeedEnabled);
    g_sprintSpeedMultiplier.store(
        settings.sprintSpeedMultiplier
    );
    g_sprintDurationEnabled.store(
        settings.sprintDurationEnabled
    );
    g_sprintDurationMultiplier.store(
        settings.sprintDurationMultiplier
    );
}

void ObservePlayerCharacter(void*) {
    // V0.26 deliberately ignores player identity for horse discovery.
}

void PollDirectHorse(void*) {
    // V0.26 deliberately ignores the active-player pointer.
}

void ObserveMovement(
    void* movementComponent,
    void* characterOwner,
    void*,
    float nativeGetMaxSpeed
) {
    if (!movementComponent ||
        !characterOwner) {
        return;
    }

    AcquireSRWLockExclusive(&g_lock);
    ObserveMovementOwnerLocked(
        movementComponent,
        characterOwner,
        nativeGetMaxSpeed
    );
    ReleaseSRWLockExclusive(&g_lock);
}

float AdjustSpeedResult(
    void* movementComponent,
    float nativeGetMaxSpeed
) {
    if (!movementComponent) {
        return nativeGetMaxSpeed;
    }

    AcquireSRWLockExclusive(&g_lock);

    HorseSlot* slot =
        FindMovementLocked(movementComponent);

    if (slot) {
        slot->nativeGetMaxSpeed = nativeGetMaxSpeed;
        slot->appliedGetMaxSpeed = nativeGetMaxSpeed;
        ApplySlotLocked(*slot);
    }

    ReleaseSRWLockExclusive(&g_lock);
    return nativeGetMaxSpeed;
}

void Tick() {
    AcquireSRWLockExclusive(&g_lock);
    for (auto& slot : g_slots) {
        if (slot.horse &&
            slot.movementValidated) {
            ApplySlotLocked(slot);
        }
    }
    ReleaseSRWLockExclusive(&g_lock);
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
    t.appliedMaxAcceleration =
        g_appliedMaxAcceleration.load();
    t.nativeSprintingMaxSpeed =
        g_nativeSprintingMaxSpeed.load();
    t.appliedSprintingMaxSpeed =
        g_appliedSprintingMaxSpeed.load();
    t.nativeSprintDrain = g_nativeSprintDrain.load();
    t.appliedSprintDrain = g_appliedSprintDrain.load();
    t.uniqueCandidatesLogged = g_snapshotCount.load();
    t.candidateMatches = g_horseCount.load();
    return t;
}

bool IsValidatedMovement(void* movementComponent) {
    if (!movementComponent) {
        return false;
    }

    AcquireSRWLockShared(&g_lock);
    const bool found =
        FindMovementLocked(movementComponent) != nullptr;
    ReleaseSRWLockShared(&g_lock);
    return found;
}

void Shutdown() {
    if (g_getHorseMountHookOwned &&
        g_getHorseMountTarget) {
        MH_DisableHook(g_getHorseMountTarget);
        MH_RemoveHook(g_getHorseMountTarget);
        g_getHorseMountHookOwned = false;
        g_getHorseMountTarget = nullptr;
        g_originalGetHorseMount = nullptr;
    }

    if (g_isHorseActiveHookOwned &&
        g_isHorseActiveTarget) {
        MH_DisableHook(g_isHorseActiveTarget);
        MH_RemoveHook(g_isHorseActiveTarget);
        g_isHorseActiveHookOwned = false;
        g_isHorseActiveTarget = nullptr;
        g_originalIsHorseActive = nullptr;
    }

    AcquireSRWLockExclusive(&g_lock);
    ClearAllLocked("shutdown");
    ReleaseSRWLockExclusive(&g_lock);
}

} // namespace dg::horse
