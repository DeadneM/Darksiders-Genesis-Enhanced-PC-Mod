# V1.32: native mouse targets and camera ownership TEST

2026-10-14, debug only. Base c41292c, cumulative V1.31 experiment, retaining user-validated acquired features. Status before CI: EXPERIMENTAL, not built or validated in game.

## User result and requested rule

The 18:35-18:37 V1.31 test is rejected for cinematic recognition/mounted aim: user reports a real cinematic did not hand off, mounted mouse shots went behind in all tested positions, pause and both wheels returned overhead even with cinematic auto-suspend OFF. This is visual user feedback, not inferred from math rays.

User's chosen rule: TPS everywhere except real cinematics and the configured temporary native-view shortcut. The shortcut remains untouched and has priority if shared with a native wheel (including E). No default/key migration or new settings required.

The captured log has 24 detailed mouse clicks (18 LMB, 6 RMB), all FOOT before mounting; 18 upward and 6 downward mathematical rays, all in the camera's forward hemisphere. Its 24-click cap does not prove mounted firing absent. It contains a 47ms SHORT_CUT, four STATIC_NO_ACTOR messages, no ENTER, eight successful RawInput re-arms, and an explicit temporary-native ON/OFF. Full private snapshot SHA256 699DAAC5FB44CCE8B98D883EA644F262A7517D9F503407763E1188F7CCF6BD98, retained locally outside the repository. No claim of native projectile impact measurement.

## New exact native evidence

All RVAs below are from the supported EXE SHA256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54. Four new detours and the pause getter have exact 16-byte signature guards; the EXE is already fully checked by TargetValidator before installation.

- Mouse direction helper 0x5719A0 and target helper 0x571E50 both read mouse pixels. **Correction to the previous audit hypothesis:** 0x1ACAA70 is also GetMousePosition, forwarding to 0x1886560. The caller discriminator must NOT be called a mouse/gamepad discriminator. New code does not rely on its meaning.
- 0x571E50's original signature is controller, output FVector, subject actor, cached FVector+valid byte, flag, opaque extra argument. It calls shared deprojection at 0x571EB3 (return 0x571EB8), then the existing native world collision query with native channel 0x13 and range 100000. It uses an existing target while cache+0xC is valid, otherwise falls back to actor position. The flag controls the native cache write/debug branch; its source-level name is unproven.
- Reflected ForceCinematicMode exec 0x798320 calls a game-instance wrapper, which reaches native lifecycle 0x648B20 with instance + enabled + allPlayers. The new observer always forwards every original argument, mirrors the enabled state and logs edges. This is a native lifecycle signal, not a proven asset/sequence name and not proof all scenes use it.
- Reflected IsGamePaused exec 0x1D15B70 calls 0x188D940. This read-only native getter accepts the current live camera UObject as world context, resolves its world and tests native pause. No guessed pause state offset or cached world dereference.

## One mouse target acquisition path

New NativeTpsFeature isolates targeting from controller input:

1. Provider requires focused exact-game TPS view, current emitted camera (<250ms), physical configured mouse fire held, no physical pad firing/recent right-stick, no overlay, wheel, UI hold, temporary native view or cinematic suspension. Subject must match current validated local character or current fresh mounted actor identity. Unknown owners pass through.
2. Compute normalized 3D ray through the physical cursor using the emitted TPS origin, pitch/yaw, FOV and client aspect. No cursor warp, XInput simulation, actor rotation or movement write.
3. Both game mouse helpers use the original **3D target helper** and its existing collision query. A thread-local synchronous scope overrides shared UE deprojection ONLY at return RVA 0x571EB8 and for the matching controller. All other calls retain original deprojection. No blanket UE raycast change.
4. Use a fresh stack target cache so an old world/screen target cannot silently survive a miss. A native hit supplies the target; sky/no hit supplies origin + ray * 100000, always ahead. Actual native hits can update the original cache according to its existing flag contract; misses do not write a persistent synthetic target.
5. The native 3D consumer gets that target. Consumers that require a 2D direction receive normalized XY **from reference point to the same collision target** instead of a ray/ground-plane intersection behind the camera. This is native target acquisition, not a camera-yaw patch. Native downstream projectile/muzzle behavior stays native and is NOT yet proven for every weapon or mounted path.
6. Retire the rejected V1.31 mouse look-vector/rotation overrides. Preserve existing controller look branches and the entire FilterNativeXInput/R3/RT/RB/horse chord implementation. Native helper calls outside the strict physical-mouse scope return the original results.

