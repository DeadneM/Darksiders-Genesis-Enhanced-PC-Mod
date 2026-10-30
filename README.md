## V1.135 - retour a l'acquisition native

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU.** Remplace le filtre de cible Statique et l'aide ajoutee par le selecteur natif et son point socket/bornes. Option0..200%,100%=reference native, OFF par defaut ; Global ou sept munitions. Deux hooks obsoletes retires, un hook natif garde ; portee/poids/degats/chaines non ajustes. Profil actif et proprietaires confirmes par1785echantillons en lecture seule, sans erreur.

Portee du nouvel ajustement : Strife a pied P1/P2 ; monture/ultime gardent le selecteur natif.218autres valeurs /55Hotkeys et fonctions validees conserves. Schema d'aide neuf,228cles. [Perimetre, nettoyage, limites et tests](docs/V1_135_NATIVE_ACQUISITION_TEST.md). GitHub seulement, V1.133 reste installee ; aucun INI ou fichier de jeu remplace. main/archive et anciennes archives preserves.

---

## V1.134 - retour a l'acquisition native

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU.** Remplace le filtre de cible Statique et l'aide ajoutee par le selecteur natif et son point socket/bornes. Option0..200%,100%=reference native, OFF par defaut ; Global ou sept munitions. Deux hooks obsoletes retires, un hook natif garde ; portee/poids/degats/chaines non ajustes. Profil actif et proprietaires confirmes par1785echantillons en lecture seule, sans erreur.

Portee du nouvel ajustement : Strife a pied P1/P2 ; monture/ultime gardent le selecteur natif.218autres valeurs /55Hotkeys et fonctions validees conserves. Schema d'aide neuf,228cles. [Perimetre, nettoyage, limites et tests](docs/V1_134_NATIVE_ACQUISITION_TEST.md). GitHub seulement, V1.133 reste installee ; aucun INI ou fichier de jeu remplace. main/archive et anciennes archives preserves.

---

## Audit Statique - 30 octobre 2026

**REGRESSION SIGNALEE V1.133 ; AUDIT CONTROLE, REPARATION NON CONSTRUITE.** Le journal compte5166applications d'aide, mais2637/2637cibles principales Statique retirees. Le filtre V1.130 modifie le chemin natif de longueur et le cycle des chaines ; sa preservation n'est plus consideree comme correcte. Les curseurs V1.131..133 ajoutent une correction, ils ne reglent pas le natif. Objectif revise : restaurer selection/cycle natifs puis ajustement optionnel,100%=natif, sans changer portee/degats/rebonds. [Audit et limites](docs/STATIC_NATIVE_BEHAVIOR_AUDIT_2026_10_30.md). Autres acquis conserves ; V1.133 reste installee, aucune nouvelle compilation/installation/capture. Verification du contrat natif de parametres avant implementation.

---

## V1.133 - visibilite et profondeur de l'aide TPS

**CONSTRUITE / CONTROLEE : [68/68 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40959653744), zero avertissement/erreur MSVC. [ZIP cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f3b9ec2cc71ef997543cfe5d2751a867190db6f/downloads/DarksidersGenesis_V1.133_AIM_ASSIST_TARGET_DEPTH_TEST.zip) - [Recu](docs/V1_133_DELIVERY_RECEIPT.md). Updater debug et Defender controles ; correction EXPERIMENTALE, NON VALIDEE EN JEU. V1.133 installee sur demande ; V1.130 reste la base acceptee.**

**CONSTRUITE / CONTROLEE ; NON VALIDEE EN JEU.** Retour V1.131 : valeur augmentee puis remise a zero, aucune application journalisee. Le garde exigeant un contact bloquant avec l'ennemi est incompatible avec des profils natifs Projectile Overlap/Ignore. Verification remplacee par absence d'obstacle avant la cible ; journal des valeurs utilisees et raisons de refus. Cela reste une correction a tester, pas la preuve de l'unique cause en jeu.

Le point assiste garde aussi la profondeur finie du candidat : convergence depuis le canon et solveur natif Lava ne visent pas le mur/ciel derriere un ennemi non bloquant. Un obstacle plus proche sur le rayon partiel garde la priorite.

Base acceptee V1.130 ; mode Global / sept munitions V1.131, valeurs par defaut et INI personnel conserves. Cone, priorite aux collisions proches, contact Statique, cameras/input, pose UI et pouvoirs inchanges. [Perimetre et essai](docs/V1_133_AIM_ASSIST_TARGET_DEPTH_TEST.md), [deux passes](docs/evidence/V1_133_STATIC_REVIEW.json). Compilation GitHub ; V1.133 installee sur demande, INI conserve, jeu non lance ; main/archive/anciens ZIP preserves.

Installation demandee le 30 octobre 2026 : V1.133 installee et relue, sauvegarde V1.131 `before-V1.133-20261030-194838-8270be80` ; INI/raccourcis byte-identiques, EXE original et PAK preserves, jeu non lance. [Preuve](docs/evidence/V1_133_INSTALL_CHECK.json).

---

## V1.132 - visibilite de l'aide TPS

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU.** Retour V1.131 : valeur augmentee puis remise a zero, aucune application journalisee. Le garde exigeant un contact bloquant avec l'ennemi est incompatible avec des profils natifs Projectile Overlap/Ignore. Verification remplacee par absence d'obstacle avant la cible ; journal des valeurs utilisees et raisons de refus. Cela reste une correction a tester, pas la preuve de l'unique cause en jeu.

Base acceptee V1.130 ; mode Global / sept munitions V1.131, valeurs par defaut et INI personnel conserves. Cone, priorite aux collisions proches, contact Statique, cameras/input, pose UI et pouvoirs inchanges. [Perimetre et essai](docs/V1_132_AIM_ASSIST_VISIBILITY_TEST.md), [deux passes](docs/evidence/V1_132_STATIC_REVIEW.json). GitHub seulement ; aucune installation V1.132 ni jeu lance, main/archive/anciens ZIP preserves.

---

## V1.131 - adjustable TPS aim assist

