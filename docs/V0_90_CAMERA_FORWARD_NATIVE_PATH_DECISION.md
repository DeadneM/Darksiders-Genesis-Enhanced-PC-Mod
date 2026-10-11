# Debug baseline: mounted TPS Camera Forward and V0.89 observed result

2026-10-11. User request: **reuse proven on-foot TPS aiming in mounted Camera Forward without touching validated game code, and reduce repeated diagnostic builds/tests**. This note does not change the ASI binary.

## Evidence from `DarksidersGenesisMod(20261011-180102).log`

- V0.89 exact supported EXE validated and native hooks installed.
- The V0.75 native player `GetPlayerLookInput` handler replaces the X/Y world-facing look vector with `cos(cameraYaw), sin(cameraYaw)` for active TPS shooting. V0.89 totals: `nativeLookInput=25358`, `padLookAim=8156`, `nativeLookRotation=0`.
- Game-specific mounted Camera Forward was active repeatedly during horse shooting (V0.86 MOUNTED AIM CAMERA_FORWARD logs).
- But V0.89 totals: `horseActorGetPlayerLookInputCalls=0` and `FORWARD RAYCAST AUDIT lookSamples=0`; actual projectile origin/trace direction has **no native detour**. The game may use a different controller, cached target, weapon or trace path while mounted. **The on-foot hook has already been reused in the mounted mode when its conditions match**, so simply duplicating the trig formula or reapplying it to actor rotation is not a fix.
- V0.87 synthetic Ctrl events previously failed user gameplay validation; do not revive.
- V0.89 Horse Everywhere overrides executed: 20 positive region permissions, 10 desummon prohibitions, server spawns 3; horse appeared in callbacks, but no proof of unrestricted mounts in all banned mission volumes.

## Correct minimal next change

Trace the **actual mounted Strife shot/raycast callsite** in the supported EXE, not general debug spam or new XInput mappings. Reuse the existing applied TPS camera yaw/direction as a pure input to the native projectile aim path, preserving native muzzle origin/collision/range. Scope only to: gameplay focus + TPS active + **mounted camera source** + `TPSMountedAimMode=0` + genuinely firing + verified local player + exact game signature. All other shots pass through untouched. There is no safe address/signature for that path in this repository yet. Do not invent one and ship untestable hooks.

## Protected functional baseline (immutable until explicit user signoff)

- Mode 1 **Vanilla Aim** mounted: validated; idle = camera-only right stick, firing = original native aiming with TPS orbit paused.
- V0.84 R3 grenade hold, 100ms release gap, 200ms second pulse and V0.86 custom key assignments.
- On-foot TPS aiming, right-stick rotation filtering, strafe/movement flags and original camera view.
- Mounted horse sprint speed/stamina, Horse Everywhere v0.89 conditional region checks, player stats, FOV, DOF, HUD, cursor, intro/logo, graphics adapter, noncumulative log.
- Separate independent issue: cyan aiming beam + semicircle candidate shader CRC32s documented in `V0_90_STRIFE_AIM_SHADER_RESEARCH.md`.

No user test required for this documentation. **No new executable/build has been produced**; only create a single meaningful gameplay candidate when the native shot path is grounded and a safe rollback is available. Do not confuse look vector telemetry with bullet trajectory.

## Git organization

- `main`: frozen stable branch; **unchanged** by cleanup.
- `debug`: last development source plus preserved user shader research and current notebook.
- `archive`: full old development commit heads as actual merge parents of its history; index file `ARCHIVE_BRANCH_INDEX.md`, snapshot includes older permanent ZIPs.
