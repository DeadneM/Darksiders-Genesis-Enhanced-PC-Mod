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

// Native HorseCharacter functions proved from the retail executable's own
// reflection registration table and disassembly.
constexpr std::uintptr_t kHorseGetNormalizedSpeedRva = 0x00670D00;
constexpr std::uintptr_t kHorseGetNormalizedSpeedInputRva = 0x00670DD0;
constexpr std::uintptr_t kHorseTryStartSprintingRva = 0x0068C120;
constexpr std::uintptr_t kHorseSetSprintingTrueRva = 0x00791F80;

constexpr std::array<unsigned char, 12> kGetNormalizedSpeedPrologue{
    0x48, 0x89, 0x5C, 0x24, 0x08, 0x57,
    0x48, 0x83, 0xEC, 0x50, 0x48, 0x8B
};
constexpr std::array<unsigned char, 12> kGetNormalizedSpeedInputPrologue{
    0x48, 0x8B, 0xC4, 0x48, 0x89, 0x58,
    0x10, 0x55, 0x48, 0x8D, 0x68, 0xA1
};
constexpr std::array<unsigned char, 12> kTryStartSprintingPrologue{
    0x80, 0xB9, 0xCC, 0x09, 0x00, 0x00,
    0x00, 0x75, 0x10, 0x0F, 0x57, 0xC0
};
constexpr std::array<unsigned char, 8> kSetSprintingTruePrologue{
    0xC6, 0x81, 0xD0, 0x08, 0x00, 0x00, 0x01, 0xC3
};

// HorseCharacter fields proved by native code / property descriptors.
constexpr std::size_t kSprintingMaxSpeedOffset = 0x760;
constexpr std::size_t kSprintingOffset = 0x8D0;
constexpr std::size_t kStaminaRecoveryOffset = 0x90C;
constexpr std::size_t kStaminaTotalRecoveryOffset = 0x910;
constexpr std::size_t kStaminaRecoveryCooldownOffset = 0x914;
constexpr std::size_t kStaminaSprintRateOffset = 0x918;
constexpr std::size_t kCurrentStaminaOffset = 0x9C0;
constexpr std::size_t kMaxStaminaOffset = 0x9C4;
constexpr std::size_t kRanOutOfStaminaOffset = 0x9CC;

// The native HorseCharacter::GetNormalizedSpeed implementation calls the
// horse's virtual getter at vtable +0x5F8 and then GetMaxSpeed on the returned
// movement component at vtable +0x3D0.
constexpr std::size_t kHorseMovementGetterVtableOffset = 0x5F8;

// UCharacterMovementComponent fields, already reflection-proved.
constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1E0;
constexpr std::size_t kMaxAccelerationOffset = 0x1F4;
constexpr std::size_t kBrakingFrictionFactorOffset = 0x1FC;

constexpr std::size_t kMaxHorseSlots = 4;

struct HorseSlot {
    void* horse = nullptr;
    void* movement = nullptr;

    bool movementReady = false;
    bool horseFieldsReady = false;
    bool staminaFieldsReady = false;

    float nativeMaxWalkSpeed = 0.0f;
    float nativeMaxAcceleration = 0.0f;
    float nativeBrakingFrictionFactor = 0.0f;
    float nativeSprintingMaxSpeed = 0.0f;
    float nativeStaminaSprintRate = 0.0f;

    float currentStamina = 0.0f;
    float maxStamina = 0.0f;

    std::uint32_t captures = 0;
};

SRWLOCK g_lock = SRWLOCK_INIT;
std::array<HorseSlot, kMaxHorseSlots> g_slots{};

std::atomic_bool g_speedEnabled{true};
std::atomic<float> g_speedMultiplier{1.25f};
std::atomic_bool g_sprintSpeedEnabled{true};
std::atomic<float> g_sprintSpeedMultiplier{1.25f};
std::atomic_bool g_sprintDurationEnabled{true};
std::atomic<float> g_sprintDurationMultiplier{2.0f};

