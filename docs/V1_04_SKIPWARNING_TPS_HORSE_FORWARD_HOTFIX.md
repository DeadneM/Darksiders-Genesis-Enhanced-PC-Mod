# V1.04 | Quarantine warning patch, restore TPS camera baseline, protect mounted forward fire

Date 2026-10-12. Branch `debug`; no changes to `main`. Based on compiled V1.03; requested regression hotfix after user supplied `DarksidersGenesisMod(20261012-215218).log`.

## Actual supplied V1.03 runtime evidence
- Correct ASI `1.03-camera-input-cleanup-native-gauge-hooks-test` and exact EXE SHA validated. Loader showed Skip Logos startup hook and **no Skip Warning startup hook installed message** in THIS session. This means one must not claim warning hook executed then. User associates turning warning skip on with a recurring crosshair/cursor issue historically observed. Deliberate risk mitigation is to quarantine the unproven native loader patch globally.
- Native reticle UI SetCursor hook repeatedly active at startup; later `Overlay -> Hide Reticle OFF` confirms the user's persisted configuration had a different Reticle state than shipped `HideReticle=0`. Loaded state was TPS ON when gameplay became active (shipping defaults `ThirdPerson=0`), consistent with an existing user INI, not proof fresh defaults failed in memory.
- Gameplay hook `TPS V1.02 MOUNTED AIM mode=CAMERA_FORWARD padFire=1` at 23:51:38. Forward analog injections=1266 across entire session, WITHOUT horse/foot split. `horseActorGetPlayerLookInputCalls=0` and projectile raycast hook=NO. Forward analog was not independently proven to execute while mounted. Source-level regression in V1.03: `FilterNativeXInput` returned prematurely if `tpsCameraStickMode==0`, before firing detection and XInput aim emulation.
- Horse spawn 1, real local horse pose from hook, live TPS horse camera, and lease=1. Horse native code untouched. Shader matches=2, draws blocked=976. Gauge hook READY for three, calls Wrath=0 Chaos=0 Synergy=4, boosts=0.
- `TPS V0.97 CINEMATIC` auto-suspension remains heuristic and not proof real cutscene state. Independent pending issue.

## Code changes
1. Remove call to `InstallSkipWarningLoopHook` from early DXGI loader no matter previous INI; disable runtime setter; `SkipWarning=0` shipped and saved; UI says disabled for crosshair safety. Skip Intro/Logos unaffected. A genuine warning-dispatch solution is still TODO.
2. Move old forward-stick gamepad emulation to execute before joystick OFF early-return; keyboard/mouse TPS orbit not reintroduced. Preserve V0.92 injection values X=0 Y=32767 and two independent RT/RB fire bindings. New count `g_tpsMountedForwardAnalogInjected` and `g_tpsFootForwardAnalogInjected`; log `TPS V1.04 FORWARD: ... scope=HORSE` to establish actual mounted injection. Joystick OFF now stops TPS camera orbit mounted or on-foot, not mounted fire. Native Vanilla Aim mode still independent.
3. Reinstate the previously default 50-unit vertical TPS framing without cross-wiring it to `cameraHeightOffset` via `kTpsBaseHeight=50`. Normal camera height remains independent. Camera reset values/INI unchanged.
4. ConfigRevision 2137: only one exact old reset tuple migrated to intended TPS defaults, otherwise retain user choices. Print authoritative config snapshot after Load; INI is persistent intentionally, so defaults are NOT forcibly reapplied over user customization at every startup.

## First user test
- Replace BOTH binary files from V1.04 ZIP together; preserve personal INI backup. Verify ASI version and `Skip Warning V1.04 ... quarantined` in loader and no `Skip Warning V1.02 ... installed` lines.
- Before touching UI, look for `V1.04 CONFIG loaded` showing actual values and compare with displayed defaults. Existing saved customizations intentionally survive. Use `TPS Camera Values Default` button if you want the camera numeric presets restored.
- With controller Right Stick (default), mount and fire RT/RB in Camera Forward, confirm actual projectiles go ahead, check **HORSE** forward injection events and shutdown `all/horse/foot` counts. Repeat TPS joystick OFF and (if useful) Left; verify OFF no longer kills forward emulation.
- Verify no original startup cross and reticle toggles normal. Validate LB/G/Tab selection and R3 hold/release, Horse Everywhere mount and native shaders.
- If camera still aims incorrectly while mounted DESPITE >0 HORSE inject counts, target actual mounted projectile raycast next; no unchecked UObject writes. Do not treat injected-stick counter as proof of projectile direction.
- Gauge boost tests deferred until core regressions stabilized. No unconditional default wipes.
