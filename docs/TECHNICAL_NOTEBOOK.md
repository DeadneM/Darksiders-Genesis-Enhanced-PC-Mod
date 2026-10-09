# Darksiders Genesis Enhanced PC Mod - Technical Notebook

This file preserves the complete historical README / development notebook that
accumulated through the V0.1 -> V0.16 experimental cycle.

It is intentionally archival. For installation and current feature status, use
the repository root `README.md`.

---

# Darksiders Genesis Enhanced PC Mod

> Dedicated repository for the ASI/DXGI gameplay-enhancement mod.
>
> The separate pre-order mount unlock project remains in
> `DeadneM/Darksiders-Genesis-Preorder-Mounts-PC`.

This file is the **authoritative development notebook** for the experimental
Darksiders Genesis ASI mod. It is intentionally cumulative so development can
be resumed later without reconstructing decisions from chat history.

> Public `main` currently tracks the validated **V0.13B ASI base**.
> **V0.14A is rejected** because it failed to identify the mounted horse.
> **V0.14B is rejected for horse-speed control** because user testing produced
> no perceptible horse acceleration.
> **V0.14C is rejected as a runtime test** because its IsHorseActive resolver
> matched zero functions at startup, so none of the direct horse movement code
> ever executed. V0.14D removes that resolver dependency while keeping the
> still-unvalidated direct MaxWalkSpeed / MaxAcceleration experiment.

---

## Project goal

Build a small, robust Win64 ASI mod for **Darksiders Genesis** with:

- a local `dxgi.dll` ASI loader;
- an in-game overlay inspired by the validated Q Protocol UI model;
- a **rebindable menu key** to open/close the overlay, default **Insert**;
- full mouse interaction while the overlay is visible;
- F1-F12 fixed hotkey slots whose actions are reconfigurable;
- one INI as the only configuration source;
- fail-open behavior if the renderer/overlay cannot initialize;
- no game-file edits for ASI functionality.

The README must continue to record:

- each build;
- exact technical changes;
- test results;
- validated/rejected bases;
- known issues;
- executable compatibility information;
- current TODO;
- next investigation.

---

## Target executable audit

File supplied for the first ASI pass:

```text
DarksidersGenesis-Win64-Shipping.exe
Size:    62,113,280 bytes
SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
Machine: x86-64 / PE32+
```

Relevant imports confirmed directly from the executable:

```text
dxgi.dll
  CreateDXGIFactory
  CreateDXGIFactory1

d3d11.dll
d3d9.dll
```

The executable therefore provides a clean path for:

```text
game
  -> local dxgi.dll proxy
  -> real Windows System32\dxgi.dll
  -> DarksidersGenesisMod.asi
  -> D3D11 Present / ResizeBuffers hook
  -> Dear ImGui overlay
```

UE4 / MoviePlayer strings confirmed in the executable include:

```text
StartupMovies
WindowsMoviePlayer
MoviePlayer
CreateMoviePlayer
GetMoviePlayer()->SetupLoadingScreenFromIni
DefaultGameMoviePlayer.cpp
```

This gives the Skip Intro feature a real engine-native investigation path.

---

## V0.3A — Native Toggle HUD

**Status: TEST CANDIDATE**

V0.3A is cumulative from V0.2A:

- validated V0.1 DXGI / D3D11 / ImGui overlay foundation;
- rebindable overlay menu key;
- first real gameplay feature: native Toggle HUD.

### HUD binary audit

The supplied target executable contains the game-specific console variable:

```text
ui.HideHud
>0 Hides all HUD completely.
Force HUD to be hidden.
<=0: default HUD behavior
1: HUD always hidden
```

Static audit of the validated executable:

```text
ui.HideHud UTF-16 string RVA : 0x26F3F78
registration name xref RVA    : 0x000E779A
CVar data-slot RVA            : 0x03803748
native boolean getter RVA     : 0x0063AB50
```

Getter body:

```asm
mov rax, [rip + ui.HideHud_data_slot]
cmp dword ptr [rax], 0
setne al
ret
```

### V0.3A implementation

The ASI **does not overwrite the CVar value**.

At runtime it:

1. finds the UTF-16 `ui.HideHud` name inside `.rdata`;
2. finds the unique RIP-relative registration reference in `.text`;
3. follows the validated registration layout to the CVar data slot;
4. locates the unique native boolean getter using that exact data slot;
5. hooks only the getter with MinHook.

Return policy:

```text
nativeHidden || modHidden
```

This means the mod can request HUD hiding without cancelling a native game
request to hide the HUD.

Runtime behavior:

- F1 defaults to Toggle HUD;
- first press: HUD hidden;
- next press: HUD visible again;
- overlay -> Features -> HUD Hidden can also change the runtime state;
- runtime HUD state starts visible on every launch;
- `[Features] ToggleHUD=1` enables the feature by default;
- if resolution fails, Toggle HUD becomes unavailable while the rest of the
  ASI stays fail-open.

### V0.2A — Rebindable menu key / safe rollback

**Status: TEST CANDIDATE**

V0.2 keeps the validated V0.1 renderer/input foundation and makes the overlay
open/close key fully rebindable at runtime.

Behavior:

- default remains `Insert`;
- `General -> Rebind Menu Key` waits for the next keyboard key;
- `Esc` cancels capture;
- the new key is saved immediately to `[Overlay] MenuKey`;
- Save / Reload / Reset Defaults all understand the menu binding;
- Reset Defaults restores `Insert`;
- arbitrary keyboard keys are persisted as a readable token when known, or
  `VK_XX` for less common virtual keys;
- the captured key is debounced so the same press does not instantly close the menu.

### V0.1 — Overlay foundation

**Status: VALIDATED IN GAME**

### First successful Windows build

GitHub Actions run:

```text
37231616133
```

Build result: **PASS**

Artifacts:

```text
dxgi.dll
  SHA-256 513262d7212e4c4fe907ab83e8794704b8970dc2b1e97137b152fafd3abb16f6

DarksidersGenesisMod.asi
  SHA-256 ec9b951727edecbeef73c984068107c06d183d078370b412889bad402f73e1e5

DarksidersGenesis_ASI_V0.1_TEST.zip
  SHA-256 7cf4fb36e1e72a1ad759a97a9ffd418757bd9dd0d5795b09a6c4627e6d7ced1d
```

Both compiled binaries were independently checked as **PE32+ x86-64 DLLs**.

### Build-system notes

The first CI pass exposed two build-environment details which are now documented:

- GitHub `windows-latest` moved to the Windows 2025 / Visual Studio 2026 image,
  so the workflow now uses the `Visual Studio 18 2026` CMake generator.
- MinHook's Windows x64 output uses its configuration postfix
  (`minhook.x64.lib`); CMake target resolution was hardened so the ASI links
  the actual MinHook target instead of guessing a library filename.

The source itself then compiled and linked successfully.

### Purpose

V0.1 is deliberately a foundation build.

It validates:

1. DXGI proxy loading;
2. automatic ASI discovery/loading;
3. D3D11 swap-chain discovery;
4. Present + ResizeBuffers hooks;
5. Dear ImGui initialization on the real game swap chain;
6. validated menu toggle;
7. mouse input/cursor access;
8. suppression of gameplay F1-F12 input while the menu is open;
9. F1-F12 action mapping;
10. INI Save / Reload / Reset Defaults;
11. clean logging to `DarksidersGenesisMod.log`.

V0.1 does **not** pretend that gameplay features are already implemented.
Every requested feature is enabled by default in configuration, but is clearly
shown as **Hook pending** until a native implementation is identified and tested.

### Overlay layout

Tabs:

```text
General
Features
Hotkeys
About
```

Hotkey architecture follows Q Protocol's clean model:

```text
F1-F12 fixed key
      -> selected Action
      -> shared feature implementation
```

No duplicate hotkey-specific gameplay code should ever be added.

### Authoritative default hotkeys

| Key | Action |
|---|---|
| F1 | Toggle HUD |
| F2 | Movement Speed |
| F3 | Action Recovery |
| F4 | Skip Intro Videos |
| F5 | Third Person |
| F6-F12 | None |

### Authoritative default feature policy

All requested options are **enabled by default**:

```ini
ToggleHUD=1
MovementSpeed=1
ActionRecovery=1
SkipIntroVideos=1
ThirdPerson=1
```

Initial tuning placeholders stored in the INI:

```ini
MovementSpeedMultiplier=1.150
ActionRecoveryMultiplier=2.000
```

These values are intentionally inert until their native hooks are implemented.

---

## Current TODO

### 1. Toggle HUD

Goal:

- toggle the actual in-game HUD without disabling unrelated rendering;
- preserve menus/overlay;
- no permanent asset edits.

Status: **implemented in V0.3A through native `ui.HideHud` getter hook; awaiting in-game validation**.

### 2. Movement Speed

Goal:

- adjustable movement speed;
- user-facing multiplier in overlay/INI;
- avoid globally changing game time;
- do not affect enemies/NPCs;
- do not consume the separate horse-speed roadmap item.

Default option state: **Enabled**.

Initial stored test value: **1.15x**.

Status: **V0.4A rejected; corrected implementation in V0.5B awaiting validation**.

Implementation notes:

V0.4A originally resolved `AMayhemCharacter::GetMaxSpeed`, but user testing
showed no movement-speed effect. That helper only queries the movement component
and is not the physics virtual consumed by CharacterMovement.

V0.5B corrects the target:

- `UMayhemCharacterMovementComponent` reflected object size: `0x850`;
- base `UCharacterMovementComponent::GetMaxSpeed` virtual slot: `+0x3D0`;
- Mayhem override RVA in the audited executable: `0x56FBE0`;
- the signature is unique in `.text`;
- the hook checks `CharacterOwner` at `+0x190`;
- applies only when `APawn::IsLocallyControlled` is true;
- applies only in `Walking` / `NavWalking` movement modes.

### 3. Action Recovery Speed

Observed behavior:

