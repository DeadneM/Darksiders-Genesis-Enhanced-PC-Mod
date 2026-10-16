# V1.45 — Native ownership for local split-screen cameras

## Baseline and objective

Solo baseline: V1.43 source902861e / delivery5fecde0. The requested health-potion refill, rate and Unlimited implementation from V1.44 source248db7b is retained unchanged. V1.44 successfully built with15/15 tests, archive publication8c633c3, verified flat four-file x64 ZIP; **not installed because the game remained open for the user's read-only dash/coop captures**. That package and all older archives remain preserved.

V1.45 objective: **each local TPS view and its camera controls must belong to its native LocalPlayer, including after the other character respawns**. Success requires a real two-player test; source evidence and unit tests alone do not validate gameplay. This is an experimental `debug` build, not a stable release or a change to main/archive.

## Read-only proof, 16 October 2026

Retail EXE SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. The game was opened by the user, PID2856, not launched or modified by this audit. All process access used QUERY_LIMITED_INFORMATION | VM_READ; no remote calls, writes or injections.

At12:27 the typed UObject inventory has313476 entries and zero read retries. Strife pawn`0x1fcb335aab0` is linked to native LocalPlayer0 / controller0; War pawn`0x1fd0cbfbbc0` to the second native LocalPlayer / controller1. Their root positions differ: about(-3623,1283,90) and(-2184,2477,96). Each has its own typed `MayhemPlayerCameraComponent`, named `SplitscreenCamera`, whose UObject.Outer is that pawn, and a distinct FollowCamera. Two native camera managers have reciprocal PCOwner links and the corresponding pawn as ViewTarget. The extra CameraActor and NPC cameras are different objects/types.

The native array at World+0x160 -> GameInstance+0x38 contains exactly those two LocalPlayers, in that order. Their controller IDs are0 and1 at LocalPlayer+0xE0. The second object's name ends in `_2`: **a UObject name suffix is not a player index**. Native metadata and read-only live reciprocity agree for:

| Relation | Offset |
| --- | --- |
| Camera UObject.Outer -> normal player | 0x20 |
| Player FollowCamera / SplitscreenCamera | 0xBA0 / 0xBA8 |
| Actor.Outer -> Level, Level.Outer -> World | 0x20 / 0x20 |
| Camera cached World | 0xF0 |
| World.OwningGameInstance | 0x160 |
| GameInstance.LocalPlayers TArray | 0x38 |
| LocalPlayer.PlayerController / ControllerId | 0x30 / 0xE0 |
| Controller.PlayerState / Pawn / Player | 0x318 / 0x350 / 0x398 |
| PlayerState.Owner / normal character | 0x108 / 0x5C0 |
| Pawn.Controller / PlayerState | 0x348 / 0x330 |

Native class slots are checked using the same class-tree relation as the retail code; additional class getter prologues are verified against the exact EXE. List bounds, reciprocal controller/state/local-player ownership and the camera's actual Follow/Split registration are required. UE fallback CameraActors are accepted only through their reciprocal native camera-manager/PC owner chain. Unknown solo camera paths retain the established solo fallback; unknown coop camera paths remain native. CDO/archetype objects, remote controllers, unrelated cameras, stale normal pawns and mismatched worlds fail open. Possession may clear the old normal pawn's PlayerState; the current controller/state determines ownership during horse/ultimate transitions.

Local evidence remains under the parent `audit-v131/`: `v145-live-views-20261016-122708.json`, `v145-coop-owner-fields.json`, `v145-coop-local-array.json`, `v145-owner-fields.txt`, `v145-class-guards.txt`. The final live read-only resolver check (`v145-live-resolver-check.json`) succeeds for all four player Follow/Split components and confirms all13 native class slots. These are local audit artifacts, not files read from or written to the mirrored `sources/` directory.

## Demonstrated code defect and repair scope

`HookCharacterGetMaxSpeed` previously replaced one `g_localPlayerCharacter` for every locally controlled pawn. The log switches between the two players at12:26:10 and again at12:26:19; the user's report that the last respawn wins agrees with this code. `HookView` reused its global pose/yaw/orbit for every view. The orbit input loop also took the first moving controller among all devices, so P2 could rotate P1's view.

The live resolver now follows the native LocalPlayers array. P1's established pose/aim/resource paths only receive that first player's character. P1 orbit samples only that player's native ControllerId. A typed P2 camera receives its own pose, fixed yaw reference, private orbit, arm distance, pause hold and emitted look yaw. P2's camera input and native-look return adjustments use only its native ControllerId and current owned view; they cannot publish the primary mouse ray or overwrite P1's camera state. P2 temporary-native and recenter use the existing configurable binding values. The global TPS enable option still enables/disables the feature for both local views; there is no new per-player options menu or independent long-hold global toggle for P2.

