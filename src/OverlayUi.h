#pragma once

#include <windows.h>

#include <atomic>
#include <string>

#include "ConfigStore.h"
#include "TargetValidator.h"

namespace dg::overlay {

using ApplySkipIntroFn = bool(*)(bool);
using ApplySkipLogosFn = bool(*)(bool);
using ApplySkipWarningFn = bool(*)(bool);
using ApplyGraphicsAdapterFn = bool(*)(int);
using LogFn = void(*)(const char*, ...);

struct Telemetry {
    bool hudHookReady = false;
    bool movementHookReady = false;
    bool finalDamageHookReady = false;
    bool hotstreakHookReady = false;
    unsigned aimUiTargetingEvents = 0;
    unsigned aimUiPositionEvents = 0;
    unsigned mountedIdleLookBlocks = 0;
    unsigned grenadePhysicalDown = 0;
    unsigned grenadePhysicalUp = 0;
    unsigned grenadeNativeReleasePulses = 0;
    bool skipIntroReady = false;
    bool skipLogosProxyAvailable = false;
    bool skipLogosTargetValid = false;
    bool skipLogosPatched = false;
    bool skipLogosEnabled = true;
    bool skipWarningReady = false;
    bool skipWarningAttempted = false;
    bool skipWarningApplied = false;


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
    std::atomic_int* captureCameraKeyIndex = nullptr;
    std::atomic_bool* hudHidden = nullptr;
    std::string* lastAction = nullptr;

    Telemetry telemetry{};

    ApplySkipIntroFn applySkipIntro = nullptr;
    ApplySkipLogosFn applySkipLogos = nullptr;
    ApplySkipWarningFn applySkipWarning = nullptr;
    ApplyGraphicsAdapterFn applyGraphicsAdapter = nullptr;
    LogFn log = nullptr;
};

void Draw(Context& context);

} // namespace dg::overlay