After some actions there is a short interval where the character is visibly
finished but cannot move yet.

Goal:

- shorten/remove only this post-action movement lock;
- do not indiscriminately speed up all animations;
- expose a tuning control if the engine path supports a scalar.

Default option state: **Enabled**.

Initial stored test value: **2.00x**.

Status: **implemented in V0.5A; awaiting in-game validation**.

Native audit result:

- owner: `UMayhemPlayerAbilityComponent`;
- reflected field: `MoveInterruptDelaySec`;
- field offset: `+0x110`;
- runtime elapsed timer used by the same gate: `+0x114`;
- `ECharacterActions::MOVE = 0x1D`;
- native gate RVA in the audited executable: `0x5B3150`;
- signature match count: exactly **1**.

The native logic explicitly rejects `MOVE` while:

```text
MoveInterruptDelaySec > elapsed timer
```

V0.5A therefore scales only this delay during the native check.

### 4. Skip Intro Cinematic

Goal:

- skip the game's opening story cinematic through the ASI;
- do not delete/rename original game files;
- leave normal loading/movie systems intact.

Implementation:

```text
g.PlayIntroCinematicOnBoot = 0
```

Default option state: **Enabled**.

Status: **VALIDATED IN GAME in V0.13B**. This skips the game's opening
cinematic, but it does **not** skip the developer/publisher startup logos.

### 4B. Skip Logos

Goal:

- skip startup company/developer/publisher logos;
- arrive directly at the title / **Press Any Key** screen;
- keep this independent from Skip Intro Cinematic;
- do not rename or delete original game files.

Known movie targets from the retail content layout:

```text
ProjectMayhem/Content/Movies/THQ_LogoBasic.mp4
ProjectMayhem/Content/Movies/AS_LogoBasic.mp4
```

The existing `g.PlayIntroCinematicOnBoot` control does not affect these two
startup movies. The ASI therefore needs a separate MoviePlayer/file-selection
path that suppresses only these logo movies while leaving normal cutscenes and
loading movies intact.

Status: **pending dedicated startup-logo hook**.

### 5. Third Person

Goal:

- switch gameplay to a usable third-person camera;
- preserve aiming/collision/control behavior as much as possible;
- ideally allow runtime toggle.

Default option state: **Enabled**.

Status: **pending camera/player-controller audit**.

### 6. Pistol Damage

Goal:

- increase pistol damage;
- ideally expose a multiplier in the overlay/INI;
- avoid affecting unrelated weapon classes.

Status: **pending weapon-damage audit**.

### 7. Melee Damage

Goal:

- increase melee damage;
- keep the modifier separate from firearm damage;
- ideally expose a multiplier in the overlay/INI.

Status: **pending melee-damage audit**.

### 8. Jump Height

Goal:

- increase jump height;
- expose an adjustable multiplier if the native movement path allows it;
- preserve reliable landing and collision behavior.

Status: **pending character-movement audit**.

### 9. Horse Speed / Sprint

Goal:

- increase normal horse movement speed;
- increase horse sprint speed;
- keep normal speed and sprint tunable independently if possible.

Status: **pending mount movement/sprint audit**.

### 9B. Horse Dash / Sprint Duration

Goal:

- increase how long the horse dash/sprint can remain active;
- keep duration separate from horse speed;
- expose an adjustable duration multiplier if the native mount ability supports it;
- preserve stamina/cooldown behavior unless explicitly tuned by a later feature.

Status: **pending horse dash/sprint ability audit**.

### 10. FOV

Goal:

- expose an adjustable gameplay FOV;
- keep it runtime-configurable from the overlay/INI;
- preserve camera transitions and special camera states.

Status: **pending camera/FOV audit**.

### 10B. Character Zoom / Camera Distance

Goal:

- allow runtime zoom in and zoom out around the controlled character;
- expose camera distance separately from FOV;
- preserve combat, aiming and special camera transitions;
- provide slider and manual numeric input when the native camera path is identified.

Status: **pending camera/player-controller audit**.

### 10C. Camera Angle

Goal:

- allow runtime adjustment of the gameplay camera angle;
- target pitch/elevation first, with yaw/orbit adjustment if the native camera path safely supports it;
- keep angle control independent from FOV and character zoom;
- preserve scripted and special camera states.

Status: **pending camera/player-controller audit**.

### 11. Hotstreak Charge

Goal:

- increase Hotstreak charge/gain rate;
- expose a configurable multiplier if the underlying system supports it;
- avoid changing unrelated resource/ability charge systems.

Status: **pending Hotstreak system audit**.

---

## Design rules

1. **Fail open.** Overlay failure must not stop the game from running.
2. **No fake success.** UI state and real gameplay-hook state are separate.
3. **INI is authoritative.** No hidden secondary config database.
4. **One feature implementation.** Hotkeys and overlay call the same primitive.
5. **No executable replacement.** ASI development should not require shipping a
   modified game EXE.
6. **Keep public validated work safe.** The existing pre-order mounts release is
   untouched while this branch is experimental.
7. **Journal every build here.** Never rely on chat history as the only source.
8. **Per-session logging.** `DarksidersGenesisMod.log` must be truncated at
   every game launch so it contains only the current session. Do not use a
   cumulative append-across-launches log.

---

## Test checklist for V0.1

Install next to the game executable:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
README.md
```

Then verify:

- [x] Game boots normally.
- [ ] `DarksidersGenesisMod.log` is created.
- [ ] Log says D3D11 hooks were installed.
- [ ] Log says ImGui overlay is READY.
- [x] Insert opens the overlay.
- [x] Insert closes the overlay.
- [x] Mouse moves and clicks correctly inside the overlay.
- [ ] Game does not react to menu clicks.
- [x] F1-F12 can be reassigned from Hotkeys.
- [ ] Save persists settings after restart.
- [ ] Reload restores INI values.
- [ ] Reset Defaults restores F1-F5 mapping and all feature defaults.
- [ ] Alt-Tab does not break the overlay.
- [ ] Resolution/fullscreen changes do not break the overlay.
- [ ] Game exits normally.

---

## Build history

### V0.1

- New DXGI proxy loader.
- Loads `*.asi` from the game executable directory.
- Forwards the game's confirmed `CreateDXGIFactory` and
  `CreateDXGIFactory1` calls to the real System32 DXGI.
- Includes compatibility forwarders for common newer DXGI exports.
- New D3D11 overlay core using MinHook + Dear ImGui.
- Insert menu toggle.
- Mouse/keyboard capture while open.
- F1-F12 Q Protocol-style action map.
- INI persistence.
- Requested features all enabled by default.
- Gameplay hooks deliberately left pending for the first foundation test.
- Startup movie strings audited and recorded.

**Compilation:** PASS.

**In-game validation:** PASS — user confirmed V0.1 foundation works perfectly.

**First successful build run:** `37231616133`.

**Canonical V0.1-test binary hashes before README-only rebuild:**

```text
dxgi.dll                  513262d7212e4c4fe907ab83e8794704b8970dc2b1e97137b152fafd3abb16f6
DarksidersGenesisMod.asi  ec9b951727edecbeef73c984068107c06d183d078370b412889bad402f73e1e5
```


### V0.2

- V0.1 becomes the validated overlay foundation.
- Adds runtime menu-key rebinding from the General tab.
- Default key remains Insert.
- Escape cancels a pending key capture.
- New binding is written immediately to `[Overlay] MenuKey`.
- INI parser accepts named common keys, F1-F24, letters/numbers and `VK_XX` fallback tokens.
- Reset Defaults restores Insert.
- Menu title shows the current open/close key instead of hard-coding Insert.
- Captured key is debounced to prevent instant menu closure on the capture press.
- Gameplay features remain enabled by default and continue to be developed independently.

**Validation:** awaiting V0.2 in-game test.


### V0.3A

- Cumulative from V0.2A.
- Added binary audit for the game-specific `ui.HideHud` CVar.
- Added semantic resolver anchored to the UTF-16 CVar name.
- No raw fixed RVA is used as the runtime resolver.
- Added native getter hook instead of writing the CVar.
- F1 now toggles runtime HUD hidden/visible state.
- Overlay Features tab exposes current HUD Hidden runtime state.
- Native game HUD-hide state is preserved with `nativeHidden || modHidden`.
- Reset Defaults restores the mod HUD runtime state to visible.
- Menu-key rebinding from V0.2A is preserved.
- Remaining Movement Speed / Action Recovery / Skip Intro / Third Person hooks
  are still pending.

**Validation:** awaiting first V0.3A in-game test.


## V0.4A — On-foot Movement Speed

**Status: TEST CANDIDATE**

Cumulative from V0.3A.

### Binary audit

The game exposes the relevant Mayhem functions through UE4 reflection:

```text
GetMaxSpeed
GetDefaultMaxSpeed
GetMayhemMovementComponent
AddSpeedLimitOverride
StopOverridingMaxSpeed
ESpeedModType::ADD_VALUE
ESpeedModType::ADD_MOD
ESpeedModType::MULTIPLY_MOD
```

For the audited executable, the `GetMaxSpeed` Blueprint exec wrapper calls the
native helper at RVA `0x56FBC0`. That helper retrieves the Mayhem movement
component from character offset `+0xA20` and dispatches its virtual max-speed
query.

Direct gameplay callers of this helper were also found outside the reflection
wrapper, confirming that it participates in real movement calculations.

### Player-only filtering

The feature must not speed enemies or NPCs.

The hook therefore applies the multiplier only when the character's inherited
`APawn::IsLocallyControlled` virtual returns true.

The audited UE4 wrapper dispatches this virtual through vtable offset:

```text
0x680
```

### Horse exclusion

Horse speed is a separate TODO and must remain independently tunable.

`AMayhemPlayerCharacter::IsHorseActive` was audited and checks:

```asm
cmp qword ptr [rcx + 0xE70], 0
setne al
```

V0.4A therefore leaves `GetMaxSpeed` unchanged while a horse mount is active.

### Runtime behavior

- Movement Speed is enabled by default.
- Default multiplier: **1.15x**.
- F2 toggles Movement Speed ON/OFF.
- Overlay slider range: **1.00x to 2.50x**.
- Slider changes save immediately to the INI.
- No global TimeScale modification.
- No enemy/NPC speed modification.
- No horse-speed modification.
- Resolver failure is fail-open and leaves vanilla movement untouched.

**Validation:** **REJECTED**. User confirmed Movement Speed produced no effect.

Root cause found during V0.5B audit: V0.4A hooked the character-side query helper
instead of the actual CharacterMovement physics virtual.


## V0.5A — Action Recovery / MOVE interrupt delay

**Status: TEST CANDIDATE**

Cumulative from V0.4A.

### What was found

The delay reported by the user is represented directly in
`UMayhemPlayerAbilityComponent` as:

```text
MoveInterruptDelaySec
```

The generated reflection data confirms:

```text
UMayhemPlayerAbilityComponent object size: 0x118
MoveInterruptDelaySec offset:              0x110
runtime elapsed timer offset:              0x114
```

A unique native action-gate function was then identified at audited RVA:

```text
0x5B3150
```

Its opening logic is equivalent to:

```text
if action == ECharacterActions::MOVE (0x1D):
    if MoveInterruptDelaySec > elapsed:
        reject movement
