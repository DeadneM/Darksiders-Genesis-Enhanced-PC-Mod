# V1.76 — OFF de pistolet par main, candidat à pied

## Objectif, base, statut

Canal debug uniquement. Base validée V1.73 dans les essais rapportés ; V1.75 construite/contrôlée mais visée seule manette et HeavyLand restent non validés et OFF initialement. L'utilisateur demande de poursuivre comme si le lot fonctionnait et testera ensemble ultérieurement : cela autorise la suite cumulative, pas un changement de statut en VALIDÉ. Installation manuelle, aucun fichier du jeu remplacé.

Objectif de ce build : **OFF à pied de chaque pistolet**, modèle masqué et bras au repos dans les poses de tir, tirs conservés ; aucun changement de la pose qui alimente les sockets des projectiles. Critère : choix indépendants, retour Native correct, direction et munitions intactes, pas d'altération de la mêlée ou de l'autre joueur. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL**, 46/46 suites Windows x64 ; aucun résultat en jeu annoncé. [Reçu, ZIP et intégrité](V1_76_DELIVERY_RECEIPT.md).

## Fonction et limites visibles

Gameplay → Appearance, chaque main : Native / Right pistol model / Left pistol model / **OFF (foot, experimental)**. Valeur 3 dans les deux clés existantes PistolRightModel/PistolLeftModel. Defaults Native (0), INI et 55 ID Hotkeys conservés. Aucun raccourci ajouté.

Seul Strife normal à pied est ciblé. Ranger les armes puis rester immobile au moins750 ms après le retour natif au repos ; le menu indique la disponibilité. La pose native est mémorisée par personnage, modèle et génération, reconstruite après changement de modèle/possession. Sans pose, signature, contexte récent ou paquet compatible : affichage natif. Aucune pose universelle inventée. L'animation de mêlée, les actions inconnues, dash, coffres et équipements gardent leurs bras natifs. Le modèle OFF reste masqué à pied quand le candidat est actif.

**Limitation assumée : cheval et ultime gardent leur présentation native ; certains effets sont communs aux deux pistolets.** Les flashs/douilles connus avec socket de main sont filtrés pour cette main. Les effets de bouche connus attachés à VFX_Root_Beam/StrifeMuzzleFlash sont partagés et restent avec une arme visible ; ils sont retirés avec les deux OFF. Les projectiles, faisceaux de gameplay, impacts, sons, autres effets et références inconnues sont conservés. Ce candidat ne remplit donc pas encore la demande complète pour tous les effets et toutes les formes. Pas de masquage global d'un parent ou de tous les particles.

## Preuves natives et conception

Exécutable supporté SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Lecture privée du PAK d'origine et capture VM_READ antérieure de 2 939 échantillons, hiérarchie de 80 os, poses clavicule/bras/mains, notifies et sockets. Nouveau relevé ponctuel tenté : GAME_NOT_RUNNING, aucun lancement ni capture actuelle inventés.

