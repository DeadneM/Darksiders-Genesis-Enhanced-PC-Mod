# V0.73 native TPS strafe: rotation authority first

Status: **RESEARCH / DESIGN ONLY** (10 October 2026). **Not built. Not tested. Not validated.**

## User mandate

Next candidate must deliver REAL lateral/backward strafe for controller, without recycling the rejected V0.61/V0.66/V0.71 `bOrientRotationToMovement`-only modification. Preserve independent mouse/keyboard and controller configuration. Stop producing superficial variants of the same flag adjustment. Keep stable functionality and original game behavior outside TPS.

## Verified baseline and failings

* Base: main V0.72 TEST (commit `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`), itself NOT officially validated.
* V0.71 native facing flag-only test was ineffective: 1607 active callback ticks, only one pad lock. Restore-only rollback made in V0.72.
* With left stick idle, right stick changed actor yaw (88.9°, 130.3°, 73.2°), even though filtered XInput was read 811 times (143 during firing). Native yaw has ANOTHER authority/path, not merely a missed XInput filter.
* V0.59 synthetic left-stick or right-stick remapping produced axis drift (LEFT UP led to moving RIGHT). Rejected.
* V0.62 simulated held-aim/Ctrl-style permanent aim and restricted mobility. Rejected.
* Only verified world pose read for current exact retail executable: live local player root world translation `Actor+0x158 -> root+0x1A0..1A8`, yaw `root+0x1F4`, sampled within validated GetMaxSpeed callback. This is observational, not a safe write target.
* Other outstanding native 3D aiming and Skip Warning must stay separate.

## Grounded engine finding, not yet proven for this game's custom code

UE4 `UCharacterMovementComponent::PhysicsRotation` selects between at least these two paths:

1. `bOrientRotationToMovement`: rotate toward acceleration.
2. `bUseControllerDesiredRotation`: rotate toward controller desired rotation.

`APawn::bUseControllerRotationYaw` is an additional rotation authority, potentially overriding movement flags. Thus touching only `bOrientRotationToMovement` cannot establish actual game-specific rotation ownership. A custom Mayhem / Strife pawn, player controller, aiming ability or animation path may also override yaw.

Engine references:
* https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/CharacterMovementComponent?application_version=5.1
* https://gist.github.com/NoirMorilec/1ebe1a6f742c193101265a928b90c897
* https://forums.unrealengine.com/t/third-person-c-movementinput-problem/13175

## Correct architecture to investigate

1. From the **exact target EXE** (size 62,113,280, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`) find actual rotation writers and call stacks: UE `PhysicsRotation`, `ACharacter::FaceRotation`, `APawn::FaceRotation`, controller `UpdateRotation` / `SetControlRotation`, and Mayhem/Strife-specific aiming/yaw logic. Do not assume generic UE engine order wins.
2. Read-only ownership investigation on VALID LIVE pawn: sample controlled input, controller desired yaw, movement-component rotation mode bits, root yaw and TPS final camera yaw before/after native rotation, including idle-right-stick versus move-left-stick with RT/RB off and on. No UObject pointer writes from Present, Win32 window procedure or stale cache.
3. Select the **one observed authoritative game-native yaw/rotation callback** for an opt-in detour. Lock desired player facing to final rendered TPS camera heading, or preserve a chosen view-relative heading, via native engine rotation API at correct frame timing. No raw writes to cached root transforms, no fake permanent firing / aim state, no synthetic stick movement.
4. Preserve left analog input and WASD semantics as input, but verify that game-native movement vectors use the same world reference frame as TPS control heading. If not, correct the native movement-reference transformation at the actual input-to-world-vector site, rather than recycling V0.59 left-stick remapping.
5. Keep independent `TPSMouseKeyboardStrafe` and `TPSControllerStrafe` options. They remain OFF and disabled in existing V0.72 until the new authority and movement reference are proven. Protect local player #1, co-op, mount, LB wheel, menus, Alt-Tab, attacks, dodges, cutscenes, TPS OFF; restore game-native behavior through verified live callbacks.
6. Remove speculative V0.72 WM_MOUSEMOVE suppression *only if* evidence shows it is counterproductive. Never globally block physical mouse RAWINPUT or break vanilla aiming.

## Acceptance checklist for V0.73 candidate

* Left stick UP moves forward, DOWN backward, LEFT/RIGHT produces true lateral strafe; Strife still faces TPS camera forward throughout. Repeat WASD separately.
* Right stick orbits camera without steering character through hidden native rotation path, idle and RT/RB firing.
* Reticle/aiming system remains honest: do not declare projectile camera-forward fixed until verified.
* L3 recenter, LB radial wheel, dodge, action recovery, running, mount and dismount, gamepad #2, mouse aim, hotkeys, overlay, camera FOV/height/distance, zone transitions, HUD, reticle, intro/logos, Engine.ini adapter unchanged.
* TPS OFF, lost focus, menu and cutscene return to native behavior with no stale pointer write and no permanent rotation state.
* Exact retail EXE guard, measured diagnostics and confirmed Windows x64 build; four root ZIP files (`dxgi.dll`, `DarksidersGenesisMod.asi`, INI, README).
* A build is **EXPERIMENTAL** until user tests in game; only the user can label it VALIDATED.

## Execution blocker

The current source repository provides code, INI and historical logs, but NOT the target Windows executable bytes for a safe proof of concrete game-native rotation hook and vtable/RVA. Public UE4 source alone is not enough to justify making a machine-code patch for this exact game. Therefore **no V0.73 build or main promotion can be honestly claimed from this document**.
