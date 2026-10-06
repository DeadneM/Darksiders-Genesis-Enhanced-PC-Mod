#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <MinHook.h>

#include "HorseFeature.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdarg>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace dg::horse {
namespace {

using GetMaxSpeedFn = float(*)(void*);

constexpr std::size_t kCharacterOwnerOffset = 0x190;
constexpr std::size_t kMovementModeOffset = 0x1B0;
constexpr std::size_t kMaxWalkSpeedOffset = 0x1D4;
constexpr std::size_t kMaxAccelerationOffset = 0x1E8;

constexpr std::size_t kStaminaRecoveryOffset = 0x910;
constexpr std::size_t kStaminaTotalRecoveryOffset = 0x914;
constexpr std::size_t kStaminaSprintDrainOffset = 0x918;

constexpr float kReferenceMaxWalkSpeed = 1300.0f;
constexpr float kReferenceMaxAcceleration = 600.0f;
constexpr float kReferenceRecovery = 15.0f;
constexpr float kReferenceTotalRecovery = 40.0f;
constexpr float kReferenceSprintDrain = 25.0f;

constexpr ULONGLONG kHorseLostTimeoutMs = 1500;
constexpr std::size_t kGetMaxSpeedVtableOffset = 0x3D0;
constexpr std::size_t kGetMaxSpeedVtableSlot =
    kGetMaxSpeedVtableOffset / sizeof(void*);
constexpr std::size_t kCompareSlotBegin = 72;
constexpr std::size_t kCompareSlotEnd = 144;
constexpr int kMinimumVtableScore = 44;
constexpr int kMinimumScoreLead = 3;

struct PeSection {
    unsigned char* begin = nullptr;
    std::size_t size = 0;
};

struct ParentCandidate {
    void** vtable = nullptr;
    void* target = nullptr;
    int score = 0;
    int textPointers = 0;
};

struct CapturedHorse {
    void* owner = nullptr;
    void* movement = nullptr;
    float maxWalkSpeed = 0.0f;
    float maxAcceleration = 0.0f;
    float sprintDrain = 0.0f;
    bool captured = false;
};

SRWLOCK g_lock = SRWLOCK_INIT;
CapturedHorse g_horse{};

GetMaxSpeedFn g_originalBaseGetMaxSpeed = nullptr;
void* g_baseGetMaxSpeedTarget = nullptr;
LogFn g_logger = nullptr;

std::atomic_bool g_resolverAttempted{false};
std::atomic_bool g_baseHookReady{false};
std::atomic_bool g_validated{false};
std::atomic_bool g_staminaReady{false};
std::atomic<void*> g_owner{nullptr};
std::atomic<void*> g_movement{nullptr};
std::atomic<void*> g_baseTargetTelemetry{nullptr};
std::atomic_int g_bestVtableScore{0};
std::atomic_int g_secondVtableScore{0};

std::atomic_bool g_speedEnabled{true};
std::atomic<float> g_speedMultiplier{1.25f};
std::atomic_bool g_sprintDurationEnabled{true};
std::atomic<float> g_sprintDurationMultiplier{2.0f};

std::atomic<float> g_nativeMaxWalkSpeed{0.0f};
std::atomic<float> g_appliedMaxWalkSpeed{0.0f};
std::atomic<float> g_nativeMaxAcceleration{0.0f};
std::atomic<float> g_appliedMaxAcceleration{0.0f};
std::atomic<float> g_nativeSprintDrain{0.0f};
std::atomic<float> g_appliedSprintDrain{0.0f};

std::atomic_uint32_t g_candidateChecks{0};
std::atomic_uint32_t g_candidateMatches{0};
std::atomic_ullong g_lastHorseSeenTick{0};

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

bool GetMainModuleSection(const char* name, PeSection& out) {
    out = {};

    auto* base = reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr));
    if (!base) {
        return false;
    }

    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        return false;
    }

    auto* section = IMAGE_FIRST_SECTION(nt);
    for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        char sectionName[9]{};
        memcpy(sectionName, section[i].Name, 8);
        if (strcmp(sectionName, name) == 0) {
            out.begin = base + section[i].VirtualAddress;
            out.size = section[i].Misc.VirtualSize;
            return true;
        }
    }

    return false;
}

