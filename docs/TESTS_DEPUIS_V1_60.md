## V1.69 — préparation depuis les retours V1.68

20 octobre 2026. Durée ultime infinie **VALIDÉE par l'utilisateur** ; changement de personnage et netteté du héros au chapitre 8 semblent bons. Décors trop nets en TPS ; Léviathan fonctionne mais trop bas. Skins chevaux/armures et retour natif OFF confirmés ; maintien après redémarrage et autonomie sans PAK non certifiés.

Lot : attache Léviathan sur la racine native (écart mesh −97 prouvé par VM_READ), méthode DOF du niveau et protection de la profondeur du personnage, sections Appearance / Weapon sheathing repliables. **PRÉPARÉ / EXPÉRIMENTAL**, construction Windows et nouveau rendu à contrôler. [Preuves, limites, critères, essais et rollback V1.69](V1_69_NATIVE_VISUAL_FOCUS_TEST.md). HUD d'arène, OFF complet pistolet et nouvelle approche Recovery ouverts ; autres FX P2 reportés.

---

## V1.68 — CONSTRUITE / CONTRÔLÉE, nouveaux comportements EXPÉRIMENTAUX

20 octobre 2026. [CI Windows x64 réussie](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39404000199) : **43/43 suites**, aucun avertissement MSVC ASI/DXGI ; avis CMake/MinHook et Node des outils distincts. Source `eac8da5d1e67e6c3afd3e50d3f83c7acf569d7a8`, archive `b1a7110048071987facf6f28af0475b87c1a272b`. [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/b1a7110048071987facf6f28af0475b87c1a272b/downloads/DarksidersGenesis_V1.68_VISUALS_ULTIMATE_TEST.zip) : **817200 octets**, SHA256 `110C3E851C966FD26E87FE31450475553E2985A7158643430923117B2A486960`. Téléchargement public HTTP 200, hash identique à la CI, CRC, AMD64, quatre fichiers racine, marqueurs, 146 clés et 54 identifiants Hotkeys contrôlés. **132 anciennes archives inchangées.** Main/archive et v1.43 conservés.

Base : V1.66 validée pour sauts/holsters + choix des pistolets V1.67 validés. Lot : vrai IsSkinUnlocked à la place du mauvais IsSkinEquipped, focus du même point utilisé pour émettre la géométrie TPS pendant le changement, case **Infinite Ultimate duration**, visuel seul **Leviathan OFF/1/2/3**. Nouveaux résultats en jeu **NON VALIDÉS**. Aucun lancement ni remplacement des fichiers du jeu par l’agent à ce stade ; INI/raccourcis et PAK personnels préservés. [Objectifs, preuves, limites, tests groupés et rollback](V1_68_VISUALS_ULTIMATE_TEST.md). Les paragraphes « préparation » suivants sont historiques.

### HUD : reproduction capturée, correction encore ouverte

L’utilisateur a préparé un HUD visible puis a reproduit la disparition après sélection d’arène verrouillée. Capture du 20 octobre : PID 12880, **V1.67** chargée (ASI SHA256 `ECDE0F74B69B8A3558669A45422BB7D05C5ADDC2135EC4D9DA127D2FCF164286`). 8907 échantillons en cinq minutes, zéro erreur de lecture : `ui.HideHud` natif jeu/rendu reste **0/0**, qualité DOF **2/2**. Le booléen interne du mod `g_hudHidden`, identifié dans le getter compilé V1.67 et lu après signature du module, vaut également **0** ; aucun événement Toggle HUD dans le journal. Cela exclut les deux masquages de ce getter lors de ce relevé, sans prouver une cause extérieure à tout le reste du mod.

