# StartupScreens / MoviePlayer: root-cause audit after V0.43

**Status:** investigative notes, no binary change or release.
**Verified supported target:** `DarksidersGenesis-Win64-Shipping.exe`,
62,113,280 bytes, SHA-256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.
**Safe baseline:** V0.43, `SkipLogos=0`, `SkipIntroVideos` separately configurable.

## Verified in-game observations

- The cross-shaped cursor appeared after loading a saved game with the
  legacy Skip Logos path enabled. The user confirmed that disabling this
  path removes that regression. This is a configuration-level causal A/B
  observation, not identification of a precise missing engine callback.
- Manual F5 / real Alt-Tab could restore the cursor; automatically
  replaying focus transitions in V0.41 was both ineffective and rejected.
- Hide Reticle ON/OFF is validated independently and must not be changed.
- The old V0.19C patch did skip THQ Nordic / Airship Syndicate startup
  logos, while warning + intro cutscene remained with Skip Intro OFF,
  but this was NOT a lifecycle-complete validation.

## Exact existing binary primitive (from repo source, no new disassembly)

Proxy `src/dxgi_proxy.cpp` patches the first FIVE bytes at
`RVA 0x260244` to `E9 0E 00 00 00`, jumping to `RVA 0x260257`.
The full 19-byte native block (repo `kNativeAttachBlock`) is:

```asm
RVA 0x260244: E8 47 89 3A 01       call RVA 0x1608B90
RVA 0x260249: 4C 8B 08             mov r9, [rax]
RVA 0x26024C: 48 8D 55 88          lea rdx, [rbp-0x78]
RVA 0x260250: 48 8B C8             mov rcx, rax
RVA 0x260253: 41 FF 51 20          call qword ptr [r9+0x20]
RVA 0x260257:                     next native instruction
```

By code structure, the first call returns an object (likely
`GetMoviePlayer()`) and the indirect call passes a stack attribute
object via RDX to a virtual method (likely
`IGameMoviePlayer::SetupLoadingScreen`). These C++ identities follow
prior project research and the UE public API; the actual call target,
ABI layout and precise virtual slot must still be confirmed against
the game binary. **The 19-byte call sequence is entirely skipped.**

The proxy applies the patch in `DllMain` before normal game startup.
Its V0.43 legacy-INI guard blocks the unsafe bypass by default; do
not regress this early safety check.

## Rejected alternatives to preserve as lessons

- V0.19A: disable `FStartupScreensModule::StartupModule`
  at `RVA 0x25FE40`: logos disappear, but the independent intro
  cutscene also disappears when Skip Intro is OFF. Rejected.
- V0.19B: zero `StartupMovies` array copied count at
  `RVA 0x25FF31`: logos disappear, intro also disappears despite
  Skip Intro OFF. Rejected.
- V0.19C: bypass MoviePlayer setup at `0x260244`: superficially
  correct logo behavior but confirmed cross-shaped cursor regression.
  Rejected as a default or final implementation.
- V0.41 automatic F5 and V0.40 cursor-source hiding controls:
  removed in V0.42. Do not reinstate.
- General movie-file renames, file-system hooks, Media Foundation
  hooks, arbitrary synthetic input or global startup-module disabling
  are not an acceptable replacement for the mod.

## API-relevant architecture: public UE context (not proof of this game's layout)

The public `FLoadingScreenAttributes` API distinguishes:
- `MoviePaths`: movies under Content/Movies;
- `WidgetLoadingScreen`: standalone or overlaid Slate widget;
- `MinimumLoadingScreenDisplayTime`, playback and auto-completion
  flags, `bMoviesAreSkippable`.

`IGameMoviePlayer::SetupLoadingScreen` accepts those attributes.
`PlayMovie`, `WaitForMovieToFinish`,
`PassLoadingScreenWindowBackToGame` and playback-finished delegates
are part of the normal lifecycle. Unreal has documented
`FDefaultGameMoviePlayer` startup-video keyboard/focus bugs
(UE-37294, fixed upstream long ago), so the lifecycle link is plausible,
but *the Darksiders game's specific missing transition is unproven*.

