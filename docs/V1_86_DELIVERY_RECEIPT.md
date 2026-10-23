# V1.86 — reçu du lot cumulatif contrôlé

**Retour utilisateur du 23 octobre 2026, après livraison : bras de War et correction Léviathan VALIDÉS dans les essais rapportés.** Leurs implémentations sont conservées. Cette confirmation ne valide ni le réticule V1.87 encore en essai, ni les autres cas/coop. Le réticule V1.86 reste rejeté pour son crash documenté ; le texte de livraison ci-dessous décrit l'état connu au moment de la livraison.

23 octobre 2026. Branche **debug** uniquement. Base V1.85 `46aff8644d065f0a25ee31895fa131e762b1bef5` ; effets des épées masquées maintenant **VALIDÉS par l'utilisateur dans l'essai rapporté**, code conservé. Bras OFF V1.81 et suivi des exécutions V1.82 conservés. **CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE** pour les nouveautés, aucun résultat visuel supposé.

- Sources compilées : `d0deeb03baffb8d18ae0d129adff31bdb09ffbda` ; archive : `9c04d2514c82a6dad6961ea05686975cb3dde6d9`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39995845270) : run 39995845270, job 120559032585, success ; **52/52 suites**, dont la nouvelle suite PlayerPresentation avec modes du bras, exception d'armure, propriétaires P1/P2, changements de choix, absence de répétition du refresh, curseurs/pages/transitions/focus et configuration. **Zéro erreur et zéro avertissement MSVC**, analyse native activée ; avertissements d'outillage MinHook/CMake et Node seulement, aucun masquage.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9c04d2514c82a6dad6961ea05686975cb3dde6d9/downloads/DarksidersGenesis_V1.86_WAR_ARM_RETICLE_LEVIATHAN_TEST.zip) : **904868 octets**, SHA256 `7D9D509CCF08FF2E39B8CB37AB1EA7C32EA5CCAB3F6B7AF90B6C48BAAAD4009F` ; téléchargement identique à l'archive locale, CRC et architecture AMD64 vérifiés.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1643008 | `23B7C932FBDA65EA901C8DD77AF856F5E62C725B33E4F87FCB890643E65D8127` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `F811808541D3946C409DBA8797910ABE0F61CDF4430F7F7B3AE3AD956E0AA866` |
| `README.md` | 38056 | `3561578AD02E36ACC28D11F0C1CA6906B02948B9EC2BD610065AA976CC28AA12` |

168 clés INI : **166 anciennes valeurs préservées et deux choix à zéro**, 55 ID Hotkeys inchangés. **150 anciennes archives inchangées**, 151 ZIP au total. **115 anciens fichiers source inchangés** ; tous les modules de caméra, tirs, entrées/manette/roues, cheval, bras OFF, effets de lame, ressources et prototype Stoneskin sont conservés. Les changements du fichier principal se limitent au raccordement du module de présentation et à l'ordre d'initialisation Léviathan. Les quatre nouvelles signatures natives et l'EXE exact ont été contrôlés. README/INI du ZIP correspondent aux sources compilées ; le README emballé indique la préparation, ce reçu ajoute les résultats sans réécrire l'archive.

## Contenu et essais

Gameplay → Appearance : War arm appearance Native/Basic/Augmented (aspect seulement, sans donner/enlever le Gantelet) et Mouse reticle Vanilla/TPS/normal/les deux (menus et transitions gardés). Léviathan : garde de signature déplacée avant l'interception partagée des FX de lame, cause confirmée par le journal et les octets de V1.85. Ni signature supprimée ni correctif général de particules.

Tester les trois choix du bras avec retour Native, combat/rechargement/armure/P2 si disponible ; les quatre choix du réticule avec pause/inventaire/roues/menu mod/Alt-Tab ; OFF puis trois effets Léviathan, avec lames Strife masquées puis visibles. Contrôle court des effets de lame, des bras OFF et des tirs validés. [Liste détaillée, limites et contrats](V1_86_WAR_ARM_RETICLE_LEVIATHAN_TEST.md).

**Stoneskin reste ouvert** : cristal incorrect rapporté sur Strife ET War, toutes vues, même slots War Native. La protection sur Strife paraît fonctionner d'après l'utilisateur, mesure complète encore attendue. L'adaptateur étranger V1.83 est inchangé ; le chemin War est renvoyé au natif, sans que cela suffise à écarter une cause indirecte. Aucun correctif visuel annoncé. Cinq autres transferts toujours non livrés : adapter les événements/timing et interruptions de Geyser, les variantes sol/air et retour du mouvement de Caltrops, les phases de Rampage et les composants propres de Clone/World Ender. Ne pas faire passer un simple choix de liste pour un transfert fonctionnel.

main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et tag v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts. Fichier de reprise non suivi conservé et hors commit. Installation manuelle ; aucun fichier du jeu, INI personnel, PAK ou sauvegarde remplacé ; aucune relance ni écriture externe de mémoire. Nouveau code non testé en jeu. Rollback : Native/Vanilla, ou ASI V1.85 conservé (avec sa régression Léviathan connue), même INI et raccourcis.

Suite : retour groupé sur la présentation ; isoler le cristal natif Stoneskin sans conclure sur sa cause, puis avancer sur le premier transfert offensif complet. Réception lourde incertaine, autres FX P2 reportés à la demande, HUD général non reproduit ; aucune validation coop supplémentaire supposée.
