# Darksiders Genesis Enhanced PC Mod

Modern runtime enhancements for **Darksiders Genesis (PC)** using a local x64
DXGI proxy and ASI plugin.

The project is deliberately fail-open for normal runtime failures and
**fail-closed for unsupported game executables**.

## Current development state

- Last published stable release on `main`: **V0.29**
- Safety hotfix **V0.31** and lazy overlay **V0.32** retained
- Latest failed cursor experiment: **V0.33**
- Current development candidate: **V0.34 Reticle Focus Test**
- Target executable:

```text
DarksidersGenesis-Win64-Shipping.exe
Size: 62,113,280 bytes
SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

V0.17 calculates the executable SHA-256 at runtime. If the executable does not
match exactly, the overlay/log can still load but **gameplay hooks are not
installed**.

## Installation

Copy these files next to the game executable:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
```

Launch the game normally.

Default overlay key:

```text
Insert
```

The menu key and F1-F12 actions can be remapped from the overlay.

## Feature status

| Feature | Status |
|---|---|
| D3D11 / ImGui overlay | Stable |
| Rebindable menu key | Stable |
| Toggle HUD | Validated |
| Movement Speed | Validated; V0.17B restores the proven V0.14F local-pawn + Jump/Glide identity path |
| Action Recovery | Validated V0.8B tail-only policy |
| Jump Height | Validated |
| Glide / Flight Duration | Validated |
| Skip Intro | Validated |
| Hotstreak Charge | Validated |
| Pistol Damage | Functional heuristic: `BaseJuice > 0` |
| Melee Damage | Experimental heuristic: zero-juice outgoing records |
| Horse Speed | Validated V0.29 native movement offset correction; lifetime-safe V0.31 |
| Horse Sprint Duration | **Validated V0.27** - native HorseCharacter stamina drain; 0x vanilla, 5x default, 20x max |
| Horse Sprint Speed | Validated V0.29 native SprintingMaxSpeed; lifetime-safe V0.31 |
| FOV | Not implemented |
| Third Person camera | Not implemented |
| Skip Logos | **Validated V0.19C/V0.19D** - proprietary `StartupScreens` MoviePlayer attachment bypass |

Unimplemented controls are disabled in the V0.17 overlay/default configuration
instead of pretending to be active.

## Default hotkeys

```text
F1  Toggle HUD
F2  Movement Speed
F3  Action Recovery
F4  Skip Intro Videos
F5  Reticle Focus Test (V0.34 test package)
F6-F12  None
```

## V0.17 core architecture

The cleanup branch separates the runtime into explicit modules:

```text
dxgi_proxy.cpp
    local DXGI forwarding + ASI loading

DarksidersGenesisMod.cpp
    active hook orchestration + D3D11 renderer integration

ConfigStore.cpp
    INI + hotkeys + debounced persistence

RuntimeSettings.cpp
    atomic gameplay settings consumed by hook threads

TargetValidator.cpp
    exact executable size/SHA-256 gate

OverlayUi.cpp
    ImGui menu only, using an explicit context

HorseFeature.cpp
    isolated horse candidate logic
```

Historical/rejected hook families are no longer compiled into the runtime core.

## Horse reference

The authoritative horse research source is the user-supplied working reference
mod:

```text
ZZZ-Horse_P.pak
SHA-256: 3719ac840e1d0d7f137c9322580a3abe58cc5cf93d4b1ea97352c66490d3a920
```

Proven vanilla -> modified values include:

```text
MaxWalkSpeed                       1300 -> 1500
MaxAcceleration                     600 -> 700
BrakingFrictionFactor                 1 -> 2
GallopSpawnSpeedThreshold            300 -> 400

StaminaRecoveryPercentageRate         15 -> 100
StaminaTotalRecoveryPercentageRate    40 -> 100
StaminaSprintPercentageRate           25 -> 0
```

See `docs/REFERENCE_PAK_AUDIT.md` for the complete reference audit.

## Configuration behavior

Gameplay hook threads do not read the editable UI config directly.

V0.17 publishes settings into atomic runtime values immediately. INI persistence
is debounced, so dragging a slider no longer rewrites the entire file every
render frame.

