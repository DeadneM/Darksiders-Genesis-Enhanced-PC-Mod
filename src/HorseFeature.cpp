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

constexpr std::uintptr_t kNativeHorseSpawnRva = 0x682680;
constexpr std::array<unsigned char, 16> kNativeHorseSpawnPrologue{
    0x40, 0x53, 0x55, 0x41, 0x56, 0x48, 0x83, 0xEC,
    0x60, 0x48, 0x8B, 0xD9, 0x49, 0x8B, 0xE8, 0x48
};

constexpr std::size_t kMaxPlayerSlots = 4;

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
std::array<HorseSlot, kMaxPlayerSlots> g_slots{};

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

std::atomic_uint32_t g_registeredPlayers{0};
std::atomic_uint32_t g_registeredHorses{0};

using NativeHorseSpawnFn = void(*)(void*, void*, void*);
NativeHorseSpawnFn g_originalNativeHorseSpawn = nullptr;
void* g_nativeHorseSpawnTarget = nullptr;
bool g_nativeHorseSpawnHookOwned = false;

LogFn g_logger = nullptr;

void FeatureLog(const char* fmt, ...) {
    if (!g_logger) {
        return;
    }

    char buffer[896]{};
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

HorseSlot* FindSlotByPlayerLocked(void* player) {
    for (auto& slot : g_slots) {
        if (slot.player == player) {
            return &slot;
        }
    }
    return nullptr;
}

HorseSlot* FindSlotByHorseLocked(void* horse) {
    for (auto& slot : g_slots) {
        if (slot.horse == horse && horse) {
            return &slot;
        }
    }
    return nullptr;
}

HorseSlot* FindSlotByMovementLocked(void* movement) {
    for (auto& slot : g_slots) {
        if (slot.movementValidated &&
            slot.movement == movement &&
            movement) {
            return &slot;
        }
    }
    return nullptr;
}

HorseSlot* GetOrCreatePlayerSlotLocked(void* player) {
    if (!player) {
        return nullptr;
    }

    if (auto* existing = FindSlotByPlayerLocked(player)) {
        return existing;
    }

    for (auto& slot : g_slots) {
        if (!slot.player) {
            slot.player = player;
            const auto count = g_registeredPlayers.fetch_add(1) + 1;
            FeatureLog(
                "HorseFeature V0.25: PLAYER REGISTERED slot=%u player=%p",
                count,
                player
            );
            return &slot;
        }
    }

    FeatureLog(
        "HorseFeature V0.25: player registry FULL player=%p",
        player
    );
    return nullptr;
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
            ReadFloat(movement, kMaxAccelerationOffset, acceleration) &&
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
            sprintMax > 0.0f;
        if (slot.sprintFieldsReady) {
            slot.sprintingMaxSpeed = sprintMax;
        }

        float recovery = 0.0f;
        float totalRecovery = 0.0f;
        float sprintDrain = 0.0f;
        slot.staminaReady =
            ReadFloat(slot.horse, kStaminaRecoveryOffset, recovery) &&
            ReadFloat(
                slot.horse,
                kStaminaTotalRecoveryOffset,
                totalRecovery) &&
            ReadFloat(
                slot.horse,
                kStaminaSprintDrainOffset,
                sprintDrain) &&
            sprintDrain >= 0.0f;

        if (slot.staminaReady) {
            slot.sprintDrain = sprintDrain;
        }

        FeatureLog(
            "HorseFeature V0.25: HORSE DETECTED source=%s "
            "player=%p horse=%p movement=%p ownerMatch=1 "
            "speedReady=%d walk=%.1f accel=%.1f brake=%.2f "
            "sprintReady=%d sprintMax=%.1f "
            "staminaReady=%d recovery=%.1f total=%.1f drain=%.1f",
            source ? source : "unknown",
            slot.player,
            slot.horse,
            movement,
            slot.speedFieldsReady ? 1 : 0,
            slot.maxWalkSpeed,
            slot.maxAcceleration,
            slot.brakingFrictionFactor,
            slot.sprintFieldsReady ? 1 : 0,
            slot.sprintingMaxSpeed,
            slot.staminaReady ? 1 : 0,
            recovery,
            totalRecovery,
            slot.sprintDrain
        );
    }

    ApplySlotLocked(slot);
    return true;
}

