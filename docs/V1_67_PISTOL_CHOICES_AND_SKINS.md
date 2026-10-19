# V1.67 — pistolets par main et disponibilité des skins

19 octobre 2026. **PRÉPARÉ / EXPÉRIMENTAL : construction et contrôles Windows à effectuer, aucun essai de cette version dans le jeu.** Branche debug uniquement. Base retenue V1.66, sauts et holsters validés par l’utilisateur, reçue dans `f53cb589b6c5afa573c3f83c571691b907920ab4` ; code runtime de cette base `f01f70d563dc4e3716464718fb6642bbdd8f9702`.

## Objectif et critère de réussite

Choisir indépendamment le pistolet natif de gauche ou de droite dans chaque main de Strife, avec une représentation rangée correspondante à cheval. Proposer séparément les deux montures bonus et les six apparences de progression dans la page Skins native, sans accord de récompense ni modification de leurs conditions de progression.

Critère en jeu : toutes les combinaisons de pistolets visibles et correctement tenues à pied/cheval, tirs et rangement conservés ; les deux cases de skins rendent uniquement leur groupe disponible, et OFF rend la décision native. Une compilation réussie et les tests isolés ne valident pas ces effets dans le jeu.

## Inventaire réellement lu

PAK PC principal : version 8, **86641 entrées**, SHA1 d’index `5d0982cd46a5221290cb6ed24696e3962e9dddff`, vérifié avant extraction privée. Les quatre tables de skins enregistrent les éléments suivants :

| Groupe | Identifiant natif | Apparence | Condition de la table originale |
| --- | --- | --- | --- |
| Tenue de base | WarSkin_Default | War normal | Disponible par défaut |
| Progression | WarSkin_Abyssal | War abyssal | Skin_War_Abyssal, marqueur War Abyssal |
| Progression | WarSkin_Silver | War argenté | Skin_War_Silver |
| Progression | WarSkin_Gold | War doré | Skin_War_Gold |
| Tenue de base | StrifeSkin_Default | Strife normal | Disponible par défaut |
| Progression | StrifeSkin_Abyssal | Strife abyssal | Skin_Strife_Abyssal, marqueur Strife Abyssal |
| Progression | StrifeSkin_Silver | Strife argenté | Skin_Strife_Silver |
| Progression | StrifeSkin_Gold | Strife doré | Skin_Strife_Gold |
| Monture de base | RuinSkin_Default | Ruin, monture de War | Disponible par défaut |
| Bonus de précommande | RuinSkin_Rampage | Rampage pour War | bUnlockedByDlc=true, PreOrder |
| Monture de base | MayhemSkin_Default | Mayhem, monture de Strife | Disponible par défaut |
| Bonus de précommande | MayhemSkin_Despair | Despair pour Strife | bUnlockedByDlc=true, PreOrder |

Résultat : **deux bonus hors progression, six variantes de progression, quatre apparences de base**. Les quatre icônes d’armure par personnage comprennent bien la tenue de base : elles ne signifient pas quatre variantes supplémentaires. Aucun autre skin jouable enregistré dans ces quatre tables n’a été trouvé. Les icônes WeaponSkin1/2, matériaux StoneSkin, représentations montées et modèles d’ennemis ne constituent pas la preuve d’un skin supplémentaire déblocable. Aucun contenu « abandonné » jouable supplémentaire n’est affirmé. L’inventaire ne prouve pas la disponibilité commerciale actuelle des anciens bonus.

Références privées : `v167-main-full-index.json`, `v167-cosmetic-tables.json`, `v167-model-inventory.json`. Assets du jeu extraits uniquement dans le dossier d’audit, hors Git et hors ZIP.

## PAK installé et choix ASI / PAK

Le PAK local `DarksidersGenesis_PreOrderMounts_P.pak` est en version 3 et contient **exactement quatre fichiers** : uasset/uexp de `MayhemSkinTable` et `RuinSkinTable`. SHA1 d’index `9fbb4e89c72a4b0a62a2267b7bd3e6f73bbef786`, contrôlé. Pour les deux seules lignes bonus, il change `bUnlockedByDefault` false → true et `bUnlockedByDlc` true → false. Les entrées de base restent identiques. Il n’apporte aucun modèle ni armure de personnage.

Le PAK reste la méthode simple déjà installée pour ces deux montures ; aucun retrait ou remplacement n’est effectué. L’ASI apporte des cases séparées et réversibles à la vérification de disponibilité, avec des gardes propres à l’exécutable exact. **Si le PAK reste installé, OFF dans l’ASI ne peut pas re-verrouiller ce que le PAK rend nativement disponible.** Un essai prouvant l’indépendance de l’ASI devra se faire après fermeture du jeu et mise à l’écart réversible de ce seul PAK. Il n’est pas nécessaire de modifier le PAK pour ajouter des modèles déjà présents dans le jeu.

## Disponibilité native et limites

