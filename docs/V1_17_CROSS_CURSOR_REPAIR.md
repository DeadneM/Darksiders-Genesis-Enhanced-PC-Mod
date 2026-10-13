# V1.17 - Targeted standard cross cursor repair, diagnostic

Date: 2026-10-13. Debug branch only; keep V1.16 archive, never modify main/archive.

## What the user observed in V1.16
The cross-shaped cursor bug appeared after the new defaults changed to ThirdPerson=1 and ShowTPSMousePointer=1. The supplied V1.16 session log from 20:13:41 to 20:14:59 has supported exact game EXE, verified V1.16 ASI, TPS=1, HideReticle=0, and preserved safe DXGI startup MoviePlayer attachment (0x260244 not patched). The two logo names (THQ_LogoBasic and AS_LogoBasic) were substituted using the existing V0.52 selective hook. Three native startup-screen definitions were observed read-only. Normal camera/reticle hooks installed; loaded game and ran normally to shutdown, with default TPS camera attached at 20:13:52. No evidence for a new unsafe startup hook being installed.

**Hypothesis, not confirmed root cause:** With ShowTPSMousePointer=1, the previous TPS cursor suppression is disabled, making a native OS cross-shaped cursor visible. The V1.16 log does not establish the OS cursor shape because low-priority historical CursorProbe events are filtered. This repair specifically tests the Windows standard IDC_CROSS hypothesis, without reverting requested default changes.

## Code change
- V1.17 **retains** new defaults ThirdPerson=1, ShowTPSMousePointer=1 in ASI member, runtime initialization, reset and shipped INI, preserving users' existing overrides through the updater.
- Under a verified active TPS gameplay view of at most 250ms freshness, game foreground, overlay closed, no HideReticle, HUD visible, no temporary TPS suspension, auto-cutscene or radial held, if native SetCursor requests exactly the stock IDC_CROSS HCURSOR, substitute stock IDC_ARROW. Only icon shape changes, no pointer position, game input, camera, or visibility toggles.
- A cross icon already set before the TPS gameplay view appears is handled by a throttled 350ms read-only GetCursorInfo probe and a post to the **game window thread** of WM_SETCURSOR. If the visible handle matches the system IDC_CROSS, the window thread restores the arrow, even if there was no new SetCursor request. This does not synthesize focus events or mouse movement.
- Bounded log: TPS V1.17 CURSOR_PROBE and TPS V1.17 CROSS_CURSOR; startup CONFIG line additionally prints ShowTPSMousePointer. If the visible cross uses a custom Unreal HCURSOR or an in-game sprite instead of stock IDC_CROSS, this filter will not hide it; the log should help narrow the alternative.
- No changes to safe V0.52 Skip Logos, disabled Skip Warning, V1.15 gain diagnostics, in-game updater, horses, LB/Y, native input, crosshair shader suppression or camera transforms.
- Version numbering: V1.17 only (no letters). Four-file cumulative ZIP at root on debug.

## Acceptance
User tests cold startup/load with ThirdPerson=1, ShowTPSMousePointer=1 and HideReticle=0, watch whether cross disappears and **arrow remains visible**. Compare pointer OFF in Camera > Third Person (should hide pointer), reticle F4, overlay Insert, wheel LB/G/Tab, cutscene, Alt-Tab; inspect TPS V1.17 CURSOR_PROBE and CROSS_CURSOR logs. If CROSS_CURSOR remains zero and cross remains visible, investigate nonstock or engine-drawn cursor with a read-only identification approach. Do not apply aggressive MoviePlayer bypass. CI x64 compile/package/publish required before sharing ZIP.
