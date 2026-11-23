# V1.186 — livraison debug

**Construite et contrôlée.** Compilation Windows x64, 86 tests passés, aucun avertissement C++. Les tests couvrent les préférences HDR et les copies rangées à pied et à cheval avec leurs paramètres de profondeur. Le résultat des lames en jeu et la sortie HDR10 restent expérimentaux. [Contrôle GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45225032877).

- Le code compilé correspond à `b6f5c0469ac17a8589be75a83c8743842656e3da`.
- Le ZIP est publié dans `2fdc6191107756e7794c590c5e3262228779c847`. Son empreinte SHA-256 est `C61B020E3E7C59D4BCB25CF4D6FA8D5658E71646F37FA48C554350AF0B8CA56C`. [Télécharger V1.186](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/2fdc6191107756e7794c590c5e3262228779c847/downloads/DarksidersGenesis_V1.186_HDR_CHOICES_BLADES_TEST.zip).
- L'archive contient `DarksidersGenesisMod.asi` et `dxgi.dll` à la racine. Intégrité ZIP, format PE x64 et lien direct vérifiés.
- La base d'essai V1.184 et les ajouts contrôlés en V1.185 sont repris. Les 249 anciennes archives et les 291 préférences précédentes sont conservées. Les cinq clés de métadonnées HDR sont rétablies. Main reste en V1.173.

L'option scRGB de V1.184 a généré la capture HDR du joueur. HDR10 exige que le jeu crée effectivement un buffer 10 bits. Vérifier les lames rangées et tenues sur les deux montures et à pied. [Périmètre, critères et retour Native](V1_186_HDR_CHOICES_BLADE_PRESENTATION_REVIEW.md).
