## V1.66 — construite et contrôlée, nouveau cycle de saut expérimental

Base **V1.65** : holsters montés, trois délais de rangement et durée ultime **VALIDÉS par l’utilisateur** ; double saut non validé (deux remontées sans animation puis blocage). V1.66 remplace la substitution de compteur par un réarmement natif conjoint publié à l’animation et anticipe la recherche des modèles montés à pied. Aucun nouveau hook ni réglage. **CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL : les sauts animés et la réduction du délai de placement restent à vérifier en jeu.**

[Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39217494013) : **41/41 suites, zéro avertissement MSVC ASI/DXGI**. Source `f01f70d563dc4e3716464718fb6642bbdd8f9702`, archive `12061a956b9257284c8b1779503cf058b75fab1f`. [ZIP cumulatif V1.66 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) : 934413 octets, SHA256 `CC15584E210717E4F173967BE47024CA949983FD24870B03190191936888F67F`. CRC, AMD64, quatre fichiers racine, marqueurs, lien public HTTP 200 et signature identique contrôlés. **139 réglages et 53 actions inchangés ; 130 anciens ZIP préservés**. Main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts. Les avis de dépréciation CMake de MinHook et Node des actions de transfert sont distincts des avertissements du code compilé.

**Essais groupés :** Always dix nouveaux sauts **avec animation**, After gliding un saut par nouveau plané, OFF/atterrissage, TPS/vue native ; délai de première montée après quelques secondes à pied, puis tir/rangement/descente. [Procédure, preuves, limites et rollback](V1_66_NATIVE_JUMP_CYCLE_TEST.md). Acquis de visée, dash, focus, grappin, munitions, ultime, rangement et commandes conservés ; effets/icônes P2 toujours reportés. Aucun lancement ni installation par l’agent, INI personnel intact. Si les sauts restent bloqués ou sans animation, le candidat est à rejeter et l’option à remettre OFF ; compilation et fixtures ne constituent pas une validation du rendu. Les préparations ci-dessous sont historiques.

---

## V1.66 — nouveau cycle de saut à tester, acquis V1.65 conservés

**Retour V1.65 :** modèles de holster montés **VALIDÉS** avec un délai de placement ; trois délais de rangement **VALIDÉS** ; durée ultime **VALIDÉE** après activation de sa case. **Double saut NON VALIDÉ** : deux remontées, sans animation, puis blocage. Ces retours remplacent les mentions « à tester » V1.65 ci-dessous.

**Candidat V1.66 EXPÉRIMENTAL, construction à effectuer :** retrait de la substitution du compteur dans DoJump ; réarmement conjoint des deux compteurs via leurs setters natifs au passage de lecture d’animation, après observation du double saut et relâchement de l’action native. Le moteur reçoit le vrai compteur disponible puis publie à nouveau deux au saut réussi. Aucun reset de nœud d’animation, compteur UE, vitesse ou touche forcé. Recherche bornée des modèles de holster anticipée à pied pour réduire l’attente à la montée ; placement validé conservé. [Preuves, limites, tests et rollback](V1_66_NATIVE_JUMP_CYCLE_TEST.md).

**À tester ensemble :** Always pendant au moins dix appuis, animation à chaque nouveau saut ; After gliding un saut par nouveau plané ; OFF, atterrissage et comparaison TPS/vue native ; premier montage après quelques secondes à pied, tir/rangement/descente. Effets et icônes P2 restent reportés à la demande de l’utilisateur. Aucune relance ni installation par l’agent, INI personnel intact. Historique antérieur conservé ci-dessous.

---

## V1.65 — construite et contrôlée, essais groupés à faire

**CONSTRUIT / CONTRÔLÉ / EXPÉRIMENTAL — pas encore VALIDÉ par l'utilisateur.** Base cumulative V1.64 ; dernier jeu observé V1.63. Le lot regroupe correction du cycle saut/plané, essai réversible des modèles natifs de holster à cheval (nouvelle case OFF par défaut), trois délais de rangement et durée ultime 0–10 inclus depuis V1.64. Les acquis validés sont conservés.

[GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39208468459) **SUCCESS : 41/41 suites, 0 avertissement MSVC ASI/DXGI**. Source `fdf7a8607e282d8da1b569c7b13828f41f0bdbe0`, archive `310c19b918f2b682add6e514f09bf2ba6fdf59a4`. [ZIP cumulatif V1.65 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/310c19b918f2b682add6e514f09bf2ba6fdf59a4/downloads/DarksidersGenesis_V1.65_AIR_JUMP_HOLSTERS_TEST.zip) : 931063 octets, SHA256 `7CEC19D21D94826FE7002A07849A250BA32EF31D5F1880456DF11973F0178CE6`. CRC, AMD64, quatre fichiers racine, marqueurs, **139 clés / 53 actions**, HTTP 200 et **129 anciens ZIP inchangés** vérifiés. Les 138 valeurs précédentes du profil fourni sont conservées ; seule MountedPistolHolsters=0 est ajoutée. Main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et tag v1.43 `902861eb1ff34c14dc5ded092581b360767da817` revérifiés inchangés.

**Tests dans une même version :** (1) sauts OFF/Always au moins dix appuis/After gliding un saut par nouveau plané, vue native et TPS ; (2) Mounted pistol holsters ON/OFF, orientation puis tir/rangement/montée-descente ; (3) trois délais distincts et raccourcis de rangement ; (4) Ultimate decrease rate 0.5/1/2 puis 0, Unlimited OFF/ON. Auto-check V1.63 encore sans retour. [Détails, preuves et rollback](V1_65_AIR_JUMP_HOLSTERS_TEST.md).

La capture de l'ancienne version identifie le crédit effacé en mode plané et les modèles de holster ; elle **ne valide pas** les nouveaux comportements. Ajustement visuel des Stowed et continuité d'Always encore à confirmer en jeu. Les autres icônes et effets P2 sont laissés en attente à la demande explicite de l'utilisateur ; aucune modification de ces couches. Autres modèles 3D et nouvelle Action Recovery restent ouverts. Aucun lancement/installation par l'agent, aucune écriture de l'INI personnel. Blocs de préparation ci-dessous conservés comme historique.

---

## V1.65 — lot cumulatif sauts, holsters, rangement et ultime

**CANDIDATE EXPÉRIMENTALE — nouveaux comportements à valider en jeu.** Base cumulative V1.64, acquis validés conservés. Les trois délais de rangement (lames Strife / pistolets Strife / épée War) et Ultimate decrease rate 0–10 restent inclus ; l'utilisateur joue encore en V1.63 lors du relevé du 19 octobre. Aucun retour ne valide implicitement V1.64/V1.65.

**Sauts :** le plané utilise le mode natif Custom, pas Falling ; l'ancienne observation effaçait donc son crédit. Le candidat conserve ce crédit et, sur la fonction native de demande de saut, termine un plané éligible avec StopGliding avant de laisser la machine d'actions native décider du saut. Always ne dépend plus d'une observation caméra préalable. L'impulsion reste native, avec extension/restauration du seul délai pendant un saut supplémentaire tardif. Aucun compteur, vitesse, animation ou bouton physique n'est forcé. OFF reste natif. La capture confirme le défaut d'observation ; elle ne valide pas encore la correction ni la continuité d'Always.

**Mounted pistol holsters (experimental)** : nouvelle case dans Weapon sheathing, **OFF par défaut**. Essai limité aux deux composants de holster de Despair : modèles natifs Stowed à la place des modèles de main, déjà chargés, via SetStaticMesh natif ; restauration sur OFF/sortie du cheval et protection contre réutilisation d'objet. Aucun masquage, rotation, socket, projectile ou arme en main modifié. L'ajustement visuel est une hypothèse étayée par les géométries natives différentes, à vérifier avant toute validation.

