# V1.40 - user defaults, ammo validation and unresolved Action Recovery

## V1.40 built, checked and installed - user defaults / ammo validated (2026-10-15)

**CONSTRUIT / CONTROLE / INSTALLE.** Base V1.39 debug474bbfa. User **VALIDATES ammunition behavior**, while **Action Recovery remains unresolved** (last test12132 ticks,8927 local montage observations,zero shortened tails/finishes). This defaults release adds no native recovery intervention and claims no recovery repair. Source86f773cabf0c2a2523c36ffb83b4127538a3e4f6; [Windows x64 CI38807877488](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38807877488) **SUCCESS,11/11 CTest suites passed**. The new real config test compares all **110 shipped INI keys** with compiled missing-INI generation and reset output, checks runtime settings and preservation of explicit older overrides. Archive publication f88880fe8a618b3ad5070170d26613d41d16b07c.

[Download cumulative V1.40 TEST ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.40_USER_DEFAULTS_AMMO_VALIDATED_TEST.zip), **811138 bytes**, SHA256 `662BAD16053DA496AF6495AFCBDA306B5EC57AA9493FFE429E17BB0279FBD77A`. Exact four root files, PE AMD64 binaries, embedded ASI1.40 identity and full packaged INI values equal to the saved user profile. CI/repository/direct-download hashes match; all **102 previous archives**, main2bc3122 and archive10e0f22 verified unchanged.

Saved profile now defines new installs, reset and Default buttons: user's menuVK_C0, debug update checks, ammo reductionON/divisor2, Unlimited togglesOFF, gains4/4/4, Ultimate camera0.30/+80, F1-F9 gauge/Unlimited actions and F12 HUD. Existing explicit personal INI values remain authoritative; legacy missing-Ultimate keys still inherit their normal camera. All native ammo/recovery/input/camera/horse/gauge/warning/updater implementations are identical to V1.39; only defaults, descriptive status and build identity changed. The user-selected defaults are incorporated; **no new in-game claim is made for V1.40 or Action Recovery**.

Installed with game closed, complete backup `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v140-20261015-231525`, matching installed binary hashes and **personal INI preserved byte for byte**, SHA256 `A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D` before/after. Installed README refreshed with this receipt; immutable ZIP retains its source-preparation README. No automatic game launch. Next: retain the validated ammunition path and investigate the real recovery lifecycle/eligibility before another intervention. [Defaults, runtime evidence, compatibility, rollback and TODO](V1_40_USER_DEFAULTS.md).


2026-10-15, debug only. Base V1.39 debug474bbfa4e7e0d89df6f559621280d627ec90f254. **CONSTRUIT / CONTROLE / INSTALLE**, verified receipt above; Action Recovery remains unresolved. User explicitly validates ammunition behavior: "pour les munitions cest parfait". User reports Action Recovery still seems ineffective and asks to use the values saved in the game for both ASI defaults and shipped INI, then push GitHub.

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
