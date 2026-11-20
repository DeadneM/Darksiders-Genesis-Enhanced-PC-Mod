# V1.171 — livraison beta cumulative

Base VALIDÉE par l’utilisateur : V1.168 sur main. V1.169 et V1.170 intermédiaires conservées. V1.171 est CONSTRUITE, CONTRÔLÉE et PUBLIÉE sur debug ; le correctif des cœurs et les options d’image restent EXPÉRIMENTAUX en jeu. Aucune installation ni modification de l’INI personnel.

## Différence depuis V1.170

Le code d’erreur Windows du chargement ASI est capturé avant l’écriture du log qui pouvait le remplacer. Aucun changement de chargement ou de gameplay ; cela ne résout pas à lui seul l’ancien essai d’un autre chargeur. Seuls dxgi_proxy.cpp et l’identité de build diffèrent dans le code compilé. La référence INI et les tests restent identiques à V1.170.

## Objectif et critère de réussite

Afficher les indicateurs de niveaux dès le premier accès à l’arbre des cœurs, sans changer les données ni devoir rouvrir la page. Ajouter deux options d’image indépendantes et réversibles. Réussite en jeu encore attendue : premier accès correct, navigation/équipement normaux et retour au rendu natif après désactivation.

## Construction et contrôles

- Source : `cf0273f25522d3c58e48a6df398a081d2db30a71` ; archive : `f1842511b13f5c3984c69b298e8975fcea007e0d`.
- [GitHub Actions Windows x64 Release](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44523095786) : 86/86 tests, aucun avertissement/erreur C++ détecté.
- ZIP : 2483438 octets ; SHA256 `BB1AA512C8618F39DF1AA7719213CA3EEC4A3AF87E9EE203B0CC6E3379F595D9`.
- Deux fichiers AMD64 à la racine : DarksidersGenesisMod.asi et dxgi.dll ; CRC et contenu du blob GitHub vérifiés. Aucun INI/README inclus.
- Lien public HTTP 200 et sélection V1.171 par l’updater contrôlés, sans téléchargement binaire HTTP supplémentaire.
- Microsoft Defender : analyse des deux binaires terminée, aucune détection les concernant. Ce contrôle ne remplace pas un essai en jeu.
- 235 anciennes archives conservées à l’identique, 236 au total. Main V1.168, archive et anciens tags contrôlés sans modification.
- Profil de référence : 258 clés, les 255 anciennes valeurs et les 55 actions Hotkeys de V1.168 conservées.

[Téléchargement direct du ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f1842511b13f5c3984c69b298e8975fcea007e0d/downloads/DarksidersGenesis_V1.171_CORE_TREE_LOADER_DIAGNOSTICS_TEST.zip) · [Preuves de livraison](evidence/V1_171_DELIVERY_CHECK.json) · [Périmètre technique](V1_171_REVIEW.md).

## Changements et tests

1. Gameplay → System and startup → **Creature core level display fix** : ON par défaut. Rafraîchissement natif unique après deux ticks terminés de la page. Après un lancement/chargement, ouvrir directement l’arbre : vérifier les niveaux avec Strife puis War, sans retour au menu précédent.
2. Naviguer, équiper/déséquiper un cœur puis fermer/rouvrir : niveaux, progression et équipement doivent rester cohérents.
3. Camera → Image effects : **Disable motion blur** et **Disable chromatic aberration**, OFF par défaut. Comparer ON/OFF en vue native et TPS ; vérifier le retour du rendu initial et l’indépendance du TAA. Une scène sans effet natif peut montrer peu de différence.
4. Vérifier un chargement et une cinématique, avec réglages mémorisés et caméra/HUD habituels. Coop en dernier si disponible.

Rollback : désactiver le correctif des cœurs et les deux filtres ; si nécessaire, rétablir les deux binaires V1.168 validés en gardant son INI. Aucun patch SourceControl/SoulBarrier, aucun gain de FPS annoncé. Suite : retour utilisateur avant toute promotion stable.


Validation fonctionnelle utilisateur reçue le 19 novembre après livraison : « tout es ok ». Nouveau signalement de micro-saccades et de charge GPU, sans ancienneté établie ; [diagnostic distinct](GPU_FREEZE_REVIEW_2026_11_19.md). Main V1.168 reste inchangée. L’archive V1.171 ci-dessus reste identique ; l’optimisation préparée ensuite n’y est pas incluse.
