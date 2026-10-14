# V1.34 - Complete TPS stick roles / native pause ownership (TEST)

Base: debug 2291e802459e237d6ea3087c5f6838ef2a86667f, cumulative V1.33. V1.33 is not globally validated. User validates wheel sticks in V1.33; V1.32 tested native cinematic and retained TPS in all three wheels remain acquired behavior to protect. New user reports: incomplete stick exchange/Camera Forward, secondary shots sometimes sideways, camera freezes on the wheel control whereas its keyboard equivalent differs, and a turn after firing. Physical button names are examples, NOT fixed native action identities.

## Objective and acceptance

One coherent build: make TPS camera/movement roles consistent with the native stick layout; remove unconditional wheel-button camera pose freezing; keep mouse target requests on the camera ray through native aim lifecycle. Success requires user tests below. Source/build tests are not gameplay proof.

## Changes and exact evidence

- `TpsStickRoles.h` defines physical camera and opposite movement roles once. Camera Left means Move Right. Raw camera polling stays physical; game-facing mapping compensates the game's native look/move swap. Forward injection and idle isolation write the NATIVE LOOK stick, not always physical RX/RY. Camera activity detectors/native look guards follow the selected camera stick on foot and horse. OFF keeps native stick layout; existing forward-fire behavior remains.
- Native retail property `bSwapGamepadLookAndMove` name RVA 0x2746D48, bool setter 0x739770 writes settings+0x230. Settings class getter 0x736620 reads class slot 0x3806F38; engine settings getter 0x1885BE0; the retail function 0x6D9AC0 performs the same getter/type/field read. V1.34 samples this read-only from the live native camera callback. Exact signatures guard calls, native class-tree test guards field access. Until known, new game-facing role filtering fails open. TPS selector remains the explicit physical camera choice; the native checkbox is compensated rather than overwritten. No native game setting is saved or changed by the mod.
- Camera `uiHold` now requires the game's real `IsGamePaused`, not a held wheel binding. With live pose, wheels use normal TPS follow/orbit composition. Only absent pose callbacks may reuse the cached view on the same camera and actor generation. Native cinematic ownership and explicit temporary native view retain priority. No forced actor orientation, clock or artificial pose freshness.
- Mouse helper eligibility no longer ends when a configured physical fire button is released. The two verified native mouse target functions request the same camera-center 3D collision target while the native game continues requesting a target. No release timer, cursor-position steering or mouse XInput emulation. Physical fire state is telemetry only in this provider. Existing local actor/horse, camera freshness, focus, UI, cinematic, temporary-view and active-controller exclusions remain. Real secondary projectile impact and post-shot body orientation are STILL UNVALIDATED.
- Existing `ProcessEvent` probe gains a passive observer for live `UMayhemHUDRadialWidget` instances (class slot 0x380E0B0, native class getter 0x794060). It reads native UWidget visibility 0x14BEA00 on the sampled camera thread only. No new hook, cached-object dereference or UI mutation. At most eight pointer identities/timestamps are held for telemetry. **This visibility signal does not control input/camera ownership:** visibility alone is not proof of an active wheel and wheel coverage is unproven.
- Native `OnAmmoSelectorShown`, `OnEnhancementSelectorShown`, `OnGearSelectorShown` metadata found (getters 0x78A400/0x78A6A0/0x78A700). Their parameter is a 0x50-byte HUD state struct, NOT a boolean open/closed flag. Do not treat it as one or invent close events. Pending: establish authoritative radial open/close lifecycle and replace legacy configurable selection guards after runtime evidence.
- Selection in the wheels retains its configurable guards and native axes; no hardcoded new key fix. Legacy LB-specific exception for temporary controller view now uses the existing radial-selection predicate; keyboard explicit shortcut retains priority. Existing horse and charged-shot native chord semantics are preserved. **Automatic recognition of every in-game rebind is NOT claimed:** wheel/fire legacy bindings still exist, are configurable, and await verified native action replacement. This is a bounded transition, not a completed global input redesign.

## Preserved base / rollback

