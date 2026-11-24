# V1.190 — sortie et interface HDR

La V1.189 est la base cumulative. Le joueur a validé les lames rangées et la montée à cheval. Le journal de son essai confirme le signal HDR10 en jeu. La correction de focus V1.189 modifiait la demande de plein écran avant la négociation du signal.

## Focus DXGI

La demande globale de résolution revient au moteur. La protection suit le viewport DX11 du mod, identifié par l'appel du constructeur et son champ de chaîne de présentation. Le constructeur reçoit le mode d'origine. Le secours DXGI garde son descripteur d'origine. Après acceptation du signal PQ sur un écran HDR, le constructeur conserve le mode fenêtré pour cette chaîne flip 10 bits. Les resizes de cette chaîne gardent son mode. Les signatures du constructeur, du champ de mode et de sa lecture lors d'un changement de focus sont contrôlées dans l'exécutable pris en charge.

## Utilisation

Le panneau HDR affiche le profil demandé et la sortie confirmée. Interface regroupe la composition native et le réglage de luminosité du HUD et des menus du jeu. L'indicateur de redémarrage suit les changements réellement différés, dont le passage Auto vers une composition explicite ou la sélection de métadonnées. Une variation du curseur de luminosité en cours d'utilisation reste directe avec la composition HDR active. Advanced output regroupe le buffer, la signalisation et les métadonnées. Diagnostics affiche le format réel et le gamut du moteur.

Le menu et les notifications du mod utilisent un shader de dessin qui traduit leur palette sRGB vers la sortie HDR effectivement négociée. Le shader encode en PQ avec les primaires BT.2020 pour le HDR10 et convertit en scRGB linéaire en RGBA16F. Le blanc de référence du menu du mod vaut 160 nits. La scène 3D et les menus du jeu suivent leurs chemins de rendu.

## Profils de couleur

L'exécutable expose quatre courbes HDR pour écran, soit HDR10 et scRGB à 1000 ou 2000 nits. Sa fonction de sortie documente aussi Linear EXR pour le rendu de fichiers. Les gamuts déclarés sont Rec.709, P3-D65, BT.2020, ACES et ACEScg. Ils règlent les primaires de sortie du moteur. Pour une présentation gérée par le mod, le contenu et le signal DXGI utilisent ensemble BT.2020/PQ ou Rec.709 linéaire étendu. Le panneau affiche ce couple pour chaque sortie confirmée. [Profils Unreal](https://dev.epicgames.com/documentation/en-us/unreal-engine/high-dynamic-range-display-output?application_version=4.27) · [Formats HDR DXGI](https://learn.microsoft.com/en-us/windows/win32/direct3darticles/high-dynamic-range).

## Essai et retour arrière

Avec la même sélection HDR10, vérifier plusieurs Alt+Tab puis les couleurs du menu du mod. Tester la luminosité des menus du jeu avec HDR blend. Vérifier ensuite qu'un lancement en SDR ou Native reprend le mode vidéo du jeu. Le retour arrière utilise le [ZIP V1.189](V1_189_DELIVERY_RECEIPT.md).
