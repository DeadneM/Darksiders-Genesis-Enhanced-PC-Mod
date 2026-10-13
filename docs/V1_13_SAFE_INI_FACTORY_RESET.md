# V1.13 — Safe factory INI reset

Date: 2026-10-13. Branch: debug only. Base commit: 179ac076b8b987280bab6f51c77e9c8e53dcecdf. Main/archive unchanged.

## Behavior
V1.12 menu layout user validated. Adds Reset INI to Factory Defaults under Gameplay > System and startup > Configuration, plus a shortcut in Hotkeys. Both entry points require explicit confirmation, and Cancel leaves the configuration unchanged. Before creating a fresh default INI, the mod renames its current DarksidersGenesisMod.ini into a unique timestamped backup (.backup-YYYYMMDD-HHMMSS-mmm.bak) in the same directory. Reset removes stale and unknown keys. Previous INI is restored if rewriting fails when possible; backup creation failure aborts before modifying runtime values. Game saves and Engine.ini are not modified. Restart game to reapply startup-related settings. Per-camera Defaults remain unchanged.

No changes to any existing gameplay hooks, controller input, Wrath/Ultimate/Synergy behavior, startup-warning handling, or config defaults. This build does NOT implement the still-planned updater.

## Verification and test plan
GitHub Windows x64 CI must build ASI/DXGI and package exactly four root files: dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md. Build experimental until user tests. Test: edit FOV/key bindings, cancel reset (no change); confirm from System, verify clean default file and dated backup with old values; restart; confirm key binding defaults; repeat to verify backups are unique; test a write-protected folder. Engine.ini and game saves untouched. Regression: horse, R3, native LB+Y, RT/RB, camera, Hotstreak/Synergy, HUD, skip logos/intro.

Latest user V1.12 log: exact supported EXE, 169 Synergy setter calls and 37 boosted local gains; Wrath and Chaos Add hooks zero. Wrath candidate 6 rises/1 fall. TPS forward injections foot=339/horse=96. Skip Warnings still unresolved.