- SendRenderDynamicDataConcurrent RVA17AD2C0 → objet à mesh+6C0, vtable2DC8930, Update1BCD990. Allocate/init du paquet avant sa mise en file vers le rendu. Init1BBFD50 possède **sept arguments, this compris**, confirmés par l'appel complet, les deux tableaux morphs/weights et le mode précédent. Tail-call1BCDEC0 retourne bool, conservé. Vtable du paquet2DC88A8 confirmée par constructeur1BAD250.
- ReferenceToLocal courant à paquet+10, précédent à+20, tableaux FMatrix64octets. Le calcul natif1BCFCB0/1BCFE40 lit les poses CS actuelles à mesh+5B0+16*selector(+5F4) et utilise les matrices inverse-référence ; le chemin des sockets lit ces poses aussi. Le candidat **lit seulement ces poses** et change la copie privée du paquet après initialisation, avant toute soumission. Aucun changement tardif d'un paquet déjà en file.
- Pose rangée stockée relativement à sa clavicule ; réapplication de 22 os de chaque branche choisie, tandis que tronc, jambes, écharpe, holsters et autre bras gardent leurs matrices natives. Ancien paquet utilise la dernière pose de rendu du bras pour éviter un ancien bras tendu dans les vecteurs de mouvement ; reprise après interruption initialise le précédent au courant.
- Masquage du modèle via SetHiddenInGame17AE540 (bit40 à+1CC, sans modifier bVisible20), enum natif1 sans propagation du flag aux enfants. Le hook observe les commandes natives ultérieures pour restaurer leur intention la plus récente. Un modèle déjà masqué n'est jamais réaffiché par défaut. Seulement les deux StaticMeshComponent exacts, propriétaires/parents vérifiés ; mesh et sockets restent présents.
- Filtre de notifies raccordé au hook particules existant du dash, sans nouveau hook concurrent de particules. SocketName+84 et template+38 confirmés dans169D0D0. Sockets propres aux mains et sockets racine différenciés par le Skeleton extrait. Aucun notify de projectile ou de son intercepté par ce candidat. Échec du filtre existant rend OFF indisponible.

Deux nouveaux hooks signature-gated : initialisation du paquet et SetHiddenInGame. Original appelé avec tous ses paramètres/résultat ; index de joueur, possession/génération, identités faibles, classe Strife, vie, rôle, propriétaire et squelette sont contrôlés. GUObject non parcouru en boucle ; aucun chargement/duplication d'asset. Callbacks de rendu n'appellent aucune fonction de gameplay ni allocation d'identité faible. Master-pose, tailles/compteurs inconnus et matrices invalides rejetés. Contrôles bornés ; après capture et avec Native/Native, aucun traitement de matrices. Gameplay hors pause/cinématique/roue et fenêtre du jeu active ; absence de contexte/paquet récent (250 ms) rétablit le comportement natif lors du prochain service game thread.

## Essais groupés et rollback

1. Strife à pied, ranger les armes puis attendre « OFF ready ». Native → OFF droit seulement → gauche seulement → les deux → Native. Vérifier modèle et bras, autre main, marche/visée et animations de mêlée. Refaire après changement de personnage, chargement et modèle d'armure ; comparer vue native/TPS et P2 si Strife y est joué.
2. Standard, Charge Shot, Lava, Graviton, Static et Beam, normal/Hotstreak : direction, origine et dégâts conservés ; pas de munition réinitialisée. Vérifier les flashs propres aux mains et la limite des effets communs avec une seule arme visible. Faisceaux/impacts de gameplay doivent rester présents.
3. Dash en déplaçant/visant/tirant, roues et équipement maintenu, mêlée, coffre, cheval/ultime, cinématique, Alt-Tab ; retour Native sans modèle manquant ni bras bloqué. Cheval/ultime et actions hors tir restent natifs dans ce candidat.
4. Nouveautés V1.75 dans le même lot : maintien/relâchement de visée seule avec un/deux boutons ; réception lourde OFF/ON depuis une chute assez haute, Strife puis War, native/TPS. Garder leurs statuts expérimentaux tant que non testés.

Rollback : remettre chaque main sur Native ; désactiver Heavy landing movement recovery et commandes Aim without firing si besoin. Retour complet : binaires V1.73 sauvegardés, INI personnel conservé ; les anciennes versions ramènent mode 3 à Native plutôt que d'appliquer une valeur inconnue. Aucun ancien ZIP remplacé.

## File suivante

Échange des six capacités toujours ouvert : les deux lookups Blueprint ne reconnaissent que leur famille, montages sur deux squelettes et dépendances de composants. Pas de sélecteur prétendu fonctionnel. Compléter OFF pour transformations/effets partagés après preuve ; HUD général en surveillance après non-reproduction, autres FX P2 reportés. Les tests automatiques et signatures ne valent pas validation du rendu ou du Recovery en jeu.
