# Darksiders Genesis Enhanced PC Mod

Modern runtime enhancements for **Darksiders Genesis (PC)** using a local x64
DXGI proxy and ASI plugin.

The project is deliberately fail-open for normal runtime failures and
**fail-closed for unsupported game executables**.

## Downloads (stable links)

**Latest experimental test ZIP:** [DarksidersGenesis_LATEST_TEST.zip](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/download/test-build/DarksidersGenesis_LATEST_TEST.zip)

**Versioned V0.64 backup:** [DarksidersGenesis_V0.64.zip](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/main/downloads/DarksidersGenesis_V0.64.zip)

**Official validated release:** [GitHub Releases](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases) (currently V0.29).

The experimental `test-build` prerelease is updated automatically only after a successful Windows build.
Never distribute signed GitHub Actions artifact redirect links, since they expire.
See [DOWNLOAD_STANDARD.md](docs/DOWNLOAD_STANDARD.md) for our reusable policy across mod repositories.
## Current development state

- Latest published binary release: **V0.29**
- Safety hotfix **V0.31** and lazy overlay **V0.32** retained
- Latest failed cursor experiment: **V0.33**
- Earlier V0.34 F5 reticle focus workaround is **not reliable**; reticle bug remains open
- Current status: **V0.38 loads successfully**; earlier V0.38 startup failure was a Windows Defender quarantine false alarm. V0.39 remains an optional loader-diagnostic build, not a required repair. V0.38 yaw and Alt-Tab changes still need separate in-game validation.
- Target executable:

