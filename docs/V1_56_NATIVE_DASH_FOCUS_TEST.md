## V1.56 — construite, contrôlée ; installée (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur nécessaire.** Base cumulative V1.55, avec les tirs précédemment validés conservés. La nouvelle capture AIM tenu confirme le défaut de reprise V1.55. Deux corrections sont livrées : service dans le passage de caméra qui subsiste après le dash ; accès au composant permanent DualPistolsBP, vérifié dans le personnage et son équipement, au lieu du contexte d'arme vide. La physique/timing de Recovery et le filtre de projectiles V1.55 restent identiques.

La profondeur de champ TPS est maintenant conservée : seule la distance de mise au point de chaque sortie P1/P2 suit le personnage. Qualité, ouverture et autres paramètres natifs restent actifs. **Aspect visuel à tester**, notamment chapitre 8, distances/angles et transitions cinématiques. **Son/animation du tir de dash encore ouverts** ; aucune substitution de variante susceptible d'altérer la charge spéciale n'est livrée.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39127789158) **SUCCESS : 30/30 suites, zéro avertissement MSVC ASI/DXGI**. Les avis CMake/Node concernent les outils et ne sont pas masqués. Source construite `e574452a645e82fd9f64198036a72f923f818e83`, archive immuable `5eb9a1c53df13df886c639439b21669759752620`. Le test natif simulé atteint bien une demande sur les pistolets enregistrés sans capacité/getter/contexte actif, avec TPS/Recovery OFF/ON et isolation P2 ; ce contrôle ne constitue pas un essai dans le jeu.

[ZIP cumulatif V1.56 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/5eb9a1c53df13df886c639439b21669759752620/downloads/DarksidersGenesis_V1.56_NATIVE_DASH_FOCUS_TEST.zip) : **885,040 octets**, SHA256 `14F95747114D77D7845693745A6CBE89F7EBE6C8DE25DAD3AE6350D789C73FB2`. Quatre fichiers racine, CRC, AMD64, version/nom d'archive, 119 clés/48 actions, téléchargement public HTTP 200 et **120 anciens ZIP inchangés** contrôlés. Aucun changement d'INI ou de raccourci. Code de balistique/cible, XInput, RawInput et ressources conservé ; aucun nouveau hook, recherche globale d'objets ou filtre audio dans l'ASI.

Installation jeu réellement fermé, 290 processus visibles contrôlés, sauvegarde `install-backup-before-v156-20261018-151919`, avec manifeste et restauration V1.55. **INI personnel identique avant/après** : `C117459A4641B5488551663FD7FC104C35E08064DA7C3967BC6D272FA5E6EA91`. Aucun lancement du jeu par l'agent ; pas de validation de gameplay V1.56.

