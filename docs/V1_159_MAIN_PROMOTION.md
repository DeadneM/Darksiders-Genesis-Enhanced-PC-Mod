# V1.159 — promotion cumulative sur main

## Autorisation et base

Le 6 novembre 2026, l'utilisateur demande explicitement « publie sur main et build ». Base debug acceptée : `063219c788e83686abaee0e46ab70f3fdc3d5473`, source binaire V1.158 `5f1591a85630ea0af128b4bd5c25996072bd5e3a`. Main précédent : `563ae9ca8c2647689af8464d320544afe55b1894` (V1.153), conservé dans l'historique.

Le nouveau numéro V1.159 permet une compilation stable sans écraser l'archive V1.158 déjà publiée. Hormis `src/BuildInfo.h` (numéro, identifiant et nom d'archive), les sources de gameplay, tests, configuration, updater et fichiers de compilation sont identiques à la base debug. Aucun correctif spéculatif ajouté lors de cette promotion.

## Contenu cumulatif

- Effets de poussière de dash natifs, validés sur Strife à pied.
- Fonctions du mod maintenues sous son menu ; édition souris et orbite manette séparées, menu accepté.
- Matériaux pierre/feu et écharpe Stoneskin acceptés.
- Reprise bornée des passifs natifs du cheval au chargement : retour utilisateur favorable, avec sa réserve.
- Tous les acquis antérieurs de V1.153, défauts et raccourcis conservés. GPU après fermeture déjà accepté.

## Limites conservées

L'aura Molten de Strife fonctionne à chaque essai du dernier relevé ; son ancienne absence intermittente n'a pas été reproduite et aucune nouvelle cause n'est déclarée corrigée. Coop/P2 reste à tester à la fin selon le choix utilisateur. Les mesures CPU inclusives des journaux ne sont pas des mesures de FPS. [Essai de l'aura et limites](V1_158_FINAL_REVIEW.md).

## Objectif, contrôle et livraison

Objectif : compiler sur GitHub Windows x64 la base acceptée, puis publier un ZIP stable découvrable par l'updater sur main. Critères : 82 suites existantes réussies ; aucune erreur de compilation ; ZIP `DarksidersGenesis_V1.159_STABLE_CUMULATIVE.zip` contenant uniquement `DarksidersGenesisMod.asi` et `dxgi.dll` à la racine ; téléchargement public et sélection de version vérifiés. Aucun INI/README dans le ZIP ; le jeu recrée l'INI absent et garde un INI existant. Aucun fichier local du jeu n'est installé par cette demande.

Statut à la préparation : **PROMOTION AUTORISÉE ; CONSTRUCTION ET CONTRÔLES À VENIR**, pas encore annoncée construite. Le reçu final précisera le commit compilé, l'exécution GitHub, le lien immuable et les empreintes.

Rollback : V1.158 debug et son archive restent disponibles ; main V1.153 reste dans l'historique. Les anciennes archives, la branche archive et le tag v1.43 sont préservés. Pas de nouvelle release ni tag créés par cette seule promotion.
