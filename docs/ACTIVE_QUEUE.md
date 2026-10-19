# File active — 19 octobre 2026

Ce tableau remplace les files anciennes conservées dans l’historique. Statut VALIDÉ réservé aux retours explicites de l’utilisateur. Dernier lot livré : **V1.66 debug, retour attendu**. Pas de nouvelle version publiée pendant ce travail de préparation.

| Sujet | État actuel | Prochaine étape / critère |
| --- | --- | --- |
| Sauts supplémentaires | Échec V1.65 : deux remontées sans animation ; candidat V1.66 construit/contrôlé, expérimental | Retour des essais Always/After gliding/OFF ; dix sauts animés et répétables |
| Holsters montés | Placement VALIDÉ V1.65, petit délai ; recherche anticipée V1.66 expérimentale | Vérifier délai à la première montée puis tir/rangement/descente |
| Trois délais de rangement | VALIDÉS par l’utilisateur | Préserver lames Strife / pistolets Strife / épée War et raccourcis communs |
| Durée ultime | VALIDÉE après activation de sa case ; faux échec rétracté par l’utilisateur | Préserver le débit natif et sa priorité Unlimited |
| Action Recovery | Ancien système retiré ; nouvelle piste d’interruption native étudiée, non implémentée | Corréler intention de déplacement, requête en attente, masque d’interruption et phase physique avant tout candidat |
| Modèles 3D des pistolets | Quatre géométries main/rangement ; aucun autre modèle de pistolet natif compatible identifié dans l’inventaire élargi | Choisir/fournir une géométrie compatible ou une source d’asset avant d’ajouter un vrai sélecteur |
| Autres sprites/effets P2 | REPORTÉS sur demande explicite de l’utilisateur | Ne pas intervenir ; prompts d’interaction déjà validés |
| Auto check update | Construit/contrôlé V1.63, retour visuel explicite absent | Avis temporaire et rappel jaune pour le canal choisi ; aucun nouvel essai demandé pendant le test V1.66 |
| Grappin | VALIDÉ dans l’essai utilisateur V1.63 | Conserver ABI des quatre arguments ; pas de validation implicite de toutes les traversées |
| Visée, reprise AIM, présentation du tir de dash, focus TPS, Standard fini, ressources et Hotkeys | Acquis rapportés conservés | Surveillance des régressions ; ne pas rouvrir un défaut sans nouveau retour |

## Ordre de travail

1. Exploiter le retour V1.66 ; ne pas superposer un second mécanisme de saut pendant l’essai.
2. Préparer Recovery à partir du chemin natif d’interruption et de ses vraies raisons. Un booléen vrai, une impulsion ou un compteur de diagnostic ne prouve pas une fonction.
3. Personnalisation des pistolets après choix d’asset et contrôle main/rangement/cheval. Aucun remplacement des armes en main à partir des meshes Stowed.
4. Réunir les changements retenus dans un prochain lot cohérent, avec version distincte, rollback et contrôles Windows/ZIP. Garder les modèles/effets P2 différés.

[Preuves nouvelles et limites](NEXT_RECOVERY_AND_MODELS.md) · [Plan](ROADMAP.md) · [État courant](CURRENT_STATE.md). Les états précédents sont conservés dans [l’ancienne roadmap](history/ROADMAP_THROUGH_V1_66.md).
