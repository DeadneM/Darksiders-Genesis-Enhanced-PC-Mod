# V1.120 - War Caltrops source-pose continuity

## Base, user evidence and objective

Cumulative V1.119 a88d2cdd532d939b831c6dfcbef380741b1779fb. Preserve its requested Geyser return1.15s (+0.5) and Stoneskin nominal activation1.50s (+0.5), still awaiting the user's separate timing verdict. Earlier Clone/Appearance/Rampage acceptances retained. Latest user clarification: surtout un mouvement du corps, sans deplacement reel. This supports a presentation diagnosis; it does not measure capsule/Z motion or certify a specific animation-state cause. Prior Caltrops End0.55s timing did NOT fix the bob. Stop duration-only retries. P2 last.

Objective/success later in game: no small body hop when transferred Caltrops finishes on War; native movement, jumping, cost/spawn/damage/sounds and other powers remain accepted. One scoped native animation setting, no extra generic correction layer.

## Source evidence and hypothesis

Read-only offline extraction of WarAnim_Blueprint shows TPS slot AnimGraphNode_Slot_E905C13C481BC22BAA7A96B6C1C1C9C3, Source LinkID160, bAlwaysUpdateSourcePose=false. Native graph source can stop updating under a full-weight montage; on reappearance a stale/reinitialized locomotion state can produce a visual rebound. This is a candidate explanation for the user's body-only report, not a trace proving the state machine did so during that cast.

Epic UE4.27 documents [AnimNode_Slot](https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/AnimNode_Slot?application_version=4.27): its source can continue updating when this option is enabled. This documentation supports meaning only, not the game's exact ABI. The supported game's prior VM_READ slot-schema observation records AnimNode_Slot size72, Source offset16, SlotName32 and bool40. Current runtime reflection must independently match exact class/field GUID, StructProperty72, AnimNode_Slot ScriptStruct, SlotName NameProperty8 at32, native BoolProperty1 at40, byte metadata [1,0,1,255], and TPS FName. Unknown layout/bitfield/value retains previous presentation and logs unavailable. No guessed blind offset.

## Implementation and lifecycle

ForeignCaltropsFeature owns two small continuity records (one per local player), reused by the existing successful ground War Caltrops adapter. After native montage playback and one confirmed native spawner, revalidate the owning local actor/world/index/controller/mesh/animation/ability and capture weak identities. Only native false0 becomes true1 on the identified instance's TPS source flag. Already-true native values are not owned or reset. No CDO/shared asset setting changes. Start/End durations, play rate, montage, sounds and projectile remain byte-for-byte retained paths.

The existing game-thread ForeignWrath observer services cleanup before any configuration/discovery early return. Keep updates through the ability's native flying phase without modifying that phase. Restore only the owned animation's same reflected flag, if still true, when the cast finishes, owner/world/context/pose changes, mesh/animation is replaced, or game-thread shutdown. Expired/reused identities and unexpected flag/native ownership changes are not written. Reset records after cleanup. No new per-frame object scan, timer, allocator, hook, loader, input or callback. Only a guarded instance render/update flag is written by the ASI when run; this agent performs no external game memory write or native call.

Native Caltrops Blueprint flying/falling/velocity, Native Strife, all other actors/powers and movement remain untouched. This is not the earlier rejected global movement/VM execution-cursor guard, and that method is not retried or bypassed. Cosmetic source-pose continuity is the safer alternative following the user's clarification. If this does not remove the bob, keep it OPEN and inspect native evaluated pose/state timing before another change.

## Verification and limits

65 existing Windows x64 suites, expanded Caltrops tests: exact scoped false->true, native flying untouched, restore at finish/world/owner/form loss, native true preservation, unsupported field size/bitfield/slot/value fallback with successful spawn, expired identity/no stale write, unexpected native flag replacement, P2 record isolation and game-thread shutdown. Existing constructor/play/spawn counts, source/actor/CDO byte preservation, reentrancy and failure lifecycle still required. Test fixtures cannot certify the actual visual result or that the runtime bool metadata was accepted on a user's executable. Logs distinguish applied from unavailable.

No change to ForeignMontage/Clone, Rampage, World Ender, Geyser/StoneSkin animation or materials, Appearance, camera/XInput/dash/horse/updater. INI212 keys/values,55 Hotkey IDs unchanged. All184 old ZIPs and protected refs preserved; local unrelated quarantined V1.108 deletion/untracked audit untouched. Compile GitHub only; no local installation/game launch.

## User test and rollback

War with transferred Caltrops: cast stationary twice, moving once, and compare the final body pose. Confirm no hop, normal movement/jump afterwards, native cost/spawn/damage/sound. Test Clone/Rampage briefly for regression. Geyser/Stoneskin timings from V1.119 remain included and pending their own verdict. Coop later. Rollback V1.119 ASI with personal INI retained; normal cast-end cleanup restores only the owned instance flag. If unavailable or bob persists, report it honestly, do not announce this experimental candidate validated.
