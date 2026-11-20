# V1.172 — livraison debug

Base V1.171 VALIDÉE. Nouvelle option d’appel aérien CONSTRUITE / CONTRÔLÉE / EXPÉRIMENTALE, pas encore validée en jeu. Source `025c79e1b44d8e2e022ce450d7a3cb6f0b4d93b6`, archive `9efaa3116784c80041e7e1c720296ee00c25bdc8`. Main/release V1.168 préservées. Aucun lancement ou remplacement des fichiers du jeu pour cette livraison.

Gameplay → Horse and riding → **Summon horse in the air**, juste sous Horse Everywhere, **OFF par défaut**. L’action native est conservée, indépendamment des touches ; les règles de zone et de déblocage restent en place. Le résultat de la recherche de sol est adapté uniquement pendant l’appel aérien du joueur local. [Contrats, limites et rollback](V1_172_REVIEW.md).

Inclut l’optimisation CPU préparée du suivi PS sans shader de visée reconnu. Aucun gain de performance ni correctif des micro-saccades annoncé validé. Les apparences, mouvements du cheval et autres fonctions validées ne sont pas modifiées par cette optimisation.

## Vérification

- [GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44537363586) : **86/86 tests**, zéro avertissement/erreur C++ détecté. Cas sol/air, OFF, plané, P1/P2, joueur étranger, possession rompue, mauvais appelant, pause/cinématique, position invalide et persistance vérifiés dans les fixtures.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9efaa3116784c80041e7e1c720296ee00c25bdc8/downloads/DarksidersGenesis_V1.172_AIR_HORSE_SUMMON_TEST.zip) : 2486510 octets ; SHA256 `1DFE88E65C8E1504C8416C56A81A8C0D9EF79569E2C1BCDF4C3872A9EC3AAF1A`.
- DarksidersGenesisMod.asi et dxgi.dll AMD64 seuls à la racine, CRC et blob GitHub vérifiés ; pas d’INI/README inclus.
- Lien public HTTP 200 et sélection V1.172 par l’updater contrôlés. Un seul nouveau ZIP récupéré via Git ; aucun téléchargement d’ancienne archive.
- Microsoft Defender : analyse des deux binaires terminée, aucune détection les concernant. Ce contrôle ne remplace pas le test en jeu.
- 236 anciennes archives intactes, 237 au total. 259 clés de référence : les 258 anciennes valeurs et les 55 actions Hotkeys inchangées. Aucun INI personnel modifié.

[Preuves de livraison](evidence/V1_172_DELIVERY_CHECK.json).

## À tester

1. OFF : appel/démontage au sol comme avant. ON : appeler pendant un saut ou une chute, Strife puis War, avec les commandes habituelles ; vérifier apparition à la hauteur actuelle et contrôle après la transition.
2. Tester le plané et les combinaisons avec Horse Everywhere : cette case seule ne doit pas autoriser les zones interdites.
3. Vérifier les quatre skins, saut/marche dans le vide et un chargement. Coop locale en dernier.
4. Comparer les micro-saccades dans une même scène ; aucune conclusion sur le seul pourcentage GPU.

Rollback : décocher l’appel aérien, ou remettre les deux binaires V1.171 en conservant son INI. Pas d’installation automatique. Suite : validation utilisateur du comportement aérien avant toute promotion stable.
