# V1.179 — onglet Graphics et état HDR

Base : debug V1.178 db9fea4, effets de mêlée à pied/cheval validés par retour utilisateur. V1.177 et ses réglages retenus. Main V1.173 conservée.

## Déplacement demandé
Nouvel onglet Graphics après Camera. Noms et contrôles conservés : Temporal anti-aliasing and sharpness ; Image effects ; Rendering quality (restart required) ; Additional post-processing ; Depth of field. Le réglage TPS character focus est également regroupé avec la profondeur de champ, avec son ancienne condition TPS actif. Caméra TPS/native, angles, distances, FOV, visée et raccourcis restent dans Camera. Barre d'onglets défilante pour conserver les titres avec une grande police.

Aucune clé, valeur, limite, défaut, migration ou méthode d'application changée. Même sauvegarde et publication runtime ; les sept réglages Rendering quality exigent toujours fermeture/relance. Les quatre premiers corps de groupes ont été comparés avant/après, hors espaces/EOL ; source de focus conservée. ConfigStore, RuntimeSettings, effets de mêlée, visibilité des armes et moteur de rendu V1.178 intacts.

## HDR : état vérifiable, pas activation ajoutée
Utilisateur : écran HDR, HDR Windows activé. C'est compatible avec la capacité matérielle, mais ne fournit pas à lui seul le chemin HDR natif manquant sur Intel dans ce jeu D3D11. Voir HDR_NATIVE_BACKEND_REVIEW.md et evidence/HDR_NATIVE_VENDOR_CHECK.json. Ne pas annoncer la présence d'une option fonctionnelle d'activation HDR dans ce build.

Sous-menu HDR : contrôle en lecture seule, lors de l'ouverture ou via Refresh HDR availability. GPU issu du device D3D11 du jeu, et écran contenant sa swapchain. Métadonnées de sortie relues depuis une factory DXGI fraîche, en associant LUID de l'adaptateur réellement utilisé et moniteur courant (pas un index de GPU). Affiche HDR/SDR rapporté par Windows, bits/canal, luminance rapportée, profondeur du buffer du jeu et disponibilité du backend natif. Une absence de l'interface DXGI moderne laisse l'état inconnu sans effet sur le jeu. Objets COM libérés ; noms UTF8 bornés ; aucune interrogation par image avec le groupe fermé. Aucun hook, shader, recréation de swapchain, SetColorSpace/SetHDRMetaData ou écriture Engine.ini/Windows ajouté.

Microsoft : https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_6/nf-dxgi1_6-idxgioutput6-getdesc1 . La metadata EDID peut être absente/par défaut ; afficher un état rapporté, pas une preuve absolue. HDR Windows et format du buffer ne prouvent pas la couleur/metadata HDR du jeu. Une activation HDR réelle sur Intel reste un travail distinct de transport/conversion et de vérification du rendu, pas un curseur de tonemapping forcé.

## Retour sur les lames montées
Effets V1.178 validés. Clarification utilisateur : même avec le choix de lames Native, elles sont déjà invisibles pendant la mêlée montée. Pas de preuve de défaut spécifique au masquage d'une seule lame ; pas d'affirmation de vanilla sans ASI testée. Aucun patch de visibilité improvisé. Futur souhait conservé : rendre les lames tenues/rangées visibles comme les pistolets dans leurs holsters ; attachements/modèles à étudier séparément.

## Contrôle et essai
PRÉPARÉ / EXPÉRIMENTAL avant CI GitHub. Aucun build local, capture mémoire, installation ou téléchargement d'ancien fichier. Tests existants à exécuter ; changement de présentation principalement réversible, pas de test qui duplique la mise en page.

Essai : ouvrir Graphics, vérifier les cinq groupes et les valeurs conservées ; modifier puis rétablir une valeur simple. Vérifier que Camera garde ses contrôles. Ouvrir HDR : GPU attendu, écran, Windows HDR rapporté ; Refresh sans changement d'image ou de réglages. Vérifier l'accès aux onglets avec la police habituelle et agrandie. Aucun test d'activation HDR demandé puisque celle-ci n'est pas incluse. Les modèles de lames montées sont laissés tels quels ; effets validés à conserver.

Rollback : remettre V1.178, INI conservé (aucune nouvelle clé). ZIP deux binaires à la racine, sans INI/README.