bool AddressInSection(const PeSection& section, const void* address) {
    const auto* p = reinterpret_cast<const unsigned char*>(address);
    return p >= section.begin && p < section.begin + section.size;
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

int CompareVtables(
    void** playerVtable,
    void** candidateVtable,
    const PeSection& text,
    int& outTextPointers
) {
    int score = 0;
    int textPointers = 0;

    for (std::size_t slot = kCompareSlotBegin;
         slot <= kCompareSlotEnd;
         ++slot) {
        void* playerEntry = playerVtable[slot];
        void* candidateEntry = candidateVtable[slot];

        if (AddressInSection(text, candidateEntry)) {
            ++textPointers;
        }

        if (slot == kGetMaxSpeedVtableSlot) {
            continue;
        }

        if (playerEntry == candidateEntry &&
            AddressInSection(text, playerEntry)) {
            ++score;
        }
    }

    outTextPointers = textPointers;
    return score;
}

bool ResolveParentGetMaxSpeed(
    void* movementComponent,
    void* playerGetMaxSpeedTarget,
    void** outTarget
) {
    if (!movementComponent || !playerGetMaxSpeedTarget || !outTarget) {
        return false;
    }

    PeSection text{};
    PeSection rdata{};
    if (!GetMainModuleSection(".text", text) ||
        !GetMainModuleSection(".rdata", rdata)) {
        FeatureLog("HorseFeature: failed to enumerate .text/.rdata");
        return false;
    }

    void** playerVtable = nullptr;
    if (!ReadAt(movementComponent, 0, playerVtable) ||
        !playerVtable ||
        !Readable(
            playerVtable + kCompareSlotBegin,
            (kCompareSlotEnd - kCompareSlotBegin + 1) * sizeof(void*))) {
        FeatureLog("HorseFeature: player movement vtable unreadable");
        return false;
    }

    if (playerVtable[kGetMaxSpeedVtableSlot] != playerGetMaxSpeedTarget) {
        FeatureLog(
            "HorseFeature: player vtable slot mismatch slot=%p resolver=%p",
            playerVtable[kGetMaxSpeedVtableSlot],
            playerGetMaxSpeedTarget
        );
        return false;
    }

    std::array<ParentCandidate, 3> top{};

    const std::size_t requiredBytes =
        (kCompareSlotEnd + 1) * sizeof(void*);

    for (std::size_t offset = 0;
         offset + requiredBytes <= rdata.size;
         offset += sizeof(void*)) {
        auto** candidate =
            reinterpret_cast<void**>(rdata.begin + offset);

        if (candidate == playerVtable) {
            continue;
        }

        void* target = candidate[kGetMaxSpeedVtableSlot];
        if (!target ||
            target == playerGetMaxSpeedTarget ||
            !AddressInSection(text, target)) {
            continue;
        }

        int textPointers = 0;
        const int score =
            CompareVtables(playerVtable, candidate, text, textPointers);

        if (textPointers < 45 || score < 20) {
            continue;
        }

        ParentCandidate current{};
        current.vtable = candidate;
        current.target = target;
        current.score = score;
        current.textPointers = textPointers;

        for (std::size_t i = 0; i < top.size(); ++i) {
            if (current.score <= top[i].score) {
                continue;
            }

            for (std::size_t j = top.size() - 1; j > i; --j) {
                top[j] = top[j - 1];
            }
            top[i] = current;
            break;
        }
    }

    const int best = top[0].score;
    const int second = top[1].score;
    g_bestVtableScore.store(best);
    g_secondVtableScore.store(second);

    auto* module =
        reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr));

    FeatureLog(
        "HorseFeature: vtable ancestry top scores=%d/%d/%d "
        "targets=[0x%zX,0x%zX,0x%zX]",
        top[0].score,
        top[1].score,
        top[2].score,
        top[0].target
            ? static_cast<std::size_t>(
                  reinterpret_cast<unsigned char*>(top[0].target) - module)
            : 0,
        top[1].target
            ? static_cast<std::size_t>(
                  reinterpret_cast<unsigned char*>(top[1].target) - module)
            : 0,
        top[2].target
            ? static_cast<std::size_t>(
                  reinterpret_cast<unsigned char*>(top[2].target) - module)
            : 0
    );

    if (!top[0].target ||
        best < kMinimumVtableScore ||
        (second > 0 && best - second < kMinimumScoreLead)) {
        FeatureLog(
            "HorseFeature: vtable ancestry not unique enough; fail-open"
        );
        return false;
    }

    *outTarget = top[0].target;
    return true;
}

