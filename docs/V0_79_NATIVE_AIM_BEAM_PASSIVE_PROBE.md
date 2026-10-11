# V0.79 | Strife blue aim line and semicircle: passive beam-function probe

**Baseline**: user-validated V0.78 / V0.77 gameplay code is **LOCKED**. The user explicitly requested that ONLY the aiming-effect investigation code be modified. Compared to the V0.78 branch, the only game C++ source file changed here is `src/DarksidersGenesisMod.cpp`, with additions exclusively in the aim-FX detection area: declarations beside the existing ground-effect counters, two independent native function wrappers in the same ground-effect hook section, one install call next to existing ground-effect installation, and one aim-effect-only summary log. NO changes to INI/defaults, native camera, XInput, controller hold, grenade, actor rotation/look, movement, mount, HUD, damage, reticle, or timing.

## Supplied V0.77 log (2026-10-11 14:04 - 14:22)
`TPS V0.77 totals: groundFxCalls=0 alreadyHidden=0 forcedHidden=0`. The actual on-screen turquoise laser-like aiming line and curved semicircle remain present, confirming the original `HideGroundTargetingEffect` is not their active show/hide path.

## Executable research
The exact 62,113,280-byte, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54` shipping EXE contains reflected `ShowBeamEffect` and `HideBeamEffect` methods, adjacent to `DoAimAbility`, `StartAiming`, `StopAiming`, ammo and pistol weapon methods. The UFunction native execution binding table maps:
- `ShowBeamEffect` to **RVA 0x007D17B0**, exact entry bytes `48 8B 42 20 45 33 C0 48 85 C0 41 0F 95 C0`. The wrapper dispatches to the object's vtable `+0x920`.
- `HideBeamEffect` to **RVA 0x007D0040**, matching 14-byte prolog but a distinct jump to a native implementation at **RVA 0x006AD240**. That method dereferences `this+0x6B0` and forwards to a component's visual-state method at **RVA 0x00601760**.
These functions control an actual native beam effect but it is **NOT yet proven** that they control Strife's specific luminous blue line or semicircle. There may be a separate arc mesh/UI component.

## Scoped passive instrument
Install **two signature-guarded MinHook callbacks** for the corresponding UFunction exec wrappers. Every call unconditionally invokes the original with the exact same arguments. Only count total and TPS-active calls and sample log owner pointers on n=1, 120, 1000 and 10000. Installation is fail-open and requires exact EXE validation; if either hook fails, neither remains installed. We **do not hide** anything in V0.79, so comparison with V0.78 demonstrates unchanged rendering and gameplay while collecting proof about the native beam mechanism.

After a short Strife aiming session, the shutdown log line `TPS V0.79 AIM FX PROBE totals: show=... showInTPS=... hide=... hideInTPS=...` determines whether the named native exec path was executed. If counts remain zero, do NOT blindly change native beam visual or projectile functions; look for direct C++ calls or cooked particle assets. If counters move, correlate timestamps with on-screen line and arc, then implement a separate independent opt-in hide of *visuals only*, never suppress player aiming or damage.

This development ZIP must leave `ThirdPerson=0` packaged by default, preserving all V0.78 INI. No official release promotion before explicit visual validation.
