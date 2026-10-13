# Current test: V1.14U TPS cursor UI and About reset confirmation fix

Fix based on uploaded 2026-10-13 V1.14U log: no factory-reset event logged; ImGui popup opened in About tab but modal attempted outside EndTabBar (ID scope mismatch). Moved popup into About; added visible success/error/cancel feedback + action log. Show Mouse Pointer in TPS moved to Camera > Third Person, OFF by default. Existing integrated updater remains. New cumulative debug ZIP DarksidersGenesis_V1.14_ZZ_TPS_RESET_FEEDBACK_TEST.zip; Windows CI run 38434291793 success; 747066-byte cumulative ZIP published on debug. In-game popup/INI reset test still pending. See [note](V1_14U_RESET_MODAL_TPS_UI.md).

---
# Current test: V1.14U integrated ASI updater (2026-10-13)

V1.14U adds Updates tab to ASI. Beta OFF=main stable (currently V0.72), ON=debug beta. GitHub check asynchronous, user-confirmed install on game exit and rollback. Post-exit WinForms .NET helper compiled and embedded as RCDATA into ASI; retains normal four file ZIP. Previous V1.14 pointer/Info-reset features unchanged. Build run [38430137501](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38430137501) SUCCESS, ZIP [V1.14U](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.14_Z_ASI_UPDATER_TEST.zip) 745505 bytes. Real game install/restore not yet tested; treat as experimental. Technical note: [V1_14U](V1_14U_INMENU_UPDATER.md).

---
# Current active test: V1.14 cursor/reset UI (2026-10-13)

V1.13 user log audited (supported EXE, TPS=1, Synergy 122/60; Wrath/Chaos Add=0). V1.14 on debug adds `ShowTPSMousePointer=0` opt-in control and consolidates the two duplicate INI full reset buttons into the final About tab, preserving confirmation and backup. UI V1.12 user-validated; Windows x64 CI run 38427995840 succeeded; verified four-file ZIP published at downloads/DarksidersGenesis_V1.14_TPS_POINTER_INFO_RESET_TEST.zip; V1.14 still needs gameplay testing. Four-root-file ZIP, main/archive untouched. Read [V1.14 details](V1_14_TPS_POINTER_INFO_RESET.md).

---
# Standalone Updater V1.0 verified CI (2026-10-13)

Windows x64 program built and published on debug, without changes to main/archive or four-root-file mod packages. Checkbox Beta OFF=main (V0.72 currently), ON=debug (V1.13); one-time manual permission when unknown mod already installed, prevent automatic downgrade; preserve existing INI, backup binaries, rollback. Verified workflow run 38423882113; real gameplay installation test still pending. See [UPDATER_V1_0.md](UPDATER_V1_0.md).

---
# Current state - V1.13 safe factory INI reset (2026-10-13)

V1.12 grouped overlay validated by user. V1.13 experiment adds confirmed Reset INI action with unique timestamped .bak of previous file and fresh defaults; no change to gameplay hooks. Docs: [V1.13](V1_13_SAFE_INI_FACTORY_RESET.md). CI Windows x64 success (run 38421589950); ZIP 641322 bytes published on debug; in-game reset behavior validation pending. Main/archive untouched.

Latest V1.12 user log: supported EXE, Synergy 169 observed / 37 boosted; Wrath/Chaos Add hooks 0; Wrath candidate 6 rises / 1 fall; TPS forward injections foot 339 / horse 96. Other investigations remain on TODO.

---
# Current authoritative state - V1.12 (2026-10-13)
# V1.12 - Resource status and responsive controls - 2026-10-13

## Base and goal
Cumulative debug base: 0a13e93 (V1.11 compiled TEST), retaining previously validated functions. V1.11 as a whole has not been user validated. No main/archive writes. Archive remains at 10e0f220; main at 2bc3122c.
Goal: readable common resource controls and truthful functional status.
Acceptance: Windows x64 compilation, exactly four ZIP root files, no changes to native hooks/configuration, then user checks compact overlay and gameplay.

