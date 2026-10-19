# V1.66 — cycle natif des sauts et anticipation du cache de holsters

19 octobre 2026. Branche debug uniquement. Base cumulative V1.65 `b5597d1365ebd72b386e8d7b10d5e5d11a8d0f26`.

## Base et objectif

Utilisateur : holsters montés corrects, petit délai de placement ; trois délais de rangement corrects ; durée ultime correcte après activation de la case. Ces chemins sont VALIDÉS dans les essais rapportés. L’échec restant des sauts est confirmé : deux remontées sans animation puis blocage. L’ancien critère « booléen natif vrai / impulsion positive » était insuffisant.

Objectif du lot : un cycle de double saut répétable avec animation, et réduire la recherche initiale des modèles montés. Critère en jeu : au moins dix nouveaux appuis produisent un saut animé, After gliding ne rend qu’un saut par nouveau plané, OFF conserve les limites natives ; holsters corrects dès la première montée après une période à pied, avec tirs et rangement inchangés. Construction et fixtures ne valident aucun de ces critères de gameplay.

## Preuves en lecture seule et source native

Session de jeu V1.65 lancée par l’utilisateur, processus 27712 ; capture VM_READ seule, aucune fonction appelée ni mémoire modifiée. 3918 échantillons valides ; 4871 lectures échouées après fermeture du processus, exclues du diagnostic. Le processus s’arrête à 09:17:29 dans son journal. Le script de capture aurait dû arrêter ses tentatives dès cette fermeture ; aucune faute du mod n’est déduite de ces lectures.

ReplicatedJumpCount `actor+974` reste à 2 tandis que JumpCurrentCount UE `+448` atteint 3 puis 4. Les remontées supplémentaires sont associées à une vitesse verticale positive ; JumpCount de l’animation reste à 2, avec bAllowEnterDoubleJump toujours vrai. Le mode alterne aussi Falling et Custom/Gliding. Ce relevé concorde avec l’absence d’animation signalée, sans prouver seul tous les points de blocage.

Le getter natif `56D400` lit le compteur répliqué via `movement+740`, sinon le compteur UE. Le cycle de mouvement emploie aussi le byte `movement+A00`, plafonné à deux par `6BA9F0`. Le contrôle ordinaire du plané `69B7F0` / `6BF5A0` vérifie notamment A00>=2 et les délais du plané. StopGliding termine le mode, sans réarmer ce compteur.

L’animation native `68D656 → 56D400`, retour `68D65B`, copie le vrai compteur dans AnimInstance.JumpCount `+2E8`. Le calcul `678C90` rend bAllowEnterDoubleJump vrai si JumpCount==DoubleJumpCount (2). Le Blueprint natif extrait confirme les transitions Fall/JumpBegin → DoubleJump et l’absence de transition DoubleJump → DoubleJump. Une impulsion supplémentaire conservant le compteur constamment à deux ne constitue donc pas une nouvelle transition d’animation fiable.

L’état d’action Jump est lu par l’animation via l’interface `actor+8D0`, fonction `669D30 → 635100`, action 0x1E. Le code exact lit bien `actor+E10+30`. L’indicateur moteur bPressedJump est le bit 4 de `actor+430`. Les nouveaux réarmements attendent ces deux états relâchés et que l’instance d’animation ait déjà observé le double saut. Le `CheckJumpInput` moteur `17131E0` incrémente son compteur séparé et publie OnJumped ; il n’est pas modifié.

## Remplacement du candidat de saut

Les trois interceptions existantes sont conservées (DoJump, getter, demande JumpInputPressed), sans nouvelle interception. La substitution du retour au seul appel interne DoJump `69FCB5` et son scope thread-local sont retirés. Le getter renvoie désormais toujours le vrai compteur natif.

Sur son appel d’animation identifié seulement, après relâchement de l’action et observation préalable du compteur deux, le candidat demande **1 aux deux setters natifs** `582FF0` et `6BA9F0`. Il vérifie leur résultat et annule la paire si un setter refuse. La même mise à jour d’animation reçoit ainsi le compteur réel un ; le prochain DoJump d’origine produit le double saut et publie réellement deux. Les autres appels au getter, acteurs non locaux, chevaux, formes ultimes, états de paroi/rebord, pause, cinématique et mode OFF restent exclus du réarmement.

