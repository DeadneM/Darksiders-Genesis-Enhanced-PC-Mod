# V1.121 - Verified native cast reset and Stoneskin launch delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Cumulative V1.120 with selective rollback to pre-duration-retune casting, retaining accepted later features and ability baseline V1.112. Latest user verdict rejects recent Caltrops and Geyser ending-pose experiments. Current Stoneskin activation/duration/coating/restoration explicitly user-validated, only launch particles missing. No local compilation, installation, game launch or external process write.

Source `99b673df540f8097ac016072293e316a5b6386d5`, archive `eb36a53e2a1bd3280a973b4caa6c4e025063d30f`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40542119595): **65/65 suites**, zero MSVC warnings/errors. Removed obsolete GeyserStaticPose suite/module; added launch-particle preservation/rejection fixture. Two static reviews agree on removal of the pose sampler/held-frame/source flag and preservation of accepted helpers. Native timing/callback/cancel/source/ownership safety checks pass; this does not certify a rendered animation or actual game result.

## Changes and preservation

Caltrops: original ~2.333s cast span again, one continuous War gesture. No appended End, temporary animation-instance flag, continuity observer or physics/VM patch. Native spawner/cost/damage/audio/ownership unchanged. Geyser: one continuous gesture over original2.3667s, no held frame or additional ending pose/evaluator; accepted damage/callback/interruption timeline and private fade safeguards retained. Both gestures may be slower than the rejected accelerated versions; absence of bob is a pending visual criterion.

Stoneskin: accepted activation normally1.50s, .16/.30 fades, status/material/cloth/restoration unchanged. New private visual helper queues only the two known base/molten unattached native particle notifies at their proportional launch phase, with native filters and existing audio preserved. It does not copy actor/blast/status/camera/surface gameplay notifies. Native War/Stinger remain intact; failure logs unavailable and retains accepted cast behavior. Clone/Rampage/WorldEnder helpers/audio byte-identical, camera/controller/dash/horse/appearance/updater/config unchanged. [Detailed contract and tests](V1_121_NATIVE_CAST_RESET_STONESKIN_FX_TEST.md).

## Package and updater

[Immutable cumulative ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/eb36a53e2a1bd3280a973b4caa6c4e025063d30f/downloads/DarksidersGenesis_V1.121_NATIVE_CAST_RESET_STONESKIN_FX_TEST.zip), 2322339 bytes, SHA256 `18318EA88CD6298FDD414AD8241F5DB9DA9BC37E064EC6137E8439EF577697BF`. Four root files, NoCompression, valid CRC, ASI/DXGI AMD64, INI/README match compiled source. ASI SHA256 `FE00E264503D66756D3B7B4853DAE8E514F8BF0A3454BF2FDF65B96D3904F5A8`. All212 INI keys/values and55 Hotkey IDs unchanged; all185 previous ZIP blobs retained (186 total), main/archive/v1.43 refs intact. Existing local V1.108 quarantine deletion and unrelated audit never staged or restored.

Updater debug selects V1.121 blob `dbfcca4ef4d45c388b28978235aa72f439a66b7c`, HTTP200, byte/hash-identical download. No game updater UI interaction claimed. Defender ZIP and four-file folder scans DisableRemediation exit0/no threats; realtime/on-access/antivirus active before/after, no exclusions or security changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.225.0. Diagnostic result, not universal certification.

## Grouped user check and next step

1. War Caltrops stationary/moving: no body hop, normal movement/jump, native cost/spawn/audio. Native Strife regression.
2. Strife Geyser stationary/moving in TPS/native: no vertical ending, jump and movement after peaks, damage/cancellation intact.
3. Strife Stoneskin Base/Molten: launch particles visible, same accepted gesture and coating, clean expiry. War unchanged.
4. Quick Clone/Rampage/WorldEnder regression. Coop last. If bob persists, report it as OPEN and use the native-pose difference, without stacking another animation/physics workaround.

Manual install/updater with personal INI retained. All previous packages remain available for rollback; V1.112 is retained accepted ability reference, V1.120 restores the previous experimental state. No automatic installation or game launch.

Evidence: [CI](evidence/V1_121_CI_CHECK.json), [ZIP](evidence/V1_121_PACKAGE_CHECK.json), [updater](evidence/V1_121_UPDATER_CHECK.json), [Defender](evidence/V1_121_DEFENDER_CHECK.json), [two reviews](evidence/V1_121_STATIC_REVIEW.json), [animation source](evidence/V1_121_ANIMATION_EVIDENCE.json).
