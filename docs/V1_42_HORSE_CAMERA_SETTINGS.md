# V1.42 — réglages TPS distincts à cheval

Date : 16 octobre 2026. Branche **debug**. Base livrée : V1.41 `04ca27e4a0b440eaa646b22cafbab55e3938b84e`. Les validations antérieures sont conservées ; le nettoyage V1.41 reste identifié comme expérimental en attente de retour utilisateur.

## Objectif et critère de réussite

Ajouter, comme pour la forme ultime, **distance** et **décalage vertical** propres à la caméra TPS à cheval. Modifier la vue montée doit laisser les valeurs à pied et ultime indépendantes ; descendre de cheval doit retrouver la vue à pied existante. Garder l'orbite, la visée, les commandes reconfigurables et les transitions natives déjà en place.

Statut final : **CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL**. Source `bf02da3`, CI Windows `38875417060` réussie,13/13 tests ; publication `64c4e93`. Aucune validation en jeu revendiquée.

## Menu et configuration

Dans **Camera → Third Person**, sous les paramètres de forme ultime : **Horseback TPS camera**.

- **Distance (Horseback)** : 0 à 3× ; curseur, saisie manuelle et Default.
- **Vertical offset (Horseback)** : −1000 à +1000 unités ; curseur, saisie manuelle et Default.
- Les nouveaux réglages sont inclus dans **TPS Camera Values Default** et le reset complet. Défauts neufs : distance0.15 et hauteur0, identiques à la vue normale par défaut de V1.41.

INI `[Values]` : `TPSHorseDistanceMultiplier`, `TPSHorseHeightOffset`. Un ancien INI sans ces clés hérite de **sa propre distance et hauteur à pied** au chargement. Les nouvelles clés, une fois sauvegardées, deviennent indépendantes. Les valeurs non finies ou illisibles reviennent aux réglages à pied ; les valeurs finies hors limites sont bornées. Les 110 réglages préexistants et Inser restent inchangés.

Le pitch, le FOV, l'orbite et le recentrage gardent leur fonctionnement commun actuel, comme avec les deux paramètres supplémentaires de forme ultime.

## Application et périmètre préservé

La sélection repose sur **usingHorse**, après validation de la pose de monture existante dans le callback caméra. Priorité : forme ultime reconnue, sinon monture reconnue, sinon pied. Aucun nouveau hook, pointeur conservé, événement clavier/manette ni changement de détection monté/démonté.

Trois lignes ajoutées au calcul existant : distance sélectionnée pour le seuil de caméra, distance appliquée à la vue et hauteur appliquée à la vue. Le seuil et la géométrie utilisent ainsi le même profil. La provenance de pose, les gardes de pause/roues/cinématique et le raccourci temporaire de vue native restent identiques. La sélection est celle de la pose déjà reconnue ; sa transition existante n'est pas remplacée par un test de touche de monture.

Comparaison à V1.41 : gameplay du fichier principal (RawInput, XInput, visée), HorseFeature, NativeTPS, ChaosPose, règles des sticks, munitions, jauges, Synergy, Skip Warning, DXGI et moteur updater inchangés. Le fichier principal actualise seulement les messages de version/configuration au démarrage et expose les deux valeurs cheval dans le journal. Source caméra identique après retrait des trois sélections des nouvelles valeurs. Action Recovery reste retiré et non réparé.

## Correction du numéro affiché dans Updates

Capture utilisateur du 16 octobre : **Running mod: V1.22**, alors que Latest affiche V1.41 et le statut reconnaît la version courante. La correction V1.41 de la comparaison updater était bien présente ; le texte de l'onglet Updates était resté écrit en dur. Cet oubli de l'audit V1.41 est confirmé dans OverlayUi.cpp.

Le libellé utilise désormais **BuildInfo::Version**, identique à l'identité de compilation qui alimente la comparaison. La V1.42 doit donc afficher **Running mod: V1.42**, puis suivre les versions suivantes sans nouveau texte figé. Le préfixe « V1.22 CONFIG loaded » du journal est également remplacé par la version compilée. Les commentaires historiques de changements V1.22 sont conservés comme historique ; ils ne représentent pas la version exécutée.