```

This is the exact post-action movement lock targeted by the feature.

### V0.5A implementation

The mod does **not** speed up animations and does **not** change global
TimeScale.

For a MOVE action only:

```text
effectiveDelay = native MoveInterruptDelaySec / ActionRecoveryMultiplier
```

The ASI temporarily substitutes that effective value only while the original
native action-gate function executes, then immediately restores the object's
original value.

This preserves every other native condition checked by the game.

### Runtime behavior

- Action Recovery is enabled by default.
- Default multiplier: **2.00x**.
- F3 toggles Action Recovery ON/OFF.
- Overlay slider range: **1.00x to 5.00x**.
- Slider changes are saved immediately.
- 2.00x means the MOVE lock lasts half as long.
- 5.00x means the MOVE lock lasts one fifth as long.
- No animation-speed change.
- No attack-speed change.
- No global TimeScale change.
- No permanent write to `MoveInterruptDelaySec`.
- Signature mismatch is fail-open and leaves vanilla behavior untouched.

### Additional input fix

V0.5A also fixes a hotkey-state issue inherited by Movement Speed:

- F2 can now re-enable Movement Speed after it has been toggled OFF.
- F3 can likewise toggle Action Recovery both OFF and back ON.
- Toggle HUD still respects its separate feature-enable checkbox.

**Compilation:** PASS.

**GitHub Actions run:** `37235986567`.

Compiled binary hashes:

```text
dxgi.dll
be75f8f2a12b1e44482ae5ffa76708e788eaddd64fdd591fff621b89a1e7277b

DarksidersGenesisMod.asi
10ee522195dd391da435249cdc9d4c1388de41c685db845a5bee7f1d8b7a6002
```

**Validation:** **REJECTED**. User testing confirmed that this implementation did not shorten the post-dodge recovery lock.


## V0.5B — Movement Speed physics-virtual fix

**Status: TEST CANDIDATE**

Cumulative from V0.5A.

### Why V0.4A failed

V0.4A hooked the character-side helper used to *query* movement speed. User
testing confirmed that changing its return value did not alter actual movement.

Binary re-audit of the movement-component vtable showed the real path:

```text
UCharacterMovementComponent::GetMaxSpeed
vtable slot: +0x3D0
```

The Mayhem movement component overrides this slot with:

```text
UMayhemCharacterMovementComponent::GetMaxSpeed
audited RVA: 0x56FBE0
class size:  0x850
```

This function is the layer that combines:

- native movement-mode speed;
- Mayhem speed limits;
- ADD_VALUE modifiers;
- ADD_MOD modifiers;
- MULTIPLY_MOD modifiers.

That is the correct place to scale the value seen by movement physics.

### V0.5B policy

The hook multiplies the native return only when:

```text
CharacterOwner != null
APawn::IsLocallyControlled == true
MovementMode == Walking or NavWalking
MovementSpeed feature == enabled
```

So it does not globally modify movement components, enemies, falling, swimming,
flying or custom movement modes.

Default remains:

```ini
MovementSpeedMultiplier=1.150
```

For validation, test **2.00x** first so the effect is unmistakable.

### Regression note

Action Recovery from V0.5A is preserved unchanged.

**Validation:** awaiting V0.5B in-game test.


### Recovery audit note — MoveInterruptDelaySec

The previous V0.5A interpretation of `MoveInterruptDelaySec` as the player's
post-dodge movement lock is rejected.

Current hypothesis:

- it may instead participate in hit reaction / stun / interruption recovery;
- it remains a valid field to investigate for a future **stun / hit recovery**
  feature;
- it must not be reused for dodge recovery without runtime proof.

Dodge recovery investigation is now focused on the dedicated Mayhem dash ability
state and its movement-input lock instead.


## Active investigation — Dodge recovery

User clarified the target behavior:

> after a dodge animation appears finished, the player can remain unable to
> move for a short tail window.

This is now treated as **dash/dodge recovery**, not generic movement recovery.

Current native leads:

- `UMayhemPlayerDashAbility`
- `UMayhemPlayerGroundDashAbility`
- `UMayhemPlayerDashStartAbility`
- `MinMontageRate`
- `MaxMontageRate`
- `MontageRateIncreaseDuration`
- player movement-input lock observed at `AMayhemPlayerCharacter + 0xAAC`
- dash entry sets that lock;
- dash finalization clears that lock.

The next implementation must target the dash-state recovery/cancel window or
early release of that movement-input lock. It must **not** reuse
`MoveInterruptDelaySec` unless runtime evidence proves relevance.

### MoveInterruptDelaySec reclassification

`MoveInterruptDelaySec` remains an interesting field, but is now tracked as a
possible **hit / stun / interruption recovery** control rather than dodge
recovery.

### Horse roadmap addition

A separate TODO exists for **Horse Dash / Sprint Duration**, distinct from horse
speed, so mount sprint duration can be tuned independently.


## V0.6A — Self-calibrating Dodge Recovery

**Status: REJECTED**

User testing reported no perceptible change. The `Player + 0xAAC` dash flag is therefore not accepted as the actual post-dodge movement blocker.

V0.5A `MoveInterruptDelaySec` recovery is no longer installed.

V0.6A targets the dash ability's own movement-input lock:

```text
AMayhemPlayerCharacter + 0xAAC
```

Native audit:

```text
GroundDash start RVA   0x5B0440
GroundDash tick RVA    0x5B72B0
GroundDash finish RVA  0x59BF80
lock set site          0x5B047A
lock clear site        0x59BFBF
```

The runtime resolver does not blindly use those RVAs. It finds unique start and
finish signatures, then derives the tick function from their shared vtable
relationship.

### Behavior

- first observed dodge is left completely vanilla;
- the ASI measures its native dash-lock lifetime;
- later dodges release only the movement-input lock slightly before native
  finalization;
- dash animation, dash velocity, collision and ability finish remain native;
- default early release: **100 ms**;
- overlay range: **0 to 300 ms**;
- F3 toggles Dodge Recovery;
- the overlay shows the learned native dash-lock duration;
- signature/vtable mismatch is fail-open.

INI:

```ini
[Values]
DodgeEarlyUnlockMs=100.000
```

This is intentionally a targeted test. If 100 ms is too early or too late, the
value can be tuned directly from the overlay without rebuilding the ASI.


## V0.6B — InputSuppressWindow diagnostic bypass

**Status: TEST CANDIDATE**

V0.6A produced no perceptible change, so the dash-local flag at
`AMayhemPlayerCharacter + 0xAAC` is now treated as an accompanying dash-state
flag rather than the proven movement blocker.

A deeper binary audit found a dedicated animation-notify state:

```text
UAnimNotify_InputSuppressWindow
Display name: "Suppress Player Input Window"
```

Its vtable resolves semantically from that display-name string.

Audited native methods:

```text
GetNotifyName : RVA 0x5D1230
NotifyBegin   : RVA 0x5D8860
NotifyEnd     : RVA 0x5D8F70
```

`NotifyBegin` resolves the player character and calls:

```text
AMayhemPlayerCharacter::SetInputSuppressed(reason, true)
```

`NotifyEnd` calls:

```text
AMayhemPlayerCharacter::SetInputSuppressed(reason, false)
```

The player keeps suppression reasons in a counted table at approximately
`Player + 0xC20`, so this system can suppress input independently of the
dash-local `+0xAAC` flag.

### Diagnostic policy

When Dodge Recovery is ON, V0.6B bypasses both
`UAnimNotify_InputSuppressWindow::NotifyBegin` and matching `NotifyEnd`
completely.

This is intentionally aggressive and diagnostic.

Goal:

- if dodge recovery suddenly changes, the real blocker has been identified;
- if nothing changes, InputSuppressWindow is eliminated as the cause and the
  next audit moves to the ability/action state machine.

A bypass counter is maintained so toggling the feature does not unbalance the
game's native suppression counters.

The log prints every bypassed Begin/End. If the feature has no visible effect,
those lines tell us whether the dodge montage actually uses this notify state.

**Do not treat V0.6B as a final implementation.**


## V0.7A — Common Ability MOVE Recovery

**Status: TEST CANDIDATE**

The latest user report clarified that the dead movement tail is **not dodge-specific**:
many player actions can finish visually, then leave the character fixed for roughly
half a second before movement resumes.

### What the V0.6 logs proved

V0.6A did not actually install in the tested executable because its dash resolver
failed closed:

```text
Dodge recovery: start/finish signature mismatch start=1 finish=2
Dodge recovery: resolver failed; feature remains fail-open
```

V0.6B did install its `UAnimNotify_InputSuppressWindow` diagnostic hooks, but no
Begin/End bypass events were recorded during the supplied test session. That
system therefore does not explain the common recovery tail observed across many
actions.

### Common ability system audit

The native ability base exposes:

```text
EnableInterrupt
IsInterruptEnabled
ProcessInterrupt
AbilityInterruptsFlags
AllowedActionsFlags
```

The interrupt enum includes:

```text
EAbilityInterrupt::NONE
EAbilityInterrupt::MOVE
EAbilityInterrupt::JUMP
...
```

The audited native `IsInterruptEnabled` function reads the ability interrupt
bitset directly from the ability object:

```text
bitset storage: +0xB0
ability state:  +0xD8
elapsed time:   +0xDC
```

The ability lifecycle enum is:

```text
0 INITIALIZING
1 STARTING
2 RUNNING
3 SUSPENDED
4 AWAITING_FINISH
5 FINISHED
6 FINALIZED
```

### V0.7A policy

V0.7A hooks the common native `IsInterruptEnabled` path.

For `EAbilityInterrupt::MOVE` only:

- native MOVE permission is always preserved;
- STARTING and RUNNING remain untouched;
- movement is **not** allowed to cancel an action early;
- if native MOVE is still disabled while the ability is in
  `AWAITING_FINISH`, the mod returns true.

This specifically targets the dead post-action tail without turning movement
into a universal animation cancel.

### Runtime diagnostics

The overlay shows:

- total MOVE interrupt queries;
- queries where native code blocked MOVE;
- queries forced by V0.7A;
- last observed ability state;
- last elapsed ability time.

The log records state transitions and the first forced MOVE events.

If the user still feels no difference:

- `Forced > 0` means the common interrupt path is active but another movement
  gate remains downstream;
- `Forced = 0` means the observed dead tail occurs before/after
  `AWAITING_FINISH`, and the logged state transition tells us where to move next.


## V0.8A — AllowedActions MOVE diagnostic

**Status: TEST CANDIDATE**

### Why V0.7A is rejected

The user reported no effect, and the supplied V0.7A log is decisive:

- the `IsInterruptEnabled` hook resolved and installed successfully;
- no `MOVE query`, `Native blocked` or `FORCE MOVE` events were emitted
  during the test session.

Therefore the observed post-action movement lock does **not** flow through
`EAbilityInterrupt::MOVE` in the tested paths.

### New target: IsActionEnabled

The reflected `IsActionEnabled` wrapper calls a tiny native bit-test helper at
audited RVA:

```text
0x5A6800
```

Its native code checks the ability's action bitset:

```text
ability + 0x80
```

This is distinct from `IsInterruptEnabled`, which reads from `ability + 0xB0`.

The previously audited generic movement gate independently established:

```text
ECharacterActions::MOVE = 0x1D
```

### Local-player filtering

The ability reflection data exposes `Instigator` at:

```text
ability + 0x48
```

V0.8A records the locally controlled player pointer from the already installed
CharacterMovement hook, then applies the diagnostic only when:

```text
ability->Instigator == local player
```

Enemies and NPCs therefore keep native action permissions.

### Diagnostic policy

When Action Recovery is ON and the local player's ability receives:

```text
IsActionEnabled(MOVE)
```

V0.8A preserves native `true`, but overrides native `false` to `true`.

This is intentionally aggressive. The purpose is to prove or reject
`AllowedActionsFlags` as the common movement lock.

Telemetry records:

- all MOVE action queries;
- local-player MOVE queries;
- local queries where native code blocked MOVE;
- forced MOVE results;
- ability state and elapsed time at the last local query.

If this finally removes the dead tail but allows movement too early during some
actions, the path is proven and the next build can restrict the override by
state/time using the captured telemetry.

If counters remain at zero, `AllowedActionsFlags` is also eliminated.


## V0.8B — Safe Action Tail + Unified Gameplay UI

**Status: TEST CANDIDATE**

### V0.8A user result

V0.8A finally produced a visible gameplay change, which is important evidence
that the `AllowedActionsFlags / IsActionEnabled(ECharacterActions::MOVE)` path
is relevant.

However the V0.8A diagnostic was intentionally aggressive and forced MOVE
whenever the local player's active ability returned false.

The user observed a concrete regression after opening a chest:

- the character could move;
- the interaction/character animation remained locked;
- the result was movement without a valid locomotion animation.

This proves that MOVE must **not** be forced while an interaction ability is
still in its normal `RUNNING` phase.

### V0.8B recovery policy

V0.8B keeps the same proven `IsActionEnabled(MOVE)` path, but changes the
policy:

```text
INITIALIZING     native
STARTING         native
RUNNING          native
SUSPENDED        native
AWAITING_FINISH  mod may release MOVE
FINISHED         native
FINALIZED        native
```

Only the `AWAITING_FINISH` dead tail is shortened.

A new user setting controls how long to wait after first observing the tail:

```ini
ActionRecoveryDelayMs=0.000
```

Overlay range:

```text
0 ms -> 500 ms
```

Default `0 ms` means release MOVE immediately once the ability has already
entered `AWAITING_FINISH`.

This is designed specifically to avoid the chest/interactions regression seen
with V0.8A while still attacking the common post-action freeze.

### Overlay redesign

The unused `General` tab has been removed.

Tabs are now:

```text
Gameplay
Hotkeys
About
```

Menu-key rebinding and configuration buttons moved to `Hotkeys`.

The Gameplay page is grouped into:

```text
Player
Combat
Horse
Camera
System
```

### Unified option model

Where a numeric tuning value makes sense, every gameplay feature now follows:

```text
[checkbox] Feature
           [value slider]
