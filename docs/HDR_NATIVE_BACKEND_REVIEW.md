> Mise à jour V1.180 : observations historiques du code natif, pas une interdiction pour Intel ni pour DX11. Les mêmes profils HDR expérimentaux sont désormais proposés pour toutes les marques ; résultat à tester. Voir V1_180_REVIEW.md.

# HDR natif — vérification hors jeu, 22 novembre 2026

Proposition initiale : Native / HDR expérimental au redémarrage, sortie réelle à vérifier. Les CVars r.AllowHDR, r.HDR.EnableHDROutput, r.HDR.Display.OutputDevice, r.HDR.Display.ColorGamut et les contrôles UI sont présents avec références natives. Leur seule présence ne démontre pas le support sur toutes les cartes.

Dans le chemin D3D11 étudié (RVA D48980), les fonctions 12BCCA0/12BCC80/12BCC90 comparent le même identifiant GPU à 10DE (NVIDIA), 1002 (AMD) et 8086 (Intel). Les branches NVIDIA/AMD appellent respectivement D61640 et D61470. La branche Intel à D48AE4 conduit au message « There is no HDR output implementation currently available for this hardware. » référencé à D48B02 depuis RVA 2902B30. Ce constat concerne ce backend natif, pas les capacités physiques de la carte ou de l'écran. Le code contient aussi un autre chemin DXGI/D3D12 ; il ne prouve pas que notre chemin D3D11 ou tous les hooks du mod fonctionnent sous D3D12.

Conséquence pour l'Intel Arc utilisée précédemment : pas de case de forçage HDR ajoutée. Éviter un tonemapping HDR sans transport correct, qui ne constituerait pas une sortie HDR fonctionnelle. Native reste intact. D3D12, injection/conversion externe et recréation forcée de swapchain hors périmètre de cette réparation. Un travail HDR futur doit vérifier GPU/backend/écran et sortie réelle avant d'être exposé.

Documentation Epic UE4.27 : https://dev.epicgames.com/documentation/en-us/unreal-engine/high-dynamic-range-display-output?application_version=4.27 (contrainte plein écran exclusif D3D11 et profils 1000/2000 nits). Ne pas confondre ces possibilités génériques avec leur implémentation dans ce jeu. Aucun essai HDR ni changement Windows/Engine.ini effectué.

L'ancien blocage de lecture par quota a disparu au nouvel essai autorisé ; aucun contrôle n'a été contourné. Sources, PAK et EXE lus uniquement, sans lecture mémoire du processus ni appel externe au moteur.
