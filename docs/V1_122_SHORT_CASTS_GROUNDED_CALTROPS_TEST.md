# V1.122 - Shorter accepted gestures and a single grounded Caltrops gesture

## Base, user steering and install

Cumulative V1.121 `f22f8f86d4c64ada8b0a2b4090ca16d22466cf35`, accepted ability reference V1.112 retained selectively. User explicitly requests installation and another approach, then clarifies: preserve Geyser/Stoneskin gestures and particles, shorten activation gesture only. V1.121 was installed with a verified V1.120 backup, personal INI byte-identical, no PAK/save change or game launch. Private install receipt retained in workspace; this does not establish runtime validation of V1.121.

## Objective and changes

Keep the selected presentations, finish their gesture sooner, and avoid reintroducing the rejected Geyser pose sampler or Caltrops source-flag/End-splice workarounds. In-game success requires smooth endings, no vertical bob/hop, normal movement and jump afterwards, intact native cost/damage/effects. Candidate not declared validated.

- Geyser: same complete Strife_Aether_Channel_Start, one private segment over1.75s rather than2.3667s. No held frame, pose evaluator, added ending clip or per-frame update. Native montage/sequence untouched. All retained particle cues and blast retain exact source times. Extracted supported source proves last particle0.719241s, blast0.488067s, nova0.459302s, all before1.75s. Unknown later cue refuses shortening before construction, never silently drops/re-times effects. Native cancellation-state starts0.789254/1.052338s retained; only duration/end link clamped to1.75s so no state extends past montage end. Same private minimum fade0.18s. Idle fallback without known owned gesture retains full old span.
- Stoneskin: unchanged own Aether gesture, .16/.30 fades, native launch-particle helper, body/cloth material mapping and status/restoration. Activation normally1.50->1.25s; native montage scale/rate preserved. Launch follows the gesture phase; particle asset, display filters and particle lifetime unchanged. Status was already applied natively before montage and keeps its original independent duration. War Stoneskin/Stinger unchanged.
- Caltrops: select the exact typed, weak-identity validated War_Channel_End as the entire one-shot animation (~.666667s), rather than appending an End to Start. Source has only known footstep notifies and complete native return. Reuse already-discovered asset/validator without another object scan or loader. Missing/expired/wrong receiver leaves V1.121 fallback intact. Exact native constructor, blend .10/.18, one projectile spawner, ownership, authority, failure/reentrancy and sound checks retained. Native Blueprint flight/fall/velocity are not changed. A grounded authored gesture is a visual hypothesis; it does not prove that a lifecycle-triggered bob is gone.

Clone's accepted Start/End helper, Rampage, WorldEnder, camera/controller/horse/appearance/menus/updater/config remain byte-identical. No new native hook, memory/VM cursor guard, physics write or external game memory access. This is a separate visual approach to the earlier rejected movement/VM guard; that guard stays absent.

## Checks, test and rollback

65 Windows x64 suites on GitHub only. Expanded Geyser tests require original damage/cue times, full same gesture1.75s, exact cancellation start with clipped endpoint, no source mutation and refusal of a later unknown effect. Caltrops tests cover grounded getter, missing/stale/wrong asset/thread/receiver fallback, native play rate/one spawn/source and physics preservation; Clone's complete-return helper tests remain. Stoneskin test checks1.25s plus scaled native source variant, while native War and failure/ownership paths stay native. Two static reviews and package/updater/Defender checks required; no gameplay result inferred from fixtures.

1. Strife Geyser stationary and moving, TPS/native: same gesture/effects, shorter smooth ending, damage and movement/jump afterwards.
2. Strife Stoneskin: shorter same activation, same body/cloth coating and launch visual, protection/coating expire normally. War regression.
3. War Caltrops: compare one-shot grounded gesture and final body pose at rest/moving, native spawn/damage/cost/sound and normal jump afterwards. Log distinguishes selected versus V1.121 fallback.
4. Quick accepted Clone/Rampage/WorldEnder regression; P2 last. Persisting Caltrops bob remains OPEN, without declaring native flight/fall fixed.

INI212 keys/values and55 Hotkeys unchanged. Exactly four root files in cumulative NoCompression ZIP, all186 prior archives and main/archive/v1.43 refs preserved. Keep personal INI. V1.121 and preinstall V1.120 backup remain available for rollback. Compilation GitHub only; installation only under user's explicit request and only with the game closed.
