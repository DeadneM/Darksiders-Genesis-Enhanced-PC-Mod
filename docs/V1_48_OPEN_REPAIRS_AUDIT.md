# V1.48 — audit des réparations ouvertes, 16 octobre 2026

## Décision utilisateur et base de travail

Base installée : **V1.48**, source `b7b90d29314ce00229a0737004426cbb0375b28d`, archive `550b7784b4f1341521bc8f7113250e5177207232`. État initial du dépôt audité : debug `2f9cf4fabae677541a9b06747dc70a9c5e91fa48`, audit initial publié dans `c620047`. Le dernier retour utilisateur accepte **le reste pour l'instant**, puis ajoute Graviton aux tirs concernés. Cette acceptation remplace la liste générale de vérifications encore attendues dans le précédent reçu ; elle conserve explicitement les six exceptions ci-dessous. Elle ne constitue pas une mesure instrumentée de chaque combinaison de paramètres.

Les gains/jauges, refills, Unlimited, munitions, nouvelles options Hotstreak et Hotkeys, présentation du menu, comportements caméra et commandes précédemment acceptés constituent désormais la référence à préserver, hors exceptions signalées. Aucun retour vers une ancienne expérience rejetée.

| Réparation ouverte | Retour utilisateur faisant autorité | Conclusion de cet audit |
| --- | --- | --- |
| Roue d'équipement en coop | Le stick ne sélectionne plus pour **les deux joueurs** | Défaut de couverture des protections d'entrée prouvé ; cause de chaque blocage encore à corréler en jeu |
| Charge Shot | **Manette**, aggravation surtout pendant **Hotstreak en reculant** ; P1/P2 non précisé pour ce signalement | Capacité et retour de rotation distincts du retour commun actuellement corrigé ; chemin natif de secours et fin de charge à observer |
| Graviton | Visée également affectée ; tir déjà descendant avec réglages TPS par défaut et caméra recentrée. Périphérique/joueur non précisés dans ce complément | Le neutre caméra conserve le View pitch de -12° ; ce pitch est copié dans le retour commun quand ses gardes passent. Graviton possède aussi des phases natives Aim/Charge/Finish ; spawn effectif non capturé |
| Lava normal | Projectile **en cloche, manette P1**, arrive presque aux pieds avec caméra horizontale | Cible 3D, rotation balistique en cache et vitesse sont liées ; correction actuelle des pistolets insuffisante pour couvrir ce retour natif |
| Visée native après dash | La visée s'annule, arc/trait disparaissent en vue du dessus | Cause non démontrée ; ancien hook de masquage persistant déjà retiré |
| Action Recovery | Dash uniquement, surtout intervalle **troisième → quatrième dash de Strife** | Fonction actuellement indisponible, sans consommateur gameplay ; point sûr de libération non établi. Coffres différés |

**Statut de cette livraison : AUDIT documentaire, aucune nouvelle build, aucun correctif expérimental installé.** Les 22 suites de tests réussies de V1.48 sont un résultat de sa CI antérieure, pas une reproduction de ces six bugs. Les fichiers locaux préfixés `v149-` désignent les outils de travail de cet audit, pas une version V1.49 construite.

## Sources et limites des observations

