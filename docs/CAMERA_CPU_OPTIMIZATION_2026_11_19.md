

## Optimisation caméra sans changement de réglages — 19 novembre 2026

Décision utilisateur : REJETÉ, le passage automatique de TPSCustomDOF sur OFF. Ne modifier ni le profil personnel ni les défauts. La suite porte exclusivement sur du travail CPU évitable, à rendu conservé.

Depuis debug 3505391 / binaire V1.172 : les trois chemins TPS réutilisent leur compensation de mise au point réussie pour éviter son second calcul dans ApplyBlur. La projection des bornes est omise lorsque la région choisie atteint déjà 20000 unités, plafond du résultat qui aurait été calculé. Aucun hook, réglage, fréquence de service, qualité GPU ou ressource persistante ajouté. Le chemin natif conserve son contrat. Retour arrière : rétablir TpsCameraFocus.h et les trois appels de CameraTraceFeature.cpp depuis 3505391.

CONTRÔLE À EFFECTUER sur GitHub uniquement : tests existants et 20 480 combinaisons comparant tous les octets de la sortie caméra avec l'ancienne implémentation, pour les chemins TPS et natif. Aucun gain FPS/GPU ni suppression des saccades annoncé ; validation en jeu encore requise. Pas de nouveau ZIP ni installation pour ce contrôle.


Contrôle terminé : Windows x64 Release, 86/86 tests réussis, zéro avertissement/erreur C++. Les 20 480 combinaisons du test de parité comparent le chemin TPS combiné et le chemin natif indépendant, octet par octet, avec le calcul précédent. Source 4e3265180592a447d5de3c74c12504895b9b6fbe. [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44600037723). Statut : CONSTRUIT et CONTRÔLÉ, gain de fluidité EXPÉRIMENTAL, non validé en jeu. Vérifier dans la même scène, avec les mêmes réglages : aspect/mise au point inchangés et éventuelle différence de saccades, à pied/cheval/ultime et pendant les roues. Aucun ZIP publié ou installé ; V1.172 reste la dernière livraison. Main/archive/v1.168 inchangés après vérification distante.
