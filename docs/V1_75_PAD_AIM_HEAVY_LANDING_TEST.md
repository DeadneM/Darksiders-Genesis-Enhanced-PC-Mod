# V1.75 — visée manette et déplacement après réception lourde

## Objectif, base, statut

Lot cumulatif sur **debug**, base V1.73 validée dans les essais rapportés. Inclut la visée manette V1.74 construite/contrôlée et un nouveau candidat ciblé sur la **réception lourde automatique après chute**, conformément à la clarification utilisateur. **PRÉPARÉ / EXPÉRIMENTAL** avant CI, pas encore de validation en jeu. Le reçu complétera les contrôles réellement obtenus. L'utilisateur installera manuellement à la fin de sa session ; aucun remplacement automatique ni fermeture à demander.

Critère : permettre le déplacement après l'impact d'une réception lourde, en vue native comme en TPS, en conservant les autres animations et fonctions acquises. Le candidat ne force aucune fin d'action et ne change ni timer, vitesse, mouvement en vol, visée, munition ou touche. OFF par défaut.

## Changements

- Gameplay → **Aim without firing** : maintien configurable par un ou deux boutons, OFF/OFF initialement. Action native AIM/AIM_RELEASE, sans Ctrl ni tir ajouté ; contrôleur propre à chaque joueur, direction via sa caméra en TPS. Règles et critères [V1.74](V1_74_NATIVE_PAD_AIM_TEST.md) conservés.
- Gameplay → Player movement → **Heavy landing movement recovery (experimental)** : autorise le déplacement normal pendant la suite d'une réception lourde, après au moins150ms dans l'action et50ms de contact natif au sol. Exclus : dash, attaques plongeantes, Rampage, knockdowns, changement de personnage, cheval et ultime. Classes exactes HeavyLand Strife/War, propriétaire local et mouvement réciproques vérifiés.

La forme normale des deux personnages est ciblée. Trois réceptions Strife ont été observées, **aucune War_HeavyLand** dans cette capture : le chemin War préparé d'après sa classe/son asset reste à tester. Les deux nouveaux comportements restent expérimentaux, même lorsque compilation et tests isolés réussissent.

## Pourquoi cette approche Recovery

La capture contient7480échantillons, trois incohérences rejetées, trois HeavyLand Strife durant environ0,9–1,2s avec mode Walking et hauteur stable. L'action native MOVE29 reste fausse pendant ces157échantillons : ne pas attendre cette sortie déjà bloquée pour libérer le mouvement.

Le code natif OnStart RVA5A3670 ajoute MOVE29 au masque AllowedActionsFlags uniquement si AllowPlayerMovement est vrai. La construction de l'entrée de mouvement (notamment67F105) consulte aussi ce booléen. Le candidat met donc à disposition ces **deux verrous de mouvement précis**, après contact, sans appeler Finish ou modifier les permissions d'interruption. L'ancien candidat dash agissait sur une autre permission et reste supprimé ; aucun ancien ID Recovery n'est réactivé.

La capture ne prouve pas que le root motion ou un autre consommateur acceptera effectivement le déplacement : **efficacité et rendu à confirmer en jeu**. C'est le critère de l'essai ; un journal indiquant l'ouverture des deux verrous ne vaut pas validation de déplacement.

Signatures de l'exécutable exact, callback game thread, classe, état actif, possession, instigator, composant de mouvement, vie et identité faible contrôlés avant les deux champs modifiés. Aucun nouveau hook Recovery. Les autres bits du masque restent conservés. OFF, perte de focus, roue, pause, cinématique, sortie de forme ou de l'action restaurent les valeurs précédentes tant que l'identité est encore valide ; objet détruit/réutilisé abandonné sans écriture. Aucun appel de restauration depuis le thread d'arrêt.

## Essais groupés et rollback

1. Visée seule : un bouton dans chaque champ séparément, puis deux boutons ensemble ; tenir/relâcher sans tir. Native/TPS, Strife/War ; en coop chaque manette pour son joueur. Tir normal/spécial et dash conservés quand on les utilise volontairement.
2. Réception lourde : choisir une chute assez haute qui provoque l'animation, maintenir le déplacement après impact. Comparer OFF/ON, d'abord Strife puis War, vue native puis TPS. Attendu : déplacement plus tôt après l'impact, sans glissement imposé, perte des sons/effets de réception ou blocage suivant. Refaire une chute sans toucher au déplacement.
3. Gardes : dash au sol/en l'air, réception ordinaire, Rampage et changement de personnage restent natifs ; aucune reprise en vol. OFF, menus, roues, cinématique et Alt-Tab ne laissent pas le déplacement forcé ou une visée tenue.
4. Régression courte : roues, rangement, équipement maintenu P1/P2, échange de sticks, munition spéciale pendant le dash. INI et raccourcis après redémarrage.

Pour revenir au comportement précédent, décocher Heavy landing movement recovery et mettre les deux commandes de visée sur OFF. Rollback complet : restaurer le binaire V1.73 avec sauvegarde préalable, conserver l'INI personnel ; les nouvelles clés sont optionnelles et ignorées par l'ancienne version.

## File honnête

**Les six capacités ne sont pas échangeables dans ce build.** Lookups Blueprint limités à leur propre personnage, deux squelettes, composants Strife spécifiques ; travail d'adaptation encore nécessaire. Le OFF complet des pistolets reste ouvert ; une frontière distincte de rendu a été identifiée mais n'est pas modifiée. [Preuves de recherche](V1_75_WRATH_HEAVY_LANDING_PISTOL_RESEARCH.md). HUD général en surveillance après non-reproduction, autres FX P2 reportés. Aucun sélecteur inerte ou correctif global ajouté pour ces sujets.
