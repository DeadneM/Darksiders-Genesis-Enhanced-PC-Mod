# V0.99 | TPS native camera classification and non-invasive mouse aim telemetry

2026-10-12. Cumulative experimental debug branch. `V0.98` cumulative ZIP archived on `archive`; protected stable `main` untouched.

## Validated base is FROZEN

User-validated: Horse Everywhere native region behavior, default Restricted only (`HorseAreaMode=2`), R3 equipment wheel selecting correctly with the native right stick, native LB selection, E shared temporary view and native equipment use, Z/Z shortcuts, horse walking/sprint/stamina, and both V0.90 exact-CRC Strife targeting shader removals. The aerial camera when selecting native wheels is a user-accepted cosmetic bug on a **deferred list**; do not attempt wheel-camera warps or change any right-stick, physical R3, grenade or summon input code.

The V0.97 mouse path was NOT explicitly validated and the native projectile 3D raycast is NOT hooked. No code should claim camera-forward shooting is fixed.

## V0.99 camera detector corrective logic

The preceding user-provided V0.97 log shows repeated `TPS V0.97 CINEMATIC` transitions immediately after native `GetCameraView` rejected ordinary native camera frames with **actorValid=0 and native pitch -45 degrees**. The old detector interpreted lack of fresh actor-position callbacks as positive cinematic evidence, even where the native camera shape looked like gameplay.

Only automatically suspend TPS if *all* are satisfied:
- TPS enabled, user `TPSAutoCutscene=1`, previous genuine TPS camera recently seen.
- Current view is not accepted as gameplay and additionally has an independent cinematic **camera-shape signal**: native pitch outside normal -89..-15 range, **or a currently verified live local player** whose camera is demonstrably far from the character.
- More than 500 ms of continuous evidence, same guard as before.
- No physical LB/R3/native wheel input is currently held (wheels must use vanilla aerial game camera without being reclassified as story cinematics).

Missing actor alone with normal gameplay native pitch resets the candidate timer and *does not trigger auto-suspension*, while the camera frame itself still passes through unchanged. On verified real gameplay frame, clear a prior auto-suspension, preserving camera basis. The option OFF remains the previous no-explicit-auto-suspension behavior; no independent game cutscene flag is available, thus still an intentionally conservative heuristic and not a guarantee for in-place cinematics.

Bounded diagnostic strings: `TPS V0.99 CAMERA AUDIT: stale actor ...` (up to five), `TPS V0.99 CINEMATIC: sustained native camera shift ...`, and `confirmed live gameplay view ...`.

## V0.99 read-only physical mouse audit

`GetPlayerLookInput` already returns unmodified mouse input on foot when physically firing and no controller fire; this remains. Native `GetPlayerLookRotation`, native WM_INPUT delivery, native cursor targeting and native projectile raycast remain unmodified in the mouse path; zero `SetCursorPos` in the mod. Existing camera orbit reads mouse `WM_INPUT` and falls back to `WM_MOUSEMOVE` only when RAW has been absent >= 1.5 seconds.

Add bounded diagnosis to distinguish the three independently suspicious paths:
1. **Native RAW receive and game pass-through**: `TPS V0.99 RAW INPUT: native WM_INPUT PASSTHROUGH ... dx/dy, LMB`. Counters `rawPackets`, `rawMotion`, `rawWhileFire`. This does NOT intercept or consume native game messages.
2. **WM_MOUSEMOVE secondary fallback**: `TPS V0.99 FALLBACK INPUT: no recent RAW input ...`. Count actual fallback mouse movements, including whether they happen during fire. No synthetic cursor position or Win32 sends.
3. **Presentation camera and game-native look during fire**: `TPS V0.99 FIRE ORBIT` (rawAge, pending mouse delta, rendered native/applied camera yaw, gameplay state), `TPS V0.99 MOUSE AIM AUDIT` (native GetPlayerLookInput returned XY, its yaw versus actual camera yaw, view freshness). Both are observational. No change to gamepad behavior, mounted aim, or native weapon targeting.

Shutdown summary records all six counts in the one-session noncumulative log; removes previously unused `g_tpsMouseTargetWarpCount` global that remained after V0.97 warps had been deleted.

**Test:** camera TPS ON on foot, spin mouse without firing 5 seconds, then hold LMB and spin 5 seconds; compare two periods of RAW input, game look getter and camera yaw. Try a genuine in-game cutscene and a screen/loading transition, compare `CINEMATIC` only when confirmed distinct view. Native equipment wheel R3 must keep functioning; Horse Everywhere Restricted must remain functional.

## Packaging and release policy

Next `V0.99` Windows x64 four root files: `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`. CI must pass configure/build/package/verified root artifact/preserve downloadable ZIP. Existing user INI untouched by feature changes; ConfigRevision remains **2132**. Archive V0.98 and keep `main` untouched.