New log budgets are separate for FOOT and HORSE, with native hit/no-hit, target and direction plus helper/collision totals. The old first-24 WndProc math probe is no longer the only evidence source.

## Camera ownership, independently from geometry

Priority: explicit temporary native shortcut / TPS OFF; native cinematic mode when auto-suspend ON; hold last emitted TPS composition in native world pause or a known held native radial; ordinary live TPS.

The held view contains only seven copied floats, a generation and an identity token. It is reused only in a current callback for that same camera and player generation. No cached actor/world is dereferenced, and no fake actor freshness is published. During UI hold, gameplay aiming/orbit is ineligible; native selection/input remains passed through. The configured temporary-native shortcut still takes precedence and can deliberately show overhead view during a shared-key wheel.

With the native module installed, the old distance/500ms cinematic classifier no longer controls suspension. The existing coarse fail-open safety for unknown/stale/far/abnormal camera views remains; it is not called a verified cinematic. Without a reusable TPS view, known pause/radial state, matching generation or native mode signal, native view remains the safe fallback. If a menu pauses through another native mechanism or a scene never sets ForceCinematicMode, additional lifecycle work may still be required. Do not claim universal recognition yet.

## Preserved / rollback / success criterion

RawInput registration/recovery, Alt-Tab, native mouse button mapping, controller XInput/RT/RB/R3, horse statistics/leases, shaders, gauges/Synergy, HUD, warnings, shortcuts, INI defaults and updater remain. No changes to HorseFeature, GaugeChargeFeature, ShaderAimFeature, ConfigStore or packaged INI. main/archive frozen, all prior numbered ZIPs retained.

Build goal: test native mouse target acquisition on foot AND mounted and camera ownership from native lifecycle, replacing the ineffective yaw and geometric-suspension experiments.

Success requires actual game impacts following upper/middle/lower cursor targets on foot/horse, both mouse buttons, near walls and sky/no hit; pause and each wheel retain TPS unless temporary shortcut is held; the previously missed scene hands off and returns correctly; controller-only, wheel selection, RawInput and Alt-Tab still work. Compile/PE/ZIP checks establish only CONSTRUIT / CONTROLE / EXPERIMENTAL. Only the user can mark gameplay VALIDATED; failures remain explicit.

Rollback: back up personal INI and reinstall preserved V1.31 ASI/loader, or the user's accepted earlier package. Local installation of V1.32 must wait for verified CI/package and a stopped game, preserve personal INI, back up replaced files, and verify destination hashes. No game EXE, assets or saves are modified.

## Cumulative TODO

- Runtime proof of helper coverage/owner gates on foot, horse, all weapon types and actual impacts; no claimed full projectile/muzzle solution from static proof alone.
- Determine whether the missed real scene calls native ForceCinematicMode, and whether each wheel/pause uses native world pause or known radial-held state.
- If a path is not covered, inspect its native consumer/lifecycle; do not add another camera distance threshold or root-yaw workaround.
- Wrath/Ultimate gain hooks and Skip Warnings remain unresolved, outside this experiment; validated additive Synergy unchanged.

Delivery target: cumulative Windows x64 ZIP, exactly dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini and README.md at root. GitHub Actions and artifact/remote heads must be checked before claiming delivery.

## Verified Windows delivery

Source cf613bb714c53c9677ca19496a01ccd88e59aa62. GitHub Actions run
[38608331116](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38608331116)
SUCCESS: configure x64, Release compile, exact-four-root ZIP, upload and debug
archive publication. Archive publication commit d842a55f1bbaa296f737f9c633591fc3024644c9.
[Permanent V1.32 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.32_NATIVE_TARGET_CAMERA_OWNERSHIP_TEST.zip)
775037 bytes, SHA256 C0EE1089862D61D37BC355724BA3DB2872346F520828BDFE4F56B709D2EA7124.
Both binaries independently checked MZ/PE AMD64 0x8664. Direct raw URL downloaded
and matched the same hash. Root entries: ASI 1144320, dxgi.dll 154624, INI 2677,
README 238614 bytes. CONSTRUIT / CONTROLE / EXPERIMENTAL; no gameplay validation.

Local V1.31 backup and personal INI backup created before installation. The first
installation attempt stopped at its running-game guard, before replacing any file.
Do not describe V1.32 as installed until a later closed-game copy and destination
hash verification succeed. main remains 2bc3122c1ae07b7333798462f85c1d9fd3a739f7;
archive remains 10e0f220b85022bc9269e0cb9390a811a3b1cf89. All prior ZIPs retained.
