# V1.28 temporary cinematic detection history / manual missed-scene marker

2026-10-14 | `debug` branch ONLY | **EXPERIMENTAL / DEV-ONLY**. Cumulative V1.27 TPS camera, RawInput recovery, dual mouse fire, gamepad, mounted camera, resource/skip/horse behavior unchanged.

## User request and truthful semantics
The user observes two of three cinematics correctly switch to native view. Asked for a list of cinematics that the mod interprets, for temporary debugging. **The mod does NOT know cutscene names, UE4 LevelSequence IDs, dialogue titles or actual playback status.** Its existing heuristic classifies native camera view when local actor is absent, view far from local actor, or pitch outside gameplay range; after >=500ms of sustained rejection from a recent gameplay TPS view, automatic suspension starts. In-place cinematic with normal-looking camera remains invisible to that heuristic.

## Delivered V1.28 debug feature (no behavioral changes)
- In **Camera > Cutscenes: detection history (DEBUG)**, a collapsible non-default-open section lists the last **24** classification *events* (not filenames). Each has event serial, candidate ID, approximate age, phase `CANDIDATE / ENTER / EXIT / REJECTED / RESET / USER_MARK`, elapsed candidate time, and reason (`NO_ACTOR / FAR_OR_HEIGHT / PITCH / GAMEPLAY_SHAPE`) with native pitch, yaw, FOV and planar actor/camera gap. Snapshot status shows current candidate, suspended flag, totals and recent native camera sample.
- `CANDIDATE` created at heuristic camera departure, `ENTER` when TPS is actually suspended after 500 ms, `EXIT` when normal TPS game view resumes, `REJECTED` when camera returns before threshold or gameplay reference lapses, `RESET` on disabled/TPS off.
- A **Mark cinematic playing NOW (manual debug)** button creates an explicit **USER_MARK** even when the heuristics think the camera looks like gameplay. It does not set tpsCutsceneSuspended and does not change native camera, player, world, menu or resources. This records the *latest* sampled native view, including its age. If the game pauses upon opening Insert, source age should be considered.
- Events written to `DarksidersGenesisMod.log` as `TPS V1.28 CINEMATIC_DEBUG`. First 120 events are logged, then every 100th, to bound log volume. At clean game exit, a copy of the last 24 entries is logged as `TPS V1.28 CINEMATIC_LIST`. Events older than the history ring are still present in earlier log lines if within the bounded logging rule.
- Thread-safe small SRW-locked ring: UI reads a copied snapshot and does not read mutable native pawn pointers. A cached view summary is updated at most every 180ms. No new game memory writes, native detours or cursor controls.

## How to test the three cinematiques
1. Enable TPS + Auto suspend (unchanged V1.27 defaults); run first cutscene, open Insert after scene and look in Camera debug panel. A correctly recognized sequence should have CANDIDATE, ENTER and EXIT under the **same candidate ID**.
2. Repeat for second. A false detection can be distinguished by `NO_ACTOR` while loading.
3. For third missed scene, open Insert while the cutscene is on screen and click **Mark cinematic playing NOW**. If there is no `CANDIDATE` and USER_MARK has `GAMEPLAY_SHAPE`, the failure is the current camera-shape heuristic. If CANDIDATE but REJECTED, it is a threshold/continuity issue.
4. Send full log, optionally the Camera panel screenshot and the order of the three scenes, especially timing after a USER_MARK. Do **not** interpret UI event labels as true named cutscene assets.

## Safeguards and pending work
No true native cinematic identification, no ability to guarantee 3/3 detections, no changes to Skip Warnings, Wrath/Ultimate or mounted bullet raycast. Preserve stable V1.26 and experimental V1.27 ZIPs and frozen `main`/`archive`. Build Windows x64 CI cumulative four-file-root ZIP. The debug panel is **temporary**, to remove/disable once we identify real native event source.

## V1.28 Windows build and publication verified (2026-10-14)

GitHub Actions [run 38557275092](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38557275092) **SUCCESS**: Windows x64 configure, Release compile, exactly four root-file ZIP validation, CI artifact upload, permanent archive commit and push to `debug`. [V1.28 cumulative TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.28_CINEMATIC_HISTORY_DEBUG_TEST.zip): **763,306 bytes**, Git blob SHA-1 `76008c8329e1ef0448815995df4a1b8146216d2b`. V1.27 (759,159 bytes) and V1.26 (756,898 bytes) preserved. `main` SHA `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, `archive` SHA `10e0f220b85022bc9269e0cb9390a811a3b1cf89` unchanged. In-game detection/history usability is **not yet tested**. Event history still provides only camera heuristics, not game asset names.

