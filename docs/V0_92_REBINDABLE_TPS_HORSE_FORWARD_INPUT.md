# V0.92 | Rebindable Horse Everywhere, TPS keyboard shortcuts, and experimental mounted Camera Forward input

2026-10-11, development branch `debug`. Baseline: user-validated **V0.91**.
Protected baseline is safely copied to `archive/downloads/DarksidersGenesis_V0.91_NATIVE_CAMERA_HORSE_RETENTION_TEST.zip`.

## User validation (V0.91 + log 20261011-191440)

- **Horse Everywhere is VALIDATED** in both normal and TPS camera, including previously banned zones. No more experimental region logic changes in V0.92.
- **Both aim indicator shader draw suppressions VALIDATED** from V0.90; preserve `src/ShaderAimFeature.cpp` without edits.
- **Camera Forward mounted remains broken** before this change. No native projectile raycast hook is known. Mode 1 `Vanilla Aim` and the validated R3 grenade handling are frozen.
- Observed native camera-independent horse lease arms and continuous original region gate bypasses, including 1136 native camera extensions. Log total: allowed region bypass 4442, desummon bypass 1682, PS matched 2, suppressed draw calls 4192. Game activity verified, user confirms visually.

## Horse summon & dismount bindings

- Two independent **physical XInput button fields**, `[Controls] HorsePadButtonA` and `HorsePadButtonB`. Defaults: `256` (LB) and `512` (RB), matching old original simultaneously held pair.
- Input only triggers when **BOTH DIFFERENT buttons are held**. Remapping takes effect in the native XInput hook also outside TPS. It forwards the original native LB+RB chord, rather than touching the validated Horse Everywhere gates or creating a horse actor manually.
- Keyboard `[TPSKeyboardHotkeys] Horse=H` defaults to the native H key. A non-H shortcut emits the original native H key using a regular focused Windows keyboard input (down, then up). Existing native game H is not blocked by the mod.

## Independent keyboard commands in the Camera overlay

Key tokens saved in `[TPSKeyboardHotkeys]`:
- `Horse=H`: summon/dismount, game-native original H remains available.
- `ToggleTPS=None`: press-and-HOLD using the same configurable 500ms threshold as the controller L3 long press.
- `RecenterTPS=None`: short press recenter. If bound to the same key as ToggleTPS, a **short release** recenters and a **long hold** toggles TPS, just like existing L3 semantics.
- `TemporaryView=None`: hold TPS temporary native view with the existing delay threshold; release returns to TPS. Can share a binding with grenade shortcut, mirroring R3 logic.
- `Grenade=None`: if an XInput pad is connected, add keyboard trigger to the already validated game-facing R3 hold/100ms gap/200ms second-pulse behavior; without a controller, use original native keyboard E (Use Gear) as an **experimental** equivalent. This fallback may differ from native R3 for particular gear pickups and should not be considered validated.

OFF defaults ensure **no collision with existing user keyboard binds**. Camera/other F1-F12 bindings remain unchanged. Every key has Rebind/Unbind in the overlay and survives INI Save/Load. Press Escape to cancel key capture. Physical XInput mappings remain available independently.

## Camera Forward targeted experiment

Only in TPS **mounted camera**, configured `TPSMountedAimMode=0`, RT/RB held, while game focused and ability wheel not active, the game's native XInput sees a synthetic straight-UP right-stick vector: `sThumbRX=0,sThumbRY=32767`. The independent camera reads the physical stick via the unfiltered GetState trampoline and continues orbiting even while firing. This is an INPUT-only experiment and may fail if the mounted projectile uses a separate cached direction, native camera rotation or raycast path. Native mounted `Vanilla Aim` (mode 1), all on-foot input, real mouse and other controls remain unchanged. No artificial Ctrl/window key injection or UObject rotation patches.

## Status and rollback

One cumulative V0.92 ZIP, exact supported EXE validation, 4 files at ZIP root, `main` untouched. This code is **experimental until in-game validation**, especially the rebinds, no-pad keyboard gear fallback and Camera Forward. Do not claim Camera Forward repaired solely because XInput injects successfully. Restore the known-good V0.91 copy from `archive/downloads` if any unexpected gameplay regression.

## Roadmap

Find the true mounted projectile direction/raycast hook only if the input-level forward-stick fallback does not achieve front-facing shots; do not modify the existing validated native aiming behavior.
