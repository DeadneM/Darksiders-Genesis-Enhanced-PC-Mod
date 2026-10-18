#include <windows.h>

#include "ConfigStore.h"
#include "RuntimeSettings.h"

#include <cwchar>
#include <cstdio>
#include <algorithm>
#include <cmath>

namespace dg::config {
namespace {

constexpr int kConfigRevision = 2140;
// Actual XInput digital buttons plus LT/RT represented as virtual binding IDs.
constexpr bool ValidPadBind(int m) {
    switch(m){case 0:case 0x0001:case 0x0002:case 0x0004:case 0x0008:
        case 0x0010:case 0x0020:case 0x0040:case 0x0080:
        case 0x0100:case 0x0200:case 0x1000:case 0x2000:
        case 0x4000:case 0x8000:case 0x10000:case 0x20000:return true;
        default:return false;}
}

bool ReadBool(
    const wchar_t* section,
    const wchar_t* key,
    bool fallback,
    const std::wstring& path) {
    return GetPrivateProfileIntW(
        section,
        key,
        fallback ? 1 : 0,
        path.c_str()
    ) != 0;
}

float ReadFloat(
    const wchar_t* section,
    const wchar_t* key,
    float fallback,
    const std::wstring& path) {
    wchar_t buffer[64]{};
    wchar_t fallbackText[64]{};
    swprintf_s(fallbackText, L"%.3f", fallback);

    GetPrivateProfileStringW(
        section,
        key,
        fallbackText,
        buffer,
        64,
        path.c_str()
    );

    wchar_t* end = nullptr;
    const float value = wcstof(buffer, &end);
    return (end && end != buffer && std::isfinite(value)) ? value : fallback;
}

// SaveNow must not report success or clear dirty state after a partial write.
// Scoped tracking also covers writes made by the typed helpers below.
struct ProfileWriteStatus { bool ok=true; DWORD error=ERROR_SUCCESS; };
thread_local ProfileWriteStatus* activeWriteStatus=nullptr;
struct ProfileWriteScope {
    ProfileWriteStatus status;
    ProfileWriteStatus* previous=activeWriteStatus;
    ProfileWriteScope(){activeWriteStatus=&status;}
    ~ProfileWriteScope(){activeWriteStatus=previous;}
};
BOOL WriteProfileString(const wchar_t* section,const wchar_t* key,
                        const wchar_t* value,const wchar_t* path){
    const BOOL ok=WritePrivateProfileStringW(section,key,value,path);
    if(!ok && activeWriteStatus){
        if(activeWriteStatus->ok)activeWriteStatus->error=GetLastError();
        activeWriteStatus->ok=false;
    }
    return ok;
}

void WriteBool(
    const wchar_t* section,
    const wchar_t* key,
    bool value,
    const std::wstring& path) {
    WriteProfileString(
        section,
        key,
        value ? L"1" : L"0",
        path.c_str()
    );
}

void WriteFloat(
    const wchar_t* section,
    const wchar_t* key,
    float value,
    const std::wstring& path) {
    wchar_t buffer[64]{};
    swprintf_s(buffer, L"%.3f", value);
    WriteProfileString(
        section,
        key,
        buffer,
        path.c_str()
    );
}

bool IsExtendedVirtualKey(int vk) {
    switch (vk) {
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_LEFT:
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
    case VK_DIVIDE:
    case VK_NUMLOCK:
        return true;
    default:
        return false;
    }
}

} // namespace

const char* ActionLabel(Action action) {
    const int i = static_cast<int>(action);
    if (i < 0 || i >= static_cast<int>(Action::Count)) {
        return "None";
    }
    return kActionLabels[static_cast<std::size_t>(i)];
}

const wchar_t* ActionToken(Action action) {
    const int i = static_cast<int>(action);
    if (i < 0 || i >= static_cast<int>(Action::Count)) {
        return L"None";
    }
    return kActionTokens[static_cast<std::size_t>(i)];
}

Action ParseAction(const wchar_t* text) {
    if (!text) {
        return Action::None;
    }

    for (int i = 0; i < static_cast<int>(Action::Count); ++i) {
        if (_wcsicmp(
                text,
                kActionTokens[static_cast<std::size_t>(i)]) == 0) {
            return static_cast<Action>(i);
        }
    }

    return Action::None;
}

std::string KeyDisplayName(int vk) {
    if (vk <= 0 || vk >= 256) {
        return "Unbound";
    }

    UINT scan = MapVirtualKeyA(
        static_cast<UINT>(vk),
        MAPVK_VK_TO_VSC
    );
    LONG keyData = static_cast<LONG>(scan << 16);
    if (IsExtendedVirtualKey(vk)) {
        keyData |= (1 << 24);
    }

    char name[64]{};
    if (GetKeyNameTextA(
            keyData,
            name,
            static_cast<int>(sizeof(name))) > 0) {
        return name;
    }

    char fallback[16]{};
    sprintf_s(fallback, sizeof(fallback), "VK_%02X", vk & 0xFF);
    return fallback;
}

std::wstring KeyTokenFromVK(int vk) {
    switch (vk) {
    case 0: return L"None";
    case VK_UP: return L"Up";
    case VK_DOWN: return L"Down";
    case VK_LEFT: return L"Left";
    case VK_RIGHT: return L"Right";
    case VK_INSERT: return L"Insert";
    case VK_DELETE: return L"Delete";
    case VK_HOME: return L"Home";
    case VK_END: return L"End";
    case VK_PRIOR: return L"PageUp";
    case VK_NEXT: return L"PageDown";
    case VK_TAB: return L"Tab";
    case VK_CAPITAL: return L"CapsLock";
    case VK_PAUSE: return L"Pause";
    case VK_SCROLL: return L"ScrollLock";
    case VK_SPACE: return L"Space";
    default:
        break;
    }

    if (vk >= VK_F1 && vk <= VK_F24) {
        wchar_t text[16]{};
        swprintf_s(text, L"F%d", (vk - VK_F1) + 1);
        return text;
    }

    if ((vk >= '0' && vk <= '9') ||
        (vk >= 'A' && vk <= 'Z')) {
        wchar_t text[2]{
            static_cast<wchar_t>(vk),
            L'\0'
        };
        return text;
    }

    wchar_t fallback[16]{};
    swprintf_s(fallback, L"VK_%02X", vk & 0xFF);
    return fallback;
}

int ParseKeyToken(const wchar_t* text, int fallback) {
    if (!text || !*text) {
        return fallback;
    }

    struct NamedKey {
        const wchar_t* name;
        int vk;
    };

    constexpr NamedKey named[] = {
        {L"None", 0},
        {L"Up", VK_UP}, {L"Down", VK_DOWN},
        {L"Left", VK_LEFT}, {L"Right", VK_RIGHT},
        {L"Insert", VK_INSERT},
        {L"Delete", VK_DELETE},
        {L"Home", VK_HOME},
        {L"End", VK_END},
        {L"PageUp", VK_PRIOR},
        {L"PageDown", VK_NEXT},
        {L"Tab", VK_TAB},
        {L"CapsLock", VK_CAPITAL},
        {L"Pause", VK_PAUSE},
        {L"ScrollLock", VK_SCROLL},
        {L"Space", VK_SPACE}
    };

    for (const auto& entry : named) {
        if (_wcsicmp(text, entry.name) == 0) {
            return entry.vk;
        }
    }

    if ((text[0] == L'F' || text[0] == L'f') && text[1]) {
        const int n = _wtoi(text + 1);
        if (n >= 1 && n <= 24) {
            return VK_F1 + (n - 1);
        }
    }

    if (text[0] && !text[1]) {
        wchar_t ch = text[0];
        if (ch >= L'a' && ch <= L'z') {
            ch = static_cast<wchar_t>(ch - L'a' + L'A');
        }
        if ((ch >= L'0' && ch <= L'9') ||
            (ch >= L'A' && ch <= L'Z')) {
            return static_cast<int>(ch);
        }
    }

    if (_wcsnicmp(text, L"VK_", 3) == 0 && text[3]) {
        wchar_t* end = nullptr;
        const long value = wcstol(text + 3, &end, 16);
        if (end != text + 3 && value > 0 && value < 256) {
            return static_cast<int>(value);
        }
    }

    return fallback;
}

Store::Store() {
    // Member initializers already carry the scalar defaults. Keep global
    // construction side-effect free and initialize only the hotkey table here.
    hotkeys = kDefaultHotkeys;
    cameraKeys = kDefaultCameraKeys;
}

void Store::SetPath(const std::wstring& path) {
    path_ = path;
}

void Store::SetLogger(LogFn logger) {
    logger_ = logger;
}

void Store::Log(const char* text) const {
    if (logger_ && text) {
        logger_(text);
    }
}

void Store::ResetDefaults(bool persist) {
    overlayEnabled = true;
    menuKey = defaults::MenuKey;
    graphicsAdapter = 0;
    updaterAutoCheck = false;
    updaterBeta = defaults::UpdaterBeta;

    toggleHudEnabled = true;interactionHudScaleEnabled=true;interactionHudScale=0.65f;
    weaponSheathingEnabled=false;sheathNeverEnabled=true;sheathDelaySeconds=10.0f;
    sheathKeyboardKey=0;sheathPadButtonA=0;sheathPadButtonB=0;
    movementSpeedEnabled = true;
    skipLogosEnabled = true;
    skipIntroEnabled = true;
    skipWarningEnabled = true;
    thirdPersonEnabled = true;
    thirdPersonFovEnabled = true;
    tpsLensFocusEnabled = true;
    cameraOrbitInputEnabled = true;
    cameraControllerOrbitEnabled = true;
    tpsCameraStickMode = 2;
    tpsCutsceneAutoSuspend = true;
    tpsHoldSuspendEnabled = true;
    tpsLongPressToggleEnabled = true;
    tpsGrenadeHoldEnabled = true;
    aimIndicatorHideMode = 2; // TPS Only (default)
    tps3DAimEnabled = true;
    tpsAimPitchOffsetDegrees = 12.0f;
    tpsMountedAimMode = 0;
    tpsGrenadeButtonMask = 0x0080;
    tpsFirePrimaryMask=0x20000;
    tpsFireSecondaryMask=0x0200;
    tpsMousePrimaryButton=VK_LBUTTON;
    tpsMouseSecondaryButton=VK_RBUTTON;
    tpsWheelPadMask=0x0100;
    tpsSuspendButtonMask = 0x0080;
    tpsSuspendDelayMs = 300;
    tpsToggleHoldButtonMask = 0x0040;
    tpsToggleHoldMs = 500;
    tpsRecenterButtonMask = 0x0040;
    horsePadButtonA = 0x0100; // LB
    horsePadButtonB = 0x0200; // RB
    tpsKeyboardKeys = kDefaultTpsKeyboardKeys;
    pistolDamageEnabled = true;
    meleeDamageEnabled = true;
    jumpHeightEnabled = true;
    glideDurationEnabled = true;
    horseEverywhereEnabled = true;
    horseAreaMode = 2;
    horseSpeedEnabled = true;
    horseSprintSpeedEnabled = true;
    horseSprintDurationEnabled = true;
    fovEnabled = false;
    hotstreakChargeEnabled = true;
    wrathChargeEnabled = true;
    ultimateChargeEnabled = true;
    synergyChargeEnabled = true;
    unlimitedWrathEnabled = false;
    unlimitedUltimateEnabled = false;
    unlimitedSynergyEnabled = false;
    finiteStandardAmmoEnabled=false;standardAmmoCapacity=200;
    grappleRangeEnabled=false;grappleRangeMultiplier=2.0f;infiniteDoubleJumpMode=0;
    reduceAmmoEnabled = defaults::ReduceAmmo;unlimitedAmmoEnabled = false;ammoConsumptionDivisor = 2.0f;
    reduceHotstreakEnabled=false;unlimitedHotstreakEnabled=false;hotstreakDecreaseDivisor=2.0f;
    potionRechargeEnabled=false;unlimitedPotionsEnabled=false;potionRechargeMultiplier=2.0f;
    unlimitedHealthEnabled=false;
    unlimitedGlideEnabled=false;
    unlimitedHorseSprintEnabled=false;

    movementSpeedMultiplier = 1.50f;
    pistolDamageMultiplier = 2.00f;
    meleeDamageMultiplier = 2.00f;
    jumpHeightMultiplier = 1.25f;
    glideDurationMultiplier = 10.00f;
    horseSpeedMultiplier = 1.25f;
    horseSprintSpeedMultiplier = 1.25f;
    horseSprintDurationMultiplier = 5.00f;
    fovDegrees = 90.0f;
    thirdPersonFovDegrees = 110.0f;
    thirdPersonCoopFovDegrees = 110.0f;
    cameraZoomPercent = 0.0f;
    cameraPitchDegrees = 0.0f;
    cameraYawDegrees = 0.0f;
    cameraHeightOffset = 0.0f;
    thirdPersonDistanceMultiplier = 0.15f;
    thirdPersonPitchDegrees = -12.0f;
    thirdPersonHeightOffset = 0.0f;
    tpsUltimateDistanceMultiplier = defaults::UltimateDistance;
    tpsUltimateHeightOffset = defaults::UltimateHeight;
    tpsHorseDistanceMultiplier = defaults::HorseDistance;
    tpsHorseHeightOffset = defaults::HorseHeight;
    tpsFootAnchorOffset = 0.0f;
    cameraStickSpeed = 180.0f;
    hotstreakChargeMultiplier = 2.00f;
    wrathChargeMultiplier = defaults::WrathGain;
    ultimateChargeMultiplier = defaults::UltimateGain;
    synergyChargeMultiplier = defaults::SynergyGain;

    hotkeys = kDefaultHotkeys;
    cameraKeys = kDefaultCameraKeys;

    PublishRuntime();

    if (persist) {
        SaveNow();
    }
}

bool Store::ResetIniFileToDefaults() {
    if (path_.empty()) {
        Log("INI factory reset refused: missing path");
        return false;
    }
    const DWORD attributes = GetFileAttributesW(path_.c_str());
    const bool hadIni = attributes != INVALID_FILE_ATTRIBUTES;
    std::wstring backupPath;
    if (hadIni) {
        if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            Log("INI factory reset refused: path is a directory");
            return false;
        }
        SYSTEMTIME now{};
        GetLocalTime(&now);
        wchar_t stamp[80]{};
        swprintf_s(stamp, L".backup-%04u%02u%02u-%02u%02u%02u-%03u",
            static_cast<unsigned>(now.wYear), static_cast<unsigned>(now.wMonth),
            static_cast<unsigned>(now.wDay), static_cast<unsigned>(now.wHour),
            static_cast<unsigned>(now.wMinute), static_cast<unsigned>(now.wSecond),
            static_cast<unsigned>(now.wMilliseconds));
        const std::wstring stem = path_ + stamp;
        backupPath = stem + L".bak";
        for (int n = 1; n < 100 &&
             GetFileAttributesW(backupPath.c_str()) != INVALID_FILE_ATTRIBUTES; ++n)
            backupPath = stem + L"-" + std::to_wstring(n) + L".bak";
        if (GetFileAttributesW(backupPath.c_str()) != INVALID_FILE_ATTRIBUTES ||
            !MoveFileW(path_.c_str(), backupPath.c_str())) {
            Log("INI factory reset refused: cannot preserve old INI");
            return false;
        }
    }

