# Roadmap active

19 octobre 2026. [File active](ACTIVE_QUEUE.md) : référence unique des tâches et statuts. [État de la dernière livraison](CURRENT_STATE.md). Le retour V1.66 est attendu ; le travail documentaire courant ne livre pas une nouvelle build.

## En test utilisateur : V1.66

Sauts OFF / Always dix nouveaux sauts avec animation / After gliding un saut par plané ; comparaison TPS/native et atterrissage. Holsters : délai de première montée après quelques secondes à pied puis tir/rangement/descente. Placement V1.65, trois délais et durée ultime déjà validés. Ne pas annoncer la réparation des sauts sur les seules fixtures ou la CI.

## Action Recovery : repartir du bon étage

Ancienne fin anticipée retirée depuis V1.58. Nouvelle étude centrée sur la demande native de déplacement et son interruption, avec séparation des actions, causes, masques et requêtes en attente. Le jeu possède un chemin qui conserve son nettoyage et le motif d’interruption ; son emploi utile et sûr dans le cas rapporté n’est pas établi. [Preuves et relevé discriminant préparé](NEXT_RECOVERY_AND_MODELS.md).

Critère : reprendre le déplacement plus tôt après la phase utile du dash, sans glissement, distance/vitesse raccourcie, état temporaire persistant ni différence imprévue TPS/native. Observer le troisième et le quatrième dash Strife ; inclure ensuite War/P2. Les coffres restent différés. Ni simple autorisation MOVE, ni ancien EndAbility(0), ni touche/stick physique ciblé.

## Modèles de pistolets

L’inventaire élargi du PAK distingue quatre StaticMesh de pistolets, les matériaux et les modèles de corps Gold/Silver/Abyssal. Aucun autre pistolet 3D compatible n’est établi. Prochain besoin : choix d’une géométrie et vérification de ses références/axes/attachements. Conserver les variantes main/rangement et la correction montée validée. Pas de faux sélecteur ni d’assets privés du jeu dans le dépôt.

## Conservations et éléments différés

- Autres sprites/effets P2 : différés explicitement ; prompts d’interaction validés conservés.
- Auto check update : implémenté, retour visuel encore absent ; ne pas compter ce diagnostic comme une fonction nouvelle.
- Fonctions validées et INI/raccourcis : conserver. Pas de promotion stable ni de remplacement d’ancienne archive.
- Prochain lot : regrouper les changements retenus et vérifiables après le retour en cours, donner un objectif mesurable et une version distincte.

Tout l’ancien plan est conservé dans [l’historique de roadmap jusqu’à V1.66](history/ROADMAP_THROUGH_V1_66.md). Les demandes, expériences rejetées et anciens protocoles y restent consultables ; leurs anciens états n’annulent pas cette file.
