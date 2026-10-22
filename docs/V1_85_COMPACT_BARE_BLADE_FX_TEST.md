# V1.85 — lot cumulatif effets de mêlée et protection Stoneskin

22 octobre 2026. Base V1.84 `6e27cf7`, debug seulement. Acquis validés V1.81 et suivi TPS des exécutions V1.82 conservés. Les effets de mêlée V1.82 étaient inefficaces dans l'essai utilisateur ; le nouveau chemin reste **EXPÉRIMENTAL, non validé visuellement**. Aucune installation ou relance par l'agent.

## Objectif et différence avec le build intermédiaire

Livrer le correctif du chemin de particules attachées et le prototype Stoneskin dans un paquet cumulatif, après suppression réelle de l'avertissement de compilation de V1.84. Cette version avait réussi 51/51 suites mais signalait C4324 pour le padding de la structure privée Plan. V1.85 place le transform aligné au début, utilise un indicateur 32 bits et contrôle `sizeof(Plan)==128`. Aucun pragma ou réglage de compilation ne masque l'avertissement. Calculs, ABI natifs, conditions d'activation et contenus du lot inchangés.

## Fonctions incluses et limites

- Effets des épées Strife masquées : traitement des huit notifications ciblées attachées à Bone_Strife_Root, auparavant exclues, et des effets détachés des troisième coup/variante. Origine main et taille ×0,4 ; parent/socket/orientation natifs conservés. Une seule lame cachée : seuls les arcs attribuables à ce côté sont modifiés ; les arcs communs restent natifs. Dégâts et portée inchangés. [Contrats V1.84 et preuve VM_READ](V1_84_ATTACHED_BARE_BLADE_FX_TEST.md), [données sans adresses privées](evidence/V1_84_MELEE_ATTACHED_CONTRACT.json). La lecture des objets ne prouve pas le passage réel du callback ; logs d'ajustement bornés, essai visuel nécessaire.
- Suivi TPS des exécutions : **VALIDÉ par l'utilisateur dans son essai**, code préservé. Pas d'extension de validation à toutes les exécutions/coop.
- Capacités : premier prototype de **protection Stoneskin de War vers Strife normal à pied** dans l'un de ses trois slots. Slot d'origine débloqué, coût natif de Stoneskin, animation de repos compatible pendant le cast. **Partiel et non validé en jeu** : onde de choc, sons et apparence de pierre non transférés. Cinq autres transferts encore ouverts. [Détails et limites V1.83](V1_83_DELIVERY_RECEIPT.md).

Bras OFF animés, modèles/holsters, tirs/World Ender, souris/manette/roues, cheval/saut/sons, ressources, 166 réglages INI et 55 ID Hotkeys conservés. Aucun nouveau réglage ; slots Native par défaut. 149 anciennes archives préservées, main/archive/v1.43 intacts.

## Essais groupés et suite

1. Strife à pied : lames Native, gauche cachée, droite cachée puis les deux. Combo au sol et attaques aériennes ; comparer taille/origine des arcs et conservation des dégâts. Contrôle court des bras, roues et suivi d'exécution TPS.
2. Facultatif et séparé : mettre Stoneskin dans un slot Strife déjà débloqué, désactiver Unlimited Wrath/Health et variantes Molten pour comparer le statut de base. Vérifier coût, protection temporaire, reprise des contrôles et expiration, puis remettre Native. L'aura n'est pas un critère de ce prototype. Icônes de roue et coop non validées.

Installation manuelle avec l'INI personnel. Rollback : ASI V1.82 conservé pour le dernier suivi d'exécution validé, même INI ; aucun PAK ou fichier de sauvegarde modifié. Après les retours : compléter les notifications de lancement de Stoneskin puis adapter Caltrops vers War. Réception lourde incertaine, autres FX P2 reportés, HUD général non reproduit.
