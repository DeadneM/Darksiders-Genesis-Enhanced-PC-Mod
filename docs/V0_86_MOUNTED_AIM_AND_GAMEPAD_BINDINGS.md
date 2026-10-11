# V0.86 | Mounted TPS shooting modes, all XInput bindings, summon input audit

Date: 2026-10-11. Based on V0.85 with the **V0.84 grenades equip/stow user-validated**, which is the protected cumulative controller baseline.

## Exact user desired mounted TPS shooting
In TPS on a HORSE, regardless of the other ON-FOOT camera/strafe/firing toggles:
- **Mode 0 default: Camera Forward**. Keep full camera orbit control while firing. Feed game-native look heading with the TPS camera direction. Both mouse and controller firing should use camera direction.
- **Mode 1: Vanilla Aim**. Suspend independent TPS camera orbit while shooting. Game receives its native aim input (including the right stick), then TPS camera orbit resumes on release.
This is **not** mounted camera HEIGHT stabilization. The retired V0.85 vertical filter was NOT the desired behavior: user log observed height smoothing correction of only -0.2 units at event n300. It is removed. Horse camera remains attached to live horse poses when fresh.

Implementation: new persisted `[Features] TPSMountedAimMode=0` with 0/1. Runtime `tpsMountedViewActive` and `tpsMountedViewTick` published ONLY on successful exact native HookView attached camera frames, expiring after 300ms. Native look input/rotation hooks respect mounted modes before on-foot switches; source actor identity remains checked. XInput right-stick filter enforces camera-free suppression in mode 0 while shooting, passes native stick on mode 1. Controller TPS orbit is forced ON while shooting mounted in mode 0, OFF in mode 1, independent of on-foot rotate while firing. Mouse orbit similarly stops only in mounted mode 1 while firing. *Native game owner, ability and projectile memory untouched*. If mounted state isn't fresh, all logic reverts to previous on-foot behavior.

## Single-selector controller hotkeys (OFF plus all supported XInput controls)
Replace checkbox + bind combos with 4 simple combos, each with **OFF**:
- R3 grenade hold/release = `[Controls] TPSGrenadeButtonMask=128` (new). A different selected button is mapped to the **native R3** behavior so V0.84's continuous hold, 100ms release gap, 200ms second stow press remains the same.
- Temporary vanilla view = `TPSSuspendButtonMask=128` (300ms default).
- L3 long hold for TPS toggle = `TPSToggleHoldButtonMask=64` (500ms).
- TPS short recenter = `TPSRecenterButtonMask=64`.
All combinations include D-Pad 4, LB/RB, L3/R3, A/B/X/Y, Start/Back, LT/RT as virtual binding IDs `0x10000` and `0x20000`. Xbox GUIDE is not accessible in standard XINPUT_GAMEPAD and is excluded. Triggers are detected via their analogue threshold, rather than fake bitflags. Gameplay non-selected buttons stay native; if the user selects a button that already shoots/opens wheel, mapping overrides its native action and UI warns of conflicts. R3 physical press is hidden from the game's normal grenade input when the user binds a different key and TPS is enabled. `OFF=0` disables the **mod action**, not vanilla game bindings. Conflicts of overlapping selected keys are shown.

ConfigRevision **2126**, retaining every existing mod config: when older settings had `TPSHoldSuspend=0`, `TPSHoldToggle=0` or `TPSGrenadeHold=0`, migrate the corresponding bind to OFF. Existing saved button selection remains. Save maintains backward-compatible enable bools inferred from nonzero masks. No new build changes to mouse speed, fire damage, movement, horse speed/stamina or unrelated HUD. TPS still OFF by default.

## Horse Everywhere
New source information from user: invoke mount via **H keyboard** or **LB+RB (L1+R1) controller**. V0.85 user log contains two actual server spawn calls, both with Horse Everywhere=1, during permitted mounts; this is still NOT a restriction bypass. New passive per-input rising edge trace in `UpdateOrbitInput`: logs actual H / LB+RB presses and server spawn call counter at that moment. Correlate with attempts in allowed/forbidden zone. No key simulation, server args changes, native zone guard bypass or fake GUI claims. User should report resulting log. If denied attempt shows input but no server call, locate Blueprint/player summon permission upstream, not server function.

## V0.85 user log observations
- Native mount + camera recognized, `TPS V0.85 MOUNTED AIM: height stabilization n=300 rawZ=2133.1 stableZ=2132.9 correction=-0.2`. Smoothing was genuinely triggered but minimal and not expected aiming behavior.
- R3 V0.84 validated remains unchanged in timing, now remappable only.
- Initial last line: `Horse Everywhere probe totals: serverSpawn=2 duringRequested=2`, not evidence of area bypass.
- Controller firing and camera look observed, but two mounted aim modes still require in-game test and final code verification.

## Validation
Test TPS mounted shooting mode 0: aim by orbit right stick/mouse while firing, hits follow camera. Mode 1: camera orbit freezes only while firing, right stick aims like vanilla; releasing resumes camera orbit. On-foot controls must not change. Grenade R3 equip/stow must remain exactly as user-approved V0.84. Test grenade alternate button plus OFF; L3 short and long; suspension (R3 300ms); full listed XInput bindings including LT/RT if needed. Press H once in permitted and forbidden areas, also LB+RB where relevant; compare input logs and server calls. If any regression, revert to V0.85 or V0.84. Never assert Horse Everywhere fixed before true native permission bypass is identified.
