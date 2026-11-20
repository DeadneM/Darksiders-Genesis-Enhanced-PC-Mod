# V1.170 — livraison beta cumulative

Base VALIDÉE par l’utilisateur : V1.168 sur main. V1.169 intermédiaire conservée. V1.170 est CONSTRUITE, CONTRÔLÉE et PUBLIÉE sur debug ; le correctif des cœurs et les options d’image restent EXPÉRIMENTAUX en jeu. Aucune installation ni modification de l’INI personnel.

## Objectif et critère de réussite

Afficher les indicateurs de niveaux dès le premier accès à l’arbre des cœurs, sans changer les données ni devoir rouvrir la page. Ajouter deux options d’image indépendantes et réversibles. Réussite en jeu encore attendue : premier accès correct, navigation/équipement normaux et retour au rendu natif après désactivation.

## Construction et contrôles

- Source : `b0a44f1533da98b717646091476f836cbd978eb2` ; archive : `ccc30635e8143a52f774b98cd256b42ea67b4688`.
- [GitHub Actions Windows x64 Release](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44455108222) : 86/86 tests, aucun avertissement/erreur C++ détecté.
- ZIP : 2496750 octets ; SHA256 `1DB4FD8E400528B7F3890847D0FC75E89A318453048D46164BB27AAEE575B96D`.
- Deux fichiers AMD64 à la racine : DarksidersGenesisMod.asi et dxgi.dll ; CRC et contenu du blob GitHub vérifiés. Aucun INI/README inclus.
- Lien public HTTP 200 et sélection V1.170 par l’updater contrôlés, sans téléchargement binaire HTTP supplémentaire.
- Microsoft Defender : analyse des deux binaires terminée, aucune détection les concernant. Ce contrôle ne remplace pas un essai en jeu.
- 234 anciennes archives conservées à l’identique, 235 au total. Main V1.168, archive et anciens tags contrôlés sans modification.
- Profil de référence : 258 clés, les 255 anciennes valeurs et les 55 actions Hotkeys de V1.168 conservées.

[Téléchargement direct du ZIP](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/ccc30635e8143a52f774b98cd256b42ea67b4688/downloads/DarksidersGenesis_V1.170_CORE_TREE_IMAGE_CONTROLS_TEST.zip) · [Preuves de livraison](evidence/V1_170_DELIVERY_CHECK.json) · [Périmètre technique](V1_170_REVIEW.md).

## Changements et tests

1. Gameplay → System and startup → **Creature core level display fix** : ON par défaut. Rafraîchissement natif unique après deux ticks terminés de la page. Après un lancement/chargement, ouvrir directement l’arbre : vérifier les niveaux avec Strife puis War, sans retour au menu précédent.
2. Naviguer, équiper/déséquiper un cœur puis fermer/rouvrir : niveaux, progression et équipement doivent rester cohérents.
3. Camera → Image effects : **Disable motion blur** et **Disable chromatic aberration**, OFF par défaut. Comparer ON/OFF en vue native et TPS ; vérifier le retour du rendu initial et l’indépendance du TAA. Une scène sans effet natif peut montrer peu de différence.
4. Vérifier un chargement et une cinématique, avec réglages mémorisés et caméra/HUD habituels. Coop en dernier si disponible.

Rollback : désactiver le correctif des cœurs et les deux filtres ; si nécessaire, rétablir les deux binaires V1.168 validés en gardant son INI. Aucun patch SourceControl/SoulBarrier, aucun gain de FPS annoncé. Suite : retour utilisateur avant toute promotion stable.
