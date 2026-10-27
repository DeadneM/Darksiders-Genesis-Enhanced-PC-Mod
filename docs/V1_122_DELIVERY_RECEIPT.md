# V1.122 - Verified shorter retained gestures / single grounded Caltrops delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Cumulative V1.121. User clarifies activation gestures should be shorter, with the chosen gestures and visual effects retained. V1.121 installed first on explicit request with verified V1.120 backup. V1.122 installee sur demande avec sauvegarde verifiee de V1.121, INI/PAK conserves, sans lancement du jeu. No local compilation, game launch or external process memory write. Cooperative tests deferred.

Source `94d3612b487f73d2d95ee3a0a7ed2f972c038f37`, archive `ee21e5c88a1f0fa8eedd6e2d4206529b86360d97`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40545654508): **65/65 suites**, zero MSVC warnings/errors. Expanded Geyser fixture checks full same gesture1.75s, exact original damage/effect times, cancellation starts/end closure, source preservation and refusal of an unknown later cue. Grounded Caltrops getter and cast tests check exact asset/thread/weak-identity/skeleton, authored speed, one spawn, fallback, unchanged actor/physics/source and lifecycle. Stoneskin1.25s and native scaled-source case checked. Existing Clone/Rampage/WorldEnder/private-particle/config/control suites retained. Two static reviews agree; these fixtures do not validate actual rendering.

## Candidate

- Geyser: same complete Aether gesture,1.75s instead of2.3667s, one segment/no added ending pose. Source-proven last particle0.719241s and blast0.488067s retain exact timestamps; no effect time scale or particle lifetime edit. Cancellation starts native0.789254/1.052338s, endpoints close at1.75s. Unknown later cue fails before construction. Source Blueprint OnFinish/SpawnBlast offline decode is size-complete: spawned blast is owned by Character, no retained blast reference/destroy/particle-stop in OnFinish. Native follow-up launch suppression unchanged. Native source assets remain untouched.
- Stoneskin: same Aether gesture, .16/.30 fades, launch particle templates/filters and body/cloth/status restoration; activation normally1.25s instead of1.50s. Status duration and particle lifetimes unchanged, War remains native.
- Caltrops: one complete native War_Channel_End as the sole gesture at authored~0.6667s, no Start/End splice, pose evaluator or source flag. Existing typed discovery reused, no new scan/hook/loader. V1.121 fallback if unavailable. Native spawner/cost/damage/sound, authority and flight/fall lifecycle unchanged. This visual hypothesis may still leave a lifecycle-induced bob; no physical correction is claimed.

Other montage helpers, Clone/Rampage/WorldEnder, materials, camera/controller/horse/menus/updater/config unchanged. No earlier rejected VM/movement guard or new native hook. [Detailed contract/tests](V1_122_SHORT_CASTS_GROUNDED_CALTROPS_TEST.md).

## Package / delivery

[Immutable four-root-file NoCompression ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ee21e5c88a1f0fa8eedd6e2d4206529b86360d97/downloads/DarksidersGenesis_V1.122_SHORT_CASTS_GROUNDED_CALTROPS_TEST.zip), 2325600 bytes, SHA256 `2586CFD855B0CE8C9C1E9EEAD99FD36B7ECC55BA876A788318EA56885A74FFC4`. ASI SHA256 `8776109FEDDC7BFCECAC69EB1DC86CBCC421878D80EF504DBBEB1179106649EE`. CRC valid, ASI/DXGI AMD64, shipped INI/README matched build source;212 INI keys/values and55 Hotkey IDs unchanged. All186 previous archives retained (187 total); main/archive/v1.43 intact. Unrelated local quarantined V1.108 deletion and audit untouched.

Updater debug selects V1.122 blob `f639c3406f9cd9e546c7b7d7fdbd4cac621eef71`, HTTP200 and exact ZIP hash/bytes; no game updater UI interaction claimed. Defender ZIP and extracted four-file scans DisableRemediation exit0/no threats, protections active before/after, no settings/exclusion changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.225.0. Scan is diagnostic evidence, not universal certification.

## User check / rollback

Geyser stationary/moving, TPS/native: same gesture/effects, shorter smooth ending, damage/cancel and movement/jump afterwards. Stoneskin: same launch/coating/protection, shorter activation, normal expiry. War Caltrops: complete one-shot gesture without hop at rest/moving, original spawn/cost/audio and normal jump. Quick Clone/Rampage/WorldEnder regression; P2 last. Persistent Caltrops bob remains OPEN. Keep personal INI; V1.121 ZIP and verified preinstall backup retained for rollback. No game launch performed.

Evidence: [CI](evidence/V1_122_CI_CHECK.json), [ZIP](evidence/V1_122_PACKAGE_CHECK.json), [updater](evidence/V1_122_UPDATER_CHECK.json), [Defender](evidence/V1_122_DEFENDER_CHECK.json), [two reviews](evidence/V1_122_STATIC_REVIEW.json), [native timeline](evidence/V1_122_ANIMATION_EVIDENCE.json).
