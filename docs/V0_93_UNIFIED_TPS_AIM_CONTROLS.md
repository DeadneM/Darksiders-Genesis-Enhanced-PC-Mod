# V0.93 | Unified TPS aiming and consolidated keyboard/controller UI (experimental)

2026-10-11. Based exclusively on last V0.92 development with user-validated V0.91 Horse Everywhere and V0.90 shader filtering. Work in `debug` only, keep `main` unchanged. The full V0.92 Windows test ZIP is separately preserved under `archive/downloads/`.

## User observations

1. Both shader aiming indicators remain hidden as intended with no collateral visuals. Frozen: `src/ShaderAimFeature.cpp`.
2. Horse Everywhere in banned regions works in the normal camera and TPS. Frozen: `src/HorseFeature.cpp`.
3. V0.92 mounted Camera Forward works noticeably better than the previous attempts. User requested adapting the exact forward-right-stick emulation to **on-foot TPS**, with an alternative **Vanilla Aim** mode. Mode 0 forward aiming on foot has NOT yet been gameplay validated.
4. UI had horse keyboard key far from the pad chord and TPS keyboard shortcuts far from pad equivalents. User asked to group by action.
5. In banned regions the initial summon command can first reset an existing/stale horse state, requiring a second physical press. Seen historically with both keyboard H and controller before custom pad binding. Root native horse toggle state not disassembled; guard any workaround.

## Horse page in Gameplay

- Move keyboard Horse key Rebind/Unbind into **Gameplay > Horse**, directly above two **individually rebindable simultaneous controller buttons**.
- One-line Horse Everywhere description; extra server/region/desummon metrics are in a collapsed "Horse Everywhere information / debug" tree.
- Never modify the validated HorseFeature native hook.
- V0.93 _tentative_ first-press recovery: after **1800ms**, if the real summon in a zone which triggered a region permission override produced **neither native server spawn nor a horse callback**, was NOT initiated while a live horse or TPS horse camera existed, and all physical summon buttons are released, emit precisely ONE native keyboard H press for 110ms. This specifically handles the stale first-toggle hypothesis. No repeated attempts, no retry on successful spawn / voluntary dismount, no retrigger while H rebound key is being held. A separate script prohibition can still prevent it. It is experimental, not guaranteed.

## TPS input modes

One UI selector "TPS aiming mode" controls both FOOT and HORSE, reusing existing `TPSMountedAimMode` config storage to preserve INI and rollback compatibility.

- **Camera Forward (mode 0):** during controller RT/RB game-native fire, feed `sThumbRX=0`, `sThumbRY=32767` to native game XInput. Camera uses unfiltered physical right stick and remains freely orbitable on foot AND horseback. On-foot behavior is experimental; mouse gameplay does not synthesize a nonexistent XInput device.
- **Vanilla Aim (mode 1):** while firing in TPS, game-native XInput right stick is left unmodified, game-native look getters pass through and independent camera orbit is suspended. At idle, the camera stays free and right stick native aim stays suppressed, both on foot and horseback. Mounted Vanilla Aim code is user-validated; this same-mode foot application is experimental. No native projectile, raycast, UObject transform or camera memory hooks added.
- Preserve the independent movement "Strafe while firing" setting. Hide the obsolete extra "Rotate on foot while firing" UI checkbox; its INI value is retained rather than silently resetting a user preference.

## Consolidated TPS shortcuts

- On the Camera page, put keyboard Rebind / Unbind **next to** the matching pad selector for: long-hold TPS on/off, short-recenter, held temporary native camera, hold-grenade/release-stow.
- Keep shared delay sliders inline under their action.
- Keep custom keyboard gear E fallback as an explicitly experimental old feature, unchanged.
- Horse keyboard key only in Gameplay Horse section, next to its two-button controller chord; not repeated in Camera.
- Existing `[Controls]`, `[TPSKeyboardHotkeys]` and `[Features] TPSMountedAimMode` values remain backward-compatible. No new files/config migration.

## Validation and boundaries

CI Windows x64 build, one cumulative ZIP with root `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`. Protect native mounted Vanilla Aim, native H/LB+RB summon, on-foot movement/strafe, R3 grenade, all V0.91 Horse Everywhere data and V0.90 shaders. Keep `main` unchanged.

User can verify on foot + horseback for both modes, regrouped UI and one real horse summon in a ban area. **No gameplay validation of V0.93 yet**. Rollback: archived original V0.92 or V0.91 ZIP.
