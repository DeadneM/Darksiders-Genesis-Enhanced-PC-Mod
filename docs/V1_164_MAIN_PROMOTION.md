# V1.164 — validation et promotion sur main sans build

Le 12 novembre 2026, l'utilisateur confirme la marche aérienne : « c'est exactement ca », puis demande « ceci etant valider on passe a la suite, push sur main mais ne build pas encore ».

**VALIDÉ par l'utilisateur : marche/course native du cheval sur le sol invisible privé.** Maintien, sauts successifs et délai conservent le chemin V1.164 testé ; aucun nouveau comportement ajouté dans la promotion. Aucun essai exhaustif supplémentaire des quatre apparences/P2 n'est inventé.

Base debug validée : `7ee4312516fbcff23c3f04213748ac690165bf60`. Source compilée : `6e8fc9339454303f242cf82f7c111ac8b6241eff` ; archive : `1b081e9b34b27ff2082227469414a2d9e096a541`. Contrôles existants : 83/83 suites Windows GitHub, aucun avertissement C++ détecté. Source/tests/configuration/updater/workflows inchangés depuis ce binaire.

Main précédent : `d6414404ba268add00834323c6da064af5d5ce42` (V1.159), conservé dans l'historique et le checkout local de rollback. Promotion par avance rapide uniquement, sans force. Branche archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` et toutes les anciennes archives préservées.

Le ZIP V1.164 existant est réutilisé **octet pour octet**, sans renumérotation, recompilation ni réemballage : SHA256 `05141D09759CFA41B85E4CD64DDF9EBBE2BD93E2CADCFA3C2949FF346ABCA3DF`, 2 456 302 octets, deux binaires seuls à la racine. Son nom historique TEST est conservé pour ne pas créer un faux nouveau build. Aucun INI personnel, raccourci, sauvegarde ou PAK modifié ; aucun lancement/installation effectué. Pas de tag ni de release créés.

Le commit de promotion porte `[skip ci]` pour ne déclencher aucun workflow push sur main/debug, comme demandé. Aucun workflow désactivé/modifié durablement, aucun dispatch manuel. Le résultat distant et l'absence d'exécution seront vérifiés après le push. [Mécanisme documenté par GitHub](https://docs.github.com/en/actions/how-tos/manage-workflow-runs/skip-workflow-runs).

Suite sur debug uniquement, sans build pour l'instant : envers de l'écharpe Stoneskin Strife (double face en conservant le feu), puis optimisation guidée par mesures. Les réglages de moteur non démontrés et les essais de matériau non validés restent hors de main. [Livraison V1.164](V1_164_DELIVERY_RECEIPT.md) · [Sol privé](V1_164_REVIEW.md).
