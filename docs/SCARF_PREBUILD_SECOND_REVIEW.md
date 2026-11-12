**État après build V1.165 : CONSTRUIT et CONTRÔLÉ, 83/83 suites réussies ; NON VALIDÉ en jeu. Le report de compilation a été levé par l’utilisateur pour ce lot. [Livraison et essais](V1_165_DELIVERY_RECEIPT.md). Les mentions de tests non exécutés ci-dessous décrivent la préparation historique.**

# Écharpe Stoneskin — seconde revue avant build

Base stable : V1.164 validée, main `e0ac8a44a9fc79425218c452efab7128acecb56f`. Candidat debug `20dc020911d0f1e80c7ebfff214f632684f1d7b1`. Le report de compilation reste appliqué.

## Deux lectures comparées

La première lecture a suivi le choix du shader, la copie de la hiérarchie de textures et les ressources natives. La seconde a suivi chaque slot : matériau hérité ou MID natif déjà retenu par le personnage, création privée, refus avant/après copie, fin de statut et restauration. Deux méthodes appliquées par le même agent, sans compilation ni essai en jeu.

Pas de nouveau défaut du code de production démontré par cette seconde lecture ; aucun hook ni correctif supplémentaire ajouté par précaution. En revanche, la simulation de test n'acceptait la copie que pour les slots 2/3 du skin standard, alors que l'écharpe Abyssale peut occuper le slot 1. Elle acceptait aussi n'importe quel matériau natif comme source au lieu d'exiger celui du slot concerné. Ces limites du banc de test sont corrigées, sans prétendre qu'elles étaient un bug du jeu.

Les nouveaux scénarios vérifient le matériau exact par slot, les valeurs des vecteurs transmises, l'absence d'écriture dans les sources partagées, la copie d'un MID natif retenu et sa restauration exacte, le slot de tissu Abyssal, puis le repli si la copie ne fournit pas l'alias d'émission attendu. Ce dernier cas ne doit pas répéter la copie complète pendant le même état actif. **Scénarios préparés, NON EXÉCUTÉS.** Le contrôle réellement effectué est la comparaison des changements et `git diff --check`.

## Coût et travail restant

Le nouveau travail coûte une copie complète par MID privé de tissu créé, puis trois réglages scalaires et, en Molten, trois vecteurs. Il ne doit pas faire de copie par image ni reconstruire le corps lors de l'arrivée tardive du tissu. La durée réelle de la copie et le coût GPU du shader double face restent inconnus. Aucun gain de FPS annoncé et aucun réglage global du moteur modifié.

Ordre de suite :

1. Quand un build sera demandé : compilation Windows et exécution des contrôles, puis essai face/envers, feu et extinction avec Native/Abyssal/bonus. L'ancien ZIP V1.164 est le rollback.
2. Mesurer les éventuelles saccades à l'activation et la fluidité en état actif, dans une même scène. L'optimisation se basera sur cette comparaison, plutôt que sur des réglages moteur génériques.
3. Régression du sol privé des chevaux et du lifecycle skin/chargement ; ce contrôle d'une nouvelle bêta ne remet pas en question la validation V1.164 déjà donnée.

Le feu de l'écharpe, les explosions/cendres et l'aura de lave au sol sont trois chemins distincts : ne pas les présenter comme un seul correctif. Pas de nouvelle panne de l'aura démontrée depuis les essais positifs précédents. Ce relevé ne couvre que l’écharpe ; les autres chantiers restent suivis dans ACTIVE_QUEUE.md. Main, configuration, raccourcis, fichiers du jeu et anciens ZIP restent intacts.


## Suite : limiter la recherche des ressources au moment utile

La copie rapide n'est pas substituée à la copie complète. Dans l'EXE local, son chemin 0x196EDD0 remonte les instances du matériau source ; il n'est pas démontré qu'il réinitialise les valeurs absentes de leurs overrides aux défauts du master. Le tissu standard de Strife ne redéfinit que Base_Color et Normal, alors que Gold redéfinit aussi Metal/Rough/Emissive, Emissive_Color_Texture et Emissive_Scroll. Une copie rapide seule peut donc laisser les valeurs étrangères héritées du parent Gold. La copie complète actuelle reste limitée à la création du MID privé ; aucune technique plus rapide équivalente n'est annoncée.

En revanche, une dépense de travail hors copie était démontrée dans RecoverResources : tant qu'une ressource manquait, le code contrôlait le contexte du chargeur et, si ce contexte était valide, pause/cinématique/roue avant de demander à Begin de refuser les essais déjà épuisés ou encore en délai. Cela pouvait continuer toutes les observations actives pour le tissu facultatif.

Le code prépare désormais une liste locale des seules ressources manquantes non épuisées dont le délai est écoulé. Si cette liste est vide, il revient avant les contrôles du chargeur et des menus. L'identité propriétaire/mesh et le scope monde/variante sont toujours rafraîchis AVANT ce retour ; changer de monde peut donc réarmer normalement la reprise. La méthode Begin reste l'autorité finale : délai initial 750 ms, pause/cinématique/roue, contexte valide, budget partagé 250 ms, trois essais espacés de cinq secondes, priorité et une demande par service conservés.

Scénarios ajoutés au banc C++ : contexte de chargeur valide, compteur de consultations de pause, arrêt après trois échecs, délai de nouvelle tentative, reprise arrivée à échéance et changement de monde réarmant les demandes. **Préparés mais ni compilés ni exécutés.** Relecture et diff sans erreur d'espacement uniquement ; ni coût CPU réel ni FPS mesurés. Il s'agit d'une réduction ciblée de vérifications dans le chemin de recherche, pas d'une optimisation générale du moteur.

Les sources partagées, le rendu pierre/feu et le sol privé des chevaux ne changent pas dans cette optimisation. Pas de nouveau réglage, hook ou diagnostic permanent. Toujours aucun build, installation ou téléchargement d'assets.