The explicit **Save** button still persists immediately.

## Documentation

- `docs/FULL_AUDIT_V0.16A.md` - audit that triggered the V0.17 cleanup
- `docs/CORE_CLEANUP_V0.17.md` - V0.17 implementation / verification report
- `docs/REFERENCE_PAK_AUDIT.md` - supplied reference mod findings
- `docs/DODGE_RECOVERY_AUDIT.md` - recovery investigation
- `docs/TECHNICAL_NOTEBOOK.md` - complete historical development notebook

## Build

The project uses CMake and GitHub Actions.

MinHook and Dear ImGui are pinned to exact upstream commits for reproducible
builds.

The test artifact remains a flat package containing:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
README.md
```

## Release policy

Experimental branches are preserved as technical history.

`main` is not advanced merely because a candidate compiles. Gameplay changes
must be validated in game before promotion.


## V0.17B player-identity rollback fix

The first V0.17 cleanup introduced a new structural identity heuristic using
MaxWalkSpeed / MaxAcceleration. Runtime testing proved that heuristic rejected
the real player and therefore blocked Movement Speed, Jump, Glide and all
features depending on the captured local-player pointer.

V0.17B removes that unvalidated module completely and restores the path already
proven in V0.14F:

```text
APawn::IsLocallyControlled
  + valid JumpZ
  + valid DoubleJumpZ
  + valid GlideDuration
  = local player movement component
```

Only after that proven movement signature succeeds is
`g_localPlayerCharacter` updated.

The V0.17 cleanup work for target validation, atomic runtime settings,
ConfigStore, OverlayUi, debounced INI persistence, shutdown and dead-code
removal is retained.


## Working rule / fil rouge

From this point forward the project follows a deliberately simple rule:

1. Keep validated behavior unless a test proves it wrong.
2. Do not add a second heuristic when an already validated path exists.
3. A feature owns one clear primitive and one clear responsibility.
4. Diagnostics must be bounded and must not become fallback trees.
5. Rejected experiments are removed from runtime code and kept in Git/docs.
6. Change one gameplay subject at a time.
7. The cumulative README records every accepted direction change and rejection.

### V0.17B final player rollback policy

Player identity is restored exactly to the previously validated V0.14F rule:

```text
APawn::IsLocallyControlled(characterOwner)
    -> g_localPlayerCharacter = characterOwner
```

JumpZ / DoubleJumpZ / GlideDuration validation remains useful, but only protects
Jump/Glide property writes. It no longer gates Movement Speed, Action Recovery,
Damage or the local-player pointer.

This is intentionally simpler than the rejected V0.17 structural identity
module and the intermediate V0.17B combined filter.


## V0.18 Skip Logos - single-path test

Scope is intentionally limited to Skip Logos.

External verification confirms the two company startup movies are loose files:

```text
ProjectMayhem/Content/Movies/THQ_LogoBasic.mp4
ProjectMayhem/Content/Movies/AS_LogoBasic.mp4
```

The rejected V0.15 paths stay rejected:

```text
late CreateFile/GetFileAttributes hooks
PEB command-line -nostartupmovies injection
literal nostartupmovies query patch
```

V0.18 uses one new primitive only:

```text
dxgi.dll DllMain, before game entry
    -> patch the main EXE import table
    -> CreateFileW
    -> GetFileAttributesW
    -> GetFileAttributesExW
    -> return FILE_NOT_FOUND only for the two exact logo basenames
```

No game file is renamed, deleted or edited.

No story/cutscene movie is targeted. In particular
`CG_Intro_LowVi.mp4` is intentionally untouched.

This test is forced ON at boot because the hook must exist before UE4 startup.
Runtime configuration will only be added after the primitive is validated.

The ASI log reports both the early hook status and how many matching file
requests were blocked.


## V0.18B - Skip Logos / Skip Intro separation

The menu and configuration now reflect the actual startup order:

```text
Skip Logos
    -> company logos before the intro cinematic
    -> boot-only
    -> applies on next launch

Skip Intro Videos
    -> native g.PlayIntroCinematicOnBoot path
    -> intro cinematic only
