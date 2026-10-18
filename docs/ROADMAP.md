# Actualisation V1.60 — 18 octobre 2026

Zoom In/Clear et rangement V1.59 validés par l’utilisateur. Candidat V1.60 limité aux raccourcis directement dans Weapon sheathing : clavier, un/deux boutons manette ou OFF, Hotkeys conservé, dédoublonnage par joueur et commande propre à chaque manette en coop. SOURCE EXPÉRIMENTALE, construction et essai en jeu à effectuer. Les autres réparations/futures fonctions restent en file, sans nouvelle validation implicite.

[Périmètre et essais V1.60](V1_60_SHEATH_SHORTCUTS_TEST.md). États précédents historiques ci-dessous.

---

# Actualisation V1.59 — 18 octobre 2026

**CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE** : CI Windows 33/33, zéro avertissement MSVC ASI/DXGI, ZIP vérifié et publié sur debug ; V1.59 installée jeu fermé avec sauvegarde V1.58, INI personnel conservé ; relevé monté effectué. Aucune nouvelle fonction V1.59 validée en jeu.

- VALIDÉ par l’utilisateur : Hotstreak sous 1 et prompts d’interaction P1/P2 de V1.58.
- Clear Zoom In : conflit en-tête/ligne identifié, candidat V1.59 et test réel ImGui.
- Dash : Action Recovery supprimé ; filtre des notifications distinct conservé, liste d’armes retirée dans la candidate. Visée sans tir toujours retenue ; tirs Standard forcés encore à valider.
- Rangement : candidat natif V1.59, contrôle automatique OFF par défaut, jamais/délai 0–300 s, action manuelle assignable. Strife mesuré 4 s ; War Blueprint 5 s, essai War/P2 nécessaire.
- Grappin, Standard finies, modèles alternatifs, holsters montés : audit poursuivi, non implémentés. Grappin : limites de recherche/relais/traction distinguées ; Standard : capacité native et sentinelle -1 identifiées, proxy exact et restauration encore à prouver. Quatre meshes de pistolets (en main/rangés) identifiés ; pas d’alternative 3D compatible démontrée.
- Flou et autres sprites P2 : encore ouverts. Aucun rendu global modifié.

[Périmètre, preuves et essais V1.59](V1_59_BINDINGS_NATIVE_SHEATH_TEST.md). Statut SOURCE EXPÉRIMENTALE ; les états précédents ci-dessous sont historiques.

---

# Actualisation — 18 octobre 2026, V1.58

L’utilisateur autorise explicitement de commencer la suite malgré le tir pendant dash encore ouvert avec toutes les munitions. Le tableau ci-dessous remplace l’ancien ordre d’attente ; les détails historiques restent conservés.

**V1.58 CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE** : 31/31 suites Windows, ZIP cumulatif vérifié et publié sur debug ; installation effectuée jeu fermé, INI personnel conservé ; aucune validation utilisateur des nouvelles fonctions. [Preuves et limites](V1_58_RESOURCE_HUD_CLEANUP_TEST.md).

| Sujet | État actuel | Prochaine preuve / essai |
| --- | --- | --- |
| Taille du prompt d’interaction | Candidat V1.58, 0.65 par défaut, saisie manuelle/OFF | Lisibilité et restauration en P1/P2 |
| Prompt P2 tournant avec P1 | Cause native identifiée, candidat V1.58 limité à InteractHUD | Rotation indépendante des deux vues |
| Autres sprites plats P2 | Ouvert, pas de correction générale | Identifier un autre composant/propriétaire démontré |
| Facteurs sous 1 | Candidat V1.58, tests numériques/config | Hotstreak, ammo et potions à 0.5 |
| Action Recovery | Expérience de fin anticipée retirée V1.58 | Repartir d’un verrou d’action natif, objectif distinct |
| Visée maintenue après dash | Validée V1.56, code préservé | Contrôle court après retrait Recovery |
| Tir pendant déplacement + visée + dash | Ouvert toutes munitions, native/TPS | Cycle montage/capacité commun, pas une touche |
| Flou chapitre 8, cheval et Ultimate | Ouvert ; distance correcte prouvée | Mélange final du rendu, méthode et paramètres du niveau |
| Rangement manuel, délai réglable/jamais automatique | Demande enregistrée, non implémentée | Fonction et état natifs de rangement |
| Standard ammo finies | Demande enregistrée, non implémentée | Réserve native, consommation et remplissage |
| Portée du grappin | Demande enregistrée, non implémentée | Distances et conditions de chaque action native |
| Modèles des pistolets | Faisabilité ouverte | Propriétaires, sockets, assets compatibles |
| Pistolets/holsters à cheval | Ouvert | Attachments et états natifs précis |

