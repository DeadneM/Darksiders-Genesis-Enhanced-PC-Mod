# V1.33 - camera-center mouse aim and native UI stick passthrough

2026-10-14. Branch debug only. **CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL. Gameplay unvalidated.**

## Verified delivery and local installation

Source 52ee7b4eb1073650e573d8896a968db6ebf35dcf. Windows x64 GitHub Actions
[38633252781](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38633252781)
SUCCESS: configure, Release compilation, exact four root ZIP entries, upload and
numbered archive publication. Archive commit 5e60284821ca85e8d2397b2955947f5a5fe4b794.
[Permanent V1.33 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.33_CAMERA_CENTER_UI_PAD_TEST.zip),
774986 bytes, SHA256 EFF76B7514FFB69F957ABB7FE9688C5122F29C7F2EC917E4ED32B157940D040E.
Independent extraction confirmed exactly ASI 1143808 bytes, dxgi.dll 154624, INI
2677, README 240120. Both binaries checked MZ/PE AMD64, embedded ASI build identity
1.33-camera-center-mouse-ui-pad-passthrough-test verified. The direct raw URL was
downloaded and matched the same ZIP hash.

Installed locally after an elevated check confirmed no game process running.
ASI, loader and README copied with destination SHA256 equality checks. Existing
personal INI was NOT replaced: SHA256 before/after
AB2F95CD2AE49AD1706E1E9A420FEEDB1E416D25FCA157E83897554EF2C3E095.
Previous four installed files and hash manifest backed up at
`../audit-v131/install-backup-before-v133-20261014-223512/`.
The packaged README reflects its pre-CI source snapshot; this verified-delivery
record supersedes those pending status lines. The installed README is updated
with these verified post-CI notes; native binaries still match the ZIP exactly.
No launch or gameplay validation performed by the agent.

Automatic review became available again on the user's new compile/install
request. Source publication and installation passed the normal approval route;
no alternative publication path was used to bypass the earlier review failure.
main 2bc3122c1ae07b7333798462f85c1d9fd3a739f7 and archive
10e0f220b85022bc9269e0cb9390a811a3b1cf89 remain unchanged. Older V1.31/V1.32 ZIP
blob identities were rechecked unchanged after V1.33 publication.

Base: V1.32 source cf613bb, published package d842a55; local delivery notes 6a69776 were not pushed because the automatic approval reviewer could not complete review after a usage-limit error. Do not bypass that review using another publication route.

## Authoritative V1.32 user test

User confirms all three wheels now keep TPS and the tested cinematic correctly activates detection. Preserve those acquired results; do not roll the camera/native lifecycle feature back. This validates that tested scene, not every scene in the game. User reports equipment wheel selection with the right stick is broken, and mouse/horse aiming seems improved but requests simpler camera-forward aim.

The camera staying in place while an ammo/ability wheel is open is V1.32's copied TPS composition hold. It is preserved. The configured temporary-native shortcut still has priority, per the user's selected rule.

The actual 19:40-19:43 game log confirms V1.32 loaded successfully despite our earlier installer stopping before copying. Do not attribute that installation to the stopped installer. Snapshot SHA256 AD201FE64D82FA5D36CE2807D66F13596D1C5016A3D0B8071A5BF475C3D17AF0, retained privately at ../audit-v131/user-v132-test-1940.log.

Native observations: helpers2=2578, helpers3=52, scoped rays=2436; FOOT=1657 and HORSE=779; native hits=2349 and no-hit forward targets=87. These prove execution of the scoped target path for both sources, not projectile impacts. Native cinematic mode enabled at 19:42:33.669 and disabled at 19:42:39.653, with camera ownership transitions on the same edges. World pause and wheel-held ownership transitions are logged independently.

## Equipment-wheel regression mechanism and bounded repair

