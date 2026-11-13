# Correctifs communautaires — étude du 13 novembre 2026

## État et périmètre

V1.167 intégralement validée par l'utilisateur, publiée sur main au commit `6795f478f2a43be490e0b69ed6f3dc71ebdcb95a`, sans build. [Contrôle de promotion](evidence/V1_167_MAIN_PROMOTION_CHECK.json) : sources/ZIP identiques, 232 archives conservées, zéro exécution Actions pour ce commit. La présente étude reste sur debug ; aucun code, réglage du jeu, pilote, chargeur ou binaire modifié. Aucune DLL ou archive communautaire téléchargée et aucune session du jeu lancée.

Cette recherche distingue un correctif de bug, un réglage de qualité et un changement complet de moteur de rendu. Un témoignage de gain n'est pas une mesure sur ce PC, et la présence d'un nom de commande dans l'EXE ne prouve pas son efficacité.

## Résultats applicables

| Piste | Sources et constat | Décision pour notre mod |
| --- | --- | --- |
| Rémanence/flou de l'anticrénelage temporel | Un joueur décrit un essai TAA + netteté sur [Steam](https://steamcommunity.com/app/710920/discussions/0/1754645911764685397/), notamment `r.TemporalAACurrentFrameWeight=0.08` et `r.Tonemapper.Sharpen=2`. Les deux commandes sont présentes dans notre EXE. L'aide native confirme le compromis entre rémanence et scintillement ; elle ne garantit pas ces valeurs pour tous les écrans. | **Candidat simple et compatible à étudier en premier**, sous forme de réglages optionnels avec retour Natif, pas d'un « fix moteur » forcé. Garder le TAA pour comparer les transparences/dithering. Aucun réglage appliqué ici ; ne pas confondre ce flou avec la profondeur de champ TPS déjà corrigée. |
| Textures floues / budget de streaming | [PCGamingWiki](https://www.pcgamingwiki.com/wiki/Darksiders_Genesis) liste un contournement iGPU avec `r.Streaming.LimitPoolSizeToVRAM=0` et pool 512 Mo. Le journal de ce PC montre **déjà LimitPoolSizeToVRAM=0 et pool final 1000 Mo**, avec Arc B580 choisie. | Ne pas copier 512 Mo par défaut : cela réduirait le budget actuel. **Candidat : budget optionnel**, seulement après comparaison des mipmaps, mémoire et temps d'image. 1000 Mo face à 12 Go de VRAM n'est pas une preuve de saturation ni d'un gain FPS possible ; d'autres allocations utilisent cette mémoire. |
| DXVK (D3D11 vers Vulkan) | Des utilisateurs rapportent un gain dans Genesis sur [Steam](https://steamcommunity.com/app/710920/discussions/0/4516632734721906306/). Le [README du projet](https://github.com/doitsujin/dxvk#dll-dependencies) demande `d3d11.dll` **et** `dxgi.dll`. Les auteurs détaillent les [limites Windows](https://github.com/doitsujin/dxvk/wiki/Windows), notamment le mélange DXGI système / D3D11 DXVK qui peut planter. Notre proxy occupe déjà `dxgi.dll` et charge explicitement la DLL système. | **Essai séparé seulement**, après adaptation du chaînage ou du chargeur. Ne pas installer seulement d3d11.dll par-dessus notre proxy. Une partie des interfaces D3D11 peut rester compatible avec le menu, mais aucun essai n'a été fait. Pas de promesse de gain, pas de DLL intégrée au ZIP. |
| Rendu DX12 par `-dx12` | Le même [fil Steam](https://steamcommunity.com/app/710920/discussions/0/4516632734721906306/) rapporte que cette option fonctionne après retrait des DLL DXVK. Le binaire contient D3D12RHI ; cela ne prouve pas un démarrage correct sur notre configuration. Le code de notre menu, des hooks et des filtres de shaders est D3D11. | **Pas compatible directement avec toute la V1.167.** Ce serait un chantier de backend et de validation des corrections de rendu, pas un réglage à activer en douce. Ne pas cumuler DX12 et un paquet DXVK D3D11 arbitraire. |
| Helix / UE4-Universal Fix 2 | L'auteur publie un vrai correctif [spécifique Genesis](https://helixmod.blogspot.com/2020/01/darksiders-genesis.html), avec autoconvergence et profondeur du curseur, pour la communauté 3D Vision. | Usage **stéréoscopique**, différent des objectifs actuels du mod. Pas de reprise de shaders/hashes sans défaut correspondant, examen de compatibilité et permissions. Aucun intérêt démontré pour remplacer nos corrections TPS validées. |

## Pistes non retenues comme patch immédiat

- **Presets « UE5 stutter fix » :** les noms `r.PSOPrecaching` et `r.Nanite` ne sont pas trouvés dans cet EXE UE4.22.2. Les commandes `r.ShaderPipelineCache.*`, `r.CreateShadersOnLoad` et plusieurs contrôles de threading existent, mais cela ne démontre pas une accélération sur D3D11. Aucun `.upipelinecache` ou `.ushaderprecache` trouvé dans l'index du PAK principal examiné ; ce constat ne couvre pas un éventuel cache de pilote ou d'autres conteneurs. Ne pas annoncer une précompilation universelle sans cache compatible et traces de saccades.
- **Crash Moloch :** défaut communautaire déjà connu, notre garde est validée et reste ON par défaut. Pas de second patch empilé sur le contournement existant.
- **Réparations génériques OpenSSL / CPU Intel :** aucun défaut de démarrage correspondant démontré ; le journal local utilise OpenSSL/1.1.1 et le jeu démarre. Pas de variable d'environnement globale ajoutée.
- **Anciens bugs officiels de progression / coop :** les signalements historiques seuls ne justifient pas un second patch de l'EXE actuel ou une écriture dans les sauvegardes. [Recherche précédente](V1_165_NATIVE_BUG_REVIEW.md).
- Les résultats de recherche qui pointent sur **notre propre page Nexus** ne constituent pas une nouvelle validation communautaire indépendante.

## Preuves locales

Le journal lu indique UE4.22.2-43953, D3D11, Arc B580 sélectionnée, pool de textures final 1000 Mo. Engine.ini contient la sélection GPU ; aucune écriture n'y a été effectuée. Les deux noms TAA/netteté et les commandes de streaming ont été retrouvés dans l'EXE supporté SHA256 `9F4702024DF5EEA1D51DF7745B0AD1EA95B97009982F73DDC1218C53DFF33D54`.

Compatibilité du proxy vérifiée dans [dxgi_proxy.cpp](../src/dxgi_proxy.cpp) : GetSystemDirectoryW puis LoadLibraryW de dxgi.dll système. Backend vérifié dans [DarksidersGenesisMod.cpp](../src/DarksidersGenesisMod.cpp) et [ShaderAimFeature.cpp](../src/ShaderAimFeature.cpp) : ID3D11Device / ID3D11DeviceContext et menu D3D11. [Synthèse de preuves](evidence/COMMUNITY_FIX_LOCAL_EVIDENCE_2026_11_13.json).

La documentation UE plus récente et les valeurs proposées par les joueurs restent des pistes : notre EXE, ses types/API et un essai comparatif sont nécessaires avant intégration. Aucune bibliothèque n'a été mise à jour ou téléchargée pour cette étude.

## Suite retenue — sans build

1. Préparer un plan d'essai optionnel pour **TAA/netteté**, valeurs natives sauvegardées/restaurables ; comparer caméra immobile/mouvement, Native/TPS, cheveux/écharpe/flammes et HUD. L'amélioration reste subjective et le scintillement doit être surveillé.
2. Examiner le **budget de textures** uniquement avec preuve de textures dégradées ou de pression de streaming ; comparer la même scène et ne pas appliquer un budget fixe à toutes les machines.
3. Garder **DXVK** comme expérience isolée de chargeur/rendu, avec retour simple à D3D11 natif ; aucun travail de backend stable lancé ici.

Ces points sont une proposition documentée, pas des fonctionnalités implémentées. Le report de build demandé par l'utilisateur reste actif. La base main validée et l'installation personnelle sont intactes.
