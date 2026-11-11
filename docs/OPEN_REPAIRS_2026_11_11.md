# Reprise du 11 novembre 2026 — suite locale de V1.159

Base stable locale : V1.159, commit `d6414404ba268add00834323c6da064af5d5ce42`. Debug local avancé sur cette base ; main/archive distants non modifiés. Aucun téléchargement, lancement du jeu, relevé mémoire, build ou installation dans cette reprise.

## Préparé localement, non compilé et non validé en jeu

- **Saut à cheval :** le filtre de MountedJumpFeature ne reconnaissait que Despair et Ruin. Mayhem (Strife natif) et Rampage (bonus War), précisément signalés par l'utilisateur, sont ajoutés. La possession locale réciproque, le composant de mouvement et son contrat natif restent obligatoires ; la commande reste l'action Jump réaffectable. Test de régression ajouté pour le déclenchement sur les quatre classes et le refus sans possession réciproque. Même liste de classes que le service des passifs du cheval déjà présent. Critère en jeu : une pression donne un saut avec chacune des quatre apparences, sans répétition à l'atterrissage.
- **Visée sans tir :** défaut manette D-pad bas seul (`AimWithoutFirePadButtonA=2`, second bouton OFF). Construction du Store, reset et lecture de clé absente utilisent la même constante ; INI de référence adapté. Tests adaptés pour ce défaut et la conservation d'un OFF explicite ou d'un raccourci personnel. L'INI du jeu n'est pas modifié. Ce changement de défaut ne remplace pas une valeur déjà enregistrée.

## Encore à traiter

- **Explosion d'activation Stoneskin sur Strife :** absence intermittente signalée. Distincte de l'aura Molten persistante au sol, dont le dernier essai était passé, et du matériau de l'écharpe accepté. Le code efface `transition.pending` avant de connaître le résultat de Spawn ; un refus de création est donc abandonné sans reprise. Piste identifiée par lecture, pas cause démontrée du défaut visuel signalé. Les dernières lignes locales disponibles (9 novembre) déclarent des créations acceptées ; elles ne prouvent pas le rendu. Préserver les effets acceptés, le geste, les matériaux et les dégâts natifs ; pas de nouvelle émission répétée ajoutée ici.
- **Coop/P2 : VALIDÉ par l'utilisateur le 11 novembre.** En réponse au point regroupant caméras, transformations, roues, raccourcis, gameplay et effets/HUD : « cest verifier tout est ok ». La validation coop n'est plus différée. Aucun relevé supplémentaire demandé.

Les réponses « ok » aux points 1 à 3 confirment l'accord sur les corrections/priorités présentées ; elles ne sont pas comptées comme un essai en jeu de changements locaux non compilés. Reste à finaliser l'explosion Stoneskin, puis compiler et livrer le lot avec le saut des quatre chevaux et le nouveau défaut de visée.

## Contrôles et protection

Contrôles locaux limités au diff et à la cohérence des paramètres ; les tests C++ ajoutés/adaptés ne sont pas exécutés ici. Compilation Windows uniquement sur GitHub lors du prochain lot. Les autres défauts, les 55 actions Hotkeys et les deux entrées configurables de visée sont conservés ; aucun patch de touche imposée dans le moteur. Code Stoneskin, ancien ZIP V1.108 absent et note REPRISE_AUDIT préexistante laissés tels quels. Les archives déjà disponibles ne sont pas retéléchargées.
