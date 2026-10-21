## V1.74 — construite et contrôlée, installation manuelle reportée au prochain lot

Base cumulative V1.73, ses trois essais rapportés validés. Nouvelle visée manette sans tir : **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, pas validée en jeu. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39498950576) : **44/44 suites, zéro avertissement MSVC**. Source `61d9d4b01405ceb45ae802c348cce6dcfaf6a053`, archive `53e2b1645f83e2449eea98d01d3f19bc39ab921f`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/53e2b1645f83e2449eea98d01d3f19bc39ab921f/downloads/DarksidersGenesis_V1.74_NATIVE_PAD_AIM_TEST.zip), 831912 octets, SHA256 `578DC4AC279472C28556F7B930AFCB2FB6C4E6D53D84E4E8B91616A7AC29AA6B`. Quatre fichiers racine, CRC, AMD64, 156 clés fournies, 55 ID Hotkeys conservés ; les 154 anciens réglages gardent exactement leurs valeurs. Les 138 anciennes archives, main/archive, tag v1.43 et fichier de reprise préexistant sont inchangés.

Le README du ZIP contient la préparation V1.74 avant CI sous le titre historique de sa base V1.73 ; le présent reçu et la version compilée V1.74 font référence. L'archive publiée reste immuable. Une tentative d'installation s'est arrêtée dès le contrôle GAME_RUNNING, **avant sauvegarde ou remplacement**. L'annonce précédente de fermeture déduite d'une lecture sandbox était erronée : le contrôle final avec les permissions d'installation a vu le processus actif. Aucun fichier du jeu remplacé. L'utilisateur précise ensuite qu'il installera **manuellement à la fin de sa session** et demande de regrouper davantage de travail ; ne plus demander sa fermeture ou installer automatiquement ce lot.

Suite : capacités et réception lourde, puis OFF pistolet selon preuves. Choix demandé : trois emplacements par personnage, Native ou n'importe laquelle des six capacités. Chaque Blueprint GetWrathAbilityById ne reconnaît que ses propres trois ID ; les six montages utilisent deux squelettes distincts. Le remplacement de FName seul serait inopérant. Ne pas livrer six options présentées fonctionnelles avant adaptation et contrôles des animations/composants. [Recherche du prochain lot](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md).

Capture VM_READ achevée : **7 480 échantillons, trois lectures incohérentes rejetées**, six capacités natives des deux personnages relevées. Trois HeavyLand de Strife au sol, position verticale stable, durées observées d'environ 0,9–1,2 s ; aucun HeavyLand de War capturé. Rampage Slam et AirDash Land de War sont distincts. Clarification utilisateur : Recovery vise **la réception lourde automatique après chute**, pas l'attaque plongeante. Pas encore de nouveau correctif Recovery construit ou testé.

---

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
