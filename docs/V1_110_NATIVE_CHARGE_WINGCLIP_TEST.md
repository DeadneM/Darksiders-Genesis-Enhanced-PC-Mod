# V1.110 - Native charge cues and WingClip

EXPERIMENTAL, not validated in game. GitHub build and delivery pending. Cumulative source base V1.109 e45d368b519d13bf516afed7ace99c58fd8c902e; accepted functional base V1.103 with subsequent user acceptances retained. No local build, game launch or installation. User installs through debug updater.

## User results retained

- V1.109 uses the V1.108 gameplay code. User explicitly validates transferred power speeds/sounds and normal Lava muzzle flash at the gun. Startup is subjectively less slow.
- Rampage opening damage/shield still appears absent, ending slam looks faithful with an odd movement after Devil Heel. Secondary spinning melee effect remains too large with blades OFF.
- Grouped VM_READ-only capture completed: 2 War starts/2 Slam3 finishes, 2 Strife starts/2 Slam3 finishes, 4 WingClip casts with both/left/right/both blade visibility configurations. User did all requested actions but could not compare enemy damage. No runtime writes, injection or remote function calls.

## Evidence and correction

The active secondary function is MayhemAbility_Strife_HeavyAttack1_Back_Blueprint_C and Strife_Atk_WingClip_B_Snare_Montage. PAK sequences Strife_Atk_WingClip_B and _F each author four slash notifies at root offsets (0,60,100)/(0,-60,100), normal and strong arcs. The earlier Kick Launch addition addressed another attack. WingClip now uses the same scoped native particle wrapper, including attachment transform and 40% scale. Each authored arc chooses its closest actual native hand at spawn; only that hidden blade's arc changes. This avoids guessing left/right from an input key or root Y sign. Damage, trace windows, impacts and other emitter classes are untouched. The validated primary melee and Lava scopes remain byte-identical.

Rampage uses an unrendered DefaultSlot to retain native War root motion without imposing the incompatible War skeleton pose on Strife. Exact graph ownership/RootMotionMode=3 and unused-slot checks remain. Offline engine analysis at 0x1693DF0 distinguishes montage-owned cues from source-sequence cues queued per slot (0x1683630). V1.108's two notify detours could only adapt callbacks after dispatch and did not supply the missing charge timeline. They are removed, along with their public helper and cached Source payload. The private Start montage now copies nine existing native charge events, keeping War motion clips/rate/loop sections intact:

| Source | Event | Native time/duration |
|---|---|---|
| War_Atk_Rampage_Start | MayhemBlast_War_Rampage_Run_Level1 via SpawnBlast | 0.213815 |
| same | two PlaySurfaceParticleEffect cues | 0.21528375 / 0.30003518 |
| War_Atk_Rampage_Run | P_Rampage_Shield timed state | 0 / 1.06666672 |
| same | four surface cues | 0 / 0.22908080 / 0.48798981 / 0.76002324 |
| same | EnableStopOnCollision | 0.20045169 |

Run events are mapped after the unchanged 0.33333334-second start segment. Native array allocation, notify classes, blast class, attach flag, effects and offsets are preserved. Socket adaptations use Strife's own spine/root. The shield gets its own private native notify-state object; begin/end/cancellation remain engine-managed. Audio is excluded from this new plan to preserve its accepted path. No invented damage multiplier, manual projectile spawning, extra timer or input simulation. Exact cue counts and state spans must match or the transfer refuses playback through its existing native abort path. Source sequence events stay unchanged in the already excluded graph slot; the graph guard is essential to prevent dispatch from two paths. No claim of live duplicate-free damage or matching enemy health is made before the user test.

Both captured Strife Slam3 abilities remained active for approximately 0.25 s after their native montage map entry disappeared. Montage_GetPosition at 0x16778E0 returns zero for a missing montage. V1.108 consequently could replay the beginning of Devil Heel during that tail. Observe now requires the own helper to still reference the same montage and native IsPlaying to remain true before reading position; otherwise only the visual pose is cleared. The phase handoff token remains valid for the final native companion. This is a targeted fix for the observed lifecycle, not an in-game validation of the final blend.

## Controls and risks

Automated regression fixtures cover all five phases, private allocator/arrays, source immutability, native blast identity, nine mapped cues, shield state duration and cleanup linkage, missing cues/unknown spine rejection, ended/replaced montage tail, and WingClip per-hand/visible-opposite preservation. Windows compile, all existing suites, package and final public URL must pass before delivery. These are structural checks, not proof of enemy damage, seamless poses or P2 gameplay. No broad input, animation graph, root motion or combat bypass. P2 tests remain deferred.

## Grouped in-game test

1. Strife Rampage through enemies: confirm surrounding effect AND damage during the dash before the finishing slam. Release, turn, encounter a wall, then cast again in native and TPS views. Compare War on the same enemies if practical, allowing for different character/core stats.
2. After Talon du Diable, check the odd extra movement, normal walking/attack and absence of persistent shield/commands lock.
3. Strife spinning secondary: Native, left OFF, right OFF, both OFF. Hidden side gets compact hand-origin arcs; visible blade keeps its normal arc. Briefly check already accepted primary melee and Lava flash.
4. Quick retained-path check: transferred power sound/speed and ordinary War Rampage. No need to repeat the whole mod test list.

If abnormal, return the affected transferred slot/blade setting to Native; prior V1.109 remains available in debug. Keep customized INI/hotkeys/saves/PAK. All 174 previous Git archive blobs, main/archive and v1.43 are preserved; the quarantined local V1.108 copy is neither restored nor staged for deletion.