std::atomic_bool g_validated{false};
std::atomic_bool g_staminaReady{false};
std::atomic_bool g_sprinting{false};
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

std::atomic_uint32_t g_nativeHookCalls{0};
std::atomic_uint32_t g_horsesFound{0};

using HorseFloatFn = float(*)(void*);
using HorseVoidFn = void(*)(void*);
using HorseMovementGetterFn = void*(*)(void*);

HorseFloatFn g_originalGetNormalizedSpeed = nullptr;
HorseFloatFn g_originalGetNormalizedSpeedInput = nullptr;
HorseVoidFn g_originalTryStartSprinting = nullptr;
HorseVoidFn g_originalSetSprintingTrue = nullptr;

void* g_getNormalizedSpeedTarget = nullptr;
void* g_getNormalizedSpeedInputTarget = nullptr;
void* g_tryStartSprintingTarget = nullptr;
void* g_setSprintingTrueTarget = nullptr;

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
        if (slot.movementReady &&
            slot.movement == movement &&
            movement) {
            return &slot;
        }
    }
    return nullptr;
}

HorseSlot* AllocateHorseLocked(void* horse) {
    if (auto* existing = FindHorseLocked(horse)) {
        return existing;
    }

    for (auto& slot : g_slots) {
        if (!slot.horse) {
            slot.horse = horse;
            g_horsesFound.fetch_add(1);
            return &slot;
        }
    }

    return nullptr;
}

void* ResolveHorseMovement(void* horse) {
    if (!horse || !Readable(horse, sizeof(void*))) {
        return nullptr;
    }

    void** vtable = *reinterpret_cast<void***>(horse);
    if (!vtable) {
        return nullptr;
    }

    const std::size_t slot =
        kHorseMovementGetterVtableOffset / sizeof(void*);

    if (!Readable(vtable + slot, sizeof(void*))) {
        return nullptr;
    }

    void* getter = vtable[slot];
    if (!ExecutableAddress(getter)) {
        return nullptr;
    }

    auto fn = reinterpret_cast<HorseMovementGetterFn>(getter);
    void* movement = fn(horse);
    if (!movement || !Readable(movement, sizeof(void*))) {
        return nullptr;
    }

    void* ownerBack = nullptr;
    if (!ReadAt(
            movement,
            kCharacterOwnerOffset,
            ownerBack) ||
        ownerBack != horse) {
        return nullptr;
    }

    return movement;
}

void PublishSlotLocked(const HorseSlot& slot) {
    g_validated.store(slot.horse != nullptr);
    g_staminaReady.store(slot.staminaFieldsReady);
    g_owner.store(slot.horse);
    g_movement.store(slot.movement);

    g_nativeMaxWalkSpeed.store(slot.nativeMaxWalkSpeed);
    g_nativeMaxAcceleration.store(slot.nativeMaxAcceleration);
    g_nativeSprintingMaxSpeed.store(slot.nativeSprintingMaxSpeed);
    g_nativeSprintDrain.store(slot.nativeStaminaSprintRate);

    unsigned char sprinting = 0;
    if (ReadAt(
            slot.horse,
            kSprintingOffset,
            sprinting)) {
        g_sprinting.store(sprinting != 0);
    }
}

void RestoreSlotLocked(HorseSlot& slot) {
    if (!slot.horse) {
        return;
    }

    if (slot.movementReady) {
        WriteFloat(
            slot.movement,
            kMaxWalkSpeedOffset,
            slot.nativeMaxWalkSpeed
        );
        WriteFloat(
            slot.movement,
            kMaxAccelerationOffset,
            slot.nativeMaxAcceleration
        );
        WriteFloat(
            slot.movement,
            kBrakingFrictionFactorOffset,
            slot.nativeBrakingFrictionFactor
        );
    }

    if (slot.horseFieldsReady) {
        WriteFloat(
            slot.horse,
            kSprintingMaxSpeedOffset,
            slot.nativeSprintingMaxSpeed
        );
    }

    if (slot.staminaFieldsReady) {
        WriteFloat(
            slot.horse,
            kStaminaSprintRateOffset,
            slot.nativeStaminaSprintRate
        );
    }
}