```

They are separate options and both default to ON:

```ini
SkipLogos=1
SkipIntroVideos=1
```

Skip Logos is displayed directly above Skip Intro Videos in the System section.

The DXGI proxy reads the same `[Features] SkipLogos` INI value before UE4
startup. The overlay writes that same value, so there is one source of truth.
The menu also shows the actual boot state and blocked-file count.

V0.18's early-IAT primitive remains under test. The previous test produced no
visible skip, so V0.18B does not claim it is validated. No alternate fallback is
stacked into this build.


## V0.18C - Skip Logos Media Foundation test

Runtime evidence from V0.18B:

```text
Skip Logos boot=ON
EARLY_IAT CreateFileW=1
GetFileAttributesW=1
GetFileAttributesExW=1
blocked=0
```

Conclusion: the two startup movies do not travel through those main-EXE Win32
IAT calls. V0.18/V0.18B early-IAT interception is rejected.

V0.18C removes `EarlySkipLogos` from `dxgi.dll` completely. The DXGI proxy is
restored to the validated minimal loader.

Skip Logos now owns one primitive only:

```text
ASI startup on exact target
    -> hook mfplat!MFCreateSourceResolver
    -> capture IMFSourceResolver instance
    -> hook CreateObjectFromURL
    -> block only:
       THQ_LogoBasic.mp4
       AS_LogoBasic.mp4
```

The hook is installed from the ASI after normal DLL loading, never from
`DllMain`.

Diagnostics are bounded to the first eight Media Foundation URLs. The menu
shows:

```text
MF READY/OFF
resolver READY/waiting
URL call count
blocked count
```

`SkipLogos=1` and `SkipIntroVideos=1` remain independent and ON by default.
Skip Intro continues to use only the validated native
`g.PlayIntroCinematicOnBoot` CVar.


## V0.18D - UE4 StartupMovies Skip Logos

V0.18C Media Foundation is rejected after in-game testing: both company logos
remain visible.

The new path is deliberately simpler and targets the UE4 startup-movie
configuration itself.

Darksiders Genesis stores user configuration under:

```text
%LOCALAPPDATA%\THQ Nordic\Darksiders Genesis\Saved\Config\WindowsNoEditor\
```

V0.18D manages a small marked block in `Game.ini`:

```ini
; BEGIN DarksidersGenesisEnhanced SkipLogos
[/Script/MoviePlayer.MoviePlayerSettings]
-StartupMovies=THQ_LogoBasic
-StartupMovies=AS_LogoBasic
; END DarksidersGenesisEnhanced SkipLogos
```

This removes only the two company startup movies from UE4's
`MoviePlayerSettings.StartupMovies` array. It does not touch the intro
cinematic or any game files.

`Skip Logos` remains directly above `Skip Intro Videos` in the menu.

Both default ON. V0.18D introduces config revision `1804`; upgrading from any
older test INI resets these two options to ON exactly once:

```ini
SkipLogos=1
SkipIntroVideos=1
```

After that migration, user choices persist normally.

Skip Logos changes require a game restart because UE4 reads StartupMovies during
boot. Skip Intro remains the independent validated native
`g.PlayIntroCinematicOnBoot` control.


## V0.18E - Early global file-block test

V0.18D UE4 `Game.ini` StartupMovies override is rejected after in-game testing:
the override is written successfully but both company logos are still displayed.

V0.18E returns to the original file-block idea, but moves it earlier and makes
it process-wide.

Single path:

```text
first DXGI factory call
    -> outside DllMain
    -> before ASI loading
    -> MinHook KernelBase!CreateFileW
    -> exact basenames only:
       THQ_LogoBasic.mp4
       AS_LogoBasic.mp4
    -> ERROR_FILE_NOT_FOUND
