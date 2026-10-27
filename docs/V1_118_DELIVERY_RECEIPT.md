# V1.118 - Verified smoother Strife casts delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Cumulative V1.117. User validates Appearance tab presence and War Clone, which are unchanged. Earlier accepted Rampage retained. Geyser/Stoneskin transitions still need the user test; artwork/library variants not retroactively validated. Caltrops hop still unvalidated; same0.55s End as accepted Clone prepared; proposed VM guard excluded after automatic approval rejection. Coop last. No local compilation/installation/game launch/external process writes.

Source `f4945e3848d98e0caffa679baccaca1bb0dcc19e`, archive `a080bb2202058ab1d3879a29231f8f8a846f66d5`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40504809854): **65/65 suites**, zero MSVC warnings/errors. Geyser fixture checks halfway blending and eased onset, root/live curve/actor/movement preservation and movement/jump passthrough. Stoneskin checks native factory arguments and existing duration/loop/trigger/lifecycle. CI is not game animation certification.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a080bb2202058ab1d3879a29231f8f8a846f66d5/downloads/DarksidersGenesis_V1.118_SMOOTHER_STRIFE_CASTS_TEST.zip), 2322840 bytes, SHA256 `610009AC02D6831AC68AB6C5DEC084D84FAD63BA425258B7D58D831B1F5B4EEC`. Four root files, NoCompression, CRC valid, ASI/DXGI AMD64, INI/README matched compiled source. ASI SHA256 `3A0582604DD9FB872C2FF12FF5C00A0037ACA8EC0583E3992116B04E5CDD3945`. All212 INI keys/values and55 Hotkey IDs retained, no new config. All182 old ZIP blobs preserved (183 total). main/archive/v1.43 intact. Unrelated local V1.108 quarantine deletion and untracked audit never staged/restored.

Updater debug selects V1.118 blob `507318131ee2628d6a53324caa4210b20111db5a`; immutable and updater download HTTP200 with identical SHA256. No game updater UI interaction claimed. Defender ZIP and extracted four-file scans DisableRemediation: exit0/no threats, realtime/on-access/antivirus active before/after, no protection or exclusion changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.225.0. Diagnostic result, not universal certification.

## Included changes and preserved baseline

Geyser stationary rendered return: same start0.85s, span0.65s instead of0.50s, smoothstep fade0.22s instead of linear0.12s. Completes1.50s rather than1.35s without extending native ability/montage. Keeps existing private native pose/curve extraction, root0 exclusion, live curves, ownership/skeleton checks and moving/jumping passthrough. No movement/action lock/callback writes.

Stoneskin Strife: same one-second Aether activation, native blend-in/out0.16/0.30s rather than0.10/0.18s. Play rate, loops, trigger, status cost/duration, sounds/materials/expiry restoration unchanged. No new return clip or forced finish/control patch. War Stoneskin/Stinger native paths intact.

Source changes are BuildInfo, GeyserStaticPose, the two fade arguments/log in ForeignWrathFeature, and Caltrops End duration0.30->0.55s (same as validated Clone, total1.1167s). Historical2.333s gesture was shortened to0.5667s then End0.30s added; user asks for the simplest animation approach. Native flight/fall lifecycle stays intact, no VM guard approval inferred. ForeignMontage/Clone, Rampage, World Ender, models/materials, appearance UI/library, camera/XInput/dash/horse and updater implementations remain byte-identical. Two static reviews agree. [Detailed contract](V1_118_SMOOTHER_STRIFE_CASTS_TEST.md).

## Open Caltrops and approval boundary

V1.117 log confirms Start/End return active during Caltrops. Offline native Blueprint decode shows AFTER PlayMontage it sets flying mode5, OnFinish returns to falling3 if flying/local, ProcessInterrupt invokes SetFalling. This is a source-based candidate for the small hop, not a live component transition measurement. Read-only snapshot found no game running; no relaunch/capture performed.

The proposed native SetMovementMode-thunk guard would modify the transient VM cursor only for a verified ground War Caltrops literal call. Automatic approval review rejected the global hook/stack-write method due crash/corruption risk and missing explicit method authorization. It was not applied, compiled or bypassed. This ZIP adjusts only Caltrops End duration to match accepted Clone; the hop is not declared fixed. [Reviewable scope and prerequisites](V1_118_CALTROPS_REVIEW.md). User chose the simpler animation approach; no further guard approval assumed or requested. Additional explicit approval and proof would be necessary if this method were ever revisited; no fix claim now.

## User test and rollback

1. Strife Geyser standing: return softer with no long frozen pose. Repeat moving and jump immediately/later.
2. Strife Stoneskin: start/end less abrupt, no sliding and original appearance after status ends. One-second activation/stat lifetime preserved.
3. Quick regression Clone/Rampage; they should keep accepted behavior. Caltrops: compare the1.12s gesture and the small hop at rest/moving; still experimental. P2 later.

Rollback preserved V1.117 ASI; personal INI retained. Next: user's grouped transition report. If Caltrops still hops, distinguish rendered pose from actual actor movement before more changes; preserve the user's simplest-animation choice.

Evidence: [CI](evidence/V1_118_CI_CHECK.json), [ZIP](evidence/V1_118_PACKAGE_CHECK.json), [updater](evidence/V1_118_UPDATER_CHECK.json), [Defender](evidence/V1_118_DEFENDER_CHECK.json), [two reviews](evidence/V1_118_STATIC_REVIEW.json), [Caltrops diagnosis](evidence/V1_118_CALTROPS_DIAGNOSIS.json).
