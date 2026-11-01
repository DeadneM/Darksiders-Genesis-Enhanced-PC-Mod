# V1.138 — Stoneskin après chargement, compilation sans masquage de variable

1er novembre 2026. **CONSTRUITE / CONTRÔLÉE : 70/70 contrôles Windows x64, zéro avertissement/erreur MSVC. EXPÉRIMENTALE, NON VALIDÉE EN JEU.** [ZIP et preuves](V1_138_DELIVERY_RECEIPT.md). Base cumulative V1.136 et correction candidate V1.137, avec les mêmes fonctions. V1.137 a passé 70/70 contrôles, mais émis deux avertissements MSVC de nommage. Cette version renomme uniquement le paramètre de temps du scan pour les éliminer ; l'archive précédente reste intacte. [Périmètre fonctionnel et tests](V1_137_STONESKIN_ASSET_RECOVERY_TEST.md).

L'utilisateur confirme que le défaut chez Dagan concerne la peau de pierre et le feu. Il compare d'autres niveaux ; aucune validation du correctif à ce boss n'est annoncée. Les ressources tardives et la dépendance de corps incorrecte sont corrigées en source, les matériaux inconnus du niveau restent protégés. Le scan garde son budget et sa demande par joueur.

Objectif/critère : retrouver le revêtement Stoneskin de Strife après chargement sans changer statistiques, gestes ou restauration normale, puis contrôle Windows sans avertissement MSVC. La cause exacte de Dagan reste à vérifier en jeu. Statique reste ouverte : le relevé l'observe dans les deux vues, ajustement OFF ; la vue native avec ASI n'est pas une référence sans mod. Les chemins de visée utilisés sont conservés et l'option OFF appelle directement le sélecteur natif.

INI et 55 raccourcis inchangés ; aucun hook/API moteur supplémentaire, aucune installation automatique. Compilation Windows x64 sur GitHub uniquement ; ZIP cumulatif quatre fichiers racine. main/archive et toutes les anciennes archives conservées.

Tests regroupés : Stoneskin sur Strife dans une mission puis Dagan, premier lancement et suivants après rechargement ; corps/écharpe, début et disparition complète ; court contrôle Geyser/Rampage et Charge/Lava. Coop en dernier. Si le revêtement manque encore, les messages distinguent source absente et matériau natif protégé. V1.138 est expérimentale tant que ces essais ne sont pas confirmés.
