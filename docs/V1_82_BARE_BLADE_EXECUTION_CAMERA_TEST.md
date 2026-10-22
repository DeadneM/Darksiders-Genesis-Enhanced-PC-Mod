# V1.82 — effets des lames masquées et suivi des exécutions

**V1.82 CONSTRUITE / CONTRÔLÉE : [50/50 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39834733781), zéro erreur et zéro avertissement MSVC. Nouveaux effets et suivi vertical EXPÉRIMENTAUX, non validés en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/03a45347f8484ca4af9fa17cf5b49d3e66062cf9/downloads/DarksidersGenesis_V1.82_BARE_BLADE_EXECUTION_CAMERA_TEST.zip) · [Reçu, empreintes et rollback](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_82_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI et ses raccourcis.

22 octobre 2026. Base V1.81 `adb5ae898f5ebab45ae0f11b6bd1c0cda7fb165f`, branche debug exclusivement. L'utilisateur a répondu « 1. ok / 2. ok / 3. ok » aux essais V1.81 : animation désarmée des bras OFF, conservation mêlée/pouvoirs puis retour, transitions. Ces cas sont VALIDÉS ; aucune extension implicite à la coop ou à toutes les animations. Le code V1.81 correspondant reste inchangé.

## Objectif et critères

1. Épée de Strife masquée : effet visuel réduit et placé à sa main, seulement du côté concerné lorsqu'il peut être identifié. L'autre épée, les coups, dégâts et portée doivent rester identiques. Deux épées masquées : compacter aussi les arcs communs connus.
2. Exécution TPS faisant monter le corps au-dessus de la capsule : la caméra accompagne la hausse puis revient au cadrage habituel. Exécutions au sol, cinématiques et autres actions gardent leur cadrage. La visée et les points de tir ne suivent pas un faux déplacement de la capsule.

**EXPÉRIMENTAL ; résultat visuel non validé en jeu.** Compilation et fixtures ne constituent pas les critères de réussite ci-dessus. Contrôles et paquet final à consigner dans le reçu après CI.

## Preuves et périmètre

EXE supporté : 62 113 280 octets, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. PAK lu avec SHA1 de ses exports contrôlé, fonctions et appels natifs désassemblés ; aucun asset du jeu n'est publié. Relevé externe VM_READ uniquement ; aucun appel ou patch externe de mémoire.

### Effets de mêlée

Les séquences des trois attaques au sol et deux attaques aériennes de Strife utilisent des AnimNotify_PlayParticleEffect. Les templates exacts `VFX_strife_meleeSlice`, `VFX_strife_meleeSlice_strong` et `VFX_strife_stabSingle` ont des offsets autour de la racine. Les notifications des traces Dagger_L/Dagger_R permettent d'identifier les deux côtés des premières attaques ; plusieurs arcs de fin de combo sont partagés. Le nom de l'effet seul ne suffit donc pas à choisir une main.

Le hook existant de notification conserve les exclusions de tir/dash/pistolet OFF, puis ouvre un contexte temporaire pour ce seul notify possédé. Un nouveau hook facultatif du SpawnEmitterAtLocation transformé (`189A140`, cinq arguments) ne change qu'une **copie du FTransform de cet appel**, si monde, template, séquence, offsets et réglage concordent. Tous les arguments, valeur de retour et appels natifs sont conservés. Pas de changement d'asset, CDO, notification, socket, animation ou trace de dégâts.

GetSocketLocation natif `17A2380` est confirmé par le virtuel mesh `+3F8` et sa signature ; les os Hand_R 43 et Hand_L 20 sont aussi validés par leur nom exact et leur mesh propriétaire. La rotation et la durée de l'effet restent natives ; origine sur la main, échelle XYZ multipliée par 0,4. Ce facteur est un candidat visuel fixe, sans nouvelle clé INI. Les arcs partagés ne changent **que si les deux épées sont masquées**, pour préserver le côté encore équipé. Les autres effets, actions inconnues, monture et ultime restent natifs. La couverture n'est pas déclarée universelle.

### Suivi des exécutions

La capture du 22 octobre 20:19:48 contient **4 342 échantillons**, trois lectures incohérentes rejetées, dont **211 lignes de finisher** Strife/Nephilim. Le bassin reste proche de la capsule, environ −40 à +3 unités : **aucune exécution haute observée**. L'utilisateur confirme que ces exécutions étaient au sol et n'avaient pas besoin d'un suivi. Elles constituent la référence de non-régression, sans prouver le comportement en hauteur. War en exécution n'a pas été observé dans cette capture.

Le type natif MayhemPlayerFinisherAbility, de `/Script/DarksidersGenesis`, est recherché dans l'ascendance bornée de l'aptitude active réellement possédée par le personnage local. Une simple détection d'animation ou de bouton ne suffit pas. Le bassin animé est lu via GetSocketLocation sur le mesh vivant. Le décalage habituel bassin/capsule est enregistré hors exécution, puis figé pendant celle-ci. Seule une hausse supplémentaire supérieure à 40 unités ajoute un décalage vertical lissé à la sortie TPS et à son point de mise au point. Le calcul horizontal, la classification caméra et les poses de visée restent inchangés. Deux états séparés pour P1/P2 ; aucun nouveau hook caméra ou entrée.

Identité de joueur/personnage/monde/mesh différente, absence de baseline normale, lecture invalide, trou de plus de 300 ms ou déplacement anatomique invraisemblable : suivi abandonné. Une reprise au milieu d'une exécution après un long arrêt peut donc rester native jusqu'à la prochaine baseline. La référence testée est la hausse animée, sans anticipation ni suivi d'une cible ennemie. Si une exécution haute utilise une autre classe ou un autre mécanisme, elle reste à capturer plutôt que d'étendre le correctif par supposition.

## Acquis et travaux encore ouverts

166 valeurs INI et 55 ID Hotkeys conservés, aucune nouvelle clé. Sources de rendu des bras OFF, modèles/holsters, cheval/saut/sons, World Ender et trajectoires, souris/XInput/RT/RB/R3/roues, jauges et skins inchangées. Le dash ne reçoit aucune nouvelle modification de logique. Le wrapper existant de sa notification de particules appelle simplement le nouveau contexte de mêlée après ses filtres.

[Étude des six capacités](V1_82_FOREIGN_WRATH_RESEARCH.md) : première piste Stoneskin, contrat du montage dynamique identifié, pas de transfert déclaré fonctionnel. Le choix reste parmi les trois pouvoirs de chacun. Réception lourde incertaine ; autres FX P2 reportés à la demande de l'utilisateur ; HUD général en surveillance car non reproduit. Aucune remise en visibilité globale ou correction par touche physique.

## Essai groupé et rollback

1. Strife, deux épées visibles puis gauche masquée, droite masquée et les deux masquées : premiers coups, combo complet, coups aériens. Vérifier départ/longueur des arcs et conservation de l'arc de l'épée encore visible. Les coups partagés sont volontairement natifs avec une seule épée masquée.
2. Exécutions TPS au sol : cadrage habituel. Puis une exécution qui fait réellement monter le corps : suivi vertical et retour, sans décalage de tir après la fin. Tester Strife et War quand l'occasion se présente ; P2 ultérieurement si souhaité.
3. Contrôle court de la base validée : bras OFF animé pendant déplacement/tir ; mêlée/pouvoir puis retour ; World Ender, roue, cheval/descente, ultime/retour. Remettre les modèles Native pour comparer.

Livraison cumulative à quatre fichiers racine, installation **manuelle**. Conserver l'INI personnel, raccourcis, PAK et sauvegardes. Rollback : ASI du ZIP V1.81 conservé, même INI. Les 146 archives précédentes et les branches main/archive ainsi que le tag v1.43 sont protégés. Aucun fichier du jeu remplacé ni relance par l'agent.
