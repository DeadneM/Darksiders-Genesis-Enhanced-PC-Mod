# V1.148 — crash de transition avec sous-titres

Base : debug `456cb1bc97ebfbf0593f20602b0d12d5f3ff2daf`, V1.147. Autres fonctions V1.147 validées par l'utilisateur ; nouvelle correction EXPÉRIMENTALE, non installée par l'agent. Compilation exclusivement GitHub.

## Objectif et critère de réussite

Passer la cinématique d'introduction de Moloch avec les **deux types de sous-titres activés**, conserver leur texte, leurs fonds et le rendu normal du combat. Le compteur du garde doit signaler la commande interceptée. Aucun succès en jeu n'est déduit des tests isolés.

## Preuves et limites

Quatre dumps : même pile native, READ0x20 à RVA D5D0A1. Le dernier ne charge ni notre ASI ni notre dxgi. Le flou TPS personnalisé désactivé ne change pas le résultat. Essai iGPU infructueux rapporté par l'utilisateur. Les deux catégories de sous-titres OFF permettent à l'utilisateur de passer la scène. Des [signalements de joueurs](https://steamcommunity.com/app/710920/discussions/0/3963662127425932021/) décrivent aussi le contournement par les sous-titres ; le résultat local est l'élément décisif ici.

Le moteur reçoit une demande d'effacement stencil sans texture profondeur/stencil puis lit la texture nulle. Le nouveau garde ignore seulement cet effacement impossible et conserve toute la commande restante. Il **ne recrée pas la cible absente** et ne prouve pas encore quel producteur de l'interface l'a omise. Le lien causal précis avec le widget de sous-titres reste à approfondir si l'essai ne suffit pas ; les noms de widgets trouvés dans les assets ne sont pas une preuve de leur responsabilité.

## Implémentation isolée

- Une interception native à D5CDD0, sur le seul exécutable reconnu, après vérification du prologue et des consommateurs des champs +C8/+D8/+D9/+120.
- Prédicat limité aux quatre captures : une couleur non nulle, pas de clear couleur/profondeur, pas de texture profondeur, clear stencil=1, actions profondeur identiques et aucun UAV actif.
- Copie de **0x128 octets** sur la pile, y compris la fin UAV ; seul l'octet +D9 devient 0. Le paquet du moteur n'est jamais modifié. Toute autre commande, ou case OFF, garde le pointeur et l'appel natifs exacts.
- Aucune allocation, verrou, API moteur, appel D3D ou journalisation dans le hook. Compteur atomique ; rapport différé depuis Present, limité en fréquence.
- Camera → Render stability → **Subtitle crash workaround (experimental)**. ON par défaut, `Features.RenderCrashGuard=1`. OFF restitue le chemin natif ; cela peut donc restituer le crash connu.
- Aucune option de sous-titres modifiée, aucun fichier GameUserSettings.ini/Engine.ini écrit par ce garde. La fonction existante de sélection GPU écrit toujours r.GraphicsAdapter comme avant ; sa valeur persiste même sans ASI.

Tests : paquet capturé, protection de l'entrée empruntée, conservation intégrale couleur/UAV/padding, appel unique, identité des arguments sur les cas valides/désactivés, compteur/option runtime, refus de signatures différentes et parité configuration générée. Revue séparée du code et des données réelles : [périmètre](evidence/V1_148_SCOPE_REVIEW.json), [quatre commandes capturées](evidence/V1_148_RHI_CAPTURED_INPUT.json).

## Palette Legion

Retrait demandé des quatre effets Vampiric et de Grasping shadows, uniquement dans Legion shieldbearer. 23 choix visibles, IDs restants inchangés ; anciennes valeurs 7/12/13/14/15 reviennent à Native. Les assets et options des pouvoirs de War restent disponibles. Native, Hidden, Original adjustable et les réglages taille/position validés ne changent pas.

## Essais groupés

1. Réactiver les deux types de sous-titres dans les options du jeu. Avec le garde ON, refaire la scène qui plantait ; vérifier texte, arrière-plan et combat après la transition. Refaire une seconde fois si elle passe.
2. Lire le compteur dans Camera → Render stability et le journal `Render safety V1.148`. Compteur nul = aucune interception observée, pas une preuve que le garde a corrigé cette scène.
3. Vérifier une autre cinématique, les menus, roues et HUD dans les deux vues, notamment leurs découpes/transparences. Le garde n'est pas une validation de ces rendus.
4. Vérifier la palette Legion et un choix conservé après chargement ; taille/position, Stinger et Léviathan doivent garder les résultats acceptés.

Repli : désactiver la nouvelle case ; pour passer la scène sans garde, le contournement sous-titres OFF a été confirmé par l'utilisateur. V1.147 reste disponible sans remplacement d'archive. INI personnel, raccourcis, sauvegardes, PAK et branches main/archive conservés. ZIP limité à ASI + dxgi.dll à la racine.

## Chargeur alternatif

Notre proxy relaie les entrées DXGI à System32 et charge les ASI voisines. Les échecs de démarrage du chargeur alternatif sont différents du crash natif de sous-titres. Son nom/version/binaire exact n'est pas disponible pour comparer architecture, exports et ordre de chargement : aucune cause affirmée et aucune exclusivité de notre chargeur démontrée. Le pont de journalisation propre au proxy est facultatif dans l'ASI. Le proxy V1.147 accepté reste inchangé.
