# V1.96 - Lot des capacités et test groupé

Base cumulative V1.95. Objectif : proposer les trois transferts restants ensemble et retirer notre adaptation de Stoneskin. Caltrops/Geyser : effets et dégâts confirmés par l'utilisateur, animations incorrectes.

## Implémentation et limites

- Clone sur War : son propre ShadowCharacterComponent, issu du CDO natif et enregistré dans son monde ; callback natif SpawnShadowClone.
- World Ender sur War : son propre composant natif, configuré sur son Chaoseater, sans socket de pistolet étranger ; callbacks StartBeam/DestroyBeam. Origine, visibilité et direction à tester.
- Rampage sur Strife : départ et quatre fins Land/Slam Level1/2/3, avec callbacks/blasts natifs. Le mouvement enraciné du squelette War reste absent : charge et poses peuvent rester incorrectes.
- Montages privés et arrays non partagés ; réserve UE et événements remis sur la chronologie globale. Seuls callbacks, particules sur racine/aucun socket, sons ponctuels et SpawnBlast sont conservés. États de notification, sons en boucle, shakes, fenêtres d'interruption/rotation et sockets d'armes étrangers non transférés. Pose idle du receveur : animations encore à améliorer.
- Construction uniquement au lancement effectif ; propriétaire local, rôle autoritaire, personnage à pied, identité/serial contrôlés ; préparation au sol. Composants propres à l'acteur, jamais empruntés à l'autre joueur. Échec : fin native du lancement identifié et transfert concerné désactivé pour la session. Un composant construit avant un échec peut rester dormant jusqu'au remplacement du personnage ou à la fin de session.
- Stoneskin : adaptation entièrement supprimée, ancien choix 6 normalisé à Native. War conserve son pouvoir original via Native. Redémarrage complet nécessaire pour comparer le cristal ; sa cause reste inconnue.
- Trace Geyser temporaire supprimée. Aucun nouveau hook, timer de gameplay, raccourci, changement des tirs/caméras/contrôles, sauvegardes ou valeurs INI.

## Critère de réussite et essais groupés

1. Après redémarrage, War sur Native : comparer Stoneskin/cristal et noter si le défaut persiste.
2. War : Clone puis World Ender, un emplacement à la fois ; effet, dégâts, coût de Courroux en mission, fin et reprise du contrôle. Comparer native/TPS pour le rayon.
3. Strife : Rampage, appui bref puis maintenu ; départ, impacts, fin, coût et reprise du contrôle. Pose incorrecte connue ; immobilisation persistante = échec.
4. Refaire Caltrops sur War et Geyser sur Strife, puis remettre les emplacements sur Native et vérifier les pouvoirs propres.

P2 différé. Les contrôles automatiques ne prouvent ni rendu ni dégâts en jeu. Aucun lancement du jeu ni installation automatique.

## Installation et retour arrière

ZIP cumulatif Windows x64, quatre fichiers à la racine. Installation manuelle jeu fermé, en gardant son INI personnel, raccourcis, PAK et sauvegardes. Copier l'ancienne ASI/dxgi.dll avant remplacement. Retour arrière : remettre V1.95 ; les 160 anciennes archives restent intactes. Les six clés d'emplacements ne changent pas.