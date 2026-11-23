# V1.188 — livraison debug

**Construite et contrôlée.** Compilation Windows x64, 86 tests passés, zéro avertissement C++. Les contrôles couvrent les quatre modes de lames pour les deux montages, avec vérification de la pose commune. Les signatures des deux accès au format HDR sont vérifiées dans l'exécutable pris en charge. La sortie HDR et le rendu des lames attendent l'essai joueur. [Contrôle GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45272917971).

- Le code compilé correspond à `367e7d06b6a4c3ccaa4ed1747ffa581ca2a1a118`.
- Le ZIP est publié dans `514f411e3661ec3e2ff6e28c42f134962da107a1`. Son empreinte SHA-256 est `CCCEC5FB8A177EB038DB7520AD50AD9A20BA6DDAB83579C973C97386E64164E7`. [Télécharger V1.188](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/514f411e3661ec3e2ff6e28c42f134962da107a1/downloads/DarksidersGenesis_V1.188_VIEWPORT_HDR10_STRIFE_BLADES_TEST.zip).
- L'archive contient `DarksidersGenesisMod.asi` et `dxgi.dll` à la racine. Intégrité ZIP, format PE x64 et lien direct vérifiés.
- La base d'essai V1.187, les 251 anciennes archives et les 296 préférences de référence sont conservées. Main reste en V1.173.

Vérifier après relance le buffer 10 bits, la présentation DXGI flip et la signalisation acceptée avec HDR10 1000. Vérifier la pose contre les hanches à pied et à cheval, puis chaque choix de lame durant les attaques sur Mayhem et Despair. [Périmètre et essai](V1_188_VIEWPORT_HDR10_STRIFE_BLADES_REVIEW.md).
