# V1.74 — visée native sans tir à la manette

## Objectif et statut de préparation

Base V1.73, branche debug uniquement. Les trois essais V1.73 rapportés par l'utilisateur sont validés dans ces cas. Ajouter un maintien de visée indépendant, par joueur, avec deux entrées manette configurables comme le rangement. Un champ OFF laisse l'autre fonctionner seul ; deux champs OFF désactivent la fonction. Valeurs initiales OFF/OFF, sans migration des anciens réglages ni déplacement des 55 identifiants Hotkeys.

**PRÉPARÉ / EXPÉRIMENTAL, pas encore construit ni validé en jeu au moment de ce document.** La CI et le reçu de livraison préciseront les contrôles effectivement réalisés. Aucun fichier du jeu ouvert n'est remplacé. INI personnel, raccourcis, PAK, sauvegardes, main/archive et anciennes archives conservés.

## Preuves et périmètre natif

Exécutable pris en charge SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Enum ECharacterActions vérifié par lecture externe seule : AIM=31, AIM_RELEASE=32, tirs=18/20. Interface du personnage à +8D0 : dispatch 681D50 et query 669D30, vérifiés sur l'objet possédé. Le dispatch conserve les verrous natifs de saisie avant le gestionnaire d'action. Signatures du dispatch/query contrôlées avant un seul hook ; variante inconnue désactivée.

Le rendu lit l'état physique original de la manette et publie des valeurs sans déréférencer de personnage. Seul le callback synchrone du joueur appelle le dispatch natif après contrôle de possession réciproque, autorité, vtable et identité faible. Aucun événement clavier, Ctrl, appel externe dans le jeu, écriture d'octets AIM, ni événement de tir généré.

Le maintien ajouté et la source native sont suivis séparément. Relâcher la commande ajoutée préserve une visée native encore tenue. Annulation native respectée ; aucun redémarrage à chaque frame. Relâchement refusé par un verrou natif : seul ce relâchement reste en attente, avec délai de reprise borné. Menus, focus, cinématiques, roues, changement de propriétaire et reconnexion exigent un relâchement avant nouvelle activation. Échantillon manette périmé après 500 ms. Une ancienne identité n'est jamais rappelée après possession ou réutilisation d'adresse.

En TPS, le getter de direction existant admet aussi cette visée sans tir, selon la caméra propre au joueur. Son état n'entre jamais dans les drapeaux de tir, le filtre XInput ou les boutons/axes sortants. Le mode de visée montée native conserve sa règle existante. Seules les formes passant les contrôles d'interface sont éligibles ; autres laissées natives. Les associations natives des boutons restent actives, comme pour le rangement : choisir une commande ou combinaison adaptée dans le jeu.

## Critères de réussite et essais groupés

1. Gameplay → Aim without firing : OFF/OFF inactif ; un bouton seul dans l'un ou l'autre champ ; deux boutons nécessaires ensemble. Tenir vise sans tirer, relâcher termine la visée. Enregistrer puis relancer conserve les choix.
2. Strife puis War, vue native puis TPS : tourner/viser, bouger et dasher sans tir parasite ; tirer normalement avec la visée tenue, puis relâcher chaque commande dans les deux ordres. Les munitions spéciales et le comportement acquis restent conservés.
3. Ouvrir/fermer pause, roues et menu du mod, Alt-Tab, changer de personnage, débrancher/rebrancher : aucune visée bloquée ou réactivation avant relâchement. Cinématique conserve sa priorité.
4. Coop : chaque manette ne vise que pour son joueur, suivant sa propre caméra. Vérifier aussi cheval et ultime compatibles ; ces formes ne sont pas annoncées validées avant essai.
5. Régression courte : rangement à deux entrées, sélection dans les roues, échange complet des sticks et maintien équipement. Les deux nouveaux champs n'altèrent pas les raccourcis existants.

Revenir aux deux champs OFF désactive la fonction. Rollback binaire : remettre ASI/DXGI de la sauvegarde V1.73, en gardant l'INI personnel ; clés ajoutées simplement ignorées par cette ancienne version.

## File distincte

L'utilisateur demande aussi **trois emplacements indépendants pour Strife et War, chacun avec Native ou l'une des six capacités de Courroux**. Recherche native préalable : classes, emplacement sélectionné, propriétaire, coût, montage et dépendances au personnage. Aucun sélecteur inopérant ni échange aveugle de classes ajouté à cette préparation. OFF complet des pistolets reste ouvert ; HUD général en surveillance après essai V1.73 non reproduit ; ground slam après le reste ; autres FX P2 reportés.
