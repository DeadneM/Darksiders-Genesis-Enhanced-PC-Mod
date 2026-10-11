# V0.82 | Experimental class-specific targeting UI event suppression

Date: 2026-10-11. User explicitly requests: **hide BOTH `UMayhemTargetingUIComponent` and `UMayhemTargetUiPositionComponent` and let them test**, without touching any of the other validated systems.

## Exact EXE verification (not guesses)
Target: `DarksidersGenesis-Win64-Shipping.exe`, 62,113,280 bytes, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.
- Native `UMayhemTargetingUIComponent` reflected UClass has size **0x150**; metadata reference string RVA **0x27EC6C8**, generated static UClass cached slot **RVA 0x3812DD0** (confirmed by `0x1407D7B00` getter).
- Native `UMayhemTargetUiPositionComponent` reflected UClass has size **0x260**; metadata string RVA **0x27EC780**, generated cached UClass slot **RVA 0x3812DE0** (confirmed by `0x1407D7CB0` getter).
- UE UObject native **`ProcessEvent` RVA 0xAC0630**, verified by native disassembly: `rcx=UObject*`, `rdx=UFunction*`, `r8=params`. Its first 19 instruction bytes are exact-signature checked. Objects store `UClass*` at `+0x10`. Native UClass hierarchy uses `UStruct.FStructBaseChain` at `+0x30` with depth `+0x38`, as independently observed in existing game disassembly.

## Isolation / behavior
This is NOT a global HUD disable, neither a shader removal nor a game-aim modification. It intercepts `ProcessEvent` and checks whether the **live object currently being dispatched** has exactly the given UClass or a subclass. All other objects, including weapon, enemies, physics, player movement, camera, XInput, equipment, reticle, mount, grenade and all gameplay functions, immediately forward to original `ProcessEvent` unchanged. No UObject instance pointers are cached; no actor, projectile, component memory or game property is written.

When TPS enabled AND hold-vanilla-view not active AND the already-existing experimental `TPSHideGroundAimFx` toggle enabled, script event calls on either of the two UI classes are *suppressed* (return before original dispatch). In vanilla mode and when the option is OFF, everything passes through. This includes F5/L3 switching back from TPS and temporary R3 hold. **No class construction, native lifecycle or C++ Tick callbacks are touched.**

This is an experimental **class event gating** test, NOT guaranteed to remove the cyan line and arc: some visuals may be driven by direct native methods, material/shader draws or cached widgets that persist without ProcessEvent updates. Blocking a UI function with an expected return/out parameter could also destabilize it. This is isolated to the two exact named classes, with user-controlled off and easy rollback to V0.81 or V0.78. Do not promote to stable before explicit user validation.

## Test
1. On Strife with the turquoise line + semicircle, enable TPS via F5 or long L3. `ThirdPerson=0` remains new-install default.
2. Under the existing camera experimental options leave `TPSHideGroundAimFx=1`. Compare by setting it to 0 and back to 1.
3. Check separately (a) turquoise straight line, (b) semicircular arc; test shooting, hit impact, and aiming ability.
4. Check if vanilla camera restores all visuals and FPS/controls behave identically.
5. Send log. Look for `TPS V0.82 UI CLASS TEST totals: TargetingUI seen=... suppressed=...; TargetUiPosition seen=... suppressed=...`. If both counts zero, no UE UFunction path was reached, and the test can be conclusively rejected without further blind suppression of native targeting.
6. If any crash or loss of normal gameplay, revert immediately to the previous V0.78 or V0.79 build and report the log. No unsupported EXE allowed (exact signature validation).

## Freeze
Only source file modified: `src/DarksidersGenesisMod.cpp`. Additions in visually scoped native event hook/FX area, one install site next to earlier FX probes, one FX-only shutdown diagnostic. All other source and config untouched; experimental V0.80 forced-visual-hide code is NOT included because the branch descends from V0.81 -> V0.79 instead. CI/README/docs plus archived test ZIP are the only other repository changes.
