# V1.71 — Lot cumulé rendu, reprise de mouvement et observations des skins

20 octobre2026. **CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL**,44/44 suites Windows x64 et zéro avertissement MSVC ; effet en jeu à vérifier. [Reçu et lien](CURRENT_STATE.md). Debug uniquement. Base cumulative V1.70, source3ed44ddf0aba240e2b85a96fa2f8d9cdec231939, installée mais rendu non encore testé. Acquis validés V1.66–V1.69 conservés ; V1.69 a le contre-exemple de flou après arène corrigé en candidat V1.70. L’utilisateur souhaite un essai groupé, pas des demandes de test successives.

## Objectif et critère de réussite

Conserver le rendu DOF par vue V1.70 et essayer une reprise du déplacement après le déplacement utile du dash, par une vraie cause MOVE native, indépendamment de TPS et des bindings. Réussite : à pied, à la fin du déplacement réel, reprendre le mouvement sans glissement, dash écourté, répétition infinie du premier dash, perte de visée ni tir Standard parasite. Tester Strife après ses trois dashs puis le quatrième et War. Le lot n’annonce ni réparation des coffres ni des attaques d’atterrissage. La nouvelle option reste OFF par défaut et n’est pas une validation de gameplay.

## Nouveau candidat Dash movement recovery

L’ancien ActionRecovery reste retiré, avec son ID3 réservé et ses anciennes clés inertes. Nouveau réglage Features.DashMovementRecovery=0, nouvelle action Hotkeys54 (55 identifiants au total), aucune touche par défaut. Case Gameplay → Player, pas de nouveau curseur de temps.

Le tick existant observe le dash après le tick natif complet, sans ajouter d’interception. Type dash, mouvement natif Custom6/5, instigateur/composant réciproque, santé/autorité et propriétaire local sont déjà contrôlés. Ajouts :

1. Avoir observé la décélération positive de cette même activation ; un début d’observation sur un compteur déjà négatif n’autorise rien.
2. Compteur224 arrivé à zéro ou en dessous, durée228 valide, personnage à vitesse totale au plus1 unité/s ET déplacement réel de la racine au plus0,01unité entre observations, pendant deux observations. Nouveau monde/propriétaire, horloge réinitialisée, trou de250ms, valeur invalide, pause/cinématique ou option OFF : état expiré.
3. Demande native MOVE29 réellement maintenue, cause MOVE1 pas déjà disponible dansB0 ; indépendance souris/manette/TPS.
4. Au plus une demande par activation, via le virtuel natif+2A0 vérifié599310(ability, MOVE1, true). Cette fonction ajoute la permission, émet son événement, puis repasse par les actions natives en attente/maintenues. Pas d’EndAbility avec NONE, d’écriture de timer/mode/masque, d’arrêt de montage ni de touche simulée. Une demande refusée n’est pas répétée chaque frame.

Preuve native EXE exact :5B93F0 décrémente224, divise par228 puis prend max(ratio,0), calcule la vélocité au+10C, et laisse le mouvement natif l’appliquer.5B2CE0 traite la cause et mène à la fin/au nettoyage natifs.599310 ajoute/supprime une cause dansB0 et rejoue l’action réelle via5B2C30 et le gestionnaire de commandes. Signatures et vtable+2A0 contrôlées ; getter d’action669D30 →635100 conservé. Le bénéfice dans la fenêtre actuelle et le comportement Strife/War/P2 restent **NON VALIDÉS**. Le log dit MOVE_PERMISSION_REQUEST, jamais succès de reprise.

OFF empêche les prochaines demandes. Une commande déjà acceptée par le jeu achève son cycle natif ; pas de restauration artificielle d’un ancien masque d’ability. Le correctif AIM/tir déjà validé reste actif indépendamment de la case.

## Skins chevaux : diagnostic, pas une réparation annoncée

SkinSelectionPage.OnSelected copie GetPlayerSkinData, remplace une entrée via SetSkin puis transmet toute la liste à SetPlayerSkinData. Les Blueprints GameInstance/PlayerState relus ne contiennent pas de filtrage de skin. Le natif possède une liste locale au+640 et un getter de liste au+650 ; celui-ci peut référencer une autre liste, comme le montre l’initialisation57AFF0. Le chargement574FB0 et la sauvegarde5823C0 copient des listes natives ; le point de perte des montures n’est pas prouvé.

Observation bornée ajoutée au callback joueur existant, uniquement avec Bonus mount skins coché : deux listes comparées en lecture seule, au plus une fois/150ms et24 changements journalisés par session. Tableaux24octets, taille/capacité≤32, classe/table/package/ligne exacts pour Despair/Rampage. Log SELECTION_READ_ONLY : own/shared count et bonus masks (Despair1, Rampage2), alias éventuel. Aucun hook, choix forcé, droit DLC, grant ou écriture save ajouté. Ce code doit être retiré ou réduit quand la perte est identifiée ; diagnostic ≠ fonctionnalité. Nouveau test : données natives décodées sans modification, listes vides distinctes des listes invalides, bornes respectées.

## OFF complet pistolet : avancée, pas livré

Le graphe Strife possède des slots Shoot_Left et Shoot_Right distincts, mais la pose de visée passe aussi par des couches partagées depuis Bone_Strife_Spine1. Couper un slot seul ne garantit pas de baisser le bras ; arrêter son montage pourrait retirer les notifies qui créent les balles. Le modèle/effets/pose doivent donc être isolés par la vraie couche visuelle. Le sélecteur actuel Native/Right/Left et ses holsters validés restent inchangés. Aucun OFF incomplet affiché comme terminé.

## Essais cumulés proposés, à faire ensemble

1. Sans custom DOF : arène puis retour mission/hub et chapitre8 ; absence de mur de flou proche, personnage net.
2. Profils native/TPS différents, Disable ON/OFF puis Custom distance0/5/20m et quantité0/0,6/2 ; angles, pied/cheval/ultime, échange Strife/War, pause/roues, cinématique, coop et redémarrage.
3. Dash movement recovery OFF puis ON : déplacement maintenu, puis nouvel appui de direction après le dash, visée, tir et munitions spéciales ; vue native et TPS. Strife : trois dashs puis quatrième, War ensuite. Pas de test coffre/attaque au sol annoncé. Si aucun bénéfice net ou régression : OFF, log conservé, candidat à rejeter plutôt qu’empiler des corrections.
4. Avec Bonus mount skins coché : choisir les deux montures, jouer jusqu’à une sauvegarde habituelle, recharger puis redémarrer. Noter si les choix restent dans la page ; les logs et sauvegardes seront lus ensuite sans les modifier. Ce point reste OUVERT.

HUD général après refus d’arène verrouillée intermittent non reproduit dans le dernier essai ; pas de réaffichage forcé. Autres sprites/FX P2 reportés. Anciennes archives, main/archive/v1.43, INI/bindings/sauvegardes/PAK à conserver. Rollback : désactiver Dash movement recovery ou réinstaller V1.70 en gardant l’INI personnel.

## Contrôles et recherche complémentaire

Les simulateurs de compteur d’actions et de lecture de pose ont été mis à jour après un premier échec de compilation de tests ; le passage final44/44 fait référence au commit indiqué dans le reçu. Les compilations remplacées/annulées n’ont publié aucune archive. Le test du callback couvre les gardes, la permission MOVE1 une seule fois et la préservation du tick, de la durée, du lifecycle et des octets acteur/mouvement. Relecture privée de167fonctions Blueprint sans arrêt de décodage, longueurs mémoire concordantes.6sauvegardes copiées pour comparaison, sans modifier les originales.