- Lecture du README, CURRENT_STATE, TECHNICAL_NOTEBOOK, audits V1.28–31 et suites V1.39–48, source actuelle, tests, EXE supporté et assets natifs pertinents. EXE SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.
- Copie du journal V1.48 : **22:51:33–23:09:42**. Il contient 88 lignes `NATIVE_RADIAL`, 16 lignes de correction `PROJECTILE_BASE` (les huit premières P1 et huit premières P2), neuf événements équipement P2, sept refills Hotstreak et un refill ammo. Ces lignes prouvent l'exécution des chemins journalisés, pas le type du projectile ni l'impact, ni la bonne sélection d'un objet. HOLD/RELEASE/STOW équipement n'est pas l'état de sélection de la roue.
- Lectures externes limitées à `QUERY_LIMITED_INFORMATION | VM_READ`, sans écriture, injection, appel natif distant ni simulation d'entrée. Aucun lancement/relaunch du jeu. L'inventaire de 23:11:30 contient deux LocalPlayers mais **un seul contrôleur actif**, celui de P1 ; le contrôleur du second LocalPlayer vaut zéro. Il ne constitue donc pas une reproduction de la panne en coop.
- Trois widgets `HUD_GearRadialSelector_C` trouvés : deux sans propriétaire résolu ; un rattaché au LocalPlayer courant avec réciprocité contrôleur/joueur correcte. Cela prouve que la propriété peut être résolue et que d'anciens widgets non possédés existent ; pas que les trois roues étaient ouvertes.
- Capture passive suivante de 20 secondes : **974 échantillons, zéro reprise de lecture**, War et ses capacités Tremor. **Aucun Charge Shot, Lava ou dash capturé.** Le champ exploratoire `povCandidateEA8` chevauche des données et ne décrit pas une pose caméra utilisable : il est exclu des conclusions. Aucune trajectoire, latence d'appui ou causalité du bug n'est revendiquée à partir de cette capture.
- Index du pak v8 : 86641 entrées, SHA1 de l'index contrôlé ; extractions locales bornées avec blocs Zlib et SHA1 des payloads vérifiés. Les assets restent privés dans le dossier d'audit local ; aucun asset du jeu n'est publié.

## 1. Roue d'équipement : l'état natif n'atteint pas les protections d'entrée

**PROUVÉ dans le source.** [NativeTpsFeature.cpp](../src/NativeTpsFeature.cpp), `ObserveUiEvent`, observe les widgets de la classe native `MayhemHUDRadialWidget` après le ProcessEvent original, sur le thread du jeu. Le stockage `RadialSample` ne contient que l'identité du widget et une date de visibilité, avec une fraîcheur de 250 ms. **Aucun propriétaire P1/P2 ni état ouvert/fermé du sélecteur n'est conservé.** `RadialActive()` est consommé uniquement par la télémétrie de [CameraTraceFeature.cpp](../src/CameraTraceFeature.cpp), pas par les filtres d'entrée.

Les protections actives utilisent encore l'intention issue du binding configuré :

- P1 : `UpdateOrbitInput`, `FilterNativeXInput` et `IsTPSRadialInputActive` dans [DarksidersGenesisMod.cpp](../src/DarksidersGenesisMod.cpp). Le filtre laisse les axes natifs si le bouton de roue configuré est maintenu ; les autres protections utilisent cet état d'entrée daté. En dehors de cette garde, le chemin TPS peut échanger les rôles des sticks, neutraliser le look natif ou injecter Camera Forward.
- P2 : `UpdateInput`, `FilterPad`, `Active` et `AdjustLook` dans [CoopCameraFeature.cpp](../src/CoopCameraFeature.cpp). `s.wheel` et le test local `wheel` viennent également du binding configuré, sans consultation de la roue native réellement affichée.

**INFÉRÉ :** une roue ouverte par une action native différente, un binding changé, ou dont le lifecycle dépasse le maintien de l'ouvreur peut laisser les filtres TPS actifs. En coop, la roue peut ne pas mettre le monde en pause. Ce défaut est compatible avec le blocage **des deux joueurs**. La capture actuelle ne relie pas un sélecteur ouvert aux axes physiques et filtrés : l'explication exacte reste à vérifier.

**Preuves natives utiles pour une réparation par fonction :**

