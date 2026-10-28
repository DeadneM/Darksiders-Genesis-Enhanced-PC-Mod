# V1.128 - menu lisible et controles homogenes

28 octobre 2026. **CONSTRUIT / CONTROLE ; RENDU COMPLET NON VALIDE EN JEU.** Base V1.127 installee et conservee, issue de V1.124 validee. Reponse a la demande utilisateur : espaces plus lisibles, descriptions courtes et experience homogene. Nouveau lot uniquement UI sur debug ; main/archive/v1.43 preserves. Aucun jeu lance, aucune compilation locale et aucune installation V1.128.

Source `a9eb384e357bec61830d111606a9c1ee6fe45aca`, archive `6c3abcc6373088e21ba10404406946ba81a46790`. [GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40764748962) : **67/67 suites**, zero avertissement/erreur MSVC. Nouvelle suite avec le vrai Dear ImGui :12combinaisons de largeur240/480/990 et police10/13/20/32 ; champs/bouton Default dans la zone disponible, clic Default reel dans les lignes simples et repliees, aucune mutation sans saisie, styles restaures. Suite OverlayBindingScope conservee : ancien conflit Zoom In/Clear couvert. Ces tests ne remplacent pas une observation du menu complet en jeu.

## Presentation et conservation

- Aide courte sous les options, details au survol des descriptions marquees (?) ; signal commun aux onglets. Les indisponibilites, retours d'action et trois confirmations critiques restent visibles. Les statuts Ready repetitifs sont retires en fonctionnement normal.
- Marges/espacements/colonnes/controles adaptes a la police choisie. Curseur, saisie manuelle et Default partagent le meme composant. Listes a libelle au-dessus, largeur adaptee ; boutons associes renvoyes dessous si besoin. Les textes de statut peuvent revenir a la ligne. Style scope restaure en sortie et sur retour anticipe.
- Appearance regroupe Character models, Unlockable skins et Leviathan lightning, fermes initialement. Aucune option desactivee en fermant un groupe ; Leviathan reste separe. Les sous-titres redondants sont retires ; les explications des pouvoirs sont actualisees.
- Reception lourde : Recovery delay indique les secondes ; Ground stability time est sous Advanced landing settings. Meme cle et valeur0,05s, filtre de contact continu au sol ; aucune modification de la mecanique acceptee.
-218valeurs INI,55ID Hotkeys et modules de jeu inchanges : visee/camera/blur, Recovery, RawInput/XInput/roues/menus natifs, apparences/holsters/armes, transferts et Leviathan. Mutations config et commandes de menu dans le meme ordre,24appels numeriques litteraux (ID/pointeur/plage/defaut) et trois confirmations compares. [Deux passes confrontees](evidence/V1_128_STATIC_REVIEW.json). Pas de promesse de FPS.

## Livraison et essai

[ZIP cumulatif immuable](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6c3abcc6373088e21ba10404406946ba81a46790/downloads/DarksidersGenesis_V1.128_MENU_READABILITY_TEST.zip), 2382234octets, SHA256 `886CAD4459767D1B463BC880841F0F5ACFCD5E0EDF2DE78F3C50DD183315912A`. Quatre racines exactes ASI/DXGI/INI/README, methode0 sans compression et CRC valide, deux binaires AMD64, README/INI compares a la source compilee.192archives anterieures identiques,193au total ; updater debug selectionne V1.128 et recupere les memes octets. Defender ZIP et contenu : exit0/aucune menace, protections actives, aucune exclusion ni reglage modifie. Ce controle ne garantit pas tous les antivirus.

V1.127 reste installee pour les tests en cours. Pour V1.128 via updater ou installation manuelle, garder l'INI. Verifier Gameplay/Camera/Appearance, survol (?), groupes, police habituelle et grande, redimensionnement, saisie+Default et listes. Verifier la capture/Clear d'une entree non utilisee ; ouvrir puis annuler une confirmation Updates si souhaite. Rendu complet et confort a confirmer par l'utilisateur.

Tir souris TPS vers caisse/pot devant un ennemi, parfois attire jusqu'au bord de l'ecran : OPEN, aucun patch dans ce lot. Coffre/dash restent a mesurer ; coop plus tard. Rollback possible vers V1.127 en gardant son INI. [Perimetre et essai](V1_128_MENU_READABILITY_TEST.md), [CI](evidence/V1_128_CI_CHECK.json), [ZIP](evidence/V1_128_PACKAGE_CHECK.json), [updater](evidence/V1_128_UPDATER_CHECK.json), [Defender](evidence/V1_128_DEFENDER_CHECK.json).

## Observation du journal V1.127 pendant la preparation

Le nouveau journal confirme V1.127 chargee puis arretee normalement. Dash :7contextes suivis/7lectures observees, une seule fin de montage observee ; MOVE etait deja present, zero demande supplementaire. Les six autres contextes ne sont pas annonces comme six activations completes. Coffre :2contextes,2fins observees,2demandes MOVE. Aucune latence ni amelioration de deplacement mesuree. Cet exemple indique que l'essai de fin de dash n'avait plus de fenetre a ouvrir a l'arret observe ; ne pas enlever les gardes ni forcer un Finish pour fabriquer un effet. Cette lecture ne change pas le code de V1.128. [Journal et limites](evidence/V1_127_RECOVERY_OBSERVATIONS.json).

La passe intermediaire9f61ce5 a passe67suites et produit un artifact Actions, mais son push du ZIP numerote a ete refuse apres l'arrivee de la source finale a9eb384 ; le garde a ensuite refuse les sources obsoletes. Aucun ZIP intermediaire numerote publie dans debug ou propose par l'updater. Le lien ci-dessus utilise uniquement l'archive finale6c3abcc. [Details CI](evidence/V1_128_CI_CHECK.json).
