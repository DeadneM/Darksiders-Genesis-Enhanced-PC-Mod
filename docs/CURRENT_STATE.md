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