```

Persistent options now exist for:

```text
Movement Speed
Action Recovery
Jump Height
Pistol Damage
Melee Damage
Hotstreak Charge
Horse Speed
Horse Sprint Speed
Horse Sprint Duration
FOV
Third Person camera distance
```

Toggle-only features remain toggle-only where a scalar has no meaningful
semantics:

```text
Toggle HUD
Skip Intro Videos
```

Pending features are clearly labelled `Pending hook`; their values are saved
now so future runtime implementations do not require another menu redesign.

### Reference-PAK-driven defaults

The new UI names follow the concrete systems recovered from the supplied
reference PAKs:

- Jump Height -> player Blueprint movement defaults;
- Horse Speed -> `MaxWalkSpeed`;
- Horse Sprint Duration -> `StaminaSprintPercentageRate`;
- Pistol Damage -> projectile `Damage`;
- Hotstreak Charge -> projectile `BaseJuice` candidate.

These pending controls are configuration/UI infrastructure only until their
runtime hooks are validated.


## V0.9A — Jump / Glide / Horse Sprint Duration Hooks

**Status: TEST CANDIDATE**

V0.9A starts converting the reference-PAK findings into real runtime hooks.

### Reflection offsets recovered from the executable

The UE4 generated property parameter tables in the audited executable expose the
actual runtime offsets:

```text
UCharacterMovementComponent
  JumpZVelocity                                      +0x1A0

UMayhemPlayerCharacterMovementComponent
  DoubleJumpZVelocity                                +0x85C
  GlideDurationSeconds                               +0x86C

AMayhemHorseCharacter
  StaminaRecoveryPercentageRate                     +0x910
  StaminaTotalRecoveryPercentageRate                +0x914
  StaminaSprintPercentageRate                       +0x918
```

These offsets are derived from the generated reflection metadata for the target
executable, not guessed from community SDK layouts.

### Runtime application model

The already-installed
`UMayhemCharacterMovementComponent::GetMaxSpeed` hook is used as a safe
game-thread heartbeat.

For each local movement component, V0.9A captures the original values once and
then applies or restores the selected settings.

This means disabling an option restores the value that the game instance
originally had instead of hardcoding one character's defaults.

### Jump Height

New active control:

```text
Jump Height [ON/OFF]
0.50x .. 3.00x
```

Targets:

```text
JumpZVelocity
DoubleJumpZVelocity
```

Because ballistic jump height is approximately proportional to vertical
velocity squared when gravity is unchanged, V0.9A uses:

```text
velocity multiplier = sqrt(height multiplier)
```

This keeps the user-facing value closer to an actual height multiplier.

### Glide / Flight Duration

New option:

```text
Glide / Flight Duration [ON/OFF]
0.50x .. 5.00x
```

Target:

```text
GlideDurationSeconds +0x86C
```

The original per-character duration is captured at runtime, so Strife and War
can retain different vanilla durations while receiving the same multiplier.

Default:

```ini
GlideDuration=1
GlideDurationMultiplier=1.500
```

### Horse Sprint Duration

The reference horse PAK proved that sprint duration is directly controlled by
stamina drain:

```text
StaminaSprintPercentageRate
```

The example mod changes the observed vanilla value:

```text
25.0 -> 0.0
```

V0.9A implements a tunable duration multiplier without forcing infinite sprint:

```text
effective stamina drain = original drain / duration multiplier
```

Examples for an original drain of 25:

```text
1.00x -> 25.0
2.00x -> 12.5
5.00x -> 5.0
10.0x -> 2.5
```

Disabling the option restores the captured original drain.

### UI state

The Gameplay page now contains active runtime controls for:

- Jump Height
- Glide / Flight Duration
- Horse Sprint Duration

Other roadmap controls remain visible and persistent but are still marked
`Pending hook` until their runtime implementation lands.

**Validation:** awaiting in-game V0.9A test.


## V0.9B — Crash fix / Jump + Glide only

**Status: TEST CANDIDATE**

V0.9A is **REJECTED** because it crashes during level loading.

Crash evidence from the supplied UE4 crash package:

```text
Crashed thread: GameThread
Module frame: DarksidersGenesisMod + 0x3676
```

Disassembly of the exact V0.9A ASI maps RVA `0x3676` to:

```asm
movss xmm0, dword ptr [rbx + 0x918]
```

That instruction is the V0.9A read of:

```text
AMayhemHorseCharacter::StaminaSprintPercentageRate
```

inside `FindOrCaptureHorseState`.

The unsafe assumption was:

```text
AMayhemPlayerCharacter + 0xE70
    == stable live AMayhemHorseCharacter pointer
