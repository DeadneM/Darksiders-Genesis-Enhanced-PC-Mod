# V1.166 debug — options natives et lot écharpe / chevaux

## Base et objectif

Main reste la V1.164 validée (`e0ac8a44a9fc79425218c452efab7128acecb56f`). Debug part de V1.165 (`2f279f595ccc90b035711088b7a83b79a9abb5ea`) : retours utilisateur 1/3 acceptés, écharpe toujours rejetée. Les optimisations de découverte et le sol privé validé sont conservés.

Objectif du lot : rendre utilisable le matériau double face déjà prévu, ajouter les sabots pendant la marche aérienne et deux réglages natifs de compétences. Critères en jeu : écharpe complète et feu conservé ; pas synchronisés sans doublon au sol ; Contre-esquive OFF sans clone ni changement du dash ; synergies selon le mode choisi, coût unique et échange après la fin seulement.

## Changements

### Écharpe : refus démontré, sans nouvelle expérimentation de shader

La lecture de V1.165 a capturé le MID privé `War_Cloth_Gold_Inst_A`, avec `Emissive_Power` présent. L'association de paramètre est un **octet**, suivi de trois octets de padding non initialisés après copie. Valeur observée : `02 2d 31 27`. Le lecteur DWORD la prenait pour 657534210 et rejetait le matériau valide ; l'ancien revêtement opaque était rétabli. Lecture corrigée à un octet dans les quatre boucles de paramètres. L'association globale doit toujours être 2, l'index -1 ; noms et contrats restent vérifiés. Aucun nouveau shader ou réglage de culling, aucune copie par frame. Le rendu corrigé reste à voir en jeu.

### Pas des chevaux sur leur sol privé

Réutilise `char_ruin_foot_base`, SoundCue natif avec variantes, volume Sfx et atténuation native. Le callback sonore des foulées déclenche un seul son lorsque le cheval possédé marche sur son support privé. Pas de cadence artificielle, de boucle vent, de chargement supplémentaire ou de changement de physique. Arrêt, saut, vrai terrain, notifications VFX/décals et acteurs étrangers suivent leur chemin natif. Identité faible, possession réciproque, génération, fraîcheur et contrat CurrentFloor contrôlés. Les quatre classes et P2 utilisent le même chemin. Si la ressource manque, notification native conservée.

### Contre-esquive de Strife

Gameplay → Passives and synergy → **Strife counter-evade : Vanilla / OFF**, Vanilla par défaut. OFF rend fausse la requête native d'activation des deux identifiants `Strife_Ability_Counter1/2` pour Strife local à pied. Achats/sauvegardes, autres compétences, cœurs, dash/stomp et touches inchangés. Revenir à Vanilla restaure immédiatement la requête native.

La description française native précise : Contre-esquive II ajoute 20 % aux dégâts du clone. « Silhouette d'ombre » est la compétence de dash descendant ; son niveau II ajoute une seconde ruée à la réception. Ces compétences ne sont pas le pouvoir de Courroux Shadow Clone.

### Synergie : Vanilla / Inverted / No Swap

Gameplay → Passives and synergy → **Synergy swap**, Vanilla par défaut.

- Vanilla : chemin original intégral.
- Inverted : le personnage actuel utilise sa synergie ; une fois terminée, demande d'échange DEFAULT natif, sans deuxième synergie.
- No Swap : le personnage actuel utilise sa synergie et reste actif.

Le hook reçoit la demande serveur déjà classifiée `ECharacterSpawnType::SYNERGY=3`. Appui court, seuil d'appui long, action réaffectable et contrôles de progression restent natifs. Réutilise la routine de synergie du personnage courant déjà appelée par le jeu en coop (`0x65CD00`), avec consommation native de la jauge. Aucun modèle ou montage de pouvoir étranger.

La demande différée est attachée aux identités acteur/contrôleur/capacité, génération et monde. Elle attend OnFinish puis la sortie de la capacité active ; pas de temporisation arbitraire pour forcer l'échange. Mort, pause/menu natif, changement de monde/personnage/réglage, perte de focus ou délai dépassé annulent la demande. La coop conserve son comportement natif sans échange ; aucun autre joueur n'est dépossédé.

## Contrôles et deux revues

Première passe : contrats confrontés à l'EXE exact, scripts cuits et relevés VM_READ ; aucun appel moteur ou écriture par les lecteurs. Deuxième passe : chemins Vanilla, transitions, coût unique, accès P2 et annulation d'identités relus ; pas d'anciens correctifs retirés sans preuve. L'essai V1.165 était refusé par un problème de padding, pas par une absence d'alias ou une preuve de shader incompatible.

Tests préparés : fixture du padding réellement capturé ; paramètre non global rejeté ; pas sur les quatre chevaux, arrêt, saut, terrain réel, VFX, ressource manquante et P2 ; suppression ciblée des deux niveaux, progression étrangère inchangée ; fin unique de synergie, Inverted/No Swap, War/Strife, échecs et annulations. Nouveaux choix sauvegardés/chargés/publiés/réinitialisés et bornés. Compilation et exécution des contrôles exclusivement sur GitHub Windows ; voir le reçu de livraison pour le résultat final.

## Configuration et limites

Deux nouvelles clés seulement : `[Features] StrifeCounterEvade=1`, `[Values] SynergySwapMode=0`. Les 250 valeurs précédentes et 55 actions Hotkeys sont conservées. INI personnel, sauvegardes et PAK non modifiés. ZIP installable : ASI et dxgi.dll uniquement à la racine, sans INI/README. Anciennes archives et main/archive protégées.

**EXPÉRIMENTAL** : pas encore de validation visuelle, sonore ou de synergie de ce build par l'utilisateur ; contrôles isolés ≠ test en jeu ou mesure FPS. Aucune amélioration FPS chiffrée annoncée. Les sons ne sont pas remplacés si l'événement natif est mixte son/VFX ou si son contrat ne correspond pas.

## Essai regroupé

1. Stoneskin Strife, armures de base et Abyssale : envers et extrémités de l'écharpe, feu, retour exact après expiration.
2. Quatre chevaux : marcher/courir/sprinter dans le vide, s'arrêter, sauter, retomber au sol ; pas rythmés, aucun son répété à l'arrêt ou doublé au sol.
3. Contre-esquive achetée : comparer Vanilla/OFF en esquivant une attaque ; clone absent seulement sur OFF, dash/stomp inchangé.
4. Synergie pleine, chacun des deux héros : appui court normal, puis appui long dans les trois modes ; vérifier héros qui lance, animation complète, jauge consommée une fois, échange seulement avec Inverted. Refaire après retour Vanilla.
5. Pas besoin de refaire toute la coop maintenant ; validation P2 finale reste dans la file.

Suite : appliquer les retours du lot, puis autres apparences utiles et audit des ajouts récents. Aucun nouveau patch moteur spéculatif.
