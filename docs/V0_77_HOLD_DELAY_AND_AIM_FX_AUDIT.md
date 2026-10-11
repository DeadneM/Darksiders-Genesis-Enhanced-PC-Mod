# V0.77: R3 delayed vanilla view, user INI defaults, and aiming-visual audit
Date: 11 October 2026. Experimental TEST build based on validated **V0.76**.

## Locked V0.76 baseline
The user confirms that R3 hold view, L3 short/long, grenade hold, TPS movement/aim and mounted controls all work and describes this as the most advanced build. Preserve V0.76 full ZIP and code unchanged in prior branch. Do NOT change native V0.75 look-input/rotation hooks, XInput isolation, horse movement/sprint, F1-F12, HUD/reticle, graphics adapter, damage, or native game animation to experiment with aim FX. Only add independently scoped functionality. Any change to the locked functionality must be disclosed before it is attempted.

## Imported values from user's actual DarksidersGenesisMod(2).ini
The supplied Oct 11 file is now the reference for **new-install defaults and Reset Defaults**, while existing INI persisted values are never overwritten by migration:
- `[Features] ThirdPerson=1`, `TPSControllerStrafeWhileFiring=1`, `TPSControllerRotateWhileFiring=1`; hold-view, long-toggle, grenade hold, mounted camera fix remain ON; `TPSHideGroundAimFx=1` remains experimental ON.
- `[Values] TPSFOVDegrees=110`, `ThirdPersonDistanceMultiplier=.200`, `ThirdPersonPitchDegrees=-12`, `ThirdPersonHeightOffset=120`, `TPSFootAnchorOffset=0`, `CameraYawDegrees=0`, `CameraHeightOffset=0`, `CameraMouseSensitivity=.120`, `CameraStickSpeed=180`.
- L3 long-press duration `TPSToggleHoldMs=500`, independent of R3 view delay.
- Preserve all other stock features and user custom settings.
- `[Controls] TPSSuspendDelayMs=300` NEW, 0-3000ms in overlay. The delay begins only after uninterrupted physical button hold with a focused game. Releasing a short press before threshold leaves TPS untouched. Releasing after the delay immediately restores TPS, no release delay or changes to R3 grenade virtual pulses.

## Aim-visual diagnosis
The user reports **Hide ground targeting effect in TPS does not change their visible aiming effect**. The V0.76 10:41:31 shutdown reports `groundAimHidden=0` despite native hook installation `RVA 0x6DDAD0`. The exact shipping EXE (file SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`) contains `HideGroundTargetingEffect` alongside `AMayhemWarPlayerCharacterProxy` naming, while separate player aim/visual class names include `UMayhemPlayerAimComponent`, `UMayhemPlayerAimAbility`, `UMayhemPlayerTargetingAbility`, `UMayhemTargetingUIComponent`, `UMayhemPlayerDualPistolAimAbility`. This suggests the old hook does not own Strife's normal weapon aiming UI. This is a **candidate-path inference from binary strings**, not a verified callgraph or automatic repair.
The prior V0.76 code also **saved then deleted `TPSHideGroundAimFx`** due to an obsolete leftover migration line. V0.77 removes this deletion so the option finally persists in the INI. This fixes INI persistence, **not proven visibility**.
The existing hook now separately logs:
- `groundFxCalls`: all native `HideGroundTargetingEffect` interceptions
- `alreadyHidden`: calls where native already asks hide=true
- `forcedHidden`: existing suppression n during TPS
This distinguishes a never-executed method from a method called only with hide=true. No other render or actor hooks were added because they could regress the user-confirmed gameplay and no exact emitting callback has been established. To fully remove Strife's visible aim element, locate and validate the corresponding aim-component UI/FX callback using game binary analysis and in-game evidence (visual screenshot or native call log).

## Acceptance tests
Validate a brief R3 press (~100ms) only equips the grenade without a flash to vanilla camera. R3 held >300ms triggers vanilla view after delay, and release returns directly to TPS; R3 grenade still stows as before. 0ms and 3000ms work, live slider persists after restart; short L3 recenter and 500ms long-toggle stay unchanged. Inspect log for delayed-view timestamps and groundFxCalls counters, test normal combat with both characters. Keep prior V0.76 ZIP available for rollback.

## Packaging
Windows x64 ZIP must contain exactly four root files `dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`. Do not publish as official stable release until in-game validation. GitHub branch `dev/v0.77-r3-delay-aim-fx-audit`.
