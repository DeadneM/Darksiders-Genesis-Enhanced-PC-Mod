# V1.186 sur debug — choix HDR et lames de Strife

**Construite et contrôlée.** 86 tests passent sur Windows x64. [Livraison et statut](V1_186_DELIVERY_RECEIPT.md).

Base V1.185 contrôlée. V1.184 a généré la capture HDR scRGB. Les choix HDR d'origine reviennent, les lames rangées sont réorientées et reprennent les paramètres de profondeur natifs à cheval. Vérification en jeu à faire. [Périmètre](V1_186_HDR_CHOICES_BLADE_PRESENTATION_REVIEW.md).

## V1.185 construite et contrôlée sur debug

Les 86 tests Windows x64 passent sans avertissement C++. Le candidat reste à essayer en jeu. [Contrôle GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45177336441).

Résolution interne et reflets en mission, lames tenues pendant la mêlée montée. Le contrôle GitHub ne produit pas de ZIP. V1.184 reste la build publiée pendant l’essai HDR. [Périmètre et critères](V1_185_LIVE_QUALITY_MOUNTED_BLADES_REVIEW.md).

# V1.184 livrée sur debug — HDR natif

**Construite et contrôlée**. L’image HDR reste expérimentale. [Livraison et essai](V1_184_DELIVERY_RECEIPT.md).

V1.183 a été rejetée en jeu malgré la signalisation scRGB acceptée. V1.184 rétablit l’accès du post-traitement au chemin HDR après détection d’une sortie HDR active. [Périmètre et essai](V1_184_HDR_RENDER_PATH_REVIEW.md).

# V1.183 livrée sur debug — HDR scRGB

**Construite et contrôlée**. [ZIP, contrôles et essai](V1_183_DELIVERY_RECEIPT.md).

Base cumulative V1.182. Signalisation scRGB 1000 confirmée sur le PC du joueur en V1.182. La conversion automatique des anciens profils et le rendu en jeu restent à vérifier en V1.183. [Diagnostic et essai](V1_183_HDR_SCRGB_REVIEW.md).

# V1.182 livrée sur debug

**Construite et contrôlée**. Les trois corrections sont à valider en jeu. La sélection du format HDR est couplée à l’encodage moteur, les lames rangées sont créées séparément et la visibilité des pistolets suit la transition de monture. Main reste en V1.173. [Livraison et essais](V1_182_DELIVERY_RECEIPT.md).

---

# V1.182 en préparation sur debug. Elle reprend les holsters croisés validés et ajuste leur transition, la création des lames rangées et le démarrage HDR. Validation en jeu attendue.

[Périmètre et essai](V1_182_REVIEW.md).

---

# V1.181 livrée sur debug — main V1.173 préservée

86/86 tests, zéro avertissement C++, ZIP de deux binaires et Defender vérifiés. Holsters montés, copies des lames rangées et transport HDR EXPÉRIMENTAUX : test visuel requis. Lames natives tenues à cheval encore ouvertes. Aucune installation. [Livraison](V1_181_DELIVERY_RECEIPT.md).

---

# État courant — V1.181 préparée, main V1.173

Holsters montés, copies des lames rangées et transport HDR concordant. EXPÉRIMENTAL, CI à effectuer. FX de mêlée validé inchangé ; lames natives tenues montées encore ouvertes. [Périmètre et limites](V1_181_REVIEW.md).

---

# État courant — V1.180 livrée, main V1.173

86/86 tests, zéro avertissement C++, ZIP vérifié et Defender exécuté. HDR expérimental sans restriction de constructeur, Native par défaut. Graphics V1.179 cumulé ; acquis V1.178 conservés. Résultat réel à tester, aucune installation. [Livraison](V1_180_DELIVERY_RECEIPT.md).

---

# État courant — V1.180 préparée, main V1.173

