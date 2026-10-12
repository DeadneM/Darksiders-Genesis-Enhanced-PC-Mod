# V0.97 | Horse zone selector, RAW mouse reset, cinematic camera and native radial debug

12 October 2026. Experimental cumulative debug branch. User test V0.96 log `DarksidersGenesisMod(20261012-153517).log`. **V0.96 ZIP archived** before modification; main remains untouched.

## Native horse area mapping: known vs. test assumption

The V0.96 user confirms the two toggles do NOT split the two visually distinct no-summon areas. `HorseAllowRestrictedSummon` behaved like master Horse Everywhere; `HorsePreventForcedDesummon` appeared to do nothing. The log shows: native `IsInHorseRegion` false -> true 51,788 of 63,977 checks, `IsInHorseDesummonRegion` true -> false 1,070 of 78,169 checks; 8 native horse spawn requests, 6 live horse leases. These are two **predicates, not documented zone classes**. HUD padlock vs absent icon mapping is not established and cannot honestly be identified from the logged getter output alone.

Replace two toggles with ONE `Horse Everywhere` combo and new INI `HorseAreaMode`:
- 0 Off: master OFF, both original native getter answers untouched.
- 1 In all area (DEFAULT): exact same native region pregrant + scoped native desummon override as user-validated V0.95/V0.96 (both ON).
- 2 Restricted only (EXPERIMENTAL): only override the native region getter FALSE for the already verified local player when the ORIGINAL native desummon predicate is FALSE. Do not override desummon.
- 3 Banned zone only (EXPERIMENTAL): only override native region getter FALSE when the ORIGINAL native desummon predicate is TRUE; maintain V0.91 scoped desummon suppression. No quest actor spawning or blanket scripted dismount bypass.

When either selective mode is used, `HorseFeature::HookHorseRegion` calls the ORIGINAL `IsInHorseDesummonRegion` trampoline (not our modified detour) to classify that exact actor. If unavailable, fail open. Native getters are expected read-only; no asset/HUD class ID for the padlock exists. User must test 2 versus 3 and validate whether the visible two area categories map to these predicate combinations. Class names in dropdown are UI hypotheses not proof.

Compatibility: migrate legacy `HorseEverywhere=0` to 0, `=1` to 1; remove old independent `HorseAllowRestrictedSummon` and `HorsePreventForcedDesummon` from saved INI, write new 0..3. On user selection save legacy master as mode!=0 for backwards compatibility, and publish the selected mode through runtime Snapshot.

## Complete RESTART for on-foot TPS physical mouse path

V0.96 source still combined V0.75 mouse look-vector injection with world-space camera yaw in some branches and one `SetCursorPos` at each physical fire-down; V0.96 log saw eight mouse cursor warps plus 1,994 legacy mouseLookAim getter modifications, native GetPlayerLookRotation calls ZERO, and user says the camera stops when firing.

V0.97 removes **all mod-generated OS mouse cursor positioning** from gameplay; deletes those globals, mouse warp routine and calls. On-foot mouse fire returns untouched native `GetPlayerLookInput` and `GetPlayerLookRotation`, for both aim modes. The camera uses only native WM_INPUT relative RAW mouse deltas; WM_MOUSEMOVE is a non-synthetic fallback ONLY when no RAW delta has been received for 1.5s. Absolutely no angle-yaw injector for mouse; controller aim still uses validated native simulated XInput and getter handling. Camera RAW orbit does NOT freeze on on-foot mouse firing, even if shared gamepad aiming mode 1 is active; mounted fire / physical controller behavior remains unchanged. `TPSMouseCenterAim` remains a compatibility INI key but its ON state now controls RAW TPS mouse orbit, NOT centering the cursor; OFF disables only the independent RAW orbit, not native mouse targeting. Default ON.

**Crucial:** this preserves game-native cursor-plane aiming, not necessarily camera-centered 3D projectile raycast. To achieve actual TPS shooting along camera forward (including pitch), finding/hooking the validated native weapon targeting raycast is still future research, not solved by this reset.

## Cinematic auto-suspension ON by default

New `TPSAutoCutscene=1`. The existing CameraTrace native `GetCameraView` candidate flags **non-gameplay** camera (native pitch/proximity/actor freshness); require a recent valid TPS view and >= 500 ms continuous mismatch to mark `tpsCutsceneSuspended` as true. Native output already passes on rejected camera views; new state ALSO gates TPS orbit, XInput suppressions, native player look detours while cinematic plays. When native gameplay camera returns, clear immediately and resume saved TPS state WITHOUT toggling ThirdPerson or resetting the orbit; no menu, wheel, hotkey, HUD, horse/weapon edits. When option OFF, only previous V0.96 view-validity heuristic is used and no explicit input suspension occurs. This is **native camera-shape heuristic**, not authoritative game `Sequencer` cutscene state; may miss in-place cinematics or classify unusual camera sequences incorrectly. Verify cutscene start, long duration, and return.

## Radial wheel R3 / LB correction

Prior XInput bypass handled LB but not *physically held R3*, which also drives equipment/gear input. V0.97 now preserves actual right-stick RX/RY native data when physical R3 is pressed while keeping the existing validated R3 grenade hold / release pulse. As before LB bypasses all TPS rewrites. Both LB and R3 tell native look detours to pass the game's selection vector, and pause independent TPS orbit from the right stick while holding those inputs. Additional log `TPS V0.97 WHEEL` confirms game-facing physical R3 right-stick data actually forwarded.

This is a concrete hypothesis, NOT a proven repair for every weapon/ammo/ability menu. The user reports wheels still broken after earlier versions. If wheel native input does not use LB/R3 or game uses a UI-specific input route, a camera/input native widget analysis will be required. **Do not falsely mark every wheel fixed before gameplay validation.**

## Defaults and packaging

Keep user supplied V0.96 base INI: ThirdPerson OFF, global height 50, TPS distance 0.15, TPS pitch -12, TPS height 0, TPS FOV 110, mouse sensitivity .12, pad speed 180, Z/Z toggle + recenter, E shared native equipment/temporary view, LB+RB horse, native R3 controls, shaders from V0.90 untouched, horse movement/sprint/stamina and all prior gameplay fixes. New `HorseAreaMode=1`, `TPSAutoCutscene=1`, `TPSMouseCenterAim=1` with redefined raw behavior. Config revision 2131. Stable main untouched, previous V0.96 archived, one Windows x64 ZIP with exactly 4 files at root.