- `MayhemHUDRadialWidget` : getter de classe `793F70`, taille native `2A0`, constructeur `795110`, vtable `27AE340`.
- Le widget courant résout le propriétaire via ses virtuels `+260 → 14B5130` et `+268 → 14B5080`. Le contexte à `widget+218` contient une **référence faible de LocalPlayer**, suivie du monde à `+220` ; ce n'est pas un pointeur faible de PlayerController. `1950E60/19589D0` résolvent ce contexte. La lecture de ce contexte a retrouvé P1 et son contrôleur réciproque.
- Le sélecteur natif `5D64C0` demande ce contrôleur par le virtuel `+260`. `GetCurrentFocusedIndex`, thunk **7983B0**, retourne `widget+298` ; `SetCurrentFocusedIndex`, thunk **799800**, écrit ce champ puis invalide le widget. Le constructeur initialise l'index à `-1`. **Cet index seul ne prouve pas l'ouverture d'une roue.** Ne pas attribuer à 7983B0 les appels de la fonction voisine 798410 dans un dump de taille fixe.
- `IsVisible=14BEA00` inspecte la visibilité Slate ; cela ne fournit ni le propriétaire ni la preuve qu'un sélecteur capte actuellement les commandes. Le journal émet déjà `visible=1` très tôt au démarrage. **Ne pas brancher aveuglément le booléen global RadialActive dans les deux filtres.** Le véritable état ouvert/fermé et ses transitions restent à identifier sur le bon objet HUD.

**Réparation retenue comme direction :** un état de roue natif, attribué au LocalPlayer et publié avec identité/génération/fraîcheur ; la même décision doit protéger filtre XInput, getter de visée et orbite du seul propriétaire. Garder TPS affiché et laisser la sélection native recevoir les axes/boutons ; conserver le maintien/stow équipement accepté après fermeture. Réutiliser l'observation existante si son lifecycle suffit ; aucun nouveau hook nécessaire n'est encore démontré.

**Trou de couverture des tests :** [CoopCameraIsolationTests.cpp](../tests/CoopCameraIsolationTests.cpp) vérifie une roue en maintenant le masque `0x100`. Il ne teste pas un sélecteur natif ouvert sans ce bouton, deux propriétaires indépendants, un widget expiré ou une ouverture en coop sans pause. Son succès ne valide pas le cas signalé.

## 2. Charge Shot : direction native et lifecycle de charge distincts

**PROUVÉ dans le source.** [ProjectileAimFeature.cpp](../src/ProjectileAimFeature.cpp) corrige `6A9F20`, retour commun de rotation des pistolets, uniquement pour le caller `7CFA3B`, les pistolets typés de Strife local et le mode Camera Forward à la manette. `ProvideProjectileRotation` P1 et `ShotRotation` P2 exigent encore l'état physique de tir primaire/secondaire. Les getters `671A40/671B00` ont aussi leurs propres conditions de tir/roue. **Un relâchement ou une phase native de fin de charge ne sont pas un lifecycle d'action suivi par ces conditions.**

L'ancien audit [V1.10](V1_10_STARTUP_TIMELINE_NATIVE_LBY.md) documente déjà le retrait d'une correction spéciale du tir chargé, inefficace et incompatible avec la protection de la roue. Le problème n'est donc pas établi comme une régression introduite par les ajouts Hotstreak V1.48. Ne pas rétablir cette expérience de simulation d'axes et ne pas généraliser un accord de boutons physique en identité d'action.

**PROUVÉ par la classe native et les assets :** les Finish normal et `MayhemAbility_Strife_ChargeShot_Secondary_Juiced_Finish_Blueprint` héritent de `MayhemPlayerProjectileAbility` ; la phase de charge utilise `MayhemDualPistolsChargeAbility`. Le retour natif identifié pour la capacité projectile est le virtuel `+2C0 → 59D280`, **distinct de 6A9F20**. Aucun hook actuel ne porte ce retour. Le passage exact de chaque projectile utilisateur dans les fonctions finales doit encore être capturé ; l'héritage ne constitue pas à lui seul une trace d'exécution d'un tir.

Chaîne native étudiée : `59D280 → 66A2B0 → 672760`. `672760` peut ignorer `671A40` si le virtuel `+720` est vrai ou si `[actor+C28] - [actor+C54] > 0`. Le chemin de secours lit notamment `D2C/D30`, puis utilise l'orientation du composant racine si la direction est nulle. `66A2B0` peut ensuite prendre le pitch depuis une cible/bouche native par `663F80`, tout en conservant le yaw précédent. **Les noms sémantiques des compteurs et de ces champs ne sont pas prouvés : ne pas les renommer arbitrairement “déplacement” ou “verrou de tir”.**

