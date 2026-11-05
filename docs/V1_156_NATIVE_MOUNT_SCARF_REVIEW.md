# V1.156 — flammes natives du cheval et écharpe Stoneskin

## Base, objectif et preuve

Base acceptée : main V1.153 (`563ae9ca8c2647689af8464d320544afe55b1894`). Le lot cumulatif part du debug `415c8bfefbbdd03715cc0656da9e178c9b14f091`, conserve les poussières V1.154 validées et la continuité menu V1.155 encore à valider. Objectif : rétablir les effets permanents natifs manquants à la première apparition du cheval ; préserver la découpe de l'écharpe de Strife pendant Stoneskin. Critère de réussite : premier cheval correctement rendu sans rappel, puis écharpe complète pendant le pouvoir et rendu d'origine exactement retrouvé après expiration, sans régression de mouvement/armes/menu.

La capture autorisée a enregistré 848 échantillons, sans erreur globale de lecture. Despair initial : 13 composants à template chargé, tous inactifs. Après rappel : autre instance de Despair, crinière, queue, traînées et sabots actifs. L'utilisateur confirme que les effets sont revenus, apparemment tous. Ce contraste ne prouve ni l'origine exacte de la désactivation ni l'innocuité de tous les hooks. Aucun correctif n'a été exécuté dans cette capture.

Les scripts originaux ont été extraits du PAK vérifié (index SHA1 `5d0982cd46a5221290cb6ed24696e3962e9dddff`) et décodés entièrement. OnLevelSpawn et OnHorseSpawn rejoignent le même parcours : visibilité de PassiveSkelArray puis Activate(false) de PassiveFXArray. Ce dernier contient aussi des sabots et des emplacements sans template, donc une liste des seules quatre flammes aurait été incomplète. La lecture suivante confirme le tableau au décalage 0xB40, ses propriétaires, le bit d'activité 0xAE/1 et ActorComponent.Activate : entrée 0x1CBE4D0, bReset booléen à 0, taille totale 1. L'exécutable confirme l'appel virtuel normal, sans écriture directe du bit. [Preuve compacte](evidence/V1_156_NATIVE_VISUALS_OBSERVATION.json).

## Cheval : reprise bornée du parcours natif

Un service sur le fil du jeu attend deux secondes de possession stable, hors pause, cinématique et roue. Il ne parcourt que PassiveFXArray du cheval local courant ; vérifie chaque propriétaire, parent et template ; ignore les emplacements vides. Il n'envoie Activate(false) qu'une seule fois à chaque composant assigné si **tous** étaient inactifs. Un tableau partiellement actif est laissé intact. Aucun rappel complet, BeginPlay, effet d'apparition/disparition, sprint ou changement de qualité/shader. Les consultations sont espacées de 250 ms et la liste n'est examinée qu'une fois par possession stable. Les contrats inconnus refusent la reprise ; aucun pointeur périmé n'est déréférencé.

Ce correctif reste EXPÉRIMENTAL : il traite l'état observé, sans prétendre avoir démontré la cause initiale. Une absence partielle d'effets ou un shader défectueux ne serait pas corrigé par cette règle. La lecture actuelle d'un cheval sain confirme que des emplacements vides sont normaux ; ils ne doivent pas empêcher le contrôle.

## Écharpe : texture de l'armure équipée

Le tissu utilisait le shader masqué de War avec sa texture Base_Color, dont la découpe n'est pas adaptée aux UV de Strife. Le MID privé de chaque emplacement tissu garde maintenant la Base_Color de l'armure effectivement équipée, alpha compris, et conserve le parent feu natif Stoneskin/Molten. Le corps minéral et ses paramètres ne changent pas. L'effet visuel reste à juger en jeu : conserver Base_Color conserve aussi ses couleurs, pas seulement son alpha.

La lecture confirme SetTextureParameterValue : entrée 0x1D97360, ParameterName 8 octets à 0, Value 8 octets à 8, taille 16. La recherche du paramètre suit au plus huit parents MIC/MID, paramètres globaux uniquement, texture Texture2D vivante. Le setter ne vise que le nouveau MID privé ; aucun matériau partagé n'est écrit. Échec du contrat ou texture absente : restauration de ce seul emplacement, reprise bornée existante. L'expiration, le changement d'armure et les transformations conservent la restitution native déjà acceptée. Aucun appel de texture par image.

## Contrôles, statut et retour arrière

Nouveau contrôle ciblé de possession/activité/lifecycle/ABI du cheval, garde des transitions et état P2 indépendant. Le contrôle Stoneskin couvre conservation des textures sources, absence d'appels répétés, API manquante et repli du tissu ; les tests existants de restauration, armure Abyssale, wrappers natifs et effets restent requis. Compilation Windows et tests exclusivement sur GitHub ; pas de compilation locale, pas d'installation ni de relance automatique.

Statut initial : implémenté, EXPÉRIMENTAL ; pas encore CONSTRUIT ni VALIDÉ en jeu. La livraison précisera la CI. INI, raccourcis et défauts conservés ; ZIP deux binaires sans INI/README. Main/archive et toutes les anciennes archives restent intacts. Retour arrière : réinstaller V1.155 sans remplacer l'INI personnel.

Test groupé : démarrage au chapitre 1 à cheval sans rappel ; pause/roue puis descente/rappel ; Strife Stoneskin avec l'armure actuelle puis une autre, avant/pendant/après ; tissu entier et feu conservé ; menu ouvert/fermé. Coop réelle reste à vérifier plus tard comme convenu.
