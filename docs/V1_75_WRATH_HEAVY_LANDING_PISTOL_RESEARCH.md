# Prochain lot — capacités, réception lourde, pistolets OFF

## Demandes et base

V1.73 installée, trois essais rapportés validés. V1.74 construite/contrôlée mais non installée, nouvelle visée manette encore expérimentale. Le prochain lot cumulera cette visée avec les ajouts réellement étayés. Installation **manuelle par l'utilisateur à la fin de sa session** ; aucune fermeture ou installation automatique à demander.

- Capacités : trois sélecteurs indépendants pour chacun de Strife et War, Native ou choix libre parmi les six capacités. L'utilisateur a écarté le simple échange de cases correspondantes. Corriger la fonction, jamais une touche fixe.
- Recovery : réception lourde automatique après une chute assez haute, **clarifié par l'utilisateur**. Exclure attaques plongeantes, Rampage, dash, réapparition et changement de personnage. Ancien Recovery dash rejeté et retiré, ne pas le ressusciter.
- Pistolet OFF par main : modèle, effets et bras tendu masqués ; projectiles/gameplay conservés. Modèles gauche/droite et holsters validés restent intacts.

## Capacités : preuves et limite actuelle

PAK principal index SHA1 `5d0982cd46a5221290cb6ed24696e3962e9dddff`, assets de référence privés, empreintes de chaque contenu vérifiées. 22 assets inspectés ; un CDO War non entièrement décodé, explicitement inconnu. Bytecode des deux GetWrathAbilityById décodé intégralement (282 octets sérialisés chacun, 406 octets en mémoire). Routes :

| Personnage | FName interne / numéro | Classe native retournée |
| --- | --- | --- |
| Strife | Strife_Wrath / 2 | MayhemAbility_ShadowStrife_Blueprint_C |
| Strife | Strife_Wrath / 3 | MayhemAbility_WorldEnder_Finish_Blueprint_C |
| Strife | Strife_Wrath / 4 | MayhemAbility_SpawnCaltrops_Blueprint_C |
| War | War_Wrath / 2 | MayhemAbility_Wrath_BladeGeyser_Blueprint_C |
| War | War_Wrath / 3 | MayhemAbility_Rampage_Start_Blueprint_C |
| War | War_Wrath / 4 | MayhemAbility_StoneSkin_Blueprint_C |

Le numéro est la représentation interne FName, pas une touche ou un numéro de case affiché. Chaque lookup ne reconnaît que sa famille. Native GetWrathAbility RVA673600 sélectionne les FName des ressources +3E4/+3EC/+3F4 via les actions 37/39/41. Pas de hook ajouté à cette frontière à ce stade.

Les six Ubergraph de capacités sont décodés intégralement ; ils lancent des montages spécifiques. Les trois montages Strife référencent Strife_Skeleton ; les trois War référencent War_Mesh_Skeleton. WorldEnder et le double font intervenir les composants de Strife. StartBeam reste partiellement décodé : ne pas extrapoler la partie inconnue. Les coûts natifs existent dans les capacités (exemples : Geyser/WorldEnder10, Stoneskin/Caltrops5) ; aucun coût ou niveau réécrit.

**Pas encore de système d'échange fonctionnel livré.** Une modification du nom seule ne traverse ni le lookup Blueprint ni les dépendances d'animation/composants. Prochaine étape : préciser le lancement, les composants et la compatibilité, sans ajouter des sélecteurs inertes ou appeler une capacité étrangère sans garde.

## Capture groupée et réception lourde

Capture externe QUERY_LIMITED_INFORMATION|VM_READ, aucun appel ni écriture dans le jeu : **7 480 échantillons retenus, trois incohérences rejetées**. Les six classes ci-dessus ont bien été observées sur leurs personnages natifs. Instigator, composant d'action, contrôle réciproque, identités, états, déplacement, actions et montages relevés ; transitions vérifiées à chaque échantillon.

Trois instances distinctes MayhemAbility_Strife_HeavyLand_C : mode Walking pendant toute l'observation, hauteur stable, durées environ1,16 /0,92 /1,06s. Cela identifie la réception lourde dans cette capture ; ne prouve pas à lui seul le verrou exact ni l'intention de déplacement de l'utilisateur. War a montré Rampage_Finish_Slam et AirDash_Land ; **aucun War_HeavyLand capturé**. Éviter de mélanger ces classes.

CDO et réflexion : AllowPlayerMovement est un booléen du joueur-ability à +161 ; AllowedActionsFlags et le mouvement/root motion sont des mécanismes distincts. Avant candidat, établir quel consommateur limite réellement la réception. Ne pas se contenter de changer un booléen supposé efficace. Garder l'impact, ses effets et les autres actions ; pas de Finish global ni déverrouillage en vol.

## OFF : nouvelle frontière de rendu identifiée

Les poses actives de Strife alimentent aussi les sockets des projectiles : ne pas écrire directement les os du personnage. La frontière SendRenderDynamicDataConcurrent du modèle (RVA17AD2C0) appelle l'objet de rendu à mesh+6C0, vtable2DC8930, méthode +20=1BCD990, vérifiés sur le modèle réellement possédé en lecture seule. Le chemin alloue des données dynamiques et les initialise via1BBFD50 avant soumission au rendu. C'est une piste pour séparer présentation et gameplay ; pas encore une ABI ou une matrice modifiée validée. Ne pas appliquer un changement après mise en file vers le rendu sans preuve de propriété et de synchronisation.

## Statuts

Recherche et captures **CONTRÔLÉES**, nouveaux comportements de ce lot **NON CONSTRUITS / NON VALIDÉS**. HUD général en surveillance après non-reproduction V1.73 ; autres icônes/effets P2 reportés comme demandé. Base, INI personnel, raccourcis, PAK, sauvegardes et branches protégées conservés.
