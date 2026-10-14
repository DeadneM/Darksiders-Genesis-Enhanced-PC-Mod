# V1.26 - Persistent relative mouse registration recovery, V1.25 log interpretation

2026-10-14, branch `debug` only. **EXPERIMENTAL** until user confirms it in game. V1.25 compiled successfully, but the uploaded session ends with missing RawInput registration after a fixed retry cap. In-game user feedback on camera motion has not yet been given.

## V1.25 actual log, 11:33:35–11:38:07
- `RawInput` 46,193 packets vs **67 in V1.24** and 17,154 in V1.23. `WM_MOUSEMOVE` fallback 324 vs 2,353 in V1.24, `RAW_RECOVERY checks=202 missing=24`, `warpsAvoided=8522`.
- The physical edge fallback triggered **6** times (zero failures), so V1.25 **did not eliminate warps entirely**. Do not claim a fully native uninterrupted FPS input system until long play with zero warps is confirmed.
- Process-wide native mouse registration repeatedly changed from our game HWND to *absent*. `RAW_REARM success` attempts 1 through **8**, no errors and no native/foreign owner seen; the final owner state **0** proves that the hardcoded max 8 attempts is insufficient.
- Why the registration disappears is not yet identified (native engine scene handling, focus behavior or other module). We do not intercept `RegisterRawInputDevices` or assert who removes it.

## V1.26 narrowly scoped change
- Preserve V1.25 conservative receiver enumeration, focused-TPS gating, WndProc `WM_INPUT` relative deltas, controller and mouse fire mapping, menus and reticle-hook removal.
- Replace **8 total lifetime rearm attempts** with **at least 2 seconds between rearm attempts**, and limit console log per attempts after the first 9 (log every 25th). This allows an indefinite series of careful repairs when the process has **no** registered mouse receiver, while never overwriting an existing native/foreign receiver and never altering registration outside focused gameplay except original startup initialization.
- Extra counts: `RAW_RECOVERY throttled` and `recoveredAfterOldCap`. A long-session test should show recovery number 9+ when registration disappears again. The V1.23 edge recapture remains a **failsafe** only when live relative packets are silent. This does not itself identify the unregistration culprit or prove that the fallback can be permanently removed.

## Secondary observations for next native research
- **Synergy:** 72 additive calls, 35 positive gains and **35 boosted**; `positiveNotBoosted=0`. Verified native caller **RVA 0x67EADC** in this combat session, no direct setter gains. Preserve x2 implementation.
- **Wrath:** live resource candidate +2, +2, +2 = **6** units; spent 20. Native `AddWrath RVA 0x660570` observed **0** calls, so active gain route is **not explained by that hook**. No direct field writes or new unverified detours justified; focus future assembly research on source of candidate field update, likely reward/pickup or script code. Native Ultimate `AddChaos RVA 0x62E060` also zero, cannot mark x2 working.
- **Startup warnings:** exact candidate identifiers stable across V1.24/V1.25: index-like record0 `0x0001C99E` number 2, active 1; record1 `0x0001CB7E` number 0, active 1; record2 `0x0001CB6C` number 0, active 0. The first two active candidates may correspond to both warnings but are **not verified onscreen identities**, and preparation loop/native MoviePlayer remains untouched. Skip Warnings still OFF.
- **Cinematics/dialogues:** `CAMERA_TRANSITION` candidate 1 far camera gap ~78,614, missing actor during scene load; candidate 2 missing actor gap ~1,505; `CINEMATIC_ENTER` once after 500ms, at 11:38:03, before shutdown. **No native dialogue or LevelSequence identity established**. R3 temporary vanilla camera works (ON then OFF once); use as fallback.

## Test plan
Test for 6–10 minutes, rotate camera continuously in both directions at borders **and after passing the 8th lost-registration event**. Observe lack of freezes and count any visible cursor jumping; a valid long-session test should show more than 8 successful `RAW_REARM` only if the game actually removes its registration again, and no foreign registration should ever be overwritten. Test native primary/secondary remaps, R3, radial wheel, mounted controls, overlay and Alt-Tab. Send full log. For gauges, isolated enemy kill vs Wrath pickup and Ultimate pickups; capture timestamps and identify visual resource movement. For warnings and dialogues, note screenshots/timing, because passive logs alone do not establish presentation.

## Build policy
Only `debug`; preserve all previous ZIPs and frozen `main`/`archive`; four files directly in ZIP root, Windows x64 CI must pass. No claim of gameplay validation before user test.