[Audit V1.58](V1_58_RESOURCE_HUD_CLEANUP_TEST.md). V1.57 reste le rollback installé ; la V1.58 est expérimentale tant que CI/paquet puis essais utilisateur ne sont pas terminés.

---

# Suite du mod — demandes du 18 octobre 2026

**Retour utilisateur du 18 octobre :** déplacement + visée + dash validé dans son essai V1.56. Le personnage reste flou au chapitre 8 ; son et manifestation Standard du Dash Fire spécial restent ouverts. Candidat [V1.57](V1_57_FOCUS_DASH_PRESENTATION_TEST.md) ciblé sur ces deux défauts, sans rouvrir la reprise AIM. Les fonctionnalités suivantes restent non implémentées.

## État et ordre de travail

**DEMANDES ENREGISTRÉES / NON IMPLÉMENTÉES.** Cette mise à jour est documentaire. Aucun changement d'ASI/DXGI, d'INI, de raccourci ou d'installation ; aucune nouvelle build. Développement exclusivement sur `debug`, main/archive et anciennes archives conservées.

La V1.56 installée est **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE**, pas encore validée par l'utilisateur. Elle conserve les tirs précédemment validés et teste la reprise de visée après dash ainsi qu'une mise au point TPS conservant la profondeur de champ. Le son et l'animation du tir Standard pendant le dash restent ouverts. Voir [état courant](CURRENT_STATE.md) et [audit V1.56](V1_56_NATIVE_DASH_FOCUS_TEST.md).

**Priorité donnée par l'utilisateur : terminer ces réparations avant les ajouts ci-dessous.** Après ce lot, examiner les défauts visuels HUD/coop/cheval, puis le contrôle de rangement, les options de gameplay et la personnalisation des modèles. Une piste statique n'est ni une fonctionnalité ni une validation en jeu.

## Demandes retenues

| ID | Demande | Comportement souhaité et réussite à vérifier |
| --- | --- | --- |
| HUD-1 | Réduire certains éléments HUD, notamment l'indication de touche au-dessus du personnage pour un levier | Réglage de taille ciblé sur les indications d'interaction concernées. Indication lisible, ancrage correct, affichage de la vraie commande reconfigurée, sans redimensionner tout le HUD. Vérifier vue native/TPS et P1/P2. |
| HUD-2 | Corriger les sprites/éléments HUD plats en P2 | Retrouver les classes et leur propriétaire ; vérifier caméra de référence, espace monde/écran, orientation, projection et échelle. Chaque vue coop doit afficher correctement les éléments, sans dégrader P1. **L'orientation vers une mauvaise caméra est une hypothèse utilisateur, pas une cause établie.** |
| WEAPON-1 | Contrôler le rengainage de Strife et War | Système désactivé : comportement natif conservé. Système activé : choix de ne jamais rengainer automatiquement ou de le faire après un délai réglable en secondes. Ajouter une action manuelle assignable dans Hotkeys pour rengainer immédiatement lorsque l'état natif l'autorise. L'utilisateur estime le délai vanilla à environ dix secondes : valeur à mesurer. |
| AMMO-1 | Pouvoir rendre les munitions normales limitées | Option distincte, désactivée par défaut pour conserver l'infini natif. Rechercher le vrai coût/réservoir du Standard Shot avant de choisir capacité et alimentation. Vérifier zéro munition, consommation, remplissage et interaction avec les options de réduction/Unlimited existantes, en solo/coop et normal/Hotstreak. |
| HOOK-1 | Augmenter la portée du grappin | Réglage avec saisie numérique cohérente avec le menu actuel. Distinguer sélection de cible, portée de traction d'ennemi et points de traversée ; modifier la bonne limite native avec ses contrôles habituels. Vérifier jeu natif/TPS, P1/P2, acquisition, traction, traversée et annulation. |
| MODEL-1 | Si possible, changer les modèles 3D des pistolets de Strife | Auditer les meshes disponibles et leur compatibilité ; les modèles souhaités restent à choisir après cet inventaire. Couvrir les deux mains, les armes rangées et le cavalier. Identifier les besoins éventuels en assets et en empaquetage avant de promettre la fonction. |
| HORSE-1 | Réparer les pistolets mal placés/hors holsters à cheval | Identifier quelle représentation est visible, ses sockets, attachements, transformations et états de visibilité. Corriger le défaut signalé à cheval sans changer le tir monté validé. Vérifier monter/descendre, tirer/cesser de tirer, rangement, changement de personnage et coop. |

