# V1.14U TPS/Reset UI repair

## V1.14U UI-fix: TPS option grouping + About factory reset modal (2026-10-13)

User reports Show Mouse Pointer in TPS is currently in Gameplay/HUD, but belongs in Camera/Third Person. Relocated checkbox and explanation to Camera > Third Person camera after the enable toggle. Key `[Features] ShowTPSMousePointer=0` remains OFF by default; TPS suppression and Hide Reticle priority unchanged.

**Confirmed code fault**: `ImGui::OpenPopup("Reset INI to Factory Defaults?")` occurred inside `BeginTabItem("About")` but `BeginPopupModal(...)` was outside `EndTabBar()`, so ImGui ID stacks differed and modal never appeared. The uploaded V1.14U log (18:39:38–18:50:41) contains four `INI saved` events but NO `INI factory reset` nor Reset action despite user click, consistent with the modal never being reached. The fix puts BeginPopupModal within the same About tab before EndTabItem, with a persistent SUCCESS/FAILED/CANCELLED message immediately below button plus LAST ACTION; explicit log entries for button/confirm/result/cancel. Existing full factory reset method and dated .bak backup logic preserved.

Other log highlights (not changed): Synergy 317 observed/113 boosted; Wrath/Chaos Add 0; TPS forward foot 2337/horse 0. LB+Y native 32, writes 0. Mod x64 hooks successfully loaded. User still needs to validate in game.

This is cumulative V1.14U with embedded updater unchanged; new ZIP name DarksidersGenesis_V1.14_ZZ_TPS_RESET_FEEDBACK_TEST.zip sorts after earlier V1.14 Z archive so updater identifies it as latest branch ZIP, without overwriting earlier test builds.

## Acceptance tests
1. Camera > Third Person camera contains checkbox; Gameplay > HUD no longer contains it; OFF remains the packaged and factory-reset default.
2. About > Reset INI to Factory Defaults opens modal immediately. Cancel closes it, does not change INI, and shows CANCELLED. Confirm creates a dated backup, fresh defaults, and shows SUCCESS (or FAILED with log explanation); restart verifies defaults. There must be exactly one full-reset button; per-setting Default buttons unchanged.
3. Verify Overlay > Last action and per-session Mod.log trace report click, confirmation, outcome. Check Alt-Tab, TPS pointer visibility, Hide Reticle priority, in-game Updates tab and four root archive files.
4. Validate GitHub Actions x64 build and user gameplay. Keep main/archive untouched and prior V1.14 ZIPs available.

## Verified compilation and published archive

Windows GitHub Actions [run 38434291793](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38434291793) completed **SUCCESS**. Verified: Windows x64 Release compilation, exactly four ZIP root entries, artifact upload, and permanent debug publication. [V1.14U TPS RESET FEEDBACK TEST](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.14_ZZ_TPS_RESET_FEEDBACK_TEST.zip), 747066 bytes, Git blob SHA-1 `2c8998c237edc1f11aea0c23acde97f63bde2f94`. `main` and `archive` untouched. This confirms the binary build, not the in-game popup/INI reset behavior, which awaits user verification.
