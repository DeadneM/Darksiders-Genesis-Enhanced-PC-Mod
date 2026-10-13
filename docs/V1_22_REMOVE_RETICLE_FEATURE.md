# V1.22 - Retire ASI Hide Reticle completely (2026-10-13)

**User directive:** V1.21 regressed cross-shaped reticle and endless TPS mouse orbit. Remove the entire mod-side reticle hiding facility, including shortcut, underlying hook, runtime flags, UI and INI setting. Do NOT silently alter any unrelated defaults. Source base V1.21, retain V1.20 Raw Input and Camera Forward. All numbered ZIPs preserved.

## Verified code
- Removed hooks for native `UAirshipUIManager::IsCursorVisible` and Win32 `user32!SetCursor`, including installers, custom forced NULL cursor, cursor probes, focus-loss cursor restorations and per-Present blank/restore. No replacement cursor logic.
- Removed `HideNativeCursorDuringTPSGameplay`, `ShouldBlankGameCursor`, `RefreshReticleCursorState`, user `HideReticle` flag, Camera > General Hide Reticle checkbox and button, TPS menu-only cursor rule, UI hook telemetry, runtime snapshot and config field.
- Removed `Action::ToggleReticle` enum/labels/tokens; F4 defaults to **None**. An old personal `F4=ToggleReticle` parses to None. An old `HideReticle` key is ignored, and removed on explicit config save (personal INI otherwise remains untouched). F1 Hide HUD retained. Mod ImGui mouse cursor for menu interaction retained, as are the separately configured blue aiming effects (not Hide Reticle).
- ThirdPerson=1 default, input, overlay, updater, horse, DXGI safe two-logo filtering, native cutscene handover, TPS mouse relative RawInput and Camera Forward shooting, and all gauge multipliers left unchanged.

## V1.21 uploaded log evidence (22:01:16-22:02:18)
- Exact supported game EXE and V1.21 startup confirmed. Despite `HideReticle=0`, the TPS auto suppression installed SetCursor and native UI cursor hooks, issued `SetCursor -> NULL` (22:01:31), then repeatedly toggled cursor suppression with camera changes.
- `RAW_REGISTER` succeeded, but only **3 raw mouse packets** vs **687 fallback mouse moves**, zero edge packets. Compare earlier V1.20 user session: **12,237 relative raw packets**, **1,940 fallback moves**, **2,541 raw packets at screen border**. This is a regression correlation; hook causation remains unproven. V1.22 removes it entirely to run a clean A/B test.

## Current resource gauge investigation
- Recent V1.21 session did not generate positive Synergy native events: all AddWrath, AddChaos/Ultimate and AddSynergy counters 0. Read-only candidate Wrath decreased **20 -> 0**, one spent event (-20), zero rises, so no charged-gain source could be identified in this session.
- Previous V1.20 combat log showed **59 Synergy positive gains, all 59 boosted**, 60 null additive calls, proving that known path works. Wrath candidate +20 over 5 rises and -70 over 6 drops, while AddWrath stayed at 0 calls; native AddChaos also at 0. This leaves true Wrath/Ultimate reward/pickup function unresolved. Do NOT modify gauge amounts or assume gem values; capture isolated gem pickup and enemy kill events with their timeline next.

## Skip Warnings investigation
- Startup order: two logos, autosave-warning black screen, controller-warning black screen, intro movie.
- Current safe selective Skip Logos substitutes only `THQ_LogoBasic` and `AS_LogoBasic`, leaving MoviePlayer attached. StartupScreens has three native 0x40-stride records, at +0x39 `1,1,0` (two active candidate flags); log explicitly `drawVerified=0`. No evidence yet that they map directly to the two warning images or that removing them preserves initialization.
- `Skip Warnings` menu entry remains disabled; never restore older broad startup loop/SetupLoadingScreen bypass because these disrupted cursor/UI/game initialization. Next stage is locate the precise warning **presentation** callbacks after native initialization, not skip the loop or entire MoviePlayer.

## Cutscene investigation: what the log REALLY proves
- V1.21: **3 candidate cinematic entries / 2 exits**, 4 candidate starts, **376 samples with invalid actor**, 0 far-camera samples. Events approx 22:02:03.951, 22:02:05.823 and 22:02:12.770; native pitch=-45, XY gaps ~1,482-1,527 units, TPS auto-suspend after ~500ms; TPS resumed twice when actor near/valid. Last event persisted to shutdown.
- Prior V1.20: 3 enters / 2 exits, 4 candidate starts, 291 invalid-actor samples. Thus the **camera handover heuristic works and often returns**, but the log does **not** identify UE Sequencer / LevelSequence / actual movie playback. A distant native view or missing actor also occurs while loading, changing player, or scripted transitions. Do not force TPS during real cutscenes. Next audit needs engine-side cinematics identity plus user timestamp comparisons.

## V1.22 verified package
GitHub Actions Windows x64 [run 38461117435](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38461117435) **SUCCESS**, Release compiled and four ZIP root entries validated. Permanent [V1.22 cumulative ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.22_NO_RETICLE_HIDE_TEST.zip) 750900 bytes, Git blob SHA-1 `07acd04899dc160d8fbb03a452ce9d362804b9d9`. Original V1.21 archive restored/preserved exact Git SHA-1 `84cbe9353497ec1f695548b6a7dce09e6d27879d`. `main` and `archive` not touched. In-game testing is still pending.

## Next user test
Launch V1.22, verify *zero* `Reticle SetCursor` or `Cursor visibility hook READY` messages, F4 None and no Hide Reticle settings. Test endless mouse orbit at screen edge with TPS ON, LMB Camera Forward, overlay Insert, gamepad/weapon wheel, horse, Alt-Tab and a scripted camera transition. Check `RAW_RELATIVE` total if mouse motion still stalls. Do not hide the cursor artificially during this test. Compare against known V1.20 Raw Input behavior. Strict numeric next build V1.23 (no letter suffix).