## Premiers repères vérifiés, sans changement de gameplay

### Indications et sprites

Les chaînes du binaire exact contiennent `GetMaterialBillboards`, `m_materialBillboards`, `m_ScreenSpaceIndicatorWidgets`, `AddScreenSpaceIndicator` et `RemoveScreenSpaceIndicator`, ainsi que les types moteur d'espace monde/écran. Ce sont des **candidats de recherche** : ils ne désignent pas encore l'indication de levier concernée ni le sprite défectueux en P2.

Le code actuel des caméras possède déjà des contextes distincts P1/P2. Il faudra vérifier que le composant UI ou son rendu utilise bien le contexte correspondant, plutôt que déduire le propriétaire d'après l'ordre d'apparition des personnages. Une rotation globale d'un élément partagé vers une seule caméra pourrait déplacer le défaut dans l'autre vue ; examiner le rendu par vue avant d'écrire une transformation.

### Armes rangées, cavalier et modèles

La référence locale extraite de `Strife_Blueprint` contient `LeftPistol`, `RightPistol`, `LeftPistolHolstered_GEN_VARIABLE`, `RightPistolHolstered_GEN_VARIABLE`, `RiderMesh_GEN_VARIABLE`, `DualPistolsBP` et la référence montée `Mayhem_Mayhem_DualPistols_Blueprint`. Ces noms indiquent plusieurs représentations à suivre ensemble ; ils ne prouvent pas encore quelle instance ou quel socket produit le défaut utilisateur.

Aucun délai de rengainage, fonction native de rangement ou chemin War n'est identifié à ce stade. Ne pas remplacer la mécanique par un simple masquage de mesh, ni supposer que les pistolets en main et rangés sont un seul objet. Respecter les transitions natives de visée, tir, combat et animation. Le rangement manuel doit être une action reconfigurable ; aucun bouton clavier/manette fixe à simuler.

### Munitions normales

`AmmoConsumptionPolicy.h::ConsumptionRequest` conserve explicitement les valeurs négatives d'infini natif ; `AmmoConsumptionFeature.cpp::OnLivePlayer` ignore ces réserves pour le remplissage et Unlimited. C'est un comportement voulu de la base validée, pas un défaut à retirer globalement. Les réservoirs finis spéciaux et leurs crédits de division doivent rester inchangés.

L'index local des assets contient les capacités Standard Shot normales et Hotstreak. Il faut encore vérifier si elles possèdent un réservoir exploitable, quelle voie consomme les tirs et comment une réserve finie peut se remplir. Pas de compteur HUD simulant une consommation qui ne serait pas appliquée au gameplay, ni d'option affichée avant que le chemin réel soit établi.

### Grappin

Les références déjà disponibles comprennent `MayhemGhostHook_Strife_Blueprint`, `MayhemGhostHookPullAbility_Strife_Blueprint`, `MayhemGhostHookZiplineAbility_Strife_Blueprint` et les états natifs de ciblage/attachement/traction. Elles ne démontrent aucune valeur de portée ni un unique multiplicateur commun. Examiner les distances natives utilisées par chaque fonction avant toute intervention.

L'inventaire complet de noms de ce premier passage est conservé localement dans `audit-v131/v156-future-features-inventory.json`. Les références extraites du jeu restent privées et inchangées ; aucun asset du jeu n'est ajouté au dépôt.

## Cadre des futures builds

- Commencer chaque expérimentation depuis la dernière base retenue après le retour utilisateur, avec un objectif et un critère de réussite précis. Garder rollback et anciennes archives.
- Lier les corrections au composant et à la fonction natives, au joueur propriétaire et à la vue, en respectant les commandes reconfigurables et l'échange des sticks.
- Conserver les INI et Hotkeys existants. Les options nouvelles préservent le comportement natif lorsqu'elles sont désactivées ; ne pas renuméroter les actions existantes.
- Uniformiser la présentation des valeurs et leur saisie manuelle. Aucun réglage sans effet, hook spéculatif, expérience écartée ou code mort dans l'ASI livré.
- Séparer contrôle de source/CI/ZIP et validation de gameplay. Aucun nouveau test, résultat, socket, modèle compatible ou réglage natif n'est annoncé sans preuve.
