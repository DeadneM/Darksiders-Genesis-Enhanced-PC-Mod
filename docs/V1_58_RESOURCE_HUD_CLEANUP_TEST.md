# V1.58 — preuves, périmètre et limites

Date : 18 octobre 2026. Statut : SOURCE DE TEST / EXPÉRIMENTAL, pas de validation en jeu.

## Preuves natives du HUD

Deux joueurs locaux observés en lecture seule, chacun avec son InteractHUD au champ personnage `EF8`, propriétaire réciproque `E8`, widget `610`, espace monde et taille native 500 × 500. Les deux champs OwnerPlayer `5E0` sont nuls ; les rotations sont identiques. Le retour visuel utilisateur confirme que le prompt P2 suit P1.

La fonction native d’orientation `5EA970` utilise GetOwnerPlayer `14B5010`, dont le repli pour un propriétaire nul prend le premier LocalPlayer du monde. Le setter `14EC0F0` effectue le nettoyage natif d’inscription puis écrit le propriétaire ; il est appelé avant l’orientation originale. La taille passe par le setter natif `17B03D0`, vtable `275B7D0 + 3C8`, et RelativeScale3D `17C`, vérifié dans les métadonnées `379E0D0 + 36`. Empreintes des trois fonctions vérifiées contre l’exécutable exact pris en charge.

Un seul hook nouveau, sur cette orientation. Filtrage thread jeu, vtable, propriétaire local/normal, réciprocité du composant ; cas inconnus inchangés. Baselines liées à l’index/série UObject, non au slot joueur : pas de multiplication cumulative lors des changements de personnage/contrôleur. Cache borné à 16 objets, aucune éviction d’un objet vivant modifié. Désactivation restaure la taille native connue ; une nouvelle taille native animée devient la nouvelle référence. Aucun scan global dans l’ASI. L’orientation par propriétaire reste corrigée quand la réduction de taille est OFF. Les autres sprites, barres partagées et effets ne sont pas couverts.

## Recovery et tirs

Suppression du pointeur de fin anticipée, état de phases, calcul de délai et champs runtime/config/UI Recovery. Nouveau nom DashCombatFeature décrivant le code conservé. Le Tick appelle l’original une fois avec le delta entier, puis observe le dash pour la reprise AIM. Corps OnLivePlayer, Owner, GroundDashActive, MainPistols, HookSpawnNotify et SkipPresentation identiques à V1.57 ; contrôlés automatiquement. Identifiant ActionRecovery 3 réservé pour ne pas décaler les 47 autres actions ; ancien token inactif, absent des nouveaux choix.

Les filtres V1.55/V1.57 ne couvrent qu’un sous-ensemble reconnu de tirs spéciaux tenus. Le nouveau retour démontre un problème plus large avec toutes les munitions et une animation encore présente. Ils sont conservés à l’identique, pas présentés comme une solution complète. Balistique et sélection de cible inchangées. Le futur Recovery doit repartir d’un verrou d’action natif démontré, sans fin forcée arbitraire ni modification d’un stick/touche.

## Facteurs et raccourcis

Consommation = coût natif / diviseur positif, avec crédit fractionnaire et saturation avant conversion entière ; sentinelles d’infini et gains restent natifs. Hotstreak = baisse native / diviseur positif, limitée aux appelants de déplétion déjà validés. Potions = seuil natif / rythme positif. 0 est le repli natif historique, pas Unlimited. Tests numériques, sous-unitaires, minuscules, invalides et aller-retour INI/runtime. Les réglages existants de mouvement, hauteur, durée, dégâts et gains acceptaient déjà les fractions.

Le Clear ne perdait pas `None` au chargement : la zone entière de réglages natifs était désactivée en TPS. Fin du bloc désactivé déplacée avant les raccourcis. Test de sauvegarde/rechargement de chacune des huit actions effacées, y compris Zoom In, sans changer les autres.

## Flou : constat, pas réparation

Après redémarrage utilisateur, capture coop P1 Havoc / P2 Rampage : focus 643.06183 / 551.21210, profondeurs calculées 643.06182 / 551.21208, poids 1 et override 08. Le champ de focus est donc reçu par les deux vues ; absence de callback ou confusion du propriétaire ne suffit plus à expliquer le flou. Bounds des modèles vérifiés par le getter natif 1908AB0 (origine 148, étendue 154, rayon 160). Leur taille pourrait nécessiter une autre profondeur utile, mais cela ne démontre pas la cause.

Le chemin de rendu natif F5D540 traite notamment méthode, ouverture, capteur et profondeur dans sa formule de cercle de confusion ; il ne suffit pas de pousser FocalRegion pour toutes les méthodes. Les volumes/mélanges finaux propres au chapitre 8 n’ont pas été identifiés. Pas de modification spéculative du rendu dans V1.58 ; TpsCameraFocus.h, CoopCameraFeature.cpp et les sources de trajectoire restent identiques. Aucun asset privé ni dump binaire publié.

## Vérification et arrêt

31 suites prévues : retrait compatible Recovery, vraie reprise AIM simulée, vrai hook HUD simulé avec P1/P2, réaffectation, objet réutilisé, scale native animée, OFF et cas exclus, facteurs et profil complet. CI/ZIP ne démontrent ni rendu ni comportement dans le jeu. Si l’orientation P2 ou la visée après dash régresse : arrêter le candidat et restaurer V1.57. INI personnel sauvegardé et conservé ; anciennes archives et branches protégées intactes.

## Résultat de construction et complément de lecture seule

31/31 suites réussies après correction du format de l’INI livré (0.650), sans changer le test de parité. Compilation MSVC/analyse statique sans avertissement ASI/DXGI ; avis des outils conservés. Voir les références de commit/CI/ZIP dans CURRENT_STATE. Pas de validation en jeu ni d’installation à ce stade.

Le volume LVL09_PostProcessVolume_Dungeon est présent dans Level_09_Base_Dungeon : activé, borné, priorité 0, rayon de mélange 100, poids 1. Les paramètres stockés comprennent focus 1000, région 3400, méthode 1 et ouverture 4, mais les flags d’override focus (octet 16, bit 08) et région (octet 17, bit 04) sont nuls. Ne pas confondre valeur stockée et réglage effectivement mélangé. Les caches caméra actuels/précédents 1A00/1F90 ont le même focus et bit 08 que la sortie de vue ; le cheval reste à 551.2121 dans les trois. Le P1 est repassé à pied lors de ce dernier relevé (329.897), la preuve Ultimate précédente reste distincte. Il faut identifier le résultat final de post-traitement, pas ajouter un hook spéculatif.

## Installation effective

Installation après confirmation utilisateur et vérification de 272 processus visibles, jeu fermé. Sauvegarde V1.57 `install-backup-before-v158-20261018-192726`, manifeste d’intégrité et `Restore-Previous.ps1` conservés ; syntaxe de restauration contrôlée, script non exécuté. **INI personnel inchangé octet pour octet** : `FA9E7A196CB348C9DEF962E2A62A93A4DA91042D21D067D091E232B295E3AB38` ; raccourcis conservés. ASI/DXGI installés identiques au paquet vérifié. Aucun lancement du jeu par l’agent, aucune validation de gameplay. README installé actualisé avec ce reçu ; le ZIP reste immuable.

V1.57 est la sauvegarde de retour arrière ; les prochains essais sont ceux du CURRENT_STATE. Les défauts connus de flou et de tir pendant dash restent ouverts.