```

The crash proves that assumption is false in at least some loading/runtime
states.

### V0.9B correction

The entire direct horse-pointer tuning path is removed.

V0.9B keeps active only:

```text
Jump Height
Glide / Flight Duration
```

Horse Sprint Duration remains visible/configurable but is marked pending until a
safe horse-instance resolver is established from an actual horse component /
ability path.

No read or write to `horse + 0x918` occurs in V0.9B.

**Validation:** **VALIDATED IN GAME.** User confirmed V0.9B works correctly after removing the unsafe horse pointer path.


### V0.9B validation result

User validation:

```text
"nickel"
```

Confirmed:

- level loading no longer crashes;
- the V0.9A horse pointer crash path is removed;
- Jump Height remains usable;
- Glide / Flight Duration remains usable.

V0.9B therefore becomes the new validated base for the movement/glide branch.

Horse Sprint Duration remains pending until a safe horse-instance resolver is
found.


## V0.10A — Hotstreak Charge / AddJuice Hook

**Status: TEST CANDIDATE**

V0.10A starts the combat-hook phase with the safest target recovered from the
reference PAK audit.

### Native target

The executable exposes the Hotstreak API:

```text
AddJuice
GetCurrentJuice
GetCurrentJuiceRatio
GetMaxJuice
RemoveJuice
TryActivateHotStreak
TryConsumeJuice
```

The generated `AddJuice` exec wrapper resolves to the native function at:

```text
RVA 0x660260
```

The native body has a unique signature in the audited executable.

### Runtime policy

V0.10A hooks:

```text
AddJuice(void* component, float Amount)
```

Only positive gains are eligible.

The native function immediately reads its player-owner pointer from:

```text
component + 0xE8
```

V0.10A compares that pointer to the already captured locally controlled player
and only multiplies the gain when they match.

No writes are made to arbitrary UObject/Blueprint fields.

Formula:

```text
effective gain = native gain * HotstreakChargeMultiplier
```

Loss/consumption paths remain native.

### UI telemetry

The existing `Hotstreak Charge` checkbox/value is now active.

The overlay displays:

```text
Boost calls
Last native gain
Last boosted gain
```

This lets the user confirm immediately whether gameplay is flowing through the
hook.

Default:

```ini
HotstreakCharge=1
HotstreakChargeMultiplier=2.000
```

### Pistol Damage

Pistol Damage remains pending in V0.10A.

The reference PAK proves the projectile Blueprint `Damage` property is the
correct gameplay target, but it is Blueprint-defined rather than a simple
native reflected field in the executable. A safe projectile/damage-call filter
will be audited separately rather than guessing an object offset.

**Validation:** **REJECTED FOR HORSE SPEED IN GAME.**

User feedback:

```text
The horses do not seem to accelerate.
```

Regardless of whether the safer V0.14B mount telemetry resolves correctly,
multiplying the horse `GetMaxSpeed` return is not accepted as a working speed
control. V0.14C therefore stops using that return value as the gameplay lever
and moves to the direct movement properties proven by the reference Horse PAK.


## V0.10B — Expanded tuning ranges

Requested maximum values:

```text
Jump Height        5.00x
Glide Duration    10.00x
Hotstreak Charge  25.00x
```

The runtime clamps were updated to match the UI limits, so the higher values are
not cosmetic-only.

Development continues on the same V0.10 combat branch.


## V0.11A — Pistol Damage / DamageRecord Hook

**Status: TEST CANDIDATE**

The executable reflection metadata confirms:

```text
FMayhemDamageEventRecord
  Damage          +0x08
  ScaleType       +0x0C
  ElementTypes    +0x10
  DamageSourceTags +0x18
  HotStreak       +0x28
    BaseJuice     +0x00
```

The native projectile base also stores:

```text
DamageEventRecord +0x88
HotStreak         +0x1B0
```

The Blueprint library `DoDamageToActor` native function resolves uniquely at:

```text
RVA 0x667300
```

### Filter

The user-supplied DualPistols reference PAKs consistently modify both:

```text
Damage
BaseJuice
```

on Strife gun projectiles.

V0.11A therefore treats a positive, sane `BaseJuice` value in the outgoing
`FMayhemDamageEventRecord` as the gun/projectile discriminator.

### Mutation policy

The hook:

1. reads native Damage and BaseJuice;
2. if BaseJuice > 0, multiplies Damage by the user value;
3. calls native `DoDamageToActor`;
4. restores the original Damage immediately.

No Blueprint/CDO or projectile instance is permanently mutated.

### Overlay telemetry

Pistol Damage now shows:

```text
Boost calls
Last native damage
Last boosted damage
Last BaseJuice
```

This will quickly confirm whether the expected DualPistols records are flowing
through the hook.

**Validation:** awaiting in-game test.


## V0.11B — Manual Numeric Input + Zero-Minimum Policy

**Status: TEST CANDIDATE**

User-requested UI/value policy:

- every tunable numeric option now exposes both a slider and an explicit manual
  input field;
- multiplier-style values use **0** as their minimum;
- future multiplier-style options should also default to a minimum of 0 unless
  zero is semantically invalid.

New requested defaults/ranges:

```text
Movement Speed
  default 1.50x
  min 0.00x

Jump Height
  default 1.25x
  min 0.00x
  max 5.00x

Glide / Flight Duration
  default 10.00x
  min 0.00x
  max 100.00x

Pistol Damage
  min 0.00x
  V0.11A hook rejected by user: no gameplay effect

Melee Damage
  min 0.00x

Hotstreak Charge
  min 0.00x
  max 25.00x

Horse Speed
  min 0.00x

Horse Sprint Speed
  min 0.00x

Horse Sprint Duration
  min 0.00x
```

Third-person distance is also treated as a multiplier and now allows 0. FOV
remains an absolute camera angle and keeps its safe nonzero range.

### Pistol Damage rollback

V0.11A `DoDamageToActor + BaseJuice` filtering is **REJECTED** because user
testing produced no pistol-damage change.

The hook is no longer installed in V0.11B. The menu entry remains available as
a pending feature while a better projectile/damage path is audited.


## V0.12A — Melee Damage / GetBaseDamage Diagnostic

**Status: TEST CANDIDATE**

The executable exposes a generated Blueprint exec wrapper for:

```text
GetBaseDamage
```

Audited wrapper:

```text
RVA 0x770D90
virtual slot +0x928
```

The wrapper calls the character virtual, receives the native float in XMM0 and
writes it to the Blueprint result pointer.

### V0.12A policy

The hook:

1. runs the native wrapper first;
2. only accepts the locally controlled player character;
3. reads the returned BaseDamage float;
4. applies `MeleeDamageMultiplier`;
5. writes only the Blueprint result value.

No character stats or UObject fields are permanently modified.

This is intentionally diagnostic because `BaseDamage` may feed more than one
player attack family. The overlay telemetry shows:

```text
Boost calls
Last native BaseDamage
Last boosted BaseDamage
```

Testing should compare melee attacks against pistol/ranged attacks. If melee
damage changes while projectile damage does not, the path is suitable for the
feature. If unrelated player damage scales too, the next build will filter the
callsite/ability rather than keeping the broad BaseDamage result override.

### Cumulative UI/value policy retained

V0.12A includes V0.11B:

- visible manual numeric entry beside every slider;
- multiplier minima at 0;
- Movement Speed default 1.50x;
- Jump Height default 1.25x, 0..5x;
- Glide / Flight Duration default 10x, 0..100x;
- Hotstreak Charge 0..25x;
- Pistol Damage V0.11A rejected and not installed.

**Validation:** awaiting in-game test.


## V0.12B — Melee 100x Diagnostic

**Manual numeric UI: VALIDATED.**

User confirmed the slider + manual-entry interface is good and should remain the
standard numeric-control pattern.

### Melee Damage status

V0.12A is **not rejected**.

User feedback indicates the setting appears to have some effect, but a 10x
setting does not produce an obviously decisive one-shot result. This is
consistent with `GetBaseDamage` potentially being only one term in the final
damage formula.

Current classification:

```text
Melee Damage / GetBaseDamage
PROMISING / PARTIALLY OBSERVED
not yet validated
not rejected
```

### V0.12B diagnostic range

Melee Damage maximum is raised to:

```text
100.00x
```

The runtime clamp is also 100x, so this is not UI-only.

Test interpretation:

- if 100x produces an unmistakable damage jump / one-shots, the
  `GetBaseDamage` path is confirmed as materially contributing to combat;
- if 100x still has only a modest effect, `GetBaseDamage` is not the final
  applied-damage control and the next audit should move downstream into the
  final damage-event scaling path.

The existing overlay telemetry remains:

```text
Boost calls
Last base damage -> boosted base damage
```

This lets the runtime call path be distinguished from the gameplay effect.


## V0.13A — Final Player Outgoing Damage Hook

**Status: TEST CANDIDATE**

### V0.12B result

V0.12B `GetBaseDamage` is **REJECTED**.

User testing at **100x** still did not produce an unmistakable damage increase.
Therefore `GetBaseDamage` is not the correct final applied-damage control for
the requested Melee Damage option.

### Native final outgoing-damage stage

The executable contains the player outgoing-damage filter at:

```text
RVA 0x668BE0
```

The function directly mutates:

```text
FMayhemDamageEventRecord::Damage +0x08
```

and natively applies:

```text
d.PlayerOutgoingDamageMultiplier
```

before continuing through outgoing-damage filters/status effects.

This is the lowest confirmed player-outgoing stage found so far and is
downstream of `GetBaseDamage`.

### V0.13A policy

V0.13A calls the complete native outgoing-damage filter first, then applies the
user multiplier to the resulting final outgoing Damage value.

The hook is local-player-only.

Current diagnostic classification:

```text
BaseJuice > 0
  -> Pistol Damage

