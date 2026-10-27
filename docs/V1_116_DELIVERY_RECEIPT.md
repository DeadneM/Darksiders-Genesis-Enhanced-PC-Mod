# V1.116 - Verified cumulative debug delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Retained gameplay V1.112; pending V1.114/115 carried unchanged, not retroactively accepted. V1.113 rejected. User requests all remaining visual families. Coop last. No local compile/install/launch or external process memory writes.

Source `1c1f7191c022ba1dc646b4e982bd451924c84ed4`, archive `22f011465c0b1eb69bf068a55a4b0fdb3e66e341`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40473336773): **64/64 suites**, zero MSVC warnings/errors. Shared-library fixture covers 43 families/451 source assets/presets, native ABI/result/one-call forwarding, old Caltrops values, missing/expired targets, world/thread safety, source preservation, visibility/Native restoration, native-hidden preservation, failure and native pool reuse. Configuration persistence checks all groups and compiled/shipped defaults. No engine visual/gameplay certification.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/22f011465c0b1eb69bf068a55a4b0fdb3e66e341/downloads/DarksidersGenesis_V1.116_NATIVE_EFFECT_LIBRARY_TEST.zip), 2310089 bytes, SHA256 `0904EA1464913702DE743C42F36BA7121591E087B3941C4928227E34081E9FA6`. Four root files, NoCompression, CRC valid, ASI/DXGI AMD64, INI/README matched compiled source. ASI SHA256 `68D886BD6ADBDF6A359BF3F73E1346477147E70A2A3A27A13330B86D362B0328`. All 170 previous INI values retained, 42 new Native keys (212 total), all 55 Hotkey IDs unchanged. 180 old ZIP Git blobs preserved (181 total). main/archive/v1.43 unchanged. Missing local V1.108 quarantine not restored or committed as a deletion; unrelated audit unchanged.

Debug updater selects V1.116 blob `b1be143f63594e0574b5be46111b5e280dc7f51b`; immutable and updater download bytes HTTP 200 and identical SHA256. No game updater UI interaction claimed. Defender ZIP and extracted four-file scans with DisableRemediation: exit 0/no threats, realtime/on-access/antivirus active, no security/exclusion changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.216.0; diagnostic result, not universal certification.

## Included and exact scope

One renderer replaces the Caltrops prototype completely. 43 independent selectors, 451 main-PAK ParticleSystem exports, 98 same-phase roles: tools, powers, Strife ammo, War weapon effects, mounts/forms and core auras/blasts. [Exact available choices](V1_116_EFFECT_LIBRARY_CATALOG.md). One-look families get Native/Hidden. Skin availability/model controls already retained remain separate; this release does not replace arbitrary textures or bodily materials.

Same existing native visual factory hook, one native call and unchanged world/Outer/auto/pooling/return. Loaded assets only, no synchronous loads/startup scan on Native, batched object discovery/class cache/sorted case-insensitive lookup. Unknown/missing/expired targets or other/stale worlds/worker threads retain original effects. Native defaults, unique collapsed UI groups, Caltrops 0..4 preserved.

Hidden keeps the original native component and template and changes only rendering visibility through the existing supported native API; limited weak-identity restoration records, refusal if capacity unavailable, restoration on Native/reuse/shutdown. Native component events/lifecycle routines are still called; their exact in-game timing has not been validated. No actor/control/damage/projectile/core/status mutation. Existing/direct SCS particles outside the factory can stay unchanged; already-running colored emitters finish normally. Stoneskin selector is particles only; coating/Leviathan/Stinger/armor/pistols/arms/horse models remain under retained paths.

Accepted camera/XInput/dash/aiming, Rampage ending/three War transfers, Geyser/Stoneskin candidate code, sound, melee, holsters and updater implementation unchanged. Two static reviews agree on those boundaries. Pending Geyser jumping/stationary tail and Stoneskin gesture/coating tests remain pending. No broadening of recovery or new experimental combat hook.

## Later user tests

1. Tools/powers: compare Native and variants on a new cast/explosion, then Native again. Missing-asset warning means fallback. Ghost Hook/Void Bomb and other one-look rows offer visibility only.
2. Strife ammunition/War weapons: visual palette changes must preserve muzzle origin, aim, trajectories, damage and equipped core behavior. Frost/Molten graphics must not grant those statuses.
3. Mounts/forms/core effects: palettes change particles, not models or stats. Existing model/armor controls and transformation controls remain.
4. Hidden: action continues, next use/pool reuse and return Native restore expected rendering. Direct/pre-created particles may remain visible and need separate evidence. Coop last.

Rollback: selectors Native or preserved V1.115, keep personal INI. [Full contract](V1_116_NATIVE_EFFECT_LIBRARY_TEST.md). Next follows the user's grouped visual report; unproven persistent material/component changes remain out of this build.

Evidence: [CI](evidence/V1_116_CI_CHECK.json), [ZIP](evidence/V1_116_PACKAGE_CHECK.json), [updater](evidence/V1_116_UPDATER_CHECK.json), [Defender](evidence/V1_116_DEFENDER_CHECK.json), [reviews](evidence/V1_116_STATIC_REVIEW.json), [full native library](evidence/V1_116_EFFECT_LIBRARY.json).