Deux instantanés avant/après, sans écriture ni appel natif : acteur HUD et racine générale présents, `bShowHUD` vrai, visibilité racine 4, opacité 1. Les pointeurs et propriétés ne doivent être comparés que sur leur taille réelle : les 16 octets de contexte capturés contiennent parfois le champ voisin ; les UFunction ne sont pas des propriétés d’instance. Le vrai test natif IsInViewport a ensuite été retracé (wrapper 15190A0, consommateur 14B41D0) et reproduit uniquement par lecture : pointeurs +208/+210 présents, compteur partagé +8 = 1, donc résultat vrai. Un simple retrait du viewport n’explique pas cette panne. L’arbre détaillé après panne est conservé en privé pour remonter les états d’affichage/bindings et couches Slate. **Cause et correction non établies ; aucune réparation HUD revendiquée dans le ZIP V1.68.** Les autres éléments possiblement manquants restent un signalement incertain. OFF complet des pistolets et autres tâches de la file restent ouverts.

---

## V1.68 — préparation expérimentale ; erreur de skins V1.67 corrigée à la source

20 octobre 2026. Base retenue : V1.66 (sauts/holsters validés), plus les sélecteurs gauche/droite de pistolet V1.67 **VALIDÉS par l’utilisateur**. Montures V1.67 **en échec** sur une nouvelle partie avec le PAK actif ; autres skins non testés. Le mauvais branchement IsSkinEquipped est retiré au profit du vrai IsSkinUnlocked, avec contrôle de l’association native nom/wrapper/appel. Les déclarations anciennes de provenance du rapport V1.67 sont corrigées dans [le rapport V1.68](V1_68_VISUALS_ULTIMATE_TEST.md).

Lot préparé : correction des skins, continuité de mise au point au changement de personnage, case durée ultime infinie (sans remplissage), sélecteur visuel Léviathan OFF/1/2/3. Trois clés nouvelles opt-in, 146 clés au total, 54 identifiants Hotkeys (anciens IDs conservés). **Construction Windows et vérification du ZIP encore à effectuer ; nouveaux résultats en jeu NON VALIDÉS.** Aucun lancement, installation, remplacement du PAK ou changement d’INI personnel.

Le flou signalé concerne plusieurs niveaux. Relevé : qualité DOF 2/2 conservée ; mode lentille TPS modifiant le rendu général, trou de compensation d’environ 0,64 s au changement observé. HUD absent après sélection d’arène verrouillée : **ouvert, aucune correction revendiquée**. Autres éléments HUD manquants : information incertaine. OFF complet des pistolets (modèle/effets/bras, projectiles conservés) encore ouvert. Recovery et autres effets P2 reportés restent séparés. [Essais groupés, preuves et rollback](V1_68_VISUALS_ULTIMATE_TEST.md).

---

## V1.67 — CONSTRUITE / CONTRÔLÉE, apparences EXPÉRIMENTALES

19 octobre 2026. Base V1.66 validée pour sauts/holsters, conservée. Choix du pistolet natif gauche/droit par main de Strife ; deux cases distinctes pour les montures bonus et les six apparences de progression. **Nouveaux comportements NON VALIDÉS dans le jeu.** [Inventaire, preuves, essais et rollback](V1_67_PISTOL_CHOICES_AND_SKINS.md).

[CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39253436601) : **42/42 suites, zéro avertissement MSVC ASI/DXGI** ; avis de dépréciation MinHook/CMake et Node distincts. Source `4a4bd1bee5126ee9a21cf78fe90048f6c509776d`, archive `c760123057e66f2b3d7053b7acf3441de2cee44e`. [ZIP cumulatif vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c760123057e66f2b3d7053b7acf3441de2cee44e/downloads/DarksidersGenesis_V1.67_PISTOL_CHOICES_SKINS_TEST.zip) : 809061 octets, SHA256 `8E37AE2BBBAD9465BEFE2BC75F2566B74311EB0908E01682D1D6CD02EF3B9591` ; CRC, AMD64, quatre fichiers à la racine, marqueurs, 143 clés/53 actions, téléchargement HTTP 200 et signature identique contrôlés. **131 anciennes archives inchangées.** Configuration personnelle et PAK non modifiés, aucun lancement ni installation par l’agent. Main/archive et tag stable conservés.

