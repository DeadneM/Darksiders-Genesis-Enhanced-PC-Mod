# V1.102 - Stoneskin pour les deux personnages, police et retrait du Rampage rejete

## Base et retours retenus

V1.98 validee pour Geyser Strife, Clone/Caltrops War et Stinger natif. L'utilisateur confirme World Ender War deja fonctionnel en V1.100 et les trois anciens transferts toujours fonctionnels en V1.101. Rampage Strife V1.100/V1.101 est REJETE (icone disparue, fonctionnement anormal). Aucun fichier source d'un transfert valide n'est remplace par l'ancien prototype. Source cumulative debug 042efcc, archives immuables.

Objectif : rendre Stoneskin selectionnable pour les deux personnages, ajouter un reglage lisible/persistant de police et retirer le chemin Rampage rejete sans regression sur les acquis. Succes en jeu : pouvoir avec cout/protection/expiration et retour de controle ; police changeable puis conservee a la reouverture ; slot natif retrouve apres ancienne selection Rampage. EXPERIMENTAL tant que l'utilisateur ne l'a pas teste.

## Retrait et icones

ForeignRampageFeature, RampageMotion, NativePropertyField et leurs deux suites specifiques sont retires du build, pas seulement masques. La valeur stable 5 ne remappe plus Strife ; War conserve son Rampage natif. Aucun War clip de mouvement, DefaultSlot sans noeud ou compagnon de charge n'est impose a Strife. Rampage Strife reste un sujet ouvert, sans nouveau candidat aveugle.

Le refus generique renvoyait result=0/unlocked=false/affordable=false. Il peut donc supprimer la presentation du slot tant que l'adaptateur n'est pas pret. Le nouveau refus demande une fois la classe et le debloquage du slot natif, conserve ces donnees et impose affordable=false. Aucun autre pouvoir ne doit etre lance et aucun debloquage n'est cree. Les fixtures verifient classe non nulle, progression conservee et lancement bloque. La cause exacte du ressenti general Rampage n'est pas prouvee : le log V1.101 releve READY, mais aucun CAST Rampage dans la capture consultee ; READY n'est pas une validation de la preparation ou du moteur.

## Stoneskin

Choix stable 6 retabli : War utilise War_Wrath numero 4 via le mapping natif deja valide ; Strife recoit la vraie classe MayhemAbility_StoneSkin_Blueprint_C. Son Blueprint applique le statut et son cout avant PlayMontage. Le contrat V1.83 est reutilise, pas l'adaptation de materiaux War ni le chemin Rampage rejete. Le scan borne existant recherche uniquement les objets deja charges, avec identites index/serie et packages exacts. Pas de nouveau hook, loader, timer ou simulation de touche.

Seul Strife local a pied, autoritaire, avec reciprocity controleur/pawn et animation propre, peut entrer dans la branche adaptee. Les deux montages War_Atk_Stoneskin[_Molten]_Montage sont identifies. La factory native 0x168A410 (signature et huit arguments controles, deja utilisee par Caltrops) cree un montage transitoire sur le geste Strife_Aether_Channel_Start valide lorsqu'il est charge, sinon Strife_Idle. Le taux conserve la duree du cast original. Le PlayMontage natif conserve sa fin ; un echec termine normalement le cast et desactive seulement ce transfert pour la session. Identites de l'acteur et de l'ability revalidees apres construction et lecture native. Les statuts, ressources, autres joueurs et assets source ne sont pas ecrits par le mod.

Limite : protection et variantes appliquees par le Blueprint natif, pas de promesse de rendu de pierre de War sur Strife ni de reprise des notifications du montage War (onde de choc, sons/effets d'arme). Les effets/degats des variantes demandent un essai ; ne pas annoncer toutes les variantes comme identiques a War. War natif et Stinger acceptes restent intacts. Tests de contrat : factory ABI, squelette/duree propres, fin, absence de relance apres echec, proprietaire remplace, scope P2 et War non adapte. Pas une preuve de protection mesuree en jeu.

## Police

Nouvelle cle [Overlay] FontSize, defaut 13.000, bornes 10–32 px. Lecture/sauvegarde refusent NaN/infini, restaurent le defaut ou bornent les valeurs. Aucun changement des 168 autres cles, des 55 ID Hotkeys ou de leur ordre. Pas de nouvelle atomique de gameplay : reglage local du menu uniquement.

ImGui exact epingle v1.91.8 : FontGlobalScale et PushFont/PopFont sont disponibles. MenuFontScope ajuste l'echelle seulement pendant le dessin du menu et ses popups, puis restaure echelle et pile de police, y compris une sortie anticipee. Le curseur natif/HUD du jeu n'est pas modifie. Le controle se trouve dans About → Menu appearance, groupe ferme par defaut, avec curseur, saisie manuelle et Default. Le changement prend effet au prochain frame.

Tests avec le vrai ImGui : taille demandee, popup, restauration de l'echelle et texte d'une autre fenetre ; tests d'INI ancien, valeur personnalisee, sauvegarde/relecture, valeurs invalides et reset. Pas de capture visuelle en jeu annoncee.

## Essais groupes et repli

1. Ouvrir About → Menu appearance, essayer 18/24 px et une saisie exacte, fermer/reouvrir le menu, verifier les listes de capacites et Default 13.
2. Strife a pied : Stoneskin dans un emplacement debloque, Unlimited Wrath/Health desactives pour comparer cout et protection ; verifier fin du cast, commandes disponibles et expiration du statut. Refaire une fois, puis remettre Native. Ne pas confondre Stinger avec le materiau du statut de base.
3. War : choisir explicitement Stoneskin dans un emplacement, verifier comportement natif et retour Native. Les emplacements verrouilles restent verrouilles. P2 differe.
4. Ancienne selection Strife Rampage : son emplacement doit redevenir Native. Verifier que les quatre transferts acceptes et les inputs/roues restent normaux. Si un transfert est en preparation/indisponible, son slot doit garder une classe/icone native et ne pas lancer le mauvais pouvoir.

Conserver INI/raccourcis/PAK/sauvegardes ; aucune installation ou relance du jeu par l'agent. 166 archives precedentes et branches main/archive/v1.43 protegees. Repli : V1.100 avec uniquement les transferts acceptes et Strife Rampage sur Native, ou V1.98 pour le sous-ensemble entierement valide avant World Ender. Prochaine etape : retour groupe Stoneskin/police ; Rampage et reception lourde restent ouverts, autres FX P2 reportes.