Preuve statique de l’exécutable exact, SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54` : le thunk `7C4390` reçoit un FDataTableRowHandle emprunté de 16 octets ; `574860` passe le gestionnaire au +650 à `60BF90(manager,handle)`. La fonction vérifie la ligne, sa catégorie, la liste d’apparences du gestionnaire, puis le booléen `bUnlockedByDefault` au +49 de la ligne. Le résultat revient dans AL. Ce chemin vérifie une disponibilité ; ce n’est pas une fonction d’attribution de récompense.

V1.67 ajoute **une seule interception**, sur ce contrôle, avec signature et pont natif contrôlés. L’original reçoit toujours exactement ses deux arguments et est appelé une fois. Un résultat natif vrai est conservé. Pour un résultat faux, l’option peut seulement accepter les huit identifiants précis ci-dessus, dans leur DataTable de classe et package exacts ; les FName avec suffixe numérique, mauvais package, autre table, tenue de base et cas inconnus restent natifs. Deux groupes indépendants, OFF par défaut.

Aucun contrôle global de DLC, Steam ou boutique n’est remplacé. Aucun drapeau de sauvegarde, compteur de pièces abyssales, inventaire, quête, succès ou statistique n’est écrit par cette fonction. La sélection et l’application restent celles du jeu. Le choix d’apparence équipé peut être mémorisé par le jeu : **ce n’est pas une attribution permanente de toutes les récompenses**. Choisir une apparence possédée avant de désactiver son groupe ; le comportement d’une apparence déjà équipée après désactivation doit être vérifié en jeu. Les effets indirects éventuels de la sélection native ne sont pas déclarés testés.

## Pistolets : choix demandé, pas de nouvel asset

Le modèle original de gauche diffère bien de celui de droite. Deux listes indépendantes : **Native / Right pistol model / Left pistol model**. Native conserve le modèle original de la main. Les deux mains peuvent recevoir le modèle droit, le gauche ou être interverties. Les réglages s’appliquent aux Strife des deux joueurs locaux ; ce ne sont pas des profils distincts P1/P2.

Les composants natifs observés sont `Strife+1040/+1048` et `Despair+BA0/+BA8` en main, `Despair+BF8/+C00` dans les holsters. Propriétaire, parent, autorité, type et vtable sont vérifiés. Les choix utilisent `Strife_Gun_Mesh_R/L` en main et leurs variantes `Strife_Gun_R/L_Stowed` à cheval. **Les modèles Stowed ne servent jamais de modèles tenus en main.** Le choix explicite d’un pistolet utilise sa variante rangée correspondante ; Native/Native conserve le comportement validé du réglage MountedPistolHolsters.

Même recherche bornée V1.66 de quatre assets déjà chargés, même setter de StaticMesh ; pas de chargement de package, nouveau hook de tir, rotation, déplacement de socket, visibilité forcée, texture ou projectile modifié. Les bouches de canon, effets, prise en main et pose de chaque combinaison demandent une vérification visuelle. Le résultat d’un setter ne valide pas l’alignement rendu.

Chaque restauration est liée au serial UE du composant et ne s’applique que si le modèle courant est encore celui du mod. Un nouveau modèle natif/étranger ne se fait pas écraser par une ancienne sauvegarde. Le retour à Native, le départ du mode et le changement d’identité restaurent les propres écritures restantes ; aucun retour vers un objet réutilisé.

## Configuration, préservation et rollback

Quatre nouvelles clés, **Native/Native et deux cases OFF** : `Values.PistolRightModel`, `Values.PistolLeftModel` (0 native / 1 droite / 2 gauche), `Features.BonusMountSkins`, `Features.ProgressionSkins`. **143 clés, 53 actions Hotkeys inchangées.** Toutes les valeurs du profil V1.66 sont conservées. INI et raccourcis personnels non écrits par l’agent. L’ancien Action Recovery et les autres effets P2 restent hors de ce lot.

Les sources des sauts, du rangement, de la visée, du dash, des caméras, du focus, des commandes, ressources et grappin restent identiques à la base validée. Anciennes archives et main/archive préservés. Rollback : Native sur chaque main et cases de skins OFF (le PAK reste prioritaire s’il est présent) ; pour retirer les nouvelles interceptions, revenir au ZIP V1.66 en conservant l’INI personnel. Aucun lancement ni installation par l’agent.

## Essais groupés

1. À pied : droite/droite, gauche/gauche, inversés, puis Native/Native ; vérifier prise en main, tir primaire/secondaire, effets et rangement.
2. À cheval : mêmes choix, apparence rangée correspondante puis tir/rangement/descente ; vérifier Native avec l’option de holster précédente ON et OFF. En coop, vérifier le Strife local sans changement de War.
3. Page Skins : Bonus seulement propose Rampage/Despair, Progression seulement propose les six armures ; les déblocages déjà possédés restent acquis. Sans le PAK uniquement pour prouver l’autonomie ASI.
4. Équiper une apparence bonus et une de progression, revenir à une apparence possédée, décocher les options et vérifier le retour natif. Surveiller une régression des acquis, sans refaire tous les anciens protocoles.

Les tests isolés contrôlent combinaisons, restauration, identités, groupes de lignes et paramètres. La construction Windows et le ZIP devront être contrôlés avant annonce de livraison ; **seul le retour utilisateur validera l’apparence et la sélection dans le jeu**.