**Essais groupés :** sauts Off / Always (10 appuis) / After gliding (un nouveau saut par plané), TPS et vue native ; holsters ON/OFF puis tir/rangement/montée-descente ; trois délais de rangement distincts et raccourcis communs ; durée ultime 0.5/1/2, puis 0, Unlimited OFF/ON. [Procédure, preuves, rollback et état de construction](V1_65_AIR_JUMP_HOLSTERS_TEST.md).

Icônes et effets de tir P2 laissés en attente à la demande de l'utilisateur. Autres modèles 3D et nouvelle Action Recovery encore ouverts. Anciennes archives, INI personnel et raccourcis préservés ; aucune installation ni relance par l'agent. Les blocs plus anciens ci-dessous décrivent leurs versions respectives.

---

## Livraison V1.64 — 19 octobre 2026

Trois délais de rangement indépendants, raccourcis communs conservés, durée ultime 0–10 : CONSTRUITS / CONTRÔLÉS, non validés en jeu, 40/40 suites. [Reçu et tests](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md). Grappin V1.63 validé dans l'essai rapporté. Double saut reste en défaut, sans changement de son code ; auto-check sans retour. Aucun autre sujet clos implicitement.

---

## V1.64 en préparation — 19 octobre 2026

**Grappin V1.63 validé par l'utilisateur dans son essai.** Nouvelle demande de rangement : conserver tous les raccourcis et séparer lames de Strife, pistolets de Strife et épée de War ; lames natives par défaut. Code et migration préparés, avec réglage de durée ultime 0–10, ×2 proposé, OFF par défaut. **CONSTRUCTION À EFFECTUER / EXPÉRIMENTAL**, aucun résultat en jeu revendiqué. Double saut encore ouvert : la séquence n'a pas été capturée ; pas de changement de son code. Aucun lancement/installation ni écriture de l'INI utilisateur par l'agent. [Périmètre, preuves et tests V1.64](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md). Les instructions d'attente et états antérieurs ci-dessous sont supersédés par le retour reçu et la demande de continuer.

---

## Retour utilisateur du 19 octobre — attente des essais V1.63

**VALIDÉS dans les essais rapportés :** raccourcis de rangement, netteté TPS, Standard limité et disparition du son/des effets du tir pendant le dash. **NON VALIDÉ : double saut** ; seul Always on autorise trois sauts supplémentaires, puis bloque. Grappin V1.63 et auto-check : retour encore attendu. La liste a été préparée avant le dernier message V1.63 ; aucune validation de cette build n'est déduite.

**À la file :** Ultimate decrease rate 0–10, durée réduite sous 1, native à 1, allongée au-dessus, **×2 par défaut** ; distinct du gain de jauge. **Attendre le retour V1.63 avant toute nouvelle modification de code/build.** [Retour exact et consigne de reprise](USER_FEEDBACK_2026_10_19.md). Les états antérieurs ci-dessous sont historiques lorsqu'ils diffèrent de ce retour.

---

## Livraison V1.63 contrôlée — 19 octobre 2026

39/39 suites et ZIP Windows vérifié ; [reçu](V1_63_GRAPPLE_INIT_UPDATE_TEST.md), [essais cumulatifs](TESTS_DEPUIS_V1_60.md). Correctif du contrat natif du grappin + auto-check, encore EXPÉRIMENTAUX en jeu. V1.62 conserve un risque de crash même portée OFF ; archives préservées. Aucun autre sujet clos implicitement.

---

## Priorité V1.63 — 19 octobre 2026

Crash grappin V1.62 identifié : trois arguments natifs perdus, y compris OFF. Correction et auto-check préparés dans [V1.63](V1_63_GRAPPLE_INIT_UPDATE_TEST.md), construction et validation en jeu à effectuer. [Essais cumulatifs depuis V1.60](TESTS_DEPUIS_V1_60.md). V1.62 conservée historiquement, son chemin grappin ne doit plus être proposé comme sûr.

---

# File active — lot V1.62 (19 octobre 2026)

Tableau cumulatif actualisé pour V1.63 ; les entrées datées conservées plus bas décrivent leurs versions respectives.

