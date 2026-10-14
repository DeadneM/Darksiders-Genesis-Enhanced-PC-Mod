# V1.23 - TPS relative mouse edge recapture (2026-10-14)

Branch `debug`, base V1.22. EXPERIMENTAL, game validation pending. User reports V1.22 functions all work except mouse yaw stops at window edge. V1.22 log: RAWINPUT registered=1, 687 physical packets, 77 at border, fallback=1354, mouse camera frames=1140, registrationErrors=0.

**Targeted change:** Retain V1.20 RawInput and original input passthrough; if real recent mouse movement (<180ms) reaches 10px of client edge, reposition OS cursor to client center with >=350ms cooldown. The resulting artificial WM_MOUSEMOVE is excluded from mod camera deltas. Only focused, fresh TPS gameplay; no overlay, equipment wheel, TPS temporary vanilla/cutscene view, or Alt-Tab. This is an explicit limited exception to the former no-pointer-warp policy, NOT a reticle hide function. Zero SetCursor / game UI visibility hooks; F4=None. No changes to gauges, intro, warnings, cinematics, controller, horse, updater, menu or default INI.

**Verify:** rotate TPS several full circles both directions at each edge, ensure no camera snap; note any visible pointer recenter. Test Insert menu, game wheel, controller R3, horse, Alt-Tab and cinematic transitions for zero recenter. Log includes `TPS V1.23 EDGE_RECAPTURE` counters. If native cursor/menu regresses, revert to preserved V1.22. Compile Windows x64, package exactly dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md at ZIP root. Next V1.24 after user feedback.


## Windows x64 CI and published archive verified (2026-10-14)

GitHub Actions [run 38534464159](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38534464159): **SUCCESS** for configure, Release build, four-root-file packaging, artifact upload and permanent `debug` ZIP publication. [V1.23 permanent ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.23_TPS_EDGE_RECAPTURE_TEST.zip): 752055 bytes, Git blob SHA-1 `567ad7f7854f515ae4f3e091dd87e4a5c65dc013`. The older V1.22 remains exactly 750900 bytes and Git blob SHA-1 `07acd04899dc160d8fbb03a452ce9d362804b9d9`. Windows compilation does not establish gameplay success. `main` and `archive` branch tips unchanged. User test still pending.
