# V1.140 — coût des traitements Strife et Stoneskin Abyssal

Statut : **CONSTRUITE / CONTRÔLÉE**, [70/70 contrôles Windows, zéro avertissement/erreur MSVC](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41323327523). **EXPÉRIMENTALE, à valider en jeu.** Base source cumulative V1.139 ; cette base n'est pas déclarée globalement validée. Les fonctions précédemment acceptées sont conservées. Branche `debug` uniquement.

## Objectif et réussite

Réduire du travail démontré répétitif, mesurer les autres coûts avant une optimisation plus intrusive, et rendre la peau de pierre/feu disponible après sélection de l'armure Abyssale. Réussite en jeu : Strife plus fluide sans perte de fonction ; revêtement présent pendant Stoneskin, puis armure d'origine correctement restaurée. Aucun gain de FPS n'est annoncé avant mesure/retour utilisateur.

## Preuves et correction visuelle

Le 1er novembre, l'utilisateur précise un ralentissement apparemment constant avec Strife à pied, même sans tirer. Le défaut Stoneskin apparaît à l'activation après changement d'armure.

La lecture externe autorisée après « prêt » utilise uniquement `QUERY_LIMITED_INFORMATION|VM_READ`, pendant trois minutes, sans appel moteur ni écriture dans le jeu. Les deux activations sont enregistrées, sans erreur de lecture : état Stoneskin/Molten actif sur `Strife_Abyssal`, matériaux natifs inchangés. Ses slots `Strife_Abyssal_Body` et `Strife_Abyssal_Cloth` étaient absents de la liste exacte de `Surface`. Le troisième slot `Cloth_LP` n'a pas de matériau et reste ignoré. L'inventaire du PAK natif vérifié par SHA-1 confirme ces noms.

Seuls ces deux noms sont ajoutés au chemin existant. Le corps reçoit le matériau minéral privé ; le tissu conserve le matériau de feu natif utilisé par le mod. Identités, appartenance, refus des overrides inconnus, restauration et effets de transition restent contrôlés. Aucun geste, statistique ou nouvel appel moteur. L'ajustement visuel aux UV de l'Abyssale reste à vérifier en jeu.

## Allègements limités

- Le journal terminé de V1.139 compte 828 686 716 appels de dessin, mais seulement 52 034 dessins masqués. Ce sont des compteurs, pas des durées. La lecture des réglages/vue/horloge du masquage n'est désormais faite qu'après correspondance avec un shader de trait/arc. Les autres dessins évitent ce calcul. La vérification du vrai shader avant suppression, nécessaire après ClearState/command lists, reste identique.
- Une recherche de champ Stoneskin validait auparavant les identités de toutes les entrées précédentes du cache. Sur un hit, seule l'identité complète de la classe correspondante est maintenant validée. Sur un miss, les entrées expirées sont récupérées. Aucune référence non vérifiée ni cache global de pages mémoire.

## Mesures

Un appel sur 64, par étape et par thread, est chronométré : service de gameplay, apparence des pistolets, échantillonnage d'animation, Stoneskin, découverte des pouvoirs/apparences et sélecteur de visée natif. Le rapport `PERF V1.140` donne moyenne/maximale en microsecondes et nombre d'échantillons ; au plus sept lignes cumulatives par minute, et un rapport final. Pas de scan externe continu pour mesurer la fluidité.

Ces temps sont **inclusifs** : ne pas additionner étapes imbriquées. Le sélecteur inclut aussi son appel original natif et ne mesure donc pas le seul surcoût du mod. Échantillonnage déterministe, maxima non exhaustifs, fréquence d'images non mesurée. Les bras désarmés continuent d'utiliser l'animation validée ; aucun calcul de trajectoire, portée Statique, XInput ou réglage de caméra n'est changé.

## Contrôles et essais

Fixtures : nouveaux slots Abyssal corps/tissu et restauration ; cache vivant et réutilisation de la même adresse avec un autre numéro de série. Suites existantes : états, P1/P2, ressources tardives, échec de fabrique, transformations, explosion/cendres. Deux passes de revue : comportement/portée, puis identités/threads/restauration et diff final. Compilation, analyse MSVC et suites Windows uniquement dans GitHub Actions.

À tester dans le même niveau, après installation manuelle en conservant l'INI :

1. Strife à pied : 60 secondes immobile, puis déplacement et tir. Comparer au ressenti V1.139 ; garder le journal pour les lignes PERF.
2. Même scène avec War, puis Strife à cheval. Éviter pause/menu pendant chaque minute de comparaison.
3. Équiper l'Abyssale, lancer Stoneskin, vérifier corps minéral/tissu en feu, explosion et disparition complète du revêtement ; refaire avec l'armure normale.
4. Vérifier le pistolet OFF et son bras pendant déplacement, pause/roues, ainsi que le masquage trait/arc dans les deux vues.

INI personnel, 228 clés, 55 actions Hotkeys, sauvegardes, PAK, anciennes archives et branches protégées conservés. Aucune installation ni lancement automatique. Rollback : remettre l'ASI/proxy V1.139 avec l'INI personnel inchangé. Si le ralentissement persiste, comparer les mesures avant de désactiver/remplacer une fonction validée.