    ResetDefaults(false);
    const bool saved = SaveNow();
    const bool verified = saved &&
        GetFileAttributesW(path_.c_str()) != INVALID_FILE_ATTRIBUTES &&
        GetPrivateProfileIntW(L"Meta", L"ConfigRevision", -1, path_.c_str()) == kConfigRevision &&
        GetPrivateProfileIntW(L"Features", L"SynergyCharge", -1, path_.c_str()) == 1;
    if (!verified) {
        if (hadIni) {
            DeleteFileW(path_.c_str());
            if (MoveFileW(backupPath.c_str(), path_.c_str())) {
                Load();
                Log("INI factory reset failed: previous INI restored");
            } else {
                Log("INI factory reset failed: previous INI retained in dated backup");
            }
        } else {
            Log("INI factory reset failed: default file could not be written");
        }
        return false;
    }
    Log(hadIni ? "INI factory reset: defaults saved, dated original backup preserved beside INI"
               : "INI factory reset: new default INI created");
    return true;
}

void Store::ResetVanillaCameraValues() {
    fovDegrees=90.0f;
    cameraZoomPercent=0.0f;
    cameraPitchDegrees=0.0f;
    cameraYawDegrees=0.0f;
    cameraHeightOffset=0.0f;
    Save();
    Log("V1.05 Camera Values Default: FOV=90 zoom=0 pitch=0 yaw=0 height=0");
}
void Store::ResetTpsCameraValues() {
    thirdPersonFovEnabled=true;
    tpsLensFocusEnabled=true;
    thirdPersonFovDegrees=110.0f;
    thirdPersonCoopFovDegrees=110.0f;
    thirdPersonDistanceMultiplier=0.15f;
    thirdPersonPitchDegrees=-12.0f;
    thirdPersonHeightOffset=0.0f;
    tpsUltimateDistanceMultiplier=defaults::UltimateDistance;
    tpsUltimateHeightOffset=defaults::UltimateHeight;
    tpsHorseDistanceMultiplier=defaults::HorseDistance;
    tpsHorseHeightOffset=defaults::HorseHeight;
    tpsFootAnchorOffset=0.0f;
    cameraStickSpeed=180.0f;
    dg::runtime::Get().cameraOrbitYawDegrees.store(0.0f);
    dg::runtime::Get().cameraOrbitPitchDegrees.store(0.0f);
    Save();
    char message[320]{};
    sprintf_s(message,"TPS Camera Values Default: solo/coop TPS FOV 110, distance .15, pitch -12, height 0; Ultimate distance %.2f, height %.0f; Horse distance %.2f, height %.0f; foot 0, speed 180",
        tpsUltimateDistanceMultiplier,tpsUltimateHeightOffset,tpsHorseDistanceMultiplier,tpsHorseHeightOffset);
    Log(message);
}
bool Store::Load() {
    if (path_.empty()) {
        return false;
    }

    const DWORD attributes=GetFileAttributesW(path_.c_str());
    if(attributes!=INVALID_FILE_ATTRIBUTES && (attributes&FILE_ATTRIBUTE_DIRECTORY)){
        Log("INI load failed: path is a directory");
        return false;
    }
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        ResetDefaults(false);
        if(!SaveNow())return false;
        Log("INI not found -> wrote authoritative defaults");
        return true;
    }

    overlayEnabled =
        ReadBool(L"Overlay", L"Enabled", true, path_);

    wchar_t menuKeyText[64]{};
    GetPrivateProfileStringW(
        L"Overlay",
        L"MenuKey",
        L"Insert",
        menuKeyText,
        64,
        path_.c_str()
    );
    menuKey = ParseKeyToken(menuKeyText, defaults::MenuKey);

    updaterAutoCheck = ReadBool(L"Updater", L"AutoCheckUpdates", false, path_);
    updaterBeta = ReadBool(L"Updater", L"Beta", defaults::UpdaterBeta, path_);
    graphicsAdapter = GetPrivateProfileIntW(
        L"System",
        L"GraphicsAdapter",
        0,
        path_.c_str()
    );
    if (graphicsAdapter < 0) {
        graphicsAdapter = 0;
    }
    if (graphicsAdapter > 4) {
        graphicsAdapter = 4;
    }

    toggleHudEnabled = true; // V0.54: hidden/visible is the only HUD state
    weaponSheathingEnabled=ReadBool(L"Features",L"WeaponSheathing",false,path_);
    sheathNeverEnabled=ReadBool(L"Features",L"NeverAutoSheathe",true,path_);
    sheathDelaySeconds=std::clamp(ReadFloat(L"Values",L"SheathDelaySeconds",10.0f,path_),0.0f,300.0f);
    wchar_t sheathKey[64]{};
    GetPrivateProfileStringW(L"Controls",L"SheatheKeyboardKey",L"OFF",sheathKey,64,path_.c_str());
    sheathKeyboardKey=ParseKeyToken(sheathKey,0);
    if(sheathKeyboardKey<0||sheathKeyboardKey>=256)sheathKeyboardKey=0;
    sheathPadButtonA=GetPrivateProfileIntW(L"Controls",L"SheathePadButtonA",0,path_.c_str());
    sheathPadButtonB=GetPrivateProfileIntW(L"Controls",L"SheathePadButtonB",0,path_.c_str());
    if(!ValidPadBind(sheathPadButtonA))sheathPadButtonA=0;
    if(!ValidPadBind(sheathPadButtonB))sheathPadButtonB=0;
    interactionHudScaleEnabled=ReadBool(L"Features",L"InteractionHudScale",true,path_);
    interactionHudScale=std::clamp(ReadFloat(L"Values",L"InteractionHudScale",0.65f,path_),0.1f,2.0f);
    movementSpeedEnabled =
        ReadBool(L"Features", L"MovementSpeed", true, path_);
    skipLogosEnabled =
        ReadBool(L"Features", L"SkipLogos", true, path_);
    skipIntroEnabled =
        ReadBool(L"Features", L"SkipIntroVideos", true, path_);
    // V1.38 presentation-only experiment; legacy asset-loop bypass stays OFF.
    skipWarningEnabled = ReadBool(L"Features",L"SkipWarning",true,path_);
    unlimitedWrathEnabled=ReadBool(L"Features",L"UnlimitedWrath",false,path_);
    unlimitedUltimateEnabled=ReadBool(L"Features",L"UnlimitedUltimate",false,path_);
    unlimitedSynergyEnabled=ReadBool(L"Features",L"UnlimitedSynergy",false,path_);
    finiteStandardAmmoEnabled=ReadBool(L"Features",L"FiniteStandardAmmo",false,path_);
    standardAmmoCapacity=std::clamp(int(GetPrivateProfileIntW(L"Values",L"StandardAmmoCapacity",200,path_.c_str())),1,10000);
    grappleRangeEnabled=ReadBool(L"Features",L"GrappleRange",false,path_);
    grappleRangeMultiplier=std::clamp(ReadFloat(L"Values",L"GrappleRangeMultiplier",2.0f,path_),1.0f,10.0f);
    infiniteDoubleJumpMode=std::clamp(int(GetPrivateProfileIntW(L"Values",L"InfiniteDoubleJumpMode",0,path_.c_str())),0,2);
    reduceAmmoEnabled=ReadBool(L"Features",L"ReduceAmmoConsumption",defaults::ReduceAmmo,path_);
    unlimitedAmmoEnabled=ReadBool(L"Features",L"UnlimitedAmmo",false,path_);
    reduceHotstreakEnabled=ReadBool(L"Features",L"ReduceHotstreakDecrease",false,path_);
    unlimitedHotstreakEnabled=ReadBool(L"Features",L"UnlimitedHotstreak",false,path_);
    hotstreakDecreaseDivisor=std::clamp(ReadFloat(L"Values",L"HotstreakDecreaseDivisor",2.0f,path_),0.0f,10.0f);
    potionRechargeEnabled=ReadBool(L"Features",L"PotionRecharge",false,path_);
    unlimitedPotionsEnabled=ReadBool(L"Features",L"UnlimitedPotions",false,path_);
    unlimitedHealthEnabled=ReadBool(L"Features",L"UnlimitedHealth",false,path_);
    unlimitedGlideEnabled=ReadBool(L"Features",L"UnlimitedGlide",false,path_);
    unlimitedHorseSprintEnabled=ReadBool(L"Features",L"UnlimitedHorseSprint",false,path_);
    potionRechargeMultiplier=std::clamp(ReadFloat(L"Values",L"PotionRechargeMultiplier",2.0f,path_),0.0f,100.0f);
    ammoConsumptionDivisor=std::clamp(ReadFloat(L"Values",L"AmmoConsumptionDivisor",2.0f,path_),0.0f,10.0f);
    thirdPersonEnabled =
        ReadBool(L"Features", L"ThirdPerson", true, path_);
    thirdPersonFovEnabled=ReadBool(L"Features",L"TPSFOV",true,path_);
    tpsLensFocusEnabled=ReadBool(L"Features",L"TPSLensFocus",true,path_);
    const bool oldOrbit=ReadBool(L"Features",L"CameraControllerOrbit",true,path_);
    tpsCameraStickMode=std::clamp(static_cast<int>(GetPrivateProfileIntW(
        L"Features",L"TPSCameraStickMode",oldOrbit?2:0,path_.c_str())),0,2);
    cameraControllerOrbitEnabled=tpsCameraStickMode!=0;
    tpsCutsceneAutoSuspend=ReadBool(L"Features",L"TPSAutoCutscene",true,path_);
    cameraOrbitInputEnabled=cameraControllerOrbitEnabled;
    tpsHoldSuspendEnabled=ReadBool(L"Features",L"TPSHoldSuspend",true,path_);
    tpsLongPressToggleEnabled=ReadBool(L"Features",L"TPSHoldToggle",true,path_);
    tpsGrenadeHoldEnabled=ReadBool(L"Features",L"TPSGrenadeHold",true,path_);
    aimIndicatorHideMode=std::clamp(static_cast<int>(GetPrivateProfileIntW(L"Features",L"AimIndicatorHideMode",2,path_.c_str())),0,2);
    tps3DAimEnabled=ReadBool(L"Features",L"TPS3DAim",true,path_);
    tpsAimPitchOffsetDegrees=std::clamp(ReadFloat(L"Values",L"TPSAimPitchOffsetDegrees",12.0f,path_),-45.0f,45.0f);
    tpsMountedAimMode=std::clamp(static_cast<int>(GetPrivateProfileIntW(L"Features",L"TPSMountedAimMode",0,path_.c_str())),0,1);
    tpsGrenadeButtonMask=static_cast<int>(GetPrivateProfileIntW(L"Controls",L"TPSGrenadeButtonMask",0x0080,path_.c_str()));
    if(!ValidPadBind(tpsGrenadeButtonMask))tpsGrenadeButtonMask=0x0080;
    tpsFirePrimaryMask=GetPrivateProfileIntW(L"Controls",L"TPSFirePrimaryMask",0x20000,path_.c_str());
    tpsFireSecondaryMask=GetPrivateProfileIntW(L"Controls",L"TPSFireSecondaryMask",0x0200,path_.c_str());
    const auto validMouse=[](int k){return k==0||k==VK_LBUTTON||k==VK_RBUTTON||k==VK_MBUTTON||k==VK_XBUTTON1||k==VK_XBUTTON2;};
    tpsMousePrimaryButton=GetPrivateProfileIntW(L"Controls",L"TPSMousePrimaryButton",VK_LBUTTON,path_.c_str());
    tpsMouseSecondaryButton=GetPrivateProfileIntW(L"Controls",L"TPSMouseSecondaryButton",VK_RBUTTON,path_.c_str());
    if(!validMouse(tpsMousePrimaryButton))tpsMousePrimaryButton=VK_LBUTTON;
    if(!validMouse(tpsMouseSecondaryButton))tpsMouseSecondaryButton=VK_RBUTTON;
    if(tpsMousePrimaryButton && tpsMousePrimaryButton==tpsMouseSecondaryButton)
        tpsMouseSecondaryButton=tpsMousePrimaryButton==VK_RBUTTON?VK_LBUTTON:VK_RBUTTON;
    tpsWheelPadMask=GetPrivateProfileIntW(L"Controls",L"TPSWheelPadMask",0x0100,path_.c_str());
    if(!ValidPadBind(tpsFirePrimaryMask))tpsFirePrimaryMask=0x20000;
    if(!ValidPadBind(tpsFireSecondaryMask))tpsFireSecondaryMask=0x0200;
    if(!ValidPadBind(tpsWheelPadMask))tpsWheelPadMask=0x0100;
    tpsSuspendButtonMask=GetPrivateProfileIntW(L"Controls",L"TPSSuspendButtonMask",0x0080,path_.c_str());
    tpsSuspendDelayMs=GetPrivateProfileIntW(L"Controls",L"TPSSuspendDelayMs",300,path_.c_str());
    tpsSuspendDelayMs=std::clamp(tpsSuspendDelayMs,0,3000);
    tpsToggleHoldButtonMask=GetPrivateProfileIntW(L"Controls",L"TPSToggleHoldButtonMask",0x0040,path_.c_str());
    tpsToggleHoldMs=GetPrivateProfileIntW(L"Controls",L"TPSToggleHoldMs",500,path_.c_str());
    tpsToggleHoldMs=std::clamp(tpsToggleHoldMs,250,2000);
    if(!ValidPadBind(tpsSuspendButtonMask))tpsSuspendButtonMask=0x0080;
    if(!ValidPadBind(tpsToggleHoldButtonMask))tpsToggleHoldButtonMask=0x0040;
    tpsRecenterButtonMask=GetPrivateProfileIntW(L"Controls",L"TPSRecenterButtonMask",0x0040,path_.c_str());
    if(!ValidPadBind(tpsRecenterButtonMask))tpsRecenterButtonMask=0x0040;
    horsePadButtonA=GetPrivateProfileIntW(L"Controls",L"HorsePadButtonA",0x0100,path_.c_str());
    horsePadButtonB=GetPrivateProfileIntW(L"Controls",L"HorsePadButtonB",0x0200,path_.c_str());
    if(!ValidPadBind(horsePadButtonA) || !horsePadButtonA)horsePadButtonA=0x0100;
    if(!ValidPadBind(horsePadButtonB) || !horsePadButtonB)horsePadButtonB=0x0200;
    if(horsePadButtonA==horsePadButtonB)horsePadButtonB=horsePadButtonA==0x0200?0x0100:0x0200;
    pistolDamageEnabled =
        ReadBool(L"Features", L"PistolDamage", true, path_);
    meleeDamageEnabled =
        ReadBool(L"Features", L"MeleeDamage", true, path_);
    jumpHeightEnabled =
        ReadBool(L"Features", L"JumpHeight", true, path_);
    glideDurationEnabled =
        ReadBool(L"Features", L"GlideDuration", true, path_);
    horseEverywhereEnabled = ReadBool(L"Features", L"HorseEverywhere", true, path_);
    horseAreaMode=std::clamp(static_cast<int>(GetPrivateProfileIntW(
        L"Features",L"HorseAreaMode",horseEverywhereEnabled?2:0,path_.c_str())),0,3);
    horseEverywhereEnabled=horseAreaMode!=0;
    horseSpeedEnabled =
        ReadBool(L"Features", L"HorseSpeed", true, path_);
    horseSprintSpeedEnabled =
        ReadBool(L"Features", L"HorseSprintSpeed", true, path_);
    horseSprintDurationEnabled =
        ReadBool(L"Features", L"HorseSprintDuration", true, path_);
    fovEnabled =
        ReadBool(L"Features", L"FOV", false, path_);
    hotstreakChargeEnabled =
        ReadBool(L"Features", L"HotstreakCharge", true, path_);
    // Resource gains use the verified native local-player paths.
    wrathChargeEnabled=ReadBool(L"Features",L"WrathCharge",true,path_);
    ultimateChargeEnabled=ReadBool(L"Features",L"UltimateFormCharge",true,path_);
    synergyChargeEnabled=ReadBool(L"Features",L"SynergyCharge",true,path_);

    movementSpeedMultiplier =
        ReadFloat(L"Values", L"MovementSpeedMultiplier", 1.50f, path_);
    pistolDamageMultiplier =
        ReadFloat(L"Values", L"PistolDamageMultiplier", 2.0f, path_);
    meleeDamageMultiplier =
        ReadFloat(L"Values", L"MeleeDamageMultiplier", 2.0f, path_);
    jumpHeightMultiplier =
        ReadFloat(L"Values", L"JumpHeightMultiplier", 1.25f, path_);
    glideDurationMultiplier =
        ReadFloat(L"Values", L"GlideDurationMultiplier", 10.0f, path_);
    horseSpeedMultiplier =
        ReadFloat(L"Values", L"HorseSpeedMultiplier", 1.25f, path_);
    horseSprintSpeedMultiplier =
        ReadFloat(L"Values", L"HorseSprintSpeedMultiplier", 1.25f, path_);
    horseSprintDurationMultiplier =
        ReadFloat(L"Values", L"HorseSprintDurationMultiplier", 5.0f, path_);
    fovDegrees =
        ReadFloat(L"Values", L"FOVDegrees", 90.0f, path_);
    thirdPersonFovDegrees=ReadFloat(L"Values",L"TPSFOVDegrees",110.0f,path_);
    if(!std::isfinite(thirdPersonFovDegrees))thirdPersonFovDegrees=110.0f; // Match TPS canonical default.
    thirdPersonFovDegrees=std::clamp(thirdPersonFovDegrees,40.0f,140.0f);
    // Preserve the personal FOV when an existing INI has no separate coop value.
    thirdPersonCoopFovDegrees=ReadFloat(L"Values",L"TPSCoopFOVDegrees",thirdPersonFovDegrees,path_);
    if(!std::isfinite(thirdPersonCoopFovDegrees))thirdPersonCoopFovDegrees=thirdPersonFovDegrees;
    thirdPersonCoopFovDegrees=std::clamp(thirdPersonCoopFovDegrees,40.0f,140.0f);
    cameraZoomPercent =
        ReadFloat(L"Values", L"CameraZoomPercent", 0.0f, path_);
    cameraPitchDegrees =
        ReadFloat(L"Values", L"CameraPitchDegrees", 0.0f, path_);
    cameraYawDegrees =
        ReadFloat(L"Values", L"CameraYawDegrees", 0.0f, path_);
    cameraHeightOffset =
        ReadFloat(L"Values", L"CameraHeightOffset", 0.0f, path_);
    thirdPersonDistanceMultiplier =
        ReadFloat(L"Values", L"ThirdPersonDistanceMultiplier", 0.15f, path_);
    thirdPersonPitchDegrees = ReadFloat(L"Values", L"ThirdPersonPitchDegrees", -12.0f, path_);
    thirdPersonHeightOffset = ReadFloat(L"Values", L"ThirdPersonHeightOffset", 0.0f, path_);
    tpsFootAnchorOffset=ReadFloat(L"Values",L"TPSFootAnchorOffset",0.0f,path_);
    if(!std::isfinite(tpsFootAnchorOffset))tpsFootAnchorOffset=0.0f;
    tpsFootAnchorOffset=std::clamp(tpsFootAnchorOffset,-500.0f,500.0f);
    cameraStickSpeed = ReadFloat(L"Values", L"CameraStickSpeed", 180.0f, path_);
    if (!std::isfinite(thirdPersonDistanceMultiplier)) thirdPersonDistanceMultiplier=0.15f;
    if (!std::isfinite(thirdPersonPitchDegrees)) thirdPersonPitchDegrees=-12.0f;
    if (!std::isfinite(thirdPersonHeightOffset)) thirdPersonHeightOffset=0.0f;
    if (!std::isfinite(cameraStickSpeed)) cameraStickSpeed=180.0f;
    thirdPersonDistanceMultiplier=std::clamp(thirdPersonDistanceMultiplier,0.0f,3.0f);
    thirdPersonPitchDegrees=std::clamp(thirdPersonPitchDegrees,-75.0f,65.0f);
    thirdPersonHeightOffset=std::clamp(thirdPersonHeightOffset,-1000.0f,1000.0f);
    // Missing new keys inherit the player's current camera, preserving old INIs.
    tpsUltimateDistanceMultiplier=std::clamp(ReadFloat(L"Values",L"TPSUltimateDistanceMultiplier",
        thirdPersonDistanceMultiplier,path_),0.0f,3.0f);
    tpsUltimateHeightOffset=std::clamp(ReadFloat(L"Values",L"TPSUltimateHeightOffset",
        thirdPersonHeightOffset,path_),-1000.0f,1000.0f);
    // Existing profiles start mounted framing at their current normal camera.
    tpsHorseDistanceMultiplier=std::clamp(ReadFloat(L"Values",L"TPSHorseDistanceMultiplier",
        thirdPersonDistanceMultiplier,path_),0.0f,3.0f);
    tpsHorseHeightOffset=std::clamp(ReadFloat(L"Values",L"TPSHorseHeightOffset",
        thirdPersonHeightOffset,path_),-1000.0f,1000.0f);
    cameraStickSpeed=std::clamp(cameraStickSpeed,30.0f,360.0f);
    hotstreakChargeMultiplier =
        ReadFloat(L"Values", L"HotstreakChargeMultiplier", 2.0f, path_);
    if(!std::isfinite(hotstreakChargeMultiplier))hotstreakChargeMultiplier=2.0f;
    hotstreakChargeMultiplier=std::clamp(hotstreakChargeMultiplier,0.0f,100.0f);
    wrathChargeMultiplier=std::clamp(ReadFloat(L"Values",L"WrathChargeMultiplier",defaults::WrathGain,path_),0.0f,100.0f);
    ultimateChargeMultiplier=std::clamp(ReadFloat(L"Values",L"UltimateFormChargeMultiplier",defaults::UltimateGain,path_),0.0f,100.0f);
    synergyChargeMultiplier=std::clamp(ReadFloat(L"Values",L"SynergyChargeMultiplier",defaults::SynergyGain,path_),0.0f,100.0f);

    for (int i = 0; i < 12; ++i) {
        wchar_t key[8]{};
        swprintf_s(key, L"F%d", i + 1);

        wchar_t value[64]{};
        GetPrivateProfileStringW(
            L"Hotkeys",
            key,
            ActionToken(hotkeys[static_cast<std::size_t>(i)]),
            value,
            64,
            path_.c_str()
        );

        hotkeys[static_cast<std::size_t>(i)] =
            ParseAction(value);
    }

    for (size_t i = 0; i < cameraKeys.size(); ++i) {
        wchar_t keyText[64]{};
        const std::wstring fallback = KeyTokenFromVK(cameraKeys[i]);
        GetPrivateProfileStringW(
            L"CameraHotkeys", kCameraTokens[i], fallback.c_str(),
            keyText, 64, path_.c_str());
        const int parsed = ParseKeyToken(keyText, cameraKeys[i]);
        cameraKeys[i] = (parsed >= 0 && parsed < 256) ? parsed : 0;
    }

    for(size_t i=0;i<tpsKeyboardKeys.size();++i){
        wchar_t value[64]{};
        const std::wstring fallback=KeyTokenFromVK(tpsKeyboardKeys[i]);
        GetPrivateProfileStringW(L"TPSKeyboardHotkeys",kTpsKeyboardTokens[i],
            fallback.c_str(),value,64,path_.c_str());
        const int vk=ParseKeyToken(value,tpsKeyboardKeys[i]);
        tpsKeyboardKeys[i]=(vk>=0&&vk<256)?vk:0;
    }
    const int revision = GetPrivateProfileIntW(
        L"Meta",
        L"ConfigRevision",
        0,
        path_.c_str()
    );

    if (revision < 2101) {
        // Preserve the historical one-time intro/horse migration only.
        skipIntroEnabled = true;
        horseSprintSpeedEnabled = true;
        skipLogosEnabled = false;
        SaveNow();
        Log("INI migrated legacy features; unsafe Skip Logos bypass remains OFF");
        return true;
    }
    if (revision < 2102) {
        // Historical unsafe-bypass migration: never auto-enable the old
        // implementation on legacy settings that predate the safe system.
        skipLogosEnabled = false;
        SaveNow();
        Log("INI migration V0.43: reset legacy StartupScreens bypass OFF; other options preserved");
        return true;
    }
    if (revision < kConfigRevision) {
        // Previous shipped Vanilla Camera Height=50; native/default is 0.
        // Upgrade ONLY the identical old default, preserve other custom values.
        if(revision<2138 && std::fabs(cameraHeightOffset-50.0f)<0.001f){
            cameraHeightOffset=0.0f;
            Log("V1.05 migrated old Camera Height 50 to vanilla default 0");
        }
        // V1.04 shipped all three OFF. Migrate only that unmodified preset once.
        // Independent user-customized gauge selections are preserved.
        if(revision<2138 && !wrathChargeEnabled &&
           !ultimateChargeEnabled && !synergyChargeEnabled){
            wrathChargeEnabled=true;ultimateChargeEnabled=true;synergyChargeEnabled=true;
            Log("V1.05 three-gauge preset migrated all OFF -> all ON");
        }
        // Only repair the exact old CameraReset tuple, preserve other choices.
        if(revision<2137 &&
           std::fabs(thirdPersonDistanceMultiplier-0.50f)<0.0001f &&
           std::fabs(thirdPersonHeightOffset-180.0f)<0.001f &&
           std::fabs(tpsFootAnchorOffset-88.0f)<0.001f){
            thirdPersonDistanceMultiplier=0.15f;
            thirdPersonHeightOffset=0.0f;
            tpsFootAnchorOffset=0.0f;
            Log("V1.04 repaired obsolete CameraReset triple, preserved all other values");
        }
        // V0.95 requested shared Z/Z instead of the previous V/V pair.
        // Migrate ONLY that unchanged pair, respecting every other
        // independent user-selected keyboard binding.
        if(revision<2129 &&
           tpsKeyboardKeys[static_cast<size_t>(TpsKeyboardAction::Toggle)]=='V' &&
           tpsKeyboardKeys[static_cast<size_t>(TpsKeyboardAction::Recenter)]=='V'){
            tpsKeyboardKeys[static_cast<size_t>(TpsKeyboardAction::Toggle)]='Z';
            tpsKeyboardKeys[static_cast<size_t>(TpsKeyboardAction::Recenter)]='Z';
            Log("V0.95 migrated prior shared V/V default to Z/Z");
        }
        // V0.88 requested default ON. For older INIs the old explicit
        // 0 was the previous experimental default: migrate once to ON;
        // user can switch OFF after migration and it will remain OFF.
        if(revision<2127)horseEverywhereEnabled=true;
        // V0.86 migration: preserve the user's previously disabled actions
        // as OFF bindings, and respect all existing active bindings.
        if(revision<2126){
            if(!tpsHoldSuspendEnabled)tpsSuspendButtonMask=0;
            if(!tpsLongPressToggleEnabled)tpsToggleHoldButtonMask=0;
            if(!tpsGrenadeHoldEnabled)tpsGrenadeButtonMask=0;
        }
        // V0.55: F5 becomes Third Person when the previous default was None.
        // Explicit non-None assignments elsewhere remain untouched.
        if (revision == 2104 && hotkeys[4] == Action::None)
            hotkeys[4] = Action::ThirdPerson;
        // Restore normal idle movement and migrate only old unchanged
        // camera pose defaults, preserving any user-customized values.
        if(revision<=2110){
            if(std::fabs(thirdPersonDistanceMultiplier-1.0f)<0.0001f)
                thirdPersonDistanceMultiplier=0.50f;
            if(std::fabs(thirdPersonHeightOffset-60.0f)<0.0001f)
                thirdPersonHeightOffset=180.0f;
        }
        if(revision<=2112 && !cameraControllerOrbitEnabled){
            cameraControllerOrbitEnabled=true;
        }
        cameraOrbitInputEnabled=cameraControllerOrbitEnabled;
        // V0.74: obsolete always-on strafe switches are retired.
        // New firing-only settings remain opt-in and default to OFF.
        PublishRuntime();
        SaveNow();
        Log("INI migrated V0.76: new hold controls ON; V0.75 gameplay settings retained");
        return true;
    }

    PublishRuntime();
    dirty_.store(false, std::memory_order_relaxed);
    Log("INI loaded");
    return true;
}

