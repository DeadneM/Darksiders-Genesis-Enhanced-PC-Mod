# V1.39 - ammo consumption and native action recovery TEST

## V1.39 built, checked and installed - ammo / native animation recovery TEST (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL.** Base V1.38 debug e39153e, with Unlimited Wrath and Skip Warning now explicitly **VALIDATED by user**, preserving earlier gauge/TPS validations. Source 1a1684a82d861cc5cc3f066879ff7dcf4d787791; [Windows x64 CI38802359996](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38802359996) **SUCCESS, 10/10 CTest suites passed**, including real ammo INI/hotkey save/reload, fractional cost/Unlimited policy and recovery delay resets. Numbered ZIP publication ee59029851ae9549556418f089d6d75b2339ed75.

[Download cumulative V1.39 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.39_AMMO_ACTION_RECOVERY_TEST.zip), **809940 bytes**, SHA256 `4BE85D59CC89B1D11D6AE4341B78B9221520CE1F95DC691FDF5447F76A2F9441`. Exact four root entries, both binaries PE AMD64, embedded ASI1.39/new-feature identities, former forced-MOVE ready path absent, default ammo controls OFF/divisor2 and all older INI defaults retained. CI/repository/direct-download ZIP hashes match. All **101 earlier archives**, main2bc3122 and archive10e0f22 checked unchanged.

Installed with game closed after the user's closure reply, binary destination hashes verified. Complete prior installation/log backup: `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v139-20261015-225133`. **Current personal INI preserved byte for byte**, SHA256 `50274EA27343151CE44A38374F4C585163D96BC06EFA64D42FA69DC7A60AD51C` before and after; no personal camera value, gauge setting or shortcut replaced. Installed README refreshed with this delivery receipt; the immutable ZIP retains its pre-CI cumulative README. No game was launched automatically and **no V1.39 in-game functionality is claimed validated**.

Gameplay -> Combat/resource controls -> **Ammunition (test)**: optional **Reduce ammo consumption** divisor0-10 (0/1 normal,2 half,10 one tenth), independent **Unlimited ammo**, each assignable in Hotkeys -> F1-F12. Existing assignments retained. Player movement -> **Action Recovery** retains its checkbox/action/delay, now testing normal native completion at natural animation blend-out instead of overriding MOVE. Full XInput/RawInput/camera aim/horse/gauge/warning implementations protected against the V1.38 base. **Next test:** finite special ammo OFF/2/10/Unlimited ON then OFF; dash and chest with Recovery OFF/ON(delay0), retained loot, proper locomotion/attacks and no sliding. Runtime counts are evidence of hook activity/completion, not measured user responsiveness. [Native evidence, remaining limits, acceptance and rollback](V1_39_AMMO_ACTION_RECOVERY.md).


Date: 2026-10-15. Branch: **debug only**. Source base: V1.38 / e39153edb578d7861d0a40675c8cb24e63e32876. Status: **CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL**, verified receipt above. Prior numbered archives and main/archive must remain unchanged.

## Accepted base and requested objective

The user now explicitly validates Unlimited Wrath after recognizing the Void hub's native free-ability exception, and validates Skip Warning. Earlier validations of all three natural gauge gains, manual refills, ordinary TPS, wheels, stick roles, mouse/controller separation and horse behavior remain the base. Do not infer separate Ultimate recenter or every secondary projectile test from general praise.

This build adds an optional ammo consumption divisor from 0 to 10, optional Unlimited ammunition, and two independently assignable F1-F12 actions. It replaces the ineffective V0.8B recovery experiment with normal native completion at the natural montage blend-out tail. Success requires user comparison after dash and chest opening, retained loot/interaction completion, proper locomotion, no sliding and normal recovery when OFF. Build/tests alone do not satisfy that criterion.

## Ammo evidence and implementation

Exact supported EXE SHA256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54. All addresses are RVAs.

- Native weapon AUTH_SetAmmo 697D30 calls ranged proxy SetAmmoCount 697E70 at 697D8D, returning to **697D92**. Setter returns an integer, checks authority, preserves -1 infinite sentinel and native capacity, and writes **proxy+34C**. +350 is base capacity, not current ammo.
- Native full-resource refill 6B81E0 iterates character proxy+428/count+430; ranged-proxy owner+108 is the character proxy. Capacity 6A90D0 includes the native ammo-capacity bonus. Type slots: character proxy3810268; ranged weapon proxy3811D40.
- The only ammo hook is the native count setter. Spending arithmetic accepts only the exact weapon caller, fresh live local owned/typed authoritative proxy, finite positive cost and current actor generation. Initialization, pickups, recharge and native infinite ammo are passed through. No cost getter is faked, no weapon is unlocked, no -1 sentinel is installed.
- Integer ammunition needs fractional credit: at divisor10, ten native one-unit costs consume one actual unit, even with duplicate count writes in between. Actor/weapon/generation/config changes reset credit. 0/1 mean normal spending, 2 means half, 10 means one tenth. Unlimited takes priority, replenishes owned finite ammunition to its actual native maximum, and prevents finite spend while enabled. Turning it OFF restores normal spending or the separately selected reduction.
- New options default OFF. Divisor defaults2 and persists in INI. Actions **ReduceAmmo** and **UnlimitedAmmo** are appended without renumbering any existing action or changing any binding. Current personal INI is preserved at install.
- Six exact native signature guards plus spend-call target verification. Ownership/caller coverage and real ammo consumption remain to be validated in game; static evidence is not live spend proof.

## Recovery: historical failures and new runtime evidence

