# Darksiders Genesis V0.98 | Restricted-only horse default and radial-wheel stabilization

Date: 2026-10-12. Status: cumulative experimental candidate on `debug`. Stable `main` unchanged. User-approved V0.97 ZIP is preserved on `archive`.

## User validation of V0.97

- Horse Everywhere works perfectly in the user's in-game tests.
- The *equipment/gear wheel is functional again* with right stick on controller. The physical R3 native-passthrough introduced in V0.97 is therefore frozen, along with existing LB wheel passthrough, the gamepad look-vector exemptions, the validated shared E native gear + temporary camera, R3 grenade/stow, Z/Z camera shortcuts, horse summon, horse speeds/stamina, and V0.90 validated shader filtering.
- Wheel UIs switching to the game's **native aerial camera** is accepted as a minor cosmetic issue and deferred. **Do not try more input/angle workarounds** for this until explicitly requested.
- The user confirmed the game's own graphics/UI options can disable the horse summon icon. They do not want any mod override of that icon.

## V0.98 exact changes

1. `HorseAreaMode=2` (`Restricted only`) is now the default of:
   - `config/DarksidersGenesisMod.ini` shipped at ZIP root.
   - `src/ConfigStore.h` member initial state.
   - `src/ConfigStore.cpp` `ResetDefaults()` and missing `HorseAreaMode` fallback when the old master HorseEverywhere is enabled.
   - `src/RuntimeSettings.h` both Snapshot initializer and atomic runtime starting mode.
   - The overlay shows `Restricted only` as default.
   The exact native region/horse-lease code is **unchanged**, including the native desummon predicate classification previously validated. `Off`, `In all area`, `Restricted only`, `Banned zone only` are still manually selectable.
2. Config revision 2132. **Existing user INIs with an explicitly stored `HorseAreaMode` retain their value**. Changing the product default is not silently rewriting a previously saved selection, including a deliberate `In all area`; installing the shipped V0.98 INI or choosing Restricted only once selects the new requested default.
3. **No horse-summon icon hide feature was ever implemented.** Code audit covered `src/DarksidersGenesisMod.cpp`, `src/HorseFeature.cpp`, `src/ShaderAimFeature.cpp`, `src/ShaderAimFeature.h`, `src/OverlayUi.cpp`, plus game/horse UI declarations. There is no horse-specific hide-icon hook, setting, shader/block, widget or draw suppression to delete. Preserve the distinct, working gameplay **Hide Reticle**, **Hide HUD**, and two exact **TPS aiming-indicator shaders**. Those are NOT the horse icon and removing them would cause regressions. Retire the proposed horse HUD TODO as `won't implement: native game option`; remove the now-obsolete icon mapping instructional text from the active overlay.
4. Remove the **failed speculative V0.95 camera-radial continuity injection** from `CameraTraceFeature.cpp`:
   - Remove the stale wheel-camera-recency observation, `radialGameplay` widening of the camera gameplay guard and its diagnostic log path.
   - Preserve the existing fresh-live-gameplay guard, separately validated on-foot mouse continuity and V0.97 cutscene detector.
   - Keep `tpsAbilityWheelHeld`, `tpsAbilityWheelTick` and `IsTPSRadialInputActive`: unlike `radialGameplay`, these control validated LB/R3 native wheel right-stick passthrough and pausing independent TPS orbit. **Do not remove these functioning wheel input paths**.
   - Native aerial view can render in the wheel; deferred cosmetic backlog, not a shipping blocker.
5. No changes to the RAW-only TPS mouse rebuild or V0.97 cinematic auto-suspension experiment. The user's current message does not provide an explicit mouse/cutscene validation result, so neither should be called fixed/validated.

## User log evidence

Test log `DarksidersGenesisMod(20261012-165742).log` shows R3 physical wheel native stick forwarded with sample `rx=8736 ry=8777` and subsequent R3 hold/release; final R3 grenade pulses=2. Native horse region totals: allowed checks 72,705 and overrides 39,955; desummon checks 110,857 and overrides 680; 9 local horse lease arms, and 9 native server horse spawn requests observed. Native grounded aiming shaders matched exactly two signatures. The mod does not perform custom horse icon masking. This log is evidence of hooks executing; it does not validate unrelated cutscene/mouse behavior by itself.

## Required regression smoke tests

- With a fresh INI, overlay should show `Restricted only`; test horse summon/dismount in restricted zones. Existing intentionally selected mode should remain.
- R3 equipment wheel right stick MUST still navigate; native aerial camera transition is accepted.
- R3 grenade stow, E gear+temporary view, horse LB+RB/H, FPS/third-person toggle Z/Z, controller camera, normal Hide HUD/Reticle remain unchanged.
- Package exactly `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md` at ZIP root from Windows x64 GitHub Actions. Keep `main` untouched.
