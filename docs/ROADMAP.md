## Reprise du 20 octobre 2026

La [file active](ACTIVE_QUEUE.md) remplace les anciens états ci-dessous. V1.67 : pistolets validés, skins bonus en échec. V1.68 en préparation : bon contrôle de skins, continuité de focus, durée ultime infinie et visuel Léviathan. HUD après arène verrouillée ouvert ; autres absences HUD incertaines ; OFF complet pistolet non livré. [Preuves et critères V1.68](V1_68_VISUALS_ULTIMATE_TEST.md).

---

# Roadmap active

19 octobre 2026. [File active](ACTIVE_QUEUE.md) : référence unique des tâches et statuts. [État de la dernière livraison](CURRENT_STATE.md). Le retour V1.66 valide les sauts et holsters dans l’essai utilisateur ; le travail documentaire courant ne livre pas une nouvelle build.

## Base retenue : V1.66 validée dans l’essai utilisateur

Retour explicite du 19 octobre 2026 : sauts et holsters « c’est bon », puis « tout a lair bon/ ». Conserver leur code V1.66 et les acquis V1.65 (trois délais de rangement et durée ultime). Ce retour clôt les deux sujets dans l’essai rapporté ; il ne décrit pas une couverture exhaustive des modes, personnages et situations coop. Auto check update n’a pas de confirmation visuelle distincte.

## Action Recovery : repartir du bon étage

Ancienne fin anticipée retirée depuis V1.58. Nouvelle étude centrée sur la demande native de déplacement et son interruption, avec séparation des actions, causes, masques et requêtes en attente. Le jeu possède un chemin qui conserve son nettoyage et le motif d’interruption ; son emploi utile et sûr dans le cas rapporté n’est pas établi. [Preuves et relevé discriminant préparé](NEXT_RECOVERY_AND_MODELS.md).

Critère : reprendre le déplacement plus tôt après la phase utile du dash, sans glissement, distance/vitesse raccourcie, état temporaire persistant ni différence imprévue TPS/native. Observer le troisième et le quatrième dash Strife ; inclure ensuite War/P2. Les coffres restent différés. Ni simple autorisation MOVE, ni ancien EndAbility(0), ni touche/stick physique ciblé.

## Pistolets et skins : lot V1.67 construit et contrôlé

Le besoin précisé consiste à choisir l’un des deux pistolets existants par main. Deux listes et les variantes de holster correspondantes sont préparées. Deux cases distinctes ciblent la disponibilité des montures Rampage/Despair et des six apparences de progression, sans attribution de récompense par le mod. Construction Windows x64 vérifiée : 42/42 suites, zéro avertissement MSVC, ZIP à quatre fichiers contrôlé. Nouveaux comportements expérimentaux, essais dans le jeu à effectuer. [Inventaire, preuves et limites V1.67](V1_67_PISTOL_CHOICES_AND_SKINS.md). Aucun asset privé du jeu dans le dépôt.

## Conservations et éléments différés

- Autres sprites/effets P2 : différés explicitement ; prompts d’interaction validés conservés.
- Auto check update : implémenté, retour visuel encore absent ; ne pas compter ce diagnostic comme une fonction nouvelle.
- Fonctions validées et INI/raccourcis : conserver. Pas de promotion stable ni de remplacement d’ancienne archive.
- Prochain lot : regrouper les changements retenus et vérifiables à partir de la base V1.66 retenue, donner un objectif mesurable et une version distincte.

Tout l’ancien plan est conservé dans [l’historique de roadmap jusqu’à V1.66](history/ROADMAP_THROUGH_V1_66.md). Les demandes, expériences rejetées et anciens protocoles y restent consultables ; leurs anciens états n’annulent pas cette file.