References (public API, not reverse-engineered local evidence):
- https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/MoviePlayer/FLoadingScreenAttributes
- https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/MoviePlayer/IGameMoviePlayer
- https://issues.unrealengine.com/issue/UE-37294
- https://forums.unrealengine.com/t/how-to-get-viewport-focus-back-from-loading-screen-movie/346728

## Next binary analysis, before implementing a new patch

**Update after the EXE upload (V0.44):** The user supplied the
matching executable. Its 62,113,280-byte length and complete SHA-256
were verified, and the function at RVA 0x25FE40 through 0x260257 was
directly disassembled. Original StartupMovies loading and the 19-byte
MoviePlayer call sequence were corroborated. Runtime names are still
unknown until the native MoviePaths diagnostic runs in game.

Direct disassembly establishes these instructions:
- `0x25FE90`: call `0x2603F0` to retrieve startup settings;
- `0x25FE98`: `[settings-provider + 0xF8]` is the settings UObject;
- `0x25FF31`: read movie count from `[rsi + 0x8]` after
  `rsi += 0x38` and copy movies into `[rbp - 0x68]`;
- `0x260244`: `call 0x1608B90`;
- `0x260253`: `call [r9 + 0x20]` with attributes at
  `[rbp - 0x78]`;
- `0x260257`: native continuation.

**For the next runtime probe and logo-only implementation:**
1. Verify original 62,113,280-byte size and exact SHA-256 above.
2. Disassemble `0x25FE40..0x260300`, recording the ownership and
   lifetime of `SStartupScreens`, `UStartupScreensSettings`, its
   `StartupMovies` TArray, `TimeToShow` entries and the full
   `FLoadingScreenAttributes` stack object at `[rbp-0x78]`.
3. Confirm `0x1608B90` and vtable slot `+0x20` exact semantics
   instead of inferring their names. Trace the gameplay/handoff
   branch at `0x253546..0x2535FD` and playback completion.
4. Locate the specific THQ_LogoBasic / AS_LogoBasic entries and their
   sequence positions. Identify the warning + intro entries and
   distinguish them by source name or validated descriptor.
5. Preferred approach: preserve the full native SetupLoadingScreen /
   MoviePlayer/Slate lifecycle and suppress only the logo media paths
   or per-logo presentation/duration at a narrowly verified point.
   Leave intro and warning untouched. No global empty-movie list.
6. Test crash-free fail-open guard, SkipLogos 0/1, SkipIntro 0/1,
   cursor immediately after load, overlay mouse, F6, F5, and Alt-Tab.

**Release gate:** no code replacement, release, or new ZIP is justified
until callgraph / struct offsets are confirmed, then tested in-game.

## V0.44 - Probe MoviePaths with the real retail EXE (TEST)

USER PROVIDED the exact 62,113,280-byte retail binary, whose SHA-256
matches 9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
Disassembly of RVA 0x25FE40..0x260257 confirms:
- settings UObject array at +0x38 contains `StartupMovies`;
- each entry in that TArray is a 16-byte UE FString;
- the compiled startup code copies it into `FLoadingScreenAttributes`
  at [rbp-0x68], count [rbp-0x60], and passes attributes at
  [rbp-0x78] to a virtual MoviePlayer method;
- the getter call at 0x260244 resolves to 0x1608B90 and virtual
  call at 0x260253 uses [vtable+0x20]. With the original 19 bytes
  present, the native call sequence remains intact.

V0.44 has a **strictly opt-in, one-shot diagnostic probe**, not a new
logo skip. Fresh test INI [Diagnostics] StartupMovieProbe=1. Remove
or set 0 to return to fully unmodified native startup (V0.43 behavior).
It uses an exact-byte-verified 19-byte trampoline at RVA 0x260244,
and replays all original calls byte-for-byte before 0x260257. The
pre-call C++ observer reads MoviePaths TArray entries without writing
to them, bounding all lengths and page accesses with VirtualQuery.
It logs names, count, and minimum loading-screen time in
DarksidersGenesisLoader.log. Diagnostic is disabled when legacy
SkipLogos=1 because that legacy code is already proven to cause the
cursor regression. An in-game attempt to enable SkipLogos while probe
is installed fails closed until a restart with probe disabled.

