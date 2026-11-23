# V1.189 — Alt+Tab en HDR10, interface HDR et lames

Base fonctionnelle V1.188. Le joueur a validé le rendu HDR10 en jeu et le placement des lames à pied et à cheval. Les menus lui paraissent délavés et Alt+Tab fait planter le jeu.

## Alt+Tab

Les trois derniers rapports V1.188 donnent `DXGI_ERROR_INVALID_CALL` dans `D3D11Viewport.cpp:361`. L'adresse de la pile du jeu correspond au contrôle du retour de `Present`. Le moteur peut basculer le mode exclusif pendant sa mise à jour du focus, hors de la séquence de redimensionnement qui libère les anciennes cibles. Le correctif V1.188 couvrait les transitions explicites de cette séquence seulement. [Règles de transition DXGI](https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-setfullscreenstate).

V1.189 demande une fenêtre sans bordure lorsque le plein écran exclusif est choisi pour le HDR10 géré par le mod. Le viewport et la chaîne DXGI sont créés en mode fenêtré et le redimensionnement natif conserve ce mode pour la chaîne flip du mod. Les choix fenêtrés gardent leur sens. Le rendu PQ, la chaîne 10 bits et les choix HDR précédents servent de base.

## Réglages d'interface

- **HDR interface composition**. **Auto** suit le moteur et sélectionne **HDR blend** lorsque la luminosité est personnalisée. **Standard** utilise la composition habituelle. **HDR blend** compose la couche SDR des menus et du HUD pour la sortie HDR. Le choix s'applique après relance.
- **Interface brightness**. Intensité de cette couche avec **HDR blend**. Le curseur agit en direct et `1.0` correspond au niveau de référence du moteur. La case décochée rend la main au moteur. La scène 3D garde la luminosité du profil HDR sélectionné.
- Les profils **1000/2000 nits** choisissent la courbe du jeu. Les métadonnées HDR10 décrivent le contenu à l'écran.

La luminosité s'applique aux deux variantes natives de composition Slate, aux appels qui transmettent le niveau de la couche d'interface. Les signatures des appels et la copie de cette valeur ont été contrôlées dans l'exécutable pris en charge. La clé `UIBrightness` est conservée et le helper restitue l'ancienne valeur d'Engine.ini si elle appartenait encore au mod. `UIComposition` vaut Auto par défaut. [Réglages HDR d'Unreal](https://dev.epicgames.com/documentation/en-us/unreal-engine/high-dynamic-range-display-output?application_version=4.27).

## Lames et montée

La rotation de rangement ajoute un demi-tour sur l'axe longitudinal local de chaque lame. Les deux positions et l'orientation longitudinale validées servent à pied et à cheval. La pose en main reprend les attaches existantes.

Le journal montre un appel en l'air à 23:08:51 suivi de la prise d'appui dans le vide. Le saut monté précédent est distinct de la montée. Les chemins d'invocation et de saut sont identiques à V1.188. La durée perçue de la montée au départ du sol reste à reproduire.

## Critères d'essai et retour arrière

Avec le même profil HDR10, passer plusieurs fois du jeu au bureau, y compris depuis un menu, puis vérifier un signal accepté, le buffer 10 bits et l'image en mission. En **HDR blend**, modifier **Interface brightness**, puis comparer le HUD et un menu du jeu dans la même scène. Comparer **Standard** et **HDR blend** après relance si besoin. Contrôler le tranchant vers le bas à pied, sur Mayhem et sur Despair. Vérifier la montée depuis le sol, puis l'appel en l'air séparément.

[V1.188 reste disponible comme retour arrière](V1_188_DELIVERY_RECEIPT.md), avec son défaut Alt+Tab connu.
