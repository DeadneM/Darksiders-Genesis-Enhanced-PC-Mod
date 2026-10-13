# V1.15 Resource Gains Audit | 2026-10-13

User asked whether Synergy 317 observed / 113 boosted is normal and described collectible gems and enemy kills as resource sources. **V1.14U log supports:** Synergy SetCurrentSynergy observed=317, verified local=317, positive=113; AddSynergyNative observed=317, verified local=317, boosted=113. All classified *positive* native additive gains were boosted x2. The other 204 are additive calls with no boosted positive gain, not evidence of 204 missed resource pickups. At the beginning of the session, raw input factors (16.002, 0, 1) and (0.450, 0, 1) produce zero gain.

The code modifies native additive input `C` in `(A*B)*C` rather than writing an absolute amount to the bar. This is mathematically a boost to the received gain and avoids doubling spends or quest/scripted SetCurrentSynergy updates. HOWEVER, no evidence yet proves that every colored resource gem and enemy-kill reward goes through this same function. Wrath and Chaos/Ultimate hooks remained enabled but uncalled (both 0) in this log, while a read-only candidate wrath field changed (rises=12 falls=5). We must identify their *true* gain paths, not blindly patch UI/setter values or object attributes.

## V1.15 changes

- **No gameplay hook behavior or multiplier changes.** Added bounded V1.15 diagnostics in GaugeChargeFeature.cpp only.
- `ADD_CLASS`: log representative native Synergy additive factor triples with precision `%.9g`, native gain, positive / zero / negative / invalid classification and verified local owner.
- `SET_CLASS`: classify absolute setter as unchanged, decrease, gain from additive call, or *direct setter gain*; use thread-local depth only while original native AddSynergy is executing. This can reveal if pickups bypass the additive helper. Additional summaries at normal shutdown: additive positive/zero/negative/invalid/skipped; setter gainFromAdd/gainDirectSet/unchanged/decreased/invalid.
- Preserve the existing exact executable SHA, Synergy x2 logic, wrath/ultimate hook pass-through, camera / TPS mouse, INI reset (About popup), built-in updater, logs per session, and 4-file ZIP.
- **Numbering convention now mandatory:** one new integer minor per build, `V1.15`, `V1.16`, etc. No A/B/C/D/U or same-version suffix experiments. Existing historic archives remain untouched; new build is V1.15 only.

## Next user test

With Synergy x2 ON, isolate **one enemy kill without grabbing a gem** versus **one collected gem without fighting** and note rough timestamps. Then use some Wrath/Ultimate-giving gems and actions. Report the log at exit. Compare `ADD_CLASS` triples, `SET_CLASS` direct gains, and `WRATH CANDIDATE` deltas. This identifies which events are positive and whether pickup/kill paths differ, without modifying gameplay based on guesses.

CI builds Windows x64 on `debug`, permanent archive `downloads/DarksidersGenesis_V1.15_RESOURCE_GAINS_AUDIT_TEST.zip` with 4 root files. Keep prior builds for rollback; `main`/`archive` untouched. Gameplay tests still pending.

## CI result confirmed
Windows x64 GitHub Actions run [38436642492](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38436642492) **SUCCESS**: compiled Release, validated exactly four ZIP root entries, uploaded artifact and published permanent debug archive. [V1.15 ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.15_RESOURCE_GAINS_AUDIT_TEST.zip), 748509 bytes, Git blob SHA1 `0be3185fbaad8629b52cd62e9e1f1f61866afc1d`. `main`/`archive` unchanged; gameplay verification pending.


## User's V1.15 audit log - 2026-10-13 19:40–19:47

Observed totals: Synergy SetCurrentSynergy=81 (local=81), positive=28, boosted=28. Native additive helper observed=81, verified local=81, boosted=28. Classification: positive=28; zero=53; negative=0; invalid=0; positiveNotBoosted=0. Setter: gainFromAdd=28, gainDirectSet=0, unchanged=53, decreased=0, invalid=0. Samples: A=1.85899997, B=3, C=1 yields native=5.57699966, multiplied to 11.1539993; and 0.449999988*3*1=1.3499999 doubled to 2.69999981. Zero-gain calls had factor B=0. This fully accounts for 81 calls in the trace, with no skipped positive Synergy additive gain detected. Still no proof that every gem and enemy reward uses this function outside the tested actions. Wrath/Chaos native Add counters remain 0; Wrath candidate rises=4 falls=3. No gain path modifications justified yet.

The user separately validated the integrated ASI updater after using this build. Do not treat that as proof of rollback or failure recovery scenarios.
