# Recovery et modèles — préparation après V1.66

19 octobre 2026. Base debug `eb0b287e085f8ad8aee2c866bbe82c98524c593b`. Demande : poursuivre les autres tâches pendant l’essai V1.66. **AUDIT / PRÉPARATION, PAS UNE NOUVELLE FONCTION NI UNE BUILD.** Sources runtime et configuration conservées intégralement.

## Action Recovery : nouveaux repères vérifiés

Le système de fin anticipée retiré en V1.58 n’est pas réintroduit. L’objectif utilisateur reste le contrôle du déplacement après le mouvement utile du dash, initialement limité au dash ; coffres différés.

Exécutable exact : SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. La table d’enum native et la fonction de conversion `72FFF0` établissent **ECharacterActions::MOVE=29 → EAbilityInterrupt::MOVE=1**, **JUMP=30 → interruption JUMP=2**. Ces nombres ne sont pas des boutons. Le `ProcessInterrupt` du Blueprint Ground_Dash_Base traite explicitement la cause 2 : c’est JUMP, pas MOVE. Ne pas transposer le nombre d’une action à sa cause d’interruption.

Les defaults cuits de Ground_Dash_Base placent MOVE dans **QueuedActionsFlags**, pas dans **AllowedActionsFlags** ni **AbilityInterruptsFlags**. Aim/Fire héritent de cette différence, avec quelques overrides propres ; War garde aussi MOVE en file. Les bitmasks sérialisés sont des entrées FName + valeur entière : les suffixes numériques de FName sont significatifs (TURN_90/TURN_180), pas des erreurs ou des booléens supplémentaires. Ces defaults ne prouvent pas la valeur runtime pendant toute l’animation.

Les propriétés natives situent AbilityInterruptsFlags au +A8 (données de masque +B0), AllowedActionsFlags au +78 (données +80/+88), QueuedActionsFlags au +170 pour les capacités joueur (données +178/+180). Le masque +F0/+F8 est une autre donnée : requêtes d’action en attente. Le masque +120 intervient dans le suivi du tick et doit encore être caractérisé complètement ; ne pas l’écrire sur une hypothèse de cache ou d’override.

Chaîne native identifiée :

1. Le traitement d’action `5B2E30` marque la requête dans +F0, convertit l’action en cause et passe par le virtuel +280.
2. Le virtuel observé du dash +280, `5B2CE0`, contrôle le masque d’interruptions +B0, passe par le traitement Blueprint et par la fin native en conservant la cause.
3. La fin native `59B080` **reçoit une cause sur un octet**, pas une sémantique booléenne démontrée. Elle propage cette valeur à `59B2D0`, au nettoyage virtuel +2A8 et à OnFinish. L’ancien nom EndAbility(false) utilisait zéro/NONE ; il ne modélisait pas la raison MOVE. Ce constat ne prouve pas à lui seul la cause des anciennes régressions.
4. Le tick `5B46C0` compare +120 et +B0, appelle le virtuel +2A0 quand ils diffèrent et conserve le lifecycle d’update. Pour le dash observé, ce virtuel vaut `599310(ability,cause,bool)` ; il manipule le masque, notifie le jeu et peut repasser par une requête d’action native. Son emploi en tant que setter autonome et le sens complet du booléen ne sont pas validés par un relevé courant.
5. Le chemin `5B2C30` vérifie à la fois une requête en attente, l’autorisation de mise en file et l’état de commande natif avant de rejouer l’action. Activer un bit de masque ne garantit donc pas qu’une commande maintenue soit effectivement reprise.

**Résultat utile :** ces mécanismes distinguent enfin autoriser le mouvement et permettre l’interruption native par une demande réelle de mouvement. La prochaine approche doit laisser ce pipeline gérer la cause et son nettoyage ; aucun nouvel appel de fin globale, flag MOVE forcé, bouton ou stick n’est ajouté au mod.

## Preuve historique et limites

Relecture du fichier privé `v154-live-dash-actions-20261018-101659.jsonl` : 37857 lignes au total ; 120 observations de Dash_Aim_Level1 réparties sur trois instances et 50 de Dash_Fire_Level1 sur deux instances. Le bit d’interruption MOVE est actif dans 35 observations Aim, à partir d’environ 0.7 s ; aucune requête MOVE +F0 n’est relevée dans ces observations. Fire : aucune des 50 observations ne montre ces deux bits actifs. Ce fichier ancien n’enregistre pas l’intention MOVE maintenue, ni les bons timers de décélération au même instant ; ce n’est pas une baseline actuelle non modifiée, ni une preuve que tous les dashs fonctionnent ainsi.