void ApplySlotLocked(HorseSlot& slot) {
    if (!slot.horse) {
        return;
    }

    const float speedMultiplier =
        Clamp(g_speedMultiplier.load(), 0.0f, 3.0f);
    const float sprintMultiplier =
        Clamp(g_sprintSpeedMultiplier.load(), 0.0f, 3.0f);
    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 10.0f);

    if (slot.movementReady) {
        const float targetWalk =
            g_speedEnabled.load()
                ? slot.nativeMaxWalkSpeed * speedMultiplier
                : slot.nativeMaxWalkSpeed;

        const float targetAcceleration =
            g_speedEnabled.load()
                ? slot.nativeMaxAcceleration * speedMultiplier
                : slot.nativeMaxAcceleration;

        const float targetBrake =
            g_speedEnabled.load()
                ? std::max(slot.nativeBrakingFrictionFactor, 2.0f)
                : slot.nativeBrakingFrictionFactor;

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

        WriteFloat(
            slot.movement,
            kBrakingFrictionFactorOffset,
            targetBrake
        );
    }

    if (slot.horseFieldsReady) {
        const float targetSprint =
            g_sprintSpeedEnabled.load()
                ? slot.nativeSprintingMaxSpeed * sprintMultiplier
                : slot.nativeSprintingMaxSpeed;

        if (WriteFloat(
                slot.horse,
                kSprintingMaxSpeedOffset,
                targetSprint)) {
            g_appliedSprintingMaxSpeed.store(targetSprint);
        }
    }

    if (slot.staminaFieldsReady) {
        float targetSprintRate =
            slot.nativeStaminaSprintRate;

        if (g_sprintDurationEnabled.load()) {
            targetSprintRate =
                durationMultiplier <= 0.0001f
                    ? slot.nativeStaminaSprintRate
                    : slot.nativeStaminaSprintRate / durationMultiplier;
        }

        if (WriteFloat(
                slot.horse,
                kStaminaSprintRateOffset,
                targetSprintRate)) {
            g_appliedSprintDrain.store(targetSprintRate);
        }
    }

    ReadFloat(
        slot.horse,
        kCurrentStaminaOffset,
        slot.currentStamina
    );
    ReadFloat(
        slot.horse,
        kMaxStaminaOffset,
        slot.maxStamina
    );

    PublishSlotLocked(slot);
}

