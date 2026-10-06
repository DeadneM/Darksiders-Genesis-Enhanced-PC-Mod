#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <MinHook.h>

#include "HorseFeature.h"

#include <algorithm>
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
constexpr ULONGLONG kHorseLostTimeoutMs = 1500;

struct PeSection {
    unsigned char* begin = nullptr;
    std::size_t size = 0;
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

std::atomic_bool g_baseHookReady{false};
std::atomic_bool g_validated{false};
std::atomic_bool g_staminaReady{false};
std::atomic<void*> g_owner{nullptr};
std::atomic<void*> g_movement{nullptr};

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

std::atomic_uint32_t g_resolverMatches{0};
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

bool IsRcXDisp32ByteRead(
    const unsigned char* p,
    std::uint32_t displacement
) {
    if (!p) {
        return false;
    }

    // movzx r32, byte ptr [rcx+disp32]
    return p[0] == 0x0F &&
           p[1] == 0xB6 &&
           (p[2] & 0xC7) == 0x81 &&
           *reinterpret_cast<const std::uint32_t*>(p + 3) == displacement;
}

bool IsRcXDisp32FloatRead(
    const unsigned char* p,
    std::uint32_t displacement
) {
    if (!p) {
        return false;
    }

    // movss xmm?, dword ptr [rcx+disp32]
    return p[0] == 0xF3 &&
           p[1] == 0x0F &&
           p[2] == 0x10 &&
           (p[3] & 0xC7) == 0x81 &&
           *reinterpret_cast<const std::uint32_t*>(p + 4) == displacement;
}

unsigned char* FindFunctionStart(
    unsigned char* textBegin,
    unsigned char* anchor
) {
    if (!textBegin || !anchor || anchor <= textBegin) {
        return nullptr;
    }

    const std::size_t maxBack =
        std::min<std::size_t>(48, static_cast<std::size_t>(anchor - textBegin));

    for (std::size_t back = 1; back <= maxBack; ++back) {
        unsigned char* p = anchor - back;
        if (*p != 0xCC) {
            continue;
        }

        while (p < anchor && *p == 0xCC) {
            ++p;
        }

        if (p < anchor) {
            return p;
        }
    }

    return nullptr;
}

unsigned char* ResolveBaseGetMaxSpeed() {
    PeSection text{};
    if (!GetMainModuleSection(".text", text)) {
        FeatureLog("HorseFeature: failed to enumerate .text");
        return nullptr;
    }

    unsigned char* matches[8]{};
    std::uint32_t matchCount = 0;

    for (std::size_t i = 0; i + 8 < text.size; ++i) {
        unsigned char* anchor = text.begin + i;
        if (!IsRcXDisp32ByteRead(
                anchor,
                static_cast<std::uint32_t>(kMovementModeOffset))) {
            continue;
        }

        unsigned char* start = FindFunctionStart(text.begin, anchor);
        if (!start) {
            continue;
        }

        bool readsMaxWalkSpeed = false;
        for (std::size_t j = 0; j + 8 < 0x120; ++j) {
            unsigned char* p = start + j;
            if (p < text.begin || p + 8 >= text.begin + text.size) {
                break;
            }

            if (IsRcXDisp32FloatRead(
                    p,
                    static_cast<std::uint32_t>(kMaxWalkSpeedOffset))) {
                readsMaxWalkSpeed = true;
                break;
            }
        }

        if (!readsMaxWalkSpeed) {
            continue;
        }

        bool duplicate = false;
        for (std::uint32_t m = 0; m < matchCount && m < 8; ++m) {
            if (matches[m] == start) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            continue;
        }

        if (matchCount < 8) {
            matches[matchCount] = start;
        }
        ++matchCount;
    }

    g_resolverMatches.store(matchCount);

    auto* module = reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr));
    if (matchCount != 1) {
        FeatureLog(
            "HorseFeature: base GetMaxSpeed resolver match count=%u "
            "rva0=0x%zX rva1=0x%zX rva2=0x%zX",
            matchCount,
            matches[0] ? static_cast<std::size_t>(matches[0] - module) : 0,
            matches[1] ? static_cast<std::size_t>(matches[1] - module) : 0,
            matches[2] ? static_cast<std::size_t>(matches[2] - module) : 0
        );
        return nullptr;
    }

    FeatureLog(
        "HorseFeature: base UCharacterMovementComponent::GetMaxSpeed "
        "candidate RVA=0x%zX",
        static_cast<std::size_t>(matches[0] - module)
    );
    return matches[0];
}

bool ReadHorseStamina(void* owner, float& sprintDrain) {
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

bool LooksLikeHorse(
    void* movement,
    void* owner,
    float& maxWalkSpeed,
    float& maxAcceleration,
    float& sprintDrain
) {
    if (!movement || !owner) {
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

    const bool movementSignature =
        std::fabs(maxWalkSpeed - kReferenceMaxWalkSpeed) <= 250.0f &&
        std::fabs(maxAcceleration - kReferenceMaxAcceleration) <= 250.0f;

    if (!movementSignature) {
        return false;
    }

    return ReadHorseStamina(owner, sprintDrain);
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

bool TryCapture(void* movement, void* owner) {
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
            "HorseFeature: VALIDATED via base GetMaxSpeed movement=%p owner=%p "
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

    TryCapture(movement, owner);
    return native;
}

} // namespace

bool Initialize(LogFn logger) {
    g_logger = logger;

    unsigned char* target = ResolveBaseGetMaxSpeed();
    if (!target) {
        FeatureLog(
            "HorseFeature: base hook unavailable; feature remains fail-open"
        );
        return false;
    }

    const MH_STATUS initStatus = MH_Initialize();
    if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED) {
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
    g_baseHookReady.store(true);
    FeatureLog(
        "HorseFeature: base GetMaxSpeed hook READY "
        "reference MaxWalkSpeed=1300 MaxAcceleration=600"
    );
    return true;
}

void SetSettings(const Settings& settings) {
    g_speedEnabled.store(settings.speedEnabled);
    g_speedMultiplier.store(settings.speedMultiplier);
    g_sprintDurationEnabled.store(settings.sprintDurationEnabled);
    g_sprintDurationMultiplier.store(settings.sprintDurationMultiplier);
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
    t.baseHookReady = g_baseHookReady.load();
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
    t.resolverMatches = g_resolverMatches.load();
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
