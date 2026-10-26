# V1.109 - Transparent ZIP delivery compatibility

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40346271897), zero avertissement/erreur. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/24cf9a9ecd44c67ff69be88b1149e3d5b7f1b675/downloads/DarksidersGenesis_V1.109_TRANSPARENT_ZIP_DELIVERY_TEST.zip) - [Recu](V1_109_DELIVERY_RECEIPT.md). Defender : aucune menace detectee dans le ZIP final et les quatre fichiers extraits ; protections actives. Updater debug : V1.109 selectionnee et octets verifies. Code de jeu V1.108 conserve, toujours EXPERIMENTAL / NON VALIDE EN JEU.**

EXPERIMENTAL. GitHub Windows build, public download and final Defender scan passed as recorded in the receipt. Gameplay remains exactly V1.108 except BuildInfo version/archive identity. Functional accepted baseline remains V1.103. User installs from in-game debug updater; no local compile or game installation.

## Observed incident

The V1.108 HTTP URL/listing and downloaded checksum remain correct. On 2026-10-26 Defender quarantined the browser download (15:56:58 detection) and the updater's temporary mod.zip (15:58:06) as Trojan:Script/Ulthar.A!ml, threat ID 2147751841, intelligence 1.461.211.0, engine 1.1.26090.9. The updater result reports Windows refusing access because of a virus or unwanted program. Installed ledger remains V1.107. No game binary was replaced by this failed attempt.

Explicit custom diagnostics with MpCmdRun -Scan -ScanType 3 -DisableRemediation confirmed the original ZIP detection; scanning the four already extracted V1.108 files found no threats. A NoCompression ZIP made from those exact files also found no threats. All four file SHA256 values equal the controlled V1.108 package, four root entries and CRC are verified. The diagnostic archive uses ordinary DEFLATE uncompressed blocks (method 8 in local .NET); no obfuscation, renaming of contained files, binary edits, exclusion, unblocking, quarantine restore or security-setting change. No flagged payload was executed. Microsoft documents this diagnostic scan mode as not applying remediation and ignoring file exclusions: https://learn.microsoft.com/en-us/defender-endpoint/command-line-arguments-microsoft-defender-antivirus .

This evidence isolates a packaging-dependent detection on this machine; it is not an independent malware certification or an official false-positive verdict. Do not instruct disabling Defender or allowing the flagged V1.108 archive. The realtime engine subsequently quarantined the local repository's V1.108 ZIP too. Keep its immutable Git blob and remote archive/history; do not restore it automatically and do not commit its local deletion.

## Change and success criterion

- Bump only BuildInfo to V1.109; no gameplay source, INI, key binding, native hook, trajectory or updater engine changes.
- GitHub packaging uses the normal NoCompression ZIP option instead of Optimal. Four required root names remain DarksidersGenesisMod.asi, dxgi.dll, DarksidersGenesisMod.ini, README.md. The workflow now reads back and compares every entry to its compiled source bytes before publication.
- Build only GitHub, retain all 173 old Git archive blobs and main/archive/v1.43. Final ZIP may be about 2 MB instead of 1 MB; the current updater accepts up to 32 MB and supports this standard format.
- Success requires 61 suites/no compiler warnings, valid AMD64/CRC/root/source parity, correct public debug selection and hash, followed by an explicit diagnostic Defender scan of final ZIP and payload with no threats. No change to Defender's protection settings. A later signature/cloud decision can change; report the scan result as observed, not a guarantee.

169 INI keys and 55 Hotkey IDs retained. Gameplay tests are the grouped V1.108 list, still experimental/P2 deferred. First verify that the in-game updater offers V1.109 and installation completes after closing the game, keeping personal INI. If Windows reports another alert, retain its exact detection and stop that install rather than overriding protection.