***Do not call this a Skip Logos fix.*** No logo suppression occurs.
The purpose is to identify the exact runtime playlist names, allowing
a later targeted plan that preserves native MoviePlayer attachment.
The default distribution enables the probe for a one-time test, but
the proxy defaults it OFF for older INIs that lack the key. Hide
Reticle ON/OFF, F6, camera, horses, HUD, intro skip, and all other
features remain unchanged. If the probe causes a crash, disable
StartupMovieProbe=0 and restore V0.43; do not ship as stable release.
No synthetic F5, focus or Win32 cursor manipulation is introduced.
Log is per-run (loader also resets per-run).

## V0.45 - ABORT unsafe StartupMovieProbe trampoline; restore safe baseline

**User-reported crash** with V0.44. Supplied loader log shows that
`StartupMovieProbe V0.44: ... trampoline ACTIVE` was written, while no
`StartupMovieProbe ... item` or `call=` entry appeared. ASI log reaches
early core init but ends before normal shutdown. This strongly implicates
the new mid-function trampoline; no crash dump or exception address
was supplied, so precise failing instruction is **not proven**.

Root engineering mistake: V0.44 diagnostic was described as read-only
because it did not modify `FLoadingScreenAttributes`, but it patched
**19 bytes of executable instructions** and redirected execution to
a manually built x64 thunk. That is intrusive and unsafe without
unwind registration, thorough calling-convention tests and execution
validation. Do not reintroduce it as an "observation-only" technique.

**V0.45 removes the entire V0.44 trampoline and observer** from the
DXGI proxy and restores exactly the proven V0.43 proxy source.
New INI omits `[Diagnostics] StartupMovieProbe`. If an old INI still
contains `StartupMovieProbe=1`, the V0.45 proxy deliberately ignores
that retired key, so an old INI cannot reactivate the probe. Keep
`SkipLogos=0` as the safe startup default. The historic bypass at
`0x260244` remains *explicitly opt-in* and not fixed; turning it ON
can reintroduce the cross-shaped cursor as previously confirmed.
`SkipIntroVideos` remains independent.

This is **safety rollback, not a repaired Skip Logos**. Reticle/F6,
camera, HUD, horses and all other gameplay code are left unchanged.
No automatic F5, synthetic focus or cursor masking reintroduced.
The live exe remains unmodified by the probe with SkipLogos OFF.

Next investigation route: inspect `StartupScreens` defaults and the
actual movie/config resources offline (for example the game's
`DefaultGame.ini`, `StartupScreens` settings, or the original game
`.pak` assets). Prefer content-/descriptor-level, logo-only changes
without bypassing `MoviePlayer::SetupLoadingScreen`; any runtime hook
requires separate safety audit. No public stable release from this
test until user validates V0.45 startup.

## V0.45 runtime validation from user logs (2026-10-09)

The user supplied both Loader and ASI logs for the V0.45 test session
(start 13:51:40, clean shutdown at 13:52:42):
- DXGI proxy logs "V0.43 SAFE: native MoviePlayer startup attachment preserved",
  and loads the ASI successfully.
- ASI logs "0.45-no-trampoline-safe-startup" with supported exact EXE
  SHA-256 and Skip Logos patched=0 enabled=0.
- D3D11, Camera, Horse, Reticle, HUD, Movement, Recovery and Damage
  hooks report ready; player detected; in-game combat and native
  cursor transitions are recorded.
- Ends with "Shutdown: complete", no crash recorded in these logs.
- The user did not explicitly describe the visual appearance of the
  reticle in this V0.45 session: do not claim visual revalidation
  from native-cursor visibility alone.
- In contrast, V0.44 opt-in early trampoline showed StartupMovieProbe
  ACTIVE and caused a startup crash. The implementation is removed
  in V0.45: never resurrect it.

Next engineering step: target the logos selectively while preserving
SStartupScreens and the native MoviePlayer attachment/lifecycle.
No automatic F5, synthetic focus, disabled startup module, globally
emptied startup movies or unsafe mid-function trampoline.
Public stable release remains unchanged.

## Follow-up: logo file identity and exact native StartupScreens arrays (2026-10-09)

External PC-specific guides consistently identify the two startup logo
movies as loose files in `ProjectMayhem/Content/Movies/`:
- `THQ_LogoBasic.mp4` (THQ Nordic)
- `AS_LogoBasic.mp4` (Airship Syndicate)
A separate third file `CG_Intro_LowVi.mp4` is the boot intro, and must
never be indiscriminately skipped with the logos because Skip Intro Videos
is an independent validated option.

