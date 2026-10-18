# V1.55 — cycle de visée après dash et projectiles imposés par l'animation

## Base et statut

Base cumulative `debug` V1.54, HEAD documentaire `6bea05ec6347ef4e0a15e34e7c2b6bc8a23491b2`. Les calculs Lava, Charge Shot, faisceaux et obstruction validés dans les cas utilisateur sont conservés, comme le correctif DOF V1.54. **SOURCE DE TEST / EXPÉRIMENTAL ; compilation et paquet à contrôler, réparation non validée en jeu.** V1.54 a été construite mais n'a pas été installée ; le jeu du test Static Shot utilise V1.53.

Objectif : conserver une capacité native de visée lorsque le dash l'a terminée malgré une demande AIM toujours tenue ; empêcher les notifications de Dash Fire d'imposer une balle Standard aux demandes de munition spéciale reconnue. Même mécanisme en vue native et TPS, Recovery ON/OFF. Aucun correctif d'une touche physique.

## Capture utilisateur Static Shot

Capture externe du 18 octobre, lecture seule `QUERY_LIMITED_INFORMATION | VM_READ`, 37 855 observations et zéro reprise de lecture échouée. Fichier privé `v154-live-dash-actions-20261018-101659.jsonl`, 30 224 021 octets, SHA256 `49AD6CD8D8706E895404A36AFDE5782E891436665E70436FB79D01BF83A46F39`. Cinq activations observées pendant les essais autour de 10:28:45–10:29:01, heure de Paris : **trois Dash Aim Level1, deux Dash Fire Level1**. Ne pas inventer un troisième dash Fire.

Les deux premiers Dash Aim remplacent la capacité DualPistols Aim ; après leur fin, la liste des capacités est vide alors que `GetPlayerLookInput` reçoit encore un vecteur de norme proche de 1. Lors du deuxième cas, cette absence dure au moins une seconde. La capture prouve le manque de capacité, pas à elle seule la valeur du booléen d'action AIM au même instant : cette valeur n'était pas capturée individuellement. Le correctif exige donc encore l'état natif AIM courant, sans le fabriquer.

Lors des Dash Fire, DualPistols Aim et StaticShot Aim sont suspendues (état 3), tandis que Dash Fire est actif (2), puis terminé (5) ; les capacités reprennent ensuite et StaticShot Finish apparaît. Les champs bruts privés étiquetés `masks` dans le collecteur ne constituent pas une preuve du type de chaque champ. En particulier, `170` contenait un pointeur : aucune interprétation en masque d'actions ni écriture de ce champ n'est utilisée.

L'utilisateur confirme le défaut de balle normale également en TPS. Il n'y a pas encore d'essai de référence sans ASI démontrant une régression causée exclusivement par le mod.

## Reprise native de visée

Le Tick de dash déjà intercepté, `5B72B0`, continue de s'exécuter intégralement avec son dt natif. Son propriétaire/localité/variante restent validés comme auparavant. Un suivi de valeurs par joueur enregistre l'activation courante, son horloge et son identité locale ; jamais de déréférencement ultérieur de l'ancien objet Ability.

Après le getter natif de visée existant, le correctif peut appeler **une seule fois** `69FBD0`, DoAimAbility, sur les pistolets natifs appartenant au personnage courant. Conditions : Strife local normal à pied, autorité, vivant, jeu non pausé/hors cinématique ; dash observé depuis au plus trois secondes ; déplacement au sol ; composant de capacités réciproque et liste vide ; action native AIM 31 toujours tenue. Une capacité active/suspendue gagne toujours. Le démarrage est protégé contre la récursion. Rejet natif : aucune boucle de nouvelles tentatives dans cette activation.

La demande AIM est lue par l'interface d'action déjà enregistrée : `actor+8D0`, virtuelle `18` égale à `669D30`, qui lit le tableau natif via `635100`. Aucune touche, valeur XInput, axe, relâchement synthétique, rotation ou bit AIM/MOVE modifié.

L'accès aux pistolets reproduit le chemin natif `5D8130` : classe d'interface à `3811D70`, `GetInterfaceAddress` `ABA640`, virtuelle `30` GetRangedWeapon, puis contrôle de classe et de propriétaire du composant. `3811D20` est MayhemRangedWeaponComponent. Le composant doit être `MayhemWeapon_DualPistols_Blueprint_C`, propriétaire `E8` égal au personnage. Son `AimAbilityClass` à `618` est le Start Ability natif ; les assets montrent qu'il appelle StartAiming puis démarre DualPistols Aim avec les données d'origine. **CC0 n'est pas utilisé : le fallback CC0 de la notification concerne un autre type de personnage.**

Si l'action AIM n'est plus tenue, si le getter/interface/composant ne correspond pas au contrat ou si une autre capacité reste présente, la reprise n'est pas forcée. Ces cas restent une limite de la réparation, à relever au test.

## Tir Standard imposé par Dash Fire

