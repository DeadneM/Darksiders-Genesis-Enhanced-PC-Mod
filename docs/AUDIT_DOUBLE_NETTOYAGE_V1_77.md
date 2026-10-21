Suivi V1.78 : le patch ci-dessous a été appliqué au lot OFF/interface après les retours utilisateur. Compilation et tests en attente à ce stade ; le rapport original conserve son statut au moment de l’audit. [Intégration et limites](V1_78_PISTOL_OFF_LIFECYCLE_CLEANUP_TEST.md).

---

# Double audit des leftovers — V1.77

21 octobre 2026. Base examinée : **debug `9dcd5fff6c3fbabe3dde4051f6fe71ac09e70361`**, source cumulative V1.77. Demande : chercher les éléments retirables pendant les essais utilisateur, vérifier deux fois puis comparer les résultats.

**AUDIT STATIQUE CONFRONTÉ / PATCH PRÉPARÉ, NON APPLIQUÉ, NON COMPILÉ.** Les sources de production et le ZIP V1.77 restent identiques. Les 47 tests du build V1.77 ne valident pas ce patch. Aucune installation, relance, modification de mémoire, INI, raccourci, PAK ou sauvegarde du jeu.

## Deux méthodes, puis confrontation

Deux passes séparées par le même agent, avec des méthodes différentes ; aucun second auditeur humain ou agent indépendant n'est prétendu. La passe B a été produite sans lire les listes de candidats de A. Une recherche textuelle fournit des candidats ; elle ne prouve pas seule qu'une fonction est inutile.

| Vérification | Résultat initial | Résultat après confrontation |
| --- | --- | --- |
| **A — compilation, inclusions et références** | 110 fichiers de production, dont 39 `.cpp`, 19 711 lignes physiques ; aucun fichier hors du graphe CMake/inclusions. 156 noms de fonctions vus une seule fois dans leur propre fichier. | **153 ont une référence qualifiée dans un autre fichier**, deux sont les `DllMain` appelés par Windows/CRT. Un getter n'a aucun appel. Pas de suppression de module entier justifiée. |
| **A — interfaces apparemment sans appel qualifié** | 21 candidats, liste recoupant la précédente. | **20 faux positifs** : 15 méthodes/utilitaires de configuration appelés autrement, deux erreurs de classement lexical sur des types, deux helpers appelés sans qualification dans leur namespace et `Potion::CancelRequest`, utilisé à l'arrêt. Même getter inutilisé confirmé. |
| **B — qui écrit, qui lit, lifecycle et effets de bord** | 48 champs de télémétrie alimentés mais sans consommateur, plus un horodatage runtime écrit mais jamais lu. | Absence de lecture confirmée dans le source et les tests ; pas de sérialisation, de pointeur sur ces membres ou d'offset attendu trouvé. Retrait de leurs copies expose **22 états atomiques miroirs** dont les producteurs sont aussi retirables. |
| **Contrôle croisé du patch** | Candidat limité aux chaînes de diagnostic et au getter sans appel. | **131 appels ciblés conservés avec les mêmes arguments**, notamment installations de hooks et appels aux fonctions natives originales. Application à blanc réussie sur la base exacte, sans option d'ignorance des espaces. |

Les divergences initiales viennent du périmètre des méthodes : A cherche les références des fonctions/fichiers ; B cherche les données sans lecteur. Elles ne sont pas interprétées comme deux listes identiques. La confrontation a écarté les faux positifs et remonté jusqu'aux producteurs des données réellement inutilisées.

## Nettoyage préparé

[Patch relisible](proposals/V1_77_CLEANUP_REVIEW.patch) · [Résultats structurés des deux passes et contrôles](proposals/V1_77_CLEANUP_COMPARISON.json).

**12 fichiers, 172 lignes retirées et cinq ajoutées : 167 lignes nettes en moins**, soit environ 0,85 % des lignes physiques du source examiné. Les ajouts remplacent des lignes existantes ; ils n'ajoutent aucune fonction ou option.