## Changes and preservation
Common tuner status now occupies a separate line instead of fixed x=520.
Below 560px available width, the multiplier slider and exact-value/default controls occupy separate rows.
Wrath and Ultimate explicitly say no confirmed gameplay multiplier effect. Hook installed does not mean gain path works. Synergy retains validated status only when its hook is available.
Build identity corrected: V1.11 still logged V1.10. New identity is 1.12-resource-status-responsive-ui-test.
INI, shortcuts, native LB+Y, RT/RB injection, R3, horse, cameras, HUD, Hotstreak and Synergy hooks unchanged. Existing light debug and startup timeline preserved.

## Native audit and limits
Wrath player+0xEA0 -> component+0x3A0 remains read-only candidate: historical V1.09 14 positive / 9 negative changes; AddWrath and AddChaos zero observed callbacks.
No executable or fresh game log available in this workspace to establish the true gain authority. Do not multiply sampled deltas: spend/refill/maximum changes and local ownership must first be distinguished.
Boot order reported by user: THQ -> Airship -> save warning -> controller warning -> intro.
StartupScreens flags 1/1/0 do not identify the drawing callback. SkipWarning remains OFF; native initialization preserved. Presentation/duration suppression remains unresolved.

## Build / test / rollback
GitHub Windows CI builds the cumulative ASI and DXGI loader with pinned dependencies, then checks the four root entries:
dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md.
Status before CI: EXPERIMENTAL, not yet CONSTRUIT. CI result and ZIP integrity must be checked separately.
Test: Insert, reduce window width, enable the three charge controls, enter x1/x2 then Default; verify key tables, Escape capture cancellation, INI save/reload. Compare Synergy gains at x1/x2; Wrath/Ultimate remain unverified. Check horse, RT/RB, LB+Y, LB wheel, R3 and cold boot/cursor.
Rollback: reinstall preserved V1.11 ZIP with backup of personal INI.

## Cumulative TODO / next step
- User validation of V1.12 layout and non-regression.
- Obtain exact supported EXE and fresh gain/spend log; trace actual Wrath/Chaos writes rather than repeat inactive Add hooks.
- Identify warning presentation/duration authority; preserve initialization before implementing skip.
- TPS mouse remains deferred.
--- Historical cumulative state ---
# CURRENT_STATE | Darksiders Genesis Enhanced PC Mod

## 2026-10-13 V1.11: menu usability, light logging, native warning order

User explicitly requests removing unnecessary debuggers, keeping compact descriptions and useful status; grouping related gameplay/camera options; making keyboard bindings readable; presenting Wrath/Ultimate/Synergy like the other charge controls. V1.11 replaces the separate 3-gauge row layout with the same common tuner as Hotstreak (enabled check, x0..100 slider, manual, Default x2, saved INI) and labels Wrath/Ultimate Experimental, Synergy Validated. Adds collapsible gameplay families and TPS vs vanilla camera groups, keyboard table rows Action/Current key/Change/Clear/Default for horse/TPS/original camera. Removes multiple verbose overlay diagnostic trees, adjusts horizontal spacing/window width. Keeps readable descriptions and small operational summaries. Extensive output logs reduced: shot-phase callsite verbose audit removed, charge candidate changes kept, less combat/reticle spam, old static native caller scan retired, startup records raw word dumps removed but timing/flags kept. No behavior-changing hooks introduced; LB+Y native preserved, R1/R2/Horse/R3/Synergy unchanged; TPS mouse remains postponed.

User's direct visual boot observation is definitive for actual order (not inferred from hook): 2 logos -> black save-data notice -> black controller notice -> intro cinematic. Native +0x39 flags remain 1/1/0, but native record-to-warning matching and draw-time callback still unproven. SkipWarning OFF until display-only suppression can be implemented; Skip Logos/Intro independent. Wrath candidate player+0xEA0 -> component+0x3A0 previously varied positive/negative; native AddWrath/AddChaos 0 runtime call, so gauges NOT yet functionally fixed. docs/V1_11_OVERLAY_USABILITY_LOG_CLEANUP.md.


## 2026-10-13 V1.10 native LB+Y, observed startup phase timeline

