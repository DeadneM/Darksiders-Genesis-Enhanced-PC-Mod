## V1.57 — construite, contrôlée et installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur nécessaire.** Base cumulative V1.56 ; déplacement + visée + dash validé par l’utilisateur et conservé. Deux corrections ciblées : bit natif d’activation de la distance de mise au point TPS corrigé (`08`, l’ancien `10` activait l’ouverture) ; filtre du son, des flashs et des douilles Standard du Dash Fire avec munitions spéciales. **Le mouvement corporel du montage reste natif**, pour préserver la charge et la reprise des capacités.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39133116240) **SUCCESS : 30/30 suites, zéro avertissement MSVC ASI/DXGI**. Les avis CMake/Node concernent les outils et ne sont pas masqués. Source construite `773480ba71feab1fcf439ff3473380f6b157df02`, archive immuable `78acf8005d429994c54b6d681d6444eb976b5b70`. Les fixtures exercent les vrais filtres avec des objets natifs simulés et reproduisent le bit lu par le mélange du rendu ; elles ne valident pas la présentation dans le jeu.

[ZIP cumulatif V1.57 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/78acf8005d429994c54b6d681d6444eb976b5b70/downloads/DarksidersGenesis_V1.57_FOCUS_DASH_PRESENTATION_TEST.zip) : **888,752 octets**, SHA256 `FB7A9D57F609C60FCE66E2C4F6CD33AB2E770BB98BC2909D5D5CAEE9E7B336F6`. Quatre fichiers racine, CRC, AMD64, métadonnées de version, 119 clés/48 actions, téléchargement public HTTP 200 et **121 anciens ZIP inchangés** contrôlés. Aucun changement d’INI/raccourci ; corps de reprise AIM, Tick/fin/physique Recovery, calcul des tirs, XInput/RawInput et fonctions hors périmètre conservés.

Installation jeu réellement fermé après confirmation utilisateur et contrôle de 274 processus visibles. Sauvegarde V1.56 `install-backup-before-v157-20261018-155347`, avec manifeste et `Restore-Previous.ps1` (syntaxe contrôlée, non exécuté). **INI personnel identique avant/après** : `8105BB2676132C3D51A1F53EF0BE8F972A56EA05DF727AB3831D64B516EC0A6A` ; raccourcis conservés. ASI/DXGI installés identiques au paquet vérifié. Aucun lancement du jeu par l’agent ; aucun résultat visuel V1.57 encore validé. Le README du ZIP garde sa préparation historique ; l’archive est immuable.

**À tester :** (1) chapitre 8 immobile, angles/distances TPS : personnage net avec profondeur de champ conservée ; (2) déplacement + visée + tir spécial + dash, quatre directions et Hotstreak : absence du son/flash/douilles Standard, reprise du tir spécial intacte ; (3) contrôle court de la visée après dash validée, Standard/Charge/Lava/faisceaux et roues P2 ; (4) chaque vue coop, retour natif/cinématique. Si visée ou charge régressent, revenir à la sauvegarde V1.56. [Audit et limites](V1_57_FOCUS_DASH_PRESENTATION_TEST.md). Les statuts de préparation ci-dessous sont historiques.

---

# V1.57 — focus et présentation du dash, audit du 18 octobre 2026

## Base et statut

Base debug `d71eeaa63151a70401f075a6af0fddbadb08642e`, ASI V1.56. L’utilisateur valide déplacement + visée + dash ; pas une validation de toutes les combinaisons coop/Recovery. Il rejette la netteté du chapitre 8 et confirme le son/animation du tir Standard sans projectile pendant le dash spécial. Ne pas rouvrir la reprise AIM validée. Build Windows x64 prévu, source de test non encore construite/installée.

## Mise au point : erreur identifiée

Exécutable supporté SHA256 `9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54`. Lecture externe QUERY_LIMITED_INFORMATION|VM_READ, PID 31480, chapitre 8 prêt et immobile confirmé par l’utilisateur ; aucun appel natif ni écriture mémoire. La vue et les deux caches privés contiennent FocalDistance `364.711151`, profondeur du personnage calculée `364.711156`, poids 1 ; override byte PP+16 `10`. Les caméras natives ont leurs réglages originaux ; le cache PP supplémentaire est vide.