V1.32 keeps a TPS image during native UI via tpsUiViewHeld. However, ShouldSuppressNativeAim did not consult that state. FilterNativeXInput could still zero RX/RY or apply gameplay firing behavior when the UI did not enter the existing LB-wheel exemption. IsLiveTPSLookActor could also permit the native look hooks to neutralize stick-origin look while the actor remained fresh. This is a concrete source path consistent with the user's broken wheel; the full runtime input chain was not captured.

V1.33 adds the existing UI-ownership state to BOTH exclusion gates:

- ShouldSuppressNativeAim returns false during tpsUiViewHeld. The existing FilterNativeXInput early return therefore preserves the native right stick for UI selection, before gameplay stick-UP/zeroing.
- IsLiveTPSLookActor returns false during the same state, making the look getter hooks return their original results.

No new opener mapping, right-stick remap, synthetic buttons or delay. The FilterNativeXInput function body itself is unchanged from V1.32. Existing R3 continuous hold/stow processing before the gate, shared temporary shortcut, horse chord and gameplay RT/RB logic remain. With no UI ownership, the old predicates retain the same result. During UI, input is native while the copied TPS camera remains.

If a wheel never publishes native UI ownership, this exemption cannot identify it; do not claim the selection fixed before user testing. V1.32's observed world-pause/known-wheel states cover the implemented exclusions.

## Mouse aim: camera center, no cursor targeting

ComputeNativeMouseRay no longer reads cursor pixels, window aspect or FOV. For current emitted camera yaw Y and pitch P it supplies:

`direction = (cos(P)*cos(Y), cos(P)*sin(Y), sin(P))`

The origin remains the emitted TPS camera position. V1.32's native collision, no-hit point ahead, local owner checks and shared FOOT/HORSE consumers are unchanged. Mouse-only scope and simultaneous genuine pad/right-stick exclusions are unchanged. This aims through the camera center using actual pitch as well as heading; there is no mouse-to-XInput simulation and no synthetic upward stick introduced for mouse. The established controller stick-UP path remains unchanged.

No new INI options/defaults or cursor modifications. The legacy WndProc cursor-ray diagnostic remains read-only; it does not describe the new aiming direction. Native target logs and the new startup CAMERA_CENTER line are authoritative for the requested direction, with actual impacts still requiring a game test.

## Verification / preservation

- Actual suppression expression evaluated across 256 flag combinations; UI ownership always disables native aim suppression, and ordinary focused TPS baseline still suppresses it.
- Actual direction expressions evaluated on 25 yaw/pitch combinations; normalized direction matches camera angles, with no cursor/FOV dependency in the targeting function.
- Both native input exclusion gates checked. XInput filter body unchanged from cf613bb.
- CameraTraceFeature, NativeTpsFeature, HorseFeature, GaugeChargeFeature, ShaderAimFeature, ConfigStore and packaged INI compared unchanged with V1.32. Preserve all old numbered ZIPs and main/archive.
- Local source whitespace checks pass. These are source-contract checks, not a Windows build or gameplay test. No local Windows C++ compiler was found in PATH or the standard Visual Studio installer locations checked.

## Goal / acceptance / rollback / next step

One coherent build goal: restore native equipment-wheel selection under retained TPS and make mouse attacks target the camera center on foot and horse.

Success: controller right stick selects items in equipment wheel; ammo/ability selection still works; all retain TPS composition; tested scene hands off/returns; temporary-native shortcut still works. Mouse LMB/RMB hits at the camera center at different yaw/pitch, foot/horse, near walls and no-hit sky, independent of physical cursor position. Controller-only RT/RB/R3/horse regressions must remain absent.

The initial source-only stage was blocked by an automatic review quota failure on the previous documentation push. This is historical: the verified delivery above supersedes it. The normal approval route later succeeded, and the V1.33 four-root-file ZIP now exists at the verified link above.

Rollback is the preserved V1.32 package or local backup, retaining personal INI. V1.33 is now the latest verified/installed Windows experiment. Next: user tests equipment-wheel selection and camera-center mouse aim on foot/horse, with V1.32 validated camera/scene behavior preserved. Native Wrath/Ultimate gain and Skip Warnings remain unrelated unresolved work.
