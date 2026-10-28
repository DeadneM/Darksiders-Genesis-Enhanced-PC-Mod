## V1.126 - essais Recovery et impact Tremor

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base V1.125 controlee, V1.124 validee reste sur main. A la demande utilisateur, apparences actuelles conservees et Leviathan confirme independant/inchange.

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

**28 octobre 2026 : V1.124 / correctif du petit saut final Caltrops War VALIDES par l'utilisateur dans son essai. Sujet clos ; Geyser/Stoneskin et acquis conserves.** [Retour exact, limites et suite](V1_124_USER_VALIDATION.md) - [file active actuelle](ACTIVE_QUEUE.md). Pas de nouvelle build ou installation ; les statuts experimentaux ci-dessous decrivent la livraison avant ce retour et restent historiques.

## V1.124 - Caltrops War, essai natif unique de retour au sol

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40601516701), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c6751b4ef8835f25e9a72cd6c424d4ea833bcff6/downloads/DarksidersGenesis_V1.124_WAR_CALTROPS_GROUND_STATE_TEST.zip) - [Recu](V1_124_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Essai Caltrops EXPERIMENTAL, non valide en jeu ; pas d'installation locale. V1.123 et acquis valides conserves.**

**CONSTRUITE / CONTROLEE ; ESSAI EXPERIMENTAL, NON VALIDE EN JEU.** Base cumulative V1.123 ; Stoneskin Strife/War, Geyser et geste Caltrops acceptes sont conserves. L'utilisateur a rouvert le sujet puis explicitement autorise « Autoriser cet appel unique » le2026-10-28, ce qui remplace la decision anterieure de differer cet essai precis. [Accord et perimetre](V1_124_CALTROPS_GROUND_STATE_REVIEW.md).

Apres un Caltrops War parti du sol, une seule demande publique SetMovementMode(1,0) dans l'ASI vise a eviter la pose aerienne parasite finale. Le moteur gere ensuite normalement sol/gravite ; pas de maintien, second appel ou modification brute de mouvement/animation. Meme joueur/monde, identites vivantes, montage/capacite courants, debut0..0.15s, faible deplacement vertical/horizontal et vitesse verticale quasi nulle obligatoires. Cas inconnu : comportement V1.123 conserve. Cout/degats/pics, geste1.8333s et fondus.10/.18, sons et gardes donneuses restent identiques. [Contrat, objectif et test](V1_124_WAR_CALTROPS_GROUND_STATE_TEST.md).

Pas de nouveau hook, scanner d'objets ou calcul de pose par frame ; pas de changement INI/Hotkeys ni du chemin manette. Lecteur externe en lecture seule, ancien garde VM refuse absent. Compilation uniquement GitHub, aucune installation ou relance locale. Le sursaut reste NON VALIDE jusqu'a essai utilisateur ; coop en dernier.

---

## Decision utilisateur : conserver V1.123 pour le moment

L'utilisateur choisit explicitement « Conserver V1.123 pour l'instant ». L'essai SetMovementMode est mis de cote : aucun patch applique, aucun appel moteur, aucune nouvelle build ou installation. Ne pas relancer cet essai ni redemander son autorisation sans reprise explicite du sujet par l'utilisateur. V1.123 reste la version installee ; Stoneskin/Geyser et le geste Caltrops acceptes sont conserves. Seul le petit saut final Caltrops reste OPEN.

---

## Caltrops War - releve live apres retour V1.123

Deux activations capturees en lecture seule : le personnage ne monte pas en position monde ; sa pose est pourtant marquee en l'air. Mouvement natif **sol -> vol (~1.83s) -> chute (~47ms) -> sol** observe deux fois, avec elevation du bassin du modele. Le sursaut final n'est pas declare corrige. Proposition : une demande publique native de marche, une seule fois au debut d'un Caltrops War parti du sol, avec controles stricts. **Patch refuse avant application par l'approbation automatique : accord humain explicite requis pour cet essai SetMovementMode, distinct des particules Stoneskin.** [Perimetre et risques](V1_124_CALTROPS_GROUND_STATE_REVIEW.md). Aucun nouveau code/build/installation ; Stoneskin valide et Geyser restent inchanges.

---

## Retour utilisateur V1.123 - Stoneskin valide, fin Caltrops ouverte

Stoneskin sur Strife (explosion et cendres incluses) et Stoneskin natif sur War : **VALIDES par l'utilisateur dans les essais effectues**. Caltrops War : geste et sortie des pics acceptes, **petit saut final toujours present** sur la video ; reste a corriger. Geyser et les acquis restent inchanges, coop en dernier. Video/logs examines ; la transition native vol/chute est une cause candidate, pas encore mesuree sur cette execution. Aucun nouveau code/ZIP livre pour ce diagnostic. [Analyse et prochaine etape](V1_123_USER_VALIDATION.md).

---

## V1.123 - Effets Stoneskin et coup de pied Caltrops

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40560388902), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6091b34fdafa759d6b37d75b17ab7ac7cc2b5e97/downloads/DarksidersGenesis_V1.123_STONESKIN_TRANSITIONS_CALTROPS_STOMP_TEST.zip) - [Recu](V1_123_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Installee sur demande avec sauvegarde V1.122 verifiee, INI/PAK conserves ; jeu non lance. Effets Stoneskin et geste Caltrops EXPERIMENTAUX, non valides en jeu. Geyser valide conserve.**

**CONSTRUITE / CONTROLEE ; NOUVEAUTES EXPERIMENTALES, NON VALIDEES EN JEU.** Base cumulative V1.122. Geyser est explicitement VALIDE : aucun changement. Geste Stoneskin1.25s, fondus, materiaux et duree native conserves. Essai visuel limite explicitement autorise : explosion Base/Molten quand le revetement apparait sur Strife, cendres quand le statut expire completement. API publique SpawnEmitterAttached avec contrat72 octets confirme en lecture seule, controle avant chaque appel. Aucun appel/ecriture du lecteur externe.

Caltrops War : geste complet de son Stoneskin (~1.8333s), pics au signal natif du coup de pied, une seule creation. Trois gardes natifs verifies par signatures filtrent seulement les notifications de peau/acteur/blast du montage prive de Caltrops ; le hook de particules existant recoit le signal. War Stoneskin ordinaire est transmis intact. Lifecycle vol/chute natif conserve : absence de rebond a verifier. Ancien chemin de particules qui refusait les effets et getter Caltrops inutilise supprimes. [Contrat et essais](V1_123_STONESKIN_TRANSITIONS_CALTROPS_STOMP_TEST.md), [autorisation et perimetre](V1_123_STONESKIN_FX_REVIEW.md).

Clone/Rampage/WorldEnder, Geyser, materiaux Stoneskin, camera/manette/cheval/menus/updater et INI/Hotkeys conserves. Les nouveaux effets et le geste Caltrops restent EXPERIMENTAUX, NON VALIDES EN JEU. Aucun lancement, compilation ou remplacement du jeu local.

---

## V1.122 - Memes gestes et effets, activations plus courtes

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40545654508), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ee21e5c88a1f0fa8eedd6e2d4206529b86360d97/downloads/DarksidersGenesis_V1.122_SHORT_CASTS_GROUNDED_CALTROPS_TEST.zip) - [Recu](V1_122_DELIVERY_RECEIPT.md). Updater debug et Defender controles. V1.122 installee sur demande avec sauvegarde verifiee de V1.121, INI/PAK conserves, sans lancement du jeu. Gestes raccourcis et Caltrops EXPERIMENTAUX, non valides en jeu.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.121. Precision utilisateur : garder les gestes et les effets choisis pour Geyser/Stoneskin, raccourcir seulement l'activation. V1.121 installee sur demande avec sauvegarde de V1.120 et INI personnel conserve ; jeu non lance par l'agent.

Geyser Strife : meme Aether_Channel_Start et memes effets, geste complet **1.75s au lieu de 2.3667s**. Declenchements des pics et degats conserves exactement (dernier effet 0.71924s, blast 0.48807s). Seule la fin des fenetres d'interruption se ferme au nouvel arret ; leurs debuts restent natifs. Aucun pic supprime ou accelere, aucun maintien de pose ou retour ajoute. Stoneskin Strife : meme geste, memes fondus, particules et materiaux, activation habituelle **1.25s au lieu de 1.50s** ; duree du statut/protection et vie des particules inchangees.

Caltrops War : essai different, un seul War_Channel_End complet a sa vitesse native (~0.6667s), a la place de Start et sans raccord Start/End. Ce geste est deja disponible dans la decouverte bornee ; aucune nouvelle recherche par frame. S'il manque ou expire, V1.121 reste le repli. Spawn, degats, cout et lifecycle physique natifs conserves ; absence du rebond a verifier en jeu. Pas de patch VM/mouvement ou nouvel hook. Clone/Rampage/WorldEnder, War Stoneskin, camera/manette/cheval/menus/updater et INI/Hotkeys gardes. [Contrat et essais](V1_122_SHORT_CASTS_GROUNDED_CALTROPS_TEST.md).

---

## V1.121 - Retour aux gestes continus, particules de lancement Stoneskin

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40542119595), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/eb36a53e2a1bd3280a973b4caa6c4e025063d30f/downloads/DarksidersGenesis_V1.121_NATIVE_CAST_RESET_STONESKIN_FX_TEST.zip) - [Recu](V1_121_DELIVERY_RECEIPT.md). Updater debug V1.121 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Retour Caltrops/Geyser et lancement Stoneskin EXPERIMENTAUX ; activation/materiaux Stoneskin deja valides conserves.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.120, avec retour cible vers les chemins precedant les retouches de duree. Reference acceptee des capacites : V1.112 ; acquis valides plus recents conserves. Dernier verdict utilisateur : Caltrops V1.120 REJETE, retour Geyser REJETE (mouvement vertical), Stoneskin activation/rendu/restauration VALIDES sauf particules de lancement absentes.

Caltrops War retrouve un seul geste sur la duree native du pouvoir (environ 2.333s avant facteur natif), comme avant le raccourcissement V1.108 : retrait du retour War_End et de toute la continuite temporaire du graphe V1.120. Geyser Strife retrouve un seul geste continu sur 2.3667s : suppression de la pose de fin ajoutee, du maintien d'une derniere image et du module de reechantillonnage GeyserStaticPose, y compris son appel dans le hook partage. Les callbacks de degats et fenetres d'interruption natives restent a leurs temps acceptes. Aucun patch de mouvement, saut, vol/chute, input ou script VM. **Le geste pourra etre plus lent ; l'absence de rebond reste a tester.**

Stoneskin garde exactement son activation acceptee (normalement 1.50s), ses fondus et ses materiaux. Les deux notifications natives de particules de lancement Base/Molten sont copiees dans son seul montage prive Strife ; phases et filtres d'affichage conserves, doublons evites. Aucun transfert du blast, du statut War/Stinger, des effets de surface, du camera shake ou des callbacks de gameplay. Un contrat indisponible est signale au journal et laisse le pouvoir accepte fonctionner.

Clone, Rampage, World Ender, War Stoneskin natif, apparences, camera/manette/cheval/menus/updater et reglages personnels preserves. INI 212 cles et 55 Hotkeys inchanges. Compilation uniquement sur GitHub debug, aucun lancement ou installation locale. [Objectif, changements et tests](V1_121_NATIVE_CAST_RESET_STONESKIN_FX_TEST.md).

---

## V1.120 - Continuite de la pose de War pendant Caltrops

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40519850736), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d218ecb22e9f4e3aa2e3ad528775d63e86f0c347/downloads/DarksidersGenesis_V1.120_CALTROPS_SOURCE_POSE_TEST.zip) - [Recu](V1_120_DELIVERY_RECEIPT.md). Updater debug V1.120 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Continuité visuelle Caltrops EXPERIMENTALE, a valider en jeu ; Geyser/Stoneskin +0.50s conserves.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.119 : demi-seconde ajoutee a Geyser et Stoneskin conservee exactement. Precision utilisateur sur Caltrops : surtout un mouvement du corps, sans deplacement reel. Le diagnostic s'oriente donc vers la reprise de la pose, pas vers un vrai saut physique ; le garde VM reste abandonne.

Le TPS Slot du graphe War a bAlwaysUpdateSourcePose=false dans le PAK. L'essai active temporairement cette option native uniquement pour Caltrops transfere sur War : sa pose de fond continue a se mettre a jour sous le geste, puis le reglage d'origine est restaure. Champ exact verifie par reflection, identites faibles du joueur/acteur/mesh/animation/pouvoir et monde controles ; un reglage deja actif reste actif, un contrat inconnu garde le comportement precedent. [Principe documente par Epic](https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/AnimNode_Slot?application_version=4.27).

Le lien entre la pose arretee et le rebond est une **hypothese**, pas un resultat observe en jeu. Aucun nouveau hook, changement de duree, mouvement/vol/chute, script VM, callback, cout, degat ou projectile. Clone/Rampage/World Ender, Native Strife, camera/manette/menus et apparences gardes. INI 212 cles et 55 Hotkeys inchanges. Compilation GitHub, aucune installation/lancement automatique. [Contrat, preuves, tests et rollback](V1_120_CALTROPS_SOURCE_POSE_TEST.md).

---

## V1.119 - Une demi-seconde de plus pour Geyser et Stoneskin

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40513219454), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/38e0bfab00c001975d8d74c83414cfaa27e8cdc2/downloads/DarksidersGenesis_V1.119_LONGER_STRIFE_CASTS_TEST.zip) - [Recu](V1_119_DELIVERY_RECEIPT.md). Updater debug V1.119 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Animations +0.50s EXPERIMENTALES a valider ; Caltrops reste OUVERT et inchange.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.118, installee et verifiee lors de la demande precedente, INI personnel conserve. Dernier retour : ajouter 0.5 seconde aux deux animations ; Caltrops War fait toujours le petit saut, donc son changement de duree V1.118 ne le corrige PAS.

Geyser immobile : retour 0.65 -> **1.15s**, meme debut 0.85s et fondu progressif 0.22s ; fin visuelle 2.00s au lieu de 1.50s, dans le montage natif 2.3667s. Aucun ajout au verrouillage du personnage, aux callbacks, au deplacement ou au saut. En mouvement, le chemin natif est conserve. Stoneskin Strife : activation habituelle 1.00 -> **1.50s** (+0.50s a la duree anterieure), memes fondus 0.16/0.30s, memes materiaux/sons et duree du statut. War Stoneskin reste natif.

Caltrops reste **OUVERT** et inchange dans cette version. Le journal V1.118 confirme le retour War_Start/End actif lors des essais ; allonger le geste n'a pas supprime le saut. Une observation du personnage entier versus sa pose doit departager un vrai decollement et un mouvement visuel. Le lifecycle natif vol/chute est une hypothese de source, pas une mesure en jeu. Le garde VM rejete precedemment reste exclu ; choix utilisateur de la methode simple conserve. Pas de nouveau hook ni nouvelle experience de mouvement.

Clone, Rampage, World Ender, menu Appearance, visee/camera/manette, INI 212 cles et 55 Hotkeys inchanges. Compilation GitHub seulement, pas d'installation ou lancement automatique. [Contrat, criteres et tests](V1_119_LONGER_STRIFE_CASTS_TEST.md).

---

## V1.118 - Transitions Geyser / Stoneskin

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40504809854), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a080bb2202058ab1d3879a29231f8f8a846f66d5/downloads/DarksidersGenesis_V1.118_SMOOTHER_STRIFE_CASTS_TEST.zip) - [Recu](V1_118_DELIVERY_RECEIPT.md). Updater debug V1.118 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Transitions EXPERIMENTALES a valider ; Caltrops reste OUVERT, garde refuse non inclus.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.117. Dernier retour utilisateur : onglet Appearance present et Clone War OK (VALIDES) ; Geyser et Stoneskin un peu trop abrupts ; petit saut de Caltrops toujours present (OUVERT). Les variantes de la bibliotheque et le rendu mineral precis de Strife ne sont pas explicitement valides par ce retour. Rampage reste valide et intact.

Geyser immobile : meme debut a 0.85s, retour visuel etire de 0.50 a 0.65s, fondu progressif sur 0.22s au lieu du fondu lineaire 0.12s. Racine/courbes actives, deplacement et saut inchanges. Stoneskin : meme geste de 1 seconde, transitions d'entree/sortie 0.16/0.30s au lieu de 0.10/0.18s ; effets, cout, duree du statut et restauration inchanges. Aucun autre changement de gameplay ni nouveau hook.

Caltrops : le journal confirme le retour War_Start/End actif. Le Blueprint natif passe ensuite en vol (5), puis chute (3) a la fin ; c'est une cause candidate du petit saut, pas une transition mesuree en direct. Le garde VM propose a ete refuse par le controle d'approbation automatique pour risque de crash/corruption et absence d'accord explicite pour cette methode. Il n'est ni ajoute ni compile. Le lecteur n a pas trouve le processus du jeu lors du releve en lecture seule ; aucun lancement. L utilisateur demande ensuite la methode la plus simple : Caltrops reprend le meme retour natif 0.55s que Clone valide (total environ 1.12s, contre 0.87s en V1.117 et 2.33s historiquement). Seule cette duree visuelle change ; lifecycle vol/chute intact, aucun garde VM. Clone reste identique. Le saut de Caltrops reste a verifier. [Diagnostic et choix simple retenu](V1_118_CALTROPS_REVIEW.md).

Compilation GitHub seulement, pas d'installation locale ; INI 212 cles, 55 Hotkeys, anciennes archives et main/archive preserves. [Contrat et tests](V1_118_SMOOTHER_STRIFE_CASTS_TEST.md).

---

## V1.117 - Retours d'animation, Stoneskin et onglet Appearance

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40494010438), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d5df57ef5cff693e8496e81bcb080baf4faefd3f/downloads/DarksidersGenesis_V1.117_CAST_RETURN_MINERAL_CLOTH_MENU_TEST.zip) - [Recu](V1_117_DELIVERY_RECEIPT.md). Updater debug : V1.117 et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Corrections EXPERIMENTALES, a valider en jeu. Rampage conserve, INI/raccourcis inchanges.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.116 ; acquis de gameplay retenus V1.112 et validations suivantes. Retour video du 27/10 : Rampage sur Strife parfait, conserve sans changement. Geyser immobile garde une pose apres les pics, Stoneskin trop court et trop en feu, Clone War trop court, Caltrops War fait un bref saut. L'utilisateur confirme ne voir aucun nouveau choix d'apparence ; aucune variante V1.116 n'est donc annoncee validee.

