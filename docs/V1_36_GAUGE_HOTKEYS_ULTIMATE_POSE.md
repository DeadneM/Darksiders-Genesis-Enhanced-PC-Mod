# V1.36 - Gauge hotkeys and native Ultimate pose TEST

Date: 2026-10-15. Cumulative base debug 56fbedbe2dfb26ebe69a49bc8ff5e3abf4c62a42 / V1.35. User now explicitly validates ALL manual gauge fillers and reports 2-3 Ultimate activations. V1.34 ordinary TPS is the accepted camera/input base, with a known Ultimate overhead-camera/control regression. This supersedes older pending-validation statements for manual refill only. Natural Wrath/Ultimate gain multipliers remain unresolved.

## Objective and acceptance

Four independently assignable existing F1-F12 actions: Fill Wrath (100%), Fill Ultimate (100%), Fill Synergy (100%), Fill all three gauges (100%). Each queues one request through the unchanged validated manual refill implementation. No new default assignments, hardcoded gameplay button, continuous refill, unlock or forced transformation. Acceptance: all four choices persist through Save/Reload and each selected F-key fills exactly the requested live native meter(s) once. The UI percentage slider still controls manual buttons; hotkey actions explicitly request100%.

Related Ultimate correction: retain TPS follow using the actual native local form pose when the usual player-movement callbacks stop, and preserve the form's native controls. Acceptance: repeat Ultimate entry/exit, movement/attacks and camera follow without overhead fallback or normal-player aim injection; ordinary TPS, native cinematics, pause/wheels, temporary native view and both stick layouts still work. This correction is EXPERIMENTAL until the user tests it.

## New log evidence, distinct from hypotheses

Read-only source log copied locally as ../audit-v131/user-v135-ultimate-repeats.log. Actual loaded V1.35 session2026-10-15 13:00:57-13:07:34. Seven manual requests and seven applied readbacks, including requests for all three gauges; nine individual setter/readback records. User confirmation establishes visible functionality beyond floats alone. Character ownership/generation changes were detected normally; Wrath maximum20 then30, Chaos100, Synergy500.

There are ZERO NATIVE_CINEMATIC_MODE events in this session. User's2-3 transformations therefore must not be attributed to a recorded cinematic flag. At shutdown missingActorSamples=2125, farCameraSamples=0. No reliable Ultimate entry/exit timestamp existed in this old log. MANUAL_CONTEXT chaosFormActive=0 appears only at startup/normal character switches; absence of1 is not evidence that the form never activated, since its sampler relies on normal-player callbacks. Loss of the normal movement pose is consistent with the reported overhead fallback; exact correlation remains to be proved by new ULTIMATE_POSE markers. The six-second cinematic signal in the older V1.34 log remains unattributed.

## Native source evidence and bounded implementation

Retail EXE SHA2569f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54, inspected read-only; no EXE/pak changes or upload.

- Native AMayhemChaosFormCharacter class getter0x76C2E0 resolves UClass slot0x380AC80; full reflected name0x277E328. Its UMayhemChaosFormCharacterMovementComponent is distinct from the normal movement class validated by the existing Jump/DoubleJump/Glide path. Native normal-player class getter0x7A6350 resolves UClass slot0x380FE78.
- UGameplayStatics::GetPlayerPawn exec0x1D15190 decodes WorldContextObject and integer PlayerIndex, then calls native0x1886B70. That obtains the controller through0x1886900 and tail-calls virtual+0xD00 for the pawn. New code uses this exact native function synchronously from the existing camera game-thread callback, with the live native camera as world context and local index0. No Present/worker-thread UObject call and no new hook.
- Require typed native player/form AND the already used APawn::IsLocallyControlled virtual+0x680. A direct local form pawn supplies its pose; otherwise the fresh normal player can supply its typed active form through+0xE78, matching native GetChaosForm/IsChaosFormActive field access0x7ACC15/0x7AD6D0. No remote player, memory scan or cached UObject dereference.
- Read only root actor+0x158 and current world location root+0x1A0/+0x1A4/+0x1A8, yaw+0x1F4, the same native actor/root layout already used by the validated pose paths. Readability, class tree, finite values and world bounds are checked. Five exact native signatures and both class-slot RIP references are checked against the local retail EXE; mismatch disables the feature.
- A valid current form pose replaces only the callback-local camera subject, without changing the normal player's identity/generation or faking movement freshness. Existing real cinematic/pause gates retain priority. The usual camera distance/height/orbit/FOV settings remain. Form scale/framing and all entry animations need gameplay review.
- While the typed local form is active and freshly observed, XInput returns the real native state before ordinary-player aim injection, axis remap, R3 helper or horse-chord remapping. The ordinary player's mouse look/raycast substitution is likewise bypassed. This deliberately gives the distinct native form its own controls. Mod-specific camera-stick role exchange/horse chord does not apply during the form; native game bindings remain. Mouse relative camera input stays on the existing path when a real TPS view is emitted. Outside the form the existing XInput function body is identical.
- Independently, native aim suppression now requires an actually fresh emitted TPS view. This avoids continuing gameplay aim filtering when the camera has stopped emitting TPS. Original focus/UI/wheel/cinematic/temporary-view gates remain.
- ULTIMATE_POSE logs entry/exit identity/source, current root validity/position and game-thread ID. Summary counts are diagnostic only. Detection covers local player0 and owned/possessed native form; coverage of each character's transformation must be tested. This is not proof that all cutscenes or transformation controls are solved.

