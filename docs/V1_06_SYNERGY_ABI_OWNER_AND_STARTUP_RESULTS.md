# V1.06 | Synergy ABI repair, exact local owner resolution and warning-screen observations

Date: 2026-10-13. Repo DeadneM/Darksiders-Genesis-Enhanced-PC-Mod. Branch debug only, main unchanged. Target exe SHA-256: 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54, 62,113,280 bytes.

## V1.05 user log `DarksidersGenesisMod(20261012-224823).log`
- Startup: all three gauges ON x2, vanilla camera normalHeight=0. Both camera defaults invoked and logged. Two independent native camera defaults correct on source, no reported gameplay regression.
- Native Synergy 75 setter callbacks: 37 positive requests. Player actor address `0x0000021234A199A0` while Synergy `this` `0x000002121989D060`: identical but distinct reference over the session, so V1.05 local pointer equality rejects 100%. First positive delta 0 -> 1.35, next 1.35 -> 2.70; max 500, proof these are legitimate resource updates. Need prove actor/controller ownership; simply matching quantity/timing cannot prove local player.
- Wrath and Ultimate native AddWrath/AddChaos observed 0. They may be script/RPC-driven or actions not exercised; do not call working. Preserve gain toggles ON and separate diagnostics.
- Camera-forward 1352 HORSE, 42 FOOT injections, 7 R3 full presses and 6 second stow pulses, 2 aiming shaders captured. No code touched.
- Startup screens: records=3, stride=0x40, byte +0x38 values 0/2/2; enabled +0x39 1/1/0. Exactly TWO active definitions, consistent with visible warning count but not proof of rendering or resource text.

## Critical native Synergy ABI finding
Exact game `RVA 0x583040`:
```asm
movss xmm0,[rcx+5A4]
...
ucomiss xmm0,xmm6
je unchanged
call 0x7C21C0
movaps xmm0,xmm6
movss [rbx+5A4],xmm6
...
ret
```
A real direct caller at `RVA 0x7C52B3` executes `call RVA 0x583040`; `RVA 0x7C52C2` executes `movss dword ptr [rdi],xmm0`. Other native code at `RVA 0x65A1E4` tail-jumps to 0x583040 after adding Synergy. So `float (*)(void*,float)` is correct, V1.05 `void` was INCORRECT; restore it in V1.06. A compiler accepting the signature did not guarantee UE ABI correctness.

## V1.06 ownership test
Read-only, bounded checks performed only while in live native Synergy hook:
1. exact same local pawn pointer (original V1.03 filter),
2. Unreal UObject::OuterPrivate chain at offset +0x20, up to 5 verified-readable UObject headers,
3. **Verified native Synergy owner's character getter**: RVA `0x56BE70` executes `mov rax,[rcx+0x5C0]; ret`. In the real resource-gain caller at `RVA 0x67E944–0x67E95B`, the engine obtains holder in `rdi` and calls `0x56BE70(rdi)` to obtain player actor in `rsi`. The same holder is passed to native Synergy gain calls at `0x67EA9F` and `0x67EAD7`. Therefore `holder+0x5C0 == g_localPlayerCharacter` is an authoritative ownership test, without object memory scanning.
Match is cached for identical native owner/player pair; invalidated if either address changes. Logs relation `kind=4` for verified native owner, `kind=2` for UE Outer, `kind=1` for direct equality, `kind=0` for unverified. Positive gains multiplied only for verified relations. Broad actor-field scanning was eliminated. Remote/co-op holder remains unchanged. Native float return forwarded unchanged.

## Exact additive gain location (supersedes absolute setter boosting)

Using the same supported game EXE: `RVA 0x65A1A0` starts with bytes `40 53 48 83 EC 40 0F 29 74 24 30` and has the Windows x64 calling signature `float (void* manager,void* owner,float factorA,float factorB,float factorC)`. It calls current Synergy getter RVA `0x56D430`, multiplies XMM2 by XMM3 and the fifth float from stack, adds to current, then jumps to setter RVA `0x583040`. The native game calls this helper at `0x67EA9F` and `0x67EAD7`. A direct native check of owner+0x5C0 proves local character association.

V1.06 now installs another exact-signature native hook **AddSynergyNative** at RVA `0x65A1A0`, multiplying only factorC when finite positive native gain and proven local owner. The existing SetCurrentSynergy hook becomes a **strict passthrough observer**, preserving absolute resets/spends/replication and float return ABI. Prior idea of scaling positive delta at absolute setter was unsafe because the game can reapply stale or reduced values. New log `Gauge V1.06 ADD_SYNERGY`, `Gauge V1.06 additive totals`; no claim of proven gauge fill before an in-game test.

## Skip Warning
Existing V1.05 passive startup records confirms native code and +0x39 flags. Old skipping the entire definition loop may delete required text/UI setup, linked to cursor crosshair bug. V1.06 does not alter original loop or screen processing. Next step trace actual Slate/widget painting or display delay once screen identities validated; selective render-hide only if no lifecycle mutation.

## Test notes
After installing V1.06 root ZIP, check `Gauge V1.06 SYNERGY relation`, `Gauge V1.06 owner totals` and `Gauge V1.03 totals`. In both Strife and War combat, test charge of each resource separately. No success claim until UI gauge visibly changes versus x1 and matching boost telemetry. Ensure reticle and warning screens still normal. Keep rollback V1.05.
