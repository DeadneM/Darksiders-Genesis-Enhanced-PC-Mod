#pragma once

#include <array>
#include <atomic>
#include <string>

namespace dg::config {

enum class Action : int {
    None = 0,
    ToggleHUD,
    MovementSpeed,
    ActionRecovery,
    SkipIntroVideos,
    ThirdPerson,
    Count
};

inline constexpr std::array<const char*, static_cast<std::size_t>(Action::Count)>
kActionLabels = {
    "None",
    "Toggle HUD",
    "Movement Speed",
    "Action Recovery",
    "Skip Intro Videos",
    "Third Person"
};

inline constexpr std::array<const wchar_t*, static_cast<std::size_t>(Action::Count)>
kActionTokens = {
    L"None",
    L"ToggleHUD",
    L"MovementSpeed",
    L"ActionRecovery",
    L"SkipIntroVideos",
    L"ThirdPerson"
};

const char* ActionLabel(Action action);
const wchar_t* ActionToken(Action action);
Action ParseAction(const wchar_t* text);

std::string KeyDisplayName(int vk);
std::wstring KeyTokenFromVK(int vk);
int ParseKeyToken(const wchar_t* text, int fallback);

using LogFn = void(*)(const char*);

class Store {
public:
    bool overlayEnabled = true;
    int menuKey = 0x2D; // VK_INSERT

    bool toggleHudEnabled = true;
    bool movementSpeedEnabled = true;
    bool actionRecoveryEnabled = true;
    bool skipLogosEnabled = true;
    bool skipIntroEnabled = true;
    bool thirdPersonEnabled = false;
    bool pistolDamageEnabled = true;
    bool meleeDamageEnabled = true;
    bool jumpHeightEnabled = true;
    bool glideDurationEnabled = true;
    bool horseSpeedEnabled = true;
    bool horseSprintSpeedEnabled = true;
    bool horseSprintDurationEnabled = true;
    bool fovEnabled = false;
    bool hotstreakChargeEnabled = true;

    float movementSpeedMultiplier = 1.50f;
    float actionRecoveryDelayMs = 0.0f;
    float actionRecoveryMultiplier = 2.00f;
    float dodgeEarlyUnlockMs = 100.0f;
    float pistolDamageMultiplier = 2.00f;
    float meleeDamageMultiplier = 2.00f;
    float jumpHeightMultiplier = 1.25f;
    float glideDurationMultiplier = 10.00f;
    float horseSpeedMultiplier = 1.25f;
    float horseSprintSpeedMultiplier = 1.25f;
    float horseSprintDurationMultiplier = 5.00f;
    float fovDegrees = 90.0f;
    float thirdPersonDistanceMultiplier = 1.00f;
    float hotstreakChargeMultiplier = 2.00f;

    std::array<Action, 12> hotkeys{};

    Store();

    void SetPath(const std::wstring& path);
    void SetLogger(LogFn logger);

    void ResetDefaults(bool persist);
    bool Load();

    // Publish immediately to runtime atomics and mark disk state dirty.
    void Save();

    // Persist the whole INI immediately.
    bool SaveNow();

    // Persist after a 500 ms quiet period, or immediately when force=true.
    void FlushIfDue(bool force = false);

    void PublishRuntime() const;
    bool IsDirty() const;

private:
    std::wstring path_;
    LogFn logger_ = nullptr;
    std::atomic_bool dirty_{false};
    std::atomic_ullong dirtyTick_{0};

    void Log(const char* text) const;
    void MarkDirty();
};

} // namespace dg::config
