# V1.41 — audit complet et nettoyage prudent

Date : 16 octobre 2026. Branche de travail et de livraison : **debug uniquement**.

## Base, objectif et statut

Base livrée V1.40 : `e230b53cc5a1d57b48b6e9c29c3d373a5654d47a`, issue de V1.39. Les munitions sont **VALIDÉES par l'utilisateur** ; les contrôles TPS ordinaires, roues, cheval, RawInput relatif, Alt-Tab, remplissages, gains de jauges et Skip Warning gardent les validations acquises. Action Recovery n'a pas été validé : l'utilisateur ne voit aucun effet.

Objectif de cette version : enlever les interventions abandonnées et le travail inutile démontrable, corriger les erreurs de configuration/mise à jour, remettre **Inser** par défaut et préserver les mécaniques acquises. Réussite attendue : Windows x64 construit, analyse statique examinée, tests de politiques/configuration réussis, ancien profil conservé sauf MenuKey, ancien stock de ZIP et branches protégées intacts ; puis test utilisateur sans régression. **Une compilation ne valide pas le comportement en jeu.**

Statut final de livraison : **CONSTRUIT / CONTRÔLÉ / INSTALLÉ**, nettoyage **EXPÉRIMENTAL** en attente de retour utilisateur. Source `d1bae62`, CI Windows `38843203433` réussie, 13/13 tests réussis ; archive `0b42a19`. Action Recovery reste **NON FONCTIONNEL / ESSAI RETIRÉ**, pas réparé.

## Méthode et limites de l'audit

Inventaire de tous les modules `src/`, en-têtes/politiques, tests, mise à jour C# et workflows ; recherche des références, lecture des callbacks, signatures, gardes de propriétaire et des chemins d'installation/retour arrière. Comparaison avec la base Git, README, carnet technique, état courant et audits précédents. Une référence textuelle unique est seulement un candidat : les exports DXGI et fonctions fournies à MinHook/GetProcAddress restent nécessaires même sans appel direct.

Dernier journal disponible : session V1.39 du 15 octobre, 654 lignes, copiée avant toute installation. Il contient `Action Recovery ... ticks=12132 localMontages=8927 blendTails=0 normalFinished=0`, `Wrath=0 Chaos=0 Synergy=458` pour les anciens hooks et 144 gains Synergy modifiés. Ce sont les preuves de cette session, pas une preuve que ces fonctions ne peuvent jamais être appelées dans un autre contenu du jeu.

Cet audit ne prétend pas prouver l'absence de tout bug, mesurer les FPS, ni remplacer une session de jeu. Les adresses natives restent conditionnées à l'exécutable exact connu et à leurs signatures. Aucun jeu lancé automatiquement, aucune écriture mémoire en jeu pour cet audit.

## Erreurs confirmées et corrections

| Sujet | Constat démontré dans V1.40 | Traitement V1.41 |
| --- | --- | --- |
| Sauvegarde INI | Les retours de `WritePrivateProfileStringW` sont ignorés ; SaveNow efface dirty et annonce toujours « INI saved ». Le bouton affiche aussi une réussite sans vérifier. | Chaque écriture contribue au résultat. Échec : false, modifications en attente, nouvelle tentative après délai normal, diagnostic limité ; le menu affiche l'échec. La création d'un INI manquant et le rechargement propagent l'échec ; un chemin répertoire est refusé. |
| Version de mise à jour | La comparaison de version est encore figée à V1.22 et à son ancien ZIP. | Identité courante partagée dans BuildInfo ; même version, version ancienne et plus récente distinguées par les tests. |
| Historique d'installation de l'updater | Un ancien installed.txt peut survivre à un remplacement manuel des binaires et servir de mauvaise référence à l'installation automatique. | État lié au SHA-256 de l'ASI **et** du DXGI réellement présents. Historique ancien, manquant ou devenu incohérent : installation considérée manuelle, action explicite requise par le flux existant. Les modifications de l'INI restent autorisées. |
| Concurrence des commandes | Des callbacks natifs/XInput lisent directement des champs ordinaires de Store pendant que le menu peut les modifier. | Huit réglages reconfigurables publiés dans les atomiques runtime ; les callbacks lisent ces valeurs. Publication des deux booléens TPS/cheval déjà existants réutilisée. Aucune règle de remappage, de tir, de stick ou de délai changée. Ce correctif n'est pas un audit de sûreté exhaustif de tous les états du mod. |
| Livraison C# | Le workflow republie toujours le même ZIP updater V1.0, écrasant l'ancienne archive lors d'un changement. | Nouveau ZIP distinct **Updater V1.1** ; V1.0 conservé. Le workflow du mod suit désormais aussi les changements updater embarqués. |

