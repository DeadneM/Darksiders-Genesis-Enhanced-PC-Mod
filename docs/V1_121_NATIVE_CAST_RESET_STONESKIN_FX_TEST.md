# V1.121 - Native cast reset and Stoneskin launch particles

## Base and acceptance

Cumulative source V1.120 receipt `4f144284641599b7bbab919cd801925a118e55fc`; retained accepted ability reference V1.112 `0eff6f24907822170c908af499e93317a9004683`. User REJECTS all later Caltrops animation attempts and Geyser vertical ending. Latest Stoneskin gesture/duration/coating/status restoration explicitly VALIDATED by user; missing activation particles remain OPEN. Older receipts are historical evidence, not present acceptance. Local V1.120 log confirms the rejected slot flag actually applied; do not mislabel the test unavailable.

## One coherent objective and criterion

Remove layered ending-pose workarounds, restore continuous casting, and complete accepted Stoneskin with its launch visual. Success in game: no Caltrops hop/Geyser vertical bob, full normal movement and jumping afterwards, original damage/cost/cancellation/sound; Stoneskin launch visible without changing its accepted gesture, coating or status. Build/static evidence cannot establish these visual criteria. Coop later.

## Rollback and different approach

- Caltrops boundaries restored from accepted V1.112. Complete War_Channel_Start gesture uses the original native cast span (~2.333333s, rate multiplier preserved), as before V1.108 shortened it to ~0.566667s. No appended War_End, no bAlwaysUpdateSourcePose reflection/flag/records/observer. The accepted native spawner, receiver ownership and failure cleanup remain. Native Blueprint flying/falling/velocity untouched. Longer gesture is intentional for this reset, not claimed visually validated.
- Geyser: before V1.111 pacing, one compatible gesture filled the original 2.366667s montage. Restore that single-segment strategy; keep later accepted callback, damage/particle/audio and interruption-window safeguards plus private 0.18s minimum fade. Remove the held last frame and entire later GeyserStaticPose module, its build/test target, asset scans, lifecycle calls and shared pose-evaluation branch. No global jump/movement restoration or animation Blueprint edits. The rejected V1.113 jump-lock implementation and global movement/VM cursor guard stay absent.
- Clone retains its accepted War_End helper, audio and timing. Rampage/WorldEnder and their gameplay/audio are unchanged. This is selective rollback, not a whole-repository revert losing later validated features.

## Stoneskin missing visual evidence and correction

Cooked War_Atk_Stoneskin(_Molten) sequence duration 1.833333373s. Native particle events at 0.740429044s and ~0.741106s reference P_WarStoneSkin_Base(_NoDust) / P_WarStoneSkin_Molten(_NoDust), unattached and with no donor socket. StonePlay previously copied only later sounds; these particles never reached the private receiver montage.

Add one scoped CopyStoneskinLaunchEffects call before native playback. Exact source montage+sequence/package/layout/known templates and local target ownership checked. Exactly those two particle notifies are queued in the private Strife montage with source phase scaled to clip length. Existing later sounds are preserved; native event filters/weights/LOD/payload retained. No direct/manual firing of both filters, no timer/hook/loader/new native entry. Native dynamic montages have flags zero: validate exact transient Outer and reject CDO/archetype rather than requiring RF_Transient. This narrow helper does not broaden the accepted audio helper. Duplicate preparation is idempotent.

Do NOT transplant War's status notify, Stinger actor spawn, blast/cost/damage, camera shake or surface notify. Native War Stoneskin unchanged. Strife accepted activation duration, .16/.30 fades, status lifecycle, body/cloth material mapping and restoration unchanged. Missing particle contract logs unavailable but retains accepted cast/status instead of disabling the ability.

## Checks and grouped test

65 Windows x64 suites requested on GitHub. Remove obsolete GeyserStaticPose fixture and add StoneCastEffects fixture: exact base/molten events and phase, private flags-zero ownership, source/filter/sound byte preservation, idempotence, invalid socket/template/class/owner/state/thread/alias/layout rejection. Geyser fixture now requires one segment and exact native interruption links without allocating an ending segment. Caltrops fixture verifies original span and one native spawner. Existing accepted control/config suites retained. No local compilation, install, game launch or external process memory write.

1. War Caltrops stationary and moving: ending body movement, normal movement/jump, cost/spawn/sound. Native Strife Caltrops regression.
2. Strife Geyser stationary and moving, TPS/native: complete ending without vertical bob, jump after effects, damage/cancel intact. Slightly slower continuous gesture expected.
3. Strife Stoneskin Base and Molten if available: launch visual, same accepted activation and body/cloth coating, status/coating disappear at native end. War remains native.
4. Quick accepted Clone/Rampage/WorldEnder check. P2 deferred. Report visual defects without marking fixtures as gameplay proof.

All prior ZIPs and main/archive/v1.43 refs preserved; INI/Hotkeys unchanged. Four-root-file cumulative NoCompression ZIP. New candidate remains EXPERIMENTAL until user accepts. Rollback retained V1.120 ASI restores earlier experiment; accepted ability reference V1.112 available in archive. Keep personal INI. Manual install/updater only, no local replacement in this turn.
