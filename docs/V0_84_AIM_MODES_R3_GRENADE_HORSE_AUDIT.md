# V0.84 | Three aim-indicator modes in HUD, native R3 hold repair, horse summon diagnosis

Date: 11 October 2026. Based on V0.83 branch while preserving validated player movement, TPS native aim, controller strafe/rotate, mount movement/stamina, startup features and regular HUD.

## User feedback from V0.83
- User sees the cyan line and semicircle despite native Hide HUD. They request a three-way control in HUD: **Hide Everywhere**, **Default Only** (normal camera only), **TPS Only** (default).
- **Horse Everywhere does not remove area restrictions**. The supplied V0.83 log confirms the signature-guarded server-spawn probe was installed but saw `serverSpawn=0`, `duringRequested=0`; therefore simply hooking native server spawn was the wrong place to lift a forbidden-zone check. The V0.83 checkbox was and remains **research-only**, not a functional bypass. Don't misstate otherwise.
- **Long R3 hold grenade broken** although the previous log detected `grenadeDown=2` and `grenadeUp=2`; those counters represent physical transitions, NOT confirmed game equip/unequip.

## HUD three-way selector
- New persisted INI `[Features] AimIndicatorHideMode=2`, with enumerated values `0 Hide Everywhere`, `1 Default Only`, `2 TPS Only`. Defaults to TPS Only on fresh installation, store reset and absent key in upgraded INIs. ConfigRevision 2125. Existing `ThirdPerson=0` remains unchanged, existing user config respected.
- Selector is in **Gameplay > HUD and aiming effects**, beside HUD Hidden and Hide Reticle, not in Camera. Prior experimental `TPSHideGroundAimFx` checkbox removed from the Camera tab, its legacy key preserved for config compatibility.
- Native existing `HideGroundTargetingEffect` detour now applies the selected mode to the scope guard, but that function has been **called 0 times** in all relevant Strife user tests. The actual turquoise line and semicircle **are not yet demonstrably hidden** because their actual world-space shader/component remains unidentified. The overlay explicitly states that and shows actual ground-FX call count. This is a correctly wired scope setting for the existing candidate hook, NOT a falsely claimed visual cure.

## R3 grenade native XInput repair
- Previous algorithm sent only a 65ms virtual DOWN pulse even while physical R3 stayed held, then a 80ms second pulse when released. Depending on frame scheduling and input polling, the engine could miss these edges or detect artificial UP during a long hold.
- New algorithm forwards R3 DOWN **continuously for as long as the user physically holds R3**. Upon release, forward UP for 100ms, then generate one fresh game-facing 200ms DOWN pulse to reproduce the native toggle for stowing. Forward all other controller bits untouched. No changes to TPS R3 hold-to-vanilla, L3 long toggle/recenter, firing/aim or camera.
- Diagnostics now record physical R3 DOWN/UP events and whether the second pulse was actually delivered by the hook to native XInput, not claiming the game necessarily acted on it. Compare sustained hold and equipment, then release and stow.

## Horse Everywhere: still not working
The current hook in `HorseFeature.cpp` is only a passive signature-guarded native server request detector. 0 server calls means the denied request may be blocked **upstream** in the player Blueprint/ability or other logic, OR no summon request was made during the observed session. The true restriction path is still unproven. There is **no safe horse spawning bypass in V0.84**. Leave the checkbox clearly labeled `experimental research`. Future action: identify and hook the exact native/BP area-permission gate or obtain cooked asset listing. Do NOT force `SERVER_SpawnHorseMount` with guessed params and break mission scripting.

## Test priorities
1. TPS ON (F5 or long L3). Press and hold R3 >1s. Observe grenade equipped; release and verify stowed, without stuck button. Test several times and inspect `TPS V0.84 GRENADE` and summary.
2. Gameplay HUD and aiming effects selector persists via INI. Default 2. Switching mode 0/1/2 should not hide unrelated HUD or alter camera. The 3D blue indicators remain an unverified visual target.
3. Horse allowed/forbidden summon while Horse Everywhere research ON, note whether `serverSpawn` changes. Even if yes, no area bypass is implemented.
4. Any crash or input loss: roll back to V0.78 stable or V0.83 for overlay comparison.
No official stable release until user validation. Main untouched.
