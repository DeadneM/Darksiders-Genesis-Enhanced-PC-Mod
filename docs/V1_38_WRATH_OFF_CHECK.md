# V1.38 - Courroux OFF check: Strife in the Void

2026-10-15, debug only. Base remains V1.38 / 6b0554f010ac8bbbef6f836538bb5ce64561075b, compiled source729f747. No new executable build or gameplay patch in this investigation.

## User report and discriminator

User reports that Wrath seems unlimited even with Unlimited OFF, and explicitly clarifies that abilities remain usable at zero. Subsequent direct answers identify **Strife (pistols)** and **the Void hub with Vulgrim/mission selection**. The Void grants free Wrath ability use independently of this mod toggle; the gameplay guide describes this explicitly for Strife Shadow Clone: [The Void guide](https://www.neoseeker.com/darksiders-genesis/The_Void). This is gameplay-context evidence, not a claim to have located and executed every native cost rule. User confirmation of the zone plus the log explain this reported test; they do NOT validate Unlimited OFF inside a mission.

## Actual log evidence

Read-only copies: ../audit-v131/user-v138-wrath-unlimited-off.log and final snapshot user-v138-wrath-unlimited-off-final.log. Correct V1.38 session20:02:39, shutdown starts20:29:00.

- 20:02:49: native observed Wrath20 ->5 before Unlimited testing, so ordinary spending was observed earlier in this session.
- 20:06:13.168: F8 Unlimited Wrath enabled=0; Ultimate and Synergy were also individually disabled moments earlier. There is no later Wrath auto-fill before the next explicit ON.
- 20:07:01.595: explicit manual Set Wrath0%, native readback0/20 matches. 20:07:06.534 independent read-only gauge observer also sees0, and no later change is observed until the explicit combined ON20:27:39.835.
- 20:27:39.841: first and only UNLIMITED_TOPUP gauge1 in the session,0 ->20, after that combined ON.
- Shutdown totals: holds=0/5316/0, topups=1/2/3 for Wrath/Ultimate/Synergy. **Zero Wrath setter hold overrides in the entire session.** This counters the hypothesis that the new Wrath hold kept forcing a full meter during the reported zero-meter period. It does not instrument every native ability/cost decision.

## Source/native checks

Both the sampled top-up and final native setter override read the current independent atomic flag; disabled returns the original request. Config Save publishes those flags immediately; debounce delays only INI persistence. No native ability permission/cost override is present in the Unlimited feature. The same preexisting native maximum and setter are used, not a persistent infinite-Wrath cheat flag.

Native SetCurrentWrath0x6BA510 only clamps the requested float, performs its original authority/RPC path if necessary, writes resource+0x3A0 and returns float. Native AddWrath0x660570 reads actor+0xEA0, adds current Wrath and dispatches through actor vslot0xC40. The reflected ConsumeWrath on PlayerState routes through0x566FD0 to an integer inventory holder+0x580 and is not sufficient evidence to hook an ability cost. GetWrathCost is a separate reflected interface method. No speculative hook is justified by the Void test.

Only the two game PAKs are installed (base and PreOrderMounts), and no other ASI was present in the named Win64 directory. A read-only process inspection was prepared but NOT performed: the user closed the game before the read, and it was not relaunched. No game memory, native gate, executable, installed binary or personal INI was changed during this investigation.

## Decision, status and next test

Keep the current V1.38 binary and every existing validated input/camera/gauge path. **Reported Void behavior explained; no mod correction claimed.** Do not force resource costs in a hub that intentionally grants free abilities. Documentation updated cumulatively; no new ZIP/version is necessary for a documentation-only finding.

Next discriminating test: enter an actual mission, use Strife with Unlimited Wrath OFF and the gauge low/zero, then use the same Wrath ability. It should spend available Wrath and refuse use when insufficient. Toggle ON then OFF in that mission to check real spend restoration. If free use persists there, capture that mission log and identify the ability before changing its native gate. Leave personal hotkeys, multiplier values, camera settings and saved last-test toggles unchanged.

Other V1.38 observations: three successful source=ULTIMATE recenter callbacks and native startup knownPages=2 / nativeCleanup=1 are in this log. These prove execution/readback paths; visual recenter and warning UX remain subject to explicit user validation. Natural gain multipliers/manual refills retain their prior user-validated status; mission Unlimited ON/OFF and new startup behavior remain EXPERIMENTAL.
