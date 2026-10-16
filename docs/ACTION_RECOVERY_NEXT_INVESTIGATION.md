# Action Recovery — priorité dash après captures V1.44

16 octobre 2026 : l'utilisateur limite désormais l'investigation **au dash uniquement**. Les coffres sont reportés. V1.43 reprend les défauts utilisateur et ne réintroduit aucune intervention recovery.


## Captures du 16 octobre : Strife

Deux captures ont été armées avant les nouveaux tests :22 puis17 instances distinctes du type natif DASH (pas un compte de touches),9877 puis9289 échantillons. La seconde est confirmée par l'utilisateur avec Strife. MoveInterruptDelaySec reste0 ; vtable observée0x27D31B8, Tick0x5B72B0, parent0x68D050. La lecture du vrai helper0x38 puis du weak pointer/AnimInstance résout les montages sans lecture hors taille. Le mode de déplacement(6,5) reste actif jusqu'à la fin native ; les champs de montage148/150 passent0/1 puis1/1, sans fin de fondu exploitable démontrée. La lecture de vélocité ne prouve pas le déplacement physique personnalisé.

Ces mesures identifient enfin le chemin dash, mais n'établissent pas un point sûr pour le terminer plus tôt. Prochaine mesure : déplacement réel et événements natifs de fin du dash, avant toute écriture. L'ancien prédicat de blend tail ne doit pas être réintroduit sur cette seule preuve. V1.44 ajoute les potions ; **aucune réparation Recovery annoncée**. [Audit détaillé](V1_44_NATIVE_POTIONS_AND_DASH_AUDIT.md).

## Ce qui est démontré

- V0.8A : autoriser MOVE alors que l'action reste RUNNING provoquait le glissement du personnage pendant l'animation de coffre. Approche **REJETÉE**.
- V0.8B : attendre AWAITING_FINISH sans identifier le vrai cycle n'avait pas d'effet utilisateur.
- Capture externe historique du 15 octobre, `audit-v131/v139-live-abilities.jsonl`, relue après la demande : **18 observations entre21:17:43 et21:17:55**, MoveInterruptDelay min=max=0. Elle contient **7 observations typées MONTAGE**, dont6 en état2 et1 en état5 ; **aucune observation typée DASH ou INTERACT**. Elle n'est pas une chronologie complète annotée des deux actions demandées. Une absence dans cet échantillon ne prouve pas qu'un type ne s'exécute jamais.
- Les lectures exploratoires helper+D8/+194 sont hors taille du véritable UMayhemMontagePlayer (0x38) et ne doivent pas être réutilisées comme états de montage.
- Essai V1.39 :12132 ticks et8927 observations locales mais **blendTails=0, normalFinished=0**. Le prédicat d'éligibilité de cet essai n'a déclenché aucune correction. Ces compteurs ne permettent pas de choisir entre mauvais chemin, condition inadaptée ou moment d'observation manqué.
- Le hook de cet essai a été retiré en V1.41 ; le menu signale l'indisponibilité. Modifier son ancien slider/INI ne raccourcit actuellement rien.

## Approche à retenir

Retrouver une **libération anticipée et complète du dash à un point sûr**, respectant la fin du mouvement natif utile. L'ouverture des coffres reste hors du travail actuel. L'événement précis doit être démontré avant de modifier sa fin : la fin visuelle de l'animation, la fin du déplacement forcé et la libération des commandes peuvent être distinctes.

Le correctif devra cibler l'action native du dash, quelle que soit la touche qui lui est affectée. Le réglage utilisateur pourra ensuite limiter le temps restant après ce point. Les raccourcis clavier/manette reconfigurables doivent rester hors du choix de la fonction ciblée : identifier l'action réelle, pas une touche supposée.

## Prochain test discriminant

1. Démarrer la capture **avant** les actions et conserver leur chronologie. Maintenir un déplacement à travers plusieurs dashs, puis comparer déplacement relâché/maintenu et noter le moment où le contrôle semble revenir. Aucun coffre à inclure dans cette phase.
2. Observer en lecture seule, à partir du joueur local validé, l'action active et son type, les transitions RUNNING/FINISHED, les conditions natives qui autorisent mouvement/attaque, les conditions de fin propres au dash et la fin du déplacement forcé et, pour les seuls vrais helpers typés, la lecture du montage valide. Si un champ ou un objet n'est pas identifié, le classer inconnu.
3. Distinguer : action encore active après son effet utile ; action finie mais mouvement/commande encore refusé ; reprise logique déjà autorisée avec impression de délai issue de l'animation. Cela détermine le bon étage à corriger.
4. Produire un correctif seulement si une condition et sa fonction de libération sont identifiées. Si la capture ne couvre pas les actions attendues, corriger le protocole de mesure avant un autre build d'essai.

## Critère d'un futur correctif

Comparaison OFF/ON sur les mêmes dashs : reprise mesurable plus tôt, déplacement et attaque de nouveau possibles, absence de glissement, aucune distance/vitesse de dash ou invulnérabilité conservée à tort, aucun effet sur cinématiques/roues/forme ultime. Une fin normale avec nettoyage est nécessaire ; appeler seulement une permission MOVE ne satisfait pas ce critère.

Statut : **PISTE ET PROTOCOLE**, aucune réparation validée ou nouvelle capacité de mesure active annoncée. Le prochain travail est cette capture synchronisée et le suivi de ses conditions natives.
