# Audit global des optimisations — 19 novembre 2026

**Résultat : du travail CPU évitable est identifié ; aucune cause GPU unique ni gain FPS n'est démontré.** Aucun réglage/défaut, code de production, profil, fichier installé ou comportement validé modifié par cet audit. Pas de build, d'installation ou de capture mémoire. Le dernier journal existant a seulement été lu.

## Base, périmètre et méthode

- Base fonctionnelle expressément validée : V1.171. Dernier ZIP : V1.172, appel aérien du cheval encore expérimental. Stable main/release : V1.168.
- Sources examinées : debug `0499f6f68e27ba8c3775b5039953c44bc623ef6c`; optimisation focus P1 `4e32651` déjà construite/contrôlée 86/86, mais non installée et sans gain runtime mesuré.
- Inventaire complet : **62 fichiers C++, 125 en-têtes, 27 404 lignes**. Les 62 sources C++ figurent dans CMake. Aucun fichier de production orphelin du build détecté ; cela ne prouve pas l'absence de fonctions inutilisées dans chaque fichier.
- Première passe : revue répartie des chemins gameplay/objets/effets, rendu/caméra et entrées/menu/configuration/updater/chargeur. Lecture des audits V1.140/V1.153 et des résultats récents, puis appels fréquents, garde-fous, caches, allocations, verrous et I/O.
- Deuxième passe : contrelecture des candidats (fréquence réelle, arrêt/restauration, invalidation, multithread, coût natif inclus), comparaison des constats et confrontation au journal actuel. Les doublons entre domaines sont fusionnés ci-dessous.
- Profondeur variable : les points chauds et leurs gardes ont été suivis en détail ; tous les corps d'animation/scripts natifs ne sont pas relus instruction par instruction. Ce n'est pas un graphe C++ calculé par compilateur, une preuve de code mort, ni un profilage GPU. Pas de recherche externe ni téléchargement de binaires nécessaire.

### Couverture des domaines

Chemins suivis : bootstrap ASI/proxy, arrêt et configuration ; callbacks caméra/déplacement et propriété P1/P2 ; services de ressources, santé, potions, munitions, Hotstreak, synergie et ultime ; mouvements/dash/réception/recovery/sauts/cheval ; dispatch des capacités natives/transférées et transitions d'animation ; matériaux/particules/Stoneskin/pistolets/holsters/Léviathan/bouclier ; D3D/shaders/Present/TAA/DOF/protection Moloch ; XInput/RawInput/bindings/menu/notifications/updater. La revue détaillée se concentre sur les chemins fréquents et les candidats listés, pas sur une preuve formelle de chaque script natif.

## Mesures existantes, sans les surinterpréter

Journal V1.172 terminé le 19 novembre à 20:10:17, session commencée à 20:04:17. Identité et inventaire dans [les preuves](evidence/FULL_OPTIMIZATION_AUDIT_2026_11_19.json).

| Étape | Moyenne échantillonnée | Maximum échantillonné | Limite de lecture |
| --- | ---: | ---: | --- |
| Service gameplay | 858,62 µs | 22,45 ms | Inclusif ; contient des sous-étapes et appels natifs |
| Découverte apparences | 98,83 µs | 8,23 ms | Comprend les retours rapides ; maximum non exhaustif |
| Découverte capacités | 449,67 µs | 1,97 ms | Échantillons du service, pas coût de toutes les images |
| Observation pistolets | 82,97 µs | 329,40 µs | Profil actuel avec un pistolet masqué |
| Observation Stoneskin | 5,90 µs | 3,30 ms | Moyenne incluant les gardes ; ne mesure pas les effets GPU |

Échantillonnage **1/64**, temps écoulé inclusif : **ne pas additionner ces étapes**. Des interruptions de thread et appels natifs peuvent participer aux maxima. Aucune correspondance temporelle avec un freeze précis n'est établie.

Le filtre des traits/arcs a compté **78 589 432 appels de dessin**, deux shaders reconnus et 54 dessins masqués. Le compteur de diagnostic est incrémenté sur chaque draw ; après reconnaissance, le suivi des contextes peut aussi prendre des verrous. Ce sont des nombres d'appels, pas des durées.

