# V1.31 follow-up: exact native targeting and camera lifecycle audit

2026-10-14. AUDITED STATICALLY; no new gameplay build, no user validation.
Base: debug `82221cc8871da66e2ce40295bd7ef25f0e506f92`, cumulative V1.31.
V1.31 is an experimental baseline, not an entirely user-validated release.
Retain the user's accepted V1.23 mouse orbit, V1.26 RawInput recovery and older validated controller/horse/Synergy functions. Do not restore an old whole source tree over those acquired features.

## Evidence acquired this session

The installed Shipping EXE is readable locally, 62,113,280 bytes, SHA256
`9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.
This supersedes older notes that the executable was unavailable. Analysis used
pefile 2024.8.26 and Capstone 5.0.9; no process attachment, game-memory writes,
game installation edits or executable upload.

User log session: 18:02:21 to 18:09:05, V1.31. Snapshot SHA256
`9c94cbfb7aa1d01c3f82da8eaf9f505fd3d0423ac941d8c147cc8a5813d1c990`.
Local snapshot and disassembly outputs are retained outside the repository in
`../audit-v131/`; these private evidence files are not part of the public ZIP.
Previous V1.30 attachment was also located through the referenced conversation.
The synced `sources/` directory has no files in this checkout.

## What this log actually says

- Cinematic counters: enters=0, exits=0, candidates=3. No sustained ENTER in this session. This does not invalidate the user's observation that pause returned to overhead view: a view return does not require an ENTER event.
- 18:04:08.246 candidate 1: missing actor, native camera shift 69,129 units. SHORT_CUT after 31 ms. A scene identity and its visible duration cannot be inferred from this event.
- 18:04:46 / 18:05:38 / 18:06:30: STATIC_NO_ACTOR observations, explicitly not cinematic candidates. No authoritative native pause state was captured.
- Candidates 2 and 3 at 18:08:04 during horse summoning have gap 2320/2335, FOV90, rejected after 15/16 ms as normal gameplay returns. Do not label these as actual cinematics.
- 18:08:57.087 temporary vanilla-view ON, binding mask 0x0080, delay300 ms, held312 ms; OFF 18:08:58.207. This is the existing shared R3/keyboard temporary view path, not a cinematic ENTER. The log prints the configured pad mask even if the keyboard branch supplied the hold, so it cannot identify the physical input source alone.
- Mouse: 25,968 relative packets, 17 fallback moves, zero edge recaptures; 5 RawInput re-arms succeeded. Keep this path frozen.
- Foot reticle getter overrides=8,315; mounted getter observations=0 and native rotation getter=0. A changed look getter is not proof that projectiles follow the reticle.
- Synergy: 96 local additive calls, 48 positive gains boosted; Wrath/Chaos native Add calls remain zero.

## Three independent sources of native camera return

| Source | Exact current path | Implication |
|---|---|---|
| Explicit temporary view | DarksidersGenesisMod.cpp UpdateOrbitInput, tpsSuspendButtonMask/keyboard Suspend, hold delay, tpsTemporarilySuspended | Accepted shared E/R3 behavior; preserve it. It can return overhead without cinematic detection. |
| Camera eligibility | CameraTraceFeature.cpp HookView: actor pose freshness <300 ms, or fresh horse <750 ms; gameplayCamera=actorValid && normalAngles && nearPawn; early return on !gameplayCamera | Native view passes through immediately even before 500 ms and even when STATIC_NO_ACTOR suppresses the cinematic candidate. Pause or UI stopping actor callbacks is a plausible source, not proven UI identity. |
| Cinematic heuristic | Same HookView, last TPS age<3500 ms, rejected native view sustained >=500 ms | Only updates suspension/history. SHORT_CUT naming does not make short scenes recognized throughout playback. A valid nearby actor and normal pitch can cause a real in-place scene to be overwritten as gameplay. |

Therefore suppressing a pause ENTER alone cannot fix the visible pause camera
change. Conversely, removing the early return or forcing a cached actor pose
through wheels risks overwriting real cinematic/menu cameras. Keep the fail-open
native behavior until authoritative view ownership and UI state are resolved.
LB wheel exemption and shared E/R3 behavior are intentional and remain unchanged.
There is no evidence in this log that either named weapon/equipment wheel was
classified as an actual cinematic. Treat the two wheels independently.

## New exact native targeting evidence (RVAs, not file offsets)

1. `0x671A40` reads character fields +0xD24/+0xD28 and returns a vector whose Z is zero, with deadzone +0xCB0 and optional rotation controlled by +0xDE5. `0x671B00` converts that vector to rotation or falls back to root rotation. This is not a 3D projectile target API.
2. Engine reflected `DeprojectMousePositionToWorld` exec thunk `0x1DBE710` calls `0x1AC3760`; it obtains mouse pixels then calls shared screen deprojection `0x187F0B0`. The shared helper obtains a projection/view matrix and calls `0x1B21F20`. Its existence does not prove it uses the final mod camera in every targeting call.
3. Two **game-native** helpers `0x5719A0` and `0x571E50` call the same `0x187F0B0` at `0x571A72` and `0x571EB3` respectively. This is substantially stronger than locating UE reflection strings alone.
4. `0x5719A0` obtains OS/game viewport mouse coordinates via `0x1886560`. It optionally accepts a cursor collision target. Its fallback uses plane normal floats at RVAs `0x365A770/774/778` = **(0,0,1)**. It computes ray-plane intersection at the supplied reference point height, checks a zero denominator, but does not require the intersection parameter to be positive. It then subtracts the reference XY and normalizes a **2D** result. This can mathematically invert direction when the intersection is behind the ray origin. This mechanism exists in the exact executable; attribution to the user's actual backward bullets remains unproven.
5. `0x571E50` obtains a different screen input via `0x1ACAA70`, deprojects it, constructs a ray endpoint 100,000 units away, and calls collision helper `0x171E560` with channel value 0x13. It stores a hit target in a structure with a valid byte at +0xC, otherwise falls back to actor root location. It is a candidate for preserving native collision rather than inventing a line trace ABI.
6. The caller around `0x57B940` chooses helper `0x571E50` or `0x5719A0` based on a virtual input-source discriminator (vslot +0x48); calls at `0x57BA19` / `0x57BA5B`. A second caller block around `0x57C130` has the analogous calls `0x57C206` / `0x57C3C9`. Branch value 0 reaches the mouse-coordinate helper; value1 reaches the other helper. Mapping all values to actual device modes and establishing local owner/weapon/mounted coverage still requires runtime proof.

Do not label `0x571E50` a controller-only function on these static facts alone.
Do not hook global UE deprojection for all consumers: it also serves UI and other
game systems. Do not equate a reflected GetAimTarget pointer with projectile
spawn authority; examined wrappers include virtual dispatch and an embedded
interface at +0xB08, not a direct verified muzzle-to-hit function.

## Decision, next implementation and acceptance

No V1.32 binary published merely to repeat geometric classifier tuning or another
yaw patch. No new native hook introduced: current evidence does not yet prove
local ownership, mouse-only scope, mounted coverage, actual muzzle origin or
the collision ABI. Shipping a 3D change now could also affect controller targeting.
This audit changes the next approach from look rotation to native target acquisition.

Next concrete work:

1. Resolve the two caller owners and device discriminator; inspect the native target consumer and projectile spawn chain. Bound read-only observation to exact callsites/EXE, copied stack arguments, and verified local owner; never dereference stale owners from a worker thread. Capture which paths execute on foot and horse, native ray origin/direction, plane parameter sign, collision hit and actual muzzle target.
2. Resolve authoritative pause/UI/sequence/view-target lifecycle, starting with reflected IsPauseMenuOpen, RequestPauseMenu, ForceCinematicMode, SetViewTargetWithBlend and their registrations/actual implementations. Names alone are not callable APIs or established state offsets. Log explicit temporary view, rejected actor freshness, radial UI, native sequence ownership separately.
3. Only then build isolated **mouse** target acquisition: ray through actual emitted camera and physical reticle, native collision hit ahead, positive-range/no-hit policy, muzzle-to-hit vector using the game's proven target consumer. Keep full XInput/RT/RB/R3 paths byte-identical, and exclude menus, both wheels, pause, cinematic, focus loss, co-op remote owner and simultaneous genuine pad use.
4. Acceptance for the future build: upper/middle/lower and left/right targets with both mouse buttons, foot and mounted; near obstacles, empty sky/no hit, camera behind player; real projectile impacts checked visually. Separately test pause, both wheels, 3 scenes, RawInput/Alt-Tab and controller-only regressions. Compilation is only CONSTRUIT/CONTROLE, never VALIDATED.

Rollback remains the preserved numbered V1.31 ZIP (or user's accepted feature baseline), with personal INI backup. No version increment or defaults/shortcuts changes for this documentation-only audit.

## Delivery checks

Live GitHub Actions run 38592694757 was re-read: configure, Release compile,
four-file package, upload and permanent debug archive publication all SUCCESS.
This is the existing V1.31 build, not a new build. Its archive is retained.
Local V1.31 archive rechecked: exactly four root files, 770718 bytes,
SHA256 `9b34bf4afdf00548d37ee717aea222e9e8e104e2b9b1c53371a9445002ba6522`.
Both binaries have MZ/PE headers and AMD64 machine 0x8664.
Remote heads before this documentation update: debug 82221cc, main
2bc3122c1ae07b7333798462f85c1d9fd3a739f7, archive
10e0f220b85022bc9269e0cb9390a811a3b1cf89.