V1.179 Graphics construite/contrôlée, vérification Defender non exécutée faute de quota d'autorisation. V1.180 ajoute les mêmes requêtes HDR à toutes les cartes, sans filtre Intel ; Native par défaut, résultats à tester. Effets et visibilité des armes intacts. [Détails](V1_180_REVIEW.md).

---

# État courant — V1.179 préparée, V1.178 retenue

Onglet Graphics et état HDR en préparation ; aucune activation HDR ajoutée. Effets de mêlée validés à pied/cheval. Les lames sont déjà invisibles à cheval avec le choix Native selon le retour utilisateur ; visibilité tenue/rangée à étudier plus tard, sans toucher aux acquis. [Détails](V1_179_REVIEW.md).

---

# État courant — V1.178 livrée, main V1.173

86/86 tests, zéro avertissement C++, ZIP contrôlé. Correction montée expérimentale, V1.177 et ses 286 valeurs conservées. HDR Intel D3D11 sans implémentation native dans le chemin étudié ; aucun forçage ajouté. Aucune installation. [Livraison](V1_178_DELIVERY_RECEIPT.md).

---

# État courant — V1.178 préparée, V1.177 validée

Correction des FX de mêlée montée préparée, sans changement à pied ni au rendu V1.177. HDR natif D3D11 : branche Intel sans implémentation, aucun forçage ajouté. [Correction](V1_178_REVIEW.md) · [HDR](HDR_NATIVE_BACKEND_REVIEW.md).

---

# État courant — V1.177 livrée, main V1.173

86/86 tests, zéro avertissement C++, deux binaires contrôlés. Sept options Native au redémarrage ; aucun profil personnel ni Engine.ini modifié pendant le développement. Résultat visuel encore expérimental. [Livraison](V1_177_DELIVERY_RECEIPT.md).

---

# État courant — V1.177 préparée, main V1.173

Qualité de rendu au redémarrage ; sept nouvelles options Native par défaut, helper existant étendu avec sauvegarde et restauration des seules valeurs possédées. CI à effectuer ; aucun fichier du jeu modifié. [Détails](V1_177_REVIEW.md).

---

# État courant — V1.176 livrée, main V1.173

86/86 tests, zéro avertissement C++, ZIP de deux binaires vérifié. 17 nouveaux réglages Native par défaut ; rendu à tester, aucun gain GPU annoncé. Aucune installation. [Livraison](V1_176_DELIVERY_RECEIPT.md).

---

# État courant — V1.176 préparée, main V1.173

17 paramètres post-traitement supplémentaires Native par défaut. Contrat chromatique V1.175 corrigé (bit voisin rejeté). Aucune installation, rendu non validé. [Analyse et tests](V1_176_REVIEW.md).

---

# État courant — V1.175 debug, main V1.173

V1.175 : véritable intensité chromatique, sans nouveau hook ; motion blur V1.174 validé conservé. 86/86 tests, zéro avertissement, rendu à tester. Aucune installation. GPU toujours sans comparaison exploitable. [Livraison](V1_175_DELIVERY_RECEIPT.md).

Nouvelle case Summon horse in the air juste sous Horse Everywhere, OFF par défaut. Action native conservée, résultat de sol adapté uniquement pendant un appel aérien local ; règles de zone/déblocage inchangées. 259 clés dont 258 anciennes valeurs préservées, 55 actions inchangées. Fonction EXPÉRIMENTALE jusqu’au retour en jeu. [Périmètre et tests](V1_172_REVIEW.md).

Petite optimisation CPU du suivi des shaders cumulée, gain non mesuré. Micro-saccades toujours ouvertes : compteurs 3D 51–65 %, environ 3,2 Gio dédiés pendant le relevé ; 20 secondes DXGI sans intervalle API >25 ms, pas de temps d’affichage effectif mesuré. [Diagnostic et limites](GPU_FREEZE_REVIEW_2026_11_19.md). Identifiant GPU vérifié séparément : Intel Arc B580, pas Radeon 780M.

INI/raccourcis personnels et installation intacts. [File active](ACTIVE_QUEUE.md).


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