Essais : combinaisons de pistolets à pied/cheval, tir/rangement et Native ; **fermer et rouvrir la page Skins après un changement d’option**, vérifier les deux groupes puis OFF. Le PAK déjà installé garde ses deux montures disponibles même avec la case ASI OFF ; l’autonomie ASI exige un essai sans ce seul PAK, déplacé réversiblement après fermeture du jeu. Choisir une apparence possédée avant de désactiver son groupe. Recovery et autres effets P2 restent hors du lot. Les préparations suivantes sont historiques ; elles ne signifient plus que la construction est en attente.

---

## V1.67 — choix des pistolets et skins, préparation expérimentale

19 octobre 2026. Base retenue V1.66 : sauts et holsters validés. Demande clarifiée : choisir le modèle du pistolet droit ou gauche dans chaque main, pas importer un modèle supplémentaire. Deux listes indépendantes préparées, variantes Stowed correspondantes à cheval, restauration par identité native. Une interception de disponibilité cosmétique, ciblée sur huit lignes exactes, propose séparément deux montures bonus et six apparences de progression ; aucun grant ni contrôle global de DLC remplacé. PAK de montures local analysé : deux tables, deux booléens par ligne bonus modifiés, aucun modèle ajouté. [Inventaire complet, preuves, limites et essais](V1_67_PISTOL_CHOICES_AND_SKINS.md).

**PRÉPARÉ / EXPÉRIMENTAL : construction Windows et contrôle du ZIP à effectuer, aucune validation en jeu de V1.67.** Quatre clés nouvelles Native/Native/OFF/OFF, 143 clés et 53 actions ; toutes les valeurs antérieures conservées. Sources sauts/visée/caméras/commandes/rangement/dash/ressources/grappin inchangées. PAK et INI personnels non modifiés, aucune relance/installation. Recovery et autres effets P2 hors de ce lot. Les reçus antérieurs restent historiques.

---

## V1.66 — sauts et holsters VALIDÉS par l’utilisateur

19 octobre 2026. Retour explicite : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». **Sauts et holsters VALIDÉS dans l’essai rapporté ; V1.66 devient la base de travail retenue sur debug.** Les trois délais de rangement et la durée ultime étaient déjà validés en V1.65. Aucun nombre de sauts, mode, personnage, cas coop ou délai mesuré n’est inventé à partir de ce retour général.

Mise à jour documentaire uniquement : code runtime, INI, raccourcis, binaires et ZIP V1.66 conservés. Pas de nouvelle build ni de promotion de main/archive. Action Recovery et modèles alternatifs de pistolets restent ouverts ; autres icônes/effets P2 toujours différés sur demande ; Auto check update sans confirmation visuelle distincte. [État courant](CURRENT_STATE.md) · [File active](ACTIVE_QUEUE.md).

Les critères et mentions « à tester » ci-dessous décrivent la préparation historique et sont remplacés, pour le statut des sauts et holsters, par ce retour utilisateur.

---

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

# Actualisation V1.64 en préparation

Grappin V1.63 désormais confirmé fonctionnel dans l'essai utilisateur. Prochaine vérification ciblée : trois délais de rangement et leurs Never séparés, raccourcis communs conservés ; durée ultime 0–10, OFF/0/0.5/1/2/10 et priorité Unlimited. [Manipulations et statut](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md). Double saut et auto-check toujours ouverts ; les passages historiques qui attendent le premier retour grappin sont supersédés.

---

# État après le retour utilisateur du 19 octobre 2026

**Plus à traiter comme non validés :** raccourcis de rangement, netteté TPS, Standard limité, disparition du son et des effets parasites pendant le dash : confirmés fonctionnels par l'utilisateur. La liste d'essais précédente est conservée ci-dessous comme historique, sans lui redemander ces validations.