La sauvegarde INI reste une série d'écritures, **pas une transaction atomique complète** : un échec intermédiaire peut laisser un fichier partiellement actualisé. V1.41 cesse de le présenter comme réussi et garde la possibilité de réessayer. L'updater conserve ses confirmations, le contrôle d'exécutable, les quatre fichiers attendus, la vérification GitHub, les sauvegardes et le fonctionnement après fermeture du jeu.

## Restes supprimés

- Ancien patch de boucle warning à `0x25FEBF`, désactivé depuis V1.04 après régression du réticule : générateur de trampoline, variables et chemin de configuration inutilisés retirés. Les trois exports legacy restent des stubs FALSE pour préserver l'ABI. Le **SkipWarningFeature validé**, qui agit sur la présentation réelle, est identique.
- Anciens hooks AddWrath `0x660570` et AddChaos `0x62E060` : zéro événement dans le dernier test, remplacés pour les gains validés par ResourceGainFeature. Deux interceptions en moins ; les calculs validés des setters Wrath/Ultimate et les deux hooks Synergy gardent leur code.
- Essai Action Recovery V1.39 au Tick `0x68D050` retiré, ainsi que son ancien test de politique : aucune transition écourtée dans les logs, aucun bénéfice utilisateur. L'INI et les identifiants de raccourci restent lisibles ; le menu indique l'indisponibilité et un ancien raccourci ne modifie rien. Il reste à identifier le véritable verrou après dash/coffre avant tout nouvel essai.
- Fonctions privées sans appel : FindAsciiString, AddressInSection, MountedVanillaAimWhileFiring, RestoreSlotLocked du cheval, ancien traducteur AbilityStateName et wrappers inutilisés de warning.
- Ancien pont Ctrl complètement vide, état et branche de relâchement devenus inaccessibles ; la touche H synthétique encore utilisée par le chemin cheval est conservée.
- Ancienne observation du flag de rotation : branche de restauration jamais propriétaire dans cette base, verrou et compteurs supprimés. Aucun nouveau changement de rotation native.
- Sondes de corrélation Courroux/tir/dégâts, anciennes observations de getters montés, comparaison de yaw et diagnostic d'invocation cheval retirés. Les timestamps d'intention qui autorisent réellement le cheval restent écrits aux mêmes événements.
- Incréments situés après un retour qui les rendait inaccessibles, compteurs constants et télémétrie de menu jamais consommée nettoyés. Les diagnostics encore utilisés pour RawInput, état caméra natif, cinématiques, Unlimited et munitions restent disponibles.

## Optimisations contrôlables

