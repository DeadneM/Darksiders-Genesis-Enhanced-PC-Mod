# V1.174 — réglages d'image avec mode Native

Base V1.173 publiée sur main (0e5b182), comportement/réactivité/coop/appel aérien confirmés par le retour utilisateur du 20 novembre. Le crash DXGI_ERROR_DEVICE_HUNG reste non attribué ; le rendu TPS reste coûteux selon le retour utilisateur. Aucun gain GPU, correctif pilote ou résultat de crash inventé.

## Objectif et comportement

Remplacer les deux simples interrupteurs d'image par des réglages indépendants et homogènes, sans modifier le flou de profondeur de champ, la caméra, les entrées ou les cadences de contrôle. Aucun nouveau hook, pas d'Engine.ini modifié. Service CVar existant, pointeurs/enregistrement/consommateurs déjà vérifiés ; restauration conditionnelle et allocation courante conservées.

- Customize motion blur décoché : **Native (défaut)** ; le jeu décide selon la scène. Coché : curseur et saisie d'Amount de 0 à 1 ; 0 désactive. Valeur d'essai lors de l'activation : 0.5.
- Customize chromatic aberration décoché : **Native (défaut)**. Coché : curseur et saisie de Maximum amount de 0 à 10 ; 0 désactive. Valeur d'essai lors de l'activation : 5. Le paramètre vérifié r.SceneColorFringe.Max est une LIMITE : il ne crée pas de franges dans une scène qui n'en a pas.
- Native restaure exactement les CVar capturées et laisse leurs sentinelles/choix de scène fonctionner. Ce n'est PAS un multiplicateur « 1 = vanilla », car les valeurs natives comprennent un mode automatique négatif. Retour Native immédiat au prochain service normal, sans nouveau délai.

Deux clés Values : MotionBlurAmount et ColorFringeMax, -1 par défaut (Native). Les 259 anciennes clés/valeurs de référence sont conservées, 261 au total. Sans nouvelle clé, DisableMotionBlur/DisableColorFringe=1 devient 0 ; OFF devient Native. Nouvelles valeurs explicites prioritaires. Les anciennes clés restent lues pour migration et sont maintenues compatibles pour rollback binaire : true uniquement pour 0. Aucun INI personnel modifié par le développement et aucun INI dans le ZIP.

Le seul nettoyage associé est la génération de la table CRC par itération sur ses entrées plutôt que par indice : même table calculée à la compilation, mêmes tests CRC ; permet de retirer le faux positif C28020 sans désactiver l'analyseur.

## Contrôles et critères

Avant CI : PRÉPARÉ / EXPÉRIMENTAL. Tests étendus : valeurs, bornes, invalides→Native, deux ombres game/render, absence d'écritures répétées, restauration après changement, sauvegarde/chargement, migration anciennes cases et priorité des clés. Autres suites inchangées. Compilation Windows Release uniquement GitHub, pas de build local.

En jeu : tester séparément Native / 0 / valeur intermédiaire, puis retour Native ; vérifier chargement et cinéma dans une scène montrant l'effet. Les deux contrôles restent indépendants de TAA et DOF ; pas de baisse de qualité automatique pour cacher le coût TPS. Réactivité et fonctions V1.173 conservées. À comparer avant toute conclusion de performance.

ZIP cumulatif prévu : DarksidersGenesis_V1.174_NATIVE_IMAGE_CONTROLS_TEST.zip, ASI + dxgi.dll seuls à la racine, pas d'INI/README. Installation manuelle, aucune installation ou relance automatique. Main et archive protégées. Rollback : deux binaires V1.173 avec INI personnel conservé.
