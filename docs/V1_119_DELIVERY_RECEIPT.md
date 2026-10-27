# V1.119 - Verified half-second-longer Strife casting delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Cumulative V1.118, retained accepted gameplay/camera/controls and later Clone/Appearance/Rampage acceptances. User requests half a second more Geyser/Stoneskin animation and reports War Caltrops still hops. The V1.118 Caltrops duration hypothesis did NOT fix it; leave OPEN, unchanged here. No broad promotion of untested material/library/P2 changes. No local compilation/installation/game launch or external process writes in this delivery.

Source `914a5c87ddd6f395503239f4f7a79e3d8d772b0e`, archive `38e0bfab00c001975d8d74c83414cfaa27e8cdc2`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40513219454): **65/65 suites**, zero MSVC warnings/errors. Geyser checks old endpoint still within new return, correct new completion, midpoint smooth blend, same root/live curve/actor/movement preservation and moving/jump passthrough. Stoneskin checks1.50s nominal playback and exact+0.50s also for a shorter normalized source (0.60->1.10), native factory/fades/loop/trigger and failure/native-War behavior retained. Tests are not live animation proof.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/38e0bfab00c001975d8d74c83414cfaa27e8cdc2/downloads/DarksidersGenesis_V1.119_LONGER_STRIFE_CASTS_TEST.zip), 2325162 bytes, SHA256 `08917F08D9DA96CBB6D37F31735A3A5CA9D41E3AB355A56F7EE74F54AB4D3850`. Four root files, NoCompression, CRC valid, ASI/DXGI AMD64, INI/README matched compiled source. ASI SHA256 `B2DE547076140679BCAD49BB3BC2560CB1320F7A2D245C2EA469CF0362672633`. All212 INI keys/values and55 Hotkey IDs unchanged. All183 previous ZIP blobs preserved (184 total), main/archive/v1.43 intact. Local quarantined V1.108 deletion and untracked audit neither staged nor restored.

Updater debug selects V1.119 blob `dd06c027418f6bd194c00364de277ee2e1b00853`, HTTP200 and byte/hash-identical to immutable public ZIP. No game updater UI interaction claimed. Defender ZIP and extracted four-file scans DisableRemediation exit0/no threats, realtime/on-access/antivirus active before/after, no security/exclusion changes. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.225.0. Diagnostic scan, not universal certification.

## Exact changes

Geyser: stationary rendered return0.65->1.15s (+0.50). Same start0.85s, smoothstep0.22s fade, so visual end1.50->2.00s, inside unchanged native2.3667s montage. Same private native pose/curve sampler, root0 exclusion, live curve protection, ownership/skeleton/proxy checks and native moving/jump path. No forced finish, callback/damage, montage duration or movement/control lock write.

Stoneskin: activation becomes min(native normalized duration,1.0)+0.5, normally1.50s instead of1.00s. Same existing rate convention; exactly+0.50 before native requested-rate scaling. Same gesture, blend-in/out0.16/0.30, native factory/loop/trigger and finish delegate. Status lifetime, cost, sound/material observation/expiry and original appearance restoration unchanged. Native War/Stinger paths intact.

Only source changes: BuildInfo, ReturnSpan/log in GeyserStaticPose and activation duration/log in ForeignWrathFeature. Two exact-diff reviews agree; Caltrops/ForeignMontage/Clone/Rampage/World Ender/skins/materials/Appearance/camera/XInput/dash/horse/updater byte-identical. No new hook/loader/timer/recovery patch or setting. [Detailed contract](V1_119_LONGER_STRIFE_CASTS_TEST.md).

## Caltrops status and next evidence

Latest V1.118 log confirms the War Start/End return was active immediately before the reported Caltrops tests (14:28:40/43,14:29:07/09); changing End to0.55s did not remove the hop. Native Blueprint flight/fall lifecycle remains a source hypothesis, not a live measurement of actor displacement. The clarification distinguishes whole-character lift from a rendered body bob; no response is fabricated and no capture claimed. No additional duration-only Caltrops retry in this build. Earlier VM-guard proposal remains excluded; user chose the simplest retained approach, no new method approval inferred or requested. P2 deferred.

## User test / rollback

1. Strife Geyser standing: return is half a second longer, no long lock. Repeat moving, jump immediately and several seconds after.
2. Strife Stoneskin: gesture half a second longer, no sliding, status duration/skin restoration remain correct.
3. Clone/Rampage should keep accepted behavior; Caltrops is knowingly unchanged and OPEN. Describe whole actor lift versus body pose before another repair.

Rollback preserved V1.118 ASI; keep personal INI. V1.118 installation was separately requested and verified in the preceding turn; this delivery does not auto-install V1.119. Next work uses the user's timing report and the Caltrops observation.

Evidence: [CI](evidence/V1_119_CI_CHECK.json), [ZIP](evidence/V1_119_PACKAGE_CHECK.json), [updater](evidence/V1_119_UPDATER_CHECK.json), [Defender](evidence/V1_119_DEFENDER_CHECK.json), [two reviews](evidence/V1_119_STATIC_REVIEW.json).