**CONSTRUITE / CONTROLEE : [68/68 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40947674176), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/73c494fb4acdd7d33125427dd2b084b921a434bc/downloads/DarksidersGenesis_V1.131_TPS_AIM_ASSIST_TEST.zip) - [Recu](docs/V1_131_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Nouvelle aide EXPERIMENTALE, NON VALIDEE EN JEU ; V1.131 ensuite installee sur demande, INI conserve, jeu non lance. main reste V1.124.**

**CONSTRUITE / CONTROLEE ; NOUVELLE AIDE NON VALIDEE EN JEU.** V1.130 explicitement validee par l'utilisateur devient la base. Ajout demande : aide TPS0..100%, Global ou Per ammunition (sept valeurs), saisie manuelle/Default ;0 par defaut conserve V1.130. Meme calcul de cible3D pour les consommateurs, cible native visible dans un cone borne ; aucune rotation d'acteur ou nouveau hook.

Neuf nouvelles cles /227 au total ;218 anciennes valeurs et55ID Hotkeys conserves. Modeles/pose UI/Statique V1.130, cameras/input et autres acquis preserves. [Perimetre, limites et essai](docs/V1_131_TPS_AIM_ASSIST_TEST.md), [validation V1.130](docs/evidence/V1_130_USER_VALIDATION.json). Construction GitHub seulement ; V1.131 installee sur demande, INI conserve, jeu non lance. main reste V1.124, archive et anciens ZIP intacts. Les sections suivantes sont historiques.

Installation V1.131 demandee et verifiee : sauvegarde V1.130 `before-V1.131-20261029-212759-8dd7d05e`, ASI/DXGI/README relus, INI personnel byte-identique, PAK/EXE preserves. [Preuve](docs/evidence/V1_131_INSTALL_CHECK.json).

---

## V1.130 - Static contact and unarmed pause presentation

**CONSTRUITE / CONTROLEE : [67/67 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40807486983), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0aac20c4f76f6d10ce11f2ac5fbf718beaf2360e/downloads/DarksidersGenesis_V1.130_STATIC_CONTACT_UNARMED_PAUSE_TEST.zip) - [Recu](docs/V1_130_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Statique/pose UI EXPERIMENTAUX, NON VALIDES EN JEU. Masquage modele V1.129 confirme conserve ; installation locale ensuite demandee et verifiee, INI conserve, jeu non lance. main reste V1.124.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Retour V1.129 : Statique et pose de main vide toujours incorrects ; utilisateur confirme que le modele masque reste cache. Masquage/modeles V1.129 conserves. Filtre initial remplace par le controle de contact pendant les reassignations natives du faisceau principal ; conservation de la derniere pose desarmee valide pendant UI/focus uniquement.

Base cumulative V1.129 ;218 valeurs INI /55 Hotkey ID et autres acquis preserves. [Causes, preuves, limites et tests](docs/V1_130_STATIC_CONTACT_UNARMED_PAUSE_TEST.md). Construction GitHub seulement, installation locale demandee et verifiee, jeu non lance ; main reste V1.124, archive et anciens ZIP preserves. Coop plus tard. Les sections suivantes sont historiques.

V1.130 installee sur demande explicite, sauvegarde V1.129 `before-V1.130-20261029-034821-7254d2ef` verifiee ; ASI/DXGI/README relus, INI personnel identique octet pour octet, PAK/EXE preserves. [Preuve d'installation](docs/evidence/V1_130_INSTALL_CHECK.json).

---

## V1.129 - Static targeting and pistol appearance lifecycle

**CONSTRUITE / CONTROLEE : [67/67 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40801514118), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/029b17124e24f766db66b5a171d5aed68cb6536f/downloads/DarksidersGenesis_V1.129_STATIC_TARGET_PISTOL_LIFECYCLE_TEST.zip) - [Recu](docs/V1_129_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Correctifs EXPERIMENTAUX, NON VALIDES EN JEU ; V1.128 observee installee, aucune installation V1.129. main reste V1.124.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base V1.128 observee installee. Statique : retirer uniquement la cible assistee initiale du faisceau principal TPS Camera Forward ; conserver rebonds et autres chemins. Pistolets : resolution du composant live dans les setters natifs de modele/visibilite, apparence independante du focus/menu et du TTL des poses. Ancienne politique cachee/gatee remplacee ; animation de bras conservee.

218 valeurs INI / 55 ID Hotkeys inchanges ; autres modules preserves. Trois suites etendues et deux passes d'audit confrontees. [Preuves, limites et essai groupe](docs/V1_129_STATIC_TARGET_PISTOL_LIFECYCLE_TEST.md). GitHub seulement, aucun lancement ni installation ; main reste V1.124, archive et anciens ZIP preserves. Coffre/dash restent non valides ; coop plus tard. Les sections suivantes sont historiques.

---

## V1.128 - clearer and consistent mod menu

**CONSTRUITE / CONTROLEE : [67/67 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40764748962), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6c3abcc6373088e21ba10404406946ba81a46790/downloads/DarksidersGenesis_V1.128_MENU_READABILITY_TEST.zip) - [Recu](docs/V1_128_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Presentation du menu uniquement ; rendu complet a valider en jeu. V1.127 reste installee, INI personnel conserve ; main reste V1.124.**

**CONSTRUITE / CONTROLEE ; MENU COMPLET NON VALIDE EN JEU.** Base V1.127 installee et conservee. Aide courte avec details au survol (?), marges et champs adaptes a la police, listes et valeurs homogenes, groupes Appearance fermes initialement, libelle/contact sol avances clarifies. Parametres, controles et modules de jeu inchanges ;218valeurs INI/55ID Hotkeys conserves. [Perimetre, deux passes et essai](docs/V1_128_MENU_READABILITY_TEST.md).

Compilation uniquement GitHub ; aucun jeu lance ou remplacement V1.128 local. main reste V1.124 validee ; anciens ZIP/archive preserves. Bug de selection du tir souris TPS sur destructible reste ouvert, coffre/dash non declares valides. Les sections suivantes sont historiques.

---

## V1.127 - unified heavy landing and Recovery evidence

**CONSTRUITE / CONTROLEE : [66/66 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40686258358), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/cfc71c93c41ce6c985487e1fa692afe18155bbe4/downloads/DarksidersGenesis_V1.127_RECOVERY_EXPERIMENTS_TEST.zip) - [Recu](docs/V1_127_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Timing reception lourde V1.126 accepte puis fusionne ; conditionnement V1.127 non valide en jeu. Gain coffre/dash et tir TPS souris restent ouverts. main reste en V1.124 ; installation locale ensuite demandee et verifiee, INI conserve ; jeu non lance.**

**CONSTRUITE / CONTROLEE ; NOUVEAU CONDITIONNEMENT NON VALIDE EN JEU.** Base V1.126 : timing personnalise reception lourde accepte par l'utilisateur, fusionne dans la case existante avec ses valeurs. Coffre :6demandes dans le journal ; dash :0. Aucun gain de controle prouve par ces traces. Ajout d'un bilan borne sans changer leurs gardes/mouvement. Visee souris TPS : investigation trace Projectile/objets Overlap et branche du faisceau ciblee ; aucun patch de visee, manette non testee. [Perimetre, limites et essai](docs/V1_127_UNIFIED_LANDING_RECOVERY_TEST.md).

218valeurs INI conservees, cle doublee retraitee a la sauvegarde ;55ID Hotkeys inchanges. Leviathan/apparences/transfer capacites/cameras/controles preserves. main reste en V1.124 ; debug seulement. Installation locale demandee puis verifiee ; pas de relance ni compilation locale. Les sections suivantes sont historiques.

---

## V1.126 - essais Recovery et impact Tremor

**CONSTRUITE / CONTROLEE : [66/66 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40672465858), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8ea941ff3993d970fb89577b4c165ad94a739eeb/downloads/DarksidersGenesis_V1.126_RECOVERY_EXPERIMENTS_TEST.zip) - [Recu](docs/V1_126_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Recovery/Tremor EXPERIMENTAUX, NON VALIDES EN JEU, nouvelles cases OFF. Leviathan et apparences precedentes conserves. main reste en V1.124 validee ; aucune installation locale.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base V1.125 controlee, V1.124 validee reste sur main. A la demande utilisateur, apparences actuelles conservees et Leviathan confirme independant/inchange.

Player movement -> Recovery experiments : temps de reception lourde optionnels et deux essais MOVE natifs apres arret complet observe du montage coffre/dash. Cases OFF par defaut, pas de Finish/Stop, touche imposee, timer/vitesse/animation ecrit ou nouveau hook. Les chemins valides sont conserves quand les essais sont OFF. Ajouter seulement Tremor surface impact (Native/or/metal/pierre/eau/Hidden), via quatre ParticleSystem natifs verifies et la fabrique existante ; pas de refonte des anciens groupes.

Sept cles nouvelles,219valeurs au total ;212anciennes et55ID Hotkeys conserves. Compilation uniquement GitHub, aucune installation/relance locale. [Perimetre, limites et essais](docs/V1_126_RECOVERY_EXPERIMENTS_TEST.md). Les etats suivants sont historiques.

---

## V1.125 - consolidation et carte des temps Recovery

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40665878336), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2dd91999a23acbed02dd5c2b4b2a7aedd6f0a6ac/downloads/DarksidersGenesis_V1.125_CONSOLIDATION_RECOVERY_MAP_TEST.zip) - [Recu](docs/V1_125_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Nettoyage NON VALIDE EN JEU ; pas de correctif Recovery general ni de correction des apparences revendiques. main reste en V1.124 validee. Aucune installation locale.**

**CONSTRUITE / CONTROLEE ; NETTOYAGE NON VALIDE EN JEU.** Base V1.124 validee, maintenant promue sans recompilation sur main a la demande utilisateur. La suite reste sur debug. [Promotion](docs/V1_124_MAIN_PROMOTION.md).

Deux passes d'audit confrontees : copie de telemetrie cheval sans lecteur retiree, adaptateur Clone reduit a son seul receveur War (tableaux a une entree et branches inaccessibles retires). Pas de changement intentionnel des capacites ; Geyser/Stoneskin/Caltrops et autres acquis conserves,212valeurs INI/55ID Hotkeys inchanges. [Audit et limites](docs/V1_125_DOUBLE_AUDIT.md).

Recovery :27assets natifs relus, distinction entre montage/fondu/interruption/fin de capacite. Les fenetres MOVE des receptions lourdes sont0,833373s Strife/0,813425s War ;0,668465/0,402459 concernent d'autres actions. **Code HeavyLanding valide inchange ; aucun correctif Recovery general annonce.** [Carte complete](docs/V1_125_RECOVERY_TIMING_MAP.md).

Retour utilisateur : apparences partiellement sans effet/peu interessantes (11groupes visibilite seulement), Rampage accepte, coop plus tard, FX P2 semblaient bons et HUD general plus reproduit. Aucun nouveau hook ou essai natif, installation/lancement local. Les statuts suivants restent historiques.

---

**28 octobre 2026 : V1.124 / correctif du petit saut final Caltrops War VALIDES par l'utilisateur dans son essai. Base retenue pour la suite ; Geyser/Stoneskin et acquis conserves.** [Validation et limites](docs/V1_124_USER_VALIDATION.md) - [file restante actuelle](docs/ACTIVE_QUEUE.md). Pas de nouvelle build ou installation. Les statuts de preparation/livraison ci-dessous restent historiques et ne remplacent pas ce retour ; le ZIP publie reste immuable.

## V1.124 - Caltrops War, essai natif unique de retour au sol

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40601516701), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c6751b4ef8835f25e9a72cd6c424d4ea833bcff6/downloads/DarksidersGenesis_V1.124_WAR_CALTROPS_GROUND_STATE_TEST.zip) - [Recu](docs/V1_124_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Essai Caltrops EXPERIMENTAL, non valide en jeu ; pas d'installation locale. V1.123 et acquis valides conserves.**

**CONSTRUITE / CONTROLEE ; ESSAI EXPERIMENTAL, NON VALIDE EN JEU.** Base cumulative V1.123 ; Stoneskin Strife/War, Geyser et geste Caltrops acceptes sont conserves. L'utilisateur a rouvert le sujet puis explicitement autorise « Autoriser cet appel unique » le2026-10-28, ce qui remplace la decision anterieure de differer cet essai precis. [Accord et perimetre](docs/V1_124_CALTROPS_GROUND_STATE_REVIEW.md).

Apres un Caltrops War parti du sol, une seule demande publique SetMovementMode(1,0) dans l'ASI vise a eviter la pose aerienne parasite finale. Le moteur gere ensuite normalement sol/gravite ; pas de maintien, second appel ou modification brute de mouvement/animation. Meme joueur/monde, identites vivantes, montage/capacite courants, debut0..0.15s, faible deplacement vertical/horizontal et vitesse verticale quasi nulle obligatoires. Cas inconnu : comportement V1.123 conserve. Cout/degats/pics, geste1.8333s et fondus.10/.18, sons et gardes donneuses restent identiques. [Contrat, objectif et test](docs/V1_124_WAR_CALTROPS_GROUND_STATE_TEST.md).

Pas de nouveau hook, scanner d'objets ou calcul de pose par frame ; pas de changement INI/Hotkeys ni du chemin manette. Lecteur externe en lecture seule, ancien garde VM refuse absent. Compilation uniquement GitHub, aucune installation ou relance locale. Le sursaut reste NON VALIDE jusqu'a essai utilisateur ; coop en dernier.

---

## Decision utilisateur : conserver V1.123 pour le moment

L'utilisateur choisit explicitement « Conserver V1.123 pour l'instant ». L'essai SetMovementMode est mis de cote : aucun patch applique, aucun appel moteur, aucune nouvelle build ou installation. Ne pas relancer cet essai ni redemander son autorisation sans reprise explicite du sujet par l'utilisateur. V1.123 reste la version installee ; Stoneskin/Geyser et le geste Caltrops acceptes sont conserves. Seul le petit saut final Caltrops reste OPEN.

---

## Caltrops War - releve live apres retour V1.123

Deux activations capturees en lecture seule : le personnage ne monte pas en position monde ; sa pose est pourtant marquee en l'air. Mouvement natif **sol -> vol (~1.83s) -> chute (~47ms) -> sol** observe deux fois, avec elevation du bassin du modele. Le sursaut final n'est pas declare corrige. Proposition : une demande publique native de marche, une seule fois au debut d'un Caltrops War parti du sol, avec controles stricts. **Patch refuse avant application par l'approbation automatique : accord humain explicite requis pour cet essai SetMovementMode, distinct des particules Stoneskin.** [Perimetre et risques](docs/V1_124_CALTROPS_GROUND_STATE_REVIEW.md). Aucun nouveau code/build/installation ; Stoneskin valide et Geyser restent inchanges.

---

## Retour utilisateur V1.123 - Stoneskin valide, fin Caltrops ouverte

Stoneskin sur Strife (explosion et cendres incluses) et Stoneskin natif sur War : **VALIDES par l'utilisateur dans les essais effectues**. Caltrops War : geste et sortie des pics acceptes, **petit saut final toujours present** sur la video ; reste a corriger. Geyser et les acquis restent inchanges, coop en dernier. Video/logs examines ; la transition native vol/chute est une cause candidate, pas encore mesuree sur cette execution. Aucun nouveau code/ZIP livre pour ce diagnostic. [Analyse et prochaine etape](docs/V1_123_USER_VALIDATION.md).

---

## V1.123 - Effets Stoneskin et coup de pied Caltrops

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40560388902), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6091b34fdafa759d6b37d75b17ab7ac7cc2b5e97/downloads/DarksidersGenesis_V1.123_STONESKIN_TRANSITIONS_CALTROPS_STOMP_TEST.zip) - [Recu](docs/V1_123_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Installee sur demande avec sauvegarde V1.122 verifiee, INI/PAK conserves ; jeu non lance. Effets Stoneskin et geste Caltrops EXPERIMENTAUX, non valides en jeu. Geyser valide conserve.**

**CONSTRUITE / CONTROLEE ; NOUVEAUTES EXPERIMENTALES, NON VALIDEES EN JEU.** Base cumulative V1.122. Geyser est explicitement VALIDE : aucun changement. Geste Stoneskin1.25s, fondus, materiaux et duree native conserves. Essai visuel limite explicitement autorise : explosion Base/Molten quand le revetement apparait sur Strife, cendres quand le statut expire completement. API publique SpawnEmitterAttached avec contrat72 octets confirme en lecture seule, controle avant chaque appel. Aucun appel/ecriture du lecteur externe.

Caltrops War : geste complet de son Stoneskin (~1.8333s), pics au signal natif du coup de pied, une seule creation. Trois gardes natifs verifies par signatures filtrent seulement les notifications de peau/acteur/blast du montage prive de Caltrops ; le hook de particules existant recoit le signal. War Stoneskin ordinaire est transmis intact. Lifecycle vol/chute natif conserve : absence de rebond a verifier. Ancien chemin de particules qui refusait les effets et getter Caltrops inutilise supprimes. [Contrat et essais](docs/V1_123_STONESKIN_TRANSITIONS_CALTROPS_STOMP_TEST.md), [autorisation et perimetre](docs/V1_123_STONESKIN_FX_REVIEW.md).

Clone/Rampage/WorldEnder, Geyser, materiaux Stoneskin, camera/manette/cheval/menus/updater et INI/Hotkeys conserves. Les nouveaux effets et le geste Caltrops restent EXPERIMENTAUX, NON VALIDES EN JEU. Aucun lancement, compilation ou remplacement du jeu local.

---

## V1.122 - Memes gestes et effets, activations plus courtes

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40545654508), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ee21e5c88a1f0fa8eedd6e2d4206529b86360d97/downloads/DarksidersGenesis_V1.122_SHORT_CASTS_GROUNDED_CALTROPS_TEST.zip) - [Recu](docs/V1_122_DELIVERY_RECEIPT.md). Updater debug et Defender controles. V1.122 installee sur demande avec sauvegarde verifiee de V1.121, INI/PAK conserves, sans lancement du jeu. Gestes raccourcis et Caltrops EXPERIMENTAUX, non valides en jeu.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.121. Precision utilisateur : garder les gestes et les effets choisis pour Geyser/Stoneskin, raccourcir seulement l'activation. V1.121 installee sur demande avec sauvegarde de V1.120 et INI personnel conserve ; jeu non lance par l'agent.

Geyser Strife : meme Aether_Channel_Start et memes effets, geste complet **1.75s au lieu de 2.3667s**. Declenchements des pics et degats conserves exactement (dernier effet 0.71924s, blast 0.48807s). Seule la fin des fenetres d'interruption se ferme au nouvel arret ; leurs debuts restent natifs. Aucun pic supprime ou accelere, aucun maintien de pose ou retour ajoute. Stoneskin Strife : meme geste, memes fondus, particules et materiaux, activation habituelle **1.25s au lieu de 1.50s** ; duree du statut/protection et vie des particules inchangees.

Caltrops War : essai different, un seul War_Channel_End complet a sa vitesse native (~0.6667s), a la place de Start et sans raccord Start/End. Ce geste est deja disponible dans la decouverte bornee ; aucune nouvelle recherche par frame. S'il manque ou expire, V1.121 reste le repli. Spawn, degats, cout et lifecycle physique natifs conserves ; absence du rebond a verifier en jeu. Pas de patch VM/mouvement ou nouvel hook. Clone/Rampage/WorldEnder, War Stoneskin, camera/manette/cheval/menus/updater et INI/Hotkeys gardes. [Contrat et essais](docs/V1_122_SHORT_CASTS_GROUNDED_CALTROPS_TEST.md).

---

## V1.121 - Retour aux gestes continus, particules de lancement Stoneskin

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40542119595), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/eb36a53e2a1bd3280a973b4caa6c4e025063d30f/downloads/DarksidersGenesis_V1.121_NATIVE_CAST_RESET_STONESKIN_FX_TEST.zip) - [Recu](docs/V1_121_DELIVERY_RECEIPT.md). Updater debug V1.121 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Retour Caltrops/Geyser et lancement Stoneskin EXPERIMENTAUX ; activation/materiaux Stoneskin deja valides conserves.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.120, avec retour cible vers les chemins precedant les retouches de duree. Reference acceptee des capacites : V1.112 ; acquis valides plus recents conserves. Dernier verdict utilisateur : Caltrops V1.120 REJETE, retour Geyser REJETE (mouvement vertical), Stoneskin activation/rendu/restauration VALIDES sauf particules de lancement absentes.

Caltrops War retrouve un seul geste sur la duree native du pouvoir (environ 2.333s avant facteur natif), comme avant le raccourcissement V1.108 : retrait du retour War_End et de toute la continuite temporaire du graphe V1.120. Geyser Strife retrouve un seul geste continu sur 2.3667s : suppression de la pose de fin ajoutee, du maintien d'une derniere image et du module de reechantillonnage GeyserStaticPose, y compris son appel dans le hook partage. Les callbacks de degats et fenetres d'interruption natives restent a leurs temps acceptes. Aucun patch de mouvement, saut, vol/chute, input ou script VM. **Le geste pourra etre plus lent ; l'absence de rebond reste a tester.**

