# V1.10 | Native LB+Y and observed startup timeline

## Scope and non-regression

User V1.09 observed charged LB+Y projectiles follow the character's moving/facing direction rather than the independent TPS camera. Gameplay V1.09 log compared ordinary RT/RB native look (camera yaw 114.7 degrees, gap 0) with LB+Y native look (0,0; camera yaw -77.7 degrees). Our speculative charge-specific right-stick and world-look edits did not correct the real fired ability. User agreed to **remove the special correction**.

V1.10 deliberately returns LB+Y to the game's original controller path. It no longer receives synthesized stick (neither 0,0 nor 0,32767), native look/rotation overrides, or a mod-side charged Y key translation. It is not flagged as generic R1/R2 firing. The existing physical LB native wheel branch is preserved when Y is down, and no player transform is forced. **RT/RB forward stick and native look corrections remain as before**, as do R3 selected-equipment USE, mounted controls, synergy, gauges, horse, logos and intro. Old TPSChargedFirePadMask may remain in existing INIs but is deliberately ignored and hidden from UI. Physical LB+Y itself remains valid in game. Diagnostic LB+Y counters indicate detection only and not any modification.

## Native startup flow established before this build

- Original executable RVA 0x2603F0 creates/returns startup settings. The owner contains settings at +0xF8. TArray of 0x40-byte StartupScreens records is at settings+0x50, count +0x58.
- Native loop RVA 0x25FED0 tests record byte+0x39. In the V1.09 user's session, the 3 records had active flags **1,1,0** and byte+0x38 values **0,2,2**. The per-enabled-record call to RVA 0xAC8760 is believed to prepare FText and is **not proven to render any warning**.
- Existing DXGI hook of the startup movie-descriptor copy at RVA 0x25FF31 runs after the above loop. It observes native playlist `THQ_LogoBasic` / `AS_LogoBasic`; when user enables Skip Logos it substitutes two nonexistent movie names but retains 2 descriptors and the game's subsequent MoviePlayer setup. Other movies remain available when present.
- The ASI separately obtains native CVar `g.PlayIntroCinematicOnBoot`. With Skip Intro ON, V1.09 log captured 1 and applied 0. **This is the game's boot-intro request, not evidence of actual video playback.**
- Previous whole-loop warning bypass at RVA 0x25FEBF was quarantined after reticle-cross regression concerns. **V1.10 leaves startup loop, records and UI lifecycle intact.** Skip Warnings remains OFF/non-functional.

## V1.10 observed event telemetry

An early DXGI proxy epoch is taken on process attach. Every `Startup V1.10 TIMELINE` line reports event sequence number, relative milliseconds, OS thread ID, exact observed stage and a stage-specific numeric detail:

- `DXGI_PROXY_PROCESS_ATTACH` : local DXGI proxy initialized.
- `ASI_LOADLIBRARY_BEGIN` / `ASI_LOADLIBRARY_RETURN` : actual LoadLibraryW call for ASI and result (1 success).
- `ASI_MAIN_WORKER_STARTED` : worker running after log initialization.
- `GAME_EXE_VALIDATED` : exact EXE match (1 yes).
- `INTRO_BOOT_CVAR_READY_VALUE` : CVar successfully captured/applied, detail is its resulting value (0 means boot intro skipped).
- `MOVIE_DESCRIPTORS_NATIVE_COPY_ENTER` : interception point immediately after the game's original movie descriptor read, detail actual playlist entry count.
- `STARTUP_RECORD_ARRAY_POST_NATIVE_LOOP` : read-only settings snapshot observed from this startup-movie-copy callback, detail record count.
- `STARTUP_RECORDS_ENABLED_NOT_PROOF_OF_DRAW` : detail count of +0x39 enabled flags (typically 2). It **does not** show that the warnings were drawn on screen or their precise presentation time.
- `TWO_LOGO_NAMES_SUBSTITUTED_PLAYER_INIT_UNCHANGED` : two native THQ/AS names matched and replaced (original native loading player remains).
- `ASI_GAMEPLAY_HOOKS_READY` : all available gameplay hooks initialization completed.
- `FIRST_GAME_D3D11_PRESENT` : first observed D3D11 Present; the frame's content is unknown, and does not prove menu or warning drawn.

These stages allow exact timing between *preparation* of warning startup records and the original MoviePlayer setup, and between early ASI initialization and first presented game frame. Stage timestamps are not a recording of actual pixels. Native intro video can only play if the game's boot CVar allows it; when skip intro is enabled, the playback is deliberately inhibited.

## Next experiment

User tests with current Skip Logos and Skip Intro settings on a **cold launch**, then shares the full per-session `DarksidersGenesisMod.log` plus the order of screens observed on display (warnings/text, title/press-any-key). If the desired gap is still unclear, the next research target is the actual Slate/widget display or MoviePlayer presentation function, not the text-preparation loop. NEVER bypass native record initialization or force stage counters to pretend a warning was hidden.

V1.09 resource research remains: guarded candidate Wrath reads=3128, rises=14, falls=9, native AddWrath/AddChaos runtime events 0, Synergy boosted 128. The next functional goals are confirming Wrath/Ultimate charge paths and safely skipping warning display, then dealing with the mouse TPS last.
