# V1.187 sur debug — HDR10 et lames montées

Vérifier le HDR10 réellement présenté, le transfert des lames sur Mayhem et Despair et l'inclinaison des lames rangées à pied. [Critères](V1_187_NATIVE_HDR10_MOVING_BLADES_REVIEW.md).

# V1.186 sur debug — choix HDR et lames de Strife

Priorité aux lames rangées gauche/droite à pied et à cheval, puis aux lames tenues dans les montages de mêlée. Valider les réglages graphiques en direct de V1.185. Vérifier le format réel si HDR10 est sélectionné. [Critères](V1_186_HDR_CHOICES_BLADE_PRESENTATION_REVIEW.md).

## Suite de V1.184

Vérifier en mission l’application directe de la résolution interne et des reflets, puis les lames sur les deux montures de Strife. Identifier le renouvellement des samplers pour l’anisotropie. Le retour en jeu sur le HDR V1.184 est attendu. [Candidat V1.185](V1_185_LIVE_QUALITY_MOUNTED_BLADES_REVIEW.md).

## V1.184 à essayer

En scRGB 1000, comparer l’image en mission à V1.183 ou à SDR. Le chemin de rendu HDR et la signalisation doivent être actifs. Les essais des lames rangées et des transitions montées de V1.182 restent ouverts. [Livraison et essai HDR](V1_184_DELIVERY_RECEIPT.md).

## V1.183 à essayer

Comparer le rendu scRGB 1000 à SDR dans la même scène. Vérifier la lisibilité du HUD et le résultat de **Refresh HDR availability** après relance. Les essais V1.182 des lames rangées et des transitions sur les deux chevaux de Strife restent ouverts. [Livraison et essai HDR](V1_183_DELIVERY_RECEIPT.md).

## V1.182 à essayer

Valider en jeu le HDR, les transitions des deux chevaux et la case des lames rangées. Lames tenues à cheval encore ouvertes. Étudier la résolution interne et les reflets en direct en premier, puis le cycle des samplers d’anisotropie. [Livraison](V1_182_DELIVERY_RECEIPT.md).

---

# V1.182 en préparation sur debug. Elle reprend les holsters croisés validés et ajuste leur transition, la création des lames rangées et le démarrage HDR. Validation en jeu attendue.

[Périmètre et essai](V1_182_REVIEW.md).

---

## V1.181 — livrée, essai utilisateur requis

[Reçu](V1_181_DELIVERY_RECEIPT.md). Vérifier les holsters sur les deux chevaux et le placement des copies rangées. Lames natives tenues montées : toujours ouvert. HDR : vérifier couleurs, buffer et signaling. Paramètres de qualité en direct : faisabilité encore à étudier. Acquis FX de mêlée conservé.

---

## V1.181 — préparation

[Source et limites](V1_181_REVIEW.md). Essai des holsters des deux chevaux, copies de lames rangées et HDR. Lames natives tenues montées : diagnostic encore ouvert. Étudier graphismes à chaud pour les sept Rendering quality ; garder les valeurs et chemins validés.

---

## Retour utilisateur — armes et holsters montés (après V1.180)

- VALIDÉ utilisateur : effet des lames à cheval ; préserver le correctif FX.
- OUVERT : lames toujours invisibles à cheval, y compris Native selon le retour précédent.
- OUVERT : présentation pistolet/holster inversée entre pied et monture ; l'autre holster reste visible.
- Règle confirmée : pistolet main droite → holster côté gauche, pistolet main gauche → holster côté droit. Distinguer côté de la main, côté visible du rangement et noms internes des os/composants. Appliquer aussi cette distinction au futur rangement des lames.
- Audit initial : PistolVisualFeature.cpp applique slot%2 uniformément aux composants montés tenus et rangés (slots 2..5), sans correspondance explicite main/rangement. Le masquage des holsters par matrices de PistolVisualPolicy.h vise le squelette normal ; il ne suffit pas pour la monture. Ce constat explique une lacune de couverture, pas encore la correspondance exacte des composants montés.
- Prochaine correction : cartographie explicite par rôle (main/rangement/holster), puis tests pied↔cheval avec droite seule, gauche seule, deux et OFF. Ne pas inverser globalement les côtés ni toucher aux FX validés. Visibilité des lames montée à traiter séparément après identification du modèle natif.
- Aucun correctif runtime ni build annoncé pour ce seul relevé.

