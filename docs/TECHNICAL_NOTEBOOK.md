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

## StartupScreens audit after V0.43 confirmation

`README.md` links a detailed engineering audit at
`docs/STARTUPSCREENS_AUDIT.md` documenting the existing native call
sequence, the rejected v0.19A/B/C methods, relevant Unreal MoviePlayer
APIs, the cross-cursor A/B result, and the exact remaining static
reverse-engineering tasks. No hook or runtime patch changes.

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

## V0.55: native pre-warning hook and camera-distance DOF compensation

1. Direct disassembly of the exact 62,113,280-byte original EXE at RVA 0x25FEB7..0x25FEEA proves that the StartupScreenDef array (+0x50, 0x40 stride) is iterated BEFORE StartupMovies are copied at 0x25FF31. The V0.54 late change was necessarily ineffective for this loop.
2. V0.55 changes the warning mechanism: a new separate signed 7-byte detour at RVA 0x25FEBF (48 C1 E7 06 48 03 FB), using a nearby executable island that replays SHL/ADD. It saves/restores RAX and, only with boot SkipWarning=1 and an observed native end pointer exactly 0x80 bytes beyond start (2x0x40 definitions), changes RDI to RBX so the native loop at 0x25FED0 does not iterate. No heap calls, UObject writes, source array count mutation, MoviePlayer bypass, synthetic focus, or game asset renaming. Separate booleans track whether the native hook was executed and its count guard matched. Fail open on signature/count mismatch; user visual confirmation pending.
3. Manual native EXE disassembly of r.DepthOfFieldQuality registration verifies name LEA at RVA 0x12259D, the final value-slot MOV at RVA 0x1225C5, and value pointer slot RVA 0x381DEA8. The ASI verifies both 7-byte signatures and exact retail EXE identity. While Third Person or camera Zoom is active, it sets the runtime native CVar to 0, and restores the captured 0–4 native quality when the transforms turn off or on normal mod shutdown. This intentionally avoids wrong focal blur by temporarily disabling DOF; no genuine FPostProcessSettings focal recalculation is yet implemented.
4. F5 Third Person becomes default, camera distance numeric input + Default are implemented; all functional feature toggles and eight camera actions plus Reset are available in F1–F12, no ReticleFocusTest. INI revision 2105 migrates only old F5=None while preserving user choices, and all ZIP files stay at root.

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


## V0.58 live test: video + log 2026-10-09 (REJECT TPS combat mapping)

User-provided 38.95 s 2560x1440/59.94fps gameplay video and matching 21:25:09-21:27:11 mod log have been examined. The V0.58 dual-stick TPS prototype is **not validated**. In video, Strife repeatedly faces sideways or toward camera, not reliably back to the camera when shooting; movement + shooting produces visibly incoherent aim direction. Camera/character yaw outruns or diverges. User specifically requests shooter/thrower facing away from camera, actor turn with camera without overshoot, and left-stick backward/sideways strafing.

Log: exact supported EXE SHA9f470202...; V0.58 TPS strafe sample #1 at 21:26:27.673: yawDelta=48.8°, leftRaw=(1364,24142), mapped=(19074,14862). Combat aim #1 at 21:26:28.339: RT=255, RB=0, yawDelta=33.8°, native RX/RY=(18216,27237). Combat aim #120 at 21:26:30.358: RT=255, yawDelta=170.5°, RX/RY=(5436,-32313). Shutdown totals: combatAim=1022, strafeRemap=1059, nativeFireOrThrowPolls=1022, native mouse blocks=628 and right-stick filtered reads=2672. The delta changed about 136.7° in ~2 s. This *does not by itself prove* positive feedback because user may have moved camera, but shows the aim target is not stable with native camera yaw. Static 32767-full-scale synthetic stick also makes the player snap independently of actual camera rotation.

Root-cause architecture: V0.58 samples `GetCameraView` telemetry (`appliedYaw-nativeYaw`), rotates an XInput full-scale virtual right stick only during RT/RB, and rotates left stick by the same instantaneous difference. It does NOT control native Actor/Controller facing, bOrientRotationToMovement, input-to-world conversion, or native weapon/grenade target selection. Camera yaw is merely changed in *transient* `FMinimalViewInfo`; actor yaw and weapon yaw remain uncontrolled. Stronger/faster synthetic stick, arbitrary offset calibration, and static actor pointer writes would not be a justified repair. V0.57 native input gating and camera/DOF are intact.

