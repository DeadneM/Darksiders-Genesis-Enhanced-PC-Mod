# V0.80 | First native beam visibility-only suppression TEST

Date: 11 October 2026. User sent `DarksidersGenesisMod(20261011-124507).log` for V0.79 passive visual effect probe.

## New confirmed evidence from user log
- Correct exact shipping EXE matched and both UE native beam exec hooks installed.
- `ShowBeamEffect` **9** calls total, **6** in TPS.
- `HideBeamEffect` **25** calls total, **18** in TPS.
- The previous `HideGroundTargetingEffect` had **0** calls. Its target function is NOT active in this scene.
- Log demonstrates that a real beam component has a show/hide control in live gameplay, but NOT that it is necessarily the turquoise world-space aim line or semicircle.

## Game-native implementation audit
The supported EXE's reflected `ShowBeamEffect` method at RVA `0x7D17B0` consumes the UE4 FFrame and jumps to a virtual visual show method via vtable slot `+0x920`. The reflected `HideBeamEffect` at RVA `0x7D0040` consumes the FFrame then forwards to native hide RVA `0x6AD240`.
The **native hide** body begins `48 8B 89 B0 06 00 00 48 85 C9`: it reads the same owner's `+0x6B0` visual object, checks null, then invokes the component's effect-shutdown method at RVA `0x601760`. That method uses component pointer `+0x28` and dispatches the engine's visual state transitions. It does not read or modify direction, target, damage or projectile parameters.

## EXACTLY scoped V0.80 change
**Only the aim effect investigation block in `src/DarksidersGenesisMod.cpp` is edited**; the build workflow/README/this doc are the only other files. Existing V0.78 settings, `ThirdPerson=0` packaged default, F5/L3/R3/grenade, mouse & pad fire, strafe, horse, HUD and other gameplay hooks are unchanged.

Inside existing `HookAimShowBeamExec`:
1. Always call native original reflected `ShowBeamEffect(owner,frame,result)` with exactly the original arguments.
2. If exact supported EXE, real current TPS active, hold-vanilla view inactive, application not shutting down, owner non-null and the existing `TPSHideGroundAimFx` checkbox enabled, call the game's **native visual hide implementation** `0x6AD240` once with that same live owner after the original completes.
3. Preserve the original `HideBeamEffect` callback unchanged, and leave normal game camera and effects untouched.
4. Guard against binary version drift with separate signatures for both reflected wrappers AND native hide implementation. Any mismatch fails open with no beam hook or visual manipulation.
5. Add only visual-specific `forcedVisualHides` and `skippedInTPS` counters.

## Explicit limits and rollback
This is **an experimental visual suppressor**, not yet an accepted fix for Strife's blue line or arc. The visual component might own a different beam effect; the arc may be another primitive. The game may show the beam before TPS is enabled, bypass the reflected wrapper for direct calls, or respawn it through another path. Do NOT conceal these limitations or change combat to make the visuals disappear.

**Validation:** With the same Strife dual-pistol aim effect visible, turn TPS ON, trigger normal aiming and look for disappearance of (a) straight blue line and (b) semicircle separately. Switch back to vanilla camera; native effects must still show. Toggle `Hide ground targeting effect in TPS` OFF to compare effect while keeping all movement/shooting identical. Test both mouse and controller, fire impacts and hit detection, save/load, mount and second player. Return user log with `TPS V0.80 AIM FX totals`. If visual doesn't disappear, revert to V0.79 and continue effect-asset tracing. No automatic promotion to official release.
