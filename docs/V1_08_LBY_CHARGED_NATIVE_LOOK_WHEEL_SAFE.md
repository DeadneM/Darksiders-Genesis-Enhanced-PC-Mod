# V1.08 | TPS native LB+Y charged aim without wheel

## User regression report
V1.07 made LB+Y open weapons wheel. It incorrectly supplied right-stick UP (RY=32767) while LB was physically held, so the radial wheel could select weapons. User wanted the SAME aiming correction as R1/R2, not a new wheel function.

## Isolate charged aiming from wheel selection
- Keep original LB+Y gamepad button bits; no changes to native charged ability chord.
- While simultaneous TPS wheel mask + charged Y bind held, set **game-facing XInput RX=RY=0** so there is no synthetic wheel selection input. Original physical right stick remains available to camera through trampoline.
- In existing native `HookPlayerLookInput`, charged chord is treated as firing, giving game native world-facing unit vector `cos TPS yaw,sin TPS yaw`; in `HookPlayerLookRotation`, charged chord maps native look yaw to TPS camera. This applies even in alternate mounted Vanilla Aim for the charged chord, whereas ordinary RT/RB code retains original policy.
- Pure LB without Y remains native wheel and right-stick selection. Rebind Y to other gamepad button in TPS overlay / INI while keeping shared wheel LB rebind and R3 native gear USE; horse LB+RB and old RT/RB synthetic right-stick UP untouched.
- `TPS V1.08 CHARGED XINPUT`, `TPS V1.08 CHARGED NATIVE LOOK`, `TPS V1.08 CHARGED ROTATION`, and shutdown `TPS V1.08 charged totals` provide independent source/aim telemetry.
- The game's charged projectile/raycast is not hooked. User in-game test needed: LB+Y shoots charged forward without opening wheel; LB-only still allows wheel selection; native RT/RB unchanged.

## Preservation
All V1.07 gauge ranges Hotstreak, Wrath, Ultimate, Synergy ×0..×100 and V1.06 additive Synergy ×2 mechanism unchanged. Wrath/Ultimate 0 native events in latest log. Skip Warning remains safe/pass-through, no startup lifecycle skip. All previous numbered ZIPs retained.