void CaptureHorseLocked(
    void* horse,
    const char* source
) {
    if (!horse || !Readable(horse, sizeof(void*))) {
        return;
    }

    HorseSlot* slot = AllocateHorseLocked(horse);
    if (!slot) {
        return;
    }

    const bool firstCapture =
        slot->captures == 0;
    ++slot->captures;

    if (!slot->horseFieldsReady) {
        float sprintMax = 0.0f;
        if (ReadFloat(
                horse,
                kSprintingMaxSpeedOffset,
                sprintMax) &&
            sprintMax > 0.0f &&
            sprintMax < 10000.0f) {
            slot->nativeSprintingMaxSpeed = sprintMax;
            slot->horseFieldsReady = true;
        }
    }

    if (!slot->staminaFieldsReady) {
        float recovery = 0.0f;
        float totalRecovery = 0.0f;
        float cooldown = 0.0f;
        float sprintRate = 0.0f;

        const bool staminaOk =
            ReadFloat(
                horse,
                kStaminaRecoveryOffset,
                recovery) &&
            ReadFloat(
                horse,
                kStaminaTotalRecoveryOffset,
                totalRecovery) &&
            ReadFloat(
                horse,
                kStaminaRecoveryCooldownOffset,
                cooldown) &&
            ReadFloat(
                horse,
                kStaminaSprintRateOffset,
                sprintRate) &&
            sprintRate >= 0.0f &&
            sprintRate < 1000.0f;

        if (staminaOk) {
            slot->nativeStaminaSprintRate =
                sprintRate;
            slot->staminaFieldsReady = true;
        }
    }

    if (!slot->movementReady) {
        void* movement = ResolveHorseMovement(horse);
        if (movement) {
            float walk = 0.0f;
            float acceleration = 0.0f;
            float braking = 0.0f;

            if (ReadFloat(
                    movement,
                    kMaxWalkSpeedOffset,
                    walk) &&
                ReadFloat(
                    movement,
                    kMaxAccelerationOffset,
                    acceleration) &&
                ReadFloat(
                    movement,
                    kBrakingFrictionFactorOffset,
                    braking) &&
                walk > 0.0f &&
                acceleration > 0.0f) {
                slot->movement = movement;
                slot->nativeMaxWalkSpeed = walk;
                slot->nativeMaxAcceleration = acceleration;
                slot->nativeBrakingFrictionFactor = braking;
                slot->movementReady = true;
            }
        }
    }

    float currentStamina = 0.0f;
    float maxStamina = 0.0f;
    unsigned char sprinting = 0;
    unsigned char ranOut = 0;
    ReadFloat(
        horse,
        kCurrentStaminaOffset,
        currentStamina
    );
    ReadFloat(
        horse,
        kMaxStaminaOffset,
        maxStamina
    );
    ReadAt(
        horse,
        kSprintingOffset,
        sprinting
    );
    ReadAt(
        horse,
        kRanOutOfStaminaOffset,
        ranOut
    );

    slot->currentStamina = currentStamina;
    slot->maxStamina = maxStamina;

    if (firstCapture) {
        float recovery = 0.0f;
        float totalRecovery = 0.0f;
        float cooldown = 0.0f;
        float sprintRate = 0.0f;
        ReadFloat(
            horse,
            kStaminaRecoveryOffset,
            recovery
        );
        ReadFloat(
            horse,
            kStaminaTotalRecoveryOffset,
            totalRecovery
        );
        ReadFloat(
            horse,
            kStaminaRecoveryCooldownOffset,
            cooldown
        );
        ReadFloat(
            horse,
            kStaminaSprintRateOffset,
            sprintRate
        );

        FeatureLog(
            "HorseFeature V0.27: NATIVE HORSE CAPTURE source=%s "
            "horse=%p movement=%p movementReady=%d "
            "walk=%.1f accel=%.1f brake=%.2f "
            "sprintMax=%.1f bSprinting=%u "
            "staminaRate=[recovery %.1f total %.1f cooldown %.1f sprint %.1f] "
            "staminaNow=%.1f/%.1f ranOut=%u",
            source ? source : "unknown",
            horse,
            slot->movement,
            slot->movementReady ? 1 : 0,
            slot->nativeMaxWalkSpeed,
            slot->nativeMaxAcceleration,
            slot->nativeBrakingFrictionFactor,
            slot->nativeSprintingMaxSpeed,
            static_cast<unsigned>(sprinting),
            recovery,
            totalRecovery,
            cooldown,
            sprintRate,
            currentStamina,
            maxStamina,
            static_cast<unsigned>(ranOut)
        );
    }

    ApplySlotLocked(*slot);
}

float HookHorseGetNormalizedSpeed(
    void* horse
) {
    g_nativeHookCalls.fetch_add(1);

    AcquireSRWLockExclusive(&g_lock);
    CaptureHorseLocked(
        horse,
        "HorseCharacter::GetNormalizedSpeed"
    );
    ReleaseSRWLockExclusive(&g_lock);

    return g_originalGetNormalizedSpeed
        ? g_originalGetNormalizedSpeed(horse)
        : 0.0f;
}

