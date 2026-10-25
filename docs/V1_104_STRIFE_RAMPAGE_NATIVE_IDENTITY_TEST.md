# V1.104 - Rampage Strife, identites de slots et mouvement natif

25 octobre 2026. Base V1.103 : les deux points d'essai reception lourde approuves par l'utilisateur ; scope limite aux essais rapportes, pas de validation P2 inventee. Stoneskin/police V1.102, Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. La suite demandee est explicitement Rampage sur Strife. Nouveau candidat EXPERIMENTAL.

## Objectif et critere de reussite

Strife lance Rampage depuis chacun de ses emplacements reaffectables, se deplace et inflige les degats natifs, respecte la collision et le relachement, termine puis rend les commandes. Deux lancements consecutifs en vue native et TPS, puis retour Native. Le Rampage propre de War et les cinq autres transferts retenus ne changent pas.

## Cause mesuree du refus precedent

Le jeu a ete prepare par l'utilisateur avec Strife a pied. Lecture externe QUERY_LIMITED_INFORMATION|VM_READ seulement, aucun appel natif, lancement du jeu, ecriture de memoire ou remplacement de fichier. Le premier lecteur restreint masquait le processus ; la lecture autorisee hors de cette restriction a retrouve le jeu ouvert. Un resultat GAME_NOT_RUNNING dans ce contexte restreint ne prouve donc pas sa fermeture.

L'ancien garde exigeait exactement UpperBody, TPS, Shoot_Left, Shoot_Right et **Upperbody_Flinches** par comparaison de texte sensible a la casse. La session expose **UpperBody_Flinches**. L'ancien garde est faux sur le relevé actuel ; les 17 autres contrats lus sont vrais : Strife local autoritaire a pied au sol, controleur/pawn reciproques, mesh/movement propres, anim Strife, squelette Strife_Skeleton, Bone_Strife_Root, Strife_Idle et son package, cinq classes de phases chargees, cinq slots uniques sans DefaultSlot, mode de mouvement 3.

Cela confirme un refus reel de Prepare, coherent avec READY sans CAST et l'icone absente V1.101. La disparition d'icone provenait aussi du retour de classe nulle deja corrige en V1.102, conserve ici. Le releve n'est pas un appel du Prepare natif ancien et ne prouve pas chaque etat de la session historique ; il reproduit sa condition fautive sur le jeu actuel.

Le nouveau garde compare les paires FName index/numero directement. Il exige cinq slots uniques sans numero d'instance et interdit que le slot de mouvement natif DefaultSlot soit present dans le graphe evalue. Les autres noms d'assets/classes restent controles. Aucun changement du helper global de noms, de la camera, des sticks ou des tirs pour contourner ce refus.

## Mouvement et lifecycle

Lecture actuelle RootMotionMode=3 a AnimInstance+0x30. Preuve EXE exact SHA256 9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54 : le tick 0x16774EB lit ce mode ; le booleen de ponderation est vrai seulement pour mode 2 (0x16774EF/0x16774F1), passe en R9 a 0x1683F30 par l'appel 0x167757A. Apres extraction a 0x168437A, la branche 0x168437F conduit en mode 3 a l'accumulation sans poids de slot (0x16843BD/0x16843C9), plutot qu'au chemin pondere 0x167A510. Ce mode est exige, jamais modifie.

Le constructeur prive conserve les clips War Start/Run porteurs du mouvement, la boucle de section et les quatre fins Land/Slam1/2/3. Le montage porte le squelette propre de Strife pour Montage_Play, sur DefaultSlot non rendu par son graphe ; il ne pose pas les os War sur son corps. Le moteur extrait le mouvement, avance les sections et gere collisions, degats, callbacks, annulation et fin. Le setter Skeleton 0x16C7130 ne change que le pointeur et GUID du montage ; il ne remplace pas ses segments.

Copies transitoires uniquement, listes distinctes des sources ; comparaisons des segments, marqueurs, sections, caches et evenements avant adaptation. Liens de montage et sockets d'impact prives rebindes sur la racine Strife ; classe de blast conservee. Aucun asset partage ecrit. Aucun nouveau hook : scan borne existant et points d'adaptation de classes/montages deja utilises. Pas de vecteur/vitesse ecrit, rotation forcee, input physique ou timer de fin.

Contrat absent = refus avant lancement avec raison bornee PREPARE. Echec de construction/lecture native = abandon Blueprint 28 pour Start ou 2 pour les fins, puis transfert indisponible pour cette session. Meme l'echec de capture de l'identite d'un callback encore vivant emprunte l'abandon, pour eviter de laisser le cast sans montage. Acteur/ability revalides apres construction et lecture native ; instance remplacee = aucun appel sur l'ancien objet. La fin auxiliaire War_LaunchAttack n'est neutralisee que pour le propre cast Rampage, avec marqueur faible acteur/monde par joueur.

Limites : la pose reste la locomotion de Strife, sans animation de charge/frappe dediee ou squelette War. Notifications des clips sources, sons et points d'attache propres a War peuvent demander une adaptation visuelle ulterieure. Le mouvement, les degats et les sorties de ce montage ne sont pas valides par un relevé passif ou une fixture. P2/concurrence des deux Rampage ne sont pas valides en jeu.

## Revue et conservation

Premiere passe : cause sensible a la casse reproduite par lecture actuelle, conditions FName/mode de mouvement verifiees, branche moteur examinee hors ligne. Deuxieme passe : regression de casse reelle et historique dans les fixtures ; refus de DefaultSlot rendu, doublons, numero d'instance, graphe invalide et modes 0/1/2 ; cinq phases, serials, proprietaires, abandons, relance, compagnon et slots semantiques controles. Ces deux passes traitent le meme garde identifie ; elles ne remplacent pas l'essai du pouvoir.

Le bloc de montages accepte avant MotionLayout est compare a la base V1.103 : Geyser, gestes propres, Clone et rayon War inchanges. StonePlay et les autres fonctions retenues du wrapper sont compares. HeavyLandingFeature, ses tests, font, ressources, config, XInput/souris, visee/camera, roues, horse et autres acquis sont preserves. 169 cles INI et 55 ID Hotkeys ; la valeur reservee 5 autorise de nouveau le candidat Strife, defauts Native inchanges. Les 168 anciennes archives et main/archive/v1.43 sont preserves.

## Essais et repli

1. Installer manuellement jeu ferme en gardant l'INI. Player Wrath abilities : Strife → choisir Rampage sur un emplacement deja debloque. Aucun input impose ; utiliser sa commande reaffectee habituelle.
2. A pied, espace libre : lancer, verifier deplacement et degats, relacher, verifier fin puis marche/attaque ; lancer une seconde fois. Comparer vue native et TPS.
3. Essayer face a un obstacle, puis chacun des trois emplacements ; le personnage doit toujours rendre les commandes. En cas de refus ou blocage, revenir a Native et garder le journal. READY prouve l'initialisation, CAST player/phase prouve le passage au montage ; aucun des deux ne prouve les degats.
4. Remettre Native et verifier Rampage de War. Les capacites et la reception lourde deja validees sont conservees, sans nouvelle campagne exhaustive demandee. P2 plus tard.

Repli : V1.103, choix Rampage de Strife=Native (la valeur 5 sera normalisee par cette ancienne version), garder tous les reglages et sauvegardes. Aucune installation ou relance du jeu par l'agent. Prochaine etape : retour Rampage cible et analyse du CAST/refus reel ; rester sur ce sujet jusqu'a sa resolution.
