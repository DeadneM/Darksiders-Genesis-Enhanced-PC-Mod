# V1.29 | real reticle-aligned projectile path research + TPS-distance cinematic candidates

Date: 2026-10-14. `debug` branch ONLY. Cumulative V1.28; experimental test until actual user signoff. The user provided actual **V1.27** log `DarksidersGenesisMod(20261014-113630).log`, not V1.28. This distinction matters: V1.28 cinematic event-list debug was not yet tested by this user.

## User-reported bugs
- Native aim was designed for the original TOP-DOWN camera. With modified behind-the-character view, screen cursor below horizontal center projects to a world point BEHIND the player, shooting backwards. At foot, existing Camera Forward look-vector override does not actually correct projectile raycast; mounted unchanged.
- User asks for **bullets to hit the mouse reticle on screen**. Accurate solution is a native 3D ray from the active TPS camera through the physical reticle pixel, traced for collision, with correct muzzle origin/projectile velocity and ownership. It must apply ON FOOT and MOUNTED; no manual yaw-only direction patch suffices.
- Cutscene candidates should use configured TPS camera distance, not unrelated fixed 6500-unit horizontal threshold, with caution about native isometric camera offset.

## Direct evidence from V1.27 log
- Startup SHA256 exact EXE `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`, TPS distance multiplier 0.150.
- Camera V0.68 reports native SpringArm **2130 units** and TPS camera final distance approximately **319.5 units**, not 2130. Thus naïve native-view-to-player `distance > selectedTPS(320)` would classify virtually ALL normal gameplay as cutscene. An offset-baseline-relative threshold is mandatory.
- Mouse relative packets `24658`, native look-forward samples `4210`, secondary samples `1864`; horse getter `0`, horse rotation getter `0`; **projectileHook=NO**. This proves current hooks are not enough and does NOT prove where the bullets' true trace function is.
- FACING_CAMERA foot large-gap 3682/15389 samples, horse 645/2765, another distinct root/mesh-facing problem.
- Cutscene 8 candidates, 6 enters, 4 exits, including one candidate with player present / gapXY ~13k; fixed 6500 horizontal heuristic is too coarse.
- Synergy 75/75 positive boosted, Wrath read-only +20 gained, AddWrath zero; resource issues untouched.

## Independent external evidence, not currently transplanted
- A community mod **Klassik 3rd person cam** (Nexus Games Darksiders Genesis /mods/2, now removed according to latest public site) previously advertised **bullets raycast to camera** for Steam build #49453 (2026), and author publicly described an object/camera/raycast callsite patch in a Cheat Engine table. See https://www.nexusmods.com/darksidersgenesis/mods/2 and https://www.reddit.com/r/Darksiders/comments/1tlla8x/got_the_fps_style_raycasting_working_only/ . Historical table link https://pastebin.com/raw/MD9L3WK2 currently inaccessible in this environment; the exact game EXE bytes / complete table and code-signature match are not available here. **Do not copy bare CE offsets from internet snippets or claim an unverified native bullet hook is safe.** Shipping arbitrary native raycast mutation risks crashing the game or corrupting player attack state. Need reliable native source and exact signature before modifying actual projectile paths.
- Without access to the game's exact executable/actual native raycast code, the only safe gameplay action is to preserve existing firing and add passive cursor/camera evidence, not create a fake "aim fixed" checkbox.

## V1.29 concrete changes
1. **Cinematic heuristic relative to configured TPS distance.** Preserve original `oldNearPawn` 6500 XY / 7500 Z failsafe. In `CameraTraceFeature::HookView`, track conservative **3D native camera-to-player baseline** from live, actor-valid gameplay snapshots, reset on player generation or FOOT/HORSE camera source transition; re-learn extremely slowly after valid frames. Compute `selectedTpsDistance = validNativeArm * thirdPersonDistanceMultiplier`. Candidate distance = baseline native 3D gap + **max(300, selectedTPSDistance)+100** UE unit anti-jitter guard, only when auto cutscene suspend enabled. `gap3D > candidateDistanceThreshold` is an additional reason to reject native gameplay camera / register CANDIDATE / enter after existing 500ms. The native camera is intentionally NOT compared directly to a tiny visible TPS distance.
2. V1.28 list and logs now expose `TPS_DISTANCE` reason, measured 3D separation, native baseline, selected TPS distance and live threshold. No native cinematic/UI callbacks changed; R3 temporary native view and cinema 500ms debounce retained.
3. Mouse shot origin **READ-ONLY** WndProc event telemetry (first 24 button-down events) `TPS V1.29 CURSOR_AIM_AUDIT` logs normalized cursor x/y [-1,+1], `belowHorizon` boolean, mounted view state and TPS yaw/pitch/FOV. *This is not bullet raycast instrumentation and does not force shots to cursor.* Existing user LMB/RMB, RawInput, Win32 WndProc and XInput paths unchanged.
4. Existing V1.28 history and manual scene-marker remain available in Camera > Cutscenes: detection history (DEBUG). No new gauge or Skip Warnings writes. ZIP exact four files at root and older stable archives retained.

## Next shot fix gated on native evidence
1. Acquire target EXE exact SHA or complete working third-party CE table/source, and locate the project's actual `line trace / target acquisition / shot spawning` function for player Strife primary + secondary, horse, and charged attacks; verify call stacks and bytes.
2. Validate camera projection (vertical/horizontal FOV, screen aspect, active cursor position, near/far clip). Construct camera ray from screen cursor, find hit by game's trace, then use muzzle-to-hit trajectory, avoiding shots striking walls behind the camera or through walls.
3. Scope to local player, TPS active/focused, actual weapon firing; keep co-op/player 2 vanilla, all modes/focus/menu/radial/cinematics vanilla, no default movement or actor yaw override. Log actual raycast caller/hit/direction before any writes; user A/B test. Once validated, retire the earlier yaw-only patch if redundant.
4. No false claims of 'fixed' until in-game projectiles follow reticle both above/below horizontal screen center, on foot and mounted.

## Test
For cutscenes, repeat the three scenes, note if new `TPS_DISTANCE` candidate appears when camera moves beyond native+TPS threshold, confirm no false cutscene during normal movement. To help actual aim investigation, stand and shoot once with LMB/RMB at a target in each of upper and lower halves of screen, then repeat mounted. The debug log gives `CURSOR_AIM_AUDIT` for initial 24 mouse presses. Preserve and share log. V1.29 does NOT promise accurate reticle-aligned bullets.

## V1.29 Windows CI and permanent ZIP verified (2026-10-14)

GitHub Actions [run 38558950360](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38558950360) **SUCCESS**: Windows x64 configure + Release compilation, exact 4 root-file cumulative ZIP (`dxgi.dll`, `DarksidersGenesisMod.asi`, `DarksidersGenesisMod.ini`, `README.md`) verified, artifact uploaded, numbered archive preserved in `debug/downloads`. [V1.29 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.29_TPS_DISTANCE_CURSOR_AIM_AUDIT_TEST.zip): **765,354 bytes**, Git blob SHA-1 `512761f52f91661f0fab1eb2e13a2755e982fcc2`. V1.28 (763,306) and V1.27 (759,159) remain. Frozen `main`: `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`. Frozen `archive`: `10e0f220b85022bc9269e0cb9390a811a3b1cf89`. User validation pending. **The screen reticle trajectory is NOT YET repaired**: the actual native projectile raycast source is not identified, even though cursor-click telemetry has been added and the new cutscene threshold is compiled.