**Essais prioritaires :** (1) déplacement + visée + dash sans relâchement, native/TPS, Recovery OFF/ON ; (2) Static/Charge/Graviton normal et Hotstreak pendant/après dash, Standard et P2/roues ; (3) chapitre 8 avec effet de profondeur conservé, plusieurs distances/angles, retour temporaire/cinématique et coop ; (4) contrôle court Lava/Charge/faisceaux validés. [Audit, limites et critères d'arrêt](V1_56_NATIVE_DASH_FOCUS_TEST.md). Les statuts de préparation ci-dessous sont historiques.

---

# V1.56 — reprise générale AIM et profondeur de champ TPS

## Base, retours et statut

Base cumulative `debug` V1.55 `0b1d21a0d35740774a5f870ebab0f1a3919e379e`. Tirs Lava/Charge validés depuis V1.52 et faisceaux/obstruction V1.53 conservés. Retour utilisateur V1.55 : visée après déplacement + dash toujours REJETÉE ; retrait des projectiles forcés confirmé mais son/animation restent, donc PARTIEL ; Standard/P2/roues « ça a l'air OK », sans généralisation ; suppression du flou VALIDÉE, remplacement par une adaptation demandé. Nouvelle source EXPÉRIMENTALE ; compilation/paquet/installation à vérifier séparément.

## Preuves de reprise

Le 18 octobre, capture externe `QUERY_LIMITED_INFORMATION | VM_READ`, sans appel de fonction ni écriture mémoire. Fichier privé `v156-live-dash-state-20261018-141058.jsonl` : 2 105 917 octets, SHA256 `CDC34EB154A8130DED0DF7C69A23BE385772D994B59694B8218A99D7CE730E0F`, 5 509 échantillons, zéro échec de lecture. Deux dashs simples précèdent les trois Dash Aim Level1 du protocole (14:11:33, 38, 43, Paris), de durées observées environ 0,70 s. Après chacun : mode natif au sol, AIM 31 = 1, vecteur de norme proche de 1, liste des capacités vide jusqu'à deux secondes observées. Ne pas confondre les cinq activations avec cinq dashs de visée.

V1.55 ne journalise aucune demande AIM_RESTART_REQUEST. Son service dépend du getter 671A40, qui peut ne plus être appelé après disparition de la capacité. La nouvelle observation prouve que le booléen natif AIM reste bien tenu : le problème n'est pas résolu en traitant une touche ou un stick.

Second défaut démontré dans le getter natif 66A7F0 : l'interface ranged à `actor+B08` consulte la pile de contexte à `actor+E08`, puis choisit une arme selon son dernier élément. Si la pile est vide, retour null. La lecture actuelle observe zéro contexte ; ce getter n'est donc pas l'accès permanent aux pistolets de visée. Réflexion du Strife courant : `DualPistolsBP`, ObjectProperty à `1038`. Même objet enregistré dans `actor+C78 -> weapons[5]`, propriétaire `E8` égal au personnage, classe `MayhemWeapon_DualPistols_Blueprint_C`, AimAbilityClass `618` égale à `MayhemAbility_Strife_DualPistols_Aim_Start_Ability_C`. Aucun CC0 ni recherche globale d'objets en runtime.

V1.56 appelle le service depuis le callback de caméra déjà existant, après sa sortie native et la résolution réciproque du joueur ; même passage vue du dessus/TPS et P1/P2, hors Tick de mouvement/capacité. Le getter de visée ne contient plus cette reprise. Le composant permanent est vérifié contre la référence réfléchie ET le slot natif ; toute divergence conserve le natif et arrête la tentative pour ce dash. Le démarrage reste le natif DoAimAbility 69FBD0 avec sa signature et sa classe de démarrage d'origine.

Garde inchangée : Strife normal local à pied, autorité, vivant, dash récent de même identité, au sol, aucune capacité active/suspendue, AIM encore tenu, hors pause/cinématique. Une demande au plus ; récursion protégée. Relâchement AIM : intention périmée, pas de reprise au prochain appui. Contrat inconnu : compteur borné AIM_WEAPON_UNAVAILABLE et arrêt de cette observation. Appel de fonction ou journal de demande ne vaut pas validation de gameplay. Aucun essai sans ASI : ne pas attribuer toute la régression au jeu natif.

## Conservation de la profondeur de champ

Le retrait effectif jeu/rendu V1.54 inclus en V1.55 est confirmé visuellement par l'utilisateur. Le nouveau but est de conserver l'effet avec un personnage net depuis la caméra rapprochée. Selon la [documentation UE4 d'Epic](https://dev.epicgames.com/documentation/en-us/unreal-engine/depth-of-field?application_version=4.27), cet effet dépend de la distance au plan de mise au point ; cette documentation ne sert pas de preuve d'offset dans l'EXE.

Offsets retrouvés dans l'exécutable exact : FMinimalViewInfo de 580, PP à 40 (copie native 16FA753), blend à 38 ; FPostProcessSettings de 530, FocalDistance à 454 (descripteur 37958E0, Offset à +36), setter d'override 1DD2DD0 : octet +16, masque 10. Les entrées voisines du tableau appartiennent à d'autres champs : ne pas lire l'offset précédent comme celui du nom courant. La lecture corrigée des caméras BA0/BA8/POV voit poids 1, focus 1000, ouverture 4, région 0, transitions 300/500, sans overrides actifs. Le foyer du postprocess final peut encore provenir des volumes natifs.

Compensation uniquement de la sortie TPS reconnue : distance = produit scalaire (personnage - caméra émise) · avant de la caméra. Ce n'est pas une distance oblique ni une nouvelle rotation de visée. Le float de focus et son bit d'override seuls sont écrits dans la sortie éphémère ; aucune écriture au composant caméra, à un volume, à l'ouverture, à la qualité ou aux autres paramètres. Poids exactement 1 obligatoire : ne pas relever un poids partiel qui augmenterait aussi l'influence des autres overrides. Données non finies, foyer derrière la caméra ou profondeur hors 1–20000 : conserver le natif. Une vue P2 conserve son propre foyer, sans CVar globale de suppression TPS.

Les vues en pause/roue qui reprennent la composition TPS retiennent aussi sa cible de foyer en valeurs. La vue native, le retour temporaire et la prise de caméra cinématique ne reçoivent pas la compensation. La qualité DOF n'est plus forcée à zéro pour le TPS supporté ; l'utilisateur qui l'a désactivée n'est pas forcé de la réactiver. L'ancien zoom manuel hors TPS et le repli de layout incompatible conservent la suppression V1.54 et sa restauration possédée. Aucune modification persistante Engine.ini, sauvegarde, asset ou shader.

Limites : les réglages des volumes, la méthode DOF, les distances/angles extrêmes et la coop peuvent encore donner un rendu trop flou ; c'est un essai de mise au point, pas une validation visuelle. Poids partiel inconnu : effet natif, correction non appliquée. Priorité à la caméra cinématique déjà détectée, sans nouveau classificateur.

## Son/animation encore ouverts ; piste rejetée avant construction

Le filtre V1.55 de SpawnProjectile est conservé à l'identique, hors noms de journaux. Aucun hook audio/particle/montage général. Les animations Dash Fire incluent une balle Standard explicite et d'autres manifestations ; supprimer le projectile ne supprime pas toute la séquence. Le son et l'animation restent donc une limitation annoncée.

La réflexion des Start Level1/2/3 donne AimDashAbility 1E8, AimFireDashAbility 1F0 et l'appel DoAbilityOptional 597BB0, ce qui permettrait une substitution ciblée. Mais les captures distinguent Dash Aim qui termine la visée et Dash Fire qui suspend/reprend les capacités spéciales. Échanger ces capacités pour supprimer une animation peut changer charge et reprise du tir. Cette piste est ÉCARTÉE avant construction ; son hook, son choix de variantes et sa politique de test ne restent pas dans l'ASI. La suite consiste à isoler la présentation native de Dash Fire tout en conservant son cycle de suspension, avant un nouveau correctif.

## Contrôles, essais et rollback

30 suites prévues. DashRecovery exécute le vrai code : dash fini avec liste de capacités et pile de contexte vides, AIM tenu, aucun getter de look appelé, demande native sur le bon propriétaire pour TPS/Recovery OFF/ON, pas de répétition et relâchement/contrat divergent refusés. Le démarrage natif est simulé : ce n'est pas une injection dans le jeu. TpsCameraFocus vérifie la projection, les angles, tous les octets hors focus inchangés, poids partiels, valeurs invalides, cibles arrière et sorties séparées P1/P2. Les suites et la compilation ne prouvent pas le rendu ni les timings réels.

1. Strife : déplacement + visée tenus, trois dashs sans tir, continuer à orienter sans relâcher ; vue du dessus/TPS et Recovery OFF/ON.
2. Static, Charge puis Graviton : déplacement + visée + tir + dash ; normal/Hotstreak. Pas de Standard ajouté et reprise native conservée ; son/animation restent ouverts.
3. Standard natif, P2 et roues. Contrôle court Lava/Charge/faisceaux/obstacles acceptés.
4. Chapitre 8 immobile puis mobile, distances et angles : personnage net, effet de profondeur conservé et visée inchangée.
5. TPS/vue native/retour temporaire/cinématique ; P1/P2 à des distances différentes ; pause et roues.

Arrêt si crash, répétition de capacité, visée/caméra/charge dégradée, joueur flou ou cinématique modifiée. Revenir au paquet/sauvegarde V1.55 pour isoler ces changements ; ne pas empiler de nouvelles corrections d'axe. Installation seulement après fermeture réelle et sauvegarde, INI personnel conservé. Aucun lancement du jeu par l'agent. Main/archive/v1.43 et 120 anciens ZIP à vérifier inchangés.