Public references (guides, not first-party files):
- https://www.magicgameworld.com/darksiders-genesis-how-to-skip-intro-videos/
- https://www.thenerdmag.com/how-to-skip-darksiders-genesis-intro-videos-pc/

The user-supplied exact retail EXE does NOT contain the literal file stems
`THQ_LogoBasic`, `AS_LogoBasic`, `CG_Intro_LowVi` in either narrow UTF-8
or wide UTF-16LE forms (verified offline). The EXE DOES contain UE4
reflection strings `/Script/StartupScreens`, `StartupMovies`,
`TimeToShow`, `StartupScreenDef`. Thus the runtime playlist is loaded
from game data or defaults not recoverable by a simple literal-string
patch of the EXE. Source/path information alone does not confirm the
actual runtime playlist order or whether the warning has an entry.

Additional directly disassembled native code, exact verified binary:
- `0x25FEB7`: `rsi+0x50` is another array with stride 0x40,
  `rsi+0x58` its count, each iterated and conditionally adjusted.
  This represents `StartupScreenDef`-like entries; exact field
  semantics are not all confirmed.
- `0x25FF19`: reads flag byte `[rsi+0x48]` and copies it into
  loading-screen attributes at `[rbp-0x53]`.
- `0x25FF24`: advances `rsi += 0x38` to the native
  `StartupMovies` TArray.
- `0x25FF31`: copies the source array count from `[rsi+8]` to
  `[rbp-0x60]`, and follows source FString pointers.
- `0x25FF47..0x25FFA9`: constructs a *deep copy* of each FString
  entry (16 bytes per item) into an allocated TArray at
  `[rbp-0x68]`. This copy must be properly owned and freed.
- `0x260244..0x260257`: native getter then virtual
  `MoviePlayer` method receives an attributes pointer at
  `[rbp-0x78]`. The old V0.19C jump removed the entire call.

Why these details matter: a safe Skip Logos implementation should
filter by **exact movie name**, not array position or total length,
while keeping *both* loading-screen widget/attributes construction
and the original SetupLoadingScreen call. A direct count-zero patch
(V0.19B) was previously tested and also removed the independently
controlled intro, so NEVER repeat that experiment.

Past experiments reviewed:
- V0.18D: Game.ini -StartupMovies override wrote a setting but failed
  to suppress company logos. This targeted **MoviePlayerSettings**,
  not necessarily `UStartupScreensSettings`.
- V0.18E: process-wide CreateFileW filter for the two mp4 names
  installed, but received zero matching video-open calls.
- V0.18F/G: bypassing generic engine SetupLoadingScreenFromIni had
  no effect on the logos.
- V0.19A/B: startup module kill / empty copied movie array removed
  intro too. V0.19C: bypass MoviePlayer attachment skipped logos but
  produced the user-confirmed cross cursor bug.
- V0.44: custom handwritten early 19-byte trampoline crashed before
  recording playlist names. Entire probe was removed in safe V0.45.

ENGINEERING RULE: Do not modify or rename installed game mp4 assets
in the patch, do not publish a new unvalidated early trampoline or
trigger F5 / Alt-Tab automatically. V0.45 with SkipLogos=0 remains
the safe baseline.

Remaining specific evidence: read the **game's StartupScreens
configuration / cooked plugin defaults**, if available under
`ProjectMayhem/Config/DefaultGame.ini`, other packaged configs, or
cooked content. Verify the *actual* array entries and any warning
widget descriptor in the same game build. The EXE alone does not
materially establish their values. When a source-of-truth descriptor
is recovered, design a name-filtered, lifecycle-preserving route
and subject any proposed x64 hook to a complete ABI/unwind/lifetime
review before game testing.

## V0.46 - Experimental exact-name first-two StartupMovies filter (TEST)

Goal: bring back Skip Logos without touching the native MoviePlayer setup
and without altering game assets. Continue directly from V0.45 stable
safe startup. V0.44's crashing 19-byte MoviePlayer wrapper remains gone.

