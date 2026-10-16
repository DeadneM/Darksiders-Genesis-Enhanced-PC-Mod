#include <windows.h>

#include "HorseFeature.h"
#include "RuntimeSettings.h"

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
// V0.83: native spawn request diagnostics ONLY; never force mount spawn.
// V0.89: verified from exact retail EXE reflected
// AMayhemPlayerCharacter native horse region functions. Both were
// proven invoked 49,923/30,177 times in the isolated V0.87 branch log.
constexpr std::uintptr_t kHorseRegionRva=0x00675E40;
constexpr std::uintptr_t kHorseDesummonRva=0x00675D20;
constexpr std::array<unsigned char,12> kHorseRegionSignature{
    0x40,0x57,0x48,0x83,0xEC,0x40,0x48,0xC7,0x44,0x24,0x20,0xFE
};
constexpr std::uintptr_t kHorseServerSpawnImplRva = 0x00682860;
constexpr std::array<unsigned char, 20> kHorseServerSpawnImplPrologue{
    0x48,0x8B,0xC4,0x55,0x57,0x41,0x54,0x41,0x55,0x41,
    0x56,0x48,0x8B,0xEC,0x48,0x81,0xEC,0x80,0x00,0x00
};

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

// UMayhemHorseCharacterMovementComponent / UCharacterMovementComponent fields.
// These offsets are decoded directly from the retail UE4 property table.
constexpr std::size_t kMaxWalkSpeedOffset = 0x1DC;
constexpr std::size_t kMaxAccelerationOffset = 0x1F0;
constexpr std::size_t kBrakingFrictionFactorOffset = 0x1F8;
constexpr std::size_t kSprintingMaxSpeedOffset = 0x760;

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
    ULONGLONG lastNativeCaptureTick = 0;
};

SRWLOCK g_lock = SRWLOCK_INIT;
std::array<HorseSlot, kMaxHorseSlots> g_slots{};

std::atomic_bool g_speedEnabled{true};
std::atomic<float> g_speedMultiplier{1.25f};
std::atomic_bool g_sprintSpeedEnabled{true};
std::atomic<float> g_sprintSpeedMultiplier{1.25f};
std::atomic_bool g_sprintDurationEnabled{true};
std::atomic<float> g_sprintDurationMultiplier{5.0f};

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
// V0.88: these are observation timestamps/identities, never persistent
// UObject ownership. Previous "validated" meant EVER observed and could
// stay true long after an area had forcibly dismounted the player.
std::atomic_ullong g_lastLiveHorseCallbackTick{0};
std::atomic<void*> g_lastObservedLiveHorse{nullptr};
std::atomic_uint32_t g_observedHorseGenerations{0};
std::atomic_uint32_t g_horsesFound{0};
std::atomic_bool g_summonServerProbeReady{false};
std::atomic_uint32_t g_summonServerCalls{0}, g_summonServerCallsRequested{0};
std::atomic_ullong g_lastServerSpawnTick{0};
std::atomic<void*> g_lastServerSpawnLocalOwner{nullptr};
// V0.91 camera-independent lifetime evidence, recorded only in live native
// callbacks. All pointers below are IDENTITIES ONLY, never dereferenced.
std::atomic<void*> g_horseLeaseOwner{nullptr};
std::atomic<void*> g_horseLeaseHorse{nullptr};
std::atomic_ullong g_horseLeaseStarted{0};
std::atomic_ullong g_observedNewHorseTick{0};
std::atomic_uint32_t g_horseLeaseArms{0};
std::atomic_uint32_t g_nativeCameraRegionHolds{0},g_nativeCameraDesummonHolds{0};
std::atomic_uint32_t g_regionQueries{0},g_regionOverrides{0};
std::atomic_uint32_t g_desummonQueries{0},g_desummonOverrides{0};
std::atomic_uint32_t g_mountedRegionHolds{0},g_mountedDesummonHolds{0};
std::atomic<void*> g_lastLocalPlayerIdentity{nullptr};
std::atomic_ullong g_lastLocalPlayerTick{0};

using HorseFloatFn = float(*)(void*);
using HorseVoidFn = void(*)(void*);
using HorseMovementGetterFn = void*(*)(void*);

