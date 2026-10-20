## V1.73 — lot montures / DOF et retrait du Recovery rejeté

20 octobre 2026, debug uniquement. Base cumulative V1.72 construite et contrôlée, pas installée ; acquis utilisateur conservés. Nouveau retour V1.71 : **Dash movement recovery ne fonctionne pas dans l'essai utilisateur, candidat REJETÉ et retiré**. La prochaine recherche Recovery commencera par le **ground slam**, après le reste de la file, conformément à la demande. Aucun nouveau candidat Recovery dans cette version.

Lot : correction du filtre qui supprimait Despair/Rampage au chargement, maintien du choix même disponibilité bonus désactivée (comme les armures), distance DOF Native/TPS jusqu'à **5 000 m**, retrait du diagnostic périodique et du candidat Recovery sans effet. Le nouveau filtre de skins et le maximum DOF attendent leur validation en jeu. **PRÉPARÉ / EXPÉRIMENTAL**, compilation et paquet à contrôler.

Le dash revient exactement au code de V1.70 pour la visée, les tirs et leurs effets validés ; seule la phrase périmée du bilan est nettoyée. Politique, état, appels natifs, checkbox, réglage runtime/INI fourni et tests spécifiques du Recovery rejeté supprimés. Les anciens identifiants de raccourci 3 et 54 restent réservés/inertes pour préserver les affectations personnelles ; ils ne sont plus proposés à l'assignation. Une ancienne clé personnelle n'est ni relue ni réécrite par le mod. **154 réglages fournis, 55 identifiants conservés dont les deux retirés**. Aucun INI personnel remplacé.

Essais groupés de cette version :

1. Despair et Rampage : choisir, sauvegarde normale, recharger puis redémarrer ; vérifier sélection et modèle. Décocher Bonus mount skins et recharger : garder le bonus équipé avec disponibilité du menu native. Choisir ensuite le cheval de base : il doit rester de base après chargement.
2. Profils DOF Native/TPS : 20 / 1 250,5 / 5 000 m, quantité 0 / 0,6 / 2 ; saisie et rechargement, arène → mission/hub, chapitre 8 et transformations. Le flou V1.71 semble bon dans le dernier retour, couverture complète non supposée.
3. Régression courte du dash acquis : déplacement + visée + tir + dash avec munition spéciale, native/TPS. Pas de test Recovery demandé : le candidat est retiré.

OFF complet des pistolets reste ouvert ; couches de pose identifiées mais aucun masquage partiel livré comme complet. HUD général après refus d'arène verrouillée intermittent, autres FX P2 reportés. Suite : OFF par main et HUD s'il réapparaît, puis recherche ground slam. [Preuves du filtre de chargement](V1_72_SELECTED_MOUNT_LOAD_DOF_RANGE_TEST.md).

Archive prévue `DarksidersGenesis_V1.73_MOUNT_DOF_CLEANUP_TEST.zip`, quatre fichiers racine. Anciennes archives, main/archive, INI/raccourcis, PAK et sauvegardes préservés. Jeu fermé par l'utilisateur ; installation après contrôles avec sauvegarde, sans lancement par l'agent.

---
