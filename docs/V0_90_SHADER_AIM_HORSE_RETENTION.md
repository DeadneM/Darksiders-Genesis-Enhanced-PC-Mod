# V0.90 | Aiming visual pixel shader suppression + live mounted horse region retention (EXPERIMENTAL)

Date: 2026-10-11. Base: **V0.89**, plus the archived shader research. `main` stays unchanged; new work goes only to `debug`; `archive` retains legacy branches and prior V0.89 ZIP.

## Input evidence

User's `DarksidersGenesisMod(20261011-182029).log` and in-game observation: horse can be summoned in a ban zone but disappears soon after; the old **1600ms** region permission expires. Log counters show `allowedChecks=17463`, `scopedOverrides=806`, `desummonChecks=17085`, `scopedOverrides=531`, `serverSpawn=4`; `HORSE -> FOOT` transitions repeatedly occur around 1.6-1.9 seconds after summoned inputs. Correlation is strong but a separate mission despawn trigger remains possible.

The user captured these exact ReShade ShaderToggler **pixel shader bytecode CRC32** hashes:
- `1212955419` (`0x484C3B1B`)
- `2294978555` (`0x88CA97FB`)

## Shader suppression

- New separate `src/ShaderAimFeature.cpp/.h`. Reuses existing project-created EARLY D3D11 probe device/context vtables inside `DiscoverAndHookD3D11`, BEFORE the real game creates shaders. No new proxy/loader and no ReShade dependency.
- Hook only `ID3D11Device::CreatePixelShader`, `ID3D11DeviceContext::PSSetShader` and seven D3D11 Draw variants. Compute ShaderToggler-compatible CRC32 over original shader bytecode. Track only the two exact hashes, hold COM references to avoid pointer reuse, and cancel drawing only while a marked pixel shader is bound and selected by the existing `AimIndicatorHideMode` (0 Hide Everywhere; 1 non-TPS; 2 TPS default). Native input/targeting/projectile path **not modified**.
- No broad HUD hide, no draw blocking while shader not captured. This is a **VISUAL TEST**, not a guarantee: these hashes may be shared by unrelated objects; normal hardware/driver recreation could cause a missed capture. Counters in overlay show matched shader objects + suppressed draws. No new configuration toggle: the three existing scope modes remain.
- Shader toggling does not fix mounted Camera Forward projectile aim: the true native raycast path remains separate.

## Horse Everywhere lifetime

- Keep V0.89 H/LB+RB 1600ms local input scope as-is to allow summon attempts.
- Additionally return allowed region / not in desummon region **only while real TPS-mounted camera is fresh and the same current live horse is observed by native HorseCharacter callbacks**, matching the validated local player identity. Both independent getters still invoke their original game functions first.
- Short expiration guards (view <=550ms, horse pose <=800ms, native horse callbacks <=1000ms) restore unmodified values immediately when on foot, horse is no longer observed, TPS suspended or Horse Everywhere OFF; never create game actors, overwrite native horse objects, or keep stale gameplay pointers.
- Mounted retention is presently scoped to **TPS riding** because it has a validated identity+camera proof. Native third-person disabled / cutscenes remain fail-open. Other mission region rules not known: the game may still forcibly remove a horse using a distinct script event.

## Protected validated behavior

Mounted `Vanilla Aim`, on-foot TPS aim, R3 grenade hold/release+stow timings, controller bindings, strafe/rotation, horse speed/stamina/sprint, FOV/camera, menu/HUD, reticle, logo/intro, graphics adapter, noncumulative logging: code unchanged, other than a single early isolated D3D11 hook init/shutdown.

## Delivery/acceptance

Status `CONSTRUIT` after CI passes, then `EXPÉRIMENTAL` until actual gameplay confirmation. A single cumulative Windows x64 ZIP, four files at archive root (dxgi.dll, DarksidersGenesisMod.asi, DarksidersGenesisMod.ini, README.md). Check that (1) both cyan line AND cyan arc disappear under selected scope without hiding important visuals, and (2) a summoned horse remains present while ridden through a previously forbidden area; confirm normal dismount and Vanilla Aim still work. Report one result, not repeated diagnostic sessions. If gameplay regression, restore preserved V0.89 ZIP from `archive/downloads`.

Remaining separate work: mounted Camera Forward true projectile raycast, occasional mouse 360-degree camera input issue.