User explicitly accepts retiring modded charged LB+Y camera-forward correction. V1.09 user test shows charged ability trajectory appears based on movement/character facing rather than TPS camera. V1.09 LOOK PHASE normal R1/R2 pre-edit native angle 114.7 matches TPS camera 114.7 gap 0; LB+Y pre-edit native look 0,0 while camera yaw -77.7. Thus V1.10 restores game-original LB+Y, no synthetic game-facing right-stick, no Y rebinding, no charged-only GetPlayerLookInput/Rotation override and no LB+Y treated as ordinary fire. LB native wheel active even with Y held; R3 USE, RT/RB synthetic stick UP and other working features preserved. Retired TPSChargedFirePadMask key ignored by code (existing INIs unchanged) and option hidden from overlay. Passive pad chord diagnostic remains.

Skip Warning remains disabled. Timeline instrumented *read-only* with shared DXGI proxy epoch, observed per-stage timestamps/threads from early DLL attach, ASI LoadLibrary, ASI init, EXE validation, intro boot CVar captured and applied, native StartupScreens 3-record array after preparation and active flags, logo descriptor copy and substitution, mod-ready and first D3D11 Present. The stage "2 active native warning definition records" does **not** prove two actual rendered warnings. The native startup loop is preserved to avoid the old cross-reticle regression. Current known movie descriptors THQ_LogoBasic and AS_LogoBasic and native Skip Intro CVar 1->0. User requests explanation of events between logos and intro; a cold-launch V1.10 per-session log is needed to verify actual times and compare visuals. See docs/V1_10_STARTUP_TIMELINE_NATIVE_LBY.md.

V1.09 gauge research: candidate Wrath location read 3128 times with rises=14, falls=9, native AddWrath=0, AddChaos=0; Synergy 128 boosts. Future: confirm resource ownership/gain paths, safe warning display-only suppression, TPS mouse last. Branch debug only; main unchanged. CI builds four root files and preserves numbered archives.


## 2026-10-13 V1.09 multi-diagnostic follow-up

User test with V1.08: native LB+Y behavior appears correct only when Wrath empty; actual charged ability after resource available fires off-camera. Thus synthetic XInput right stick UP for RT/RB differs from LB+Y (native LOOK vector only, XInput stick neutral to protect wheel). Last user V1.08 log: charged chord 1333, native look override 6108, native rotation override 0, RT/RB forward XInput 373 (foot only), Synergy setter 179 with 57 boosts, AddWrath 0, AddChaos 0. Hypothesis: charged ability captures direction elsewhere/earlier, not proven. User requests multiple simultaneous investigations per build; Wrath, Ultimate/Chaos, Skip Warnings, defer mouse TPS.

V1.09 adds only read-only forensic telemetry: exact native GetPlayerLookInput callsite RVA / input-onset timing and pre-edit world look comparison for normal vs charged, original pad edge samples and forward policy; guarded native Wrath resource candidate reads in validated local native callback; one-time exact-binary near CALL site enumeration for two untriggered gauge entry points and Synergy comparator; raw bounded fingerprint of StartupScreens record fields. **No new combat behavior changes**, no UI startup bypass, no Synergy changes, no TPS mouse edits, and no proof until user tests. Track in docs/V1_09_MULTIPATH_AUDIT.md. Preserve debug/zip; main unchanged.


## 2026-10-13 V1.08 wheel-safe native charged-shot aim

User rejects V1.07: LB+Y now opens weapons wheel. Root cause V1.07 injected RX=0 RY=32767 in game-facing XInput while LB held, which navigates/selects weapons radial. V1.08 preserves native LB+Y button bits; makes game-facing right stick 0,0 for charged chord while steering game's native GetPlayerLookInput/Rotation toward TPS camera heading, even in mounted Vanilla Aim. Ordinary LB wheel returns original XInput and still selects using stick. RT/RB forward-stick simulation unchanged. Preserve config for independent charged Y and wheel LB, R3 selected gear USE, horse combo, additive Synergy, all four ×0-×100 charge ranges and inactive Skip Warning. Validation required in actual gameplay: charged shot trajectory, wheel not opening, and successful rebind. Diagnostic logs V1.08 CHARGED NATIVE LOOK / ROTATION / XINPUT / charged totals.


