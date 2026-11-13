# V1.168 — validation et publication stable

Validation globale reçue le 13 novembre 2026. Base précédente : V1.167 validée, main `6795f47`. V1.168 retient les réglages globaux optionnels TAA/netteté, OFF par défaut et restaurables, ainsi que le diagnostic neutre de chargement ASI. Chargeur et fonctions antérieures validées conservés. Aucun succès supplémentaire de chargeur alternatif ni gain FPS déduit de cette validation.

La build existante a déjà été compilée en configuration **Release x64** dans [Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43456874935), source `8db34a0d8cddd5cc4970a97fb3400e4605de694c` : **85/85 tests**, zéro avertissement/erreur C++ détecté. Pas de recompilation : le ZIP publié est exactement celui installé et validé.

Archive du dépôt : `DarksidersGenesis_V1.168_OPTIONAL_TEMPORAL_AA_TEST.zip`, 2485486 octets, SHA256 `7572BB6F2ADF9C7D9200578834CA3BDD9C3A3D244A8BA889C6AEC269653DFEB8`. Le nom historique TEST est conservé pour ne pas modifier une archive numérotée ; son statut est maintenant VALIDÉ. L’asset de release porte un nom de distribution stable avec les mêmes octets. Deux fichiers à la racine : ASI et dxgi.dll ; aucun INI/README. Les 233 archives cumulatives, les raccourcis, l’INI personnel et la branche archive sont préservés. Le canal main de l’updater retrouve désormais V1.168.

Release demandée : [v1.168](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/tag/v1.168). La confirmation effective et les références distantes sont consignées dans la preuve de publication après vérification. Aucun lancement ni réinstallation du jeu nécessaire ; test futur uniquement en cas de nouveau changement.
