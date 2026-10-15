# V1.35 - Manual native gauge levels for Ultimate regression tests

Date: 2026-10-15. Base: debug b94de95fc9740f85c47f18cd5081f2330ad7a4ca, cumulative V1.34. User reports ordinary V1.34 play appears working, then qualifies validation: Ultimate Form near session end returned to overhead view and controls behaved incorrectly. Preserve V1.34 outside that known regression. User explicitly prioritizes filling gauges at will before correcting the transformation camera/controls.

## Objective / acceptance

Set Courroux/Wrath, Ultimate/Chaos and Synergy once to a chosen 0-100 percent of each real native maximum, for the current local player. UI lives under Resource recharge, with individual buttons and Set all three gauges. No automatic refill, infinite mode, unlock, synthetic button or automatic Ultimate activation. Success: a click produces matching native before/after readback AND the user sees the actual gauge/ability respond. CONSTRUIT or a matched float alone is not gameplay validation.

## Actual user log and camera finding

Copied locally (read-only source retained) to ../audit-v131/user-v134-ultimate.log, session ends 2026-10-14 23:54:05. V1.34 actually loaded; native swap known=1, last native swap=0; radialEvents=891; mouse native targets FOOT585/HORSE149, all734 hits, 312 requests without a held physical fire button. These observations do not establish all native rebind/impact coverage.

Near the end native ForceCinematicMode ON23:52:08.195, camera NATIVE_CINEMATIC23:52:08.198, OFF23:52:14.029/RETURN23:52:14.031. Earlier ON/OFF23:50:54-57 and23:48:29-33 also exist. **There is no Ultimate entry/exit marker in the old log**, so the six-second final cinematic flag cannot yet be attributed definitively to the user's transformation. No heuristic/camera/input fix is mixed into this build. Pending investigation after reproducible refills: native form lifecycle, possessed pawn and movement identity, camera rejection while transformed, and whether gameplay input filtering can remain active after TPS view stops being emitted. Do not classify all ForceCinematicMode calls as narrative cutscenes without this distinction.

Wrath and Chaos old Add hooks: ZERO callbacks despite Wrath candidate readings5774, gains36/spends36. Synergy682 setter calls,279 positive native gains boosted. Repeating larger old multipliers would not prove a functioning Wrath/Chaos gain path. Those multipliers remain unverified and unchanged.

## Exact executable evidence

Read-only local retail EXE, SHA256 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54. No EXE/pak edits or uploads.

- Reflected DebugRefillWrath/DebugRefillChaos/DebugRefillJuice all register the same exec thunk0x521AE0, tail-jumping to0x246330 (`ret 0`). They are shipping NO-OPS, not usable refill commands. DebugSetCurrentWrath and DebugToggleChaosForm are likewise not assumed functional by name.
- Native AddWrath0x660570 and character GetMaxWrath0x670230 prove live character+0xEA0 -> native resources object. Current Wrath is resources+0x3A0. Maximum is +0x3A4 plus +0x44C (getter0x6A9460). Native setter0x6BA510 clamps at that maximum and zero, respects its server path, writes current and returns float in XMM0.
- Native AddChaos0x62E060 reaches the SAME resources through its verified character owner. Current Chaos is resources+0x3A8; maximum+0x3AC. Actual setter0x6BA2A0 clamps to that maximum and zero and returns float. Calling old AddChaos with an unverified component is unsafe: its owner/type failure can leave a null dereference; it is NOT used by this feature.
- Resource UClass getter0x7AFB10 -> class slot0x3810268. Only a live, readable instance of this exact native base class reached from the validated local player is eligible. Manual setters additionally require native resource role+0x118==3 (authority); no forced client RPC or peer mutation.
- Synergy setter0x583040 is the already validated native setter. Current+0x5A4, maximum+0x498, holder->character+0x5C0 getters0x56D430/0x56FEC0/0x56BE70. APawn.PlayerState reflected property descriptor0x377BBC8 declares offset+0x330; typed state class getter0x7BFDA0 -> slot0x38115D8. Require typed live actor+0x330 AND holder+0x5C0==the same current player, authority, finite positive maximum. If any link is absent, disable that button; do not scan memory or use cached holders. Runtime success of this retrieval remains unvalidated.
- Native IsChaosFormActive exec0x7AD6D0 checks character+0xE78 != null; manual read-only sampling records this flag on live eligible player callbacks. It is not a complete form lifecycle detector if those callbacks stop during transformation.

## Implementation boundaries

