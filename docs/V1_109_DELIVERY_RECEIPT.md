# V1.109 - Verified transparent ZIP delivery

**CONSTRUITE / CONTROLEE : [61/61 Windows x64 suites](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40346271897), zero MSVC warning/error. [Immutable public ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/24cf9a9ecd44c67ff69be88b1149e3d5b7f1b675/downloads/DarksidersGenesis_V1.109_TRANSPARENT_ZIP_DELIVERY_TEST.zip). Defender diagnostic scans: no threats in final ZIP or extracted payload; realtime protection stayed active. Public debug updater selects V1.109 and downloads matching bytes. Gameplay remains the V1.108 experimental candidate, not user validated.**

Compiled source `00204f931f0fd75fd275bd844cb28ef023d4ae9f`; archive commit `24cf9a9ecd44c67ff69be88b1149e3d5b7f1b675`. Base is V1.108 delivery receipt `4864b4dd17647210e5e7ad8c401257529dc619d0`, functional accepted gameplay baseline remains V1.103. Only BuildInfo version/archive identity changed under source/tests/config; no gameplay, input, camera, native hook or updater-engine changes. Personal INI and hotkeys retained.

Cause observed on this PC: Windows Defender quarantined browser and updater downloads of V1.108 as Trojan:Script/Ulthar.A!ml. Original ZIP diagnostic also detected it; its four extracted files were not detected. A NoCompression diagnostic ZIP with identical file SHA256 values was not detected. This is packaging-dependent evidence, not an official Microsoft false-positive determination or a general safety certification.

GitHub now uses standard NoCompression ZIP packaging and compares every archived file to its compiled source bytes before publication. Final V1.109 entries are stored (method 0), four root files, CRC valid, ASI/dxgi AMD64, README/INI matched to the compiled commit. Final ZIP 2126749 bytes, SHA256 `955339AFC8079273F3237B0E7A05A84E5BEDCF0CBECBDD54A48A1989A63B5500`. Unchanged INI has 169 keys, SHA256 `599E7AEE6350160BF70985B406A4C0B35094F282B6269971A48C88778AB5C29B`; 55 Hotkey IDs preserved.

MpCmdRun custom diagnostic scans, with no remediation and exclusions ignored, returned exit 0 and 'found no threats' for both final ZIP and all four extracted files. Engine `1.1.26090.9`, security intelligence `1.461.211.0`. Protection was not disabled; no exclusion, quarantine restore, unblocking or renamed executable. No flagged payload was executed. These are observed results for the current engine/signatures; later signatures/cloud judgments may differ.

HTTP 200 and byte equality checked on the immutable archive and the URL returned by the public debug listing. The game updater window was not observed; the actual current selection rules and returned archive were verified. Next action: Update -> debug -> Check for Updates, select V1.109; keep personal INI. The failed V1.108 queued request is obsolete, so check again before retrying installation.

173 historical Git archive blobs compared unchanged, 174 ZIPs in remote history/tree. main/archive/v1.43 refs verified intact. Defender quarantined the local V1.108 copy; its local missing status is not committed and no restoration was attempted. No local compile, install, game-file edit, save edit or game launch by agent.

User validation still needed: updater download/installation on their machine, then the grouped V1.108 gameplay tests. V1.109 includes all V1.108 gameplay candidates without additional behavior changes. P2 remains deferred. If a new warning appears, preserve its exact detection instead of overriding protection. Older archives remain available for legitimate rollback, with the V1.108 container issue explicitly recorded.

[Incident and diagnostic scope](V1_109_TRANSPARENT_ZIP_DELIVERY_TEST.md). [V1.108 gameplay test list](V1_108_RAMPAGE_LAVA_POWER_PRESENTATION_TEST.md).