Read [the cumulative recovery audit](DODGE_RECOVERY_AUDIT.md) and notebook before this implementation. V0.5A MoveInterruptDelay had no user effect. V0.6B InputSuppressWindow and V0.7A interrupt-bit probes had zero relevant callbacks. V0.8A forcing MOVE during RUNNING caused chest-animation sliding and remains **REJECTED**. V0.8B's AWAITING_FINISH-only route was ineffective according to the user.

The user relaunched V1.38 and performed dash/chest tests, authorizing read-only inspection. The twelve-second [read-only capture](logs/V1_38_ACTION_RECOVERY_READONLY.json) has MoveInterruptDelay=0 throughout and live local montage abilities ending from RUNNING(2) to FINISHED(5), without an observed AWAITING_FINISH(4) interval. This does not identify the exact full dash/chest timeline and does not prove how much time a blend tail contributes. It does disqualify the assumptions of the old timer/state-only approach for these samples.

Player montage helpers at ability+1D8 have native UMayhemMontagePlayer vtable27BC3B0 and **size38**, montage+28, weak player+30. Earlier exploratory reads at helper+D8/+194 were outside that structure and are deliberately discarded, not treated as recovery fields.

Native proof:

- Player ability Tick68D050 calls the original tick/notifies and maintains input state. Montage Tick5B7620 and interaction Tick5B7650 finish when their helper disappears.
- Native montage callback5A76D0 calls helper IsPlaying676620 and normal EndAbility59B080 when playback is no longer active. IsPlaying waits for FAnimMontageInstance blend target+150 and current+148 to reach zero; this explicitly includes its final blend tail.
- UMayhemMontagePlayer resolves weak player, obtains its anim instance through654180, then finds the matching montage instance with1660F90. Position is+128; current/target weights are+148/+150. SetDesiredValue165A9E0 writes FAlphaBlend+20, matching instance+150. Instance+160 is the interrupted flag used by native Stop169D4F0.
- Native EndAbility handles authority/network state and invokes normal finish cleanup. For local authority, completion normally goes to state5 directly; state4 is not a generic post-animation timer.

New behavior:

- Remove the old IsActionEnabled hook entirely. Hook only player ability Tick68D050, call the original first, then consider typed local player montages in state2 with authority3, matching instigator and player, exact helper vtable, matching weak local owner and fresh matching montage instance.
- Retain native behavior for active core playback, pause, cinematics, interrupted blends, unsupported/nonlocal abilities and malformed fields. Only **natural blend-out** (target0, current positive, not interrupted) is eligible. No input key/button, MOVE allowance, animation speed or global time scale is changed.
- After the configured Recovery Delay (0-500ms from observed blend start), construct native FAlphaBlend(0) with164C6A0, call native Stop(instance, blend, **bInterrupted=false**), then normal EndAbility(reason0) if still running. This runs the game's actual ability/interaction/input cleanup instead of releasing movement while the old action remains active.
- Existing ActionRecovery checkbox, F-key action, INI and delay are retained. OFF calls only native tick. Nine retail byte guards plus exact helper identity protect this path. Logs count real candidate tails and finished states; a ready hook is not called a gameplay success.

Limits: the change trims a genuine native animation blend tail, not an arbitrary duration before that tail. Some animation notifies can lie near the end of a montage; loot, interaction completion, locomotion and attacks after a chest/dash therefore need explicit user checks. No claim that every chest/dash or Ultimate animation uses this path. The short V1.38 read-only capture does not record blend weights, so the new timing/visual effect is **not yet measured in game**. If an animation has no eligible natural tail, it stays native. Do not broaden to RUNNING MOVE overrides to force a visible result.

## Verification and test criterion

Keep the existing seven CTest suites, add ammo arithmetic (single/burst/fractional costs, duplicate writes, OFF, Unlimited ON/OFF, sentinels and identity changes), actual INI/hotkey save-reload/default/range tests, and recovery delay/reset tests. Check all 15 new native signatures and unchanged validated modules/input blocks against V1.38. Windows x64 build, CI and four-file package receipts must be recorded after success.

In a mission with Strife and a finite special ammunition:

1. Both ammo toggles OFF: spend normally. Reduction ON with2 then10: compare the real number consumed over several shots, including a burst. 0/1 should match OFF. Test secondary ammo as available.
2. Unlimited ON from an empty owned ammo supply: verify it fills and fires; toggle OFF and verify costs resume (disable reduction too for the native comparison). Assign each action independently in Hotkeys; preserve existing F1-F8 choices.
3. Action Recovery OFF then ON, delay0: repeat dash while holding movement, then open chests. Require the main animation and loot to work, normal movement/attacks to resume, no gliding/sliding, no repeated chest use. Try a nonzero delay and return OFF. Inspect NATIVE_BLEND_END / normalFinished logs.
4. Non-regression: controller stick roles and all wheels, mouse/horse aim, pause/cinematic TPS ownership, gauge controls, warning skip and Alt-Tab.

## Rollback and cumulative TODO

Disable the new options to return to native ammo/recovery behavior. Full binary rollback uses the pre-V1.39 four-file/log backup or retained V1.38 ZIP; restore the saved personal INI if required. No game is launched automatically.

- V1.39: user-validate ammo path/consumption/Unlimited OFF and native recovery/loot/locomotion; only then mark VALIDATED.
- Preserve earlier unresolved precise secondary projectile/Ultimate recenter confirmations separately from the now validated gauges/Skip Warning.
- Keep debug experiments isolated, all previous numbered ZIPs and main/archive untouched.
