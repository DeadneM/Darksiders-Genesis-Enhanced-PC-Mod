## V1.128 - ergonomie du menu

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; MENU COMPLET NON VALIDE EN JEU.** Base V1.127 installee et conservee. Aide courte avec details au survol (?), marges et champs adaptes a la police, listes et valeurs homogenes, groupes Appearance fermes initialement, libelle/contact sol avances clarifies. Parametres, controles et modules de jeu inchanges ;218valeurs INI/55ID Hotkeys conserves. [Perimetre, deux passes et essai](V1_128_MENU_READABILITY_TEST.md).

Compilation uniquement GitHub ; aucun jeu lance ou remplacement V1.128 local. main reste V1.124 validee ; anciens ZIP/archive preserves. Bug de selection du tir souris TPS sur destructible reste ouvert, coffre/dash non declares valides. Les sections suivantes sont historiques.

---

## V1.127 - reception lourde acceptee, coffre/dash a mesurer

**CONSTRUITE / CONTROLEE : [66/66 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40686258358), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/cfc71c93c41ce6c985487e1fa692afe18155bbe4/downloads/DarksidersGenesis_V1.127_RECOVERY_EXPERIMENTS_TEST.zip) - [Recu](V1_127_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Timing reception lourde V1.126 accepte puis fusionne ; conditionnement V1.127 non valide en jeu. Gain coffre/dash et tir TPS souris restent ouverts. main reste en V1.124 ; installation locale ensuite demandee et verifiee, INI conserve ; jeu non lance.**

**CONSTRUITE / CONTROLEE ; NOUVEAU CONDITIONNEMENT NON VALIDE EN JEU.** Base V1.126 : timing personnalise reception lourde accepte par l'utilisateur, fusionne dans la case existante avec ses valeurs. Coffre :6demandes dans le journal ; dash :0. Aucun gain de controle prouve par ces traces. Ajout d'un bilan borne sans changer leurs gardes/mouvement. Visee souris TPS : investigation trace Projectile/objets Overlap et branche du faisceau ciblee ; aucun patch de visee, manette non testee. [Perimetre, limites et essai](V1_127_UNIFIED_LANDING_RECOVERY_TEST.md).

218valeurs INI conservees, cle doublee retraitee a la sauvegarde ;55ID Hotkeys inchanges. Leviathan/apparences/transfer capacites/cameras/controles preserves. main reste en V1.124 ; debug seulement. Installation locale demandee puis verifiee ; pas de relance ni compilation locale. Les sections suivantes sont historiques.

---

## V1.126 - essais Recovery et impact Tremor

**CONSTRUITE / CONTROLEE : [66/66 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40672465858), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8ea941ff3993d970fb89577b4c165ad94a739eeb/downloads/DarksidersGenesis_V1.126_RECOVERY_EXPERIMENTS_TEST.zip) - [Recu](V1_126_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Recovery/Tremor EXPERIMENTAUX, NON VALIDES EN JEU, nouvelles cases OFF. Leviathan et apparences precedentes conserves. main reste en V1.124 validee ; aucune installation locale.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base V1.125 controlee, V1.124 validee reste sur main. A la demande utilisateur, apparences actuelles conservees et Leviathan confirme independant/inchange.

Player movement -> Recovery experiments : temps de reception lourde optionnels et deux essais MOVE natifs apres arret complet observe du montage coffre/dash. Cases OFF par defaut, pas de Finish/Stop, touche imposee, timer/vitesse/animation ecrit ou nouveau hook. Les chemins valides sont conserves quand les essais sont OFF. Ajouter seulement Tremor surface impact (Native/or/metal/pierre/eau/Hidden), via quatre ParticleSystem natifs verifies et la fabrique existante ; pas de refonte des anciens groupes.

Sept cles nouvelles,219valeurs au total ;212anciennes et55ID Hotkeys conserves. Compilation uniquement GitHub, aucune installation/relance locale. [Perimetre, limites et essais](V1_126_RECOVERY_EXPERIMENTS_TEST.md). Les etats suivants sont historiques.

---

## V1.125 - consolidation et carte des temps Recovery

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40665878336), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2dd91999a23acbed02dd5c2b4b2a7aedd6f0a6ac/downloads/DarksidersGenesis_V1.125_CONSOLIDATION_RECOVERY_MAP_TEST.zip) - [Recu](V1_125_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Nettoyage NON VALIDE EN JEU ; pas de correctif Recovery general ni de correction des apparences revendiques. main reste en V1.124 validee. Aucune installation locale.**

**CONSTRUITE / CONTROLEE ; NETTOYAGE NON VALIDE EN JEU.** Base V1.124 validee, maintenant promue sans recompilation sur main a la demande utilisateur. La suite reste sur debug. [Promotion](V1_124_MAIN_PROMOTION.md).

Deux passes d'audit confrontees : copie de telemetrie cheval sans lecteur retiree, adaptateur Clone reduit a son seul receveur War (tableaux a une entree et branches inaccessibles retires). Pas de changement intentionnel des capacites ; Geyser/Stoneskin/Caltrops et autres acquis conserves,212valeurs INI/55ID Hotkeys inchanges. [Audit et limites](V1_125_DOUBLE_AUDIT.md).

Recovery :27assets natifs relus, distinction entre montage/fondu/interruption/fin de capacite. Les fenetres MOVE des receptions lourdes sont0,833373s Strife/0,813425s War ;0,668465/0,402459 concernent d'autres actions. **Code HeavyLanding valide inchange ; aucun correctif Recovery general annonce.** [Carte complete](V1_125_RECOVERY_TIMING_MAP.md).

Retour utilisateur : apparences partiellement sans effet/peu interessantes (11groupes visibilite seulement), Rampage accepte, coop plus tard, FX P2 semblaient bons et HUD general plus reproduit. Aucun nouveau hook ou essai natif, installation/lancement local. Les statuts suivants restent historiques.

---

**28 octobre 2026 : V1.124 / correctif du petit saut final Caltrops War VALIDES par l'utilisateur dans son essai. Sujet clos ; Geyser/Stoneskin et acquis conserves.** [Retour et limites](V1_124_USER_VALIDATION.md) - [file restante actuelle](ACTIVE_QUEUE.md). Apparences/degats de charge a confirmer, coop/FX P2 differes, HUD en surveillance ; reception lourde deja validee. Consolidation du code recommandee, pas encore executee. Les etats ci-dessous sont historiques. Aucun nouveau build ni changement runtime.

V1.124 delivered:65/65 GitHub suites, no MSVC warning/error, ZIP/updater/Defender controlled. Explicitly authorized one-shot War Caltrops ground-state API trial EXPERIMENTAL. V1.123 validated features preserved; no local install/launch. [Receipt](V1_124_DELIVERY_RECEIPT.md).

## V1.124 authorized one-shot Caltrops ground-state trial

- Explicit new user authorization received2026-10-28 for the exact bounded SetMovementMode(1,0) call; earlier deferral is superseded for this trial only.
- Preserve validated V1.123 Stoneskin/Geyser and Caltrops stomp/spawn/audio. Candidate: one early engine walking request, after a proven ground-start foreign War cast; no held mode, new hook, pose sampler or VM edit.
- Verify GitHub65 suites, no MSVC warnings, four-root NoCompression ZIP/updater/Defender,188 prior archives/protected refs. No local compilation, installation or launch.
- Runtime hop correction EXPERIMENTAL: test idle/moving native/TPS, movement/jump/ledge/cancel; Strife native Caltrops regression; coop last.

## Decision utilisateur : conserver V1.123 pour le moment

L'utilisateur choisit explicitement « Conserver V1.123 pour l'instant ». L'essai SetMovementMode est mis de cote : aucun patch applique, aucun appel moteur, aucune nouvelle build ou installation. Ne pas relancer cet essai ni redemander son autorisation sans reprise explicite du sujet par l'utilisateur. V1.123 reste la version installee ; Stoneskin/Geyser et le geste Caltrops acceptes sont conserves. Seul le petit saut final Caltrops reste OPEN.

---

## Caltrops native-state observation - approval boundary

- Two casts captured read-only: root does not rise; in-air pose flags true during native flying, then falling~47ms and walking. Full stomp and existing validated visuals unchanged.
- Prepared scoped public SetMovementMode(1,0) one-shot candidate for initial grounded War Caltrops. Automatic approval refused source patch for missing explicit authorization of this distinct movement trial. Patch NOT applied; no build or game mutation. Await specific human approval; do not bypass.
- [Measured evidence](evidence/V1_124_CALTROPS_MOTION_OBSERVATION.json), [scope/risk](V1_124_CALTROPS_GROUND_STATE_REVIEW.md). Coop last.

## V1.123 user validation after installation

- User VALIDATED Strife Stoneskin including launch/expiry visuals, and native War Stoneskin in tested scenarios. Freeze these paths and Geyser.
- War Caltrops stomp/burst accepted; terminal small hop still OPEN and visible on attached video. Source flight5 -> falling3 candidate; no latest live motion timing yet.
- Read-only local War position/mode/pelvis capture prepared; waiting for user readiness. No movement/VM guard, new ASI or diagnostic-only ZIP. Coop last.
- [Video/log review](V1_123_USER_VALIDATION.md).

V1.123 subsequently installed on explicit user request: verified V1.122 backup, exact payload hashes, personal INI byte-identical, no game launch. Gameplay validation still pending. [Install receipt](evidence/V1_123_INSTALL_CHECK.json).

V1.123 delivered:65/65 GitHub suites, ZIP/updater/Defender controlled. Geyser frozen; Stoneskin launch/expiry and War stomp Caltrops experimental. No local install or gameplay validation. [Receipt](V1_123_DELIVERY_RECEIPT.md).

## V1.123 latest steering

- Geyser V1.122 explicitly VALIDATED: frozen unchanged. Stoneskin gesture/materials validated; only launch/complete-expiry particles requested.
- Explicit limited visual authorization received for ASI SpawnEmitterAttached; public function and 72-byte contract read from game, external reader remains read-only. Internal notify constructor/direct-invocation proposal abandoned.
- Candidate: Strife Base/Molten launch once at coating, native ash once at full status expiry; no cast-end timer, world/form/shutdown false expiry.
- War Caltrops: full Stoneskin foot stomp, one native caltrops burst on existing particle callback; three actual source notify entries filter private donor coating/actor/blast. Native physics/VM untouched; hop result OPEN.
- Remove failed launch copier and unused Caltrops getter/tests; preserve validated Clone/Rampage/WorldEnder, controls/config. Coop last. No local build or installation.

V1.122 delivered:65/65 GitHub suites, ZIP/updater/Defender controlled. Geyser1.75s, Stoneskin1.25s with same gestures/FX, Caltrops grounded one-shot candidate; gameplay unverified. V1.122 installee sur demande avec sauvegarde verifiee de V1.121, INI/PAK conserves, sans lancement du jeu. [Receipt](V1_122_DELIVERY_RECEIPT.md).

## V1.122 latest steering

- V1.121 installed on explicit request, verified backup/INI preservation, no launch/runtime validation.
- User keeps Geyser/Stoneskin gestures and effects, wants shorter activation. Candidate Geyser1.75s with all FX/damage times native; Stoneskin1.25s with independent status lifetime.
- Caltrops different candidate: complete grounded War gesture alone, no Start/End splice, source flag, physics/VM patch or new scan/hook. V1.121 fallback if asset unavailable. Visual result OPEN.
- Preserve Clone/Rampage/WorldEnder, all validated settings/control paths. Coop last.

V1.121 delivered: 65/65 GitHub suites, ZIP/updater/Defender controlled. Caltrops/Geyser original-span retry and Stoneskin launch FX EXPERIMENTAL; accepted Stoneskin duration/materials retained. [Receipt](V1_121_DELIVERY_RECEIPT.md).

## V1.121 latest verdict and clean retry

- REJECTED: Caltrops recent duration/End/source-pose attempts, Geyser added ending pose/held frame (vertical bob). Removed from current ASI, historical packages preserved.
- VALIDATED by user: current Stoneskin gesture/duration/materials/restoration. Missing launch particles only: scoped native particle candidate added; activation/status remain accepted.
- EXPERIMENTAL: one continuous gesture over original cast duration for Caltrops/Geyser, no added end pose, source flag, physics or VM patch. Visual outcome and normal movement/jump must be checked.
- Clone/Rampage/WorldEnder and other accepted features/settings preserved. Coop tests last. No local install/build/launch.

V1.120 delivered: 65/65 GitHub tests, ZIP/updater/Defender controlled; Caltrops source-pose continuity EXPERIMENTAL, not yet user validated. Both half-second Strife animation changes retained. [Receipt](V1_120_DELIVERY_RECEIPT.md).

## V1.120 Caltrops clarification and visual candidate

- User: body bobs without actual displacement. Native movement/VM guard abandoned; no physical jump fix attempted.
- Candidate: keep native War TPS source pose updating only during owned transferred Caltrops; exact reflection/identity guards and restoration, no new hook/timing change. Visual hypothesis, not runtime validated.
- Prior End0.55 timing did NOT fix bob. Geyser/Stoneskin +0.50s retained exactly, pending user verdict. Clone/Appearance/Rampage accepted paths preserved.
- INI/Hotkeys unchanged, GitHub only, P2 last.

V1.119 delivered: 65/65 GitHub suites, ZIP/updater/Defender controlled; both +0.50s animation adjustments EXPERIMENTAL. Caltrops hop OPEN, unchanged. [Receipt](V1_119_DELIVERY_RECEIPT.md).

## V1.119 user feedback and candidate

- User requests Geyser/Stoneskin animations +0.50s: stationary return1.15s / nominal activation1.50s prepared, native gameplay retained.
- Caltrops War still hops in V1.118: NOT FIXED by the0.55s return. Stop duration-only retries; ask actor lift versus rendered-pose bob before another repair. Caltrops code unchanged now.
- Earlier Clone/Appearance/Rampage validations preserved. Stoneskin artwork/library variants/P2 not newly validated.
- No hook/VM/movement-control patch, no new config key, GitHub build only; personal INI untouched.

V1.118 delivered: 65/65 GitHub tests, ZIP/updater/Defender controlled; Geyser/Stoneskin transitions EXPERIMENTAL, Caltrops OPEN. [Receipt](V1_118_DELIVERY_RECEIPT.md).

User chooses simplest animation approach: Caltrops return0.55s as validated Clone, total1.12s. VM guard excluded; no explicit method approval inferred. Hop still needs test.

## V1.118 latest user report

- VALIDATED by user: Appearance tab present, War Clone OK; preserve those and earlier Rampage.
- OPEN: Geyser/Stoneskin a little abrupt; isolated fade/return retune prepared, native gameplay intact.
- OPEN: Caltrops hop persists; Blueprint flight/fall path identified offline. Proposed VM guard rejected by automatic approval review; no guard implemented. Explicit method approval and further proof required.
- Library variants/mineral artwork not explicitly validated; P2 deferred. No new keys/hooks/local build/install.

V1.117 delivered: 65/65 GitHub suites, package/updater/Defender controlled; gameplay corrections still EXPERIMENTAL. [Receipt](V1_117_DELIVERY_RECEIPT.md).

## V1.117 grouped video follow-up

- User-validated: Strife Rampage perfect, preserve. Prior accepted camera/controls and War World Ender retained.
- OPEN: Geyser static tail (V116 refused curve-bearing output); new private native pose/curve return prepared, gameplay untouched.
- OPEN: Strife Stoneskin too short and body too fiery; 1s activation / mineral body / native cloth prepared, full expiry restoration retained.
- OPEN: War Caltrops end hop / Clone gesture too short; private native channel return prepared.
- User could see NO new appearance choices. Existing 43-group library moved into direct Appearance tab; variants still unvalidated, no renderer expansion.
- 212 INI keys/55 Hotkey IDs unchanged. Github-only build, no local install/launch. Coop remains last.

## V1.116 shared appearance library

- User requests all remaining families: 43 groups / 451 native particles / 98 roles prepared; runtime validation pending.
- One common renderer replaces Caltrops-only code. All defaults Native, 42 new keys; 170 previous values and 55 Hotkey IDs retained.
- Only emitted particles: direct/static components and model/material variants need separate evidence; no fabricated alternate for single-look families.
- V1.114/115 remain unvalidated, V1.113 rejected; gameplay modules and accepted work unchanged; P2 last.

## V1.115 current appearance experiment

- User defers V1.114 validation and asks for skins; do not mark its Geyser/Stoneskin work accepted.
- Caltrops Native/Base/Frost 1/2/3 initial glyph and explosion selector prepared, runtime test pending. Ground model/idle/loop stay native; loaded assets only.
- Other catalogue families and Caltrops loop remain future work. All previous pending tests retained; coop last.

## V1.114 current priority

- V1.113 REJECTED: persistent jump lock after Geyser, wrong Stoneskin gesture/render.
- Retain V1.112 montage/control paths; stationary render-only Geyser and short original Stoneskin activation await user validation.
- Native Stoneskin A/B full shaders preserve fire switches; final Strife UV fit remains experimental.
- Rampage Strife ending, three Strife powers on War and status expiry accepted; initial Rampage charge damage unconfirmed; P2 last.

# Remaining work after V1.111 / candidate V1.112

- V1.112 user validation: Rampage Strife ending, all three Strife powers on War, and Stoneskin expiration accepted. V1.113 tests only Geyser static return, Stoneskin grounded gesture/movement and body/cloth rendering.
- Rampage opening charge damage on real enemies and latest shield orientation require explicit confirmation.
- Appearance inventory recorded in V1_112_APPEARANCE_CATALOG.md: 1,502 candidate export tables read, 269 detailed assets, Caltrops base/frost 1/2/3 visual bindings confirmed. Next implement a visual-only selector after runtime validation; tool/core usage, unlocks and equipped saves remain separate.
- P2 runtime tests deferred by user. Audit private instancing of mutable interruption-notify caches before simultaneous foreign casts; existing single-player acceptance is not coop proof.
- Heavy landing recovery remains a separate, previously retained experiment; no dash-recovery revival or broader control changes in V1.112.
- Arena HUD disappearance: no longer reproducible in later user tests; monitor logs, no forced HUD restore.

Preserve: accepted pistol OFF arms/holsters/blade effects, execution camera, horse jump/audio, War arm, cursor, Leviathan/Stinger, skins, gauges/ammo/hotkeys, blur controls and updater. Never treat older experimental feedback as blanket validation.