Le total de source native passe de **14795 à 13993 lignes**, soit **802 lignes nettes en moins** dans le source construit. Cette mesure décrit le code, pas la vitesse. Trois installations de hooks natives actives en moins (les deux vieux gains et l'essai recovery). La génération de l'ancien hook warning, déjà inactive, ne compte pas comme un hook actif supprimé.

Les deux callbacks fréquents de visée qui demandaient un instantané complet de télémétrie pour obtenir seulement le yaw lisent maintenant **un seul atomic**, avec le même ordre mémoire et la même valeur. Le corps des calculs et de la publication de caméra est conservé. La suppression des sondes évite des lectures mémoire, recherches de fenêtre/touches, compteurs et copies de télémétrie qui n'agissaient pas sur le gameplay. **Aucun pourcentage de FPS ou de latence n'est annoncé sans mesure.**

## Ce qui reste volontairement

| Zone | Pourquoi la conserver / suite possible |
| --- | --- |
| Gros fichier principal et nombreuses petites gardes mémoire semblables | Dette réelle de lisibilité. Une extraction par responsabilité sera utile, mais les gardes diffèrent sur protection, portée et propriétaire. Les fusionner globalement dans ce nettoyage augmenterait le risque. |
| Pointeurs UE4, captures de pose, publications multi-champs | Les vérifications de mémoire et timestamps ne constituent pas une garantie de durée de vie ni un instantané cohérent global. Revoir isolément l'ownership/génération et les échanges game/render/XInput avant une refonte. Aucun crash attribué à cette hypothèse ici. |
| Store, WndProc et captures de raccourcis | Les huit lectures natives traitées ne résolvent pas à elles seules toute la concurrence potentielle entre fenêtre et rendu. Un modèle de commandes/configuration en instantanés reste une suite possible. |
| Shaders et appels Draw | Filtrage validé, références shader bornées et libérées. Les compteurs atomiques et verrous par appel Draw peuvent coûter ; la table des contextes n'est pas purgée à la destruction de chaque contexte. Besoin de profilage et d'un test de recréation D3D avant changement. |
| Initialisation/fermeture, DllMain du proxy | Chargement d'ASI et initialisation startup encore proches du loader lock, arrêt global MinHook/COM à examiner avec une vraie stratégie d'unload. Risques structurels, pas des incidents démontrés par les logs fournis. Pas de remplacement improvisé du loader dans cette version. |
| Diagnostics cinéma V1.28 et événements UI natifs | Ils servent encore au débogage et aux roues. Le callback ProcessEvent appelle ObserveUiEvent : son nom ancien de « probe » ne le rend pas mort. Historique borné conservé. |
| Clés legacy de l'INI et identifiants d'actions | Compatibilité des anciens profils. `ActionRecovery`, `ActionRecoveryDelayMs`, `ActionRecoveryMultiplier` et `DodgeEarlyUnlockMs` restent sérialisés sans chemin actif ; quelques publications runtime anciennes associées, et tpsFollowPlayer désormais constant, restent à retirer lors de l’extraction de configuration ; elles ne sont pas présentées comme des fonctions efficaces. Les supprimer ou renuméroter casserait des configurations. |
| Updater et rollback | Copie par fichier avec restauration, pas transaction de tout le lot ; interruption/disque plein peut rendre la restauration elle-même partielle. Gestionnaire autonome à éprouver séparément, pas d'installation réseau automatique utilisée comme test. |
| Nombreux workflows anciens | Majoritairement manuels ou limités à leurs branches, ils gardent la reproductibilité des anciennes expériences. Ils ne sont pas du code de gameplay actif. Ne pas supprimer leur historique ni les anciennes archives. |

Les écarts éventuels du tir secondaire et la couverture précise de chaque attaque de Strife ultime/recentrage ne sont pas déclarés résolus par ce nettoyage. La validation utilisateur précédente reste attachée à ce qui a réellement été observé.

## Vérification, compatibilité et test utilisateur

Contrôles de source : modules munitions, ResourceGain, ManualGauge, NativeTPS, ChaosPose, règles des sticks, Skip Warning, shaders, Engine.ini et validation d'exécutable comparés à V1.40 ; fonctions restantes du cheval et fonctions Synergy identiques. Caméra : fonctions préexistantes identiques, ajout d'un getter yaw seul. Seule différence de valeurs dans les **110 clés** de l'INI livré : `[Overlay] MenuKey=Insert` ; 109 autres réglages conservés. Les boutons Default et génération d'un INI manquant utilisent aussi Inser. Un profil existant avec une autre touche reste accepté.

CTest exécuté, **13/13 réussis** : politiques des sticks/jauges/munitions/warning, vrais cycles INI/hotkeys/caméra, parité des 110 valeurs, échec réel d'écriture Windows puis reprise, version updater et état C# avec remplacement manuel ASI/DXGI. Analyse MSVC `/analyze` activée sur les sources natives du mod/proxy ; les avertissements seront analysés dans le reçu de construction, sans les confondre avec une validation en jeu.

Test après installation : ouvrir/fermer avec Inser, utiliser un remplissage et Unlimited puis le désactiver, vérifier une dépense de munitions ; tir pied/cheval, deux configurations de sticks, trois roues, pause, Alt-Tab, cinématique et forme ultime. Dash/coffre restent au timing natif attendu, puisque Recovery est retiré. Noter toute régression avec le journal de cette nouvelle session.

Retour arrière : réinstaller les fichiers sauvegardés avant V1.41 ou les quatre fichiers V1.40, en conservant l'INI personnel. Anciennes archives intégralement conservées, main/archive non modifiées. Livraison attendue : `DarksidersGenesis_V1.41_AUDIT_CLEANUP_INSERT_TEST.zip`, exactement ASI + dxgi.dll + INI + README à la racine. Updater autonome V1.1 séparé, deux fichiers ; il ne s'ajoute pas au ZIP cumulatif du mod.

## TODO cumulatif après ce nettoyage

1. Obtenir la validation utilisateur V1.41 sur les acquis ; ne pas confondre les tests automatiques et le jeu.
2. Rechercher la vraie fin du verrou dash/coffre avec preuve native avant de réintroduire Action Recovery.
3. Profiler les hooks Draw et le journal, puis isoler configuration/entrée, caméra et combat du fichier principal sans changer leur comportement.
4. Étudier les durées de vie UE4 et la cohérence des poses/configurations entre threads ; garder chaque changement indépendant et réversible.
5. Tester séparément l'updater en installation/rollback, interruptions et profil personnel conservé.


## Reçu d'analyse statique — huit avertissements examinés

Ces diagnostics n'ont pas été masqués. Ils restent dans le journal de la CI réussie ; l'analyse statique n'est pas une preuve de comportement en jeu.

| Diagnostic | Lecture du code et décision |
| --- | --- |
| dxgi_proxy.cpp:170, C6385 | Buffer local `name[190]` initialisé à zéro ; taille native validée 1..180, n après retrait du zéro final vaut 0..180 ; copie n caractères, terminateur à n, boucle i<n. L'accès local signalé est borné dans ce flux. La garde de mémoire native reste une garde instantanée, pas une garantie générale de durée de vie. |
| CameraTraceFeature.cpp:705, C4456 et C6246 | Deux diagnostics pour le même masquage de variable locale n, avec deux compteurs distincts dans des scopes imbriqués. Lisibilité à améliorer dans un prochain changement isolé, aucune modification des calculs caméra dans V1.41. |
| CameraTraceFeature.cpp:306, C6393 | Le nombre 365 désigne la taille d'un buffer **de texte de log**, pas une table de calendrier. Le diagnostic d'année bissextile ne correspond pas à son usage. |
| SkipLogosFeature.cpp:103, C6011 | Apply refuse le cas pointeur nul + résolution échouée ; la réussite de ResolveProxyExports exige les quatre exports, dont celui appelé. L'analyse ne transporte pas cette postcondition. Une capture locale et un test explicite rendraient le contrat plus lisible ; aucun null dereference observé. |
| SkipWarningFeature.cpp:129, C6385 | CanAdvanceWarning impose count1..16 et une page valide : current>=0 sinon target>=0, page<count. La lecture de durations[page] est donc dans les 16 éléments. Les tests de politique passent ; le corps validé du warning reste identique. |
| UpdateFeature.cpp:149 et :163, C6262 | Deux buffers wchar_t[32768], environ64Ko chacun, dans la localisation/extraction du helper hors boucle de jeu. Diagnostic de budget de pile, pas de dépassement démontré. Une allocation sur heap reste possible dans un travail isolé sur l'updater. Le nom temporaire historique Updater-1.14 est également cosmétique ; la version réellement comparée vient désormais de BuildInfo. |

Autre simplification possible dans ce dernier module : gérer par RAII les références temporaires de module du helper, déjà épinglé pour protéger ses threads. Ne pas confondre le pin intentionnel de durée de vie et une fuite de contenu par frame. Les points de dette ci-dessus restent explicitement ouverts ; cet audit ne prétend pas que tout code historique a disparu.

## Preuves de livraison et rollback

Archive mod : `0BEB4EAFC2C1212EC56EB5CC4FEA32F4F8ECB360CC0D2A0F0B1F82AF2DEDB67B`, 803900 octets. ASI1182720 octets, SHA256 `2B9CE06376B925196C39BC70139F46D02D84D3C7E3FFBE822791702B1C78732B` ; DXGI155648 octets, SHA256 `23AF8685D22826EEC8027C7CA574BB515C3B05FD795BC4CEED236C92D481F4DA`. Anciennes103 archives identiques ; main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89` vérifiés.

Updater autonome V1.1 : 17024 octets, SHA256 `4D1C34F44E90753608F8EC9E5EBC09C420142C636AA7063C299F5EFE09524271`, publication `664e2d9` ; V1.0 conservé. Ce binaire autonome n'est pas installé ou lancé automatiquement par cet audit.

Sauvegarde : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v141-20261016-050412`. Les quatre anciens fichiers et le journal sont conservés avec leurs hashes. INI avant/après : `A351972C25730520AC248854F0032C8A659FA94434F2C9F0F8111CCAD772EB8D` / `FA634ED0793339FC6AE34E72C08D47A86447CCB8300295EE9056C8BFDECF097E` ; comparaison brute et sémantique limitée à MenuKey. Pour revenir en arrière, jeu fermé, recopier les quatre fichiers de ce dossier ; conserver le nouvel Inser si souhaité en ne restaurant pas l'INI, ou restaurer l'INI pour retrouver exactement le profil antérieur. Aucun jeu lancé par l'agent.
