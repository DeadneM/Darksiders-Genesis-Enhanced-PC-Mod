# Reference PAK Audit

This document records findings from user-supplied community/example PAK mods.
They are used as reverse-engineering references only.

Target game mount paths identify the project as:

```text
../../../ProjectMayhem/Content/
```

All four samples are Unreal Engine PAK version 3 with readable indexes.

## Reference files

```text
ZZZ-Movement_P.pak
SHA-256 46dd237774fbf2584f6ed4c4e0d1e01b1a89d18d41bb4d35527a80b44e8f119f

ZZZ-Horse_P.pak
SHA-256 3719ac840e1d0d7f137c9322580a3abe58cc5cf93d4b1ea97352c66490d3a920

ZZZ-Horsemen_P.pak
SHA-256 e480c25aa8405af2178dfe9e975a94298d4736f8844bfe95163291c57948add1

ZZZ-Gun Shots Higher Damage_P.pak
SHA-256 b6b71743c4411ae4f0d361a385021d7531d3ac39c9b4a2a2adf87a9ad2d5315c
```

---

## 1. ZZZ-Movement_P.pak

Mount:

```text
../../../ProjectMayhem/Content/Blueprints/
```

Contains 8 entries:

```text
Strife_Blueprint.uasset
Strife_Blueprint.uasset.bak
Strife_Blueprint.uexp
Strife_Blueprint.uexp.bak
War_Blueprint.uasset
War_Blueprint.uasset.bak
War_Blueprint.uexp
War_Blueprint.uexp.bak
```

The included `.bak` files are byte-identical-size vanilla references and allow
an exact property-value diff.

### Proven Strife and War deltas

Both characters receive the same changes:

```text
DoubleJumpZVelocity       1000.0 -> 900.0
MaxDoubleJumpDelaySec       50.0 -> 100.0
MaxGlideJumpDelaySec        50.0 -> 100.0
GravityScale                 2.7 -> 2.5
JumpZVelocity             1700.0 -> 900.0
```

These are direct serialized `FloatProperty` changes in the Blueprint CDO data.

### Vanilla movement values recovered from the backups

Strife:

```text
MaxWalkSpeedOverride        950.0
MaxWalkSpeed                950.0
MaxAcceleration            5000.0
BrakingFrictionFactor       200.0
GroundFriction              100.0
BrakingDecelerationWalk    1700.0
MaxCustomMovementSpeed     3000.0
```

War:

```text
MaxWalkSpeedOverride        950.0
MaxWalkSpeed                950.0
MaxAcceleration            5000.0
BrakingFrictionFactor       200.0
GroundFriction              100.0
BrakingDecelerationWalk    1700.0
```

### Relevance

This sample proves that jump/movement tuning lives directly in the
`Strife_Blueprint` and `War_Blueprint` serialized CharacterMovement defaults.

For the ASI roadmap, prefer these concrete properties over generic TimeScale.

---

## 2. ZZZ-Horse_P.pak

Mount:

```text
../../../ProjectMayhem/Content/Blueprints/Player/Horse/
```

Important assets include:

```text
Abilities/MayhemAbility_Mayhem_Jump_Blueprint
Abilities/MayhemAbility_Mayhem_Sprint_Blueprint
Abilities/MayhemAbility_Ruin_Jump_Blueprint
Abilities/MayhemAbility_Ruin_Sprint_Blueprint
MayhemHorseCharacter_Blueprint
MayhemBlast_Horse_Standard_Blueprint
```

A `MayhemHorseCharacter_Blueprint.uexp.bak` is included, allowing exact
vanilla -> mod comparison.

### Proven horse deltas

Common horse changes:

```text
GallopSpawnSpeedThreshold          300.0 -> 400.0
StaminaRecoveryPercentageRate       15.0 -> 100.0
StaminaTotalRecoveryPercentageRate  40.0 -> 100.0
StaminaSprintPercentageRate         25.0 -> 0.0
```

One modified horse Blueprint copy also changes:

```text
MaxWalkSpeed       1300.0 -> 1500.0
MaxAcceleration     600.0 -> 700.0
BrakingFrictionFactor 1.0 -> 2.0
```

### Observed horse ability values

These values are present in the supplied modified assets. They are useful
targets, but without a matching backup they are not claimed as deltas:

```text
Mayhem Sprint:
  PlayerTurnRate                350.0
  RotationRateFalloffMinSpeed   200.0
  RotationRateFalloffMaxSpeed  2200.0

Ruin Sprint:
  PlayerTurnRate                350.0
  RotationRateFalloffMinSpeed   200.0
  RotationRateFalloffMaxSpeed  2200.0

Mayhem Jump:
  RunSpeed                     1000.0

Ruin Jump:
  RunSpeed                     1200.0
```

### Relevance

The user's requested **horse sprint duration** should not initially be modeled
as an arbitrary duration timer.

The strongest proven native control is:

```text
StaminaSprintPercentageRate
```

A duration multiplier can be implemented as a stamina-consumption multiplier:

```text
effective drain = vanilla drain / duration multiplier
```

Examples:

```text
1.0x duration -> 25.0
2.0x duration -> 12.5
5.0x duration -> 5.0
infinite       -> 0.0
```

