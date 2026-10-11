# V0.94: native radial menus, TPS input consolidation and user's INI defaults

11 October 2026. Experimental development build based on V0.93. **Main stays untouched**; previous V0.93 ZIP frozen under `archive/downloads`.

## What the user submitted

- INI `DarksidersGenesisMod(4).ini`, from their V0.93 gameplay session: TPS enabled, `ThirdPersonDistanceMultiplier=0.150`, `ThirdPersonPitchDegrees=0`, `ThirdPersonHeightOffset=100`, FOV 110, mouse sensitivity 0.120, stick 180, all earlier boosts and HUD/intro configuration intact. Keyboard Horse=H, ToggleTPS=V, RecenterTPS=V, TemporaryView=E, Grenade=E; controller LB+RB horse, R3 grenade/temporary view, L3 toggle/recenter. All relevant settings adopted as **V0.94 default values**, except obsolete keys explicitly removed.
- User reports opening equipment/ammo/ability radial menus in TPS reverts camera to native, and right stick stops selecting the equipment wheel; changing Block Native Right Stick and Strafe While Firing OFF breaks TPS. Keyboard grenade duplicates the game's native E. Horse Everywhere still needs two physical H presses in banned regions.
- Log: `TPS V0.77: temporary vanilla-view ON` and OFF occurred 20 times (R3/E collision possible); `keyboardHorse=11` synthetic H injection attempts after rebinding horse to X then H; `mappedHorseChord=0`, meaning default physical LB+RB was passed native, **NOT** evidence that the mod injected two pad summon events. Native `SERVER_SpawnHorseMount` appeared later than keyboard presses in a banned region; before second/third presses, native `IsInHorseRegion` and `IsInHorseDesummonRegion` were being overridden, but no spawn had occurred yet. This supports a scheduling or in-game state conflict; it does not prove the original H input alone is the cause.
- Log confirms user-validated horse retention `leases=7`, two shader IDs `matchedPS=2`, `blockedDraws=22952`, and 5,147 forward stick emulations. Both V0.91 HorseFeature.cpp and V0.90 ShaderAimFeature.cpp **must remain unchanged**.

## Architecture: wheel priority over TPS combat

Input code uses original game hardware XInput for **LB native radial menu**, and native keyboard **E (gear)**, **G (gear selection)**, **Tab (ammo/enhancements)**, **Left Alt (abilities)**. When a native wheel key is held during TPS:
1. **Never suspend the TPS view** due to R3 or the keyboard E also being mapped as temporary camera.
2. **Let the game's original right-stick input through**, as before in the XInput hook.
3. **New:** let the original player look-input AND look-rotation getters return untouched while the wheel is active. Before V0.94 those hooks could zero the same direction used for native wheel selection after XInput had passed it through.
4. **Pause mod camera orbit from LB** (existing behavior), so the right stick selects items, not orbit the third-person camera. Keep the TPS camera positioned rather than switching to native camera.
5. A short bounded wheel timestamp handles order differences between native game input polling and Present.

Scope is gameplay/window-focused only. A real game UI wheel may use additional input/state paths not instrumented yet; wheel correctness is subject to user visual test.

## TPS mode consolidation and removal of redundant parameters

The only user-visible aim selector is `[Features] TPSMountedAimMode` for compatibility: 0 Camera Forward (native up/right-stick simulation while firing; camera keeps physical stick), 1 Vanilla Aim (native aim during fire; independent mod camera paused until fire stops). Both modes apply to FOOT and HORSE. **HorseCharacter data lifetime/mount retention is not copied into foot scope.**

`TPSSuppressNativeRightStick`, `TPSControllerStrafeWhileFiring`, `TPSControllerRotateWhileFiring` are retired as configuration and runtime fields. They were redundant or confusing: native right stick isolation and camera-vector look during fire are fixed internal TPS rules, using the selected mode and wheel priority. In particular, the old "Strafe" checkbox only gated a native look vector, not real left-stick strafe. Player movement inputs remain native. These keys are purged on next INI save/ConfigRevision 2128 migration without altering unrelated settings.

Keyboard `[TPSKeyboardHotkeys] Grenade` is removed from UI, INI, and input code. The game already uses `E` for gear. Native controller R3 hold / release + native second stow pulse is preserved unchanged, except it is bypassed in the LB radial wheel to avoid consuming right-stick selection.

## Banned-area keyboard horse first press

The V0.93 speculative 1.8-second second H emulation is removed to prevent inadvertent second toggle / dismount. Instead, native `WM_KEYDOWN(H)` in the game's hooked WndProc marks the exact known local horse summon intention **before** passing H to the game's input handler. Remapped keyboard horse key still resolves to one native H down/up; that synthetic H also follows the early-arm WndProc path. On controller, the selected two-button simultaneous chord is marked early in the XInput GetState hook **before** handing the native LB+RB state to the game. No extra summon is fabricated for the default physical LB+RB. Same V0.91 region hooks remain untouched.

This is a timing hypothesis to test, not a proven one-press solution. Observe `Horse Everywhere V0.94: native WM_KEYDOWN H pre-armed...` in the next log when testing; if absent, input arrives via another native route.

## Defaults from provided INI

Authoritative `config/DarksidersGenesisMod.ini` now contains the user's exact behavior and numeric preferences including TPS=1, FOV 110, camera distance 0.15, height 100, V short/long TPS, H horse and E temporary camera, with legacy fields removed. `ConfigStore` ResetDefaults/Load fallback and `RuntimeSettings` default align. **E is both temporary view and native Use Gear** in the supplied INI; when opening its native wheel, the wheel wins and temporary view is intentionally suppressed. To use temporary view independently, bind it to a key that is not already native wheel or gear.

## Verification & release

- CI Windows x64 Release, ZIP entries at the root: `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`.
- The V0.93 test ZIP is preserved in `archive`; previous V0.92 and V0.91 also remain.
- Do not claim the new wheel/horses keyboard behavior is validated until tested in-game.