Stoneskin garde exactement son activation acceptee (normalement 1.50s), ses fondus et ses materiaux. Les deux notifications natives de particules de lancement Base/Molten sont copiees dans son seul montage prive Strife ; phases et filtres d'affichage conserves, doublons evites. Aucun transfert du blast, du statut War/Stinger, des effets de surface, du camera shake ou des callbacks de gameplay. Un contrat indisponible est signale au journal et laisse le pouvoir accepte fonctionner.

Clone, Rampage, World Ender, War Stoneskin natif, apparences, camera/manette/cheval/menus/updater et reglages personnels preserves. INI 212 cles et 55 Hotkeys inchanges. Compilation uniquement sur GitHub debug, aucun lancement ou installation locale. [Objectif, changements et tests](docs/V1_121_NATIVE_CAST_RESET_STONESKIN_FX_TEST.md).

---

## V1.120 - Continuite de la pose de War pendant Caltrops

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40519850736), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d218ecb22e9f4e3aa2e3ad528775d63e86f0c347/downloads/DarksidersGenesis_V1.120_CALTROPS_SOURCE_POSE_TEST.zip) - [Recu](docs/V1_120_DELIVERY_RECEIPT.md). Updater debug V1.120 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Continuité visuelle Caltrops EXPERIMENTALE, a valider en jeu ; Geyser/Stoneskin +0.50s conserves.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.119 : demi-seconde ajoutee a Geyser et Stoneskin conservee exactement. Precision utilisateur sur Caltrops : surtout un mouvement du corps, sans deplacement reel. Le diagnostic s'oriente donc vers la reprise de la pose, pas vers un vrai saut physique ; le garde VM reste abandonne.

Le TPS Slot du graphe War a bAlwaysUpdateSourcePose=false dans le PAK. L'essai active temporairement cette option native uniquement pour Caltrops transfere sur War : sa pose de fond continue a se mettre a jour sous le geste, puis le reglage d'origine est restaure. Champ exact verifie par reflection, identites faibles du joueur/acteur/mesh/animation/pouvoir et monde controles ; un reglage deja actif reste actif, un contrat inconnu garde le comportement precedent. [Principe documente par Epic](https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/AnimNode_Slot?application_version=4.27).

Le lien entre la pose arretee et le rebond est une **hypothese**, pas un resultat observe en jeu. Aucun nouveau hook, changement de duree, mouvement/vol/chute, script VM, callback, cout, degat ou projectile. Clone/Rampage/World Ender, Native Strife, camera/manette/menus et apparences gardes. INI 212 cles et 55 Hotkeys inchanges. Compilation GitHub, aucune installation/lancement automatique. [Contrat, preuves, tests et rollback](docs/V1_120_CALTROPS_SOURCE_POSE_TEST.md).

---

## V1.119 - Une demi-seconde de plus pour Geyser et Stoneskin

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40513219454), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/38e0bfab00c001975d8d74c83414cfaa27e8cdc2/downloads/DarksidersGenesis_V1.119_LONGER_STRIFE_CASTS_TEST.zip) - [Recu](docs/V1_119_DELIVERY_RECEIPT.md). Updater debug V1.119 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Animations +0.50s EXPERIMENTALES a valider ; Caltrops reste OUVERT et inchange.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.118, installee et verifiee lors de la demande precedente, INI personnel conserve. Dernier retour : ajouter 0.5 seconde aux deux animations ; Caltrops War fait toujours le petit saut, donc son changement de duree V1.118 ne le corrige PAS.

Geyser immobile : retour 0.65 -> **1.15s**, meme debut 0.85s et fondu progressif 0.22s ; fin visuelle 2.00s au lieu de 1.50s, dans le montage natif 2.3667s. Aucun ajout au verrouillage du personnage, aux callbacks, au deplacement ou au saut. En mouvement, le chemin natif est conserve. Stoneskin Strife : activation habituelle 1.00 -> **1.50s** (+0.50s a la duree anterieure), memes fondus 0.16/0.30s, memes materiaux/sons et duree du statut. War Stoneskin reste natif.

Caltrops reste **OUVERT** et inchange dans cette version. Le journal V1.118 confirme le retour War_Start/End actif lors des essais ; allonger le geste n'a pas supprime le saut. Une observation du personnage entier versus sa pose doit departager un vrai decollement et un mouvement visuel. Le lifecycle natif vol/chute est une hypothese de source, pas une mesure en jeu. Le garde VM rejete precedemment reste exclu ; choix utilisateur de la methode simple conserve. Pas de nouveau hook ni nouvelle experience de mouvement.

Clone, Rampage, World Ender, menu Appearance, visee/camera/manette, INI 212 cles et 55 Hotkeys inchanges. Compilation GitHub seulement, pas d'installation ou lancement automatique. [Contrat, criteres et tests](docs/V1_119_LONGER_STRIFE_CASTS_TEST.md).

---

## V1.118 - Transitions Geyser / Stoneskin

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40504809854), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a080bb2202058ab1d3879a29231f8f8a846f66d5/downloads/DarksidersGenesis_V1.118_SMOOTHER_STRIFE_CASTS_TEST.zip) - [Recu](docs/V1_118_DELIVERY_RECEIPT.md). Updater debug V1.118 : choix et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Transitions EXPERIMENTALES a valider ; Caltrops reste OUVERT, garde refuse non inclus.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.117. Dernier retour utilisateur : onglet Appearance present et Clone War OK (VALIDES) ; Geyser et Stoneskin un peu trop abrupts ; petit saut de Caltrops toujours present (OUVERT). Les variantes de la bibliotheque et le rendu mineral precis de Strife ne sont pas explicitement valides par ce retour. Rampage reste valide et intact.

Geyser immobile : meme debut a 0.85s, retour visuel etire de 0.50 a 0.65s, fondu progressif sur 0.22s au lieu du fondu lineaire 0.12s. Racine/courbes actives, deplacement et saut inchanges. Stoneskin : meme geste de 1 seconde, transitions d'entree/sortie 0.16/0.30s au lieu de 0.10/0.18s ; effets, cout, duree du statut et restauration inchanges. Aucun autre changement de gameplay ni nouveau hook.

Caltrops : le journal confirme le retour War_Start/End actif. Le Blueprint natif passe ensuite en vol (5), puis chute (3) a la fin ; c'est une cause candidate du petit saut, pas une transition mesuree en direct. Le garde VM propose a ete refuse par le controle d'approbation automatique pour risque de crash/corruption et absence d'accord explicite pour cette methode. Il n'est ni ajoute ni compile. Le lecteur n a pas trouve le processus du jeu lors du releve en lecture seule ; aucun lancement. L utilisateur demande ensuite la methode la plus simple : Caltrops reprend le meme retour natif 0.55s que Clone valide (total environ 1.12s, contre 0.87s en V1.117 et 2.33s historiquement). Seule cette duree visuelle change ; lifecycle vol/chute intact, aucun garde VM. Clone reste identique. Le saut de Caltrops reste a verifier. [Diagnostic et choix simple retenu](docs/V1_118_CALTROPS_REVIEW.md).

Compilation GitHub seulement, pas d'installation locale ; INI 212 cles, 55 Hotkeys, anciennes archives et main/archive preserves. [Contrat et tests](docs/V1_118_SMOOTHER_STRIFE_CASTS_TEST.md).

---

## V1.117 - Retours d'animation, Stoneskin et onglet Appearance

**CONSTRUITE / CONTROLEE : [65/65 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40494010438), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d5df57ef5cff693e8496e81bcb080baf4faefd3f/downloads/DarksidersGenesis_V1.117_CAST_RETURN_MINERAL_CLOTH_MENU_TEST.zip) - [Recu](docs/V1_117_DELIVERY_RECEIPT.md). Updater debug : V1.117 et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Corrections EXPERIMENTALES, a valider en jeu. Rampage conserve, INI/raccourcis inchanges.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base cumulative V1.116 ; acquis de gameplay retenus V1.112 et validations suivantes. Retour video du 27/10 : Rampage sur Strife parfait, conserve sans changement. Geyser immobile garde une pose apres les pics, Stoneskin trop court et trop en feu, Clone War trop court, Caltrops War fait un bref saut. L'utilisateur confirme ne voir aucun nouveau choix d'apparence ; aucune variante V1.116 n'est donc annoncee validee.

Nouvel onglet **Appearance**, visible directement entre Gameplay et Camera : ouvrir **Effect appearances (experimental)** puis Tools / Powers / Strife ammunition / etc. Les groupes restent fermes par defaut. Les 43 groupes / 451 effets natifs sont conserves ; aucun nouveau skin invente. Models/armures/reticule/Leviathan sont regroupes dans ce meme onglet. [Catalogue des choix](docs/V1_116_EFFECT_LIBRARY_CATALOG.md).

Geyser : retour visuel seulement a l'arret, avec contexte natif prive pour les os et courbes ; le journal V1.116 montrait que les contextes avec courbes etaient refuses. Mouvement/saut/racine/courbes actives et fin native du pouvoir restent intacts. Stoneskin : activation propre de Strife portee de 0.75 a 1 seconde ; corps/gear mineral sans emission uniforme, tissu garde son materiau de feu. Clone/Caltrops sur War : retour natif War_Channel_End ajoute au geste War_Channel_Start, 0.55 / 0.30 seconde ; effets/sons/callbacks existants conserves. Le resultat visuel exact attend l'essai en jeu.

Pas de compilation/installation locale, lancement ni ecriture dans le jeu. INI 212 cles et 55 Hotkeys inchanges. [Contrat, preuves video, criteres et rollback](docs/V1_117_CAST_RETURN_MINERAL_CLOTH_MENU_TEST.md).

---

## V1.116 - Bibliotheque des effets natifs

**CONSTRUITE / CONTROLEE : [64/64 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40473336773), zero avertissement/erreur MSVC. [ZIP public cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/22f011465c0b1eb69bf068a55a4b0fdb3e66e341/downloads/DarksidersGenesis_V1.116_NATIVE_EFFECT_LIBRARY_TEST.zip) - [Recu](docs/V1_116_DELIVERY_RECEIPT.md). Updater debug V1.116 : selection et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Bibliotheque EXPERIMENTALE, NON VALIDEE EN JEU ; V1.114/115 restent en attente du retour utilisateur.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** L'utilisateur demande toutes les autres familles ; V1.114 et V1.115 restent en attente de son essai. Base de gameplay retenue V1.112 ; V1.113 reste REJETEE. Aucun nouveau changement de visée, dash, controle, animation ou revetement Stoneskin dans ce lot.

Appearance > Effect appearances : **43 groupes / 451 ParticleSystem natifs / 98 roles visuels** verifies dans le PAK principal. Outils, pouvoirs, munitions Strife, armes War, montures/formes et coeurs. Variantes nommees seulement lorsqu'un asset natif du meme role existe ; **Native / Hidden** pour les familles sans autre habillage prouve. Chaque groupe est independant, toutes les nouvelles options sont Native et les categories sont fermees par defaut. [Liste exacte des choix](docs/V1_116_EFFECT_LIBRARY_CATALOG.md).

Un seul moteur et le hook visuel deja utilise en V1.115 remplacent le prototype Caltrops, sans doublon ni ancien module compile. Templates a la creation uniquement ; Hidden garde le composant et son lifecycle natifs, masque le rendu puis restaure uniquement la visibilite modifiee. Pas de nouveau hook de combat, classe de projectile/coeur, degat, cout, statut, sauvegarde, modele ou materiau remplace. Les slots Caltrops 0..4 et les 170 anciens reglages restent identiques ; 42 nouvelles cles Native, 212 au total, 55 ID Hotkeys inchanges.

Assets deja charges uniquement : une variante manquante garde le rendu natif. Les effets deja presents ou les composants crees directement hors du point de creation commun peuvent garder leur apparence ; ce lot ne pretend pas remplacer toutes les textures du jeu. Leviathan, Stinger, armures, pistolets/bras/holsters et corps Stoneskin gardent leurs options et chemins precedents. Les formes/montures changent ici seulement leurs effets, pas leurs modeles. [Contrat, preuves, tests et rollback](docs/V1_116_NATIVE_EFFECT_LIBRARY_TEST.md).

---

## V1.115 - Premier choix d'apparence Caltrops

**CONSTRUITE / CONTROLEE : [64/64 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40452540269), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/979ce9420809716c83f6ab7c17074592746db481/downloads/DarksidersGenesis_V1.115_CALTROPS_BURST_APPEARANCE_TEST.zip) - [Recu](docs/V1_115_DELIVERY_RECEIPT.md). Updater debug : V1.115 selectionnee et octets verifies. Defender ZIP/quatre fichiers : aucune menace detectee, protections actives. Apparences EXPERIMENTALES, NON VALIDEES EN JEU ; V1.114 reste egalement en attente du retour utilisateur.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** L'utilisateur demande de poursuivre les skins et remet son essai V1.114 a plus tard : **V1.114 n'est pas validee**. Base retenue de gameplay V1.112, experience V1.114 conservee sans modification dans ce cumul. V1.113 reste REJETEE. Rampage Strife / trois pouvoirs sur War / expiration Stoneskin gardent leur validation precedente.

Menu Gameplay > apparences > Ability appearances : **Caltrops Native / Base / Frost 1 / Frost 2 / Frost 3**. Ce premier choix agit sur le **glyphe d'apparition et l'explosion des nouveaux effets**. Modele au sol, effet d'attente et glyphe en boucle restent natifs. D'autres familles restent au catalogue, pas annoncees comme selectionnables. Aucun coeur, degat, rayon, cout, statut, projectile ou progression n'est remplace.

