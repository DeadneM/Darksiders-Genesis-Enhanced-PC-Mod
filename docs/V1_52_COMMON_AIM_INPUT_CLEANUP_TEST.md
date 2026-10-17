# V1.52 — base de visée commune, entrées natives et nettoyage

## Objectif et base

Base debug V1.51 `eca1273`, avec les fonctions validées de V1.50 conservées. **Retours V1.51 rejetés** : désactiver 3D réintroduit le tir chargé Hotstreak incorrect ; activer 3D change l'angle caméra ; certains tirs paraissent décalés à gauche. L'utilisateur demande un socle unique, seule la visée verticale devant différer. Les roues, l'équipement, le changement de personnage et les fonctions gameplay validées restent protégés.

Objectif de cette build : même caméra ON/OFF, correction de direction commune au relâchement, angle vertical indépendant et entrées natives préservées hors TPS effectif. Nettoyer les huit avertissements sans les masquer. Critères : Charge Shot/Graviton en reculant et en Hotstreak utilisables dans les deux modes ; aucune variation caméra au changement de case/angle ; visée au stick disponible après dash en vue native. **EXPÉRIMENTAL : le dernier critère n'est pas déclaré corrigé sans essai en jeu.**

## Source native : correction placée trop tôt

L'ancien hook `59D280` corrigeait la rotation par défaut de la capacité. **`59C930` appelle ensuite le virtuel `+2C0`, puis peut remplacer son yaw/roll par ceux de l'Instigator** (`59C962` à `59CA43`). Cette fonction est appelée par `6629C0`, `662C42` et `62EE30`. La substitution tardive est prouvée ; son rôle dans chaque tir constaté reste à valider en jeu.

V1.52 retire l'interception `59D280` et utilise le retour final `59C930`, après l'original, uniquement pour une capacité projectile Strife possédant son arme de pistolets locale vérifiée. Le nombre d'interceptions projectile reste **trois** : pistolet, rotation finale de capacité et préparation balistique Lava. Aucun ajout d'un second hook par-dessus l'ancien.

La réflexion de `GetProjectileSpawnRotation` désigne son argument FVector comme **`SpawnLocation`** (`3640478`), transmis à R8 par `7CF9B0`. Pour les tirs ordinaires, cette position fournie par le jeu remplace l'ancienne référence approximative `66A3B0`. Cette dernière fonction n'est plus appelée ni requise par le module. Cela supprime une source de parallaxe injustifiée, **sans correction horizontale fixe**. Pour les capacités, l'origine utilisée reste `Instigator.RootComponent`, également employée par le calcul Lava natif ; tous les sockets/offsets de spawn de chaque variante ne sont pas démontrés. Un décalage résiduel près de la cible reste à signaler avec la munition et le mode.

## Une base commune, une seule différence verticale

Les deux modes partagent propriétaire, action native, acquisition de cible/collision, correction finale du yaw et fonctionnement au relâchement. Aucun bouton physique encore maintenu n'est exigé pour une fin de charge. La sélection des roues, la pause et les cinématiques rendent le contrôle natif.

- **OFF** : rayon horizontal Camera Forward, correction finale de direction conservée ; pitch et physique verticale natifs conservés. Ce mode ne désactive plus tout le module et ne réactive plus volontairement l'ancien bug de tir chargé.
- **ON** : même chaîne, avec direction verticale du rayon = inclinaison caméra + réglage **« Shot vertical angle »**, de −45° à +45°, saisie manuelle incluse. Défaut **+12°**, qui compense le cadrage personnel par défaut −12°. La caméra elle-même ne bouge pas. Le réglage influe sur la cible et la direction réelle du projectile ; ce n'est pas un changement seulement cosmétique du tracer.
- **Lava en cloche** : sa vitesse et son angle sont couplés. ON garde le solveur natif borné de V1.51 ; la correction finale commune ne réécrit pas indépendamment ce pitch balistique. OFF conserve la paire native tout en corrigeant la direction finale. Graviton ne reçoit aucune gravité inventée.

Le helper de caméra V1.51 conditionné par la case 3D est supprimé. P1/P2 calculent leur pose comme avant ce changement, et le même helper de direction de rayon est utilisé chez les deux joueurs. Une seule nouvelle clé `Values/TPSAimPitchOffsetDegrees`, valeur par défaut `12.000` : **119 clés, 48 IDs Hotkeys inchangés**. Les 118 valeurs antérieures, INI personnel et bindings restent conservés.

## Dash, TPS et sticks

Deux défauts de portée sont corrigés :