Les propriétés sérialisées repérées montrent `AllowPlayerRotation=true` pendant charge/Finish Juiced, un override de vitesse de marche 450 et un seuil de pitch descendant différent (`-15` Juiced, `-5` Finish normal). Cette lecture n'est pas un dump exhaustif des valeurs effectives héritées. Ces variantes doivent garder leurs paramètres natifs ; aucune valeur de vitesse/pitch n'est modifiée.

**INFÉRÉ :** la direction de secours et la perte de l'éligibilité Camera Forward au relâchement peuvent expliquer l'aggravation en reculant, notamment dans la variante Juiced. **INCONNU :** branche réellement choisie, action active, moment du spawn, dernière cible 3D et direction du projectile signalé. Les logs des pistolets ne nomment pas Charge Shot et ne prouvent pas cette causalité.

**Réparation à préparer :** suivre la capacité de charge/Finish du bon joueur jusqu'au spawn et fournir une direction/cible cohérente avec sa caméra 3D, strictement dans le chemin manette Camera Forward demandé. Traiter l'instant du tir comme une action native, pas seulement comme un bouton encore enfoncé. Conserver souris, visée native, roues, spread/physique et réglages propres à chaque projectile. Pas de rotation forcée du personnage, ni de hook global sur toutes les capacités pour masquer le symptôme.

## 3. Complément Graviton / View pitch : le neutre conserve -12°

Nouveau retour utilisateur : **Graviton est aussi affecté** ; avec le cadrage TPS par défaut et la caméra recentrée, certains tirs permettant de monter/descendre partent déjà légèrement vers le bas. L'utilisateur propose View pitch comme piste. Le périphérique, P1/P2 et la variante normale/Hotstreak ne sont pas reprécisés pour Graviton ; ils ne sont pas inventés à partir des autres signalements.

**PROUVÉ dans le source et la configuration livrée :**

- `ThirdPersonPitchDegrees=-12` est le défaut dans ConfigStore, RuntimeSettings, INI et bouton Default du menu. P1 et P2 calculent `pitch = clamp(basePitch + orbitPitch, -75, 65)`. Le recentrage remet l'orbite verticale à zéro ; **le pitch rendu reste donc -12°, pas 0°**.
- `ProvideProjectileRotation` P1 retourne `view.appliedPitch`, P2 retourne `heldView[3]`. `ProjectileAimFeature::Rotation` copie cette valeur dans le Rotator du départ commun `6A9F20` quand les gardes joueur/pistolets/manette/Camera Forward/caller passent. Aucune origine de bouche ni cible de collision 3D n'entre dans ce fournisseur de rotation.
- Une direction de pitch -12° possède une composante verticale d'environ -0,208 et descend d'environ 21,3 unités pour 100 unités horizontales **avant toute gravité**, si ce retour est effectivement utilisé tel quel. Cela prouve le sens de l'inclinaison transmise, pas la trajectoire d'un tir Graviton mesuré.
- Le test existant `ProjectileSpawnTests.cpp` vérifie explicitement que le pitch -12 du fournisseur est recopié. Il vérifie le comportement du retour et son périmètre ; **il ne vérifie pas la convergence depuis la bouche vers le réticule, ni le spawn natif de Graviton**. Aucune réexécution des tests C++ n'est revendiquée dans ce complément documentaire.

**Couverture Graviton identifiée :** le relevé historique V1.46 `v146-live-weapon-objects-20261016-174035.json` expose `MayhemGravityShot_Blueprint_C`, vtable `27A5EC8`, slot `+8E8 → 6A9F20`. L'analyse statique des callbacks de classe donne `MayhemGravityShot (78FED0) → MayhemChargeShot (773730) → MayhemDualPistols (77F4E0) → MayhemRangedWeaponComponent (7CE080)`. Le test de parenté pistolets peut donc inclure ce composant ; **cela ne prouve pas que le Finish signalé passe par le caller 7CFA3B avec le bouton encore maintenu**.

