## V1.135 - retour a l'acquisition native

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU.** Remplace le filtre de cible Statique et l'aide ajoutee par le selecteur natif et son point socket/bornes. Option0..200%,100%=reference native, OFF par defaut ; Global ou sept munitions. Deux hooks obsoletes retires, un hook natif garde ; portee/poids/degats/chaines non ajustes. Profil actif et proprietaires confirmes par1785echantillons en lecture seule, sans erreur.

Portee du nouvel ajustement : Strife a pied P1/P2 ; monture/ultime gardent le selecteur natif.218autres valeurs /55Hotkeys et fonctions validees conserves. Schema d'aide neuf,228cles. [Perimetre, nettoyage, limites et tests](V1_135_NATIVE_ACQUISITION_TEST.md). GitHub seulement, V1.133 reste installee ; aucun INI ou fichier de jeu remplace. main/archive et anciennes archives preserves.

---

## V1.134 - retour a l'acquisition native

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU.** Remplace le filtre de cible Statique et l'aide ajoutee par le selecteur natif et son point socket/bornes. Option0..200%,100%=reference native, OFF par defaut ; Global ou sept munitions. Deux hooks obsoletes retires, un hook natif garde ; portee/poids/degats/chaines non ajustes. Profil actif et proprietaires confirmes par1785echantillons en lecture seule, sans erreur.

Portee du nouvel ajustement : Strife a pied P1/P2 ; monture/ultime gardent le selecteur natif.218autres valeurs /55Hotkeys et fonctions validees conserves. Schema d'aide neuf,228cles. [Perimetre, nettoyage, limites et tests](V1_134_NATIVE_ACQUISITION_TEST.md). GitHub seulement, V1.133 reste installee ; aucun INI ou fichier de jeu remplace. main/archive et anciennes archives preserves.

---

## Audit Statique - 30 octobre 2026

**REGRESSION SIGNALEE V1.133 ; AUDIT CONTROLE, REPARATION NON CONSTRUITE.** Le journal compte5166applications d'aide, mais2637/2637cibles principales Statique retirees. Le filtre V1.130 modifie le chemin natif de longueur et le cycle des chaines ; sa preservation n'est plus consideree comme correcte. Les curseurs V1.131..133 ajoutent une correction, ils ne reglent pas le natif. Objectif revise : restaurer selection/cycle natifs puis ajustement optionnel,100%=natif, sans changer portee/degats/rebonds. [Audit et limites](STATIC_NATIVE_BEHAVIOR_AUDIT_2026_10_30.md). Autres acquis conserves ; V1.133 reste installee, aucune nouvelle compilation/installation/capture. Verification du contrat natif de parametres avant implementation.

---

## V1.133 - visibilite et profondeur de l'aide TPS

**CONSTRUITE / CONTROLEE : [68/68 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40959653744), zero avertissement/erreur MSVC. [ZIP cumulatif verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f3b9ec2cc71ef997543cfe5d2751a867190db6f/downloads/DarksidersGenesis_V1.133_AIM_ASSIST_TARGET_DEPTH_TEST.zip) - [Recu](V1_133_DELIVERY_RECEIPT.md). Updater debug et Defender controles ; correction EXPERIMENTALE, NON VALIDEE EN JEU. V1.133 installee sur demande ; V1.130 reste la base acceptee.**

**CONSTRUITE / CONTROLEE ; NON VALIDEE EN JEU.** Retour V1.131 : valeur augmentee puis remise a zero, aucune application journalisee. Le garde exigeant un contact bloquant avec l'ennemi est incompatible avec des profils natifs Projectile Overlap/Ignore. Verification remplacee par absence d'obstacle avant la cible ; journal des valeurs utilisees et raisons de refus. Cela reste une correction a tester, pas la preuve de l'unique cause en jeu.

Le point assiste garde aussi la profondeur finie du candidat : convergence depuis le canon et solveur natif Lava ne visent pas le mur/ciel derriere un ennemi non bloquant. Un obstacle plus proche sur le rayon partiel garde la priorite.

Base acceptee V1.130 ; mode Global / sept munitions V1.131, valeurs par defaut et INI personnel conserves. Cone, priorite aux collisions proches, contact Statique, cameras/input, pose UI et pouvoirs inchanges. [Perimetre et essai](V1_133_AIM_ASSIST_TARGET_DEPTH_TEST.md), [deux passes](evidence/V1_133_STATIC_REVIEW.json). Compilation GitHub ; V1.133 installee sur demande, INI conserve, jeu non lance ; main/archive/anciens ZIP preserves.