No config/INI defaults or schema change. HorseFeature, GaugeChargeFeature/Synergy, ShaderAimFeature, skip/update/loader modules untouched. RawInput relative pipeline, Alt-Tab handlers, original native cinematic hook, native targeting/collision implementations and existing R3 hold/stow helper are retained. Scoped controller role mapping is the intentional controller change requested by the user; do not describe all controller code as unchanged. Native UI axes are not remapped. All previous ZIPs and main/archive are protected.

Rollback: quit game, restore the complete local pre-V1.34 backup or the four-root V1.33 ZIP (preserve personal INI). No executable/pak edits. Backup/destination hashes must be recorded at delivery.

## Checks / status

Prepared source: EXPERIMENTAL. Windows x64 compile, actual C++ stick-role matrix via CTest, PE AMD64 identity, exact four ZIP root entries, immutable hashes and closed-game backup/install remain required. Local source audits compare protected modules to base. Do not claim CONSTRUIT or INSTALLE until observed.

## User test / remaining TODO

1. TPS camera Right/move Left, then camera Left/move Right: idle, move, primary/secondary fire, FOOT/HORSE; repeat with native game stick-swap checkbox ON and OFF. The selected physical camera role should remain consistent; native checkbox compensation must preserve movement and forward aim. OFF should retain native layout.
2. Open/select/close all three wheels at keyboard and pad, including a rebound opener. Keep TPS, selections working; live follow should not freeze solely on a held binding. Native pause remains still; scene returns native; existing temporary-native shortcut retains priority.
3. Both mouse shots, hold/release/repeat while standing and moving FOOT/HORSE; compare shot direction and body turn immediately after release. Secondary spread, near-camera collision/parallax and actual projectile consumer are unresolved until measured in game. A native target HIT log is not a bullet impact.
4. Confirm RawInput/Alt-Tab, summon/dismount, R3 use/stow, charged shot, native UI axes and Synergy. Inspect `NATIVE_STICK_SWAP`, `NATIVE_RADIAL` and buttonHeld=0 target logs. If radial observer reports false positives or no events, keep it diagnostic and investigate the real native action; never promote on name/visibility alone.

Delivery evidence will be appended after the actual build and installation.

## Verified delivery and installation

Windows CI [38639339105](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38639339105) completed SUCCESS for source d362076f357306db1d45d727ae21622236bf5f6d. Release binaries, CTest TpsStickRoles (1/1 passed, 0.03s), exact four-root ZIP validation, upload and numbered debug publication all succeeded. Archive commit 77bb0b43e99032dc36a331a3f516bd2ae1f6305e.

ZIP 777132 bytes; SHA256 8D91A23547D30EA335A8E0FD69B27A9CA1E8189183A136438A12179483B8A561. Independent ZIP/PE AMD64/build-string checks passed; direct raw download matches the repository archive and CI hash. Root files: ASI1146368, dxgi154624, INI2677, README242021 bytes. ASI SHA256 934DC62BC91AE9CA75DA3400E761E7677B203D0C94D695A58DE658D70686D2BB; dxgi SHA256 251D5B94E1C20F1AA79274D248923DF610F43D75B981AE854ACDF3400EB9D08D.

Normal authorized installation succeeded after elevated process checks confirmed the game closed. Backup ../audit-v131/install-backup-before-v134-20261014-232427/ contains all four prior files, old log and hash manifest. ASI/loader destination hashes match the verified ZIP. Personal INI preserved unchanged, SHA256 AB2F95CD2AE49AD1706E1E9A420FEEDB1E416D25FCA157E83897554EF2C3E095. Installed README refreshed with post-build delivery notes (archive remains immutable). Package/install verification manifests are kept locally, not game binaries or EXE uploaded as audit evidence.

The user's actual V1.33 log (22:43-23:01 session) shows repeated TPS_UI_HOLD with worldPaused=0/wheelHeld=1, native scene ON22:59:06.596/OFF22:59:13.005 and 3317 scoped mouse rays (2590FOOT/727HORSE), 3266 native hits/51 misses. Confirms the unconditional input-driven pose hold existed and the native target path runs; does NOT establish projectile impacts or V1.34 gameplay success.

main remains 2bc3122c1ae07b7333798462f85c1d9fd3a739f7; archive 10e0f220b85022bc9269e0cb9390a811a3b1cf89. Downloads tree diff from base adds only V1.34; previous archives retained. CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL. User validation and remaining native-action/rebind migration still outstanding as specified above.