Nouvel onglet **Appearance**, visible directement entre Gameplay et Camera : ouvrir **Effect appearances (experimental)** puis Tools / Powers / Strife ammunition / etc. Les groupes restent fermes par defaut. Les 43 groupes / 451 effets natifs sont conserves ; aucun nouveau skin invente. Models/armures/reticule/Leviathan sont regroupes dans ce meme onglet. [Catalogue des choix](V1_116_EFFECT_LIBRARY_CATALOG.md).

Geyser : retour visuel seulement a l'arret, avec contexte natif prive pour les os et courbes ; le journal V1.116 montrait que les contextes avec courbes etaient refuses. Mouvement/saut/racine/courbes actives et fin native du pouvoir restent intacts. Stoneskin : activation propre de Strife portee de 0.75 a 1 seconde ; corps/gear mineral sans emission uniforme, tissu garde son materiau de feu. Clone/Caltrops sur War : retour natif War_Channel_End ajoute au geste War_Channel_Start, 0.55 / 0.30 seconde ; effets/sons/callbacks existants conserves. Le resultat visuel exact attend l'essai en jeu.

Pas de compilation/installation locale, lancement ni ecriture dans le jeu. INI 212 cles et 55 Hotkeys inchanges. [Contrat, preuves video, criteres et rollback](V1_117_CAST_RETURN_MINERAL_CLOTH_MENU_TEST.md).

---

## V1.116 - Bibliotheque des effets natifs

**CONSTRUITE / CONTROLEE : [64/64 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40473336773), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/22f011465c0b1eb69bf068a55a4b0fdb3e66e341/downloads/DarksidersGenesis_V1.116_NATIVE_EFFECT_LIBRARY_TEST.zip) - [Recu](V1_116_DELIVERY_RECEIPT.md). Updater debug V1.116 : selection et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Bibliotheque EXPERIMENTALE, NON VALIDEE EN JEU ; V1.114/115 restent en attente du retour utilisateur.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** L'utilisateur demande toutes les autres familles ; V1.114 et V1.115 restent en attente de son essai. Base de gameplay retenue V1.112 ; V1.113 reste REJETEE. Aucun nouveau changement de visée, dash, controle, animation ou revetement Stoneskin dans ce lot.

Appearance > Effect appearances : **43 groupes / 451 ParticleSystem natifs / 98 roles visuels** verifies dans le PAK principal. Outils, pouvoirs, munitions Strife, armes War, montures/formes et coeurs. Variantes nommees seulement lorsqu'un asset natif du meme role existe ; **Native / Hidden** pour les familles sans autre habillage prouve. Chaque groupe est independant, toutes les nouvelles options sont Native et les categories sont fermees par defaut. [Liste exacte des choix](V1_116_EFFECT_LIBRARY_CATALOG.md).

Un seul moteur et le hook visuel deja utilise en V1.115 remplacent le prototype Caltrops, sans doublon ni ancien module compile. Templates a la creation uniquement ; Hidden garde le composant et son lifecycle natifs, masque le rendu puis restaure uniquement la visibilite modifiee. Pas de nouveau hook de combat, classe de projectile/coeur, degat, cout, statut, sauvegarde, modele ou materiau remplace. Les slots Caltrops 0..4 et les 170 anciens reglages restent identiques ; 42 nouvelles cles Native, 212 au total, 55 ID Hotkeys inchanges.

Assets deja charges uniquement : une variante manquante garde le rendu natif. Les effets deja presents ou les composants crees directement hors du point de creation commun peuvent garder leur apparence ; ce lot ne pretend pas remplacer toutes les textures du jeu. Leviathan, Stinger, armures, pistolets/bras/holsters et corps Stoneskin gardent leurs options et chemins precedents. Les formes/montures changent ici seulement leurs effets, pas leurs modeles. [Contrat, preuves, tests et rollback](V1_116_NATIVE_EFFECT_LIBRARY_TEST.md).

---

## V1.115 - Premier choix d'apparence Caltrops

**CONSTRUITE / CONTROLEE : [64/64 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40452540269), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/979ce9420809716c83f6ab7c17074592746db481/downloads/DarksidersGenesis_V1.115_CALTROPS_BURST_APPEARANCE_TEST.zip) - [Recu](V1_115_DELIVERY_RECEIPT.md). Updater debug : V1.115 selectionnee et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Apparences EXPERIMENTALES, NON VALIDEES EN JEU ; V1.114 reste egalement en attente du retour utilisateur.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** L'utilisateur demande de poursuivre les skins et remet son essai V1.114 a plus tard : **V1.114 n'est pas validee**. Base retenue de gameplay V1.112, experience V1.114 conservee sans modification dans ce cumul. V1.113 reste REJETEE. Rampage Strife / trois pouvoirs sur War / expiration Stoneskin gardent leur validation precedente.

Menu Gameplay > apparences > Ability appearances : **Caltrops Native / Base / Frost 1 / Frost 2 / Frost 3**. Ce premier choix agit sur le **glyphe d'apparition et l'explosion des nouveaux effets**. Modele au sol, effet d'attente et glyphe en boucle restent natifs. D'autres familles restent au catalogue, pas annoncees comme selectionnables. Aucun coeur, degat, rayon, cout, statut, projectile ou progression n'est remplace.

Une substitution du seul template de particules a la creation, sur six references exactes du catalogue, dans le monde local courant. Pas de modification des CDO, materiaux, composants existants ni sauvegardes. Native est le defaut et restaure les prochains effets. Assets deja charges uniquement ; un effet absent garde son rendu natif. Pas de chargement synchrone ou parcours d'assets au demarrage avec Native. Groupe ferme par defaut.

170 cles INI (169 preservees + CaltropsAppearance=0), 55 ID Hotkeys inchanges. Anciennes archives et main/archive conserves. Compilation exclusivement GitHub ; installation via updater debug par l'utilisateur. [Contrat, limites, tests et rollback](V1_115_CALTROPS_BURST_APPEARANCE_TEST.md).

---

## V1.114 - Reprise V1.112, Geyser statique et Stoneskin

**CONSTRUITE / CONTROLEE : [63/63 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40445567125), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4ee6f649835353de4e369d03b63100b66112c967/downloads/DarksidersGenesis_V1.114_RETAINED_CASTS_STATIC_GEYSER_FIRE_TEST.zip) - [Recu](V1_114_DELIVERY_RECEIPT.md). Updater debug : V1.114 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP et les quatre fichiers, protections actives. EXPERIMENTALE, NON VALIDEE EN JEU. V1.113 REJETEE, base retenue V1.112.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** V1.113 REJETEE : le saut reste bloque apres Geyser, meme apres la fin des pics ; son geste Stoneskin et le rendu corps/tissu ne correspondent pas a la demande. Retour complet du constructeur de montage V1.112, sans les segments ni nouveaux etats d'interruption V1.113. Rampage Strife, trois pouvoirs Strife sur War et expiration Stoneskin restent VALIDES par l'utilisateur.

Geyser : gameplay et montage V1.112 conserves ; seulement a l'arret, presentation de la vraie fin Aether via l'evaluation visuelle existante. Aucune ecriture de deplacement, saut, commande, duree, evenement ou aptitude. Mouvement, intention de mouvement et chute conservent la pose native. Une courbe non prise en charge refuse l'ajustement visuel ; le gameplay reste V1.112.

Stoneskin : geste Aether V1.112 raccourci a 0,75 s a vitesse normale, statut et expiration independants. Materiaux prives heritant des vrais shaders natifs A pour corps/equipement et B pour tissu, base/Molten selon le statut : ne supprime plus les regions en feu. **Le placement final des textures War sur les UV de Strife reste experimental.**

169 cles INI / 55 ID Hotkeys, camera, XInput, tirs et anciennes archives preserves. Coop en dernier. Compilation exclusivement GitHub ; installation manuelle/updater debug par l'utilisateur. [Contrat, limites, tests et rollback](V1_114_RETAINED_CASTS_STATIC_GEYSER_FIRE_TEST.md).

---

## V1.113 - Retours au sol Strife et roles Stoneskin

**CONSTRUITE / CONTROLEE : [62/62 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40410419216), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/84583fc7f21b497a747733ddb3f1ea923fa3c6c9/downloads/DarksidersGenesis_V1.113_GROUNDED_GEYSER_STONESKIN_ROLES_TEST.zip) - [Recu](V1_113_DELIVERY_RECEIPT.md). Updater debug : V1.113 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Nouveaux ajustements EXPERIMENTAUX, NON VALIDES EN JEU.**

**PREPAREE / EXPERIMENTALE, NON VALIDEE EN JEU. Compilation et livraison GitHub controlees, essai en jeu requis.** Base V1.112. Retours utilisateur : fin Rampage Strife, trois pouvoirs de Strife sur War et expiration/restauration Stoneskin Strife explicitement VALIDES. Ces chemins retenus restent preserves. Restent la levitation Geyser a l'arret, l'animation Stoneskin trop longue avec glissement, et le corps rouge/tissu pierre.

Geyser garde son debut accepte puis joue son vrai retour Aether au sol a vitesse native, au lieu de tenir sa pose suspendue ; pic/cout/duree et commandes inchanges. Stoneskin reprend le geste sol Strife et sa fin a vitesse native, avec fenetres d'interruption natives copiees sur des etats prives : MOVE a 0,942 s. Duree/cout/statut/sons restent natifs, pas de Finish anticipe ni touche simulee.

Le revetement vise les vrais slots Strife_Body / Strife_Gear, garde les deux tissus natifs, et coupe sur ses seuls materiaux prives l'emission rouge dont le masque UV est propre a War. Expiration/restauration retenues, refus mis en cache pour eviter de recreer un materiau a chaque image. **Le tissu conserve son apparence native ; le rendu Molten de la cape de War n'est pas reproduit a l'identique sur Strife.**

169 cles INI / 55 ID Hotkeys, camera, input/XInput, tirs, fonctions validees et anciennes archives conserves. P2 toujours differe. Compilation uniquement GitHub, installation par l'utilisateur via updater debug. [Contrat, preuves, essais et rollback](V1_113_GROUNDED_GEYSER_STONESKIN_ROLES_TEST.md).

---

## V1.112 - Fins des pouvoirs et revetement Stoneskin Strife

**CONSTRUITE / CONTROLEE : [62/62 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40402937664), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2b609415cffaff77a99fc1615cf8ba51c3574813/downloads/DarksidersGenesis_V1.112_POWER_BLENDS_STRIFE_STONESKIN_TEST.zip) - [Recu](V1_112_DELIVERY_RECEIPT.md). Updater debug : V1.112 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

**EXPERIMENTALE / NON VALIDEE EN JEU. Compilation et livraison GitHub controlees ; essai en jeu requis.** Base V1.111, avec les acquis explicitement valides : effets WingClip V1.110, sons/vitesses des pouvoirs, flash Lava et fonctions precedentes. Le retour V1.111 signale des fins abruptes pour Rampage/Geyser Strife et les trois pouvoirs de Strife sur War ; cela reste le critere de validation du nouveau lot.

Geyser et World Ender gardent une pose continue jusqu'au fondu, sans saut vers un segment idle. Les copies privees des montages conservent leurs fondus plus longs et ont un minimum de sortie de 0,18 s ; Caltrops et Stoneskin utilisent aussi cette sortie douce. Rampage suit le poids et la position de son vrai montage pendant le fondu, meme apres la fin de la capacite, au lieu d'arreter brutalement la pose. Mouvement, durees, evenements de degats/cout et fenetres de reprise des commandes conserves.

Capture Stoneskin : War recoit ses materiaux natifs, Strife recoit les memes statuts sans revetement. Nouveau candidat : quatre materiaux dynamiques prives pour le corps/equipement/tissu de Strife, parametres natifs Stoneskin ou Molten selon les vrais statuts. Les materiaux d'origine sont gardes par le parent du materiau prive, puis restaures a l'expiration ou transformation ; un remplacement natif gagne. War, Stinger, statistiques et durees restent natifs. **Compatibilite visuelle des textures de War sur Strife a verifier en jeu.**

169 cles INI / 55 ID Hotkeys conserves. Aucun changement de camera, tirs, input/XInput, reception lourde ou reglages. P2 toujours differe. Compilation uniquement GitHub ; installation manuelle/updater debug par l'utilisateur. Anciennes archives preservees. [Preuves, limites, essais et rollback](V1_112_POWER_BLENDS_STRIFE_STONESKIN_TEST.md).

---

## V1.111 - Repere Rampage et rythme des gestes natifs

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40363876283), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0f1a5d47ac05b67ed950d9338fe06d8b7c38a3d0/downloads/DarksidersGenesis_V1.111_NATIVE_POWER_TIMING_FRAME_TEST.zip) - [Recu](V1_111_DELIVERY_RECEIPT.md). Updater debug : V1.111 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

**EXPERIMENTALE : compilation, controles et livraison GitHub verifies ; NON VALIDEE EN JEU.** Base V1.110, effets du coup tournoyant explicitement VALIDES par l'utilisateur, sons/vitesses des autres pouvoirs et flash Lava conserves. Retour V1.110 : bouclier Rampage present mais tourne de 90 degres a droite dans les deux vues ; fin nettement meilleure mais court arret ; geste Geyser Strife mal synchronise, pas une demande de raccourcir seulement le blocage final. Degats de charge non confirmes.

Le bouclier prive reprend la rotation du vrai socket War VFX_Root lue dans le squelette natif ; aucun angle de camera compense. Les fenetres natives d'interruption des fins Rampage et de Geyser sont conservees sur le montage prive, avec debut/fin exacts. Geyser joue le geste propre Strife a sa vitesse d'origine puis revient a sa pose d'attente : fini l'etirement uniforme d'une seconde sur 2,37 secondes. Temps de SpawnBlast, effets, cout et duree du montage restent natifs. Aucun Finish anticipe, timer ou nouveau hook.

Acquis du coup tournoyant, tirs, camera, XInput et reception lourde inchanges ; 169 cles INI / 55 ID Hotkeys conserves. P2 differe. Compilation uniquement GitHub, updater debug et ZIP quatre fichiers NoCompression ; aucune installation locale. [Preuves, limites, essais et rollback](V1_111_NATIVE_POWER_TIMING_FRAME_TEST.md).

---

## V1.110 - Charge Rampage, sortie de pose et coup tournoyant

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40356864172), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b43590e2f481e48ece13cd321c568001f26a4846/downloads/DarksidersGenesis_V1.110_NATIVE_CHARGE_WINGCLIP_TEST.zip) - [Recu](V1_110_DELIVERY_RECEIPT.md). Updater debug : V1.110 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

**EXPERIMENTALE : compilation, controles et livraison GitHub verifies ; NON VALIDEE EN JEU.** Base fonctionnelle V1.103, acquis precedents conserves ; sur V1.109 l'utilisateur valide les vitesses/sons des pouvoirs transferes et le flash Lava au canon. Demarrage juge moins lent. Charge Rampage (degats/effet), petit mouvement apres Talon du Diable et effets du coup tournoyant restent ouverts.

Capture groupee V1.109 : deux Rampages War et deux Strife, puis les quatre configurations de lames. Le coup tournoyant est WingClip, pas Kick Launch. Ses arcs sont reduits a 40 % et rapproches de leur main native uniquement si sa lame est OFF ; meme chemin visuel que les attaques deja acceptees. Rampage reprend neuf evenements natifs de charge sur son montage prive (blast, six effets de sol, bouclier temporel et collision), avec les classes/temps/durees du jeu. Deux hooks de charge retires. Le suivi de pose cesse lorsque le vrai montage se termine, meme si la capacite reste active environ 0,25 s.

Sons, vitesses, flash Lava, trajectoires, stats/chiffres de degats, inputs/camera et reception lourde valides conserves ; 169 cles INI, 55 ID Hotkeys, aucune nouvelle option. P2 differe. Les degats de charge n'ont pas ete compares sur des ennemis pendant le releve : ils restent a tester. Compilation uniquement GitHub, updater debug et ZIP quatre fichiers NoCompression ; aucune installation locale. [Preuves, limites, essais et rollback](V1_110_NATIVE_CHARGE_WINGCLIP_TEST.md).

---

## V1.109 - Livraison ZIP sans compression

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40346271897), zero avertissement/erreur. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/24cf9a9ecd44c67ff69be88b1149e3d5b7f1b675/downloads/DarksidersGenesis_V1.109_TRANSPARENT_ZIP_DELIVERY_TEST.zip) - [Recu](V1_109_DELIVERY_RECEIPT.md). Defender : aucune menace detectee dans le ZIP final et les quatre fichiers extraits ; protections actives. Updater debug : V1.109 selectionnee et octets verifies. Code de jeu V1.108 conserve, toujours EXPERIMENTAL / NON VALIDE EN JEU.**

**EXPERIMENTALE : recompilation, livraison GitHub et scan Defender verifies. Code de jeu V1.108 conserve ; NON VALIDE EN JEU.** V1.108 est disponible sur GitHub mais Defender a bloque son ZIP avec Trojan:Script/Ulthar.A!ml. Le controle sur les quatre fichiers extraits ne detecte rien ; un ZIP NoCompression contenant les memes fichiers verifies par SHA256 passe aussi le controle. C'est un constat sur ce moteur/signatures, pas une certification generale ni un avis Microsoft de faux positif.

V1.109 change uniquement l'identite de version et le conditionnement ZIP NoCompression, avec comparaison de chaque fichier archive a la sortie compilee. Aucun nouveau correctif de jeu, option, touche ou changement des protections antivirus. 169 cles INI / 55 ID Hotkeys conserves. Les 173 anciennes archives restent dans GitHub ; l'antivirus a mis la copie locale de V1.108 en quarantaine, elle n'est ni restauree ni supprimee de l'historique. [Incident, preuves et controles](V1_109_TRANSPARENT_ZIP_DELIVERY_TEST.md).

---

## V1.108 - Rampage, presentation des pouvoirs, Lava et decouverte bornee

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40313742351), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4d27f93e3db5d829adf268bb07e58957cdaf51cf/downloads/DarksidersGenesis_V1.108_RAMPAGE_LAVA_POWER_PRESENTATION_TEST.zip) - [Recu](V1_108_DELIVERY_RECEIPT.md). Listing public updater debug : V1.108 selectionnee, octets verifies. Correctifs EXPERIMENTAUX, NON VALIDES EN JEU.**

**EXPERIMENTALE : compilation Windows x64 GitHub, controles et livraison verifies ; NON VALIDEE EN JEU.** Base fonctionnelle validee V1.103, code de transition V1.107 conserve. Les nouveaux essais V1.107 montrent des phases Start/Slam repetables ; l'utilisateur confirme encore absence des degats/effets de charge, mauvaises poses, lenteur des transferts et sons manquants. Cela ne valide pas tout Rampage.