## Hotkeys, preservation, tests and rollback

New action enum values/tokens are appended; all existing IDs/tokens, INI schema/default assignments and parser/save behavior remain. Menu: Hotkeys -> F1-F12 -> choose one of the four Fill actions -> Save. Existing personal assignments are retained. Only a focused, non-minimized game with the mod overlay closed can queue a refill key. Key edges consumed while editing the overlay cannot apply later on close. Request limits remain identical to validated buttons: fresh local-authority player context, native caps, actor/generation cancellation, one pending request, cinematic postponement and expiry. During transformation/loading/paused play the native player context can be unavailable; return to normal gameplay on foot to refill. Failure does not trigger an unsafe fallback.

ManualGaugeFeature, GaugeRequestPolicy, natural GaugeChargeFeature/Synergy, HorseFeature, TpsStickRoles, native ray/cinematic/UI observation module, RawInput/Alt-Tab blocks, ConfigStore.cpp/default INI, loader and other modules compared unchanged to V1.35. Only the explicitly described active-form input bypass and emitted-view suppression gate alter input policy. No changes to native projectile calculation, natural gains or rebinding assumptions. main/archive and all old ZIPs must remain unchanged.

Windows CTest adds GaugeHotkeys using the actual ConfigStore/RuntimeSettings implementation: every action token round-trips; labels, masks and legacy IDs checked; four new F-key choices saved/reloaded through a unique temporary INI; unrelated slots and original defaults checked. Existing TpsStickRoles and GaugeRequests tests remain. These checks cannot prove native UObject lifetime or gameplay behavior. Source prepared, build/package/install pending at this record's creation.

Rollback: close game and restore the backed-up pre-V1.36 ASI/dxgi/README, keeping the personal INI, or restore the cumulative V1.35 archive. New action tokens in an INI are ignored as None by old versions; reassign those slots after rollback if already saved.

## TODO / user test

1. Assign four unused F-slots, save/reload, close menu. Spend each meter and verify three independent100% refills and one combined refill. Check no repeat while held and no fill while overlay/background.
2. Fill Ultimate in normal gameplay, activate normally2-3 times. Check camera follows the transformed actor, attacks/movement, then return to normal TPS. Save log; match ULTIMATE_POSE entry/exit with any failure. Test both characters separately.
3. Recheck ordinary foot/horse aiming, all three wheels/sticks, both native/mod stick layouts, pause, real scene, temporary native shortcut and Alt-Tab. Native form controls and framing are explicitly not user-validated yet.
4. If evidence does not confirm correct local form ownership, stop this fallback and return to V1.35; do not infer ownership from missing actors. Natural Wrath/Ultimate gain multiplier investigation stays separate from validated manual refill.

## Verified delivery and installation

Source44b6d12d1507e74926c54fe414ba93e46aec46eb, Windows CI38727065139 / job116395439271 SUCCESS. CTest TpsStickRoles0.01s, GaugeRequests0.01s, GaugeHotkeys0.04s, 3/3 passed. Configure/build, exact-root package, upload and permanent numbered debug publication succeeded. Archive commit4d4ecd62c1b2c5c95fbab2f1cd5d61ff4f8bcf00. Legacy warnings concern unchanged unused locals/functions and a pre-existing shadowed local; no new form-module diagnostic. This is build/static/test evidence, not native gameplay validation.

ZIP 788051 bytes, SHA256 9538DC2BA73A1A649FA5653F51A63A2882C8B2ACC4B8C2BCBB4B5C56BBE55944. Direct raw download, repository ZIP and CI-reported SHA256 match. Four exact root entries independently checked, binaries AMD64 and ASI1.36 identity present:

- DarksidersGenesisMod.asi: 1161216 bytes; SHA256 D8C08C7525339FE1D94D139455ED7F7632B74F6FC943CB807A30089FB4EBEB7A
- DarksidersGenesisMod.ini: 2677 bytes; SHA256 22B4A6D034316DDD6F632FBAF15BA9C5B01B486FA429AA4D7A236483279E66BD
- dxgi.dll: 154624 bytes; SHA256 5975257FC9D2E4104C96C498AC459352ED89B03390B41C04C594F3C47C671D2B
- README.md: 248142 bytes; SHA256 A313EBF7041CE84E0FF8A6725A5D7249FFDCEE5FD46D75A1012A8BF769384124

Installed with game closed. Backup ../audit-v131/install-backup-before-v136-20261015-132719/ contains prior V1.35 four files, log and backup hashes. Installed ASI/loader match package. Personal INI preserved exactly 82AF15917398293EFA674C28EF064E15F85FF4D646B60ECBD11763AC3AD69D68, all previous settings/bindings retained. README refreshed with receipt; immutable archive untouched. Local hash manifests, CI log and old user log retained.

Only V1.36 ZIP added relative to base56fbedb; no older archive changed. main2bc3122c1ae07b7333798462f85c1d9fd3a739f7 / archive10e0f220b85022bc9269e0cb9390a811a3b1cf89 remain protected. Source checks confirm protected modules and RawInput block unchanged, normal XInput body identical outside typed active form, five exact retail signatures/class slots/owned-form field/native pawn getter checked.

Final status CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL for new behavior. User has validated underlying V1.35 manual refill for ALL gauges. Four hotkey activations and Ultimate TPS/native-control follow remain USER TEST REQUIRED. Next work follows actual entry/exit logs; do not call a filled meter a repaired natural multiplier, or missing actor samples a proven cinematic event.
