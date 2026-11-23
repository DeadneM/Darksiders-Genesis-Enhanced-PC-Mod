# V1.187 — livraison debug

**Construite et contrôlée.** Compilation Windows x64, 86 tests passés, zéro avertissement C++. Les essais automatiques couvrent la demande HDR10, le repli d'encodage, les holsters à pied et la transition de la lame du bon côté sur monture. La sortie HDR10 sur le PC du joueur et le rendu des lames en jeu restent expérimentaux. [Contrôle GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45234878240).

- Le code compilé correspond à `45300758806c8f0bb8a039049a851102d1c5cd77`.
- Le ZIP est publié dans `f1cda7519ae928b481aecda05e0e68c797c40235`. Son empreinte SHA-256 est `7BF73FC34E9ECEFF05D198F39998406F1526C5564630784182686B571E9B254B`. [Télécharger V1.187](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f1cda7519ae928b481aecda05e0e68c797c40235/downloads/DarksidersGenesis_V1.187_NATIVE_HDR10_BLADES_TEST.zip).
- L'archive contient `DarksidersGenesisMod.asi` et `dxgi.dll` à la racine. Intégrité ZIP, format PE x64 et lien direct vérifiés.
- La base d'essai V1.186, les 250 anciennes archives et les 296 préférences de référence sont conservées. Main reste en V1.173.

Vérifier après relance le buffer 10 bits, la présentation DXGI flip et la signalisation acceptée avec HDR10 1000. Vérifier les deux côtés de l'attaque sur Mayhem et Despair, puis l'inclinaison des lames rangées à pied. [Périmètre et retour Native](V1_187_NATIVE_HDR10_MOVING_BLADES_REVIEW.md).