Ces données empêchent de promettre qu’un simple bit activé réglerait le problème. Une nouvelle capture externe bornée est **préparée**, avec requête MOVE, état de commande natif, masques +80/+B0/+120/+F0/+178, compteurs 224/228, position réelle, vélocité et montage. Elle lit seulement le jeu, s’arrête après fermeture ou erreurs répétées, ne lance rien et n’appelle aucune fonction du jeu. Son contrôle de syntaxe/bootstrap a été effectué sans attachement. **Pas de nouveau relevé de jeu dans cette reprise.**

Prochain test discriminant, après le retour V1.66 : chaîne Strife de trois dashs puis quatrième, déplacement maintenu puis nouvel appui de direction à la fin. Comparer le retour de contrôle avec le moment où l’interruption et la requête existent réellement. Ne demander une intervention qu’après observation d’un point sûr sans déplacement utile restant. Si cette preuve manque, conserver le diagnostic et ne livrer aucune case sans effet.

## Modèles 3D : inventaire élargi

Index du PAK principal vérifié par SHA1 : **86641 entrées**, sélection de **238 fichiers / 119 assets** couvrant le dossier Model de Strife, les références d’armes/skins et les dashs pertinents. Références extraites seulement dans le dossier d’audit privé ; aucune copie d’asset du jeu dans le dépôt ou les ZIP.

| Objet | Classe vérifiée | Usage établi |
| --- | --- | --- |
| Strife_Gun_Mesh_R / L | StaticMesh | Pistolets en main |
| Strife_Gun_R_Stowed / L_Stowed | StaticMesh | Représentations rangées, utilisées par la correction montée validée |
| Strife_Gun_02 / Strife_Gun_Inst | MaterialInstanceConstant | Matériaux, aucune nouvelle géométrie de pistolet |
| Strife_Gun_02_d / _n / _mre_packed | Texture2D | Textures, pas des modèles 3D |
| Strife_Gold / Silver / Abyssal | SkeletalMesh | Modèles de personnage entier, pas des pistolets alternatifs compatibles démontrés |

Aucun autre StaticMesh de pistolet Strife n’est identifié dans ce périmètre élargi du jeu installé. Cette conclusion ne porte pas sur des créations externes ou un autre jeu.

Pour un véritable remplacement : choisir l’asset, vérifier le cook Windows compatible, les unités/axes/origines, la prise en main droite/gauche, la pose rangée et les matériaux. Une paire destinée aux mains ne remplace pas automatiquement les représentations Stowed ; leurs géométries et axes diffèrent, comme l’a montré la réparation montée. Couvrir les personnages normaux, les composants de monture, les transitions de visée/tir/rangement et les deux joueurs. Les références de canon, effets et projectiles doivent rester cohérentes sans dégrader les tirs validés. Ne pas promettre un chargement de contenu externe sans en avoir établi le mécanisme et les fichiers nécessaires.

**Statut : faisabilité documentée, aucun remplacement alternatif ni sélecteur affiché.** Les modèles souhaités restent à choisir ; les icônes/effets P2 restent différés. La correction de holsters V1.65/V1.66 n’est pas réutilisée comme changeur d’armes en main.

## Nettoyage documentaire et continuité

Les anciens tableaux du README contenaient TPS/FOV « non implémentés », des raccourcis F1/F3 historiques et une ancienne validation Recovery en contradiction avec l’état courant. README, CURRENT_STATE et ROADMAP sont désormais des documents actuels ; leurs versions intégrales précédentes sont archivées octet pour octet, avec [manifest de signatures](history/DOCUMENT_HISTORY_MANIFEST.json). Le carnet reste cumulatif. [File active](ACTIVE_QUEUE.md).

Pas de nouveau build ni installation ; la version testée reste V1.66. Aucun fichier sous sources/ modifié, aucune sauvegarde/INI personnel remplacé, aucune branche main/archive ou archive numérotée changée. Les nouveaux contrôles concernent les preuves, les liens documentaires, l’intégrité des historiques et la conservation du code ; ils ne sont pas des essais de gameplay.
