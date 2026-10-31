# Statique : enquête avant le premier signalement

31 octobre 2026. **AUDIT HISTORIQUE CONTRÔLÉ ; cause initiale non isolée en jeu.** Aucun code de tir, fichier du jeu ou INI modifié ; aucune compilation, installation, capture mémoire ou relance. Travail sur `debug` uniquement.

La référence demandée est le comportement natif avec seulement une caméra TPS. Il faut examiner les versions antérieures au premier signalement, sans attribuer ce premier défaut aux corrections qui ont suivi.

## Borne chronologique

Le premier retour « un ennemi de très loin est privilégié face à une caisse/pot » arrive après les essais Recovery de V1.126 et avant la demande d'installation V1.127. Le journal archivé correspondant est celui du 28 octobre, de12:30à12:57, V1.126. Le fait que le défaut soit remarqué à cette étape ne prouve pas qu'il a été introduit dans cette version.

Les filtres Statique de V1.129/130, l'aide ajoutée V1.131–133 et le remplacement du sélecteur/cache V1.134–135 sont donc exclus comme origine du premier signalement. Ils ont pu modifier ou aggraver le comportement ensuite. L'audit précédent de V1.135 reste pertinent pour l'état actuel, pas pour dater la première apparition.

## Première passe : historique du code

Dix instantanés ont été comparés : V1.52,53,79,108,116,124,125,126,127,128. Les trois fichiers `ProjectileAimFeature.cpp`, `NativeTpsFeature.cpp` et `TpsAimDirection.h` sont **strictement identiques dans Git de V1.79 à V1.128**, pas seulement semblables après suppression des commentaires. Les cinq fonctions principales de fourniture du rayon et de regard dans `DarksidersGenesisMod.cpp` sont également identiques entre V1.79 et V1.126.

| Étape | Modification pertinente constatée |
| --- | --- |
| V1.32 | Remplacement de la direction souris2D native par une cible issue du helper de collision3D, dans le contexte TPS autorisé. |
| V1.46 | Ajout de la correction de rotation de projectile vers la caméra TPS. |
| V1.50–52 | Généralisation au point3D, aux capacités et à Lava ; à partir de V1.52, la direction horizontale commune reste corrigée dans les deux modes3D ON/OFF. |
| V1.53 | Correction du vecteur de la **branche libre** du faisceau, au retour61D5E5. Les branches avec cible affectée restent natives. |
| V1.79 | Ajout d'une exclusion précise World Ender. Elle exige son arme et son faisceau propres ; ce n'est pas un réglage de Statique. |
| V1.80–126 | Aucun changement de ces trois fichiers de visée. Aucun nouveau réglage explicite d'angle/rayon du sélecteur natif trouvé dans cette séquence. |

Les changements récents revus concernent notamment les gestes/pouvoirs transférés et leur présentation, les apparences, le curseur et Recovery. Le helper World Ender transféré est gardé pour War et ce pouvoir précis. Le masquage du curseur change un enum de présentation, pas ses coordonnées. Le suivi d'exécution V1.82 ajoute une hauteur pendant une exécution, avec retour vers un décalage nul hors exécution. Cela constitue une preuve négative au niveau source, pas une garantie d'absence de tout effet indirect.

Les journaux V1.116,123,126 montrent aussi le même cadrage initial FOV110, distance0,15, pitch−12, hauteur0. Dans le profil V1.126 conservé,3D est activé, correction verticale+12 et apparence Statique sur Native. Ces données ne démontrent pas l'état de chaque option à chaque instant de la session.

## Deuxième passe : le défaut ancien concret

Le traitement natif `61D410` suit cet ordre :

1. À61D492, si un composant cible est affecté au faisceau (`+620`), il calcule directement sa direction vers la position de ce composant.
2. Sinon, un point cible explicite (`+628`) peut encore déterminer sa direction à61D544..61D588.
3. Ce n'est qu'en l'absence des deux qu'il utilise la direction libre du root/socket, à61D58D..61D601. Notre correctif V1.53 intercepte seulement ce chemin, au retour61D5E5.

À V1.126, l'affectation native de cible reste intacte : création du faisceau à69818F..6981CB ; mise à jour à5B9F80..5B9FFF ; setter617B90. Le sélecteur natif conserve son propre choix et sa direction issue des commandes natives du personnage. **Une cible affectée peut donc détourner le faisceau du rayon TPS sans passer par notre correction de direction libre.** Ce mécanisme existait avant le premier signalement et peut expliquer une déviation latérale importante, sans augmentation récente de la portée ou de la force d'aide.

Il s'agit d'une incohérence ancienne entre deux chemins, pas de la preuve que V1.53 aurait augmenté l'aide native. Elle peut rester discrète tant qu'aucun ennemi admissible ne déclenche l'autre branche. L'influence réelle du cadrage et des commandes sur le choix de cet ennemi exige une comparaison en jeu.

Une deuxième différence ancienne concerne les destructibles : le remplacement V1.32 utilise le helper3D571E50 et sa trace Projectile19. Le helper2D d'origine5719A0 possède une branche facultative de sélection sous le curseur qui vérifie la réponse du composant au canal AutoTargeting18. Le profil original Destructible est Overlap sur Projectile : une trace de blocage peut ignorer la caisse/pot et poursuivre derrière. Cette différence peut expliquer un destructible ignoré ; elle ne suffit pas à expliquer à elle seule un rayon qui part latéralement vers un ennemi.

## Ce que les anciennes preuves permettent de conclure

Le journal V1.126 compte1708corrections de direction libre du faisceau et3837corrections de capacité en P1. Il prouve que les chemins TPS ont été utilisés, mais ne contient ni l'identité du faisceau/cible au moment précis du défaut, ni l'ordre des collisions, ni le nombre de tirs passés par la branche verrouillée. Ces compteurs regroupent les munitions de la session.

Le profil partagé7500/90/45 observé ultérieurement n'était pas mesuré dans cette session historique. Il ne doit pas être présenté comme une mesure rétrospective ni comme la preuve qu'un angle aurait été augmenté avant le signalement.

**Les deux passes concordent : aucun renforcement récent de l'aide n'a été trouvé avant le signalement ; en revanche, une autorité de visée divisée entre rayon TPS libre et verrouillage natif existait déjà.** C'est désormais la piste historique prioritaire. La première version où le ressenti a changé reste inconnue, faute de comparaison avec un ancien ASI dans la même scène.

## Suite retenue

Examiner ensemble l'affectation de cible native et la direction libre, puis comparer une vraie référence de ciblage natif sous caméra TPS. Ne pas corriger seulement le vecteur libre, réintroduire le filtre de contact rejeté, ni raccourcir la portée pour cacher le verrouillage. Les constantes de portée, règles de validité natives, chaînes et contrôles validés doivent être conservés. Le recentrage inconditionnel ajouté en V1.134–135 reste un problème distinct à retirer pour cette comparaison.

[Matrice des commits, empreintes et journaux](evidence/PRE_REPORT_STATIC_HISTORY_2026_10_31.json). Aucun nouveau test de compilation : source inchangée. Le résultat est un diagnostic historique vérifié, pas une réparation validée.