```

This differs from V0.18/V0.18B:

- no main-EXE IAT patch;
- no Game.ini override;
- no Media Foundation hook;
- no extra fallback tree.

Because the hook targets the actual Win32 function rather than one module's IAT,
calls from any loaded game/UE4 module pass through the same detour.

The proxy records:

```text
CreateFileW total calls
.mp4 calls
blocked target calls
```

The overlay exposes those counters beside Skip Logos.

`SkipLogos=1` and `SkipIntroVideos=1` remain independent and ON by default.
Config revision `1805` resets those two values to ON exactly once when upgrading
from an older test build.

Skip Intro remains exclusively on the validated native
`g.PlayIntroCinematicOnBoot` path.


## V0.18F - native SetupLoadingScreenFromIni bypass

V0.18E proved that a process-wide `CreateFileW` hook was installed and active,
but no MP4 open ever passed through it. That file-open route is rejected.

A direct executable audit identified the actual UE4 startup-movie path.

Exact retail target:

```text
DarksidersGenesis-Win64-Shipping.exe
size       62,113,280 bytes
SHA-256    9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
SizeOfImage 0x03DDF000
```

Engine initialization contains the marker:

```text
GetMoviePlayer()->SetupLoadingScreenFromIni
```

The caller resolves `GetMoviePlayer()`, reads its vtable and invokes slot
`+0x78`. The corresponding vtable entry points exactly to:

```text
FDefaultGameMoviePlayer::SetupLoadingScreenFromIni
RVA 0x0160BC50
VA  0x14160BC50
```

That function reads, in order:

```text
bWaitForMoviesToComplete
bMoviesAreSkippable
StartupMovies
```

V0.18F therefore owns one primitive only:

```text
first DXGI factory call
    -> outside DllMain
    -> before ASI startup
    -> validate retail SizeOfImage + exact native prologue
    -> MinHook RVA 0x160BC50
    -> Skip Logos ON: return immediately
    -> Skip Logos OFF: call original function
```

No file hook, no Media Foundation hook, no Game.ini override and no fallback
tree remain in the active Skip Logos path.

Overlay telemetry:

```text
NATIVE READY/OFF
target VALID/INVALID
calls
skipped
```

Skip Intro stays entirely separate on the already validated native
`g.PlayIntroCinematicOnBoot` control.

Config revision `1806` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from an older test build.


## V0.18G - earliest native MoviePlayer RET patch

V0.18F proved that the audited MoviePlayer target was valid and the MinHook
detour installed successfully, but runtime telemetry stayed:

```text
setupCalls=0
skipped=0
```

Therefore the one startup call to `SetupLoadingScreenFromIni` occurs before the
first DXGI factory call where V0.18F installed its hook.

Executable audit confirms `dxgi.dll` is a normal import, not a delay import.
Its `DllMain(DLL_PROCESS_ATTACH)` therefore executes before the game entry
point.

V0.18G removes MinHook from the DXGI proxy entirely and uses a one-byte
fail-closed patch:

```text
DLL_PROCESS_ATTACH
    -> validate PE64
    -> SizeOfImage == 0x03DDF000
    -> validate exact prologue at RVA 0x0160BC50
    -> Skip Logos ON:
       0x48 -> 0xC3
       SetupLoadingScreenFromIni returns immediately
    -> Skip Logos OFF:
       restore native 0x48
```

No thread, no media hook, no file hook, no Game.ini override and no fallback
tree are used.

The ASI only exposes runtime control and telemetry:

```text
EARLY PATCHED/NATIVE
target VALID/INVALID
```

Skip Intro remains entirely separate on the validated native
`g.PlayIntroCinematicOnBoot` path.

Config revision `1807` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from an older test build.


### V0.18G in-game verdict: REJECTED

Latest runtime test:

```text
Target validation: exact=1
Skip Logos EARLY: proxy=1 target=1 patched=1 enabled=1 RVA=0x160BC50
```

Despite the exact retail target being validated and the one-byte RET patch being
successfully applied before normal game startup, the THQ Nordic / Airship
Syndicate startup logos are still displayed.

Conclusion:

- the patch timing is early enough;
- RVA `0x160BC50` is a real and valid MoviePlayer function;
- bypassing `SetupLoadingScreenFromIni` does **not** control these two observed
  company logos in this build;
- V0.18G is rejected and must not be promoted to `main`;
- do not revisit the V0.18G RET path without new executable evidence.

The validated gameplay core remains unchanged. Skip Intro continues to work
independently through `g.PlayIntroCinematicOnBoot`.

Next Skip Logos work must begin from fresh native analysis of the actual company
logo playback path rather than another timing variation of the rejected
`SetupLoadingScreenFromIni` route.


## V0.18H - CustomSplashScreen branch bypass

V0.18G proved that the MoviePlayer `SetupLoadingScreenFromIni` path was not the
source of the visible THQ Nordic / Airship Syndicate logos: the exact target was
validated and patched before game startup, yet both logos still appeared.

A deeper audit of the surrounding `FEngineLoop::PreInitPostStartupScreen`
control flow identified the second startup-screen path:

```text
GetMoviePlayer()->HasEarlyStartupMovie()
    YES -> Initialize -> PlayEarlyStartupMovies()
    NO  -> FPreLoadScreenManager
           -> HasRegisteredPreLoadScreenType(CustomSplashScreen)
           -> PlayFirstPreLoadScreen(CustomSplashScreen)
