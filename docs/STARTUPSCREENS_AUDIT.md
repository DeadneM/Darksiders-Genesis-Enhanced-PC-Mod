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