## 2026-10-13: V1.07 TPS LB+Y charged attack aim bridge + 0-100 charge factors

Latest V1.06 user log: Synergy native owner verified for all 443 calls, 94 boosted positive gains, native additive gain path and ABI validated, user confirms it works. User changed factor to 25x (log 1.5 -> 37.5); preserve user settings. Wrath/Ultimate AddWrath/AddChaos 0 native calls, still not demonstrated. Hotstreak max raised to 100x; Wrath, Ultimate, Synergy min0 max100, all default 2x and ON. Zero factor suppresses positive earned points at hooked functions.

TPS charged LB+Y issue: V1.06 early return game-facing XInput whenever radial LB held, which skipped CAMERA_FORWARD XInput RX=0/RY=32767 for charged Y. V1.07 only bypasses wheel early-return when BOTH configurable wheel and charged button held, and treats chord as firing for native TPS camera-view look hooks. Ordinary wheel remains native (original V0.98), native R3 remains selected gear use/stow. Charged button default Y=0x8000, new persist/rebind TPSChargedFirePadMask; wheel binding retains TPSWheelPadMask LB=0x0100. When charged bind custom and held with wheel, translate to native Y in GAME-FACING XInput; camera original XInput unchanged. Keep separate timeout and counters, no synthetic mouse/keyboard. Native shot alignment remains in-game verification pending, and no projectile/raycast hooks have been added. Skip Warning remains quarantined. See V1_07_GAUGES100_TPS_LB_Y_CHARGED_AIM.md.


## V1.06 final Synergy additive hook (2026-10-13)

Exact native gain helper RVA `0x65A1A0` computes `GetCurrentSynergy(holder) + factorA*factorB*factorC` then tailcalls `SetCurrentSynergy`. Two direct gameplay callsites at RVAs `0x67EA9F` and `0x67EAD7` use this helper. V1.06 hooks its additive factor C only for positive local gains validated by the native holder+0x5C0 character getter (0x56BE70); setter is now always pass-through so legitimate spends/resets are preserved. `float` return restored for native setter ABI. New `Gauge V1.06 ADD_SYNERGY` and `additive totals` log lines separate requested/current from actual gains. V1.05 no-boost issue may be resolved for this source path, pending user gameplay validation. Other gauges remain research.

## Latest development: V1.06 native Synergy return type and local-owner resolution

Uploaded V1.05 log 2026-10-13 00:41:49-00:48:19 verifies three gauges ON x2, vanilla Camera Height 0, both reset buttons executed, Horse forward injected 1352 HORSE/42 FOOT, correct startup original native lifecycle. Synergy 75 native SetCurrentSynergy calls, 37 positive gains of approx 1.35, MAX=500 but 0 boosts: **owner this != g_localPlayerCharacter** for all; Wrath/Chaos Add hooks never called in this test. 3 startup warning definitions native flags 1,1,0, matching 2 active candidates, not yet identified screen names.

Exact binary disassembly proves SetCurrentSynergy RVA 0x583040 returns float in XMM0 and Blueprint wrapper RVA 0x7C52C2 consumes returned float after direct call at 0x7C52B3. V1.05 incorrectly switched return to void; V1.06 immediately restores float ABI and forwards return. V1.06 local Synergy owner resolution via exact native getter RVA 0x56BE70 (holder+0x5C0 -> character) verified at caller RVA 0x67E944/0x67EA9F; fallback UE4 OuterPrivate chain; broad actor-field scanning removed. No all-object multiplication. Log whether a verified relation was found and boost count; if relation 0 further research needed. Preserve validated V1.05 vanilla Camera Height 0 / reset buttons and V1.04 horse aiming / R3 / shaders. Skip warning patch still quarantined; do not bypass original three-definition loop. See [V1.06 native research](V1_06_SYNERGY_ABI_OWNER_AND_STARTUP_RESULTS.md).


## 2026-10-13 V1.05 candidate: native gauge owner probes + true vanilla camera reset + startup record observation