bool ReadHorseStamina(
    void* owner,
    float& recovery,
    float& totalRecovery,
    float& sprintDrain
) {
    if (!ReadFloat(owner, kStaminaRecoveryOffset, recovery) ||
        !ReadFloat(owner, kStaminaTotalRecoveryOffset, totalRecovery) ||
        !ReadFloat(owner, kStaminaSprintDrainOffset, sprintDrain)) {
        return false;
    }

    return
        std::fabs(recovery - kReferenceRecovery) <= 12.0f &&
        std::fabs(totalRecovery - kReferenceTotalRecovery) <= 20.0f &&
        std::fabs(sprintDrain - kReferenceSprintDrain) <= 15.0f;
}

bool LooksLikeHorse(
    void* movement,
    void* owner,
    float nativeGetMaxSpeed,
    float& maxWalkSpeed,
    float& maxAcceleration,
    float& sprintDrain
) {
    if (!movement || !owner ||
        !std::isfinite(nativeGetMaxSpeed) ||
        nativeGetMaxSpeed < 900.0f ||
        nativeGetMaxSpeed > 1700.0f) {
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

    if (std::fabs(maxWalkSpeed - kReferenceMaxWalkSpeed) > 120.0f ||
        std::fabs(maxAcceleration - kReferenceMaxAcceleration) > 100.0f) {
        return false;
    }

    float recovery = 0.0f;
    float totalRecovery = 0.0f;
    if (!ReadHorseStamina(
            owner,
            recovery,
            totalRecovery,
            sprintDrain)) {
        return false;
    }

    return true;
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
    g_lastHorseSeenTick.store(0);
}

void RestoreLocked() {
    if (!g_horse.captured) {
        return;
    }

    WriteFloat(g_horse.movement, kMaxWalkSpeedOffset, g_horse.maxWalkSpeed);
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

void ClearHorse(bool restore, const char* reason) {
    AcquireSRWLockExclusive(&g_lock);

    if (restore) {
        RestoreLocked();
    }

    if (g_horse.captured && reason) {
        FeatureLog("HorseFeature: release validated horse (%s)", reason);
    }

    g_horse = {};
    PublishCleared();

    ReleaseSRWLockExclusive(&g_lock);
}

void ApplyLocked() {
    if (!g_horse.captured) {
        return;
    }

    const float speedMultiplier =
        Clamp(g_speedMultiplier.load(), 0.0f, 3.0f);

    const float targetSpeed = g_speedEnabled.load()
        ? g_horse.maxWalkSpeed * speedMultiplier
        : g_horse.maxWalkSpeed;
    const float targetAcceleration = g_speedEnabled.load()
        ? g_horse.maxAcceleration * speedMultiplier
        : g_horse.maxAcceleration;

    if (WriteFloat(g_horse.movement, kMaxWalkSpeedOffset, targetSpeed)) {
        g_appliedMaxWalkSpeed.store(targetSpeed);
    }
    if (WriteFloat(
            g_horse.movement,
            kMaxAccelerationOffset,
            targetAcceleration)) {
        g_appliedMaxAcceleration.store(targetAcceleration);
    }

    const float durationMultiplier =
        Clamp(g_sprintDurationMultiplier.load(), 0.0f, 10.0f);

    float targetDrain = g_horse.sprintDrain;
    if (g_sprintDurationEnabled.load()) {
        targetDrain = durationMultiplier <= 0.0001f
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
    float sprintDrain = 0.0f;

    if (!LooksLikeHorse(
            movement,
            owner,
            nativeGetMaxSpeed,
            maxWalkSpeed,
            maxAcceleration,
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
        g_lastHorseSeenTick.store(GetTickCount64());

        FeatureLog(
            "HorseFeature: VALIDATED movement=%p owner=%p "
            "MaxWalkSpeed=%.1f MaxAcceleration=%.1f sprintDrain=%.3f",
            movement,
            owner,
            maxWalkSpeed,
            maxAcceleration,
            sprintDrain
        );

        ApplyLocked();
    }

    const bool accepted = g_horse.movement == movement;
    ReleaseSRWLockExclusive(&g_lock);
    return accepted;
}

float HookBaseGetMaxSpeed(void* movement) {
    const float native = g_originalBaseGetMaxSpeed
        ? g_originalBaseGetMaxSpeed(movement)
        : 0.0f;

    if (!movement) {
        return native;
    }

    void* owner = nullptr;
    if (!ReadAt(movement, kCharacterOwnerOffset, owner) || !owner) {
        return native;
    }

    if (g_validated.load()) {
        if (movement == g_movement.load()) {
            g_lastHorseSeenTick.store(GetTickCount64());
            AcquireSRWLockExclusive(&g_lock);
            ApplyLocked();
            ReleaseSRWLockExclusive(&g_lock);
        }
        return native;
    }

    TryCapture(movement, owner, native);
    return native;
}

bool InstallResolvedBaseHook(void* target) {
    if (!target) {
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK &&
        initStatus != MH_ERROR_ALREADY_INITIALIZED) {
        FeatureLog(
            "HorseFeature: MinHook initialize failed status=%d",
            static_cast<int>(initStatus)
        );
        return false;
    }

    const MH_STATUS createStatus = MH_CreateHook(
        target,
        reinterpret_cast<LPVOID>(&HookBaseGetMaxSpeed),
        reinterpret_cast<LPVOID*>(&g_originalBaseGetMaxSpeed)
    );
    if (createStatus != MH_OK &&
        createStatus != MH_ERROR_ALREADY_CREATED) {
        FeatureLog(
            "HorseFeature: base hook create failed status=%d",
            static_cast<int>(createStatus)
        );
        return false;
    }

    const MH_STATUS enableStatus = MH_EnableHook(target);
    if (enableStatus != MH_OK &&
        enableStatus != MH_ERROR_ENABLED) {
        FeatureLog(
            "HorseFeature: base hook enable failed status=%d",
            static_cast<int>(enableStatus)
        );
        return false;
    }

    g_baseGetMaxSpeedTarget = target;
    g_baseTargetTelemetry.store(target);
    g_baseHookReady.store(true);

    auto* module =
        reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr));
    FeatureLog(
        "HorseFeature: parent GetMaxSpeed hook READY RVA=0x%zX",
        static_cast<std::size_t>(
            reinterpret_cast<unsigned char*>(target) - module)
    );
    return true;
}

} // namespace

void Initialize(LogFn logger) {
    g_logger = logger;
    FeatureLog(
        "HorseFeature: waiting for validated player movement vtable anchor"
    );
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintDurationEnabled.store(settings.sprintDurationEnabled);
    g_sprintDurationMultiplier.store(settings.sprintDurationMultiplier);
}

void ObservePlayerMovement(
    void* movementComponent,
    void* playerGetMaxSpeedTarget
) {
    if (g_resolverAttempted.exchange(true)) {
        return;
    }

    void* parentTarget = nullptr;
    if (!ResolveParentGetMaxSpeed(
            movementComponent,
            playerGetMaxSpeedTarget,
            &parentTarget)) {
        FeatureLog(
            "HorseFeature: parent GetMaxSpeed resolution failed; fail-open"
        );
        return;
    }

    InstallResolvedBaseHook(parentTarget);
}

void Tick() {
    if (!g_validated.load()) {
        return;
    }

    const ULONGLONG lastSeen = g_lastHorseSeenTick.load();
    const ULONGLONG now = GetTickCount64();

    if (lastSeen != 0 && now - lastSeen > kHorseLostTimeoutMs) {
        ClearHorse(true, "movement no longer active");
    }
}

Telemetry GetTelemetry() {
    Telemetry t{};
    t.resolverAttempted = g_resolverAttempted.load();
    t.baseHookReady = g_baseHookReady.load();
    t.movementValidated = g_validated.load();
    t.staminaReady = g_staminaReady.load();
    t.horseOwner = g_owner.load();
    t.horseMovement = g_movement.load();
    t.baseGetMaxSpeedTarget = g_baseTargetTelemetry.load();
    t.bestVtableScore = g_bestVtableScore.load();
    t.secondVtableScore = g_secondVtableScore.load();
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
    ClearHorse(true, "shutdown");

    if (g_baseGetMaxSpeedTarget) {
        MH_DisableHook(g_baseGetMaxSpeedTarget);
    }

    g_baseHookReady.store(false);
    FeatureLog("HorseFeature: shutdown");
}

} // namespace dg::horse
