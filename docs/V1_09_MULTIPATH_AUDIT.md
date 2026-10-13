# V1.09 | Multi-path native diagnostics without unproven game rewrites

## User's current observations
V1.08 appears aligned for LB+Y without Wrath; *executing* charged shot when Wrath is available fires in a wrong direction. Native analog stick forwarding **differs**: R1/R2 forwards 0/32767 to game, charged chord neutral 0/0 because LB+virtual UP navigated weapons wheel in rejected V1.07. V1.08 log confirms chord=1333, look overrides=6108, rotation overrides=0, ordinary stick forwards=373, Wrath=0, Chaos=0, Synergy boosted=57. This does NOT prove the charged ability reads GetPlayerLookInput at firing/spawn.

## Four coordinated read-only observations
1. `TPS V1.09 PAD EDGE`: sample physical input on original XInput poll (code's actual mode enum: 0 idle, 2 R1/R2, 3 LB+Y, 4 LB wheel), raw right-stick values and trigger/button bits. Capture input onset tick.
2. `TPS V1.09 LOOK PHASE`: in already hooked GetPlayerLookInput, before rewriting, log original direction, TPS camera yaw, gap, direct native return/caller RVA, and elapsed time since button onset. Compare charged vs normal actor game source. Existing look rules remain unchanged; no new projectile hooks.
3. `TPS V1.09 WRATH READ-ONLY`: at most once every 85 ms **inside verified local native look callback** with guarded VirtualQuery spans, inspect exact-game AddWrath candidate (actor+0xEA0 pointer to component, +0x3A0 float). Track rises/falls and time since charged input. Do not assume the candidate is confirmed or directly write resource. `Gauge V1.09 STATIC`: enumerate direct near CALL instructions in executable PE sections to known Wrath, Chaos and Synergy gain targets once at startup. Distinguish native direct callers from indirect Blueprint calls; counts do not imply runtime execution.
4. `StartupScreens V1.09 READONLY`: bounded 64-bit field fingerprints for each 0x40 native record. Keep +0x39 active flags 1,1,0 unchanged. Does **not** dereference text pointers, modify record or skip original lifecycle.

## Validation requests
- TPS foot, Wrath empty: hold LB+Y and aim; then gain enough Wrath and shoot fully triggered LB+Y in several directions.
- Ordinary RT/RB in same camera angle, then native wheel LB-only with physical stick. Send FULL single-session Mod.log, including shutdown totals.
- Gain Wrath and Ultimate in-game, ideally show UI before and after. Need compare Wrath candidate deltas vs native hook counts.
- Verify two startup warnings and reticle still look vanilla (skip remains OFF).
- New logs are bounded, no user settings reset, existing Synergy x0..100 and Hotstreak unchanged, old builds preserved. No mouse TPS work.

Known unresolved: actual charged projectile aiming function; Wrath/Chaos reliable gain path; warning display-only skip without breaking startup; last mouse TPS. Branch debug only, main untouched.