Installation demandee le 30 octobre 2026 : V1.133 installee et relue, sauvegarde V1.131 `before-V1.133-20261030-194838-8270be80` ; INI/raccourcis byte-identiques, EXE original et PAK preserves, jeu non lance. [Preuve](evidence/V1_133_INSTALL_CHECK.json).

---

## V1.132 - visibilite de l'aide TPS

**PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU.** Retour V1.131 : valeur augmentee puis remise a zero, aucune application journalisee. Le garde exigeant un contact bloquant avec l'ennemi est incompatible avec des profils natifs Projectile Overlap/Ignore. Verification remplacee par absence d'obstacle avant la cible ; journal des valeurs utilisees et raisons de refus. Cela reste une correction a tester, pas la preuve de l'unique cause en jeu.

Base acceptee V1.130 ; mode Global / sept munitions V1.131, valeurs par defaut et INI personnel conserves. Cone, priorite aux collisions proches, contact Statique, cameras/input, pose UI et pouvoirs inchanges. [Perimetre et essai](V1_132_AIM_ASSIST_VISIBILITY_TEST.md), [deux passes](evidence/V1_132_STATIC_REVIEW.json). GitHub seulement ; aucune installation V1.132 ni jeu lance, main/archive/anciens ZIP preserves.

---

## File active - V1.131

