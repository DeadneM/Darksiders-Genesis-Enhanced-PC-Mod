# V1.00 | V0.98 mouse camera baseline, mouse-only forward look and the remaining Gameplay roadmap
2026-10-12. Experimental V1.00 under `debug`; stable `main` untouched. Last user-provided log is **V0.98** (`DarksidersGenesisMod(20261012-174455).log`), not V0.99. The user explicitly says V0.98 mouse TPS rotation works properly *without firing* and the equipment wheel works in V0.98. V0.99 is superseded as a test candidate, NOT promoted as validated. V0.99 and earlier ZIPs archived.

## Correct control names: NO wheel-opening R3

**R3 = USE / deploy the selected equipment** (with the existing native tap + release pulse, optional temporary vanilla view), NOT open the radial wheel. LB and keyboard E / G / Tab / LAlt are candidates for distinct native equipment, accessory, ammunition and ability wheel openings; the actual accessory wheel opener needs in-game mapping and separate validation. The misleading V0.97 `R3 physical + native right-stick passed` diagnostic records a valid R3 USE action, not proof of gear-wheel selection. For compatibility, the R3 passthrough and wheel-input guard still remain byte-identical with the user-tested V0.98. *Do not modify native gamepad XInput paths as part of this camera/mouse test.*

## Exact source-level V0.98 -> V0.99 camera comparison

V0.99 changed only the non-gameplay/cinematic classification in `src/CameraTraceFeature.cpp` relative to user-working V0.98, in addition to read-only mouse diagnostics. The controller XInput code was unchanged. The exact entire camera file from V0.98 commit `c5fe4d0adf66675d0b44d23b53dcc246c6bd9ed9` was restored (blob SHA `7fb75854a579a3abaa4b94235d9e862cacf20635`). This brings back V0.98 wheel-related camera behavior without changing pad controls. **The V0.98 cinematic detector's known false-positive on stale actors may consequently recur.** This is an intentional, documented comparison rollback.

## New mouse-only experimental forward simulation

The user requested: keep V0.98 camera when no fire and emulate a **stick pointing straight forward while firing with physical mouse**.

- At `GetPlayerLookInput` native detour, ONLY for verified local gameplay actor, TPS ON, foreground, **foot actor (not mounted)**, physical LMB held, **no controller fire**, valid recent TPS gameplay camera and no native wheel:
  - If `TPSMouseForwardFire=1` (default), replace the returned native world X/Y **look vector** with `{cos(TPSYaw), sin(TPSYaw)}`, Z=0. This simulates forward-directed game look input, **NOT actual XInput injection**, and it does not hook the projectile raycast or change the native mouse cursor position.
  - `TPSMouseForwardFire=0` passes the unmodified native result (equivalent to V0.98 mouse firing).
  - No changes to controller XInput, virtual R3/LB, mounted aim, gamepad strafe/rotation, `HookPlayerLookRotation`, mouse camera RAW/WM_MOUSEMOVE handling, mouse orbit speed, or any horse/HUD/shader hooks.
- Bounded `TPS V1.00 MOUSE_FORWARD` applied/bypassed count and yaw logs aid the test. No claim of guaranteed projectile steering.
- The INI also retains `TPSMouseCenterAim` legacy option controlling mouse orbit. New config revision `2133`.

## Three new gameplay gauges requested (NOT implemented until native owner identified)

The user requests three **independent** gameplay modifiers for:
1. **Wrath / Courroux**
2. **Ultimate Form**
3. **Synergy / Synergie**

Design intention is **earned-charge multipliers** comparable to Hotstreak Charge, each with independent ON/OFF and bounded `x1..x25` slider, rather than rewriting visual HUD fill or conflating three meters. Before touching memory, identify the specific original native owner/charge accumulation function per meter and confirm **local player only**. The existing `Hotstreak AddJuice` at game RVA `0x660260` is not evidence that it owns any of these three different gauges. Without a verified executable/unique signature, do **not** wire these features to the existing Hotstreak function or sell UI sliders that do nothing. Obtain a verified native call path and then add one gauge at a time, preserving unrelated features.

## Skip Warnings: why unfinished

The existing early startup proxy `src/dxgi_proxy.cpp::InstallSkipWarningLoopHook` installs an exact-signature trampoline at RVA `0x25FEBF`. It suppresses the list only when the span matches ONE or TWO 0x40-byte definitions. User's V0.98 log proves `span=0xC0` => **three** definition records, `seen=1 skipped=0`. Changing the count condition to include three without inspecting the actual three native data entries would risk removing unrelated legal/loading screens. Next: inspect the game executable, identify which record(s) are warnings, add a fail-open guarded native filter or targeted skip and confirm the intro/logos stay independent. Do not change the 3-entry loop globally based solely on the count.

## Supplied V0.98 log special warning

The user's V0.98 log includes thousands of repeated `Player identity: active player` events alternating rapidly around 19:29:36 onward, triggering hundreds of `camera locked to actor axis` reanchors. This indicates player identity sampling instability under that scenario, independent of the forward-fire experiment. It is **not** proof of mouse path regression; track separately if it recurs with V1.00. Avoid creating a new high-volume log flood.

## Build / test protocol

Use `debug` only, archive the prior ZIP, preserve exact four files at ZIP root. Windows CI success required before linking ZIP. Test V1.00 default ON versus OFF with mouse firing and compare actual projectile direction vs native game look, spin when idle; also test equipment and accessory wheels **separately**. If wheel regression persists despite exact V0.98 camera restore, instrument the physical opening control before any pad code changes. Don't claim Wrath/Ultimate/Synergy or warnings implemented before native audit. Keep `main` untouched.
