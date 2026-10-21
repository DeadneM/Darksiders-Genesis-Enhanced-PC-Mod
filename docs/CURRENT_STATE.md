## V1.77 — construite et contrôlée, réaffectation native du Courroux

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
