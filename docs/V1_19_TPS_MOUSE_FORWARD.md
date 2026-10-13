# V1.19 TPS mouse Camera Forward integration (2026-10-13)

## User directives
- Build based **only on V1.18** (canonical V1.15 TPS restored); V1.16 and V1.17 remain REJECTED and V1.17 IDC_CROSS repair must never be imported.
- **Only** default overrides explicitly requested: ThirdPerson=1 and ShowTPSMousePointer=1. These are now true in Store member defaults, ResetDefaults(), missing-key fallback, runtime State/Snapshot, packaged INI. No changes to other unrelated defaults. Existing personal INI values are *never overwritten* by built-in updater; if a user has both OFF, check the Camera tab or edit two keys.
- ShowTPSMousePointer preference hidden behind the Third Person enable checkbox. When TPS OFF, Store::PublishRuntime() sets showTpsMousePointer runtime state false; OS/UI cursor hooks still implement global Hide Reticle but no TPS-pointer processing is active.
- One and only one TPS mouse camera-forward/fire route, **no separate enable/tuning option**. New code is NOT copied from earlier rejected V0.98 or V1.17 implementations.

## V1.18 user test conclusions
- Confirmed ASI and supported executable; startup DXGI logo substitution once, MoviePlayer attach unchanged, no cross reticle reported in the described comparison. Control tests with ShowTPSMousePointer=0 could not reveal the cross due to invisibility; retest requires pointer=1.
- V1.18 observed 413 native WM_MOUSEMOVE and 10 WM_INPUT (undecoded), LMB=4, no mouse camera injection; saved native Wrath candidate 20->18 (one decrease), no gains; Wrath, Chaos, Synergy hooks all observed zero. Startup definitions three native records with byte +0x39 values 1,1,0; display not identified or skipped.
- **DXGI source actual diff** V1.15->V1.18: seven read-only warning-record diagnostic lines only. No evidence the logo detour changed functionality between V1.15 and V1.18. V1.16/17 defaults and mouse interventions are independent confounders; do not assert DXGI is cause without genuine ON/ON/pointer-visible comparison.

## Mouse implementation
- WndProc collects relative mouse movement from RID_INPUT mouse raw packets where available and from WM_MOUSEMOVE change fallback when not. No suppression of game WM_INPUT, no SetCursorPos, no ClipCursor call, no fake XInput.
- Only when live TPS gameplay: foreground, overlay closed, no radial menu, no cinematic or temporary vanilla view, native camera valid. Deltas bounded then consumed in existing Present / UpdateOrbitInput camera path. Camera yaw/pitch blend with controller's native right-stick orbit, using one internal heading and bounded pitch.
- LMB firing in the local verified native GetPlayerLookInput/GetPlayerLookRotation TPS detours uses the SAME camera-forward direction path that pad RT/RB already uses. No second mouse shooter mode and no new UI option. Native inputs and underlying attack mechanism unchanged. Native projectile origin/raycast verification still required: tests must confirm shot direction visually. Diagnostic counters MOUSE_CAMERA and MOUSE_FORWARD totals.
- TPS mouse interaction is inert while TPS OFF. Existing controller native wheel, R3 use, LB+Y, horse/TPS focus guards retained.

## Skip Warning
- Restored a visible but disabled **Skip Warnings** checkbox in the startup menu with an explicit explanation that feature is NOT functional: native warning-screen startup-loop bypass remains quarantined to preserve reticle and UI setup. Keep old SkipWarning=0 default untouched. No fake success.
- Scope of actual safe warning skip research remains screen presentation identification after initializing three records; do not bypass 0x25FED0/0x260244 or alter records.

## Test protocol
1. Test V1.19 cold boot, pointer ON and TPS ON, Skip Logos ON; check cross bug and report.
2. TPS OFF: mouse pointer-specific checkbox hidden, OS pointer follows native rules, camera untouched; resume TPS and confirm pointer ON.
3. TPS ON: move mouse left/right/up/down both idle and while LMB firing; check Camera Forward shot direction vs controller RT/RB and ensure wheels/AltTab/horse/cutscene unchanged. No extra activation required.
4. Skip Warnings entry visible, disabled and clearly labeled research-only; warnings still show.
5. Logs MOUSE_CAMERA and MOUSE_FORWARD; assess Synergy/Wrath/Ultimate only if pickups or combat actually occurred.
6. Four root files in ZIP; debug only; main/archive untouched; next version V1.20 without letters.