SOURCE: The user-supplied retail executable was verified against
62,113,280 bytes and SHA256
9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54.
RVA 0x25FF31 contains exactly:
44 8B 76 08 (mov r14d,[rsi+8], array count)
48 8B 36    (mov rsi,[rsi], array descriptor pointer).
The game's unmodified 19 bytes at RVA 0x260244 are independently
validated and NEVER changed by this version.

V0.46 uses an early 7-byte exact-signature redirect at RVA 0x25FF31.
A nearby executable island replays those original two instructions,
saves flags and all volatile GP registers, allocates the Win64 32-byte
shadow space and calls a bounded validator. The validator reads at most
3 names with VirtualQuery guards and logs them to the Loader log.
Only if first two FString descriptors name THQ_LogoBasic and AS_LogoBasic
in either order, count is 3..64 and all relevant buffers are readable
does the island increase RSI by 32 (skip two 16-byte descriptors) and
decrease R14D by 2. This preserves the third and subsequent entries.
In all other circumstances the ORIGINAL full playlist is passed on.
The island restores original registers/flags and uses a RIP-indirect
absolute jump back at RVA 0x25FF38, not clobbering RAX.

The following native code deep-copies the resulting descriptors and
calls the ORIGINAL SetupLoadingScreen vtable method. Unlike the
V0.19C failed attachment bypass, this retains the MoviePlayer/Slate
startup path and original cleanup ownership. It is nonetheless an
experimental early executable detour; crash-free behavior is NOT
proven. On crash use V0.45 or SkipLogos=0 (fresh startup required).

Fresh TEST ZIP includes SkipLogos=1 to exercise the new mechanism.
Existing INIs are preserved and V0.45 users must explicitly change
SkipLogos=0 to 1 to test it. Missing/old INIs do not automatically
enable it. The old V0.19C attachment bypass is removed entirely from
the proxy; it cannot be invoked by this build. Skip Intro remains
controlled by g.PlayIntroCinematicOnBoot and unaffected by code changes.

If the named logo files are NOT the first two native entries, the
validator logs FAIL OPEN and the logos still show. This is intentional
rather than guessing. Future builds may expand to name-matching in
arbitrary positions after observing real runtime ordering.

TEST GATE: startup with new proxy, log messages in
DarksidersGenesisLoader.log, logos skipped or not, warning display,
intro with SkipIntroVideos OFF, in-game cursor after save load,
Alt-Tab and overlay; do not publish a public tagged release before
user validates. Files at ZIP root. No new auto F5/focus simulations.

## V0.49: user log proves runtime StartupMovies count=2, not 3

V0.47 runtime session on Oct 9 at 15:13 (NOT a V0.48 binary)
shows that the early RVA 0x25FF31 hook executed as intended without a
crash. Log from the existing unified Mod.log:
- "Skip Logos V0.46: exact-name prefix filter requested"
- "exact 7-byte startup copy hook installed"
- ASI reports 0.47-unified-compact-log.
- At native call: "StartupMovies count=2 validPtr=1"
- V0.46 incorrectly logs "FAIL OPEN, unexpected movie count or array",
  because count < 3 is hard-rejected BEFORE logging names.
- Session shuts down normally.

Critical constraint: V0.19B already tried copying a zero-count
StartupMovies array. That experiment removed the independently
controlled intro as well as logos. A naive V0.49 count>=2 change
would repeat that regression, so it is specifically NOT done.

V0.49 extends the existing non-mutating native diagnostic to
allow count=2 for *name reading* while still refusing to empty
the movie array. It prints the two FString entries and whether
they EXACTLY identify THQ_LogoBasic and AS_LogoBasic. If there
are only two entries, even an exact match returns false so the
original native video array and MoviePlayer path remain intact.
The original >2 filter behavior is unchanged. Result is a
focused investigation build, NOT a complete Skip Logos fix.
It preserves V0.48 single Mod.log with compact output, never
creates/deletes/touches any old Loader.log, and contains the
same cumulative gameplay features. When names are confirmed,
research a logo-skip mechanism that keeps the native
MoviePlayer and intro lifecycle (not count-zero, skipping
attachment, or early-function RET patch).

Public release remains unchanged. User should install both
dxgi.dll and ASI from the V0.49 test archive (four files at ZIP
root), launch with SkipLogos=1, then provide only
DarksidersGenesisMod.log.