---

# Priorité — essai HDR V1.180

1. HDR10 1000 / Native / scRGB 1000, fermeture et relance entre profils, Windows HDR actif.
2. Comparer image, hautes lumières et menus ; aucun blocage par marque.
3. Effets de mêlée validés et conservés. Plus tard : lames visibles tenues/rangées.

[Livraison](V1_180_DELIVERY_RECEIPT.md).

---

# Priorité — V1.180

1. Compiler/contrôler puis tester les profils HDR identiques sur toutes les marques, dans Graphics.
2. HDR10 1000 / Native / scRGB 1000, fermeture et relance entre modes, menus/HUD à vérifier.
3. Effets de mêlée validés, conserver. Plus tard : modèles de lames visibles tenues/rangées.
4. Les anciennes branches natives observées ne justifient aucun blocage par constructeur.

---

# Priorité — V1.179

1. Livrer Graphics : cinq groupes déplacés et état HDR GPU/écran en lecture seule.
2. Activation HDR réelle sur Intel : chemin natif manquant, travail séparé avant toute promesse.
3. Plus tard : lames visibles tenues/rangées. Déjà invisibles à cheval avec le choix Native ; ne pas attribuer cela au seul choix OFF sans preuve.
4. Effets de mêlée V1.178 validés, conserver le comportement.

---

# Priorité — essai V1.178

1. À cheval : Native / gauche OFF / droite OFF / les deux OFF, attaques des deux côtés.
2. Comparer à pied ; P2 en jeu reste à tester lorsque l'utilisateur sera prêt.
3. HDR natif D3D11 Intel non forcé : limite d'implémentation démontrée, autres solutions hors de cette correction.

[Livraison](V1_178_DELIVERY_RECEIPT.md).

---

# Priorité — V1.178

1. CI puis essai des arcs montés Native / gauche OFF / droite OFF / les deux OFF, puis régression à pied.
2. HDR : ne pas forcer le chemin natif Intel D3D11 sans implémentation ; étude hors jeu consignée.
3. Main stable conservée ; aucune installation automatique.

---

# Priorité après validation V1.177

1. Réparer uniquement les effets de mêlée Strife à cheval : relever les notifies/FX montés avant extension du filtre existant. À pied validé, à conserver.
2. Évaluer HDR natif du jeu : Native / expérimental au redémarrage proposé, prise en charge réelle non démontrée.
3. Accès EXE/assets du jeu en attente : contrôle automatique indisponible pour limite d'utilisation. Sources/cache analysés ; aucune build ni installation.

[Analyse](NEXT_MOUNTED_MELEE_HDR_REVIEW.md).

---

# Priorité — essai V1.177

1. Résolution interne 75 %, fermeture/relance, puis Native et même cycle.
2. Comparer ensuite anisotropie, ombres, visibilité et SSR dans des scènes adaptées.
3. Aucun résultat GPU déduit des tests CI ; main reste V1.173.

[Livraison](V1_177_DELIVERY_RECEIPT.md).

---

# Priorité — V1.177

1. Contrôler et livrer les sept réglages de rendu au redémarrage.
2. Test groupé puis Native ; rendu à confirmer, aucune conclusion GPU.
3. Les quatre effets V1.176 conditionnels restent documentés, sans correction spéculative.

---

# Priorité — test groupé V1.176

1. Chromatique corrigée et motion blur Amount/Maximum, puis un groupe post-traitement à la fois.
2. Vérifier rendu et retour Native ; étendre ensuite les domaines/passes justifiés.
3. GPU toujours sans nouveau diagnostic probant.

[Livraison et limites](V1_176_DELIVERY_RECEIPT.md).

---

# Priorité — V1.176