```

The executable's `EarlyStartupMovie` block matches the Unreal startup flow:
when no early MoviePlayer startup movie owns the screen, the engine falls back
to a registered `CustomSplashScreen`.

Exact retail call-site:

```text
RVA 0x002535DE  test al, al
RVA 0x002535E0  74 0F     je skip_custom_splash
...
RVA 0x002535EB  call FPreLoadScreenManager::PlayFirstPreLoadScreen
```

V0.18H owns one primitive only:

```text
dxgi.dll DllMain
    -> validate PE64 + SizeOfImage 0x03DDF000
    -> validate exact surrounding bytes
    -> Skip Logos ON:
       RVA 0x2535E0  74 -> EB
       always skip PlayFirstPreLoadScreen(CustomSplashScreen)
    -> Skip Logos OFF:
       restore 74
```

No MoviePlayer setup patch, file hook, media hook, Game.ini override, MinHook,
thread or fallback tree is active for Skip Logos.

This follows the lesson from the validated POSTAL startup-logo fix: target the
actual early startup-screen state/control path rather than the eventual media
file open.

Skip Intro remains fully independent on the validated native
`g.PlayIntroCinematicOnBoot` path.

Config revision `1808` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from older test builds.


## V0.18I - EarlyStartupMovie / CustomSplashScreen selector bypass

The supplied runtime log was from **V0.18G**, not V0.18H:

```text
Darksiders Genesis Enhanced ASI 0.18G-dllmain-native-ret-test
Skip Logos EARLY: proxy=1 target=1 patched=1 enabled=1 RVA=0x160BC50
```

That confirms the already documented V0.18G failure, but it does not constitute
an in-game test of V0.18H.

A fresh audit of the exact supported retail executable:

```text
DarksidersGenesis-Win64-Shipping.exe
size       62,113,280 bytes
SHA-256    9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

shows the complete early startup-screen selector in
`FEngineLoop::PreInitPostStartupScreen`.

Relevant control flow:

```text
RVA 0x253546  call GetMoviePlayer
...
RVA 0x253551  call [vtable+0x30]
RVA 0x253554  test al,al
RVA 0x253556  je 0x2535A8

TRUE branch:
RVA 0x253558  "EarlyStartupMovie"
...
RVA 0x25359B  call [vtable+0x38]

FALSE branch:
RVA 0x2535A8  "PlayFirstPreLoadScreen"
...
RVA 0x2535EB  call FPreLoadScreenManager::PlayFirstPreLoadScreen

common resume:
RVA 0x2535FD
```

This proves why V0.18H was incomplete: its patch at `0x2535E0` affected only
the `CustomSplashScreen` fallback. When an EarlyStartupMovie exists, execution
takes the TRUE branch and never reaches the patched H branch.

V0.18H is therefore **superseded by native analysis before validation**. It is
not promoted and does not need another in-game test.

V0.18I owns one primitive only:

```text
dxgi.dll DllMain
    -> validate PE64 + SizeOfImage 0x03DDF000
    -> validate exact startup-selector bytes
    -> Skip Logos ON:
       RVA 0x253546
       E8 45 56 3B 01
       ->
       E9 B2 00 00 00

       jump directly to RVA 0x2535FD
       bypass both:
         EarlyStartupMovie
         CustomSplashScreen

    -> Skip Logos OFF:
       restore E8 45 56 3B 01
```