La session ancienne de 13:57–14:00 comptait 24 683 182 draws et **zéro** shader reconnu : les raccourcis y évitaient déjà les verrous du filtre, mais pas le comptage. Ne pas mélanger ces deux sessions pour accuser les verrous à tort.

## Candidats comparés et retenus

P1 = première priorité d'investigation ; P2 = lot simple ou contextuel ; P3 = secondaire. La priorité n'est pas une promesse de gain. Les liens pointent sur les sources de la base examinée.

| Priorité | Constat confirmé à la seconde passe | Proposition minimale | Risque / portée |
| --- | --- | --- | --- |
| P1 | [ShaderAimFeature.cpp](../src/ShaderAimFeature.cpp#L140) : sept hooks font un incrément atomique `g_drawCount` à chaque draw, même sans shader cible. Le total ne sert qu'au diagnostic de fermeture. | Sortir ce comptage du chemin normal, ou l'agréger avec une collecte correcte. Un simple `relaxed` ne supprime pas l'opération atomique verrouillée sur x64. | Fonctionnellement limité au diagnostic ; agrégation multithread/flush à concevoir. Ne pas retirer le filtrage validé. |
| P1 | [ShaderAimFeature.cpp](../src/ShaderAimFeature.cpp#L105) : après le premier match, recherche et mise à jour de contexte sous deux verrous par bind ; [Skip](../src/ShaderAimFeature.cpp#L116) consulte une map sous verrou par draw. | Petit suivi des contextes borné, ou réduction des accès sous verrou, mesuré avant/après. | Moyen : contextes différés, threads, ClearState et command lists. Garder la vérification `PSGetShader` des seuls candidats masqués. |
| P1 | [NativeMemoryReader.h](../src/NativeMemoryReader.h#L12) vérifie chaque lecture par VirtualQuery. [Item](../src/NativeObjectIdentity.h#L20) fait six lectures sur un succès, dont trois dans le même enregistrement. | Grouper adresse/flags/serial du même enregistrement : six appels de lecture pourraient devenir quatre, sans cache mémoire durable. | Bas à moyen : conserver bornes, protections, flags GC, index/serial et refus de réutilisation d'adresse ; tester frontières de régions et conserver les lectures unitaires si le bloc ne peut pas être validé. Aucun nombre de VirtualQuery par image mesuré. |
| P1 mesure | [ServiceLocalGameplay](../src/DarksidersGenesisMod.cpp#L1441) est appelé par [la caméra](../src/CameraTraceFeature.cpp#L163) et [GetMaxSpeed](../src/DarksidersGenesisMod.cpp#L1892). Plusieurs modules recherchent encore le joueur pourtant déjà résolu. | Partager le contexte de lecture prouvé, compter les appels par source/joueur et mesurer les sous-étapes manquantes avant dédoublonnage ciblé. | Moyen à élevé : **pas** de verrou « une fois par image » global. Il pourrait perdre une commande, une restauration ou la seule voie active sans caméra. |
| P2 | [HookCreatePS](../src/ShaderAimFeature.cpp#L76) calcule un CRC bit à bit de tous les shaders créés <4 Mio avant de vérifier les 168 signatures à taille exacte. | Préfiltrer par les tailles dérivées du catalogue, puis CRC à table équivalente. | Bas après tests CRC/signatures : agit aux créations/chargements, pas sur chaque image. Conserver compteur de créations et contrôles du bytecode. |
| P2 | [CoopCameraFeature.cpp](../src/CoopCameraFeature.cpp#L113) et [ligne 152](../src/CoopCameraFeature.cpp#L152) gardent deux paires Compensate+ApplyBlur. L'optimisation récente ne couvrait que les trois chemins P1. | Réutiliser `ApplyFocusAndBlur` pour ces deux chemins P2, avec parité complète des sorties. | Bas ; uniquement coop, donc pas l'explication des saccades solo. Conserver verrou, target, lens, bounds et cinematic(1). |
| P2 | [UpdateCameraDof](../src/DarksidersGenesisMod.cpp#L728) valide la zone CVar avant de savoir si le fallback global est nécessaire. | Évaluer la politique avant la validation mémoire ; restauration toujours vérifiée quand le mod possède une modification. | Bas à moyen : préserver remplacement d'allocation et autorité native. Microgain CPU seulement. |
| P2 | [EffectAppearanceFeature.cpp](../src/EffectAppearanceFeature.cpp#L83) : quota 512 objets/16 ms, lectures groupées, mais pas de budget de durée ; maximum inclusif observé 8,23 ms. [MountedHolsterFeature.cpp](../src/MountedHolsterFeature.cpp#L40) lit encore les records individuellement pendant sa découverte. | Grouper les lectures contiguës du scan holster ; mesurer puis borner le temps de découverte apparences comme le fait déjà Wrath. | Moyen pour le budget : une découverte reportée ne doit pas devenir une absence d'effet. Les scans s'arrêtent déjà quand prêts. |
| P2 contextuel | [PistolVisualFeature.cpp](../src/PistolVisualFeature.cpp#L396) prépare identité/pose/verrous sans sortie inactive lorsque aucun pistolet/lame n'est OFF. [Léviathan](../src/LeviathanVisualFeature.cpp#L130) vérifie l'identité avant le test disabled. | Retour inactif uniquement après restauration complète et absence d'état conservé ; réveil immédiat au changement de choix. | Bas à moyen. **Pas de bénéfice pour le profil actuel** : un pistolet est OFF et Léviathan est activé. Ne pas retirer les états nécessaires aux bras/holsters. |
| P2 mesure | [XInput](../src/DarksidersGenesisMod.cpp#L2666) peut être relu jusqu'à cinq fois solo/huit coop par Present dans les gardes éligibles ; échecs supplémentaires avec plusieurs DLL sans manette connectée. | Réutiliser un échantillon physique par contrôleur à l'intérieur du seul Present, si le contrat d'entrée est accepté et testé ; résoudre les exports une fois. | Moyen : cela change l'instant d'échantillonnage. Pas de cache de la sortie filtrée ni entre images ; roues, maintien, reconnexion, P1/P2 et Alt-Tab à préserver. |
| P2 contextuel | [FlushIfDue](../src/ConfigStore.cpp#L1448), depuis Present, sauvegarde après 500 ms de silence dirty ; [SaveNow](../src/ConfigStore.cpp#L1120) écrit les champs séparément en synchrone. | Mesurer cette sauvegarde ; envisager ensuite uniquement les champs modifiés avec échecs/retry et clés inconnues conservés. | Moyen. Candidat aux pointes après réglage/raccourci, **pas une écriture continue**. Déplacer naïvement Store sur un worker introduirait des courses. |
| P3 | [Log](../src/DarksidersGenesisMod.cpp#L318) formate avant certains filtres et ouvre/écrit/ferme par ligne ; [trace fenêtre](../src/DarksidersGenesisMod.cpp#L3237) répète des lectures Win32 par Present. [RecoveryTail](../src/RecoveryTailFeature.cpp#L72) refait certaines classifications pendant une capacité active. | Éviter formatages supprimés, partager l'instantané de fenêtre local, éventuellement cacher seulement la classification immuable d'une identité complète. | Gain probablement modeste ; logs déjà largement plafonnés. Ne pas ajouter une queue, un cache durable ou un délai sans preuve de coût. |

## Fausses pistes et suppressions écartées

| Hypothèse de la première passe | Vérification | Décision |
| --- | --- | --- |
| ImGui dessine constamment en plein écran | HookPresent ne lance NewFrame/Render que pour menu ou notification visibles ; RTV créé à la demande et réutilisé. | Conserver. Aucun second flou plein écran ajouté par ImGui démontré. |
| L'INI est réécrit en boucle | Retour immédiat si non dirty, attente 500 ms sinon. | Optimiser éventuellement la sauvegarde ponctuelle, pas incriminer une boucle permanente. |
| Le réseau/updater bloque l'UI | Travail réseau sur workers, garde busy et auto-check unique. | Pas de cause permanente identifiée ici. |
| Les scans chargent tout sans limite | Wrath : 256 objets, budget ~1 ms contrôlé par lots, délai 15/250 ms. Assets sélectionnés : stabilisation/cooldown, trois tentatives bornées par asset et contexte. | Garder ces limites ; cibler les scans encore sans budget temps. |
| Stoneskin recrée ses matériaux chaque image | Gardes 250 ms au repos/50 ms actif, caches de classes/champs/fonctions et création sur transition. | Pas de création constante démontrée ; préserver restauration et identités GC. |
| Le correctif des flammes du cheval redémarre sans arrêt les effets | Attente, garde 250 ms, tentatives bornées puis activation ciblée unique. | Correctif validé conservé. |
| Les boucles RawInput/XInput installent les hooks en continu | Vérification RawInput 1300 ms ; énumération des DLL XInput 2500 ms ; hooks déjà présents réutilisés. | Ne pas casser reconnexion/Alt-Tab pour un faux gain. |
| La branche debug produit un binaire sans optimisations | Workflow construit la configuration **Release**, malgré le nom de branche ; `/analyze` est un contrôle de compilation. | Ne pas proposer de changer de branche pour « activer Release ». Pas de nouveaux drapeaux agressifs à l'aveugle. |
| On peut enlever les gardes de pointeurs pour accélérer | GC, chargements et réutilisation d'adresse font partie des défauts déjà traités. | Regrouper des lectures, jamais remplacer par des pointeurs persistants non vérifiés. |
| Une référence unique signifie code mort | Hooks, callbacks, exports, compatibilité INI et états de restauration ont des usages indirects. | **Aucune suppression de production démontrée sûre par cet audit.** Une analyse compilateur supplémentaire serait nécessaire avant suppression large. |

## Flou TPS et charge GPU

Le retour utilisateur OFF plus fluide / ON moins fluide reste un indice important. Le code règle la profondeur de champ **native** (méthode Gaussian, région et intensité), il ne calcule pas lui-même un deuxième filtre d'image dans l'ASI. Aucun coût GPU par passe n'a été capturé et aucune optimisation GPU à résultat identique n'est prouvée.

L'audit ne contourne donc pas le problème en désactivant le flou, en baissant sa qualité, en changeant les distances ou les défauts. Les pistes ci-dessus réduisent du travail CPU du mod ; elles peuvent être utiles sans garantir la disparition du coût du flou natif. Aucun cache ne doit empêcher le moteur de réactualiser ses propres paramètres.

## Lots recommandés et critères de réussite

1. **Premier lot contenu** : CRC/préfiltre des shaders, groupement d'identité, focus P2 et validation du fallback DOF après test de nécessité. Traiter le compteur par draw séparément : conserver le diagnostic utile sans opération globale sur chaque draw. Critère : sorties/refus identiques pour identités valides/GC/recyclage/frontières ; CRC et signatures identiques ; caméra octet par octet ; tests Windows existants. Pas de nouveau hook.
2. **Mesure ciblée** : volumes et temps des lectures, services par source, recherche d'apparences, XInput et sauvegarde. Compteurs agrégés/échantillonnés, aucun log par image. Critère : même scène, mêmes réglages, même GPU, fenêtre au premier plan, répétitions contrôlées ; comparer temps CPU et intervalles d'images, sans confondre API Present et affichage effectif.
3. **Second lot conditionnel** : réduire le suivi shader sous verrou et partager le contexte gameplay uniquement si les mesures le justifient. Tester plusieurs contextes de rendu et transitions/entrées avant de considérer le gain acquis.

État de cet audit : **CONTRÔLÉ par revue statique**, propositions **NON IMPLÉMENTÉES / EXPÉRIMENTALES**. Le dernier CI 86/86 concerne `4e32651`, pas des optimisations nouvelles de cette liste. Aucun gain ne passe en VALIDÉ sans retour en jeu. Rollback des futurs lots : sources/ZIP de base conservés, INI/bindings/sauvegardes jamais remplacés. Main/archive et anciens ZIP restent hors périmètre.