**Branch debug only. Main unchanged.** From V1.04 uploaded log: Wrath=0 Chaos=0 Synergy=35 native calls and all boosts=0, despite hook READY; no evidence of working charge multipliers. V1.05 sets all three ON by default x2, migrates only old all-OFF shipped preset, logs local owner, positive Synergy delta and max, and corrects Synergy's native void return type. Never multiply unknown actors.

Vanilla Camera Height should be native default 0, but code/INI/Reset set 50. V1.05 reconciles ALL, migrates exactly old default 50 once to 0, restores TPS FOV override in TPS Camera Values Default. Per-camera settings remain independent; TPS +50 private framing retained. User-customized other values untouched.

Startup warning research: exact EXE function RVA 0x2603F0 produces startup settings with 3 entries at +0x50/0x58, stride 0x40. Loop RVA 0x25FED0 tests record+0x39 and calls RVA 0xAC8760. Old SkipWarning bypass of entire loop potentially skipped UI initialization. V1.05 only passively logs flags from existing movie hook after the loop; no warning skip, no record writes. Which 2 entries draw warning messages is not yet proven. See [V1.05 audit](V1_05_GAUGE_CAMERA_STARTUP_WARNING_AUDIT.md). Retain V1.04 mounted 946 forward-stick injections, horse/shader/R3 gameplay freeze.


## Latest result: V1.04 safety rollback + mounted forward hotfix (2026-10-12)

**Branch debug**, stable main unchanged. Source changes: quarantine startup Skip Warning hook after user-reported crosshair regression; retire toggle, default and legacy persistence OFF. Keep Skip Logos and Skip Intro. V1.03 user session loaded the correct binaries but had NO loader skip warning installation log; do not call causal chain confirmed.

Source V1.03 `tpsCameraStickMode==OFF` short-circuited `FilterNativeXInput` BEFORE firing/forward-stick injection. V1.04 decouples and restores validated V0.92 controller-forward injection even with camera joystick OFF, while OFF stops native TPS camera orbit on foot or horse. Report separate HORSE/FOOT analog injections in log: a nonzero horse injection proves the intended path ran, NOT bullet raycast correctness. Restore V0.98 50-unit TPS-height baseline as TPS-private constant, independent of vanilla height. Log exact loaded INI + FPS/TPS camera config/reticle/mounted aim values to explain saved settings; repair only exact obsolete CameraReset triple, never wipe other preferences.

User log: `Gauge V1.03` hooks all READY, observed Wrath=0 Chaos=0 Synergy=4, boosted all zero. Horse native lease=1, camera source genuinely horse, shader count 2. Skip Warning still unresolved, native cutscene identification still heuristic. User must test actual horse bullet direction and crosshair after new build. See [V1.04 hotfix technical record](V1_04_SKIPWARNING_TPS_HORSE_FORWARD_HOTFIX.md) and older current project state below.


## 2026-10-12 latest authoritative development result: V1.03 compiled experiment

**Working branch `debug` only, `main` unchanged.** V1.03 source compiled and 4-root-file ZIP published successfully via [GitHub Actions run 38311864934](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38311864934). [V1.03 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.03_CAMERA_INPUT_NATIVE_GAUGES_TEST.zip). [Detailed disassembly provenance, settings and test matrix](V1_03_NATIVE_GAUGES_CAMERA_INPUT_TEST_PLAN.md). All **gameplay changes unvalidated until user test**.

User's V1.02 log validated correct loader and ASI, 8 physical R3 equipment DOWN/UP and 8 game-facing secondary stow pulses, native warning `seen=1 skipped=1` without proven visual skip, `rightStickFiltered=7401`, 2 aiming shaders matched and horse region overrides; V1.02 still incorrectly classified R3 as a wheel. V1.03 preserves the equipment hold/pulse algorithm but removes its wheel classification, and retains LB/G/Tab native wheel detection.

