## V1.54 — construite et contrôlée, installation en attente du jeu fermé (18 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — PAS ENCORE INSTALLÉ NI VALIDÉ EN JEU.** Base V1.53 conservée pour les tirs déjà validés. Le correctif de profondeur de champ atteint désormais les copies jeu **et rendu** ; leur restauration conserve les deux valeurs d'origine et donne priorité à la suspension cinématique existante. La lecture du jeu ouvert en V1.53 a confirmé **jeu=0, rendu=2** : l'ancien journal « DOF supprimée » ne prouvait donc pas une suppression effective dans le rendu. Le test visuel du chapitre 8 reste nécessaire.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39110618829) **SUCCESS : 28/28 suites, zéro avertissement MSVC ASI/DXGI**. Les avis de dépréciation des outils CMake/Node restent distincts ; aucune suppression d'avertissement ajoutée. Source `56a7c6cfc6f39574a7292bd58d6076330610ee60`, archive immuable `ca87039de126c7c395d0359ed83b069917c90fe0`.

[ZIP cumulatif V1.54 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ca87039de126c7c395d0359ed83b069917c90fe0/downloads/DarksidersGenesis_V1.54_CAMERA_DOF_RENDER_FIX_TEST.zip) : **873 459 octets**, SHA256 `1E2579128C11AEBCF0D2EAE21A23BF6771ADB9DFC2A4D7C626F20756900151E0`. Quatre fichiers racine, CRC, AMD64, version, 119 clés/48 actions et téléchargement public HTTP 200 contrôlés. **118 anciennes archives inchangées** ; main/archive, tag et release v1.43 conservés. 72 autres fichiers source et toutes les fonctions hors DOF du module principal inchangés. INI/raccourcis sans modification.

Le jeu est ouvert : **aucun fichier installé remplacé**, aucune écriture dans sa mémoire ni lancement par l'agent. La première interrogation non privilégiée ne voyait pas ses processus ; vérification Windows corrigée en dehors de ce contexte restreint, puis observation `VM_READ` seulement. Ne pas déduire « jeu fermé » d'une liste de processus vide dans un contexte sans visibilité suffisante. Installer après fermeture réelle, sauvegarde et contrôle de l'INI personnel.

**Dash toujours ouvert :** déplacement + visée + dash perd la réponse de visée ; avec tir, une balle normale peut remplacer la munition équipée, Recovery ON comme OFF. Nouveau fait statique : les deux notifications de tir de `Strife_Aim_Dash_Fire_F` imposent `ProjectileOverride = MayhemProjectile_Standard_Blueprint_C` ; les séquences B/L/R référencent également cette classe et restent à contrôler au point d'appel. Cela fournit une piste native concrète, sans prouver encore la provenance de chaque projectile observé ni un comportement du jeu sans ASI. Les variantes spéciales ont leur propre cycle de tir : ne pas remplacer arbitrairement une classe de projectile sans ce cycle. Aucune réparation risquée de munition/visée n'est embarquée dans V1.54 ; capture externe prête pour identifier la transition réelle.

**À tester après installation :** chapitre 8 immobile/mouvement, bascules TPS/vue native, retour temporaire et cinématique ; contrôle court Lava/Charge Shot, faisceaux, collisions devant/derrière et roues coop. [Audit, limites et suite](V1_54_CAMERA_DOF_RENDER_FIX_TEST.md). Les anciens statuts ci-dessous sont historiques.

---


# V1.54 — profondeur de champ et suite de l'audit dash

## Base et objectif

Base `debug` V1.53, état documentaire `997433c2e05170f7bf41f65118c137c691101930`, ASI construit depuis `a240350a0ebeb996a84821f9e39db881c9b909ef`. Les corrections faisceaux libres/obstruction sont validées par l'utilisateur dans ses cas testés, comme Lava/Charge Shot précédemment. Les deux défauts déplacement + visée + dash restent présents Recovery ON et OFF. Nouveau retour : chapitre 8 excessivement flou en TPS **même immobile**.

Objectif isolé V1.54 : atteindre la véritable valeur de profondeur de champ lue par le rendu, en conservant la restauration native. Réussite en jeu : paysage net en TPS au même endroit du chapitre 8, retour des effets d'origine en vue native/cinématique. **SOURCE EXPÉRIMENTALE ; aucun résultat visuel affirmé avant retour utilisateur.**

## Preuve native du défaut de flou

Exécutable Windows x64 supporté, SHA256 `9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54`.

- Enregistrement `122560` : nom UTF-16 `r.DepthOfFieldQuality` à `2814AD0`, défaut 2, flags `60`. La virtuelle `+38` du CVar renvoie ses données ; pointeur stocké à `381DEA8`.
- Rendu `F5D540` : recherche du **même nom** à `F5D6DD`, même virtuelle `+38` à `F5D6F5`, cache du pointeur à `39BC248`. `F5D59C` relit ce cache et `F5D5A3` teste **`[data+4]`** avant le calcul de profondeur de champ.
- Ancien code du mod : vérification et écriture de quatre octets seulement, `data[0]`. Le rendu pouvait donc continuer avec `data[1]` positif malgré le journal « quality -> 0 ».