Une substitution du seul template de particules a la creation, sur six references exactes du catalogue, dans le monde local courant. Pas de modification des CDO, materiaux, composants existants ni sauvegardes. Native est le defaut et restaure les prochains effets. Assets deja charges uniquement ; un effet absent garde son rendu natif. Pas de chargement synchrone ou parcours d'assets au demarrage avec Native. Groupe ferme par defaut.

170 cles INI (169 preservees + CaltropsAppearance=0), 55 ID Hotkeys inchanges. Anciennes archives et main/archive conserves. Compilation exclusivement GitHub ; installation via updater debug par l'utilisateur. [Contrat, limites, tests et rollback](docs/V1_115_CALTROPS_BURST_APPEARANCE_TEST.md).

---

## V1.114 - Reprise V1.112, Geyser statique et Stoneskin

**CONSTRUITE / CONTROLEE : [63/63 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40445567125), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4ee6f649835353de4e369d03b63100b66112c967/downloads/DarksidersGenesis_V1.114_RETAINED_CASTS_STATIC_GEYSER_FIRE_TEST.zip) - [Recu](docs/V1_114_DELIVERY_RECEIPT.md). Updater debug : V1.114 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP et les quatre fichiers, protections actives. EXPERIMENTALE, NON VALIDEE EN JEU. V1.113 REJETEE, base retenue V1.112.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** V1.113 REJETEE : le saut reste bloque apres Geyser, meme apres la fin des pics ; son geste Stoneskin et le rendu corps/tissu ne correspondent pas a la demande. Retour complet du constructeur de montage V1.112, sans les segments ni nouveaux etats d'interruption V1.113. Rampage Strife, trois pouvoirs Strife sur War et expiration Stoneskin restent VALIDES par l'utilisateur.

Geyser : gameplay et montage V1.112 conserves ; seulement a l'arret, presentation de la vraie fin Aether via l'evaluation visuelle existante. Aucune ecriture de deplacement, saut, commande, duree, evenement ou aptitude. Mouvement, intention de mouvement et chute conservent la pose native. Une courbe non prise en charge refuse l'ajustement visuel ; le gameplay reste V1.112.

Stoneskin : geste Aether V1.112 raccourci a 0,75 s a vitesse normale, statut et expiration independants. Materiaux prives heritant des vrais shaders natifs A pour corps/equipement et B pour tissu, base/Molten selon le statut : ne supprime plus les regions en feu. **Le placement final des textures War sur les UV de Strife reste experimental.**

169 cles INI / 55 ID Hotkeys, camera, XInput, tirs et anciennes archives preserves. Coop en dernier. Compilation exclusivement GitHub ; installation manuelle/updater debug par l'utilisateur. [Contrat, limites, tests et rollback](docs/V1_114_RETAINED_CASTS_STATIC_GEYSER_FIRE_TEST.md).

---

## V1.113 - Retours au sol Strife et roles Stoneskin

**CONSTRUITE / CONTROLEE : [62/62 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40410419216), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/84583fc7f21b497a747733ddb3f1ea923fa3c6c9/downloads/DarksidersGenesis_V1.113_GROUNDED_GEYSER_STONESKIN_ROLES_TEST.zip) - [Recu](docs/V1_113_DELIVERY_RECEIPT.md). Updater debug : V1.113 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Nouveaux ajustements EXPERIMENTAUX, NON VALIDES EN JEU.**

**PREPAREE / EXPERIMENTALE, NON VALIDEE EN JEU. Compilation et livraison GitHub controlees, essai en jeu requis.** Base V1.112. Retours utilisateur : fin Rampage Strife, trois pouvoirs de Strife sur War et expiration/restauration Stoneskin Strife explicitement VALIDES. Ces chemins retenus restent preserves. Restent la levitation Geyser a l'arret, l'animation Stoneskin trop longue avec glissement, et le corps rouge/tissu pierre.

Geyser garde son debut accepte puis joue son vrai retour Aether au sol a vitesse native, au lieu de tenir sa pose suspendue ; pic/cout/duree et commandes inchanges. Stoneskin reprend le geste sol Strife et sa fin a vitesse native, avec fenetres d'interruption natives copiees sur des etats prives : MOVE a 0,942 s. Duree/cout/statut/sons restent natifs, pas de Finish anticipe ni touche simulee.

Le revetement vise les vrais slots Strife_Body / Strife_Gear, garde les deux tissus natifs, et coupe sur ses seuls materiaux prives l'emission rouge dont le masque UV est propre a War. Expiration/restauration retenues, refus mis en cache pour eviter de recreer un materiau a chaque image. **Le tissu conserve son apparence native ; le rendu Molten de la cape de War n'est pas reproduit a l'identique sur Strife.**

169 cles INI / 55 ID Hotkeys, camera, input/XInput, tirs, fonctions validees et anciennes archives conserves. P2 toujours differe. Compilation uniquement GitHub, installation par l'utilisateur via updater debug. [Contrat, preuves, essais et rollback](docs/V1_113_GROUNDED_GEYSER_STONESKIN_ROLES_TEST.md).

---

## V1.112 - Fins des pouvoirs et revetement Stoneskin Strife

**CONSTRUITE / CONTROLEE : [62/62 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40402937664), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2b609415cffaff77a99fc1615cf8ba51c3574813/downloads/DarksidersGenesis_V1.112_POWER_BLENDS_STRIFE_STONESKIN_TEST.zip) - [Recu](docs/V1_112_DELIVERY_RECEIPT.md). Updater debug : V1.112 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

**EXPERIMENTALE / NON VALIDEE EN JEU. Compilation et livraison GitHub controlees ; essai en jeu requis.** Base V1.111, avec les acquis explicitement valides : effets WingClip V1.110, sons/vitesses des pouvoirs, flash Lava et fonctions precedentes. Le retour V1.111 signale des fins abruptes pour Rampage/Geyser Strife et les trois pouvoirs de Strife sur War ; cela reste le critere de validation du nouveau lot.

Geyser et World Ender gardent une pose continue jusqu'au fondu, sans saut vers un segment idle. Les copies privees des montages conservent leurs fondus plus longs et ont un minimum de sortie de 0,18 s ; Caltrops et Stoneskin utilisent aussi cette sortie douce. Rampage suit le poids et la position de son vrai montage pendant le fondu, meme apres la fin de la capacite, au lieu d'arreter brutalement la pose. Mouvement, durees, evenements de degats/cout et fenetres de reprise des commandes conserves.

Capture Stoneskin : War recoit ses materiaux natifs, Strife recoit les memes statuts sans revetement. Nouveau candidat : quatre materiaux dynamiques prives pour le corps/equipement/tissu de Strife, parametres natifs Stoneskin ou Molten selon les vrais statuts. Les materiaux d'origine sont gardes par le parent du materiau prive, puis restaures a l'expiration ou transformation ; un remplacement natif gagne. War, Stinger, statistiques et durees restent natifs. **Compatibilite visuelle des textures de War sur Strife a verifier en jeu.**

169 cles INI / 55 ID Hotkeys conserves. Aucun changement de camera, tirs, input/XInput, reception lourde ou reglages. P2 toujours differe. Compilation uniquement GitHub ; installation manuelle/updater debug par l'utilisateur. Anciennes archives preservees. [Preuves, limites, essais et rollback](docs/V1_112_POWER_BLENDS_STRIFE_STONESKIN_TEST.md).

---

## V1.111 - Repere Rampage et rythme des gestes natifs

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40363876283), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0f1a5d47ac05b67ed950d9338fe06d8b7c38a3d0/downloads/DarksidersGenesis_V1.111_NATIVE_POWER_TIMING_FRAME_TEST.zip) - [Recu](docs/V1_111_DELIVERY_RECEIPT.md). Updater debug : V1.111 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

**EXPERIMENTALE : compilation, controles et livraison GitHub verifies ; NON VALIDEE EN JEU.** Base V1.110, effets du coup tournoyant explicitement VALIDES par l'utilisateur, sons/vitesses des autres pouvoirs et flash Lava conserves. Retour V1.110 : bouclier Rampage present mais tourne de 90 degres a droite dans les deux vues ; fin nettement meilleure mais court arret ; geste Geyser Strife mal synchronise, pas une demande de raccourcir seulement le blocage final. Degats de charge non confirmes.

Le bouclier prive reprend la rotation du vrai socket War VFX_Root lue dans le squelette natif ; aucun angle de camera compense. Les fenetres natives d'interruption des fins Rampage et de Geyser sont conservees sur le montage prive, avec debut/fin exacts. Geyser joue le geste propre Strife a sa vitesse d'origine puis revient a sa pose d'attente : fini l'etirement uniforme d'une seconde sur 2,37 secondes. Temps de SpawnBlast, effets, cout et duree du montage restent natifs. Aucun Finish anticipe, timer ou nouveau hook.

Acquis du coup tournoyant, tirs, camera, XInput et reception lourde inchanges ; 169 cles INI / 55 ID Hotkeys conserves. P2 differe. Compilation uniquement GitHub, updater debug et ZIP quatre fichiers NoCompression ; aucune installation locale. [Preuves, limites, essais et rollback](docs/V1_111_NATIVE_POWER_TIMING_FRAME_TEST.md).

---

## V1.110 - Charge Rampage, sortie de pose et coup tournoyant

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40356864172), zero avertissement/erreur MSVC. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b43590e2f481e48ece13cd321c568001f26a4846/downloads/DarksidersGenesis_V1.110_NATIVE_CHARGE_WINGCLIP_TEST.zip) - [Recu](docs/V1_110_DELIVERY_RECEIPT.md). Updater debug : V1.110 selectionnee, octets verifies. Defender : aucune menace detectee dans le ZIP final et les quatre fichiers, protections actives. Corrections EXPERIMENTALES, NON VALIDEES EN JEU.**

**EXPERIMENTALE : compilation, controles et livraison GitHub verifies ; NON VALIDEE EN JEU.** Base fonctionnelle V1.103, acquis precedents conserves ; sur V1.109 l'utilisateur valide les vitesses/sons des pouvoirs transferes et le flash Lava au canon. Demarrage juge moins lent. Charge Rampage (degats/effet), petit mouvement apres Talon du Diable et effets du coup tournoyant restent ouverts.

Capture groupee V1.109 : deux Rampages War et deux Strife, puis les quatre configurations de lames. Le coup tournoyant est WingClip, pas Kick Launch. Ses arcs sont reduits a 40 % et rapproches de leur main native uniquement si sa lame est OFF ; meme chemin visuel que les attaques deja acceptees. Rampage reprend neuf evenements natifs de charge sur son montage prive (blast, six effets de sol, bouclier temporel et collision), avec les classes/temps/durees du jeu. Deux hooks de charge retires. Le suivi de pose cesse lorsque le vrai montage se termine, meme si la capacite reste active environ 0,25 s.

Sons, vitesses, flash Lava, trajectoires, stats/chiffres de degats, inputs/camera et reception lourde valides conserves ; 169 cles INI, 55 ID Hotkeys, aucune nouvelle option. P2 differe. Les degats de charge n'ont pas ete compares sur des ennemis pendant le releve : ils restent a tester. Compilation uniquement GitHub, updater debug et ZIP quatre fichiers NoCompression ; aucune installation locale. [Preuves, limites, essais et rollback](docs/V1_110_NATIVE_CHARGE_WINGCLIP_TEST.md).

---

## V1.109 - Livraison ZIP sans compression

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40346271897), zero avertissement/erreur. [ZIP public immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/24cf9a9ecd44c67ff69be88b1149e3d5b7f1b675/downloads/DarksidersGenesis_V1.109_TRANSPARENT_ZIP_DELIVERY_TEST.zip) - [Recu](docs/V1_109_DELIVERY_RECEIPT.md). Defender : aucune menace detectee dans le ZIP final et les quatre fichiers extraits ; protections actives. Updater debug : V1.109 selectionnee et octets verifies. Code de jeu V1.108 conserve, toujours EXPERIMENTAL / NON VALIDE EN JEU.**

**EXPERIMENTALE : recompilation, livraison GitHub et scan Defender verifies. Code de jeu V1.108 conserve ; NON VALIDE EN JEU.** V1.108 est disponible sur GitHub mais Defender a bloque son ZIP avec Trojan:Script/Ulthar.A!ml. Le controle sur les quatre fichiers extraits ne detecte rien ; un ZIP NoCompression contenant les memes fichiers verifies par SHA256 passe aussi le controle. C'est un constat sur ce moteur/signatures, pas une certification generale ni un avis Microsoft de faux positif.

V1.109 change uniquement l'identite de version et le conditionnement ZIP NoCompression, avec comparaison de chaque fichier archive a la sortie compilee. Aucun nouveau correctif de jeu, option, touche ou changement des protections antivirus. 169 cles INI / 55 ID Hotkeys conserves. Les 173 anciennes archives restent dans GitHub ; l'antivirus a mis la copie locale de V1.108 en quarantaine, elle n'est ni restauree ni supprimee de l'historique. [Incident, preuves et controles](docs/V1_109_TRANSPARENT_ZIP_DELIVERY_TEST.md).

---

## V1.108 - Rampage, presentation des pouvoirs, Lava et decouverte bornee

**CONSTRUITE / CONTROLEE : [61/61 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40313742351), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4d27f93e3db5d829adf268bb07e58957cdaf51cf/downloads/DarksidersGenesis_V1.108_RAMPAGE_LAVA_POWER_PRESENTATION_TEST.zip) - [Recu](docs/V1_108_DELIVERY_RECEIPT.md). Listing public updater debug : V1.108 selectionnee, octets verifies. Correctifs EXPERIMENTAUX, NON VALIDES EN JEU.**

