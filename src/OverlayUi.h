#pragma once

#include <windows.h>

#include <atomic>
#include <string>

#include "ConfigStore.h"
#include "TargetValidator.h"

namespace dg::overlay {

using ApplySkipIntroFn = bool(*)(bool);
using ApplySkipLogosFn = bool(*)(bool);
using AbilityStateNameFn = const char*(*)(unsigned char);
using LogFn = void(*)(const char*, ...);

struct Telemetry {
    bool hudHookReady = false;
    bool movementHookReady = false;
    bool recoveryHookReady = false;
    bool finalDamageHookReady = false;
    bool hotstreakHookReady = false;
    bool skipIntroReady = false;
    bool skipLogosProxyAvailable = false;
    bool skipLogosInstalled = false;
    bool skipLogosEnabled = true;
    LONG skipLogosCreateFileCalls = 0;
    LONG skipLogosMp4Calls = 0;
    LONG skipLogosBlocked = 0;

    int actionMoveQueries = 0;
    int actionMoveLocalQueries = 0;
    int actionMoveNativeBlocked = 0;
    int actionMoveForced = 0;
    int lastActionMoveState = -1;
    float lastActionMoveElapsed = 0.0f;

    int pistolDamageBoostCalls = 0;
    float lastNativePistolDamage = 0.0f;
    float lastBoostedPistolDamage = 0.0f;
    float lastPistolBaseJuice = 0.0f;

    int meleeDamageBoostCalls = 0;
    float lastNativeBaseDamage = 0.0f;
    float lastBoostedBaseDamage = 0.0f;
    unsigned lastOutgoingScaleType = 0;
    int lastOutgoingTagCount = 0;

    int hotstreakBoostCalls = 0;
    float lastNativeJuiceGain = 0.0f;
    float lastBoostedJuiceGain = 0.0f;

    LONG* skipIntroData = nullptr;
    LONG skipIntroOriginalValue = 1;
};

struct Context {
    const char* build = nullptr;
    config::Store* config = nullptr;
    const target::ValidationResult* targetValidation = nullptr;

    std::atomic_bool* overlayVisible = nullptr;
    std::atomic_bool* captureMenuKey = nullptr;
    std::atomic_bool* hudHidden = nullptr;
    std::string* lastAction = nullptr;

    Telemetry telemetry{};

    ApplySkipIntroFn applySkipIntro = nullptr;
    ApplySkipLogosFn applySkipLogos = nullptr;
    AbilityStateNameFn abilityStateName = nullptr;
    LogFn log = nullptr;
};

void Draw(Context& context);

} // namespace dg::overlay