Horse and ultimate distance/height settings are shared settings applied to each owned pose, while actual pose ownership is separate. War's ultimate aiming remains native. P1 horse camera sampling in coop requires proved ownership rather than proximity; its own live camera can supply the current possessed mount pose. Resource hotkeys/UI remain targeted to P1; no promise of per-player resource configuration or hotkey routing is made.

No new native detour is installed for coop. The change uses the existing view, springarm, look and XInput callbacks; extra ownership reads never write actor/controller/animation memory. Buttons and triggers in the new P2 camera path pass through; only camera/movement stick role mapping and the existing camera-forward look policy apply during an owned active TPS view. Configurable wheel selection retains native axes. P1 grenade/horse chord and other validated button paths remain intact. Keyboard/RawInput mouse orbit is still P1's stream; Steam/HID devices outside native XInput and online coop are not claimed covered by the new second-controller path.

## Top-down aiming after dash

The old `HideGroundTargetingEffect` detour at0x6DDAD0 was an unproven diagnostic visual route. Native disassembly shows it stores a persistent hide flag at its own object's+0x79C and updates two effect components. That object is **not established as the player**: live player+0x730/+0x738 do not match those effect pointers. The current session log has no ground-FX audit calls; therefore this audit **does not claim it caused the reported dash issue**.

The obsolete visibility detour and its unused telemetry are removed. The user-validated shader line/arc filter, its exact CRC hashes and all three configured visibility scopes are unchanged. This leaves the native action's hide/show lifecycle intact and removes an unnecessary possible persistent override. With TPS off, both primary and secondary camera-forward/native-look overrides must pass through. Retest native aiming + dash and the arc/line; odd direction or absent graphics remain **unresolved until the real gameplay result is known**. Do not force an aim key, replay inputs or arbitrarily call show-effects after a dash.

## Strife third-to-fourth dash finding

The third position capture,13,753 samples /979 active dash observations /16 instances /zero read retries, is preserved locally as `v144-live-dash-position-20261016-121253.jsonl`. Native FNames resolved in the same running process identify `Strife_Dash_Level1_Montage`, `Strife_Dash_Back_Level1_Montage` and `Strife_Dash_Level3_Montage`. The chain commonly plays first/second Level1 near0.40s, then Level3 near0.83s. Four Level3 profiles show rapid movement in the first roughly0.4–0.5s and residual speeds around30UE units/s in the final roughly0.3s. Some samples straddle native action completion, so finished/walking frames must not be mistaken for active root motion.

The observed gap from completion of Level3 to the next active dash is only0–32ms: the extra duration is inside Level3, rather than a proved post-action global delay. The capture contains no input-press edge timestamps, so it cannot measure exactly how long a requested fourth dash waited. The old generic MoveInterruptDelay is zero; the old Montage-only hook did not match the actual dash class. **No forced early finish, movement-mode override or arbitrary animation time cutoff is introduced. Action Recovery remains unavailable.** Next work must examine Level3's actual montage/action interrupt and cleanup rules, or a native recovery section/rate seam, rather than repeat the rejected generic MOVE permission patch. Chests remain deferred by the user's dash-only scope.

## Verification, acceptance and rollback

Source prepared; V1.45 build/package/install receipts will be added after actual checks. New tests execute the real ownership resolver on two-player/respawn/remote/malformed/world/horse/form graphs and the real secondary camera/input code with read-only native boundaries replaced. The latter checks private view/orbit, primary input isolation, configurable fire/wheel/recenter/suspend, swapped stick roles, pause, cinema, focus, respawn and native top-down pass-through. These prove software contracts, not in-game camera feel or projectile impact.

Acceptance after user relaunch: separate the two players, rotate each camera independently, respawn each in turn, then check native view + aim + dash, wheel selection, cinematic return and both players' horse/ultimate views if available. Potions: refill below native cap; compare rate OFF/ON with actual native healing overflow; Unlimited ON then OFF in a mission with finite charges. Keep prior ammunition/gauge/Skip Warning behavior and all personal bindings.

Installation requires a genuinely closed game; back up four installed files and log, preserve the newest personal INI byte for byte, replace only verified binaries/README and compare destination hashes. No automatic launch. Roll back through that full backup or the retained V1.43 archive if gameplay regresses. All108 older ZIPs, main2bc3122, archive10e0f22 and public releasev1.43 must remain intact.
