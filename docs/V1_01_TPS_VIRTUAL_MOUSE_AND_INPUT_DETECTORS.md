# V1.01 experimental: virtual TPS mouse stick and native control detectors
Date: 2026-10-12. Branch: `debug` ONLY; `main` unchanged.

## Status / rollback
V1.00 TPS mouse is explicitly **USER REJECTED**. V0.98 mouse orbit when NOT shooting and equipment-wheel handling are confirmed rollback references. V0.97 horse + native wheel behaviors remain frozen. V1.01 is experimental until Windows x64 CI and user gameplay validation.

## Single virtual-stick mouse source
- Remove V1.00 physical LMB-only `GetPlayerLookInput` forward world-vector rewrite. The underlying physical mouse aim/look getter on foot now returns native, as in V0.98, without cursor warp or projectile raycast hooking.
- `TPSMouseStickMode`: **0 = OFF / legacy V0.98 orbit**, **1 = virtual LEFT (movement)**, **2 = virtual RIGHT (aim and TPS camera; fresh default)**. Existing RAW input with WM_MOUSEMOVE fallback yields relative dx/dy once per frame. Scale to virtual analogue from mouse sensitivity and gamepad camera speed, clamp -32767..32767. Virtual RIGHT drives the same TPS camera sample without double-counting old raw orbit; virtual LEFT intentionally does not rotate TPS camera.
- Emit synthetic XInput axis only with valid focused TPS gameplay view, no overlay, detected cutscene, temporary vanilla view or radial. Clear stick on inactive, zero when there is no new motion. Sample freshness expires after 120 ms. If the real controller is disconnected, report virtual user 0 on active TPS only. Preserve physical controller buttons and non-TPS paths. Whether game XInput polling actually accepts disconnected-pad emulation and whether it affects mouse-fired projectiles require in-game validation, not assumptions.
- New config revision **2134**. Existing player INIs preserve their other values; V1.00 `TPSMouseForwardFire` is retired on save.

## Configurable buttons are GAME-DETECTION bindings, not game remappers
- `TPSFirePrimaryMask=131072` (default RT): primary game shoot input detection.
- `TPSFireSecondaryMask=512` (default RB): alternative game shoot input detection.
- `TPSWheelPadMask=256` (default LB): native radial opener detector to bypass TPS right-stick filtering. Existing R3 use-equipment native hold+release still has its own `TPSGrenadeButtonMask` and is not a wheel opener.
- `RadialE`, `RadialG`, `RadialTab`, `RadialAlt`: 4 editable native wheel key detectors, default E/G/Tab/LeftAlt. Keys detect physical holds, no simulated key events; player must match mappings after rebinding in game's own settings.
- Preserve independent shortcuts: L3 short recenter/long TPS, R3 temporary vanilla view, H and LB+RB horse, validated HUD and Strife aim shader.

## Remaining work NOT marked fixed
- **Skip Warning:** user V1.00 log indicates `span=0xc0`, i.e. three native definitions; current exact-signature early hook RVA `0x25FEBF` intentionally recognizes one/two only: `seen=1 skipped=0`. Need inspect all three descriptors and identify warning separately. Bypassing all three without proof could break logos/intro/loading lifecycle.
- **Cinematic TPS OFF:** V0.97 camera heuristic does suspend/restores native look/input, but V1.00 log shows frequent suspected false-positive transitions with stale actor and normal pitch -45. Native cutscene detection and wheel-camera distinction need isolated audit. Do not claim authoritative sequencer flag.
- **Wrath / Ultimate Form / Synergy gauges:** still three independent local-earned charge multipliers pending native owner identification. Never substitute Hotstreak's AddJuice without verification.

## Test order
1. Check 4-file ZIP root, Windows CI, supported exact target executable; archive older validated zips unchanged.
2. TPS mouse RIGHT: idle motion, LMB fire, mouse release; test with real pad connected and disconnected.
3. TPS mouse OFF: regression reference V0.98; LEFT: intentional movement mode; check no input leak with overlay, Alt-Tab, wheel, cinematic, TPS OFF.
4. Match game fire/wheel remaps in overlay, test actual LB-based radial versus R3 equipment USE independently.
5. Record whether Skip Warning still displays and cinematic TPS resumes; these remain independent TODOs.
