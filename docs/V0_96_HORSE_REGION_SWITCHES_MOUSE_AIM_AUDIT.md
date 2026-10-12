# V0.96 | Independent native horse restrictions and TPS mouse firing audit

2026-10-12, experimental development branch `debug`. **V0.95 fully backed up** on `archive`. Stable `main` untouched. No speculative horse spawns, no blanket HUD/shader removal.

## What the uploaded V0.95 log actually establishes

- User confirms Horse Everywhere works well and E shared equipment + temporary view / Z + Z bindings remain working. In the V0.95 log, native region permission false→true: **9,190 times**; native desummon true→false: **1,353 times**; five scoped horse leases, five native server-spawn requests. V0.95 was the first version to pregrant native IsInHorseRegion for the verified local player before the game's horse input.
- There are **two independent native calls**, but the user-described HUD cases *lock icon / no icon* have **NOT** been proven one-to-one with these checks. To map their visuals, isolate each feature by its new UI checkbox and record which HUD case or action changes, without guessing from the icon alone.
- TPS mouse: 58 repeated `SetCursorPos` calls were issued during firing; 784 calls to the *separate* native look-input getter had their vector overwritten by the TPS camera yaw (`source=mouse`). The look-rotation hook applied **zero** `mouseFace` overrides according to summary counters, so the look-vector and cursor warp are stronger observed collision candidates than the rotation hook. The older `CameraTraceFeature.cpp` native-pitch admission heuristic admitted only `-89..-15` degrees before V0.95 radial exceptions. There is no evidence yet that a camera pitch outside that interval actually occurred on a failing shot; V0.96 adds a bounded on-foot mouse-firing continuity guard to test this hypothesis.
- **No native projectile raycast hook** has ever been installed. A perfectly aligned 3D TPS cursor / projectile trajectory cannot be claimed merely from changing the 2D mouse look direction.

## Two independently configurable horse gates

The master `[Features] HorseEverywhere=1` remains. Add:
- `HorseAllowRestrictedSummon=1`: allow native `AMayhemPlayerCharacter::IsInHorseRegion` false→true **for the existing validated local actor** in exactly the same scope as V0.95. Region pregrant remains active; this is the admission/eligibility gate.
- `HorsePreventForcedDesummon=1`: suppress native `AMayhemPlayerCharacter::IsInHorseDesummonRegion` true→false **only in the existing V0.91 summon/mounted/horse-lease scopes**; NOT a blanket native desummon for arbitrary players.
- Both ON: mathematically identical override decisions to V0.95. One OFF: ONLY that associated native getter's original return is respected. Both OFF or master OFF: vanilla region answers. Horse speed, sprint, stamina, actual native mount lifetime, HUD, aim shaders unchanged.
- Overlay Gameplay > Horse now shows both subordinate checkboxes under master; labels use the **function names**, not an unproven 1:1 mapping to the two prompt appearances.
- Test every combination in areas with lock prompt and areas with no prompt. **This option separates two native restrictions, but does not yet identify a particular UI/icon resource.**

## Isolating TPS mouse during firing (V0.96 on-foot experimental A/B)

With existing `[Features] TPSMouseCenterAim=1` (user-supplied default), and ONLY on foot in Camera Forward:
1. Physical mouse left-button firing through the local validated player getter no longer overwrites `GetPlayerLookInput`'s native `X/Y` look vector with `cos(cameraYaw),sin(cameraYaw)`; that previous V0.75 path remains for **controller** and remains available for mouse when the new experiment is OFF.
2. `GetPlayerLookRotation` remains completely native for this same mouse path instead of potentially adding a second TPS yaw. The user log showed 0 effective overrides through this function, so this is preventative path isolation rather than a proven root cause.
3. Mouse targeting recenters the native OS cursor **once at a new physical left-click down**, not every Present frame while the trigger is held. Physical RAW mouse deltas still orbit the mod camera. Synthetic WM_MOUSEMOVE corresponding to this one warp is still ignored in the mod's own orbit calculations.
4. Preserve the *already accepted* local TPS camera view if native mouse firing momentarily violates the legacy `nativePitch <= -15` camera admission gate: only with local live actor, close native view, recent (650ms) already accepted TPS view and physically held LMB. Menu, cutscene, unrelated cameras and mounts are excluded. V0.95 radial continuity remains.
5. A/B test via **Camera > TPS > Mouse TPS: native mouse aiming during fire (test)**. Enabled new native mouse path; Disabled legacy yaw injection and no cursor warping. This is a diagnosis and optimization, not verified projectile raycast alignment.

All mounted Camera Forward and Vanilla Aim, independent controller right-stick camera, E gear + temporary vanilla view, radial right-stick navigation and the two verified shader hashes remain untouched.

## Exact ASI default values from user-provided DarksidersGenesisMod(5).ini

Source of truth: V0.95 user-tested attached INI, ConfigRevision 2129. The fresh V0.96 `config/DarksidersGenesisMod.ini` file and C++ `Store::ResetDefaults`, `Store::Load` missing-key fallback, `ConfigStore.h` and `RuntimeSettings.h` initializers now agree on:
- TPS OFF on startup (`ThirdPerson=0`), TPS FOV 110, global FOV feature OFF, native mouse-centering test ON, all intro/hud/combat/horse toggles as supplied.
- Camera global height **50**, attached TPS distance **0.150**, TPS pitch **-12**, attached TPS height **0**, foot anchor **0**.
- Camera mouse sensitivity **0.120**, stick speed **180**, movement 1.5, recovery 0 delay/2 multiplier, pistol and melee 2, jump 1.25, glide 10, horse walk and sprint 1.25, horse stamina 5, hotstreak 2.
- Keyboard H horse, shared Z/Z short/long TPS, E native gear + temporary view, controller LB+RB horse, L3 short/long TPS and R3 grenade/temporary view. Menu Insert, graphics adapter 0, existing F1-F5 actions and camera hotkeys.
- **New** sub-option defaults `HorseAllowRestrictedSummon=1`, `HorsePreventForcedDesummon=1`. This adds two keys without changing any other uploaded user-set value.
- ConfigRevision **2130**: existing customized INIs preserve values; two missing options default ON. Old shared V/V pair migration to Z/Z from 2129 remains. Fresh install uses these shipped and built-in defaults. A malformed floating-point default recovery for TPS distance/stick speed also aligns with the selected defaults.

## Verification and open items

Build CI Windows x64 with root `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`, verified exactly four files. Stable main not changed; V0.95 backed up in archive.

Not yet solved: exact HUD horse summon/lock widget remains unidentified; mouse projectile 3D raycast angle and vertical aiming are still not native-hooked; native X/Y mouse targeting experiment needs user test before promoting.