Native gain hooks newly identified in exact hash-matching user EXE via reflected UFunction/wrapper disassembly: Wrath `AddWrath RVA 0x660570` actor/float, Ultimate `AddChaos RVA 0x62E060` owner component/float, Synergy `SetCurrentSynergy RVA 0x583040` absolute float; synergy getter field `+0x5A4`, max `+0x498`. All signature-guarded, **OFF by default**, x1-25, manual entry and Default x2, telemetry available. Hotstreak remains independent. Local-owner filtering may need adjustment from next game log if signature READY but no boosts.

Independent Vanilla Camera Values Default and TPS Camera Values Default buttons + F-key actions; separated non-TPS yaw/height/zoom/FOV, saved vanilla sliders greyed when TPS enabled; TPS player attachment fixed true and Foot Origin grouped; camera pad selector OFF/LEFT/RIGHT default right; Default button for each rebindable key and controller mapping, shared Z/Z default. User first test to check R3 equipment, native wheels, controller movement/strafe/fire, horse and shaders. **Skip Warning visual state and actual native cinematic detection remain unfinished**, not silently called fixed. No guessed DPad wheel remapping.

The older V1.02 snapshot below is maintained for historical comparison only; this V1.03 block supersedes its statement that all three resource controls are inert.


**Latest 2026-10-12 task: V1.02 controller-only TPS isolation + three new gauges research + guarded Skip Warning correction.** User test of V1.01 confirmed menu bindings appeared correct and requested **remove all TPS mouse behavior** to establish whether it interferes with native controller TPS. Uploaded V1.01 log reports RAW=59605, motion=59587, duringFire=5851, WMfallback=55, orbitFrames=1294, injected=4396, virtualPadReads=0. V1.02 source removes all mouse-to-stick, RAW/WM fallback, mouse-orbit, mouse-facing/camera-continuity logic. Gamepad orbit remains driven by physical unmodified pad; WndProc native physical mouse remains passthrough. New boot `Skip Warning` guarded island also recognizes exactly 3 native 0x40-byte definitions (span `0xC0`, as observed V1.01) in addition to 1/2. **Both require in-game validation.** Windows CI build and ZIP publication SUCCEEDED; root ZIP entries were validated by packaging step. Three independent persistent Courroux / Ultimate Form / Synergie research presets (checkbox OFF, x2.00 slider range x1-x25) are shown in overlay; **they currently do not affect the actual gauges** because native charge gain owner is not identified. Hotstreak AddJuice is a separate mechanism. `main` untouched, previous ZIPs on `debug` and `archive`. [V1.02 technical note](V1_02_CONTROLLER_ONLY_GAUGES_SKIP_WARNING.md).

> **Canonical handoff file for future ChatGPT conversations. Read this FIRST**, then `README.md`, the latest `docs/V0_*.md`, the latest user log and the actual source code. Update this file **after each meaningful build or user test**, never replace validated results with guesses.

**Updated:** 2026-10-12, after V1.01 mouse isolation request.  
**Repository:** https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod  
**Working branch:** `debug` ONLY. **`main` remains protected/unchanged**; old cumulative binaries in `archive`. **Do not force-push or touch main.**  
**Current compiled experiment:** `V1.02`, Windows x64 CI SUCCESS https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38301250776 ; permanent ZIP verified in `debug/downloads/` (631269 bytes); **in-game confirmation pending**. V1.01 CI: https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38297828118.  
**Verified V1.02 ZIP:** https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.02_CONTROLLER_ONLY_GAUGES_SKIPWARN_TEST.zip  
**Previous V0.98 ZIP archived:** https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/archive/downloads/DarksidersGenesis_V0.98_RESTRICTED_HORSE_DEFAULT_WHEEL_CLEANUP_TEST.zip  
**V0.97 user-validated horse+equipment-wheel reference ZIP archived:** https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/archive/downloads/DarksidersGenesis_V0.97_HORSE_AREAS_RAW_MOUSE_CUTSCENE_WHEELS_TEST.zip  
**Exact target executable:** `DarksidersGenesis-Win64-Shipping.exe`, 62,113,280 bytes, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Do not assume updated/different game executable works.

## 2026-10-12 later update: user V0.98 rollback baseline / V1.00 candidate source