**Double saut en échec :** Always on permet trois sauts supplémentaires puis s'arrête ; les autres modes ne donnent pas le résultat attendu. Ne pas présenter Always on comme infini. Reprise de l'analyse après le retour V1.63.

**En attente :** grappin V1.63 et auto-check. Aucun résultat de V1.63 n'est encore déduit du retour préparé avant le dernier message. Nouvelle demande à la file : Ultimate decrease rate 0–10, sens durée et ×2 par défaut. [Retour exact](USER_FEEDBACK_2026_10_19.md).

---

# Essais cumulatifs depuis V1.60

Cette liste n'implique aucune validation en jeu. V1.63 remplace le chemin grappin défectueux de V1.62 ; ne pas reproduire volontairement le crash sur V1.62.

1. **Grappin — V1.63 prioritaire :** OFF/×1, viser puis lancer, annuler et réutiliser avec War puis Strife ; ensuite ON ×2 sur une cible plus lointaine et une traction/relais. Coop P1/P2 et obstacles. La traversée/tyrolienne reste à démontrer.
2. **Rangement — raccourcis V1.60 :** assigner/effacer/recharger le clavier ; bouton manette 1 seul, 2 seul, puis combinaison. Maintien = un seul déclenchement. Coop : chaque manette agit sur son personnage. Même F-touche dans Hotkeys et la ligne dédiée : pas de double déclenchement. Menu/Alt-Tab/reconnexion : pas de rangement différé. Le rangement natif était déjà validé ; contrôle court du délai/jamais automatique.
3. **Flou — focus V1.61 :** chapitre 8, angles auparavant flous puis nets, personnage immobile, TPS à pied/cheval/ultime ; solo et P1/P2. Comparer TPS lens focus ON/OFF, vérifier personnage net, profondeur visible et fluidité. Vue native/cinématique : effet natif.
4. **Standard limité — V1.62 :** activer, capacité 10, Reduce Ammo et Unlimited OFF ; vider et vérifier le refus à zéro. Fill Ammo, réduction ÷2 et priorité Unlimited ; diminuer la capacité borne la réserve, l'augmenter ne remplit pas. OFF rend l'infini natif. Deux slots/P1/P2, changement de personnage et chargement. Hotstreak conserve ses règles natives de coût.
5. **Double saut — V1.62 :** Off natif ; Always = au moins quatre sauts successifs ; After gliding = pas de troisième saut avant un plané, un seul nouveau double saut après chaque nouveau plané. Atterrissage/mort/changement de personnage remettent le droit à zéro. Caméra native/TPS et P1/P2 ; cheval/ultime exclus du correctif.
6. **Tir pendant dash — encore ouvert :** déplacement + visée + tir + dash, munition normale puis spéciale/Hotstreak, TPS puis vue native. Relever séparément projectile Standard parasite, son, flash et animation. La reprise de visée après dash sans tirer était validée ; simple contrôle de régression.
7. **Mise à jour — V1.63 :** Auto check update OFF = pas de check au démarrage ; ON = un seul check du canal choisi. Version strictement plus récente : notification 8 secondes visibles puis bandeau jaune au-dessus des onglets. Menu fermé/Alt-Tab : aucune capture des commandes, notice encore visible au retour. Même version, version antérieure ou réseau indisponible : pas de faux bandeau. Changer de canal efface le résultat. L'installation reste manuelle.
8. **Contrôles courts des acquis :** tirs Lava/Charge/Static/Beam/Graviton à l'arrêt et en reculant ; roues et équipements P1/P2, sticks échangés, TPS à cheval/ultime et cinématiques. Ne pas requalifier ces fonctions en panne sans nouveau résultat.

Non livrés : correction des holsters montés, autres sprites P2 à identifier, modèles 3D alternatifs compatibles, nouvelle Action Recovery. Zoom In/Clear et les prompts d'interaction ne sont pas à refaire comme des nouveautés : déjà validés.
