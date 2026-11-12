# V1.165 — recherche des bugs natifs du jeu et d'UE4

Demande utilisateur : optimisation globale et étude des bugs natifs, puis build debug. Recherche du 12 novembre 2026. EXE supporté inchangé ; dernier journal local : UE4.22.2-43953, moteur graphique D3D11, Intel Arc B580. Aucun changement de pilote, registre, moteur graphique ou Engine.ini.

| Piste examinée | Conclusion pour ce lot |
| --- | --- |
| Crash Moloch à la transition avec sous-titres | Déjà reproduit, garde ASI validée par l'utilisateur. Conserver exactement ce correctif et son défaut ON ; pas un nouveau fix V1.165. |
| Clés Trickster perdues, blocages de chargement multi, caméra multi détachée, anciens problèmes de collision/progression | Le [patch officiel nº4 du 20 mars 2020](https://steamcommunity.com/app/710920/allnews/) annonce déjà des corrections. Ces anciens signalements seuls ne prouvent pas qu'il faut modifier notre EXE actuel ou les sauvegardes. Pas de second patch arbitraire. |
| D3D device lost / HUNG | [UE-51650 chez Epic](https://issues.unrealengine.com/issue/UE-51650) couvre plusieurs causes/callstacks et dépend du matériel/pilote. Aucune cause reproductible additionnelle démontrée ici ; ne pas masquer la perte du périphérique ni supprimer les contrôles RHI. Distinct du tampon absent du défaut Moloch. |
| Crash au second démarrage lié au cache de shaders DX12 | [UE-65474 chez Epic](https://issues.unrealengine.com/issue/UE-65474) décrit explicitement DX12 et le cache ushaderprecache. La session observée est D3D11 : pas de preuve d'applicabilité, donc aucune purge du cache ou bascule DirectX ajoutée. |
| Saccades et streaming/compilation | À distinguer du coût propre à l'ASI. Le dernier log donne des durées CPU échantillonnées inclusives, pas une mesure GPU/FPS. Les optimisations de découverte V1.165 répondent à du travail répétitif démontré dans le code ; elles ne sont pas vendues comme un correctif général de streaming ou de shaders UE4. |

**Résultat honnête : étude effectuée, aucun nouveau correctif de bug natif suffisamment justifié pour ce build.** Moloch reste acquis. Les autres pistes nécessitent reproduction et attribution précises avant une interception moteur. Pas de hooks de rendu/GC/sauvegarde supplémentaires, ni de réglages génériques UE5 appliqués à ce jeu.

Les notes officielles sont utilisées pour distinguer les anciens défauts déjà traités ; elles ne prouvent pas que tous les problèmes ont disparu. Les fiches Epic servent à classer les symptômes ; leur présence dans une version de moteur n'établit pas à elle seule leur présence dans ce binaire personnalisé.