New ManualGaugeFeature has NO hooks. UI only queues a request containing actor identity/generation, percentage, mask and creation time. Native calls occur only in the existing validated player movement callback, on the thread previously sampled by the native camera callback. No UObject/native gameplay calls from Present, overlay drawing or a worker thread. No cached UObject is dereferenced: resources/state are freshly obtained inside the live callback. Native cinematic flag postpones a pending request. Pawn/generation changes cancel it; 30-second expiry prevents applying an old request later. A request can be cancelled in UI. Calls/readback run outside UI/state lock, with same-thread recursion guard. Freshness expiry disables buttons and hides stale values.

Manual initialization verifies native entry/body signatures and type offsets BEFORE the existing Synergy hook is installed. Wrath/Chaos calls go directly to their bounded native setters. Synergy goes through the native setter's existing passthrough hook; earned-gain multiplier code is identical. Direct manual sets are logged separately; they are NOT evidence of natural gains or a repaired multiplier.

All three maximums are native. Percentage mapping rejects NaN/infinity, invalid/zero/huge maxima and out-of-range percentages. Only explicit clicks write; starting the game with this build never refills a gauge. Native progress/unlock conditions are retained; filling Chaos does not unlock a locked form.

## Preserved base / rollback

CameraTraceFeature, native cinematic observer, selected-stick role policy, FilterNativeXInput/RT/RB, R3 helper, horse, RawInput/Alt-Tab, natural GaugeChargeFeature/Synergy, config/defaults/schema, loader and old archives are unchanged. NativeTpsFeature only exposes a read-only sampled-thread predicate. Main gains initialization/shutdown and one live-player callback. No INI change or new keyboard binding. Buttons may be unavailable during pause/loading/transformation/mounted play when there is no fresh validated on-foot player callback, or on a non-authority client. Return to ordinary local-host gameplay on foot to test; no guessed fallback write.

Rollback: close game and restore the pre-V1.35 four-file backup, or V1.34 cumulative ZIP while preserving personal INI. main and archive remain frozen.

## Verification / status / TODO

Source EXPERIMENTAL, not built yet at source preparation. Static checks match eight exact retail signatures, both class slots, reflected PlayerState offset and debug no-op. Protected-module comparison and unchanged XInput filtering checked. C++ GaugeRequestTests exercise native percentage mapping, invalid input rejection, request expiry and actor/generation cancellation; existing TPS stick-role tests remain. Windows build/CTest/PE/four-root ZIP/direct-download checks and backed-up closed-game install are required before delivery.

User test: Insert -> gameplay resource recharge -> Target level100% -> Set Ultimate, close overlay, verify actual meter then activate Ultimate normally. Save the log after reproducing camera/control regression. Separately test Courroux and Synergy at50%/100%; log MANUAL_SET reports before/requested/after/max/readbackMatches, never calls it gameplay validated. Confirm untouched ordinary TPS, all wheels, both stick layouts and natural Synergy. Next build only after this evidence: correct transformation ownership/input coherently; continue finding genuine natural Wrath/Chaos gain paths rather than treating a manual refill as a gain multiplier fix.

## Verified delivery and installation

[Windows CI38717620159](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38717620159) SUCCESS for source25f2d94e1ab62a56e9f18f62333bb353f7582498. CTest TpsStickRoles passed0.02s and GaugeRequests passed0.01s, 2/2 tests. Build, four-root packaging, upload and numbered debug publication succeeded; archive commit93adad6cbb44182a02ac0474ba874f8baa2ce3ec.

ZIP784427 bytes, SHA256 6CBAE44DF0FE7C8B226211170C8464D7DE1EB4A81A077349D3B241245E0136BD. Independent exact entry/PE AMD64/build-string checks passed; direct download matches CI and repository hashes. ASI1156096 bytes SHA256 4CF0F53CA438E7DA412CE8B71A540776AB76B9B4DA00F71CCF327C882AD24B62; dxgi154624 bytes SHA256 CADBA2E3F80957F6234573DD9BF326EB4047E1FD03723949A8D0704CA245B141; shipped INI2677 bytes; README244953 bytes.

Normal authorized closed-game installation succeeded. Backup ../audit-v131/install-backup-before-v135-20261015-120713/ contains all four prior V1.34 files plus old log and backup hashes. Binary destination hashes match verified ZIP; personal INI preserved exactly, SHA256 82AF15917398293EFA674C28EF064E15F85FF4D646B60ECBD11763AC3AD69D68. Installed README refreshed with verified delivery receipt; immutable archive untouched. Local package/install hash manifests retained.

Protected main2bc3122c1ae07b7333798462f85c1d9fd3a739f7 and archive10e0f220b85022bc9269e0cb9390a811a3b1cf89 remain unchanged. Downloads diff from base adds only V1.35, no old archive modification. All source checks passed; legacy compiler warnings remain outside the new manual module and do not imply runtime validation. Status CONSTRUIT / CONTROLE / INSTALLE / EXPERIMENTAL. Actual refill/HUD/Ultimate activation and subsequent transformed-camera repair still require the user test above.
