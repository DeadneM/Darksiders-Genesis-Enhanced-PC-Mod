# V1.83 — premier transfert : Stoneskin sur Strife

**V1.83 CONSTRUITE / CONTRÔLÉE : [51/51 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/39841420585), zéro erreur et zéro avertissement MSVC. Protection Stoneskin sur Strife EXPÉRIMENTALE, partielle et non validée en jeu.** [ZIP cumulatif immuable vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/aa9a096a232dca76de9456387a7565b69de8925d/downloads/DarksidersGenesis_V1.83_STRIFE_STONESKIN_TRANSFER_TEST.zip) · [Reçu et limites](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/blob/debug/docs/V1_83_DELIVERY_RECEIPT.md). Installation manuelle, conserver son INI.

**Limite du prototype : seule la protection native et son coût sont transférés.** L'animation compatible de repos ne porte pas les notifications de War : onde de choc de lancement, sons, particules et apparence de pierre ne sont pas encore transférés. Les variantes de cœurs et la protection réelle nécessitent un essai ; ne pas attendre une aura visible du statut de base. Le mélange complet des six capacités reste ouvert.

Retour utilisateur V1.82 du 22 octobre : **suivi TPS des exécutions VALIDÉ dans l'essai rapporté ; effets des épées cachées inefficaces**, dossier rouvert. Lecture seule : huit notifications ciblées des deux premiers coups et des attaques aériennes sont attachées à Bone_Strife_Root, chemin exclu par V1.82 ; les six notifications du troisième coup sont détachées. Le contrat des mains et des assets est confirmé, le passage réel des callbacks reste à vérifier.

22 octobre 2026. Base cumulative V1.82 `6c916ffcaa7e953ae8503854076d3b668942c0c2`, debug uniquement. V1.82 est en cours d'essai, sans nouvelle validation supposée. La dernière base explicitement validée est V1.81 pour les trois essais rapportés. Aucun fichier de jeu remplacé et aucune relance par l'agent.

## Objectif et statut

**EXPÉRIMENTAL**, compilation et paquet à consigner après CI. Premier transfert fonctionnel candidat parmi les six pouvoirs demandés : **Stoneskin de War vers Strife normal à pied**, sur chacun de ses trois emplacements. Critères : protection réelle, coût natif, sortie du cast et expiration du statut ; restauration du pouvoir initial avec Native ; pouvoirs de War et autres fonctions conservés. Aucun résultat en jeu acquis par la compilation ou les fixtures.

Choix dans Gameplay → Wrath ability slots. Les trois pouvoirs habituels de Strife et ceux de War restent disponibles. Blade Geyser/Rampage vers Strife et Clone/World Ender/Caltrops vers War sont encore ouverts, sans sélecteurs inertes. Coop/P2 à vérifier en jeu, même si le contrat de propriété est testé séparément.

## Contrats et mise en œuvre