Ce constat provient du binaire exact. Le principe de deux copies jeu/rendu est également décrit dans la [documentation Epic de TConsoleVariableData](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Core/TConsoleVariableData) ; cette documentation récente apporte le contexte, pas la preuve des offsets UE4 du jeu.

## Correction et restauration

Remplacement de l'ancien bloc DOF dans `DarksidersGenesisMod.cpp`, sans nouveau hook : contrôle de huit octets accessibles en écriture, captures distinctes jeu/rendu, échanges atomiques 32 bits sur les deux copies. Initialisation protégée par l'exécutable exact et les signatures de l'enregistrement et de la lecture native `data+4`.

Les valeurs sont capturées à l'entrée de la caméra déplacée, plutôt qu'une fois pour toute la session. À la sortie, chaque copie est restaurée seulement si elle contient encore notre zéro. Un changement natif non nul est conservé. Une allocation remplacée n'est jamais relue pour restauration. La vue native sans intervention du mod peut donc changer sa qualité normalement ; une activation suivante capture ces nouvelles valeurs. Le journal indique séparément les copies jeu/rendu, avec budget de corrections borné.

Le signal existant `tpsCutsceneSuspended` donne priorité aux effets cinématiques natifs. Aucun nouveau détecteur, aucun traitement d'une touche physique. Le raccourci de retour temporaire restaure aussi la profondeur de champ en l'absence de zoom manuel. Le zoom manuel conserve sa règle de suppression en gameplay. Cette CVar est globale : la qualité DOF n'est pas indépendante entre deux vues coop. Ne pas annoncer une restauration par viewport.

## Audit dash : progrès et limite

Analyse externe des scripts cuits Dash Start/Aim/Fire et base commune, sans publication des assets propriétaires. Le parseur privé contrôle les tailles de code sérialisé et de code en mémoire ; les chemins étudiés ont des fonctions distinctes. Dash Start choisit Fire si les actions natives 18/20 sont actives, sinon Aim si 31, sinon déplacement. Les niveaux ont chacun leur prochain dash et leurs montages. Les fins et interruptions sont gérées par les capacités et leurs actions en attente.

Le getter de visée reçoit encore des axes dans les observations V1.53. Cela exclut une absence systématique de données à ce point, **pas** un défaut d'état de capacité. L'essai Recovery OFF n'est pas un essai sans ASI ; aucune innocence globale des hooks ni cause native du jeu sans mod n'est affirmée.

Correction d'interprétation pendant l'audit : `69FBD0` lit le champ `AimAbilityClass` du ranged weapon à `618` puis démarre une capacité. Il s'agit de **DoAimAbility**, pas du véritable calcul de trajectoire ni d'un choix démontré de balle normale. La fonction voisine `5B7300` n'est pas le Tick `5B72B0` du dash observé. Ne pas mélanger ces chemins parce qu'ils sont voisins dans le désassemblage.

**Toujours ouverts, aucune nouvelle réparation ASI ajoutée sans preuve :** perte de réponse de visée après déplacement + visée + dash ; munition normale pendant déplacement + visée + tir + dash, Recovery ON/OFF. Prochaine étape : relever noms/classes des capacités, états/actions en attente et armes équipées pendant une reproduction utilisateur ; vérifier ensuite le vrai point de reprise/choix de munition. Capture externe `QUERY_LIMITED_INFORMATION | VM_READ` uniquement. Le jeu n'est jamais lancé par l'agent. Réception lourde et progression du triple dash restent séparées.

## Préservation et contrôles attendus

Les autres fonctions de `DarksidersGenesisMod.cpp` et les autres modules source sont identiques à V1.53. En particulier : XInput et échanges de sticks, RawInput, Alt-Tab, roues et équipement P1/P2, ownership coop, horse/Ultimate, balistique et cible commune, ressources/Hotkeys, Recovery. INI livré, 119 clés et 48 actions inchangés. Pas de diagnostic externe embarqué dans l'ASI. 118 archives antérieures à conserver. Main/archive et release v1.43 intacts.

Compilation Windows x64 et 28 suites existantes à vérifier dans Actions ; contrôles de signatures, intégrité du ZIP de quatre fichiers racine et préservation à relever après construction. Ces contrôles ne constituent pas une validation de gameplay.

## Protocole utilisateur

1. Chapitre 8, même lieu et même orientation : rester immobile, comparer TPS ON/OFF, puis tourner et marcher. Attendu : disparition du flou de profondeur de champ en TPS ; signaler si un flou uniforme reste présent.
2. Répéter les bascules TPS et le retour temporaire. Avec zoom manuel nul, la vue native doit retrouver ses effets. Tester une cinématique puis son retour TPS.
3. Contrôle court Lava, Charge Shot normal/Hotstreak, Static/Beam Shot, obstacle devant et derrière la caméra. Aucune modification de ces calculs dans cette build.
4. Contrôle court roues/équipement en coop. Les deux défauts dash restent à reproduire pour l'audit, pas à considérer corrigés par V1.54.

Rollback : archive V1.53 conservée et sauvegarde des fichiers installés avant remplacement. INI personnel préservé ; installation seulement jeu fermé, sans lancement automatique.
