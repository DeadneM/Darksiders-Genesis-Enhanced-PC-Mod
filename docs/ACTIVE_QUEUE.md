# File active — 19 octobre 2026

Ce tableau remplace les files anciennes conservées dans l’historique. Statut VALIDÉ réservé aux retours explicites de l’utilisateur. Base retenue : **V1.66 debug, sauts et holsters VALIDÉS dans l’essai utilisateur du 19 octobre 2026**. Nouveau lot V1.67 préparé pour les choix de pistolet et les skins, expérimental ; construction et essai dans le jeu restent à effectuer. [Détails](V1_67_PISTOL_CHOICES_AND_SKINS.md).

| Sujet | État actuel | Prochaine étape / critère |
| --- | --- | --- |
| Sauts supplémentaires | VALIDÉS dans l’essai utilisateur V1.66 ; ancien échec V1.65 remplacé par ce retour | Conserver le cycle natif validé ; rouvrir seulement sur nouveau défaut rapporté |
| Holsters montés | VALIDÉS dans l’essai utilisateur V1.66 ; placement V1.65 déjà acquis | Conserver les modèles et la recherche anticipée validés |
| Trois délais de rangement | VALIDÉS par l’utilisateur | Préserver lames Strife / pistolets Strife / épée War et raccourcis communs |
| Durée ultime | VALIDÉE après activation de sa case ; faux échec rétracté par l’utilisateur | Préserver le débit natif et sa priorité Unlimited |
| Action Recovery | Ancien système retiré ; nouvelle piste d’interruption native étudiée, non implémentée | Corréler intention de déplacement, requête en attente, masque d’interruption et phase physique avant tout candidat |
| Modèles des pistolets | Demande précisée : choisir gauche/droite par main, deux listes préparées V1.67 | Vérifier combinaisons, prise en main, tir/rangement, variantes montées et retour Native |
| Skins bonus / progression | Deux montures et six variantes de personnage identifiées ; deux cases préparées V1.67, OFF par défaut | Tester la page Skins native et l’indépendance des groupes ; PAK existant prioritaire pour ses déblocages |
| Autres sprites/effets P2 | REPORTÉS sur demande explicite de l’utilisateur | Ne pas intervenir ; prompts d’interaction déjà validés |
| Auto check update | Construit/contrôlé V1.63, retour visuel explicite absent | Confirmation visuelle distincte encore attendue pour l’avis temporaire et le rappel jaune du canal choisi |
| Grappin | VALIDÉ dans l’essai utilisateur V1.63 | Conserver ABI des quatre arguments ; pas de validation implicite de toutes les traversées |
| Visée, reprise AIM, présentation du tir de dash, focus TPS, Standard fini, ressources et Hotkeys | Acquis rapportés conservés | Surveillance des régressions ; ne pas rouvrir un défaut sans nouveau retour |

## Ordre de travail

1. Conserver V1.66 comme base retenue avec sauts et holsters validés ; ne rouvrir ces sujets que sur un nouveau défaut rapporté. La validation ne présume pas tous les modes, personnages ou états coop testés.
2. Préparer Recovery à partir du chemin natif d’interruption et de ses vraies raisons. Un booléen vrai, une impulsion ou un compteur de diagnostic ne prouve pas une fonction.
3. Construire et vérifier V1.67, puis tester les combinaisons gauche/droite et les deux groupes de skins. Aucun remplacement des armes en main à partir des meshes Stowed.
4. Réunir les changements retenus dans un prochain lot cohérent, avec version distincte, rollback et contrôles Windows/ZIP. Garder les modèles/effets P2 différés.

[Preuves nouvelles et limites](NEXT_RECOVERY_AND_MODELS.md) · [Plan](ROADMAP.md) · [État courant](CURRENT_STATE.md). Les états précédents sont conservés dans [l’ancienne roadmap](history/ROADMAP_THROUGH_V1_66.md).
