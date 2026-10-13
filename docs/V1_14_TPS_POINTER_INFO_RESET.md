# V1.14 - TPS pointer opt-in and one factory reset under Info

Date 2026-10-13. Base `debug` eadf1b914802ec2fb18180a5d9919d73c29e0d94; previous ZIP V1.13 preserved. Source modifications made only on debug.

## Cursor / native view
- New `[Features] ShowTPSMousePointer=0`, OFF by default. When OFF, the existing V1.13 automatic TPS suppression remains bit-for-bit equivalent. When ON, the TPS-specific cursor suppression is bypassed in both UAirshipUIManager::IsCursorVisible native hook and Win32 SetCursor fallback. Existing `HideReticle=1` still takes precedence. UI overlay still allows mouse interaction, and focus restoration/Alt-Tab, pause/cutscene/temporary view logic remain unchanged. Does not synthesize mouse input or force a new cursor type. Applies live when saved through overlay.
- Setting appears under Gameplay > HUD / aiming effects, with readable explanation. This does not alter normal camera or blue aiming effects.

## Reset menu consolidation
- Existing `System and startup` full reset button and `Hotkeys > Reset INI...` both called `Store::ResetIniFileToDefaults()` through the same modal. They were not different actions. Both removed.
- Exactly one full reset button now in the **last About / Info tab**. Same confirmation, dated INI backup, fresh defaults, unchanged saves and Engine.ini. Keep Save/Reload in Hotkeys and per-control Default buttons; these are NOT factory resets.
- No changes to V1.13 reset logic, TPS controls, FOV, horse, Synergy, gameplay damage, LB+Y, startup warnings, or updater architecture.

## User V1.13 log audit
- V1.13 loaded correctly, supported EXE SHA256, TPS=1, HideReticle=0; automatic TPS cursor hiding logged (showing why separate toggle is needed).
- Gains: Synergy 122 observed, 60 boosted; Wrath and Chaos Add hooks 0; Wrath candidate reads 1715, 4 rises and 2 falls. No proof of Wrath/Ultimate multiplier operation.
- TPS right-stick forward injections 610 on foot, 0 on horse. This does not mean horse fails, just no horse shot input in that session. LB+Y seen 0, so not tested in this log.
- Damage diagnostic BaseJuice is an extremely large negative number and ScaleType sometimes 1065353217; values are suspicious and need ABI/format audit separately. No evidence of crash or native gameplay corruption from this sample.
- There is `INI loaded` at 18:02:34, but no `INI factory reset` success message; factory reset feature NOT confirmed from this log.
- Skip Warnings still pass-through and unresolved; do not change it in V1.14.

## CI and acceptance
Build x64 and verify four ZIP files at root: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Preserve previous numbered ZIPs. In-game tests: TPS enabled, Show Mouse Pointer OFF = invisible by default; ON = original game cursor can appear; Hide Reticle ON overrides ON; toggle overlay/ALT-TAB/cutscenes; normal camera unchanged; only one factory reset under About; Cancel leaves INI, confirm preserves old as .bak; game saves unchanged. Experimental until user test.
