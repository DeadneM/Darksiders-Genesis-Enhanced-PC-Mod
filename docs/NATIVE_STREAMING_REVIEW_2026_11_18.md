# Streaming natif — diagnostic du 18 novembre 2026

Base : V1.168 validée et publiée. Statut : **DIAGNOSTIC, aucun correctif implémenté ou gain annoncé**. Sources locales déjà présentes ; aucune archive téléchargée, modification du jeu, lecture de mémoire de processus ou nouvelle build.

## Constat précis

Le journal moteur du 16 novembre contient une seule demande introuvable de `/Script/SourceControl`, liée à `/Game/Blueprints/Interactables/SoulBarrier_Orb_Blueprint`, à 19:48:22.087 UTC. Elle suit StartLoadingDestination pour Level_00. Le moteur annonce une recherche disque pouvant provoquer une saccade de streaming ; il ne donne pas sa durée. Le retour en chargement/flush quelques secondes plus tard ne peut pas être attribué intégralement à cette erreur.

Le pool final est 1000 Mo. Pas de message de dépassement de pool trouvé dans ce journal. Cela ne prouve ni l’absence de pression mémoire à tout instant, ni un intérêt à augmenter arbitrairement ce budget.

## Provenance native confirmée

Index déjà en cache de ProjectMayhem-WindowsNoEditor.pak : identité du footer/index vérifiée. Seules les deux entrées SoulBarrier_Orb_Blueprint.uasset/.uexp ont été lues et décompressées, avec contrôle de leur SHA1 compressé et taille décompressée. L’uasset natif contient `/Script/SourceControl`, `Default__SourceControlHelpers` et `SourceControlHelpers`. La dépendance est donc présente dans les données livrées du jeu ; ce constat ne prouve pas qu’un ASI ne puisse jamais influencer le moment où l’objet est chargé.

Aucune occurrence de SoulBarrier ou SourceControl trouvée dans le code src du mod. Le journal du mod ne contient pas de demande nommée correspondante. La documentation [Epic sur Source Control](https://dev.epicgames.com/documentation/en-us/unreal-engine/source-control-in-unreal-engine) décrit l’outil de gestion de versions de l’éditeur. Cette documentation actuelle aide à comprendre le nom, mais ne fournit pas un correctif applicable au binaire UE4.22 de Genesis.

## Décision

Ne pas ajouter une fausse DLL de module, supprimer globalement la recherche de packages, masquer l’erreur ou modifier une dépendance sans analyser ses usages. L’absence de durée mesurée et l’occurrence unique ne permettent pas d’expliquer un ralentissement permanent de Strife.

Suite effectuée : les 13 fonctions sont intégralement décodées. IsEnabled est appelé par SetInitialAnimState et sélectionne réellement une branche d’état/effet ; ce n’est pas une référence inutilisée. Aucun patch d’asset ni hook de chargement global retenu pour cette occurrence non chronométrée. [Flux et décision du 19 novembre](CONTINUATION_REVIEW_2026_11_19.md).

[Extraits et empreintes natifs](evidence/NATIVE_STREAMING_REVIEW_2026_11_18.json). Les deux extraits d’assets restent dans le cache local privé, pas dans GitHub. [Réduction des fichiers d’état, preuves de conservation](evidence/STATE_COMPACTION_2026_11_18.json).
