# V1.168 préparée — TAA optionnel et rôle du chargeur

## Base, objectif et statut

Base : V1.167 intégralement VALIDÉE par l'utilisateur, main `6795f47`. Demande : commencer les réglages TAA, expliquer notre dxgi.dll et l'ancien échec d'un autre ASI loader. Le report de build reste actif.

**PRÉPARÉ / EXPÉRIMENTAL, non construit.** Aucun workflow déclenché, ZIP créé, DLL téléchargée, lancement/capture du jeu, installation ou modification de l'INI personnel. Le numéro V1.168 réserve la prochaine identité de build pour éviter d'écraser l'archive V1.167 testée ; aucune V1.168 téléchargeable n'est annoncée.

## TAA et netteté

Camera → Temporal anti-aliasing and sharpness : case **Customize temporal AA**, OFF par défaut. Deux réglages avec curseur, saisie et bouton de valeur d'essai :

| Réglage | Bornes | Valeur d'essai mémorisée, inactive tant que OFF |
| --- | --- | --- |
| Current frame weight | 0,01–1 | 0,08 |
| Sharpness | 0–2 | 1 |

Ce sont des valeurs d'essai, pas les valeurs natives. La personnalisation capture/restaure les valeurs réellement rencontrées, y compris celles d'un Engine.ini personnel. Le défaut compilé du jeu relevé est 0,04 pour le poids et 0 pour la netteté. Poids plus élevé : moins de rémanence, potentiellement plus de scintillement. Netteté trop forte : halos. Aucun changement de méthode AA, de profondeur de champ, de visée ou de shader ; le poids n'a d'effet qu'avec le TAA du jeu, la netteté est un réglage de tonemapping global. Ces valeurs concernent toutes les vues, y compris les cinématiques.

Implémentation : deux données float natives, sans nouveau hook ; service dans le Present existant. Validation SHA de l'EXE par le cœur, puis contrats exacts d'enregistrement float, drapeaux, noms et lectures de la copie de rendu à +4. Les deux copies jeu/rendu sont échangées atomiquement. Pas de scan à chaque image ; chemin OFF inactif sans accès aux données natives ni verrou. Aucun appel moteur supplémentaire ni écriture Engine.ini. Le retour natif utilise un échange conditionnel pour ne pas écraser une modification externe ; un changement d'allocation abandonne l'ancienne adresse. À l'arrêt, le service est désarmé avant restauration.

Trois nouvelles clés seulement : Features/CustomizeTemporalAA, Values/TemporalAAFrameWeight, Values/TonemapperSharpen. Les **252 anciennes valeurs** du profil de référence et les 55 raccourcis sont inchangés. Le profil de référence reste dans le dépôt, pas dans le ZIP. Main, chargeur et anciennes archives intacts.

## Ce que fait exactement notre dxgi.dll

1. **Proxy DXGI** : les exports CreateDXGIFactory/1/2 et les autres exports supportés relaient leurs appels vers la dxgi.dll de System32. Ce n'est pas une réécriture du moteur ni un pilote graphique.
2. **Chargeur ASI** : au premier appel de création d'une factory, recherche les `.asi` voisins et les charge une seule fois avec LoadLibraryW. L'ASI démarre ensuite son propre worker depuis son DllMain ; notre proxy ne lui impose pas d'initialiseur privé.
3. **Démarrage spécifique Genesis** : Skip Logos installe très tôt son crochet validé de playlist, avec signatures et contrôle de la cible, en conservant MoviePlayer et l'introduction. Le journal de démarrage partagé et la télémétrie sont également fournis par le proxy. Skip Warning et Moloch Boss Crash Fix sont dans l'ASI, pas dans cette DLL.

Le contrôle de l'ASI **livré** en V1.167 ne trouve aucune importation de fonctions DG privées, ni même une dépendance DXGI explicite dans sa table d'importations. Les ponts vers les fonctions privées sont résolus dynamiquement et vérifiés. En leur absence, le journal se crée seul et les autres fonctions continuent ; **Skip Logos perd son mécanisme précoce spécifique**. Le code gameplay, caméra, interface, Stoneskin, etc. n'exige pas le proxy propriétaire.

Cela démontre l'absence de verrou volontaire, pas la compatibilité de tous les chargeurs. Il faut un chargeur x64 réellement chargé par le jeu, qui trouve l'ASI et permette son initialisation au bon moment. Conserver l'ASI et son INI au même emplacement évite aussi un changement de chemin de configuration lors d'un essai.

## Ancien essai Ultimate ASI Loader

Le binaire alternatif retrouvé était Ultimate ASI Loader 10.0.2 x64, GitHash `56fa0b0`, avec les deux exports DXGI importés par le jeu. Son [code officiel exact](https://github.com/ThirteenAG/Ultimate-ASI-Loader/blob/56fa0b0/source/loader/plugins/plugins.cpp) appelle LoadLibraryW, puis n'appelle InitializeASI que si cet export existe. Son absence dans notre ASI ne démontre donc pas la cause du blocage. [Preuve binaire conservée](evidence/V1_148_ALTERNATIVE_LOADER.json).

**Cause de l'ancien échec non établie.** Le point d'accrochage et le moment du chargement sont des différences possibles, pas une conclusion. Pas d'ajout d'un faux InitializeASI, de remplacement du loader ou de correctif spéculatif ici. L'essai éventuel devra d'abord enregistrer si le loader est chargé, s'il charge l'ASI et jusqu'où va son journal. Il sera séparé du TAA. Notre proxy reste identique à V1.167 ; une compatibilité DXVK demanderait une réflexion distincte sur le chaînage, puisqu'il charge actuellement le DXGI système.

## Contrôles effectués et suite

- Contrôle hors jeu : **10 contrats natifs exacts** retrouvés dans l'EXE supporté ; table d'importations de l'ASI V1.167 lue dans le ZIP déjà local ; parité des 252 anciennes clés ; absence de changement du proxy. [Preuve statique](evidence/V1_168_TAA_LOADER_STATIC_CHECK.json).
- Fixtures C++ préparées : valeurs invalides, copies jeu/rendu, restauration, changement externe, remplacement d'allocation, concurrence d'échange et persistance INI. **Non compilées et non exécutées**, conformément à l'absence de build ; aucun succès C++ ou en jeu annoncé.
- Critère du futur build : comparer OFF/ON dans la même scène en mouvement puis immobile, en vue native et TPS ; inspecter cheveux, écharpe, flammes, contours et HUD. Vérifier plusieurs réglages, puis OFF et retour exact au comportement capturé. Tester un chargement et une cinématique. Aucun gain de FPS promis.
- Retour : décocher la personnalisation ; si le candidat ne convient pas, revenir aux deux binaires V1.167 validés. Aucune édition de configuration du moteur nécessaire.