**Next prerequisite:** audit native player-facing and movement controller using exact `DarksidersGenesis-Win64-Shipping.exe` (62,113,280 B / SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`), not currently available as mountable raw bytes. Need identify native rotation/aim/throw and movement APIs, their exact signatures and validation against EXE, so new TPS logic can lock player heading to actual camera-forward only when shooting/throwing; preserve proper strafing and disable immediately when Third Person OFF. Preserve V0.57 baseline and reversibility; V0.58 should remain test-only. User INI helpful for exact camera settings.

Skip Warning remains separate/nonfunctional: native definition span 0xC0 indicates three 0x40-size entries and existing gate only bypasses counts 1/2; last log seen=1 skipped=0. Defer as user requested. No V0.59 compiled yet; do not imply that these notes are a new build.

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


## V0.61 follow-up (2026-10-09): runtime evidence from user log

User attached `DarksidersGenesisMod(20261009-203809).log` and gameplay video `ProjectMayhem   2026-10-09 22-37-10.mp4`. The log was read fully. Video visual inspection could not be completed this turn because container/Python tools returned ClientError repeatedly; do **not** claim to have visually validated its contents.

Session 22:35:09–22:37:50:
- V0.61 ASI loaded under exact validated executable (size 62,113,280, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`).
- Native ground aim FX hook READY; native strafe lock triggered 25 times and restored 25 times; no observed leaked flags in logged callbacks, but this is not proof of correct strafing animations or actor tracking.
- `TPS V0.61 totals: nativeStrafeLocked=25 restored=25 groundFXHidden=0` (could mean checkbox OFF or no matching native display callback; the log does **not** distinguish them).
- `combatAim=3061 experimentalStrafeRemap=0 ... liveActorYaw=24067 leftStickPassthrough=2766 movingFire=1670`. Experimental leftstick remapping remained OFF as intended.
- At 22:36:17.631: cameraYaw=-46.2°, actorYaw=-135°, actorGap=+88.8°, commandYaw=-132.8°; at 22:36:21.640: cameraYaw=-66.4°, actorYaw=-134.9°, gap=+68.5°, commandYaw=-66.4°. These demonstrate that native facing bit toggle is **not sufficient to guarantee Strife faces away from camera**. Desired command yaw may match camera yaw while actor yaw diverges; a native aim/control rotation integration is still needed.
- NO `TPS V0.61: native aim camera drift locked` line in log. Possibilities include runtime camera-pivot checkbox disabled, stale/unusable actor location, missing idle baseline when attack starts, or hook branch not reached. The log does not identify which. Instrument checkbox state and camera pivot gating conditions before making another speculative camera transform.
- Skip Warning still no effect: native warning span=0xC0/3 definitions, seen=1 skipped=0. Out of scope of present TPS test.

Next evidence-driven build priorities:
1. Add one-shot config snapshot and non-spam camera pivot gate telemetry (actor position validity/freshness, aiming state, baseline existence and pivot change) to determine why stabilization is not observed. Do not silently claim "fixed".
2. Add calls-seen and forced-hidden counters for the native `HideGroundTargetingEffect` function separately; inspect whether generic Strife aiming FX uses a different UFUNCTION instead of claiming this hook affects it.
3. Find the native authority controlling actor yaw/aim target and movement facing in the exact executable. Existing `bOrientRotationToMovement` bit test is proven to execute but **not** proven to implement strafe. Focus on actor yaw vs camera yaw and control/aim updates; avoid synthetic full-scale stick.
4. Re-examine the supplied gameplay video when local media tool access is available. In its absence, do not infer observed visual effects.
5. Preserve all validated mod subsystems (V0.55 DOF, V0.57 aim isolation, horse, HUD, reticle) and treat V0.61 as experimental. No V0.62 created or tested in this evidence-only update.

## V0.62 (9 October 2026): full-time TPS facing test (unvalidated)

