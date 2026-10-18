# V1.53 — faisceaux, obstacle derrière la caméra et protection du dash en combat

## Base, retour utilisateur et objectif

Base debug V1.52 `8ec540a0542af4a2e77938b932f3e7c8531bbb3b`. Le 18 octobre, l'utilisateur **valide Lava et le tir chargé**. Tir et dash en vue du dessus refonctionnent dans le cas simple. Quatre défauts restent signalés : Static Shot et Beam Shot à gauche ; perte de visée après déplacement + visée + dash ; balles normales pendant dash avec Recovery et une autre munition équipée ; tir retourné vers la caméra quand elle touche le décor. La validation ne couvre pas globalement toutes les munitions/variantes/P2. INI, valeurs par défaut et raccourcis à préserver.

Objectif : corriger la direction du faisceau à son véritable consommateur et empêcher une cible de caméra derrière le départ du tir ; protéger la transition native du dash en combat. Critères : faisceaux vers la cible sans décalage fixe, caméra contre le décor sans tir arrière, munition et visée conservées au dash. **SOURCE DE TEST / EXPÉRIMENTAL**, compilation, installation et comportement réel à vérifier avant de les annoncer.

## Preuves et changements

### Faisceaux

Les assets natifs Static Shot Finish et Beam Shot utilisent `MayhemPlayerBeamShotAbility`, dérivée de la capacité projectile. Leur `SpawnBeam` rejoint `6864B0`, puis `619760` / `6BC9A0` / `697FD0` : la branche Beam Shot crée/attache son acteur par `6191B0` et ne consomme pas la rotation calculée pour les projectiles ordinaires. Une correction plus tôt peut donc être contournée.

Le constructeur natif de `MayhemBeamShot` (`5F2D30`, classe à `380A4B0`) installe le volume `5E8` comme racine `158`. Son Tick `61D410` choisit une cible native/chaîne, un point explicite, ou la direction du socket/de la racine. Dans la branche libre sans socket reconnu, le retour de `1773600` au site `61D5E5` devient la vraie direction `volume+270` ; portée et collisions sont ensuite traitées par le jeu.

Une seule interception supplémentaire remplace **ce retour précis**, après original, pour un volume racine réciproque d'un Beam Shot appartenant au Strife local P1/P2. L'Owner natif `AActor+108` est confirmé par le thunk GetOwner `7F30A0`. Cible commune et origine réelle du volume. ON adapte le vertical ; OFF garde le pitch natif et corrige la direction horizontale commune. Aucun angle horizontal constant, aucune rotation d'acteur, aucune écriture du volume, de la portée, des dégâts ou de la chaîne. Les branches de socket reconnu et de cible/chaîne restent natives : leur couverture visuelle n'est pas prouvée. Un compteur distinct indique les retours réellement modifiés.

### Caméra contre le décor

V1.52 acceptait une collision située entre la caméra et le départ du tir. Viser ce point depuis le personnage pouvait inverser le projectile. Le même résolveur de cible est désormais utilisé par l'acquisition souris et les consommateurs pistolet/capacité/faisceau : une cible déjà devant reste identique ; une cible derrière provoque une seule nouvelle requête native, sur le même rayon à partir du plan du départ du tir. Une pénétration persistante produit un point devant, sans conserver un faux cache de collision. Les obstacles devant restent interrogés et les collisions natives des projectiles/faisceaux restent actives. Origine pistolet = argument SpawnLocation ; capacité = Instigator.Root comme avant ; faisceau = volume natif. Ni portée balistique Lava, ni angle caméra modifiés.

### Dash et munition

Le journal V1.52 du 18 octobre 08:18:12–08:32:04 contient 143 observations après dash : P1 TPS ON/OFF 18/48, P2 29/48 ; toutes avec Recovery ON. La visée physique atteint encore les getters natifs dans plusieurs cas de vue native ; ces mesures ne prouvent pas l'activation de la bonne capacité. 25 + 15 demandes de fin Recovery sont enregistrées. Aucun essai Recovery OFF n'est démontré par ces relevés.

Les Blueprints Dash/Aim/Fire gèrent des OnFinish, actions en attente et prochains dashs distincts. Le module Recovery pouvait demander une fin native dès la décélération épuisée, même en plein combat. Il s'abstient maintenant pendant les actions natives AIM, RANGED_ATTACK_ONE ou TWO, et abandonne son armement pour cette activation. Accès par l'interface GetActionState native `actor+8D0`, virtuelle `+18`, utilisée par le thunk `770D00` et le code de dash ; IDs d'action 31/18/20 vérifiés dans l'énumération du jeu. **Aucune touche physique imposée.** Interface inconnue : timing natif conservé.

Recovery reste disponible pour les dashs sans visée/tir. Cette protection est une réduction de portée volontaire : **la responsabilité exacte de la fin anticipée dans les deux bugs n'est pas encore prouvée**, et elle n'est pas présentée comme une réparation native de tous les dashs. Aucun remplacement forcé de munition, bit AIM/MOVE, montage, coffre ou réception lourde. Les observations ajoutent l'état d'action combat pour le prochain essai.

## Nettoyage et conservation

La requête de collision native, auparavant répétée dans trois endroits, est centralisée. Les nouveaux chemins utilisent la même cible que les tirs validés. Le calcul `SolveLava` reste identique à V1.52. Le socle Charge Shot, la pose caméra ON/OFF, XInput, échange des sticks, roues, équipement, RawInput, Alt-Tab, ressources et Hotkeys sont conservés. Aucune nouvelle option ni nouvelle clé. 119 clés et 48 IDs inchangés. Aucune suppression d'archive ou modification de main/archive/v1.43.

## Essais à faire

1. Static Shot et Beam Shot : TPS, 3D ON puis OFF, immobile puis en déplacement ; viser devant à plusieurs angles. Relever aussi normal/Hotstreak et P1/P2. Attendu : rayon aligné et portée habituelle.
2. Caméra contre un mur derrière le personnage : tourner et tirer. Puis viser un mur **devant**. Attendu : aucun tir vers la caméra ; le mur devant continue à bloquer le tir.
3. Vue du dessus : maintenir déplacement + visée, dasher, poursuivre la visée. Comparer Recovery OFF puis ON. Essayer ensuite le même cas en tirant.
4. Munition spéciale équipée : tirer pendant les dashs avec Recovery ON puis OFF. Attendu : aucune substitution introduite par le mod ; noter le type et le joueur en cas d'échec.
5. Contrôle court des acquis : Lava en cloche, Charge Shot en Hotstreak/recul, roue d'équipement et maintien d'équipement en coop. La vue caméra doit rester identique entre 3D ON/OFF.

Retour arrière : conserver V1.52 et son ZIP immuable ; désactiver Recovery pour isoler le dash. Installer seulement après fermeture du jeu, sauvegarder les fichiers et conserver l'INI personnel. Aucun lancement automatique autorisé.