1. Construire et contrôler le catalogue post-traitement, puis test visuel groupé.
2. Étendre uniquement les domaines/passes démontrés disponibles ; 119 champs candidats ne sont pas 119 contrôles utilisables.
3. GPU : aucune nouvelle attribution sans mesure exploitable.

---

# Priorité actuelle — V1.175 debug, main V1.173

1. Valider l'intensité chromatique 0/1/5/Native ; motion blur déjà accepté.
2. Diagnostic GPU : pas de nouvelle capture avant premier plan garanti et accès adapté. Mesures précédentes non exploitables, aucune passe identifiée.

[Dernière livraison](V1_175_DELIVERY_RECEIPT.md). Historique ci-dessous, sans reprise automatique d'autres pistes.

---

# Priorité actuelle — V1.174 debug, main V1.173

1. Valider les deux réglages d'image : Native / 0 / intermédiaire / retour Native.
2. TPS encore gourmand : isoler la passe GPU coûteuse avant un nouveau correctif. Réglages et réactivité impérativement préservés.
3. Crash HUNG à surveiller, cause non attribuée. Les autres recherches anciennes ci-dessous restent historiques, sans reprise automatique.

V1.173, coop et appel aérien acceptés par l'utilisateur. [Dernière livraison](V1_174_DELIVERY_RECEIPT.md).

---

# File active — V1.171 validée sur debug, main V1.168

Dernier point : 19 novembre 2026. Fonctions V1.171 VALIDÉES par l’utilisateur sur debug ; main/release V1.168 intactes. Nouveau sujet : micro-saccades, ancienneté inconnue. [Mesures, optimisation CPU préparée et limites](GPU_FREEZE_REVIEW_2026_11_19.md). Aucun réglage ou fichier du jeu modifié par le diagnostic. [Dernière livraison](V1_171_DELIVERY_RECEIPT.md).

## Suite, par ordre de preuve disponible

1. **Dépendance native SourceControl** — une erreur au chargement Level_00, référence confirmée dans SoulBarrier_Orb_Blueprint du PAK d’origine. Les 13 fonctions sont décodées : IsEnabled de SourceControl est réellement appelé. Pas de retrait aveugle, correctif ou gain mesuré. [Diagnostic](NATIVE_STREAMING_REVIEW_2026_11_18.md).
2. **Autre ASI loader** — cause de l’ancien échec inconnue. Prochaine preuve nécessaire : journal du chargeur et jalons ASI lors d’un essai réversible explicitement préparé. Aucun nouveau chargeur installé ; [constats existants](V1_168_TAA_LOADER_REVIEW.md).
3. **Budget de textures** — 1000 Mo dans le dernier journal, aucune alerte de dépassement trouvée dans celui-ci. Pas de valeur forcée ni de curseur supplémentaire sans problème ou mesure correspondants.
4. **DXVK** — expérience séparée éventuelle ; conflit de rôle autour de dxgi.dll à résoudre avant essai. DX12 demanderait un backend de rendu du mod adapté. [Étude](COMMUNITY_FIX_REVIEW_2026_11_13.md).

5. **Niveaux des cœurs dans l’arbre** — affichage incomplet au premier accès, rétabli en rouvrant ; deux relevés comparés : données stables, indicateurs cachés puis visibles. Rafraîchissement natif unique ajouté en V1.170 puis validé avec le lot V1.171.

## Règles de reprise

Travailler sur debug, préserver main/release, INI et raccourcis. Build cumulative V1.170 autorisée par la demande utilisateur ; aucun lancement/installation automatique. Captures du menu effectuées en lecture seule ; aucun remplacement de fichiers pendant que le jeu est ouvert. Réutiliser les reçus et le cache local ; lire seulement les différences. Un diagnostic n’est pas une correction validée.

L’ancienne file chronologique est conservée intégralement dans [l’historique](history/ACTIVE_QUEUE_THROUGH_V1_168_2026_11_18.md). Ses anciens statuts ne remplacent pas la validation globale V1.168. Les autres idées historiques restent consultables et ne deviennent pas automatiquement du travail actif.

## Préparé depuis la livraison V1.170

