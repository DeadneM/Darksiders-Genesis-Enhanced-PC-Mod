# V1.138 — reçu de livraison

1er novembre 2026. **CONSTRUITE / CONTRÔLÉE ; EXPÉRIMENTALE, NON VALIDÉE EN JEU.** Base cumulative V1.136, correction candidate de ressources Stoneskin de V1.137, avertissements de nommage éliminés. Aucun correctif Statique annoncé.

Source compilée `ed96bc2d4f857e948753bd04f611fdb7b0dbb3c4` ; commit archive `9f9d97875443f29d806c059cec158a1e47e953cd`. [GitHub Actions : 70/70, zéro avertissement/erreur MSVC](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41297934297). Les notices externes CMake/MinHook et Actions/Node sont séparées des diagnostics MSVC.

[ZIP cumulatif, quatre fichiers racine](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9f9d97875443f29d806c059cec158a1e47e953cd/downloads/DarksidersGenesis_V1.138_STONESKIN_ASSET_RECOVERY_TEST.zip) — 2428651 octets, SHA256 `C931955ADE379F790F91ACEFC3877EA56B3A92A709C8EE324DEA263E788781BF`. ASI et DXGI AMD64, CRC et contenu relus ; compression stockage. 202 archives précédentes, dont V1.137, conservées en Git ; 203 au total. 228 clés INI et 55 ID Hotkeys inchangés. main/archive/tag v1.43 vérifiés intacts.

Le listing utilisé par l'updater debug choisit ce ZIP, téléchargé HTTP 200 avec les mêmes octets ; l'écran de l'updater en jeu n'est pas observé. Defender : ZIP et quatre fichiers extraits sans menace, protections conservées.

**Non installée par l'agent.** L'ASI locale reste V1.136, empreinte relue ; aucun INI personnel, raccourci, sauvegarde ou PAK remplacé. Aucun lancement/capture mémoire dans cette intervention. Le relevé natif/TPS analysé date de la session précédente.

À tester après installation via l'updater debug ou manuellement en gardant l'INI : peau de pierre/feu de Strife après chargement et chez Dagan ; premier lancement puis suivants, disparition complète, explosion/cendres ; contrôle court Geyser/Rampage et Charge/Lava. La cause du défaut Dagan n'est pas confirmée, les overrides inconnus restent protégés. Statique reste ouverte : le phénomène a aussi été observé en vue native avec ASI ; aucune référence sans ASI n'est déclarée testée.

La livraison ne modifie ni gestes/timings des capacités, ni shaders validés, statistiques, caméra/input, modèles/holsters ou raccourcis. Le nettoyage trop large du résolveur reste annulé ; seuls le test fictif du pistolet et les compteurs sont corrigés. Les détails et limites figurent dans [la note de version](V1_138_STONESKIN_ASSET_RECOVERY_TEST.md).

Preuves : [CI](evidence/V1_138_CI_CHECK.json), [package](evidence/V1_138_PACKAGE_CHECK.json), [updater](evidence/V1_138_UPDATER_CHECK.json), [Defender](evidence/V1_138_DEFENDER_CHECK.json), [installation inchangée](evidence/V1_138_NO_INSTALL_CHECK.json).