Lot : attaches natives de la charge Rampage pour Strife, pose propre Strife_Dash puis Talon du Diable (AirKick1), sans remplacer le mouvement natif ; temps du geste War corrige pour Clone/Caltrops et preparation/retour World Ender, avec intervalle de degats du rayon conserve ; son de lancement valide avant le detour partage du dash et boucle sonore native du laser restauree. Flash Lava normal rattache au vrai canon correspondant, sans changement des projectiles/trajectoires. Arc de l'attaque secondaire reconnu pour les lames OFF. Trois scans d'objets remplaces par un seul, au plus 256 entrees et budget haute resolution de 1 ms controle toutes les 16 entrees.

169 cles INI et 55 ID Hotkeys conserves, aucune nouvelle option ni touche. Chemins input/camera, reception lourde, autres acquis et 172 anciennes archives gardes ; main/archive/v1.43 intacts. P2 reste differe. Voir [preuves, limites et essais](V1_108_RAMPAGE_LAVA_POWER_PRESENTATION_TEST.md). Telechargement depuis l'updater du jeu, canal debug ; aucun build ou remplacement local par l'agent.

---

## V1.107 - Rampage : conserver la transition native depart vers frappe

**CONSTRUITE / CONTROLEE : [60/60 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40225339622), zero avertissement/erreur MSVC. [ZIP GitHub immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/52cf0bc7939f65200ce17b978fd10e19109d0371/downloads/DarksidersGenesis_V1.107_RAMPAGE_NATIVE_PHASE_HANDOFF_TEST.zip) - [Recu](V1_107_DELIVERY_RECEIPT.md). Transition Rampage EXPERIMENTALE, NON VALIDEE EN JEU. Disponible pour l'updater canal debug, compilation uniquement GitHub. Audio V1.106 inchange, essai utilisateur differe.**

**EXPERIMENTALE, compilation et livraison GitHub controlees, essai en jeu requis ; NON VALIDEE EN JEU.** Base fonctionnelle V1.103 validee ; acquis conserves. Retour V1.106 : deplacement Rampage puis commandes bloquees, candidat REJETE pour cet essai. Sons V1.106 : essai utilisateur differe, adaptation audio conservee sans modification.

Journal : CAST depart puis, 413 ms plus tard, Slam3 refuse comme orphelin. Observe supprimait l'association des que Start n'etait plus actif, avant la creation native de la frappe ; le compagnon pouvait aussi consommer un token avant toute phase de fin. V1.107 conserve un token faible propre acteur/monde, exige Start avant une seule phase de fin et consomme le token au compagnon final ; un compagnon precoce reste neutralise sans effacer Start. Aucun ancien objet de capacite n'est conserve ou appele pour autoriser la transition. Pas de timer, nouvel input ou nouveau hook.

La copie privee des notifies est reconstruite depuis les evenements source, comme les autres transferts retenus ; elle ne doit pas etre identique octet par octet a une copie native qui peut instancier ses notifies. Les sections acceptent uniquement le relien natif vers leur copie ; boucles/temps/autres champs restent compares, tableaux partages toujours refuses. Mouvement natif et vitesse 1,5 du depart conserves.

Suggestion d'animation etudiee : Strife_Dash, Strife_AirDash_Fall et Strife_AirDash_Land_Still existent dans le PAK original. **Leur adaptation visuelle n'est pas incluse** : leur remplacement direct supprimerait la piste de mouvement Rampage ou ajouterait les callbacks du dash. La pose actuelle reste la locomotion de Strife ; cette version cible le blocage et la sortie du pouvoir. [Contrat, preuves et suite](V1_107_RAMPAGE_NATIVE_PHASE_HANDOFF_TEST.md).

Compilation uniquement GitHub Actions, updater canal debug ; aucune installation locale. 169 cles INI, 55 ID Hotkeys, reception lourde et chemins input/camera inchanges ; 171 anciennes archives preservees. Test : Rampage Strife native/TPS, relachement puis marche/attaque et second lancement, ensuite obstacle ; verifier que le journal atteint la phase de fin sans refus orphelin. P2 differe.

---

## V1.106 - Rampage et sons transferts, plan audio unique

**CONSTRUITE / CONTROLEE : [60/60 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40216944702), zero avertissement/erreur MSVC. [ZIP GitHub immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a8f88bdd9d5771085d4cddf838b7384d03eb24ef/downloads/DarksidersGenesis_V1.106_RAMPAGE_FOREIGN_AUDIO_COMPACT_TEST.zip) - [Recu](V1_106_DELIVERY_RECEIPT.md). Rampage et audio V1.106 restent EXPERIMENTAUX, NON VALIDES EN JEU. Disponible pour l'updater du jeu, canal debug ; compilation uniquement GitHub, aucune installation locale.**

**EXPERIMENTALE, compilation GitHub et livraison controlees, essai en jeu requis ; NON VALIDEE EN JEU.** Base fonctionnelle validee V1.103. Ce lot cumule la correction mesuree Rampage (vitesse native du depart 1,5) et la reprise des sons des pouvoirs transferee de V1.105. Regles, couts, degats et autres acquis conserves.

V1.105 a passe 60/60 suites Windows, mais MSVC a signale C6262 : CopyLaterSounds utilisait 18 828 octets de pile en dupliquant son plan d'evenements. V1.106 compacte et trie le meme plan en place ; pas de deuxieme plan, nouveau timer ou etat de cast. Le test couvre aussi deux sons issus des deux chronologies source, conserves et tries. Le nom du workflow devient generique, l'identite du ZIP continue de venir du BuildInfo compile. [Recu V1.105](V1_105_DELIVERY_RECEIPT.md).

La cause du grisement V1.104 est un abandon natif apres refus du montage de depart a 1,5 par le garde qui exigeait 1. Cette constante est corrigee, sans forcer une autre vitesse. Sons de lancement via le notify sonore natif sur le receveur apres succes ; sons plus tardifs Caltrops/Stoneskin copies sur leur geste prive. Les risques audio de boucle/doublon et le cycle Rampage complet restent a valider. [Preuves, limites et essais](V1_106_RAMPAGE_FOREIGN_AUDIO_COMPACT_TEST.md).

Compilation uniquement GitHub Actions ; telechargement depuis l'updater du jeu canal debug. Aucune installation locale. 169 cles INI, 55 ID Hotkeys et reception lourde validee inchanges ; 170 anciennes archives preservees. Test groupe : Rampage Strife native/TPS, relachement/obstacle/deuxieme lancement ; puis sons Geyser/Stoneskin Strife et Clone/Caltrops/World Ender War, sans doublon ni son qui persiste. P2 differe, pose Rampage locomotion Strife.

---

## V1.105 - Rampage : vitesse native, sons des capacites transferees

**EXPERIMENTALE, compilation GitHub et livraison controlees, essai en jeu requis ; NON VALIDEE EN JEU.** Base validee V1.103 conservee. Retour V1.104 : Rampage Strife ne se lance pas puis devient grise ; candidat REJETE dans cet essai. Sons des capacites transferees signales absents.

Le journal atteint bien le lancement, puis le constructeur refuse le montage. Lecture du jeu actuel : War_Atk_Rampage_Start_Montage a RateScale=1,5, alors que le garde V1.104 exigeait 1. Les quatre fins sont a 1. V1.105 exige ces valeurs mesurees et les conserve dans la copie ; aucun ralentissement force, vitesse du personnage ecrite ou retrait des controles de proprietaire. Les refus de copie/events sont maintenant distingues dans le journal.

Audio : les poses propres Caltrops/Stoneskin perdaient la piste sonore source. Geyser/Clone/World Ender copiaient deja des sons, tous leurs sons de lancement releves etant a t=0. Leur non-audibilite exacte n'a pas ete tracee dans le mixeur. Le candidat appelle une fois le notify sonore natif d'origine apres lancement confirme, sur le mesh propre du receveur ; ces sons sont exclus des pistes privees Geyser/Clone/World Ender pour eviter une double lecture. Les sons ulterieurs de Caltrops/Stoneskin sont copies sur leur geste prive avec le meme instant reel. Cue, voix, volume, pitch, attenuation et mixer restent natifs. Pas de nouveau hook, timer, touche imposee ou fichier audio ajoute.

Reglages, raccourcis, reception lourde validee et comportements des pouvoirs natifs inchanges. Debug uniquement ; 169 cles INI, 55 ID Hotkeys, 169 anciennes archives preservees. Compilation exclusivement GitHub Actions, telechargement via l'updater du jeu canal debug ; aucune compilation/installation locale. [Contrat, limites et essais](V1_105_RAMPAGE_NATIVE_RATE_FOREIGN_AUDIO_TEST.md).

Essais groupes : Rampage Strife en native/TPS (deplacement, degats, relachement, obstacle, deuxieme cast), puis sons Geyser/Stoneskin Strife et Clone/Caltrops/World Ender War. Verifier sons de lancement, sons pendant/apres effet, absence de doublon et pouvoirs natifs inchanges. P2 reste differe. La pose Rampage reste la locomotion Strife, sans charge/frappe dediee.

---

## V1.104 - Rampage Strife, refus identifie et identites natives

**CONSTRUITE / CONTROLEE : [59/59 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40203015804), zero avertissement/erreur MSVC. [ZIP GitHub immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/acd4ba2cf79577a3c8aa9b8cf09e40f4eee2193b/downloads/DarksidersGenesis_V1.104_STRIFE_RAMPAGE_NATIVE_IDENTITY_TEST.zip) - [Recu](V1_104_DELIVERY_RECEIPT.md). Rampage Strife reste EXPERIMENTAL, NON VALIDE EN JEU. Compilation exclusivement sur GitHub Actions ; archive disponible pour l'updater du jeu, canal debug. Aucune compilation ni installation locale.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; essai en jeu requis.** Base V1.103 reception lourde validee par l'utilisateur, avec Stoneskin/police V1.102 et Geyser/Clone/Caltrops V1.98/World Ender War V1.100 conserves. Priorite explicite : Rampage sur Strife.

Le releve actuel en lecture seule reproduit le refus de l'ancien garde : il exigeait le texte Upperbody_Flinches alors que le FName charge s'affiche UpperBody_Flinches. Les cinq classes de phases sont chargees ; squelette/racine/idle et proprietaires corrects. Le nouveau garde compare les vrais couples index/numero des cinq slots, verifie leur unicite et refuse le slot de mouvement s'il est rendu. Aucun changement global des comparaisons de noms.

Rampage est de nouveau selectionnable dans les trois emplacements de Strife : mouvement/segments/boucle/collisions et fins natifs, montages prives, pose de locomotion propre. Mode moteur 3 verifie avant de construire : il extrait ce mouvement sans le ponderer par un slot de rendu absent. Pas de pose osseuse War imposee a Strife, vitesse manuelle, fin par timer ou touche simulee. Refus/abandon journalises et natifs ; icone/classe du slot conservee si indisponible. **Mouvement effectif, degats, relachement, collisions et fin restent a tester.**

169 cles INI, 55 ID Hotkeys et defauts inchanges ; 168 anciennes archives preservees ; debug seulement. Installation manuelle jeu ferme, garder son INI/raccourcis/PAK/sauvegardes. [Contrat, preuves, limites et essais](V1_104_STRIFE_RAMPAGE_NATIVE_IDENTITY_TEST.md). P2 et effets P2 restent differes.

Test : Strife a pied, Rampage sur un emplacement debloque, essai en espace libre puis face a un obstacle en vue native et TPS ; verifier deplacement/degats, relachement, reprise et deuxieme lancement. Revenir a Native si anomalie. Ne pas confondre READY avec CAST dans le journal.

---

## Retour utilisateur V1.103 - reception lourde validee, priorite Rampage

25 octobre 2026 : l'utilisateur confirme « 1. oui bravo ca marche / 2. oui » pour les deux points d'essai de V1.103. **Reception lourde VALIDEE dans ses essais rapportes**, sans inventer de mesures ou de validation P2. Conserver ce chemin, Stoneskin/police V1.102 et les transferts deja acceptes. Priorite explicite : poursuivre **Rampage sur Strife**, pas un autre sujet. Aucun nouveau build pour cette consignation.

---

## V1.103 - reception lourde par interruption native du mouvement

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40199153318), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/74cb774b480f1cfb5cd2ed6e8d8a15ccd0fd39ec/downloads/DarksidersGenesis_V1.103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.zip) - [Recu](V1_103_DELIVERY_RECEIPT.md). Nouvelle reception lourde EXPERIMENTALE, NON VALIDEE EN JEU ; base V1.102 validee pour Stoneskin et police.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; essai en jeu requis.** Base V1.102 : Stoneskin et taille de police explicitement VALIDES par l'utilisateur ; Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. Rampage Strife V1.100/V1.101 reste rejete et retire.

La reception lourde apres une chute utilise maintenant sa vraie fenetre native d'interruption MOVE, ouverte apres l'impact (0,15 s et sol continu 50 ms). Le moteur decide de la fin du pouvoir, du blend de l'animation et de la reprise lorsque le joueur se deplace. L'ancien correctif qui ecrivait AllowPlayerMovement et le bit d'action MOVE est retire. Pas de fin forcee, simulation de touche, changement des dashs ou ecriture de vitesse. La case existante reste OFF par defaut ; les raccourcis reaffectes suivent l'action native.

169 cles INI, 55 ID Hotkeys et anciens reglages inchanges ; 167 anciennes archives preservees ; debug uniquement. Installation manuelle jeu ferme en conservant son INI/raccourcis/PAK/sauvegardes. [Contrat, preuves, limites et essais](V1_103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.md). Le transfert Rampage demande encore un releve du refus exact ; aucun candidat aveugle reactive. P2 reste a tester plus tard.

Essai groupe : Strife puis War, meme chute en vue native et TPS, case OFF puis ON ; maintenir le deplacement a la reception et comparer la reprise, refaire sans bouger (animation/impact natifs), puis verifier dash et attaques. Remettre OFF si le rendu ou les commandes regressent.

---

## Retour utilisateur V1.102 - Stoneskin et police valides

25 octobre 2026 : « stoneskin valide / taille police ok ». **Stoneskin et taille de police VALIDES par l'utilisateur dans ses essais.** Conserver ces chemins et leurs limites documentees ; aucune nouvelle mesure de degats, variante ou validation P2 inventee. World Ender War V1.100 et Geyser/Clone/Caltrops V1.98 restent retenus. Base de reprise : V1.102 avec ces acquis. Rampage Strife V1.100/V1.101 reste rejete et retire ; reception lourde toujours ouverte. Le refus d'icone indisponible n'a pas de validation visuelle distincte rapportee. Pas de nouveau build pour cette consignation.

---

## V1.102 - Stoneskin pour les deux personnages et police du menu

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40194346246), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/00581c2daeb87a192b99d2bfbac75ea1b36557c8/downloads/DarksidersGenesis_V1.102_STONESKIN_MENU_FONT_TEST.zip) - [Recu](V1_102_DELIVERY_RECEIPT.md). Stoneskin Strife et police EXPERIMENTAUX, NON VALIDES EN JEU ; acquis V1.98 et World Ender War V1.100 valides conserves.**

**EXPERIMENTALE ; compilation et livraison controlees ; test utilisateur requis.** Base retenue : acquis V1.98 et World Ender sur War V1.100, explicitement confirme par l'utilisateur. Geyser/Clone/Caltrops toujours valides. Rampage sur Strife V1.100/V1.101 REJETE : icone de capacite disparue et comportement anormal. Ce chemin, ses montages, sa reflexion et ses tests specifiques sont retires de l'ASI ; une ancienne selection Strife=5 revient a Native. Rampage propre de War conserve.

Stoneskin est selectionnable dans chacun des trois emplacements des deux personnages : chemin natif sur War ; sur Strife, statut/cout/duree natifs et geste compatible avec son propre squelette. Transfert Strife EXPERIMENTAL et partiel : materiau de pierre War, onde de choc du montage et effets d'arme non copies. Stinger reste natif, sans reattache ou correction visuelle.

About → Menu appearance : taille de police du menu de 10 a 32 px, saisie exacte et Default 13 px, enregistree dans [Overlay] FontSize. Curseur et fenetres du mod gardent leur fonctionnement ; autres overlays et jeu hors du scope de cette taille. INI personnel conserve, cle absente = taille d'origine.

Refus d'un transfert indisponible : le resolveur garde la classe/icone et le debloquage natifs du slot, puis bloque seulement son utilisation ; il ne renvoie plus une classe vide. Une capacite encore verrouillee reste verrouillee. Ce changement traite un defaut confirme du code, sans attribuer une cause runtime non capturee a Rampage.

169 cles INI (une ajout), 55 ID Hotkeys et anciens reglages preserves ; 166 archives precedentes intactes, debug seulement. Installation manuelle jeu ferme, garder INI/raccourcis/PAK/sauvegardes. P2 reporte. [Contrat, preuves, essais et repli](V1_102_STONESKIN_MENU_FONT_TEST.md).

Test groupe : police et reouverture du menu ; Stoneskin Strife/War (cout, protection et expiration, fin du cast/retour Native) ; controle de l'icone apres ancienne selection Rampage et bref controle des quatre transferts valides. Aucun nouveau resultat en jeu annonce.

---

## V1.101 - lot Rampage Strife / rayon War, controle des joueurs explicite

**CONSTRUITE / CONTROLEE : [58/58 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40191183951), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/417e6047dd3857850e6f55dd82cbdb83c0470a20/downloads/DarksidersGenesis_V1.101_STRIFE_RAMPAGE_WAR_BEAM_TEST.zip) - [Recu](V1_101_DELIVERY_RECEIPT.md). Corrections War et Rampage Strife EXPERIMENTALES, NON VALIDEES EN JEU ; base V1.98 validee.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; validation en jeu requise.** Base validee V1.98. Cumul des candidats V1.99/V1.100 : direction et pose de World Ender War, mouvement natif et fins de Rampage Strife. V1.100 a passe 58/58 suites mais l'analyse MSVC signale C6386 sur l'index du tableau d'etats ; son ZIP reste une archive intermediaire.

V1.101 remplace tous les acces variables a ce tableau par un resolveur borne explicite P1/P2, avec refus des indices invalides et tests -1/2. Aucune suppression d'avertissement. Les deux candidats de gameplay restent identiques : rayon dans le repere propre de War, geste unique puis pose maintenue ; Rampage en mouvement natif sur une piste non evaluee par le corps de Strife. Pas d'animation Rampage dediee annoncee.

