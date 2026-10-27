# V1.117 - Verified cumulative debug delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Cumulative V1.116, retained V1.112 gameplay and later accepted paths. Latest supplied video explicitly validates Strife Rampage; preserved exactly. P2 stays last. No local compilation/installation, game launch or external process writes.

Source `abbae7a03656ec2d7b608a50a264381bfde2cf69`, archive `d5df57ef5cff693e8496e81bcb080baf4faefd3f`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40494010438): **65/65 suites**, zero MSVC warnings/errors. One preparatory run was superseded before packaging after native disassembly proved dynamic Caltrops montages have zero constructor flags: private ownership is checked by the exact engine transient Outer, not an assumed RF_Transient flag. Final run includes that correction. Tests and static native evidence are not gameplay proof.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d5df57ef5cff693e8496e81bcb080baf4faefd3f/downloads/DarksidersGenesis_V1.117_CAST_RETURN_MINERAL_CLOTH_MENU_TEST.zip), 2320237 bytes, SHA256 `3C8EF5CFBC06D4E651C787F4E795186CF214201529C05B60872BDA3D01A216A4`. Four root files, NoCompression, CRC valid, ASI/DXGI AMD64, INI/README matched compiled source. ASI SHA256 `931670DD564F2058711A583E1F80C64FB827CE31505AD3B33A065887AD23E8E1`. All 212 INI keys/values and 55 Hotkey IDs retained, no new config key. All 181 old ZIP blobs preserved (182 total). main/archive/v1.43 intact; unrelated local V1.108 quarantine deletion and untracked audit never staged/restored.

Updater debug selects V1.117 blob `0596013dc83386ea5591b87cc6b67e5219d26fef`; immutable and updater download HTTP 200 with identical SHA256. No game updater UI interaction claimed. Defender ZIP and extracted four-file scans DisableRemediation: exit0/no threats, realtime/on-access/antivirus active before/after, no protection/exclusion changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.224.0. Diagnostic scan, not universal certification.

## Changes and preserved behavior

- Direct Appearance tab between Gameplay and Camera, library first. Open Effect appearances (experimental), then a category. Existing 43 selectors / 451 native particles / 98 roles preserved, same exact UI config references, values and renderer. User could not find any choices in V1.116; variant behavior is still unvalidated. Groups collapsed by default. [Choice catalog](V1_116_EFFECT_LIBRARY_CATALOG.md).
- Geyser stationary return: V1.116 log proved curve-bearing-context refusal. Use the already accepted native pose/curve context allocator and pure sampler, with alias rejection. Only rendered bones1..79 blend into Strife channel return; root, live curves, moving/jumping and ability/movement memory retained. Return starts0.85s, spans0.50s, blends0.12s. Runtime still experimental.
- Stoneskin Strife: retained gesture at1.0s instead of0.75s. Private mineral body/gear MIDs suppress two native emissive scalar values; native cloth shader keeps its fire. Shared source materials stay byte-identical. Native status/cost/sounds/expiry restoration retained. Strife UV/art fit still needs the user's visual check.
- War Clone/Caltrops: append verified native War_Channel_End once to the privately owned War_Channel_Start montage, with0.55s/0.30s return. Constant authored root track; two End footsteps, no additional ability callback. Existing first segment/callbacks/sounds/sections unchanged. Missing/expired End retains previous valid gesture. Same bounded shared asset discovery; no loader/hook/startup-load addition.
- ForeignRampageFeature, RampageMotion, WorldEnder/Retarget, Geyser native builder, sound/cancel helpers, camera/XInput/aiming/dash/pistol arms/melee/Leviathan/horse/cosmetics/updater unchanged. Two reviews agree on these boundaries. The new return touches Clone only in Compatible; native War Stoneskin/Stinger unaffected.

## Grouped user check

1. Appearance > Effect appearances > Tools or Powers: choices visible, try one available variant on a new cast, then Native. Loaded-asset warning means fallback; direct/SCS components may stay native.
2. Strife Geyser standing: arms lower with spikes. Repeat while moving, then jump immediately and several seconds later.
3. Strife Stoneskin: complete short activation, mineral body and mainly fiery scarf, no sliding, previous skin returns after expiry.
4. War Caltrops: no end hop, effects/spawn/sound intact. War Clone: longer complete gesture, clone/control return intact.
5. Rampage regression only; user-approved behavior must remain. Other library families can be tried in the same session. P2 deferred.

Rollback: keep personal INI; use preserved V1.116 or accepted V1.112 ASI if needed. Native for library choices. Next work follows this grouped report, not unverified broad recovery/lock changes. [Full implementation/evidence contract](V1_117_CAST_RETURN_MINERAL_CLOTH_MENU_TEST.md).

Evidence: [CI](evidence/V1_117_CI_CHECK.json), [ZIP](evidence/V1_117_PACKAGE_CHECK.json), [updater](evidence/V1_117_UPDATER_CHECK.json), [Defender](evidence/V1_117_DEFENDER_CHECK.json), [two reviews](evidence/V1_117_STATIC_REVIEW.json), [native contracts](evidence/V1_117_NATIVE_CONTRACTS.json). Video stays local, only bounded observations/hash recorded.
