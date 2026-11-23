# V1.184 — rétablir le chemin de rendu HDR du moteur

Base cumulative V1.183 sur debug. Le 23 novembre, le joueur a rejeté le rendu V1.183. Le journal de 12 h 05 confirme la demande scRGB 1000, moteur 1/5/0, buffer RGBA16F et signalisation DXGI acceptée. L’objectif est que la scène 3D tire réellement parti de la plage de luminance HDR, avec des hautes lumières distinctes et un rendu lisible.

## Cause côté moteur

Le constructeur du périphérique DX11 appelle le détecteur natif à `0xD62120` et enregistre son résultat à `0x3A11395` (`0xD4E3EF`). Le détecteur suit les anciens chemins NVIDIA/AMD. Le chemin Intel à `0xD625B9` poursuit l’énumération sans signaler la prise en charge. La passe de post-traitement vérifie cette capacité à `0xF8F4FA` avant de lire l’état HDR demandé. Le choix du format flottant du viewport et le succès de `SetColorSpace1` peuvent donc coexister avec ce chemin de rendu désactivé.

Le réglage cible cette capacité sur l’exécutable exact. La signature de l’écriture native et celle de sa lecture en post-traitement doivent correspondre. La mise à jour atomique à 1 est autorisée après concordance du profil scRGB, du buffer RGBA16F et du signal DXGI, sur un périphérique D3D11 dont la sortie annonce HDR actif. La prise en charge est déterminée par l’écran utilisé. La vérification est faite à la première présentation et lors d’un changement de buffer. Les profils Native et SDR conservent le chemin de démarrage d’origine.

## Essai et limite

Recharger une mission en scRGB 1000. Rafraîchir le statut, puis fermer le menu et comparer couleurs et hautes lumières dans la scène. Les sorties attendues sont `HDR signaling: accepted` et `Native HDR render path: enabled`. Le journal indique le passage de la capacité native de 0 à 1 et le mode de présentation DXGI réellement utilisé.

Le rendu à l’écran reste expérimental. La chaîne DX11 native est conservée. La conversion vers le modèle flip, si elle devient nécessaire, doit couvrir la création, le nombre de buffers et le rétablissement du rendu après les transitions plein écran. Le chemin actuel redimensionne avant le basculement plein écran. Windows demande un redimensionnement après ce basculement pour le modèle flip. [Cycle de présentation DXGI](https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/for-best-performance--use-dxgi-flip-model).

Pour revenir au rendu du jeu, choisir Native et fermer le jeu afin de restaurer les valeurs Engine.ini possédées par le helper. Le reste de la build cumulative est repris de V1.183.