Geyser/Clone/Caltrops valides, Stinger natif, cameras, tirs, manettes, roues et autres acquis conserves. 168 cles INI et 55 ID Hotkeys inchanges ; 165 archives precedentes a conserver, debug uniquement. Installation manuelle jeu ferme en gardant INI/raccourcis/PAK/sauvegardes. P2 differe. [Contrat, essais et repli](V1_101_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

Essai groupe : War World Ender dans les deux vues (direction, pose, fin et relance) ; Strife Rampage en espace libre puis face a un obstacle (mouvement, degats, fin/reprise et relance) ; retour Native et controle rapide des trois transferts deja valides. Ces comportements restent a approuver en jeu.

---

## V1.100 - Rampage Strife et correction du rayon War

**EXPERIMENTALE, NON VALIDEE EN JEU. Compilation Windows et livraison a verifier.** Base validee V1.98 ; cumul V1.99 avec retour utilisateur partiel : World Ender fonctionne sur War, mais tire a 90 degres a droite dans les deux vues et repete son geste. Ces deux defauts restent ouverts jusqu'au prochain essai.

Ce lot remplace la source artistique VFX_Aura par le repere de tir du composant prive, aligne sur la capsule de War. Son geste se joue une fois, maintient sa pose pendant le rayon, puis revient a son idle ; duree, callbacks, cout et annulation natifs conserves. Aucun changement de camera ni correctif general des tirs.

Rampage sur Strife revient par un chemin distinct de V1.96 : mouvements portes par les animations, boucle, collisions et phases de fin natifs dans un montage prive. La piste utilise un emplacement absent du graphe de Strife, verifie a chaque lancement : son corps garde ses propres poses de locomotion, sans recevoir les os de War. **Pas d'animation Rampage dediee a Strife annoncee ; mouvement, degats et sortie restent a tester.** Contrats refuses = abandon natif du pouvoir et transfert indisponible pour la session, sans timer de secours.

Geyser/Clone/Caltrops valides, Stoneskin/Stinger natifs, controles, tirs et cameras conserves. INI de 168 cles et 55 ID Hotkeys inchanges ; 164 anciennes archives a conserver, debug seulement. Installation manuelle jeu ferme, garder son INI, ses raccourcis, PAK et sauvegardes. P2 differe. [Preuves, essais et rollback](V1_100_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

Test groupe : World Ender sur War dans les deux vues, orientation, pose maintenue et sortie ; Rampage sur Strife en espace libre puis face a un obstacle, fin et relance ; retour Native et comparaison des pouvoirs propres. Aucun resultat CI ne remplace ces essais en jeu.

---

## V1.99 - World Ender sur War, composant natif propre

**CONSTRUITE / CONTROLEE : [56/56 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40174981083), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/030d79dc867acc19431c3b710b5b9eb62ce60818/downloads/DarksidersGenesis_V1.99_WAR_WORLD_ENDER_NATIVE_COMPONENT_TEST.zip) - [Recu](V1_99_DELIVERY_RECEIPT.md). World Ender War EXPERIMENTAL, NON VALIDE EN JEU ; base V1.98 validee.**

**Base V1.98 VALIDEE par l'utilisateur le 25 octobre 2026** : gestes Geyser sur Strife, Clone/Caltrops sur War approuves ; Stinger natif deja accepte et cloture. Ce nouveau candidat conserve ces acquis. **V1.99 EXPERIMENTALE, NON TESTEE EN JEU ; compilation et livraison controlees, voir le recu ci-dessus.**

L'ancien transfert World Ender cherchait une propriete `Chaoseater` absente de War. Le nouveau chemin utilise son vrai modele `CharacterMesh0`, le point natif `VFX_Aura` et un composant de rayon prive lui appartenant, enregistre dans son monde et attache a sa capsule par le moteur. Montage prive avec geste War, deux callbacks StartBeam/DestroyBeam uniques aux temps natifs et etats d'annulation conserves. Pas de nouveau hook, timer, touche imposee ni modification d'un asset partage. [Contrat, preuves, essais et rollback](V1_99_WAR_WORLD_ENDER_NATIVE_COMPONENT_TEST.md).

Rampage sur Strife reste indisponible : sa boucle utilise le mouvement porte par les animations de War, absent des clips Strife et de la pose idle rejetee. Aucune reactivation de cet ancien adaptateur. Pouvoirs propres natifs, tirs/cameras/controleurs/roues et acquis conserves. INI de 168 cles, 55 ID Hotkeys et valeurs par defaut inchanges ; 163 anciennes archives intactes, debug uniquement. Installation manuelle en conservant son INI, ses raccourcis, son PAK et ses sauvegardes. P2 reporte.

Test groupe : War a pied en mission, World Ender dans un emplacement, verifier rayon et degats en vue native puis TPS, fin du pouvoir, reprise du mouvement et second lancement ; remettre Native et comparer Strife. Verifier rapidement les trois transferts valides. Le rendu, l'orientation, les sons et les variantes du rayon sur War restent a approuver en jeu.

---

## V1.98 - Gestes propres au receveur et cristal Stinger identifie

### Retour utilisateur du 25 octobre 2026 - Stinger cloture

Sur V1.98, l'utilisateur confirme que le cristal du coeur Stinger est dans le dos, bien attache au personnage, et accepte ce rendu comme le comportement vanilla retrouve. **Apparence Stinger VALIDEE par l'utilisateur ; demande de deplacement/orientation vers l'avant CLOTUREE.** Conserver cet effet natif, sans nouvelle adaptation, reattache ou decalage. Le releve de rotations est arrete. Cette validation concerne uniquement Stinger/Stoneskin, pas les gestes Geyser/Clone/Caltrops V1.98, qui restent experimentaux et a tester. Aucun code ASI, INI, raccourci, PAK ou fichier du jeu modifie ; aucun nouveau build.

**CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40135148178), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9f21be738b703e1cc6f40c5422531e4130b57486/downloads/DarksidersGenesis_V1.98_RECEIVER_CASTING_GESTURES_TEST.zip) - [Recu](V1_98_DELIVERY_RECEIPT.md). Rendu des gestes EXPERIMENTAL, non valide en jeu.**

Lot EXPERIMENTAL, validation en jeu encore requise. Base cumulative V1.97 ; acquis utilisateur : effets/degats Geyser et Caltrops V1.95, effet Clone V1.96. V1.97 reste un repli construit/controle, pas une version declaree validee par l'utilisateur.

Geyser sur Strife utilise un geste de canalisation Strife ; Clone/Caltrops sur War un geste War. Squelettes propres, fichiers d'animation natifs inchanges, sons/pas seuls dans ces gestes. Durees des casts, callbacks et effets retenus conserves ; retour a la pose precedente si le geste charge est indisponible. Aucun nouveau hook, chargement force ou timer. Rampage sur Strife et World Ender sur War restent retires ; pouvoirs propres natifs conserves.

Cristal Stoneskin : capture en lecture seule identifie P_Stinger_CreatureCore_StoneSkin du coeur Stinger, attache a CollisionCylinder, 97 unites au-dessus de la racine du modele. L'utilisateur confirme sa disparition en retirant ce coeur ; les effets Stoneskin ordinaires restent presents. Source visuelle CONFIRMEE ; aucune preuve de deplacement cause par le mod, aucun correctif global d'effets ajoute. [Preuves, objectif, essais et rollback](V1_98_RECEIVER_CASTING_GESTURES_TEST.md).

Installation manuelle jeu ferme, conserver son INI/raccourcis/PAK/sauvegardes. 168 cles et 55 ID Hotkeys inchanges ; 162 archives anterieures a conserver ; debug seulement, P2 differe. Le recu de livraison donne les resultats Windows/ZIP lorsqu'ils sont disponibles. Les entrees suivantes sont historiques.

---

## V1.97 - Repli apres rejet de V1.96

V1.96 REJETEE en jeu : Rampage sur Strife bloque le personnage ; World Ender sur War echoue ; Clone fonctionne sans animation correcte. Caltrops/Geyser : effets et degats confirmes, animations toujours incorrectes. **V1.97 CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40123319154), zero avertissement/erreur MSVC ; repli NON VALIDE EN JEU.** [ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/81b4e41b4d0b9a2d74eee45ba6f63bcbe986c217/downloads/DarksidersGenesis_V1.97_FOREIGN_POWERS_SAFE_SUBSET_TEST.zip) - [Recu](V1_97_DELIVERY_RECEIPT.md). Base : V1.95 et uniquement les effets retenus de Clone V1.96.

Le code des deux transferts rejetes est retire, pas seulement masque dans le menu. Les anciennes selections Rampage sur Strife et World Ender sur War reviennent a Native. Leurs pouvoirs d'origine restent disponibles sur leur propre personnage. Clone/Caltrops sur War et Geyser sur Strife conserves. Aucun changement des tirs, cameras, controles, effets valides ou defauts INI.

Stoneskin reste natif sur War ; aucune adaptation de ce pouvoir. Cristal toujours signale, uniquement pendant Stoneskin : NON CORRIGE. Le PAK d'origine contient un effet de pointes de pierre conditionne par un coeur ; piste seulement, aucun composant observe en jeu et aucune attribution de cause. Animations : les poses idle ne sont pas une solution finale ; il faut reprendre le transfert d'animation et son lifecycle avant de reintroduire les pouvoirs rejetes.

Installation manuelle avec INI/raccourcis/PAK/sauvegardes conserves. 168 cles, 55 ID Hotkeys, 161 anciennes archives ; debug uniquement, P2 reporte. [Etat, preuves et essais](V1_97_RETAINED_FOREIGN_SUBSET_TEST.md). Les entrees suivantes sont historiques.

---

## V1.96 - lot des capacites et retrait de notre Stoneskin

24 octobre 2026. **CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40119582099), zero avertissement/erreur MSVC. Nouveaux transferts EXPERIMENTAUX, non valides en jeu.** [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/fbb17b46f293e9c64fb59ebfa7ae5d4cb92bc923/downloads/DarksidersGenesis_V1.96_FOREIGN_POWER_BATCH_TEST.zip) - [Recu](V1_96_DELIVERY_RECEIPT.md). Base cumulative V1.95. Caltrops sur War et Geyser sur Strife : effets et degats confirmes par l'utilisateur ; animations incorrectes. Ces chemins sont conserves.

Le meme lot ajoute Clone et World Ender sur War, Rampage et ses quatre suites sur Strife. Composants propres au lanceur et montages prives compatibles. Poses idle : animations, mouvement de Rampage et nouveaux effets/degats non valides en jeu.

Notre adaptation de Stoneskin et son choix explicite sont supprimes. Ancien choix 6 = Native ; Stoneskin original de War conserve via Native. Comparer le cristal apres redemarrage complet, sa cause reste inconnue. Trace Geyser temporaire retiree. Aucun nouveau hook ou timer de gameplay.

Installation manuelle avec INI, raccourcis, PAK et sauvegardes conserves. 168 cles, 55 ID Hotkeys et 160 anciennes archives preserves. Debug exclusivement ; P2 reporte. [Contrat et essais](V1_96_FOREIGN_POWER_BATCH_TEST.md). Les entrees suivantes sont historiques.

---

## V1.95 — Geyser sur Strife, candidat final et identité du ZIP centralisée

24 octobre 2026. **CONSTRUITE / CONTRÔLÉE : [54/54 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40114886605), zéro erreur et zéro avertissement MSVC. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/027dae183a714754e66263e4f0f3bc0622498c42/downloads/DarksidersGenesis_V1.95_STRIFE_GEYSER_PRESENTATION_TEST.zip) · [Reçu](V1_95_DELIVERY_RECEIPT.md). Base validée V1.87 dans les cas rapportés ; cumul V1.93 et correction de présentation V1.94. Celle-ci a passé 54/54 suites sans avertissement MSVC, mais son archive publiée porte encore le suffixe Caltrops : elle reste immuable et intermédiaire. V1.95 conserve exactement son code de jeu et fait lire le nom du ZIP depuis BuildInfo, avec vérification de sa version avant compilation.

Geyser sur Strife : son de lancement, lames/nova et variantes à leurs temps natifs ; un seul SpawnBlast. Racine propre de Strife dans des notifications privées, sans effets de son épée ni voix de War. **Pose Strife_Idle encore utilisée, dégâts et rendu non validés en jeu.** [Contrat et test ciblé](V1_95_STRIFE_GEYSER_PRESENTATION_TEST.md). Pas de nouveau hook/minuterie, ni modification des autres capacités ou des acquis.

Installation manuelle, garder son INI/raccourcis/PAK/sauvegardes. 168 clés, 55 ID Hotkeys et 159 anciennes archives intacts ; main/archive/v1.43 préservés, P2 reporté. Les sections suivantes sont historiques.

---

## V1.94 — présentation native de Geyser sur Strife

24 octobre 2026. **V1.94 INTERMÉDIAIRE CONSERVÉE : 54/54 suites et zéro avertissement MSVC, mais suffixe d’archive resté sur Caltrops. Remplacée par V1.95 avec le même code de jeu et un nom de ZIP centralisé. Aucun résultat en jeu supposé.** Base validée V1.87 dans les cas rapportés ; cumul V1.93 et diagnostic réel du 24 octobre. Le dernier essai Strife reste rejeté : pose figée sans effet. Le journal prouve cependant l’appel de SpawnBlast et un objet d’attaque appartenant au bon personnage ; les dégâts ne sont pas mesurés.

Correction ciblée : le montage privé conserve maintenant le son de lancement, les lames/nova et leurs variantes avec leurs temps natifs, ainsi qu’un seul SpawnBlast. Les points Bone_War_Root deviennent la racine de Strife dans des notifications privées ; voix, balayages et effets de son épée ne sont pas transférés. Le jeu conserve allocation, notifications et fin du montage. **La pose reste Strife_Idle : aucune nouvelle animation de lancement annoncée, ni dégâts prétendus réparés.** [Contrat, limites, test et rollback](V1_94_STRIFE_GEYSER_PRESENTATION_TEST.md).

Installation manuelle avec INI/raccourcis/PAK/sauvegardes personnels conservés. Caltrops sur War V1.93 reste expérimental ; Stoneskin, capacités propres, caméras/tirs, XInput/roues et autres acquis inchangés. P2 reporté. 168 clés, 55 ID Hotkeys et 158 anciennes archives préservés ; debug exclusivement. Les sections suivantes sont historiques.

---

## Diagnostic Geyser V1.93 — 24 octobre 2026

Essai comparatif reçu : **Strife reste figé sans effet ; transfert non validé**. Le journal prouve pourtant l’appel réel de SpawnBlast et des événements d’un objet d’attaque appartenant au bon personnage, avec War puis Strife. Le code et une nouvelle extraction vérifiée du PAK montrent que le montage privé omet les sons et effets natifs et utilise une pose de repos. **Défaut de présentation identifié ; dégâts non mesurés, aucun nouveau correctif/build annoncé.** [Résultat, limites et correction ciblée](GEYSER_COMPARISON_2026_10_24.md). Les sections suivantes sont historiques ; V1.93 reste le dernier ZIP.

---

## V1.93 — Caltrops sur War au sol, candidat final hors jeu

24 octobre 2026. **V1.93 CONSTRUITE / CONTRÔLÉE : [54/54 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40058379301), zéro erreur et zéro avertissement MSVC. Caltrops sur War EXPÉRIMENTAL, NON TESTÉ EN JEU.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/85f4d9a893ddfe85e6aef2799e339b2b7d567102/downloads/DarksidersGenesis_V1.93_WAR_CALTROPS_GROUND_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_93_DELIVERY_RECEIPT.md). Base validée V1.87 dans les cas rapportés ; cumul V1.91 et prototype V1.92, dont le code corrigé est `dac4d9d`. Poursuite autorisée sans pouvoir lancer le jeu : **aucune validation fictive**. Geyser reste NON CORRIGÉ et son diagnostic V1.91 inchangé ; P2 reporté.

**Caltrops sur War au sol**, dans chacun des trois emplacements existants : pose War compatible, spawner natif demandé au démarrage depuis son bassin et avec son propre propriétaire. Pas de nouveau hook/minuterie de gameplay, bouton physique, composant partagé ou CDO modifié. Réentrance limitée à l’instance exacte ; arrêt du seul transfert si le contrat de cast échoue. Le lancer aérien et l’animation Strife ne sont pas transférés. Effet, dégâts, coût en mission, fin/annulation et cœurs **non testés en jeu**. [Contrat et essais différés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_93_WAR_CALTROPS_GROUND_TEST.md).

V1.92 a été archivée depuis 6161055 pendant son remplacement ; ses 54 suites passaient, mais le ZIP n’inclut pas la correction dac4d9d. Il reste immuable et n’est pas le candidat final. V1.93 ajoute aussi une garde CI : refuser la publication si les sources compilées ont changé sur debug. 168 clés et défauts, 55 ID Hotkeys et 157 anciennes archives conservés ; main/archive/v1.43 intacts. Installation manuelle, aucun jeu lancé ni fichier installé ; tests groupés plus tard.

Clone/World Ender vers War : composants de réception et nettoyage à couvrir ; Rampage vers Strife : quatre fins Land/Slam à adapter. Ils restent indisponibles et en étude, sans fonctionnalité prétendue. Les sections suivantes sont historiques.

---

## V1.92 — prototype Caltrops vers War au sol

24 octobre 2026. **V1.92 INTERMÉDIAIRE ARCHIVÉE, remplacée par V1.93 : 54 suites passées sur les sources 6161055, mais ce ZIP précède la correction dac4d9d de réentrance par instance. Son run a été annulé trop tard, après publication. Aucun résultat de jeu supposé ; archive conservée sans modification.** Base expressément validée V1.87 pour les cas rapportés, acquis antérieurs conservés ; base cumulative V1.91 `d974c18`, dont le diagnostic Geyser **n’a pas été testé**. L’utilisateur autorise la poursuite des autres sujets sans pouvoir lancer le jeu : cet accord ne constitue pas une validation fictive. Geyser sur Strife reste NON CORRIGÉ ; P2 reste reporté.

Gameplay → Wrath ability slots : **Caltrops sur War**, indépendamment dans ses trois emplacements. Prototype **au sol seulement** : pose War_Idle compatible pendant la durée native du cast, création demandée au démarrage par la fonction native Caltrops depuis **Bone_War_Pelvis**, avec le propriétaire War courant. Aucun nouveau hook, minuterie de gameplay, composant partagé, modification de CDO ou commande physique ajoutée. Coût et déblocage de l’emplacement restent gérés par le résolveur natif. Cette variante n’est pas le lancer animé de Strife ; effet/dégâts/sons, cœurs, fin et annulation restent **à vérifier en jeu**. [Contrat, limites et essais différés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_92_WAR_CALTROPS_GROUND_TEST.md).

