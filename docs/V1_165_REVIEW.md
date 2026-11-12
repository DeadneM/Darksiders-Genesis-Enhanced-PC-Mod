# V1.165 — optimisation de la découverte et écharpe double face

## Base, objectif et réussite

Base VALIDÉE : V1.164, source `6e8fc9339454303f242cf82f7c111ac8b6241eff`, main `e0ac8a44a9fc79425218c452efab7128acecb56f`. Candidat écharpe/reprise des ressources préparé depuis sur debug, dernier état `0ae04fed80a9deee2aad5c8534a71d7a79776c1e`. L'utilisateur autorise maintenant les points 1/2 et une build debug ; le report de compilation est levé pour ce lot. Main/archive restent intacts.

Objectif : réduire les lectures répétitives de la découverte des capacités et les vérifications de chargement inutiles, tout en testant le candidat écharpe déjà préparé. Réussite : contrôles Windows complets, deux binaires x64 au ZIP racine, configuration/acquis identiques ; puis en jeu, capacités disponibles après chargements, stabilité et coût comparables, écharpe entière visible des deux côtés avec feu/restauration corrects. Gain FPS NON démontré par les tests statiques.

## Changements groupés

- **Découverte des capacités :** le pointeur de chunk UObject est conservé uniquement sur la pile d'une tranche de scan. L'en-tête objet/flags de 16 octets est lu ensemble au lieu de deux lectures séparées. L'accès à la classe de l'objet reste contrôlé. Passage de chunk, flags unreachable/pending-kill et bornes restent vérifiés ; aucun pointeur de chunk conservé entre callbacks. Budget 256 objets / environ 1 ms et cursor/reprise existants conservés.
- **Classe ParticleSystem :** identité faible index/serial mémorisée comme les classes de séquence/matériau déjà présentes. Une fois découverte, comparaison de pointeur à la place de la lecture de son nom pour chaque objet. Invalidation de l'identité avant chaque tranche ; aucun simple cache d'adresse perpétuel.
- **Reprise Stoneskin :** ne consulte plus chargeur/état UI si toutes les demandes manquantes sont en délai ou épuisées. Scope renouvelé avant cette sortie, même délai initial, trois essais, priorité et budget partagé. Préparé dans le commit debug f0a83fc, encore à compiler.
- **Écharpe :** candidat double face cuit, MID privé, hiérarchie de textures du skin équipé et paramètres de feu ciblés ; repli vers le revêtement précédent si le contrat échoue. Corps minéral, explosions/cendres et aura de lave distincts conservés. [Détails et limites](NEXT_STONESKIN_SCARF_REVIEW.md). Coût de la copie complète par création du tissu encore à mesurer.

Le dernier bilan local V1.164 du 12 novembre : gameplay-service inclusif moy. 1 122,08 µs / max. échantillonné 22 721,70 µs ; wrath-discovery moy. 759,06 µs ; appearance-discovery 144,76 µs ; Stoneskin 18,30 µs. Échantillonnage 1/64, mesures imbriquées non additionnables. Ces durées orientent le travail, pas un gain V1.165 ni une attribution GPU.

## Bugs natifs étudiés

[Revue des notes officielles et des fiches Epic](V1_165_NATIVE_BUG_REVIEW.md). Pas de nouveau bug moteur démontré et patché aveuglément. Moloch validé conservé, aucun nouveau hook de rendu ni modification de sauvegarde/registre/cache/pilote.

## Préservation et contrôles

Sol invisible et comportement V1.164 des chevaux inchangés ; fonctions validées, caméra/visée/souris/manette, capacités/coûts/animations, skins et raccourcis conservés. 250 valeurs et 55 actions Hotkeys inchangées. Aucune installation/relance ou modification de l'INI personnel. ZIP neuf `DarksidersGenesis_V1.165_DISCOVERY_CLOTH_TEST.zip`, ASI/dxgi uniquement, anciennes archives intactes.

Tests ajoutés aux suites existantes : nombre de lectures par tranche, frontière entre deux chunks, flags et indices invalides, nouvelle table au callback suivant, lecture échouée récupérable ; classe ParticleSystem réutilisée avec nouveau serial ; cas d'écharpe/Abyssal/MID natif et de reprise préparés précédemment. **Contrôles GitHub à venir**, aucun résultat inventé avant compilation.

## Essai utilisateur regroupé après livraison

1. Strife à pied : même scène, immobile puis déplacement/tir, avec et sans Stoneskin. Comparer la fluidité ; noter les pics visibles à l'activation et au chargement.
2. Changer de niveau puis de personnage : les capacités transférées restent disponibles, sans nouveau blocage ou perte d'effet. Aucun changement de leur mécanique n'est attendu.
3. Écharpe Stoneskin Native puis Abyssale : avant/arrière, tissu complet, feu Molten puis restauration à la fin. Vérifier le premier lancement après chargement.
4. Régression courte : cheval au sol puis marche aérienne/saut/chute, et un passage TPS/vue native/menu/roue.

Rollback : V1.164 déjà validée sur main, INI personnel conservé. Nouvelle bêta EXPÉRIMENTALE en jeu jusqu'au retour utilisateur.