User request: "il faut tester d'avoir le personnage toujours dos à la camera". V0.61 activates native `bOrientRotationToMovement` bit clearing only while RT/RB held. Log previously showed 25 locks/25 restores and large yaw error despite gamepad synthetic combat aim. V0.62 tests keeping Strife's **native facing directed by camera all the time**, whether idle, moving forward/backward/sideways or firing.

New `TPSAlwaysFaceCamera=1` (ConfigRevision 2110), with an independent overlay checkbox in Camera. ON only in ThirdPerson with `CameraMouseGamepad=1`, focus on exact supported executable, closed ImGui, controller index 0, and fresh validated live player sample <200ms. The XInput native aim path is now synthesized continuously (not only during RT/RB), using existing camera absolute yaw, rate-limited to configured camera stick speed, and 55%-magnitude native right stick. The camera samples the actual original controller right stick through XInput trampoline, unaffected by this. The authoritative movement `bOrientRotationToMovement` bit (+0x240, mask 0x10) is cleared in the *validated live player GetMaxSpeed callback* while recent (220ms) camera-facing aim pulses exist, allowing left stick to strafe without reorienting to movement. Only owned flag is restored on the next valid native callback after TPS OFF, option OFF, game focus/overlay loss or inactivity; never restore through stale cached UObject pointer. While locked, if the game reasserts the bit, it is cleared again and separately counted. Left stick remains **unchanged**; old experimental left-stick remap stays OFF.

New logs `TPS V0.62: always face sample` include cameraYaw, real actorYaw, signed yaw error, commanded yaw, left axes, firing state; final command/reassert totals. In-game test must check idle rotation, moving in all directions, shooting, LB wheel, F5 OFF, old effect indicators and camera pivot. This is still a synthetic native aim route, NOT verified direct actor rotation. It may cause the native vanilla aim visual outside shooting, especially until that visual is identified properly, and may not perfectly lock actor orientation depending on native aim code. Fail-open if no active validated player/controller/camera; in-game test result essential before declaring functional. Existing ThirdPerson pose, native visual ground FX option, camera pivot stabilization, FOV, DOF, horse, combat damage, HUD, reticle, intro skip and logos unchanged. Skip Warning still does NOT WORK and is unchanged.

## V0.63 (10 October 2026): rejected always-face reverted; camera pose defaults & native facing cone

User rejected V0.62 Always Face Camera: it simulates Ctrl-like permanent aim until dodge and restricts some movement. Log showed `alwaysFaceCommands=15855` even when RT=RB=0. All idle synthetic right-stick input is removed; TPS combat synthetic aiming is active only with RT/RB and `TPSControllerCombatAim=1` (default ON), while existing V0.57 idle input isolation stays. Experimental left-stick rotation was observed 3542 times in the user's log; implementation, UI and INI option are removed completely. Native LX/LY are always unchanged.

New [Features] `TPSCameraFacingGuard=1` and [Values] `TPSFacingToleranceDegrees=90` (20..180): in the exact validated live-player movement callback, compare actual player root yaw against final camera yaw. Inside this 90-degree cone, temporarily disable `bOrientRotationToMovement` bit +0x240/0x10 to reduce automatic movement-oriented turning without forced aiming. Outside the cone restore the native bit and allow vanilla orientation; combat lock from V0.61 still applies while RT/RB. **This does not directly rotate the character** and is experimental; only full game testing can validate strafing and dodge. Log counts cone IN/OUT, owned lock/restores.

New Third Person Distance range 0.0..3.0, default 0.50 (0 radial arm distance now genuinely allowed by camera calculation, old clamp min 120 removed). Vertical offset default +180 (was +60). Overlay Default controls and F-key Camera Reset updated. ConfigRevision 2111 migration updates only unchanged prior defaults 1.0/60.0 and preserves customized values; removes old deprecated INI keys.

Hide Vanilla Ground Aiming Effect test was nonfunctional (native hook installed yet zero calls). Remove that misleading option and hook while continuing future search for the actual visual effect path. Other camera pivot stabilization, DOF, horses, HUD, reticle, skip intros/logos unchanged. Skip Warning still fails span 0xC0 and remains deferred. Build is V0.63 TEST pending validation.
