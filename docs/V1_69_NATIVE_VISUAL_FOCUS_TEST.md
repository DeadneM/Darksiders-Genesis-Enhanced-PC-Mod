# V1.69 — Léviathan, profondeur de champ TPS et menu

20 octobre 2026. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL** : 43/43 suites Windows x64, zéro avertissement MSVC, ZIP contrôlé. Nouveau rendu en jeu à vérifier. [Reçu de livraison et signatures](CURRENT_STATE.md). Debug uniquement. Base cumulative V1.68 `02a11feb84dfd722c9ddbf8c1fe0fcc5b5d6ebe2`, acquis V1.66/V1.67 conservés.

## Retours retenus

Durée ultime infinie validée ; changement de personnage et netteté au chapitre 8 semblent bons. Les décors lointains sont cependant trop nets en TPS. Léviathan fonctionne mais trop bas. Sélection des skins de chevaux et armures puis retour aux disponibilités natives avec OFF confirmés ; maintien de l'apparence après redémarrage à vérifier. Le premier retour portait sur une installation avec le PAK ; l'absence effective du PAK n'est pas établie par le dernier message. Aucun PAK déplacé par l'agent, aucune autonomie ASI certifiée sans ce contrôle.

## Changements et critères de réussite

1. **Léviathan : hauteur native.** Capture VM_READ du 20 octobre, PID 4460, 293973 objets examinés, zéro erreur. L'émetteur natif `P_Dagon_CreatureCore_Status_Lvl3` appartient à Strife et s'attache à `CollisionCylinder`, sans socket, translation/rotation nulles, échelle 1. Le mesh `CharacterMesh0` est à Z relatif **−97** : l'attache V1.68 sur ce mesh explique le décalage. V1.69 reprend la racine réciproquement possédée, sans hauteur arbitraire. Corps natif `6BC230` : propriétaire+68, racine acteur+158, bone+40, template+50. Les trois defaults Léviathan ont BoneName=None ; le statut live le confirme pour Lvl3. Garde du corps natif ajoutée ; même restauration, identité faible et arrêt après trois échecs. Aucun cœur ni bonus accordé. Réussite : effet forcé à la même hauteur que le natif, aucun doublon, décocher restaure le natif.
2. **TPS : conserver le flou lointain du niveau.** Retrait du forçage CircleDOF=2 et des anciennes gardes inutiles. La case `TPSLensFocus` est conservée, intitulée **TPS character focus**. Même point focal gameplay V1.68. Méthode, FarBlurSize, FarTransition, ouverture, qualité et poids restent natifs. Dans la sortie TPS seule, NearBlurSize=0 protège l'avant du personnage en Gaussian ; une région nette derrière le pivot est projetée depuis les bounds du mesh vivant. À cheval, union monture/cavalier si disponibles. Propriétaire réciproque vérifié ; seules des valeurs sont gardées dans les vues maintenues. Bounds absents/invalides/éloignés : pas d'override de région. Une région caméra explicitement plus large est conservée. OFF garde la correction de distance seule. Aucun volume modifié et aucun nouveau hook. Réussite : personnage entier net, arrière-plan flou là où le niveau en possède, sans changement de géométrie caméra/visée.
3. **Menu :** Appearance et Weapon sheathing sont des sections repliables distinctes de Player movement, sans changement de réglage ou raccourci. Description DOF corrigée et ancien statut HUD « F1 » remplacé par un renvoi aux raccourcis configurables.

Le correctif n'ajoute pas de flou dans un niveau qui n'en possède pas. La protection vise le Gaussian observé ; d'autres méthodes natives peuvent interpréter ces champs différemment. Ni les tests mémoire ni un modèle mathématique ne prouvent le rendu GPU. Nouveau résultat visuel et coût **NON VALIDÉS**.

## Preuves et contrôles

Consommateurs natifs sur l'EXE exact : `1B2F79D` lit +17 mask04 pour FocalRegion +460 ; `1B2F87E` teste le signe de +17 (mask80) pour NearBlurSize +474. **+18 mask01 est FarBlurSize : ne pas le confondre avec NearBlurSize.** Signatures et offsets réfléchis contrôlés au démarrage. Foyer +454 / bit +16 mask08 inchangé. Bounds mesh+148, propriétaire+E8, mesh acteur+380 déjà relevés dans le jeu.

