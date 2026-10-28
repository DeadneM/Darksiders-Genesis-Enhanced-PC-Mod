# Recovery - carte des verrous et des temps, 28 octobre 2026

**AUDIT HORS JEU / PAS UN CORRECTIF RECOVERY.** Base V1.124 validee et promue vers main. L'utilisateur decrit un personnage encore immobile apres la fin visuelle d'une animation et demande ou se trouve ce temps. Aucune duree globale unique n'est demontree. La fin du geste visible, la fin du montage et la restitution du deplacement sont trois evenements differents.

Lecture actuelle de l'EXE supporte (SHA256 `9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54`) et extraction privee de27assets depuis le PAK original ; index SHA1 `5d0982cd46a5221290cb6ed24696e3962e9dddff`, empreintes individuelles verifiees,0limite de decodage sur les exports retenus. Pas de processus attache, lancement, ecriture du jeu ou appel moteur. [Donnees lisibles](evidence/V1_125_RECOVERY_ASSET_MAP.json).

## Ou chercher

| Niveau | Champ ou fonction controle | Ce que cela signifie |
| --- | --- | --- |
| Composant de capacites joueur | `MoveInterruptDelaySec`, propriete native au+0x110 | Delai existe, mais les captures historiques dash le voyaient deja a0. Sa modification anterieure n'a pas corrige le probleme. Pas un delai global prouve apres toute animation. |
| Capacite active | `AllowPlayerMovement`+0x161, `AllowedActionsFlags` donnees+0x80 | Permissions, pas des secondes. Etre dans une pose visuellement neutre ne suffit pas a les liberer. Lecture native controlee notamment en5A3670/67F105. |
| Capacite active | `AbilityInterruptsFlags` donnees+0xB0 et fenetre temporaire+0x120 | Autorise le moteur a interrompre une action sur une demande reelle. MOVE action29 devient cause1 ; JUMP action30 devient cause2. Les nombres ne sont pas des touches. |
| File de commandes | Requetes+0xF0, `QueuedActionsFlags` donnees+0x178 ;5B2E30/5B2CE0/5B2C30 | Une commande maintenue peut etre en file ou refusee pendant la capacite. Un masque ouvert seul ne garantit pas sa reprise. |
| Dash | Compteurs prives+0x224/+0x228 ;5B93F0 | Le tick decremente224, calcule le rapport avec228 et applique la vitesse. Changer ce temps peut modifier le dash utile, pas seulement sa fin. |
| Dash | Verrou personnage+0xAAC, efface dans59BF80 | Debut/fin natifs du dash possedent ce verrou ; ne pas le mettre arbitrairement a0. |
| Dash start | `MinMontageRate`+0x1F8, `MaxMontageRate`+0x1FC, `MontageRateIncreaseDuration`+0x200 | Vitesse d'animation/montee de rythme d'une classe distincte. Les memes offsets sur une autre classe n'ont pas la meme signification. |
| AnimMontage | `SequenceLength`, segments/rates, `BlendOut.BlendTime`, `BlendOutTriggerTime`, `bEnableAutoBlendOut` | Duree totale, vitesse et fondu peuvent survivre au geste utile. Un fondu de0,1s ne prouve pas0,1s de blocage ajoute apres la fin du clip. |
| Capacite | `AbilityTimeout` | Valeur3s sur Ground_Dash_Base : limite de secours, pas une preuve de3s de recovery. |
| Fin native | Etat+0xD8, fin en attente+0x148,59B080/59B2D0 | Le montage peut ne plus etre visible alors que la capacite doit encore terminer ses callbacks/nettoyages. Fin forcee sans cause correcte deja rejetee. |

## Temps confirmes dans les assets natifs

Ces nombres sont des temps d'asset avant les vitesses/segments/runtime : **pas des mesures de latence du joueur ni des valeurs a mettre a zero globalement**.

| Action | Donnees confirmees |
| --- | --- |
| Reception lourde Strife | Clip1,6333s ; interruptions attaques/saut a0,668465s ; **MOVE a0,833373s** ; fondu montage0,2s. |
| Reception lourde War | Clip1,6s ; interruptions attaques/saut a0,402459s ; **MOVE a0,813425s** ; fondu montage0,2s. |
| Dash Strife | Sequence Strife_Dash1,4333s ; MOVE a0,691174s. Montage Level3 longueur2,3333s et fondu0,15s ; capture historique Level3~0,83s en jeu : ne pas confondre temps asset, segments et temps reel. |
| Dash War | War_Atk_Dash1,7667s ; MOVE a0,880815s ; montages Level1/3 fondus0,1s. |
| Coffre Strife | Strife_IA_Chest_A_Open et En_Chest2,7333s, root motion natif, montage fondu0,1s. Aucun notify MOVE explicite dans les exports retenus ; la liberation depend donc aussi du cycle d'interaction. |
| Coffre War | War_IA_Chest_A_Open et En_Chest2,1667s, root motion natif, montage fondu0,1s ; meme limite d'observation. |

**Correction documentaire :** les seuils0,668465/0,402459 utilises dans le code HeavyLanding valide ne sont pas le debut des notifies MOVE cuits ; ce sont ceux des autres interruptions ci-dessus. Leur description anterieure comme fenetre MOVE native etait trop large. Le module valide ouvre son essai apres0,15s et50ms au sol ; son code n'est pas modifie dans ce lot. Revoir separement la propriete/restauration de cette fenetre avant toute retouche de comportement.

## Etape suivante utile

Pour une action precise, relever ensemble la commande de deplacement maintenue, l'action courante et ses masques, le vrai montage/sa position/son poids de fondu, les timers propres et la position du personnage. Cela permettra de distinguer une attente dans la fin du clip d'une capacite encore bloquante apres sa fin. Les precedentes captures n'enregistrent pas toujours l'intention au meme instant ; elles ne prouvent pas encore le temps mort actuel apres un coffre.

Priorite suivante proposee : coffre/fin d'interaction puis troisieme dash Strife, sans rouvrir Geyser/Stoneskin/Caltrops valides, sans modifier les touches reaffectables. Aucune case Recovery generale ou raccourcissement aveugle ajoute en V1.125.
