# V1.13 — Safe factory INI reset

Date: 2026-10-13. Branch: debug only. Base commit: 179ac076b8b987280bab6f51c77e9c8e53dcecdf. Main/archive unchanged.

## Behavior
V1.12 menu layout user validated. Adds Reset INI to Factory Defaults under Gameplay > System and startup > Configuration, plus a shortcut in Hotkeys. Both entry points require explicit confirmation, and Cancel leaves the configuration unchanged. Before creating a fresh default INI, the mod renames its current DarksidersGenesisMod.ini into a unique timestamped backup (.backup-YYYYMMDD-HHMMSS-mmm.bak) in the same directory. Reset removes stale and unknown keys. Previous INI is restored if rewriting fails when possible; backup creation failure aborts before modifying runtime values. Game saves and Engine.ini are not modified. Restart game to reapply startup-related settings. Per-camera Defaults remain unchanged.

No changes to any existing gameplay hooks, controller input, Wrath/Ultimate/Synergy behavior, startup-warning handling, or config defaults. This build does NOT implement the still-planned updater.

## Verification and test plan
GitHub Windows x64 CI must build ASI/DXGI and package exactly four root files: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Build experimental until user tests. Test: edit FOV/key bindings, cancel reset (no change); confirm from System, verify clean default file and dated backup with old values; restart; confirm key binding defaults; repeat to verify backups are unique; test a write-protected folder. Engine.ini and game saves untouched. Regression: horse, R3, native LB+Y, RT/RB, camera, Hotstreak/Synergy, HUD, skip logos/intro.

Latest user V1.12 log: exact supported EXE, 169 Synergy setter calls and 37 boosted local gains; Wrath and Chaos Add hooks zero. Wrath candidate 6 rises/1 fall. TPS forward injections foot=339/horse=96. Skip Warnings still unresolved.

## Verified Windows build and published ZIP - 2026-10-13
- GitHub Actions run [38421589950](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38421589950) completed successfully: Visual Studio x64 configure, Release compilation, ZIP content validation, artifact upload, and permanent branch publication.
- Source commit d6380d34267ff65d6d35e38c9b54f47bcb4261cd. Permanent debug ZIP: [V1.13](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.13_SAFE_INI_FACTORY_RESET_TEST.zip), 641322 bytes.
- CI verifies four archive-root entries: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md.
- Branch heads after publication: main 2bc3122c1ae07b7333798462f85c1d9fd3a739f7, archive 10e0f220b85022bc9269e0cb9390a811a3b1cf89: unchanged.
- V1.13 source compiled and ZIP published; IN-GAME factory-reset test still pending. Copy old custom INI before testing if desired.
