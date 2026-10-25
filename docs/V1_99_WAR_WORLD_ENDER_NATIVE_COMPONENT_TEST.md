# V1.99 - World Ender sur War

## Base, objectif et statut

Base cumulative `f703595606a410ea9c7817de22ea626a31b9985b` et ZIP V1.98. L'utilisateur vient de confirmer « oui tester et approuver » pour les gestes Geyser Strife et Clone/Caltrops War ; ces chemins sont maintenant VALIDES dans les cas rapportes. Stinger : rendu natif dorsal accepte, dossier cloture. V1.99 est un nouveau candidat EXPERIMENTAL ; les tests de fixtures et la CI ne constituent pas une validation en jeu.

Objectif : rendre le transfert World Ender utilisable par War, avec son propre composant, sa propre pose, le cout et la fin du pouvoir natifs. Reussite en jeu : rayon visible et orientable, degats, fin/annulation sans rayon persistant, mouvement et attaques rendus, second lancement possible, aucun changement des pouvoirs valides. Installation manuelle ; aucun fichier installe ni jeu lance par l'agent.

## Preuves natives et correction

EXE SHA256 `9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54`. Releves externes QUERY_LIMITED_INFORMATION|VM_READ seulement ; aucune ecriture ni fonction du jeu appelee par les lecteurs.

- V1.96 cherchait une ObjectProperty `Chaoseater` sur War. Son schema reel ne la contient pas, alors que son modele principal existe a `Mesh +0x380`, nom `CharacterMesh0`. Le journal rejete ne contient aucun CAST choice=2. Cela explique un refus du contrat avant playback ; ce constat ne prouve pas a lui seul que le nouveau rayon fonctionnera.
- `MayhemWorldEnder_Blueprint_C` est une SceneComponent derivee de MayhemBeamShot, pas un simple ActorComponent. Une creation/enregistrement sans attache ne reproduit pas son contexte natif.
- NewObject neuf arguments `0xAC57A0`, RegisterComponentWithWorld deux arguments `0x1750880`, K2_AttachToComponent de SceneComponent sept arguments `0x17A7310` (wrapper `0x1DD86B0`). Regles SnapToTarget pour position/rotation/echelle ; socket None, weld false. AttachParent reflechie `+0x108`, pas `+0xD0`. Nettoyage du seul composant cree via methode native Actor `0x163EFA0` si son proprietaire correspond.
- Source du rayon : noms reels du mesh War et du socket natif `VFX_Aura`, lui-meme sur `Bone_War_Root`, decalage natif (0,0,120). Recherche bornee dans les Sockets reflechies du modele/squelette. Aucun faux pistolet ni arme empruntee a Strife.
- Seul le composant cree recoit ces noms : DefaultWeaponMesh +0x698, DefaultMuzzleSocket +0x690. SourceMeshName +0x1F8 et SourceSocketName +0x200 de l'ability doivent rester None ; le resolveur natif utilise alors les valeurs de ce composant. Aucun champ d'ability ecrit ; CDO et templates sources inchanges.
- Blueprint StartBeam recupere le composant de SON instigateur, choisit le projectile suivant ses ameliorations natives et appelle SpawnBeam. Le wrapper natif d'ability `0x7AF3A0` mene a `0x6864B0`, puis `0x619760`/`0x6BC9A0`. Lookup de mesh par FName reel confirme a `0x5BE120`.
- Montage natif `Strife_Aim_WorldEnder_Montage`, duree 4.8999996 s : trois segments (Start, Fire repete trois fois, End). Copie privee native, squelette et geste War ; temps et repetitions conserves. StartBeam a 0.6564341 et DestroyBeam a 3.5871196, exactement une fois chacun. Rejet avant allocation si un callback manque ou change de temps.
- Etats `AnimNotify_EnableAbilityInterrupt` conserves avec duree et lien de FIN au bon instant ; sons/particules racine compatibles seuls. Pas d'effets sur sockets de pistolets et pas de retarget de squelette donneur. Nettoyage de l'ability native `0x669250` : DestroyBeam puis remise a zero de BeamShot +0x208. Aucun timer de remplacement.
- Decouverte des trois assets par la passe chargee existante (512 objets par tranche), pas de scanner supplementaire. Identites faibles avec numero de serie, controles d'acteur local/autorite/sol/mesh/monde ; arret de ce seul transfert pour la session en cas de contrat invalide.

## Rampage et limites

Rampage sur Strife reste retire. Son montage natif Start puis Run entre dans une section Loop auto-bouclante ; les deux sequences War portent du root motion. Les sept clips de deplacement Strife compares n'en portent pas. La pose idle de V1.96 ne transporte donc pas la charge. Ses variantes de dash ont en plus des notifications propres au dash, impropres a copier dans Rampage. Le cycle natif de fin/impacts est identifie, mais le transfert de mouvement compatible n'est pas prouve. Aucune vitesse forcee, touche simulee, fin par minuterie ou piste de mouvement War cachee n'est livree.

Le geste War est une canalisation compatible deja validee, pas l'animation de tir de Strife. World Ender sur War reste experimental : rayon, direction, rendu proche en TPS, sons, degats, cout, interruption, ameliorations/coeurs et rechargement de niveau a tester. P2 reste differe. Aucun changement du correctif de visibilite/alignement World Ender de Strife ni de la visee TPS.

## Nettoyage et controles

Le module Clone valide reste identique. Les vestiges World Ender/Rampage devenus inaccessibles dans ses fixtures sont retires ; la nouvelle suite World Ender teste la vraie structure War, sans propriete Chaoseater fictive. Les suites existantes couvrent les gestes et chemins conserves. Les tests verifient signatures/ABI de construction, enregistrement et attache, identites/proprietaires, refus de socket absent, nettoyage en cas d'echec, callback unique, durees d'etats et non-alias des tableaux sources. Les fixtures n'executent pas le jeu.

168 cles INI, 55 ID Hotkeys, valeurs/bindings par defaut, configuration utilisateur inchanges. 163 ZIP precedents conserves. `main`, `archive`, tag v1.43 intacts ; publication debug seulement. ZIP attendu a quatre fichiers a la racine : ASI, dxgi.dll, INI, README. Les preuves CI, hashes et URL immuable sont a inscrire dans le recu apres construction et controle reels.

## Essai groupe et repli

1. En mission, War a pied : mettre World Ender dans un emplacement deja debloque. Unlimited Courroux OFF pour verifier le cout. Lancer en vue native, viser plusieurs directions, verifier visuel/degats/fin/mouvement puis un second lancement.
2. Meme essai en TPS, pres puis avec camera reculee si le gros effet masque le rayon. Verifier annulation par une action autorisee et absence de rayon persistant apres changement de personnage ou de niveau.
3. Remettre l'emplacement sur Native. Strife World Ender reste fonctionnel ; Clone/Caltrops War et Geyser Strife gardent leur comportement approuve. Pas de demande de test P2 maintenant.

Si le pouvoir echoue, selectionner Native et transmettre le journal ; ne pas multiplier les casts apres un blocage. Repli : reinstalle le ZIP V1.98 valide en conservant son INI/raccourcis/PAK/sauvegardes. Aucun changement natif du joueur ou de la sauvegarde par les lecteurs.
