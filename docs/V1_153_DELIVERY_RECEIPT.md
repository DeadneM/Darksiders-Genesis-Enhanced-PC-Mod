# V1.153 sur debug

Le rework dash/stomp de V1.152 est VALIDÉ par l'utilisateur. La case Tap / hold air dash l'active par défaut dans Gameplay → Player Movement. Les paramètres de réception sont visibles sous leur case principale. Les récupérations expérimentales de dash et de coffre ont chacune leur case et un délai partagé.

Stoneskin reprend après un refus provisoire, conserve son rendu quand une lecture de statut est incomplète et utilise trois sources de matériaux requises. Les fonctions et paramètres vérifiés sont mémorisés. Les observations sont limitées à 50 ms en activité ou en attente de ressources et à 250 ms au repos. Une activation ou un changement de propriétaire force l'observation. [Audit et méthode](V1_153_STONESKIN_REVIEW.md).

**CONSTRUIT et CONTRÔLÉ** sur GitHub Windows x64 avec 81 tests réussis et aucun avertissement de compilation détecté sur la source finale. Le premier essai a détecté une confusion entre une adresse de matériau et l'heure du prochain essai. La correction a été contrôlée par le même test de reprise. [Contrôle GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/42021391821).

Le ZIP de 2419438 octets contient deux modules AMD64 à la racine. Le téléchargement public, les CRC et la sélection par les règles du canal debug ont été vérifiés. Les 217 archives précédentes sont conservées. Les 246 anciennes valeurs du modèle INI sont reprises et la nouvelle clé Features.AirDashRework vaut 1. Le modèle INI compte 247 valeurs. Les références main, archive et v1.43 ont été vérifiées. [Preuve du ZIP](evidence/V1_153_PACKAGE_CHECK.json).

Source `1a4bbe29f65e8533ba8741cd9a93e3ee30b42a28`. Archive `6063efc6a06e2f760dec404fe67d5a19ae451fb9`. SHA256 `BAAB9A9D41096983349B5650F4FE01F1DF7E073FDD9B42795FD3A0F52E67A9CD`.

**EXPÉRIMENTAL en jeu pour Stoneskin et les changements du menu.** Les tests reproduisent les refus temporaires, les statuts incomplets et les reprises sur une adresse identique. Le gain FPS et la consommation électrique restent à mesurer. Le premier chargement ciblé garde le coût de l'appel de chargement natif.

Defender n'a rien détecté dans le ZIP ni dans ses deux modules, avec les signatures 1.463.132.0 et les protections actives.

Tester Stoneskin avec Strife après un changement d'armure et un chargement, plusieurs fois dans une mission. Vérifier sa fin et comparer la fluidité au même endroit avec V1.152. Vérifier ON/OFF puis la persistance de la nouvelle case. Le repli V1.152 est disponible.

[Télécharger V1.153](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/6063efc6a06e2f760dec404fe67d5a19ae451fb9/downloads/DarksidersGenesis_V1.153_STONESKIN_LIFECYCLE_COST_TEST.zip).
