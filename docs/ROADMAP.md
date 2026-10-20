## V1.72 — montures sélectionnées au chargement et distance DOF étendue

Base cumulative V1.71 ; acquis validés conservés. **PRÉPARÉ / EXPÉRIMENTAL**, construction Windows et contrôle du paquet à effectuer. Retour V1.71 : le flou semble bon dans l'essai utilisateur ; le skin cheval reste perdu au rechargement, l'armure reste. Aucun nouveau verdict sur Dash movement recovery.

Objectifs : conserver Despair/Rampage déjà choisis lors du chargement même après désactivation de Bonus mount skins, comme les armures, et permettre une distance nette personnalisée supérieure à 1 000 m dans les deux vues. Limite commune portée à **5 000 m** (curseur, saisie, INI et rendu), valeurs par défaut 10 m/0,6 % inchangées. Aucun nouveau réglage ni raccourci.

Le filtre natif de chargement retirait les lignes marquées DLC avant leur copie au joueur. Le candidat laisse ce filtre fonctionner puis réinsère uniquement les deux choix bonus qu'il vient réellement de supprimer, avec le setter natif de skin. Pas de choix automatique, droit DLC, bonus de gameplay, modification de table ou écriture directe de fichier de sauvegarde. La case agit seulement sur la disponibilité du menu ; OFF ne retire pas un choix bonus déjà enregistré. Le modèle de base reste sélectionnable. Le diagnostic périodique de sélection V1.71 et son appel par image sont retirés : son rôle est terminé. **Nouveau résultat en jeu non validé.**

Critère de réussite : un bonus choisi reste sélectionné et rendu après chargement puis redémarrage, que l'option soit cochée ou décochée ; les choix d'armure, autres règles DLC et sauvegardes restent natifs. Sans choix bonus, aucun bonus n'est ajouté. OFF rend la disponibilité native du menu ; sélectionner le modèle de base remplace durablement le choix bonus. La distance 1 250,5 m puis 5 000 m doit survivre à la sauvegarde/relecture et déplacer la limite nette sans déplacer la caméra ou le personnage.

Essais groupés :

1. Choisir Despair et Rampage, laisser le jeu sauvegarder, recharger puis redémarrer avec Bonus mount skins coché. Vérifier sélection dans la page et monture réellement affichée. Décocher l'option puis recharger : bonus équipé conservé, disponibilité du menu native. Choisir ensuite le modèle de base et recharger : pas de retour bonus imposé.
2. Rendu automatique : arène → hub/mission, chapitre 8, changements de personnage et pied/cheval/ultime. Retour V1.71 encourageant, couverture complète non supposée.
3. Profils DOF Native/TPS : Disable puis Custom, valeurs différentes par vue, 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2. Saisie, redémarrage, roues/pause, cinématiques et coop.
4. Dash movement recovery V1.71 OFF/ON, Strife troisième/quatrième dash et War, visée/tir en déplacement, native/TPS. Toujours expérimental, OFF par défaut ; rejeter si glissement, aucun bénéfice ou régression de tir/visée.

OFF complet des pistolets reste ouvert : modèles validés conservés ; couches Shoot_Left/Right et couche partagée Spine1 relevées en lecture seule, pas de modification de pose ou de notifies. HUD général après refus d'arène verrouillée intermittent ; autres effets P2 reportés sur demande.

[Audit et preuves](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md). Installation : fermer le jeu, sauvegarder les anciens fichiers, conserver l'INI personnel et les raccourcis. Rollback vers V1.71 conservée ; main/archive et anciennes archives intacts. Aucun lancement du jeu par l'agent.

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
