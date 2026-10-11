# V0.91: default-camera Horse Everywhere native ride retention; preserve validated V0.90 shader masking

2026-10-11, cumulative development build on `debug`. Latest user feedback:
- **Confirmed validated:** V0.90 shader suppression makes both Strife cyan aiming indicators disappear, without perceptible loss of other visuals. Frozen implementation `src/ShaderAimFeature.cpp` and existing `AimIndicatorHideMode` untouched.
- **Confirmed works while TPS:** Horse Everywhere retained region bypass while visibly mounted in TPS.
- **Confirmed still broken:** With normal (non-TPS) camera, local summon in restricted region succeeds briefly, then horse vanishes when 1600ms input scope ends.
- **Still broken:** Mounted Camera Forward shot direction. Do not alter validated native Vanilla Aim or synthesize Ctrl or player rotations.

## V0.90 user log `DarksidersGenesisMod(20261011-185838).log`

- D3D11: `matchedPS=2`, `blockedDraws=3220`, `drawCalls=39015568`. The exact two shader hash matches are sufficient to suppress the visual, not to deduce its Unreal component, CPU command source or projectile target function. Further draw-call hooks might add risk, no need.
- Horse: `allowedChecks=27033`, `scopedOverrides=7740`, `mountedHolds=5414`; `desummonChecks=23605`, `scopedOverrides=0`. Region authorization is the active gate in this test; no desummon restriction returned true. Most of the initial 1600ms authorizations are followed by native calls indicating continued riding only in TPS.
- `horseActorGetPlayerLookInputCalls=0`, `forward lookSamples=0`, `projectileHook=NO`, despite mounted Camera Forward active. **Rendering data does not identify projectile trace origin/direction.**

## Native camera retention change

Preserve V0.89 physical H/LB+RB summon scope and V0.90 TPS-mounted scope **byte for byte**. Add a second, separate proof path that does not depend on `thirdPersonEnabled`:
1. Native `SERVER_SpawnHorseMount_Implementation` callback is recorded with actor identity and must match the previously validated local player pointer.
2. A real H/LB+RB summon rising-edge must be observed and be recent; same for the server callback.
3. In the live `HorseCharacter` callback, require a newly observed native horse identity and known current player identity before arming a lease. Only atomic identity/timestamp snapshots. Never retain an owning reference or dereference a cached UObject.
4. Region and desummon hook still call **the original native function first**. If genuine local horse remains the same and it has fresh native callbacks (<= 350ms), allow local region / prevent region desummon, regardless of camera mode.
5. Invalid when: opt-in OFF, actor or horse pointer identity changes, horse callback expires, or validated local foot movement resumes well after original horse creation. Fail-open means original native region answers.
6. In all other respects no game object, input handling, scene shader or camera writes have changed. No artificial mount spawns and no periodic respawn.

Caveat: Native callbacks prove the horse is alive, **not necessarily ridden** in all cinematics/co-op/missions. Player movement resumption is used as a best-effort dismount signal, hence this remains `EXPERIMENTAL`. A distinct mission script may forcibly despawn a horse without consulting these two native region getters. Neither outcome should be claimed as proven without gameplay feedback.

## Frozen baseline

- V0.90 user-validated two pixel shader draw suppressions and three visibility modes.
- TPS mounted native **Vanilla Aim** and camera/orbit interactions.
- On-foot TPS, XInput, right-stick filtering and R3 grenade/stow pulse.
- Horse speed/sprint/stamina, all earlier gameplay boosts and config values.
- HUD, FOV, Skip Logos/Intro, Engine.ini adapter, noncumulative log.
- Main stable branch unchanged. V0.90 validated visual ZIP frozen at `archive/downloads/DarksidersGenesis_V0.90_SHADER_AIM_HORSE_RETENTION_TEST.zip`.

Build: one V0.91 ZIP with exactly four root files. No repeated diagnostics. Most useful check is simply native camera horse retention; shader effect need not be retested except regression smoke check. Camera Forward remains open pending verified game-native mounted projectile trace callback; not part of this patch.
