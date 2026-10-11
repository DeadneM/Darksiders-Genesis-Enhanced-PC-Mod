# V0.85 | Stabilize Mounted TPS Camera While Firing: source audit, repair candidate and real live counters

2026-10-11. User confirmed **V0.84 R3 grenade hold/stow works**. This code is frozen in V0.85. Their new observation: *Stabilize mounted TPS camera while firing* seems ineffective, with gameplay log `DarksidersGenesisMod(20261011-154341).log`.

## What user log actually shows
- Exact supported EXE accepted; both native camera hooks installed.
- User successfully summons/rides horses: `SERVER_SpawnHorseMount request n=1`, `HORSE MOVEMENT READY` for three horse owners, and `TPS V0.70: LIVE horse camera pose` at samples #1/#1000/#10000.
- TPS camera switches between source `HORSE` and `FOOT` seven times during the recording. That establishes an opportunity for an intermittent mounted pivot issue; it **does not** prove which switches happened during firing.
- V0.84 log had **no telemetry** about mounted stabilization requests, latch, smoothed frames, or actual correction. It is impossible to conclude the switch works from the old log.
- R3 native release pulses recorded repeatedly, and the user has validated proper equip and stow. Do NOT adjust grenade hooks.

## Root cause confirmed by source audit
The old `CameraTraceFeature::HookView` reads `tpsHorseStableAimCamera` and `tpsAimActive`. Prior implementation ONLY latches the horse pose from an already-selected `HORSE` camera source, to give horse priority over a **fresh foot pose** during shooting. It **does not perform camera smoothing**. Where the foot pose has naturally become stale, it already selects a fresh horse pose independently of the checkbox: visually, ON and OFF frequently have no distinction. Worse, the latch unconditionally did `actorValid=false` even when horse pose was stale, potentially rejecting a valid gameplay camera frame after dismount/mount transitions.

## Exactly scoped V0.85 experiment
In `src/CameraTraceFeature.cpp` and its diagnostic data header:
1. Continue selecting native unmodified foot and horse poses using their existing time freshness checks. If a horse position isn't recent, never invalidate a fresh foot camera and never read a cached UObject.
2. When TPS and **Stabilize Mounted TPS Camera While Firing ON** and the game reports active RT/RB aiming and a horse was already the actual frame source, retain the latest fresh horse pose rather than transient foot source.
3. Add a **150ms first-order low-pass filter to the vertical camera pivot only** while actively shooting on a recognized horse. XY following, actor yaw, orbit, native targeting vectors, damage, projectiles and physical controls are unchanged. Reset filter on mount identity change, teleport/large height jump >300 units, time gap >300ms or after firing stops. No game memory writes, camera output only.
4. If OFF, preserve original unfiltered camera behavior.
5. Add observable camera telemetry: `mountedFireRequested`, `mountedFireHorseSelected` (only successfully attached TPS frames), `mountedFireOverrodeFoot`, `mountedFireHorseUnavailable`, `mountedFireVerticalSmoothed`, last vertical correction and current-active flag. New `TPS V0.85 MOUNTED AIM` log lines include actual height correction. UI adds readable `ACTIVE/waiting for fire on horse/OFF` information next to existing option.

**Limitations:** No proof yet that `tpsAimActive` is populated for every firing modality while riding, or that vertical bobbing is the dominant symptom. These counters are specifically designed to tell whether the option is reached and whether a physical height change was actually applied. The user must compare mounted firing with checkbox ON and OFF. This is not a confirmed visual fix.

## Strict regression freeze
This branch forks from V0.84 and changes **ONLY** `src/CameraTraceFeature.cpp`, `src/CameraTraceFeature.h`, `src/OverlayUi.cpp`, documentation/build workflow; `src/DarksidersGenesisMod.cpp` (grenades, L3/R3 inputs, strafe, weapon input), `src/HorseFeature.cpp` (horse speed/sprint/stamina and summon probe), `src/ConfigStore.cpp`, runtime settings, HUD, aim indicator modes remain BYTE-FOR-BYTE identical. Main branch and prior ZIP stay intact.

## Test
- Enable TPS and stabilize option; summon and mount a horse.
- While mounted, press/hold fire and compare camera movement with stabilization ON and OFF without changing other settings. Check in the overlay `Mounted aim: ACTIVE`, >0 horse frames, >0 smoothing frames and nonzero latest correction during movement; compare foot->horse source transitions.
- R3 hold and release once: must still equip and stow exactly as V0.84.
- Send log for `TPS V0.85 MOUNTED AIM` and latest overlay readings.
- If requested>0 but horse frames=0, camera cannot identify fresh mount or active game camera; if requested=0, the game's controller/mouse firing may not set `tpsAimActive`. Never silently claim stabilized.
