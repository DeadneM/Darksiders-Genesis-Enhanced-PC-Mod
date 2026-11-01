# V1.137 — ressources Stoneskin et référence de visée conservée

1er novembre 2026. **PRÉPARÉE / EXPÉRIMENTALE ; contrôle GitHub et essai requis.** Base cumulative V1.136 installée ; ses autres fonctions validées sont conservées. La visée Statique reste ouverte. Cette version ne prétend pas corriger son verrouillage excessif.

## Objectif et critère de réussite

Éviter qu'une recherche de ressources terminée empêche la peau de pierre/feu de Strife d'apparaître après un chargement. Réussite attendue : le revêtement et ses effets de début/fin sont retrouvés pendant un statut Stoneskin valide, sans relancer son animation, sans modifier ses statistiques et sans remplacer les matériaux temporaires natifs du niveau. Tester notamment le boss appelé « Dagan » par l'utilisateur ; sa cause exacte n'est pas encore capturée.

## Correction des ressources

La table d'objets UE peut réutiliser un indice déjà parcouru sans changer son nombre d'entrées. Le scan partagé pouvait alors rester « terminé » alors que des matériaux venaient de charger dans cet indice. Un revêtement actif qui manque de ressources peut maintenant demander une nouvelle passe ; le budget existant reste 256 objets / environ 1 ms par tranche, avec le service habituel à 15 ms. Le redémarrage d'une passe terminée est espacé d'au moins une seconde. Aucun chargement forcé, scan complet synchrone, nouvelle allocation ou nouvel appel moteur n'est ajouté. Dès les ressources nécessaires disponibles, la demande s'arrête ; hors revêtement actif, ce correctif ne relance aucune passe.

La vérification de dépendances est aussi corrigée : le corps minéral utilise toujours le matériau de base, y compris avec l'écharpe Molten. L'ancien garde exigeait au contraire le matériau de corps Molten, puis pouvait lancer une fabrique avec le vrai matériau de base manquant et mémoriser cet échec. L'attente porte désormais sur le corps de base et l'écharpe effectivement nécessaires. Un matériau arrivé plus tard peut être appliqué pendant le même statut.

Des messages limités distinguent ressources manquantes et override de matériau natif/utilisateur conservé. Les shaders, l'aspect pierre/feu validé, les gestes et timings Geyser/Rampage/Caltrops/Stoneskin, les statistiques et les paramètres de particules restent inchangés. Les overrides inconnus restent protégés ; leur remplacement n'est pas une correction annoncée ici.

## Résultat du comparatif Statique et correction du nettoyage

La capture précédente contient 1 770 échantillons sur 180 secondes, sans erreur de lecture mémoire : réglage OFF, 55 faisceaux affectés à l'ennemi sur 84 observés en vue native ; 60 sur 65 en TPS. Les directions/cibles varient : ces proportions ne mesurent pas une différence d'intensité. Elles confirment avec le retour utilisateur que le phénomène existe dans les deux vues. La vue native gardait l'ASI chargée : aucune référence « jeu sans mod » n'est déclarée vérifiée.

Le journal complet de cette session compte ensuite **1 124 points observés** par le résolveur. La suppression globale de ce mécanisme dans le nettoyage `588af56` était donc trop large ; compilation et 70 tests ne la justifiaient pas. Aucun binaire de ce nettoyage n'a été livré ou installé. V1.137 conserve le résolveur et les portées des capacités/du solveur Lava qui peuvent effectivement l'utiliser. Le pistolet à pied observé rend le rotateur déjà enregistré : son test n'invente plus un appel au résolveur. La petite portée générique reste aussi conservée pour les interfaces des autres poses, notamment les montures, dont ce chemin n'est pas démontré identique ; ceux des capacités restent des contrôles de contrat, pas des preuves d'exécution en jeu.

Un compteur séparé `NATIVE_POINT_ACCEPTED` relève désormais les points acceptés par la recherche de cible TPS, avant la réussite éventuelle du solveur balistique, distinctement des simples observations du résolveur, qui peuvent inclure des contextes finalement refusés. L'option d'acquisition OFF passe directement au sélecteur original sans demander de rayon TPS ni modifier ses arguments. À ON/100 les angles restent originaux. Aucun cache de cible V1.135, filtre de contact Statique, changement de portée, dégâts ou chaînes n'est réintroduit. L'efficacité de l'ajustement sur toutes les munitions reste à vérifier ; ces seuls compteurs ne la prouvent pas.

La valeur `FLT_MAX` répétée dans le point explicite du faisceau est sa sentinelle native « non défini » ; ce ne sont pas des cibles lointaines. Le récapitulatif a été corrigé en conséquence. [Capture et journal](evidence/NATIVE_COMPARISON_2026_11_01.json).

## Vérification prévue et limites

Fixtures : tableau UE de taille constante avec réutilisation d'un ancien indice après scan terminé ; absence de nouvelle passe sans demande et respect du délai ; matériau du corps de base manquant puis retrouvé ; matériau Molten du corps non requis ; dépendances FX tardives ; restauration, transformations, P1/P2 et matériaux inconnus toujours protégés. L'appel natif de sélection OFF est vérifié avec les mêmes arguments et sans fournisseur TPS. Les calculs de collision/relâchement chargé et de paire angle-vitesse Lava restent testés.

Compilation Windows x64 via GitHub Actions uniquement. ZIP cumulatif à quatre fichiers racine, INI/55 Hotkeys conservés, aucune installation automatique. main/archive et anciennes archives conservées. Les contrôles ne valident pas l'apparence pendant Dagan ni la cause historique de l'accrochage Statique.

## Essai regroupé après installation manuelle

1. Avec Strife, activer Stoneskin après chargement d'une mission, puis dans le niveau du boss Dagan : vérifier corps minéral, écharpe en feu selon le cœur, explosion de départ et cendres à l'expiration complète. Comparer le premier lancement aux suivants.
2. Recharger la partie ou changer de niveau, refaire le même pouvoir, puis vérifier le retour à l'armure normale. Ne pas confondre un override du niveau volontairement conservé avec une ressource absente : le journal les distingue.
3. Contrôle de non-régression court : Geyser, Rampage et un tir chargé/Lava en TPS. Vérifier Statique/chaînes sans annoncer son bug résolu ; conserver son retour pour l'enquête native.

Rollback : archive V1.136 conservée, avec l'INI personnel. La coopération reste à vérifier séparément quand l'utilisateur le souhaite.
