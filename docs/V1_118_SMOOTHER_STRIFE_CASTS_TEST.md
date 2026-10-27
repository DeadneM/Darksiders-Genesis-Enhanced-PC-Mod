# V1.118 - Softer Strife casting transitions

## Base, status and objective

Cumulative V1.117 (5878450b61212d2d68470a1db99c76272c2d4db3), retained gameplay V1.112 plus later explicit acceptances. Latest user: Appearance tab present; War Clone OK. Preserve those and accepted Rampage. Geyser and Stoneskin described as a little abrupt: interpretation is gesture transitions, not new duration/stat requests. Caltrops still hops; OPEN. Body/cloth artwork and library variants were not specifically certified in this report. Coop last.

Success: Geyser static return and Stoneskin activation/end feel less abrupt while gameplay/camera/input/status/cost remain accepted. No claims of user validation until tested. GitHub x64 only, no local build/install or game launch.

## Narrow implementation

GeyserStaticPose keeps the native private pose/curve extraction, root0 exclusion, ownership/skeleton/weak identity checks, live-curve preservation and moving/jumping passthrough. Start0.85s unchanged. Return span0.50 ->0.65s (completes1.50s, previously1.35s). Weight ramp0.12 ->0.22s with smoothstep t*t*(3-2*t), zero slope at both endpoints. No montage/callback/ability finish/movement/jump writes, no time extension of the native ability. Only rendered bones1..79 change, stationary only.

Stoneskin Strife retains exact 1-second activation, the same Aether gesture and native CreateSlotAnimationAsDynamicMontage entry. Only native blend-in/out arguments change from0.10/0.18 to0.16/0.30s. Play rate, sequence length, loops, trigger, status lifecycle, sounds, private mineral/cloth materials and original skin restoration unchanged. No new return clip or control release patch. Native War Stoneskin and Stinger remain untouched.

Caltrops, ForeignMontage (including the validated Clone Start/End return), Rampage, World Ender, skins/materials, appearance UI/renderer, camera/controller/XInput/dash/horse and updater code unchanged. 212 INI keys/values and55 Hotkey IDs unchanged. No new hook or runtime VM/movement patch in this release. [Separate Caltrops review](V1_118_CALTROPS_REVIEW.md).

## Verification and tests

65 existing Windows x64 suites required. Geyser fixture now checks completed return, half-blended rendered bone (no snap, root unchanged), eased onset, live curve header/value preservation, alias refusal, movement/intent/velocity/jump passthrough and actor/movement/ability byte identity. Stoneskin native factory fixture checks softer arguments and unchanged1-second rate/loop/trigger, original montage preservation, failure lifecycle and native War forwarding. No new test suite added solely for constants. CI is not animation/gameplay proof.

Later grouped user check: Strife Geyser standing, then moving and jumping after; Strife Stoneskin activation and return with/without moving, status/skin restoration; quick Clone/Rampage regression. Caltrops remains knowingly open, do not announce this ZIP as its fix. Rollback preserved V1.117 ASI, personal INI retained. P2 remains deferred.