Le paramètre de délai du double saut est encore étendu uniquement pendant un saut supplémentaire réarmé, puis restauré si le natif ne l’a pas modifié. Impulsion, contrôles d’aptitude et transitions d’action restent natifs. Aucun compteur UE, limite globale, vitesse, drapeau d’animation, transformation, physique ou bouton n’est écrit. **Les compteurs du jeu sont désormais réarmés par leurs fonctions natives** ; la revendication historique « aucun compteur modifié » de V1.65 ne s’applique pas à V1.66.

After gliding conserve un crédit par plané observé, consommé seulement quand le saut natif réussit et publie deux. Une demande pendant le plané utilise StopGliding, réarme la même paire, puis transmet exactement une demande d’origine ; la sortie du mode Gliding assure ici la transition visuelle vers Fall/DoubleJump. Identités/générations P1/P2 et modes sont séparés. OFF ou changement de réglage restaure un réarmement non consommé si les deux valeurs appartiennent encore au candidat ; une valeur native ultérieure ou un atterrissage gagne. Les gardes d’admission natives ne sont pas élargies.

Les quatorze sites de saut sont contrôlés octet par octet pour l’exécutable exact ; les points de lecture/action, deux setters et calcul d’animation sont inclus. Activation groupée des trois hooks, retrait complet en cas d’échec. Fonction OFF : retour rapide du getter sans résolution de propriétaires quand aucun réarmement ne reste à restituer.

**Limites :** le redémarrage visuel du cycle reste EXPÉRIMENTAL ; les lectures et le graphe ne prouvent pas le résultat rendu, notamment à cadence d’appuis très rapide ou pendant un mélange d’animations. Si l’absence d’animation/blocage persiste, le candidat n’est pas validé : OFF et conserver les états, sans empiler un second mécanisme d’impulsion ou de reset d’animation.

## Holsters

La recherche des quatre modèles chargés pouvait démarrer seulement une fois Strife monté. Elle démarre désormais aussi à pied, option activée, depuis le composant de pistolet droit réciproque `actor+1040`, type de personnage Strife, pawn==character. Même vtable de composant, propriétaire, parent et classe StaticMesh vérifiés. War et formes ultimes ne fournissent pas cette source. Aucun modèle n’est modifié à pied.

Budget identique : 512 objets maximum par 16 ms tant que le cache est incomplet ; arrêt des scans dès que les quatre identités sont valides. Aucun chargement de package. Les mêmes deux composants de holster, modèles Stowed validés, setters, restauration, sockets et visibilités sont conservés. L’anticipation réduit la latence attendue ; elle ne garantit pas une application instantanée si les assets ne sont pas encore chargés ou si la montée suit immédiatement le chargement du niveau.

## Contrôles et état

Source EXPÉRIMENTALE ; construction Windows x64 / contrôles de paquet à effectuer. AirJumpTests vérifie maintenant la publication réelle de un puis deux à l’animation, la paire du plané, le refus pendant l’action maintenue, l’attente d’une première observation du double saut, les setters refusés, OFF/restauration, les restrictions natives, générations et crédits distincts. Le test ne remplace pas le moteur ni l’image rendue. MountedHolsterTests ajoute la sélection de source à pied sans setter visuel, War/ultime exclus et parent incorrect refusé.

Aucun nouveau réglage, 139 clés INI et 53 actions conservées. Les sources de durée ultime, trois délais de rangement, grappin, munitions, dash, visée TPS/native, RawInput, Alt-Tab, contrôleurs, roues, caméra et focus ne sont pas modifiées. Ancien Action Recovery non réintroduit. Icônes/effets P2 reportés explicitement. Anciens ZIP, main/archive et tag stable à vérifier intacts avant annonce. Pas d’installation ni lancement par l’agent, pas d’écriture de l’INI personnel.

## Essais groupés

1. Always : dix appuis distincts, avec animation à chaque saut supplémentaire et déplacement conservé ; répéter TPS puis vue native. Essayer ensuite quelques appuis rapprochés.
2. After gliding : un nouveau saut après chaque plané ; essayer aussi une demande de saut pendant le plané. Sans nouveau plané, pas de saut supplémentaire. Atterrir et recommencer ; OFF doit rendre les limites normales.
3. Holsters ON : rester quelques secondes à pied avant la première montée, regarder le délai et l’alignement ; tirer, ranger, descendre/remonter. OFF doit restaurer l’aspect initial.

Les délais de rangement et la durée ultime sont déjà validés : simple surveillance de régression, pas de nouvelle demande de refaire leurs essais complets. Auto-check reste sans retour explicite. Les autres éléments P2 restent en attente. Rollback du lot : conserver l’INI personnel, remettre V1.65 pour les holsters validés et laisser Infinite double jump OFF ; toutes les archives restent disponibles.