void Store::PublishRuntime() const {
    dg::runtime::Snapshot runtime{};
    runtime.overlayMenuKey=menuKey;
    runtime.toggleHudEnabled = toggleHudEnabled;
    runtime.interactionHudScaleEnabled=interactionHudScaleEnabled;
    runtime.interactionHudScale=interactionHudScale;
    runtime.weaponSheathingEnabled=weaponSheathingEnabled;
    runtime.sheathNeverEnabled=sheathNeverEnabled;
    runtime.sheathDelaySeconds=sheathDelaySeconds;
    runtime.fovEnabled = fovEnabled;
    runtime.movementSpeedEnabled = movementSpeedEnabled;
    runtime.skipLogosEnabled = skipLogosEnabled;
    runtime.skipIntroEnabled = skipIntroEnabled;
    runtime.skipWarningEnabled = skipWarningEnabled;
    runtime.thirdPersonEnabled = thirdPersonEnabled;
    runtime.thirdPersonFovEnabled = thirdPersonFovEnabled;
    runtime.tpsLensFocusEnabled = tpsLensFocusEnabled;
    // Mouse TPS orbit shares the existing orbit result; it does not need
    // a controller stick enabled. In native view no TPS orbit is applied.
    runtime.cameraOrbitInputEnabled = thirdPersonEnabled;
    runtime.cameraControllerOrbitEnabled = cameraControllerOrbitEnabled;
    runtime.tpsCameraStickMode = tpsCameraStickMode;
    runtime.tpsMousePrimaryButton = tpsMousePrimaryButton;
    runtime.tpsMouseSecondaryButton = tpsMouseSecondaryButton;
    runtime.tpsFirePrimaryMask = tpsFirePrimaryMask;
    runtime.tpsFireSecondaryMask = tpsFireSecondaryMask;
    runtime.tpsWheelPadMask = tpsWheelPadMask;
    runtime.tpsGrenadeButtonMask = tpsGrenadeButtonMask;
    runtime.horsePadButtonA = horsePadButtonA;
    runtime.horsePadButtonB = horsePadButtonB;

    runtime.tpsCutsceneAutoSuspend = tpsCutsceneAutoSuspend;
    runtime.aimIndicatorHideMode = aimIndicatorHideMode;
    runtime.tps3DAimEnabled = tps3DAimEnabled;
    runtime.tpsAimPitchOffsetDegrees = tpsAimPitchOffsetDegrees;
    runtime.tpsMountedAimMode = tpsMountedAimMode;
    runtime.thirdPersonDistanceMultiplier = thirdPersonDistanceMultiplier;
    runtime.thirdPersonPitchDegrees = thirdPersonPitchDegrees;
    runtime.thirdPersonHeightOffset = thirdPersonHeightOffset;
    runtime.tpsUltimateDistanceMultiplier = tpsUltimateDistanceMultiplier;
    runtime.tpsUltimateHeightOffset = tpsUltimateHeightOffset;
    runtime.tpsHorseDistanceMultiplier = tpsHorseDistanceMultiplier;
    runtime.tpsHorseHeightOffset = tpsHorseHeightOffset;
    runtime.tpsFootAnchorOffset = tpsFootAnchorOffset;
    runtime.cameraStickSpeed = cameraStickSpeed;
    runtime.pistolDamageEnabled = pistolDamageEnabled;
    runtime.meleeDamageEnabled = meleeDamageEnabled;
    runtime.jumpHeightEnabled = jumpHeightEnabled;
    runtime.glideDurationEnabled = glideDurationEnabled;
    runtime.horseEverywhereEnabled = horseEverywhereEnabled;
    runtime.horseAreaMode = horseAreaMode;
    runtime.horseSpeedEnabled = horseSpeedEnabled;
    runtime.horseSprintSpeedEnabled = horseSprintSpeedEnabled;
    runtime.horseSprintDurationEnabled = horseSprintDurationEnabled;
    runtime.hotstreakChargeEnabled = hotstreakChargeEnabled;
    runtime.wrathChargeEnabled = wrathChargeEnabled;
    runtime.ultimateChargeEnabled = ultimateChargeEnabled;
    runtime.synergyChargeEnabled = synergyChargeEnabled;
    runtime.unlimitedWrathEnabled = unlimitedWrathEnabled;
    runtime.unlimitedUltimateEnabled = unlimitedUltimateEnabled;
    runtime.unlimitedSynergyEnabled = unlimitedSynergyEnabled;
    runtime.finiteStandardAmmoEnabled = finiteStandardAmmoEnabled;
    runtime.standardAmmoCapacity = standardAmmoCapacity;
    runtime.grappleRangeEnabled = grappleRangeEnabled;
    runtime.grappleRangeMultiplier = grappleRangeMultiplier;
    runtime.infiniteDoubleJumpMode = infiniteDoubleJumpMode;
    runtime.reduceAmmoEnabled = reduceAmmoEnabled;
    runtime.unlimitedAmmoEnabled = unlimitedAmmoEnabled;
    runtime.ammoConsumptionDivisor = ammoConsumptionDivisor;
    runtime.reduceHotstreakEnabled = reduceHotstreakEnabled;
    runtime.unlimitedHotstreakEnabled = unlimitedHotstreakEnabled;
    runtime.hotstreakDecreaseDivisor = hotstreakDecreaseDivisor;
    runtime.potionRechargeEnabled = potionRechargeEnabled;
    runtime.unlimitedPotionsEnabled = unlimitedPotionsEnabled;
    runtime.unlimitedHealthEnabled = unlimitedHealthEnabled;
    runtime.unlimitedGlideEnabled = unlimitedGlideEnabled;
    runtime.unlimitedHorseSprintEnabled = unlimitedHorseSprintEnabled;
    runtime.potionRechargeMultiplier = potionRechargeMultiplier;

    runtime.fovDegrees = fovDegrees;
    runtime.thirdPersonFovDegrees = thirdPersonFovDegrees;
    runtime.thirdPersonCoopFovDegrees = thirdPersonCoopFovDegrees;
    runtime.cameraZoomPercent = cameraZoomPercent;
    runtime.cameraPitchDegrees = cameraPitchDegrees;
    runtime.cameraYawDegrees = cameraYawDegrees;
    runtime.cameraHeightOffset = cameraHeightOffset;
    runtime.movementSpeedMultiplier = movementSpeedMultiplier;
    runtime.pistolDamageMultiplier = pistolDamageMultiplier;
    runtime.meleeDamageMultiplier = meleeDamageMultiplier;
    runtime.jumpHeightMultiplier = jumpHeightMultiplier;
    runtime.glideDurationMultiplier = glideDurationMultiplier;
    runtime.horseSpeedMultiplier = horseSpeedMultiplier;
    runtime.horseSprintSpeedMultiplier = horseSprintSpeedMultiplier;
    runtime.horseSprintDurationMultiplier = horseSprintDurationMultiplier;
    runtime.hotstreakChargeMultiplier = hotstreakChargeMultiplier;
    runtime.wrathChargeMultiplier = wrathChargeMultiplier;
    runtime.ultimateChargeMultiplier = ultimateChargeMultiplier;
    runtime.synergyChargeMultiplier = synergyChargeMultiplier;

    dg::runtime::Publish(runtime);
}

