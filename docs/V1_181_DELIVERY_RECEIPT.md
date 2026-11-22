# V1.181 — livraison debug contrôlée

Base V1.180, source `90a1a51b238c3db38a2d48d9f8afb7f26448518a`, archive `ba11fdc977b49a236dc2205a20c5dc746f50c2e6`. [ZIP direct vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ba11fdc977b49a236dc2205a20c5dc746f50c2e6/downloads/DarksidersGenesis_V1.181_MOUNTED_PRESENTATION_HDR_TRANSPORT_TEST.zip) — 2543342 octets, SHA256 `D33917D2881F58BF1C4E36E3FBB2275CCF0DB4190C8CCC12B4B04B9A037BA37F`.

CONSTRUITE / CONTRÔLÉE : Windows x64 Release via [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45078359677), 86/86 tests, zéro avertissement/erreur C++. ZIP de deux binaires racine, PE AMD64, CRC/identité et HTTP200 vérifiés. Defender ZIP et charge utile : aucune menace détectée, protection inchangée. 245 anciennes archives préservées ; 288 anciennes valeurs par défaut identiques, huit préférences nouvelles (296 total). Aucun fichier de l'installation ni INI personnel remplacé.

[Périmètre et limites](V1_181_REVIEW.md). Correspondance croisée des holsters et deux classes de cheval, masque des holsters des deux maillages montés, case Show Strife blades when sheathed près du choix de masquage. Copies purement décoratives, collision OFF, armes/traces natives intactes. Nouveau transport HDR avec buffer concordant et contrôle DXGI, métadonnées HDR10 auto/personnalisées. Le rendu et le placement restent EXPÉRIMENTAUX.

Le premier contrôle complet de la candidate a réussi 85/86, mais détecté un tableau de bornes incomplet dans l'updater ; corrigé avant le résultat final 86/86. Aucune archive issue de cet échec n'a été publiée. Deux compilations préparatoires arrêtées avant livraison lors du renforcement des garde-fous ; ces tentatives ne sont pas annoncées comme des validations.

## Test groupé
1. Pistolet droit seul puis gauche seul : comparer rangement à pied et sur les deux skins du cheval, repos/tir/pause/roue. Le holster appartient à la main opposée ; vérifier l'autre holster masqué.
2. Case des lames rangées ON : contrôler placement et côtés Native/gauche OFF/droite OFF/deux OFF, combat puis retour au repos et montée/descente. OFF doit retirer les copies. La visibilité des lames natives tenues pendant l'attaque montée n'est PAS annoncée corrigée.
3. HDR10 1000 : Match HDR profile, signaling ON, métadonnées auto. Lancer/fermer puis relancer pour laisser appliquer le nouveau buffer après l'installation. Vérifier 10 bits et signaling accepté, couleurs, hautes lumières, menus. Comparer scRGB 1000 avec le même cycle (float16), puis Native pour restaurer. Métadonnées personnalisées = informations transmises à l'écran, pas recalibrage de la courbe du jeu. Rendu HDR réel non validé.
4. TAA/effets d'image/post-traitement/DOF restent en direct. Rendering quality et sortie HDR gardent pour l'instant le redémarrage ; étude des setters sûrs à chaud ouverte.

Retour arrière : lames rangées OFF ; HDR Native puis fermeture complète pour restituer les clés Engine.ini possédées avant un downgrade. Main/archive non promues. Suite : retours visuels, lames tenues montées et faisabilité des sept réglages de qualité en direct.
