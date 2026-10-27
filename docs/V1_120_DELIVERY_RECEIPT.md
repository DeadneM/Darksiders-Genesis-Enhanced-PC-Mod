# V1.120 - Verified Caltrops source-pose continuity delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Cumulative V1.119. Latest user clarification is mostly a body movement without actual displacement; no capsule trace was collected or invented. Previously accepted Clone/Appearance/Rampage retained; V1.119 Geyser1.15s return and Stoneskin1.5s activation (+0.5s) unchanged, their user timing verdict still pending. Caltrops visual fix is a new hypothesis, not declared accepted. P2 last. No local compile/install/game launch/external process write.

Source `da6af2537dae1251b51901d5a08318c2b70143d1`, archive `d218ecb22e9f4e3aa2e3ad528775d63e86f0c347`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40519850736): **65/65 suites**, zero MSVC warnings/errors. Expanded Caltrops fixture verifies scoped flag activation/restoration, native flying unchanged, native true preservation, malformed reflection/bitfield/name/value/template fallback with successful spawn, owner/world/form loss, expired identity/no stale write, native flag replacement, separate P2 record and cleanup after P2 removal/reassignment, and game-thread shutdown. Existing actor/CDO/source byte preservation, constructor/play/spawn/sound/rate, reentrancy/failure tests retained. CI is not engine rendering proof.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d218ecb22e9f4e3aa2e3ad528775d63e86f0c347/downloads/DarksidersGenesis_V1.120_CALTROPS_SOURCE_POSE_TEST.zip), 2341159 bytes, SHA256 `6F5751F0A0933194F3C2E9CF5A0D0241AEA4F91767AC4ED0B4CD7D1623EB259E`. Four root files, NoCompression, CRC valid, ASI/DXGI AMD64, INI/README matched compiled source. ASI SHA256 `CF1919B463FDDF1547B4F0B61E5873CB616BE5750B2A1B8AAB4C075DD280B0D7`. All212 INI keys/values and55 Hotkey IDs unchanged. All184 prior ZIP blobs preserved (185 total), main/archive/v1.43 intact. Local V1.108 quarantine deletion and untracked audit never staged or restored.

Updater debug selects V1.120 blob `784748668f8d2fada01305cbf6a91b5ad7b68759`, HTTP200 and byte/hash-identical to immutable ZIP. No game updater UI interaction claimed. Defender ZIP and extracted four-file scans DisableRemediation exit0/no threats; realtime/on-access/antivirus active before/after, no security or exclusion changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.225.0. Diagnostic result, not universal certification.

## Candidate and evidence boundaries

War's cooked TPS Slot node has bAlwaysUpdateSourcePose=false. [Epic UE4.27 semantics](https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/AnimNode_Slot?application_version=4.27) explain how to keep the underlying source pose updating under the montage; these docs do not prove this game's ABI. Prior supported-game read-only slot schema records72-byte struct, SlotName32, bool40. Current runtime reflection must match the exact War node GUID, structure/type/size, TPS name and native byte-bool metadata before a write.

Only successful transferred Caltrops on owned local War can temporarily change native false0 to true1 on that instance. Existing true values stay native. Weak actor/ability/mesh/anim/world/index checks; current mesh and non-template animation revalidated. Existing game-thread observer services both records, including removed/reassigned P2. Restore only the same owned reflected flag if still true after finish/ownership/world/form change or game-thread shutdown. Unknown layout/value or expired/replaced object keeps the previous presentation; logs distinguish applied versus unavailable. No loader/object scan/new hook/timer. This does not retry the earlier rejected global movement/VM-cursor guard.

One byte of an owned animation-instance setting is written by the ASI during gameplay; no external process write is performed by this agent. No changes to flying/falling/velocity/position/input, VM/script, native montage timing, spawner/cost/damage/sound, native Strife or other abilities. The old grounded eligibility gate remains the default for starting Caltrops. Only continuity servicing can observe native non-walking phases without changing them. Clone/ForeignMontage/Rampage/WorldEnder, Geyser/Stoneskin timings/materials, Appearance, camera/controller/horse/updater unchanged. Two static reviews agree. [Detailed contract](V1_120_CALTROPS_SOURCE_POSE_TEST.md).

Suspended source-pose update is a plausible explanation for the reported visual bob; no live state-machine trace proves it caused this case. Runtime bool metadata compatibility and removal of the hop both need user confirmation. If unavailable or still wrong, keep Caltrops OPEN and gather pose/state evidence instead of adding timing or physics patches.

## Grouped user check / rollback

1. War Caltrops twice standing, once moving: final body pose without the hop; normal movement/jumping afterwards, native spawner/damage/cost/sound retained.
2. Quick Clone/Rampage regression; their accepted behavior should remain. Both Geyser/Stoneskin half-second additions from V1.119 remain included for their pending timing check.
3. Coop deferred. Report unsupported/unavailable log or persistent bob honestly; no validation implied by fixtures.

Rollback preserved V1.119 ASI with personal INI retained. No automatic install or launch. Next uses the user's grouped visual result and runtime applied/unavailable log.

Evidence: [CI](evidence/V1_120_CI_CHECK.json), [ZIP](evidence/V1_120_PACKAGE_CHECK.json), [updater](evidence/V1_120_UPDATER_CHECK.json), [Defender](evidence/V1_120_DEFENDER_CHECK.json), [two reviews](evidence/V1_120_STATIC_REVIEW.json), [animation source](evidence/V1_120_ANIMATION_EVIDENCE.json).
