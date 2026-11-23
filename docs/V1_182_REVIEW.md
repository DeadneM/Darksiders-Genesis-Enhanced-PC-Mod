# V1.182 — démarrage HDR, transitions montées et lames rangées

Base cumulative V1.181 sur `debug`. Le 23 novembre, le joueur a confirmé la correspondance des holsters à cheval. Il a vu un pistolet sous Strife pendant la montée ou la descente. La case des lames rangées s’enregistrait sans effet. Le rendu HDR10 restait délavé.

## HDR

La capture affiche HDR Windows, un profil HDR10, un buffer 16 bits flottant et le refus DXGI 0x887A0004. Le journal de l’exécutable UE 4.22.2 confirme deux refus supplémentaires. `r.HDR.Display.OutputDevice` et `r.HDR.Display.ColorGamut` sont restés à 0 car le profil Windows a priorité sur les deux sections d’Engine.ini utilisées pour les CVars. La valeur de luminosité maximale des métadonnées était aussi multipliée par 10 000. DXGI attend des nits entiers pour le maximum et des dix-millièmes de nit pour le minimum. [Structure DXGI HDR10](https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_5/ns-dxgi1_5-dxgi_hdr_metadata_hdr10).

Les profils choisissent automatiquement RGB10 pour PQ et RGBA16F pour scRGB. L’option de conserver le format natif reste disponible. Les anciennes sélections manuelles sont converties en sélection automatique, y compris dans le helper qui relit un ancien INI. Le chemin après fermeture et la restauration des valeurs possédées restent les mêmes.

Pour un choix explicite, le moteur reçoit les deux CVars de couleur avec son propre setter d’entiers `SetByCode`, juste après la lecture des CVars de démarrage. Si l’ASI arrive trop tard, le premier suivi du joueur sur le thread du jeu fournit une tentative de secours. Le code confirme que le moteur a chargé l’état HDR demandé et respecte les priorités supérieures. La valeur consommée par le rendu est relue avant la demande DXGI. Le menu indique désormais les valeurs du moteur et le résultat de la signalisation. DXGI est sollicité pour une paire format/encodage concordante, avec au plus une reprise si le profil moteur est appliqué tard. [Advanced Color sous DirectX](https://learn.microsoft.com/en-us/windows/win32/direct3darticles/high-dynamic-range).

Vérification statique sur l’exécutable pris en charge. La lecture ConsoleVariables utilise la priorité SystemSettingsIni 0x04000000. Le profil de périphérique utilise 0x05000000. Le setter natif à 0x81E5F0 propage les données d’entiers du thread de jeu au rendu et accepte 0x08000000 comme SetByCode. Les slots et la table de fonctions doivent correspondre à cette version. Aucune conclusion visuelle en jeu à ce stade.

## Pistolets et lames

Le code pouvait restaurer la visibilité d’un pistolet OFF immédiatement après la perte du propriétaire courant, alors que le jeu montrait encore la transition. La restauration attend maintenant le même composant, rattaché à un corps actif. Un changement de visibilité par le jeu durant la transition reprend la main. Le profil croisé des holsters validé en V1.181 est conservé. Le symptôme visuel reste à comparer en jeu.

Le modèle de pistolet retenu auparavant appartient au script de construction de Strife. Il est absent du tableau des modèles consulté par AddComponent au lancement d’un ajout dynamique. La nouvelle copie décorative est un StaticMeshComponent transitoire sans modèle de départ. Le code désactive sa collision, assigne le modèle de la lame et prépare son attache croisée avant l’enregistrement auprès du moteur. La lame de combat d’origine garde ses propriétés. Le journal indique le succès ou l’étape qui a empêché l’apparition. Une seule tentative par lame et par monture, corps ou squelette actif. L’équipement de la lame en combat à cheval reste un sujet ouvert.

## Critères d’essai

- Effectuer plusieurs montées et descentes avec le réglage de pistolets utilisé quand le défaut est apparu, puis comparer les deux chevaux. Vérifier la réapparition d’un pistolet après passage à Native.
- Cocher **Show Strife blades when sheathed**. Contrôler les côtés au repos à pied et à cheval, puis en mêlée à pied avec un côté masqué. Décocher pour retirer les copies.
- Laisser **Match HDR profile** et **Use DXGI HDR signaling**, choisir HDR10 1000 et laisser le premier cycle de fermeture appliquer le format. Après relance, attendre 10 bits, sortie moteur 3, gamut 2, HDR ON et signalisation acceptée. En scRGB 1000, attendre 16 bits flottant, sortie moteur 5 et gamut 0. Comparer les couleurs, les hautes lumières et la lisibilité du menu.
- Avant un retour à une ancienne version, choisir HDR Native et fermer le jeu pour que le helper rende les valeurs précédentes.

Les trois correctifs sont expérimentaux. La compilation et le contrôle du ZIP sont suivis séparément de la validation visuelle. Les réglages de qualité encore appliqués au redémarrage restent à étudier en direct.
