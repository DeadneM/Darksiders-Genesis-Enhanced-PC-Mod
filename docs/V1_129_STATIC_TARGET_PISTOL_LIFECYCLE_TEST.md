# V1.129 - Static primary targeting and persistent pistol appearance

29 octobre 2026. CONSTRUITE / CONTROLEE ; EXPERIMENTALE, NON VALIDEE EN JEU. Base cumulative V1.128 observee dans le journal et la video utilisateur du 28 octobre ; main reste V1.124 validee. Deux objectifs : empecher Statique TPS de privilegier son ennemi assiste initial, et conserver les choix de modeles/OFF pendant les transitions d'interface. Compilation seulement GitHub ; aucun lancement ni installation locale.

## Preuves et correction Statique

La video montre une deviation laterale du faisceau. Sur l'EXE supporte (SHA256 dans la preuve jointe), 697FD0 cree le faisceau principal, le stocke dans arme+688, puis transmet la cible de l'interface d'entree a SetTargetComponent 617B90 depuis 6981CB (retour 6981D0). Le Tick du faisceau suit cette cible au lieu de sa branche libre, meme si notre vecteur TPS est correct.

Le nouveau filtre laisse cette assignation initiale sans cible uniquement pour le faisceau principal de MayhemStaticShot_Blueprint_C, retrouve dans les slots d'armes 2/3 de Strife local a pied, lorsque le fournisseur TPS Camera Forward livre un rayon valide. Le setter natif garde sa comptabilite ; aucune ecriture brute de cible, rotation, portee ou degat. Les autres appelants, les enfants/rebonds, World Ender, les autres munitions et la vue native restent natifs. Le chemin partage 3D aiming ON/OFF est conserve ; aucune modification XInput, touche ou joystick.

Le hook optionnel est protege par l'EXE exact et deux signatures. Son indisponibilite ne desactive pas les anciens hooks de visee. Limite : correction du choix assiste initial, pas une preuve de collision sur tous les destructibles, ni un correctif revendique a cheval/ultime. Si une autre assignation attire encore le rayon, relever le journal et le cas exact ; ne pas generaliser le filtre aux rebonds.

## Pourquoi l'ancien correctif pistolets etait insuffisant

V1.78 intercepte deja SetHiddenInGame et preserve OFF sur les composants connus. Les gates de focus/cinematique existaient deja a cette epoque ; elles ne sont pas imputees a V1.81. Le traitement dependait encore des composants caches et de l'observation gameplay. Les holsters du paquet de rendu partageaient l'expiration courte des poses de bras. Le module de modeles revenait aussi aux reconciliations gameplay ; la video montre non seulement le pistolet OFF revenu, mais un modele choisi remplace par le modele natif.

Remplacement du resolver cache seul par une resolution du composant LIVE a l'entree des setters : classe Strife/Despair, joueur local, controleur/pawn reciproques, role, corps/parent, slot courant exact et identites faibles. Meme table de huit slots pour pistolets/holsters/lames, avec noms Dagger explicites. Les modeles utilisent les quatre assets natifs deja charges et le setter deja employe par le mod ; pas de chargement ni de scan dans le hook. Un changement de composant est suivi avant le prochain observateur gameplay. Le choix Native garde la derniere intention native connue ; les objets/meshes inconnus restent natifs.

Les gates focus/cinematique sont retires du masquage des modeles, et l'expiration des poses ne concerne plus les holsters. Les poses de bras animes gardent leur fraicheur courte et leurs gardes melee/pouvoirs/menus ; aucun ancien solveur remplace, aucune pose perimee appliquee. Le hook de visibilite conserve les arguments de propagation natifs puis reimpose OFF uniquement au composant possede. Une assignation de mesh reapplique aussi la visibilite. Aucun catalogue de touches ou de menus, aucun hook clavier/manette ajoute. La politique obsolete est remplacee ; les mecanismes valides ne sont pas doubles.

## Controle et statut

Deux passes confrontees : comparaison de tous les modules et des corps de fonctions deja acceptes, puis revue independante des liens de propriete, identites, appels natifs et transitions. Resultat dans STATIC_REVIEW ; preuves de signatures et provenance dans NATIVE_PROOF. Trois suites etendues : ProjectileSpawn, PistolVisual et MountedHolsters. Elles testent les arguments transmis une seule fois, les exclusions, P1/P2, les rayons invalides, la conservation Native, la visibilite sans pose fraiche et le retour des composants remplaces. Ces fixtures ne prouvent pas le resultat visuel dans le jeu.

218 valeurs INI et 55 ID Hotkeys conserves ; config, UI, RawInput/XInput, roues, cameras/blur, Recovery, sons, Leviathan et capacites et autres fichiers de jeu inchanges. Les deux nouveaux hooks optionnels gardent le comportement precedent s'ils ne peuvent etre installes. Le risque nouveau se limite aux setters natifs scopes ; rollback V1.128 en conservant l'INI. Aucune ancienne archive supprimee/reecrite, main/archive preserves.

## Essai groupe

1. TPS Camera Forward, Statique : viser une caisse/pot proche avec ennemi tres loin ou au bord de l'ecran ; essayer 3D aiming ON/OFF et Hotstreak. Le faisceau principal doit suivre le tir, les rebonds apres impact doivent rester actifs. Comparer souris et manette ; ne pas annoncer les deux valides avec un seul essai.
2. Vue native : Statique doit garder son aide habituelle ; verifier rapidement une autre munition et World Ender.
3. Choisir un seul pistolet et OFF pour l'autre : pause, chacune des roues, perte de focus/overlay, retour gameplay, cheval et sortie ultime. Refaire avec les cotes inverses, puis deux OFF. Aucun retour momentane des modeles ou holsters masques.
4. Revenir a Native pour les modeles ; verifier melee/pouvoirs et animations de bras conservees. Coop P2 plus tard selon preference utilisateur, couverture fixture ne vaut pas validation coop.

Ne pas remplacer l'INI personnel. Le journal doit indiquer STATIC_PRIMARY READY puis le compteur initialAssignmentOnly quand le cas est utilise ; ce compteur prouve le chemin execute, pas l'impact en jeu.
