# Suite après V1.73 — OFF des pistolets et HUD général

20 octobre 2026. **AUDIT / PRÉPARATION D'OBSERVATION**, aucune nouvelle version construite. Base cumulative V1.73 installée, nouveaux comportements de chargement des montures et plage DOF encore sans retour de jeu. Les acquis utilisateur et le retrait du Recovery dash sont conservés. Branche debug seulement.

## OFF : le nom du slot ne désigne pas la main

Inspection hors jeu de 18 assets natifs, soit 15 montages, 3 séquences et 121 exports. Extraction privée depuis le PAK du jeu avec vérification SHA1 de chaque entrée ; aucun asset du jeu ajouté au dépôt ou à une archive. Index PAK `5d0982cd46a5221290cb6ed24696e3962e9dddff`. Propriétés sérialisées examinées ; les données compressées d'animation ne sont pas interprétées comme des propriétés.

**Constat qui change l'approche : 14 des 15 montages examinés utilisent Shoot_Right ; celui du cheval utilise Blend.** Les montages Standard, Rapid et Lava contiennent pourtant chacun des notifications de projectile pour `RightPistol` ET `LeftPistol`. Charge niveau 1 et Graviton examiné utilisent même `LeftPistol` sous un montage `Shoot_Right`. La présence distincte de slots Shoot_Left et Shoot_Right dans le graphe ne prouve donc pas une séparation des mains dans les tirs réellement utilisés.

| Élément observé | Preuve dans les propriétés natives | Conséquence |
| --- | --- | --- |
| Standard à pied | Flash sur VFX_Weapon_Gun_L/R ; douilles sur Gun_L/R_Socket | Côté identifiable pour ces effets exacts |
| Standard à cheval | Flash/douilles sur Bone_Strife_Fing_Index2_L/R | Chemin distinct de celui à pied |
| Standard, Rapid, Lava normal | Notifications SpawnProjectile avec WeaponMesh RightPistol et LeftPistol dans le même montage | Garder les notifications et le cycle natif de tir |
| Charge et Graviton examinés | WeaponMesh LeftPistol, malgré le slot Shoot_Right | Ne pas déduire le côté du slot |
| Beam, Static et plusieurs tirs spéciaux | Effets ponctuels ou durables sur VFX_Gun_Aim, VFX_Root_Beam ou StrifeMuzzleFlash | Une correspondance socket → main unique n'est pas démontrée |
| Lava normal | Deux notifications de flash avec les mêmes socket et décalages sérialisés | Ni le nom ni le décalage seuls ne distinguent les mains |
| Visée | Plusieurs états du graphe ont déjà une pose de visée avant les slots de tir | Court-circuiter seulement un slot ne garantit pas un bras baissé |

**Piste écartée avant implémentation : arrêter ou filtrer Shoot_Right pour masquer le bras droit.** Cela concernerait les deux pistolets et pourrait supprimer les événements qui créent les projectiles. Ne pas étendre non plus le correctif validé du dash à tous ces montages : son périmètre reste les séquences F/B/L/R et le Standard parasite.

L'objectif reste OFF indépendant par main : modèle masqué, présentation de cette arme retirée, bras ne pointant plus devant, tirs et dégâts conservés. Le point de pose visuelle doit encore être établi sans perturber les sockets utilisés par le gameplay. Aucun nouveau hook d'animation, arrêt de montage, changement de pose ou masquage global d'effets n'est introduit par cet audit. Les modèles gauche/droite V1.67 et les holsters validés sont intacts.

Le moteur distingue l'évaluation de pose d'un slot ; [l'API Epic SlotEvaluatePose](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/Animation/FAnimInstanceProxy/SlotEvaluatePose?application_version=5.5) fournit un repère conceptuel. Cette documentation UE5 n'établit ni l'ABI ni le comportement exact du jeu UE4. L'exécutable local confirme séparément le dispatch de pose `168AFC0` vers le virtuel `+18`, ainsi que la table LayeredBoneBlend `2A7FFA8` ; cela n'autorise pas à modifier les poids ou les poses depuis un autre thread. Aucun appel natif externe effectué.

## HUD : corriger l'observation avant de choisir un patch

Défaut rapporté : choisir une arène verrouillée affiche le refus, rend le menu transparent puis fait disparaître le **HUD général**. Ancien essai reproduit, dernier essai V1.69 non reproduit. Aucun nouveau résultat en jeu pendant cette reprise : le jeu est fermé.

La comparaison ancienne incluait des lectures de 16 octets pour des champs plus petits, et même des entrées de fonction interprétées comme des champs. Après réduction à la largeur des primitives, les différences annoncées sur les pointeurs WidgetTree, deux booléens et les fonctions disparaissent. Reste un LastRenderTimeOnScreen d'InteractHUD qui avance ; le changement de TimerHandle de HUD_ToastMinor est réel mais ne démontre pas la cause d'une disparition générale. Cette correction de lecture ne prouve pas que tous les états Slate ou les transformations étaient corrects.

Observation suivante préparée hors ASI : snapshot avant/après avec taille déclarée de chaque propriété, masque des booléens, enfants appartenant au HUD et à la page de missions, transformations, opacité, visibilité, piles UI et identité d'objet recontrôlée après parcours. Accès `QUERY_LIMITED_INFORMATION | VM_READ`, sans lancement, appel natif, injection ou écriture dans le jeu. Parcours borné ; capture incomplète explicitement marquée. Syntaxe contrôlée ; essai sans jeu terminé proprement par `GAME_NOT_RUNNING`. **Capture réelle non effectuée.**

Contrôle du lecteur sur données artificielles : des octets voisins différents ne changent pas le champ comparé ; masque d'un booléen respecté ; NaN encodé explicitement ; tableau hors bornes non déréférencé. Ces contrôles valident ces garde-fous, pas le fonctionnement du HUD en jeu.

Le lancement natif de mission reste une fonction void ; le refus précède la préparation de mission et le Blueprint joue ensuite son outro. La piste d'une transition UI après refus reste une hypothèse, pas une correction démontrée. Aucun ShowHUD forcé, déverrouillage d'arène ou hook générique de Blueprint ajouté.

## Étape suivante et limites

Une seule préparation utilisateur demandée : lancer V1.73 avec Strife en mission et le HUD visible, laisser le jeu ouvert. Enregistrer d'abord l'état normal ; demander ensuite seulement le refus d'une arène verrouillée. La comparaison doit porter sur la même session et signaler tout changement de propriétaire ou objet détruit.

Le ground slam reste après cette file, selon la décision utilisateur. Les autres icônes/effets P2 restent reportés. Cet audit ne justifie pas une build de diagnostic supplémentaire. ASI, INI personnel, raccourcis, PAK et sauvegardes ne sont pas modifiés.