Preuve décisive dans le consommateur natif `1B2F68F–1B2F6DC` : `test al,8` décide le mélange de PP+454 (FocalDistance) ; `test al,10` décide celui de PP+300 (Fstop). Le setter correspondant au focus est `1DD2DB0`, pas `1DD2DD0`. L’interprétation de la table de réflexion V1.56 avait associé le setter adjacent au mauvais nom. La distance était correcte, mais ignorée ; l’ouverture pouvait être imposée sans le vouloir. Le test V1.56 copiait cette hypothèse et ne pouvait pas découvrir le défaut.

Correction : seul le bit ajouté devient 08 ; distance/calcul, chaque sortie P1/P2, gardes et champs préexistants inchangés. Initialisation vérifie le setter ET les 16 octets de la branche native de mélange, plus les gardes existantes de copie/métadonnée. Le test montre que 10 seul laisse un focus natif 1000, puis que 08 permet de lire 365 ; conserve un 10 déjà authentiquement présent et n’en crée pas sur une vue fraîche. Ni suppression globale de DOF, ni champ de volume, shader ou réglage persistant touché. Le résultat visuel reste à vérifier.

## Tir Standard du Dash Fire : présentation isolée

Lecture privée des quatre séquences `Strife_Aim_Dash_Fire_F/B/L/R` du PAK, intégrité SHA1 contrôlée lors de leur extraction précédente. Chacune référence trois sons `char_strife_gunfire_Cue`, des `VFX_strife_muzzleFlash` et `VFX_strife_bulletCasings`, et deux notifications SpawnProjectile avec override Standard. Les sons `char_strife_dash` et `char_strife_vo_effort_dash` sont séparés. La réflexion du jeu ouvert confirme Sound et PSTemplate à +38, classes exactes AnimNotify_PlaySound et AnimNotify_PlayParticleEffect.

Entrées natives : son `16977D0`, vtable `2C939B0+240` ; effet `16977C0`, vtable `2C935D8+240`, thunk de 10 octets vers la méthode virtuelle +260. Signatures et pointeurs vtable contrôlés avant installation ; si un des deux hooks échoue, les deux sont retirés et le comportement de présentation reste natif. La reprise AIM et le filtre de projectiles existants restent disponibles indépendamment.

Le prédicat déjà utilisé pour le projectile est partagé, sans deuxième politique concurrente : thread du jeu, propriétaire local réciproque, Strife normal vivant à pied, hors pause/cinématique, capacité de dash reconnue active, séquence F/B/L/R exacte et notification appartenant à cette séquence, actions natives 18/20 tenues et réserves correspondantes toutes spéciales reconnues. Ensuite seuls les trois noms de ressources ci-dessus et les deux classes exactes passent dans le filtre de présentation. Aucun silence global audio/particules ; ennemis, Standard, mixte Standard/spécial, inconnu, cheval/Ultime et autres sons/effets conservent l’original. Journaux bornés à huit événements par type.

Le montage et les transitions de capacités restent natifs. Les mouvements de bras/recul éventuellement inscrits dans l’animation ne sont pas retirés. Cela évite de substituer Dash Aim à Dash Fire et de changer la suspension/reprise de Charge Shot. **Ne pas annoncer que toute animation corporelle est réparée.** Si une présentation Standard subsiste, relever sa ressource exacte avant d’élargir le filtre.

## Contrôles et critères d’arrêt

Tests source : vrais hooks son/effets/projection avec moteur simulé ; P1/P2, quatre directions, TPS/Recovery OFF/ON, actions primaire/secondaire ; conservation des sons/effets non ciblés, des objets acteur/capacité, de l’état natif en cas de propriétaire/activité inconnus. Tests AIM et fin physique précédents conservés. Les appels natifs sont remplacés dans les fixtures : compilation et tests ne prouvent ni installation des hooks dans le jeu ni résultat visuel.

Critère de réussite : personnage net chapitre 8 tout en conservant le DOF ; pas de son/flash/douilles Standard pendant le dash avec munition spéciale ; visée après dash et reprise/charge spéciales intactes. Contrôler aussi Standard et chaque vue coop, passage en cinématique/native, roues. Si charge/visée/son de dash normal régressent ou si crash, arrêt du candidat et retour V1.56 sauvegardée. Pas de nouvelle tentative risquée en cascade.

Preuves privées non distribuées : `v157-live-camera-focus.json`, `v157-live-notify-layout.json`, `v157-dash-presentation-properties.json`, dumps natifs ciblés et logs utilisateur copiés sous audit-v131. Archives anciennes/branches protégées, INI et Hotkeys à vérifier avant livraison. Aucun asset propriétaire incorporé au dépôt ou à l’ASI.