**CONSTRUITE / CONTROLEE : [68/68 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40947674176), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/73c494fb4acdd7d33125427dd2b084b921a434bc/downloads/DarksidersGenesis_V1.131_TPS_AIM_ASSIST_TEST.zip) - [Recu](V1_131_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Nouvelle aide EXPERIMENTALE, NON VALIDEE EN JEU ; V1.131 ensuite installee sur demande, INI conserve, jeu non lance. main reste V1.124.**

**CONSTRUITE / CONTROLEE ; NOUVELLE AIDE NON VALIDEE EN JEU.** V1.130 explicitement validee par l'utilisateur devient la base. Ajout demande : aide TPS0..100%, Global ou Per ammunition (sept valeurs), saisie manuelle/Default ;0 par defaut conserve V1.130. Meme calcul de cible3D pour les consommateurs, cible native visible dans un cone borne ; aucune rotation d'acteur ou nouveau hook.

Neuf nouvelles cles /227 au total ;218 anciennes valeurs et55ID Hotkeys conserves. Modeles/pose UI/Statique V1.130, cameras/input et autres acquis preserves. [Perimetre, limites et essai](V1_131_TPS_AIM_ASSIST_TEST.md), [validation V1.130](evidence/V1_130_USER_VALIDATION.json). Construction GitHub seulement ; V1.131 installee sur demande, INI conserve, jeu non lance. main reste V1.124, archive et anciens ZIP intacts. Les sections suivantes sont historiques.

Installation V1.131 demandee et verifiee : sauvegarde V1.130 `before-V1.131-20261029-212759-8dd7d05e`, ASI/DXGI/README relus, INI personnel byte-identique, PAK/EXE preserves. [Preuve](evidence/V1_131_INSTALL_CHECK.json).

---

## File active - V1.130

**CONSTRUITE / CONTROLEE : [67/67 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40807486983), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0aac20c4f76f6d10ce11f2ac5fbf718beaf2360e/downloads/DarksidersGenesis_V1.130_STATIC_CONTACT_UNARMED_PAUSE_TEST.zip) - [Recu](V1_130_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Statique/pose UI EXPERIMENTAUX, NON VALIDES EN JEU. Masquage modele V1.129 confirme conserve ; installation locale ensuite demandee et verifiee, INI conserve, jeu non lance. main reste V1.124.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Retour V1.129 : Statique et pose de main vide toujours incorrects ; utilisateur confirme que le modele masque reste cache. Masquage/modeles V1.129 conserves. Filtre initial remplace par le controle de contact pendant les reassignations natives du faisceau principal ; conservation de la derniere pose desarmee valide pendant UI/focus uniquement.

Base cumulative V1.129 ;218 valeurs INI /55 Hotkey ID et autres acquis preserves. [Causes, preuves, limites et tests](V1_130_STATIC_CONTACT_UNARMED_PAUSE_TEST.md). Construction GitHub seulement, installation locale demandee et verifiee, jeu non lance ; main reste V1.124, archive et anciens ZIP preserves. Coop plus tard. Les sections suivantes sont historiques.

V1.130 installee sur demande explicite, sauvegarde V1.129 `before-V1.130-20261029-034821-7254d2ef` verifiee ; ASI/DXGI/README relus, INI personnel identique octet pour octet, PAK/EXE preserves. [Preuve d'installation](evidence/V1_130_INSTALL_CHECK.json).

---

## File active - V1.129

**CONSTRUITE / CONTROLEE : [67/67 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40801514118), zero avertissement/erreur MSVC. [ZIP cumulatif public verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/029b17124e24f766db66b5a171d5aed68cb6536f/downloads/DarksidersGenesis_V1.129_STATIC_TARGET_PISTOL_LIFECYCLE_TEST.zip) - [Recu](V1_129_DELIVERY_RECEIPT.md). Updater debug et Defender controles. Correctifs EXPERIMENTAUX, NON VALIDES EN JEU ; V1.128 observee installee, aucune installation V1.129. main reste V1.124.**

**CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU.** Base V1.128 observee installee. Statique : retirer uniquement la cible assistee initiale du faisceau principal TPS Camera Forward ; conserver rebonds et autres chemins. Pistolets : resolution du composant live dans les setters natifs de modele/visibilite, apparence independante du focus/menu et du TTL des poses. Ancienne politique cachee/gatee remplacee ; animation de bras conservee.

218 valeurs INI / 55 ID Hotkeys inchanges ; autres modules preserves. Trois suites etendues et deux passes d'audit confrontees. [Preuves, limites et essai groupe](V1_129_STATIC_TARGET_PISTOL_LIFECYCLE_TEST.md). GitHub seulement, aucun lancement ni installation ; main reste V1.124, archive et anciens ZIP preserves. Coffre/dash restent non valides ; coop plus tard. Les sections suivantes sont historiques.

---

## File active - V1.128 presentation du menu

**CONSTRUITE / CONTROLEE ; MENU COMPLET NON VALIDE EN JEU.** Base V1.127 installee et conservee. Aide courte avec details au survol (?), marges et champs adaptes a la police, listes et valeurs homogenes, groupes Appearance fermes initialement, libelle/contact sol avances clarifies. Parametres, controles et modules de jeu inchanges ;218valeurs INI/55ID Hotkeys conserves. [Perimetre, deux passes et essai](V1_128_MENU_READABILITY_TEST.md).

Compilation uniquement GitHub ; aucun jeu lance ou remplacement V1.128 local. main reste V1.124 validee ; anciens ZIP/archive preserves. Bug de selection du tir souris TPS sur destructible reste ouvert, coffre/dash non declares valides. Les sections suivantes sont historiques.

Releve ulterieur V1.127 : au seul arret de dash observe, MOVE etait deja ouvert (0demande) ;2coffres ont emis2demandes. Aucun gain de controle mesure ; pas de changement Recovery dans V1.128. [Preuve et limites](evidence/V1_127_RECOVERY_OBSERVATIONS.json).

---

## File active - V1.127

V1.127 ensuite installee sur demande, sauvegarde V1.126 verifiee ; INI/raccourcis/PAK conserves, jeu non lance. Precision visee souris TPS : un ennemi peut attirer le rayon jusqu'au bord de l'ecran au lieu du destructible proche. Bug ouvert ; aucun correctif de visee dans ce lot. [Installation](evidence/V1_127_INSTALL_CHECK.json).

**CONSTRUITE / CONTROLEE ; NOUVEAU CONDITIONNEMENT NON VALIDE EN JEU.** Base V1.126 : timing personnalise reception lourde accepte par l'utilisateur, fusionne dans la case existante avec ses valeurs. Coffre :6demandes dans le journal ; dash :0. Aucun gain de controle prouve par ces traces. Ajout d'un bilan borne sans changer leurs gardes/mouvement. Visee souris TPS : investigation trace Projectile/objets Overlap et branche du faisceau ciblee ; aucun patch de visee, manette non testee. [Perimetre, limites et essai](V1_127_UNIFIED_LANDING_RECOVERY_TEST.md).

218valeurs INI conservees, cle doublee retraitee a la sauvegarde ;55ID Hotkeys inchanges. Leviathan/apparences/transfer capacites/cameras/controles preserves. main reste en V1.124 ; debug seulement. Pas d'installation, relance ou compilation locale. Les sections suivantes sont historiques.

---

# File active - V1.126 construite/controlee, essais en jeu attendus

28 octobre 2026. Garder toutes les apparences actuelles ; Leviathan confirme reste a part. Ajout candidat : impact de surface Tremor or/metal/pierre/eau, sans nouveau renderer. Recovery : trois essais opt-in (custom reception lourde, fin coffre, fin dash) et temps reglables ; nouvelle suite + anciennes fixtures avant livraison. [Contrat et essai groupe](V1_126_RECOVERY_EXPERIMENTS_TEST.md).

main reste en V1.124 validee ; debug seulement. Geyser/Stoneskin/Caltrops et autres acquis preserves ; coop plus tard, HUD/P2 en surveillance. Aucun correctif declare valide avant retour utilisateur. Pas de lancement/installation locale.

---

# File active - promotion V1.124 et consolidation V1.125

28 octobre 2026. Demandes utilisateur prises en compte : main contient la V1.124 validee ; travaux suivants uniquement debug. V1.125 construite/controlee, nettoyage non valide en jeu, pas de nouvelle intervention Recovery.

| Sujet | Etat courant |
| --- | --- |
| Apparences | Retour negatif partiel : plusieurs sans effet/peu interessantes.11groupes visibilite seulement. Identifier composants utiles et cibles chargees avant un nouveau correctif ; options/INI conserves. |
| Recovery apres animation | Recherche active : [carte des valeurs/verrous](V1_125_RECOVERY_TIMING_MAP.md),27assets natifs controles. Aucun delai global prouve. Code reception lourde valide inchange. |
| Nettoyage post-V1.78 | Deux passes confrontees, suppressions bornees ; 65/65 suites, ZIP/updater/Defender controles ; essai en jeu encore attendu. [Audit](V1_125_DOUBLE_AUDIT.md). |
| Rampage Strife | Accepte dans le retour «2. ok». Aucun chiffre de degats mesure invente. |
| Coop | Plus tard, selon utilisateur ; aucune validation exhaustive supposee. |
| FX/icones P2 | «cetait ok il me semble» : pas de bug actuel confirme, surveiller au prochain essai coop. |
| HUD general | Plus reproduit selon utilisateur, surveillance seulement. |

Geyser/Stoneskin/Caltrops et autres acquis conserves. Les anciens tableaux ci-dessous sont historiques et ne remettent pas ces sujets en attente.

---

# File active - V1.124 validee, 28 octobre 2026

**Base retenue : V1.124. Caltrops War et son petit saut final : VALIDES par l'utilisateur, sujet clos.** Geyser, Stoneskin, Rampage/World Ender/Clone acceptes, controles et autres acquis conserves. [Retour exact et limites](V1_124_USER_VALIDATION.md).

| Sujet restant | Etat et prochaine action |
| --- | --- |
| Consolidation du code actuel | Recommandation : nouvel audit en deux passes des transferts/diagnostics/restaurations ; aucun retrait non examine. Nettoyage V1.77 deja integre en V1.78. |
| Bibliotheque d'apparences | Onglet livre ; variantes/Hidden/retour Native a tester par famille. Modeles/materiaux/composants persistants hors bibliotheque de particules. |
| Rampage Strife, degats de charge | Comparaison sur ennemis encore non rapportee explicitement ; presentation et fin acceptees. |
| Coop/P2 | Essais finaux differes : pouvoirs simultanes, sons/effets, roues, camera/formes et reglages P2. |
| Autres icones/FX plats P2 | Reportes expressement ; identifier avant intervention. Prompts d'interaction valides conserves. |
| HUD general apres arene verrouillee | Surveillance uniquement, non reproduit aux derniers essais. |
| Recovery dash/coffres | Anciennes pistes differees ; pas de retour du candidat dash rejete. Reception lourde V1.103 deja validee. |

Pas de nouvelle build pour la validation documentaire. Suite conseillee : consolidation, essais groupes des apparences/degats, puis coop. Aucune promotion stable automatique. INI/raccourcis/PAK/sauvegardes et archives preserves.

---

# Historique conserve - les anciens etats ne remplacent pas la file ci-dessus

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

# Ajouts actifs — V1.74 en préparation

- **Visée sans tir manette : EN COURS.** Deux entrées, maintien, Native AIM/AIM_RELEASE ; OFF par défaut. Fonction à construire et tester sur base V1.73.
- **Capacités de Courroux : RECHERCHE.** Trois choix indépendants par personnage ; Native ou l'une des six capacités, choix libre confirmé. Dépendances de classes/animations/propriétaire à vérifier avant implémentation.

La file historique ci-dessous est conservée ; OFF pistolet et ground slam restent ouverts, autres FX P2 reportés.

---

# File active — 20 octobre 2026, après le retour V1.73

Branche **debug** uniquement. V1.73 est construite, contrôlée et installée. L'utilisateur répond **« 1. marche / 2. marche / 3. marche / prêt »** à la suite des trois essais V1.73 : conservation du choix des montures au chargement, réglages DOF et contrôle visée/tir/dash. Ces essais sont **VALIDÉS par l'utilisateur** ; aucun cas supplémentaire non décrit n'est supposé testé.

## Travail fonctionnel restant

| Sujet | État et résultat attendu |
| --- | --- |
| OFF indépendant pour chaque pistolet | **OUVERT.** Retirer modèle, effets de cette arme et bras tendu ; conserver projectiles et gameplay. Les choix de modèles gauche/droite et les holsters sont validés. Le nom Shoot_Right ne désigne pas la main : certains montages font tirer les deux armes. Aucun OFF partiel livré. |
| Nouvelle approche Action Recovery | **À REPRENDRE APRÈS LE RESTE.** Commencer par le ground slam, conformément à la décision utilisateur. Le candidat dash sans bénéfice est retiré. Les anciennes demandes dash/coffres restent des pistes ultérieures, sans correctif actif ni résultat annoncé. |

## Reporté ou seulement à confirmer

| Sujet | Décision |
| --- | --- |
| HUD général après refus d'arène verrouillée | **SURVEILLANCE / NON REPRODUIT en V1.73.** L'utilisateur confirme HUD toujours visible. Relevés avant/après complets, sans erreur ; nouvelle racine visible/opacité1, bShowHUD vrai, pile UI vide. Aucun correctif dédié, cause historique inconnue. Reprendre seulement si le défaut revient. |
| Autres icônes au-dessus des personnages et effets de tir plats/mal orientés en P2 | **REPORTÉS expressément par l'utilisateur.** Conserver la correction validée des indications d'interaction ; identifier les autres éléments avant d'intervenir. |
| Autres éléments HUD supposés manquants | Signalement incertain, lié à l'étude HUD ; aucun élément précisément identifié à restaurer. |
| Auto check update | Fonction déjà codée : vérification au démarrage du canal choisi, notification temporaire et rappel jaune. **Retour visuel spécifique encore absent** ; reste un essai, pas une fonction à recoder d'office. |

## Acquis à préserver

Caméra TPS/native, souris/manette et réaffectations ; profils pied/cheval/ultime et coop ; visée/tir/dash ; ressources, munitions/Hotstreak, vie/potions ; grappin, sauts/plané/sprint ; rangement et trois délais ; modèles des pistolets et holsters ; skins bonus et six variantes de progression ; durée ultime/infinie ; Léviathan ; flou et indications d'interaction ; skip warning et mises à jour. Les validations historiques restent bornées aux essais rapportés.

Le nettoyage du code, le retrait des diagnostics devenus inutiles, la cohérence du menu et la documentation accompagnent chaque correction. Ils ne doivent pas réintroduire les candidats rejetés ni modifier les raccourcis personnels.

Préserver INI, raccourcis, PAK, sauvegardes, main/archive et tous les anciens ZIP. Regrouper les changements cohérents ; aucune nouvelle build de diagnostic seule. [Audit OFF/HUD](AUDIT_PISTOL_OFF_HUD_2026_10_20.md) · [État courant](CURRENT_STATE.md) · [Historique intégral de la précédente file](history/ACTIVE_QUEUE_THROUGH_AUDIT_2026_10_20.md).

## Orientation Stinger - CLOTUREE le 25 octobre 2026

V1.98 : cristal dans le dos, bien attache, comportement vanilla accepte par l'utilisateur. Conserver le rendu natif ; aucune reattache ni correction vers l'avant. Releve arrete. Validation limitee a Stinger/Stoneskin ; gestes des pouvoirs encore a tester.
