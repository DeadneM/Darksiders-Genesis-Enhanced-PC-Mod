# V1.133 - visibilite et profondeur de l'aide TPS

PREPAREE / CONTROLES GITHUB EN ATTENTE ; NON VALIDEE EN JEU. Base acceptee : V1.130. V1.132 visibilite a ete construite par GitHub avant la fin de la revue de profondeur ; son archive est conservee, sans installation ni validation en jeu. Les curseurs Global / Per ammunition V1.131 sont conserves, mais leur effet positif est rejete par le retour utilisateur. Il avait augmente la valeur puis remis zero : le dernier INI a zero ne prouve pas un essai desactive.

## Probleme et correction limitee

Le journal V1.131 confirme le chargement et zero application P1/P2. Il ne conserve pas les valeurs successives ni les raisons de refus. Le code exigeait un hit BLOQUANT du rayon Projectile sur le meme ennemi selectionne. Le DefaultEngine.ini original verifie declare Pawn=Overlap et CharacterMesh=Ignore pour ce canal. Un ennemi peut donc etre eligible et devant la camera sans jamais satisfaire ce garde. Les fixtures V1.131 imposaient artificiellement un contact bloquant : elles ne couvraient pas ce contrat natif.

Le garde de visibilite accepte maintenant une requete native reussie sans obstacle avant la cible, meme si elle touche le decor derriere. Un contact direct avec la cible reste valide ; un autre obstacle plus proche, une requete indisponible ou une identite expiree refusent l'aide. Ni le canal ni les profils du moteur ne sont modifies. Pas de nouveau hook, scan ou ecriture d'angle de personnage. La priorite du rayon original a une collision proche, le cone de 6 degres et l'interpolation 0..100 restent identiques. Zero reste la base V1.130, sans nouvelle requete de cible/visibilite. Le contact physique strict de Statique V1.130 reste distinct et inchange.

Le point 3D interpole conserve la profondeur du candidat au lieu de reprendre le decor/ciel derriere lui lorsque Projectile traverse son modele. Le rayon partiel est encore verifie : une collision plus proche gagne. Cette profondeur sert a la convergence depuis le canon et a l'entree du solveur natif Lava, dont les formules restent identiques. Le booleen de cible interne indique ici un point de visee fini pour la balistique ; aucun FHitResult du moteur ni contact physique de Statique n'est invente.

## Journal et tests

Le journal observe les valeurs effectivement utilisees par les tirs (lignes bornees) et compte les refus : aide OFF, munition inconnue, cible native absente/invalide, geometrie ou collision originale proche, requete indisponible, obstacle sur le rayon vers la cible, application. Il ne mesure pas les impacts. Pas d'affirmation que le garde trouve etait le seul blocage de cette session ; cible native disponible et cone restent a controler en jeu.

Deux passes confrontees : contrat natif/config et parite des acquis, puis geometrie et consommateurs. Tests etendus sur les profils non bloquants, decor derriere, obstacle devant, contact direct, erreur de requete et identites expirees. Les tests synthetiques ne remplacent pas le prochain essai. [Preuves et limites](evidence/V1_133_STATIC_REVIEW.json).

## Essai et retour arriere

Garder son INI. En TPS Camera Forward, ennemi visible a cote du centre : comparer Global 0 / 50 / 100, puis Per ammunition avec un type a 0 et un autre a 100. Tester d'abord Standard et Statique, puis autres munitions/Hotstreak. Un ennemi loin sur le bord ou derriere un obstacle ne doit pas attirer. Verifier le retour a zero, la vue native et la main vide pendant pause/roues. Fermer normalement le jeu pour obtenir les bilans. Coop plus tard. La capture externe preparee est uniquement en lecture, sans appels au jeu ; aucun jeu lance automatiquement.

227 valeurs et 55 actions Hotkeys conservees ; cameras, souris/manette, poses/modeles et solveur Lava inchanges. Compilation GitHub seulement ; aucune installation de V1.133 sans demande. Retour zero ou archive V1.130 validee. main V1.124, archive, anciennes archives et profil personnel preserves.
