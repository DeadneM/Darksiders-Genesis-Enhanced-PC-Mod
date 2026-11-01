# V1.142 — aim assist accessible et ZIP binaire

**PRÉPARÉE / EXPÉRIMENTALE ; contrôles GitHub en attente.** Base V1.141 : menu et défauts acceptés par l'utilisateur, seul l'accès à l'aim assist est demandé plus visible. Branche debug uniquement, aucune installation ou promotion main.

## Objectif et critère

Présenter l'aim assist avec le même composant que les autres options et fournir exactement deux fichiers racine : DarksidersGenesisMod.asi et dxgi.dll. Réussite : case clairement accessible, réglages conservés, ZIP accepté par le nouvel updater sans toucher au profil existant. Une première installation manuelle depuis V1.141 est explicitement acceptée par l'utilisateur, seul utilisateur actuel du mod.

## Menu

Le petit TreeNode blanc est remplacé par DrawToggleFeature, déjà employé ailleurs. Case TPS aim assist visible directement dans les contrôles TPS ; mode global/par munition et valeurs immédiatement dessous quand elle est ON. OFF conserve les valeurs mémorisées. Calculs, portée, chaînes, champs INI et défaut global 50 % inchangés. Une seule aide courte, détails au survol. Le reste du thème V1.141 est conservé.

## Distribution et updater

- Nouveaux ZIP du mod : deux binaires uniquement. Aucun INI/README ni dossier englobant. README, carnet et reçu restent versionnés sur GitHub ; INI de référence conservé dans config pour contrôler les défauts compilés.
- INI existant : jamais remplacé, créé ou restauré par l'updater. S'il est absent, l'ASI le crée au prochain lancement à partir des défauts approuvés, chemin déjà couvert par le contrôle Windows UserDefaultsConfig.
- Moteur embarqué : accepte exactement les deux binaires ou les quatre fichiers historiques ; aucune combinaison intermédiaire. Les fichiers documentaires des anciennes archives ne sont pas installés. Chemins imbriqués, doublons, fichiers étrangers, tailles anormales et mauvais PE restent refusés ; checksum GitHub et contrôle du jeu fermé conservés.
- Nouvelle sauvegarde : paire binaire et état d'installation vérifiés. Anciennes sauvegardes complètes toujours reconnues et intégralement vérifiées avant écriture ; restauration uniquement des binaires, jamais de l'INI ni du README.
- Transition V1.141 : son ancien updater exige quatre entrées et ne peut accepter ce nouveau ZIP. Fermer le jeu et remplacer manuellement les deux fichiers une seule fois. Ensuite, utiliser l'updater embarqué. Aucun exécutable autonome supplémentaire n'est requis ni automatiquement publié.
- Workflow autonome conservé pour un lancement manuel futur, versionné V1.3 et sans README dans son ZIP. Le workflow du mod compile et teste déjà le moteur embarqué.

La fabrication debug et les définitions des prochaines fabrications main/release partagent une validation de ZIP à deux binaires, sans écrasement d'une archive numérotée. La définition main utilise désormais le numéro du header, au lieu de réécrire l'ancienne archive V0.72. Aucune branche main/archive ni release actuelle n'est modifiée. Workflows historiques et anciennes archives conservés.

## Contrôles et essai

Tests du moteur : nouveaux ZIP à deux fichiers et archives historiques à quatre ; refus des trois fichiers, fichier manquant/étranger, traversée de chemin, doublon, mauvaise casse et mauvaise architecture. Sauvegardes nouvelles et anciennes, rejet d'une sauvegarde corrompue avant écriture, paire restaurée avec état correspondant, INI/README personnels conservés, absence d'INI conservée jusqu'au lancement de l'ASI. Tests de configuration et de menu V1.141 maintenus. Compilation exclusivement GitHub Windows x64.

Après installation manuelle avec sauvegarde de l'ASI/proxy V1.141 et conservation de l'INI : vérifier la case TPS aim assist et les champs Global/Per ammunition ; OFF/ON conserve la valeur ; vérifier que les réglages personnels restent présents et que Check for Updates fonctionne. Le prochain remplacement effectif via updater doit encore être testé en jeu fermé ; les tests de fichiers ne constituent pas une validation de cette installation réelle.

Rollback : remettre les deux binaires V1.141, garder l'INI personnel. Les 228 clés/55 identifiants Hotkeys et tous les défauts approuvés restent inchangés. Aucun nouveau hook, aucune correction de tir ni changement de gameplay dans ce lot.
