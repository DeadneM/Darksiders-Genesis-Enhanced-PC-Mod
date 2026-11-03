# V1.148 — livraison debug, 3 novembre 2026

**CONSTRUIT / CONTRÔLÉ. Garde de crash EXPÉRIMENTAL, à valider en jeu.** Base V1.147 dont le reste est validé par l'utilisateur ; aucune installation, relance ou écriture de fichiers du jeu par l'agent.

[ZIP cumulatif à deux fichiers](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f6684c13256c21aea912418ead0753d4cbdeaf31/downloads/DarksidersGenesis_V1.148_STENCIL_TARGET_GUARD_TEST.zip) — [objectif, code et limites](V1_148_SUBTITLE_CRASH_TEST.md).

## Changements

Garde de la demande d'effacement stencil avec cible absente retrouvée dans quatre dumps, dont un sans ASI/chargeur. L'utilisateur a confirmé que désactiver les deux types de sous-titres permet de passer Moloch. La nouvelle version permet de **les réactiver pour essayer le garde**, sans changer ces réglages à sa place. Case Camera → Render stability → Subtitle crash workaround (experimental), ON par défaut ; OFF restitue la fonction native.

Le garde conserve la commande complète et ignore seulement l'effacement impossible correspondant aux captures. Aucun tampon n'est recréé et le producteur initial fautif n'est pas encore identifié : le rendu des sous-titres, des découpes de l'interface et du combat reste à valider.

Quatre choix Vampiric et Grasping shadows retirés du bouclier Legion seulement ; 23 choix visibles. Anciennes valeurs retirées vers Native ; autres IDs, bibliothèque de War et réglages des effets validés conservés.

## Contrôles vérifiés

- Compilation/publication [004b9f4](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41670091124) : **75/75 suites**. Un C4312 dans le seul test isolé a été corrigé par conversion de largeur explicite ; aucun avertissement du code de l'ASI/proxy. [Second contrôle](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41671452502) : **75/75, aucun avertissement de compilation C/CS/LNK/D détecté**. Seul le test change, tous les fichiers de production sont identiques. Le second contrôle ne republie aucune archive.
- ZIP 2353902 octets, SHA256 `FD32078FD0E60602AFB6CD545984BEA76E89DB5FC490F203059AADD39DB34B94` ; ASI + dxgi.dll à la racine, AMD64/CRC vérifiés, sans INI/README. Téléchargement public HTTP200 et sélection updater debug de mêmes octets vérifiés. Interface et installation en jeu non observées.
- **212 anciennes archives inchangées**, 213 au total ; main/archive/tag v1.43 intacts. Les 245 anciennes valeurs par défaut et 55 identifiants Hotkeys restent identiques. Seule nouvelle clé : Features.RenderCrashGuard=1.
- Source **et section machine .text/export table du proxy identiques à V1.147**. L'empreinte globale du fichier recompilé diffère ; cela ne correspond pas à un nouveau comportement du chargeur. Notre fonction existante écrit toujours r.GraphicsAdapter dans Engine.ini, et ce réglage persiste sans ASI. Cause de l'échec au démarrage du chargeur alternatif non établie, différente du crash sous-titres.
- Defender : aucune détection ZIP/binaires, protections actives et inchangées. Revues source/configuration et binaire/dumps séparées, concordantes. Ces contrôles ne prouvent ni réussite du passage de scène ni absence de défaut visuel en jeu.

Source compilée : `004b9f49216c5869944ec08d2d1c1bb391e2b4ae`. Archive immuable : `f6684c13256c21aea912418ead0753d4cbdeaf31`. Correction du seul test : `436d26736442636e8f818b3ccc67ce264471be7a`. [Preuves de compilation](evidence/V1_148_CI_CHECK.json), [ZIP](evidence/V1_148_PACKAGE_CHECK.json), [updater](evidence/V1_148_UPDATER_CHECK.json), [proxy](evidence/V1_148_PROXY_COMPARISON.json).

Complément chargeur : retrouvé dans Downloads/dxgi-x64.zip, **Ultimate ASI Loader 10.0.2**, GitHash56fa0b0. Architecture x64 correcte, CreateDXGIFactory et CreateDXGIFactory1 présents : ce n'est pas un manque de ces exports. Le code officiel contrôle la présence d'InitializeASI avant de l'appeler ; son absence dans notre ASI n'explique donc pas à elle seule un appel nul. Différence des points de chargement étudiée, causalité du blocage non démontrée. Aucune installation/exécution de cette DLL pendant l'analyse. [Constat binaire](evidence/V1_148_ALTERNATIVE_LOADER.json).

## Test attendu et suite

1. Réactiver les deux types de sous-titres et rejouer la scène de Moloch avec le garde ON ; vérifier le texte, son fond et le combat après la transition. Refaire une fois si elle passe.
2. Consulter le compteur et le journal Render safety V1.148. Un compteur nul indique que ce garde n'a pas été observé, même si la scène passe.
3. Vérifier menus/roues/HUD, une autre cinématique et les apparences Legion conservées. Taille/position, Stinger et Léviathan doivent rester comme validés.

Si l'essai échoue, conserver le contournement sous-titres OFF confirmé et relever le nouveau crash sans ajouter un autre correctif spéculatif. V1.147 reste disponible. Pose après munition, Stoneskin intermittent et dash aérien restent ouverts/reportés, sans changement dans ce lot. Aucun test coop supplémentaire requis avant ce passage.