This does **not** patch `SetupLoadingScreenFromIni`, does not hook file I/O,
does not hook Media Foundation, and does not modify `Game.ini`.

Normal game cutscenes remain outside this selector. The separately validated
Skip Intro feature remains exclusively controlled by:

```text
g.PlayIntroCinematicOnBoot
```

Config revision `1809` resets `SkipLogos` and `SkipIntroVideos` to ON once
when upgrading from older test builds.

V0.18I also removes two harmless duplicate `skipLogosEnabled` runtime
publications and replaces the obsolete overlay text `Game.ini write failed`
with the correct startup-patch diagnostic.


## V0.19A/B/C - proprietary StartupScreens audit and final Skip Logos fix

A direct audit of the supported retail executable identified the game-specific
startup screen plugin compiled into Darksiders Genesis:

```text
ProjectMayhem/Plugins/StartupScreens/Source/StartupScreens/Private/SStartupScreens.cpp
ProjectMayhem/Plugins/StartupScreens/Source/StartupScreens/Private/StartupScreensModule.cpp
```

The plugin exposes its own startup-screen configuration and playback path,
including:

```text
UStartupScreensSettings
StartupScreenDef
StartupMovies
TimeToShow
Image
PromptText
FadeTime
bMoviesAreSkippable
SStartupScreens
```

This explains why the previous generic UE4 MoviePlayer / StartupMovies
experiments did not control the visible THQ Nordic / Airship Syndicate logos.

Three independent plugin-level candidates were tested:

### V0.19A - StartupScreens module kill

```text
FStartupScreensModule::StartupModule
RVA 0x25FE40
-> immediate RET
```

**In-game verdict: REJECTED AS TOO BROAD.**

The logos disappear, but the intro cinematic is also skipped even with
`Skip Intro Videos=OFF`.

### V0.19B - empty StartupScreens movie playlist

```text
RVA 0x25FF31
44 8B 76 08
->
45 33 F6 90
```

This forces the copied `StartupMovies` count to zero before
`SStartupScreens` receives the playlist.

**In-game verdict: REJECTED AS TOO BROAD.**

The logos disappear, but the intro cinematic is still skipped when
`Skip Intro Videos=OFF`. The warning screen remains visible, proving this path
controls more than only the two company logo presentations.

### V0.19C - bypass StartupScreens MoviePlayer attachment

```text
RVA 0x260244
-> jump to RVA 0x260257
```

This keeps the proprietary `StartupScreens` module and object construction
intact, but skips only the block that attaches `SStartupScreens` to the engine
MoviePlayer.

**In-game verdict: VALIDATED.**

Observed behavior:

```text
Skip Logos ON
  -> THQ Nordic / Airship Syndicate logos skipped

Skip Intro Videos OFF
  -> warning screen remains
  -> intro cinematic still plays normally

Skip Intro Videos ON
  -> independent validated intro skip remains functional
```

A slightly longer black transition can occur when Skip Intro is OFF, but it is
minor and does not affect correctness or stability.

### V0.19D - cleanup / validated baseline

V0.19D is the clean continuation of V0.19C. It changes no gameplay behavior and
retains exactly the validated primitive:

```text
StartupScreens MoviePlayer attachment bypass
RVA 0x260244 -> 0x260257
```

No generic StartupMovies patch, no Media Foundation hook, no file hook, no
StartupModule kill, and no empty-playlist fallback remain in the active path.

The two user-facing options are now cleanly independent:

```text
Skip Logos
    -> proprietary StartupScreens attachment bypass

Skip Intro Videos
    -> g.PlayIntroCinematicOnBoot
```


## V0.27 / V0.28 - HorseCharacter detection and Sprint Duration validated

V0.27 finally resolves the horse at the correct layer by hooking native
`HorseCharacter` functions directly instead of inferring the mount from player
identity, Blueprint accessors or generic movement components.

Validated native capture paths include:

```text
HorseCharacter::GetNormalizedSpeed
HorseCharacter::GetNormalizedSpeedInput
HorseCharacter::TryStartSprinting
HorseCharacter::SetSprintingTrue
```

In-game validation captured a real horse instance with the expected native
stamina data:

```text
StaminaRecoveryPercentageRate      15
StaminaTotalRecoveryPercentageRate 40
StaminaRecoveryCooldown             2
StaminaSprintPercentageRate        25
CurrentStamina / MaxStamina        62 / 62
```

Horse Sprint Duration is therefore considered validated. The feature changes
the native sprint stamina drain rate rather than enlarging the stamina pool.

```text
0x   = vanilla drain / vanilla duration
5x   = default, approximately 5x sprint duration
20x  = maximum
```

V0.28 changes only the user-facing range/default for this validated feature:
default `5.00x`, maximum `20.00x`. The native HorseCharacter detection path
from V0.27 is retained unchanged.


## V0.29 - Horse Speed / Sprint Speed native movement correction

V0.29 keeps the validated V0.27 native `HorseCharacter` resolver and the
validated V0.28 sprint-duration path, but fixes the remaining horse speed
primitives.

The retail UE4 reflection table proves the exact movement fields:

```text
UMayhemHorseCharacterMovementComponent / UCharacterMovementComponent

MaxWalkSpeed            +0x1DC
MaxWalkSpeedCrouched    +0x1E0
MaxSwimSpeed            +0x1E4
MaxFlySpeed             +0x1E8
MaxCustomMovementSpeed  +0x1EC
MaxAcceleration         +0x1F0
MinAnalogWalkSpeed      +0x1F4
BrakingFrictionFactor   +0x1F8
BrakingFriction         +0x1FC

UMayhemHorseCharacterMovementComponent
SprintingMaxSpeed       +0x760
```

Previous horse candidates incorrectly wrote some of these neighboring fields.
V0.29 also removes the invalid `movement+0x190 == horse` acceptance test:
`HorseCharacter::GetNormalizedSpeed` already resolves the native movement
component through its own virtual getter at vtable slot `+0x5F8`.

Horse Speed now targets the real movement component's `MaxWalkSpeed` and
`MaxAcceleration`; Horse Sprint Speed targets that same movement component's
`SprintingMaxSpeed`.


## V0.30 - Graphics Adapter Engine.ini control

V0.30 adds a small system setting for selecting Unreal Engine's graphics
adapter index.

The overlay exposes values `0` through `4` with `0` as the default.
The selected value is persisted in `DarksidersGenesisMod.ini` and written to:

```text
%LOCALAPPDATA%\THQ Nordic\Darksiders Genesis\Saved\Config\WindowsNoEditor\Engine.ini
```

The mod writes exactly:

```ini
[SystemSettings]
r.GraphicsAdapter=N
```

If `[SystemSettings]` or `r.GraphicsAdapter` does not exist, it is created.
If the key already exists, only its value is updated through the Win32 INI API.
Other Engine.ini settings are preserved.

A game restart is required for adapter selection to affect device creation.


## V0.31 - Horse UObject lifetime safety hotfix

V0.29 validated the native horse speed, sprint-speed and stamina primitives, but
the first long level-load test exposed an important lifetime bug inherited by
V0.30.

The horse subsystem cached raw `HorseCharacter` and horse movement pointers and
re-applied tunings every rendered frame. During seamless travel or level reload,
UE4 can destroy those UObjects while the cached addresses remain readable and may
later be recycled for unrelated allocations. Continuing to write horse offsets
through such stale pointers can corrupt unrelated game state.

The reported failure presented as:
- a malformed/default-looking reticle after loading;
- corrupted HUD/menu visual elements;
- eventual `0xC0000005` write access violation inside the game's reference-
  counted object destruction path.

V0.31 changes the ownership rule:

```text
Native HorseCharacter callback active
    -> horse pointer is considered live
    -> resolve current horse movement
    -> apply Horse Speed / Sprint Speed / Sprint Duration

Outside a native HorseCharacter callback
    -> no horse-memory writes
```

Additional safety:
- `Tick()` performs no cached-pointer writes;
- the generic movement hook performs no horse writes;
- shutdown never restores values through cached raw UObject pointers;
- the horse movement pointer is re-resolved on every native horse capture;
- a changed movement pointer resets cached baselines as a new horse generation;
- generic movement exclusion accepts a cached horse movement only for a short
  window after a live native HorseCharacter callback.

