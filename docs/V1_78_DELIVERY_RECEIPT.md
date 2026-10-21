# V1.78 — reçu de livraison

21 octobre 2026. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE, non installée par l’agent et non validée en jeu.** Base cumulative V1.77 avec validations partielles et défauts signalés par l’utilisateur. [Objectif, changements, limites et essais](V1_78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.md).

[Téléchargement direct immuable du ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/08a5a2ace26e80ea8950a31996ca7fed6650a8ad/downloads/DarksidersGenesis_V1.78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.zip) · [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39669799213). HTTP 200 et SHA256 contrôlés après publication.

- Source : `3c4f11f1c11f8cd7336736d01eb3f53d3850f9ce` ; commit d’archive : `08a5a2ace26e80ea8950a31996ca7fed6650a8ad`, exclusivement debug.
- **47/47 suites réussies**, zéro avertissement MSVC / analyse C. Fixtures étendues pour épaules, holsters, visibilité des deux côtés montés, roues/pause, propagation native, retour de transformation avant pose prête, refus de propriétaire invalide, Native et Never sauvegardé. Ces tests ne prouvent pas le rendu visuel en jeu.
- ZIP **859530 octets**, SHA256 `92DE396AF50880006569973C1256E6997E4C7AE24283F0EAC31B61371D2A1B89` ; CRC, quatre fichiers exactement à la racine, binaires PE AMD64 et marqueurs de version/acquis contrôlés.
- **163 clés INI fournies et 55 ID Hotkeys conservés**, toutes les anciennes valeurs identiques à V1.77 ; l’INI fourni a la même empreinte. Nouveau mode Never, ID 3, sans changer le défaut TPS Only.
- **142 anciens ZIP inchangés**, nouvelle archive distincte. main/archive/v1.43 vérifiés avant publication, seul debug ciblé ; fichier de reprise préexistant conservé et exclu des commits.
- Patch de nettoyage confronté aux deux audits : 167 lignes nettes retirées dans ses 12 fichiers, code final recoupé avec le candidat revu. Les fonctions du nouveau lot ajoutent du code : ASI **1 519 104 octets, soit +5 120 par rapport à V1.77**. Pas de réduction binaire globale ni gain de FPS annoncé.
- Sources des quinze modules sensibles recoupées avant build (30 fichiers), inchangées : visée/projectiles, commandes natives/TPS/coop, dash, munitions, Hotstreak, vie/potions, sauts, skins, grappin, visée sans tir, réception lourde et sélection des modèles/holsters. Le nettoyage touche séparément ses douze fichiers de télémétrie.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| DarksidersGenesisMod.asi | 1519104 | `E5F06AB8E4FA80680D1642BE598734F1E4046BEBF9DF7190F07AE70A57FE5B71` |
| DarksidersGenesisMod.ini | 4587 | `6D35759332C234D21867F509A542F9EE97046648DDDF15B4C80D5555B8BA4345` |
| dxgi.dll | 155136 | `2B260F6CDB33D544E3F6E8BE5053F4118D5E24D8854B2CABA03706942F654FE1` |
| README.md | 20480 | `D60A45E496F41775DECB046CDCDF8BE602AA0D4DAA0A148C7D98498BFE42F5C6` |

Le README inclus au ZIP décrit honnêtement la préparation avant la CI ; le présent reçu fixe le statut construit/contrôlé. L’archive publiée n’est pas réécrite après coup. L’ASI annonce V1.78 dans sa version compilée.

## Limites et suite

- OFF du modèle persiste dans roues et retour ultime ; pose du bras droit et holster à pied sont de nouveaux candidats. À cheval, modèles en main/rangés masqués ; bras, effets de tir et cuir monté restent natifs. Seul le cas pistolet droit visible à pied était déjà validé par l’utilisateur.
- World Ender reste invisible en TPS dans le dernier essai, visible en vue du dessus et infligeant des dégâts. Acteur de rayon, template et wrapper d’effet présents ; pas de correction dangereuse ajoutée ni de réussite annoncée. Les deux captures VM_READ sont terminées, aucun appel natif ou écriture mémoire.
- Échange libre des six capacités entre personnages encore ouvert ; réception lourde après chute incertaine ; caméra suivant la hauteur des exécutions ajoutée pour plus tard. Coop à tester ultérieurement ; autres FX P2 reportés. HUD général non reproduit.

Installation **manuelle** à la convenance de l’utilisateur, en conservant son INI et ses raccourcis ; aucun fichier du jeu, PAK ou sauvegarde changé par l’agent. Aucun lancement/re-lancement. Rollback : Native par main, ou précédent ZIP V1.77 en conservant l’INI personnel. Ne pas confondre compilation/tests statiques avec validation de ces nouveaux comportements en jeu.
