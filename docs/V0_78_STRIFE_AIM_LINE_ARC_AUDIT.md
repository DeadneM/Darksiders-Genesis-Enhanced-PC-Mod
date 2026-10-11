# V0.78 | Strife blue aim line and semicircle (visual evidence)

Date: 2026-10-11. The V0.77 controller/camera gameplay behavior is user-validated and FROZEN; this is a narrowly scoped follow-up. **Do not touch** the original V0.75 GetPlayerLookInput/GetPlayerLookRotation hooks, native right-stick isolation, controller grenade/L3/R3, TPS camera attachment, mounted camera source, aiming/gameplay direction, HUD, reticle, combat, or speed/damage systems without separately informing the user and preserving a rollback.

## Precisely identified visual target
User supplied the 21.08-second, 2560x1440 `ProjectMayhem 2026-10-11 11-12-21.mp4`.
At approx 5 to 7 seconds of the video, Strife stands with pistols raised. A narrow **luminous cyan/blue-green straight line** extends from the character's aiming position toward the world target, and a matching **cyan semicircular arc** marks the aiming direction immediately in front of the character. These are WORLD-SPACE aiming visuals, not the OS cursor, D3D overlay, item selector, UI reticle, and not a stand-alone grenade targeting effect. The user wants **both** hidden in TPS; normal mode should retain both. One or both may be particle/mesh/beam elements; this is not yet verified from game cooked assets.

## Log evidence
User V0.77 `DarksidersGenesisMod(20261011-091333).log`: native `HideGroundTargetingEffect` hook signature validated/installed, but shutdown recorded `groundFxCalls=0 alreadyHidden=0 forcedHidden=0`. The old hook cannot remove the visible blue line or semicircle during the shown session. No runtime FX function was modified here. More invasive viewport-wide postprocessing and removing native targeting ability or changing game look-vector are expressly rejected because they could break the validated combat.

## Candidate native/asset graph, still hypotheses
The exact `DarksidersGenesis-Win64-Shipping.exe` includes reflection names `RangedTargetingComponent`, `UMayhemPlayerAimComponent`, `UMayhemPlayerAimAbility`, `UMayhemPlayerDualPistolAimAbility`, `UMayhemTargetingUIComponent`, `AMayhemBeamVisual`, `BeamVisualClass` and `AimTrajectoryBeamEffectProperties`. Their presence does **not** prove which of these renders the specific Strife cyan beam and arc. In particular, `AimTrajectoryBeamEffectProperties` appears alongside projectile/gear targeting fields and could be unrelated to dual pistols. Proper removal requires tracing the actual asset/material/particle or spawn/visibility callback in the cooked game files, then adding an **exact-signature-guarded, TPS-only, reversible** visual suppression hook to a new experiment. Do not claim a visual fix before it is confirmed by the user.

## Explicit default correction requested
`[Features] ThirdPerson=0` for ALL newly packaged V0.78 INIs and also defaults / Reset Defaults / INI read fallbacks. No forced migration of any existing user's settings; if an existing INI has ThirdPerson=1, respect it. TPS remains activatable by F5 and long L3. Other values from the user's Oct 11 INI preserved: FOV, offset, camera controls, R3 hold delay 300ms, L3 long press 500ms, fire options. All previous V0.77 feature hooks are otherwise unchanged.

## Build and acceptance
V0.78 ZIP at root: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Verify `ThirdPerson=0` in ZIP; check first launch default TPS OFF, F5 and L3 still toggle, R3 delay/throw works, blue line and arc remain documented as **unresolved**. Do not promote the experimental ZIP to the stable release yet.
