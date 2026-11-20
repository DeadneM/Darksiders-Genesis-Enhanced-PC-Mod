# V1.173 — optimisation CPU sans délai supplémentaire

Base cumulative : V1.172, fonctions V1.171 validées ; main/release V1.168 inchangées. Source précédente debug c11ea66, focus P1 contrôlé dans 4e32651 déjà inclus. L'appel aérien V1.172 reste expérimental.

## Objectif et réussite

Retirer du travail répétitif sans changer réglages/défauts, rendu, commandes, cadence de vérification, intercepts ni mise à jour dynamique des skins/effets. Aucun cache inter-image ou délai ajouté. Réussite : contrôles Windows puis comparaison dans la même scène à réglages identiques, sans retard ni régression visuelle. Aucun gain FPS/GPU annoncé avant mesure.

## Changements retenus

- Sept hooks de dessin conservés, filtrage et contrôles du shader réel inchangés. Seul le comptage atomique du nombre total de draws est supprimé ; statistiques shaders reconnus/dessins masqués toujours présentes. Le journal indique explicitement drawCalls=not-collected. Pas de remplacement par une file/cache supplémentaire.
- Empreinte CRC32 shader à table équivalente et préfiltre des tailles dérivé du catalogue. La création native et son comptage restent exécutés ; les tailles impossibles évitent le calcul du CRC. Aucune signature, empreinte secondaire ou règle de masquage modifiée. Interception de chaque création inchangée.
- Identité UObject : lecture groupée exacte des champs du même enregistrement ; si refus du bloc, retour aux lectures séparées actuelles. Flags, index, serial, GC, recyclage d'adresse et protections conservés ; pas de cache durable de pointeurs/pages.
- Les deux chemins TPS P2 réutilisent le focus déjà calculé, comme les trois chemins P1. Caméra, blur, pause et réglages conservés. La région de flou dominée évite déjà des calculs depuis 4e32651.

Non inclus : changement de fréquence des scans/services/entrées, cache XInput, budget temporel supplémentaire, réorganisation des verrous shader, sauvegarde asynchrone, changement DOF/GPU. Ces pistes restent à mesurer ; ne pas payer une optimisation par un retard de changement dynamique.

## Contrôles et test utilisateur

Tests CRC contre l'algorithme précédent et signatures/tailles ; identités/GC/recyclage et fallback aux frontières ; tests caméra de parité octet par octet, isolation P1/P2 et suites existantes. Compilation uniquement GitHub Windows Release. État avant CI : PRÉPARÉ / EXPÉRIMENTAL, aucun nouveau test exécuté localement.

Après installation manuelle en gardant l'INI :
1. Même scène/mêmes réglages : comparer micro-saccades et rendu du flou, sans se fier au seul pourcentage GPU.
2. Changer rapidement skins/effets ; changer de personnage, charger une partie, monter/descendre et utiliser l'ultime. Vérifier application/restauration réactive, sans ancienne apparence ni effet manquant.
3. Vérifier traits/arcs, tirs et World Ender ; pause/roues puis retour au jeu. Ne pas retester toutes les fonctions sans symptôme nouveau.
4. Coop en dernier : caméra P2 et pause, réglages indépendants.

ZIP cumulatif attendu : DarksidersGenesis_V1.173_REACTIVE_CPU_OPTIMIZATION_TEST.zip, deux binaires AMD64 seuls à la racine, sans INI/README. Aucune installation/lancement automatique. Rollback : remettre les deux binaires V1.172 en conservant profil/raccourcis/sauvegardes. Main/archive et anciens ZIP préservés.
