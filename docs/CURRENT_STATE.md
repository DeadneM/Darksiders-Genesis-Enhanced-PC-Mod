# CURRENT_STATE | Darksiders Genesis Enhanced PC Mod
> **Canonical handoff file for future ChatGPT conversations. Read this FIRST**, then `README.md`, the latest `docs/V0_*.md`, the latest user log and the actual source code. Update this file **after each meaningful build or user test**, never replace validated results with guesses.

**Updated:** 2026-10-12, after user test of V0.99 and report of mouse/radial regression.  
**Repository:** https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod  
**Working branch:** `debug` ONLY. **`main` remains protected/unchanged**; old cumulative binaries in `archive`. **Do not force-push or touch main.**  
**Current compiled experiment:** `V0.99`, CI Windows x64 success https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38282878717  
**Current V0.99 ZIP:** https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V0.99_TPS_MOUSE_CINEMATIC_NATIVE_AUDIT_TEST.zip  
**Previous V0.98 ZIP archived:** https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/archive/downloads/DarksidersGenesis_V0.98_RESTRICTED_HORSE_DEFAULT_WHEEL_CLEANUP_TEST.zip  
**V0.97 user-validated horse+equipment-wheel reference ZIP archived:** https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/archive/downloads/DarksidersGenesis_V0.97_HORSE_AREAS_RAW_MOUSE_CUTSCENE_WHEELS_TEST.zip  
**Exact target executable:** `DarksidersGenesis-Win64-Shipping.exe`, 62,113,280 bytes, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Do not assume updated/different game executable works.

## The hard rules: NEVER regress these

1. User explicitly tested **Horse Everywhere working perfectly** in V0.97. Four-way overlay: Off / In all area / Restricted only / Banned zone only. **Requested fresh-install default is Restricted only, `HorseAreaMode=2`**, implemented V0.98 in shipped INI, `ConfigStore` fallbacks and `RuntimeSettings` initializers. Existing user's explicitly saved value MUST be preserved. Horse native getter/lease logic unchanged unless separately and explicitly requested.
2. **Controller equipment wheel/right-stick selection worked again in user-tested V0.97.** The V0.97 physically-held R3 + LB native right-stick passthrough and native look getter wheel exemptions are essential. **NEVER rewrite or simplify XInput/R3/LB/horse chord/controller aim as part of a mouse fix.** V0.98 accepted the temporary *native aerial camera in radial wheels* as a cosmetic issue to defer; do not spend changes on that cosmetic issue while radial selection is broken.
3. **Shared keyboard E** triggers BOTH native equipment selection and the configurable temporary vanilla camera view; validated. Shared Z/Z short recenter/long TPS-toggle defaults work. Original R3 grenade/stow pulse, H + LB/RB horse, speed, sprint/stamina, independent gamepad TPS strafe and aiming stay untouched.
4. V0.90 exact two Strife cyan aiming-indicator shader suppressions validated. Preserve shader identification, Hide Reticle (independent toggle), Hide HUD, native overlay, skip logos/intro/warning, GPU adapter Engine.ini, and every previously validated unrelated gameplay feature.
5. **Horse summon HUD icon** is a vanilla GAME OPTIONS toggle. No horse-icon-specific hide hook ever existed in active code. Do not create one.
6. Existing ASI/user INI profile defaults (other than HorseAreaMode) must not be reset. The latest config revision was `2132`. The mod's log must be per session, NOT cumulative.
7. Every candidate: GitHub source on `debug`, cumulative README/history and this file, native Windows x64 CI build verified success, **exactly four files at ZIP root** (`dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`), stable raw GitHub URL, archive the previous cumulative ZIP, and do not claim a feature is fixed until the user tests it. After each build report conversation margin large/medium/low and warn if a new chat is prudent.

## Latest direct user feedback (2026-10-12, after V0.99)

- **TPS physical mouse camera is terrible**; user requests a **from-scratch rewrite of the MOUSE ONLY path** rather than repeated band-aids, without changing *anything* in the controller path. If a safe separation is not yet clear, do a full source audit BEFORE implementation. Do not keep stacking mouse hacks.
- **Accessory radial wheel no longer selects using right stick** in V0.99. The user previously reported equipment wheel working again with V0.97. **Do not conflate accessory wheel with equipment wheel**, or assume the two have the same opener/input route. Need identify actual native activation and right-stick route.
- Request specifically to create/update `docs/CURRENT_STATE.md` and a reusable next-chat continuation prompt.

## V0.99 uploaded test log: actual observations vs hypotheses