| Bloc confirmé | Retrait proposé | Ce qui est conservé |
| --- | --- | --- |
| `EngineIniFeature` | `GetLastAppliedGraphicsAdapter`, sa déclaration, son état privé `g_lastApplied`, son écriture et l'include atomique devenu inutile. | L'application et la relecture de `r.GraphicsAdapter`, les messages de résultat et la conservation du reste d'Engine.ini. Aucun symbole exporté de l'ASI n'expose ce getter. |
| `GaugeChargeFeature` / `RuntimeSettings` | `lastNativeSynergyPositiveTick`, son appel d'horloge et son écriture : aucun lecteur trouvé. | Le calcul, le multiplicateur, les contrôles de propriétaire et l'appel natif du gain Synergy. |
| Interface du menu | **14 champs** de télémétrie copiés vers le menu sans jamais y être lus. | Les champs affichés et les logs fondés sur les variables locales ; aucun contrôle, réglage ou raccourci supprimé. |
| Caméra | **13 champs** de copie télémétrique sans lecteur. | Les valeurs appliquées au jeu, l'état utilisé pour la caméra/visée et la récupération des valeurs natives nécessaires. |
| Cheval | **19 champs** de copie télémétrique sans lecteur. | Les vitesses/endurance, références d'origine, restauration, callbacks et propriétaires des montures. |
| Masquage visuel de visée | **Deux champs** de copie télémétrique sans lecteur. | Les compteurs utilisés dans les logs et surtout `g_matchCount`, qui participe à la décision réelle de masquer un shader. |
| Producteurs devenus inutiles | **22 miroirs atomiques** et leurs écritures, identifiés après retrait des copies ci-dessus. | Leurs valeurs locales et les calculs servant au gameplay ou aux logs encore lus. |

### Piège trouvé lors de la seconde vérification

`g_summonServerProbeReady.store(InstallNativeHook(...))` est un état de diagnostic sans lecteur utile après retrait de sa copie. **L'appel `InstallNativeHook` est indispensable.** Supprimer toute l'affectation supprimerait aussi l'installation du callback cheval.

Le patch retire seulement le stockage du booléen et **conserve cet appel avec tous ses arguments**. C'est contrôlé séparément des compteurs ordinaires ; le générateur refuse de supprimer un stockage dont l'argument contient un appel non examiné. Les autres écritures retirées portent sur des valeurs scalaires déjà calculées.

## Éléments anciens conservés volontairement

- **ID Hotkeys 3 et 54**, tokens et anciennes valeurs personnelles : compatibilité volontaire, pas fonctions Recovery encore actives. Les retirer ou renuméroter risquerait les affectations existantes.
- **Entrées de DLL et exports DXGI historiques** : un appel par Windows ou par nom de symbole ne se voit pas comme un appel C++ ordinaire. Les anciens exports inertes restent une compatibilité de chargement.
- **Fallback géométrique des cinématiques** : encore conditionné par l'indisponibilité du module natif (`autoSuspend && !native_tps::TargetReady()`). Ce n'est pas du code inatteignable dans tous les cas ; son retrait serait une décision de comportement distincte du présent nettoyage.
- **Historique cinématique et marqueur manuel** : consommés par le menu et le journal d'arrêt. Leur ancien préfixe de version ne les rend pas inutilisés.
- **Compteurs de tirs, RawInput, roues, rendu et diagnostics encore lus** : certains alimentent les logs ou une décision fonctionnelle, même s'ils ne sont plus tous affichés dans le menu.
- **Anciens ZIP, documents et workflows** : leur présence dans le dépôt ne les ajoute pas automatiquement à l'ASI. Aucun gain binaire à promettre en supprimant cette histoire ; les archives restent conservées.

## Contrôles et limites

- Les 110 empreintes du source de production sont restées identiques à celles enregistrées avant les audits. Aucun source actif modifié pendant les essais.
- La vérification du binaire V1.77 trouve une table d'exports ASI vide : les structures de télémétrie concernées sont internes. Les exports DXGI sont distincts et conservés.
- Recherche indépendante de lectures, usages dans les tests, `offsetof` et pointeurs sur membres ; contrôle des dépendances du getter supprimable et de l'horodatage.
- Comparaison des appels ciblés avant/après et de leurs arguments, contrôle des terminaisons CRLF/LF et `git apply --check` sur la base exacte. Cela ne remplace pas une compilation C++ ni un essai du jeu.
- Le patch ne touche ni `ConfigStore`, ni l'INI fourni, ni les ID de raccourci, ni les fonctions de filtrage XInput/RawInput. Aucun calcul de projectile, algorithme de caméra ou nouveau hook introduit.
- **Aucune baisse de taille de l'ASI ni hausse de FPS mesurée.** L'allègement du source et le retrait de copies/écritures inutilisées sont établis ; l'effet du compilateur et du linker doit être mesuré sur le prochain build.

La recherche couvre les leftovers du source ASI/loader actif. Ce n'est pas une preuve formelle d'absence de bugs du mod ni un nouvel audit complet de concurrence, de chaque offset natif ou du code C# de l'updater.

## Suite

Intégrer ce patch au prochain lot cohérent après prise en compte des résultats V1.77, compiler Windows x64 et repasser les 47 suites existantes. Vérifier l'absence d'avertissement et mesurer les fichiers produits ; tester brièvement ressources, caméra, cheval/holsters, roues et affichage du menu. Les pouvoirs entre personnages et OFF universel restent dans leur file séparée.

Aucune nouvelle build de diagnostic créée pour cet audit. Les 142 archives existantes et les branches main/archive sont conservées ; V1.77 reste le lot utilisateur en cours d'essai.
