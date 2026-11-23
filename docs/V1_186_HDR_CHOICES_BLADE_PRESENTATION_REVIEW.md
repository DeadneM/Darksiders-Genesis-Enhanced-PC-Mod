# V1.186 — choix HDR et présentation des lames

Candidat debug cumulatif. Base d'essai V1.184, avec le code V1.185 déjà contrôlé en CI pour la résolution interne, les reflets en direct et les lames tenues pendant la mêlée montée. Le retour sur les couleurs et les lames rangées concerne V1.184.

## Sortie HDR

Le journal V1.184 associe la capture du 23 novembre à scRGB 1000, au buffer RGBA16F, à un signal DXGI accepté et au chemin HDR du moteur passé à 1. Le fichier JXR de cette capture contient des hautes lumières d'environ 670 nits dans les flammes (échelle scRGB 1 = 80 nits). Le réglage du rendu à l'écran revient au joueur. Le chemin scRGB de V1.184 reste identique et aucune nouvelle conversion de couleurs n'est ajoutée.

Les profils HDR10 et scRGB à 1000 et 2000 nits sont de nouveau sélectionnables indépendamment, avec choix du buffer, luminosité du HUD, signalisation DXGI et métadonnées HDR10. Les anciens champs de métadonnées sont relus. Native reste la valeur des nouveaux profils. Un choix de format explicite incompatible est conservé dans le menu et laisse le jeu reprendre ses réglages d'origine au redémarrage. Le jeu reste propriétaire de sa chaîne de rendu. Le mod vérifie le buffer réellement alloué, le profil moteur et le support DXGI. HDR10 exige un RGB10A2 effectif, encore à démontrer sur ce moteur DX11. RGBA16F/scRGB est le couple observé sur le PC du joueur. Les métadonnées HDR10 apparaissent seulement dans les profils concernés et sont envoyées après acceptation du signal PQ.

[Advanced Color et scRGB](https://learn.microsoft.com/en-us/windows/win32/direct3darticles/high-dynamic-range) · [Métadonnées HDR10 DXGI](https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_5/ns-dxgi1_5-dxgi_hdr_metadata_hdr10)

## Lames de Strife

Les sockets Base et Tip des deux modèles donnent des axes locaux opposés. Les squelettes à pied et montés partagent la base des attaches de holster. Pour chaque copie rangée, la rotation locale place la pointe vers le bas et la face vers l'extérieur dans la pose de référence. Les lames tenues restent attachées aux os natifs. Sur Despair et Mayhem, profondeur personnalisée et stencil sont repris du composant natif du même côté lors de la création de la copie. Le correctif d'occlusion reste à valider visuellement. Les composants de combat, les effets de mêlée et le placement des pistolets gardent leur fonctionnement.

## Essai

- Avec **Show Strife blades when sheathed**, vérifier le sens des deux lames, puis une lame OFF à la fois. Répéter à pied et sur les deux montures, y compris devant des murs et pendant montée/descente. Les lames rangées doivent suivre les holsters et l'occlusion du cavalier. Vérifier la lame tenue pendant chaque attaque à cheval.
- En **scRGB 1000 / Match HDR profile**, relancer puis rafraîchir la section HDR. Vérifier le buffer flottant, le signal accepté et les hautes lumières dans la scène. Si le jeu reste en RGBA16F avec HDR10 sélectionné, le menu doit signaler que le buffer ne convient pas à HDR10. Pour rendre les valeurs Engine.ini au jeu, sélectionner **Native**, puis fermer le jeu.
- Tester la résolution interne et les reflets dans une même mission, puis revenir à Native. Voir le [candidat V1.185](V1_185_LIVE_QUALITY_MOUNTED_BLADES_REVIEW.md).

Réglages et raccourcis personnels conservés. Le ZIP debug contient les deux binaires à la racine. Statuts de contrôle et d'essai indiqués dans le reçu de livraison.
