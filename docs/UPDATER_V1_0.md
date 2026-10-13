# Updater V1.0 | Main / Beta debug, safe installation

**Branch:** debug only. The mod's four-file cumulative ZIP format is unchanged. Standalone updater has its own ZIP with just EXE + README.

- UI: select game binary folder, checkbox Beta -> debug when ON, main when OFF, automatic install after check, Check Updates, Install Latest, Restore Previous. Settings persist in LOCALAPPDATA.
- GitHub Contents API lists versioned downloads on the selected branch, uses highest parsed numeric Vmajor.minor, SHA-1 verification of Git blob and strict HTTPS raw branch URL.
- ZIP validated before install: four named root files, no nested paths/duplicates/oversized payloads, MZ/PE x64 signatures. Exact supported game executable SHA-256 verified.
- Game running -> queue installation until process closes (updater must stay open).
- Backups of binaries and README go into timestamped LOCALAPPDATA backup folders per target folder; restore previous installed state from newest backup. Existing customized DarksidersGenesisMod.ini never overwritten; install default only if absent.
- On first run if an existing manually installed mod is detected, no automatic replacement. Automatic downgrades prohibited, manual downgrades require explicit confirmation.
- No updater installation inside regular mod ZIP. No log uploads. No changes to main/archive.
- Main is currently V0.72 while beta debug is V1.13. Channel setting is not equivalent to 'newest overall'.

## Test matrix
Check Windows x64 compile, 2-file standalone ZIP with EXE and README. Test first launch and folder selection, Beta main/debug selection, one-time manual approval with unknown existing mod, current version comparison, install while game closed, queue while running, INI preserved byte-for-byte, backup and rollback, bad game executable rejection, malformed ZIP rejection. User tests pending. Windows Actions result must be verified separately.

## Verified Windows CI build and delivery
- Build source commit b0d6ea60e516f83f0d49c0775f83ff3726106e5f.
- GitHub Actions run [38423882113](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38423882113): Windows x64 compilation, PE AMD64 validation, two-root-file updater ZIP validation, artifact upload, and permanent branch publication all SUCCESS.
- [Permanent V1.0 updater ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesisUpdater_V1.0_Windows.zip), size 15987 bytes, Git blob 0fc184e75c88b35820cc1df76ec5a33baa85ea66.
- The final build fixes startup preference loading so Beta read from disk never initiates an accidental early install.
- Windows compilation is confirmed, **real on-device download/install/rollback testing remains pending**. main/archive are not modified.
