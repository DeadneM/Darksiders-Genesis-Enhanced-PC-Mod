# Dodge Recovery Audit

Target executable:

```text
DarksidersGenesis-Win64-Shipping.exe
SHA-256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54
Size 62,113,280 bytes
```

## User-observed problem

After a dodge, the visible dodge animation can appear finished while the player
remains unable to resume normal movement for a short tail window.

This is tracked as **dodge/dash recovery**, not generic movement recovery.

## Rejected lead

`UMayhemPlayerAbilityComponent::MoveInterruptDelaySec` was tested through the
V0.5A Action Recovery hook and produced no visible improvement for this problem.

It remains a possible lead for hit/stun/interruption recovery only.

## Dash ability family

Current binary audit points to the dedicated Mayhem player dash path:

- `UMayhemPlayerDashAbility`
- `UMayhemPlayerGroundDashAbility`
- `UMayhemPlayerDashStartAbility`

Related reflected names include:

- `ForwardMontage`
- `BackwardMontage`
- `LeftMontage`
- `RightMontage`
- `MaxDashSpeed`
- `MinMontageRate`
- `MaxMontageRate`
- `MontageRateIncreaseDuration`
- `DashStart`
- `DashEnd`

## Movement-input lock

The audited player object has a byte at:

```text
AMayhemPlayerCharacter + 0xAAC
```

Dash startup paths set it to 1:

```asm
mov byte ptr [Player + 0xAAC], 1
```

Observed set sites:

```text
RVA 0x5B047A
RVA 0x5B06FC
```

The dash finalization path clears it:

```asm
mov byte ptr [Player + 0xAAC], 0
```

Observed clear site:

```text
RVA 0x59BFBF
function start RVA 0x59BF80
```

This is a substantially better match for the reported symptom than
`MoveInterruptDelaySec`.

## Ground-dash virtual table lead

A Mayhem dash vtable contains:

```text
start/activate : RVA 0x5B0440
tick/update    : RVA 0x5B72B0
finish         : RVA 0x59BF80
```

The same start/finish relationship appears in duplicate generated vtable data,
and both copies resolve to the same update function.

The update function accepts a float argument consistent with a per-frame delta
and delegates to the inherited ability update path.

## V0.6 direction

Do not immediately clear the movement lock at dash start.

The safer experimental design is:

1. observe dash start;
2. measure native dash lifetime;
3. observe the native update on the game thread;
4. release only the movement-input lock slightly before native finalization;
5. keep animation playback, dash velocity, collision and ability finalization
   native;
6. expose the early-release window in milliseconds;
7. fail open when any signature or vtable relation does not match.

This avoids global TimeScale changes and avoids speeding unrelated animations.