float HookHorseGetNormalizedSpeedInput(
    void* horse
) {
    g_nativeHookCalls.fetch_add(1);

    AcquireSRWLockExclusive(&g_lock);
    CaptureHorseLocked(
        horse,
        "HorseCharacter::GetNormalizedSpeedInput"
    );
    ReleaseSRWLockExclusive(&g_lock);

    return g_originalGetNormalizedSpeedInput
        ? g_originalGetNormalizedSpeedInput(horse)
        : 0.0f;
}

void HookHorseTryStartSprinting(
    void* horse
) {
    g_nativeHookCalls.fetch_add(1);

    AcquireSRWLockExclusive(&g_lock);
    CaptureHorseLocked(
        horse,
        "HorseCharacter::TryStartSprinting"
    );
    ReleaseSRWLockExclusive(&g_lock);

    if (g_originalTryStartSprinting) {
        g_originalTryStartSprinting(horse);
    }
}

void HookHorseSetSprintingTrue(
    void* horse
) {
    g_nativeHookCalls.fetch_add(1);

    AcquireSRWLockExclusive(&g_lock);
    CaptureHorseLocked(
        horse,
        "HorseCharacter::SetSprintingTrue"
    );
    ReleaseSRWLockExclusive(&g_lock);

    if (g_originalSetSprintingTrue) {
        g_originalSetSprintingTrue(horse);
    }
}