Lecture bornée des assets Graviton, index pak et payloads contrôlés : le composant référence une capacité Aim, une phase Charge et des Finish normaux/Juiced. Les références du Finish de base comprennent `MayhemPlayerProjectileAbility`, déjà étudiée pour Charge Shot ; les Finish ont donc une préparation native à examiner en plus du retour du composant. Propriétés sérialisées repérées : rotation autorisée pendant Aim/Charge/Finish ; vitesse de déplacement de charge 450 ; vitesse initiale du projectile de base 3000, Level1 2500, Level3 3500, Juiced 1500. Ce sont des overrides d'assets, pas une mesure des valeurs finales héritées en jeu. **Aucune valeur de gravité n'est déduite du seul nom “GravityShot”**, ni de l'absence d'un override sérialisé.

**Interprétation :** la piste du View pitch repose maintenant sur un mécanisme réel du mod, compatible avec la pente descendante signalée. Un cadrage incliné vers le bas implique aussi que le centre de l'image regarde vers le bas ; ce fait ne suffit pas à déclarer le défaut -12 erroné. Copier l'orientation caméra produit une direction parallèle depuis la bouche ; pour converger au point visé, il faut connaître cette bouche et le point 3D réellement choisi. Le décalage entre ces deux origines et la préparation propre à chaque capacité peuvent s'ajouter à l'effet du pitch. Leurs contributions exactes restent **INCONNUES en jeu**.

**Direction de réparation commune :** construire le point 3D à partir de la bonne caméra/réticule, puis calculer la direction de départ depuis la bouche réelle, avec le solveur natif quand le tir est balistique. Préserver la possibilité de viser haut/bas, les phases de charge/relâchement, les variantes Hotstreak et les réglages personnels. Les trois tirs concernés partagent cet objectif mais pas nécessairement le même dernier calcul natif. Aucune soustraction systématique de 12°, aucun pitch forcé horizontal, aucun changement du défaut ou de l'INI personnel n'est introduit.

Test discriminant à préparer : même munition/variante et même position, comparer des pitches **effectifs** -12/0/positif, puis recul ; relever ensemble pitch rendu, point cible, bouche, action Finish et direction initiale du projectile. Séparer la pente initiale de la courbure due à la gravité. Ce complément utilise source, fichiers natifs et inventaire historique ; **aucune nouvelle capture Graviton ni réussite en jeu**.

## 4. Lava normal : préserver le calcul balistique complet

Les preuves [V1.48 précédentes](V1_48_HOTSTREAK_REFILLS.md) restent valables. La capacité native `MayhemPlayerLavaShotProjectileAbility`, constructeur `7BB280`, fournit son retour final par **7BBF20**, rotation en cache à `ability+20C/214`. Son Tick `5B7580` appelle `5B7DC0`, qui utilise `66A2B0`, la cible/les positions natives, vitesse et gravité, puis `6647A0` pour le pitch et `664800` pour la vitesse (cache `+218`). Ce retour final n'est pas celui corrigé dans le module des pistolets.

**PROUVÉ :** cible 3D, pitch et vitesse participent ensemble à l'arc. **INFÉRÉ :** un point de visée trop proche, un départ incohérent ou un cache pris avant/après le bon état d'action peut expliquer le tir aux pieds avec caméra horizontale. **INCONNU :** leurs valeurs pendant le tir signalé. Aucun tir Lava n'a été enregistré dans la capture passive de cet audit.

La réparation doit fournir le bon point 3D et vérifier le résultat balistique avant le spawn. Un simple remplacement du yaw ou un ajout arbitraire de pitch ne constitue pas une solution. La variante Lava Hotstreak en faisceau est distincte ; **elle n'est pas le Charge Shot décrit dans le nouveau retour** et n'est pas déclarée défectueuse sans observation correspondante.

## 5. Visée native après dash : distinguer annulation et affichage

L'ancien detour natif de masquage persistant `6DDAD0` a été retiré en V1.45 ; son ancien champ d'effet `+79C` ne doit pas être réintroduit. [ShaderAimFeature.cpp](../src/ShaderAimFeature.cpp) masque seulement les appels de dessin des deux shaders identifiés (trait `484C3B1B`, arc `88CA97FB`). Il ne modifie pas l'état d'action/visée du personnage.