EXE exact SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`, 62 113 280 octets. Cinq signatures du nouvel adaptateur comparées à l'EXE avant activation ; sa défaillance n'empêche pas les réaffectations natives existantes. Les recherches V1.80 et [V1.82](V1_82_FOREIGN_WRATH_RESEARCH.md) sont conservées comme historique.

### Déverrouillage, sélection et coût

Pour les pouvoirs propres, V1.77 conserve le déverrouillage du pouvoir sélectionné et son coût. Pour le transfert étranger, le choix explicite remplace la classe dans **l'emplacement d'origine de Strife** : celui-ci doit être débloqué. Il ne requiert pas de donner à Strife un ID War absent de ses propres ressources. La progression et les emplacements de War ne sont pas débloqués ou écrits. Cette règle est affichée dans le menu ; elle diffère volontairement du contrôle d'un autre pouvoir de la même famille.

Le hook de résolution complète `663220` est conservé. Un contexte temporaire par thread permet au getter `7AAC30` de fournir la classe native Stoneskin uniquement pour ce même acteur et ce même ID pendant cet appel. Le résolveur original garde ensuite **son contrôle du slot et le coût de la classe choisie** ; GetWrathCost du Blueprint Stoneskin retourne 5. Le jeu garde DoAbility et la consommation. Aucun appel de lancement synthétique ni événement d'entrée ajouté. Les requêtes imbriquées d'autres ID/acteurs restent natives et le contexte est rétabli à la sortie.

La classe exacte `/Game/Blueprints/Abilities/Stoneskin/MayhemAbility_StoneSkin_Blueprint` est cherchée parmi les objets déjà chargés, en tranches de 512 au maximum sur le thread du jeu. La recherche est demandée par l'ouverture du groupe ou un choix Stoneskin déjà enregistré, puis arrêtée quand la classe est trouvée ou la table parcourue ; reprise après changement du monde, croissance de la table ou expiration de l'identité. Le cache utilise index/série vérifiés avant chaque usage, sans conserver l'acteur d'un autre joueur. Aucun chargement de package ni recherche de toute la table par frame une fois prêt. Classe/default ou animation du receveur indisponibles : seul le slot explicitement réglé sur Stoneskin reste inactif, sans coût et sans lancer un autre pouvoir.

### Animation, effet et fin

Le Blueprint natif applique Steamroller puis Molten ou le statut de base avant PlayMontage. Le statut de base dérive de MayhemStatusEffect_DamageTakenModifier, avec DamageModifier=0,6 et durée=10 s. Ses huit fonctions ont été décodées ; aura et Stinger utilisent MayhemPlayerCharacter. Son retrait de matériaux spécifique à War est derrière un cast vérifié vers War : il n'est pas appliqué à Strife et n'est pas copié ou forcé par le mod. Les variantes et leur disponibilité restent contrôlées par les cœurs du receveur. Les variantes Molten peuvent changer la protection ; ne pas supposer une réduction de 40 % dans tous les cas.

Le second hook `5AA190` est limité à l'instance Stoneskin vivante, rôle local autoritaire, Character/Instigator concordants et Strife réellement propriétaire. Pour les deux montages Stoneskin connus de War, un montage transitoire est créé par la factory native `168A410` depuis Strife_Idle déjà chargé, même squelette que son AnimInstance, slot TPS vérifié. Les huit arguments de la factory sont conservés : séquence, FName du slot, blend in/out, rate, loops, blend-out trigger et start time. La durée du cast est celle du montage natif fourni, par adaptation du taux du montage compatible. Il passe ensuite dans le PlayMontage original de l'aptitude, qui conserve son instance et son délégué de fin. L'animation est une **pose de repos de Strife pendant le cast**, pas l'animation de pétrification de War ni une promesse d'animation retargetée.

La requête native `676620` vérifie que l'instance a effectivement commencé. En cas d'échec de source, création ou lecture, la fin normale native `59B080(cause=0)` libère l'action. Ce zéro est prouvé par le callback de fin de montage `5A76D0`, et non deviné depuis un nom d'énumération. Pendant OnStart, le jeu diffère lui-même cette fin jusqu'à son retour. Le statut déjà appliqué et son coût restent natifs ; aucun montage War incompatible n'est lancé sur l'instance identifiée de Strife. Les autres classes, acteurs, threads, prototypes et actions passent dans l'original sans changement.

## Préservation et nettoyage

Deux hooks facultatifs nouveaux, pas de hook global ProcessEvent, aucun bouton fixé, aucune écriture d'ID de ressource, CDO ou Blueprint, aucun montage partagé modifié. L'ancienne réaffectation de famille reste dans son chemin. Le parsing d'ID War étudié précédemment n'est pas dans l'ASI : il n'est plus utile pour la règle de slot d'origine. Un seul adaptateur, sans ancien candidat parallèle. Les logs de préparation/usage/échec sont bornés.

166 clés/valeurs INI, six Native par défaut, 55 ID Hotkeys conservés. Aucun ajout de réglage : la valeur stable 6, déjà réservée à Stoneskin, devient supportée pour Strife. Le code caméra, effets de mêlée V1.82, bras/pistolets/holsters, World Ender, souris/manette/roues, cheval et jauges est inchangé ; seul un appel d'observation des capacités est ajouté au service local.

## Essai groupé

1. En mission avec Strife à pied, mettre Stoneskin dans un emplacement débloqué, puis l'activer par sa commande habituelle. Désactiver Unlimited Wrath pour voir le coût. Vérifier aura/protection puis reprise des contrôles à la fin du cast. Pour comparer les dégâts, désactiver Unlimited Health et les variantes Molten ; le statut de base doit expirer après sa durée native.
2. Essayer chacun des trois slots séparément, puis remettre Native : retour du pouvoir d'origine. Un emplacement encore verrouillé reste verrouillé. Ne pas se fier aux anciennes icônes de roue pour identifier le pouvoir choisi : elles peuvent garder leur présentation de départ.
3. Tester les variantes de cœurs si disponibles, puis changement de personnage/cheval/ultime et retour. Les pouvoirs de War restent normaux ; le transfert n'est actif que sur Strife normal à pied. Coop/P2 ensuite si souhaité, sans partager le composant ou l'acteur de P1.
4. Contrôle court de la base : tir/visée/dash, OFF des armes, roues, puis essais V1.82 encore attendus sur les arcs de mêlée et une exécution haute.

Livraison manuelle à quatre fichiers racine, conserver son INI personnel et ses raccourcis. Rollback immédiat : ASI V1.82 conservé (ou V1.81 pour la dernière base validée), même INI ; aucun changement de progression ou sauvegarde induit par le mapping. Les 147 anciens ZIP, main/archive et v1.43 restent protégés. Toute activation de la fonction et son comportement réel restent à valider par l'utilisateur.
