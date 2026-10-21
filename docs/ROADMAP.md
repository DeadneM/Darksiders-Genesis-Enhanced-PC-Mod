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

## Reprise du 20 octobre 2026

La [file active](ACTIVE_QUEUE.md) remplace les anciens états ci-dessous. V1.67 : pistolets validés, skins bonus en échec. V1.68 en préparation : bon contrôle de skins, continuité de focus, durée ultime infinie et visuel Léviathan. HUD après arène verrouillée ouvert ; autres absences HUD incertaines ; OFF complet pistolet non livré. [Preuves et critères V1.68](V1_68_VISUALS_ULTIMATE_TEST.md).

---

# Roadmap active

19 octobre 2026. [File active](ACTIVE_QUEUE.md) : référence unique des tâches et statuts. [État de la dernière livraison](CURRENT_STATE.md). Le retour V1.66 valide les sauts et holsters dans l’essai utilisateur ; le travail documentaire courant ne livre pas une nouvelle build.

## Base retenue : V1.66 validée dans l’essai utilisateur

Retour explicite du 19 octobre 2026 : sauts et holsters « c’est bon », puis « tout a lair bon/ ». Conserver leur code V1.66 et les acquis V1.65 (trois délais de rangement et durée ultime). Ce retour clôt les deux sujets dans l’essai rapporté ; il ne décrit pas une couverture exhaustive des modes, personnages et situations coop. Auto check update n’a pas de confirmation visuelle distincte.

## Action Recovery : repartir du bon étage

Ancienne fin anticipée retirée depuis V1.58. Nouvelle étude centrée sur la demande native de déplacement et son interruption, avec séparation des actions, causes, masques et requêtes en attente. Le jeu possède un chemin qui conserve son nettoyage et le motif d’interruption ; son emploi utile et sûr dans le cas rapporté n’est pas établi. [Preuves et relevé discriminant préparé](NEXT_RECOVERY_AND_MODELS.md).

Critère : reprendre le déplacement plus tôt après la phase utile du dash, sans glissement, distance/vitesse raccourcie, état temporaire persistant ni différence imprévue TPS/native. Observer le troisième et le quatrième dash Strife ; inclure ensuite War/P2. Les coffres restent différés. Ni simple autorisation MOVE, ni ancien EndAbility(0), ni touche/stick physique ciblé.

## Pistolets et skins : lot V1.67 construit et contrôlé

Le besoin précisé consiste à choisir l’un des deux pistolets existants par main. Deux listes et les variantes de holster correspondantes sont préparées. Deux cases distinctes ciblent la disponibilité des montures Rampage/Despair et des six apparences de progression, sans attribution de récompense par le mod. Construction Windows x64 vérifiée : 42/42 suites, zéro avertissement MSVC, ZIP à quatre fichiers contrôlé. Nouveaux comportements expérimentaux, essais dans le jeu à effectuer. [Inventaire, preuves et limites V1.67](V1_67_PISTOL_CHOICES_AND_SKINS.md). Aucun asset privé du jeu dans le dépôt.

## Conservations et éléments différés

- Autres sprites/effets P2 : différés explicitement ; prompts d’interaction validés conservés.
- Auto check update : implémenté, retour visuel encore absent ; ne pas compter ce diagnostic comme une fonction nouvelle.
- Fonctions validées et INI/raccourcis : conserver. Pas de promotion stable ni de remplacement d’ancienne archive.
- Prochain lot : regrouper les changements retenus et vérifiables à partir de la base V1.66 retenue, donner un objectif mesurable et une version distincte.

Tout l’ancien plan est conservé dans [l’historique de roadmap jusqu’à V1.66](history/ROADMAP_THROUGH_V1_66.md). Les demandes, expériences rejetées et anciens protocoles y restent consultables ; leurs anciens états n’annulent pas cette file.
