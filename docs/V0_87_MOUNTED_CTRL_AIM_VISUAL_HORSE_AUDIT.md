# V0.87 | Mounted aim consistency, native Ctrl bridge and focused visual/horse diagnostics

2026-10-11. Base **V0.86**. User confirms mounted **Vanilla Aim works while firing**: camera stops rotating and game-native aiming works. Regression: without firing, right stick moves camera AND game aim simultaneously. **Camera Forward** still fails to direct mounted shots where TPS camera points. User explains keyboard Ctrl initiates the game's native aiming. Mouse/keyboard TPS camera sometimes sticks at particular angles and cannot rotate full 360 degrees: **known issue, intentionally deferred**, do NOT secretly modify yaw wrap/clamp/rotation in this branch. Preserve validated V0.84 R3 grenade and V0.86 rebinding.

## What log proves
- Exact EXE, camera view/SpringArm, XInput look, grenade and horse hooks install.
- During mounted Camera Forward, many `TPS V0.86 MOUNTED AIM mode=CAMERA_FORWARD` events, so its selector and mode detection are active, **yet user observes wrong shot aim**. Merely assigning the native look vector did not activate the game's Ctrl-style targeting state.
- In mode Vanilla Aim `controller camera orbit SUSPENDED` and `RESUMED` appear, confirming correct firing-only freeze. Native stick previously leaked through to the game **even when not firing**.
- `Horse Everywhere V0.86 summon INPUT #1/#2 key=LB+RB` and `SERVER_SpawnHorseMount request n=1` show successful game invocation path. A later input #3 had no subsequent server request. However it cannot by itself prove this was in a restricted area or that input was interpreted as a summon (could be dismount).
- `HideGroundTargetingEffect` remained `groundFxCalls=0`. Do not claim blue line/arc is fixed.

## Mounted Vanilla Aim correction
For TPS with fresh **actual mounted-camera source** + `TPSMountedAimMode=1`:
- **NOT FIRING**: forward raw right-stick inputs to our TPS orbit; clear game-facing XInput right stick, and suppress right-stick native look vector/rotation through existing exact native hooks. Game-facing shooting/targeting and on-foot controls remain untouched.
- **FIRING**: the XInput right stick and original game-native look getters are passed through; independent TPS orbit freezes exactly as already validated. On release, camera-only resumes automatically.
- Diagnostic `TPS V0.87 MOUNT: idle right-stick native look suppressed` and total count.
No camera angle wrap changes to avoid introducing another 360-degree mouse regression.

## Mounted Camera Forward: experimental Ctrl bridge
When TPS + real mounted camera + mode 0 + **actual RT/RB or left mouse fire** and focused gameplay window, synthesize a `WM_KEYDOWN VK_CONTROL` to **that game window only**; on release/leave mounted camera/TPS/focus, synthesize paired `WM_KEYUP`. If user physically holds Ctrl, preserve physical key state. The rest of the original V0.86 camera-forward GetPlayerLookInput and XInput filtering remains unchanged. Never call system-wide SendInput, disable aim abilities, or mutate player/game camera pointers. **Not guaranteed** to work: Unreal may use RawInput/async keyboard state; log `TPS V0.87 CTRL AIM` counts only proves messages were posted, not that the game accepted the action. Actual shot direction must be judged in game.

## Alternative line and arc candidates: passive class dispatch probe
- User's target is the cyan straight Strife aim line and cyan semicircle **drawn even when Hide HUD is active**.
- V0.77-V0.86 `HideGroundTargetingEffect` was not called. V0.80 Show/HideBeamEffect suppression was already visually rejected. V0.82 broad two-class event blocking was unvalidated; NOT merged.
- In this branch install an **exact-EXE-signature-checked** passive `UObject::ProcessEvent` probe (native RVA `0xAC0630`) that detects only **two other suspect visual UI components**, native class slot RVAs `0x3812DD0` (`UMayhemTargetingUIComponent`) and `0x3812DE0` (`UMayhemTargetUiPositionComponent`). Every original event is passed through unchanged. Count targeting, targeting in TPS, targeting during fire and positioning. Presence/absence will help determine if either is event-driven during Strife aiming. The material/particle owner might instead be `AMayhemBeamVisual`, `RangedTargetingComponent` or `AimTrajectoryBeamEffectProperties`; those hypotheses remain **unproven** until asset or shader isolation. No attempt to hide globally or break aiming here.
- Final log `TPS V0.87 AIM UI PROBE totals`, overlay live counters. This is a NEW passive diagnostic of the alternative native UI branch, NOT the final visual hide fix.

## Horse Everywhere upstream diagnostic based on validated native horse recognition
- Native `HorseCharacter::GetNormalizedSpeed*` / `TryStartSprinting` callbacks establish live horses and their movement, safely (no stale memory dereference).
- The user's game calls mount with **H** or **LB+RB**; log new attempt with `serverBefore`, `liveHorse` and native horse-callback count. After ~1800ms log `serverDelta`, `horseCallbacksDelta`, current horse state, and whether a server request did not occur. This isolates an upstream permission/Blueprint gate versus a server-spawn failure more precisely.
- **Horse Everywhere still research-only, not working in forbidden zones.** Do not call `SERVER_SpawnHorseMount` with guessed parameters or bypass mission scripts. Use comparison of allowed and denied attempts to determine native/Blueprint permission candidate.

## Regression freeze and acceptance
No updates to: grenade pulse algorithm, config defaults/bindings, horse speed/stamina logic, HUD/reticle, game damage, jump/glide, DOF, skip startup, camera yaw/angle limits. Tested modes: Vanilla Aim off-fire camera-only, on-fire vanilla; Camera Forward follow camera + native Ctrl input experiment; L3/R3 unchanged, custom binds unchanged, normal on-foot unchanged. Log must show actual game behavior; rollback V0.86 if issues.