The validated V0.30 Graphics Adapter Engine.ini control is retained unchanged.


## V0.32 - Reticle / Alt-Tab isolation

V0.31 removes the horse stale-pointer write path, but a separate visual issue
remains: immediately after loading a level the reticle can appear as a broken
cross, while a simple Alt-Tab restores the correct reticle.

The V0.31 session log proves the malformed reticle appears before the first
fully validated horse capture, so this visual issue is treated separately from
the horse lifetime crash.

V0.32 isolates the D3D11 overlay path:

- ImGui is no longer initialized on the first game Present;
- no overlay RTV is created during normal gameplay;
- the game window is not subclassed until the user opens the mod menu;
- the complete overlay backend is created lazily on the first menu-key press;
- game-window foreground transitions are logged;
- every process-owned DXGI ResizeBuffers call is logged before and after.

Test policy:

1. Start the game and do not open the mod overlay.
2. Load a save and inspect the reticle.
3. If the broken cross is present, Alt-Tab once.
4. Preserve the per-session log.

This determines whether the fix comes from DXGI ResizeBuffers, focus activation,
or merely avoiding early ImGui/RTV/WndProc initialization.


## V0.33 - Cursor-layer reticle fix

The V0.32 focus trace isolates the broken-cross issue from both the horse system
and the ImGui overlay:

- the malformed cross can appear before ImGui is initialized;
- Alt-Tab repairs it through a foreground transition;
- no DXGI `ResizeBuffers` occurs during the fixing Alt-Tab;
- the reticle/cross remains visible when `ui.HideHud` is forced, proving it is
  handled by the cursor layer rather than the ordinary HUD layer.

Retail executable audit identifies:

```text
UAirshipUIManager::IsCursorVisible
RVA 0x715800
```

The native function ultimately reads the PlayerController mouse-cursor visible
state.

V0.33 therefore:
- hooks `UAirshipUIManager::IsCursorVisible`;
- forces cursor visibility false while mod Hide HUD is active;
- schedules a cursor refresh when a newly validated player instance appears;
- posts `WM_SETCURSOR` to the real game window several times after load;
- logs the active `HCURSOR`, visibility and position around focus transitions
  and forced refreshes.

This is intended to reproduce the cursor-reset part of Alt-Tab without actually
changing application focus.


## V0.34 - Reticle focus-cycle isolation (experimental)

V0.33 is **rejected as an effective reticle fix**. Its scheduled WM_SETCURSOR
messages were successfully posted after a new player was found, but did not
repair the cross-shaped reticle. The user's V0.33 log also shows that a genuine
Alt-Tab involves a different OS cursor handle after focus comes back, and no
DXGI ResizeBuffers call was observed. The trace contained an F4 action but no
F1 Hide HUD toggle, so Hide HUD cursor suppression is not yet proven either way.

V0.34 removes automatic WM_SETCURSOR refreshes that did nothing. It introduces
one explicitly requested test action:

- `F5 = ReticleFocusTest` in the *test package* INI;
- the action is also selectable from the remappable Hotkeys tab;
- the test does **not** Alt-Tab, steal foreground or move the mouse;
- it posts one paired simulated deactivate/activate message sequence to the
  game's window: `WM_ACTIVATEAPP`, `WM_ACTIVATE`, then `WM_SETFOCUS`
  and `WM_SETCURSOR`;
- logs each posting result and cursor handle, then captures cursor state
  500 ms afterward;
- no ongoing timer, native horse writes, or per-frame cursor replacement.

**How to test:** With the malformed cross visible and the mod overlay CLOSED,
press F5 once. Compare the reticle before/after. If nothing changes, perform one
real Alt-Tab and confirm if it still repairs the cross. Do not claim the
experimental focus pulse works until the user confirms it in-game.
If using an old mod INI, set `F5=ReticleFocusTest` or remap a key to
`Reticle Focus Test` in the overlay. The test can temporarily change the
game's perceived input focus; a real Alt-Tab should restore normal state.

**Safety:** V0.31 horse lifetime ownership rules remain untouched; V0.32 lazy
ImGui creation remains. `main` and the V0.29 release remain unchanged.
