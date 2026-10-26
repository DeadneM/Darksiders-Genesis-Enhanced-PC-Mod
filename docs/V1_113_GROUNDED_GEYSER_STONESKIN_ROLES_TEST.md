# V1.113 - Grounded Strife recovery and Stoneskin roles

## Retained base and user acceptance

Base 0eff6f24907822170c908af499e93317a9004683 / V1.112. User explicitly validates Rampage ending on Strife, all three Strife powers on War, and Stoneskin expiration on Strife. Retain these, WingClip, audio/speeds, Lava flash, camera/input and all earlier accepted settings. Geyser is almost perfect: only static airborne tail remains too long. Stoneskin body/cloth render is reversed, and movement resumes before its stretched pose ends. P2 remains last. Rampage opening charge damage on real enemies is still not explicitly compared/confirmed; approval of its ending is not blanket damage validation.

## Evidence before change

Read-only current process PID 30316, QUERY_LIMITED_INFORMATION|VM_READ: native Strife material slots are Strife_Body, Strife_Gear, Strife_Cloth, Strife_Cloth_ncl1_1, then empty Cloth_LP. Source MIDs are already cleared after the user's test. Original base mesh references Strife_Body_Inst_A, Gear_Inst_A, Cloth_Inst_A twice. The user's visual report is authoritative; an index count alone did not establish correct UV mapping.

Native PAK and loaded clips agree: Strife_Aether_Channel_Start 1 s, End 0.8333333135 s, no root motion. Strife_Channel_Start 0.5666666627 s and End 0.6666666865 s are the paired native ground channel gestures. Both ground clips have the root-motion flag but a constant root track (translation key count 1, rotation key count 1, offsets 0/12), so no extracted root delta; no flag or actor transform is edited. The return/ground clips contain only sound/footstep notifies, not ability or projectile callbacks. Packages, skeleton, first track, flags, event classes/counts/times and lengths are guarded.

War Stoneskin/Molten duration is 1.8333333731 s; native broad interruption window starts at 0.8002610803 s, MOVE at 0.9422869086 s. V1.112 used the floating Aether Start uniformly stretched across the full span and copied only sounds. Native status removal is already accepted and stays status-driven. Scalar setter reflected signature: native wrapper RVA 0x1D97290, ParameterName FName at 0, Value float at 8. Scalar parameter records are 36 bytes with global association 2/index -1. Native FSkeletalMaterial is 40 bytes, MaterialSlotName at 8.

## Changes and safety boundaries

Geyser: accepted one-second start unchanged, then actual Aether End at authored speed, then only the grounded final end frame for the remaining timeline. No change to the native 2.3666665554 s duration, 0.4880667329 blast or any sound/damage/control timing. A missing or altered return asset retains the previous guarded fallback; readiness discovery now includes all return assets. Source clips/graph are read only.

Stoneskin: private factory uses the grounded Strife Start, then private native-owned 32-byte segments add the paired End and grounded rest. Visible gesture finishes at about 1.233 s while the original ability/audio timeline remains 1.833 s. Native sound at 0.716946 s stays unchanged. Two native interruption windows are copied, with separate native-constructed notify state objects/enum arrays and freshly empty weak target caches; no shared cache is reset. Native Begin/End signatures at 0x5A8510 / 0x5A8830 and exact reflected Interrupts layout are checked. This lets native MOVE interruption stop the private presentation instead of carrying a frozen full-body pose while walking. No new hook, timer, manual velocity, global action bit or early Finish.

Only the newly added Stoneskin window states are instantiated here. Existing Geyser/Rampage/War beam state reuse is unchanged, as is the remaining P2 audit item. No claim of runtime coop correctness. Native root motion, all retained War power builders, Rampage handling and pose sampler are unchanged.

Material roles: body/gear only, selected by actual native slot FNames, so a slot reorder does not paint cloth. Native cloth keeps its own textures/color; this does not recreate War's molten cloak shader. Copied body MIDs set only Emissive_Power_Base and Emissive_Power_Scroll to zero, preventing War UV masks from lighting Strife skin red. Shared source material parameters/textures are not edited. Base/Molten status selection and parent retention stay native/status-scoped. Failed copy/scalar API rolls back and caches refusal until the observed slot changes, avoiding repeated allocation each frame. Mesh asset identity is also tracked; a changed asset restores native inheritance rather than an old explicit skin.

## Controls and grouped test

GitHub-only Windows build, all existing 62 suites, package/public updater and Defender checks are required. Tests cover exact authored return segments and unchanged blast, grounded Stoneskin duration/audio/windows, deep state copies and independent cached targets, source preservation, body/cloth slot permutations, zero body emission, rollback/refusal caching and accepted expiration/ownership paths. These checks do not validate synchronization, smoothness or final UV appearance in the game.

1. Geyser Strife at rest, then moving, in native/TPS: returns to ground with the spike ending; same attack/damage, no extra lock or jump at return.
2. Stoneskin Strife stationary then while holding movement: shorter ground gesture, normal walking animation after native control recovery, no slide; attacks and a second cast still work.
3. Body/gear look mineral, cloth retains native color rather than stone, no red skin patch; verify status expiration and current armor restore, base/Molten if available. Original War effect untouched.
4. Separately, when convenient, confirm opening Rampage charge damage on an enemy. Ending and the three War transfers are already accepted, not requested again as an exhaustive test.

P2 last. Rollback: affected slots Native or preserved V1.112, personal INI retained. Appearance catalogue is already recorded; Caltrops visual-only selector remains queued after these corrections.
