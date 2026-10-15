# V1.40 - user defaults, ammo validation and unresolved Action Recovery

2026-10-15, debug only. Base V1.39 debug474bbfa4e7e0d89df6f559621280d627ec90f254. **Source prepared / EXPERIMENTAL build**, CI/package/install receipts to follow. User explicitly validates ammunition behavior: "pour les munitions cest parfait". User reports Action Recovery still seems ineffective and asks to use the values saved in the game for both ASI defaults and shipped INI, then push GitHub.

## Source of the defaults

The current installed INI was copied to a read-only audit snapshot before editing the repository. SHA256 **A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D**; it contains 110 keys in 10 sections. The shipped `config/DarksidersGenesisMod.ini` reproduces all values and current bindings from that snapshot; normal Git line-ending conversion may change its byte hash. No game save or game configuration outside the mod INI is read or changed.

| Setting | V1.39 shipped | User / V1.40 default |
| --- | --- | --- |
| Overlay.menukey | Insert | VK_C0 |
| Updater.beta | 0 | 1 |
| Features.reduceammoconsumption | 0 | 1 |
| Values.wrathchargemultiplier | 2.000 | 4.000 |
| Values.ultimateformchargemultiplier | 2.000 | 4.000 |
| Values.synergychargemultiplier | 2.000 | 4.000 |
| Values.tpsultimatedistancemultiplier | 0.150 | 0.300 |
| Values.tpsultimateheightoffset | 0.000 | 80.000 |
| Hotkeys.f1 | ToggleHUD | RefillAllGauges |
| Hotkeys.f2 | MovementSpeed | RefillUltimate |
| Hotkeys.f3 | ActionRecovery | RefillSynergy |
| Hotkeys.f4 | None | RefillWrath |
| Hotkeys.f5 | ThirdPerson | UnlimitedAllGauges |
| Hotkeys.f6 | None | UnlimitedUltimate |
| Hotkeys.f7 | None | UnlimitedSynergy |
| Hotkeys.f8 | None | UnlimitedWrath |
| Hotkeys.f9 | None | UnlimitedAmmo |
| Hotkeys.f12 | None | ToggleHUD |

Reduction is ON with divisor2, Unlimited ammo OFF, all three Unlimited gauge toggles OFF. Normal TPS camera remains distance0.15, pitch-12, height0; Ultimate camera now defaults to distance0.30 and height+80. Wrath/Ultimate/Synergy natural gains default to4; Hotstreak stays2. Menu token VK_C0 is the user's saved Win32 key, not a hardcoded game action. Beta selects debug update checks and retains the existing explicit-install confirmation flow.

## Compiled defaults and compatibility

`DefaultSettings.h` provides the changed scalar defaults to Store construction/reset, load fallbacks, runtime initialization and individual UI Default buttons. `kDefaultHotkeys` matches the requested F1-F12 profile. Full TPS reset includes Ultimate0.30/+80. F1 fills all gauges, F2 Ultimate, F3 Synergy, F4 Wrath; F5 all Unlimited gauges, F6 Ultimate, F7 Synergy, F8 Wrath; F9 Unlimited ammo; F10/F11 None; F12 HUD. Menu/default-help text is updated accordingly. Action IDs and binding parsing are unchanged.

Existing explicit INI values and rebindings remain authoritative. ConfigRevision2140 is retained so no migration reassigns an existing profile. The earlier compatibility rule for legacy INIs **missing Ultimate camera keys** remains: inherit their existing normal camera values. Clean/no-INI generation and explicit resets use the requested0.30/+80 defaults; no existing explicit camera key is overwritten on load.

## Runtime evidence and truthful statuses

The [V1.39 feedback log excerpt](logs/V1_39_AMMO_RECOVERY_FEEDBACK.log) records ammo ready1,99 setter observations,93 spending calls,53 modified requests and one bounded Unlimited top-up with matching readback. The user validates the ammunition feature; its implementation/policy is unchanged in V1.40. This is not a claim that every weapon and game mode has been individually tested.

Action Recovery has ready1,12132 ticks,8927 local montage observations, **blendTails0 / normalFinished0**. That proves the V1.39 recovery operation did not run in this test. It does not identify which later eligibility check rejected those observations, nor prove the character's actual blocking duration. The user reports no visible improvement. The UI now states **No effect in last test / Unresolved** instead of presenting hook readiness as functional recovery. Its source, native hooks, conditions and delay are unchanged, and its enabled flag is preserved exactly as in the supplied profile. **No Action Recovery repair is claimed in V1.40.** No risky hook or broader forced-MOVE bypass is added merely to force a result.

## Build objective and verification

Objective: the compiled defaults, freshly generated INI, shipped INI and Default controls agree with the user's requested profile, while the user-validated ammo implementation and previously validated camera/input/gauge paths stay identical. Success criterion for this build is actual Windows C++ config generation/save/reload equality for all profile keys, user override preservation and a checked four-root-file cumulative package. This is a defaults build, not an Action Recovery fix.

Keep the ten earlier suites, update the intentional new default assertions, and add UserDefaultsConfig: generate an absent INI through the actual Store code; compare every section/key to the shipped profile; check runtime values, preserve explicit older menu/channel/ammo/camera/gain/hotkey overrides, then reset and compare again. Retain the old missing-Ultimate inheritance test. Independently verify native modules and full XInput/RawInput/native ray code against V1.39 and all102 older ZIP hashes. Build Windows x64, confirm real GitHub Actions completion and publish a unique V1.40 ZIP with exactly ASI,dxgi.dll,INI,README at the root. main/archive must stay unchanged.

## Rollback and cumulative TODO

Retain all older archives and the full pre-install four-file/log backup. Restore the saved personal INI if explicitly resetting/replacing configuration; an ordinary install preserves the current personal INI byte for byte. No automatic game launch.

- **VALIDATED by user:** ammo reduction / Unlimited behavior in the reported V1.39 test; earlier manual fillers, three natural gauge gains, Unlimited Wrath, Skip Warning and accepted ordinary TPS/input behavior remain preserved.
- **UNRESOLVED:** Action Recovery after dash and chest. Next research must distinguish its rejected eligibility stages and the real action/animation/input-control lifecycle before adding a new intervention; a zero-application hook is not a repair.
- Keep separately bounded earlier confirmations for secondary projectile impacts and exact Ultimate recenter scenarios; no new claims from this defaults release.