## V0.50: safe startup and single-log recovery following V0.49 silent run

The user's report after the V0.49 diagnostic is: "il ny a plus de log."
No new runtime log or crash dump was supplied. This alone does NOT
establish whether the proxy was loaded, the ASI was quarantined,
the wrong binary was copied, log-file permissions failed, or the
experimental RVA 0x25FF31 startup hook failed before logging.
Do not claim a specific root cause.

The V0.49 source had a journaling reliability bug: g_loaderLogInitialized
was set TRUE before checking whether CreateFileW succeeded. The ASI
then trusted that boolean when deciding whether to truncate a prior
log. V0.50 changes the flag only AFTER successful CreateFileW and
adds an ASI check that the canonical logfile exists. This avoids
silent log-init success on failed file I/O.

More importantly, the V0.46-V0.49 early handwritten assembly
trampoline is REMOVED entirely from src/dxgi_proxy.cpp. DllMain
does NOTHING except record its HMODULE and call
DisableThreadLibraryCalls. No GetPrivateProfileIntW, no early
VirtualProtect, no early trampoline, no manual executable patch.
All early proxy diagnostics are written outside DllMain on first
DXGI factory/ASI loading, to a single
DarksidersGenesisMod.log (at the dxgi.dll/ASI install directory).

Skip Logos is TEMPORARILY UNAVAILABLE in V0.50; calls to the proxy's
DGSetSkipLogosEnabled return false for enabled=1, log a clear
warning, and leave the engine's MoviePlayer/startup screens native.
An existing user INI with SkipLogos=1 will not re-enable the hook.
The fresh packaged INI has SkipLogos=0. V0.49's observed count=2
remains in docs as evidence; the *last actual working video-skipping
fix is not established*.

All other ASI hooks, camera, horses, HUD, reticle and skip intro
remain unchanged. One compact Mod.log per launch (proxy truncates,
ASI appends); an existing DarksidersGenesisLoader.log is NEVER
created/opened/deleted/modified. This is a crash/log recovery TEST,
not a completed logo fix. Do not create a public release until
the user confirms in-game behavior. If the file is still absent,
diagnose ASI/proxy load/permissions/AV rather than patching
MoviePlayer at random. Four ZIP entries remain at root.

## V0.51 - Undo speculative V0.50 rollback; keep proven log improvements

The user clarified that their earlier report "no log" was a
wrong-file-location mistake. Their subsequently supplied V0.49
single-session log from 2026-10-09 15:39:20 to 15:39:57
proves **both proxy and ASI loaded correctly**:
- 0.49-two-movie-names-diagnostic startup marker.
- StartupMovies count=2.
- entry[0] 'THQ_LogoBasic'.
- entry[1] 'AS_LogoBasic'.
- TWO_LOGOS_CONFIRMED; no removal because earlier count-zero
  patch inadvertently removed the intro cinematic too.
- Healthy gameplay runtime, camera input, player capture and
  normal mod shutdown.

Therefore the *suspected* missing-log failure behind V0.50
is NOT an observed failure of V0.49. Revert ONLY V0.50's
forced disable/removal of the startup movie diagnostic
and restore the exact V0.49 native probe/hook implementation.
Do NOT modify the way it filters, shifts or handles a two-item
playlist. At count=2 it is an observer only, NOT a working
Skip Logos fix, and must be described that way.

Keep these useful improvements from V0.50:
- successful CreateFileW required before setting
  g_loaderLogInitialized.
- ASI checks unified Mod.log file existence before trusting
  proxy readiness.
- One compact per-session DarksidersGenesisMod.log.
- NEVER create, overwrite, or delete old
  DarksidersGenesisLoader.log files.
- No change to existing gameplay, camera, horse, HUD or reticle.

V0.51 is an exact experimental V0.49 startup diagnostic plus
hardened logging, built with SkipLogos=1 in the fresh test INI.
Existing user's INI remains honored; when off, early hook
is not installed. No public release until test validation.

Next development target: safely skip exactly the two confirmed
logo entries WITHOUT zeroing the entire StartupMovies list
or bypassing native MoviePlayer initialization. Avoid
guessing that list count 3+ or modifying the intro toggle.

