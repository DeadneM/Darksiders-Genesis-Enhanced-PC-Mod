# V1.116 - Shared native effect library

## Objective, base and scope

User asks for all other appearance families. Retained gameplay V1.112; cumulative source 2adb699d202d332d00a6fa5c9411c7805826ac1c / V1.115. V1.114 Geyser/Stoneskin and V1.115 Caltrops are still EXPERIMENTAL, not user accepted; V1.113 is rejected. Preserve accepted Rampage ending, three powers on War, status expiry, firing, camera and controls. No new gameplay/animation fixes in this build. Coop remains last.

Deliver a common menu for every remaining player-effect family in the bounded V1.112 inventory, plus typed core/Aether references missing from that first path filter. 43 independent groups, 451 exact ParticleSystem assets, 98 roles. All exported particle classes/name/package/PAK SHA1 checked individually from the main PAK, not inferred from filenames alone. Read-only PAK footer/index SHA1 must remain 5d0982cd46a5221290cb6ed24696e3962e9dddff. A one-look family gets Native/Hidden, not invented skins. Models, materials, armor unlocks and body coatings retain their existing paths; this is explicitly an emitted-particle library, not a replacement for every texture/model in the game.

Success requires later gameplay test: visible variant on a new effect, Native restoration on subsequent use, preserved damage/radius/core behavior, Hidden does not remove the native component or gameplay events. CI/fixtures are not in-game proof.

## Shared implementation and boundaries

EffectAppearanceFeature replaces CaltropsAppearanceFeature/Policy completely; no compatibility wrapper or second hook remains. Same exact factory RVA 0x187E600 and ABI already controlled in V1.115: template, UWorld, Outer, auto-destroy, pool enum, PSC result. Calls it once with the chosen template and otherwise identical arguments. The compiled table assigns each source asset exactly one role/family, avoiding two overlapping corrections. Same-phase role is preserved: e.g. World Ender scale slots remain paired, War Thunderclap/Dagon shared effects have a single selector, and mount phase/source origins are retained. Variant labels describe actual native graphics, not new gameplay upgrades.

All Native defaults use an atomic zero-mask fast path, without object discovery or loading. Explicit selection starts a bounded 512-object pass with 16-record table reads, cached native ParticleSystem class, one object header read, sorted ASCII case-insensitive name lookup and exact package verification. Case-insensitivity is necessary for the real NatureShot heal-aura export spelling. Weak identity/serials, no synchronous loading, no repeated full scan after completion; appended objects or expired caches/selection/world changes can resume discovery. Availability refresh is throttled to 200 ms and shared between groups. Source/target missing, unknown assets, expired identities, worker thread or stale/unpublished worlds retain original rendering.

Only newly emitted particles through this factory are replaced. Existing particles and pre-created SCS components outside this factory may retain native appearance. No speculative SetTemplate/combat hook was added to overcome that limitation. Material/skeletal graph changes are not smuggled into a graphics selector. Native/base/Molten Stoneskin coating from V1.114 remains unchanged and pending test; this new Stoneskin selector affects particles only. The separate Leviathan emitter and Stinger crystal correction are excluded from this library.

Hidden calls the native factory with the original template, preserving native component, return, pooling and lifetime, then SetVisibility(false, propagate=1) only on the returned owned ParticleSystemComponent with the expected template and live weak identity. It does not skip the callback, kill the emitter, remove damage or stop native events. Already-native-hidden components are not recorded/unhidden. Up to 128 records; if capacity is unavailable leave the native component visible, never lose a restoration record. Native/another choice restores the visibility actually changed within the game-thread service. Reused native pool components have that visibility restored before another option is applied. Expired identities are discarded. No direct actor, asset, ability or projectile memory writes.

The visibility ABI/signature 0x17B1840 and bVisible flag +0x1CC/0x20 already have native evidence and use in the retained Leviathan visual path. No new visibility hook is added. Original native flags other than visibility are untouched. Source templates are never edited and existing native data remains authoritative.

## Schema and preservation

43 independent native/default choices in one metadata schema drive store/reset/load/save/runtime and UI. Keep CaltropsAppearance and its values 0..4 exactly; append Hidden=5. Other 42 keys default Native=0. All 170 old INI key/value pairs, all Hotkey IDs/bindings remain, 212 total keys. Existing customized INI is preserved by updater; missing new keys behave natively. Mask/generation published from normalized choices, only selection changes reset discovery. Categories and root group collapsed by default; unique per-family ImGui IDs.

ForeignMontage, GeyserStaticPose, Stoneskin coating, ForeignWrath/Rampage/Caltrops/Beam/Power, PistolVisual, MeleeEffect, NativeTPS, HeavyLanding, Leviathan, horse, cosmetic unlocks and updater implementations must be unchanged. Main entry changes only appearance module include/init/service/shutdown. All 180 old archives and main/archive/v1.43 preserved. No local build/install/game launch/external process write.

## Controls and test matrix

64 GitHub Windows x64 suites remain required; the old Caltrops suite is replaced with a shared-library suite, not left as a dead wrapper. Exercise every source asset and supported preset, exact native ABI/return and one-call rule, source/package/object-table preservation, old Caltrops mapping, invalid values, missing/expired assets, other/stale world, off-thread, Hidden restoration, native hidden preservation, pool reuse, failed factory and shutdown. Configuration test persists all families together and checks old/default parity. These are meaningful safety/selection fixtures, not engine rendering certification.

Later user test in one session:
1. Tools: Caltrops Base/Frost, Tremor levels, Vorpal standard/imbued, Aether idles. Ghost Hook and Void Bomb offer Native/Hidden because no second equivalent palette is proved. Compare new effects and return Native.
2. Powers: Geyser far blades, World Ender Base/Frost and blast levels, Rampage Base/Chaos, Stoneskin particle themes, shadow aura and clone effects. Body materials and aiming/damage stay unchanged.
3. Strife ammo: Beam/impact, Charge, Graviton, Lava muzzle/other visibility, Nature, Static and standard impact. Check firing forward and both hands; choices must not modify trajectory, hit damage or core statuses.
4. War weapon effects: Earthsplitter, Flamebrand, Grasping Shadows, Thunderclap, Vampiric and Vortex. Visual levels must not grant the corresponding gameplay upgrade.
5. Mount/form palettes and core aura/blast families. Existing models and native core behavior must remain. Other-effect groups offer visibility only; no invented cross-attack skins.
6. Hidden must keep action/lifetime/controls, restore Native; repeat a cast to check pooled component visibility. Missing-asset warning means fallback, not validation. Coop last. Pending V1.114 tests stay pending.

Rollback: all selectors Native or preserved V1.115 ASI, personal INI retained. Next work follows the user's visual report; persistent/component/material overrides need their own evidence rather than broadening the hook blindly.
