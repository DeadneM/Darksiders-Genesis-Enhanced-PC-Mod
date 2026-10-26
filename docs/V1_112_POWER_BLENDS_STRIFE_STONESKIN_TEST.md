# V1.112 - Power blends and Strife Stoneskin

## Base and acceptance

Source baseline ad3aaa547028a7da6a5b7d6bf1be5a42db52afd3 (V1.111). User validated WingClip V1.110, transferred power audio/speed and Lava muzzle placement. V1.111 endings are NOT validated: Rampage/Geyser Strife and Clone/Caltrops/World Ender War end abruptly. The V1.110 remaining movement freeze was explicitly a control lock, not just a visual pose; this build does not invent another interruption window or shorten native gameplay duration.

## Evidence and changes

VM_READ capture v212-stone-materials-20261026-215109.jsonl, PID 32252, installed V1.111 verified by ASI SHA256. War at 56.797 s has StoneSkin SteamRoller3, Molten2 and Stinger1 statuses. At 57.531 s its three body overrides become War_Inst_StoneSkin_Molten_B/A/A. At 66.781 s overrides become null and statuses expire. Strife at 72.141 s has the same three statuses but no override; at 82.156 s statuses expire. The user's grouped test was performed and acknowledged; it is diagnostic evidence, not validation of this new implementation. Native Stoneskin material cleanup explicitly casts to War and ignores Strife.

Native reflected AnimMontage BlendOut is AlphaBlend size 48 at +0xE0, BlendTime float at +8. Only RF_Transient private montages receive a minimum 0.18 s, preserving longer fades. Original assets and control windows are unchanged. Geyser's one-second gesture remains authored speed, then its last 1/30-second span is held on the same sequence instead of jumping to Strife_Idle; total duration and blast at 0.4880667329 s stay native. War World Ender likewise holds its final cast frame through the existing 0.2 s tail, then blends to the graph. Its complete fire interval and callback times stay as V1.108/V1.111.

Rampage: AnimInstance active-map position lookup can disappear before the native blend is complete. The owning list (+0x78) retains the FAnimMontageInstance: montage +0, position +0x128, current blend weight +0x148. Observer reads this bounded list and publishes only the matching private montage. Pure pose sampling now blends by the engine's weight, preserving the native graph root. Ability lifetime no longer cuts the visual fade; no animation update, notify dispatch, movement, Finish, timer or root motion is added. Removed the now-unused active-map getter and visual ability identity.

Stoneskin body visuals use existing game-thread observer and existing bounded asset discovery. No new hook, external asset loader or recurring global scan. Exact native reflected APIs: CreateDynamicMaterialInstance, GetMaterial, SetMaterial and K2_CopyMaterialInstanceParameters, with parameter offsets and native wrapper RVAs checked. ProcessEvent is the mod's existing supported boundary. Private MIDs retain each original effective material as Parent, keeping it strongly reachable during the effect. Copying uniform parameters does not replace Parent. Null/inherited versus explicit overrides are restored separately, only if the current slot is still our MID. Failed copy rolls back the factory-installed MID. Invalid owner/serial/mesh/world/form cannot redirect the write. War and Stinger remain native. True status presence selects lifetime and Molten variant; no invented ten-second timer.

Important visual limit: Strife materials use MM_Basic_Character, War's stone variants use MM_Basic. Uniform parameter names Base_Color, Normal, Metal/Rough/Emissive, Fresnel and emissive/tint are shared. The engine copies these into a private MID retaining Strife's own parent/shader. War textures have not been authored for Strife's UVs; fit, coverage and Molten appearance remain EXPERIMENTAL. We do not claim a perfect native Strife skin from static checks. No permanent save/skin selection change.

## Preservation and limits

Camera, XInput/input, aiming/projectiles, accepted primary/spinning melee, Lava muzzle, HeavyLanding, War visuals, Stinger, INI/hotkeys/updater and protected refs untouched. Existing source interruption-notify state reuse remains a pre-existing coop audit item; no P2 runtime validation is claimed, and P2 tests remain deferred. No new multiplayer readiness claim.

Compile/tests only on GitHub. Focused tests exercise native fade weights after ability end, no time reset, source montage preservation, private blend-only changes, continuous gesture segments, material factory rollback, original inheritance/explicit override restoration, native skin replacement, transformations, weak identity expiry and separate per-player state. These are structural controls, not proof of smoothness or UV appearance.

## Grouped test and rollback

1. Strife Rampage + Geyser, native/TPS: continuous gesture and soft return, movement/attack/recast, no persistent lock; shield direction and charge damage still need confirmation if not already tested.
2. War Clone/Caltrops/World Ender: smooth ends at accepted speed; correct damage and sounds, no laser sound/effect left behind.
3. Strife Stoneskin: visible body coating, base and Molten if equipped, disappears at true expiration. Verify current armor restored, including after switching character/mount/ultimate, then cast again. Original War Stoneskin and Stinger unchanged.
4. Brief accepted WingClip/Lava smoke test. P2 deferred.

If abnormal, return affected wrath slots to Native or install preserved V1.111, keeping personal INI. This version does not activate an unvalidated recovery feature. Next: inventory original appearance/model/material/core variants; only then consider user-selectable visual overrides.