## V0.52 - Two native startup logos suppressed by substitution (needs game validation)

User's Oct 9 15:50 V0.51 log confirms exact original UE playlist:
count=2, \`THQ_LogoBasic\` and \`AS_LogoBasic\` (in that order).
The previous experiment did not remove either entry.
V0.52 moves from read-only observation to actively omitting the two
logo videos with an explicitly constrained method:
- Leave the native \`UStartupScreensSettings\` and game-owned
  \`StartupMovies\` source array wholly unmodified.
- At exactly the same signature-validated RVA 0x25FF31 early
  copy point proven to run correctly by V0.49 and V0.51, return
  a static two-element FString descriptor array with names
  \`DG_Skipped_THQ_Logo\` and \`DG_Skipped_AS_Logo\`,
  pointing at immutable, process-lifetime UTF-16 strings with
  correct capacity and null-terminated sizes.
- The native \`StartupMovies\` count is left at **2**. The
  original engine-owned deep-copy loop copies these descriptors
  and the original \`GetMoviePlayer()->SetupLoadingScreen\` call
  runs untouched, unlike rejected count-zero V0.19B and
  MoviePlayer-attach bypass V0.19C.
- A mismatch of game fingerprint, instruction bytes, array
  pointer/count or exact logo names yields NO modification.
- For a future unexpected count >2 with the two logos at the
  start, return \`movies+2\` with new count \`count-2\`, keeping
  all other video entries.
- The original x64 island now uses a pointer-returning helper
  and emits \`test rax,rax; je +13; mov rsi,rax;
  cmp r14d,2; je +4; sub r14d,2\`. Old GP register/flag saves,
  shadow space and native continuation are unchanged.
- No V0.44 movie attach hook, synthetic F5, cursor rebind,
  MP4 file modifications, or alternate overlay behavior.
- A nonexistent movie path may cause black frames or a
  media-backend fallback; only the user's in-game test can
  establish timing/intro/focus behavior. It is not described
  as already validated.

V0.52 has a **normal version name** and \`downloads/DarksidersGenesis_V0.52.zip\`;
no "experimental" or "TEST" suffix per user request.
The zip retains exactly four files at root, cumulative features,
unified noncumulative \`DarksidersGenesisMod.log\`, and NEVER touches
\`DarksidersGenesisLoader.log\`. New INI keeps \`SkipLogos=1\`.
Do not tag a public release before user validates in-game:
1. Logos actually absent after fresh boot.
2. Warning screen / loading transition, and intro when
   \`SkipIntroVideos=0\`, remain functional.
3. Cursor/cross after loading a save, F6 reticle/overlay input.
4. No crash, no black screen that hangs indefinitely.
If any issue, set \`SkipLogos=0\` and restart, or restore stable V0.45.

## V0.53 - Freeze V0.52 Skip Logos + Skip Intro; compact UI

The user's V0.52 runtime log (2026-10-09 16:53) confirms:
- the early verified movie-list hook installed and matched
  the exact TWO entries: THQ_LogoBasic and AS_LogoBasic;
- two nonexistent names substituted with nonzero count=2;
- independent native intro CVar applied with SkipIntroVideos=1;
- player identity, reticle hooks, camera, horse hooks, damage
  and movement initialized; normal shutdown.
In-game USER FEEDBACK: reticle appears OK; Skip Intro and Skip Logos
appear to work. The user explicitly asks to FREEZE their
implementations. Therefore V0.53 does not change:
src/dxgi_proxy.cpp, src/SkipLogosFeature.cpp, native Skip Intro
implementation or native reticle handling. Preserve exactly V0.52.

V0.53 normalizes SkipLogos=1 and SkipIntroVideos=1 across
new install INI, C++ Store member defaults, Reset Defaults,
fallback reads, and RuntimeSettings init. ConfigRevision=2103,
with explicit migration for older config revisions:
- Old pre-2102 bypass configurations retain historical safety OFF
  migration (do not reenable the unsafe legacy bypass).
- Existing revision 2102 settings retain user choices, including
  deliberate SkipLogos=0 or SkipIntroVideos=0; updating to
  revision 2103 only synchronizes saved metadata.
- Fresh installs and the Reset Defaults button select both ON.

Overlay: beside each numeric slider's Reset button, show ONE
dimmed LIVE/WAIT indicator, with a hover-only tooltip carrying
technical hook details. Keep rich Combat, Horse, Camera and Recovery
diagnostics accessible through collapsed Debug tree nodes.
System has concise READY/restart status. Native values are not
fabricated and no runtime feature implementation changed.

NEXT FEATURE (not implemented in V0.53): new separate
SkipWarning option to remove exactly the two startup notices
"play with controller" and "autosave warning". They are not
part of StartupMovies (which has exactly the two logo names)
and must NOT be removed by the name-substitution hook. The
startup screen definition array (noted previously at
UStartupScreensSettings +0x50, stride 0x40) is a separate
candidate requiring correct native timing and ownership audit.
Neither DisableStartupScreens nor bypass SetupLoadingScreen is
acceptable since these previously disrupted intro/cursor.
Do not expose a checkbox that promises functionality before
the independent warning suppression is implemented.
Once implemented, choose a user-controlled INI default
separately from the two frozen video skips.

Normal cumulative archive, four root-level files, new V0.53
label, no "TEST" suffix. Unified per-session Mod.log; old
Loader.log untouched. No public release/tag without user request.

## V0.54 Skip Warning independent attempt and Third Person

User log for V0.53 confirms logos are matched/substituted, the
native intro CVar is set, camera hooks, HUD, reticle, movement and
horse hooks enabled; user INI supplies F1 ToggleHUD, F4 ToggleReticle,
F5-F12 None and camera keys with Home/End + PageUp/PageDown.

The V0.54 Skip Warning checkbox controls a SEPARATE guarded filter
for the startup-screen definition array (settings +0x50; each 0x40
bytes in original disassembly, unrelated to StartupMovies at +0x38).
The known working early 7-byte startup hook passes the original
movie TArray field pointer through volatile r8 as the third argument
to the existing C++ name filter. Before the native MoviePlayer
Setup call, if native definition count is EXACTLY 2 and pointers,
capacity, regions and write permissions are valid, set only
StartupScreenDef.Num=0. Existing actual THQ+AS name replacement and
movie count=2 stay the same. No external files touched. On unexpected
count/layout, warnings remain and a fail-open line is logged.
This is a gameplay *attempt*, not yet validation that controller and
autosave notices are actually controlled by these definitions. An
existing INI can set SkipWarning=0 for the next boot.

Third Person: optional, OFF in the user's provided INI. Native view
and arm hooks already existed; new transient camera output computes
a pivot from original view/pitch/yaw and spring-arm length, then
repositions the view with a shallow -12-degree pitch and a 0.25x-3x
distance multiplier. This may not exactly follow character facing,
and needs real camera test. Switch OFF to restore native framing.

Removed ReticleFocusTest, its synthetic focus pulse, and stale F5
description. HUD Hidden is the only overlay HUD toggle; F1 action
remains. No change to validated reticle suppression/Skip Intro hooks.
User defaults copied into INI. New normal V0.54 build/archive ZIP
has all four files at root; no public tagged release yet.

## V0.55: Skip Warning early native-loop attempt

Original StartupScreens loop is at RVA 0x25FEB7..0x25FEEA and references the StartupScreenDef array in UStartupScreensSettings at offset +0x50, with 0x40-byte elements. V0.54 tried to mutate the array Num in a callback at RVA 0x25FF31, already too late. V0.55 removes that write entirely, adding an independent guarded hook at RVA 0x25FEBF (exact native bytes 48 C1 E7 06 48 03 FB). Its trampoline replays both instructions and only sets loop end equal to start when exactly two entries are present and SkipWarning=1 at boot. The original MoviePlayer/Slate setup and validated exact-name startup movie substitution are not changed. No source array mutation, no asset edits. This is not validated to suppress the two warning pages until the user's in-game test; on mismatch or disabled state, native warning screens are preserved.

## V0.56 native warning span audit

V0.55 log proves hook executed but count !=2. V0.56 records raw span RDI-RBX, preserves RFLAGS, RAX and RDX after the original instructions, skips only one or two exact 0x40-byte definitions, otherwise leaves native flow unchanged. The two notices (controller suggestion and autosave warning) may be elsewhere; monitor log and in-game result before calling Skip Warning fixed.
