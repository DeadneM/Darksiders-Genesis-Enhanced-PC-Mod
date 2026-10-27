# V1.123 - Stoneskin transitions and native Caltrops foot stomp

## Base and objective

Base V1.122 `6979c8dd6e193dda2409dcb9430318fbf2e23588`. User explicitly validates Geyser and says not to touch it; validates Stoneskin gesture, materials and independent buff duration. Requested additions: explosion at stone/fire appearance, ashes at complete status removal. War Caltrops still hops; user proposes War's native Stoneskin stomp with caltrops at foot contact.

Success: unchanged accepted Strife gesture/materials/damage/status lifetime, one matching launch explosion, one ash effect on full expiry; War Caltrops has one smooth stomp and one native burst at its foot-contact cue, no borrowed coating/Stinger/blast, normal movement/jump afterwards. All new runtime outcomes remain EXPERIMENTAL pending user test. No local build, installation or game launch.

## Read-only evidence and limited authorization

Live PID38944 running V1.122: reflection confirms GameplayStatics native/static SpawnEmitterAttached flags0x4822401, ParmsSize72 at UFunction+0x9E, exact ten named parameters. Boolean AutoDestroy is full byte at61. Existing material dispatcher ProcessEvent reused; no particle constructor address. Class held weakly; derive and verify its strongly-owned Default__GameplayStatics CDO each call rather than allocating a weak serial for a template. Local stack arguments only; own authoritative foot Strife mesh/controller, exact asset packages, property types/sizes/offsets validated. API return component must be non-null for success. [Review and explicit permission](V1_123_STONESKIN_FX_REVIEW.md).

The earlier internal AnimNotify constructor/direct-native-notify proposal and initial public API proposal were refused by automatic review. Neither was applied or executed. User then explicitly authorized the bounded public-API visual trial. That accepted method is the only new Stoneskin emission path. External reader made no native call and no memory write. No save/PAK change.

Native status Blueprint casts recipient to War before ash generation, explaining why transferred Strife has none. Its OnRemove attaches P_War_StoneSkin_Base_Crumble_Expiration to own mesh, socketNone, relative Z180, rotation0, scale1, KeepRelativeOffset0, AutoDestroytrue, Pool0. Launch uses P_WarStoneSkin_Base/Molten at relative0. Launch occurs once after an accepted cast and actual MID coating; recast supported without material recreation. Expiry is a known no-status edge of the same weak actor/world/mesh/asset. No ash on transformation, world/owner loss, unknown read or shutdown, nor Molten-to-base transition. Native costs/stats/status duration unchanged. Deferred discovery may delay a launch if assets are not yet observed; no fabricated successful rendering.

Old launch copier read Attached at padding0x8C; actual property is bit0 at0x80, SocketName0x84. Captured padding was zero: real layout bug, but not proven sole reason all previous guards refused. Failed copying queue and its obsolete fixture removed, replaced with exact API/lifetime tests. No old effect path stacks with new one.

## Caltrops and source notification routing

Use War_Atk_Stoneskin (War skeleton, exact package, duration1.8333333731s, authored rate1), one private dynamic montage with existing factory and .10/.18 fades. Original ground Caltrops source untouched. Original caltrops sound cues retained. Source PS notify at0.740429/0.741106s triggers one existing native caltrops spawner on this cast's War pelvis. No spawn at cast start, timer, poll of animation time, cloned status or new physics field.

Live notify vtables (+0x240) disprove the initial assumption that all donor actions share existing particle/projectile hooks. Source Blueprint coating uses0x1697780, SpawnActor0x5D7C00, SpawnBlast0x5D7DF0, each verified three-argument virtual signature; three narrowly scoped MinHook detours are necessary. Exact supported executable and16-byte prefixes gate all three; partial creation/enable rolls back. Unknown/other callbacks forward to original unchanged. Existing particle entry0x16977C0 receives stomp and suppresses donor PS only for the current private Caltrops cast. Existing dash projectile hook0x5D8130 is unrelated and stays unchanged.

Gate every intercepted cue by game thread, source animation/notify outer, living weak actor/mesh/ability/private montage, reciprocal ownership, matching current native helper montage, active ability stack, current local world/index/foot War. Spawn once; duplicate/no-dust cue cannot double it. Three donor paths are consumed only inside this exact cast, preventing borrowed stone coating/Stinger actor/damage blast. Ordinary War Stoneskin and all other montages forward. Native CameraShake/surface dust/footstep cues retained. Generic Blueprint-notify hook immediately forwards when no private Caltrops cue exists. Full native Caltrops Blueprint flying5/falling3 lifecycle remains: visual hop may persist; no physical fix claimed. Rejected VM cursor/movement guard remains absent.

## Preserved code and verification

ForeignMontage Geyser function and all retained helpers byte-identical to baseline after removing only obsolete CaltropsGesture and CopyStoneskinLaunchEffects. StonePlay activation formula (1.25s), rate, fades, material shader/UV behavior unchanged. Accepted Clone/Rampage/WorldEnder, camera/input/horse/menus/updater/config byte-identical. Native War Stoneskin outside private foreign Caltrops unaffected.

65 suites expected on GitHub Windows x64 only. New exact API fixture covers parameter layout, source/CDO isolation, assets, ownership, P2 context, negative contracts and null component result. Expanded visual fixture covers single launch/recast, true expiry, Molten, deferred assets, no world/form/unknown/shutdown ash. Caltrops fixtures cover delayed one spawn, exact source, active montage/ability, weak identity, native flight preservation, and all three donor forwarding/suppression paths. Existing control and Geyser fixtures retained. Two source reviews, package/updater/Defender checks required; none prove gameplay.

## Grouped user test and rollback

First GitHub attempt compiled but failed the new public-FX fixture: the shared Name type defaults to invalid {-1,0}, not None. Explicit {0,0} now marshals the verified socket argument, without weakening the test. One C28020 warning on cue index assignment is addressed with explicit P1/P2 branches after identity capture. No ZIP was created/published by that failed attempt. The next complete CI run must pass all65 suites with no MSVC warning before delivery.

1. Strife Stoneskin Base and Molten: accepted gesture/coating, explosion when coating appears, ash only when full protection/coating ends, movement/jump and stats normal.
2. War Caltrops at rest/moving in TPS/native: complete foot stomp, one burst at stomp, no little hop, no stone coating/Stinger borrowed blast, native cost/damage/sound; can move and jump afterwards.
3. Ordinary War Stoneskin: appearance/Stinger remain native. Quick accepted Geyser/Clone/Rampage/WorldEnder regression; Geyser not modified. Coop last.

Preserve personal INI:212 defaults and55 Hotkeys unchanged. Exactly four root files in NoCompression cumulative ZIP,187 prior archives retained (188 total after build), main/archive/v1.43 untouched. Keep V1.122 ZIP and prior verified backup for rollback. No local installation during open game.
