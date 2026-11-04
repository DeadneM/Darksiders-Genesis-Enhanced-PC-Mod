# V1.152 — préparation, aucune livraison installable

Base cumulative : source V1.151 `02f9eac97a90e09106604d582311c9b52ff6b020`, documents à `2c5bc9f3fa0561c947a981100754166e779bdf7d`. Travail exclusivement sur debug. Le dernier ZIP disponible reste V1.151. Aucun fichier de jeu, INI personnel, sauvegarde ou PAK remplacé. Compilation prévue seulement sur GitHub en mode `[check only]`, sans publication de ZIP.

## Retour utilisateur et décision

- **VALIDÉ utilisateur** : disparition du trait/arc dans Moloch. Conserver intégralement le catalogue des 168 shaders et les chemins de dessin.
- **REJETÉ** : sélection stomp/dash selon MOVE, incohérente dans les deux sens. Le garde V1.151 du seul événement LAND est retiré, avec ses compteurs et fixtures spécifiques. La nouvelle cible demandée est **appui court : descente ; appui maintenu : dash vers l'avant, même en l'air**. Elle n'est pas encore implémentée dans cette préparation.
- **NON VALIDÉ** : sortie de pose de tir. Le défaut peut suivre toute interruption de visée ou de tir ; il n'est pas limité au stomp.
- **NON VALIDÉ** : Stoneskin fonctionne au début puis disparaît plus tard dans la même scène. Le journal montre un refus de matériau natif, pas une preuve d'asset manquant dans cet épisode.
- **À TESTER utilisateur** : persistance du choix GPU à la fermeture. Code et aide de sortie V1.151 conservés.

## Deux corrections préparées

Le setter natif `SetAimMode` contient exactement trente octets et ne fait que `AimMode = Mode`. L'ancienne comparaison attendait le paramètre `Mode`, alors que le pool de noms du jeu le résout `mode`. Le garde rejetait donc la fonction avant tout appel. La comparaison des seuls identifiants de membres/paramètres respecte désormais cette différence de casse ; les comparaisons exactes d'objets, de packages, de types et les limites restent inchangées.

La sortie de pose est séparée du module Dash : un service de cycle de vie Strife à pied traite un éventuel mode AIM orphelin après la fin de l'interruption, indépendamment du type de cette interruption et de la vue. Il ne demande qu'une sortie native, avec script intégral, références, type Function, propriétés, propriétaire et schéma vérifiés. Aucune capacité active, visée/tir maintenu, cible verrouillée, phase aérienne, pause, roue ou cinématique ne peut être interrompue par ce service. Pas de nouvel hook, de tir synthétique, de modification de montage ou de booléen brut. Le mode avant le tir réparateur n'avait pas été capturé lors de l'ancien essai : **l'efficacité de cette correction reste expérimentale**.

Stoneskin accepte désormais les enveloppes `MaterialInstanceDynamic` que le moteur crée sur les matériaux de l'armure équipée. La capture montre quatre enveloppes natives, détenues par le personnage et toujours référencées par son tableau `m_dynMaterials`. Leur Parent mène à chaque matériau exact de l'armure Silver. Le garde exige cette référence forte native et cette filiation bornée ; un Outer seul, un autre matériau, un propriétaire étranger ou un cycle sont refusés. La fin de Stoneskin restaure l'enveloppe native exacte et ses paramètres, plutôt que le matériau par défaut de l'asset. Les sources et le rendu pierre/feu validés restent inchangés.

## Dash court/long : preuve et travail restant

Le script intégral `PerformDash` (716 octets en mémoire) sélectionne systématiquement AirDash dans la branche aérienne, sous réserve de l'amélioration native DashDown2. Le MOVE natif n'y choisit pas un dash horizontal. La fabrique `CreateDashInfoForPlayer` peut aussi produire un vecteur de repli sans déplacement : la seule présence d'un vecteur ne prouve donc pas une direction maintenue.

L'énumération native donne `ABILITY_ONE=14` et `ABILITY_ONE_RELEASE=15` ; il faut confirmer leur routage Dash dans le personnage, et ne pas interpréter leur nom comme un numéro de touche. Le point d'entrée d'actions déjà intercepté pour la visée manuelle est réutilisable ; aucun bouton clavier/manette ne doit être imposé.

AirDash expose `FallSpeed=5000` et `ForwardSpeed=0`. Sa physique native applique la vitesse avant puis écrase la vitesse verticale par `-FallSpeed`. Cela démontre l'existence des paramètres, **pas** un cycle de dash horizontal opérationnel. La durée, la fin native, la restitution du mouvement et la présentation doivent être établies avant de modifier une instance. `AbilityTimeout=3` ne constitue pas à lui seul une preuve de durée active. Aucun changement de ces valeurs ou du mode de mouvement n'a été exécuté sur le jeu.

## Contrôles et réussite attendue

Tests significatifs : refus des scripts/paramètres inconnus, casse des identifiants sans assouplir les noms d'objets, sortie au plus une fois et indépendance des joueurs ; frontières natives du service avec faux répartiteur ; cycle complet Stoneskin avec conservation/restauration des matériaux natifs et refus des filiations étrangères/cycliques. Ces fixtures et la compilation ne constituent pas une validation en jeu.

Avant livraison cumulative : terminer le choix court/long et sa sortie propre ; vérifier CI Windows, absence d'avertissements et conservation des chemins validés ; livrer uniquement deux binaires à la racine. Après livraison : tester pose interrompue par plusieurs actions, Stoneskin après changement d'armure et plus tard dans la mission, dash court/long en l'air, puis GPU après fermeture. Pas de promotion main/archive.