Horse speed should independently target `MaxWalkSpeed` / sprint ability speed
rather than sharing the stamina-duration control.

---

## 3. ZZZ-Horsemen_P.pak

Important assets:

```text
Blueprints/Main/MayhemPlayerController_Blueprint
Blueprints/Main/MayhemPlayerState_Blueprint
Blueprints/Strife_Blueprint
Blueprints/War_Blueprint
Data/MayhemPlayerCharacters
Data/PlayerCharacterDamageTable
```

The Strife/War assets can be compared against the vanilla backups supplied by
`ZZZ-Movement_P.pak`.

### Proven Strife deltas

```text
GlideDurationSeconds          3.0 -> 4.0
MinGlideSpeedXY             400.0 -> 800.0
MaxGlideSpeedXY             500.0 -> 1000.0
BrakingDecelerationFlying  1700.0 -> 1400.0
```

### Proven War deltas

```text
GlideDurationSeconds          2.0 -> 4.0
MinGlideSpeedXY             400.0 -> 800.0
MaxGlideSpeedXY             500.0 -> 1000.0
BrakingDecelerationFlying  1700.0 -> 1400.0
```

### Useful observed player/data values

The supplied modded assets expose:

```text
MayhemPlayerCharacters:
  BaseDamage                  25.0
  BaseWrathDamage             30.0
  MaxJuice                    60.0
  MaxWalkSpeedOverride       950.0

Strife:
  HotStreakJuiceDecayRate      0.5
  IdleJuiceDecayRate           0.0
  IdleJuiceJuiceDecayDelay     5.0

MayhemPlayerState:
  BaseDamagePercentagePerEnhancement 50.0
```

These are observed values only unless a matching vanilla backup is available.

### Relevance

This sample gives direct names for the player damage/juice systems and supports
future work on:

- melee/base damage;
- Hotstreak/juice behavior;
- movement defaults;
- glide tuning.

---

## 4. ZZZ-Gun Shots Higher Damage_P.pak

Contains 124 entries and replaces a large set of Strife DualPistols ability and
projectile Blueprints.

Representative projectile assets:

```text
MayhemProjectile_Standard_Blueprint
MayhemProjectile_chargedPistol_lv1
MayhemProjectile_chargedPistol_lv2
MayhemProjectile_chargedPistol_lv3
MayhemBeamProjectile_BeamShot_Blueprint
MayhemProjectile_LavaShot_Blueprint
MayhemProjectile_NatureShot_Blueprint
MayhemProjectile_GravityShot_*
MayhemProjectile_StaticShot_*
```

### Proven target property names

The modified projectile CDOs expose direct properties:

```text
Damage
BaseJuice
DamageInterval
bApplyImpulseOnDamage
```

Representative values in the supplied mod:

```text
MayhemProjectile_Standard_Blueprint:
  Damage      10.0
  BaseJuice    1.0

MayhemProjectile_chargedPistol_lv1:
  Damage      50.0
  BaseJuice    2.0

MayhemProjectile_chargedPistol_lv2:
  Damage      25.0
  BaseJuice    3.0

MayhemProjectile_chargedPistol_lv3:
  Damage      50.0
  BaseJuice    4.0

MayhemBeamProjectile_BeamShot_Blueprint:
  Damage      25.0
  BaseJuice    2.0
```

There is no vanilla `.bak` in this sample, so these values are **not claimed
as exact vanilla -> mod deltas**.

### Relevance

For pistol damage, the most direct target is now proven to be the projectile
Blueprint `Damage` property, rather than a generic global damage multiplier.

For Hotstreak charge, `BaseJuice` on projectile Blueprints is a strong
candidate for per-hit/per-projectile meter gain. This must still be validated
at runtime before being treated as the final Hotstreak-charge control.

---

## Roadmap impact

These reference mods materially improve the ASI roadmap.

### Movement speed

Prefer runtime control of the same values serialized by the player Blueprints:

```text
MaxWalkSpeed
MaxWalkSpeedOverride
MaxAcceleration
```

The previous generic GetMaxSpeed-return hook is no longer the preferred design.

### Jump height

Direct targets:

```text
JumpZVelocity
DoubleJumpZVelocity
GravityScale
```

### Horse speed

Direct targets:

```text
MaxWalkSpeed
MaxAcceleration
```

Sprint ability data should remain separate.

### Horse sprint duration

Primary proven target:

```text
StaminaSprintPercentageRate
```

Recovery controls:

```text
StaminaRecoveryPercentageRate
StaminaTotalRecoveryPercentageRate
```

### Pistol damage

Primary target:

```text
Projectile Blueprint -> Damage
```

### Hotstreak charge

Strong candidates:

```text
Projectile Blueprint -> BaseJuice
Strife Blueprint -> HotStreakJuiceDecayRate
Player data -> MaxJuice
```

These three controls represent gain, decay and capacity respectively, pending
runtime validation.

## Important limitation

The reference PAKs do **not** currently identify the user's common post-action
~0.5 second movement lock.

The Movement sample changes jump/glide variables, not action recovery.
The supplied examples should therefore guide movement/damage/horse/Hotstreak
features, while post-action recovery continues as a separate native-state audit.