Installation manuelle, garder son INI/raccourcis/PAK/sauvegardes. 168 clés et défauts, 55 ID Hotkeys et 156 anciennes archives préservés ; main/archive/v1.43 intacts. Code des acquis, cast Stoneskin, montage et trace Geyser V1.91 conservés. Clone et World Ender vers War exigent encore leurs propres composants de réception ; Rampage vers Strife exige sa chaîne de déplacement/fin. Ces trois transferts restent indisponibles, sans case inerte annoncée comme fonctionnelle. Essais groupés plus tard ; aucune relance ni capture demandée maintenant. Les sections suivantes sont historiques.

---

## V1.91 — diagnostic Geyser avec identité native War correcte

23 octobre 2026. **V1.91 CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40016586657), zéro erreur et zéro avertissement MSVC. DIAGNOSTIC seulement ; Geyser sur Strife NON CORRIGÉ, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0be129010adbb857e0d3de6bbcdd32b3d7022c1b/downloads/DarksidersGenesis_V1.91_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_91_DELIVERY_RECEIPT.md). Base expressément validée V1.87 (réticule, bras War et Léviathan, avec acquis antérieurs conservés). Base cumulative V1.90 archivée `cc3932a`. Geyser V1.89 sur Strife **REJETÉ** par l’utilisateur ; retour Native et pouvoirs propres fonctionnels. P2 reporté.

V1.90 a passé 53 suites sans avertissement MSVC, mais le second audit a relevé un type War erroné dans le filtre **et dans son test synthétique**. Archive conservée, intermédiaire non retenue. V1.91 utilise War=2 comme le chemin natif WrathSlotFeature, rejette l’ancien type 0 et impose aussi le thread de jeu pour découvrir la fonction. **Aucun changement du pouvoir Geyser ni promesse de réparation.** La trace est limitée à huit appels et quatre événements imbriqués par appel ; les quatre arguments et l’appel natif restent transmis une seule fois, sans nouvel hook/minuterie. [Contrat et essai](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_91_GEYSER_DISPATCH_DIAGNOSTIC_TEST.md).

Installation manuelle, conserver son INI/raccourcis/PAK/sauvegardes. 168 clés et défauts, 55 ID Hotkeys, 155 anciennes archives et branches main/archive/v1.43 préservés. Test unique : préparer un emplacement Strife sur Geyser, une activation War Native puis Strife, à pied et hors menus ; revenir à Native et conserver le journal. Objectif : confirmer l’entrée dans la fonction et l’existence éventuelle de l’attaque, pas annoncer des dégâts réparés. Pas d’essai P2 demandé. Ensuite : corriger le maillon démontré et retirer la trace temporaire ; les quatre autres transferts restent ouverts. Les sections suivantes sont historiques.

---

## V1.90 — diagnostic ciblé Geyser, échec V1.89 confirmé

23 octobre 2026. **V1.90 INTERMÉDIAIRE NON RETENUE pour le diagnostic comparatif : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40015555280), zéro erreur et zéro avertissement MSVC, mais filtre War erroné (type 0 au lieu du type natif 2) découvert au second audit. Remplacement V1.91 préparé ; Geyser sur Strife NON CORRIGÉ.** [Archive immuable conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/cc3932ad985ba779d47f60b3f2865e01d5970239/downloads/DarksidersGenesis_V1.90_GEYSER_DISPATCH_DIAGNOSTIC_TEST.zip) · [Reçu et limite](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_90_DELIVERY_RECEIPT.md). Base expressément validée : V1.87 pour réticule, bras War et Léviathan, avec les acquis antérieurs conservés. Base cumulative V1.89 `6921fb7`. **Transfert Geyser V1.89 REJETÉ en jeu** : Strife reste en pose de repos, aucune attaque visible ; retour Native et pouvoirs propres confirmés fonctionnels par l’utilisateur. **P2 reporté**. Aucun nouveau correctif de capacité prétendu.

La comparaison native War/Strife montre le montage actif et la notification d’attaque présente dans les deux files d’animation. Elle ne prouve pas l’entrée dans SpawnBlast ni la création de l’attaque. V1.90 ajoute uniquement une trace limitée à huit appels sur le passage d’événements déjà utilisé : entrée/sortie de Geyser et événements imbriqués de ses acteurs d’attaque. Appels natifs et quatre arguments transmis intégralement ; aucun nouvel hook, timer, coût, dégâts ou délai modifié. Une absence d’événement imbriqué ne prouve pas une absence d’acteur. [Contrat et essai](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_90_GEYSER_DISPATCH_DIAGNOSTIC_TEST.md).

Installation **manuelle**, conserver son INI/raccourcis/PAK/sauvegardes. 168 clés et leurs défauts, 55 ID Hotkeys, 154 anciennes archives et branches main/archive/v1.43 préservés. Après installation : régler un emplacement Strife sur Geyser, faire une activation avec War sur Native puis une avec Strife, hors menus ; revenir à Native et conserver le journal. Aucun essai coop demandé. Suite : analyser cette exécution réelle avant toute nouvelle modification de Geyser, puis supprimer la trace temporaire ; Rampage vers Strife et les trois pouvoirs Strife vers War restent ouverts. Les sections suivantes sont historiques.

---

## V1.89 — candidat Geyser finalisé

23 octobre 2026. **V1.89 CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40009032079), zéro erreur et zéro avertissement MSVC. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8507ca9ffb9fbc24a62ea6d13146245ae13c335e/downloads/DarksidersGenesis_V1.89_STRIFE_GEYSER_CLEAN_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_89_DELIVERY_RECEIPT.md). Base expressément validée : V1.87 (réticule, bras War et Léviathan dans les essais rapportés), code des acquis conservé. Base cumulative : V1.88 `4309438`, prototype Geyser encore expérimental, 53 tests passés mais avertissement C6386 sur son marqueur joueur ; archive préservée. Accès au marqueur désormais centralisé, uniquement P1/P2, sans index dynamique ni avertissement masqué. Aucun changement de calcul du pouvoir. [Objectif, limites et tests](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_89_STRIFE_GEYSER_CLEAN_TEST.md).

Gameplay → Wrath ability slots : **Blade Geyser sur Strife**, trois emplacements indépendants, attaque déclenchée par sa notification native. Pose de repos compatible ; **enchaînement rising slash de War non transféré**. Coût/déblocage/variantes et fin natifs, aucun nouvel hook ni minuterie. Tester apparition, dégâts, coût, fin, retour Native puis Geyser propre de War ; P2 séparément. Protection Stoneskin conservée, apparence native à identifier d’après la précision utilisateur. Rampage → Strife et les trois pouvoirs Strife → War restent ouverts. Installation manuelle, INI/PAK/sauvegardes personnels conservés ; 168 clés, 55 ID Hotkeys et 153 archives antérieures intacts. Autres FX P2 reportés, réception lourde incertaine, HUD général non reproduit.

---

## V1.88 — Geyser natif sur Strife

23 octobre 2026. **V1.88 intermédiaire CONSTRUITE / CONTRÔLÉE : [53/53 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40008159794), zéro erreur, un avertissement d’analyse MSVC C6386 restant sur le nettoyage du marqueur compagnon. Archive conservée, remplacée à la livraison par V1.89 après correction de cet accès. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [Archive immuable vérifiée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4309438f8a87d95ef33ab14fe0a4bdc0f4dda245/downloads/DarksidersGenesis_V1.88_STRIFE_GEYSER_TRANSFER_TEST.zip). Base cumulative V1.87 `c20319f`. **Correctif du réticule V1.87 maintenant VALIDÉ par l’utilisateur dans son essai**, comme le bras War et Léviathan ; code de ces acquis inchangé. Branche **debug** seulement ; installation manuelle, conserver son INI/raccourcis/PAK/sauvegardes. Aucun résultat coop supplémentaire supposé.

Gameplay → Wrath ability slots : ajout expérimental **Blade Geyser sur Strife**, indépendamment dans chacun des trois emplacements. Déclenchement `SpawnBlast` natif à environ 0,488 s, progression/coût/variantes et fin gérés par le jeu ; copie temporaire du montage natif avec une pose de repos compatible. Pas de minuterie ni nouvel hook. **L’enchaînement de mêlée rising slash propre à War n’est pas transféré.** Emplacement inactif si ses assets ne sont pas prêts ; échec du contrat de montage : arrêt natif du cast puis blocage de ce seul transfert jusqu’au redémarrage. [Objectif, limites, preuves, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_88_STRIFE_GEYSER_TRANSFER_TEST.md).

Protection Stoneskin V1.83 conservée. Précision utilisateur : il n’est plus certain qu’un cristal était présent avant ; **aspect natif à identifier**, sans régression visuelle affirmée ni correctif visuel inventé. Rampage vers Strife et Clone/World Ender/Caltrops vers War restent ouverts. 168 clés INI et leurs défauts, 55 ID Hotkeys, commandes souris/manette, caméras, tirs, roues, cheval, effets validés et 152 archives antérieures préservés. Réception lourde incertaine, autres FX P2 reportés, HUD général non reproduit. Les sections suivantes sont historiques.

---

## V1.87 — correction du crash de curseur V1.86

23 octobre 2026. **V1.87 CONSTRUITE / CONTRÔLÉE : [52/52 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39998309541), zéro erreur et zéro avertissement MSVC. Défaut de retour curseur corrigé et contrôlé ; reproduction en jeu encore à confirmer.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/64b612f3341a1abebf3dc2961de1580a73904089/downloads/DarksidersGenesis_V1.87_CURSOR_ENUM_ABI_FIX_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_87_DELIVERY_RECEIPT.md). Base cumulative V1.86 `7763e99`. **Interception du réticule V1.86 REJETÉE pour crash**, remplacée par le contrat de retour entier 32 bits natif. Retour utilisateur du 23 octobre : **bras de War et correction Léviathan VALIDÉS dans les essais rapportés**, code conservé. Le correctif du réticule V1.87 reste à valider en jeu ; aucune validation de la version entière ni de la coop déduite. Acquis validés V1.85 (effets des lames), V1.82 (suivi d'exécution) et V1.81 (bras OFF) conservés, branche debug seulement, installation manuelle et INI/raccourcis personnels gardés.

Le minidump montre un index de curseur `0xEDFB8D08` au lieu de `8`. Cet index, sa conversion signée et le calcul du tableau correspondent exactement à l'adresse de lecture invalide. Le champ natif est un octet, **la fonction renvoie un enum entier 32 bits dans EAX** ; V1.86 déclarait à tort le hook et son trampoline avec un retour octet. Les bits restants étaient indéfinis, y compris en renvoyant le résultat natif dans un menu. Contrat corrigé sans nouvel hook ni modification d'entrée. [Preuves, limites, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_87_CURSOR_ENUM_ABI_FIX_TEST.md).

Les anciens tests consommaient eux aussi un octet et ne couvraient pas ce contrat. Désormais, toutes les politiques de curseur passent par un consommateur 32 bits compilé dans un fichier distinct, avec contrôle de type, enums natifs et absence de troncature. INI identique (168 clés), 55 ID Hotkeys et 151 anciennes archives conservés. Tester les quatre modes, puis les pages/roues et leurs transitions ; le reste du lot V1.86 est inclus. Cristal Stoneskin et cinq autres transferts restent ouverts. Les sections suivantes sont historiques.

---

## V1.86 — bras de War, réticule et régression Léviathan

23 octobre 2026. **V1.86 CONSTRUITE / CONTRÔLÉE : [52/52 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39995845270), zéro erreur et zéro avertissement MSVC. Nouveaux choix visuels et retour Léviathan EXPÉRIMENTAUX, à confirmer en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9c04d2514c82a6dad6961ea05686975cb3dde6d9/downloads/DarksidersGenesis_V1.86_WAR_ARM_RETICLE_LEVIATHAN_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_86_DELIVERY_RECEIPT.md). Base cumulative V1.85 `46aff86`. Les effets des épées masquées V1.85 sont maintenant **VALIDÉS par l'utilisateur dans son essai** ; leur code reste inchangé, comme les bras OFF et le suivi TPS des exécutions déjà validés. Branche **debug** seulement, installation manuelle, conserver son INI et ses raccourcis.

Gameplay → Appearance : **War arm appearance — Native / Basic / Augmented**, aspect seulement sans donner le Gantelet ; les armures qui remplacent les éléments enfants gardent leur politique native. **Mouse reticle — Vanilla / Hide in TPS / Hide in normal view / Hide in both views** : seul le réticule de jeu est masqué, les pages, roues, pause et leurs transitions conservent le curseur natif. Deux clés supplémentaires, Native/Vanilla par défaut ; les 166 anciennes valeurs et 55 ID Hotkeys sont conservés.

Léviathan : le journal et les octets du jeu démontrent que la vérification de signature s'effectuait après l'interception de SpawnEmitterAttached par les effets de lame V1.84/V1.85. Initialisation déplacée avant cette interception, garde conservée et effets de mêlée inchangés. Retour des éclairs à confirmer en jeu avec les lames masquées. [Objectif, preuves, limites, essais et rollback V1.86](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_86_WAR_ARM_RETICLE_LEVIATHAN_TEST.md).

Stoneskin : protection sur Strife jugée plausible par l'utilisateur, sans mesure complète ; cristal incorrect sur **Strife et War**, dans les deux vues et même avec les emplacements War Native. **Défaut ouvert**, aucune correction visuelle prétendue. Le transfert partiel V1.83 est inchangé. Geyser/Rampage vers Strife et Clone/World Ender/Caltrops vers War restent à adapter ; les cinq transferts ne sont pas livrés. Autres FX P2 reportés, réception lourde incertaine, HUD général non reproduit. Les sections suivantes sont historiques.

---

## V1.85 — lot cumulatif effets des épées et prototype Stoneskin

22 octobre 2026. **V1.85 CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39845333890), zéro erreur et zéro avertissement MSVC. Nouveaux effets et protection Stoneskin EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6f05855e2c3ba54b904d78116bc9562895ff997a/downloads/DarksidersGenesis_V1.85_COMPACT_BARE_BLADE_FX_TEST.zip) · [Reçu et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_85_DELIVERY_RECEIPT.md). Installation manuelle, garder son INI et ses raccourcis. Base cumulative V1.84 `6e27cf7`. Acquis V1.81 et suivi TPS des exécutions V1.82 validés dans les essais rapportés, conservés sans modification. Branche debug seulement ; installation manuelle, conserver son INI et ses raccourcis.

Correction des effets de mêlée attachés de V1.84 conservée : arcs compacts depuis les mains des côtés masqués, sans changer dégâts/portée. V1.84 avait passé 51 tests avec un avertissement C4324 de padding ; la structure privée est réordonnée, 128 octets contrôlés à la compilation, sans masquer l'avertissement ni changer les calculs. [Contrats, limites, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_85_COMPACT_BARE_BLADE_FX_TEST.md).

Prototype V1.83 inclus : protection Stoneskin vers Strife, encore partielle et non validée en jeu. Onde de choc de lancement, sons et apparence de pierre non transférés ; mélange complet des six capacités toujours ouvert. 166 réglages INI, 55 ID Hotkeys et 149 anciennes archives conservés. Tester les effets des lames masquées puis, séparément, la protection et le retour Native. Les sections suivantes sont historiques.

---

## V1.84 — correction du chemin des effets de mêlée attachés

22 octobre 2026. **V1.84 intermédiaire CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39844030589), zéro erreur, un avertissement MSVC C4324 de padding de Plan. Nouvelle compilation V1.85 prévue après correction de disposition, sans masquer l'avertissement. Effets EXPÉRIMENTAUX, non validés en jeu.** [Archive conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c58304fd20466e173e0960ca09e47b399455aa7a/downloads/DarksidersGenesis_V1.84_ATTACHED_BARE_BLADE_FX_TEST.zip) · [Reçu](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_84_DELIVERY_RECEIPT.md). Installation manuelle. Base cumulative V1.83 `81d61dd`. Acquis validés V1.81 conservés ; le suivi TPS des exécutions V1.82 est maintenant **VALIDÉ par l'utilisateur dans son essai**, code inchangé. Le correctif V1.82 des effets d'épées cachées est inefficace dans son essai et remplacé pour les chemins manquants. Branche debug seulement.

Cause démontrée par lecture seule : les huit notifications ciblées des premiers coups/attaques aériennes utilisent SpawnEmitterAttached sur Bone_Strife_Root ; V1.82 les excluait. Elles passent maintenant par le même contexte de propriétaire et de côté que les effets détachés. Position de la main exprimée dans le repère racine natif, taille ×0,4 ; parent, socket, rotation, destruction et pooling conservés. Variante du troisième coup `_03_b` ajoutée d'après ses notifications chargées exactes. Dégâts, portée, poses, tirs et caméra inchangés. Les arcs communs avec une seule lame visible restent natifs. [Contrats, limites, essais et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_84_ATTACHED_BARE_BLADE_FX_TEST.md).

Le prototype V1.83 de **protection Stoneskin vers Strife** est inclus sans modification ; il reste partiel et non validé en jeu : onde de choc, sons et apparence de pierre de War non transférés. Le mélange complet des six capacités reste ouvert. Installation manuelle, conserver son INI et ses raccourcis ; 148 anciennes archives préservées.

---

## V1.83 — premier transfert de pouvoir : Stoneskin sur Strife