template <std::size_t N, typename Fn>
bool InstallNativeHook(
    std::uintptr_t rva,
    const std::array<unsigned char, N>& prologue,
    void* detour,
    Fn& original,
    void*& targetOut,
    const char* name
) {
    HMODULE module = GetModuleHandleW(nullptr);
    if (!module) {
        return false;
    }

    auto* target =
        reinterpret_cast<unsigned char*>(module) +
        rva;

    if (!Readable(target, prologue.size()) ||
        std::memcmp(
            target,
            prologue.data(),
            prologue.size()) != 0) {
        FeatureLog(
            "HorseFeature V0.27: %s target validation FAILED RVA=0x%llX",
            name,
            static_cast<unsigned long long>(rva)
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        FeatureLog(
            "HorseFeature V0.27: MinHook init FAILED for %s status=%d",
            name,
            static_cast<int>(initStatus)
        );
        return false;
    }

    Fn nativeOriginal = nullptr;
    const MH_STATUS createStatus = MH_CreateHook(
        target,
        detour,
        reinterpret_cast<LPVOID*>(&nativeOriginal)
    );

    if (createStatus != MH_OK) {
        FeatureLog(
            "HorseFeature V0.27: %s hook create FAILED status=%d",
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
            "HorseFeature V0.27: %s hook enable FAILED status=%d",
            name,
            static_cast<int>(enableStatus)
        );
        return false;
    }

    original = nativeOriginal;
    targetOut = target;

    FeatureLog(
        "HorseFeature V0.27: %s hook READY RVA=0x%llX",
        name,
        static_cast<unsigned long long>(rva)
    );

    return true;
}

void RemoveHook(void*& target) {
    if (!target) {
        return;
    }

    MH_DisableHook(target);
    MH_RemoveHook(target);
    target = nullptr;
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;

    FeatureLog(
        "HorseFeature V0.27: native HorseCharacter hooks armed; "
        "no player identity, no Blueprint accessors, no generic movement scan."
    );

    InstallNativeHook(
        kHorseGetNormalizedSpeedRva,
        kGetNormalizedSpeedPrologue,
        reinterpret_cast<void*>(
            &HookHorseGetNormalizedSpeed),
        g_originalGetNormalizedSpeed,
        g_getNormalizedSpeedTarget,
        "HorseCharacter::GetNormalizedSpeed"
    );

    InstallNativeHook(
        kHorseGetNormalizedSpeedInputRva,
        kGetNormalizedSpeedInputPrologue,
        reinterpret_cast<void*>(
            &HookHorseGetNormalizedSpeedInput),
        g_originalGetNormalizedSpeedInput,
        g_getNormalizedSpeedInputTarget,
        "HorseCharacter::GetNormalizedSpeedInput"
    );

    InstallNativeHook(
        kHorseTryStartSprintingRva,
        kTryStartSprintingPrologue,
        reinterpret_cast<void*>(
            &HookHorseTryStartSprinting),
        g_originalTryStartSprinting,
        g_tryStartSprintingTarget,
        "HorseCharacter::TryStartSprinting"
    );

    InstallNativeHook(
        kHorseSetSprintingTrueRva,
        kSetSprintingTruePrologue,
        reinterpret_cast<void*>(
            &HookHorseSetSprintingTrue),
        g_originalSetSprintingTrue,
        g_setSprintingTrueTarget,
        "HorseCharacter::SetSprintingTrue"
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
    // V0.27 deliberately ignores player identity.
}

void PollDirectHorse(void*) {
    // V0.27 deliberately ignores player identity.
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

    HorseSlot* slot =
        FindMovementLocked(movementComponent);

    if (slot &&
        slot->horse == characterOwner) {
        g_nativeGetMaxSpeed.store(nativeGetMaxSpeed);
        g_appliedGetMaxSpeed.store(nativeGetMaxSpeed);
        ApplySlotLocked(*slot);
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
        FindMovementLocked(movementComponent);

    if (slot) {
        g_nativeGetMaxSpeed.store(nativeGetMaxSpeed);
        g_appliedGetMaxSpeed.store(nativeGetMaxSpeed);
        ApplySlotLocked(*slot);
    }

    ReleaseSRWLockExclusive(&g_lock);
    return nativeGetMaxSpeed;
}

void Tick() {
    AcquireSRWLockExclusive(&g_lock);

    for (auto& slot : g_slots) {
        if (slot.horse) {
            if (!slot.movementReady) {
                void* movement =
                    ResolveHorseMovement(slot.horse);

                if (movement) {
                    float walk = 0.0f;
                    float acceleration = 0.0f;
                    float braking = 0.0f;

                    if (ReadFloat(
                            movement,
                            kMaxWalkSpeedOffset,
                            walk) &&
                        ReadFloat(
                            movement,
                            kMaxAccelerationOffset,
                            acceleration) &&
                        ReadFloat(
                            movement,
                            kBrakingFrictionFactorOffset,
                            braking) &&
                        walk > 0.0f &&
                        acceleration > 0.0f) {
                        slot.movement = movement;
                        slot.nativeMaxWalkSpeed = walk;
                        slot.nativeMaxAcceleration = acceleration;
                        slot.nativeBrakingFrictionFactor = braking;
                        slot.movementReady = true;

                        FeatureLog(
                            "HorseFeature V0.27: movement resolved later horse=%p movement=%p walk=%.1f accel=%.1f brake=%.2f",
                            slot.horse,
                            movement,
                            walk,
                            acceleration,
                            braking
                        );
                    }
                }
            }

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
    t.playerOwner = nullptr;
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
    t.uniqueCandidatesLogged = g_nativeHookCalls.load();
    t.candidateMatches = g_horsesFound.load();
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
    RemoveHook(g_getNormalizedSpeedTarget);
    RemoveHook(g_getNormalizedSpeedInputTarget);
    RemoveHook(g_tryStartSprintingTarget);
    RemoveHook(g_setSprintingTrueTarget);

    AcquireSRWLockExclusive(&g_lock);
    for (auto& slot : g_slots) {
        RestoreSlotLocked(slot);
        slot = {};
    }
    ReleaseSRWLockExclusive(&g_lock);

    FeatureLog(
        "HorseFeature V0.27: native horse hooks shutdown"
    );
}

} // namespace dg::horse