void RegisterMountLocked(
    void* player,
    void* horse,
    const char* source
) {
    if (!player || !horse || !LooksLikeUObject(horse)) {
        FeatureLog(
            "HorseFeature V0.25: mount register REJECTED source=%s player=%p horse=%p",
            source ? source : "unknown",
            player,
            horse
        );
        return;
    }

    HorseSlot* slot = GetOrCreatePlayerSlotLocked(player);
    if (!slot) {
        return;
    }

    if (slot->horse != horse) {
        if (slot->horse) {
            RestoreSlotLocked(*slot);
        }

        slot->horse = horse;
        slot->movement = nullptr;
        slot->movementValidated = false;
        slot->speedFieldsReady = false;
        slot->sprintFieldsReady = false;
        slot->staminaReady = false;
        slot->nativeGetMaxSpeed = 0.0f;
        slot->appliedGetMaxSpeed = 0.0f;

        const auto count = g_registeredHorses.fetch_add(1) + 1;
        FeatureLog(
            "HorseFeature V0.25: HORSE REGISTERED #%u source=%s player=%p horse=%p",
            count,
            source ? source : "unknown",
            player,
            horse
        );
    }

    // Reflection fallback. This is no longer trusted blindly: movement+0x190
    // must point back to this exact horse before it is accepted.
    void* reflectedMovement = nullptr;
    if (ReadAt(
            horse,
            kHorseMovementOffset,
            reflectedMovement) &&
        reflectedMovement) {
        CaptureMovementLocked(
            *slot,
            reflectedMovement,
            "horse+0x390 owner-back"
        );
    }
}

void RegisterPlayerLocked(
    void* player,
    const char* source
) {
    if (!player || !LooksLikeUObject(player)) {
        return;
    }

    HorseSlot* slot = GetOrCreatePlayerSlotLocked(player);
    if (!slot) {
        return;
    }

    void* horse = nullptr;
    if (ReadAt(
            player,
            kPlayerHorseMountOffset,
            horse) &&
        horse) {
        RegisterMountLocked(player, horse, source);
    }
}

void HookNativeHorseSpawn(
    void* playerCharacter,
    void* arg2,
    void* arg3
) {
    if (g_originalNativeHorseSpawn) {
        g_originalNativeHorseSpawn(
            playerCharacter,
            arg2,
            arg3
        );
    }

    void* horse = nullptr;
    const bool readOk =
        ReadAt(
            playerCharacter,
            kPlayerHorseMountOffset,
            horse);

    FeatureLog(
        "HorseFeature V0.25: NATIVE SPAWN RETURN player=%p m_pHorseMount=%p read=%d",
        playerCharacter,
        horse,
        readOk ? 1 : 0
    );

    if (!readOk || !horse) {
        return;
    }

    AcquireSRWLockExclusive(&g_lock);
    RegisterMountLocked(
        playerCharacter,
        horse,
        "native spawn RVA 0x682680"
    );
    ReleaseSRWLockExclusive(&g_lock);
}

bool InstallNativeHorseSpawnHook() {
    HMODULE module = GetModuleHandleW(nullptr);
    if (!module) {
        return false;
    }

    auto* target =
        reinterpret_cast<unsigned char*>(module) +
        kNativeHorseSpawnRva;

    if (!Readable(target, kNativeHorseSpawnPrologue.size()) ||
        std::memcmp(
            target,
            kNativeHorseSpawnPrologue.data(),
            kNativeHorseSpawnPrologue.size()) != 0) {
        FeatureLog(
            "HorseFeature V0.25: native spawn target validation FAILED RVA=0x%llX",
            static_cast<unsigned long long>(
                kNativeHorseSpawnRva)
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        FeatureLog(
            "HorseFeature V0.25: MinHook init FAILED status=%d",
            static_cast<int>(initStatus)
        );
        return false;
    }

    NativeHorseSpawnFn original = nullptr;
    const MH_STATUS createStatus = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(
            &HookNativeHorseSpawn),
        reinterpret_cast<LPVOID*>(&original)
    );

    if (createStatus != MH_OK &&
        createStatus != MH_ERROR_ALREADY_CREATED) {
        FeatureLog(
            "HorseFeature V0.25: native spawn hook create FAILED status=%d",
            static_cast<int>(createStatus)
        );
        return false;
    }

    if (createStatus == MH_OK) {
        g_originalNativeHorseSpawn = original;
        g_nativeHorseSpawnTarget = target;
        g_nativeHorseSpawnHookOwned = true;
    }

    const MH_STATUS enableStatus = MH_EnableHook(target);
    if (enableStatus != MH_OK &&
        enableStatus != MH_ERROR_ENABLED) {
        if (g_nativeHorseSpawnHookOwned) {
            MH_RemoveHook(target);
            g_nativeHorseSpawnHookOwned = false;
            g_nativeHorseSpawnTarget = nullptr;
            g_originalNativeHorseSpawn = nullptr;
        }

        FeatureLog(
            "HorseFeature V0.25: native spawn hook enable FAILED status=%d",
            static_cast<int>(enableStatus)
        );
        return false;
    }

    FeatureLog(
        "HorseFeature V0.25: native horse spawn hook READY RVA=0x682680 assignmentRVA=0x6827CD field=player+0xE78"
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
            "HorseFeature V0.25: registry cleared (%s)",
            reason
        );
    }
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;

    FeatureLog(
        "HorseFeature V0.25: two-player native registry armed "
        "War/Strife slots=%zu m_pHorseMount=+0xE78 "
        "movement owner=+0x190 fallback horse movement=+0x390",
        kMaxPlayerSlots
    );

    if (!InstallNativeHorseSpawnHook()) {
        FeatureLog(
            "HorseFeature V0.25: native spawn hook unavailable; "
            "validated player registry fallback remains active"
        );
    }
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintSpeedEnabled.store(settings.sprintSpeedEnabled);
    g_sprintSpeedMultiplier.store(settings.sprintSpeedMultiplier);
    g_sprintDurationEnabled.store(settings.sprintDurationEnabled);
    g_sprintDurationMultiplier.store(
        settings.sprintDurationMultiplier
    );
}

