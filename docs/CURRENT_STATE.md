# État courant — V1.168

- **VALIDÉE intégralement par l’utilisateur**, build Release x64, 85/85 contrôles Windows.
- Main et debug vérifiés au départ : `3e731bd18198d214dee567bb682c9f7067990841` ; tag v1.168 : `582d7aa07063f3d5ca1f65a1d857273c206be83d`.
- [Release stable](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/tag/v1.168). ZIP publié identique à celui installé/testé : SHA256 `7572BB6F2ADF9C7D9200578834CA3BDD9C3A3D244A8BA889C6AEC269653DFEB8`.
- ASI + dxgi.dll à la racine, aucun INI/README dans le ZIP. 255 valeurs de référence, 55 actions Hotkeys. INI personnel et installation conservés.
- TAA et netteté optionnels validés, OFF par défaut. Fonctions antérieures conservées ; aucune exclusivité de chargeur ASI démontrée.

## Reprise du 18 novembre

Recherche hors jeu uniquement. Une dépendance SourceControl absente est demandée au chargement Level_00 par un Blueprint natif ; la référence existe dans le PAK d’origine. Aucun coût précis mesuré et aucun patch appliqué. Pool final de textures toujours 1000 Mo, pas de dépassement signalé dans le journal examiné. [Diagnostic et preuves](NATIVE_STREAMING_REVIEW_2026_11_18.md).

La file et l’état courant sont désormais courts. Leurs anciens contenus sont conservés byte pour byte dans les historiques liés ci-dessous. Aucun code, binaire, réglage ou fichier personnel modifié, aucune nouvelle build/install/capture du jeu.

- [Prochaines étapes](ACTIVE_QUEUE.md)
- [Livraison V1.168](V1_168_DELIVERY_RECEIPT.md) · [Publication stable](V1_168_MAIN_RELEASE.md)
- [Ancien état complet](history/CURRENT_STATE_THROUGH_V1_168_2026_11_18.md)
- [Ancienne file complète](history/ACTIVE_QUEUE_THROUGH_V1_168_2026_11_18.md)