Les séquences Strife Aim Dash Fire F/B/L/R référencent la classe Standard. Les deux notifications SpawnProjectile du cas F ont explicitement `ProjectileOverride = MayhemProjectile_Standard_Blueprint_C` dans leur propriété sérialisée. Le code de notification `5D8130` lit cet override à `50` et le transmet au spawn natif. Cette voie peut fonctionner alors que le cycle du tir spécial est suspendu.

Une seule nouvelle interception concerne cette notification native. Elle s'abstient de produire le projectile forcé seulement si : propriétaire local Strife à pied valide ; capacité Ground Dash active reconnue et réciproque ; animation exactement l'une des quatre séquences ; notification SpawnProjectile typée et appartenant à cette animation ; override exactement Standard ; demandes de tir natives 18/20 encore tenues et sélections correspondantes reconnues comme spéciales. L'équipement est lu dans le composant/slots natifs observés, avec classe et propriétaire réciproque. Aucun binding physique n'intervient.

**Comportement visé :** les balles Standard ajoutées par l'animation disparaissent ; les capacités spéciales suspendues continuent leur reprise native après le dash. On ne transforme pas arbitrairement un projectile Standard en Lava, faisceau ou tir chargé, et on n'invente ni coût, cadence ni trajectoire. Les flashes/animations natifs peuvent continuer. Cette build ne promet pas un tir spécial continu pendant toute l'animation.

Reconnaissance actuelle : Static, Charge, Gravity, Lava, Nature, WorldEnder. Standard/Hotstreak Standard inchangés. Tir simultané Standard + spécial, classe/animation inconnue, absence de demande tenue, personnage distant, cheval/Ultime et notifications hors dash : original. Aucun acteur de projectile, angle, offset de caméra ou ressource modifié directement. Les nouvelles signatures sont vérifiées dans l'EXE exact avant installation du hook ; un échec conserve le natif et ne retire pas Recovery.

## Nettoyage et conservation

Le diagnostic V1.53 des axes physiques et ses caches/budgets sont retirés, ainsi que l'appel d'observation XInput correspondant. La transformation XInput restante est identique ; la correction utilise l'action AIM et le cycle de capacités. Un suivi court par joueur et deux compteurs bornés remplacent le diagnostic. Pas d'option/clé/hotkey supplémentaire.

Recovery conserve les mêmes gardes, sa détection de décélération terminée, sa demande EndAbility native et sa protection de dash en combat. Le suivi et la reprise sont indépendants de l'interrupteur Recovery. Les autres modules source, la balistique/cible commune, la caméra/DOF V1.54, roues, coop, ressources et paramètres INI sont conservés. 119 clés, 48 actions et 119 ZIP antérieurs à préserver ; main/archive/v1.43 inchangés.

Une suite DashCombat vérifie les limites de cycle : absence de dash, action relâchée, capacité prioritaire, changement d'identité, expiration/retour d'horloge, récursion évitée par consommation unique, réutilisation d'activation, isolation des suivis, munitions standard/spéciale/inconnue et demandes mixtes. Les 28 suites existantes restent nécessaires. Ce contrôle de politique et la compilation ne font pas exécuter les hooks dans le jeu.

## Tests utilisateur et arrêt

1. Strife, vue du dessus, déplacement + visée maintenus : trois dashs sans tir, puis continuer de déplacer la visée. Refaire Recovery OFF puis ON. Attendu : aucun besoin de relâcher/reprendre la commande de visée.
2. Refaire en TPS. Le correctif doit avoir la même règle ; aucun saut de caméra ajouté.
3. Static Shot, puis Charge Shot : déplacement + visée + tir + dash, vue native puis TPS. Attendu : aucune balle Standard imposée ; reprise du tir spécial après la fin du dash. Contrôler normal/Hotstreak et Recovery OFF/ON.
4. Standard Shot : le tir natif pendant le dash doit rester présent. Tester séparément le maintien simultané de deux demandes ; la combinaison Standard + spécial reste une limite déclarée.
5. Coop : même essai avec Strife en P2 ; vérifier que l'autre joueur n'est pas affecté, puis roues/équipement.
6. Contrôle court des acquis : Lava en cloche, Charge Shot en recul/Hotstreak, Static/Beam alignés, obstacle devant/derrière la caméra ; chapitre 8 immobile, bascules TPS/vue native et cinématique pour le DOF V1.54.

Arrêt de l'expérience : crash, capacités répétées ou bloquées, balle manquante hors des quatre animations concernées, perturbation de War/P2/roues. Revenir à la sauvegarde précédente et conserver le journal. Pas de répétition d'injections ni de nouvelle correction d'axe physique pour masquer un échec. Le prochain audit doit distinguer un contrat non reconnu d'une demande native rejetée.

Installation seulement jeu réellement fermé, sauvegarde et INI personnel conservé. Aucun lancement automatique. Les premiers contrôles privilégiés ont été interrompus par un échec du contrôle automatique lié à une limite d'utilisation ; aucun accès mémoire exécuté par cette action. Après la période indiquée, la vérification a pu être exécutée et a constaté que le jeu était fermé. Ne pas confondre cela avec un résultat de gameplay.
