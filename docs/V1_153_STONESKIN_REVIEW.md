# V1.153 — Stoneskin et Player Movement

Le lot a passé les contrôles GitHub. Le [reçu de livraison](V1_153_DELIVERY_RECEIPT.md) fournit le ZIP et les résultats.

Base cumulative V1.152. L'utilisateur a validé le rework dash/stomp et demande une case activée par défaut dans Gameplay → Player Movement. Il signale encore un rendu Stoneskin intermittent et demande de réduire le coût des changements de ressources. Le dernier journal accessible contient le démarrage de V1.152, sans activation Stoneskin ou mesure de performance correspondante. La cause précise de son dernier incident reste à confirmer.

## Méthode conservée et corrections

Le moteur crée une instance de matériau propre au personnage pour chaque emplacement pris en charge. Le matériau de départ, ses références natives et le statut actif déterminent la restauration. Cette méthode permet de changer le rendu pendant la durée du pouvoir.

Trois défauts ont été identifiés dans le code. Un refus provisoire de matériau ou de fabrique était mémorisé tant que l'adresse restait identique, y compris lors d'une nouvelle activation. Une lecture incomplète du statut pouvait retirer le visuel ou faire passer Molten à la variante de base. Enfin, la découverte attendait encore le matériau de corps Molten, alors que le rendu actuel utilise toujours le corps minéral de base.

Les refus temporaires peuvent désormais être réessayés après une seconde. Une nouvelle activation réouvre immédiatement l'essai. Une lecture incomplète conserve le visuel existant jusqu'à une observation concluante. La dépendance de corps Molten inutilisée a été retirée. Les identifiants des emplacements respectent la comparaison de noms native, qui accepte les différences de casse.

## Coût de fonctionnement

L'observation approfondie est limitée à une fois toutes les 50 ms lorsque le visuel est actif et toutes les 250 ms au repos. Un changement de propriétaire ou une activation force une vérification immédiate. Les appels de caméra répétés partagent cette observation. Les fonctions de matériaux et leurs paramètres vérifiés sont mis en cache avec leurs identités natives. Leur entrée native est encore contrôlée à l'utilisation. La découverte générale terminée est consultée moins souvent pendant les périodes sans ressource Stoneskin manquante.

Les chargements ciblés conservent leur budget existant. Les nouvelles statistiques de fin de session comptent les observations, les retours rapides et les recherches de fonctions. Les tests vérifient les nombres de recherches et d'observations. Ils ne mesurent pas les FPS ou la consommation électrique du jeu.

## Menu

La case Tap / hold air dash est activée par défaut. La nouvelle clé Features.AirDashRework vaut 1 en l'absence de réglage personnel. Sa désactivation rend les prochains appuis au chemin natif. Un geste déjà commencé conserve sa fin et la restitution de ses paramètres.

Les réglages de réception sont placés sous la case Heavy landing movement recovery. Les expériences de récupération de dash et de coffre disposent chacune de leur case. Les descriptions ont été raccourcies et le délai partagé apparaît lorsque l'une de ces expériences est activée.

## Contrôle et essai

Les régressions visées sont le rétablissement sur une adresse de matériau identique, l'arrivée tardive d'une référence native forte, la lecture incomplète de statut, la comparaison des emplacements, l'invalidation des caches et les changements de propriétaire. Le test de cadence soumet mille observations au même instant et attend une seule vérification approfondie. Le test de fonctions soumet cent demandes et attend une recherche de fonction et une validation de paramètres.

En jeu, tester Stoneskin avec Strife sur plusieurs armures, à plusieurs reprises dans une mission et après un chargement. Vérifier le rendu pendant le pouvoir et son retrait à la fin. Comparer la fluidité au même endroit avec V1.152. Pour le menu, vérifier ON/OFF et la sauvegarde de Tap / hold air dash, puis les réglages de réception et les deux cases de récupération. Les résultats de compilation et de livraison seront enregistrés après les contrôles GitHub.
