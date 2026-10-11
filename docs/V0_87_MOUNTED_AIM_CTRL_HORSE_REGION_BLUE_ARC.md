# V0.87 | Mounted aim control separation, Ctrl comparison, experimental horse-region gates and blue indicator candidates

Date: 2026-10-11. Experimental branch from V0.86. Keep main and stable V0.84 / V0.86 ZIPs unchanged.

## V0.86 log + user's direct feedback
From `DarksidersGenesisMod(20261011-164014).log`:
- Exact 62,113,280-byte executable accepted, native XInput/look/camera/horse hooks installed.
- `CAMERA_FORWARD` was selected many times during mounted pad firing, yet shots did not necessarily follow camera as requested. Mode activation log is **not** proof the in-game ballistics aligned.
- Mounted `VANILLA_AIM` was visually validated by the user **WHILE FIRING**: TPS camera stops, native stick can aim around. Regression OUTSIDE firing: stick also controls native aim alongside TPS camera. This must be fixed without changing its validated firing behavior.
- `Horse Everywhere V0.86 summon INPUT` and `SERVER_SpawnHorseMount` confirm a successful LB+RB normal request; however `HorseEverywhere=0` during that session and there was **no forbidden-zone summon validation**.
- User reports **keyboard/mouse TPS camera can get stuck at certain angles / cannot orbit full 360 degrees**. Explicitly deferred until main gameplay is coherent. No mouse orbit/clamp/360 change in V0.87.

## V0.87 mounted aiming fixes
1. **VANILLA_AIM mode 1:** game-facing native right stick is now gated while mounted and *not firing*, even though the TPS camera receives raw unfiltered XInput. Game-native look getters also block only the alternate right-stick path when idle. On actual firing, native look remains unmodified just as the user validated in V0.86 and independent camera orbit is suspended. Release returns camera-only look. Other on-foot settings do not override mounted behavior.
2. **CAMERA_FORWARD mode 0:** from V0.86 the camera already orbits freely and native GetPlayerLookInput / GetPlayerLookRotation write TPS-facing vectors during RT/RB or mouse fire. The V0.86 call identity filter was restricted to the last ON-FOOT local player. New identity check accepts either the validated live player **or the current fresh HorseCharacter identity if the actual TPS camera source is HORSE**; no stale object is read or written. Log `TPS V0.87 CAMERA_FORWARD` includes count, projected camera yaw, actor kind, and state of physical Ctrl. This tests whether a native camera-forward look path is really executed on the mount. The user cited the keyboard **Ctrl** native aiming mode: V0.87 passively samples Ctrl presses and compares native look dispatch; it does NOT blindly synthesize Ctrl or modify held keys or quest abilities. The exact Ctrl input action and trajectory ownership remain to be proven from game's input assets.
3. **Known limitation**: a native look-vector adjustment may not redirect a mounted projectile if the game's mount shooting path bypasses those getters. In that case use the V0.87 callback counts to identify the missing native aiming ability; do not claim a finished fix.

## Horse Everywhere region-gate experiment
Exact x64 EXE binary and reflected UE functions were audited with objdump:
- `AMayhemPlayerCharacter::IsInHorseDesummonRegion` UFunction wrapper RVA `0x7AD730`, native `0x675D20`.
- `AMayhemPlayerCharacter::IsInHorseRegion` UFunction wrapper RVA `0x7AD760`, native `0x675E40`.
- Native methods inspect the root/overlapping regions; they are separate from the validated `HorseCharacter::GetNormalizedSpeed` / `GetNormalizedSpeedInput` used to recognize an active mount, and upstream from `SERVER_SpawnHorseMount`.
- Signature-guarded MinHook wrappers call BOTH original functions unchanged by default. **Only with HorseEverywhere ON, exact live-local-player identity published by the validated movement hook, and physical H or LB+RB pressed within 1.6s**, the experiment can answer `IsInHorseRegion=true` even if native returned false and `IsInHorseDesummonRegion=false` even if native returned true. Original getters are still called for diagnostics. No server spawn invocation, no actor/property patch and no global region modification. It is not verified this is the real restriction; game may use another Blueprint gate.
- Counters for calls and conditional overrides are visible in Horse overlay and log: `Horse Everywhere V0.87 native region totals`. If count=0 on forbidden summon, the gate isn't used and the probe is inconclusive. If there is any unintended quest/zone behavior, set Horse Everywhere OFF immediately (defaults OFF) or roll back to V0.86.
- No changes whatsoever to horse speed, acceleration, sprint/stamina or horse actor ownership.

## New cyan line / semicircle candidate asset audit
The global HUD `ui.HideHud` has no effect on the line/arc. `HideGroundTargetingEffect` native hook was called **0** times in V0.86 log; old `ShowBeamEffect` / `HideBeamEffect` tests did NOT hide the line.
Exact PE reflected identifiers found in shipped EXE:
- `m_trajectoryBeamEffects` string offset `0x27EE060`, with `AimTrajectoryBeamEffectProperties` `0x27EE180`, `ValidTargetEffect` `0x27EE210`, `InvalidTargetEffect` `0x27EE1F8`, `TargetingModeAimSpeed` `0x27EE118`. These belong to a targeting-ability property cluster and could render a trajectory, not necessarily Strife's normal cyan line.
- `LockOnIndicatorClass` `0x27EAE00`, `bShowLockOnTargets` `0x27EAE58`, `TargetingIndicatorClass` `0x27EAE70`, `bShowCurrentTarget` `0x27EAE88`. These are another distinct targeting indicator cluster, possibly enemy lock-on indicators rather than the cyan firing trail.
- `OnShowTargetingUIDelegate__DelegateSignature` `0x27DB460` and `bShowUI` `0x27DB458` are additional UI presentation clues.
None is confirmed as the two requested objects. Avoid indiscriminate ProcessEvent-blocking (V0.82 was not user-validated), property writes or shared shader suppression until a specific instance / draw call is observed. HUD `AimIndicatorHideMode` selector remains saved (default TPS Only); visual disappearance **still unverified**. Next narrow step: check cooked PAK asset file listing or RenderDoc draw for `m_trajectoryBeamEffects` materials.

## Freeze and test
Protected V0.84 R3 continuous hold + 100ms gap + 200ms stow pulse, all gamepad buttons/OFF V0.86, all on-foot strafe/rotation camera settings, native springarm, HUD, boot, damage, horse movement. Only mounted look eligibility/gating, read-only Ctrl probe, narrow horse region trial, UI clarity, docs and test workflow changed.
Testing: While mounted, mode 1 no fire = camera only, fire = native aim, release = camera only. Mode 0 fire = camera continues and shots should follow TPS camera direction; inspect `TPS V0.87 CAMERA_FORWARD` log if not. Compare Ctrl on a horse to identify true native aim ability. For horse: try H/LB+RB in forbidden and allowed zones with experimental feature ON (and OFF comparison); share region counters. HUD line and semicircle may remain visible. Mouse 360 bug deliberately unfixed.
