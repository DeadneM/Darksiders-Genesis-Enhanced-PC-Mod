# V0.95 | Shared E, native radial wheels, mouse aim and horse-region admission

Date: October 11, 2026. Experimental cumulative build on `debug`; stable `main` unchanged. Protected source history: V0.90 two exact aiming shaders and V0.91 native horse-mount retention. The complete V0.94 test ZIP is preserved on `archive`.

## User's explicit clarification

- `TemporaryView=E` **must deliberately share** the vanilla equipment/gear E key. One physical E can trigger the native gear wheel and, after the TPS temporary-view hold delay, temporarily use the original game camera. V0.94's suppression on native-wheel input was WRONG for this shared binding.
- On gamepad, equipment radial selection by RIGHT STICK still fails in TPS. Other radial menus sometimes return to the default view instead of remaining TPS.
- Horse Everywhere reliably keeps/summons horses once admitted, but in banned zones the initial native summon sometimes requires two physical taps, especially on controller.
- Hide **only** the native horse summon prompt/lock HUD icon while Horse Everywhere ON. Other HUD elements must remain intact. An exact widget/texture/shader identity has NOT been recovered; do not hide other UI experimentally.
- TPS mouse camera/aim remains unreliable; requested repair.
- Set both `ToggleTPS=Z` and `RecenterTPS=Z` as authoritative defaults.

## Native wheel fixes

The controller guides confirm that Genesis uses LEFT BUMPER to open the ability/gear wheel and RIGHT STICK to choose its entry. Our own TPS filtering had incorrectly left a gamepad R3 transformation active before passing LB-through in some hook paths; now the **physical LB fast path** occurs before any TPS R3/aim transformations and returns the original native XInput packet to the game. In addition, previous V0.94 native GetPlayerLookInput and GetPlayerLookRotation wheel paths remain pass-through.

CameraTraceFeature now uses a guarded **radial UI camera continuity path**: on a real held radial wheel with previously valid attached TPS view within 650ms, the same live locally-controlled player and bounded native camera proximity, keep TPS camera even if the game's radial camera changes its native pitch. Normal cutscenes, menus, unbounded cameras and stale actors still fail open to native output. A few `TPS V0.95 RADIAL` events log only recovered views.

For keyboard E (and any separately rebound temporary-view key), input is deliberately **NOT suppressed** by the wheel flag. Controller temporary-view R3 is suppressed only while the separate LB wheel is physically held; otherwise original R3 hold/release + 300ms camera threshold still applies. The shared E gear+view behavior is restored without synthesizing a new E press.

## On-foot TPS mouse aim trial

New setting `[Features] TPSMouseCenterAim=1`, default ON, separately togglable in Camera > TPS. During a real LEFT MOUSE firing hold, on-foot in TPS Camera Forward only, while focused, not in a wheel, not in a menu, with a fresh validated TPS gameplay camera view, center the **game's native OS cursor** on the client area so the native mouse-plane aiming point can track the TPS forward direction. The independent TPS camera continues reading physical WM_INPUT RAW mouse deltas. Ignore the exact synthetic center WM_MOUSEMOVE for mod orbit calculations; forward the native Windows event so the game's targeting can use it. Do not touch native projectile raycasts, game camera UObject pointers, horse camera, Vanilla Aim or gamepad aiming. This is an **unvalidated mouse experiment**, not a guaranteed hit-direction fix. User can disable the setting if cursor targeting is worse.

## Horse first-press reliability (EXPERIMENTAL)

V0.94 prearmed keyboard WM_KEYDOWN/H and XInput simultaneous LB+RB, but sporadic double taps remained. V0.95 complements those gates by answering the native IsInHorseRegion query **as permitted for the exact validated, recently live, local on-foot player whenever Horse Everywhere is enabled**, even if the engine checks eligibility *before* receiving the first physical key/chord. Only the original getter's FALSE is changed; the original always executes first; no actor spawn, input replay, UObject memory writes or side-effect calls occur. The **desummon region hook remains V0.91 behavior**, scoped only during actual summon/mount, so native script/mission restrictions aren't globally bypassed by that second hook. This broadens *region admissibility only* and may change some scripted behavior; needs actual user testing, including normal/unmounted gameplay and quest zones.

## HUD summon/locked icon

**Not modified in V0.95.** None of the existing horse native functions expose a provable UMG widget/texture or unique shader for the summon/locked prompt. Blocking the general HUD CVar, drawing a black rectangle, or filtering a guessed pixel shader would conceal unrelated UI and regress validated V0.90 masking. To isolate the specific icon for a later build, identify its rendering draw or native UMG asset through a before/after shader capture in both allowed and banned areas. The same texture/icon apparently represents summon or locked with an overlay; handle them together once identified.

## INI defaults, compatibility and rollback

V0.95 ConfigRevision 2129: fresh profile has both TPS shortcuts set to `Z`; migration changes ONLY an old identical `V/V` shortcut pair to `Z/Z`, preserving different individually rebound pairs. Keep all V0.94 user INI numeric settings. `TPSMouseCenterAim=1` added as a reversible gameplay test. Other settings retained, including shared `TemporaryView=E`, `Horse=H`, 0.15 camera distance, 100 height, 110 TPS FOV, LB+RB native horse, R3 grenades/temporary view.

One Windows x64 compiled `debug` ZIP must contain exactly `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md` at archive root. `main` untouched. V0.94 snapshot on `archive`.
