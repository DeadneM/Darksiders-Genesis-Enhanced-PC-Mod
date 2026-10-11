# V0.81 | Strife targeting line + semicircle: V0.80 negative result and next asset investigation

Date: 11 October 2026. User explicitly reports the V0.80 visual-hide test does NOT affect the cyan line or semicircular arc visible in the earlier gameplay video. Preserve existing validated mod behavior. **NO executable changes in this branch**: it is deliberately forked from V0.79 passive beam probe, NOT from V0.80 visual-hide experiment.

## User log: V0.80 test NEGATIVE

`DarksidersGenesisMod(20261011-130549).log` (2026-10-11 15:00-15:05) confirms:
- Supported EXE exact, old targeting hook still unused (`groundFxCalls=0`).
- ShowBeamEffect `show=5`, all five in TPS.
- HideBeamEffect `hide=18`, all 18 in TPS.
- New `forcedVisualHides=5`, `skippedInTPS=0`.
- V0.80 calls native beam visual hide after every intercepted ShowBeamEffect but **the user's actual turquoise aiming line and arc remain visible**.
- Therefore **ShowBeamEffect/HideBeamEffect is NOT the correct visual visibility path for the requested Strife indicator**. Do not pursue it or expand its suppression to other game paths without new evidence. Build V0.80 rejected for targeting-effects goal, not a stable baseline.

## Target to hide
From user video `ProjectMayhem 2026-10-11 11-12-21.mp4`, sample gameplay image at 5-7 seconds:
1. Bright thin cyan world-space straight aim line from Strife's pistols toward its forward direction.
2. Curved matching cyan semicircle / angle arc close to the muzzle/character.
These should disappear **only in Third Person** and reappear in vanilla camera, without changing aim vectors, collision, projectile trajectories, gameplay input, HUD reticle, damage, or other player behavior.

## Candidate classes, not verified sources of this visual
From static UTF-16 names in exact EXE: `UMayhemTargetingUIComponent` at file offset `0x27EAEC8`, `UMayhemTargetUiPositionComponent` `0x27EAF80`, `UMayhemPlayerAimComponent` `0x27BFC38`, `UMayhemPlayerAimAbility` `0x27BFB68`, `UMayhemPlayerDualPistolAimAbility` `0x27CF790`. **Presence of names does not establish that any emits the requested visuals**. `MeleeTargetingComponent`, `RangedTargetingComponent` text near `AMayhemHorseCharacter` only and should not be assumed relevant to on-foot Strife.

## Reliable next step
Inspect cooked asset directory listing of `ProjectMayhem/Content/Paks/ProjectMayhem-WindowsNoEditor.pak`, expected to be about **8.8 GiB** (do NOT ask the user to upload this giant file just for a listing). Obtain a list via a compatible UnrealPak executable (`UnrealPak.exe "ProjectMayhem-WindowsNoEditor.pak" -List > paklist.txt`) or UE4 asset browser tools' export filename list. Search for `Strife`, `Target`, `Aim`, `Line`, `Arc`, `UI`, `Indicator`, `Spline`, `Beam`, `Cursor` and materials/particle systems. Afterwards request only the specific small `.uasset/.uexp` pairs needed to inspect their references. Avoid guessing visual-component hooks or blocking game-native aiming.

User requirement: **Do not change any other code**. Preserve V0.78 as the last gameplay code checkpoint, V0.79 passive test as inspection baseline, V0.80 failed visual experiment quarantined. No compiled version required for a documentation-only asset audit; no official release change.
