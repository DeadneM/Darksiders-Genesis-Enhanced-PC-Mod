# V1.70 — Profondeur de champ par vue et région native

20 octobre 2026. Base cumulative V1.69 `02aac6e0a04ad3fec7d2e7dfcc01425a0d08318f`, acquis conservés. **PRÉPARÉ / EXPÉRIMENTAL** : compilation Windows, paquet et rendu à vérifier. Debug uniquement.

## Retours et objectif

V1.69 : hauteur Léviathan, rangement du menu et netteté confirmés dans les premiers essais utilisateur. Nouveau contre-exemple : après l’arène, décor TPS flou dès environ deux mètres, arène également floue. La validation visuelle V1.69 est donc partielle. Objectif du lot : conserver la région nette du niveau par défaut et fournir des contrôles DOF explicites indépendants pour vue native et TPS. Réussite : pas de remplacement implicite de la région du niveau par celle du mesh ; réglages indépendants, persistants, utilisables à pied/cheval/ultime et en coop ; cinématiques préservées. Le rendu reste à valider par l’utilisateur.

## Preuve du défaut et correction

Capture VM_READ PID17116, 20 octobre, 439 échantillons / zéro erreur, scène Level_00 après le signalement. Qualité r.DepthOfFieldQuality constante 2/2. Volume PostProcessVolume2 actif et non borné : FocalRegion **3000 unités** (30 m), méthode 0, FarBlurSize15, FarTransition500. Sortie caméra TPS : focus **324,457**, région **environ55**, région override17/04 actif ; aucun forçage de méthode. Le mod écrase ainsi la région du volume appliqué ensuite. Cette preuve explique une zone nette beaucoup trop courte ; elle ne constitue pas une mesure du rendu GPU ni une preuve de tous les états d’arène.

V1.70 n’ajoute plus de région caméra si celle-ci n’en possédait pas : le volume conserve son autorité. Si la caméra en possède déjà une, le mod peut seulement l’élargir aux bounds du personnage et conserve une valeur plus large. Même correction focale et NearBlurSize0 de la V1.69. Aucun volume, aucune géométrie caméra et aucun chemin de visée modifiés. Un test de régression simule le mélange d’un volume3000 avec une caméra sans override : le résultat reste3000.

## Contrôles Camera → Depth of field

Deux groupes **Native view / TPS view**. Chaque groupe propose :

- **Disable depth-of-field blur**, OFF par défaut. Prioritaire sur le réglage personnalisé ; utilise Gaussian avec noyaux proche/lointain nuls, dans cette seule sortie de vue.
- **Customize background blur**, OFF par défaut. Quand coché : **Clear distance behind character**, 0–200m, valeur inactive par défaut10m ; **Blur amount**, 0–5% de largeur de vue, valeur inactive par défaut0,6%. Curseur, saisie manuelle et Default communs au menu.
- La distance personnalisée garde comme minimum l’arrière du mesh courant ; monture et cavalier sont réunis. Le mode personnalisé choisit explicitement Gaussian, fixe le plan focal au personnage, supprime le noyau proche et règle le noyau lointain. Le taux est absolu, pas un multiplicateur du volume. Transition, ouverture et autres effets restent natifs. Les unités décrivent les paramètres transmis au moteur ; rendu et perception restent à tester.

Les profils suivent la vue réellement produite, y compris vue native temporaire de P1/P2 et vues TPS maintenues en pause. Pas de CVar global pour ces contrôles, pas de hook ajouté, pas de pointeur de mesh retenu. Aucun nouveau raccourci. Les cinématiques ignorent les nouveaux profils, indépendamment de la case TPS cutscene. Le repli historique du zoom natif ne coupe plus globalement le DOF lorsque le profil natif personnalisé vérifié le remplace ; il restaure aussi la qualité lors d’une cinématique native observée.

Gardes : poids PP exactement1, méthodes connues0–2, disposition/consommateurs natives vérifiés. FarBlurSize +478, mask18/01, méthode+23 mask18/02, valeur Gaussian1 à2EBD608 ; focus/région/near déjà gardés. Disposition inconnue, valeurs invalides ou pose absente : sortie native conservée. OFF n’a pas d’écriture de restauration persistante : la prochaine sortie native fraîche reprend son effet.

Configuration : huit clés ajoutées, les146 anciennes clés et leurs valeurs fournies inchangées ;154 clés au total,54 identifiants Hotkeys. INI personnel conservé à l’installation. Le reset de géométrie TPS ne réinitialise pas ces préférences distinctes ; le reset global les remet OFF. Tests : parité INI/defaults/runtime, roundtrip des deux profils, limites/NaN, priorité disable, isolation des sorties, cinématiques, pose absente et préservation de tous les octets sans rapport.

## Autres sujets, état honnête

- **Chevaux : bug toujours ouvert.** Précision utilisateur : Bonus mount skins reste coché au chargement, les bonus ne sont plus sélectionnés et doivent l’être à nouveau. L’ancien retour « décoché » ne clôt pas ce cas. Les sauvegardes lues contiennent parfois les deux entrées montures, parfois seulement les armures ; sans chronologie contrôlée, pas de cause attribuée. Copie native peau joueur→sauvegarde5823C0 et sauvegarde→joueur5825B0 identifiée. Aucun patch de sauvegarde, de droit DLC, ni de sélection forcée livré.
- **HUD général après refus d’arène verrouillée :** ancien défaut confirmé, non reproduit dans le nouvel essai V1.69. Intermittent/ouvert, sans réaffichage global forcé ni correction annoncée.
- OFF complet pistolet et nouvelle approche Action Recovery restent ouverts. Autres sprites/effets P2 reportés selon l’utilisateur. Aucun changement de ces chemins.

## Essais groupés / rollback

1. Profils personnalisés et Disable décochés : arène puis retour au hub/mission, chapitre8 et autres angles ; vérifier personnage net et absence du mur de flou à deux mètres.
2. En TPS, Disable ON/OFF ; puis Custom, distance0/5/20m et quantité0/0,6/2. Contrôler l’effet des deux valeurs, la saisie manuelle et le retour au rendu du niveau.
3. Régler la vue native autrement que TPS ; basculer TPS/native temporaire, pied/cheval/ultime, changements Strife/War et coop. Chaque vue doit suivre son groupe ; contrôler aussi pause/roues et cinématique.
4. Redémarrer : conserver les réglages et raccourcis. Les skins chevaux restent un test de diagnostic, pas une correction de cette build.

Rollback : ancienne archive V1.69 conservée, ou V1.68 si le flou V1.69 gêne ; conserver son INI. Pas de modification main/archive, v1.43 stable ni des anciennes archives.
