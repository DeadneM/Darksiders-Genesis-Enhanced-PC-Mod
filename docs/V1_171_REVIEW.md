# V1.171 — beta cumulative et diagnostic du chargeur

Base stable : V1.168 VALIDÉE. V1.170 est construite/contrôlée mais son test utilisateur est reporté ; ses fonctions expérimentales restent identifiées comme telles dans ce lot cumulatif. Main/release intactes. Objectif de cette build : livrer le correctif préparé du journal sans remplacer l’archive V1.170. Aucun autre changement fonctionnel ajouté faute de preuve suffisante.

## Différence depuis V1.170

Dans le chargeur, le code d’erreur Windows de LoadLibraryW est capturé avant l’écriture de la chronologie, qui pouvait le remplacer. Le chargement, les exports et Skip Logos gardent leur comportement. Cela ne corrige pas et n’explique pas à lui seul l’ancien échec d’un chargeur alternatif. [Deux passes et preuves hors jeu](CONTINUATION_REVIEW_2026_11_19.md).

Le reste est identique à la source V1.170, hors identité de build : correctif expérimental de l’affichage des niveaux des cœurs (ON, désactivable), filtres flou de mouvement/franges colorées (OFF). Les 258 clés de référence et 55 actions Hotkeys restent identiques. Aucun patch SourceControl, budget VRAM ou DXVK ajouté, aucun gain FPS annoncé.

## Réussite attendue et tests

Contrôles de livraison : build GitHub Windows x64, suites existantes réussies, archive unique à deux binaires à la racine, versions et contenu vérifiés, anciens ZIP et branches protégées inchangés. Pas de compilation locale ni installation automatique. INI/README exclus du ZIP ; documentation conservée sur GitHub.

1. Démarrage normal, menu accessible, version V1.171 affichée.
2. Premier accès à l’arbre des cœurs après lancement/chargement : niveaux visibles sans revenir au menu précédent. Navigation et équipement normaux, Strife puis War.
3. Camera → Image effects : comparer séparément les deux filtres ON/OFF en vue native et TPS, puis leur indépendance du TAA. Vérifier le retour au rendu initial sur OFF.
4. Un chargement et une cinématique : caméra, HUD et réglages conservés.

Pas d’échec de chargement à provoquer pour tester le journal. Validation en jeu des fonctions nouvelles encore attendue. Rollback : désactiver les trois nouvelles options ou revenir aux deux binaires V1.168 validés, en conservant son INI. V1.170 reste téléchargeable à l’identique.
