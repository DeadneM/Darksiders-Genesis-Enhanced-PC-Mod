# V1.115 - Caltrops initial glyph / explosion appearance

## Base, scope and success

Retained gameplay base V1.112 (0eff6f24907822170c908af499e93317a9004683). Current cumulative source starts at 7f31ac5ba1a039d4080e36f92d696c1efab5a3fb / V1.114, explicitly pending user validation. The user says they will verify it later and requests skins now. This is authorization to continue, not validation of jump/pose/Stoneskin appearance. V1.114 code is unchanged. V1.113 remains rejected. Rampage ending, all Strife powers on War and status expiration retain prior user acceptance; P2 last.

First appearance selector: Native/Base/Frost 1/2/3 for Caltrops initial glyph and explosion only. Do not claim the whole catalogue, looping glyph, idle or ground model has been replaced. Success: the user sees selected emitted effects on new casts/explosions, Native restores subsequent native effects, no change in actual damage/radius/core behavior. Fixture success is not gameplay success.

## Evidence before hook

Read-only native main PAK catalogue V1.112 identifies six exact ParticleSystem objects/packages: two initial glyphs and four explosions. SteamRoller level 1/2/3 blast classes also change radius 200/300/400 and DamageEventRecord, so copying those classes is rejected. Ground mesh and idle are shared; native Frost looping glyph is None. Keeping that loop native is an explicit first-lot limitation, not a fabricated equivalent full skin.

Supported EXE SHA256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54 verified from disk before disassembly. The native common PSC factory RVA 0x187E600 takes ParticleSystem template, UWorld, optional UObject Outer, bool auto-destroy and byte pooling enum, returns PSC. Full native factory preserves/reuses pooled components or creates a component and calls native SetTemplate (0x19F3EC0). Both location overloads (0x189A140, 0x189A2B0) and attached (0x189A510) call it. Blueprint execSpawnEmitterAtLocation at 0x1D19930 calls the second overload, so adding only to the existing first-overload melee hook would miss Blueprint glyphs. Attached caller uses world from component+0xF0 and optional Outer from component Owner+0xE8. These arguments are forwarded untouched. Entry fingerprint 48895c24104889742418574883ec60410f.

One new guarded hook at the shared visual factory is used instead of detouring a damage/ability function or changing projectile classes. It substitutes only its template argument, once. World/Outer/auto-destroy/pooling/result keep native semantics. No position, rotation, socket, projectile, collision, status, sound, cost or damage input is touched. Source assets, class defaults and live actor/component fields are not written.

## Implementation and failure handling

Default and invalid values normalize to Native. The default hook path reads the choice and directly forwards; no object scan, new asset load or startup pass. On explicit non-Native selection, the existing local-player game-thread service performs one bounded pass of 512 loaded object records per service interval. Count growth visits appended objects; changed world/selection or expired cache can restart; all six live assets stop further table scanning. Cache uses full package/name/class, weak identity and serial. No synchronous loader or new frame timer.

Substitution runs only on game thread, for one of the six cached exact assets and a fresh <=500 ms local UWorld identity from the existing player context. Both local players share the setting. The scope is Caltrops effects in that local world, not an invented per-projectile owner after the engine has resolved its world. Unrelated systems, other/stale worlds, other threads, unknown or expired source/target assets pass through. Per-effect missing target retains the native original, so partial loaded availability can temporarily yield mixed native/selected effects. Menu reports missing required assets. No native effect is hidden without a loaded replacement.

Native/OFF or shutdown requires no pointer restoration: no persistent source/actor data was replaced. Existing particles finish according to their original native lifecycle; only subsequent creation changes. All transforms and emission properties remain authored by the selected particle asset and the native spawn. Givre is appearance only; no slowing/freeze is granted or removed.

## Configuration and preservation

One new Values key CaltropsAppearance=0, choices 0..4. All 169 previous INI values and 55 Hotkey IDs retained; no shortcut/input change. Compiled defaults, reset/load/save/runtime and shipped INI must agree. The new subgroup is collapsed by default. Existing personal INI remains authoritative and updater retains it; missing key defaults to Native.

V1.114 Geyser/ForeignMontage/ForeignWrath/Stoneskin modules unchanged, as are all accepted power, shot, melee, XInput, camera, horse, arms/holsters, visual unlock and blur paths. Main entry only adds include/init/local-service/shutdown; no other gameplay change. All 179 old archives preserved. No local compilation, installation, game launch or external process write.

## Controls and grouped user tests

GitHub Windows x64 compilation and 64 suites; new fixture verifies all six source roles/four targets, loaded lookup/package guards, Native zero-scan, one native call/identical other arguments/result, byte-identical source/world/object memory, missing/expired replacement, stale/other world, thread isolation and shutdown fallback. Existing configuration test covers persistence, default and invalid normalization. Check 4-root-file ZIP, CRC/AMD64/source bytes, immutable download/updater, Defender/protected refs/old blobs separately. Runtime visuals and frozen stats on enemies are not proven by these fixtures.

1. Keep personal INI. Gameplay > appearance controls > Ability appearances > Caltrops: Native, then Base/Frost 1/2/3. Wait for asset preparation before a new cast. Compare initial glyph and explosion; other native ground/loop effects deliberately remain.
2. Trigger on an enemy: same original damage, radius, core status/cost/ammunition as before; Frost appearance without the core must not add gameplay freezing. Restore Native and create new effects: normal core-selected visual returns.
3. Missing asset warning: original effect remains; do not interpret it as selected skin validation. Native/TPS should agree. P2 remains deferred by user.

V1.114 Geyser/short Stoneskin tests remain pending and can be grouped with this later. Rollback: choose Native, or preserved V1.114 while keeping personal INI. Next: actual rendering feedback and additional catalogue families once their visual-only boundary is proven.
