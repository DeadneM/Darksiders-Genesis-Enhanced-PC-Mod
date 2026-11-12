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

Le feu de l'écharpe, les explosions/cendres et l'aura de lave au sol sont trois chemins distincts : ne pas les présenter comme un seul correctif. Pas de nouvelle panne de l'aura démontrée depuis les essais positifs précédents. Aucun autre défaut fonctionnel confirmé ajouté à cette file. Main, configuration, raccourcis, fichiers du jeu et anciens ZIP restent intacts.