- Journal du chargeur : capturer GetLastError avant l’écriture de la chronologie. Correction préparée seulement ; aucun nouvel essai, build ou diagnostic de compatibilité annoncé.
- SourceControl : flux SetInitialAnimState entièrement précisé ; le résultat choisit une branche d’état/effet. Aucun retrait natif retenu pour cette occurrence unique non chronométrée.
- Deux passes sur le delta récent : aucune suppression fonctionnelle justifiée. [Revue et suite](CONTINUATION_REVIEW_2026_11_19.md).

## V1.171 demandée

Construire le lot cumulatif et la correction de journal, puis vérifier CI/ZIP/lien. Aucun autre défaut ne dispose de preuve suffisante pour un patch supplémentaire. Pas d’installation automatique ; V1.170 et main V1.168 préservées.

## Livraison V1.171

Construite et contrôlée : 86/86 tests. Correction de journal incluse ; archive V1.170 préservée. Tests utilisateur reportés sur le lot cumulatif V1.171, sans nouveau test de gameplay ajouté. [Livraison](V1_171_DELIVERY_RECEIPT.md). Les autres pistes restent sans patch démontré.


## V1.171 validée — freezes et charge GPU

Validation fonctionnelle utilisateur reçue, sans promotion main. Freezes confirmés pendant 31 secondes de compteurs Windows : 3D 51–65 %, mémoire dédiée environ 3,2 Gio ; pas de temps par image. Optimisation CPU de HookSetPS préparée sans build : éviter les verrous/table tant qu’aucun shader de visée n’est reconnu ; native transmise et filtrage existant préservés. Gain et cause des freezes non établis. [Preuves, limites et suite](GPU_FREEZE_REVIEW_2026_11_19.md).


## V1.172 — appel aérien demandé

Case indépendante OFF sous Horse Everywhere ; contrôle de sol de l’action native adapté uniquement en chute/saut ou plané local. Signatures natives, résultat et hausse de 100 unités contrôlés hors jeu. 259 clés dont les 258 anciennes inchangées, 55 actions conservées. Petite optimisation CPU précédente cumulée ; micro-saccades toujours ouvertes. Construction GitHub, aucun remplacement automatique du jeu. [Périmètre et tests](V1_172_REVIEW.md).


## V1.172 — livraison vérifiée

86/86 tests Windows, zéro avertissement/erreur C++, ZIP deux binaires contrôlé, lien vérifié et Defender sans détection. 259 clés, les 258 anciennes valeurs et 55 actions préservées ; aucune installation ou promotion main. Appel aérien OFF par défaut sous Horse Everywhere, validation utilisateur attendue. Optimisation CPU cumulée, micro-saccades non annoncées corrigées. [Reçu, ZIP et tests](V1_172_DELIVERY_RECEIPT.md).


## GPU — comparaison native du 19 novembre

Comparaison autorisée V1.172 / sans ASI, carte Arc B580 identique et premier plan suivi. Retour utilisateur : charge plus basse et jeu plus fluide sans mod ; utilisation 3D médiane Windows 50 % / 47,5 %, mémoire dédiée environ 3,35 / 2,85 Gio, intervalles API proches sur ces fenêtres courtes. Cause non isolée. Deux fichiers V1.172 restaurés, INI et sauvegardes intacts ; aucun fichier ne reste désactivé. Prochain essai proposé : seul NativeCustomDOF OFF avec le mod, sans build et après préparation utilisateur. [Résultats et limites](GPU_NATIVE_COMPARISON_2026_11_19.md).


## TPS — effet du flou confirmé par retour utilisateur

L’essai suivant a porté sur TPSCustomDOF OFF, pas sur NativeCustomDOF : préparation lue TPS=1, NativeCustomDOF=1, TPSCustomDOF=0. 15 observations toutes au premier plan ; retour utilisateur répété : OFF plus fluide, ON moins fluide. Le chiffre 100 % pourrait selon lui relever du pilote, sans confirmation technique. Aucun relevé ON contrôlé capturé car jeu fermé. Choix demandé avant changement : uniquement TPSCustomDOF OFF dans le profil actuel et les futurs défauts ; garder le profil natif, la correction de mise au point et l’option réactivable. Aucun défaut/profil changé automatiquement à ce stade. Fichiers V1.172 restaurés, aucun fichier ne reste désactivé. [Analyse et limites](GPU_NATIVE_COMPARISON_2026_11_19.md).