La configuration personnelle copiée utilise le mode 2 (masquer en TPS). Dans le prédicat actuel, TPS réellement OFF ou vue temporaire native effectivement activée rendent ce masquage inactif. Cela ne prouve pas que le bon état est publié au moment du bug : un état TPS encore vrai alors que la caméra paraît native doit être distingué d'une annulation native de la visée. Le test P2 actuel “top-down” vérifie un pad rendu inchangé quand TPS est OFF ; il ne déroule pas le lifecycle natif visée → dash → retour et ne prouve pas la persistance de l'arc.

**Cause INCONNUE.** Capture utile : état TPS/vue temporaire effectif, intention de visée, action avant/après dash et décision du filtre de dessin au même instant. Un nouvel appui synthétique ou un StartAiming forcé risquerait de masquer le problème et de rompre les bindings. Aucune correction de touche spécifique retenue.

## 6. Action Recovery : du code de compatibilité, pas une fonctionnalité active

**PROUVÉ dans le source actuel :** le menu affiche `Action Recovery - unavailable` ; la Hotkey écrit `retired: no effect` et retourne. `ActionRecovery`, `ActionRecoveryDelayMs`, `ActionRecoveryMultiplier` et `DodgeEarlyUnlockMs` sont toujours chargés/sauvegardés. Les états publiés correspondants n'ont **aucun consommateur gameplay**. Changer ces valeurs ne peut pas améliorer le dash actuel. Ces clés/IDs sont des restes de compatibilité intentionnels ; ne pas les supprimer au prix d'une renumérotation des raccourcis.

Les captures V1.44–45 ont identifié `Strife_Dash_Level3_Montage` (~0,83 s contre ~0,40 s pour les premières variantes), avec une fin de déplacement faible après ~0,5 s. Elles n'ont pas enregistré l'appui physique du quatrième dash : elles ne mesurent donc pas le délai input → action ni un point sûr de déblocage. `MoveInterruptDelaySec` était déjà zéro ; le retoucher n'est pas une réparation étayée.

Correction de lecture native à conserver : **5B93F0 est une mise à jour du déplacement du dash**, qui décrémente le timer `224` et applique une vitesse liée au rapport `224/228` ; ce n'est pas EndAbility. Le Tick `5B72B0` peut appeler la fin native `59B080`, et poursuit ensuite son traitement virtuel. Un arrêt anticipé de ce code pourrait toucher physique/invulnérabilité et rappeler le glissement déjà rejeté.

Prochaine mesure discriminante : appuis physiques troisième/quatrième dash synchronisés avec action/montage/buffer natifs, mouvement réel, timers `224/228` et état d'invulnérabilité. Il faut identifier ce qui refuse réellement la quatrième action, puis agir sur cette transition native avec nettoyage complet. Répéter seulement une capture de positions ne résoudrait pas cette inconnue. **Dash seulement ; coffre reporté conformément à la décision utilisateur.**

## Nettoyage ciblé et simplification

1. Unifier la décision “roue native du joueur ouverte” dans les trois consommateurs P1/P2 au lieu d'ajouter une exception par touche. Conserver des états privés par joueur ; un booléen global peut bloquer l'autre vue ou traiter un widget périmé.
2. Séparer intention physique, action native en cours et autorité du projectile. Les conditions communes doivent être partagées, sans perdre la séparation souris/manette ni convertir toutes les capacités en tirs de pistolet.
3. `g_tpsChargeChordHeld` et `g_tpsChargeChordTick` sont encore écrits sans être lus ; leurs compteurs servent à la télémétrie. En revanche **TPSChargeChordDown a encore un usage comportemental dans la garde de forme ultime** : ne pas supprimer l'ensemble comme code mort. L'ancien `TPSChargedFirePadMask` est conservé/ignoré volontairement depuis V1.10.
4. Plusieurs commentaires historiques de `HookPlayerLookInput` disent “diagnostic / never alters any result” alors que le code voisin modifie le résultat. Corriger ces commentaires avec la prochaine modification de cette zone ; aucun changement fonctionnel ne peut être justifié par ces commentaires seuls.
5. Les clés Action Recovery obsolètes et les marqueurs de version historiques des logs ne prouvent pas l'exécution d'anciennes fonctionnalités. Leur nettoyage doit préserver les IDs/INI et garder une identité de build actuelle lisible. Aucun gain de performances chiffré n'est mesuré ou revendiqué ici.

