# Retour complémentaire reçu — 19 octobre 2026

**Grappin V1.63 : « marche », VALIDÉ dans l'essai rapporté.** Rangement : les lames de Strife restent en main avec l'ancien délai commun. Décision finale : garder les raccourcis pour toutes les armes, ajouter trois délais indépendants lames Strife / pistolets Strife / épée War. Cette décision remplace l'exclusion envisagée des lames/raccourcis. L'utilisateur a ensuite demandé de continuer ; l'attente V1.63 indiquée plus bas est levée. Double saut encore ouvert, auto-check sans retour, autres acquis préservés. Voir [V1.64](V1_64_WEAPON_TIMERS_ULTIMATE_TEST.md).

---

# Retour utilisateur consigné — 19 octobre 2026

L'utilisateur précise avoir préparé cette liste avant la dernière réponse de livraison V1.63. La version exacte de ces essais n'est pas précisée : ne pas transformer ce retour en validation du correctif grappin ni de l'auto-check V1.63. Ses confirmations font foi pour les essais rapportés, sans étendre implicitement leur couverture.

| Fonction | Retour et statut |
| --- | --- |
| Raccourcis de rangement | « marche » — VALIDÉS par l'utilisateur |
| Netteté TPS | « marche » — VALIDÉE par l'utilisateur |
| Standard limité | « marche » — VALIDÉ par l'utilisateur |
| Portée du grappin | À tester sur V1.63 ; aucun résultat nouveau |
| Trois modes de double saut | NON VALIDÉS : « marche pas sauf en always on ou on peut faire trois saut en plus puis plus rien ». Always on n'est donc pas infini ; ne pas le déclarer fonctionnel. Les autres modes ne donnent pas le résultat attendu dans le retour. Cause non encore recherchée. |
| Son et effets du tir pendant le dash | Ont bien disparu — correction VALIDÉE par l'utilisateur |
| Auto check update V1.63 | Aucun retour fourni ; reste à vérifier |

## Demande à la file après le retour V1.63

Ajouter **Ultimate decrease rate**, réglage 0 à 10, **×2 par défaut**. Le sens demandé est celui de la durée de la forme ultime : valeurs de 0 à 1 raccourcissent la durée, 1 conserve la durée native, de 1 à 10 l'allongent. Exemples : 0.5 = moitié de la durée, 2 = double, 10 = dix fois. La borne 0 appartient au côté qui raccourcit ; ne pas l'interpréter comme Unlimited. Le calcul natif et la gestion de cette borne seront étudiés lors de la reprise. Ce réglage concerne la décharge/durée, distincte du gain de jauge déjà existant. Aucune modification des valeurs de l'INI ni des gains actuels pour enregistrer cette demande.

## Consigne de reprise

**ATTENDRE le retour utilisateur sur V1.63.** Pas de nouvelle modification de code, compilation, livraison, installation ou lancement du jeu pendant cette attente. Ensuite seulement : intégrer le retour grappin/auto-check, reprendre le défaut de double saut et le réglage de durée ultime. Préserver les quatre validations ci-dessus. Holsters montés, autres sprites P2, modèles alternatifs et refonte Action Recovery restent en file.