## Optimisation caméra sans changement de réglages — 19 novembre 2026

Décision utilisateur : REJETÉ, le passage automatique de TPSCustomDOF sur OFF. Ne modifier ni le profil personnel ni les défauts. La suite porte exclusivement sur du travail CPU évitable, à rendu conservé.

Depuis debug 3505391 / binaire V1.172 : les trois chemins TPS réutilisent leur compensation de mise au point réussie pour éviter son second calcul dans ApplyBlur. La projection des bornes est omise lorsque la région choisie atteint déjà 20000 unités, plafond du résultat qui aurait été calculé. Aucun hook, réglage, fréquence de service, qualité GPU ou ressource persistante ajouté. Le chemin natif conserve son contrat. Retour arrière : rétablir TpsCameraFocus.h et les trois appels de CameraTraceFeature.cpp depuis 3505391.

CONTRÔLE À EFFECTUER sur GitHub uniquement : tests existants et 20 480 combinaisons comparant tous les octets de la sortie caméra avec l'ancienne implémentation, pour les chemins TPS et natif. Aucun gain FPS/GPU ni suppression des saccades annoncé ; validation en jeu encore requise. Pas de nouveau ZIP ni installation pour ce contrôle.


Contrôle terminé : Windows x64 Release, 86/86 tests réussis, zéro avertissement/erreur C++. Les 20 480 combinaisons du test de parité comparent le chemin TPS combiné et le chemin natif indépendant, octet par octet, avec le calcul précédent. Source 4e3265180592a447d5de3c74c12504895b9b6fbe. [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44600037723). Statut : CONSTRUIT et CONTRÔLÉ, gain de fluidité EXPÉRIMENTAL, non validé en jeu. Vérifier dans la même scène, avec les mêmes réglages : aspect/mise au point inchangés et éventuelle différence de saccades, à pied/cheval/ultime et pendant les roues. Aucun ZIP publié ou installé ; V1.172 reste la dernière livraison. Main/archive/v1.168 inchangés après vérification distante.


## Audit global des optimisations — 19 novembre 2026

Sources debug 0499f6f : inventaire 62 C++ / 125 headers, deux passes comparées (chemins fréquents puis garde-fous/invalidation/mesures). Dernier journal V1.172 lu, sans capture : gameplay inclusif avg858,62µs/max22,45ms ; appearance-discovery max8,23ms ; 78 589 432 draws comptés, deux shaders reconnus, 54 masqués. Pas de causalité freeze ni temps GPU déduits. Priorités : télémétrie/gestion des shaders, lectures d'identité groupées, contexte gameplay partagé, scans encore sans budget temps ; petits candidats CRC, focusP2 et fallbackDOF. Aucune suppression de production démontrée sûre. Propositions non implémentées ; aucun profil/défaut, source, binaire ou ZIP modifié. Dernier contrôle 86/86 reste celui de 4e32651. [Audit, comparaison et prochaines étapes](FULL_OPTIMIZATION_AUDIT_2026_11_19.md).


## V1.173 — lot CPU demandé, réactivité à préserver

Depuis V1.172/c11ea66 : comptage atomique par draw retiré (filtrage conservé), CRC/tailles shaders, lectures d'identité groupées avec fallback, focus P2 partagé. Aucun changement de réglage/défaut, rythme de scan/service/input ou interception. Pas de cache inter-image. PRÉPARÉ / EXPÉRIMENTAL avant CI GitHub ; aucun build local ni installation. Critère : même comportement/rendu et transitions skins/effets immédiates, puis gain à mesurer. [Périmètre, risques et tests](V1_173_REVIEW.md).


## V1.173 — livraison vérifiée, réactivité à tester

