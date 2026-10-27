# V1.119 - Half a second longer on Strife Geyser and Stoneskin

## Base, exact request and retained acceptances

Cumulative V1.118 f3d7105a80eb0e17fa2ce6b587ac661fe61e151c. V1.118 was installed on the user's explicit request with V1.117 rollback, personal INI byte-preserved. Current log confirms V1.118 casting paths. User now requests half a second more animation for Geyser and Stoneskin and confirms War Caltrops still hops. Do not claim V1.118 Caltrops timing fixed it. Earlier Appearance tab/Clone and Rampage acceptances retained; no blanket promotion of all V1.118 experiments. P2 last.

## Narrow changes

GeyserStaticPose ReturnSpan0.65 ->1.15s, exactly+0.50s. Same ReturnStart0.85s and smoothstep ReturnBlend0.22s. Rendered return ends at2.00s rather than1.50s, still inside unchanged native2.3666665554s montage. Same pure native sequence sample into private native pose/curve context, same skeleton/proxy/weak ownership, live curve preservation and root0 exclusion. Movement/intent/jump/velocity/acceleration passthrough unchanged. No ability duration, callback/damage, movement/jump flag or control lock edit. This lengthens only the already-isolated stationary rendered return; the accepted moving animation path is untouched.

Stoneskin activation uses min(original normalized duration,1.0)+0.5 rather than min(duration,1.0), so it adds exactly0.5 at the existing requested play-rate convention; usual nominal duration1.5s rather than1s. Same Aether clip, blend-in0.16/out0.30, native factory/loop/trigger, source montage and finish delegate. Status applied before animation retains its own native duration, cost, material observation/expiry, sounds and original appearance restoration. Native War/Stinger remain untouched. No appended return animation or early forced finish.

Caltrops remains V1.118 byte-identical: Start0.5667s plus End0.55s, same native spawner/cost/sounds. Latest V1.118 log records War cast return immediately before Caltrops at14:28:40/43 and14:29:07/09. Same hop persists. The previous duration hypothesis is insufficient. A clarification asks whether the whole actor leaves the ground or only the body pose bobs, before choosing another fix. Native Blueprint's flying5/falling3 path remains source evidence only, not an observed component transition. No VM/native movement hook or execution cursor manipulation is introduced; earlier rejected guard is not retried or bypassed. User preference for the simplest retained method remains binding. [Previous source analysis](V1_118_CALTROPS_REVIEW.md).

## Verification and limits

65 existing Windows x64 suites, no local compilation. Geyser fixture checks unchanged half-weight/root at transition midpoint, sample still progressing at old1.50s endpoint, completed return by2.10s, private curves and actor/movement/ability byte preservation, alias refusal and moving/jumping passthrough. Stoneskin fixture checks1.50s nominal rate, same factory/loop/trigger/fades and failure/ownership/native-War behavior. Existing all-policy/config tests retained. INI212 keys/values,55 Hotkey IDs unchanged; no new settings. Static exact-diff review must allow only BuildInfo/GeyserStaticPose/ForeignWrathFeature. Clone/ForeignMontage/Caltrops/Rampage/WorldEnder/materials/Appearance/camera/controller/horse/updater unchanged.

These tests do not certify game visuals. Success later: both requested gestures are0.5s longer and feel right, no frozen/sliding Strife, Geyser moving/jump remains accepted, Stoneskin status and restoration intact. Caltrops remains OPEN and must not be listed as fixed. Preserve personal INI and rollback to V1.118 ASI if desired. No automatic installation or game launch; debug-only delivery, all183 old ZIPs and protected refs intact.