**EXPERIMENTALE : compilation Windows x64 GitHub, controles et livraison verifies ; NON VALIDEE EN JEU.** Base fonctionnelle validee V1.103, code de transition V1.107 conserve. Les nouveaux essais V1.107 montrent des phases Start/Slam repetables ; l'utilisateur confirme encore absence des degats/effets de charge, mauvaises poses, lenteur des transferts et sons manquants. Cela ne valide pas tout Rampage.

Lot : attaches natives de la charge Rampage pour Strife, pose propre Strife_Dash puis Talon du Diable (AirKick1), sans remplacer le mouvement natif ; temps du geste War corrige pour Clone/Caltrops et preparation/retour World Ender, avec intervalle de degats du rayon conserve ; son de lancement valide avant le detour partage du dash et boucle sonore native du laser restauree. Flash Lava normal rattache au vrai canon correspondant, sans changement des projectiles/trajectoires. Arc de l'attaque secondaire reconnu pour les lames OFF. Trois scans d'objets remplaces par un seul, au plus 256 entrees et budget haute resolution de 1 ms controle toutes les 16 entrees.

169 cles INI et 55 ID Hotkeys conserves, aucune nouvelle option ni touche. Chemins input/camera, reception lourde, autres acquis et 172 anciennes archives gardes ; main/archive/v1.43 intacts. P2 reste differe. Voir [preuves, limites et essais](docs/V1_108_RAMPAGE_LAVA_POWER_PRESENTATION_TEST.md). Telechargement depuis l'updater du jeu, canal debug ; aucun build ou remplacement local par l'agent.

---

## V1.107 - Rampage : conserver la transition native depart vers frappe

**CONSTRUITE / CONTROLEE : [60/60 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40225339622), zero avertissement/erreur MSVC. [ZIP GitHub immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/52cf0bc7939f65200ce17b978fd10e19109d0371/downloads/DarksidersGenesis_V1.107_RAMPAGE_NATIVE_PHASE_HANDOFF_TEST.zip) - [Recu](docs/V1_107_DELIVERY_RECEIPT.md). Transition Rampage EXPERIMENTALE, NON VALIDEE EN JEU. Disponible pour l'updater canal debug, compilation uniquement GitHub. Audio V1.106 inchange, essai utilisateur differe.**

**EXPERIMENTALE, compilation et livraison GitHub controlees, essai en jeu requis ; NON VALIDEE EN JEU.** Base fonctionnelle V1.103 validee ; acquis conserves. Retour V1.106 : deplacement Rampage puis commandes bloquees, candidat REJETE pour cet essai. Sons V1.106 : essai utilisateur differe, adaptation audio conservee sans modification.

Journal : CAST depart puis, 413 ms plus tard, Slam3 refuse comme orphelin. Observe supprimait l'association des que Start n'etait plus actif, avant la creation native de la frappe ; le compagnon pouvait aussi consommer un token avant toute phase de fin. V1.107 conserve un token faible propre acteur/monde, exige Start avant une seule phase de fin et consomme le token au compagnon final ; un compagnon precoce reste neutralise sans effacer Start. Aucun ancien objet de capacite n'est conserve ou appele pour autoriser la transition. Pas de timer, nouvel input ou nouveau hook.

La copie privee des notifies est reconstruite depuis les evenements source, comme les autres transferts retenus ; elle ne doit pas etre identique octet par octet a une copie native qui peut instancier ses notifies. Les sections acceptent uniquement le relien natif vers leur copie ; boucles/temps/autres champs restent compares, tableaux partages toujours refuses. Mouvement natif et vitesse 1,5 du depart conserves.

Suggestion d'animation etudiee : Strife_Dash, Strife_AirDash_Fall et Strife_AirDash_Land_Still existent dans le PAK original. **Leur adaptation visuelle n'est pas incluse** : leur remplacement direct supprimerait la piste de mouvement Rampage ou ajouterait les callbacks du dash. La pose actuelle reste la locomotion de Strife ; cette version cible le blocage et la sortie du pouvoir. [Contrat, preuves et suite](docs/V1_107_RAMPAGE_NATIVE_PHASE_HANDOFF_TEST.md).

Compilation uniquement GitHub Actions, updater canal debug ; aucune installation locale. 169 cles INI, 55 ID Hotkeys, reception lourde et chemins input/camera inchanges ; 171 anciennes archives preservees. Test : Rampage Strife native/TPS, relachement puis marche/attaque et second lancement, ensuite obstacle ; verifier que le journal atteint la phase de fin sans refus orphelin. P2 differe.

---

## V1.106 - Rampage et sons transferts, plan audio unique

**CONSTRUITE / CONTROLEE : [60/60 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40216944702), zero avertissement/erreur MSVC. [ZIP GitHub immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/a8f88bdd9d5771085d4cddf838b7384d03eb24ef/downloads/DarksidersGenesis_V1.106_RAMPAGE_FOREIGN_AUDIO_COMPACT_TEST.zip) - [Recu](docs/V1_106_DELIVERY_RECEIPT.md). Rampage et audio V1.106 restent EXPERIMENTAUX, NON VALIDES EN JEU. Disponible pour l'updater du jeu, canal debug ; compilation uniquement GitHub, aucune installation locale.**

**EXPERIMENTALE, compilation GitHub et livraison controlees, essai en jeu requis ; NON VALIDEE EN JEU.** Base fonctionnelle validee V1.103. Ce lot cumule la correction mesuree Rampage (vitesse native du depart 1,5) et la reprise des sons des pouvoirs transferee de V1.105. Regles, couts, degats et autres acquis conserves.

V1.105 a passe 60/60 suites Windows, mais MSVC a signale C6262 : CopyLaterSounds utilisait 18 828 octets de pile en dupliquant son plan d'evenements. V1.106 compacte et trie le meme plan en place ; pas de deuxieme plan, nouveau timer ou etat de cast. Le test couvre aussi deux sons issus des deux chronologies source, conserves et tries. Le nom du workflow devient generique, l'identite du ZIP continue de venir du BuildInfo compile. [Recu V1.105](docs/V1_105_DELIVERY_RECEIPT.md).

La cause du grisement V1.104 est un abandon natif apres refus du montage de depart a 1,5 par le garde qui exigeait 1. Cette constante est corrigee, sans forcer une autre vitesse. Sons de lancement via le notify sonore natif sur le receveur apres succes ; sons plus tardifs Caltrops/Stoneskin copies sur leur geste prive. Les risques audio de boucle/doublon et le cycle Rampage complet restent a valider. [Preuves, limites et essais](docs/V1_106_RAMPAGE_FOREIGN_AUDIO_COMPACT_TEST.md).

Compilation uniquement GitHub Actions ; telechargement depuis l'updater du jeu canal debug. Aucune installation locale. 169 cles INI, 55 ID Hotkeys et reception lourde validee inchanges ; 170 anciennes archives preservees. Test groupe : Rampage Strife native/TPS, relachement/obstacle/deuxieme lancement ; puis sons Geyser/Stoneskin Strife et Clone/Caltrops/World Ender War, sans doublon ni son qui persiste. P2 differe, pose Rampage locomotion Strife.

---

## V1.105 - Rampage : vitesse native, sons des capacites transferees

**EXPERIMENTALE, compilation GitHub et livraison controlees, essai en jeu requis ; NON VALIDEE EN JEU.** Base validee V1.103 conservee. Retour V1.104 : Rampage Strife ne se lance pas puis devient grise ; candidat REJETE dans cet essai. Sons des capacites transferees signales absents.

Le journal atteint bien le lancement, puis le constructeur refuse le montage. Lecture du jeu actuel : War_Atk_Rampage_Start_Montage a RateScale=1,5, alors que le garde V1.104 exigeait 1. Les quatre fins sont a 1. V1.105 exige ces valeurs mesurees et les conserve dans la copie ; aucun ralentissement force, vitesse du personnage ecrite ou retrait des controles de proprietaire. Les refus de copie/events sont maintenant distingues dans le journal.

Audio : les poses propres Caltrops/Stoneskin perdaient la piste sonore source. Geyser/Clone/World Ender copiaient deja des sons, tous leurs sons de lancement releves etant a t=0. Leur non-audibilite exacte n'a pas ete tracee dans le mixeur. Le candidat appelle une fois le notify sonore natif d'origine apres lancement confirme, sur le mesh propre du receveur ; ces sons sont exclus des pistes privees Geyser/Clone/World Ender pour eviter une double lecture. Les sons ulterieurs de Caltrops/Stoneskin sont copies sur leur geste prive avec le meme instant reel. Cue, voix, volume, pitch, attenuation et mixer restent natifs. Pas de nouveau hook, timer, touche imposee ou fichier audio ajoute.

Reglages, raccourcis, reception lourde validee et comportements des pouvoirs natifs inchanges. Debug uniquement ; 169 cles INI, 55 ID Hotkeys, 169 anciennes archives preservees. Compilation exclusivement GitHub Actions, telechargement via l'updater du jeu canal debug ; aucune compilation/installation locale. [Contrat, limites et essais](docs/V1_105_RAMPAGE_NATIVE_RATE_FOREIGN_AUDIO_TEST.md).

Essais groupes : Rampage Strife en native/TPS (deplacement, degats, relachement, obstacle, deuxieme cast), puis sons Geyser/Stoneskin Strife et Clone/Caltrops/World Ender War. Verifier sons de lancement, sons pendant/apres effet, absence de doublon et pouvoirs natifs inchanges. P2 reste differe. La pose Rampage reste la locomotion Strife, sans charge/frappe dediee.

---

## V1.104 - Rampage Strife, refus identifie et identites natives

**CONSTRUITE / CONTROLEE : [59/59 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40203015804), zero avertissement/erreur MSVC. [ZIP GitHub immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/acd4ba2cf79577a3c8aa9b8cf09e40f4eee2193b/downloads/DarksidersGenesis_V1.104_STRIFE_RAMPAGE_NATIVE_IDENTITY_TEST.zip) - [Recu](docs/V1_104_DELIVERY_RECEIPT.md). Rampage Strife reste EXPERIMENTAL, NON VALIDE EN JEU. Compilation exclusivement sur GitHub Actions ; archive disponible pour l'updater du jeu, canal debug. Aucune compilation ni installation locale.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; essai en jeu requis.** Base V1.103 reception lourde validee par l'utilisateur, avec Stoneskin/police V1.102 et Geyser/Clone/Caltrops V1.98/World Ender War V1.100 conserves. Priorite explicite : Rampage sur Strife.

Le releve actuel en lecture seule reproduit le refus de l'ancien garde : il exigeait le texte Upperbody_Flinches alors que le FName charge s'affiche UpperBody_Flinches. Les cinq classes de phases sont chargees ; squelette/racine/idle et proprietaires corrects. Le nouveau garde compare les vrais couples index/numero des cinq slots, verifie leur unicite et refuse le slot de mouvement s'il est rendu. Aucun changement global des comparaisons de noms.

Rampage est de nouveau selectionnable dans les trois emplacements de Strife : mouvement/segments/boucle/collisions et fins natifs, montages prives, pose de locomotion propre. Mode moteur 3 verifie avant de construire : il extrait ce mouvement sans le ponderer par un slot de rendu absent. Pas de pose osseuse War imposee a Strife, vitesse manuelle, fin par timer ou touche simulee. Refus/abandon journalises et natifs ; icone/classe du slot conservee si indisponible. **Mouvement effectif, degats, relachement, collisions et fin restent a tester.**

169 cles INI, 55 ID Hotkeys et defauts inchanges ; 168 anciennes archives preservees ; debug seulement. Installation manuelle jeu ferme, garder son INI/raccourcis/PAK/sauvegardes. [Contrat, preuves, limites et essais](docs/V1_104_STRIFE_RAMPAGE_NATIVE_IDENTITY_TEST.md). P2 et effets P2 restent differes.

Test : Strife a pied, Rampage sur un emplacement debloque, essai en espace libre puis face a un obstacle en vue native et TPS ; verifier deplacement/degats, relachement, reprise et deuxieme lancement. Revenir a Native si anomalie. Ne pas confondre READY avec CAST dans le journal.

---

## Retour utilisateur V1.103 - reception lourde validee, priorite Rampage

25 octobre 2026 : l'utilisateur confirme « 1. oui bravo ca marche / 2. oui » pour les deux points d'essai de V1.103. **Reception lourde VALIDEE dans ses essais rapportes**, sans inventer de mesures ou de validation P2. Conserver ce chemin, Stoneskin/police V1.102 et les transferts deja acceptes. Priorite explicite : poursuivre **Rampage sur Strife**, pas un autre sujet. Aucun nouveau build pour cette consignation.

---

## V1.103 - reception lourde par interruption native du mouvement

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40199153318), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/74cb774b480f1cfb5cd2ed6e8d8a15ccd0fd39ec/downloads/DarksidersGenesis_V1.103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.zip) - [Recu](docs/V1_103_DELIVERY_RECEIPT.md). Nouvelle reception lourde EXPERIMENTALE, NON VALIDEE EN JEU ; base V1.102 validee pour Stoneskin et police.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; essai en jeu requis.** Base V1.102 : Stoneskin et taille de police explicitement VALIDES par l'utilisateur ; Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. Rampage Strife V1.100/V1.101 reste rejete et retire.

La reception lourde apres une chute utilise maintenant sa vraie fenetre native d'interruption MOVE, ouverte apres l'impact (0,15 s et sol continu 50 ms). Le moteur decide de la fin du pouvoir, du blend de l'animation et de la reprise lorsque le joueur se deplace. L'ancien correctif qui ecrivait AllowPlayerMovement et le bit d'action MOVE est retire. Pas de fin forcee, simulation de touche, changement des dashs ou ecriture de vitesse. La case existante reste OFF par defaut ; les raccourcis reaffectes suivent l'action native.