1. Le getter de vecteur de regard pouvait encore neutraliser le stick sans exiger une vue TPS réellement fraîche, contrairement au getter de rotation et au filtre XInput. Le contrôle commun de propriétaire P1 exige maintenant cette vue récente : hors TPS effectif, ces retours restent natifs.
2. Le polling matériel de caméra appelait un trampoline XInput mais ne protégeait pas les appels éventuellement relayés vers un autre export hooké. Des scopes par thread garantissent maintenant **zéro transformation pendant une lecture matérielle** et **une seule transformation pendant une lecture du jeu**, même si les fournisseurs s'appellent. Ils ne laissent aucun état permanent à la sortie. Les règles de roues, équipement, échange des sticks et boutons de tir restent celles validées, sans touche de dash imposée.

Ces défauts sont visibles dans le code et les cas d'appels imbriqués sont reproduits par un test. **Leur présence dans le scénario utilisateur et leur responsabilité dans le dash ne sont pas prouvées.** Le journal V1.51 n'a que deux dashs échantillonnés, 28 observations, toutes avec TPS et Recovery ON ; aucune comparaison exploitable en vue native/OFF. Le mouvement natif y repasse de 6/5 à 1/0 après les demandes de fin. Les directions physiques de manette sont désormais jointes aux relevés bornés existants, avant transformation, si elles sont fraîches sur le même thread. Les lectures caméra sont exclues de ce relevé ; absence de mesure indiquée explicitement.

La physique et la demande de fin Recovery ne sont pas changées. Progression triple dash, réception lourde et perte de visée au stick restent des tickets de gameplay. Aucun nouveau hook de montage, aucun forçage d'un flag AIM/MOVE, aucune correction de shader ajoutée.

## Huit avertissements : réparations de source

| Alerte V1.51 | Changement |
| --- | --- |
| DXGI C6385, copie/lecture du nom de vidéo | Copie caractère par caractère bornée explicitement à 180, terminaison conservée |
| Caméra C4456 + C6246 | Compteur imbriqué renommé, aucun masquage de variable |
| Caméra C6393 | Tampon texte de log de 365 remplacé par 384 ; l'heuristique de calendrier ne s'applique pas à ce texte |
| Skip Logos C6011 | Exports publiés seulement s'ils sont complets, pointeurs atomiques, capture locale vérifiée avant appel |
| Skip Warning C6385 | Bornes de l'indice revérifiées au point d'accès, en plus du helper |
| Updater C6262 ×2 | Deux grands tampons UTF-16 déplacés vers des vecteurs sur le tas, mêmes limites/validation des chemins |

Aucune désactivation d'avertissement, aucune modification des seuils d'analyse. Le nombre réel d'alertes de la nouvelle CI sera consigné après compilation. Zéro avertissement éventuel ne signifiera pas que toutes les dettes de threads/arrêt/INI ou tous les bugs du mod sont résolus.

## Tests courts de cette build

| # | Essai | Attendu / à relever |
| --- | --- | --- |
| 1 | Caméra immobile : case 3D ON/OFF puis modifier Shot vertical angle | **Aucun mouvement ni changement d'angle/FOV de caméra** |
| 2 | Charge Shot et Graviton, normal/Hotstreak, immobile puis en reculant ; OFF puis ON | Correction finale conservée dans les deux modes, même après relâchement ; pas de tir lié au déplacement |
| 3 | ON, angle +12 puis valeurs plus basses/hautes ; tirs ordinaires, cible près/loin | Direction verticale réglable, pas de compensation horizontale arbitraire ; relever toute dérive à gauche par munition |
| 4 | Lava normal en cloche OFF/ON, cible proche/loin puis ciel | Couple arc/portée cohérent ; ON peut corriger en 3D, OFF conserve la verticale native |
| 5 | Vue du dessus, maintenir le stick de visée puis dash ; Recovery OFF puis ON | La visée reste commandable ; si échec, traces physique/native pour rechercher la transition qui la perd |
| 6 | Refaire 1–5 chez P2 ; contrôle court des trois roues et de l'équipement maintenu | Caméras privées, sélection/maintien validés conservés ; interversion native des sticks respectée |

Pour isoler le bug natif, désactiver réellement Third Person dans le menu pour le test 5, puis essayer aussi le raccourci de vue native temporaire. La case 3D OFF ne désactive pas TPS. Ne pas attribuer automatiquement à Recovery un défaut déjà présent avec Recovery OFF.

## Contrôles et livraison

27 suites prévues, dont scénarios réels des modules projectile/caméra, lecture XInput imbriquée, valeurs INI, garde de ressources et antérieurs. Windows x64 via GitHub Actions, package immuable à quatre racines. Anciennes archives et main/archive/v1.43 protégés. Installation uniquement jeu fermé, sauvegarde et contrôle de conservation de l'INI ; aucun lancement automatique.

Rollback : restaurer ASI/DXGI/README V1.51 sauvegardés, conserver l'INI courant. La case 3D OFF est désormais un mode vertical natif du même socle, **pas un rollback de la correction commune**. Les échecs V1.51 restent documentés.
