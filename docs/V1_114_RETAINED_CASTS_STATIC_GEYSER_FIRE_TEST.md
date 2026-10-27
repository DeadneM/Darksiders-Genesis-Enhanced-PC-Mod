# V1.114 - Retained casts, stationary Geyser and native Stoneskin fire

## Retained base and rejection

Latest retained gameplay base: V1.112 / 0eff6f24907822170c908af499e93317a9004683. V1.113 is REJECTED by the user: Geyser permanently prevents jumping even after the spikes finish; movement already worked before that experiment. The grounded Stoneskin gesture was wrong. Desired render is mineral body with burning regions including scarf and parts of the body, not zero emission or unchanged native cloth. This report is user evidence; the precise cause of the persistent lock was not independently captured.

User VALIDATED Rampage ending on Strife, the three Strife powers on War, Stoneskin status expiry/restoration. Preserve them. Opening Rampage charge damage on enemies is still not explicitly confirmed. Coop last. V1.113 remains archived for history, not recommended as a working baseline.

## Objective and success criterion

Restore Geyser jump/movement behavior from V1.112 while shortening only its stationary displayed tail; restore the original Stoneskin activation gesture with shorter casting and status-independent duration; render native body/cloth material roles without suppressing native fire. Success requires user gameplay confirmation: repeated jump after Geyser, unchanged moving Geyser, short recognizable Stoneskin cast without sliding, mineral/fire visual split and proper expiry.

## Implementation and boundaries

ForeignMontage.cpp/h and its tests are restored byte-for-byte from V1.112. The rejected return segments, ground gestures, recovery registry and additional native interruption-state objects are removed. Original Geyser duration, callbacks, cost, damage, sounds and interruption timeline are unchanged. There is no attempted global reset of jump/action bits and no key-specific patch. A restart after replacing the ASI is required; no live repair of an already locked process is attempted.

The new small GeyserStaticPose observer uses the existing owned Strife root evaluation hook after native evaluation; validated Rampage has priority. It samples only Strife_Aether_Channel_End through the already-supported pure native pose extractor, with guarded exact package/name/length/root flag/skeleton. It does not update animation, execute notifies or move the actor. Root transform is preserved. A 0.18-second visual blend starts after the retained one-second gesture and follows the native montage weight. Pure rendering is the only destination written; actor, movement and ability memory are not written.

Eligibility is Foot, owned weak-identity-checked actor/mesh/animation/ability/montage/movement, matching world, active ability, native walking mode, and zero current/last movement input plus negligible velocity/acceleration. Held direction against a wall counts as movement. Native compact-pose ABI, root vtable, skeleton, full 80-bone map, buffers and fresh snapshot are checked. Unknown curve-bearing contexts refuse the visual sample. It may therefore safely retain the previous stationary pose on an unsupported context; logs distinguish refusal from actual sampling. No claim of successful in-game stationary adjustment yet. No extra hook, loader or recurrent object scan; discovery joins the existing bounded scan.

Stoneskin uses the original V1.112 Aether casting sequence and CopyLaterSounds path. Only adjusted play rate changes: activation min(native duration, 0.75 s), retaining the supplied ability rate. Native status was applied separately, and continues on its own accepted lifecycle. No manual Finish/timer/status shortening or new return animation. The source/target players' other power builders are unchanged.

Visuals now create private MIDs whose parent is the actual native War StoneSkin A/body or B/cloth material, base or Molten selected from actual status. Previous copying of only uniforms did not preserve static shader switches. Previous forced zero emission is removed. Exact Strife_Body/Gear/Cloth slot names select roles, including the secondary cloth slot. Native A/B parent retains its own textures, static fire/scroll switches and parameter values; shared materials are unchanged. No core/stat effect is forced to obtain fire. Base B naturally differs from Molten B; appearance follows the native variant. War UV masks may not fit Strife perfectly: this is explicitly experimental, not a bespoke Strife texture.

Before substitution the effective original must equal the skeletal asset's strongly referenced default slot material. Unsupported arbitrary overrides are refused. The new MID is strongly held by the owned mesh override; original stays held by its skeletal asset. Factory failure rolls back only its own/source replacement and caches refusal, avoiding per-frame material allocations. Existing weak identity, status ownership, restoration-only-if-ours and changed-mesh inheritance protections remain. No game assets are modified.

## Validation and retained scope

GitHub-only Windows x64 compilation with 63 suites is required, plus public ZIP/updater selection, 4 root files, architecture/CRC/bytes, prior archive hashes, protected refs and Defender checks. New fixture verifies stationary render, original root preservation, movement/intent/falling passthrough, unchanged actor/movement/ability bytes, completed ability/expired asset refusal. Material fixture verifies full A/B shader parents, no uniform copy/emissive writes, source preservation, role reordering, refused unsupported overrides, one-time creation and accepted status cleanup. Fixtures are not game visual or jump validation.

ForeignRampageFeature, ForeignCaltropsFeature, ForeignPowerFeature, ForeignBeamFeature, MeleeEffect, NativeTpsFeature, HeavyLanding, RuntimeSettings, config and updater remain unchanged. PistolVisual only adds the Geyser dispatch after native evaluation and Rampage. All 178 old ZIP Git blobs, main/archive/v1.43, personal INI/bindings and synced sources are preserved. No local compile, installation, game launch or external process writes. Existing shared interruption-state reuse in other power paths remains a deferred P2 audit item; the new V1.113 states were removed.

## Grouped user test

1. Install V1.114 via debug updater, restart normally. Strife Geyser at rest: shorter displayed tail, then jump repeatedly after spikes; no permanent lock. Repeat Geyser while moving and in TPS/native: retain the previously working movement/aim behavior.
2. Stoneskin at rest and while holding movement: original Aether activation, short cast, normal walking after control returns; status lasts normally and repeated activation works.
3. Stoneskin mineral body/equipment with native burning regions including scarf for the corresponding variant; no stone scarf/red-only body reversal. Verify expiry restores the current armor. Original War unchanged. If UV fit still fails, report it rather than claiming native source inheritance proves final visual correctness.

No repeat of all accepted features; coop later. Rollback: original Native slots or preserved V1.112 ASI while retaining personal INI. Caltrops visual-only selector remains queued after these regressions.
