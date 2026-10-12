# V1.02 | Controller-only TPS, independent gauge research and native warning skip
**Date:** 2026-10-12. **Branch:** `debug` only. `main` untouched. **Status:** Windows x64 CI SUCCESS (run 38301250776), permanent ZIP published and 4 root entries verified by workflow; in-game user validation required. **Base:** compiled V1.01, preserve user-validated V0.97/V0.98 controller/horse/gear behavior.

## Why this isolation build
User's V1.01 log `DarksidersGenesisMod(20261012-195105).log` contains `virtual RIGHT stick`, `RAW INPUT` and `FIRE ORBIT`. Shutdown counters: RAW=59605, motion=59587, duringFire=5851, WMfallback=55, orbitFrames=1294, injected=4396, virtualPadReads=0. The latter proves full virtual-pad creation was not seen, but axis writes still executed. This build removes these source paths entirely, not merely turns a switch OFF.

## TPS changes
- Remove mouse-to-XInput axis writes, disconnected pad synthesis, mouse emulation options, RAW/WM_MOUSEMOVE capture and fallback, fire mouse orbit and cross-input priority arbitration. WndProc now lets all gameplay mouse messages follow original window processing (overlay's own ImGui/UI and cursor hooks are unrelated and retained).
- Remove TPS mouse orbit from camera `UpdateOrbitInput`; use **only native unfiltered controller right stick** for TPS orbit. On-foot mouse camera steering deliberately **does not work** in this test.
- Native mouse-only shot look/getter remains original, including while mounted; no LMB-dependent TPS camera continuity. Camera native horseback fire status is controlled by pad input alone.
- Keep gamepad XInput RT/RB, physical R3 USE equipment + validated hold/stow pulse, LB/native wheels, mounted forward aim, co-op/horses and keys E/Z/Z. Existing camera cutscene heuristic remains unvalidated and can still misclassify stale actor as cinematic.
- Remove `CameraMouseOrbit`, `TPSMouseCenterAim`, `TPSMouseStickMode`, `CameraMouseSensitivity` and prior retired mouse-related keys from active settings / migrated INI. `ConfigRevision=2135`; leave unrelated saved settings intact.

## Three independent gameplay gauges: research preparation, NOT applied
- Separate persistent research toggles and sliders (default OFF, x2.00 range x1-x25) for `WrathCharge` / `WrathChargeMultiplier`, `UltimateFormCharge` / `UltimateFormChargeMultiplier`, `SynergyCharge` / `SynergyChargeMultiplier`.
- The overlay labels all three as **INACTIVE, native hooks not found**. Runtime has independent atomic settings, but no native writers are connected. Changing the sliders **cannot** change gauge fill in V1.02.
- Need identification of the exact retail native charge-add owner, local-player vs enemy/co-op and resource storage for each, followed by read-only tracing and narrow signature-checked hook. **Hotstreak AddJuice is NOT assumed equivalent to Wrath, Ultimate or Synergy.** No speculative memory offsets used.

## Skip Warning native loader correction (candidate)
- Same guarded supported retail EXE and seven-byte signature `48 C1 E7 06 48 03 FB` at RVA `0x25FEBF`.
- User V1.01 loader reports **span=0xC0, 3 entries of 0x40**, seen=1 skipped=0. Prior native loop island skipped only size 0x40 or 0x80, hence no suppression.
- New island accepts exactly **0x40 / 0x80 / 0xC0** while boot SkipWarning gate is ON, sets end pointer=start and leaves native records intact. Other spans fall through unchanged. No edits to movie player, logos, cinematics, or native warning content.
- **Still unvalidated in-game**. Next test: restart game with `SkipWarning=1`, log `Skip Warning V1.02 ... span=0xc0 ... seen=1 skipped=1`, and visually confirm the warning panel is bypassed without breaking startup/menu/intro. OFF should show warnings. If not, identify descriptors and actual native display path rather than assuming hooked loop is authoritative.

## V1.02 test matrix
1. Controller-only: F5 TPS, rotate with right stick, strafe/shoot with RT/RB, use selected gear with R3, both actual radial wheels select correctly, temporary native view E/R3, mounted shoot/summon/dismount, no phantom mouse camera rotation.
2. Physical mouse: no TPS camera steering or virtual controller; vanilla click/aim remains untouched by the mod.
3. Restart and verify warning suppression + logo/intro unchanged and no startup crash; compare SkipWarning=0 if needed.
4. Three research preset entries can save/load x1-x25, **no gauge effect expected**.
5. Camera cutscene false positives separately tracked; left unchanged intentionally for isolation.

**ZIP:** `debug/downloads/DarksidersGenesis_V1.02_CONTROLLER_ONLY_GAUGES_SKIPWARN_TEST.zip` (verified published; size 631269 bytes). Four files directly at root. CI https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38301250776. User testing still required.