## Contrôles réalisés

Tests Windows existants, avec extension du vrai cycle Store/Win32 : héritage d'un ancien INI, persistance des trois vues distinctes, publication runtime, modification ultérieure de la vue pied sans écraser cheval/ultime, limites et valeurs non finies, remise à zéro. Le test des défauts compare maintenant **112 clés** livrées/générées, dont les deux nouvelles. Les tests de comparaison updater suivent la version compilée plutôt qu'un numéro futur figé.

Analyse statique MSVC conservée. Packaging x64 cumulatif : `DarksidersGenesis_V1.42_HORSE_CAMERA_SETTINGS_TEST.zip`, exactement ASI + DXGI + INI + README à la racine. Les **105 archives antérieures**, main et archive sont protégés.

## Installation et retour arrière

Jeu ouvert au début de cette demande : aucune modification de ses fichiers pendant la préparation. Après construction et fermeture du jeu : sauvegarde des quatre fichiers et du log, installation des binaires vérifiés ; INI personnel conservé intégralement byte pour byte. Les deux clés manquantes héritent des valeurs à pied au chargement et sont créées lors du prochain enregistrement des réglages. Les clés cheval déjà présentes sont conservées. Aucun jeu lancé automatiquement.

Rollback : jeu fermé, restaurer les quatre fichiers de la sauvegarde ou les binaires V1.41 en conservant l'INI ; les nouvelles clés sont ignorées par V1.41. Les anciennes archives ne sont pas écrasées.

## Test utilisateur et suite

1. Régler une distance et une hauteur à cheval nettement différentes de celles à pied et ultimes.
2. Monter, tourner la caméra et tirer souris/manette ; descendre et vérifier le retour aux valeurs à pied.
3. Vérifier la forme ultime, les trois roues, la pause, le raccourci temporaire et une cinématique.
4. Sauvegarder/recharger puis vérifier la persistance des trois profils.

Validation du cadrage/transitions en jeu nécessaire. Les limites ouvertes de V1.41 et l'enquête Action Recovery restent dans le TODO cumulatif ; cette version ajoute seulement le profil caméra monté demandé.


## Reçu de livraison

[Actions38875417060](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38875417060), job116889177088 : SUCCESS, **13/13 suites**, notamment UltimateCameraConfig étendu aux trois profils et UserDefaultsConfig112 clés. Les huit avertissements MSVC sont de mêmes codes et même portée qu'en V1.41 (shadowing caméra désormais ligne708, autres emplacements conservés) ; aucun masquage ni nouvelle promesse d'analyse sans avertissements.

ZIP 806041 octets, SHA256 `3C47E465DAF03D7C9AE8B6767045A1444CCA03C0A37D67BC15AE30B89F76E2FC`. ASI1184256 octets SHA256 `687BD4E1972229D8A751C5C540B9D1BF81AFE3CF7042996657CBF744C90CE950`, DXGI155648 octets SHA256 `53600F742684DD2550F60A450035AF6AF5534CAB0DC09A07197AE5FC144668D9`. Quatre entrées, AMD64, identité1.42, libellé « Running mod: %s » et chaînes des réglages cheval contrôlés ; ancien libellé « Running mod: V1.22 » absent du binaire. Cela prouve la construction du chemin d'affichage, pas une capture en jeu.

Sauvegarde `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v142-20261016-104624` ; quatre fichiers antérieurs et log présents. INI avant/après `CD18319DD0468D35E7E0BCDA3A7EEEE9C76F8B7689131C0CA9AD75E313518AE9` / `CD18319DD0468D35E7E0BCDA3A7EEEE9C76F8B7689131C0CA9AD75E313518AE9`.105 archives antérieures identiques ; main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` inchangées. Jeu fermé au remplacement ; aucun lancement automatique.

La question Action Recovery est suivie dans [l'investigation dédiée](ACTION_RECOVERY_NEXT_INVESTIGATION.md) ; aucun retour de l'ancien hook sans effet dans ce build. Le protocole de test caméra ci-dessus reste à valider par l'utilisateur.
