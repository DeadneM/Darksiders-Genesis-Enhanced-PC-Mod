# V1.115 - Verified cumulative debug delivery

**CONSTRUIT / CONTROLE ; EXPERIMENTAL, NON VALIDE EN JEU.** Base of accepted gameplay V1.112; cumulative V1.114 retained unchanged and explicitly pending user test. User requested skins while deferring V1.114 validation. V1.113 remains rejected. Prior Rampage ending, three Strife powers on War and Stoneskin expiry acceptance is preserved.

Source `fd278264d07822dfcc66e45117c0de9e45524060`, archive `979ce9420809716c83f6ab7c17074592746db481`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40452540269) **64/64 suites**, zero MSVC warnings/errors. Native five-argument factory ABI and template-only forwarding fixtures pass; configuration persistence/default/invalid-value tests pass. No runtime appearance/damage validation and no local compilation/install/launch.

[Immutable ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/979ce9420809716c83f6ab7c17074592746db481/downloads/DarksidersGenesis_V1.115_CALTROPS_BURST_APPEARANCE_TEST.zip): 2214166 bytes, SHA256 `DC3FD66A468FD67F1FF12F3A6A0A7D9D4A6A01BB8401C2FEA4E1B8B97616A8F4`. Exactly four root files, NoCompression, CRC valid, ASI/DXGI AMD64; README/INI matched compiled source. ASI SHA256 `F7611F6075DDA40CCB1F5F515BF01D8C1FB75D80E6535704B5AB1D75289976EB`. 170 INI keys: all 169 previous values unchanged, one CaltropsAppearance=0. All 55 Hotkey IDs unchanged. All 179 older archive Git blobs preserved (180 total). Protected main/archive/v1.43 unchanged. Local missing V1.108 quarantine not restored/deleted in Git; unrelated audit preserved.

Debug updater public rules select this ZIP, blob `5a747e170bc0fe7d871b65b97c458de18eaac541`; immutable and updater download bytes match HTTP 200 and SHA256. No in-game updater UI interaction claimed. Defender ZIP/payload custom scans with DisableRemediation: exit 0/no threats, realtime/on-access/antivirus enabled, no protection or exclusion change. Platform 4.18.26090.9, engine 1.1.26090.9, signatures 1.461.216.0; diagnostic result, not universal certification.

## New appearance option and limits

Gameplay > appearance controls > Ability appearances (collapsed by default): Caltrops Native/Base/Frost 1/Frost 2/Frost 3. Only the initial glyph and explosion for newly created effects. Ground model, idle and looping glyph remain native, including native Frost's absent looping glyph. This is not a complete skin swap or all catalogue families. No core, freeze status, damage/radius/cost, projectile class, progression or save change.

Six package/name/class-checked native ParticleSystem assets, loaded objects only. No startup scan on Native and no synchronous load. Missing selected asset retains the native effect; partially loaded targets may leave mixed initial/explosion appearance. Setting applies to Caltrops templates in fresh current local worlds for both local players, not a claimed new per-projectile ownership system. Worker threads/unknown worlds/assets are untouched. Existing emitters finish normally; Native restores subsequent creation.

One exact-fingerprint visual factory hook, source EXE hash and all three native/Blueprint creation routes inspected before implementation. Only template argument substituted; world/Outer/auto-destroy/pooling/return forwarded once. No CDO, asset, actor, component, transform or gameplay field edits. V1.114 source modules and accepted camera/XInput/melee/abilities/arms/holsters/horse/blur/Leviathan/updater implementations unchanged. No revised animation or Stoneskin fix is being silently validated in this release.

## Later grouped user tests

1. Keep personal INI. Try Caltrops Native, then Base and Frost 1/2/3, wait for preparation and cast again; compare initial glyph/explosion. Trigger an explosion near an enemy if possible.
2. Damage/radius, cost and equipped core behavior must remain native. A Frost appearance alone must not grant freezing. Restore Native and create a new effect: original appearance returns. Missing-asset warning means fallback, not successful selected-skin validation.
3. Separately keep the pending V1.114 Geyser jumping/stationary-versus-moving and short Stoneskin/material tests for the user's later session. Coop remains last.

Rollback: choose Native, or preserved V1.114 ASI with personal INI. Next: actual rendering feedback and additional visual-only catalogue families, not a false claim that every indexed skin is delivered. [Implementation and boundaries](V1_115_CALTROPS_BURST_APPEARANCE_TEST.md).

Evidence: [CI](evidence/V1_115_CI_CHECK.json), [package](evidence/V1_115_PACKAGE_CHECK.json), [updater](evidence/V1_115_UPDATER_CHECK.json), [Defender](evidence/V1_115_DEFENDER_CHECK.json), [two source reviews](evidence/V1_115_STATIC_REVIEW.json), [native particle contracts](evidence/V1_115_PARTICLE_CONTRACTS.json).