22 octobre 2026. **V1.83 CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39841420585), zéro erreur et zéro avertissement MSVC. Protection Stoneskin sur Strife EXPÉRIMENTALE, partielle et non validée en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/aa9a096a232dca76de9456387a7565b69de8925d/downloads/DarksidersGenesis_V1.83_STRIFE_STONESKIN_TRANSFER_TEST.zip) · [Reçu et limites](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_83_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI. Base cumulative V1.82 `6c916ff`, actuellement en essai utilisateur ; dernière base expressément validée V1.81 dans les cas rapportés. Branche **debug** seulement, installation manuelle.

Chacun des trois emplacements de Strife peut recevoir Stoneskin. L'emplacement d'origine doit être débloqué ; coût, statut de protection, variantes de cœurs et fin restent natifs. Un montage temporaire compatible utilise le repos de Strife pour la durée du cast de War. Aucun déblocage de progression War ni modification des assets/CDO, entrées ou ressources de l'autre joueur. Le reste du mélange des six capacités reste ouvert. [Objectif, contrats, limites, essais et rollback V1.83](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_83_STRIFE_STONESKIN_TRANSFER_TEST.md).

INI et raccourcis inchangés, Native par défaut dans les six choix. Les pouvoirs propres de chaque personnage et tout le lot V1.82 sont conservés. Aucun résultat du nouvel adaptateur en jeu supposé ; les nouvelles fonctions de V1.82 attendent toujours le retour utilisateur. Réception lourde encore incertaine, autres FX P2 reportés, HUD général non reproduit.

**Limite du prototype : seule la protection native et son coût sont transférés.** L'animation compatible de repos ne porte pas les notifications de War : onde de choc de lancement, sons, particules et apparence de pierre ne sont pas encore transférés. Les variantes de cœurs et la protection réelle nécessitent un essai ; ne pas attendre une aura visible du statut de base. Le mélange complet des six capacités reste ouvert.

Retour utilisateur V1.82 du 22 octobre : **suivi TPS des exécutions VALIDÉ dans l'essai rapporté ; effets des épées cachées inefficaces**, dossier rouvert. Lecture seule : huit notifications ciblées des deux premiers coups et des attaques aériennes sont attachées à Bone_Strife_Root, chemin exclu par V1.82 ; les six notifications du troisième coup sont détachées. Le contrat des mains et des assets est confirmé, le passage réel des callbacks reste à vérifier.

---

## V1.82 — effets des lames masquées et suivi vertical des exécutions

22 octobre 2026. Base cumulative V1.81 `adb5ae8`, **VALIDÉE par l'utilisateur pour les trois essais rapportés** (bras OFF animés, mêlée/pouvoirs, transitions). Branche **debug** seulement ; aucun résultat coop supplémentaire supposé. Le code des bras OFF, modèles/holsters, cheval/sons et visée reste conservé.

**V1.82 CONSTRUITE / CONTRÔLÉE : [50/50 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39834733781), zéro erreur et zéro avertissement MSVC. Nouveaux effets et suivi vertical EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/03a45347f8484ca4af9fa17cf5b49d3e66062cf9/downloads/DarksidersGenesis_V1.82_BARE_BLADE_EXECUTION_CAMERA_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_82_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI et ses raccourcis. Les effets de lame identifiables du côté masqué partent de sa main avec une taille de 40 %, sans changer dégâts ou portée. Les effets communs restent natifs avec une seule épée visible. Nouveau suivi TPS de la montée animée du bassin pendant une exécution native, indépendant par joueur et sans modifier la pose servant à la visée. [Objectif, preuves, limites, essais et rollback V1.82](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_82_BARE_BLADE_EXECUTION_CAMERA_TEST.md).

La capture n'a relevé que des exécutions au sol, confirmé par l'utilisateur : elles servent de référence de cadrage et **ne valident pas le suivi en hauteur**. Les six capacités étrangères restent en étude : contrat du montage dynamique pour Stoneskin précisé, aucun adaptateur incomplet livré. Réception lourde encore incertaine ; autres FX P2 différés, HUD général non reproduit. Installation manuelle, conserver son INI personnel et ses raccourcis.

---

## V1.81 — candidat d'animation désarmée pour les bras OFF

22 octobre 2026, base cumulative V1.80 `3533b3d`, branche **debug** seulement. Le cheval de V1.80 est **VALIDÉ par l'utilisateur dans son essai** et reste inchangé. Le bras OFF figé est rejeté : la demande est une vraie animation désarmée, tout en conservant explicitement la mêlée et les pouvoirs.

**V1.81 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39803846206), zéro erreur et zéro avertissement MSVC. Bras animés EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/36d31209abfbf4c0fea6b5afd64184855f1f964b/downloads/DarksidersGenesis_V1.81_NATIVE_UNARMED_ANIMATION_TEST.zip) · [Reçu et empreintes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_81_DELIVERY_RECEIPT.md). Installation manuelle, garder son INI et ses raccourcis. Repos/marche/course natifs échantillonnés dans une pose privée, appliquée au seul bras dont le pistolet est OFF. Ancienne capture immobile supprimée. Aucun nouveau réglage ; INI et raccourcis conservés. [Objectif, preuves, limites, essais et rollback V1.81](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_81_NATIVE_UNARMED_ANIMATION_TEST.md). Installation manuelle. Six capacités étrangères, réception lourde et suivi vertical des exécutions restent ouverts ; autres FX P2 différés, HUD général en surveillance.

---

## V1.80 — saut validé, sons montés et candidat bras OFF

22 octobre 2026, branche **debug**, base cumulative V1.79 `5e2221b`. L'utilisateur confirme le saut à cheval et les points 1 à 5 de ses essais, **avec une exception explicite : les bras sans pistolet restent incorrects des deux côtés**. Les choix d'apparence, holsters, épées et la correction native de World Ender sont retenus dans les essais rapportés ; pas de validation coop supposée.

**V1.80 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39793144590), zéro avertissement MSVC. Sons et nouveau rendu des bras EXPÉRIMENTAUX, à confirmer en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8124f3e683d16dafbabf6bc610c7f2998a2dce4f/downloads/DarksidersGenesis_V1.80_MOUNTED_SOUNDS_PACKET_ARMS_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_80_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI et ses raccourcis.

Lot construit **EXPÉRIMENTAL** : case Jump on horseback déplacée dans **Gameplay → Horse**, son de départ/atterrissage via les SoundCue du jeu, choix Native jump (varied) / Horse snort (varied) / Off ; nouvelle correction de rendu des bras utilisant le même paquet, avec conservation du repos compatible après cheval/ultime. Le saut validé, les fonctions de visée et les raccourcis sont conservés. [Objectif, preuves, limites et essais V1.80](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_80_MOUNTED_SOUNDS_PACKET_ARMS_TEST.md).

Les six capacités entre personnages restent ouvertes : le sélecteur actuel conserve les trois pouvoirs de chaque famille. Aucun pouvoir étranger lancé sur un squelette ou un composant incompatible. [Contrats restant à adapter](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_80_FOREIGN_WRATH_CONTRACTS.md). Installation manuelle, garder l'INI personnel. Les sections suivantes restent historiques.

---

## V1.79 — candidat du lot apparence / World Ender / saut monté

**V1.79 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39683569838), zéro avertissement MSVC. Nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f579ed3f068ff1cba6f25a7a6ba680ba5759517/downloads/DarksidersGenesis_V1.79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_79_DELIVERY_RECEIPT.md). Les 143 anciennes archives et les 163 anciens réglages sont conservés. Installation manuelle, garder son INI.

Base debug `5d4883e` / V1.78. Retours : maintien OFF et interface jugés bons ; bras encore incorrects, côtés des holsters inversés ; coop différée. Nouveautés expérimentales et critères : [V1.79](V1_79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.md).

Deux branches OFF clavicule comprise sur Spine2 ; holsters rendus inversés ; Native/masquage gauche/droite/deux pour les épées exactes de Strife ; exemption de World Ender dans le hook des faisceaux ordinaires, par provenance du projectile, sans changement de caméra ni matériau ; saut monté OFF par défaut par action JUMP et DoJump natifs. Capture VM_READ de 2 378 échantillons sans erreur ; le rayon existe et son PSC est visible/actif. L'utilisateur confirme qu'il est visible caméra reculée et que sa direction indépendante de l'animation est incorrecte. Rendu proche encore à vérifier, ne pas annoncer réparé. Contrôles CI et ZIP dans le reçu après livraison. Aucune installation ni relance par l'agent.

---

## V1.78 — lot OFF / interface / nettoyage construit et contrôlé

21 octobre 2026. **CONSTRUIT / CONTRÔLÉ : [47/47 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39669799213), zéro avertissement MSVC ; nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** Base cumulative V1.77, retours utilisateur intégrés, branche debug seulement. [Détail et essais](V1_78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.md).

Validations rapportées sur V1.77 : visée sans tir, sauvegarde des réglages, updater ; OFF gauche avec seul le pistolet droit visible à pied ; autres pouvoirs de Strife et pouvoirs de War semblent fonctionner dans les essais rapportés. HUD général non reproduit. Heavy landing reste incertain, coop attendra. Ne pas étendre ces validations à toutes les combinaisons de réaffectation.

Construit : persistance visuelle OFF dans les roues, après ultime, et masquage des quatre modèles montés par côté ; bras droit OFF incluant son épaule dans la copie de rendu, branche gauche validée conservée ; holsters à pied masqués par leurs deux os vérifiés. Never ajouté au trait/arc, groupes du menu fermés par défaut. Patch revu des deux audits appliqué : 167 lignes nettes retirées dans 12 fichiers, appels natifs préservés. Nettoyage intégré au build contrôlé. [ZIP immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/08a5a2ace26e80ea8950a31996ca7fed6650a8ad/downloads/DarksidersGenesis_V1.78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.zip) · [Reçu, empreintes et rollback](V1_78_DELIVERY_RECEIPT.md).

World Ender : deux activations enregistrées en lecture seule, 2 932 échantillons sans erreur ; référence du rayon présente durant environ 2,8 s, puis destruction native. Une lecture ponctuelle après sa fin avait été présentée à tort comme absence de création ; conclusion corrigée après bilan complet. Deuxième capture : 2 864 échantillons, aucune erreur ; acteur de rayon, template `P_Strife_worldender_scale1` et `MayhemBeamEffect` présents. L’utilisateur confirme dégâts et visibilité uniquement en vue normale ; Default Only en TPS ne le rétablit pas. Défaut de présentation TPS encore ouvert, aucune correction prétendue. Inspection privée de trois effets / 1 096 exports, matériaux et montage, sans changement d’asset.

Ajout demandé pour plus tard : caméra TPS ne suivant pas assez la hauteur pendant les exécutions, action hors champ. Les choix étrangers parmi les six capacités restent à adapter. Autres FX P2 toujours reportés. Aucune installation ou relance par l’agent ; INI, PAK et sauvegardes conservés. Les sections suivantes sont historiques.

---

## V1.77 — construite et contrôlée, réaffectation native du Courroux

Complément du 21 octobre : [double audit des leftovers](AUDIT_DOUBLE_NETTOYAGE_V1_77.md), deux méthodes confrontées. Proposition de nettoyage **167 lignes nettes / 12 fichiers**, patch séparé non appliqué et non compilé. Les sources actives et le ZIP V1.77 restent inchangés pendant les essais utilisateur. Intégrer au prochain lot cohérent et repasser les contrôles ; l'échange des six capacités et OFF universel restent ouverts.

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE, non validée en jeu.** Poursuite autorisée sans attendre l'essai du lot ; aucune nouvelle validation utilisateur supposée. Base cumulative V1.76, base explicitement validée V1.73 dans les essais rapportés. V1.75 toujours retrouvée installée sur disque ; aucun fichier du jeu remplacé par l'agent. Installation manuelle ultérieure.

Trois choix indépendants pour Strife et trois pour War, pour l'instant **parmi les trois pouvoirs de chaque personnage**. Nouveau panneau Gameplay → Wrath ability slots ; Native partout par défaut. Une seule interception du lookup natif avec déverrouillage et coût, sans service par frame ni cache d'objets. Le choix libre parmi **les six capacités reste ouvert** : adaptation des animations, notifications et composants nécessaire. Les indications de roue peuvent encore décrire l'emplacement initial. Visée sans tir, réception lourde V1.75 et OFF à pied V1.76 conservés, leurs essais en jeu toujours attendus.

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39632822621) : **47/47 suites, zéro avertissement MSVC**. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b5da4721104b0b5a08911bf8cc51af88a4559f6f/downloads/DarksidersGenesis_V1.77_NATIVE_WRATH_SLOTS_TEST.zip), 856818 octets, SHA256 `47507C0A5901B8C2E44DFBC16EFB5964A6523C35759EE2FBF5D0C9625CEE15C1`. Quatre fichiers racine, CRC/AMD64, 163 clés INI dont six nouvelles Native, 157 anciens réglages et 55 ID Hotkeys conservés, 141 anciennes archives inchangées. Quinze modules de caméra, visée, dash, ressources, holsters et OFF inchangés par rapport à V1.76 ; main/archive/v1.43 conservés. [Reçu, preuves et rollback](V1_77_DELIVERY_RECEIPT.md) · [Essais groupés](V1_77_NATIVE_WRATH_SLOTS_TEST.md).

La recherche étrangère progresse séparément : 40/40 fonctions Blueprint des six pouvoirs entièrement décodées, dépendances du clone et du rayon précisées ; aucune capacité étrangère lancée. Suite : adaptation entre personnages, OFF universel encore ouvert. HUD général en surveillance ; autres FX P2 reportés ; notification updater à confirmer. Les sections suivantes sont historiques.

---

## V1.76 — construite et contrôlée, candidat OFF à pied

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE PAR L'AGENT ; nouveauté EXPÉRIMENTALE, non validée en jeu.** Base validée V1.73 dans les essais rapportés, suite cumulative de V1.75 dont les nouveautés attendent le test utilisateur. V1.75 a été retrouvée installée sur disque ; ne pas transformer cela en validation de ses fonctions.

Nouveau choix OFF par main pour Strife normal à pied : modèle masqué, bras au repos dans les poses de tir par copie de rendu, tirs/sockets natifs conservés. Ranger les armes et rester immobile brièvement pour enregistrer la pose. Mêlée, actions inconnues, cheval et ultime gardent leur présentation native. Effets propres à une main filtrés ; effets communs connus conservés avec une arme visible et retirés avec les deux OFF. **OFF universel de tous les effets/formes toujours ouvert**, six capacités encore en adaptation.

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39614279357) : **46/46 suites, zéro avertissement MSVC**. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip), 850967 octets, SHA256 `9FF084D8FBF542EDC0D65D78F06A9DC3D06FC363D2EB5BFEBE72CE93ED6B2A27`. Quatre fichiers racine/CRC/AMD64, 157 clés INI et 55 ID Hotkeys conservés, 140 anciennes archives inchangées. Installation manuelle ultérieure ; aucun fichier du jeu remplacé par l'agent. [Reçu, intégrité, essais et rollback](V1_76_DELIVERY_RECEIPT.md).

Tester plus tard dans le même lot : OFF droit/gauche/deux puis Native, tirs normaux/Hotstreak et dash, mêlée/roues/formes/P2 ; visée sans tir et réception lourde V1.75. [Recherche des capacités](V1_76_WRATH_ADAPTATION_RESEARCH.md). HUD général en surveillance, autres FX P2 reportés, notification de mise à jour à confirmer. Les sections suivantes sont historiques.

---

## V1.75 — construite et contrôlée, installation manuelle

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / NON INSTALLÉE ; nouvelles fonctions EXPÉRIMENTALES, non validées en jeu.** Base V1.73 validée dans les essais rapportés, canal debug. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39537334409) : 45/45 suites, zéro avertissement MSVC. Visée seule manette avec un/deux boutons et nouveau candidat de déplacement après réception lourde automatique ; les deux sont OFF par défaut. Aucun ancien Recovery du dash réactivé.

[ZIP cumulatif public vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a816cc3db3c446fac407b93dc881058c19e7e43d/downloads/DarksidersGenesis_V1.75_PAD_AIM_HEAVY_LANDING_TEST.zip) : 836203 octets, SHA256 `B6D1F0CE0D3ACE5B5CF27A3C75373778707CC84EA9FA41764B4036B5ABB6EE08`. Quatre fichiers racine/CRC/AMD64 ; 156 valeurs INI antérieures, 55 ID Hotkeys et 139 anciennes archives conservés. main/archive/v1.43 inchangés. Installation manuelle demandée ; V1.73 reste installée, aucun fichier du jeu remplacé.

À tester ensemble : maintien/relâchement de visée native/TPS, personnages et P2 ; réception lourde OFF/ON après haute chute (Strife puis War) ; contrôle court dash/tirs spéciaux/roues/rangement. Efficacité Recovery encore inconnue, War_HeavyLand non observé dans la capture. [Reçu, intégrité, essais et rollback](V1_75_DELIVERY_RECEIPT.md).

**Suite :** adaptation des six capacités, OFF complet par pistolet. Ni sélecteur inerte ni OFF partiel livré. HUD en surveillance, autres FX P2 reportés, notification de mise à jour à confirmer. [Preuves](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md). Les sections suivantes sont historiques ; leurs statuts avant construction ne remplacent pas ce reçu.

---

## V1.74 — construite et contrôlée, installation manuelle reportée au prochain lot

Base cumulative V1.73, ses trois essais rapportés validés. Nouvelle visée manette sans tir : **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, pas validée en jeu. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39498950576) : **44/44 suites, zéro avertissement MSVC**. Source `61d9d4b01405ceb45ae802c348cce6dcfaf6a053`, archive `53e2b1645f83e2449eea98d01d3f19bc39ab921f`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/53e2b1645f83e2449eea98d01d3f19bc39ab921f/downloads/DarksidersGenesis_V1.74_NATIVE_PAD_AIM_TEST.zip), 831912 octets, SHA256 `578DC4AC279472C28556F7B930AFCB2FB6C4E6D53D84E4E8B91616A7AC29AA6B`. Quatre fichiers racine, CRC, AMD64, 156 clés fournies, 55 ID Hotkeys conservés ; les 154 anciens réglages gardent exactement leurs valeurs. Les 138 anciennes archives, main/archive, tag v1.43 et fichier de reprise préexistant sont inchangés.

Le README du ZIP contient la préparation V1.74 avant CI sous le titre historique de sa base V1.73 ; le présent reçu et la version compilée V1.74 font référence. L'archive publiée reste immuable. Une tentative d'installation s'est arrêtée dès le contrôle GAME_RUNNING, **avant sauvegarde ou remplacement**. L'annonce précédente de fermeture déduite d'une lecture sandbox était erronée : le contrôle final avec les permissions d'installation a vu le processus actif. Aucun fichier du jeu remplacé. L'utilisateur précise ensuite qu'il installera **manuellement à la fin de sa session** et demande de regrouper davantage de travail ; ne plus demander sa fermeture ou installer automatiquement ce lot.

Suite : capacités et réception lourde, puis OFF pistolet selon preuves. Choix demandé : trois emplacements par personnage, Native ou n'importe laquelle des six capacités. Chaque Blueprint GetWrathAbilityById ne reconnaît que ses propres trois ID ; les six montages utilisent deux squelettes distincts. Le remplacement de FName seul serait inopérant. Ne pas livrer six options présentées fonctionnelles avant adaptation et contrôles des animations/composants. [Recherche du prochain lot](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md).

Capture VM_READ achevée : **7 480 échantillons, trois lectures incohérentes rejetées**, six capacités natives des deux personnages relevées. Trois HeavyLand de Strife au sol, position verticale stable, durées observées d'environ 0,9–1,2 s ; aucun HeavyLand de War capturé. Rampage Slam et AirDash Land de War sont distincts. Clarification utilisateur : Recovery vise **la réception lourde automatique après chute**, pas l'attaque plongeante. Pas encore de nouveau correctif Recovery construit ou testé.

---

