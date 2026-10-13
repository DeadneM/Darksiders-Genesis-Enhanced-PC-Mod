# V1.07 | Charge 0-100x and charged TPS shot LB+Y

Source branch debug only; main untouched. Cumulative from V1.06.

## Native diagnostics from V1.06 user log, 13 October 2026
- User states Synergy appears validated. Actual telemetry: Synergy setter=443, native additive=443, verified local=443, boosted=94. All native local relations kind=4, owner+0x5C0. Specific normal gain 1.500 and multiplied gain 37.500 demonstrate x25 runtime setting, not default x2.
- Wrath=0 native AddWrath events, Ultimate/Chaos=0 native AddChaos events, therefore changes to their multipliers are capability/UI only, not proof of effective gameplay boosts. Preserve V1.06 gain hooks and diagnostic totals.
- Prior TPS CAMERA_FORWARD RT/RB injection: 3,084 samples, 199 on horse, 2,885 on foot. Charged LB+Y not previously detected as fire because game-facing XInput returned early for native wheel LB.

## Controls, wheel invariants, and shot correction
- ConfigStore persists TPSWheelPadMask default 0x0100 (LB) and independent TPSChargedFirePadMask default 0x8000 (Y/Triangle). Both appear side by side in TPS overlay as rebinding combos. New charged pad mask can be OFF=0. It must be different from wheel mask.
- On true foreground in-game TPS and both wheel+charged held, bypass wheel-only XInput return; native LB wheel bit remains as is. A custom charged key maps to native game Y only WHILE chord held, without any keyboard or mouse injection.
- Native look input and look rotation receive charged shot as fire; radial look isolation bypasses only this chord while active. Simulated forward stick RX=0, RY=32767 uses the same validated game-facing CAMERA_FORWARD policy, even if the alternative mounted Vanilla Aim mode is selected; user raw camera still samples original XInput.
- Plain wheel-only LB returns original pad state, retaining player radial selection; R3 still uses selected gear, not wheel. Horse LB+RB native chord remains independently guarded; intentionally do not remap charged input during an active horse chord.
- New telemetry counts chord detections and charge-specific forward-stick injection. Smoke tests: (a) foot LB+Y, (b) horse LB+Y, (c) LB alone plus right stick selecting wheel, (d) R3 hold/stow unchanged, (e) custom X as charged -> LB+X emits native Y, (f) ALT-TAB/off TPS fail-open. User needs to check visual projectile/charged ability direction; XInput injection itself is not proof.

## Charge multipliers
Hotstreak slider and native clamp max100 (min0 unchanged), default2. Wrath/Ultimate/Synergy sliders, manual input, load clamps, native validation min0 max100, default2. Setting 0 aims to disable positive gain through those native callbacks; value 1 matches vanilla. Synergy original SetCurrentSynergy stays pass-through (spends and resets unchanged). Preserved all other values and individual enable toggles. Revision 2139 writes new TPSChargedFirePadMask while leaving all pre-existing user binds intact.

## Risk and rollback
V1.06 continues available in debug/downloads. No changes to Skip Warning (safe disabled), logo/movie initialization, camera baseline, equipment wheel except charged chord exception, horse engine, native projectiles/raycast, Synergy signed ABI or HUD/shaders. Track any wheel Y conflict or mounted camera instability from tests before promoting to main/release.
