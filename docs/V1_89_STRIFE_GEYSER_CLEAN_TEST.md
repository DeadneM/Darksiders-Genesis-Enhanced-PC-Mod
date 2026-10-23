# V1.89 — Geyser vers Strife, finalisation du candidat

**EXPÉRIMENTAL, finalisation préparée ; compilation et contrôles à consigner après CI.**

Base expressément validée : V1.87 pour le retour du réticule sans crash, War arm et Léviathan ; bras OFF, effets de lames et suivi d’exécution validés auparavant. Base cumulative V1.88 `4309438f8a87d95ef33ab14fe0a4bdc0f4dda245`, Geyser non validé en jeu. **Aucune validation supplémentaire supposée.** [Contrat natif complet et limites du prototype](V1_88_STRIFE_GEYSER_TRANSFER_TEST.md).

V1.88 a passé 53/53 suites mais le second contrôle gardait C6386 sur le nettoyage du marqueur du compagnon, après correction de la borne du marqueur de cast. La garde réciproque de propriétaire existe déjà, mais l’analyseur ne la propage pas à travers ForeignLaunch. V1.89 centralise tous les accès dans GeyserActor : seuls les index 0 et 1 rendent les deux adresses constantes ; tout autre index rend null. Lectures, écriture du cast et nettoyage consomment ce pointeur vérifié. Test explicite pour -1/0/1/2. **Aucun avertissement désactivé ni garde supprimée.** Geyser, Stoneskin et leurs arguments natifs inchangés.

Objectif : livrer le candidat Geyser avec tous ses contrôles et sans avertissement ASI. Réussite en jeu, encore à confirmer : Strife à pied en mission, emplacement débloqué réglé sur Blade Geyser ; l’attaque apparaît, touche les ennemis, consomme son Courroux et se termine en rendant les commandes. Vérifier vue native/TPS, chaque emplacement puis P2 si disponible ; revenir à Native et vérifier les trois pouvoirs Strife, puis le Geyser/Stoneskin propres de War. Le code de caméra, visée, tirs, souris/manette, roues et ressources validées n’est pas modifié.

Limites : pose de repos compatible plutôt que geste retargeté ; sons/effets propres au montage et companion rising slash de War non transférés ; le blast garde ses propres effets natifs. Assets déjà chargés nécessaires. Si le contrat échoue pendant le cast, fin native puis transfert Geyser bloqué jusqu’au redémarrage ; le premier coût peut déjà avoir été consommé. Revenir à Native et conserver le journal en cas d’échec. Les icônes de roue peuvent rester celles de l’emplacement d’origine. Les autres pouvoirs ne sont pas bloqués par cet arrêt.

Stoneskin : code de protection existant conservé. L’utilisateur ne confirme plus qu’un cristal existait avant ; son aspect n’est pas un bug confirmé. Les changements de matériaux et l’effet d’expiration du Blueprint natif ne prouvent pas à eux seuls l’origine de l’aspect observé. Aucune correction visuelle spéculative. Quatre transferts restants, réception lourde incertaine et autres FX P2 toujours dans la file.

168 clés INI et défauts identiques, 55 ID Hotkeys, 153 anciennes archives préservés. Branche debug seulement ; main/archive/v1.43 intacts. Installation manuelle après fermeture du jeu, garder son INI, ses raccourcis, son PAK et ses sauvegardes. Aucun fichier du jeu remplacé automatiquement et aucune relance.

Rollback : Native dans l’emplacement, ou ASI V1.87 validé avec son INI personnel. V1.88 reste archivée avec son avertissement déclaré, sans écrasement du ZIP. Suite : retour utilisateur sur Geyser avant validation, puis quatre autres transferts.