## V1.74 — visée manette native sans tir, préparation

Base V1.73 validée dans les essais rapportés ; nouvelle fonction EXPÉRIMENTALE en préparation. Deux entrées configurables, OFF/OFF par défaut, maintien par joueur ; action native AIM/AIM_RELEASE, sans Ctrl ni événement de tir. Source native de visée conservée, protection menus/focus/possession/déconnexion ; direction TPS via la caméra du joueur, sans changement des sorties XInput. [Objectif, preuves, critères et tests](V1_74_NATIVE_PAD_AIM_TEST.md). CI, compilation et jeu non encore validés à ce stade.

Nouvelle demande précisée : trois emplacements indépendants pour chacun des deux personnages, avec Native ou choix parmi les six capacités. Étude des propriétaires, animations et coûts avant échange ; pas de sélecteur factice. HUD V1.73 en surveillance, OFF complet toujours ouvert. Installation actuelle V1.73 conservée pendant le jeu ; aucun PAK/INI/sauvegarde modifié.

---

## HUD non reproduit et poses de Strife relevées — 20 octobre 2026

Retour utilisateur après refus d'arène verrouillée : **« hud toujours visible »**. Défaut placé en surveillance ; aucune correction dédiée ni attribution à un ancien changement. Captures avant/après V1.73 complètes, zéro erreur : pile UI vide, HUD final bShowHUD vrai et nouvelle racine visible/opacité1. L'identité de la racine a changé ; ne pas affirmer une conservation du même widget.

Essai des bras effectué par l'utilisateur : repos, visée seule, Standard et Charge Shot. Capture externe VM_READ terminée : **2 939 échantillons retenus, 30 lectures refusées par les contrôles de cohérence**, aucun changement du jeu. AnimInstance reliée au Strife possédé ; hiérarchie de80os, états natifs de visée et classes de tir relevés. Source active de pose identifiée ; les tableaux Cached* sont vides dans cette session. Le chemin natif des sockets de tir consulte aussi les poses : modifier directement les os ne préserverait pas leur origine. OFF reste ouvert ; recherche d'une séparation visuelle, sans patch direct des poses ou de montage. [Preuves et limites](AUDIT_PISTOL_OFF_HUD_2026_10_20.md).

V1.73 et ses acquis restent en place. Aucun nouvel ASI/ZIP, aucune écriture de configuration, PAK ou sauvegarde. [File restante](ACTIVE_QUEUE.md) : OFF, puis ground slam ; autres FX P2 reportés et Auto check update à confirmer visuellement.

---

## Retour utilisateur V1.73 — trois essais confirmés, 20 octobre 2026

Réponse «1. marche /2. marche /3. marche /prêt» : conservation des montures au chargement, réglages DOF et contrôle visée/tir/dash **VALIDÉS dans les essais rapportés**, sans extrapolation à des cas non décrits. Le jeu est lancé par l'utilisateur. Capture HUD initiale en lecture seule : parcours complet de307380objets,9853objets UI retenus, zéro erreur, racine générale Visibility4/opacité1. Aucun correctif HUD ni résultat après refus encore annoncé ; reproduction demandée.

[File restante remise à jour](ACTIVE_QUEUE.md) : OFF complet par pistolet, HUD général intermittent, puis ground slam. Autres icônes/FX P2 reportés ; Auto check update déjà codé, retour visuel encore attendu. Ancienne file archivée intégralement, aucun historique perdu. Aucun changement de binaire, INI, PAK ou sauvegarde.

---

## Suite après V1.73 — audit OFF / HUD, 20 octobre 2026

**AUDIT / OBSERVATION PRÉPARÉE**, aucun changement ASI ni nouveau build. Inspection privée de 18 assets / 121 exports : 14 montages utilisent Shoot_Right, y compris des montages qui font tirer les deux pistolets ; le cheval utilise Blend. Le slot ne désigne donc pas une main. Arrêt/filtrage d'un slot par main écarté ; conserver les événements de projectile et le correctif dash validé. Les effets spéciaux sur sockets communs et la pose partagée restent à résoudre.

HUD général intermittent : ancienne comparaison corrigée pour exclure les octets voisins et les fonctions. Aucun état fautif global démontré ; nouvelle observation avant/après à largeur exacte et propriétaires recontrôlés préparée hors ASI. Jeu fermé lors du contrôle, arrêt sans lancement ni capture. Préparation utilisateur en attente. [Preuves, piste écartée et prochaine étape](AUDIT_PISTOL_OFF_HUD_2026_10_20.md). V1.73 reste installée ; aucune nouvelle validation en jeu. Ground slam après le reste, autres FX P2 reportés.

---

## V1.73 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. Base cumulative V1.72 contrôlée, retours V1.71 pris en compte. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39469924007) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. La suite propre au candidat Recovery supprimé est retirée ; compatibilité testée dans LegacyRecovery. Avis CMake/MinHook et Node des outils distincts. Source `65f43520c99eda56cf0b63acbb37b7e4d1e83050`, archive `516db3d8cd898e9d544c571a2ba73fdfa38d070b`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/516db3d8cd898e9d544c571a2ba73fdfa38d070b/downloads/DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip) : **826993 octets**, SHA256 `1D768FD2F9E178AE969685B23DB91220BEBCDCC71F5BC8DB2FADB98F3A84AF38`. HTTP 200/hash, CRC, AMD64 et quatre fichiers racine contrôlés. **154 réglages fournis, 55 identifiants de raccourci conservés, 137 anciennes archives inchangées** ; main/archive et v1.43 préservés.

Installation locale contrôlée après fermeture du jeu par l'utilisateur et sauvegarde : ASI/DXGI/README vérifiés, INI personnel et raccourcis conservés, aucun PAK ni sauvegarde modifiés. Aucun lancement par l'agent. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son statut. **Nouveau résultat en jeu NON VALIDÉ.**

Lot : maintien des montures bonus déjà choisies pendant le chargement, y compris disponibilité désactivée, comme les armures ; distance DOF native/TPS jusqu'à **5 000 m** ; retrait du diagnostic périodique des skins et du Recovery dash sans effet. Le modèle de cheval de base reste sélectionnable et son choix n'est jamais remplacé automatiquement. Le filtre natif reste complet pour les autres skins/DLC. Les valeurs par défaut et toutes les fonctions validées restent conservées.

Retour utilisateur : Recovery V1.71 sans effet dans l'essai, **candidat REJETÉ**. Code, politique, état, appel de permission MOVE, checkbox, réglage runtime et clé de l'INI fourni supprimés. Le module de dash revient au code V1.70, vérifié à l'identique pour le comportement ; seule la phrase périmée du bilan a changé. Les anciens ID 3 et 54 et leurs tokens restent réservés/inertes pour conserver les affectations personnelles, exclus des nouvelles assignations ; aucune lecture/écriture de l'ancienne clé de fonctionnalité. La prochaine recherche Recovery commencera par le **ground slam**, après le reste de la file, comme demandé.

Preuves du défaut de monture : Despair existe dans le joueur, le SaveGame actif et le disque avant chargement ; après, seules les armures restent au joueur/SaveGame alors que le disque contient encore Despair. Filtre natif 6A2460 de retrait des lignes DLC identifié. Le candidat conserve seulement les deux choix bonus réellement retirés, via SetSkin sur cette liste. Classes, appel, signatures, types et tailles bornés contrôlés. Aucun choix imposé, droit DLC modifié, patch global de table ou écriture directe de fichier. [Audit natif](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Essais groupés :

1. Choisir Despair et Rampage, sauvegarde normale, recharger puis redémarrer ; vérifier sélection ET modèle. Décocher Bonus mount skins et recharger : bonus équipé conservé avec disponibilité du menu native. Choisir le cheval de base et recharger : il reste de base.
2. DOF Native/TPS : 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2 ; saisie et rechargement, arène → mission/hub, chapitre 8 et transformations. Le dernier rendu V1.71 semble bon dans l'essai utilisateur ; nouvelles distances à valider.
3. Vérification courte des acquis : déplacement + visée + tir spécial + dash, native/TPS. Pas de nouvel essai Recovery demandé.

Suite : OFF complet par main des pistolets (modèle, effets, bras ; projectiles conservés), HUD général intermittent après refus d'arène verrouillée s'il réapparaît, puis étude du ground slam. Autres FX P2 reportés explicitement. V1.72 est préservée, mais n'a pas été installée : V1.73 intègre le dernier retour avant installation. [Critères et rollback](V1_73_MOUNT_DOF_CLEANUP_TEST.md). Les états historiques suivants ne remplacent pas ce reçu.

---

## V1.72 — CONSTRUITE / CONTRÔLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. Base cumulative V1.71, acquis validés préservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39468587155) : **44/44 suites, zéro avertissement MSVC ASI/DXGI**. Avis CMake/MinHook et Node des outils distincts. Source `2973184516b1b035d47e80f800b0833af6411d37`, archive `9dc0eef9afdb237fea1953fea2df7c9632fdbbee`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9dc0eef9afdb237fea1953fea2df7c9632fdbbee/downloads/DarksidersGenesis_V1.72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.zip) : **829605 octets**, SHA256 `A2D87C852B5CBB836B57ECC5EECF3ABCC8FC33B4D59CA829D72BE2A7F78190D2`. HTTP 200/hash, CRC, AMD64 et exactement quatre fichiers racine contrôlés. **155 clés et 55 actions conservées ; 136 anciennes archives inchangées.** Main/archive et v1.43 préservés.

Installation locale prête mais non effectuée : le jeu est encore ouvert. Demande de fermeture en attente ; aucun fichier du jeu remplacé par cette livraison à ce stade. INI, raccourcis, PAK et sauvegardes conservés. Aucun lancement par l'agent. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son statut. **Nouveau résultat en jeu NON VALIDÉ.**

Lot : maintien des deux choix de monture bonus au chargement et distance DOF native/TPS jusqu'à **5 000 m**, sans changer les valeurs par défaut. L'option de skins contrôle seulement leur disponibilité dans le menu : un bonus déjà équipé reste conservé même OFF, comme les armures. Choisir le modèle de base le conserve normalement aux chargements suivants, sans réimposition du bonus. Le filtre natif reste complet pour les autres skins et droits DLC. L'ancien diagnostic périodique V1.71 et son appel par image sont retirés ; un seul hook de chargement ciblé, pas d'empilement de correctifs de personnage.

Preuves : sur la session V1.71, Despair existe dans le joueur, le SaveGame actif et le disque avant rechargement ; après, les deux armures restent et Despair manque au joueur/SaveGame, alors que le disque le contient encore. Filtre natif 6A2460 identifié : retrait des lignes marquées DLC absent. Réinsertion des seuls choix bonus retirés via SetSkin natif, dans cet appel. Classes, appel, signatures, types et listes bornés contrôlés ; pas d'écriture directe de sauvegarde ni de droits DLC. Les simulations vérifient notamment OFF conservant le choix mais pas sa disponibilité, modèle de base/non-bonus non forcé, autre choix natif prioritaire et absence de doublon. [Audit, limites et rollback](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Essais groupés :

1. Choisir Despair et Rampage, laisser la sauvegarde normale, recharger puis redémarrer ; vérifier sélection et modèle. Refaire case décochée : bonus équipé conservé, disponibilité du menu native. Choisir ensuite le cheval de base et recharger : il doit rester de base.
2. DOF : Native/TPS, saisie 20 / 1 250,5 / 5 000 m et quantité 0 / 0,6 / 2 ; redémarrage, arène → mission/hub, chapitre 8, angles, pied/cheval/ultime et changements de personnage. Dernier retour V1.71 : flou « ça a l'air ok », limité à cet essai.
3. Dash movement recovery V1.71 OFF/ON, Strife troisième/quatrième dash et War, déplacement/visée/tir, native/TPS ; toujours OFF par défaut, bénéfice sans nouveau verdict. Rejeter en cas de glissement ou régression.

OFF complet des pistolets et HUD général intermittent après refus d'arène verrouillée restent ouverts ; autres effets P2 reportés. La première compilation V1.72 a été annulée pendant la construction après la précision utilisateur sur OFF ; elle n'a publié aucune archive. La CI ci-dessus inclut cette précision. Les états historiques ci-dessous ne remplacent pas ce reçu.

---

## V1.71 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, lot EXPÉRIMENTAL

20 octobre2026. Base V1.70, acquis validés conservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39454685096) : **44/44 suites, zéro avertissement MSVC ASI/DXGI**. Source `4ec834595bcf3aca9809006862019459b29f4257`, archive `82a5c678985c5ed397466a37f6ef28fc56ce1bb1`. [ZIP immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/82a5c678985c5ed397466a37f6ef28fc56ce1bb1/downloads/DarksidersGenesis_V1.71_DASH_MOVE_BATCH_TEST.zip) :826278octets, SHA256 `ABE9F9E24C7BBD53E315D12B142E74EE82B74B9FCD608DE924A05E3148310A82` ; HTTP200/hash, CRC, AMD64 et quatre fichiers racine contrôlés. **155 clés,55 actions,135 anciennes archives inchangées** ; main/archive/v1.43 conservés.

Installation locale avec sauvegarde, jeu fermé : ASI/DXGI/README vérifiés, INI et raccourcis personnels conservés, aucun PAK ni sauvegarde modifiés. Aucun lancement. Le README du ZIP décrit la préparation avant CI ; ce reçu complète son état. **Le nouveau rendu et la reprise du déplacement sont NON VALIDÉS en jeu.**

Lot cumulé : réglages DOF par vue et région native V1.70 ; candidat Dash movement recovery OFF par défaut, sans nouveau hook. Il attend une décélération observée terminée, une vélocité quasi nulle ET une position réelle stable sur deux observations, puis permet une seule demande native MOVE réelle par activation. L’ancienne fin forcée Recovery n’est pas réintroduite. Nouvelle action Hotkeys54, sans affectation ; anciens ID/bindings inchangés. Le simulateur du callback vérifie P1/P2 et native/TPS, refus pendant déplacement réel malgré vélocité nulle, pause et implémentation inconnue, absence de fin forcée ou d’écriture d’acteur/mouvement. Ces contrôles ne prouvent pas le bénéfice en jeu. [Objectif, preuve, essais regroupés et rollback](V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

Les nouvelles observations de sélection des chevaux sont un **diagnostic borné**, sans choix forcé ni écriture save. Copie privée de6sauvegardes conservée pour comparaison, sources inchangées. Le dernier relevé des deux parties actives contient les armures sans les entrées bonus de monture ; il confirme la perte, pas sa cause. **Chevaux au chargement et OFF complet pistolet restent OUVERTS.** Relecture privée de167fonctions Blueprint, tailles de script toutes concordantes ; slots Shoot_Left/Right distincts mais visée partagée au niveau Spine1. Aucun OFF partiel ni manipulation de montage présentée comme terminée.

### Essais à faire ensemble quand l’utilisateur sera prêt

1. Rendu automatique : arène → hub/mission, chapitre8, angles, changements de personnage, pied/cheval/ultime.
2. DOF : groupes Native/TPS différents ; Disable puis Custom distance0/5/20m et quantité0/0,6/2 ; saisie, pause/roues, cinématique, coop, redémarrage.
3. Dash : nouveau candidat OFF/ON, Strife trois dashs puis quatrième, War, déplacement maintenu puis nouvel appui, visée et tir spécial, native/TPS et coop. Rejeter le candidat si aucun bénéfice net, glissement, premier dash répété ou régression de visée/tir ; OFF arrête les prochaines demandes.
4. Skins : choisir les deux bonus avec option cochée, laisser une sauvegarde habituelle, recharger puis redémarrer. Les observations de listes et les fichiers privés permettront ensuite de comparer le point de perte ; réparation non annoncée.

HUD général après refus d’arène verrouillée intermittent ; autres sprites/FX P2 reportés. Pas de test immédiat supplémentaire demandé. Les préparations historiques ci-dessous ne remplacent pas ce reçu.

---

## Préparation V1.71 — lot cumulé, 20 octobre2026

Demande utilisateur : poursuivre et regrouper les essais. Base V1.70 construite/installée, rendu encore expérimental ; acquis validés conservés. Nouveau candidat **Dash movement recovery** OFF par défaut : reprise par la cause MOVE native après décélération observée et arrêt, sans fin d’ability forcée. Observation des deux listes de skins chevaux, bornée et sans écritures. OFF complet pistolet : slots gauche/droite identifiés, couche de visée partagée ; pas encore livré. **PRÉPARÉ / EXPÉRIMENTAL**, aucune validation de jeu supplémentaire. [Preuves, critère, essais groupés](V1_71_NATIVE_DASH_MOVE_BATCH_TEST.md).

## V1.70 — CONSTRUITE / CONTRÔLÉE / INSTALLÉE, rendu EXPÉRIMENTAL

20 octobre 2026. Base V1.69 et acquis préservés. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39443047647) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. Avis d’outillage CMake/MinHook et Node distincts. Source `3ed44ddf0aba240e2b85a96fa2f8d9cdec231939`, archive `e7a9a18b166480b296780191e22ed85e81bfbc57`. [ZIP immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/e7a9a18b166480b296780191e22ed85e81bfbc57/downloads/DarksidersGenesis_V1.70_PER_VIEW_DOF_TEST.zip) : **822447 octets**, SHA256 `2FF56D6E2995985F462EA2D85CEED1A245A39C7D5588D18A0D3450C1B06DE6CE` ; HTTP200/hash, CRC, AMD64 et exactement quatre fichiers racine contrôlés. **154 clés,54 actions,134 anciennes archives inchangées** ; main/archive/v1.43 préservés. Aucun ancien défaut déclaré validé par la CI.

**Installée avec sauvegarde, jeu fermé : ASI/DXGI/README contrôlés ; INI personnel, raccourcis, sauvegardes et PAK conservés. Aucun lancement.** Le README contenu dans le ZIP décrit la préparation avant la CI ; ce reçu confirme les contrôles effectués après publication.

Correction du flou proche TPS : sans région déjà surchargée par la caméra, conserver celle du niveau ; la capture V1.69 montrait3000 unités remplacées par environ55 après l’arène. Deux profils **Native view / TPS view** : Disable, Custom distance0–200m et quantité0–5% absolue ; tous OFF par défaut, valeurs inactives10m/0,6%. Même disposition que les autres valeurs (curseur/saisie/Default). Pas de nouveau hook. Cinématiques ignorées par les profils. **Rendu NON VALIDÉ en jeu.** [Preuves, critères, essais et rollback](V1_70_PER_VIEW_DOF_TEST.md).

