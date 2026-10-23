# V1.87 — crash réticule : contrat de retour natif

23 octobre 2026. **EXPÉRIMENTAL, correctif préparé ; compilation et contrôles à consigner après CI.**

## Base, objectif et critère

Base cumulative V1.86 `7763e99de8817362e69099cc339287ea07732902`. Son interception de réticule est rejetée après le crash utilisateur. Les autres fonctionnalités V1.86 sont incluses sans nouvelle validation supposée ; acquis V1.85/V1.82/V1.81 conservés. Objectif unique : transmettre le type natif exact du curseur sans valeur indéfinie. Réussite à confirmer en jeu : quatre modes du réticule, pause/inventaire/roues et transitions sans crash, curseur utilisable dans les menus, comportement de visée inchangé.

## Cause démontrée et correction

Crash du 23 octobre vers 19:42:14 heure de Paris, ASI V1.86 identique au ZIP publié (SHA256 `23B7C932FBDA65EA901C8DD77AF856F5E62C725B33E4F87FCB890643E65D8127`). Rapport local conservé en privé ; aucun dump, identifiant de compte ni sauvegarde publié.

Le minidump donne une lecture invalide à l'instruction native `B10F66`, dans le tableau des curseurs. Le paramètre et le champ de réponse de curseur valent tous deux `0xEDFB8D08`, dont seul l'octet bas vaut le réticule 8. L'index signé et le calcul `table + index*8 + 0x88` reconstituent exactement l'adresse fautive. La pile passe par le consommateur Slate `C09350` qui lit l'entier 32 bits de réponse avant l'appel.

Le désassemblage du GetMouseCursor natif `1ACAA40` retourne `movzx eax, byte [rbx+551]` ou `xor eax,eax`. Le stockage de CurrentMouseCursor est un octet, mais le résultat est l'enum entier de 32 bits. **Erreur de V1.86 : typedef et hook unsigned char**, qui ne définissent que AL pour le contrat appelant ; EAX complet est ensuite pris pour un index. Cela peut toucher un chemin de menu ou Vanilla renvoyant le résultat natif, sans masquage actif. Le garde de thread ou de focus ne corrigeait pas cette erreur de retour.

Le hook et le trampoline utilisent maintenant `std::int32_t`, sans troncature sur aucun retour. Signature complémentaire sur le `movzx eax` natif ; aucun nouvel hook ni contournement des gardes. Log de configuration clarifié : aucun hook de curseur Windows, requête native enum32. [Contrat public résumé](evidence/V1_87_CURSOR_CRASH_CONTRACT.json).

## Contrôles et limite précédente

Les tests V1.86 déclaraient eux aussi un retour octet et appelaient directement le hook, ce qui masquait l'erreur du consommateur natif. La nouvelle déclaration du consommateur et l'assertion de type refusent ce contrat octet à la compilation. Fichier séparé NativeCursorConsumer.cpp, non inclus dans l'ASI : appel indirect et stockage complet de l'entier EAX, utilisés par tous les cas existants (modes, menus, transitions, pause, roues, focus, P2, thread, état TPS inconnu et arrêt). Ajout du passage inchangé des enums natifs 0–14 et d'une valeur sentinelle à bits hauts non nuls, qui ne doit pas devenir réticule 8 par troncature. Les tests de configuration et de bras War restent inclus.

La preuve du défaut ABI et la compilation ne constituent pas un nouvel essai en jeu. Aucun processus lancé, aucun fichier du jeu/INI personnel changé ; les rapports ont seulement été lus et copiés dans l'analyse privée.

## Essai groupé et préservation

Installer manuellement en gardant son INI. Tester Vanilla, Hide in TPS, Hide in normal view et Hide in both views ; alterner vue normale/TPS ; ouvrir/fermer pause, inventaire/carte et les trois roues, notamment la transition de roue présente juste avant le crash. Tester menu mod et Alt-Tab. Le réticule doit disparaître uniquement selon le choix et rester disponible dans les menus ; la visée et les sticks doivent garder leur comportement.

168 clés INI et leurs valeurs par défaut identiques à V1.86, 55 ID Hotkeys et 151 anciennes archives immuables. Bras de War, correction Léviathan, effets des lames, bras OFF, exécutions TPS, caméras, tirs, manettes, roues, RawInput, Alt-Tab, cheval, ressources et pouvoirs inchangés. main/archive/v1.43 intacts. PAK et sauvegardes personnels préservés.

Rollback de sécurité du nouveau module : ASI V1.85 conservé, même INI et raccourcis, avec sa régression Léviathan connue. **Vanilla dans V1.86 ne garantit pas d'éviter le défaut**, son interception restant installée. V1.86 reste archivée pour l'historique, pas recommandée comme repli pour ce crash. Suite : retour utilisateur du correctif, puis cristal Stoneskin et transferts de capacités toujours ouverts ; autres FX P2 reportés et réception lourde incertaine.