**After the prior V0.99 mouse/wheel report, the user provided an actual V0.98 log** (`DarksidersGenesisMod(20261012-174455).log`), which explicitly starts `0.98-restricted-horse-default-equipment-wheel-validated-cleanup`. They clarify: V0.98 mouse TPS camera **works while NOT shooting** and V0.98 equipment wheel **works**. Treat these as the baseline; V0.99 is an experimental *rejected* candidate. This later update supersedes the previous "rebuild all mouse paths" request: first preserve V0.98 off-fire behavior and test isolated forward-look simulation under physical LMB only.

**Important native input semantics correction:** R3 physically **USES the selected equipment** and does **NOT open the equipment radial wheel**. The existing source logs say `TPS V0.97 WHEEL: R3 physical` but that name is misleading. Do not change verified R3 use or gamepad XInput path as part of wheel investigation. Accessory wheel and equipment wheel must be separately tested with their actual opener.

**V1.00 compiled Windows x64 on `debug`, USER TEST STILL PENDING:** exact V0.98 `src/CameraTraceFeature.cpp` file restored (blob SHA `7fb75854a579a3abaa4b94235d9e862cacf20635`); this also restores known V0.98 cinematic false positive behavior and should be documented. New reversible `TPSMouseForwardFire=1` modifies ONLY the original native local on-foot mouse look-vector returned by `GetPlayerLookInput` while physical LMB is held, no gamepad fire, fresh gameplay camera, no active wheel, using `{cos(TPSYaw),sin(TPSYaw)}`. OFF=unaltered V0.98 physical mouse look. This **mimics forward stick for aiming, but does not inject actual XInput** and does not change projectile raycast or physical gamepad. New INI ConfigRevision 2133. Do not consider validated until gameplay.
- New requested Gameplay features: *Courroux/Wrath*, *Ultimate Form*, *Synergie/Synergy* charge multipliers. **NOT implemented**; existing Hotstreak `AddJuice` hook cannot safely stand in for these three different gauges without proving native ownership. Audit local-player charge getters and add independent toggles/sliders only when native addresses validated.
- Remaining **Skip Warnings**: current loader hook at RVA `0x25FEBF` sees three definition records (native span `0xC0`, stride `0x40`, `seen=1 skipped=0`) but skips only 1/2 records. Identify which startup definition actually draws legal warnings BEFORE extending gate to three, or use targeted safe native dispatch bypass. Preserve logos and intro.
- Last V0.98 log includes rapid player-identity switching and repeated camera-zone reference resets, a separate potential root cause of camera jumps. Log size >10,000 lines, not normal; avoid noisy follow-up instrumentation.
- Detailed engineering audit: `docs/V1_00_MOUSE_FORWARD_AND_GAMEPLAY_ROADMAP.md`.
- CI verified for V1.00; candidate remains experimental until the user tests mouse ON/OFF, the accessory and equipment wheels, and startup. Keep V0.99/V0.98/V0.97 ZIPs available in `archive`.

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

## Verified delivery - 2026-10-13
CONSTRUIT / CONTROLE / EXPERIMENTAL; no user gameplay validation.
Source commit db97a0a3ea9a45821103b67266afd79662c1aa14.
Windows CI run 38417365147: success (configure, Release build, package, upload, numbered ZIP publication).
https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38417365147
ZIP publication commit be47d5a3f7f71f35f730784cb5bf6644231b44ce on debug.
Downloaded permanent raw URL successfully; root entries checked:
DarksidersGenesisMod.asi 893952 bytes; dxgi.dll 153600 bytes; DarksidersGenesisMod.ini 2484 bytes; README.md 200430 bytes.
Both binaries checked for MZ/PE headers and AMD64 machine 0x8664.
SHA256: DBE658AC84009ADA84D6A42701AD96F3706A0A6CA5522D732D210E244756548B.
main 2bc3122c1ae07b7333798462f85c1d9fd3a739f7 and archive 10e0f220b85022bc9269e0cb9390a811a3b1cf89 unchanged after build.
README bundled with build describes pre-CI status; this verified delivery entry supersedes that pending-build status.
No fresh runtime tests; Wrath/Ultimate multipliers and Skip Warnings remain unresolved as documented.