HorseFloatFn g_originalGetNormalizedSpeed = nullptr;
HorseFloatFn g_originalGetNormalizedSpeedInput = nullptr;
HorseVoidFn g_originalTryStartSprinting = nullptr;
HorseVoidFn g_originalSetSprintingTrue = nullptr;
using HorseServerSpawnImplFn=void(*)(void*,void*,void*,unsigned char);
HorseServerSpawnImplFn g_originalServerSpawnImpl=nullptr;
using HorseRegionFn=bool(*)(void*);
HorseRegionFn g_originalHorseRegion=nullptr;
HorseRegionFn g_originalHorseDesummon=nullptr;

void* g_getNormalizedSpeedTarget = nullptr;
void* g_getNormalizedSpeedInputTarget = nullptr;
void* g_tryStartSprintingTarget = nullptr;
void* g_setSprintingTrueTarget = nullptr;
void* g_serverSpawnImplTarget = nullptr;
void* g_horseRegionHookTarget=nullptr;
void* g_horseDesummonHookTarget=nullptr;

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

    // HorseCharacter::GetNormalizedSpeed itself calls this exact virtual slot
    // and immediately treats the returned object as its movement component.
    // Do not reject it using an unrelated CharacterOwner offset.
    auto fn = reinterpret_cast<HorseMovementGetterFn>(getter);
    void* movement = fn(horse);

    if (!movement || !Readable(movement, sizeof(void*))) {
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



void ApplySlotLocked(HorseSlot& slot) {
    if (!slot.horse) {
        return;
    }

    const float speedMultiplier =
        Clamp(g_speedMultiplier.load(), 0.0f, 3.0f);
    const float sprintMultiplier =
        Clamp(g_sprintSpeedMultiplier.load(), 0.0f, 3.0f);
    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 20.0f);

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

    if (slot.horseFieldsReady &&
        slot.movementReady) {
        const float targetSprint =
            g_sprintSpeedEnabled.load()
                ? slot.nativeSprintingMaxSpeed * sprintMultiplier
                : slot.nativeSprintingMaxSpeed;

        if (WriteFloat(
                slot.movement,
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

// V0.70 camera-only sample, never writes into horse/game objects.
// Proven ACharacter layout shared by HorseCharacter in this exact retail
// build, checked against native camera distance before camera adoption.
void PublishHorseCameraPoseFromLiveCallback(void* horse) {
    auto& rt=dg::runtime::Get();
    if(!rt.thirdPersonEnabled.load())return;
    void* root=nullptr;
    if(!ReadAt(horse,0x158,root)||!root)return;
    float x=0,y=0,z=0,yaw=0;
    if(!ReadFloat(root,0x1A0,x)||!ReadFloat(root,0x1A4,y)||
       !ReadFloat(root,0x1A8,z)||!ReadFloat(root,0x1F4,yaw)||
       std::fabs(x)>1.0e7f||std::fabs(y)>1.0e7f||
       std::fabs(z)>1.0e7f||std::fabs(yaw)>36000.0f)return;
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG playerTick=rt.tpsActorLocationTick.load();
    const bool footRecent=playerTick&&now>=playerTick&&now-playerTick<15000ull;
    const float dx=x-rt.tpsActorWorldX.load();
    const float dy=y-rt.tpsActorWorldY.load();
    const float dz=z-rt.tpsActorWorldZ.load();
    const bool nearLastPlayer=footRecent&&std::hypot(dx,dy)<2800.0f&&
        std::fabs(dz)<1100.0f;
    const bool sameLiveHorse=
        rt.tpsHorseOwnerIdentity.load()==reinterpret_cast<std::uintptr_t>(horse)&&
        now>=rt.tpsHorsePoseTick.load()&&now-rt.tpsHorsePoseTick.load()<2000ull;
    if(!nearLastPlayer&&!sameLiveHorse)return;
    // Atomic pose is read on the camera thread using the tick as a release
    // marker. Pointer identity is never dereferenced outside this callback.
    rt.tpsHorseWorldX.store(x);
    rt.tpsHorseWorldY.store(y);
    rt.tpsHorseWorldZ.store(z);
    rt.tpsHorseYawDegrees.store(std::remainder(yaw,360.0f));
    rt.tpsHorseOwnerIdentity.store(reinterpret_cast<std::uintptr_t>(horse));
    rt.tpsHorsePoseTick.store(now,std::memory_order_release);
    static std::atomic_uint32_t count{0};
    const unsigned n=count.fetch_add(1)+1;
    if(n==1||n==1000||n==10000)
        FeatureLog("TPS V0.70: LIVE horse camera pose=%u horse=%p xyz=(%.0f,%.0f,%.0f) yaw=%.1f closeFoot=%d",
            n,horse,x,y,z,yaw,nearLastPlayer?1:0);
}

void CaptureHorseLocked(
    void* horse,
    const char* source
) {
    if (!horse || !Readable(horse, sizeof(void*))) {
        return;
    }

    const auto tick=GetTickCount64();
    g_lastLiveHorseCallbackTick.store(tick,std::memory_order_release);
    if(g_lastObservedLiveHorse.exchange(horse,std::memory_order_acq_rel)!=horse){
        g_observedNewHorseTick.store(tick,std::memory_order_release);
        const auto generation=++g_observedHorseGenerations;
        if(generation<=24)
            FeatureLog("Horse Everywhere V0.88 LIVE MOUNT: horse identity changed generation=%u live=%p (callback verified; NOT proof of current rider)",
                generation,horse);
    }
    // V0.91: correlate a newly appearing HorseCharacter with a REAL
    // H/LB+RB summon and an already observed native local server spawn.
    // The callback itself proves this horse is currently alive. No cached
    // UObject access or camera dependency. Never arm on another co-op pawn.
    const auto& leaseRt=dg::runtime::Get();
    const ULONGLONG input=leaseRt.horseSummonIntentTick.load(std::memory_order_acquire);
    const ULONGLONG server=g_lastServerSpawnTick.load(std::memory_order_acquire);
    const ULONGLONG appeared=g_observedNewHorseTick.load(std::memory_order_acquire);
    void* player=g_lastLocalPlayerIdentity.load(std::memory_order_acquire);
    const ULONGLONG lastPlayer=g_lastLocalPlayerTick.load(std::memory_order_acquire);
    if(leaseRt.horseEverywhereEnabled.load(std::memory_order_relaxed)&&
       player&&g_lastServerSpawnLocalOwner.load(std::memory_order_acquire)==player&&
       input&&server&&appeared&&lastPlayer&&
       tick>=input&&tick-input<=1400ull&&
       tick>=server&&tick-server<=1500ull&&
       tick>=appeared&&tick-appeared<=1400ull&&
       tick>=lastPlayer&&tick-lastPlayer<15000ull&&
       g_horseLeaseHorse.load(std::memory_order_acquire)!=horse){
        g_horseLeaseOwner.store(player,std::memory_order_relaxed);
        g_horseLeaseStarted.store(tick,std::memory_order_relaxed);
        g_horseLeaseHorse.store(horse,std::memory_order_release);
        const unsigned n=++g_horseLeaseArms;
        if(n<=10)
            FeatureLog("Horse Everywhere V0.91: new local horse from physical summon + server; camera-independent ride scope armed #%u",n);
    }
    PublishHorseCameraPoseFromLiveCallback(horse);
    HorseSlot* slot = AllocateHorseLocked(horse);
    if (!slot) {
        return;
    }

    // Lifetime safety hotfix:
    // We only dereference horse/movement pointers while a native HorseCharacter
    // method is actively executing with this exact live 'this' pointer.
    // Resolve the movement fresh on every native capture. If UE4 recycled the
    // HorseCharacter address across a level transition, a changed movement
    // pointer marks a new generation and all cached baselines are discarded.
    void* liveMovement = ResolveHorseMovement(horse);

    if (slot->captures > 0 &&
        liveMovement &&
        slot->movement &&
        liveMovement != slot->movement) {
        FeatureLog(
            "HorseFeature V0.31: HORSE GENERATION CHANGED "
            "horse=%p oldMovement=%p newMovement=%p -> reset baselines",
            horse,
            slot->movement,
            liveMovement
        );

        const void* preservedHorse = slot->horse;
        *slot = {};
        slot->horse = const_cast<void*>(preservedHorse);
    }

    const bool firstCapture =
        slot->captures == 0;
    ++slot->captures;
    slot->lastNativeCaptureTick = GetTickCount64();

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
        void* movement = liveMovement;
        if (movement) {
            float walk = 0.0f;
            float acceleration = 0.0f;
            float braking = 0.0f;
            float sprintMax = 0.0f;

            const bool movementFieldsOk =
                ReadFloat(
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
                ReadFloat(
                    movement,
                    kSprintingMaxSpeedOffset,
                    sprintMax);

            if (movementFieldsOk &&
                walk > 0.0f &&
                acceleration > 0.0f &&
                sprintMax >= 0.0f &&
                sprintMax < 10000.0f) {
                slot->movement = movement;
                slot->nativeMaxWalkSpeed = walk;
                slot->nativeMaxAcceleration = acceleration;
                slot->nativeBrakingFrictionFactor = braking;
                slot->nativeSprintingMaxSpeed = sprintMax;
                slot->horseFieldsReady = sprintMax > 0.0f;
                slot->movementReady = true;

                FeatureLog(
                    "HorseFeature V0.31: HORSE MOVEMENT READY "
                    "horse=%p movement=%p walk=%.1f accel=%.1f "
                    "brake=%.2f sprintMax=%.1f sprintFieldReady=%d",
                    horse,
                    movement,
                    walk,
                    acceleration,
                    braking,
                    sprintMax,
                    slot->horseFieldsReady ? 1 : 0
                );
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
            "HorseFeature V0.31: NATIVE HORSE CAPTURE source=%s "
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

// Probe of actual server-side spawn request. Same arguments are forwarded;
// disabled/forbidden mount checks remain entirely owned by the game.
void HookHorseServerSpawnImpl(void* actor,void* a,void* b,unsigned char flag){
    g_lastServerSpawnLocalOwner.store(
        actor&&actor==g_lastLocalPlayerIdentity.load(std::memory_order_acquire)
            ? actor : nullptr,std::memory_order_release);
    g_lastServerSpawnTick.store(GetTickCount64(),std::memory_order_release);
    const unsigned n=++g_summonServerCalls;
    const bool requested=dg::runtime::Get().horseEverywhereEnabled.load(std::memory_order_relaxed);
    if(requested)++g_summonServerCallsRequested;
    if(n==1||n==10||n==100)
        FeatureLog("Horse Everywhere V0.83 diagnostic: SERVER_SpawnHorseMount request n=%u optIn=%d (native unchanged)",n,requested?1:0);
    if(g_originalServerSpawnImpl)g_originalServerSpawnImpl(actor,a,b,flag);
}

// V0.89 carefully scoped experimental native answer patch: ONLY a fresh
// real physical H or LB+RB press and exact known local player identity.
// The original native region getter ALWAYS executes first. Outside this
// 1,600-ms window both native answers remain byte-for-byte identical.
// Never create horse actors, touch UObject memory, or retain horses across
// mission/script-based forced desummon volumes.
bool IsLocalSummonIntent(void* actor){
    if(!actor)return false;
    auto& rt=dg::runtime::Get();
    if(!rt.horseEverywhereEnabled.load(std::memory_order_relaxed))return false;
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG pressed=rt.horseSummonIntentTick.load(std::memory_order_acquire);
    const ULONGLONG localTick=g_lastLocalPlayerTick.load(std::memory_order_acquire);
    return pressed&&localTick&&now>=pressed&&now-pressed<=1600ull&&
        now>=localTick&&now-localTick<15000ull&&
        actor==g_lastLocalPlayerIdentity.load(std::memory_order_acquire);
}
// V0.90: extend the already working 1.6-second summon permission ONLY
// while this exact locally controlled player's LIVE TPS mounted camera and
// native HorseCharacter callbacks confirm the same current horse. Nothing is
// written to game UObjects and there is no respawn loop. Stale/foot/mismatch
// immediately falls back to native answers; Horse Everywhere OFF bypasses both.
bool IsLiveLocalMountedRide(void* actor){
    if(!actor || actor!=g_lastLocalPlayerIdentity.load(std::memory_order_acquire))
        return false;
    const auto& rt=dg::runtime::Get();
    if(!rt.horseEverywhereEnabled.load(std::memory_order_relaxed) ||
       !rt.thirdPersonEnabled.load(std::memory_order_relaxed) ||
       rt.tpsTemporarilySuspended.load(std::memory_order_relaxed) ||
       !rt.tpsMountedViewActive.load(std::memory_order_acquire))
        return false;
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG view=rt.tpsMountedViewTick.load(std::memory_order_acquire);
    const ULONGLONG horsePose=rt.tpsHorsePoseTick.load(std::memory_order_acquire);
    const ULONGLONG nativeHorse=g_lastLiveHorseCallbackTick.load(std::memory_order_acquire);
    const auto selected=rt.tpsHorseOwnerIdentity.load(std::memory_order_acquire);
    const auto observed=reinterpret_cast<std::uintptr_t>(
        g_lastObservedLiveHorse.load(std::memory_order_acquire));
    return view&&horsePose&&nativeHorse&&now>=view&&now-view<=550ull&&
        now>=horsePose&&now-horsePose<=800ull&&
        now>=nativeHorse&&now-nativeHorse<=1000ull&&
        selected&&selected==observed;
}
// Independent of TPS: retain native horse region permission after summon
// while the local player and the SAME newly spawned horse are still alive.
// Returning to validated on-foot movement after mount formation invalidates
// this path, as does any actor identity change or stale horse callback.
bool IsCameraIndependentLocalRide(void* actor){
    if(!actor||!dg::runtime::Get().horseEverywhereEnabled.load(std::memory_order_relaxed))
        return false;
    void* owner=g_horseLeaseOwner.load(std::memory_order_acquire);
    if(!owner||actor!=owner||
       owner!=g_lastLocalPlayerIdentity.load(std::memory_order_acquire))
        return false;
    void* horse=g_horseLeaseHorse.load(std::memory_order_acquire);
    if(!horse||horse!=g_lastObservedLiveHorse.load(std::memory_order_acquire))
        return false;
    const ULONGLONG now=GetTickCount64();
    const ULONGLONG born=g_horseLeaseStarted.load(std::memory_order_acquire);
    const ULONGLONG alive=g_lastLiveHorseCallbackTick.load(std::memory_order_acquire);
    if(!born||!alive||now<born||now<alive||now-alive>350ull)return false;
    // Validated player GetMaxSpeed normally stops while riding. Once it
    // resumes a full second after spawning, presume dismount and fail open.
    const ULONGLONG onFoot=g_lastLocalPlayerTick.load(std::memory_order_acquire);
    if(now-born>1200ull&&onFoot>born+850ull)return false;
    return true;
}
// V0.95: permit entering the game's OWN native mount state while the
// locally controlled PLAYER is live on foot, not only AFTER the keypress.
// A banned-area UMG/Blueprint caller can query region permissions before
// receiving H or the controller chord; delaying permission until H makes
// the first press only clear the lock and forces a second press.
// This never spawns a horse or modifies a quest/scripted desummon function.
bool IsLiveLocalPlayerForHorseRegion(void* actor){
    if(!actor)return false;
    const auto& rt=dg::runtime::Get();
    if(!rt.horseEverywhereEnabled.load(std::memory_order_relaxed))return false;
    if(actor!=g_lastLocalPlayerIdentity.load(std::memory_order_acquire))
        return false;
    const auto at=g_lastLocalPlayerTick.load(std::memory_order_acquire);
    const auto now=GetTickCount64();
    return at&&now>=at&&now-at<=15000ull; // same live-owner age bound as the validated summon scope
}
bool HookHorseRegion(void* actor){
    const bool original=g_originalHorseRegion?g_originalHorseRegion(actor):false;
    const unsigned calls=++g_regionQueries;
    const bool summoned=IsLocalSummonIntent(actor);
    const bool ridden=IsLiveLocalMountedRide(actor);
    const bool nativeRide=IsCameraIndependentLocalRide(actor);
    const bool pregranted=IsLiveLocalPlayerForHorseRegion(actor);
    // V0.97 mode: 0=off, 1=all (EXACT validated V0.95 behavior),
    // 2=restricted only, 3=banned zone only. The engine provides TWO
    // independent predicates, not two documented named zone enums.
    // Experimental classification: a blocked native desummon predicate
    // means "banned"; no desummon predicate means "restricted".
    // Always use the ORIGINAL native function, never our own detour,
    // and only classify verified local actors. A derived native lookup
    // is read-only; fail open when no valid native getter is present.
    const int mode=dg::runtime::Get().horseAreaMode.load(std::memory_order_relaxed);
    const bool scope=summoned||ridden||nativeRide||pregranted;
    const bool selective=mode==2||mode==3;
    const bool banned=selective && scope && g_originalHorseDesummon &&
        g_originalHorseDesummon(actor);
    const bool zoneAllowed=mode==1 || (mode==2 && !banned &&
        g_originalHorseDesummon!=nullptr) || (mode==3 && banned);
    if(scope&&zoneAllowed&&!original){
        if(ridden&&!summoned)++g_mountedRegionHolds;
        if(nativeRide&&!ridden&&!summoned)++g_nativeCameraRegionHolds;
        const unsigned n=++g_regionOverrides;
        if(n<=5)FeatureLog("Horse Everywhere V0.95 REGION: native=false => allowed #%u call=%u input=%d tps=%d nativeRide=%d localPregrant=%d",
            n,calls,summoned?1:0,ridden?1:0,nativeRide?1:0,pregranted?1:0);
        return true;
    }
    if(calls<=5)FeatureLog("Horse Everywhere V0.89 REGION: query #%u native=%d scope=%d",calls,original?1:0,scope?1:0);
    return original;
}
bool HookHorseDesummon(void* actor){
    const bool original=g_originalHorseDesummon?g_originalHorseDesummon(actor):false;
    const unsigned calls=++g_desummonQueries;
    const bool summoned=IsLocalSummonIntent(actor);
    const bool ridden=IsLiveLocalMountedRide(actor);
    const bool nativeRide=IsCameraIndependentLocalRide(actor);
    // Native desummon permission is changed only in All/Banned modes.
    // Restricted-only deliberately leaves the original desummon answer.
    const bool scope=summoned||ridden||nativeRide;
    const int mode=dg::runtime::Get().horseAreaMode.load(std::memory_order_relaxed);
    if((mode==1||mode==3)&&scope&&original){
        if(ridden&&!summoned)++g_mountedDesummonHolds;
        if(nativeRide&&!ridden&&!summoned)++g_nativeCameraDesummonHolds;
        const unsigned n=++g_desummonOverrides;
        if(n<=3)FeatureLog("Horse Everywhere V0.91 DESUMMON: native=true => false #%u call=%u input=%d tps=%d nativeRide=%d",
            n,calls,summoned?1:0,ridden?1:0,nativeRide?1:0);
        return false;
    }
    if(calls<=5)FeatureLog("Horse Everywhere V0.89 DESUMMON: query #%u native=%d scope=%d",calls,original?1:0,scope?1:0);
    return original;
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
            "HorseFeature V0.31: %s target validation FAILED RVA=0x%llX",
            name,
            static_cast<unsigned long long>(rva)
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        FeatureLog(
            "HorseFeature V0.31: MinHook init FAILED for %s status=%d",
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
            "HorseFeature V0.31: %s hook create FAILED status=%d",
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
            "HorseFeature V0.31: %s hook enable FAILED status=%d",
            name,
            static_cast<int>(enableStatus)
        );
        return false;
    }

    original = nativeOriginal;
    targetOut = target;

    FeatureLog(
        "HorseFeature V0.31: %s hook READY RVA=0x%llX",
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
        "HorseFeature V0.31: native HorseCharacter hooks + lifetime-safe horse movement resolver armed; "
        "writes only during live native HorseCharacter callbacks; no per-frame cached-pointer writes."
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

    // V0.89: exact previously discovered native horse region gates.
    // Scoped to 1600ms after physical summon key; log native values.
    InstallNativeHook(kHorseRegionRva,kHorseRegionSignature,
        reinterpret_cast<void*>(&HookHorseRegion),
        g_originalHorseRegion,g_horseRegionHookTarget,
        "PlayerCharacter::IsInHorseRegion (V0.89 scoped test)");
    InstallNativeHook(kHorseDesummonRva,kHorseRegionSignature,
        reinterpret_cast<void*>(&HookHorseDesummon),
        g_originalHorseDesummon,g_horseDesummonHookTarget,
        "PlayerCharacter::IsInHorseDesummonRegion (V0.89 scoped test)");
    // Existing native server callback is always invoked unchanged.
    g_summonServerProbeReady.store(InstallNativeHook(
        kHorseServerSpawnImplRva,kHorseServerSpawnImplPrologue,
        reinterpret_cast<void*>(&HookHorseServerSpawnImpl),
        g_originalServerSpawnImpl,g_serverSpawnImplTarget,
        "PlayerCharacter::SERVER_SpawnHorseMount_Implementation (OBSERVE ONLY)"));

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

void ObservePlayerCharacter(void* player) {
    // Identity only from already-validated player movement hook.
    // Never dereference this cached pointer or modify it.
    if(player){
        g_lastLocalPlayerIdentity.store(player,std::memory_order_release);
        g_lastLocalPlayerTick.store(GetTickCount64(),std::memory_order_release);
    }
}

void PollDirectHorse(void*) {
    // V0.27 deliberately ignores player identity.
}

void ObserveMovement(
    void*,
    void*,
    void*,
    float
) {
    // V0.31 lifetime safety:
    // Never dereference or write a cached horse pointer from the generic
    // movement hook. Only native HorseCharacter callbacks may apply tunings.
}

float AdjustSpeedResult(
    void*,
    float nativeGetMaxSpeed
) {
    // Horse speed fields are already applied from live native HorseCharacter
    // callbacks. Returning the engine result here avoids any stale-pointer
    // write path from the shared movement hook.
    g_nativeGetMaxSpeed.store(nativeGetMaxSpeed);
    g_appliedGetMaxSpeed.store(nativeGetMaxSpeed);
    return nativeGetMaxSpeed;
}

void Tick() {
    // V0.31 lifetime safety:
    // Deliberately no per-frame writes. Horse UObject pointers are weak raw
    // observations and can die during seamless travel / level reload. All
    // tuning writes happen only from native HorseCharacter callbacks.
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.validated = g_validated.load();
    t.staminaReady = g_staminaReady.load();
    t.sprinting = g_sprinting.load();
    t.summonProbeReady = g_summonServerProbeReady.load();
    t.summonServerCalls = g_summonServerCalls.load();
    t.summonServerCallsWhileRequested = g_summonServerCallsRequested.load();
    t.lastServerSpawnTick = g_lastServerSpawnTick.load();
    t.regionQueries=g_regionQueries.load();
    t.regionOverrides=g_regionOverrides.load();
    t.desummonQueries=g_desummonQueries.load();
    t.desummonOverrides=g_desummonOverrides.load();
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
    t.lastHorseNativeTick = g_lastLiveHorseCallbackTick.load(std::memory_order_acquire);
    t.horseGeneration = g_observedHorseGenerations.load(std::memory_order_relaxed);
    const auto now=GetTickCount64();
    t.horseCallbackFresh = t.lastHorseNativeTick && now>=t.lastHorseNativeTick &&
        now-t.lastHorseNativeTick<1100ull;
    return t;
}

bool IsValidatedMovement(void* movementComponent) {
    if (!movementComponent) {
        return false;
    }

    const ULONGLONG now = GetTickCount64();
    bool found = false;

    AcquireSRWLockShared(&g_lock);
    HorseSlot* slot =
        FindMovementLocked(movementComponent);

    if (slot &&
        slot->lastNativeCaptureTick != 0 &&
        now >= slot->lastNativeCaptureTick &&
        now - slot->lastNativeCaptureTick <= 2000) {
        found = true;
    }
    ReleaseSRWLockShared(&g_lock);

    return found;
}

void Shutdown() {
    RemoveHook(g_getNormalizedSpeedTarget);
    RemoveHook(g_getNormalizedSpeedInputTarget);
    RemoveHook(g_tryStartSprintingTarget);
    RemoveHook(g_setSprintingTrueTarget);
    RemoveHook(g_serverSpawnImplTarget);
    RemoveHook(g_horseRegionHookTarget);
    RemoveHook(g_horseDesummonHookTarget);
    FeatureLog("Horse Everywhere V0.91 region totals: allowedChecks=%u overrides=%u tpsHolds=%u nativeCameraHolds=%u desummonChecks=%u overrides=%u tpsHolds=%u nativeCameraHolds=%u leases=%u",
        g_regionQueries.load(),g_regionOverrides.load(),g_mountedRegionHolds.load(),g_nativeCameraRegionHolds.load(),
        g_desummonQueries.load(),g_desummonOverrides.load(),g_mountedDesummonHolds.load(),g_nativeCameraDesummonHolds.load(),
        g_horseLeaseArms.load());
    FeatureLog("Horse Everywhere V0.83 probe totals: serverSpawn=%u duringRequested=%u (server args unchanged)",
        g_summonServerCalls.load(),g_summonServerCallsRequested.load());

    AcquireSRWLockExclusive(&g_lock);
    for (auto& slot : g_slots) {
        // Do not restore through cached raw UObject pointers here. They may
        // already have been destroyed by UE4 during world teardown.
        slot = {};
    }
    ReleaseSRWLockExclusive(&g_lock);

    FeatureLog(
        "HorseFeature V0.31: native horse hooks shutdown; no stale-pointer restore"
    );
}

} // namespace dg::horse
