# V1.51 — visée 3D optionnelle et vue neutre horizontale

## Base et retour utilisateur

Base de travail : V1.50 debug, `c9432c7`, avec les acquis V1.48/V1.49 conservés. **V1.50 n'est pas une base entièrement validée** : Charge Shot, Graviton et Lava restent rejetés pour leur direction trop basse. Le retour utilisateur valide les roues, l'équipement et la continuité de caméra lors du changement de personnage ; ces mécanismes ne sont pas réécrits. Recovery produit désormais un effet constaté, décrit comme la répétition du premier dash. Cinématiques encore à vérifier ; transformations/caméras coop et autres fonctions paraissent bonnes, sans validation générale déduite.

Objectif de cette build : comparer une visée 3D recentrée horizontalement avec un mode simple conservant la trajectoire native. Critère de réussite : Charge Shot/Graviton, normal et Hotstreak, et Lava normal ne partent plus vers le sol au recentrage ; incliner volontairement la vue permet encore de viser haut/bas en 3D ; OFF retrouve le comportement simple. **EXPÉRIMENTAL : aucun impact en jeu validé avant essai utilisateur.**

## Preuves du journal V1.50

Session du 17 octobre, 19:09:47–19:49:35, copiée sans modification dans l'audit local. Au total : pistolets corrigés P1=569/P2=158 ; Finish P1=11619/P2=15 ; Lava recalculés P1=0/P2=0, **1623 rejets**. Des rotations de pistolet atteignent environ −21° chez P1 et −34° chez P2 ; ce sont des rotations calculées, pas des mesures d'impact. Les logs ne permettent pas d'attribuer chaque tir à une munition précise.

Recovery observe 139 phases P1 et 7 P2, avec autant de demandes de fin natives. Cela confirme l'exécution du chemin, le retour utilisateur confirme son effet ; cela ne prouve ni la bonne progression des trois dashs ni une correction des réceptions. Les roues enregistrent bien des ouvertures/fermetures pour les deux joueurs. Les traces actuelles ne contiennent pas la chaîne d'entrée nécessaire pour expliquer le stick devenu inopérant après dash.

## Changements

- **Case « 3D aiming (experimental) », activée par défaut**, section caméra Third Person. Clé `Features/TPS3DAim=1`, défaut compilé ON si absente d'un ancien INI. Save/reload/Reset et publication runtime raccordés ; aucun nouvel ID Hotkey, aucun ancien binding déplacé.
- **ON en Camera Forward : séparation position/orientation.** La position reste calculée avec l'inclinaison de cadrage personnelle et l'orbite, les mêmes distance/hauteur/offset. L'orientation réellement rendue utilise l'inclinaison d'orbite : un recentrage donne 0°, pas le −12° de cadrage. Le rayon de collision utilise exactement cette orientation rendue, chez P1/P2. La vue regarde donc plus haut qu'en V1.50 au recentrage ; le personnage doit se placer plus bas dans l'image. Pas d'ajout fixe de quinze degrés à tous les projectiles, pas de rotation du corps écrite en mémoire, pas de rayons décalés invisibles par rapport à la vue.
- **OFF : mode simple.** Getters pistolet/Finish et cache rotation/vitesse Lava conservent intégralement leurs sorties natives ; les overrides de cible 3D souris sont aussi désactivés. La direction de regard Camera Forward à plat reste commune aux commandes souris/manette. Les calculs natifs de hauteur/auto-ciblage et de trajectoire ne sont plus remplacés. Le cadrage/orientation de caméra précédent est restauré. Ce mode reprend le mécanisme simple, sans prétendre reproduire octet pour octet tout un ancien ASI ni corriger ses limites historiques. Le mode distinct Vanilla Aim garde sa pose précédente.
- **Lava : contrôle des composants par défaut corrigé.** V1.50 appliquait au ProjectileMovement trouvé sur le CDO une exclusion destinée aux instances jouées. Les flags `RF_ClassDefaultObject=0x10` et `RF_ArchetypeObject=0x20` sont légitimes sur ces modèles ; le test accepte désormais cette provenance précise et continue de vérifier le type. Les instances arme/capacité restent exclues si modèles. Les objets en destruction sont refusés. Les bornes natives négatives finies sont acceptées si ordonnées et dans ±89°. La paire rotation/vitesse reste atomiquement décidée après un calcul entièrement valide. **Cause plausible des rejets V1.50, pas preuve de leur motif exact**, absent de l'ancien journal. Les nouveaux rejets donnent leur étape et les flags observés. [Référence des flags Unreal](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/CoreUObject/EObjectFlags).
- Nettoyage : nom local d'identité caméra renommé ; compteurs de tirs indexés explicitement par P1/P2 pour supprimer les ambiguïtés de l'analyse statique.
- **Visée native après dash : observation en lecture seule**, dans le getter original existant avant les filtres TPS. Pendant trois secondes après un dash reconnu, échantillons espacés de 250 ms, maximum 96 par joueur/session : vecteur de regard natif, sortie originale, état du mouvement, première capacité active. ON/OFF Recovery sont observables. Aucun nouveau hook, aucune touche imposée, aucune remise à zéro des états de visée. Ce relevé ne constitue pas une réparation.