169 cles INI, 55 ID Hotkeys et anciens reglages inchanges ; 167 anciennes archives preservees ; debug uniquement. Installation manuelle jeu ferme en conservant son INI/raccourcis/PAK/sauvegardes. [Contrat, preuves, limites et essais](docs/V1_103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.md). Le transfert Rampage demande encore un releve du refus exact ; aucun candidat aveugle reactive. P2 reste a tester plus tard.

Essai groupe : Strife puis War, meme chute en vue native et TPS, case OFF puis ON ; maintenir le deplacement a la reception et comparer la reprise, refaire sans bouger (animation/impact natifs), puis verifier dash et attaques. Remettre OFF si le rendu ou les commandes regressent.

---

## Retour utilisateur V1.102 - Stoneskin et police valides

25 octobre 2026 : « stoneskin valide / taille police ok ». **Stoneskin et taille de police VALIDES par l'utilisateur dans ses essais.** Conserver ces chemins et leurs limites documentees ; aucune nouvelle mesure de degats, variante ou validation P2 inventee. World Ender War V1.100 et Geyser/Clone/Caltrops V1.98 restent retenus. Base de reprise : V1.102 avec ces acquis. Rampage Strife V1.100/V1.101 reste rejete et retire ; reception lourde toujours ouverte. Le refus d'icone indisponible n'a pas de validation visuelle distincte rapportee. Pas de nouveau build pour cette consignation.

---

## V1.102 - Stoneskin pour les deux personnages et police du menu

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40194346246), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/00581c2daeb87a192b99d2bfbac75ea1b36557c8/downloads/DarksidersGenesis_V1.102_STONESKIN_MENU_FONT_TEST.zip) - [Recu](docs/V1_102_DELIVERY_RECEIPT.md). Stoneskin Strife et police EXPERIMENTAUX, NON VALIDES EN JEU ; acquis V1.98 et World Ender War V1.100 valides conserves.**

**EXPERIMENTALE ; compilation et livraison controlees ; test utilisateur requis.** Base retenue : acquis V1.98 et World Ender sur War V1.100, explicitement confirme par l'utilisateur. Geyser/Clone/Caltrops toujours valides. Rampage sur Strife V1.100/V1.101 REJETE : icone de capacite disparue et comportement anormal. Ce chemin, ses montages, sa reflexion et ses tests specifiques sont retires de l'ASI ; une ancienne selection Strife=5 revient a Native. Rampage propre de War conserve.

Stoneskin est selectionnable dans chacun des trois emplacements des deux personnages : chemin natif sur War ; sur Strife, statut/cout/duree natifs et geste compatible avec son propre squelette. Transfert Strife EXPERIMENTAL et partiel : materiau de pierre War, onde de choc du montage et effets d'arme non copies. Stinger reste natif, sans reattache ou correction visuelle.

About → Menu appearance : taille de police du menu de 10 a 32 px, saisie exacte et Default 13 px, enregistree dans [Overlay] FontSize. Curseur et fenetres du mod gardent leur fonctionnement ; autres overlays et jeu hors du scope de cette taille. INI personnel conserve, cle absente = taille d'origine.

Refus d'un transfert indisponible : le resolveur garde la classe/icone et le debloquage natifs du slot, puis bloque seulement son utilisation ; il ne renvoie plus une classe vide. Une capacite encore verrouillee reste verrouillee. Ce changement traite un defaut confirme du code, sans attribuer une cause runtime non capturee a Rampage.

169 cles INI (une ajout), 55 ID Hotkeys et anciens reglages preserves ; 166 archives precedentes intactes, debug seulement. Installation manuelle jeu ferme, garder INI/raccourcis/PAK/sauvegardes. P2 reporte. [Contrat, preuves, essais et repli](docs/V1_102_STONESKIN_MENU_FONT_TEST.md).

Test groupe : police et reouverture du menu ; Stoneskin Strife/War (cout, protection et expiration, fin du cast/retour Native) ; controle de l'icone apres ancienne selection Rampage et bref controle des quatre transferts valides. Aucun nouveau resultat en jeu annonce.

---

## V1.101 - lot Rampage Strife / rayon War, controle des joueurs explicite

**CONSTRUITE / CONTROLEE : [58/58 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40191183951), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/417e6047dd3857850e6f55dd82cbdb83c0470a20/downloads/DarksidersGenesis_V1.101_STRIFE_RAMPAGE_WAR_BEAM_TEST.zip) - [Recu](docs/V1_101_DELIVERY_RECEIPT.md). Corrections War et Rampage Strife EXPERIMENTALES, NON VALIDEES EN JEU ; base V1.98 validee.**

**EXPERIMENTALE, NON VALIDEE EN JEU ; compilation et livraison controlees ; validation en jeu requise.** Base validee V1.98. Cumul des candidats V1.99/V1.100 : direction et pose de World Ender War, mouvement natif et fins de Rampage Strife. V1.100 a passe 58/58 suites mais l'analyse MSVC signale C6386 sur l'index du tableau d'etats ; son ZIP reste une archive intermediaire.

V1.101 remplace tous les acces variables a ce tableau par un resolveur borne explicite P1/P2, avec refus des indices invalides et tests -1/2. Aucune suppression d'avertissement. Les deux candidats de gameplay restent identiques : rayon dans le repere propre de War, geste unique puis pose maintenue ; Rampage en mouvement natif sur une piste non evaluee par le corps de Strife. Pas d'animation Rampage dediee annoncee.

Geyser/Clone/Caltrops valides, Stinger natif, cameras, tirs, manettes, roues et autres acquis conserves. 168 cles INI et 55 ID Hotkeys inchanges ; 165 archives precedentes a conserver, debug uniquement. Installation manuelle jeu ferme en gardant INI/raccourcis/PAK/sauvegardes. P2 differe. [Contrat, essais et repli](docs/V1_101_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

Essai groupe : War World Ender dans les deux vues (direction, pose, fin et relance) ; Strife Rampage en espace libre puis face a un obstacle (mouvement, degats, fin/reprise et relance) ; retour Native et controle rapide des trois transferts deja valides. Ces comportements restent a approuver en jeu.

---

## V1.100 - Rampage Strife et correction du rayon War

**EXPERIMENTALE, NON VALIDEE EN JEU. Compilation Windows et livraison a verifier.** Base validee V1.98 ; cumul V1.99 avec retour utilisateur partiel : World Ender fonctionne sur War, mais tire a 90 degres a droite dans les deux vues et repete son geste. Ces deux defauts restent ouverts jusqu'au prochain essai.

Ce lot remplace la source artistique VFX_Aura par le repere de tir du composant prive, aligne sur la capsule de War. Son geste se joue une fois, maintient sa pose pendant le rayon, puis revient a son idle ; duree, callbacks, cout et annulation natifs conserves. Aucun changement de camera ni correctif general des tirs.

Rampage sur Strife revient par un chemin distinct de V1.96 : mouvements portes par les animations, boucle, collisions et phases de fin natifs dans un montage prive. La piste utilise un emplacement absent du graphe de Strife, verifie a chaque lancement : son corps garde ses propres poses de locomotion, sans recevoir les os de War. **Pas d'animation Rampage dediee a Strife annoncee ; mouvement, degats et sortie restent a tester.** Contrats refuses = abandon natif du pouvoir et transfert indisponible pour la session, sans timer de secours.

Geyser/Clone/Caltrops valides, Stoneskin/Stinger natifs, controles, tirs et cameras conserves. INI de 168 cles et 55 ID Hotkeys inchanges ; 164 anciennes archives a conserver, debug seulement. Installation manuelle jeu ferme, garder son INI, ses raccourcis, PAK et sauvegardes. P2 differe. [Preuves, essais et rollback](docs/V1_100_STRIFE_RAMPAGE_WAR_BEAM_TEST.md).

Test groupe : World Ender sur War dans les deux vues, orientation, pose maintenue et sortie ; Rampage sur Strife en espace libre puis face a un obstacle, fin et relance ; retour Native et comparaison des pouvoirs propres. Aucun resultat CI ne remplace ces essais en jeu.

---

## V1.99 - World Ender sur War, composant natif propre

**CONSTRUITE / CONTROLEE : [56/56 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40174981083), zero avertissement/erreur MSVC. [ZIP immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/030d79dc867acc19431c3b710b5b9eb62ce60818/downloads/DarksidersGenesis_V1.99_WAR_WORLD_ENDER_NATIVE_COMPONENT_TEST.zip) - [Recu](docs/V1_99_DELIVERY_RECEIPT.md). World Ender War EXPERIMENTAL, NON VALIDE EN JEU ; base V1.98 validee.**

**Base V1.98 VALIDEE par l'utilisateur le 25 octobre 2026** : gestes Geyser sur Strife, Clone/Caltrops sur War approuves ; Stinger natif deja accepte et cloture. Ce nouveau candidat conserve ces acquis. **V1.99 EXPERIMENTALE, NON TESTEE EN JEU ; compilation et livraison controlees, voir le recu ci-dessus.**

L'ancien transfert World Ender cherchait une propriete `Chaoseater` absente de War. Le nouveau chemin utilise son vrai modele `CharacterMesh0`, le point natif `VFX_Aura` et un composant de rayon prive lui appartenant, enregistre dans son monde et attache a sa capsule par le moteur. Montage prive avec geste War, deux callbacks StartBeam/DestroyBeam uniques aux temps natifs et etats d'annulation conserves. Pas de nouveau hook, timer, touche imposee ni modification d'un asset partage. [Contrat, preuves, essais et rollback](docs/V1_99_WAR_WORLD_ENDER_NATIVE_COMPONENT_TEST.md).

Rampage sur Strife reste indisponible : sa boucle utilise le mouvement porte par les animations de War, absent des clips Strife et de la pose idle rejetee. Aucune reactivation de cet ancien adaptateur. Pouvoirs propres natifs, tirs/cameras/controleurs/roues et acquis conserves. INI de 168 cles, 55 ID Hotkeys et valeurs par defaut inchanges ; 163 anciennes archives intactes, debug uniquement. Installation manuelle en conservant son INI, ses raccourcis, son PAK et ses sauvegardes. P2 reporte.

Test groupe : War a pied en mission, World Ender dans un emplacement, verifier rayon et degats en vue native puis TPS, fin du pouvoir, reprise du mouvement et second lancement ; remettre Native et comparer Strife. Verifier rapidement les trois transferts valides. Le rendu, l'orientation, les sons et les variantes du rayon sur War restent a approuver en jeu.

---

## V1.98 - Gestes propres au receveur et cristal Stinger identifie

### Retour utilisateur du 25 octobre 2026 - Stinger cloture

Sur V1.98, l'utilisateur confirme que le cristal du coeur Stinger est dans le dos, bien attache au personnage, et accepte ce rendu comme le comportement vanilla retrouve. **Apparence Stinger VALIDEE par l'utilisateur ; demande de deplacement/orientation vers l'avant CLOTUREE.** Conserver cet effet natif, sans nouvelle adaptation, reattache ou decalage. Le releve de rotations est arrete. Cette validation concerne uniquement Stinger/Stoneskin, pas les gestes Geyser/Clone/Caltrops V1.98, qui restent experimentaux et a tester. Aucun code ASI, INI, raccourci, PAK ou fichier du jeu modifie ; aucun nouveau build.

Lot EXPERIMENTAL, validation en jeu encore requise. Base cumulative V1.97 ; acquis utilisateur : effets/degats Geyser et Caltrops V1.95, effet Clone V1.96. V1.97 reste un repli construit/controle, pas une version declaree validee par l'utilisateur.

Geyser sur Strife utilise un geste de canalisation Strife ; Clone/Caltrops sur War un geste War. Squelettes propres, fichiers d'animation natifs inchanges, sons/pas seuls dans ces gestes. Durees des casts, callbacks et effets retenus conserves ; retour a la pose precedente si le geste charge est indisponible. Aucun nouveau hook, chargement force ou timer. Rampage sur Strife et World Ender sur War restent retires ; pouvoirs propres natifs conserves.

Cristal Stoneskin : capture en lecture seule identifie P_Stinger_CreatureCore_StoneSkin du coeur Stinger, attache a CollisionCylinder, 97 unites au-dessus de la racine du modele. L'utilisateur confirme sa disparition en retirant ce coeur ; les effets Stoneskin ordinaires restent presents. Source visuelle CONFIRMEE ; aucune preuve de deplacement cause par le mod, aucun correctif global d'effets ajoute. [Preuves, objectif, essais et rollback](docs/V1_98_RECEIVER_CASTING_GESTURES_TEST.md).

Installation manuelle jeu ferme, conserver son INI/raccourcis/PAK/sauvegardes. 168 cles et 55 ID Hotkeys inchanges ; 162 archives anterieures a conserver ; debug seulement, P2 differe. Le recu de livraison donne les resultats Windows/ZIP lorsqu'ils sont disponibles. Les entrees suivantes sont historiques.

---

## V1.97 - Repli apres rejet de V1.96

V1.96 REJETEE en jeu : Rampage sur Strife bloque le personnage ; World Ender sur War echoue ; Clone fonctionne sans animation correcte. Caltrops/Geyser : effets et degats confirmes, animations toujours incorrectes. **V1.97 CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40123319154), zero avertissement/erreur MSVC ; repli NON VALIDE EN JEU.** [ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/81b4e41b4d0b9a2d74eee45ba6f63bcbe986c217/downloads/DarksidersGenesis_V1.97_FOREIGN_POWERS_SAFE_SUBSET_TEST.zip) - [Recu](docs/V1_97_DELIVERY_RECEIPT.md). Base : V1.95 et uniquement les effets retenus de Clone V1.96.

