# V1.112 - Verified debug delivery

## Status and base

CONSTRUIT / CONTROLE ; EXPERIMENTAL, NOT VALIDATED IN GAME. Functional retained baseline V1.111, accepted V1.110 WingClip/audio/Lava paths preserved. User diagnostic Stoneskin comparison completed on V1.111; that is not validation of the V1.112 rendering candidate. Source gameplay commit 35c09c0464c8578c183f797d74d0dbcaf08448bc, fixture-only correction 8d6e2f502f108643440bdfc9427685eac4cfc120, archive commit 2b609415cffaff77a99fc1615cf8ba51c3574813.

The first GitHub run 40402083695 failed to compile the new test because its local_players namespace was unqualified. ASI compilation itself succeeded; no archive was published from that run. The second [GitHub Windows x64 run](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40402937664) passes **62/62 suites**, zero MSVC C/LNK warnings/errors. Runner Node deprecation notice is separate. [CI evidence](evidence/V1_112_CI_CHECK.json).

## Package, updater and preservation

[Download immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2b609415cffaff77a99fc1615cf8ba51c3574813/downloads/DarksidersGenesis_V1.112_POWER_BLENDS_STRIFE_STONESKIN_TEST.zip) - 2181183 bytes, SHA256 `EBDF021A0CD4B0C4DD315F9CC376E8B80177FE21CAFF492E0D4F35F426AF86D1`.

Exactly four stored/NoCompression root entries, CRC valid, ASI/DLL AMD64, compiled-source README and INI bytes checked. ASI SHA256 `55DEF7C47E305F034B04FA570DCF4C0EFFDB296DFAC45E30E7DE56367C30B47E`. INI remains 169 keys, 55 hotkey IDs. All **176 old Git ZIP blobs unchanged** (177 total); local quarantined V1.108 remains missing and is neither restored nor staged for deletion. main/archive/v1.43 refs match the protected baseline. [Package evidence](evidence/V1_112_PACKAGE_CHECK.json).

Updater public debug listing selects V1.112 using the existing version rules, HTTP 200, byte hash equals this ZIP; blob `373df36a74c3ccd850352050d6f841bc4c09a812`. No in-game updater UI operation was performed. [Updater evidence](evidence/V1_112_UPDATER_CHECK.json).

Final Defender custom diagnostics with DisableRemediation found no threats in ZIP and extracted payload, exit 0. Realtime/on-access/antivirus enabled before and after; no exclusions, protection changes or restore. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.211.0. This is a local scan result, not a universal security certification. [Scan evidence](evidence/V1_112_DEFENDER_CHECK.json).

## Included change and runtime test

Continuous compatible cast/hold segments for Strife Geyser and War World Ender; private montage exit fades minimum 0.18 s preserving longer authored values; native Rampage pose weight/position followed through the actual fade rather than cut at ability end. Removed unused active-map getter and presentation ability identity. No control, root-motion or damage-window shortcut.

Strife Stoneskin private body MIDs copy native base/Molten uniform parameters only while its real status exists. Previous material parents remain reachable; inherited/explicit overrides restore separately, only if still owned. Native War/Stinger unaffected. Texture/shader compatibility on Strife remains experimental; a pre-existing dynamic material that the native factory reuses is refused rather than edited. No source/shared material mutation. [Implementation, evidence and limits](V1_112_POWER_BLENDS_STRIFE_STONESKIN_TEST.md).

Tests for the user, together: (1) Strife Rampage/Geyser ends, movement/recast and native/TPS; (2) War Clone/Caltrops/World Ender soft end, accepted sounds and damage; (3) Strife Stoneskin visible coating and expiration/restoration, base/Molten if available, then original War Stoneskin/Stinger; (4) quick WingClip/Lava preservation. Shield/charge damage not blanket-validated; P2 deferred. No automatic game launch, local compile/install or personal INI changes.

## Appearance inventory and next step

[Native appearance catalogue](V1_112_APPEARANCE_CATALOG.md): 1,502 candidate export tables inspected, 269 detailed assets, models/materials/effects kept separate from unlockable skins. Caltrops base/frost 1/2/3 render bindings confirmed; core classes also carry gameplay differences and will not be transplanted as cosmetic options. No new visual selector is shipped in V1.112. Next: grouped user validation, then render-only appearance selection and remaining tool/core mapping. Existing shared interrupt-notify cache instancing remains a pre-existing P2 audit item; no coop readiness claim.

Rollback: return affected wrath slots to Native or use preserved V1.111, keeping the personal INI. [Static preservation check](evidence/V1_112_STATIC_REVIEW.json). The user's untracked REPRISE_AUDIT_2026_10_13.md stays untouched (SHA256 8872360A13AD7F0C6ADE23E5A7D9DF2FBBD65C04DCA03F6BCDFACC893BC77E01).