## Ce qui reste ouvert

1. Confirmer en jeu l'alignement 3D, la parallaxe proche et les variantes de projectile. Un rayon horizontal depuis une caméra surélevée peut donner une légère élévation depuis l'origine du personnage vers une collision proche ; la référence native 66A3B0 n'est pas prouvée comme socket de bouche de chaque projectile.
2. Le stick qui ne permet plus de viser après dash en vue du dessus : problème de commande, pas seulement de trait/arc. Comparer Recovery OFF/ON ; aucun shader supplémentaire modifié dans cette build.
3. La réception lourde/frappe au sol après un dash en hauteur. La routine actuelle ne termine que la décélération d'un dash identifié. La capacité de réception et sa fenêtre interruptible n'ont pas encore été prouvées ; pas d'arrêt arbitraire de toutes les animations. Coffres toujours hors périmètre.
4. Progression native du triple dash et éventuel effet de répétition du premier : préserver le mouvement et vérifier les états temporaires à la sortie. Pas déclaré résolu.
5. Finir les essais cinématiques et coop laissés en attente par l'utilisateur.

## Tests prioritaires

En mission, commencer par **recentrer**. L'orbite conservée de la session reste une visée volontairement inclinée.

| # | Essai | Résultat attendu / observation nécessaire |
| --- | --- | --- |
| 1 | 3D ON, recentrage, pistolets/Charge Shot/Graviton, immobile puis en reculant, normal puis Hotstreak | Plus de plongée involontaire au recentrage ; caméra et tir alignés |
| 2 | ON, viser volontairement haut puis bas ; essayer cible proche puis éloignée | Déplacement vertical cohérent ; aucun départ latéral lié au déplacement |
| 3 | Lava normal en cloche, ON : sol/ennemi proche, loin puis ciel | Arc et portée utilisables ; logs `LAVA_HIT_ARC`/`LAVA_FREE_ARC`, ou motif précis de rejet |
| 4 | Décocher 3D, refaire les trois munitions ; ennemis en hauteur/en contrebas | Mode simple disponible immédiatement ; hauteur/trajectoire natives ; sauvegarde/rechargement conserve OFF |
| 5 | Vérification courte ON/OFF souris et manette ; P2, cheval et Strife ultime | Même option par joueur/mode, pas de vol de caméra ni de régression des roues/équipement |
| 6 | Vue du dessus : viser au stick, dash, continuer à viser ; Recovery OFF puis ON | Relever quand les sticks cessent de viser ; nouveaux logs en lecture seule, bug encore ouvert |
| 7 | Triple dash puis quatrième, et dash depuis une hauteur avec réception lourde | Décrire reprise du déplacement/attaque et conservation des trois dashs ; réception non corrigée ici |
| 8 | Cinématique courte/longue jusqu'au retour, puis transformations coop | Terminer les vérifications encore en attente ; ne pas déduire une validation des tests logiciels |

Roues, maintien équipement et échange Strife/War : simple contrôle de non-régression, sans reprendre tous les essais déjà validés. Ressources/refills/Unlimited, INI, raccourcis, RawInput/Alt-Tab, Skip Warning et chemins de contrôle antérieurs restent conservés.

## Livraison et retour arrière

Build Windows x64 attendue via GitHub Actions, 26 suites ; contrôles réels et empreintes ajoutés après exécution. ZIP cumulatif à quatre fichiers racine : ASI, dxgi.dll, INI, README. INI personnel préservé octet pour octet lors de l'installation ; l'option absente prend ON sans modifier ce fichier à l'installation. Anciennes archives, main/archive et release v1.43 conservées.

Rollback rapide de la visée : décocher 3D. Pour revenir intégralement à la V1.50 sauvegardée, restaurer ses binaires et README, garder l'INI courant ; sa visée reste rejetée. Aucun lancement automatique du jeu.