**Skins chevaux : bug OUVERT**, option Bonus mount skins toujours cochée au chargement, choix bonus à refaire ; armures semblent rester. La tentative de lecture supplémentaire a trouvé le jeu fermé et s’est arrêtée sans lancement. Copie native des sélections identifiée, point de perte non démontré ; aucun patch save/DLC ou sélection forcée. **HUD général après refus d’arène verrouillée : intermittent, non reproduit dans l’essai actuel**, pas de correction annoncée. OFF complet pistolet et nouvelle approche Recovery ouverts ; autres sprites/effets P2 reportés selon l’utilisateur.

### Essais V1.70

1. Laisser Disable et Custom OFF dans les deux groupes : arène puis hub/mission, chapitre8, plusieurs angles, changement de personnage ; le décor ne doit plus devenir flou à deux mètres.
2. En TPS : Disable ON/OFF, puis Custom distance0/5/20m et quantité0/0,6/2. Vérifier curseur et saisie, intensité, distance et retour au rendu du niveau.
3. Donner des valeurs différentes aux deux vues : TPS/native temporaire, pied/cheval/ultime, pause/roues, cinématique et coop. Recharger pour vérifier la persistance des réglages ; sélection des chevaux toujours en diagnostic.

---

## Reprise V1.70 — rendu par vue, 20 octobre 2026

Base cumulative V1.69 ; Léviathan et menu validés par l’utilisateur. Le flou V1.69 a un nouveau contre-exemple après l’arène : région volume3000 remplacée par environ55 dans la caméra TPS, capture439 relevés sans erreur. V1.70 préserve la région du volume en mode automatique et ajoute deux profils DOF indépendants Native/TPS (Disable, Custom distance/intensité), tous OFF par défaut. **PRÉPARÉ / EXPÉRIMENTAL**, compilation/paquet/rendu à vérifier. [Critères, preuves, essais et rollback](V1_70_PER_VIEW_DOF_TEST.md).

Skins chevaux : **ouverts**, retour modèle natif au chargement avec Bonus mount skins coché, sélection à refaire. Armures semblent conservées. HUD général après refus d’arène verrouillée : ancien défaut confirmé, non reproduit dans l’essai actuel ; aucune correction annoncée. Aucun patch de sélection/save ou HUD aveugle.

## Précision utilisateur — HUD général après refus d’une arène verrouillée

20 octobre 2026, après livraison V1.69. Le problème touche **le HUD général**. Déclencheur : cliquer une arène encore verrouillée ; un message indique que le joueur ne l’a pas débloquée, le menu devient transparent, puis le HUD général manque. L’ancienne abréviation « HUD d’arène » ne désignait pas le bon élément ; ne pas limiter la recherche au widget UI_ArenaHUD.

Les données cuites déjà relevées montrent StartMission puis l’animation outro dans le chemin de sélection ; cette animation possède deux pistes RenderOpacity. Le corps natif 585010 présente aussi une branche d’échec après le test 5841F0, avant le chemin de préparation de mission. Cela concorde avec le refus et la transparence décrits, **sans établir quelle branche a été exécutée dans la reproduction ni la cause finale**. Piste : transition de fermeture et restitution du mode UI/HUD après refus. Les captures précédentes (hide natif/mod=0, racine visible, opacité 1, IsInViewport vrai) restent pertinentes.

Prochain relevé ciblé : HUD général, page de sélection, message de refus, pile du gestionnaire UI et état de l’animation de fermeture, avant/après le clic. Critère de réussite : après refus, l’interface retrouve son état correct et le HUD général revient en reprenant le jeu, sans bascule manuelle du HUD. **Correction ouverte ; aucun nouveau patch ni build dans cette clarification.** ASI installé V1.69, INI, raccourcis et archives inchangés.

---

## V1.69 — CONSTRUITE / CONTRÔLÉE, corrections visuelles EXPÉRIMENTALES

20 octobre 2026. [GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39412347126) : **43/43 suites, zéro avertissement MSVC ASI/DXGI**. Avis CMake/MinHook et Node des outils distincts. Source `02aac6e0a04ad3fec7d2e7dfcc01425a0d08318f`, archive `28f923c0c3336c75d7b7ec76232fc22bc56a13b0`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/28f923c0c3336c75d7b7ec76232fc22bc56a13b0/downloads/DarksidersGenesis_V1.69_NATIVE_VISUAL_FOCUS_TEST.zip) : **818621 octets**, SHA256 `C5197C120DD59FB607BF2B33AF56B15EC9D020DA33C4DB01A7E82D3591CFE514`. Téléchargement public HTTP 200, hash identique à la CI, CRC, AMD64, exactement quatre fichiers racine, marqueurs, **146 clés et 54 actions** contrôlés. **133 anciennes archives et trois documents historiques inchangés** ; main/archive et v1.43 conservés.

Base V1.68, acquis V1.66/V1.67 conservés. Lot : attache native Léviathan, méthode DOF du niveau et protection de la profondeur du personnage en TPS, sections Appearance / Weapon sheathing repliables. Ancien forçage CircleDOF retiré, aucun nouveau hook ou réglage. Nouveau rendu **NON VALIDÉ en jeu**. Installation locale contrôlée, jeu fermé et sauvegarde préalable : ASI/DXGI/README vérifiés, INI personnel et absence actuelle du PAK de skins conservés. Aucun lancement par l’agent. [Preuves, critères, essais groupés et rollback](V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

### Skins : autonomie ASI établie dans le dernier essai

Le journal moteur de la session V1.68 du 20 octobre, ouverte à 12:47:13 et fermée à 12:57:56, n’enregistre que le montage de ProjectMayhem-WindowsNoEditor.pak. Le PAK PreOrder est absent du dossier. Avec le retour utilisateur sur la sélection des chevaux/armures et le retour natif OFF, cela confirme l’essai **sans le PAK cosmétique**. Maintien de l’apparence après redémarrage encore à tester. Aucun PAK déplacé/supprimé par l’agent. Cette preuve complète les réserves des paragraphes historiques suivants.

HUD après arène verrouillée, OFF complet pistolet et nouvelle approche Recovery restent ouverts. Les autres FX P2 restent reportés ; Auto check attend sa confirmation visuelle. Les paragraphes de préparation ci-dessous sont historiques, pas des contrôles encore en attente.

---

## V1.69 — préparation depuis les retours V1.68

20 octobre 2026. Durée ultime infinie **VALIDÉE par l'utilisateur** ; changement de personnage et netteté du héros au chapitre 8 semblent bons. Décors trop nets en TPS ; Léviathan fonctionne mais trop bas. Skins chevaux/armures et retour natif OFF confirmés ; maintien après redémarrage et autonomie sans PAK non certifiés.

Lot : attache Léviathan sur la racine native (écart mesh −97 prouvé par VM_READ), méthode DOF du niveau et protection de la profondeur du personnage, sections Appearance / Weapon sheathing repliables. **PRÉPARÉ / EXPÉRIMENTAL**, construction Windows et nouveau rendu à contrôler. [Preuves, limites, critères, essais et rollback V1.69](V1_69_NATIVE_VISUAL_FOCUS_TEST.md). HUD d'arène, OFF complet pistolet et nouvelle approche Recovery ouverts ; autres FX P2 reportés.

---

## V1.68 — CONSTRUITE / CONTRÔLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. [CI Windows x64 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39404000199) : **43/43 suites**, aucun avertissement MSVC ASI/DXGI ; avis CMake/MinHook et Node des outils distincts. Source `eac8da5d1e67e6c3afd3e50d3f83c7acf569d7a8`, archive `b1a7110048071987facf6f28af0475b87c1a272b`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b1a7110048071987facf6f28af0475b87c1a272b/downloads/DarksidersGenesis_V1.68_VISUALS_ULTIMATE_TEST.zip) : **817200 octets**, SHA256 `110C3E851C966FD26E87FE31450475553E2985A7158643430923117B2A486960`. Téléchargement public HTTP 200, hash identique à la CI, CRC, AMD64, quatre fichiers racine, marqueurs, 146 clés et 54 identifiants Hotkeys contrôlés. **132 anciennes archives inchangées.** Main/archive et v1.43 conservés.

Base : V1.66 validée pour sauts/holsters + choix des pistolets V1.67 validés. Lot : vrai IsSkinUnlocked à la place du mauvais IsSkinEquipped, focus du même point utilisé pour émettre la géométrie TPS pendant le changement, case **Infinite Ultimate duration**, visuel seul **Leviathan OFF/1/2/3**. Nouveaux résultats en jeu **NON VALIDÉS**. Aucun lancement ni remplacement des fichiers du jeu par l’agent à ce stade ; INI/raccourcis et PAK personnels préservés. [Objectifs, preuves, limites, tests groupés et rollback](V1_68_VISUALS_ULTIMATE_TEST.md). Les paragraphes « préparation » suivants sont historiques.

### HUD : reproduction capturée, correction encore ouverte

L’utilisateur a préparé un HUD visible puis a reproduit la disparition après sélection d’arène verrouillée. Capture du 20 octobre : PID 12880, **V1.67** chargée (ASI SHA256 `ECDE0F74B69B8A3558669A45422BB7D05C5ADDC2135EC4D9DA127D2FCF164286`). 8907 échantillons en cinq minutes, zéro erreur de lecture : `ui.HideHud` natif jeu/rendu reste **0/0**, qualité DOF **2/2**. Le booléen interne du mod `g_hudHidden`, identifié dans le getter compilé V1.67 et lu après signature du module, vaut également **0** ; aucun événement Toggle HUD dans le journal. Cela exclut les deux masquages de ce getter lors de ce relevé, sans prouver une cause extérieure à tout le reste du mod.

Deux instantanés avant/après, sans écriture ni appel natif : acteur HUD et racine générale présents, `bShowHUD` vrai, visibilité racine 4, opacité 1. Les pointeurs et propriétés ne doivent être comparés que sur leur taille réelle : les 16 octets de contexte capturés contiennent parfois le champ voisin ; les UFunction ne sont pas des propriétés d’instance. Le vrai test natif IsInViewport a ensuite été retracé (wrapper 15190A0, consommateur 14B41D0) et reproduit uniquement par lecture : pointeurs +208/+210 présents, compteur partagé +8 = 1, donc résultat vrai. Un simple retrait du viewport n’explique pas cette panne. L’arbre détaillé après panne est conservé en privé pour remonter les états d’affichage/bindings et couches Slate. **Cause et correction non établies ; aucune réparation HUD revendiquée dans le ZIP V1.68.** Les autres éléments possiblement manquants restent un signalement incertain. OFF complet des pistolets et autres tâches de la file restent ouverts.

---

## V1.68 — préparation expérimentale ; erreur de skins V1.67 corrigée à la source

20 octobre 2026. Base retenue : V1.66 (sauts/holsters validés), plus les sélecteurs gauche/droite de pistolet V1.67 **VALIDÉS par l’utilisateur**. Montures V1.67 **en échec** sur une nouvelle partie avec le PAK actif ; autres skins non testés. Le mauvais branchement IsSkinEquipped est retiré au profit du vrai IsSkinUnlocked, avec contrôle de l’association native nom/wrapper/appel. Les déclarations anciennes de provenance du rapport V1.67 sont corrigées dans [le rapport V1.68](V1_68_VISUALS_ULTIMATE_TEST.md).

Lot préparé : correction des skins, continuité de mise au point au changement de personnage, case durée ultime infinie (sans remplissage), sélecteur visuel Léviathan OFF/1/2/3. Trois clés nouvelles opt-in, 146 clés au total, 54 identifiants Hotkeys (anciens IDs conservés). **Construction Windows et vérification du ZIP encore à effectuer ; nouveaux résultats en jeu NON VALIDÉS.** Aucun lancement, installation, remplacement du PAK ou changement d’INI personnel.

Le flou signalé concerne plusieurs niveaux. Relevé : qualité DOF 2/2 conservée ; mode lentille TPS modifiant le rendu général, trou de compensation d’environ 0,64 s au changement observé. HUD absent après sélection d’arène verrouillée : **ouvert, aucune correction revendiquée**. Autres éléments HUD manquants : information incertaine. OFF complet des pistolets (modèle/effets/bras, projectiles conservés) encore ouvert. Recovery et autres effets P2 reportés restent séparés. [Essais groupés, preuves et rollback](V1_68_VISUALS_ULTIMATE_TEST.md).

---

## V1.67 — CONSTRUITE / CONTRÔLÉE, apparences EXPÉRIMENTALES

19 octobre 2026. Base V1.66 validée pour sauts/holsters, conservée. Choix du pistolet natif gauche/droit par main de Strife ; deux cases distinctes pour les montures bonus et les six apparences de progression. **Nouveaux comportements NON VALIDÉS dans le jeu.** [Inventaire, preuves, essais et rollback](V1_67_PISTOL_CHOICES_AND_SKINS.md).

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39253436601) : **42/42 suites, zéro avertissement MSVC ASI/DXGI** ; avis de dépréciation MinHook/CMake et Node distincts. Source `4a4bd1bee5126ee9a21cf78fe90048f6c509776d`, archive `c760123057e66f2b3d7053b7acf3441de2cee44e`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c760123057e66f2b3d7053b7acf3441de2cee44e/downloads/DarksidersGenesis_V1.67_PISTOL_CHOICES_SKINS_TEST.zip) : 809061 octets, SHA256 `8E37AE2BBBAD9465BEFE2BC75F2566B74311EB0908E01682D1D6CD02EF3B9591` ; CRC, AMD64, quatre fichiers à la racine, marqueurs, 143 clés/53 actions, téléchargement HTTP 200 et signature identique contrôlés. **131 anciennes archives inchangées.** Configuration personnelle et PAK non modifiés, aucun lancement ni installation par l’agent. Main/archive et tag stable conservés.

Essais : combinaisons de pistolets à pied/cheval, tir/rangement et Native ; **fermer et rouvrir la page Skins après un changement d’option**, vérifier les deux groupes puis OFF. Le PAK déjà installé garde ses deux montures disponibles même avec la case ASI OFF ; l’autonomie ASI exige un essai sans ce seul PAK, déplacé réversiblement après fermeture du jeu. Choisir une apparence possédée avant de désactiver son groupe. Recovery et autres effets P2 restent hors du lot. Les préparations suivantes sont historiques ; elles ne signifient plus que la construction est en attente.

---

# V1.67 préparée — choix des pistolets et disponibilité cosmétique

19 octobre 2026. **PRÉPARÉ / EXPÉRIMENTAL, construction Windows et contrôle du ZIP à effectuer.** Base retenue V1.66, sauts et holsters validés dans le retour utilisateur ci-dessous. Choix indépendant du modèle droit/gauche par main de Strife, plus deux cases OFF pour montures bonus et six skins de progression. [Inventaire, preuve native, limites, tests et rollback](V1_67_PISTOL_CHOICES_AND_SKINS.md). Configuration personnelle et PAK installés non modifiés ; aucun lancement ni installation. Les nouvelles fonctions V1.67 ne sont pas validées en jeu.

L’état V1.66 ci-dessous décrit la base validée conservée, pas le statut des nouvelles options V1.67.

---

# État courant — V1.66 debug

19 octobre 2026. **Base de travail retenue : V1.66, CONSTRUITE / CONTRÔLÉE ; sauts et holsters VALIDÉS par l’utilisateur dans son essai.** Retour explicite reçu après les tests ; le canal reste debug. Cette mise à jour enregistre la validation sans modifier le code, la configuration ni le paquet publié.

## Livraison vérifiée

- Source : `f01f70d563dc4e3716464718fb6642bbdd8f9702`.
- Archive : `12061a956b9257284c8b1779503cf058b75fab1f`.
- [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39217494013) : 41/41 suites, aucun avertissement MSVC ASI/DXGI. Avis de dépréciation des outils distincts.
- [ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) : 934413 octets, SHA256 `CC15584E210717E4F173967BE47024CA949983FD24870B03190191936888F67F`. CRC, AMD64, quatre fichiers racine, marqueurs et téléchargement HTTP 200 contrôlés.
- 139 réglages et 53 actions conservés ; 130 anciens ZIP vérifiés intacts à la livraison. Main/archive et tag v1.43 protégés. Aucun remplacement du ZIP V1.66 par ce travail documentaire.

## Derniers retours faisant autorité

État antérieur V1.65 : holsters montés correctement placés, petit délai ; trois délais de rangement validés ; durée ultime validée après activation de sa case (l’utilisateur a rétracté son premier échec). Le double saut était encore défectueux : deux remontées sans animation puis blocage.

V1.66 remplace la substitution isolée du getter par un réarmement natif conjoint au point de lecture d’animation. L’animation voit le vrai compteur disponible puis le saut natif republie deux. Recherche des modèles montés anticipée à pied. **Retour V1.66 : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». Sauts et holsters VALIDÉS dans cet essai, code retenu.** Le retour ne détaille ni nombre de sauts, ni modes/personnages testés, ni mesure du délai initial ; aucune couverture exhaustive n’est déduite. [Détails et essais](V1_66_NATIVE_JUMP_CYCLE_TEST.md).

Les acquis précédents de visée, reprise AIM après dash, retrait des projectiles/sons/effets Standard parasites, netteté TPS, Standard limité, grappin, ressources et raccourcis sont conservés. Aucun nouveau retour ne les invalide. Autres effets et icônes P2 reportés sur demande explicite.

## Travail poursuivi pendant l’essai

Nouvel audit du chemin d’interruption natif de Recovery et inventaire élargi des modèles de Strife. **Aucune modification de source runtime, de configuration, de binaire ou de mémoire du jeu dans cette reprise.** Le relevé externe préparé n’a pas été exécuté sur le jeu. L’ancien Recovery reste retiré. [Rapport et limites](NEXT_RECOVERY_AND_MODELS.md).

README, file active et roadmap séparés des anciennes préparations ; les trois anciens documents sont conservés octet pour octet dans `docs/history/`, avec manifest de signatures. Le carnet technique reste cumulatif. Les versions et références du présent état n’impliquent aucune promotion de main ni de release publique.

## Reprise suivante

Repartir de V1.66 validée pour les sauts et holsters. Consulter la [file active](ACTIVE_QUEUE.md) et les logs réellement disponibles. Action Recovery et modèles alternatifs restent des travaux ouverts ; autres effets P2 différés, Auto check update sans confirmation visuelle distincte. Préserver l’INI personnel, les raccourcis, les sauvegardes et les acquis. Pas de lancement/rechargement du jeu par l’agent sans demande explicite. Construire une version distincte seulement pour un nouveau lot retenu.

[Ancien état intégral jusqu’à V1.66](history/CURRENT_STATE_THROUGH_V1_66.md) · [Carnet technique](TECHNICAL_NOTEBOOK.md) · [Roadmap actuelle](ROADMAP.md).
