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

# V1.67 préparée — choix des pistolets et disponibilité cosmétique

19 octobre 2026. **PRÉPARÉ / EXPÉRIMENTAL, construction Windows et contrôle du ZIP à effectuer.** Base retenue V1.66, sauts et holsters validés dans le retour utilisateur ci-dessous. Choix indépendant du modèle droit/gauche par main de Strife, plus deux cases OFF pour montures bonus et six skins de progression. [Inventaire, preuve native, limites, tests et rollback](V1_67_PISTOL_CHOICES_AND_SKINS.md). Configuration personnelle et PAK installés non modifiés ; aucun lancement ni installation. Les nouvelles fonctions V1.67 ne sont pas validées en jeu.

L’état V1.66 ci-dessous décrit la base validée conservée, pas le statut des nouvelles options V1.67.

---

# État courant — V1.66 debug

19 octobre 2026. **Base de travail retenue : V1.66, CONSTRUITE / CONTRÔLÉE ; sauts et holsters VALIDÉS par l’utilisateur dans son essai.** Retour explicite reçu après les tests ; le canal reste debug. Cette mise à jour enregistre la validation sans modifier le code, la configuration ni le paquet publié.

## Livraison vérifiée

- Source : `f01f70d563dc4e3716464718fb6642bbdd8f9702`.
- Archive : `12061a956b9257284c8b1779503cf058b75fab1f`.
- [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39217494013) : 41/41 suites, aucun avertissement MSVC ASI/DXGI. Avis de dépréciation des outils distincts.
- [ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/12061a956b9257284c8b1779503cf058b75fab1f/downloads/DarksidersGenesis_V1.66_NATIVE_JUMP_CYCLE_TEST.zip) : 934413 octets, SHA256 `CC15584E210717E4F173967BE47024CA949983FD24870B03190191936888F67F`. CRC, AMD64, quatre fichiers racine, marqueurs et téléchargement HTTP 200 contrôlés.
- 139 réglages et 53 actions conservés ; 130 anciens ZIP vérifiés intacts à la livraison. Main/archive et tag v1.43 protégés. Aucun remplacement du ZIP V1.66 par ce travail documentaire.

## Derniers retours faisant autorité

État antérieur V1.65 : holsters montés correctement placés, petit délai ; trois délais de rangement validés ; durée ultime validée après activation de sa case (l’utilisateur a rétracté son premier échec). Le double saut était encore défectueux : deux remontées sans animation puis blocage.

V1.66 remplace la substitution isolée du getter par un réarmement natif conjoint au point de lecture d’animation. L’animation voit le vrai compteur disponible puis le saut natif republie deux. Recherche des modèles montés anticipée à pied. **Retour V1.66 : « pour les sauts c'est bon bravo », « pour les holsters c'est bon bravo », puis « donc tout a lair bon/ ». Sauts et holsters VALIDÉS dans cet essai, code retenu.** Le retour ne détaille ni nombre de sauts, ni modes/personnages testés, ni mesure du délai initial ; aucune couverture exhaustive n’est déduite. [Détails et essais](V1_66_NATIVE_JUMP_CYCLE_TEST.md).

Les acquis précédents de visée, reprise AIM après dash, retrait des projectiles/sons/effets Standard parasites, netteté TPS, Standard limité, grappin, ressources et raccourcis sont conservés. Aucun nouveau retour ne les invalide. Autres effets et icônes P2 reportés sur demande explicite.

## Travail poursuivi pendant l’essai

Nouvel audit du chemin d’interruption natif de Recovery et inventaire élargi des modèles de Strife. **Aucune modification de source runtime, de configuration, de binaire ou de mémoire du jeu dans cette reprise.** Le relevé externe préparé n’a pas été exécuté sur le jeu. L’ancien Recovery reste retiré. [Rapport et limites](NEXT_RECOVERY_AND_MODELS.md).

README, file active et roadmap séparés des anciennes préparations ; les trois anciens documents sont conservés octet pour octet dans `docs/history/`, avec manifest de signatures. Le carnet technique reste cumulatif. Les versions et références du présent état n’impliquent aucune promotion de main ni de release publique.

## Reprise suivante

Repartir de V1.66 validée pour les sauts et holsters. Consulter la [file active](ACTIVE_QUEUE.md) et les logs réellement disponibles. Action Recovery et modèles alternatifs restent des travaux ouverts ; autres effets P2 différés, Auto check update sans confirmation visuelle distincte. Préserver l’INI personnel, les raccourcis, les sauvegardes et les acquis. Pas de lancement/rechargement du jeu par l’agent sans demande explicite. Construire une version distincte seulement pour un nouveau lot retenu.

[Ancien état intégral jusqu’à V1.66](history/CURRENT_STATE_THROUGH_V1_66.md) · [Carnet technique](TECHNICAL_NOTEBOOK.md) · [Roadmap actuelle](ROADMAP.md).
