## V1.1 - verified installed binary state (2026-10-16)

The updater now binds its installed-version record to the SHA-256 of both the ASI and dxgi.dll. Replacing either file manually invalidates an old record; automatic installation then waits for an explicit Install Latest action. Personal INI edits do not invalidate it. Old V1.0 records are treated as manual installs until the next explicit installation. The old V1.0 ZIP remains available unchanged.

# Darksiders Genesis Enhanced PC Mod Updater

A standalone Windows x64 updater built against the .NET Framework already included in Windows. This is separate from the mod's four-file ZIP.

1. Run DarksidersGenesisUpdater.exe, select the folder containing DarksidersGenesis-Win64-Shipping.exe. The updater tries to locate a standard Steam installation.
2. Leave **Beta unchecked** for **main** (stable channel). Enable **Beta (debug branch)** for experimental builds.
3. Click **Check Updates** or let the updater check when opened. **Install Latest** installs the selected version. Automatic installation when checking is enabled by default after the first updater-managed install.
4. Exit the game before installing; the updater can keep an installation queued until the running game closes.
5. **Restore Previous** rolls back the previous mod binaries.

Safety: the program checks the executable's known SHA-256, official GitHub ZIP blob checksum, exactly four root archive entries and Windows x64 binaries before changing files. It backs up previous mod files to %LOCALAPPDATA%\DarksidersGenesisEnhancedUpdater. It NEVER overwrites your existing DarksidersGenesisMod.ini. It doesn't modify Engine.ini or your game saves.

**Important:** main currently contains mod V0.72 whereas debug has V1.13. Turning Beta off does NOT force an automatic downgrade. Existing manually installed builds won't be automatically replaced on first run; choose Install Latest explicitly. A manual downgrade prompts for confirmation.

This updater is a standalone program, not a Windows background service. To check for new versions, launch it. It does not transmit logs or make ChatGPT able to read your local files.