BaseJuice == 0
  -> Melee Damage diagnostic
```

The pistol discriminator comes directly from the supplied Strife projectile
reference PAKs, where gun projectiles carry positive `BaseJuice`.

The zero-juice branch is intentionally still diagnostic: it may also include
some non-pistol abilities. Runtime telemetry logs `ScaleType` and damage-tag
count so the filter can be tightened after testing.

### Runtime telemetry

Overlay/logs now report:

```text
Pistol:
  event count
  final native damage -> boosted damage
  BaseJuice

Melee diagnostic:
  event count
  final native damage -> boosted damage
  ScaleType
  DamageSourceTags count
```

### Range

Both final-damage multipliers can reach 100x in this diagnostic build.

If a 100x value still has no visible effect, the problem is no longer an
upstream stat/filter issue and the next audit must move to the target-side
health subtraction path.

**Validation:** awaiting in-game test.


## V0.13B — Final Outgoing Damage + Functional Skip Intro

**Status: TEST CANDIDATE**

V0.13B combines the new final outgoing-damage pipeline with a real native
Skip Intro implementation in one cumulative build.

### Functional Skip Intro

The executable contains the native console variable:

```text
g.PlayIntroCinematicOnBoot
default = 1
```

Its registration and boot-time use were audited directly in the target EXE.

Registration xref:

```text
RVA 0x000E831D
```

Native boot read:

```text
RVA 0x0063C465

mov rax,[g.PlayIntroCinematicOnBoot_data]
cmp dword ptr [rax],0
je  skip_intro_path
```

The resolver does not hardcode the runtime address. It:

1. finds the UTF-16 CVar name;
2. resolves the unique registration LEA;
3. derives the UE4 CVar data-slot store;
4. validates the unique boot-time `cmp [CVarData],0` use;
5. waits briefly for UE4 static CVar initialization;
6. captures the vanilla value;
7. applies:
   - Skip Intro ON -> `0`
   - Skip Intro OFF -> captured vanilla value.

The control is applied **before D3D11 probe initialization** so the default
enabled state can take effect as early as possible during startup.

The Present hook also reapplies the selected state so overlay/hotkey changes are
kept synchronized. Turning Skip Intro on/off after boot naturally affects the
next startup rather than retroactively cancelling an already-started cinematic.

### UI / hotkey

`Skip Intro Videos` is no longer marked Pending.

The overlay shows:

```text
Native g.PlayIntroCinematicOnBoot control
Native CVar now
Vanilla captured value
```

F4 continues to be the default hotkey and now toggles the real native control.

### Damage

V0.13B also contains the V0.13A final player outgoing-damage hook:

```text
RVA 0x668BE0
```

Current diagnostic split:

```text
BaseJuice > 0  -> Pistol Damage
BaseJuice == 0 -> Melee Damage diagnostic
```

Both are applied after the game's native outgoing-damage filtering.

**Validation:** awaiting in-game test for both final damage and boot intro skip.


## V0.13B validation result

**VALIDATED IN GAME.**

User feedback:

```text
"oui nickel"
```

Validated cumulative behavior:

- functional native Skip Intro control through `g.PlayIntroCinematicOnBoot`;
- final player outgoing-damage hook is accepted as the new damage base;
- V0.13B remains cumulative with the previously validated manual numeric input,
  Jump Height, Glide / Flight Duration, Movement Speed, HUD and safe Action
  Recovery behavior.

V0.13B becomes the new canonical development base.

The next implementation phase targets the horse movement/stamina feature set,
using a safe horse component / ability path rather than the rejected
`Player + 0xE70` pointer assumption from V0.9A.


## V0.14A — Safe Horse Runtime

**Status: REJECTED**

V0.14A reintroduces the horse feature set after the V0.9A crash, but no longer
trusts `Player + 0xE70` as a horse object by itself.

### Structural validation chain

The previously audited native function:

```text
AMayhemPlayerCharacter::IsHorseActive
cmp qword ptr [rcx + 0xE70], 0
setne al
ret
```

is now hooked and used only as the GameThread heartbeat.

The pointer at `Player + 0xE70` is treated as a **candidate**, never as proof.

Before V0.14A reads or writes any horse gameplay property it requires:

```text
local player
  -> dynamically discovered CharacterMovement member offset
candidate horse at player + 0xE70
  -> CharacterMovement at the same member offset
horse movement + 0x190
  -> CharacterOwner == candidate horse
horse stamina fields
  -> finite and sane
horse movement vtable + 0x3D0
  -> GetMaxSpeed target inside executable .text
```

If any check fails the horse feature remains fail-open and vanilla.

### Dynamic CharacterMovement member discovery

The already validated player movement hook knows both:

```text
player CharacterOwner
player MovementComponent
```

V0.14A scans aligned pointer members on the local player for that exact
MovementComponent pointer and accepts the offset only when it occurs uniquely.

The same discovered offset is then required to work on the candidate horse.

This removes the need to guess the ACharacter CharacterMovement member offset.

### Horse Speed

The validated horse movement component provides its actual virtual
`GetMaxSpeed` target from vtable slot:

```text
+0x3D0
```

V0.14A dynamically hooks that horse-specific function when needed.

If the horse happens to share the same Mayhem GetMaxSpeed implementation as the
player, the existing movement hook handles the validated horse path without a
second hook.

The lowest positive native horse max speed observed is captured as the normal
baseline.

```text
native <= baseline * 1.08 -> Horse Speed
native >  baseline * 1.08 -> Horse Sprint Speed
```

This classification is diagnostic and telemetry is shown in the overlay.

### Horse Sprint Duration

After the actor/movement structural proof, the reflected horse properties are
used:

```text
StaminaRecoveryPercentageRate       +0x910
StaminaTotalRecoveryPercentageRate  +0x914
StaminaSprintPercentageRate         +0x918
```

Only `StaminaSprintPercentageRate` is modified.

Formula:

```text
effective drain = original drain / duration multiplier
```

A value of `0x` is treated as effectively no sustainable sprint and applies a
very high drain rather than dividing by zero.

Unmounting restores the captured original sprint-drain value when the object is
still safely writable.

### Overlay telemetry

The Horse section now reports:

- whether a structurally validated mount exists;
- discovered CharacterMovement member offset;
- native -> effective horse speed;
- NORMAL / SPRINT classification;
- captured normal-speed baseline;
- native -> effective sprint stamina drain;
- validation success / rejection counters.

**Validation:** **REJECTED IN GAME.**

User feedback:

```text
V0.14A did not find the mount even while the player was mounted.
```

The native `IsHorseActive` heartbeat itself is still useful, but the
V0.14A structural chain was too restrictive. In particular, it assumed that
the object reachable through `Player + 0xE70` could expose its
`CharacterMovement` pointer at the exact same member offset dynamically
discovered on the player character.

That assumption is now rejected. `Player + 0xE70` must not be treated as the
horse actor or as the basis for horse gameplay reads/writes.


## V0.14B - Movement-owner horse detection

**Status: REJECTED FOR HORSE SPEED**

V0.14B starts from V0.14A but removes the failed mount-identification chain.

### Detection policy

`AMayhemPlayerCharacter::IsHorseActive` is retained only as a native
**mounted-state signal** for the locally controlled player.

V0.14B no longer **assumes** that `Player + 0xE70` is a ready-to-use
`AMayhemHorseCharacter`.

The native pointer is now treated only as an **opaque candidate**. While mounted,
the mod safely scans aligned pointer members of that candidate and accepts a
movement component only when:

```text
candidate member pointer
  -> CharacterOwner +0x190 points back to the same candidate
  -> MovementMode is sane
  -> GetMaxSpeed slot +0x3D0 points inside executable .text
candidate actor
  -> horse stamina fields +0x910/+0x914/+0x918 are readable and sane
