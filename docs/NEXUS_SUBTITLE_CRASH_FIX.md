# Note pour la future publication Nexus — crash sous-titres

**Préparation documentaire seulement, rien publié sur Nexus.** Correctif introduit en V1.148, validé par l'utilisateur le3 novembre2026 sur le passage de la cinématique d'introduction de Moloch avec les sous-titres activés. Le journal montre la protection effectivement déclenchée ; pas seulement un passage sans interception.

## Formulation publique proposée

**Subtitle crash fix:** fixes the reproduced crash at the Moloch cinematic transition with subtitles enabled. Subtitles can remain enabled; the fix is switchable under Gameplay → System and startup.

Ne pas généraliser à tous les crashs, toutes les cinématiques, toutes les langues ou toutes les éditions du jeu : ces cas ne sont pas validés. Présenter la scène et la configuration Windows supportée comme périmètre testé.

## Explication accessible

Lors du défaut, le moteur demande une opération sur un tampon graphique absent puis essaie de le lire, provoquant le crash. Le mod reconnaît cette demande invalide et ignore uniquement l'effacement impossible. Il conserve les sous-titres et laisse les commandes valides fonctionner normalement. Il ne désactive pas globalement le stencil ou les effets visuels et ne recrée pas un tampon absent.

## Détails vérifiables

- Quatre dumps présentent la même lecture nulle à RVA D5D0A1, dont un sans chargeur/ASI chargés ; les modifications persistantes de configuration ne sont pas présentées comme remises à zéro lors de cet essai.
- L'utilisateur a d'abord passé la scène en désactivant les deux types de sous-titres, puis l'a validée en V1.148 avec le correctif.
- Fonction native D5CDD0 : paquet complet0x128 octets, cible profondeur/stencil nulle et clearStencil=1. Copie privée complète, seul l'octet clearStencil+D9 devient0 pour la combinaison précise capturée ; entrée du moteur intacte, autres demandes transmises avec leur pointeur original.
- Exécutable supporté et signatures vérifiés ; code isolé, option désactivable. Pas de changement du fichier EXE sur disque ni de réglage de sous-titres forcé.
- Le producteur initial qui a omis le tampon n'est pas encore identifié. Décrire cette protection ciblée sans prétendre avoir réécrit toute la gestion des sous-titres.

[Périmètre et preuve](V1_148_SUBTITLE_CRASH_TEST.md), [validation et journal](evidence/V1_148_USER_VALIDATION.json). Avant la publication finale, citer la version stable qui inclura ce correctif ; ne pas promouvoir automatiquement debug vers main.
