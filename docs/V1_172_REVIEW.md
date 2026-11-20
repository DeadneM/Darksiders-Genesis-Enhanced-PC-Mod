# V1.172 — appel du cheval en l’air

Base fonctionnelle VALIDÉE : V1.171. Travail exclusivement sur debug, main/release V1.168 inchangées. Demande : une case sous Horse Everywhere pour appeler la monture en l’air. Construction GitHub uniquement, aucune installation automatique, INI/raccourcis personnels préservés.

## Option

Gameplay → Horse and riding → **Summon horse in the air**, immédiatement sous Horse Everywhere. OFF par défaut pour conserver le comportement validé ; clé Features/HorseAirSummon=0. Les 258 anciennes valeurs de référence et les 55 actions Hotkeys ne changent pas. Aucune touche codée en dur : l’action native d’appel commune au clavier et à la manette est conservée. Les restrictions de zone restent réglées séparément par Horse Everywhere.

## Contrat et portée

Le Blueprint d’appel utilise SpawnHorseMount à sa fin. Les entrées DoSummonHorseMount des personnages passent par l’action native ; le contrôle observé avant son lancement exige un résultat positif d’un rayon vers le sol. Le code natif copie la position obtenue puis ajoute 100 unités en hauteur. Les signatures et instructions du binaire supporté, le résultat de 16 octets et son booléen à +12 sont vérifiés hors jeu. [Preuve statique](evidence/V1_172_AIR_SUMMON_STATIC.json).

Le candidat intercepte le calcul de sol existant, appelle toujours l’original avec les mêmes arguments et ne remplace son résultat que depuis l’unique site d’appel de l’action cheval. Le joueur doit être local, à pied, possédé réciproquement et autoritaire ; seuls chute/saut (mode Falling) et plané natif actif sont acceptés, hors pause/cinématique. Le résultat temporaire fournit la position aérienne actuelle, compensée des 100 unités que le code natif ajoute ensuite. Pas d’écriture de mode de déplacement, de position d’acteur, de montage ou de données de sauvegarde ; pas de deuxième commande, création forcée ou boucle de respawn. Les autres utilisateurs de la fonction, les appels au sol et le démontage gardent leurs résultats natifs. Signatures inconnues : option indisponible.

Les vérifications antérieures de région, déblocage et délai, puis le lancement d’aptitude et la création native du cheval, restent en place. Il peut donc subsister un autre refus natif pendant certaines actions ; aucune réussite en jeu n’est annoncée avant essai. Les tests simulés ne valident ni le rendu, ni la transition animée, ni la collision réelle à l’arrivée de la monture.

## Lot cumulatif et performances

Inclut la petite optimisation CPU préparée précédemment : pas de verrou/table de suivi PS tant qu’aucun shader de visée n’a été reconnu. Liaison native et détection exactes conservées. Aucun gain FPS, baisse de charge GPU ou disparition des micro-saccades promis. [Diagnostic conservé](GPU_FREEZE_REVIEW_2026_11_19.md). Aucun autre réglage de rendu ajouté ou modifié.

## Critères et tests

Contrôles de build attendus : suites Windows existantes incluant les cas air/sol, OFF, joueur étranger, P1/P2, possession rompue, mauvais appelant, plané, pause/cinématique et position invalide ; génération/persistance du réglage. ZIP cumulatif à deux binaires, sans INI/README, toutes les anciennes archives conservées.

1. OFF : appel et démontage au sol inchangés ; absence d’autorisation supplémentaire en l’air.
2. ON : avec Strife puis War, sauter/chuter et appeler via l’action habituelle. Le cheval doit apparaître à la hauteur actuelle et rester contrôlable. Tester le plané si possible.
3. Zone interdite : vérifier la combinaison avec les modes Horse Everywhere ; cette case seule ne doit pas ouvrir les zones.
4. Vérifier les quatre skins, saut et marche aérienne déjà validés, puis OFF et chargement de partie. Coop locale en dernier, séparément pour P1/P2.
5. Surveiller les micro-saccades dans la même scène, sans conclure à un gain sur la seule utilisation GPU. Aucun essai d’autre chargeur requis.

Statut avant CI : PRÉPARÉ / EXPÉRIMENTAL. Retour simple : désactiver la case, ou rétablir les deux binaires V1.171 validés en gardant son INI. L’optimisation CPU et l’appel aérien ne sont pas encore validés en jeu.
