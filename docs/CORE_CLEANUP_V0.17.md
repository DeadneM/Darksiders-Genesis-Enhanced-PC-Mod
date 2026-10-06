# V0.17 Core Cleanup Report

Date: 2026-10-06

Branch:

```text
dev/v0.17-core-cleanup
```

Code build validated at:

```text
c119542ceb145c4cb17a7b2c0856d593a8d1fe14
```

Canonical `main` remains intentionally unchanged on V0.13B.

## Purpose

V0.17 is a consolidation pass. It does not add a new camera, logo or sprint
feature.

Its job is to make the already validated mod core safer and maintainable before
more gameplay work continues.

## Completed audit actions

### Exact executable validation

New module:

```text
TargetValidator.cpp
TargetValidator.h
```

The mod now verifies:

```text
DarksidersGenesis-Win64-Shipping.exe
size   62,113,280 bytes
SHA256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
```

Policy:

```text
exact executable -> gameplay hooks may install
mismatch         -> overlay + log only
```

Hash I/O uses a heap buffer instead of a 1 MiB stack allocation.

### Player identity separated from local pawn state

New module:

```text
PlayerIdentity.cpp
PlayerIdentity.h
```

The old generic `APawn::IsLocallyControlled` filter is no longer used as a
player-character identity test.

The selected player requires:

```text
local pawn
CharacterOwner back-pointer
player-range MaxWalkSpeed
player-range MaxAcceleration
valid JumpZ
valid DoubleJumpZ
valid GlideDuration
```

The reference player shape deliberately excludes the proven horse signature
1300 MaxWalkSpeed / 600 MaxAcceleration.

This protects:

- player Movement Speed;
- Jump / Glide property writes;
- `g_localPlayerCharacter`;
- Action Recovery instigator filtering;
- final outgoing-damage ownership.

### Runtime config made thread-safe

New module:

```text
RuntimeSettings.cpp
RuntimeSettings.h
```

Gameplay hooks no longer read mutable ImGui configuration fields.

The editable config publishes to atomics. Hook-thread reads use those atomics
with relaxed ordering because each setting is an independent scalar policy.

Final static audit:

```text
runtime hook functions with direct g_config reads: 0
```

### INI / hotkeys extracted and debounced

New module:

```text
ConfigStore.cpp
ConfigStore.h
```

ConfigStore owns:

- INI loading;
- INI writing;
- defaults;
- F1-F12 actions;
- menu-key conversion;
- runtime publication;
- dirty state;
- 500 ms persistence debounce.

Dragging a slider no longer rewrites the complete INI every rendered frame.

Explicit Save still writes immediately.

Global ConfigStore construction is side-effect free; runtime publication starts
from MainThread / Load.

### Overlay UI extracted

New module:

```text
OverlayUi.cpp
OverlayUi.h
```

The menu receives an explicit context rather than reading engine globals
directly.

The context contains:

- config;
- read-only telemetry snapshot;
- target-validation result;
- HUD/menu interactive atomics;
- selected callbacks.

### Historical hook code removed from runtime

These old installers and their associated code are no longer present in the
compiled core:

```text
InstallPistolDamageHook
InstallMeleeDamageHook
InstallActionRecoveryHook
InstallDodgeRecoveryHooks
InstallInputSuppressWindowHooks
InstallCommonActionRecoveryHook
```

The active installer set is now exactly:

```text
InstallSkipIntroControl
InstallHudHook
InstallMovementSpeedHook
InstallActionRecoveryV08B
InstallHotstreakChargeHook
InstallFinalOutgoingDamageHook
```

Each has one definition and one call site.

### Shutdown / restore path

V0.17 adds controlled process-exit cleanup:

```text
flush dirty config
HorseFeature::Shutdown / native value restore
clear PlayerIdentity
restore Skip Intro CVar
restore WndProc
shutdown ImGui backends/context
release RTV / D3D11 context / device
disable MinHook hooks
MH_Uninitialize
```

### Horse safety hardening

HorseFeature remains isolated and installs no extra gameplay hook.

V0.17 additionally:

- only spends candidate-log slots on broad horse-shaped movement components;
- validates the movement -> CharacterOwner back-pointer before restoring values;
- skips restore when the captured object identity is no longer valid.

The long-term source of truth remains the supplied working `ZZZ-Horse_P.pak`.

### Placeholder features made honest

Defaults now use:

```text
HorseSprintSpeed=0
FOV=0
ThirdPerson=0
F5=None
```

Their overlay controls are disabled until real native implementations exist.

Skip Logos remains paused.

### Documentation cleanup

The historical root README was preserved in:

```text
docs/TECHNICAL_NOTEBOOK.md
```

The root README is now a concise installation/current-status page.

### Reproducible dependencies

Pinned commits:

```text
MinHook
c3fcafdc10146beb5919319d0683e44e3c30d537

Dear ImGui
dbb5eeaadffb6a3ba6a60de1290312e5802dba5a
```

### CI cleanup

README-only changes no longer launch a binary rebuild.

The V0.17 artifact name is:

```text
DarksidersGenesis_ASI_V0.17_CORE_CLEANUP_TEST
```

## Source-size result

The old monolithic `DarksidersGenesisMod.cpp` was approximately 4,200 lines
during V0.16A.

After the cleanup:

```text
DarksidersGenesisMod.cpp  2,254 lines
```

Major isolated sources now include:

```text
ConfigStore.cpp
OverlayUi.cpp
HorseFeature.cpp
PlayerIdentity.cpp
RuntimeSettings.cpp
TargetValidator.cpp
dxgi_proxy.cpp
```

The cleanup removed 2,194 lines from the main source relative to `main`, while
preserving history in Git/docs.

## Final CI verification

GitHub Actions run:

```text
37476887049
```

Result:

```text
Configure PASS
Build     PASS
Package   PASS
Upload    PASS
C++ warnings: 0
```

Packaged file hashes:

```text
dxgi.dll
0450AF176C02E8A7A4EE6591E006048BF4738464F56C07687B895BF15D2A69A5

DarksidersGenesisMod.asi
5FF8F49CCEBEAB83CE6D82BF5BA97F7920569248165AFA86926D850C1DE294B9

DarksidersGenesisMod.ini
9B9028F9C72EB29A67421FAF09E663B8800C3D725F62B5286F9A8A17987E793F

README.md
DCFBBD9CBB4466DADCD12500585663F45F1DA6EB26C4BED9AA28B6B897CAE7E8
```

Artifact ZIP digest:

```text
e1ea0b0d7396818e503ef4b056ab0cea90c5811a71053d77ed8f7d9026683f7f
```

## What remains deliberately unresolved

V0.17 does not claim new validation for:

- Horse Speed;
- Horse Sprint Duration;
- Horse Sprint Speed;
- FOV;
- Third Person;
- Skip Logos;
- a stricter final melee classifier.

The next in-game test should be a regression pass of the validated core before
new feature work resumes.

## Promotion rule

Do not merge V0.17 into `main` solely because CI is green.

Required before promotion:

```text
launch exact target
overlay open/close + rebind
HUD toggle
Movement Speed
Action Recovery
Jump
Glide
Pistol Damage
Melee diagnostic
Hotstreak
Skip Intro
horse candidate telemetry
Alt-Tab / resize
clean exit
```
