# V1.176 — catalogue post-traitement livré sur debug

**CONSTRUIT / CONTRÔLÉ ; rendu EXPÉRIMENTAL.** Base debug V1.175, base stable main V1.173 conservée. Aucune installation automatique.

17 réglages supplémentaires, tous Native par défaut : bloom, dirt, lens flare/seuil, vignette, grain/jitter, AO intensité/rayon/puissance, SSR intensité/qualité/roughness, exposition, cubemap, LUT et maximum du motion blur. Amount motion blur et netteté jusqu'à 10. Chromatique V1.175 corrigée : le bit voisin 0x20 était erroné ; contrat natif exact 0x10 recoupé avec DOF/motion. Ne plus présenter le rendu V1.175 comme validé.

Les plafonds sont exploratoires, pas des maxima absolus prouvés du moteur. Les dépendances de textures, passes graphiques et mélanges partiels restent natives. Il s'agit du premier catalogue scalaire : les 119 champs inventoriés ne sont pas tous des effets utilisables ; domaines signés/stricts, textures/vecteurs et RTX ne sont pas exposés aveuglément. [Périmètre et test groupé](V1_176_REVIEW.md).

- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44954467996) : Windows x64 Release, **86/86 tests, zéro avertissement/erreur C++**. Premier run rejeté (attente de dix sections INI devenue obsolète), corrigé ; aucun ZIP du run échoué. Une correction intermédiaire du test Windows n'a pas été publiée en binaire.
- Source `d46d1e52466da5b54a5e4f1d5267643c48d4d8ba` ; archive `ce6bcbcc006c60dcf93a69c6eb5ae98b4639fe10`.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ce6bcbcc006c60dcf93a69c6eb5ae98b4639fe10/downloads/DarksidersGenesis_V1.176_NATIVE_POSTPROCESS_LIBRARY_TEST.zip) : 2501870 octets, SHA256 `E25D1BABAA4AADB1A9D98C35CD80200DB009DABC08FF5A70E9B0CE4455861AD3`.
- Deux binaires AMD64 seuls à la racine, CRC/empreintes/blob distant/HTTP200 contrôlés ; Defender ZIP et binaires : aucune menace détectée, protection intacte.
- 240 anciennes archives intactes, 241 cumulées. 262 anciennes clés et valeurs de référence conservées ; 17 ajouts Native, total 279. Défauts gameplay, raccourcis, INI personnel et installation intacts. Aucun build local. Un seul nouveau ZIP récupéré par Git, aucune ancienne archive retéléchargée.
- Main/archive vérifiées inchangées ; ni promotion main ni release stable. Aucun gain FPS/GPU ni effet visuel garanti par les tests.

## À tester
Dans Camera → Additional post-processing, un seul effet à la fois : Native / 0 / valeur d'essai / retour Native. D'abord chromatique 0/1/5 ; motion blur Amount 0/2 avec maximum 5/20 en mouvement ; ensuite bloom, vignette, grain, AO, SSR, exposition. Comparer vue native/TPS, transitions et retour Native. Les cubemaps/LUT/dirt demandent un asset natif présent ; une absence d'effet doit être signalée, pas assimilée à une réussite.

Installer manuellement en gardant l'INI, ou updater debug. Rollback : conserver l'INI et remettre les deux binaires précédents ; V1.175 conserve son erreur chromatique. [Preuves](evidence/V1_176_DELIVERY_CHECK.json).
