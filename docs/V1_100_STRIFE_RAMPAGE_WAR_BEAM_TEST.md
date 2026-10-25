# V1.100 - Rampage Strife et correction du rayon War

25 octobre 2026. EXPERIMENTAL ; aucun test en jeu de ce nouveau code. Source de depart debug db9b67f, derniere base explicitement validee V1.98. V1.99 est conservee, avec retour partiel : rayon War fonctionnel mais tourne de 90 degres a droite en native ET TPS ; geste en boucle. Ne pas traiter V1.99 comme entierement validee.

## Objectif et critere de reussite

Lot coherent des deux transferts restants : World Ender de War tire devant lui et maintient une pose sans repetition ; Rampage de Strife se deplace, respecte les obstacles, termine son effet et rend le controle, deux fois de suite. Comparaison native/TPS et retour Native obligatoires. Les trois transferts V1.98 doivent rester identiques.

## Rayon et pose de War

V1.99 utilisait le socket VFX_Aura du squelette de War, dont le repere artistique ne garantit pas l'axe de tir. Ce socket et sa recherche sont retires. Le composant MayhemWorldEnder prive, deja enregistre sur le bon acteur et attache SnapToTarget a sa capsule, fournit maintenant son propre repere : son FName natif complet devient DefaultWeaponMesh et DefaultMuzzleSocket reste None. Le FindActorMeshByName natif (RVA 0x5BE120) parcourt les MeshComponents et compare les deux parties du FName. Aucune soustraction arbitraire de 90 degres, aucun changement de camera ou d'asset partage. Il s'agit d'un candidat de correction, pas d'une preuve visuelle.

Le montage prive conserve les trois plages et les temps natifs de StartBeam/DestroyBeam. War_Channel_Start se joue une fois au debut ; la plage Fire utilise sa derniere image pendant toute la duree du rayon ; la derniere plage utilise War_Idle. La pose ne recommence plus trois gestes de canalisation. Les gestes charges et le repli idle utilisent toujours le squelette propre de War. Les autres chemins de conversion Geyser et Clone restent identiques.

## Rampage : mouvement natif, pose propre

Preuves hors ligne sur le Shipping EXE exact (SHA256 9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54) et les PAK :

- Montage_Play UE4 (0x1677E30) exige le meme Skeleton que l'AnimInstance et configure le RootMotionMontageInstance sans exiger un noeud de rendu correspondant.
- ExtractRootMotionForTrackRange (0x168E5E0) utilise la premiere piste ; remplacer ses clips par Idle avait donc supprime le mouvement en V1.96.
- Strife expose exactement UpperBody, TPS, Shoot_Left, Shoot_Right et Upperbody_Flinches. Son squelette connait DefaultSlot, mais son graphe ne l'evalue pas. Le FName natif est lu a base+0x3A32340 et son texte est verifie ; aucun index invente.
- La boucle du montage War_Atk_Rampage_Start_Montage et ses clips Start/Run portent le mouvement. Les quatre montages de fin Land/Slam1/2/3 terminent le pouvoir. Les branches Blueprint d'abandon sont 28 pour Start, 2 pour les fins ; une fin ordinaire aurait sinon lance une phase War supplementaire.

Le nouveau module ForeignRampageFeature utilise le scan borne existant (512 objets), pas un nouveau hook ou scanner. Classes, packages et serials sont verifies. Seul Strife local a pied, avec autorite et reciprocite controleur/pawn, est accepte. La preparation exige le sol. A chaque lancement, la reflexion du graphe verifie les cinq noeuds exacts et refuse DefaultSlot, doublon, offset inconnu ou cycle. Le montage prive garde les clips natifs seulement pour leur mouvement et notifications ; leur pose War n'est pas evaluee par le graphe de Strife. Le squelette du montage prive devient celui de Strife.

Segments, temps, sections (dont index de segment Loop), marqueurs etats/branchements restent byte-identiques ; aucune duplication de callback. Toutes les listes doivent etre distinctes des sources, avant mutation. Seuls le slot prive, les liens internes vers le montage prive et les sockets des impacts prives changent. Le socket SpawnSocket/BlastClass est trouve par reflexion, pas par offset devine ; l'impact Slam utilise la racine de Strife avec la meme classe de blast. Les assets/notifications source ne sont jamais modifies.

La fin auxiliaire War_LaunchAttack n'est consommee que pour le propre cast Rampage de Strife, avec marqueur faible par joueur et monde. Celui-ci est efface par la fin observee, le changement d'acteur/monde ou l'expiration. War d'origine reste natif. Aucun mouvement manuel, vitesse ecrite, delai artificiel ni touche imposee.

## Limites et controles

Le corps de Strife conserve sa locomotion, sans animation de charge/frappe dediee. Les notifications des clips source restent natives ; leurs sons/effets et les sockets propres a War peuvent demander une adaptation visuelle ulterieure. L'extraction effective du mouvement, les degats et les sorties sur collision necessitent le jeu. Contrat absent = abandon natif et transfert desactive pour la session, message borne dans le log. P2 non valide et reporte.

Tests de contrats : cinq phases, clips/marqueurs/callbacks source inchanges, refus des alias/CDO, socket d'impact prive, graphe sans piste War, serials invalides, proprietaire remplace, chemins natifs intacts, causes d'abandon 28/2, retour de lifecycle et compagnon borne au propre acteur. Ce sont des fixtures, pas une validation du moteur en execution.

## Essai utilisateur groupe

1. War a pied : World Ender en vue native puis TPS. Tourner le personnage, verifier tir devant, pose maintenue, degats, fin, mouvement et deux activations. Comparer a Strife natif. Si la camera est dans l'effet, reculer temporairement pour distinguer direction et visibilite.
2. Strife a pied : placer Rampage sur un emplacement. D'abord en espace libre : deplacement, fin automatique/relachement, reprise de marche et d'attaque, deux activations. Puis face a un obstacle ; comparer vue native/TPS. En cas de blocage, arreter ce transfert et revenir a Native.
3. Remettre les emplacements sur Native. Verifier Rampage original de War, World Ender original de Strife et rapidement Geyser/Clone/Caltrops valides.

## Conservation et rollback

168 cles INI, 55 ID Hotkeys et defauts inchanges ; la valeur stable 5 autorise maintenant Rampage et n'ajoute aucune cle. Stoneskin (ancien choix 6) reste Native, adaptation retiree. Aucun changement des cameras, systemes de visee/tir, manettes, roues, holsters, modeles, skins, flou ou Stinger. Pas de lancement/installation automatique. Garder INI/PAK/sauvegardes. Repli entierement valide : ZIP V1.98 ; V1.99 demeure archive experimentale partiellement testee. Toutes les 164 archives anterieures et main/archive/v1.43 restent intacts.
