# V1.03 Experimental | Native gauge charge hooks and camera/input cleanup

Date 2026-10-12. Work only on `debug`, `main` untouched. Based on V1.02 and user-validated V0.98 wheel/R3 baseline. Verified target executable provided by user SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`, size 62,113,280 bytes. All gauge hooks are **experimental, OFF by default**, signature-guarded, no guarantees until user gameplay test.

## Static reverse engineering with exact shipping PE

The native UE4 exported UFunction name tables and wrappers, not guessed offsets, led to three separate C++ functions. Identifiers and call locations verified by disassembly of user-provided .exe:

- **Wrath / AddWrath:** UFunction name `AddWrath` at file offset 0x27C1BA8; wrapper RVA 0x7ABD90 loads float arg and calls native **RVA 0x660570**, first bytes `80 B9 18 01 00 00 03`. Native checks actor type field `+0x118==3`, loads `actor+0xEA0` and adds native Wrath at component `+0x3A0`, then dispatches setter via vtable. Hook typed `void(actor,float)`, only local actor positive gain.
- **Ultimate Form / AddChaos:** UFunction name `AddChaos` in same gameplay family, wrapper RVA 0x76FC60 invokes native **RVA 0x62E060**, first bytes `40 53 48 83 EC 30`. Native reads `this+0xE8` owner and forwards a positive float amount through player/Chaos component; hook typed `float(component,float)`, only when component owner matches live local player.
- **Synergy / SetCurrentSynergy:** UFunction registration `SetCurrentSynergy`, wrapper RVA 0x7C5240 invokes native **RVA 0x583040**, signature prefix `40 53 48 83 EC 30 F3 0F 10 81`. The setter accepts **absolute float**. A separate native `GetCurrentSynergy` at RVA 0x56D430 reads float at `this+0x5A4`, and `GetMaxSynergy` at RVA 0x56FEC0 reads `this+0x498`. Hook only multiplies positive delta between requested value and current; never multiplies consumption or resets, and bounds to max. Check if player identity matches; may need owner resolution if no local player events occur.
- `AddChaos` corresponds to in-game "Ultimate Form / Chaos" resource, not to Hotstreak. `AddWrath` corresponds to Wrath, and the Synergy setter is a separate resource. The read-only strings are not by themselves proof the game actually uses these paths during every gain. The new test log reports hook READY and observed/boosted calls, so we can verify at runtime.

All three hooks use MinHook and byte/signature checks; no patch on mismatching executable. Runtime `wrathChargeEnabled`, `ultimateChargeEnabled`, `synergyChargeEnabled` default FALSE. Multipliers default 2.0, limits 1..25. UI offers independent checkbox, slider, typed numeric value, Default. Observe `Gauge V1.03 ... READY` and `Gauge V1.03 totals observed...` in logs.

## Camera/UI/input cleanup

- Separate camera values Default actions and overlay buttons; both actions bindable to F1-F12. Do not change TPS enable state when resetting TPS pose.
- Non-TPS camera yaw/height/zoom/FOV no longer applied inside attached TPS; when TPS active vanilla camera UI and camera keyboard steps are disabled, but settings remain persisted for next TPS OFF.
- TPS Follow Player forcibly true, checkbox and old INI key removed. Foot origin correction moved alongside TPS pose controls. Camera stick selector `0 OFF, 1 LEFT, 2 RIGHT` defaults right; left stick shares physical movement input and requires explicit test for collisions; old Boolean `CameraControllerOrbit` retained only for migration.
- Keyboard wheel detectors only G (equipment) and Tab (ammo/abilities); native E is USE equipment, R3 is physical USE equipment and retains the proven V0.84 continuous press/UP gap/secondary stow pulse. LB remains controller wheel. Retired E, LAlt, R3 wheel classification.
- Each user binding has its own Default: menu key, camera keyboard, TPS keyboard, horse keyboard, horse 2-button chord, TPS controller masks, F1-F12 action assignment; intentionally shared Z/Z default remains allowed.
- Three charge native hooks integrated to `GaugeChargeFeature.cpp`, separate from Hotstreak `AddJuice`, horse and shader code.

## Required manual in-game test

1. Check exact startup pair `1.03-camera-input-cleanup-native-gauge-hooks-test` + V1.02 launcher SkipWarning, and 3 `Gauge V1.03 ... READY` lines. If any not READY, do not enable that gauge.
2. Individually enable WRATH x5, gain wrath with War or Strife, see faster refill and `Gauge V1.03 Wrath LOCAL gain...`. Restore OFF. Repeat Chaos x5 during combat and Synergy x5 in appropriate co-op context. Check log counters on shutdown; zero observed = wrong entry point or game path not activated, zero boosted but observed >0 = owner mismatch/negative gains. Do not equate "READY" with functional gameplay.
3. Hold R3 and short-tap R3; verify usable equipment and the validated delayed stow pulse; R3 should NEVER be logged as Wheel. LB wheel, keyboard G and Tab selection, press native E USE.
4. TPS camera Right stick default, OFF mode, Left mode with moving character, RT/RB shoot, horse mount/dismount, cutscene and loading; verify normal camera sliders don't alter TPS and vice versa.
5. Reset each camera set using buttons and F1-F12 assigned actions; verify native height 50, TPS distance .15, height 0, origin 0, and preserved unrelated values; verify independent keybind Default and INI persistence.
6. Confirm previous shader/horse/alt-tab/reticle/skip-intro functions unchanged. Skip Warning V1.02 reports `skipped=1` but its visible effect remains unconfirmed and is NOT claimed fixed here. Cutscene detection remains heuristic; this build primarily targets input and charge hooks.

Build workflow `.github/workflows/build-v090-debug.yml` packs EXACTLY four files at root of `downloads/DarksidersGenesis_V1.03_CAMERA_INPUT_NATIVE_GAUGES_TEST.zip` (dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md). No zip is valid until GitHub Actions build and archive publication both succeed.