Source 28ab24f958084aa5982404888c3cebb1866685cc, archive d84168bd5184247f09088e61bbee529438eaf51f. GitHub :86/86 tests, zéro erreur C++, un avertissement C28020 de génération constexpr revu et documenté (aucun masquage). ZIP deux binaires contrôlé ; Defender aucune détection ; 259 clés/défauts/bindings préservés ; 237 anciennes archives et branches protégées inchangées. Aucune temporisation ou nouvelle réduction de cadence ; aucune installation. Gain de fluidité EXPÉRIMENTAL. Suite : essai groupé réactivité skins/effets/transitions, rendu/fluidité à réglages constants, coop en dernier. [Livraison et rollback](V1_173_DELIVERY_RECEIPT.md).


## Publication V1.173 demandée — 20 novembre

Promotion main autorisée, binaires Release V1.173 réutilisés sans recompilation. Aucun gain de fluidité ni correction du crash GPU déclarés validés. Archives/configuration conservées. [Périmètre et limites](V1_173_MAIN_RELEASE.md).


Publication V1.173 CONFIRMÉE sur main et release v1.173 : asset vérifié par empreinte GitHub identique au build existant ; pas de recompilation. 86/86 tests antérieurs, un avertissement constexpr documenté ; crash GPU non attribué. [Preuve](evidence/V1_173_RELEASE_CHECK.json).


## V1.174 — Native et réglages des deux effets d'image

Retour utilisateur : V1.173/coop/appel aérien acceptés ; TPS encore gourmand, crash GPU non attribué. Lot limité aux curseurs motion blur / limite chromatique, Native par défaut, 0 désactive, sauvegarde et migration anciennes cases. Aucune modification de DOF/cadence/skins. Table CRC générée sans indice pour l'analyseur. PRÉPARÉ avant CI GitHub ; pas de build local ni installation. [Détail et tests](V1_174_REVIEW.md).


## V1.174 — livraison contrôlée

Source a4f27706d89118ac9c40066f2b0c59058aa79716, archive 69fe6bef946cfa5306bcfbace9fee6966fa1836c : 86/86 tests Windows, zéro avertissement/erreur C++, ZIP deux binaires vérifié, Defender aucune détection. Mode Native par défaut ; slider/saisie motion blur0–1 et limite chromatique0–10. 259 anciennes valeurs conservées,2nouvelles ; aucune cadence ou DOF changé, aucune installation. Nouvelle image à valider en jeu. Main V1.173/archives préservées. Prochaine priorité TPS : mesure ciblée des passes GPU, pas de changement de qualité à l'aveugle. [Livraison et rollback](V1_174_DELIVERY_RECEIPT.md).


## V1.175 — chromatique et relevé GPU non concluant

Motion blur validé, plafond chromatique sans effet apparent : le vrai champ d'intensité/bit est prouvé hors jeu et appliqué par le callback caméra existant, avec publication atomique. Native/poids partiels inchangés ; aucun nouveau hook ni délai. Tests préparés, rendu expérimental. WPR GPU refusé par Windows ; TPS0/12 au premier plan et pas de native mesurée, aucune comparaison exploitable ni conclusion GPU. [Contrat, limites et tests](V1_175_REVIEW.md).


## V1.175 — livraison contrôlée

Source c740283326b05872c0677e7bdbc6374caa08c6ba, archive f32bdd5a94046def8a7d6a44cd8c976a5ed348ea. Après correction du Snapshot oublié dans le premier run :86/86 tests, zéro avertissement/erreur C++, ZIP deux binaires contrôlé, Defender aucune détection. Intensité chromatique réelle via caméra existante, Native et mélanges partiels conservés, aucun nouveau hook/cadence ; motion blur inchangé. Rendu expérimental à tester0/1/5/Native. 261 anciennes valeurs préservées,262 clés ;239 anciens ZIP conservés. Aucune installation, main V1.173 intacte. GPU : aucune comparaison exploitable (TPS0/12 premier plan, pas de native, WPR refusé), aucune conclusion de performance. [Livraison et rollback](V1_175_DELIVERY_RECEIPT.md).