| Sujet | État actuel | Suite |
| --- | --- | --- |
| Zoom In/Clear, rangement natif, prompts P1/P2, facteurs sous 1 | VALIDÉS par l’utilisateur | Préserver, contrôle court |
| Reprise AIM après mouvement+visée+dash | VALIDÉE V1.56, conservée | Ne pas réintroduire l’ancien Recovery |
| Standard limité | VALIDÉ par l’utilisateur | Préserver |
| Double saut trois modes | NON VALIDÉ : Always on permet trois sauts supplémentaires puis bloque ; autres modes sans résultat attendu | Reprendre après le retour V1.63 |
| Ultimate decrease rate | CONSTRUIT / CONTRÔLÉ V1.64, expérimental : 0–10, valeur ×2, case OFF par défaut | Comparer 0.5/1/2/0 et priorité Unlimited, P1/P2 |
| Grappin étendu | VALIDÉ V1.63 dans l’essai utilisateur | Préserver ; pas de validation implicite de toutes les traversées |
| Auto check update | CONSTRUIT / CONTRÔLÉ V1.63, OFF par défaut | Canal choisi au démarrage, avis temporaire passif puis rappel jaune, validation en jeu |
| Raccourcis de rangement V1.60 | VALIDÉS par l’utilisateur | Préserver |
| Focus TPS V1.61 | Netteté VALIDÉE par l’utilisateur | Préserver |
| Présentation du tir pendant dash | Son et effets parasites disparus, VALIDÉ par l’utilisateur | Préserver ; ne pas élargir le retour à des symptômes non rapportés |
| Holsters montés | Audit, pas de correction démontrée | Pose finale des bones/sockets, pas de masquage global |
| Autres sprites P2 | Ouvert après InteractHUD validé | Identifier un autre composant visible défectueux |
| Modèles 3D alternatifs | Faisabilité ouverte | Quatre meshes main/rangés connus, pas d’alternative compatible prouvée |
| Nouvelle Action Recovery | Ancienne expérience retirée | Verrou natif précis, objectif et preuve distincts |

[Périmètre et essais du lot V1.62](V1_62_NATIVE_GAMEPLAY_BATCH_TEST.md). Une seule livraison du lot après les contrôles ; aucun sujet restant n’est clos implicitement.

---

# Actualisation V1.61 — 19 octobre 2026

Candidate limitée au flou TPS dépendant des angles, toutes formes : option TPS lens focus ON par défaut, OFF = politique V1.60. Correction de l’ancien audit : le volume de niveau active bien les overrides de focus/région/méthode ; leur prétendue absence était une mauvaise lecture. Nouvelle capture VM_READ et garde native du mode circulaire ; CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE : 34/34 suites Windows, paquet vérifié, disponible dans l’updater debug ; essai visuel encore nécessaire. Raccourcis V1.60 installés, validation encore attendue. Grappin, Standard finies, holsters, autres sprites P2 et modèles restent ouverts.

[Preuves et critères V1.61](V1_61_TPS_LENS_FOCUS_TEST.md). Les anciennes conclusions datées ci-dessous sont historiques et supersédées par cette correction.

---

# Actualisation V1.60 — 18 octobre 2026

Zoom In/Clear et rangement V1.59 validés par l’utilisateur. Candidat V1.60 limité aux raccourcis directement dans Weapon sheathing : clavier, un/deux boutons manette ou OFF, Hotkeys conservé, dédoublonnage par joueur et commande propre à chaque manette en coop. CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE : 34/34 suites et ZIP vérifié, installation par l’utilisateur via updater vérifiée, sauvegarde V1.59 contrôlée. Nouveaux raccourcis non validés en jeu. Les autres réparations/futures fonctions restent en file, sans nouvelle validation implicite.

[Périmètre et essais V1.60](V1_60_SHEATH_SHORTCUTS_TEST.md). États précédents historiques ci-dessous.


Retour complémentaire : chapitre 8 encore flou sous certains angles en TPS, à pied, à cheval et en ultime. Non corrigé par V1.60 ; reprise sur le chemin commun du rendu et la zone nette, sans attribuer la cause aux transformations seules.
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