## TODO actuel et objectifs des prochains essais

| Ordre | Objectif isolé | Critère de réussite |
| --- | --- | --- |
| 1 | Protéger les entrées de la vraie roue par propriétaire | P1 et P2 sélectionnent chacun l'équipement, roue sans pause et bindings changés/sticks intervertis ; l'autre caméra reste indépendante, TPS conservé ; maintien/stow fonctionne après fermeture |
| 2 | Charge Shot Camera Forward | Immobile puis recul/latéral, hors puis pendant Hotstreak : direction cohérente dès le projectile issu de la fin de charge, maintien et relâchement ; pas de régression des tirs ordinaires ou des roues |
| 3 | Graviton et cohérence du pitch 3D | Au recentrage puis en visant haut/bas, normale/Juiced : bouche et point du réticule cohérents ; comparer pitches effectifs -12/0/positif et recul, sans neutraliser la visée verticale |
| 4 | Lava normal en cloche | Caméra basse/horizontale/haute : cible 3D cohérente, origine/vitesse/pitch natifs compatibles ; comparer native/TPS et P1/P2, conserver la variante faisceau |
| 5 | Visée native → dash → visée | Intention conservée et trait/arc réapparaissent quand le mode d'affichage le demande, sans appui ajouté et avec bindings reconfigurés |
| 6 | Action Recovery dash fonctionnel | Réduction mesurée du délai troisième → quatrième dash après le déplacement utile, sans glissement, perte de distance, état d'action bloqué ou invulnérabilité indue ; OFF restitue le natif |

Regrouper uniquement les changements d'une même responsabilité. Les trajectoires et la libération du dash demandent des essais distincts de la réparation des roues. Chaque build ultérieure doit avoir un nouveau numéro et un rollback vers la **V1.48 acceptée hors ces exceptions**, avec conservation fraîche de l'INI et des raccourcis personnels. Pas de nouvelle vérification globale imposée à l'utilisateur pour les fonctions qu'il vient d'accepter.

## Conservation et traçabilité

Cet audit modifie seulement README et documentation cumulative. Source, tests, configuration livrée, ASI/dxgi installés et fichiers personnels restent hors mutation. Aucun workflow de compilation n'est relancé : le workflow debug désigne encore le ZIP immuable V1.48 et ne doit pas le reconstruire sous le même nom à l'occasion d'un audit. Commit documentaire avec `[skip ci]`, push uniquement sur debug.

112 ZIP sont présents sous downloads (**110 archives du mod et deux de l'updater**), arbre Git `881d40b5c8daf8fd540470e74b18054096a07090` à la base de l'audit. Références distantes relues : main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, tag v1.43 `902861eb1ff34c14dc5ded092581b360767da817`. La [CI V1.48](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38964259848) a été relue : completed/success, source `b7b90d2`. La release publique v1.43 est toujours l'ID 414771221, publiée le 16 octobre à 09:29:27Z, avec son ZIP et son fichier SHA256. Aucune opération de release dans cet audit.

Pièces locales non publiées sous `audit-v131` : `v149-audit-baseline.json`, journal copié SHA256 `25669c319d44ed8ca1d180f6c0357f85242a4b6d972c6e228f2262cd27a2a672`, inventaire `v149-live-views-20261016-231130.json`, propriétaires `v149-ui-owners.json`, capture `v149-actions-readonly.json`, propriétés `v149-asset-properties.json` et désassemblages bornés `v139-full-*`. Le manifeste conserve les empreintes de ces pièces ; les fichiers `v149-*` ne sont pas une nouvelle build.

Complément local Graviton : `v149-graviton-pak-index.json`, `v149-graviton-properties.json`, `v149-graviton-native-lineage.json` et `v149-graviton-pitch-evidence.json`. Assets extraits et désassemblages restent locaux. Audit source repris depuis `c620047`, sans mutation des fonctions, configurations ou archives.
