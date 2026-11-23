# V1.187 — sortie HDR10 et lames en main à cheval

Base cumulative V1.186. Le joueur a validé le placement et l'occlusion des lames au repos à cheval. Le transfert à la main et le rendu HDR10 sont rejetés en V1.186. Les anciens profils et les 296 valeurs de référence sont repris.

## HDR10

Le journal V1.186 du 23 novembre confirme un moteur en PQ (sortie 3, gamut 2), un buffer RGBA16F (DXGI 10), une présentation historique (SwapEffect 0) et un refus 0x887A0004. Le jeu a lu r.DefaultBackBufferPixelFormat=4. L'initialisation et le redimensionnement de FSceneViewport prennent néanmoins le format HDR natif à partir de la valeur moteur initialisée à PF_FloatRGBA (10). Les deux lectures sont à 0x145D248 et 0x1459467. La table DX11 qui mappe PF_A2B10G10R10 (18) vers DXGI RGB10A2 (24) est vérifiée à 0xD3E924.

Pour HDR10 avec signalisation DXGI, le candidat sélectionne le format moteur RGB10A2 avant la création du viewport, sauf si « Keep native buffer » est choisi. Il utilise la création DXGI flip de la seule fenêtre de jeu DX11 prise en charge. La création passe par la fabrique et le périphérique du jeu. Elle conserve dimensions et mode initial, et utilise deux buffers ou plus. Une fois la surface 10 bits et le signal PQ acceptés, le moteur peut sélectionner le profil PQ, puis appliquer les métadonnées. En cas de refus, la création native est reprise et l'encodage reste SDR. La demande INI utilise SDR pendant cette négociation. Native, SDR et scRGB gardent leur fonctionnement. Le choix de calibration reste au joueur.

Le ResizeBuffers natif de 0xD60DD5 demande un seul buffer. La correction réserve deux buffers exclusivement à cette chaîne flip. Lors d'un changement de mode depuis le redimensionnement moteur, le redimensionnement flip est fait après le passage plein écran et avant la reconstruction du backbuffer moteur. Les deux appels reconnus sont à 0xD60E5C et 0xD60E92. La fenêtre active et les transitions plein écran restent à valider en jeu. [Présentation DXGI flip](https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/for-best-performance--use-dxgi-flip-model) · [Formats HDR](https://learn.microsoft.com/en-us/windows/win32/direct3darticles/high-dynamic-range).

## Lames

Les squelettes de référence partagés donnent une échelle unitaire aux os de dagues. Dans les deux maillages réellement utilisés, **Strife_Despair_Mesh** et **Strife_Mayhem_Mesh**, ces mêmes os sont réduits à **0,000099**. Le log V1.186 confirme la création d'une lame tenue pendant un montage. Le candidat fixe la copie sur la main parente à échelle unitaire et reprend la prise native. Il reconnaît les deux attaques montées même si une autre animation est présente dans la liste des montages. Dès que la copie tenue est prête, la copie du même côté se masque au holster. Fin de montage, désactivation et changement de monture restaurent le rendu approprié. Le masquage par main est respecté. Effets de mêlée et dégâts suivent les fonctions existantes.

L'axe local +Y des holsters à pied pointe vers l'arrière, vers le bas et légèrement vers l'extérieur dans la pose de référence. Les pointes des lames suivent cet axe tout en gardant la prise au même emplacement. L'inclinaison des copies rangées à cheval garde celle validée en V1.186.

## Essai

- HDR10 1000, **Match HDR profile**, signalisation DXGI cochée et Windows HDR actif. Fermer puis relancer. Sur une sortie acceptée, attendre **10-bit**, **DXGI flip**, **accepted** et **Native HDR render path: enabled**. Vérifier la lisibilité et la transition plein écran. Pour rendre les valeurs Engine.ini au jeu, sélectionner Native puis fermer le jeu.
- Avec les deux lames visibles et **Show Strife blades when sheathed**, vérifier le rangement à pied. Sur Mayhem et Despair, vérifier que la lame du côté de l'attaque quitte son holster, apparaît en main et revient au repos. Vérifier ensuite une seule lame visible.
- V1.186 reste le ZIP de retour pour les acquis précédents. Le candidat ne remplace aucun fichier du jeu sur le PC pendant la compilation.

V1.187 est expérimentale jusqu'à l'essai joueur.