```

This specifically removes the failed V0.14A assumption that the horse movement
pointer must exist at the exact same member offset as the player movement
pointer.

A second fallback path remains active: while the native mounted state is true,
the validated player `GetMaxSpeed` hook observes live non-player movement
owners and can adopt one as the horse after the same structural checks.

Only after structural proof does the mod:

- capture the horse runtime state;
- enable Horse Speed / Horse Sprint Speed classification;
- apply Horse Sprint Duration through the captured native stamina drain;
- install a dedicated horse GetMaxSpeed hook only if the mount does not share
  the already hooked player movement target.

This keeps V0.14B fail-open if neither discovery path can prove the mount.

### Diagnostics

Horse validation failures now keep a reason code and only log when the reason
changes, avoiding per-frame log spam.

### Log policy change

Starting with V0.14B, `DarksidersGenesisMod.log` is truncated once at startup.
The log therefore contains **only the current game session** and is no longer
cumulative across launches.

### Camera roadmap additions

Added to the TODO list:

- **Character Zoom / Camera Distance**
- **Camera Angle**

These remain separate from FOV and Third Person so each camera behavior can be
tuned independently.

**Validation:** awaiting in-game test.


## V0.14C - Direct Horse Movement Properties

**Status: TEST CANDIDATE**

V0.14C keeps the safer horse discovery work from V0.14B but changes the actual
speed-control mechanism.

### Why V0.14B is not the speed base

V0.14B attempted to multiply the result of the horse movement
`GetMaxSpeed` virtual. In-game testing did not produce perceptible horse
acceleration.

The reference Horse PAK provides a much stronger gameplay target because it
changes the horse CharacterMovement defaults directly:

```text
MaxWalkSpeed      1300.0 -> 1500.0
MaxAcceleration    600.0 -> 700.0
```

V0.14C therefore treats the GetMaxSpeed hook as telemetry only.

### Runtime offsets under test

The target executable already gave three exact CharacterMovement anchors:

```text
CharacterOwner  +0x190
JumpZVelocity   +0x1A0
MovementMode    +0x1B0
```

These align with the UE4 CharacterMovement property sequence used by this
engine build. V0.14C derives the following **test candidates**:

```text
MaxWalkSpeed    +0x1D4
MaxAcceleration +0x1E8
```

These are not blindly written. The horse candidate must first pass all V0.14B
structural validation and the live values must also fall inside horse-specific
ranges consistent with the reference PAK:

```text
MaxWalkSpeed    1000 .. 2000
MaxAcceleration  250 .. 1500
```

This intentionally rejects the known local-player movement defaults such as
the much higher player MaxAcceleration.

### Application model

Once the horse movement component is validated, V0.14C captures its native
values once and applies:

```text
effective MaxWalkSpeed    = native MaxWalkSpeed    * HorseSpeedMultiplier
effective MaxAcceleration = native MaxAcceleration * HorseSpeedMultiplier
```

Disabling Horse Speed or unmounting restores the captured native values.

The direct values are re-applied while mounted so runtime overlay changes take
effect without requiring a remount.

### Sprint separation

**Horse Sprint Speed is not claimed as implemented by V0.14C.**

The reference assets indicate that sprint has its own ability/runtime
`RunSpeed` path. Until that path is resolved, Horse Sprint Speed remains
pending instead of pretending that GetMaxSpeed scaling works.

Horse Sprint Duration remains a separate stamina-drain feature through
`StaminaSprintPercentageRate`.

### New telemetry

The Horse overlay reports:

- native and applied `MaxWalkSpeed`;
- native and applied `MaxAcceleration`;
- native mounted signal;
- horse/player movement member offsets;
- GetMaxSpeed telemetry only;
- validation/rejection counters and last rejection reason.

Additional V0.14C rejection reasons:

```text
9  direct movement properties unreadable
10 direct movement values outside horse-specific range
11 MaxWalkSpeed / MaxAcceleration not writable
```

**Validation:** **REJECTED AS A RUNTIME TEST.**

User feedback:

```text
Horse speed still does not change.
```

The supplied V0.14C log explains why this result does **not** invalidate the
direct MaxWalkSpeed / MaxAcceleration path:

```text
Horse runtime: IsHorseActive signature match count=0
Horse runtime: IsHorseActive resolver failed; horse features remain fail-open
Horse runtime unavailable; horse features remain fail-open
```

No later `mounted`, `VALIDATED`, `MaxWalkSpeed`, or horse rejection-reason
telemetry appears. Therefore V0.14C never reached horse discovery or any direct
movement write.

The same log also exposed excessive repeated diagnostics:

```text
Horse runtime: CharacterMovement member discovery ambiguous matches=2
```

This repeated thousands of times and is treated as a logging bug.




## V0.14D - Horse Heartbeat Resolver Fix

**Status: TEST CANDIDATE**

V0.14D fixes the root cause proven by the V0.14C runtime log.

### Remove the failed IsHorseActive byte hook

The audited native logic is still:

```text
AMayhemPlayerCharacter::IsHorseActive
    Player + 0xE70 != nullptr
```

V0.14A-C tried to locate and hook a tiny compiled helper implementing that
check. In the actual V0.14C session the resolver returned zero signature
matches, disabling all horse features before gameplay.

V0.14D no longer installs or requires that hook.

Instead, the already validated
`UMayhemCharacterMovementComponent::GetMaxSpeed` hook acts as a GameThread
heartbeat. From the last known locally controlled player it safely reads:

```text
Player + 0xE70
```

and derives the same mounted state directly:

```text
null     -> unmounted
non-null -> mounted candidate present
```

The pointer remains an opaque candidate only. It is never trusted as a horse
actor until the full structural validation chain passes.

### Discovery sequence

While mounted, V0.14D:

1. reads the opaque candidate from `Player+0xE70`;
2. scans that candidate for a unique movement-like member;
3. requires `movement+0x190 -> CharacterOwner == candidate`;
4. validates MovementMode and executable GetMaxSpeed vtable target;
5. validates horse stamina fields;
6. validates the direct movement property candidates;
7. only then captures and modifies the horse.

The V0.14B movement-owner fallback remains available as a second path.

### Direct speed experiment retained

Because V0.14C never executed the horse code, these candidates remain
**unvalidated rather than rejected**:

```text
MaxWalkSpeed    +0x1D4
MaxAcceleration +0x1E8
```

If V0.14D reaches validation, the overlay/log should finally expose the native
and applied values and provide a real in-game test of this path.

### Logging cleanup

The log remains truncated at every game launch.

V0.14D additionally suppresses per-frame diagnostic floods:

- ambiguous CharacterMovement member count is logged only when the count changes;
- rejected non-player movement captures have a small per-session log budget;
- horse rejection reasons still log only when the reason changes.

This keeps the current-session log useful instead of producing tens of
thousands of duplicate lines.

**Validation:** awaiting in-game test.


## V0.14E - UI Defaults + Jump Height 20x + Flat Artifact

**Status: TEST CANDIDATE**

V0.14E is cumulative from V0.14D.

### Jump Height

The previous overlay/runtime ceiling of 5x is raised to:

```text
0.00x .. 20.00x
```

The runtime clamp is raised at the same time, so values above 5x are effective
rather than UI-only.

The default remains:

```text
JumpHeightMultiplier=1.250
```

### Per-value Default buttons

Every currently tunable numeric control now exposes a `Default` button next
to its slider/manual field.

Defaults restored by those buttons are:

```text
Movement Speed         1.50x
Action Recovery        0 ms
Jump Height            1.25x
Glide / Flight         10.00x
Pistol Damage          2.00x
Melee Damage           2.00x
Hotstreak Charge       2.00x
Horse Speed            1.25x
Horse Sprint Speed     1.25x
Horse Sprint Duration  2.00x
FOV                    90 deg
Third Person Distance  1.00x
```

Pressing `Default` updates the in-memory value and saves it to the INI.

### Packaging fix

V0.14D's GitHub Actions artifact contained a ZIP file inside the GitHub artifact
ZIP.

V0.14E removes the inner `Compress-Archive` package from the artifact path.
The GitHub artifact now contains the four distributable files directly:

```text
dxgi.dll
DarksidersGenesisMod.asi
DarksidersGenesisMod.ini
README.md
```

Downloading the Actions artifact therefore produces one ZIP with the usable
files at its root, not a ZIP containing another ZIP.

**Validation:** awaiting in-game test.


## V0.14F - Safe Horse Diagnostics

**Status: TEST CANDIDATE**

V0.14F is cumulative from V0.14E and keeps:

- Jump Height up to 20x;
- per-value `Default` buttons;
- flat GitHub artifact packaging;
- all previously validated non-horse hooks.

### Root cause found in the V0.14E runtime log

The V0.14E session reached the mounted-state heartbeat successfully, but then
the broad mounted-time fallback treated every non-local movement owner entering
the shared `GetMaxSpeed` hook as a possible horse.

The resulting log contained roughly **301k horse rejection lines in one
session**, alternating primarily between:

```text
reason=7  no unique horse movement member found
reason=3  horse stamina fields outside sane range
```

No `Horse runtime: VALIDATED` or `MaxWalkSpeed` line appeared, so the horse
speed write path was never reached. The failure was discovery/logging pressure,
not a proven failure of the direct movement-property idea.

### V0.14F safety changes

1. Removes the broad non-local movement-owner horse fallback.
2. Uses only the opaque object reached from `Player+0xE70` as the discovery
   root.
3. Throttles the expensive horse discovery scan to **one attempt per second**.
4. Rate-limits horse rejection logging to **one line per second maximum**, even
   when rejection reasons alternate.
5. Keeps all horse writes disabled until the candidate passes the complete
   structural validation chain.
6. Adds bounded diagnostics at mount time:
   - opaque candidate pointer;
   - raw stamina triplet at +0x910/+0x914/+0x918;
   - number of movement-like members;
   - first four candidate member offsets.

This candidate is intentionally diagnostic-first. Stability of the already
validated player/combat/UI features has priority over forcing horse values.

**Validation:** **STABILITY VALIDATED IN GAME.**

User feedback:

```text
Tout est redevenu normal.
```

Validated in V0.14F:

- general game/mod stability restored;
- no V0.14E-style horse log storm;
- existing validated non-horse features remain usable;
- Jump Height 20x UI/runtime ceiling retained;
- per-value Default buttons retained;
- flat artifact packaging retained.

Horse speed / sprint behavior is **not yet validated**. The horse path remains
diagnostic-first and must not be promoted to canonical functionality until the
runtime structure is identified and tested successfully.


## V0.16A - Clean Shared-Hook Horse Test

**Status: TEST CANDIDATE**

V0.15A / B / C horse and Skip Logos experiments are not used as a runtime
base for this build.

V0.16A restarts from the V0.14F stability-validated branch and imports only the
clean source split introduced later.

The supplied V0.15C runtime log revealed that the already validated
`UMayhemCharacterMovementComponent::GetMaxSpeed` hook sees several non-player
movement components in normal gameplay. Those components showed `JumpZ=600`
while the local player movement components showed `JumpZ=900`.

That evidence makes the extra horse hooks, actor scans, Player+0xE70 path and
vtable ancestry resolver unnecessary.

### Runtime model

The existing shared GetMaxSpeed hook now does exactly this:

```text
GetMaxSpeed(movement)
  -> read CharacterOwner
  -> determine whether owner is local player
  -> HorseFeature observes non-local component
  -> normal validated player path continues unchanged