Le code des deux transferts rejetes est retire, pas seulement masque dans le menu. Les anciennes selections Rampage sur Strife et World Ender sur War reviennent a Native. Leurs pouvoirs d'origine restent disponibles sur leur propre personnage. Clone/Caltrops sur War et Geyser sur Strife conserves. Aucun changement des tirs, cameras, controles, effets valides ou defauts INI.

Stoneskin reste natif sur War ; aucune adaptation de ce pouvoir. Cristal toujours signale, uniquement pendant Stoneskin : NON CORRIGE. Le PAK d'origine contient un effet de pointes de pierre conditionne par un coeur ; piste seulement, aucun composant observe en jeu et aucune attribution de cause. Animations : les poses idle ne sont pas une solution finale ; il faut reprendre le transfert d'animation et son lifecycle avant de reintroduire les pouvoirs rejetes.

Installation manuelle avec INI/raccourcis/PAK/sauvegardes conserves. 168 cles, 55 ID Hotkeys, 161 anciennes archives ; debug uniquement, P2 reporte. [Etat, preuves et essais](docs/V1_97_RETAINED_FOREIGN_SUBSET_TEST.md). Les entrees suivantes sont historiques.

---

## V1.96 - lot des capacites et retrait de notre Stoneskin

24 octobre 2026. **CONSTRUITE / CONTROLEE : [55/55 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40119582099), zero avertissement/erreur MSVC. Nouveaux transferts EXPERIMENTAUX, non valides en jeu.** [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/fbb17b46f293e9c64fb59ebfa7ae5d4cb92bc923/downloads/DarksidersGenesis_V1.96_FOREIGN_POWER_BATCH_TEST.zip) - [Recu](docs/V1_96_DELIVERY_RECEIPT.md). Base cumulative V1.95. Caltrops sur War et Geyser sur Strife : effets et degats confirmes par l'utilisateur ; animations incorrectes. Ces chemins sont conserves.

Le meme lot ajoute Clone et World Ender sur War, Rampage et ses quatre suites sur Strife. Composants propres au lanceur et montages prives compatibles. Poses idle : animations, mouvement de Rampage et nouveaux effets/degats non valides en jeu.

Notre adaptation de Stoneskin et son choix explicite sont supprimes. Ancien choix 6 = Native ; Stoneskin original de War conserve via Native. Comparer le cristal apres redemarrage complet, sa cause reste inconnue. Trace Geyser temporaire retiree. Aucun nouveau hook ou timer de gameplay.

Installation manuelle avec INI, raccourcis, PAK et sauvegardes conserves. 168 cles, 55 ID Hotkeys et 160 anciennes archives preserves. Debug exclusivement ; P2 reporte. [Contrat et essais](docs/V1_96_FOREIGN_POWER_BATCH_TEST.md). Les entrees suivantes sont historiques.

---

## V1.95 — Geyser sur Strife, candidat final et identité du ZIP centralisée

24 octobre 2026. **CONSTRUITE / CONTRÔLÉE : [54/54 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40114886605), zéro erreur et zéro avertissement MSVC. Geyser sur Strife EXPÉRIMENTAL, non validé en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/027dae183a714754e66263e4f0f3bc0622498c42/downloads/DarksidersGenesis_V1.95_STRIFE_GEYSER_PRESENTATION_TEST.zip) · [Reçu](docs/V1_95_DELIVERY_RECEIPT.md). Base validée V1.87 dans les cas rapportés ; cumul V1.93 et correction de présentation V1.94. Celle-ci a passé 54/54 suites sans avertissement MSVC, mais son archive publiée porte encore le suffixe Caltrops : elle reste immuable et intermédiaire. V1.95 conserve exactement son code de jeu et fait lire le nom du ZIP depuis BuildInfo, avec vérification de sa version avant compilation.

Geyser sur Strife : son de lancement, lames/nova et variantes à leurs temps natifs ; un seul SpawnBlast. Racine propre de Strife dans des notifications privées, sans effets de son épée ni voix de War. **Pose Strife_Idle encore utilisée, dégâts et rendu non validés en jeu.** [Contrat et test ciblé](docs/V1_95_STRIFE_GEYSER_PRESENTATION_TEST.md). Pas de nouveau hook/minuterie, ni modification des autres capacités ou des acquis.

Installation manuelle, garder son INI/raccourcis/PAK/sauvegardes. 168 clés, 55 ID Hotkeys et 159 anciennes archives intacts ; main/archive/v1.43 préservés, P2 reporté. Les sections suivantes sont historiques.

---

## V1.94 — présentation native de Geyser sur Strife

24 octobre 2026. **V1.94 INTERMÉDIAIRE CONSERVÉE : 54/54 suites et zéro avertissement MSVC, mais suffixe d’archive resté sur Caltrops. Remplacée par V1.95 avec le même code de jeu et un nom de ZIP centralisé. Aucun résultat en jeu supposé.** Base validée V1.87 dans les cas rapportés ; cumul V1.93 et diagnostic réel du 24 octobre. Le dernier essai Strife reste rejeté : pose figée sans effet. Le journal prouve cependant l’appel de SpawnBlast et un objet d’attaque appartenant au bon personnage ; les dégâts ne sont pas mesurés.

Correction ciblée : le montage privé conserve maintenant le son de lancement, les lames/nova et leurs variantes avec leurs temps natifs, ainsi qu’un seul SpawnBlast. Les points Bone_War_Root deviennent la racine de Strife dans des notifications privées ; voix, balayages et effets de son épée ne sont pas transférés. Le jeu conserve allocation, notifications et fin du montage. **La pose reste Strife_Idle : aucune nouvelle animation de lancement annoncée, ni dégâts prétendus réparés.** [Contrat, limites, test et rollback](docs/V1_94_STRIFE_GEYSER_PRESENTATION_TEST.md).

Installation manuelle avec INI/raccourcis/PAK/sauvegardes personnels conservés. Caltrops sur War V1.93 reste expérimental ; Stoneskin, capacités propres, caméras/tirs, XInput/roues et autres acquis inchangés. P2 reporté. 168 clés, 55 ID Hotkeys et 158 anciennes archives préservés ; debug exclusivement. Les sections suivantes sont historiques.

---

## Diagnostic Geyser V1.93 — 24 octobre 2026

Essai comparatif reçu : **Strife reste figé sans effet ; transfert non validé**. Le journal prouve pourtant l’appel réel de SpawnBlast et des événements d’un objet d’attaque appartenant au bon personnage, avec War puis Strife. Le code et une nouvelle extraction vérifiée du PAK montrent que le montage privé omet les sons et effets natifs et utilise une pose de repos. **Défaut de présentation identifié ; dégâts non mesurés, aucun nouveau correctif/build annoncé.** [Résultat, limites et correction ciblée](docs/GEYSER_COMPARISON_2026_10_24.md). Les sections suivantes sont historiques ; V1.93 reste le dernier ZIP.

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

## V1.79 test — World Ender natif, bras OFF, épées et saut monté

**V1.79 CONSTRUITE / CONTRÔLÉE : [48/48 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39683569838), zéro avertissement MSVC. Nouveaux comportements EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f579ed3f068ff1cba6f25a7a6ba680ba5759517/downloads/DarksidersGenesis_V1.79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_79_DELIVERY_RECEIPT.md). Les 143 anciennes archives et les 163 anciens réglages sont conservés. Installation manuelle, garder son INI.

**Lot EXPÉRIMENTAL, base cumulative V1.78, debug seulement.** Bras gauche/droit OFF traités sur la même base, côtés de holsters inversés, choix séparé pour masquer les épées de Strife. World Ender retrouve son axe natif : la correction des faisceaux ordinaires ne doit plus le diriger à l'opposé des canons selon la caméra. Nouvelle case **Jump on horseback**, OFF par défaut, utilisant l'action de saut réaffectable du jeu et le mouvement natif des montures. La visibilité de World Ender à caméra proche et les nouvelles animations restent à confirmer en jeu.

Voir [objectif, preuves, limites et liste des essais](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_79_NATIVE_WORLDENDER_BLADES_MOUNTED_JUMP_TEST.md). Compilation, contrôles et empreintes seront consignés dans le reçu de livraison ; ils ne constituent pas une validation en jeu. Installation **manuelle**, conserver son INI. Les groupes fermés par défaut, Never, maintien OFF, menus/manette et les réglages existants sont conservés. Coop différée, capacités étrangères, réception lourde et suivi vertical des exécutions restent ouverts.

---

## V1.78 test — Darksiders Genesis Enhanced PC Mod

**CONSTRUITE / CONTRÔLÉE : [47/47 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39669799213), zéro avertissement MSVC. Nouveautés EXPÉRIMENTALES, non validées en jeu.** Base cumulative V1.77, avec les retours utilisateur du 21 octobre : visée sans tir, réglages et updater validés dans les essais rapportés ; OFF avec pistolet droit visible validé à pied. Réception lourde incertaine, coop reportée. [Objectif, preuves, limites et essais](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.md).

Lot : choix OFF indépendant du cache de pose, maintien pendant les roues et au retour de l’ultime ; masquage des modèles en main et rangés à cheval ; candidat pour reposer aussi l’épaule droite lorsque seul le pistolet gauche est visible ; masquage du holster à pied du côté OFF dans la seule copie de rendu. Projectiles, sockets et animations de gameplay conservés. Les bras et holsters en cuir du modèle monté restent natifs : aucune validation visuelle supplémentaire n’est supposée.

Le menu propose désormais **Never (always visible)** pour le trait/arc, et ses groupes démarrent repliés. Les choix déjà ouverts restent ouverts pendant la session. Tous les réglages fournis et les raccourcis sont conservés. Nettoyage issu des deux audits : **167 lignes nettes de code inutilisé retirées**, sans gain de FPS ou de taille binaire revendiqué.

**World Ender reste ouvert en TPS.** Le rayon et son effet natifs sont créés et infligent des dégâts ; l’utilisateur voit le rayon en vue du dessus seulement. Le masquage trait/arc n’explique pas le défaut. Aucun hook de rayon ajouté sur cette hypothèse. Les six capacités entre personnages, la réception lourde et le suivi vertical pendant les exécutions restent dans la file active.

[ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/08a5a2ace26e80ea8950a31996ca7fed6650a8ad/downloads/DarksidersGenesis_V1.78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_78_DELIVERY_RECEIPT.md). Installation **manuelle**, conserver l’INI personnel. Aucun fichier du jeu, PAK ou sauvegarde remplacé par l’agent. Les sections suivantes conservent l’historique de leurs propres versions.

---

## V1.77 test — Darksiders Genesis Enhanced PC Mod

[Audit de maintenance comparé](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/AUDIT_DOUBLE_NETTOYAGE_V1_77.md) : patch de nettoyage préparé pour le prochain lot, non appliqué ; V1.77 reste inchangée pendant les essais.

Lot cumulatif debug **CONSTRUIT / CONTRÔLÉ : 47/47 suites Windows x64, zéro avertissement MSVC**. Nouveau panneau Gameplay → **Wrath ability slots** : trois choix indépendants par personnage, parmi ses trois capacités natives. Tous les choix fournis valent Native. Le jeu conserve lancement, déverrouillage, coût et animations. **EXPÉRIMENTAL, non validé en jeu.**

**L'échange libre des six capacités reste à terminer.** Les pouvoirs étrangers nécessitent l'adaptation des animations et composants. Les indications de roue peuvent encore décrire l'emplacement initial. Candidats visée sans tir/réception lourde V1.75 et OFF à pied V1.76 repris sans modification, toujours à tester. Base validée V1.73 dans les essais rapportés.

[Télécharger le ZIP V1.77 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b5da4721104b0b5a08911bf8cc51af88a4559f6f/downloads/DarksidersGenesis_V1.77_NATIVE_WRATH_SLOTS_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_77_DELIVERY_RECEIPT.md) · [Essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_77_NATIVE_WRATH_SLOTS_TEST.md). Installation manuelle ultérieure, garder l'INI personnel et ses raccourcis ; les six nouvelles clés absentes restent Native. Aucun fichier du jeu remplacé par l'agent. Les sections suivantes décrivent les versions précédentes et leurs statuts historiques.

---

## V1.76 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif debug, **CONSTRUIT / CONTRÔLÉ : 46/46 suites Windows x64, zéro avertissement MSVC**. Nouvelle option **OFF (foot, experimental)** par pistolet de Strife dans Gameplay → Appearance. **Non validée en jeu.** Reprend la visée manette sans tir et le Recovery de réception lourde V1.75, encore à tester. Base validée V1.73 dans les essais rapportés.

Pour OFF à pied : ranger les armes puis rester immobile brièvement ; le menu indique quand la pose est prête. Le candidat masque le pistolet choisi et repose son bras pendant la visée, avec tirs actifs. Retour Native réversible. Mêlée, cheval et ultime restent natifs. Les effets connus propres à une main sont filtrés ; certains effets communs restent avec un pistolet visible, puis sont retirés avec les deux OFF. **OFF universel des effets/formes encore ouvert**, ainsi que l'échange libre des six capacités.

[Télécharger le ZIP V1.76 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/026664c3e06a85b3714af5591ff3bef135ff2cb1/downloads/DarksidersGenesis_V1.76_FOOT_PISTOL_OFF_TEST.zip) · [Reçu et intégrité](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_76_DELIVERY_RECEIPT.md) · [Essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_76_FOOT_PISTOL_OFF_TEST.md). Installation manuelle ultérieure : conserver l'INI personnel et ses raccourcis. Aucun fichier du jeu remplacé par l'agent. Les sections suivantes décrivent les bases précédentes et leurs statuts historiques.

