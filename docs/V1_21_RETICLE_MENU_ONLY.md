# V1.21 unified TPS menu-only cursor + General Hide Reticle (2026-10-13)

User report after V1.20 game test: gameplay and relative TPS mouse worked until toggling Gameplay > Hide Reticle and its inverse TPS pointer control; user asks only ONE global manual Hide Reticle in Camera > General, with automatic menu-only cursor visibility while TPS. **Do not change unrelated defaults** or resurrect rejected V1.16/V1.17 pointer replacement. V1.20 code is the direct base.

## Log evidence
V1.20 (21:22-21:28 local): ASI x64 loaded, safe Skip Logos setup, relative mouse registered, 12,237 raw mouse events incl 2,541 at border (no cursor warps), 2,690 mouse-camera frames, 1,515 native forward-look injections. HUD/reticle changed ON at 21:23:46 -> OS suppression after overlay close -> OFF at 21:23:58; TPS pointer toggled OFF/ON at 21:24:15-16; global Hide Reticle ON/OFF again 21:28:29/36. Separate controls competed for the same Win32 SetCursor/UI manager suppression. The log does **not prove** a permanently stuck cursor, but it corroborates conflicting toggles and suppression transitions. Synergy 59 positive native add gains / 59 boosted, Wrath rises 5, AddWrath and AddChaos 0.

## Actual changes
- Move unchanged single **Hide Reticle (independent of HUD)** checkbox and Apply/Return button from Gameplay/HUD to **Camera > General camera and reticle**, keeping F4 mapping and existing HideReticle=0 default. Gameplay keeps its HUD switch and aiming line settings.
- Remove the inverse **Show Mouse Pointer in TPS** checkbox. Replace in Camera > Third Person with informational line **"Show Reticle in Menus Only While TPS: Automatic"**. No user toggle or separate TPS cursor preset, no mouse/camera/sensitivity/input changes.
- Both existing cursor hooks now consult the same `HideNativeCursorDuringTPSGameplay`: TPS enabled, active fresh TPS gameplay view, game foreground, overlay closed, wheel closed, not suspended for cinematic/temporary vanilla. Suppress pointer only during TPS gameplay; native cursor can be used in mod overlay, equipment wheels, non-TPS and native cutaway. Global Hide Reticle also suppresses gameplay cursor when enabled but respects interactive overlay and wheel.
- Remove obsolete `showTpsMousePointer` field from ConfigStore/RuntimeSettings, reading and runtime publishing. `ShowTPSMousePointer` is omitted from new default INI and deleted on explicit settings save (existing personal INI otherwise remains untouched by updater). **ThirdPerson=1 remains unchanged**, HideReticle=0 remains unchanged and all other defaults untouched.
- Startup DXGI, V1.20 RawInput relative mouse, Camera Forward shooting, mount, cutscenes, read-only Wrath and working Synergy unaffected. No old cursor icon substitution introduced.

## QA
1. Install V1.21 and enter TPS, move mouse: no stuck cursor, camera turns infinitely with raw relative input. In TPS gameplay cursor not shown automatically; open Insert mod menu and wheel: cursor visible and usable; close to resume gameplay.
2. Check Camera > General **Hide Reticle** ON/OFF, F4 mapping, Apply and Return. Confirm exactly one manual general reticle control; Gameplay has none and TPS has only informational auto policy.
3. TPS OFF/ON, menu, wheel, cutscene, horse, alt-tab, controller and LMB Camera Forward. Any missing native menu pointer is a regression, provide log and precise menu.
4. No new setting needed; old ShowTPSMousePointer INI key ignored and removed on next explicit save. Upgrade preserves all other saved settings. Four root files, x64 CI and main/archive untouched. Next version V1.22.
