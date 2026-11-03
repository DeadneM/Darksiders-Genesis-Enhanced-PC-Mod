# V1.150 — ressources visuelles, livraison debug

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu.** Base cumulative V1.149 ; garde sous-titres V1.148 VALIDÉ par l'utilisateur strictement conservé. [ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f657ae4a1e186ff07e321762d2446247082671b6/downloads/DarksidersGenesis_V1.150_SELECTED_EFFECT_RESOURCE_RECOVERY_TEST.zip) — [périmètre et essais](V1_150_SELECTED_EFFECT_RESOURCE_RECOVERY_TEST.md).

## Changement livré

- La bibliothèque demande les seuls assets absents du choix actif, au lieu d'attendre exclusivement leur chargement naturel. Contrôles du type, nom, paquet et identité avant utilisation. Native/Hidden sans chargement ; bouclier et Léviathan gardent leurs cycles validés.
- Stoneskin Strife : demande du corps/tissu/effet de départ/cendres nécessaires au statut actif ; variante inutilisée exclue. État de reprise distinct par joueur, armure et monde. Nouvelle activation réarme le budget des ressources manquantes. Garde de propriété des matériaux et restauration inchangés.
- Même API native vérifiée, aucun nouveau hook. Stabilisation 750 ms, budget partagé 250 ms, trois tentatives espacées de 5 secondes ; hors menus/cinématiques/Alt-Tab. Pas de chargement massif au démarrage ni depuis les callbacks d'émetteurs. Message du menu distingue préparation et échec borné.

L'existence des466 chemins est vérifiée dans le PAK natif. Cela ne valide pas l'apparence. Le premier chargement bloquant peut provoquer une courte pause. Aucune correction universelle de Stoneskin intermittent ni amélioration FPS annoncée avant essai.

## Contrôles

[GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41725892925) : **78/78 suites réussies, aucun avertissement de compilation C/CS/LNK/D détecté**. Nouveau test de budget/temporisations/identités, plus dépendances d'effets Stoneskin actifs. Deux passes de revue comparées : périmètre/configuration, puis ABI/identités/cycle de vie/budget. Seule erreur documentaire corrigée dans V1.148 :0x128=296 octets ; code du garde inchangé.

ZIP **2361582 octets**, deux binaires AMD64 à la racine (DarksidersGenesisMod.asi, dxgi.dll), CRC vérifiés, sans INI/README. SHA256 `4D21060DBFD32B133DCEE9BD9C1AD8F0FE3E9D3797944603DB384434742FD684`. Téléchargement public HTTP200 et sélection par les règles de l'updater debug vérifiés ; interface/installation du jeu non observées. Defender sans détection, protections inchangées.

**214 anciennes archives identiques, 215 au total.** 246 clés et 55 actions conservées ; main/archive/tag v1.43 intacts. Aucun INI personnel, raccourci, PAK ou sauvegarde modifié. Aucun jeu lancé, aucun build local, aucune installation automatique.

Source `041b9bd72bc8318ad0687871791fdc0c71920250`, archive immuable `f657ae4a1e186ff07e321762d2446247082671b6`. [CI](evidence/V1_150_CI_CHECK.json), [ZIP](evidence/V1_150_PACKAGE_CHECK.json), [updater](evidence/V1_150_UPDATER_CHECK.json), [double revue](evidence/V1_150_SCOPE_REVIEW.json).

## Test groupé et suite

1. Après chargement, conserver un effet sélectionné, fermer le menu, attendre quelques secondes puis utiliser le pouvoir. Vérifier le rendu, recharger et recommencer ; comparer aussi bouclier/Léviathan validés.
2. Stoneskin Strife avant/après changement d'armure dont Abyssal, dans plusieurs niveaux : pierre/feu, départ/cendres et retour au skin normal. Vérifier qu'aucun chargement ne se répète en permanence.
3. Contrôles V1.149 encore ouverts : GPU après redémarrage, trait/arc Moloch. Sous-titres activés pour non-régression du passage validé.

Pose bloquée : attendre son prochain retour spontané comme convenu. Dash aérien : chaînage natif identifié, aucune nouvelle règle de mouvement livrée ici ; stomp sans direction et dash directionnel à l'atterrissage restent la cible suivante. Coop à contrôler en dernier. Repli V1.149 conservé, aucune promotion main ni publication Nexus.
