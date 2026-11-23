# V1.183 — scRGB sur le buffer natif DX11

Cette build reprend V1.182 sur debug et aligne l’encodage du moteur sur le format HDR du jeu. L’essai passe si le menu affiche un buffer 16 bits flottant, HDR ON, sortie 5 ou 6 et gamut 0, avec signalisation DXGI acceptée. Vérifier ensuite couleurs, hautes lumières et HUD en mission.

## Comparaison V1.182 sur Intel Arc B580

- À 11 h 27, le démarrage HDR10 demande la sortie 3, gamut 2. Le jeu présente un buffer DXGI de format 10 (RGBA16F). Le contrôle refuse l’association avec l’encodage PQ prévu pour RGB10 (`0x887A0004`).
- À 11 h 30, scRGB 1000 démarre avec la sortie 5, gamut 0, sur le même format RGBA16F. Le journal indique `matched=1 signaling=1 result=00000000`, concordant avec la capture de 11 h 31. Le rendu perçu reste à comparer en jeu.
- L’exécutable DX11 de détail fournit explicitement le format HDR au viewport (`0x145D23B` et `0x1459464`). Le format global à `0x3677F38` vaut `PF_FloatRGBA`. Le chemin D3D11 consulte `r.DefaultBackBufferPixelFormat` quand aucun format explicite ne lui est fourni. Forcer la valeur 4 dans Engine.ini laisse donc le buffer HDR flottant en place. [Formats HDR DXGI](https://learn.microsoft.com/en-us/windows/win32/direct3darticles/high-dynamic-range).

## Changement

L’ASI au démarrage et le helper de fermeture interprètent les anciens modes HDR10 1 et 2 comme scRGB 1000 et 2000. Le nouveau menu présente les quatre choix effectifs Native, SDR, scRGB 1000 et scRGB 2000. Les réglages du HUD, le choix du buffer natif et la case de signalisation sont préservés. Le helper conserve son mécanisme de restauration sur Native. Le premier cycle de fermeture peut encore être nécessaire pour appliquer les CVars persistantes.

La signalisation garde la vérification des valeurs moteur, du buffer et du support DXGI. Le chemin des métadonnées PQ et ses contrôles sont retirés de la version actuelle. Les anciennes clés de métadonnées restent dans les INI existants, où une version antérieure peut les relire. Le format de la swapchain reste choisi par le jeu, avec le même chemin pour les différentes marques de GPU. [Profils HDR UE4](https://dev.epicgames.com/documentation/en-us/unreal-engine/high-dynamic-range-display-output?application_version=4.27).

## Essai

Garder Windows HDR actif. Après relance, ouvrir **Graphics → HDR**, choisir **Refresh HDR availability** et relever le buffer, la réponse DXGI et les valeurs moteur. Pour scRGB 1000, attendre 16 bits flottant, sortie 5, gamut 0 et réponse `accepted`. Comparer les couleurs, la lisibilité du HUD et les hautes lumières en mission avec SDR. Un changement de profil s’applique après fermeture et relance. Pour rétablir les CVars d’origine, choisir **Native** avant de fermer le jeu.

Les correctifs V1.182 des lames rangées et des transitions des pistolets restent dans le lot cumulatif. Leur validation est distincte de l’essai HDR. La mêlée à cheval et les contrôles de rendu en direct restent dans la file.