```

HorseFeature installs **no hook**.

For at most eight unique non-player movement components it records one compact
fingerprint:

```text
native GetMaxSpeed
MovementMode
JumpZVelocity
MaxWalkSpeed
MaxAcceleration
StaminaRecoveryPercentageRate
StaminaTotalRecoveryPercentageRate
StaminaSprintPercentageRate
```

A component is automatically accepted only when it matches the reference Horse
PAK signature:

```text
MaxWalkSpeed              ~= 1300
MaxAcceleration           ~= 600
StaminaRecovery           ~= 15
StaminaTotalRecovery      ~= 40
StaminaSprintDrain        ~= 25
```

Only after that exact signature is proven does the feature write:

```text
MaxWalkSpeed
MaxAcceleration
StaminaSprintPercentageRate
```

No candidate rejection spam is emitted.

Horse Sprint Speed remains pending its independent `RunSpeed` primitive.

### Skip Logos

Skip Logos is intentionally **not modified in V0.16A**. The previous file-hook,
command-line and literal-string query approaches are rejected. A new Skip Logos
attempt will not be shipped until a native target is proven from the retail
executable or startup configuration.

**Validation:** awaiting in-game test.


## Horse Direction Reset - Reference PAK Becomes Authoritative

The user correctly pointed back to the supplied working horse reference mod.
This changes the implementation priority.

Reference:

```text
ZZZ-Horse_P.pak
SHA-256 3719ac840e1d0d7f137c9322580a3abe58cc5cf93d4b1ea97352c66490d3a920
```

The reference PAK contains direct edits to the horse Blueprint defaults and
therefore provides a stronger source of truth than indirect movement-hook
experiments.

### Proven vanilla -> mod horse deltas

```text
GallopSpawnSpeedThreshold           300.0 -> 400.0
StaminaRecoveryPercentageRate        15.0 -> 100.0
StaminaTotalRecoveryPercentageRate   40.0 -> 100.0
StaminaSprintPercentageRate          25.0 -> 0.0

MaxWalkSpeed                       1300.0 -> 1500.0
MaxAcceleration                     600.0 -> 700.0
BrakingFrictionFactor                 1.0 -> 2.0
```

### New implementation rule

Future horse work must prefer direct control of the same Blueprint/CDO
properties proven by the reference PAK:

```text
MayhemHorseCharacter_Blueprint
  MaxWalkSpeed
  MaxAcceleration
  BrakingFrictionFactor
  GallopSpawnSpeedThreshold
  StaminaRecoveryPercentageRate
  StaminaTotalRecoveryPercentageRate
  StaminaSprintPercentageRate
```

The V0.14/V0.15 experimental paths remain useful historical diagnostics but are
no longer the preferred architecture:

- Player+0xE70 horse-pointer interpretation;
- actor/member scans;
- secondary/base GetMaxSpeed hooks;
- vtable ancestry resolver.

Horse Sprint Duration continues to map to stamina drain:

```text
effective drain = vanilla StaminaSprintPercentageRate / duration multiplier
```

Example with vanilla drain 25:

```text
1.0x -> 25.0
2.0x -> 12.5
5.0x -> 5.0
10x  -> 2.5
infinite -> 0.0
```

Horse Sprint Speed remains separate and must be resolved from the sprint ability
data rather than conflated with stamina duration.

### Skip Logos

Skip Logos remains paused after the rejected V0.15A/B/C methods. No new Skip
Logos candidate should be shipped until a native retail target is proven.


### V0.16A local-control correction

The first V0.16A draft passed a boolean derived from
`APawn::IsLocallyControlled` into HorseFeature and skipped those candidates.

That was incorrect for mounts: a horse controlled by the local player can
itself be locally controlled.

The final V0.16A ordering is therefore:

```text
shared GetMaxSpeed hook
  -> HorseFeature observes every movement component
  -> exact reference-PAK property signature decides horse identity
  -> APawn::IsLocallyControlled is evaluated only afterwards
  -> normal player movement tuning path continues unchanged
```

Horse identification is now independent from pawn control state.


## V0.19 - Skip Logos root cause and validated fix

The final root cause was not the generic UE4 startup-movie path. The retail
executable contains a ProjectMayhem-specific plugin:

```text
ProjectMayhem/Plugins/StartupScreens/
```

with `SStartupScreens`, `StartupScreensModule`,
`UStartupScreensSettings`, `StartupScreenDef` and `StartupMovies`.

Three plugin-level candidates were tested in game:

```text
V0.19A  StartupModule RET @ 0x25FE40
        Logos skipped
        Intro also skipped with Skip Intro OFF
        -> rejected, too broad

V0.19B  force StartupMovies copied count to 0 @ 0x25FF31
        Logos skipped
        Intro also skipped with Skip Intro OFF
        Warning message remains
        -> rejected, too broad

V0.19C  bypass MoviePlayer attachment block
        0x260244 -> 0x260257
        Logos skipped
        Skip Intro OFF still shows warning + intro cinematic
        Skip Intro ON remains independent
        -> VALIDATED
```

A slightly longer black transition is visible with Skip Intro OFF, but was
reported as minor/non-problematic.

Final rule for Skip Logos:

```text
Do not disable StartupScreens globally.
Do not empty its whole movie playlist.
Bypass only SStartupScreens attachment to MoviePlayer.
```

V0.19D freezes this exact V0.19C behavior as the validated baseline.


## V0.27-V0.28 - Native HorseCharacter resolver succeeds

V0.27 abandons all indirect horse identification paths and hooks native
`HorseCharacter` functions directly. This produced the first confirmed in-game
horse pointer and exact native stamina values.

Observed validated horse state:

```text
recovery     = 15
totalRecovery= 40
cooldown     = 2
sprintRate   = 25
stamina      = 62 / 62
```

Validated runtime fields used by Horse Sprint Duration:

```text
HorseCharacter +0x918 = StaminaSprintPercentageRate
HorseCharacter +0x9C0 = CurrentStamina
HorseCharacter +0x9C4 = MaxStamina
```

The duration feature preserves the stamina pool and scales effective duration by
reducing `StaminaSprintPercentageRate`:

```text
multiplier <= 0 -> native sprintRate
multiplier > 0  -> native sprintRate / multiplier
```

V0.28 user-facing policy:

```text
minimum 0x   = vanilla
default 5x
maximum 20x
```


## V0.29 - Correct horse movement ownership and field offsets

The V0.27 log proves that native HorseCharacter capture is correct, but also
shows `movementReady=0`. Re-audit of `HorseCharacter::GetNormalizedSpeed`
and the UE4 reflection tables identifies two independent causes:

1. The virtual call at HorseCharacter vtable `+0x5F8` already returns the
   horse movement component. Rejecting that pointer with
   `movement+0x190 == horse` was invalid.
2. Earlier candidates used neighboring UCharacterMovementComponent offsets.

Exact reflected movement layout:

```text
MaxWalkSpeed            0x1DC
MaxWalkSpeedCrouched    0x1E0
MaxSwimSpeed            0x1E4
MaxFlySpeed             0x1E8
MaxCustomMovementSpeed  0x1EC
MaxAcceleration         0x1F0
MinAnalogWalkSpeed      0x1F4
BrakingFrictionFactor   0x1F8
BrakingFriction         0x1FC
```

`SprintingMaxSpeed +0x760` belongs to
`UMayhemHorseCharacterMovementComponent`, not to `AMayhemHorseCharacter`.
Native disassembly corroborates this: movement code reads its owner horse's
`bSprinting +0x8D0` and then reads `this+0x760` when sprinting.

V0.29 therefore:
- trusts the HorseCharacter native movement getter;
- writes Horse Speed to movement `MaxWalkSpeed / MaxAcceleration`;
- writes Horse Sprint Speed to movement `SprintingMaxSpeed`;
- preserves V0.28 Sprint Duration on HorseCharacter stamina drain.


## V0.31 - Crash audit: stale Horse UObject writes

A user crash dump after a level load showed:

```text
Exception: 0xC0000005 write access violation
Fault RVA: DarksidersGenesis-Win64-Shipping.exe +0xCC0FBC
Faulting instruction: lock xadd dword ptr [rbx+8], eax
Corrupt ref-count pointer base: rbx = 0x1E3000010
Attempted write: 0x1E3000018
```

The fault occurs in the game's reference-counted destruction path, not inside
the ASI module. Combined with pre-crash reticle/HUD corruption, this is consistent
with earlier memory corruption.

Source audit found the unsafe producer in HorseFeature: raw `horse` and
`movement` pointers were retained indefinitely and dereferenced/written from
`Tick()` every frame, including after seamless travel.

V0.31 invariant:

```text
A raw HorseCharacter pointer may be dereferenced for tuning only while one of
the hooked native HorseCharacter functions is currently executing with that
pointer as its live this-object.
```

No asynchronous/per-frame restore or write through cached horse pointers is
permitted. This rule should be preserved in future camera/mount work as well.


## V0.32-V0.34 - Malformed reticle after load, focus-cycle solution

- V0.31's live-only native HorseCharacter writes eliminated the known unsafe
  cached-UObject write pattern. This rule is immutable for later builds.
- V0.32 deferred ImGui/RTV/WndProc initialization until first overlay open.
  The reticle issue reproduced without ImGui initialization, and a real
  Alt-Tab repaired it without a D3D11 ResizeBuffers call.
- V0.33 hooked `UAirshipUIManager::IsCursorVisible` and posted WM_SETCURSOR
  three times following player detection. The user reported **no visual fix**;
  the log confirms that the messages were posted, so that strategy is rejected.
- V0.34, branched from V0.33, removes the ineffective automatic refresh loop
  and adds the remappable `ReticleFocusTest` action (F5 in the included INI).
  One press queues a synthetic WM_ACTIVATEAPP/WM_ACTIVATE deactivation, then
  reactivation with WM_SETFOCUS and WM_SETCURSOR; cursor state is logged.
- **User validation, 2026-10-08:** "parfait ca marche" after testing V0.34.
  Accept the **manual F5 reticle correction** as working. This does NOT prove
  automatic correction on level load and does NOT separately validate hiding
  the cursor with F1 Hide HUD.
- The Win32 message pulse is a workaround, not a new engine-level cursor or
  reticle asset hook. Keep an explicit hotkey and preserve the safe rollback
  path for focus anomalies. No cursor refresh writes to HorseFeature memory.
- `main` should contain V0.30 GraphicsAdapter, V0.31 horse lifetime safety,
  V0.32 lazy overlay and V0.34 manually validated cursor focus pulse.


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
