# V1.139 — acquisition native par munition

**CONSTRUITE / CONTRÔLÉE : 70/70 contrôles Windows x64, zéro avertissement/erreur MSVC. EXPÉRIMENTALE, NON VALIDÉE EN JEU.** [ZIP et preuves](V1_139_DELIVERY_RECEIPT.md). Base cumulative V1.138 (`9a5e9b8`). V1.130 reste une référence explicitement validée pour les deux défauts de l'époque ; V1.138/Stoneskin reste en essai, pas promue stable. L'installation locale V1.138 a été rétablie après le comparatif sans mod, empreintes et INI conservés.

## Résultat qui motive le changement

L'utilisateur juge Statique probablement identique sans ASI (« identique je pense »). Le défaut est également rapporté avec Vanilla Aim et 3D ON/OFF. Aucun renforcement récent de la fonction native n'est donc établi. Le verrouillage forcé était vide pendant les tirs capturés. [Preuves et limites du comparatif](STATIC_LOCK_AND_BASELINE_2026_11_01.md).

Un défaut de couverture du réglage **par munition** est en revanche visible dans le code : une requête native provenant de DualPistols commun conservait systématiquement 100 %, même pendant un tir Statique réglé à une autre valeur. Les réglages de l'instance de munition et du profil commun pouvaient donc différer. Cela concerne l'option ajoutée après le signalement initial ; ce n'est pas une explication rétroactive de son origine.

## Correction limitée

- OFF : appel direct au sélecteur natif, sans lire la nouvelle correspondance ni demander le rayon TPS.
- Global : comportement V1.138 conservé.
- Par munition : une requête commune identifie la munition réellement équipée dans le registre du personnage, d'après les actions natives de tir primaire/secondaire 18/20. Aucune dépendance aux touches ou boutons physiques.
- Les octets d'action sont lus dans la disposition utilisée par le getter natif déjà vérifié : interface `actor+8D0`, thunk `669D30`, feuille `635100`, état `actor+E10+action`. Signatures et table d'interface vérifiées ; aucun nouvel appel moteur, hook ou pointeur conservé.
- Vérification du propriétaire, de la correspondance DualPistols `1038`/emplacement 5 et des munitions enregistrées 2/3. Les contextes inconnus gardent le calcul natif.
- Requête commune ambiguë (repos ou deux tirs de munitions différentes) : 100 % conservé pour cette seule requête. Deux tirs de la même munition restent identifiables. Les requêtes propres à chaque munition gardent leur réglage.

Seuls les angles sont multipliés pendant l'appel original et restaurés avant son retour. Ni cible imposée ni cache ; portée, poids, filtres, contacts, trajectoires, chaînes Statique et valeurs par défaut inchangés. Le point natif déjà consommé et les corrections Charge/Lava validées ne sont pas retirés. INI/55 Hotkeys inchangés. Aucun changement de Stoneskin, caméra, inputs XInput, gestes, sons, shaders, bras ou holsters.

## Critère de réussite et essai manuel

Avec Strife à pied en TPS, choisir **Par ammunition**, cocher l'ajustement, puis mettre Statique à 0, 50, 100 et 200 %. Relâcher puis recommencer chaque tir ; conserver autant que possible position, ennemi et direction. Les cibles acquises hors axe doivent varier avec l'angle demandé ; 100 % et OFF doivent retrouver la référence sans réglage. Les chaînes électriques déjà créées ne sont pas supprimées par le curseur.

Comparer ensuite Standard dans l'autre emplacement avec une valeur différente : chaque action de tir doit prendre son propre réglage, sans reprendre celui du tir précédent. Les commandes réaffectées et P2 restent à confirmer en jeu. Vérifier aussi Global et les autres munitions ; le mode simultané ambigu décrit plus haut est une limite explicite.

Les fixtures étendues couvrent les deux actions et les deux propriétaires, les mêmes/différentes munitions simultanées, repos, zéro/100 %, table/signatures inconnues, mauvais propriétaire, registre incomplet, restauration des angles et conservation du résultat natif. Leur réussite ne remplace pas la validation en jeu.

Construction exclusivement GitHub Actions ; ZIP cumulatif quatre fichiers racine prévu. Anciennes archives et branches main/archive conservées. Rollback : archive V1.138, en conservant l'INI personnel. Aucun nouveau build installé par l'agent.