Tests du helper réel : paramètres lointains/méthode préservés octet par octet, piège Near/Far, projection, bounds manquants/étrangers, région explicite, poids partiels, P1/P2, union monture/cavalier et OFF identique. Test Léviathan : attache racine à zéro, rejet d'une racine étrangère, restauration et arrêts conservés. **146 clés et 54 actions**, aucune clé ajoutée, profil fourni inchangé. Aucun test automatisé ne mesure l'image du jeu.

Référence générale : [DOF Gaussian, Epic UE4](https://dev.epicgames.com/documentation/en-us/unreal-engine/mobile-depth-of-field-method?application_version=4.27). Offsets et modes viennent du binaire du jeu, sans lui attribuer la version de cette documentation. Captures et assets privés ne sont pas redistribués.

## HUD et file ouverte

Le Blueprint cuit de sélection d'arène appelle StartMission puis joue son outro sans contrôle de résultat dans ce graphe. Le wrapper natif `789EA0` appelle `585010` sans écrire de ReturnValue : ne pas inventer un booléen de réussite à intercepter. OnUiDisplayModeChanged du HUD général n'y fait pas de réaffichage global. Cela ne démontre pas la cause de la panne. La capture V1.67 reste valable : hide natif/mod=0, racine visible/opacité 1, bShowHUD vrai, IsInViewport vrai. Prochain discriminant : état du gestionnaire UI/pile de pages et couche de rendu après l'échec de sélection. Une nouvelle lecture typée est préparée ; elle n'a pas capturé de panne, le jeu étant déjà fermé. **Aucun correctif HUD revendiqué.**

OFF complet des pistolets (modèle/effets/bras, projectiles conservés), nouvelle approche Recovery et confirmation Auto check restent ouverts. Autres icônes/effets P2 différés sur demande. Aucun changement de visée, projectile, input, dash, ressource ou mouvement dans ce lot.

## Essais groupés et rollback

1. Léviathan natif puis Override 1/2/3 et OFF : hauteur, doublons, retour natif. Refaire monture/ultime/P2 si possible.
2. Chapitre 8 et autre niveau : personnage entier net, flou lointain là où le niveau en possède. Plusieurs angles, changement Strife/War, pied/cheval/ultime, puis coop. Vérifier aussi vue native temporaire et cinématique.
3. Menu : Appearance / Weapon sheathing, réglages et raccourcis conservés, aucun avertissement ImGui.
4. Skins : cheval/armure sélectionnables, OFF rend le natif ; vérifier l'apparence après redémarrage. Essai sans PAK séparé, après fermeture et sauvegarde réversible de ce seul PAK.

Rollback : [V1.68 conservée](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b1a7110048071987facf6f28af0475b87c1a272b/downloads/DarksidersGenesis_V1.68_VISUALS_ULTIMATE_TEST.zip), en conservant l'INI personnel. Focus OFF garde la distance focale seule ; il ne reproduit pas CircleDOF de V1.68 ON. Main/archive, v1.43, anciennes archives, PAK, sauvegardes et raccourcis préservés.

## Preuve complémentaire sans PAK

Le journal moteur de la session V1.68 12:47–12:57 ne monte que le PAK original ; le PAK cosmétique est absent. Le retour utilisateur valide sélection des chevaux/armures et retour natif OFF dans cet essai sans PAK. Maintien après redémarrage à vérifier. Aucun PAK modifié par l’agent. Cette preuve lève la réserve du début du rapport.

## Précision après livraison : disparition du HUD général

Le clic sur une arène encore verrouillée affiche un refus de déblocage, rend le menu transparent et fait disparaître **le HUD général**. « HUD d’arène » était une abréviation imprécise. Rechercher la restitution de l’interface après ce refus ; les deux pistes RenderOpacity de l’outro de sélection sont une piste statique cohérente, pas une cause prouvée. Aucun nouveau correctif livré par cette mise à jour documentaire. Voir le relevé précis en tête de CURRENT_STATE.
