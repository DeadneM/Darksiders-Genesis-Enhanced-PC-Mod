# V1.25: relative RawInput ownership recovery and passive gauges / warning / cinematic study

Date: 2026-10-14. Branch **debug** ONLY. Cumulative baseline V1.24, confirmed by user to retain primary/secondary mouse shooting. Test pending.

## User V1.24 evidence
- V1.23 user-validated relative mouse = 17,154 packets; fallback moves 1,213; edge recaptures 37 (no errors).
- V1.24 log: relative mouse = 67 packets; fallback moves 2,353; WM_MOUSEMOVE 2,465; edge recaptures 49 (no errors); native raw samples near screen edge 0. Two mouse shots work. These establish starvation, NOT yet its cause.
- Previous startup registered RawInput exactly once; it was never checked after game/UI scene transitions. Microsoft documents that only one receiver of a device class is active per process and `GetRegisteredRawInputDevices(nullptr,...)` reports required capacity with ERROR_INSUFFICIENT_BUFFER. The old query mishandled that expected result when registrations existed: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getregisteredrawinputdevices

## V1.25 precise changes
1. `EnsureRelativeMouse` now polls mouse ownership every 1.3 seconds while TPS enabled, enumerating registered process devices correctly. Logs `TPS V1.25 RAW_OWNER` target HWND / flags changes.
2. NEVER replaces a mouse registration owned by another HWND, including the game's own registration. If none exists, the mod arms its previously working foreground RawInput to the game's actual HWND at startup, or re-arms in focused live TPS gameplay. Max 8 attempts per game session; `RAW_REARM` confirms success/fail and `RAW_STALL` shows when legacy WM_MOUSEMOVE events continue without relative RawInput although registration is present.
3. No physical cursor repositioning while actual relative RawInput is fresh (500 ms). Preserve V1.23 edge recapture only as a backup when RawInput goes silent, so the game remains playable rather than stuck at a physical edge. **Not yet a promise of permanently eliminating SetCursorPos**. The diagnostic distinguishes registration loss from WndProc/event routing loss. No native cursor visibility hooks or Hide Reticle code.
4. Native resource origin telemetry: read-only caller RVA for verified Synergy gains; and for Wrath/Chaos native additive functions IF they ever execute. Synergy x2 gain behavior remains unchanged. Wrath/Ultimate x2 remain UNVERIFIED and cannot be described as working.
5. Startup warnings: read-only candidate three dwords from +0x20,+0x24,+0x28 in each of three known 0x40-stride native definitions. The 8-byte samples in V1.24 resembled an index/number pair but are **not proved to be a UE FName or visible warning text**. The original preparation loop and MoviePlayer remain unmodified; Skip Warnings stays OFF/disabled.
6. Cutscene heuristic: logs first 24 `CAMERA_TRANSITION` candidates with actor validity, native angles/FOV/Z, player distance and auto-suspend flag; first 20 `CINEMATIC_ENTER` expanded with classification. No direct native LevelSequence / dialogue event detection or gameplay change. R3 held remains temporary vanilla view.

## In-game test plan
- Rotate camera continuously in both directions for multiple 360° revolutions at every border, then after firing and after a loading transition. Check whether the OS pointer no longer jumps while native relative stream is healthy.
- Open/close overlay Insert, equipment wheel, Alt-Tab. Confirm both native mouse shots with defaults and a swapped binding, and controller RT/RB/R3, horse and HUD.
- Compare `RAW_OWNER`, `RAW_STALL`, `RAW_REARM`, `RAW_RECOVERY totals`, `RELATIVE_INPUT totals` in log. If native registration is foreign, deliberately no override; this test does not yet solve that route.
- During combat, collect and spend Wrath/Ultimate, see `WRATH_CANDIDATE`, `SYNERGY_GAIN_CALLER`, `WRATH_NATIVE_CALLER`, `CHAOS_NATIVE_CALLER`. Identify timing of each pickup if feasible.
- Confirm 2 warnings still visible (not skipped); `WARNING_IDENTITY_CANDIDATE` is research. Trigger real dialogue/cinematic and check `CAMERA_TRANSITION` log and R3 hold fallback. Do not mark automatic detection native/complete without proof.
- Windows CI Release must verify ZIP exactly 4 ROOT files: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Existing personal INIs and old V1.23/V1.24 ZIPs preserved. Do not touch main or archive.


## V1.25 CI and permanent ZIP publication verified (2026-10-14)

GitHub Actions [run 38542601698](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38542601698): **SUCCESS**, including Windows x64 CMake configuration, Release compilation, exact-four-root-files archive inspection, upload and permanent `debug` push. [Permanent V1.25 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.25_RELATIVE_RAW_RECOVERY_TEST.zip): **756,018 bytes**, Git blob SHA-1 `16dc0637d7ac86c130d8d2d7055af02b79bed801`. V1.24 size 753,948 bytes and V1.23 size 752,055 bytes remain untouched. No modifications to `main` (2bc3122c1ae07b7333798462f85c1d9fd3a739f7) or `archive` (10e0f220b85022bc9269e0cb9390a811a3b1cf89). CI success verifies build and packaging only, **not** real in-game relative-input continuity or a working warning skip. Validation remains pending.