User-supplied log: `DarksidersGenesisMod(20261012-172631).log` (this chat's uploaded attachment, NOT automatically stored in GitHub). Its source reports build `0.99-tps-mouse-raw-audit-cinematic-shape-gating-test` loaded on the exact exe and native camera hooks installed.

**MOUSE:**
- `TPS V0.99 RAW INPUT` reports only the bounded first RAW observation (example dx=-1, dy=0) then `TPS V0.99 FALLBACK INPUT` starts because no recent RAW input is seen. During physical mouse fire, `FIRE ORBIT` reports RAW ages **1,422 ms** and **3,782 ms**, indicating the fallback can dominate. Not enough data to infer absolute RAW packet counts because the provided session ends before usual mouse shutdown summary.
- Native game character look vector and modified TPS camera yaw are grossly divergent: **167.3°, 101.7°, 134.2°** (three sampled `MOUSE AIM AUDIT` events). This is the first confirmed architecture-level reason why camera direction and native targeting can disagree. **It does NOT prove the exact projectile raycast yaw/pitch; NO native projectile hook exists.**
- Existing C++ `src/DarksidersGenesisMod.cpp`: `OverlayWndProc` reads `WM_INPUT` with `GetRawInputData` but there is no explicit `RegisterRawInputDevices` found by full-text audit in the mod source, so RAW event delivery might depend on the game's own registration / window focus. The fallback reads `WM_MOUSEMOVE` client coordinates and adds deltas to mod orbit after RAW has been absent 1.5 seconds. The game's native mouse input is passed through. `HookPlayerLookInput` and `HookPlayerLookRotation` still contain combined/controller legacy branches; for foot LMB with no pad firing they return native as of V0.97. `UpdateOrbitInput` combines RAW-or-fallback mouse orbit with independent pad orbit, native mounted fire freeze and recoil/aim behavior. No repeated cursor warps remain.
- **Next phase MUST isolate separate `TPSMouseInput` module (or equivalent strict mouse-only function)** before changing behavior: game window WndProc/RAW registration audit; native event delivery/focus/overlay; track physical mouse source vs Steam Input/XInput synthetic messages; model raw relative orbit and native cursor targeting separately; avoid phantom yaw writes and any native right-stick path. Add explicit read-only source diagnostics first, then build a rollback-friendly mouse-only experimental candidate. No claims of proper forward projectile firing until engine's actual targeting ray is located and verified.

**ACCESSORY RADIAL:**
- User reports broken right-stick selection in V0.99. Provided short V0.99 log has **no `TPS V0.97 WHEEL: R3 physical`** event, so it does not identify which button/opener the accessory wheel used in that test. Do NOT assert the wheel was R3 or LB.
- Confirmed GitHub diff V0.97 -> V0.99: **V0.98 removed** `radialGameplay` continuity from `src/CameraTraceFeature.cpp`, which had preserved a recently-live TPS view during a wheel; V0.99 changed cinematic classification and added read-only mouse telemetry. V0.97 physically-held R3/LB `FilterNativeXInput` passthrough remained unchanged. This CAMERA change is a *regression candidate*, **not yet a proven cause**. Compare `src/CameraTraceFeature.cpp` at Git commit `a3490e1a556f88b36088a924cc7ed8336d0569d5` to `debug`, trace how it affects `tpsViewGameplay`, wheel flags and look hooks before reverting.
- Test accessory wheel and equipment wheel **separately** on unchanged controller path; capture the actual opener, physical RX/RY, whether right stick reaches game, whether mouse fallback or cutscene heuristic runs while open. A useful repair may be a minimal *camera classification* restore, not altering XInput mapping. Protect already working native equipment wheel.

**CINEMATICS:**
- V0.97 cutscene detection falsely classified a missing/stale actor at normal native camera pitch -45° as cutscene. V0.99 correctly logged `CAMERA AUDIT: stale actor, native view not cinematic` for such samples; native camera remains game-controlled. User has NOT yet validated actual cutscene start/end, so this feature is **experimental default ON**, not proven.
- Do not patch camera volume memory, assume every failed actor sample means a cinematic, or confuse a radial UI camera with a cinematic.

## Immediate engineering sequence for next chat

1. Verify `debug`, `archive`, `main` and current CI/git commits. Read this file and newest docs/README; inspect attached V0.99 log (ask for attachment only if not accessible, not a repeat of known facts).
2. **Freeze horse / gamepad / gear and shader.** Keep V0.97 and V0.98 rollback ZIPs available. Do not change gamepad code even while repairing accessory wheel; start with original camera-view classification and **read-only** radial controller trace.
3. **Full mouse audit before editing:** enumerate every `WM_INPUT`, `WM_MOUSEMOVE`, camera orbit, native look getter and physical mouse-fire path; why RAW packets appear stale; discover if game registers RAW mouse itself; trace why aiming yaw deviates 100–167° from drawn TPS camera. Decide and document clean independent mouse architecture; implement only when truly decoupled from controller. Do not merely center OS cursor / inject synthetic mouse coordinates.
4. Fix accessory wheel regression with a **minimal, isolated, testable change** after comparing V0.97 camera source; **no changes to `FilterNativeXInput`/R3 gamepad behavior**. If relation uncertain, deliver instrumentation first rather than speculative wheel fix.
5. Build next numbered cumulative test candidate only after above, verify CI and four root files, README + `CURRENT_STATE.md` same commit lineage, give real raw GitHub ZIP link. Ask user to test just accessory vs equipment selection and mouse before/while shooting. Update validation status from actual user results.

## Relevant source and engineering documents

- `src/DarksidersGenesisMod.cpp`: `OverlayWndProc`, `FilterNativeXInput`, `UpdateOrbitInput`, `HookPlayerLookInput`, `HookPlayerLookRotation`, and input/overlay hooks. **Controller hooks frozen.**
- `src/CameraTraceFeature.cpp`: `HookView`, `gameplayCamera`, `radialGameplay` history, TPS pitch/actor validity, cutscene heuristic.
- `src/HorseFeature.cpp`: VERIFIED native horse-region logic. Do not touch.
- `src/ConfigStore.{h,cpp}`, `src/RuntimeSettings.{h,cpp}`, `config/DarksidersGenesisMod.ini`: preserve default `HorseAreaMode=2`.
- `src/ShaderAimFeature.cpp`: verified exact two-shader aim suppression, do not modify.
- `docs/V0_97_HORSE_AREA_RAW_MOUSE_CINEMATICS_RADIAL.md`, `docs/V0_98_RESTRICTED_HORSE_WHEEL_CLEANUP.md`, `docs/V0_99_TPS_MOUSE_CINEMATIC_NATIVE_AUDIT.md`: supporting detailed technical history.
- `README.md` has historical notes from very old versions that may contradict current details in its early sections. **This file and the newest dated README changelog sections take precedence for current development status; don't mistake older archival prose for latest instructions.**
