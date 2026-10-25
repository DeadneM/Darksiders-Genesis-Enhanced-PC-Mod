# V1.107 - Rampage : enchainement des phases natives

**Objectif** : conserver l'association du Rampage Strife pendant le passage entre Start, une frappe/atterrissage et le compagnon final. **Critere de reussite** : deplacement, frappe/degats, relachement, reprise marche/attaque et second lancement en native/TPS, sans blocage. EXPERIMENTAL, NON VALIDE EN JEU.

## Retour et preuves

V1.106 REJETEE pour Rampage dans l'essai rapporte : le deplacement existe, mais Strife reste bloque ensuite. Audio V1.106 non teste, differe par l'utilisateur ; aucun changement audio dans V1.107. La base validee reste V1.103 et les transferts deja acceptes. Le lecteur externe a trouve le jeu ferme lors de cette reprise : aucune nouvelle capture live, aucun lancement ou appel natif par l'agent.

Journal du 25 octobre : 18:59:14.820 CAST phase=0 ; 18:59:15.233 stopped phase=4 orphan finish phase. La phase Slam3 est donc demandee par le jeu, puis refusee avant le constructeur. Le code Observe effacait le marqueur si l'ancienne instance Start etait inactive ; l'ancien intercepteur de compagnon acceptait aussi un marqueur qui n'avait pas encore de phase de fin. Ces deux transitions prematurees sont corrigees. Le journal ne dit pas laquelle a efface le token dans cette session ; la fixture reproduit explicitement le cas Start termine, Observe, puis nouvel objet Slam3.

## Token de chaine et copie privee

Le token ne conserve plus l'ancienne instance de capacite. Il contient seulement identite faible de l'acteur, monde et numero de phase. Fin normale de Start : token conserve. Une seule phase Land/Slam est permise depuis Start ; le nouveau callback est controle par ses propres identites, proprietaires, classe/package et etat actif. Le compagnon natif War_LaunchAttack ne consomme le token qu'apres une phase de fin. Nouveau depart remplace le token ; monde/pawn/monture/serial different ou echec le supprime. Aucun appel sur une ancienne instance, timer ou touche simulee. Le token peut rester en attente sur le meme acteur si le jeu ne produit ni fin ni compagnon ; il n'autorise qu'une seule phase Rampage nommee, pas une autre famille de capacites.

La verite des tableaux d'evenements est la source Rampage controlee. Les evenements sont relus depuis cette source et reconstruits dans le tableau prive non partage ; sockets/liens prives adaptes comme avant. L'ancienne comparaison brute avec les notifies du clone est retiree : NewObject peut instancier ses propres objets Notify. Ce comportement precis n'a pas ete releve dans la session bloquee, puisque l'ancien garde orphelin refusait la frappe avant construction. Cette modification aligne le constructeur sur les transferts retenus et couvre les copies natives normalisees dans les fixtures. Les sections ne tolerent que le relien du montage source vers sa copie ; le reste reste compare. Clips de mouvement, vitesse native 1,5, marqueurs, etats, boucles et alias restent controles.

## Animations proposees

PAK original lu et index valide par SHA1 : Strife_Dash (1,4333333 s), Strife_AirDash_Fall et Strife_AirDash_Land_Still (1,6333333 s) existent. Dash/atterrissage ont ete extraits dans le dossier d'audit seulement, sans toucher au jeu. Les animations comportent leurs propres notifies. La piste actuelle extrait le mouvement depuis les clips War, avec le squelette de montage Strife et un slot non rendu. Remplacer directement ces segments par Dash ou sa reception ferait perdre ce mouvement ou changerait les actions/callbacks. Une piste visuelle distincte avec notifies isoles devra etre examinee apres validation de la sortie de Rampage. **Pas d'animation de dash injectee dans V1.107 ; pose locomotion actuelle conservee.** Pas de nouvel allocateur, de deuxieme montage concurrent ou de nouveau hook de rendu introduit pour cet essai.

## Deux revues et conservation

Premiere passe : journal plus lecture du lifecycle ; difference entre capacite Start terminee et chaine Rampage terminee. Deuxieme passe : fixture de deux objets differents, Start inactif observe avant Slam3, fin de Slam observee avant compagnon, token consomme une fois, compagnon premature non intercepte, contextes et serials invalides, doublon de fin refuse. Constructeur : notifies natifs normalises reconstruits depuis la source, section deja reliee a la copie acceptee, alias/cache incorrects toujours refuses. Les tests ne prouvent pas la reprise des commandes du moteur.

Prefixe des constructeurs retenus et helpers audio compares inchanges a V1.106. Reception lourde validee, camera/visee, souris/XInput, roues, cheval, skins, jauges et reglages preserves. Aucun nouveau parametre : 169 cles INI et 55 ID Hotkeys. 171 anciennes archives conservees ; main/archive/v1.43 intacts. Compilation uniquement GitHub, ZIP cumulatif quatre fichiers racine, updater debug. Aucun lancement/installation par l'agent. Resultats CI et archive a consigner apres compilation.

## Essai et repli

Strife a pied, emplacement Rampage debloque : vue native puis TPS. Lancer puis relacher ; verifier frappe/degats, marche/attaque apres fin, second lancement, puis obstacle. Garder le journal s'il bloque encore : phase 0 seule, phase de fin jouee ou motif de refus permettent de distinguer le prochain point. Rampage original War a conserver. Sons et P2 restent differes selon la decision utilisateur.

Repli V1.103 avec Rampage Strife sur Native, INI/raccourcis conserves ; ne pas poursuivre la session de test si commandes bloquees. Suite : valider la sortie du pouvoir, puis adaptation visuelle Dash/AirDash suivant la proposition utilisateur, sans remettre le mouvement natif en cause.
