# V1.111 - Native power frame and animation pacing

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40363876283), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0f1a5d47ac05b67ed950d9338fe06d8b7c38a3d0/downloads/DarksidersGenesis_V1.111_NATIVE_POWER_TIMING_FRAME_TEST.zip) - [Recu](V1_111_DELIVERY_RECEIPT.md). Updater debug : V1.111 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

EXPERIMENTAL; GitHub build, package, public updater and Defender checks passed, not validated in game. Source base V1.110 receipt 6e7929162e5280aa688b05e226a37198340209b3. Cumulative accepted gameplay retained. No local compilation, game launch, installation, save/PAK/custom INI edit or runtime process write.

## Latest user result

- WingClip spinning melee effect correction is explicitly VALIDATED. Its implementation is unchanged.
- Rampage shield is now visible, but faces 90 degrees right in both native and TPS. Finishing presentation is much better, with a brief frozen period remaining. Opening damage has not been confirmed by this report.
- Blade Geyser on Strife has an animation synchronization problem. User explicitly clarified the problem is the gesture itself, not a request merely to shorten post-cast input lock.
- Retain accepted power audio/speeds and normal Lava muzzle flash. Startup was subjectively improved. P2 remains deferred.

## Evidence before changes

Native PAK exports compare both VFX_Root sockets: War_Mesh_Skeleton binds Bone_War_Root with RelativeRotation (0,-90,0); Strife_Skeleton binds Bone_Strife_Root with zero relative rotation. Both reference root bone transforms are the same quaternion (0,0,0.70710683,0.70710671), zero translation and unit scale within float tolerance. V1.110 mapped the shield directly to Strife root, losing the War socket's artistic frame. The shield's native timed state is at RotationOffset +0x4C, LocationOffset +0x40 (0,0,120), SocketName +0x38, PSTemplate +0x30; reflected VM_READ metadata and original asset agree.

The private shield now reads the actual original War sequence/skeleton/socket through reflected fields. It requires the exact native socket class/owner/root, finite pure-yaw orientation and zero socket translation. The native socket yaw is composed with the private state's rotation. The vertical-only source offset is validated and preserved. No guessed global -90 correction, camera change or shared socket edit. A fixture changes the source socket yaw to -45 and expects -45, proving the result comes from data. Unsupported altered socket geometry refuses this transfer through the existing native abort path.

Native War_Atk_Rampage_Strike contains interruption states at 0.3766137362 (0.8900529146 duration) and MOVE at 0.5021510124 (0.7645156384 duration), ending at 1.26666665. They lived in the War source sequence under the unrendered motion slot; V1.110 copied only the finite montage's impact events. The private finite plan now includes those native state objects/events with correct linked end time, as already done for accepted beam cancellation states. Landing states are retained by the same native event collector when present. Start's nine charge cues, root-motion segments, loop sections, impact blast class and values stay unchanged. No manual Finish, arbitrary cancellation timer or key-specific action patch is added. Whether the user's remaining freeze was visual or control-related was not separately confirmed; this restores demonstrably omitted native interruption behavior without claiming an in-game result.

Geyser used Strife_Aether_Channel_Start (1 second) uniformly stretched across the native 2.36666656-second montage, while SpawnBlast and the blade effects fired at 0.4880667329. Thus the compatible gesture was only ~0.206 seconds into its own clip at the blast. When available, the gesture now plays once at its own rate 1, followed by Strife's own idle for the remaining native span. All original War Geyser montage event times stay unchanged; the owned gesture and its own sequence cues now use their natural pace. The original War source's MOVE state (0.7892537713, duration 1.5774126053) and other native interruptions (1.0523383617, duration 1.3143284321) are retained with correct start/end links across the two compatible segments. Exact native field/schema and owner guards remain.

The extra private segment uses the supported executable's existing 32-byte TArray reserve helper at RVA 0x256970, fingerprint verified. Offline body audit shows capacity calculation, size-32 allocation and native Realloc (0x83ED00); no constructor, gameplay or external call. This avoids CRT memory handed to Unreal, out-of-range sequence sampling or using the 184-byte event allocator for a different type. The helper is optional: an unavailable fingerprint keeps the prior guarded one-segment fallback rather than disabling accepted unrelated powers. No new asset loader or discovery scan.

## Preservation, checks and limits

Only ForeignMontage.cpp, BuildInfo and focused regression fixtures change under source/tests. WingClip/primary melee/Lava, ForeignRampage ownership/phase/pose sampler, other power handlers, camera, inputs, XInput, HeavyLanding and configuration remain unchanged. Native slot/RootMotionMode and source non-alias checks remain essential. Original sockets, sequences, montages, notifies and all 175 previous Git ZIP blobs are preserved. Local quarantined V1.108 is neither restored nor staged for deletion. main/archive/v1.43 must remain unchanged.

Fixtures check data-derived socket yaw and source preservation, unsupported source frames, all five Rampage phases including empty-event landing, finite state spans and exact blast pointer, normal-speed Geyser/idle split, unchanged native callback time and duration, crossed segment end links, and accepted other powers. GitHub Windows x64 compile, all existing suites, package CRC/AMD64/four root files, updater public download and final Defender scans are required before delivery. These checks are not proof of visual quality, opening enemy damage, flawless blends or P2 behavior.

## Grouped user test and next step

1. Strife Rampage native/TPS: shield faces forward and follows the character; test turning, releasing, wall, recast. Check opening charge damage on enemies if possible. This remains open until observed.
2. Finish Rampage while holding movement: compare the brief freeze, normal movement/attack and final pose. No persistent shield or command lock.
3. Strife Blade Geyser native/TPS: gesture now at natural pace relative to the spikes, same visible attack/damage and successful second cast. Verify movement/cancellation after the native recovery window; if still offset, describe whether gesture or spikes lead.
4. Briefly preserve the validated spinning melee/primary/Lava and other transferred power audio. Full P2 testing remains deferred.

If abnormal, return the affected slot to Native or use preserved V1.110. User installs through debug updater, retaining personal INI/hotkeys/saves/PAK. Next build follows this grouped feedback; no complete fix or damage comparison is claimed from static evidence.
