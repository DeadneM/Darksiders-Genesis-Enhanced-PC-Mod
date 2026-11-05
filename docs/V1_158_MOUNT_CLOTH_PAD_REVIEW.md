# V1.158 — feu de l'écharpe, chargement à cheval et caméra manette sous le menu

## Base et retour utilisateur — 6 novembre 2026

Base cumulative debug V1.157 `6ff63f2bde6016e15df64ead28eda77b66ea294b`, code binaire `6b382f3d410826a13f189b2eabcccf7dfe58a0a4`. Base stable main V1.153 inchangée. Clics du menu VALIDÉS ; actions apparemment fonctionnelles selon l'utilisateur, avec sa réserve. Écharpe complète mais feu totalement absent : le remplacement de texture V1.156/V1.157 est REJETÉ pour ce rendu. Cheval toujours sans flammes : reprise V1.156/V1.157 NON VALIDÉE. L'utilisateur précise que cela semble survenir en chargeant directement un niveau déjà à cheval. Nouvelle demande : pouvoir tourner la caméra au stick pendant que le menu reste affiché. GPU déjà accepté ; coop réelle à la fin.

Objectif : garder l'écharpe complète et retrouver son feu ; restaurer les passifs du cheval lors de l'initialisation d'un niveau à cheval ; permettre l'orbite au stick sous le menu en gardant la souris pour les contrôles. Critère : rendu avant/pendant/après Stoneskin, premier cheval sans rappel, orbite manette sans bouger le curseur ni déclencher de recentrage parasite.

## Écharpe — retirer la substitution de texture rejetée

Les exports natifs montrent que la variante de base War_Inst_StoneSkin_B désactive les deux switches Emissive et Emissive_Scroll. La variante A conserve Emissive ; Molten A conserve Emissive et Emissive_Scroll. Les B imposent BLEND_Masked ; les A ne l'imposent pas. Modifier Base_Color sur B n'assure donc pas le feu et transfère d'autres paramètres de texture que le seul masque. [Comparaison complète des quatre sources](evidence/V1_158_NATIVE_STONE_MATERIALS.json).

La nouvelle proposition utilise les sources natives A et Molten A pour le tissu sans changer leur texture. Le corps conserve son parent A minéral et les deux suppressions d'émission uniquement sur ses MID privés. Un MID du corps et un MID du tissu restent distincts même avec le même parent ; aucun paramètre partagé ne change. Le feu, les couleurs et la couverture exacts restent EXPÉRIMENTAUX en jeu.

Les fonctions BaseColor/ClothTexture, le setter SetTextureParameterValue et leur échafaudage de test V1.156 sont retirés. Les trois anciens identifiants de sources matériaux sont remplacés par deux sources réellement utilisées ; demandes de ressources de six à cinq (deux matériaux, trois particules de transition). Fabrique/restauration des MID, armures, statuts, explosions et cendres conservés. Aucun nouveau hook ou appel moteur.

## Cheval — disponibilité native, pas premier passage définitif

L'observation déjà faite est réutilisée : 848 échantillons en lecture seule, tous les effets assignés du premier Despair inactifs, puis passifs actifs sur l'instance rappelée ; l'utilisateur confirmait le retour des effets. Aucune nouvelle capture n'est imposée après sa remarque « on a déjà fait ça ». Le journal V1.157 contient le démarrage correct mais aucun message de demande Mounted FX. Cela ne démontre pas à lui seul quel refus a eu lieu.

Trois conditions du code sont revues : (1) ne plus marquer checked avant d'avoir une liste native exploitable ; (2) borner le nombre d'éléments lus (16), sans rejeter une capacité d'allocation supérieure à 16 (capacité sanitaire maximale 128) ; (3) un passif déjà actif ne doit pas empêcher de reprendre les autres inactifs de la même liste. Emplacements sans template ignorés ; liste entièrement vide/non assignée = initialisation encore incomplète. Après deux secondes stables hors pause/roue/cinématique, au plus 32 contrôles espacés de 250 ms, puis arrêt. Une seule passe d'activation des passifs manquants, aucune boucle force-active permanente et aucun effet sprint/apparition/disparition rejoué.

Le service vérifie aussi directement la classe du Pawn possédé et sa réciprocité Controller/Pawn, plutôt que d'exiger un ancien personnage distinct ou le seul indicateur PoseKind. Une alias de personnage courant lors d'une possession initiale ne doit pas bloquer des effets locaux. La disposition globale des joueurs/caméras n'est pas modifiée. La capture V1.157 n'a pas relevé toutes ces conditions : le fait que chacune soit la cause sur ce chargement précis n'est pas annoncé confirmé. Les garde-fous de disponibilité sont démontrés par lecture du code et scénarios ciblés, leur résultat visuel reste à tester.

## Caméra — stick conservé, souris réservée à l'éditeur

Le retour anticipé de l'orbite P1 sous l'éditeur est supprimé. La contribution du joystick caméra configuré continue ; la contribution souris est mise à zéro par la politique commune quand le menu est affiché. Aucun recentrage du curseur Windows dans ce cas. Les commandes de recentrage/toggle restent capturées par l'éditeur, avec suivi des relâchements ; Alt-Tab et minimisation continuent d'annuler les deux contributions. La vitesse et le choix de stick existants restent appliqués.

P2 utilise la même règle : inputCaptured réserve les commandes, plus l'orbite analogique entière. Test d'isolation adapté ; ce contrôle n'est pas une validation coop réelle. Aucun changement du routage XInput RT/RB, du tir, des roues ou des affectations.

## Contrôles et livraison prévue

Contrôles étendus : arrivée tardive d'une liste avec capacité 27, templates tardifs, liste partiellement active, possession à cheval sans ancien personnage distinct, abandon borné ; contributions joystick/souris sous menu ; source A partagée sans émission du corps et restauration exacte, y compris Abyssal/Molten. L'ancien test négatif de copie de paramètres reste utile et conservé. Deux relectures : contrats/usages, puis init/expiration/relâchement. Pas de nouvelle suppression spéculative.

Statut initial : EXPÉRIMENTAL, pas encore CONSTRUIT ni VALIDÉ en jeu. Compilation Windows uniquement sur GitHub. ZIP cumulatif deux binaires, sans INI/README ; défauts/INI/raccourcis, main/archive et anciennes archives conservés. Installation après contrôles et fermeture du jeu, dans la continuité de la demande d'installation précédente ; sauvegarde avant remplacement, aucune relance automatique.

Test groupé : 1) menu souris inchangé et caméra au stick ; 2) chargement direct déjà à cheval, sans rappel ; 3) Stoneskin complet avec feu, puis restauration de l'écharpe, sur armure actuelle et autre armure. GPU accepté, coop différée. Retour arrière : sauvegarde V1.157. Suite : exploiter les résultats du lot avant tout autre remplacement d'effets.
