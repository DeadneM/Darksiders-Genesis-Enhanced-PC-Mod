# V1.161 — première activation Stoneskin, reçu de livraison

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ sur debug ; EXPÉRIMENTAL en jeu pour ce correctif.** Base V1.160 `7c429b778266e41bb5ebd2324e7bc605685aae2f` ; source `f43d3670d52aeee4fe7c72a0af2625332c96c11f` ; archive `050ff8edf221d4bf21763e7da0eb071adba12078`. Saut et raccourci V1.160 acceptés par l'utilisateur ; coop déjà acceptée. Main reste V1.159.

Le premier cast peut être observé avant que son statut natif ne soit publié. Le ticket visuel n'est plus immédiatement effacé dans ce cas : attente d'une seconde au maximum, sans créer de statut ni relancer le pouvoir. L'expiration après un statut observé reste immédiate. Propriétaire, monde/forme perdus et délai dépassé annulent le ticket. La reprise bornée des créations refusées de V1.160 est conservée. Même effet, matériaux, geste, cendres, aura et dégâts ; aucun nouveau hook/API.

Les trois seuls fichiers modifiés dans les sources/tests/configuration depuis V1.160 sont BuildInfo.h, StoneSkinVisual.cpp et son test. Saut Mayhem/Rampage, D-pad bas et INI/raccourcis inchangés.

GitHub Windows x64 : **82/82 suites réussies**, aucun avertissement C++ détecté. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43038912530). Nouvelles régressions : première liste vide/illisible puis statut, Molten, timeout, interruption d'observation, monde/forme perdus. Les avertissements externes d'outillage sont consignés ; pas de build local.

[ZIP V1.161](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/050ff8edf221d4bf21763e7da0eb071adba12078/downloads/DarksidersGenesis_V1.161_STONESKIN_FIRST_STATUS_TEST.zip) — 2431726 octets, SHA256 `3C14F3387266630798FC86FDEA87873E8647CA8C29856CC4830AFB23C5EB67F3`. Deux binaires AMD64 à la racine, CRC vérifiés, aucun INI/README. 225 anciennes archives conservées, 248 valeurs et 55 actions Hotkeys inchangées. Disponibilité HEAD 200 et version debug la plus élevée vérifiées. Le nouveau ZIP n'est récupéré qu'une fois par Git ; anciens fichiers réutilisés localement. Defender : aucune menace, protections inchangées.

**Test ciblé : après lancement ou chargement, vérifier la première explosion Stoneskin sur Strife, puis deux activations suivantes.** Pas de retest général du saut/raccourci/coop demandé. Le lien entre publication tardive et défaut visuel reste expérimental jusqu'au retour en jeu. Un composant créé mais invisible peut avoir une autre cause.

Aucune installation ni relance du jeu effectuée. Garder son INI, utiliser l'updater debug ou le ZIP. Rollback V1.160 déjà disponible localement. [Analyse du journal et périmètre](V1_161_REVIEW.md) · [CI](evidence/V1_161_CI_CHECK.json) · [Paquet](evidence/V1_161_PACKAGE_CHECK.json) · [Defender](evidence/V1_161_DEFENDER_CHECK.json).
