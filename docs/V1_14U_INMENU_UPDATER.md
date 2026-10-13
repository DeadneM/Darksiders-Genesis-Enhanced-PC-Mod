# V1.14U experimental: in-game Updates tab (2026-10-13)

Source line: V1.14 TPS-pointer option (OFF) and About-only factory reset unchanged. Work exclusively on debug. Preserve `DarksidersGenesis_V1.14_TPS_POINTER_INFO_RESET_TEST.zip` and previous archives. No change to main/archive.

## User interface
* In-game ASI menu > Updates tab, directly before About/Info (About remains last).
* Beta checkbox: OFF selects `main`, ON selects `debug`; saved as `[Updater] Beta=0` in DarksidersGenesisMod.ini.
* `Check for Updates`: GitHub Contents API listing for the selected branch, asynchronous on a worker thread so render thread never waits for HTTP.
* Shows running V1.14 and latest version from the channel; `Install on Game Exit` confirms package, explicitly warns about downgrading to older main releases; `Restore Previous` also requires confirmation.
* No installation on toggling Beta, no automatic downgrade, and no DLL changes while game is running.

## Implementation
* `src/UpdateFeature.cpp` uses WinHTTP HTTPS requests, bounded GitHub response size, validates branch and raw URL, parses cumulative archive names. Package is rechecked in the exit helper rather than blindly accepting stale metadata.
* An x64 C# helper from `updater/Program.cs`, `updater/UpdaterForm.cs`, `updater/UpdateEngine.cs` is compiled as part of the Windows ASI build, embedded as resource ID 101 (RCDATA). CMake builds the helper before compiling the ASI resource.
* On confirmed install, the ASI extracts its embedded helper into LOCALAPPDATA and starts `--install-on-exit`. The helper waits for the game to exit, verifies selected filename and GitHub blob SHA, exact supported game executable SHA256, archive length and checksum, exactly four root entries, and PE AMD64 signatures. Only then it backs up binaries and installs, preserving existing INI. Rollback uses `--restore-on-exit` and the most recent updater backup.
* This is a standalone child process for safe after-exit file replacement, not a second window or persistent background service. No user logs are collected, uploaded, or synchronized. A downloaded mod archive and the embedded installer need normal write permissions for the game folder.

## Distribution and acceptance
* Versioned cumulative test ZIP: `downloads/DarksidersGenesis_V1.14_Z_ASI_UPDATER_TEST.zip` on `debug`, exactly four root files (dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md). Suffix Z lets the existing numeric/lexical updater pick the new V1.14 build over the original V1.14 test package.
* Windows CI must compile Release x64 with C# embedded helper and resource, verify the ZIP root, upload and publish it. Gameplay validation remains necessary.
* Test: Beta OFF main (currently latest V0.72, explicitly older), Beta ON debug (V1.14U); Check non-blocking, correct no-update status; install newer future build, quit game and relaunch, verify preserve INI, backup, restore; ensure no unrelated gameplay, HUD, controller, pointer/Alt-Tab or About/Info reset regression.