void Store::MarkDirty() {
    dirty_.store(true, std::memory_order_relaxed);
    dirtyTick_.store(GetTickCount64(), std::memory_order_relaxed);
}

void Store::Save() {
    PublishRuntime();
    MarkDirty();
}

bool Store::SaveNow() {
    if (path_.empty()) {
        return false;
    }

    ProfileWriteScope writes;
    PublishRuntime();

    wchar_t revisionText[16]{};
    swprintf_s(revisionText,L"%d",kConfigRevision);
    WriteProfileString(
        L"Meta",
        L"ConfigRevision",
        revisionText,
        path_.c_str()
    );

    WriteBool(L"Overlay", L"Enabled", overlayEnabled, path_);
    WriteBool(L"Updater", L"AutoCheckUpdates", updaterAutoCheck, path_);
    WriteBool(L"Updater", L"Beta", updaterBeta, path_);
    const std::wstring menuKeyToken = KeyTokenFromVK(menuKey);
    WriteProfileString(
        L"Overlay",
        L"MenuKey",
        menuKeyToken.c_str(),
        path_.c_str()
    );

    wchar_t graphicsAdapterText[8]{};
    swprintf_s(
        graphicsAdapterText,
        L"%d",
        graphicsAdapter
    );
    WriteProfileString(
        L"System",
        L"GraphicsAdapter",
        graphicsAdapterText,
        path_.c_str()
    );

    WriteProfileString(L"Features", L"ToggleHUD", nullptr, path_.c_str());
    WriteBool(L"Features",L"WeaponSheathing",weaponSheathingEnabled,path_);
    WriteBool(L"Features",L"NeverAutoSheathe",sheathNeverEnabled,path_);
    WriteProfileString(L"Controls",L"SheatheKeyboardKey",KeyTokenFromVK(sheathKeyboardKey).c_str(),path_.c_str());
    wchar_t sheathPadText[32]{};
    swprintf_s(sheathPadText,L"%d",sheathPadButtonA);
    WriteProfileString(L"Controls",L"SheathePadButtonA",sheathPadText,path_.c_str());
    swprintf_s(sheathPadText,L"%d",sheathPadButtonB);
    WriteProfileString(L"Controls",L"SheathePadButtonB",sheathPadText,path_.c_str());
    WriteFloat(L"Values",L"SheathDelaySeconds",sheathDelaySeconds,path_);
    WriteBool(L"Features",L"InteractionHudScale",interactionHudScaleEnabled,path_);
    WriteFloat(L"Values",L"InteractionHudScale",interactionHudScale,path_);
    WriteBool(L"Features", L"MovementSpeed", movementSpeedEnabled, path_);
    WriteBool(L"Features", L"SkipLogos", skipLogosEnabled, path_);
    WriteBool(L"Features", L"SkipIntroVideos", skipIntroEnabled, path_);
    WriteBool(L"Features", L"SkipWarning", skipWarningEnabled, path_);
    WriteBool(L"Features",L"UnlimitedWrath",unlimitedWrathEnabled,path_);
    WriteBool(L"Features",L"UnlimitedUltimate",unlimitedUltimateEnabled,path_);
    WriteBool(L"Features",L"UnlimitedSynergy",unlimitedSynergyEnabled,path_);
    WriteBool(L"Features",L"FiniteStandardAmmo",finiteStandardAmmoEnabled,path_);
    WriteProfileString(L"Values",L"StandardAmmoCapacity",std::to_wstring(standardAmmoCapacity).c_str(),path_.c_str());
    WriteBool(L"Features",L"GrappleRange",grappleRangeEnabled,path_);
    WriteFloat(L"Values",L"GrappleRangeMultiplier",grappleRangeMultiplier,path_);
    WriteProfileString(L"Values",L"InfiniteDoubleJumpMode",std::to_wstring(infiniteDoubleJumpMode).c_str(),path_.c_str());
    WriteBool(L"Features",L"ReduceAmmoConsumption",reduceAmmoEnabled,path_);
    WriteBool(L"Features",L"UnlimitedAmmo",unlimitedAmmoEnabled,path_);
    WriteBool(L"Features",L"ReduceHotstreakDecrease",reduceHotstreakEnabled,path_);
    WriteBool(L"Features",L"UnlimitedHotstreak",unlimitedHotstreakEnabled,path_);
    WriteFloat(L"Values",L"HotstreakDecreaseDivisor",hotstreakDecreaseDivisor,path_);
    WriteBool(L"Features",L"PotionRecharge",potionRechargeEnabled,path_);
    WriteBool(L"Features",L"UnlimitedPotions",unlimitedPotionsEnabled,path_);
    WriteBool(L"Features",L"UnlimitedHealth",unlimitedHealthEnabled,path_);
    WriteBool(L"Features",L"UnlimitedGlide",unlimitedGlideEnabled,path_);
    WriteBool(L"Features",L"UnlimitedHorseSprint",unlimitedHorseSprintEnabled,path_);
    WriteFloat(L"Values",L"PotionRechargeMultiplier",potionRechargeMultiplier,path_);
    WriteFloat(L"Values",L"AmmoConsumptionDivisor",ammoConsumptionDivisor,path_);
    WriteBool(L"Features", L"ThirdPerson", thirdPersonEnabled, path_);
    WriteBool(L"Features",L"TPSFOV",thirdPersonFovEnabled,path_);
    WriteBool(L"Features",L"TPSLensFocus",tpsLensFocusEnabled,path_);
    WriteProfileString(L"Features",L"TPSFollowPlayer",nullptr,path_.c_str());
    // Remove obsolete TPS switches from existing user INIs once migrated.
    for(const wchar_t* key : { L"TPSFixedCenterReticle",L"TPSLockZoneCamera",
        L"TPSControllerCombatAim",L"TPSLockCombatFacing",L"TPSStrafeLock",
        L"TPSCameraFacingGuard",L"TPSLockCombatCameraPivot" }) {
        WriteProfileString(L"Features",key,nullptr,path_.c_str());
    }
    // V0.69: remove the broken setting from old INIs. Blocking mouse
    // messages freezes the game's world-plane aim target.
    WriteProfileString(L"Features",L"TPSSuppressVanillaMouseAim",nullptr,path_.c_str());
    // V0.70: persist independent orbit controls, remove old combined key.
    WriteProfileString(L"Features",L"CameraMouseGamepad",nullptr,path_.c_str());
    // Remove stale mouse-specific INI configuration entirely.
    WriteProfileString(L"Features",L"CameraMouseOrbit",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSMouseCenterAim",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSMouseForwardFire",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSMouseStickMode",nullptr,path_.c_str());
    WriteBool(L"Features",L"CameraControllerOrbit",tpsCameraStickMode!=0,path_);
    wchar_t stickBuffer[8]{};
    swprintf_s(stickBuffer,L"%d",tpsCameraStickMode);
    WriteProfileString(L"Features",L"TPSCameraStickMode",stickBuffer,path_.c_str());
    WriteBool(L"Features",L"TPSAutoCutscene",tpsCutsceneAutoSuspend,path_);
    // Remove the V0.71/0.73 ineffective switches, no mapping of old values.
    WriteProfileString(L"Features",L"TPSMouseKeyboardStrafe",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSControllerStrafe",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSMouseStrafeWhileFiring",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSMouseRotateWhileFiring",nullptr,path_.c_str());
    // V0.94: these were mutually confusing user toggles. TPS handles both
    // natively according to selected aiming mode and radial menu state.
    WriteProfileString(L"Features",L"TPSSuppressNativeRightStick",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSControllerStrafeWhileFiring",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"TPSControllerRotateWhileFiring",nullptr,path_.c_str());
    WriteBool(L"Features",L"TPSHoldSuspend",tpsSuspendButtonMask!=0,path_);
    WriteBool(L"Features",L"TPSHoldToggle",tpsToggleHoldButtonMask!=0,path_);
    WriteBool(L"Features",L"TPSGrenadeHold",tpsGrenadeButtonMask!=0,path_);
    wchar_t aimModeBuffer[12]{};
    swprintf_s(aimModeBuffer,L"%d",std::clamp(aimIndicatorHideMode,0,2));
    WriteProfileString(L"Features",L"AimIndicatorHideMode",aimModeBuffer,path_.c_str());
    WriteBool(L"Features",L"TPS3DAim",tps3DAimEnabled,path_);
    WriteFloat(L"Values",L"TPSAimPitchOffsetDegrees",tpsAimPitchOffsetDegrees,path_);
    wchar_t mountModeText[12]{};
    swprintf_s(mountModeText,L"%d",std::clamp(tpsMountedAimMode,0,1));
    WriteProfileString(L"Features",L"TPSMountedAimMode",mountModeText,path_.c_str());
    wchar_t buttonBuffer[16]{};
    swprintf_s(buttonBuffer,L"%d",tpsGrenadeButtonMask);
    WriteProfileString(L"Controls",L"TPSGrenadeButtonMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsFirePrimaryMask);
    WriteProfileString(L"Controls",L"TPSFirePrimaryMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsFireSecondaryMask);
    WriteProfileString(L"Controls",L"TPSFireSecondaryMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsMousePrimaryButton);
    WriteProfileString(L"Controls",L"TPSMousePrimaryButton",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsMouseSecondaryButton);
    WriteProfileString(L"Controls",L"TPSMouseSecondaryButton",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsWheelPadMask);
    WriteProfileString(L"Controls",L"TPSWheelPadMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsRecenterButtonMask);
    WriteProfileString(L"Controls",L"TPSRecenterButtonMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",horsePadButtonA);
    WriteProfileString(L"Controls",L"HorsePadButtonA",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",horsePadButtonB);
    WriteProfileString(L"Controls",L"HorsePadButtonB",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsSuspendButtonMask);
    WriteProfileString(L"Controls",L"TPSSuspendButtonMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",std::clamp(tpsSuspendDelayMs,0,3000));
    WriteProfileString(L"Controls",L"TPSSuspendDelayMs",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsToggleHoldButtonMask);
    WriteProfileString(L"Controls",L"TPSToggleHoldButtonMask",buttonBuffer,path_.c_str());
    swprintf_s(buttonBuffer,L"%d",tpsToggleHoldMs);
    WriteProfileString(L"Controls",L"TPSToggleHoldMs",buttonBuffer,path_.c_str());
    WriteProfileString(L"Features",L"TPSAlwaysFaceCamera",nullptr,path_.c_str());

    WriteBool(L"Features", L"PistolDamage", pistolDamageEnabled, path_);
    WriteBool(L"Features", L"MeleeDamage", meleeDamageEnabled, path_);
    WriteBool(L"Features", L"JumpHeight", jumpHeightEnabled, path_);
    WriteBool(L"Features", L"GlideDuration", glideDurationEnabled, path_);
    WriteBool(L"Features", L"HorseEverywhere", horseEverywhereEnabled, path_);
    WriteProfileString(L"Features",L"HorseAllowRestrictedSummon",nullptr,path_.c_str());
    WriteProfileString(L"Features",L"HorsePreventForcedDesummon",nullptr,path_.c_str());
    wchar_t horseMode[8]{}; swprintf_s(horseMode,L"%d",std::clamp(horseAreaMode,0,3));
    WriteProfileString(L"Features",L"HorseAreaMode",horseMode,path_.c_str());
    WriteBool(L"Features", L"HorseSpeed", horseSpeedEnabled, path_);
    WriteBool(L"Features", L"HorseSprintSpeed", horseSprintSpeedEnabled, path_);
    WriteBool(L"Features", L"HorseSprintDuration", horseSprintDurationEnabled, path_);
    WriteBool(L"Features", L"FOV", fovEnabled, path_);
    // V1.22: retire obsolete option on explicit save only.
    WriteProfileString(L"Features", L"HideReticle", nullptr, path_.c_str());
    // Obsolete TPS pointer toggle: now internal, menu-only policy.
    // Remove deprecated key only when user saves settings.
    WriteProfileString(L"Features",L"ShowTPSMousePointer",nullptr,path_.c_str());
    // Remove retired V0.40/V0.41 experiment keys on next save.
    // No automatic focus changes or cursor-source overrides remain.
    WriteProfileString(L"Features", L"CrossCursorTestMode", nullptr, path_.c_str());
    WriteProfileString(L"Features", L"AutoReticleRefreshOnLoad", nullptr, path_.c_str());
    WriteBool(L"Features", L"HotstreakCharge", hotstreakChargeEnabled, path_);
    WriteBool(L"Features",L"WrathCharge",wrathChargeEnabled,path_);
    WriteBool(L"Features",L"UltimateFormCharge",ultimateChargeEnabled,path_);
    WriteBool(L"Features",L"SynergyCharge",synergyChargeEnabled,path_);

    WriteFloat(L"Values", L"MovementSpeedMultiplier", movementSpeedMultiplier, path_);
    WriteFloat(L"Values", L"PistolDamageMultiplier", pistolDamageMultiplier, path_);
    WriteFloat(L"Values", L"MeleeDamageMultiplier", meleeDamageMultiplier, path_);
    WriteFloat(L"Values", L"JumpHeightMultiplier", jumpHeightMultiplier, path_);
    WriteFloat(L"Values", L"GlideDurationMultiplier", glideDurationMultiplier, path_);
    WriteFloat(L"Values", L"HorseSpeedMultiplier", horseSpeedMultiplier, path_);
    WriteFloat(L"Values", L"HorseSprintSpeedMultiplier", horseSprintSpeedMultiplier, path_);
    WriteFloat(L"Values", L"HorseSprintDurationMultiplier", horseSprintDurationMultiplier, path_);
    WriteFloat(L"Values", L"FOVDegrees", fovDegrees, path_);
    WriteFloat(L"Values",L"TPSFOVDegrees",thirdPersonFovDegrees,path_);
    WriteFloat(L"Values",L"TPSCoopFOVDegrees",thirdPersonCoopFovDegrees,path_);
    for(const wchar_t* key : { L"TPSCombatAimTurnRate",L"TPSAimYawOffsetDegrees",
        L"TPSFacingToleranceDegrees" })
        WriteProfileString(L"Values",key,nullptr,path_.c_str());
    WriteFloat(L"Values", L"CameraZoomPercent", cameraZoomPercent, path_);
    WriteFloat(L"Values", L"CameraPitchDegrees", cameraPitchDegrees, path_);
    WriteFloat(L"Values", L"CameraYawDegrees", cameraYawDegrees, path_);
    WriteFloat(L"Values", L"CameraHeightOffset", cameraHeightOffset, path_);
    WriteFloat(L"Values", L"ThirdPersonDistanceMultiplier", thirdPersonDistanceMultiplier, path_);
    WriteFloat(L"Values", L"ThirdPersonPitchDegrees", thirdPersonPitchDegrees, path_);
    WriteFloat(L"Values", L"ThirdPersonHeightOffset", thirdPersonHeightOffset, path_);
    WriteFloat(L"Values",L"TPSUltimateDistanceMultiplier",tpsUltimateDistanceMultiplier,path_);
    WriteFloat(L"Values",L"TPSUltimateHeightOffset",tpsUltimateHeightOffset,path_);
    WriteFloat(L"Values",L"TPSHorseDistanceMultiplier",tpsHorseDistanceMultiplier,path_);
    WriteFloat(L"Values",L"TPSHorseHeightOffset",tpsHorseHeightOffset,path_);
    WriteFloat(L"Values",L"TPSFootAnchorOffset",tpsFootAnchorOffset,path_);
    WriteFloat(L"Values", L"CameraStickSpeed", cameraStickSpeed, path_);
    WriteFloat(L"Values", L"HotstreakChargeMultiplier", hotstreakChargeMultiplier, path_);
    WriteProfileString(L"Values",L"CameraMouseSensitivity",nullptr,path_.c_str());
    WriteFloat(L"Values",L"WrathChargeMultiplier",wrathChargeMultiplier,path_);
    WriteFloat(L"Values",L"UltimateFormChargeMultiplier",ultimateChargeMultiplier,path_);
    WriteFloat(L"Values",L"SynergyChargeMultiplier",synergyChargeMultiplier,path_);

    for (int i = 0; i < 12; ++i) {
        wchar_t key[8]{};
        swprintf_s(key, L"F%d", i + 1);
        WriteProfileString(
            L"Hotkeys",
            key,
            ActionToken(hotkeys[static_cast<std::size_t>(i)]),
            path_.c_str()
        );
    }

    // Native E already selects gear; the keyboard grenade duplicate is retired.
    WriteProfileString(L"TPSKeyboardHotkeys",L"Grenade",nullptr,path_.c_str());
    WriteProfileString(L"TPSKeyboardHotkeys",L"RadialE",nullptr,path_.c_str());
    WriteProfileString(L"TPSKeyboardHotkeys",L"RadialAlt",nullptr,path_.c_str());
    for(size_t i=0;i<tpsKeyboardKeys.size();++i){
        const std::wstring token=KeyTokenFromVK(tpsKeyboardKeys[i]);
        WriteProfileString(L"TPSKeyboardHotkeys",kTpsKeyboardTokens[i],
            token.c_str(),path_.c_str());
    }
    for (size_t i = 0; i < cameraKeys.size(); ++i) {
        const std::wstring value = KeyTokenFromVK(cameraKeys[i]);
        WriteProfileString(
            L"CameraHotkeys", kCameraTokens[i], value.c_str(), path_.c_str());
    }

    if(!writes.status.ok){
        MarkDirty(); // Retry after the normal quiet period; runtime values stay published.
        if(!saveFailureLogged_){
            char message[160]{};
            sprintf_s(message,"INI save failed (Windows error %lu); changes remain pending",writes.status.error);
            Log(message);
            saveFailureLogged_=true;
        }
        return false;
    }
    saveFailureLogged_=false;
    dirty_.store(false, std::memory_order_relaxed);
    Log("INI saved");
    return true;
}

void Store::FlushIfDue(bool force) {
    if (!dirty_.load(std::memory_order_relaxed)) {
        return;
    }

    const ULONGLONG now = GetTickCount64();
    const ULONGLONG dirtyAt =
        dirtyTick_.load(std::memory_order_relaxed);

    if (!force && now - dirtyAt < 500) {
        return;
    }

    SaveNow();
}

bool Store::IsDirty() const {
    return dirty_.load(std::memory_order_relaxed);
}

} // namespace dg::config