void ObservePlayerCharacter(void* playerCharacter) {
    if (!playerCharacter) {
        return;
    }

    AcquireSRWLockExclusive(&g_lock);
    RegisterPlayerLocked(
        playerCharacter,
        "validated player movement"
    );
    ReleaseSRWLockExclusive(&g_lock);
}

void PollDirectHorse(void* knownLocalPlayer) {
    ObservePlayerCharacter(knownLocalPlayer);
}

void ObserveMovement(
    void* movementComponent,
    void* characterOwner,
    void* knownLocalPlayer,
    float nativeGetMaxSpeed
) {
    if (knownLocalPlayer) {
        ObservePlayerCharacter(knownLocalPlayer);
    }

    if (!movementComponent || !characterOwner) {
        return;
    }

    AcquireSRWLockExclusive(&g_lock);

    HorseSlot* slot = FindSlotByHorseLocked(characterOwner);
    if (slot) {
        if (CaptureMovementLocked(
                *slot,
                movementComponent,
                "GetMaxSpeed owner-match")) {
            slot->nativeGetMaxSpeed = nativeGetMaxSpeed;
            slot->appliedGetMaxSpeed = nativeGetMaxSpeed;
            PublishSlotLocked(*slot);
        }
    }

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
        FindSlotByMovementLocked(movementComponent);

    if (slot) {
        slot->nativeGetMaxSpeed = nativeGetMaxSpeed;
        slot->appliedGetMaxSpeed = nativeGetMaxSpeed;
        PublishSlotLocked(*slot);
    }

    ReleaseSRWLockExclusive(&g_lock);
    return nativeGetMaxSpeed;
}

void Tick() {
    AcquireSRWLockExclusive(&g_lock);

    for (auto& slot : g_slots) {
        if (!slot.player) {
            continue;
        }

        void* currentHorse = nullptr;
        if (ReadAt(
                slot.player,
                kPlayerHorseMountOffset,
                currentHorse)) {
            if (currentHorse &&
                currentHorse != slot.horse &&
                LooksLikeUObject(currentHorse)) {
                RegisterMountLocked(
                    slot.player,
                    currentHorse,
                    "tick player+0xE78"
                );
            } else if (!currentHorse &&
                       slot.horse) {
                FeatureLog(
                    "HorseFeature V0.25: HORSE CLEARED player=%p horse=%p",
                    slot.player,
                    slot.horse
                );
                RestoreSlotLocked(slot);
                slot.horse = nullptr;
                slot.movement = nullptr;
                slot.movementValidated = false;
                slot.speedFieldsReady = false;
                slot.sprintFieldsReady = false;
                slot.staminaReady = false;
            }
        }

        if (slot.horse) {
            if (!slot.movementValidated) {
                void* reflectedMovement = nullptr;
                if (ReadAt(
                        slot.horse,
                        kHorseMovementOffset,
                        reflectedMovement) &&
                    reflectedMovement) {
                    CaptureMovementLocked(
                        slot,
                        reflectedMovement,
                        "tick horse+0x390 owner-back"
                    );
                }
            }

            if (slot.movementValidated) {
                ApplySlotLocked(slot);
            }
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
    t.uniqueCandidatesLogged = g_registeredPlayers.load();
    t.candidateMatches = g_registeredHorses.load();
    return t;
}

bool IsValidatedMovement(void* movementComponent) {
    if (!movementComponent) {
        return false;
    }

    AcquireSRWLockShared(&g_lock);
    const bool found =
        FindSlotByMovementLocked(movementComponent) != nullptr;
    ReleaseSRWLockShared(&g_lock);
    return found;
}

void Shutdown() {
    if (g_nativeHorseSpawnHookOwned &&
        g_nativeHorseSpawnTarget) {
        MH_DisableHook(g_nativeHorseSpawnTarget);
        MH_RemoveHook(g_nativeHorseSpawnTarget);
        g_nativeHorseSpawnHookOwned = false;
        g_nativeHorseSpawnTarget = nullptr;
        g_originalNativeHorseSpawn = nullptr;
    }

    AcquireSRWLockExclusive(&g_lock);
    ClearAllLocked("shutdown");
    ReleaseSRWLockExclusive(&g_lock);
}

} // namespace dg::horse