---

## V1.75 test — Darksiders Genesis Enhanced PC Mod

Lot cumulatif **debug** sur la base V1.73 validée dans les essais rapportés : **visée sans tir à la manette** avec un ou deux boutons configurables, et candidat **Heavy landing movement recovery** pour reprendre le déplacement après une réception lourde automatique. Version **CONSTRUITE / CONTRÔLÉE**, 45/45 suites Windows x64, zéro avertissement MSVC. Les deux nouveautés sont **EXPÉRIMENTALES**, OFF par défaut et non validées en jeu. [ZIP vérifié, intégrité et reçu de livraison](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_75_DELIVERY_RECEIPT.md). [Objectif et essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_75_PAD_AIM_HEAVY_LANDING_TEST.md).

Gameplay → **Aim without firing** : un champ OFF laisse l'autre bouton fonctionner seul ; deux entrées actives nécessitent leur maintien ensemble ; OFF/OFF désactive la fonction. Le maintien appelle l'action native, sans simuler Ctrl ou ajouter de tir. En TPS, il suit la caméra du joueur ; chaque manette concerne son propre joueur. Les affectations natives restent actives. Relâcher après menu, changement de personnage ou reconnexion avant de viser à nouveau.

Gameplay → Player movement → **Heavy landing movement recovery (experimental)** : ouvre les deux verrous de déplacement après l'impact d'une chute assez haute, sans forcer l'arrêt de l'animation. Ce candidat vise seulement les HeavyLand normaux de Strife/War ; dash, attaques, Rampage, transformations et changements de personnage restent conservés. L'efficacité réelle et les effets doivent être vérifiés en jeu ; trois réceptions Strife observées, pas de War_HeavyLand dans la capture.

**L'échange libre des six capacités et le OFF complet par pistolet restent en étude et ne sont pas annoncés fonctionnels.** Les dépendances de squelettes, composants et rendu sont documentées. [File restante](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md). Installation manuelle par l'utilisateur à la fin de sa session, INI personnel et raccourcis à conserver. Aucun fichier du jeu remplacé pendant la préparation.

---

### Acquis de la base V1.73

Canal **debug**, lot cumulatif : maintien de Despair/Rampage au chargement, distance DOF native/TPS jusqu'à **5 000 m**, nettoyage du Recovery du dash signalé sans effet. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ** ; trois essais V1.73 confirmés par l’utilisateur : 43/43 suites Windows x64, zéro avertissement MSVC ; installation avec sauvegarde et INI personnel conservé. Chargement des montures, réglages DOF et contrôle visée/tir/dash confirmés dans les essais V1.73 rapportés. [ZIP immuable contrôlé](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/516db3d8cd898e9d544c571a2ba73fdfa38d070b/downloads/DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip). [Critères et essais groupés](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_73_MOUNT_DOF_CLEANUP_TEST.md).

La case Bonus mount skins règle la disponibilité du menu ; le bonus déjà équipé reste conservé même OFF, comme les armures. Le modèle de base reste sélectionnable et son choix n'est jamais remplacé automatiquement. Chargement des montures, réglages DOF et contrôle visée/tir/dash confirmés dans les essais V1.73 rapportés. Le flou V1.71 semble bon dans le dernier retour utilisateur ; les réglages DOF V1.73 sont également confirmés dans les essais rapportés.

Le candidat Dash movement recovery V1.71 est **REJETÉ dans l'essai utilisateur et retiré**, avec son état, sa checkbox et son réglage runtime. Les corrections validées de visée/tir pendant le dash sont conservées. Les anciens raccourcis Recovery restent inertes sans décaler les autres. V1.75 ajoute un nouveau candidat distinct pour la réception lourde automatique après chute, OFF par défaut et encore expérimental.

Archive vérifiée : `DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip`, quatre fichiers racine. OFF complet des pistolets reste ouvert ; modèles gauche/droite validés conservés.

Suite en cours : [audit OFF des pistolets et HUD général](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/AUDIT_PISTOL_OFF_HUD_2026_10_20.md). Les montages de tir ne séparent pas les mains par leur nom de slot ; aucun OFF partiel ajouté. Essai V1.73 après refus d’arène : HUD toujours visible, relevés avant/après effectués ; défaut historique placé en surveillance.

### Installation et mise à jour

Placer les quatre fichiers à côté de `DarksidersGenesis-Win64-Shipping.exe`, dans `ProjectMayhem/Binaries/Win64`. Sauvegarder les fichiers déjà présents. Pour mettre à jour une installation existante, **conserver l’INI personnel et ses raccourcis** ; les réglages manquants prennent leur valeur par défaut compilée. Fermer le jeu avant de remplacer les fichiers chargés.

Le menu s’ouvre avec **Inser** dans le profil fourni ; cette touche est reconfigurable. Onglets : **Gameplay**, **Camera**, **Hotkeys**, **Updates**, **About**. Les valeurs numériques disposent des contrôles indiqués dans le menu ; utiliser les descriptions de chaque option pour son unité et le sens de son facteur.

Dans **Updates**, choisir le canal voulu, puis vérifier les mises à jour. Le changement de canal seul n’installe rien. L’installation à la fermeture et la restauration utilisent le système de sauvegarde intégré. **Auto check update**, facultatif et désactivé par défaut, vérifie au démarrage le canal choisi ; une mise à jour trouvée produit un avis temporaire puis un rappel jaune dans le menu. Son comportement en jeu attend encore un retour explicite.

### Fonctions et état

| Groupe | Contenu | État à retenir |
| --- | --- | --- |
| Caméra et commandes | TPS/native, souris relative, manette, échange des sticks, FOV coop, profils pied/cheval/ultime, cinématiques et roues | Chemins acquis conservés ; réglages et commandes reconfigurables |
| Visée et dash | Visée souris/manette, trajectoires, reprise AIM et retrait des tirs/sons/effets Standard parasites pendant le dash | Corrections confirmées dans les essais utilisateur, conservées |
| Netteté et interaction HUD | Focus TPS, taille et orientation des indications d’interaction P1/P2 | Validés dans les essais rapportés ; autres icônes/effets P2 reportés |
| Ressources | Gains, remplissages et Unlimited ; munitions, Hotstreak, vie, potions, ultime ; Standard limité | Fonctions validées conservées ; facteurs et descriptions propres à chaque réglage |
| Mouvement et grappin | Réglages de déplacement/saut/plané/cheval, sprint, portée native du grappin | Acquis conservés ; portée validée dans l’essai utilisateur |
| Rangement | Délais séparés lames Strife / pistolets Strife / épée War ; action commune avec raccourcis clavier et manette | Délais validés V1.65 ; holsters validés dans l’essai V1.66 |
| Sauts supplémentaires | OFF / After gliding / Always on | Sauts validés dans l’essai utilisateur V1.66 |
| Recovery | Ancien candidat dash retiré ; nouveau Heavy landing movement recovery pour réception lourde automatique | OFF par défaut, expérimental V1.75 ; efficacité à confirmer en jeu |
| Modèles 3D des pistolets | Choix indépendant du modèle droit ou gauche par main de Strife | Choix gauche/droite validés ; OFF complet encore ouvert |
| Skins | Deux montures bonus ; six variantes de progression dans deux groupes séparés | Sélection et OFF confirmés V1.68 ; sélection sans PAK confirmée ; maintien au chargement V1.73 confirmé dans les essais rapportés |

[État courant](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/CURRENT_STATE.md) et [file active](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ACTIVE_QUEUE.md) font référence pour les éléments restant à tester ou réparer. Une validation utilisateur porte sur l’essai rapporté, sans supposer tous les personnages, chapitres et états coop vérifiés.

### Apparences et durée ultime

Les choix **Native / Right pistol model / Left pistol model** par main de Strife sont conservés. Les holsters montés utilisent la variante rangée correspondante. Le futur OFF complet (modèle, effets et bras, avec projectiles conservés) n’est pas livré dans cette version.

**Bonus mount skins** et **Progression skins** restent indépendants et OFF par défaut. V1.67 interceptait à tort « équipé » ; V1.68 remplace ce branchement par le vrai « débloqué ». Aucun droit DLC, récompense ou bonus de gameplay accordé par le mod. Le jeu garde la sélection native ; fermer et rouvrir la page Skins après un changement. L’essai V1.68 de chevaux/armures et le retour natif OFF sont confirmés sans PAK cosmétique par le journal moteur. Les armures restent dans le dernier essai. Le défaut de chargement des montures a été localisé dans le filtre DLC natif ; V1.72 conserve les deux choix bonus déjà présents même lorsque leur disponibilité dans le menu est désactivée, comportement confirmé dans les essais V1.73 rapportés. Un PAK éventuellement réinstallé peut maintenir des montures disponibles avec l’option ASI OFF.

**Infinite Ultimate duration** bloque seulement le débit de la transformation déjà active, au niveau courant de jauge. Il ne remplit rien et ne rend pas l’activation gratuite. Décocher reprend le multiplicateur de durée si sa case est active, sinon le débit natif. Nouvelle action assignable dans Hotkeys, non attribuée par défaut. **Unlimited Ultimate** conserve son rôle de remplissage.

**Override Leviathan lightning**, décoché par défaut, rend le comportement natif. Une fois coché : **OFF / Effect 1 / Effect 2 / Effect 3**. Visuel seulement, sans accorder le cœur ou son bonus. Si le visuel choisi est indisponible, le menu l’indique et conserve l’effet natif. Pied/cheval/ultime et P2 à tester, ainsi que le retour natif sans doublon.

**TPS character focus** garde la méthode DOF du niveau. En TPS, la couche Gaussian proche est désactivée et la région nette protège la profondeur du modèle ; flou lointain, qualité et ouverture restent natifs. Clé INI `TPSLensFocus` conservée. OFF garde la correction de distance seule. V1.69 remplace le forçage CircleDOF qui rendait les décors trop nets. V1.70 conserve aussi la région nette du niveau tant que la caméra ne la surcharge pas déjà. Dans Camera → Depth of field, Native view et TPS view ont chacun Disable et Custom. Custom remplace explicitement le DOF par Gaussian ; distance0–5000m, quantité0–5% absolue (pas un facteur), saisie manuelle et Default. Distance10m et quantité0,6% sont inactives tant que Custom est OFF. Disable est prioritaire et garde les valeurs mémorisées. Les réglages DOF sont confirmés dans les essais V1.73 rapportés.

Léviathan s'attache à la racine comme le natif ; l'ancien rattachement au modèle de Strife était 97 unités plus bas. Hauteur et rangement du menu validés par l’utilisateur dans l’essai V1.69. **Appearance** et **Weapon sheathing** sont repliables et distinctes de **Player movement** ; réglages et raccourcis conservés.

La disparition du HUD après refus d’arène n’est pas reproduite dans le dernier essai V1.73 ; surveillance, sans correction dédiée ni réaffichage global forcé. OFF complet pistolet et nouvelle approche Recovery ouverts. [Essais et rollback V1.69](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_69_NATIVE_VISUAL_FOCUS_TEST.md).

### Raccourcis du profil fourni

Toutes les affectations suivantes sont reconfigurables et l’INI personnel reste prioritaire.

| Touches | Actions fournies |
| --- | --- |
| F1–F4 | Remplir toutes les jauges / Ultime / Synergie / Courroux |
| F5–F8 | Unlimited toutes les jauges / Ultime / Synergie / Courroux |
| F9 | Unlimited munitions |
| F10–F11 | Non attribuées |
| F12 | Afficher/masquer le HUD |

Les commandes de caméra native et de TPS ont leurs sections dédiées ; elles ne remplacent pas la configuration rebindable du jeu. Le rangement dispose aussi de raccourcis directement dans Gameplay, en plus des actions Hotkeys. **Les raccourcis de rangement dédiés sont non attribués par défaut.**

### Validation V1.66 et retour arrière

Retour explicite du 19 octobre 2026 : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». Ces deux sujets sont clos dans l’essai rapporté ; V1.66 devient la base de travail retenue. Ce retour ne décrit pas une matrice exhaustive de modes, personnages ou situations coop et ne confirme pas séparément Auto check update.

Le code validé est conservé. En cas de nouvelle régression, noter le contexte et garder le journal ; le système de restauration et les anciennes archives permettent un retour arrière en préservant l’INI personnel. Le [ZIP V1.66 conservé](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) permet de revenir à cette base en conservant l’INI personnel.

### Sources, construction et historique

Source C++20/CMake, Windows x64 ; dépendances MinHook et Dear ImGui fixées par le projet. Le workflow [.github/workflows/build-v090-debug.yml](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/.github/workflows/build-v090-debug.yml) configure, compile, teste et préserve une nouvelle archive numérotée sur `debug`. Une archive publiée n’est jamais remplacée par des octets différents. La release stable n’est pas promue par une simple réussite de CI.

[Carnet technique cumulatif](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/TECHNICAL_NOTEBOOK.md) · [Prochaines étapes](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/ROADMAP.md) · [Audit Recovery et modèles](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/NEXT_RECOVERY_AND_MODELS.md) · [Règles du projet](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/DEVELOPMENT_RULES.md).

Le README actuel sépare les informations actives des anciennes préparations. **L’ancien README intégral est préservé, octet pour octet, dans [l’historique jusqu’à V1.66](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/history/README.md)**. Les anciennes affirmations de statut et anciens raccourcis de cet historique ne décrivent pas la version actuelle. Les anciens ZIP restent inchangés.