```text
DarksidersGenesis-Win64-Shipping.exe
Size: 62,113,280 bytes
SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

V0.17 calculates the executable SHA-256 at runtime. If the executable does not
match exactly, the overlay/log can still load but **gameplay hooks are not
installed**.


## Development and versioning policy

Development now advances directly on `main`: one new numbered build per
version (V0.35, V0.36, V0.37...), recorded by normal Git commits. Do not
create a new `dev/v...` branch for every build. A published release is only
created after in-game validation; the latest release V0.29 stays unchanged
until a newer version is approved. Work-in-progress commits on `main` are
not release candidates.

Historic `dev/` branches are legacy experiments. Do not delete them until
their divergent Git history is confirmed reachable from a durable reference.

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
| Manual Reticle Focus Refresh | **Unreliable/experimental** - F5 focus pulse may help, but does not fix persistent cross-shaped reticle |
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
F5  Reticle Focus Test (V0.34 validated manual fix)
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


## V0.34 - Reticle focus-cycle workaround (validated manually in-game)

V0.33 is **rejected as an effective reticle fix**. Its scheduled WM_SETCURSOR
messages were successfully posted after a new player was found, but did not
repair the cross-shaped reticle. The user's V0.33 log also shows that a genuine
Alt-Tab involves a different OS cursor handle after focus comes back, and no
DXGI ResizeBuffers call was observed. The trace contained an F4 action but no
F1 Hide HUD toggle, so Hide HUD cursor suppression is not yet proven either way.

V0.34 removes automatic WM_SETCURSOR refreshes that did nothing. It introduces
one manually triggered reticle-repair action:

- `F5 = ReticleFocusTest` in the included INI;
- the action is also selectable from the remappable Hotkeys tab;
- the test does **not** Alt-Tab, steal foreground or move the mouse;
- it posts one paired simulated deactivate/activate message sequence to the
  game's window: `WM_ACTIVATEAPP`, `WM_ACTIVATE`, then `WM_SETFOCUS`
  and `WM_SETCURSOR`;
- logs each posting result and cursor handle, then captures cursor state
  500 ms afterward;
- no ongoing timer, native horse writes, or per-frame cursor replacement.

**In-game validation (2026-10-08):** The user confirms that V0.34's F5
focus-cycle action fixes the malformed reticle after loading. The confirmed
result is the **manual F5 workaround**, not an automatic fix on every load.
The separate V0.33 Hide HUD cursor visibility behavior has not yet received a
dedicated F1 test; do not describe it as validated.

**Usage:** With the malformed cross visible and the mod overlay CLOSED,
press F5 once. If using an older mod INI, set `F5=ReticleFocusTest` under
`[Hotkeys]` or remap a key to `Reticle Focus Test` in the overlay.
This action can temporarily change the game's perceived input focus; a real
Alt-Tab may restore the normal state if focus behaves unexpectedly.

**Safety:** V0.31 horse lifetime ownership rules remain untouched; V0.32 lazy
ImGui creation remains. The V0.29 binary release is kept as a rollback option.


## V0.35 - camera investigation restarted from V0.34

Source: validated V0.34 commit 32f7827cc2968596a34dadc6466730d4280ff94b.
None of the V0.35A/B/C or V0.36 camera changes are inherited.

The camera is NOT modified by this test build. Two individually validated
read-only native-method hooks record whether UCameraComponent::GetCameraView
(RVA 0x16F9790) and USpringArmComponent::UpdateDesiredArmLocation
(RVA 0x6F57B0) are exercised in actual gameplay. The hooks call the original
implementation unchanged. They only sample a live FOV output / current arm
distance and log the first 12 calls then one of every 5000. The correct
retail EXE hash and instruction prologues must match before installation.
No native UObject pointers survive callbacks; no camera setting or rotation
is changed. Existing FOV is OFF/locked, and no fake zoom setting is added.

Test on foot with War and Strife, then mounted if possible, while leaving
the mod overlay closed. Press F5 only if the reticle needs repairing.
Quit the game and provide the non-cumulative DarksidersGenesisMod.log.
Look for CameraTrace V0.35 READY and callback call counts.
Successful compilation does NOT prove that either native path controls
the rendered gameplay view. No promotion to main or public release until
an in-game test confirms behavior. Further zoom/FOV/angle work must use
verified live gameplay callbacks.


## V0.36 - independent Hide Reticle and visible camera controls (test)

This build continues directly on main, without creating a development branch.
Hide Reticle (default OFF; Hotkey F6 by default) is separate from Toggle HUD
(F1) and preserves F5's validated manual reticle focus repair. The existing
native UAirshipUIManager::IsCursorVisible hook now obeys Hide Reticle too.
A second, opt-in Windows SetCursor suppression path acts only when the game
is the foreground window, Hide Reticle is ON, and the mod overlay is closed.
This targets the malformed Windows cursor cross which may survive ui.HideHud.
The OS cursor is restored on exit from that suppression state. The in-game
behavior is NOT yet validated. The original reticle bug is not claimed fixed.

Camera is now a dedicated enabled tab, with FOV override (40 to 140,
default OFF/90), Zoom (+ closer / - farther, -75 to +200 percent, default 0),
and Camera Pitch Offset (-35 to +35 degrees, default 0). All settings are
persisted to the INI and published to atomics. Native, verified-method hooks
modify only a live FMinimalViewInfo output (FOV and pitch) and temporarily
scale the live SpringArm TargetArmLength in its own callback, restoring that
UObject field immediately afterward. No camera UObject pointer is cached
across frames. Hook readiness, callback counters and observed native/applied
values are shown in Camera and logged once per session with sampling.

Caveat: native GetCameraView and UpdateDesiredArmLocation are known code
addresses but their use by the actual gameplay camera remains unproven.
This is an EXPERIMENTAL candidate, not a claim that FOV/zoom/pitch work.
If no calls are observed, the next step is to locate the actual camera path
from the game's native call graph. Test normal gameplay, Strife, and mounted
camera if possible, and supply DarksidersGenesisMod.log.

Defaults remain vanilla for Hide Reticle, FOV, Zoom and Pitch. The config
revision does not change, to avoid resetting existing player preferences.
Older INI files read the new settings using safe defaults when missing.
The validated horse lifetime ownership, lazy ImGui overlay, skip logos and
F5 reticle recovery remain unchanged. Artifacts contain files at the ZIP root.

## V0.37 - Camera height + configurable arrow keys (TEST, 2026-10-08)

Based directly on the working V0.36 main commit 634f89dc.
**User in-game status:** FOV, Zoom and Camera Pitch are working.
A camera-height setting is now provided: CameraHeightOffset from -1500 to
+1500 world units, default zero (vanilla). It changes only the transient
FMinimalViewInfo.Location.Z output at +0x08 in the existing live camera
callback. It does not cache or write camera UObjects.

New camera keybinding actions are configured under [CameraHotkeys] and can
be rebound in the Camera tab. Defaults:
- Up: raise camera by 50 units; Down: lower camera by 50
- Left: Zoom Out by 10 percent; Right: Zoom In by 10 percent
- PitchDown, PitchUp: unbound; optional increments of 5 degrees

Keys repeat when held (initial delay 290 ms, repeat every 90 ms), only
while the game is foreground and mod overlay is closed. Duplicate bindings
are unbound in other camera actions. Capturing Escape cancels; menu key and
F1-F12 are reserved. Assigning None from the Camera UI is supported.
Existing INIs without [CameraHotkeys] use those defaults. Camera values and
hotkeys are saved via the normal debounce/persistence mechanism.

**Known Zoom issue (deferred):** Zoom changes camera arm distance without
updating UE4 depth-of-field focal distance or blur/postprocess parameters,
so a zoomed image can become very blurry. Do NOT regress the working zoom
just to compensate; native DOF correction is a separate future task.

**Reticle status corrected:** earlier V0.34 F5 focus pulse was prematurely
described as a validated repair. The user confirms the cross-shaped reticle
still occurs in V0.36. Thus F5 and the V0.36 Hide Reticle mechanism are
experimental, not a reliable bug fix; neither is promoted as solved.

One build number per iteration; changes committed on main; legacy stable
release v0.29 remains untouched. Per-launch non-cumulative log retained.
Test V0.37 in gameplay before promoting features to release.

## V0.38 - horizontal camera yaw, reticle UI and Alt-Tab mitigation (TEST)

Base: main V0.37 commit 1108b307a71886a17be6187fba0a4b8a690b04a8.
User log V0.36: Window focus trace switches foreground 1 to 0 around
23:17:10, with no observed return before logging stops; not proof
the mod caused lost focus. V0.37 log: Hide Reticle ON checked in menu,
but OS pointer suppression begins only on overlay close. F6 on/off
and Win32 SetCursor interceptions are proven by log.

V0.38:
- Native FMinimalViewInfo::Rotation.Yaw (+0x10) adjusted in transient
  GetCameraView output only; CameraYawDegrees -180..+180, default 0,
  configured with a live slider, number entry, default button.
- Bindable action YawLeft and YawRight, default NumPad 4 and NumPad 6
  with Num Lock enabled, 5 degrees per step. Prior bindings preserved.
- Hide Reticle menu explains gameplay-only behavior and includes
  Apply Reticle Setting and Return to Game to close the overlay.
  F6 remains the toggle; this does not repair the underlying malformed cross.
- Focus mitigation: restore cursor even when losing foreground
  (V0.37 erroneously only restored when foreground); release pointer
  clip on that suppression exit, do not block WM_ACTIVATEAPP,
  WM_ACTIVATE, WM_SETFOCUS, WM_KILLFOCUS, WM_MOUSEACTIVATE or Alt
  system key messages, and log WndProc focus messages.
  Only release cursor clipping from an active overlay while the game
  has foreground. In-game Alt-Tab still requires validation.
- No changes to validated FOV, Zoom, Pitch, Height or horse protections.
  Zoom postprocess Depth of Field blur is a tracked future task.
- No release until user in-game confirmation, sequential build
  V0.38 on main, artifacts ZIP root, session-only logs.

Validation: test yaw +/-30 degrees, NumPad4/6, custom bindings,
Hide Reticle checkbox and apply button, F6 with overlay closed,
Alt-Tab with overlay both open and closed and Hide Reticle both states.

## V0.39 - Startup loader diagnostic (TEST)

The user reported that the *direct-download* V0.38 did not show either
the overlay or any hooks. V0.36 and V0.37 previously loaded and functioned
on the same PC. The existing uploaded logs predate this V0.38 failure.
Static inspection of the V0.37/V0.38 Windows build artifacts found both
ZIPs valid with all four root-level files, same x64 PE architecture and
matching imported DLL families. No confirmed root cause yet.

V0.39 changes ONLY startup observability (not the camera or gameplay):
- The DXGI proxy writes DarksidersGenesisLoader.log into the same folder
  as dxgi.dll; this log is independent of DarksidersGenesisMod.asi.
- The first loader log truncates the previous session. It lists the
  proxy DLL path, ASI scan, each ASI LoadLibrary result and GetLastError,
  and the real System32 dxgi.dll load result. No cached pointers, no
  new hooks, and no changes to camera, reticle, horse or HUD behavior.
- Core startup version string updated to V0.39 so an old/cached ASI is
  immediately recognizable in DarksidersGenesisMod.log.
- A missing loader log means the game did not invoke the proxy's
  DXGI factory export, the proxy was not placed/loaded correctly, or
  initialization happened earlier than logging. In that case, confirm
  dxgi.dll is next to the actual DarksidersGenesis-Win64-Shipping.exe.
- If loader log exists but ASI log does not, look for the precise
  LoadLibrary result/error; also check game EXE hash and quarantine.
- If both log files exist with version 0.39, check native hook readiness.
- Treat V0.37 as verified fallback, not V0.38. No stable release.

The other unresolved issues remain: unreliable malformed reticle/F5,
potential Alt-Tab focus recovery, zoom-induced DOF blur, and in-game
validation of horizontal yaw. V0.39 makes no claims to fix these.

## 2026-10-09 correction: V0.38 startup failure was antivirus quarantine

The user confirmed that V0.38 **does load and work**. The preceding
"no overlay / no hooks" report was caused by Windows Defender flagging
and quarantining the mod, **not** by a confirmed loader or code defect.
This correction supersedes the original startup-failure hypothesis
documented in the V0.39 diagnostic entry above. Keep that historical
entry for traceability, but do not treat it as a current failure.

V0.38 is reinstated as a functioning test build. This does NOT by itself
validate every V0.38 feature (yaw camera rotation, Alt-Tab recovery, or
persistent reticle fix); these retain their separate test statuses.
V0.39 remains an optional loader diagnostic, not a mandatory patch.

Installation guidance: check Microsoft Defender Protection History if
the ASI or DLL disappears. Verify the downloaded file comes from the
project's GitHub repository before deciding how to handle a warning;
do not advise disabling antivirus globally or bulk-excluding folders.

## V0.40 - Diagnose cross-shaped reticle source after save load (TEST)

V0.38 user test confirms Hide Reticle ON/OFF fixed, while cursor still
looks like a cross immediately after loading a save. Log shows native
UAirshipUIManager::IsCursorVisible changing 0->1 after loading.
F5 synthetic focus pulse is unreliable and not a permanent fix.

V0.40 provides a reversible test mode separate from validated HideReticle:
[Features] CrossCursorTestMode=0 (default) = unmodified behavior.
1 = suppress only the Win32 SetCursor output while leaving native Unreal
cursor visibility untouched.
2 = suppress only the native Unreal cursor while leaving Win32 cursor
output untouched. Existing Hide Reticle independently hides both layers.
The ImGui overlay keeps its clickable mouse pointer. The user's menu
can apply changes and return to gameplay. Settings survive restart.
No additional game hooks and no cached UObject writes.

The SetCursor hook now logs changed requested OS cursor handles with a
bounded budget even if HideReticle is OFF. Native UI cursor visibility
transitions are also logged on state changes. This evidence distinguishes
the source of the cross from the legitimate reticle.

TEST: after loading the same save and reproducing cross, keep Hide Reticle
OFF. Try cross mode 1, then mode 2, returning to game each time. Tell
whether the cross disappears and whether normal reticle remains visible.
Restore 0 for default. This build is a diagnostic/possible mitigation,
not an already validated automatic reticle repair.

V0.38 functionality and camera, horse, focus, F6 kept unchanged;
V0.39 loader diagnostics retained. Build V0.40 from main, no dev
branches or automatic public release; permanent download ZIP on GitHub
since temporary artifact links have failed for this user. Log is
noncumulative per launch.

## V0.41 - Auto F5 reticle correction after save loading (test)

Actual V0.40 log: the test mode stayed CrossCursorTestMode=0, so the two
separate source-filter modes 1/2 were not exercised; do not consider
those experiments disproven. The game's normal load shows native UI
cursor visibility 0->1 after scene load. User's V0.38 test establishes
that pressing F5 or actual Alt-Tab fixes the cross; Hide Reticle ON/OFF
also works and must remain independent.

New [Features] AutoReticleRefreshOnLoad=1 defaults ON, with a gameplay
overlay checkbox. On a native UI cursor 0->1 load transition, arm a
2500ms delayed, one-shot replay of the existing F5 activate/deactivate
state machine. Run only on game Present with a validated local player,
foreground nonminimized window and closed overlay. No synthetic mouse,
SetForegroundWindow, UObject writes, cached game pointers or new hooks.
Only one automatic cycle per 20s, cancel pending on manual F5 or real
Alt-Tab, expire after 30s if gameplay is not ready. Log all arms,
dispatches and cancellations. Disable via INI if focus changes undesirably.

This is a candidate automatic workaround, not a validated fix until
user tests in game. V0.40 is preserved for rollback; gameplay settings,
cursor source diagnostic tests, camera yaw, zoom, height, horse, HUD,
F1/F6 and skip intro remain unchanged. The build ZIP keeps four files at
root, noncumulative logs, no public release before user validation.

## V0.42 - Cleanup and READ-ONLY reticle root-cause instrumentation (TEST)

**User result:** V0.41 automatic F5 replay did not fix the cross and is
an undesirable workaround. User asked to remove leftover useless experiments
and investigate the actual mechanism causing the cross after save-load.

Audit of code vs V0.38/V0.40:
- V0.41 AutoReticleRefreshOnLoad and its delayed focus pulse scheduler,
  state, INI, overlay toggle, config and runtime plumbing: REMOVED.
- V0.40 CrossCursorTestMode 1/2, which were never exercised in supplied
  logs and would only mask symptoms: REMOVED.
- Old INI keys AutoReticleRefreshOnLoad/CrossCursorTestMode are deleted on
  next normal config save. Existing INI revision stays unchanged.
- Preserve VALIDATED Hide Reticle ON/OFF including F6, the native cursor
  visibility hook and Win32 SetCursor suppression only when Hide Reticle ON.
- Preserve manual F5 (user confirms it clears the cross) but NEVER trigger
  it automatically. It is an opt-in A/B control for the diagnostic.
- Preserve Alt-Tab focus handling, lazy ImGui, verified camera/yaw/zoom,
  horse pointer safety, HUD, intro skip, movement and all other features.
- Legacy ThirdPerson / other inactive config keys remain for backward
  compatibility: do not silently remove user-requested saved settings.
- Native cursor visibility remains read-only except existing Hide Reticle.
  With Hide Reticle OFF the diagnostics NEVER change SetCursor's output.

NEW READ-ONLY DIAGNOSTIC:
- Sample GetCursorInfo every ~350ms while rendering. On change (cursor
  handle, showing flags, game focus), log desktop cursor handle, flags,
  foreground, mouse position, GetIconInfo hotspot and bitmap dimensions,
  clipping and last Win32 requested handle. Release icon bitmaps immediately.
- Existing SetCursor hook, on first <=40 requested-handle transitions, logs
  calling thread, return address and game exe relative caller RVA. These
  pinpoint which code path requested the cursor after loading.
- Correlate with native UAirshipUIManager::IsCursorVisible state changes.
- Snapshot before manual F5 and after the paired cycle, and across
  real Alt-Tab transitions; no keypress or focus generated by the probe.

TEST PROCEDURE: keep HideReticle OFF and never open overlay until after the
cross appears. Load save, observe cross for several seconds; then do one
real Alt-Tab out-and-back, see whether cross disappears; optionally press
manual F5, then exit and send noncumulative DarksidersGenesisMod.log.
Ideally also compare whether the cross appears with all mod binaries
temporarily absent, to distinguish a native game bug from mod interaction.
Do not attribute the root cause before comparing code RVA/state transitions.
No public release until validated. ZIP files at root.

## V0.43 - Skip Logos attachment bypass implicated in malformed cursor (TEST)

User discovered cross appears after save load when Skip Logos is enabled.
The earlier V0.19C patch at RVA 0x260244 jumps to 0x260257, avoiding
SStartupScreens attachment to MoviePlayer. This bypass was initially
validated for faster boot and independent Skip Intro cinematic behavior,
not for the lifecycle of Slate input/focus/cursor in later levels.
It is a STRONG causal lead, not yet proof of which cleanup call is missing.

This build DOES NOT introduce another cursor workaround or speculate a
replacement low-level patch. It preserves the V0.42 removal of automatic
F5 and abandoned cursor mode 1/2 experiments, and passive cursor traces.
It preserves working Hide Reticle ON/OFF and F6, camera, HUD, horse, etc.

Safe-by-default, controlled A/B:
- Fresh INI SkipLogos=0; SkipIntroVideos remains independent and ON.
- Bump config revision 2101 -> 2102. Migrates existing INI with ONLY
  SkipLogos forced OFF; other settings preserved.
- CRITICAL: earliest proxy checks config revision BEFORE the ASI loads;
  old revisions cannot enable the attachment bypass on that first boot.
- Explicit SkipLogos=1 after migration can still activate old V0.19C
  for A/B if needed, with warning in GUI and loader/ASI log.
- SkipLogos=0 leaves vanilla MoviePlayer attachment intact.
- Changing either option affects startup on NEXT game launch only.
- No new public release until tested.

Controlled test: SkipLogos OFF, restart game, load same save, check
cross without F5 or Alt-Tab. If cross absent, association confirmed;
optional second run SkipLogos ON verifies regression with otherwise
identical setup. Next engineering step is disassemble StartupScreens'
attach, playback completion and destruction paths to implement a
true native lifecycle-preserving logo skip without touching game assets.
Do not claim a permanent working Skip Logos replacement yet.

### StartupScreens root-cause audit (after V0.43)

The full audit and exact known 19-byte setup sequence are in
[docs/STARTUPSCREENS_AUDIT.md](docs/STARTUPSCREENS_AUDIT.md).
The failed logo bypass at `0x260244` skips both a call to
`0x1608B90` (probable GetMoviePlayer) and the virtual method call at
`[vtable+0x20]` (probable loading-screen setup) with `[rbp-0x78]`.
Those identities must be checked with the exact retail executable.
**User verified SkipLogos OFF removes the cross-shaped cursor.**
Retain V0.43 safe default; avoid workarounds. The game EXE is not
present among files currently accessible, so no replacement binary
patch is being claimed or published.

## V0.44 - Probe MoviePaths with the real retail EXE (TEST)

USER PROVIDED the exact 62,113,280-byte retail binary, whose SHA-256
matches 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
Disassembly of RVA 0x25FE40..0x260257 confirms:
- settings UObject array at +0x38 contains `StartupMovies`;
- each entry in that TArray is a 16-byte UE FString;
- the compiled startup code copies it into `FLoadingScreenAttributes`
  at [rbp-0x68], count [rbp-0x60], and passes attributes at
  [rbp-0x78] to a virtual MoviePlayer method;
- the getter call at 0x260244 resolves to 0x1608B90 and virtual
  call at 0x260253 uses [vtable+0x20]. With the original 19 bytes
  present, the native call sequence remains intact.

V0.44 has a **strictly opt-in, one-shot diagnostic probe**, not a new
logo skip. Fresh test INI [Diagnostics] StartupMovieProbe=1. Remove
or set 0 to return to fully unmodified native startup (V0.43 behavior).
It uses an exact-byte-verified 19-byte trampoline at RVA 0x260244,
and replays all original calls byte-for-byte before 0x260257. The
pre-call C++ observer reads MoviePaths TArray entries without writing
to them, bounding all lengths and page accesses with VirtualQuery.
It logs names, count, and minimum loading-screen time in
DarksidersGenesisLoader.log. Diagnostic is disabled when legacy
SkipLogos=1 because that legacy code is already proven to cause the
cursor regression. An in-game attempt to enable SkipLogos while probe
is installed fails closed until a restart with probe disabled.

***Do not call this a Skip Logos fix.*** No logo suppression occurs.
The purpose is to identify the exact runtime playlist names, allowing
a later targeted plan that preserves native MoviePlayer attachment.
The default distribution enables the probe for a one-time test, but
the proxy defaults it OFF for older INIs that lack the key. Hide
Reticle ON/OFF, F6, camera, horses, HUD, intro skip, and all other
features remain unchanged. If the probe causes a crash, disable
StartupMovieProbe=0 and restore V0.43; do not ship as stable release.
No synthetic F5, focus or Win32 cursor manipulation is introduced.
Log is per-run (loader also resets per-run).

## V0.45 - ABORT unsafe StartupMovieProbe trampoline; restore safe baseline

**User-reported crash** with V0.44. Supplied loader log shows that
`StartupMovieProbe V0.44: ... trampoline ACTIVE` was written, while no
`StartupMovieProbe ... item` or `call=` entry appeared. ASI log reaches
early core init but ends before normal shutdown. This strongly implicates
the new mid-function trampoline; no crash dump or exception address
was supplied, so precise failing instruction is **not proven**.

Root engineering mistake: V0.44 diagnostic was described as read-only
because it did not modify `FLoadingScreenAttributes`, but it patched
**19 bytes of executable instructions** and redirected execution to
a manually built x64 thunk. That is intrusive and unsafe without
unwind registration, thorough calling-convention tests and execution
validation. Do not reintroduce it as an "observation-only" technique.

**V0.45 removes the entire V0.44 trampoline and observer** from the
DXGI proxy and restores exactly the proven V0.43 proxy source.
New INI omits `[Diagnostics] StartupMovieProbe`. If an old INI still
contains `StartupMovieProbe=1`, the V0.45 proxy deliberately ignores
that retired key, so an old INI cannot reactivate the probe. Keep
`SkipLogos=0` as the safe startup default. The historic bypass at
`0x260244` remains *explicitly opt-in* and not fixed; turning it ON
can reintroduce the cross-shaped cursor as previously confirmed.
`SkipIntroVideos` remains independent.

This is **safety rollback, not a repaired Skip Logos**. Reticle/F6,
camera, HUD, horses and all other gameplay code are left unchanged.
No automatic F5, synthetic focus or cursor masking reintroduced.
The live exe remains unmodified by the probe with SkipLogos OFF.

Next investigation route: inspect `StartupScreens` defaults and the
actual movie/config resources offline (for example the game's
`DefaultGame.ini`, `StartupScreens` settings, or the original game
`.pak` assets). Prefer content-/descriptor-level, logo-only changes
without bypassing `MoviePlayer::SetupLoadingScreen`; any runtime hook
requires separate safety audit. No public stable release from this
test until user validates V0.45 startup.

## V0.45 runtime validation from user logs (2026-10-09)

The user supplied both Loader and ASI logs for the V0.45 test session
(start 13:51:40, clean shutdown at 13:52:42):
- DXGI proxy logs "V0.43 SAFE: native MoviePlayer startup attachment preserved",
  and loads the ASI successfully.
- ASI logs "0.45-no-trampoline-safe-startup" with supported exact EXE
  SHA-256 and Skip Logos patched=0 enabled=0.
- D3D11, Camera, Horse, Reticle, HUD, Movement, Recovery and Damage
  hooks report ready; player detected; in-game combat and native
  cursor transitions are recorded.
- Ends with "Shutdown: complete", no crash recorded in these logs.
- The user did not explicitly describe the visual appearance of the
  reticle in this V0.45 session: do not claim visual revalidation
  from native-cursor visibility alone.
- In contrast, V0.44 opt-in early trampoline showed StartupMovieProbe
  ACTIVE and caused a startup crash. The implementation is removed
  in V0.45: never resurrect it.

Next engineering step: target the logos selectively while preserving
SStartupScreens and the native MoviePlayer attachment/lifecycle.
No automatic F5, synthetic focus, disabled startup module, globally
emptied startup movies or unsafe mid-function trampoline.
Public stable release remains unchanged.

### Native StartupScreens continuation, verified logo identities

The exact game EXE and public PC-specific guides support further
analysis recorded in [docs/STARTUPSCREENS_AUDIT.md](docs/STARTUPSCREENS_AUDIT.md):
logo files are `THQ_LogoBasic.mp4` and `AS_LogoBasic.mp4`;
`CG_Intro_LowVi.mp4` is separate. Unlike a config-file rename,
the desired mod must remain ASI-only. The file stems are absent from
the matching EXE; only reflected settings/array fields are present.
The actual runtime movie list is unknown without inspecting the
game's cooked config/resources. V0.45 is unchanged and safe with
`SkipLogos=0`. No speculative patch is permitted.

## V0.46 - Experimental exact-name first-two StartupMovies filter (TEST)

Goal: bring back Skip Logos without touching the native MoviePlayer setup
and without altering game assets. Continue directly from V0.45 stable
safe startup. V0.44's crashing 19-byte MoviePlayer wrapper remains gone.

SOURCE: The user-supplied retail executable was verified against
62,113,280 bytes and SHA256
9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
RVA 0x25FF31 contains exactly:
44 8B 76 08 (mov r14d,[rsi+8], array count)
48 8B 36    (mov rsi,[rsi], array descriptor pointer).
The game's unmodified 19 bytes at RVA 0x260244 are independently
validated and NEVER changed by this version.

V0.46 uses an early 7-byte exact-signature redirect at RVA 0x25FF31.
A nearby executable island replays those original two instructions,
saves flags and all volatile GP registers, allocates the Win64 32-byte
shadow space and calls a bounded validator. The validator reads at most
3 names with VirtualQuery guards and logs them to the Loader log.
Only if first two FString descriptors name THQ_LogoBasic and AS_LogoBasic
in either order, count is 3..64 and all relevant buffers are readable
does the island increase RSI by 32 (skip two 16-byte descriptors) and
decrease R14D by 2. This preserves the third and subsequent entries.
In all other circumstances the ORIGINAL full playlist is passed on.
The island restores original registers/flags and uses a RIP-indirect
absolute jump back at RVA 0x25FF38, not clobbering RAX.

The following native code deep-copies the resulting descriptors and
calls the ORIGINAL SetupLoadingScreen vtable method. Unlike the
V0.19C failed attachment bypass, this retains the MoviePlayer/Slate
startup path and original cleanup ownership. It is nonetheless an
experimental early executable detour; crash-free behavior is NOT
proven. On crash use V0.45 or SkipLogos=0 (fresh startup required).

Fresh TEST ZIP includes SkipLogos=1 to exercise the new mechanism.
Existing INIs are preserved and V0.45 users must explicitly change
SkipLogos=0 to 1 to test it. Missing/old INIs do not automatically
enable it. The old V0.19C attachment bypass is removed entirely from
the proxy; it cannot be invoked by this build. Skip Intro remains
controlled by g.PlayIntroCinematicOnBoot and unaffected by code changes.

If the named logo files are NOT the first two native entries, the
validator logs FAIL OPEN and the logos still show. This is intentional
rather than guessing. Future builds may expand to name-matching in
arbitrary positions after observing real runtime ordering.

TEST GATE: startup with new proxy, log messages in
DarksidersGenesisLoader.log, logos skipped or not, warning display,
intro with SkipIntroVideos OFF, in-game cursor after save load,
Alt-Tab and overlay; do not publish a public tagged release before
user validates. Files at ZIP root. No new auto F5/focus simulations.

## V0.47 - Unified compact log, with ALL gameplay features unchanged (TEST)

User requested **one** log containing essentials, instead of two logs.
This revision modifies only diagnostic output and its filenames.
New per-session canonical filename: `DarksidersGenesisMod.log`.

- DXGI proxy creates/truncates the canonical logfile on the first
  loader event. It logs only early Skip Logos decisions, relevant
  prefix-filter matches/mismatches, ASI load result and errors.
- The proxy deletes the obsolete `DarksidersGenesisLoader.log` if
  present, to avoid confusing next-run stale results.
- Proxy exports `DGUnifiedLogActive` so the ASI can append to the
  existing log rather than erasing early events in `ResetLogFile`.
  If the proxy is unavailable, ASI retains its independent log reset.
- ASI suppresses old, high-volume cursor snapshots, repeated focus,
  camera trace frames, rejected movement-candidate dumps and raw
  repeated HorseCharacter capture traces. It keeps hooks' READY/FAILED,
  exact supported game fingerprint, player capture, setting changes,
  overlay actions, single first combat samples and clean shutdown.
- These filters affect **messages only**. The camera/horse/reticle
  hooks and event semantics are untouched. No new focus workaround.
- New version label V0.47; INI remains unchanged. Current V0.46
  experimental logo-prefix filter is preserved verbatim, not reworked.
- IMPORTANT: user's uploaded logs at 15:03 were still from V0.45,
  so they do not validate V0.46's experimental Skip Logos.
- Keep public release unchanged pending user tests of V0.47.

## V0.48 - Preserve old Loader.log files (requested correction)

V0.47 introduced a single log but unnecessarily deleted the existing
`DarksidersGenesisLoader.log` on process attach. **V0.48 removes that
DeleteFileW operation and its related stale-file path entirely.**
From now on the mod does not CREATE, WRITE, RESET or DELETE
`DarksidersGenesisLoader.log`. An existing old loader log is preserved
byte-for-byte on disk. It is historical data, not an active logfile.

All early proxy and ASI events still go to the single per-session
`DarksidersGenesisMod.log`; the proxy truncates only that file once
per game launch and the ASI appends without discarding early events.
Compact filtering, settings, hooks and the V0.46 experimental selective
Skip Logos implementation remain unchanged. Build uses the existing
root-level four-file ZIP layout; no public tagged release until tested.

## V0.49: user log proves runtime StartupMovies count=2, not 3

V0.47 runtime session on Oct 9 at 15:13 (NOT a V0.48 binary)
shows that the early RVA 0x25FF31 hook executed as intended without a
crash. Log from the existing unified Mod.log:
- "Skip Logos V0.46: exact-name prefix filter requested"
- "exact 7-byte startup copy hook installed"
- ASI reports 0.47-unified-compact-log.
- At native call: "StartupMovies count=2 validPtr=1"
- V0.46 incorrectly logs "FAIL OPEN, unexpected movie count or array",
  because count < 3 is hard-rejected BEFORE logging names.
- Session shuts down normally.

Critical constraint: V0.19B already tried copying a zero-count
StartupMovies array. That experiment removed the independently
controlled intro as well as logos. A naive V0.49 count>=2 change
would repeat that regression, so it is specifically NOT done.

V0.49 extends the existing non-mutating native diagnostic to
allow count=2 for *name reading* while still refusing to empty
the movie array. It prints the two FString entries and whether
they EXACTLY identify THQ_LogoBasic and AS_LogoBasic. If there
are only two entries, even an exact match returns false so the
original native video array and MoviePlayer path remain intact.
The original >2 filter behavior is unchanged. Result is a
focused investigation build, NOT a complete Skip Logos fix.
It preserves V0.48 single Mod.log with compact output, never
creates/deletes/touches any old Loader.log, and contains the
same cumulative gameplay features. When names are confirmed,
research a logo-skip mechanism that keeps the native
MoviePlayer and intro lifecycle (not count-zero, skipping
attachment, or early-function RET patch).

Public release remains unchanged. User should install both
dxgi.dll and ASI from the V0.49 test archive (four files at ZIP
root), launch with SkipLogos=1, then provide only
DarksidersGenesisMod.log.

## V0.50: safe startup and single-log recovery following V0.49 silent run

The user's report after the V0.49 diagnostic is: "il ny a plus de log."
No new runtime log or crash dump was supplied. This alone does NOT
establish whether the proxy was loaded, the ASI was quarantined,
the wrong binary was copied, log-file permissions failed, or the
experimental RVA 0x25FF31 startup hook failed before logging.
Do not claim a specific root cause.

The V0.49 source had a journaling reliability bug: g_loaderLogInitialized
was set TRUE before checking whether CreateFileW succeeded. The ASI
then trusted that boolean when deciding whether to truncate a prior
log. V0.50 changes the flag only AFTER successful CreateFileW and
adds an ASI check that the canonical logfile exists. This avoids
silent log-init success on failed file I/O.

More importantly, the V0.46-V0.49 early handwritten assembly
trampoline is REMOVED entirely from src/dxgi_proxy.cpp. DllMain
does NOTHING except record its HMODULE and call
DisableThreadLibraryCalls. No GetPrivateProfileIntW, no early
VirtualProtect, no early trampoline, no manual executable patch.
All early proxy diagnostics are written outside DllMain on first
DXGI factory/ASI loading, to a single
DarksidersGenesisMod.log (at the dxgi.dll/ASI install directory).

Skip Logos is TEMPORARILY UNAVAILABLE in V0.50; calls to the proxy's
DGSetSkipLogosEnabled return false for enabled=1, log a clear
warning, and leave the engine's MoviePlayer/startup screens native.
An existing user INI with SkipLogos=1 will not re-enable the hook.
The fresh packaged INI has SkipLogos=0. V0.49's observed count=2
remains in docs as evidence; the *last actual working video-skipping
fix is not established*.

All other ASI hooks, camera, horses, HUD, reticle and skip intro
remain unchanged. One compact Mod.log per launch (proxy truncates,
ASI appends); an existing DarksidersGenesisLoader.log is NEVER
created/opened/deleted/modified. This is a crash/log recovery TEST,
not a completed logo fix. Do not create a public release until
the user confirms in-game behavior. If the file is still absent,
diagnose ASI/proxy load/permissions/AV rather than patching
MoviePlayer at random. Four ZIP entries remain at root.

## V0.51 - Undo speculative V0.50 rollback; keep proven log improvements

The user clarified that their earlier report "no log" was a
wrong-file-location mistake. Their subsequently supplied V0.49
single-session log from 2026-10-09 15:39:20 to 15:39:57
proves **both proxy and ASI loaded correctly**:
- 0.49-two-movie-names-diagnostic startup marker.
- StartupMovies count=2.
- entry[0] 'THQ_LogoBasic'.
- entry[1] 'AS_LogoBasic'.
- TWO_LOGOS_CONFIRMED; no removal because earlier count-zero
  patch inadvertently removed the intro cinematic too.
- Healthy gameplay runtime, camera input, player capture and
  normal mod shutdown.

Therefore the *suspected* missing-log failure behind V0.50
is NOT an observed failure of V0.49. Revert ONLY V0.50's
forced disable/removal of the startup movie diagnostic
and restore the exact V0.49 native probe/hook implementation.
Do NOT modify the way it filters, shifts or handles a two-item
playlist. At count=2 it is an observer only, NOT a working
Skip Logos fix, and must be described that way.

Keep these useful improvements from V0.50:
- successful CreateFileW required before setting
  g_loaderLogInitialized.
- ASI checks unified Mod.log file existence before trusting
  proxy readiness.
- One compact per-session DarksidersGenesisMod.log.
- NEVER create, overwrite, or delete old
  DarksidersGenesisLoader.log files.
- No change to existing gameplay, camera, horse, HUD or reticle.

V0.51 is an exact experimental V0.49 startup diagnostic plus
hardened logging, built with SkipLogos=1 in the fresh test INI.
Existing user's INI remains honored; when off, early hook
is not installed. No public release until test validation.

Next development target: safely skip exactly the two confirmed
logo entries WITHOUT zeroing the entire StartupMovies list
or bypassing native MoviePlayer initialization. Avoid
guessing that list count 3+ or modifying the intro toggle.

## V0.52 - Two native startup logos suppressed by substitution (needs game validation)

User's Oct 9 15:50 V0.51 log confirms exact original UE playlist:
count=2, \`THQ_LogoBasic\` and \`AS_LogoBasic\` (in that order).
The previous experiment did not remove either entry.
V0.52 moves from read-only observation to actively omitting the two
logo videos with an explicitly constrained method:
- Leave the native \`UStartupScreensSettings\` and game-owned
  \`StartupMovies\` source array wholly unmodified.
- At exactly the same signature-validated RVA 0x25FF31 early
  copy point proven to run correctly by V0.49 and V0.51, return
  a static two-element FString descriptor array with names
  \`DG_Skipped_THQ_Logo\` and \`DG_Skipped_AS_Logo\`,
  pointing at immutable, process-lifetime UTF-16 strings with
  correct capacity and null-terminated sizes.
- The native \`StartupMovies\` count is left at **2**. The
  original engine-owned deep-copy loop copies these descriptors
  and the original \`GetMoviePlayer()->SetupLoadingScreen\` call
  runs untouched, unlike rejected count-zero V0.19B and
  MoviePlayer-attach bypass V0.19C.
- A mismatch of game fingerprint, instruction bytes, array
  pointer/count or exact logo names yields NO modification.
- For a future unexpected count >2 with the two logos at the
  start, return \`movies+2\` with new count \`count-2\`, keeping
  all other video entries.
- The original x64 island now uses a pointer-returning helper
  and emits \`test rax,rax; je +13; mov rsi,rax;
  cmp r14d,2; je +4; sub r14d,2\`. Old GP register/flag saves,
  shadow space and native continuation are unchanged.
- No V0.44 movie attach hook, synthetic F5, cursor rebind,
  MP4 file modifications, or alternate overlay behavior.
- A nonexistent movie path may cause black frames or a
  media-backend fallback; only the user's in-game test can
  establish timing/intro/focus behavior. It is not described
  as already validated.

V0.52 has a **normal version name** and \`downloads/DarksidersGenesis_V0.52.zip\`;
no "experimental" or "TEST" suffix per user request.
The zip retains exactly four files at root, cumulative features,
unified noncumulative \`DarksidersGenesisMod.log\`, and NEVER touches
\`DarksidersGenesisLoader.log\`. New INI keeps \`SkipLogos=1\`.
Do not tag a public release before user validates in-game:
1. Logos actually absent after fresh boot.
2. Warning screen / loading transition, and intro when
   \`SkipIntroVideos=0\`, remain functional.
3. Cursor/cross after loading a save, F6 reticle/overlay input.
4. No crash, no black screen that hangs indefinitely.
If any issue, set \`SkipLogos=0\` and restart, or restore stable V0.45.

## V0.53 - Freeze V0.52 Skip Logos + Skip Intro; compact UI

The user's V0.52 runtime log (2026-10-09 16:53) confirms:
- the early verified movie-list hook installed and matched
  the exact TWO entries: THQ_LogoBasic and AS_LogoBasic;
- two nonexistent names substituted with nonzero count=2;
- independent native intro CVar applied with SkipIntroVideos=1;
- player identity, reticle hooks, camera, horse hooks, damage
  and movement initialized; normal shutdown.
In-game USER FEEDBACK: reticle appears OK; Skip Intro and Skip Logos
appear to work. The user explicitly asks to FREEZE their
implementations. Therefore V0.53 does not change:
src/dxgi_proxy.cpp, src/SkipLogosFeature.cpp, native Skip Intro
implementation or native reticle handling. Preserve exactly V0.52.

V0.53 normalizes SkipLogos=1 and SkipIntroVideos=1 across
new install INI, C++ Store member defaults, Reset Defaults,
fallback reads, and RuntimeSettings init. ConfigRevision=2103,
with explicit migration for older config revisions:
- Old pre-2102 bypass configurations retain historical safety OFF
  migration (do not reenable the unsafe legacy bypass).
- Existing revision 2102 settings retain user choices, including
  deliberate SkipLogos=0 or SkipIntroVideos=0; updating to
  revision 2103 only synchronizes saved metadata.
- Fresh installs and the Reset Defaults button select both ON.

Overlay: beside each numeric slider's Reset button, show ONE
dimmed LIVE/WAIT indicator, with a hover-only tooltip carrying
technical hook details. Keep rich Combat, Horse, Camera and Recovery
diagnostics accessible through collapsed Debug tree nodes.
System has concise READY/restart status. Native values are not
fabricated and no runtime feature implementation changed.

NEXT FEATURE (not implemented in V0.53): new separate
SkipWarning option to remove exactly the two startup notices
"play with controller" and "autosave warning". They are not
part of StartupMovies (which has exactly the two logo names)
and must NOT be removed by the name-substitution hook. The
startup screen definition array (noted previously at
UStartupScreensSettings +0x50, stride 0x40) is a separate
candidate requiring correct native timing and ownership audit.
Neither DisableStartupScreens nor bypass SetupLoadingScreen is
acceptable since these previously disrupted intro/cursor.
Do not expose a checkbox that promises functionality before
the independent warning suppression is implemented.
Once implemented, choose a user-controlled INI default
separately from the two frozen video skips.

Normal cumulative archive, four root-level files, new V0.53
label, no "TEST" suffix. Unified per-session Mod.log; old
Loader.log untouched. No public release/tag without user request.

## V0.54 Third Person, HUD and hotkey cleanup
Uses user-provided F1 HUD / F2 Movement / F3 Recovery / F4 Reticle and camera controls, all defaults copied to the new INI. Removed Reticle Focus Test action and all synthetic focus pulses. HUD Hidden is the only display toggle, with F1 still supported. Third Person is now an opt-in native camera view transform (OFF by default), using original springarm distance to estimate pivot and apply transient shoulder-height view. May require in-game tuning. Skip Warning will use an independent guarded StartupScreenDef filter, not the movie list. No changes to validated Hide Reticle or Skip Intro logic.

## V0.54 Skip Warning independent attempt and Third Person

User log for V0.53 confirms logos are matched/substituted, the
native intro CVar is set, camera hooks, HUD, reticle, movement and
horse hooks enabled; user INI supplies F1 ToggleHUD, F4 ToggleReticle,
F5-F12 None and camera keys with Home/End + PageUp/PageDown.

The V0.54 Skip Warning checkbox controls a SEPARATE guarded filter
for the startup-screen definition array (settings +0x50; each 0x40
bytes in original disassembly, unrelated to StartupMovies at +0x38).
The known working early 7-byte startup hook passes the original
movie TArray field pointer through volatile r8 as the third argument
to the existing C++ name filter. Before the native MoviePlayer
Setup call, if native definition count is EXACTLY 2 and pointers,
capacity, regions and write permissions are valid, set only
StartupScreenDef.Num=0. Existing actual THQ+AS name replacement and
movie count=2 stay the same. No external files touched. On unexpected
count/layout, warnings remain and a fail-open line is logged.
This is a gameplay *attempt*, not yet validation that controller and
autosave notices are actually controlled by these definitions. An
existing INI can set SkipWarning=0 for the next boot.

Third Person: optional, OFF in the user's provided INI. Native view
and arm hooks already existed; new transient camera output computes
a pivot from original view/pitch/yaw and spring-arm length, then
repositions the view with a shallow -12-degree pitch and a 0.25x-3x
distance multiplier. This may not exactly follow character facing,
and needs real camera test. Switch OFF to restore native framing.

Removed ReticleFocusTest, its synthetic focus pulse, and stale F5
description. HUD Hidden is the only overlay HUD toggle; F1 action
remains. No change to validated reticle suppression/Skip Intro hooks.
User defaults copied into INI. New normal V0.54 build/archive ZIP
has all four files at root; no public tagged release yet.

## V0.55 (9 October 2026): Third Person distance controls, DOF, hotkeys, Skip Warning

- Third Person Distance now has a manual numeric input, a 1.00x Default button, 0.25x–3.00x limits, and finite-value validation. F5 toggles Third Person by default; INI ConfigRevision 2105 migrates a V0.54 F5=None to this new default without changing other user bindings.
- Every working binary feature toggle is selectable in the F1–F12 Hotkeys list, including HUD, Reticle, Movement, Recovery, Skip Intro, Skip Logos, Skip Warning, Third Person, FOV, Pistol/Melee Damage, Jump, Glide, Horse Speed/Sprint Speed/Sprint Duration, Hotstreak, eight camera directions, and Camera Reset.
- Depth of Field: while Third Person or manual Zoom displaces the camera, the mod temporarily sets the verified native UE4 console variable r.DepthOfFieldQuality to 0, restoring its captured native value when both modes are off (and at normal shutdown). This avoids incorrect blur but disables DOF while modified; it is not a physical focal-distance recalculation. The CVar registration signature and storage were confirmed against the supplied exact game EXE. No Engine.ini write.
- Skip Warning V0.54 did not work because its array adjustment ran after the native screen-definition loop. V0.55 moves the intercept to a distinct exact-byte-validated native sequence at RVA 0x25FEBF, replaying the original instructions and skipping that loop only if exactly two 0x40-byte screen definitions exist. Neither source array nor MoviePlayer is modified. Outcome needs in-game validation. Disable via SkipWarning=0 and restart to isolate.
- Skip Logos name substitution, Skip Intro CVar, reticle, horse and gameplay internals remain unchanged. ZIP at root contains dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Only DarksidersGenesisMod.log is active per run; no touching the old Loader.log. No public release until validated.

## V0.56 (9 Oct 2026) - Third Person pose, mouse/right-stick orbit, warning native span audit

- User validated V0.55 DOF: native quality 2 -> 0 when Third Person enabled, 0 -> 2 when disabled. Keep V0.55 DOF unchanged.
- Expose previously fixed Third Person pitch -12 degrees, vertical offset +60 Unreal units and existing distance multiplier in Camera tab; reset controls and INI persistence. Preserve manual camera height too.
- CameraMouseGamepad=0 by default, optional right-stick XInput (dead-zone) and RAWINPUT mouse with WM_MOUSEMOVE fallback. No game input consumption, no cursor synthesis. Accumulated orbit offsets are session-only and reset when disabled/unfocused/menu open; sensitivity and stick speed persist. The WndProc observer is installed on demand without initializing ImGui. In-game camera control requires testing.
- Skip Warning V0.55 observed its native loop but failed count=2 guard. V0.56 records the actual byte span and preserves original RFLAGS/RAX/RDX; one/two 0x40-byte definition counts are allowed to bypass, all other counts fail open. Still experimental until warning pages disappear in-game. No original movie array, intro, reticle or native UObject writes.
- ConfigRevision=2106 migrates user settings without changing existing hotkeys; 4 files at ZIP root; no tagged release until validated.

## V0.57 (9 October 2026) - Isolate original aiming from Third Person camera

User feedback: V0.56 Third Person controls work very well. The Depth of Field compensation remains validated. Skip Warning still DOES NOT WORK, and is deliberately unchanged in this build (pending separate diagnosis). The native aiming visual effect remains for future work; it is NOT hidden in V0.57.

When Third Person is active, gameplay focused and ImGui closed, the camera input observer now consumes native WM_MOUSEMOVE and mouse WM_INPUT notifications after capturing the movement for our orbit camera. This does not intercept mouse buttons, clicks, scrolling, keyboard input, focus or Alt-Tab. The observer attaches on Third Person activation regardless of whether camera mouse/gamepad input is enabled; it is not a global Windows input blocker. All events are passed through again when Third Person is off, menu open, or focus lost. Native GetCursorPos and direct UE player aim logic are NOT hooked, so an absolute-position aiming path may still rotate the player; do not claim complete native aim suppression without in-game test.

A separately scoped MinHook filter masks only XInput X/Y right-thumbstick values exposed to the game while Third Person is on. Left stick, buttons, triggers and other inputs remain intact. The mod's orbit camera polls the unfiltered original XInput trampoline. Tries xinput1_4, xinput1_3, xinput9_1_0, xinputuap and GetStateEx ordinal 100. If the game reads controller input through Steam Input/HID instead, this method may not catch it; aim filter counts in the log identify whether the XInput gate was exercised.

Both filters fail open outside the exact validated game and are disabled when the overlay is open or game unfocused. Counters are printed only when first exercised, at 10,000 messages, and on orderly shutdown. V0.56 camera pose, DOF, skip logos/intro, reticle, horse and gameplay mechanics are left unchanged. Install test ZIP with 4 root-level files. No public stable release without in-game validation.

## V0.58 (9 October 2026) - controller TPS combat axis test (UNVALIDATED)

User confirmed V0.57 filtering via log: mouse native move messages suppressed 9,777, right stick native XInput reads filtered 26,909. This proves the game actually polls XInput 1.3/1.4, and is a safer integration point than writing actor rotations or movement-component flags without validated native offsets. Skip Warning still FAILS: loader span is 0xC0 (three 0x40-byte definitions), so V0.56's 1/2 definition gate does not skip; **no changes to Skip Warning** in this build.

User requests a real controller TPS: move camera with right stick while firing/throwing, aim in the facing direction, and move backward/sideways with the left stick. The existing native right-stick filter now synthesizes the engine's own directional aim input during RT (fire / throw) and RB (secondary fire). Desired direction uses the validated camera's transient applied yaw relative to vanilla native yaw: RX=sin(delta)*32767, RY=cos(delta)*32767. Left stick is rotated by the same camera delta in the native game's input frame (X'=LX*cos(delta)+LY*sin(delta), Y'=LY*cos(delta)-LX*sin(delta)). Input magnitude is preserved, avoiding direct player actor writes. Controller 0 only; LB ability menu and all other controls remain vanilla. Outside active fire/throw, V0.57 right-stick isolation remains. A camera calibration yaw adjustment -180..180 degrees is exposed for game-specific aim mapping if axes differ.

New [Features] TPSControllerCombatAim=1 and TPSControllerStrafe=1, [Values] TPSAimYawOffsetDegrees=0, ConfigRevision=2107, all active ONLY with ThirdPerson ON + foreground gameplay and exact supported game. The user can independently disable either experimental mapping in Camera. Applies to gamepad only; no mouse aim change. No assumptions about static player pointers or native yaw offsets are introduced. V0.56 pose, validated DOF, reticle, HUD, horses, intro and skip logos remain as they were.

WARNING / test limitation: this is an XInput-level prototype, **not** a proven native SetActorRotation or movement-component orientation-to-movement implementation. Native game action code must actually rotate Strife from the synthesized right-stick aim, and movement must retain lateral facing. If game auto-orients the pawn to moving direction, a follow-up *native actor/movement rotation hook* is required after exact executable disassembly; do not claim true TPS strafe yet. Test camera facing while shooting with RT, RB, throwing held item, left-stick strafing while RT held, LB radial selection, Third Person OFF and F5 and Alt-Tab. New log TPS V0.58 records first 1/120 firing samples, strafe samples and end totals. Request game observations, rather than more memory writes based on guesses. Workflow artifact is delivered via GitHub Actions artifacts by user preference, not chat sandbox.

## V0.59 (9 October 2026): exact executable audit, absolute-world yaw & live facing probe (EXPERIMENTAL)

User supplied the exact exe bytes `DarksidersGenesis-Win64-Shipping(1).exe` (62,113,280 B, SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54). Native PE .text/RDATA reflection audit confirms `GetNormalizedAimRotation` native generated dispatchers and actual functions at RVA `0x638240` and `0x570260`. Both implementations read `RootComponent` from Actor `+0x158`, a cached component FRotator at root `+0x1F0..0x1F8`, and actor aim state at `+0x900/+0x904`. These getters calculate angle differences. They are **not** safe, verified SetActorRotation entrypoints and must not be used as setters. `GetPlayerLookRotation` is present in native registration, but its owner/callers require further audit. No arbitrary actor rotation writes introduced.

V0.59 collects READ-ONLY local actor worldYaw from root+0x1F4 inside the already validated live player movement GetMaxSpeed callback (in-game validity predicate from jump/double-jump/glide). It checks allocated memory with VirtualQuery, finite angle and clips into -180..180; retains only float+tick. Zero cached UObject pointer derefs from the D3D11 Present or XInput hooks. Log first, 120th, 10000th player world yaw reading. Aim logs snapshot `actorYaw` and `actorFresh` if sample is within 200ms.

V0.58 used `appliedCameraYaw-nativeCameraYaw` as a WORLD aim target; this changes dramatically when the native camera reorients. V0.59 **separates** world-space desired aim and relative camera movement: combat right-stick synthetic target is now `appliedCameraYaw+TPSAimYawOffsetDegrees` (absolute world yaw), fed as sin/cos at 72% magnitude, rate capped at 180deg/s from last aim command; initial heading seeds from latest validated actor yaw when available. Relative applied-native yaw is retained only to transform camera-relative LEFT stick input. No writes to native player orientation or camera UObject. This is a controlled attempt with additional runtime diagnostics, NOT proven to achieve the requested TPS facing/strafe. Third Person OFF or menu/focus loss reverts to game native inputs. Existing V0.57 Win32/XInput input gates, validated DOF/horse/reticle etc preserved.

Test: third-person camera enabled, RT shooting still camera vs while moving sideways, RB secondary, grenade throw, standing turn, release RT, F5 OFF. Examine `TPS V0.59 combat sample`, `TPS V0.59 live actor facing` and outcome; if actorYaw remains stale or orientation misaligns, next implementation must hook authoritative combat facing or native movement orientation and verify with a video. Skip Warning remains *nonfunctional*, span 0xC0 three startup entries, unchanged for this version.

## V0.60 (9 October 2026): left stick safety rollback + TPS aim paced by camera (TEST)

User rejected V0.59: pushing LEFT STICK UP can make player travel RIGHT, and camera and actor turning are not well synchronized. V0.59 log (20261009-200302) verifies player yaw read 14,855 times. First RT sample: camera=-74.0, native=-45, command=-94.2, actor=-97.1, error camera-actor = +23.1 degrees. Sample #120: camera=+2.2, command=+2.2, actor=+0.2, error +2.0 deg. Therefore actor rotation is updated and converges; remaining early discrepancy + unpredictable left-stick behavior must be isolated.

**Cause / mitigation:** V0.58/0.59 rotated BOTH left stick axes with a per-frame camera-relative delta despite the game having its own movement-to-heading mapping. This can effectively rotate "forward" into a "rightward" analog input, especially at 90-degree camera orbit. V0.60 disables `TPSControllerStrafe` by default, including a **targeted migration from revision 2107 to 2108**. Native left stick remains byte-for-byte unchanged. The old transformation is retained ONLY when user explicitly enables EXPERIMENTAL left-stick rotation in Camera, and then ONLY during RT/RB attack so normal locomotion is unaffected. The goal is to verify whether the native combat facing itself already yields correct forward/strafe movement when the native right stick is driven by TPS camera heading.

**Aim pacing:** absolute camera world yaw continues V0.59, but cap command step to the configured `CameraStickSpeed` (30..180 deg/s, default 135 deg/s), rather than fixed 180 deg/s faster than camera. Synthetic native aim-stick length reduced from 23592 (72%) to 18022 (55%), still well above gamepad deadzone. Existing actor-yaw probe, boot/HUD/reticle/horse/DOF/skip logos and intro remain unchanged. Logs now include left stick raw passthrough first/120/10000, camera world yaw vs live actor yaw, and final counts including moving while firing.

**LIMITATION:** this patch removes a provably wrong extra transform but does not guarantee a true native character-facing lock or strafing animation. If native gameplay reorients actor toward walk direction during firing, the next fix must hook the authoritative in-game control rotation/movement orientation, based on disassembly with exact EXE, rather than another synthetic stick transformation. Test priority: ThirdPerson ON, rightstick orbit, RT fire while leftstick UP, then LEFT/RIGHT/BACK, then release RT and move. Leave EXPERIMENTAL rotation OFF. A full game exit supplies totals and early yaw samples. Skip Warning stays nonfunctional (`0xC0` span, three definitions) and unchanged. This is V0.60 TEST, not release.

## V0.61 (9 October 2026): native TPS combat facing, camera pivot stabilization and aim ground FX test

V0.60 user result: strafe while firing still absent (moving camera can make it look like strafing); vanilla visual aiming effect and aiming camera lateral soft offset disturb TPS. Log: 3420 combat polls, 7010 native left stick passthrough, user explicitly enabled experimental remap 868 times, first RT cameraYaw=-45 vs actorYaw=+44.4 (gap -89.4). The previous XInput axis-rotation approach cannot independently control native character facing.

Exact supplied game EXE reflective disassembly: `bOrientRotationToMovement` UCharacterMovementComponent native generated setter at RVA `0x1CF0300` has bytes `80 89 40 02 00 00 10 C3`: OR bit 0x10 at movement+0x240. V0.61 in the *validated live local-player GetMaxSpeed hook* clears only this bit during ThirdPerson RT/RB firing, then restores bit if previously owned once fire stops/TP off/overlay appears; never dereferences stale cached components. [Features] `TPSLockCombatFacing=1`, opt-out; default left-stick XInput passthrough unchanged. Does not guarantee true TPS until user tests.

[Features] `TPSLockCombatCameraPivot=1` stabilizes the native springarm camera target during RT/RB, using last idle camera-to-live-player XY relative offset, refreshed only from live local-player ComponentToWorld.Translation (root+0x1D0..1D8), with 250ms actor freshness, 12s baseline, world plausibility checks, and reset on actor identity generation. Does not freeze camera in the world when the player moves; only avoids native top-down aim target side-drift. OFF returns V0.60 transient camera positioning.

[Features] `TPSHideGroundAimFx=0` optional EXPERIMENTAL native effect hook at RVA `0x6DDAD0` (`HideGroundTargetingEffect(bool)` verified bytes). Forces hide=true ONLY when this function is called during ThirdPerson. May hide ground grenade/item aim effect. **NOT PROVEN to affect Strife gun laser or generic vanilla aiming FX.** No indiscriminate FX disabling. Hook doesn't retain effect actor pointer; native effects may only restore when native visibility call happens. Existing F4 reticle remains independent. UI exposes separately all three options and preserves cumulative features.

INI revision 2109 migrates previously enabled experimental left-stick reorientation back OFF to prevent double transforms. Skip Warning known to fail on three 0x40 definitions (span 0xC0), no change. V0.61 TEST pending user feedback with native-facing/restore, pivot compensation and ground aim hook log lines; no official Release.

## V0.62 (9 October 2026): full-time TPS facing test (unvalidated)

User request: "il faut tester d'avoir le personnage toujours dos à la camera". V0.61 activates native `bOrientRotationToMovement` bit clearing only while RT/RB held. Log previously showed 25 locks/25 restores and large yaw error despite gamepad synthetic combat aim. V0.62 tests keeping Strife's **native facing directed by camera all the time**, whether idle, moving forward/backward/sideways or firing.

New `TPSAlwaysFaceCamera=1` (ConfigRevision 2110), with an independent overlay checkbox in Camera. ON only in ThirdPerson with `CameraMouseGamepad=1`, focus on exact supported executable, closed ImGui, controller index 0, and fresh validated live player sample <200ms. The XInput native aim path is now synthesized continuously (not only during RT/RB), using existing camera absolute yaw, rate-limited to configured camera stick speed, and 55%-magnitude native right stick. The camera samples the actual original controller right stick through XInput trampoline, unaffected by this. The authoritative movement `bOrientRotationToMovement` bit (+0x240, mask 0x10) is cleared in the *validated live player GetMaxSpeed callback* while recent (220ms) camera-facing aim pulses exist, allowing left stick to strafe without reorienting to movement. Only owned flag is restored on the next valid native callback after TPS OFF, option OFF, game focus/overlay loss or inactivity; never restore through stale cached UObject pointer. While locked, if the game reasserts the bit, it is cleared again and separately counted. Left stick remains **unchanged**; old experimental left-stick remap stays OFF.

New logs `TPS V0.62: always face sample` include cameraYaw, real actorYaw, signed yaw error, commanded yaw, left axes, firing state; final command/reassert totals. In-game test must check idle rotation, moving in all directions, shooting, LB wheel, F5 OFF, old effect indicators and camera pivot. This is still a synthetic native aim route, NOT verified direct actor rotation. It may cause the native vanilla aim visual outside shooting, especially until that visual is identified properly, and may not perfectly lock actor orientation depending on native aim code. Fail-open if no active validated player/controller/camera; in-game test result essential before declaring functional. Existing ThirdPerson pose, native visual ground FX option, camera pivot stabilization, FOV, DOF, horse, combat damage, HUD, reticle, intro skip and logos unchanged. Skip Warning still does NOT WORK and is unchanged.

## V0.63 (10 October 2026): rejected always-face reverted; camera pose defaults & native facing cone

User rejected V0.62 Always Face Camera: it simulates Ctrl-like permanent aim until dodge and restricts some movement. Log showed `alwaysFaceCommands=15855` even when RT=RB=0. All idle synthetic right-stick input is removed; TPS combat synthetic aiming is active only with RT/RB and `TPSControllerCombatAim=1` (default ON), while existing V0.57 idle input isolation stays. Experimental left-stick rotation was observed 3542 times in the user's log; implementation, UI and INI option are removed completely. Native LX/LY are always unchanged.

New [Features] `TPSCameraFacingGuard=1` and [Values] `TPSFacingToleranceDegrees=90` (20..180): in the exact validated live-player movement callback, compare actual player root yaw against final camera yaw. Inside this 90-degree cone, temporarily disable `bOrientRotationToMovement` bit +0x240/0x10 to reduce automatic movement-oriented turning without forced aiming. Outside the cone restore the native bit and allow vanilla orientation; combat lock from V0.61 still applies while RT/RB. **This does not directly rotate the character** and is experimental; only full game testing can validate strafing and dodge. Log counts cone IN/OUT, owned lock/restores.

New Third Person Distance range 0.0..3.0, default 0.50 (0 radial arm distance now genuinely allowed by camera calculation, old clamp min 120 removed). Vertical offset default +180 (was +60). Overlay Default controls and F-key Camera Reset updated. ConfigRevision 2111 migration updates only unchanged prior defaults 1.0/60.0 and preserves customized values; removes old deprecated INI keys.

Hide Vanilla Ground Aiming Effect test was nonfunctional (native hook installed yet zero calls). Remove that misleading option and hook while continuing future search for the actual visual effect path. Other camera pivot stabilization, DOF, horses, HUD, reticle, skip intros/logos unchanged. Skip Warning still fails span 0xC0 and remains deferred. Build is V0.63 TEST pending validation.


## V0.64 (10 Oct 2026): TPS player-follow camera, zone camera baseline, independent TPS FOV, input separation (TEST)

User findings after V0.63: region triggers override native camera location/angles/zoom; some global camera sliders appear not to work while ThirdPerson is active; mouse reticle aim is grounded in top-down world-plane projection and may jump between front/back at a horizontal projection boundary; right stick originally rotates player; need player-anchored camera and separate TPS FOV. These are distinct systems. **Do not claim a fix for mouse weapon aim** until the game's mouse-target world projection, shot origin and weapon traces have been identified. Former V0.62 Always Face rejected (simulates held Ctrl). Never reintroduce permanent native aim commands.

**[Features] TPSFOV=1** and **[Values] TPSFOVDegrees=90.0** (40..140) are independent from the existing global FOV override. When ThirdPerson=1 AND TPSFOV=1, TP FOV wins regardless of global FOV. Otherwise the global FOV setting works exactly as before, or native FOV if global also OFF. Observable `TPS V0.64: independent FOV` logs. The ordinary FOV override is still usable outside TP.

**[Features] TPSFollowPlayer=1**: in the validated local player's native GetMaxSpeed callback, previously existing read-only root ComponentToWorld.Translation (X/Y/Z) + update tick + generation are published. Camera V0.64 uses that fresh (max 250ms) actor XYZ to construct FMinimalViewInfo.Location: pivot = player XYZ; output camera = pivot - cameraForward * TPSDistance + TPSVerticalOffset and global CameraHeightOffset. Rotation is the TPS view pitch and yaw. It does **not** install SetViewTargetWithBlend, reparent UCameraComponent, set camera UObject transforms or preserve stale player pointers. This is an effectively player-attached *transient camera view*, with fallback to V0.63 camera reconstruction if no validated live player. Guarding native output proximity (XY under 15000 UE units, Z under 7000) avoids overriding distant cinematic camera views. The camera's native collision/SpringArm trace is not guaranteed to continue as before, because final rendered view location is reconstructed, so test collisions/walls.

**[Features] TPSLockZoneCamera=1** (effective only when TPSFollowPlayer=1): freeze a baseline native camera yaw + SpringArm distance at first validated nearby native gameplay view per player generation. Derive TPS yaw from that baseline plus existing cameraYawDegrees and player orbitYaw; derive TP distance from captured native arm length * ThirdPersonDistanceMultiplier. Region-triggered native yaw/distance/target-pivot changes then no longer drive the TPS rendered view. Resets on ThirdPerson OFF or player generation changes. Does NOT patch the game's camera zones, triggers, assets, camera volumes or cinematic state; only replaces the final transient gameplay camera output when proximity/player validation passes. If TPSFollowPlayer=0, old native-derived camera algorithm and old TPSLockCombatCameraPivot are retained. With follow enabled, TPSLockCombatCameraPivot is redundant; UI explains these interactions. Generic CameraPitchDegrees gets overridden by View pitch##TP. Native SpringArm Zoom primarily affects vanilla/fallback camera, NOT attached zone-locked TP; these old controls are NOT missing code, but superseded by TPS mode.

**[Features] TPSSuppressNativeRightStick=1**: independently toggle the V0.57 original XInput filtering. When ThirdPerson and overlay closed and player 0, original controller right stick is blocked from native character-yaw interpretation outside RT/RB; our camera still reads RAW XInput via native original trampoline for orbit. OFF lets native right stick also rotate character; selection wheel LB, co-op, unfocused/TP OFF remain vanilla. Actual RT/RB combat aiming when TPSControllerCombatAim=1 continues its own native synthetic aim behavior independently.

**[Features] TPSSuppressVanillaMouseAim=1**: independently toggle V0.57 Win32 WM_MOUSEMOVE/WM_INPUT suppression; our mouse orbit can still read original relative mouse deltas, but this does *not* neutralize all game mouse aiming paths, absolute cursor world projection or targeting. Mouse and gamepad aim remain fundamentally different, and visual "horizontal line" / target front/back switch is unresolved. Treat mouse control as an open native target-projection task, not as a patch victory.

**[Values] TPSCombatAimTurnRate=180.0** (30..360), now separate from CameraStickSpeed (camera orbit) because character aiming and camera rotation are different systems; only native synthetic RT/RB aim uses this rate limiter. No forced aim outside actual RT/RB. Reticle suppression F4 remains separate.

**Config revision fix:** SaveNow historically writes literal ConfigRevision=2106 despite current kConfigRevision increasing to 2111, causing migration on future loads. V0.64 writes current revision 2112 using swprintf_s, preserving hotkeys and all calibrated existing settings. New flags default to 1 as indicated and new values default to 90 FOV / 180 aim turn. TP distance 0..3 default 0.5 and TP vertical offset default +180 retained.

V0.64 test tasks: activate F5 TPS; toggle FPS FOV against global FOV; walk/rotate camera and confirm player remains framing center; cross one known camera-changing zone and verify angle/distance stable; walk close to wall and check collisions; test right stick native-suppression on/off while camera orbit remains; test mouse aiming separately and log what remains broken; open/close overlay, co-op, mounts, cutscenes, Alt-Tab; TPS OFF returns original. The local player camera-follow is conditional and not a true camera UObject attachment, so this test is **not** a validated final fix. Existing horse/DOF/HUD/reticle/logo/intro code retained. Skip Warning still nonfunctional (0xC0 native span count 3), not changed.


## V0.65 (10 October 2026): simplified foot-centered TPS camera TEST

**User goal:** replace the collection of competing Third Person camera tweaks with a centered, player-linked camera, suppress native camera-zone changes except cutscenes, keep the native right stick from rotating Strife, and place the reticle at the screen center. Previous V0.64 gameplay NOT validated.

**Camera:** exact game executable only. After game GetCameraView, use an independent transient FMinimalViewInfo pose computed from the real local-player root XYZ. Pivot XY = player root X/Y. Pivot Z = rootZ - [Values] TPSFootAnchorOffset (default 88 Unreal units), then + ThirdPersonHeightOffset (default 180). This foot correction is **approximate**, since exact capsule half-height has not been validated; editable 0..200 in overlay. The orbit reference yaw is the actual player yaw captured when TPS camera mode first locks, **not** the top-down native camera yaw. Right stick/mouse orbit yaw/pitch act relative to that initial direction. Native SpringArm distance is sampled once per player generation; camera-zone yaw, XY aim drift and zoom changes should no longer alter the attached TPS view. Character rotation is NOT forcibly tied to camera orbit.

**Cutscene fail-open:** native view is returned without any TPS/FOV/reticle transformation if player data is stale >300 ms, native camera is farther than 6500 units XY or 7500 Z from player, or native pitch is outside [-89,-15] degrees. This is only a gameplay/cutscene **heuristic**, not direct cinematic-state detection. A near-player cinematic using a similar top-down pitch can still be mistaken for gameplay. Persistent UCameraComponent/SpringArm state is not written. Collision with walls is NOT automatically guaranteed for our final override.

**Center reticle:** [Features] TPSFixedCenterReticle=1 draws lightweight ImGui crosshair at exact viewport center while TPS gameplay camera is active and the game is foreground. Existing native UI cursor-visibility hook and SetCursor hook blank the vanilla cursor while this display is active. No mouse-warp or button intercept is used. This is a *visual* fix only: actual mouse screen-to-world targeting and bullet direction still use vanilla game aim rules and need separate analysis. Draw initialization stays lazy: triggered only by opened menu OR active fixed TPS reticle. Cinematics and TPS OFF hide the crosshair; independent existing Hide Reticle / Hide HUD still take priority.

**Inputs:** Controller's original right stick is filtered from native player rotation via existing TPSSuppressNativeRightStick=1 while idle, but is separately read unfiltered for camera orbit. TPSControllerCombatAim=1 remains enabled while RT/RB firing, which can intentionally steer native shot aim and the player while attacking; no permanent virtual Ctrl aim as rejected V0.62. CameraMouseGamepad defaults ON (migrates existing revision <=2112). Superseded experimental movement-facing cone and native soft-aim-pivot correction now default OFF and are removed from primary TPS UI, retained as INI-only compatibility diagnostics.

**ConfigRevision=2113.** Preserve existing FOV TPS override, other mod features, horse, HUD/reticle, intro/logo skips, hotkeys. Skip Warning is still broken and unchanged. Test: idle/walk/dodge player screen center, rotate camera, adjust foot offset, zoom/distance/height, move through camera zone, approach walls, launch near/far cutscene and return, mouse/controller input, overlay, Alt-Tab, TPS OFF. Test-build only, not official Release. Versioned permanent ZIP + rolling latest test prerelease.


## V0.66 (10 October 2026) TEST: camera eligibility repair & native walking strafe

USER BUG REPORT V0.65: "la camera ne change pas / avec la manette les pas de coter ne marche pas". User's V0.65 log `DarksidersGenesisMod(20261010-185350).log` proves the exact-supported EXE and hooks READY, local player movement validated, 8,062 actor-yaw samples, but `TPS V0.65: native camera returned to game (cinematic/invalid player) n=1 near=0 pitch=-45.0`, and NO recorded foot camera attached / independent TPS FOV line. The V0.65 gameplay guard refused native top-down camera because of its tight 6500 XY / 7500 Z proximity, so camera remained vanilla. Also 374 original left-stick samples, only one native strafe lock/restoration while actually firing, meaning no persistent movement-facing lock for passive strafing. Mouse input filtered 5,345, right-stick filtered 76. Do not call these fixes verified until tested.

### Camera eligibility repair
V0.66 checks the native playable top-down camera signature: fresh exact live player world coordinates + native pitch [-89,-15] + either proximity within 6500 XY/7500Z OR ordinary game native pitch [-55,-35] and FOV [60,85]. This **rescues the player's valid gameplay camera even if native view-to-player distance is larger than 6500**. At the first few rescued or rejected frames, logs now print native camera and player XYZ, XY and Z gaps, FOV and pitch. This is a diagnostic fallback and **not a proven native cutscene detector**: a cutscene sharing identical camera pitch/FOV may be mistaken for gameplay. Conversely unusual game camera angles still restore vanilla. No persistent camera UObject changes. Also, if no SpringArm value is captured yet, use a one-time 900-UU base arm for TPS so missing arm telemetry can no longer silently block all TPS changes; captured native arm is retained when available. Keeps V0.65 player-root-to-foot pivot and independent TPS FOV.

V0.65 reset `tpsViewGameplay=false` at start of every GetCameraView, including secondary non-game views. V0.66 only clears it when TPS is disabled, then uses the existing fresh successful view timestamp (<250ms in Present) for crosshair visibility; avoids other view samples cancelling the valid game view in the same frame.

### Controller strafe test
New [Features] `TPSStrafeLock=1` (ON by default) and overlay toggle `Keep player facing while strafing (TEST)`. In the validated live local-player GetMaxSpeed callback, if Third Person is ON, the game is focused, ImGui menu closed, and MovementMode is Walking (1) or NavWalking (2), clear only **bOrientRotationToMovement** bit +0x240 mask 0x10 so left-stick movement cannot automatically reorient player to walk direction. Restored when TPS OFF, option OFF, overlay/focus loss or movement mode changes on next valid callback, without writing stale UObject pointers. Unlike rejected V0.62 mode, DO NOT inject persistent CTRL or virtual right-stick aiming; continue original left-stick passthrough and original right-stick camera-only input outside RT/RB. Actual firing still retains its separate TPSControllerCombatAim option. Log strafeLockTicks and native flags plus actorYaw/cameraYaw.

This suppresses only one native turning mechanism, not necessarily every animator/ability/pawn-rotation path. Confirm real backward/sideway movement with controller **without firing**, during shooting, after dodges and when switching to non-TPS mode; report failures instead of assuming success. Old V0.65 fixed crosshair and foot correction are kept and remain display-only. Avoid claiming weapon aim follows the crosshair; mouse aim world-ray remains outstanding.

Build version V0.66, ConfigRevision=2114 (backward compatible, old INI defaults preserved), ZIP files at root, rolling latest TEST prerelease and versioned downloads V0.66; official Release unchanged and still needs user validation. Skip Warning still non-functional.


## V0.67 (10 October 2026): correct native root component world translation (TEST)

**User V0.66 result:** camera goes to a remote point on map and does not follow the moving player. User log `DarksidersGenesisMod(20261010-190828).log` shows explicit native view (31838,-6410,3024), reported actor world location **(0,0,0)**, synthetic TPS foot view (-389,966,313), repeated with 32,477-unit planar gap. This definitively rules out a mere spring-arm zoom, FOV or player yaw issue. Mod was attaching to wrong coordinates.

**EXE-backed root cause proof:** user-supplied exact shipping executable SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Native assembly at RVA **0x669728** loads RootComponent from `Actor+0x158`, and at RVA **0x66976C** executes `movups xmm1, XMMWORD PTR [rax+0x1A0]` and extracts X/Y/Z scalar via `movss` + `shufps`; this is a grounded read of world translation. Prior V0.64-0.66 code incorrectly assumed world translation at `root+0x1D0/+0x1D4/+0x1D8`, which silently returned 0/0/0. V0.67 reads `root+0x1A0,+0x1A4,+0x1A8`, only inside the existing validated live-player GetMaxSpeed callback and with page/finite-value guards; no saved UObject pointer writes. Logs first, 120, 1000, 10000 `TPS V0.67: live actor transform` with real world coordinates; TPS camera uses these coordinates directly, so movement can actually carry the camera. Player yaw getter remains unchanged (root+0x1F4).

V0.66 workaround that accepted a far-away native view solely because pitch was -45 and FOV 70 is now **removed**; synthetic TPS only writes when fresh validated player coordinates are within 6500 XY and 7500 Z of native view and pitch in [-89,-15]. This protects arbitrary remote/cinematic viewpoints and prevents accidental teleporting. Still a heuristic, not explicit cutscene detection. No angle/height/zoom/FOV changes, no changes to native right stick, TPSReticle/face while strafing logic, mouse input or all other already present mod features. Strafe lock was active 4,908 native movement callbacks in V0.66 but user has not validated genuine strafing; do not describe it as resolved. V0.67's sole runtime objective is correct live-player world coordinates and stable player-following TPS camera. The old F4 reticle and Skip Warning (still ineffective) are unchanged.

This is experimental V0.67 TEST with ConfigRevision unchanged (2114), validated Windows Actions build required, versioned downloads archive plus rolling prerelease latest TEST, no official release upgrade before in-game validation.


## V0.68 (10 October 2026): TPS input simplification + rebindable L3 recenter TEST

User feedback on V0.67 (supplied `DarksidersGenesisMod(20261010-193310).log`): camera now correctly follows actor root real XYZ; controller native right stick is suppressed except while shooting because `TPSControllerCombatAim` **injects artificial right-stick axes during RT/RB**, undoing suppression. User explicitly rejects Ignore Zone Camera Changes option, fixed center drawn crosshair, Keep Player Facing While Strafing, Native Strafe While Shooting, and other obsolete facing-cone/pivot/aim calibration. Mouse aim remains projected onto **flat native top-down world plane**, so a shot at a wall can go to vanilla mouse reticle location rather than TPS camera forward. This requires a native shot target/trace direction detour not available in current hooks. Do not describe firing as fixed by recentering the UI or hiding a cursor.

V0.68 implements:
- Removed TPSControllerCombatAim synthetic native right stick on ALL RT/RB. XInput right stick filtering now **always** clears right-axis data on controller #1 during active TPS even when shooting, while our orbit polls original raw XInput through existing trampoline. Native LB wheel and co-op #2 continue vanilla. Mouse and right stick camera orbit remain unchanged. **Shots still use native game targeting** and are NOT forced onto camera forward; native projectile target hook remains TOP PRIORITY.
- Retired V0.65 fixed ImGui central crosshair completely. Restore lazy ImGui only when Insert opens, no extra D3D overlay render frame on TPS gameplay. Hide OS/UI cursor automatically during valid active TPS gameplay while preserving F4 Hide Reticle for vanilla outside TPS; do not draw any replacement reticle. Actual native HUD reticle element may still require further removal research.
- Removed all user-facing obsolete settings and cleaned them on INI SaveNow: `TPSFixedCenterReticle`, `TPSLockZoneCamera` (its switch only, player-camera reference stays naturally stable), `TPSControllerCombatAim`, `TPSLockCombatFacing`, `TPSStrafeLock`, `TPSCameraFacingGuard`, `TPSLockCombatCameraPivot`, `TPSCombatAimTurnRate`, `TPSAimYawOffsetDegrees`, `TPSFacingToleranceDegrees`. Legacy native facing-bit override now **only restores** any owned bit on a live validated player, no longer forces "strafe". No injected aim/CTRL. FOV and stable TPS camera pose preserved.
- Rebindable controller button via `[Controls] TPSRecenterButtonMask=64` (default `XINPUT_GAMEPAD_LEFT_THUMB`, L3). Overlay camera control Combo offers L3, R3, LB, RB, A and D-pad directions. On a rising edge from unmodified controller #1, `camera_trace::RecenterOnPlayer()` computes `orbitYaw = currentActorYaw - capturedBaseYaw - manualCameraYaw`, resets orbitPitch to 0, without moving the player, modifying game camera components, or changing TPS distance. Requires a fresh player transform and camera baseline. Button gameplay function remains native (the rebind may overlap gameplay action). Supports hot-rebinding by overlay; changes persisted to INI.
- [Meta] `ConfigRevision=2115`: migrate saved INI safely, delete retired keys and preserve existing player settings/hotkeys; no other validated mod features changed.

Test: TPS ON (F5), orbit with right stick while RT/RB firing and idle, verify character is never turned by right stick; L3 click recenter behind current facing, verify rebind via Insert; game remains functional in LB wheel and no forced strafing; vanilla on TPS OFF. Mouse shot-world-plane issue intentionally tracked and uncorrected; log to identify native aim computation separately. Preserve official release V0.29, build V0.68 as TEST, archive with 4 root files, rolling test prerelease + permanent versioned ZIP.


## V0.69 (10 October 2026) TEST: mouse passthrough and controller fire gate ONLY

User report V0.68: when `Block vanilla right-stick player rotation` is disabled, holding RT/RB should **stop the mod's gamepad camera orbit** so the native game can use its right stick without both systems rotating; when block is enabled, user reports inconsistent results and requests a fix. `Isolate native flat mouse aiming movement` appears broken and player fires at arbitrary locations. Preserve all other features unchanged.

**Implemented only these two input fixes:**
- Disabled + fire RT or RB (controller #1): suppress OUR **right-stick orbit input only** for the duration of fire, using the unfiltered XInput state already read by `UpdateOrbitInput`. Native right stick passes through as before (the original XInput callback never clears it when the checkbox is OFF). Mouse orbit and other controls are unchanged.
- Enabled: unchanged semantic behavior of isolating native right stick while our mod camera still reads original unfiltered right stick, *also during RT/RB* (V0.68 already had this intent). Added independent logging `native right-stick filtered DURING FIRE`, `rightStickReadsFiltered`, `whileFiring` counts to verify the XInput route actually reached the native filter; if user still observes character rotation, investigate alternate Steam Input/HID/native aim paths rather than assume success.
- Removed the misleading `Isolate native flat mouse aiming movement` checkbox, its default/INI/runtime setting and the suppression of WM_MOUSEMOVE/WM_INPUT in TPS. The old interception moved the mod camera but froze the game's flat-plane world aim target. New behavior observes raw mouse deltas for TPS camera orbit while forwarding all native mouse events normally, returning to vanilla targeting (which may still differ from TPS camera-forward ray). The obsolete `TPSSuppressVanillaMouseAim` key is purged from existing user INIs on migration. **Native 3D projectile targeting remains unresolved.**

Version `0.69-tps-shoot-stick-gate-native-mouse`, config revision 2116. No modifications to camera attachment/geometry, L3 recenter, FOV, movement speed, strafing, combat hooks, horse, HUD toggle, other features, or official validated release. Build is **TEST**, not user validated. Reproduce with RT/RB and Block ON then OFF, check resulting game character rotation, 3D camera position and shot direction, native mouse ability to aim, and collect fresh log to diagnose alternative input if ON still fails.


## V0.70 (10 October 2026) TEST: independent TPS camera devices, horse pose, right-stick investigation

User's V0.69 log (22:01:48-22:04:14) shows valid foot TPS camera transform, two successful L3 recenter presses, and XInput right stick **filtered 3,741 reads, 1,136 of them during shooting**. Yet the user reports native character rotation still happens. This proves filtering is invoked on at least one controller API, NOT that all game/Steam Input/UE paths obey it. Do **not** claim it solved until game testing. There is no forced native actor yaw write. Instrument an on-foot right-stick-only controlled yaw probe when left stick is idle and the right-stick filter is on; emit limited diagnostic only if actor yaw changes >8 degrees in 650 ms. The follow-up log distinguishes an alternative rotation path from a nonfunctional filter and preserves co-op inputs.

**Independent TPS camera input controls:** previous `Control camera with mouse / controller right stick` becomes two unrelated overlay toggles:
- `Control camera with mouse` -> [Features] CameraMouseOrbit=1;
- `Control camera with controller right stick` -> [Features] CameraControllerOrbit=1.
Each retains its own speed slider and can be independently disabled. The old combined `CameraMouseGamepad` key is read as migration default for both and removed on SaveNow; the runtime legacy `cameraOrbitInputEnabled` remains derived as logical OR only for camera orbit/recenter math. The separate **Block vanilla right-stick player rotation** option is preserved unchanged, including the V0.69 shooting gate. If both are disabled the camera stays at the last orbit angle, with existing TPS follow still available.

**Horse TPS:** The prior V0.67-0.69 synthetic TPS rejects camera when the on-foot `UMayhemCharacterMovementComponent::GetMaxSpeed` callback stops on mount. The log clearly shows `HorseFeature: HORSE MOVEMENT READY` at 22:02:26 followed by `TPS camera REJECT actorValid=0 fresh=0` at 22:02:26. V0.70 adds a read-only `ACharacter::RootComponent` world pose sample `Actor+0x158` -> component translation `+0x1A0,+0x1A4,+0x1A8`, yaw `+0x1F4` inside the **already working live native HorseCharacter callbacks**, never dereferences a saved horse UObject from Present or camera. First accept only if horse is within 2800 XY and 1100 Z of a recently observed local player (<15 s); subsequent updates only for same live horse pointer seen within 2 s. Camera selects horse pose only when on-foot callback is stale (>300 ms), horse pose is fresh (<750 ms), camera proximity and native pitch checks pass, and it returns to foot pose when player callback resumes. Logs `LIVE horse camera pose` and `camera pose source HORSE/FOOT`. Uses same existing TPS distance, height, FOV, recenter, angle, no mount-speed change. This is **experimental**: horse ACharacter root offsets and callback cadence to be verified by log; unusual cinematics still use proximity/pitch heuristic. Do not use cached horse pointers for writes.

Other features strictly unchanged: horse stamina/sprint speed code, camera positioning on foot, existing FOV, HUD, L3 bindings, damage, intro, etc. Revision 2117; deliver Windows build V0.70 TEST through fixed rolling prerelease and versioned backup ZIP, files directly at archive root. Official validated release unchanged.
