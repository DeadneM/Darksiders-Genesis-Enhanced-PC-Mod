# V1.114 - Verified debug delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Retained base V1.112 / 0eff6f24907822170c908af499e93317a9004683. V1.113 REJECTED: user confirms persistent jump lock after Geyser, wrong Stoneskin gesture and material roles. Source rollback removes the rejected experiment; exact root cause of the persistent lock was not independently captured. Success still requires the user's gameplay test.

Source `299121174810d71468357e5eb1e708a4c09de96a`, archive `4ee6f649835353de4e369d03b63100b66112c967`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40445567125): **63/63 suites**, zero MSVC warnings/errors. GitHub-only compilation, no local build/install/launch. Runner action notice is separate from compiler diagnostics.

[Immutable cumulative ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4ee6f649835353de4e369d03b63100b66112c967/downloads/DarksidersGenesis_V1.114_RETAINED_CASTS_STATIC_GEYSER_FIRE_TEST.zip), 2204106 bytes, SHA256 `86BF7AA1983340F55D53EBC173B02E2C60927C2A9077E2A38C1884D42B936057`. Exactly four root files, NoCompression, CRC valid, AMD64 ASI/DXGI. README/INI matched to compiled source. ASI SHA256 `3F478FAA84F107FD4A8323BD97706B944014353E1D5AB65FF2A22CBBA98FD875`. 169 INI keys / 55 Hotkey IDs preserved. All 178 previous ZIP Git blobs unchanged (179 total). main/archive/v1.43 unchanged. Local missing V1.108 quarantine neither restored nor staged for deletion; unrelated audit preserved.

Debug updater public rules select V1.114, blob `7b4130000162008d36ef442d824c6bf58ead1fe7`. Both immutable/public updater downloads HTTP 200 and identical SHA256. No in-game updater UI action or runtime validation claimed. Defender custom scans (DisableRemediation) of final ZIP and extracted four files: exit 0/no threats, realtime/on-access/antivirus active, no protection/exclusion change. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.216.0. Diagnostic result, not universal certification.

## Included and preserved

ForeignMontage.cpp/h and original fixture restored exactly from V1.112. Its Geyser events, callbacks, native duration, cost/damage/audio and interruption timeline are retained. V1.113 return segments/new interruption-state code removed. New Geyser observer changes only stationary rendered pose via the existing root evaluation, leaving movement/jump/ability bytes untouched in fixtures. Movement input, last input, velocity/acceleration, falling and ended/expired states refuse adjustment. Unknown curve context also refuses safely; logs identify refusal versus actual sample. This guard may leave the old static pose in an unsupported context and is not presented as runtime success.

Stoneskin original Aether activation gesture returns, capped at 0.75 seconds at normal rate; native status duration and accepted expiry/restoration remain independent. Full native A/body and B/cloth shaders parent private MIDs, preserving base/Molten fire/static switches. No forced zero emission or shared material changes. Strong original skeletal-asset ownership, weak identities, factory rollback/refusal cache, and restore-only-if-ours remain. War's UV texture masks on Strife can still need adjustment; final mineral/fire appearance is experimental.

Validated Rampage ending, the three Strife powers on War, sounds/speeds, melee/Lava/camera/XInput/settings and all earlier accepted features preserved. Opening Rampage charge damage still needs explicit enemy comparison; coop last. Other pre-existing shared notify-state reuse remains a deferred P2 audit item. Caltrops cosmetic-only selector is still queued.

## First user tests

1. Geyser at rest, jump repeatedly after the spikes; then Geyser moving, in TPS/native. No persistent lock, preserve moving behavior, shorten only the static pose.
2. Stoneskin static and holding movement: original recognizable short gesture, normal walking, status remains for its normal lifetime and restores correctly.
3. Mineral body with native fire regions including scarf for the corresponding status variant; no red-body/stone-cloth reversal. Original War unchanged.

Rollback: preserved V1.112 or affected ability slots Native, retain personal INI. [Full contract, limitations and next work](V1_114_RETAINED_CASTS_STATIC_GEYSER_FIRE_TEST.md).

Evidence: [CI](evidence/V1_114_CI_CHECK.json), [package](evidence/V1_114_PACKAGE_CHECK.json), [updater](evidence/V1_114_UPDATER_CHECK.json), [Defender](evidence/V1_114_DEFENDER_CHECK.json), [source preservation](evidence/V1_114_STATIC_REVIEW.json).
