# V1.166 — reçu de livraison debug

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ ; EXPÉRIMENTAL en jeu.** Base main V1.164 validée, inchangée ; ajouts V1.165 de découverte conservés. Source `df2dedb2f84abc34dd9ff2577dd5b9c6e0c4c0a5`, archive `c8834113d659aa0e7daf4762cf17f0af4565ac55`. Aucune compilation locale, installation ou relance du jeu par l'agent.

## Lot

- Correction démontrée du refus du matériau double face : association de paramètre lue sur un octet, sans interpréter ses trois octets de padding. Pas de nouveau shader ou de copie par frame ; rendu encore à valider.
- Sabots natifs synchronisés aux foulées pendant la marche/sprint des quatre chevaux sur leur sol privé. Aucun son forcé sur le vrai terrain, à l'arrêt ou en saut ; pas de boucle artificielle.
- Gameplay → Passives and synergy : Contre-esquive I/II de Strife **Vanilla / OFF**, sans toucher aux achats ; **Vanilla par défaut**.
- Même groupe : **Synergy swap — Vanilla / Inverted / No Swap**. Inverted lance la synergie du héros actuel puis échange à sa fin ; No Swap garde ce héros. Appui court, action réaffectable, coûts et coop natifs préservés ; Vanilla par défaut.

[Périmètre, deux revues, limites et tests](V1_166_REVIEW.md). Les lectures de contrôle étaient exclusivement VM_READ : aucun appel moteur ou changement de mémoire par les lecteurs. Les appels de jeu décrits dans le code appartiennent à la nouvelle ASI, à tester après installation.

## Contrôles

GitHub Windows x64 : **84/84 suites réussies**, zéro erreur et aucun avertissement C++ détecté dans le journal final. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43316459389). Régression du padding capturé, sons/terrain réel/VFX/4 chevaux/P2, passifs ciblés, cycle de synergie et annulations, configuration complète exécutés. Ces tests isolés ne prouvent pas le son, le rendu ou le comportement réel du jeu. Les dépréciations externes CMake/Actions sont conservées dans la preuve.

Une tentative précédente a échoué en compilation sur un appel Read ambigu du son aérien ; appel explicitement qualifié, puis compilation et suite complètes relancées. Aucun ZIP issu de cette tentative échouée.

ZIP : [DarksidersGenesis_V1.166_NATIVE_ABILITY_OPTIONS_TEST.zip](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c8834113d659aa0e7daf4762cf17f0af4565ac55/downloads/DarksidersGenesis_V1.166_NATIVE_ABILITY_OPTIONS_TEST.zip), 2,488,558 octets, SHA256 `179B6E1BE4932B905630C9D967389A05EB815CD11FAA850D5F48201F51C8259A`. CRC/PE AMD64 contrôlés, ASI et dxgi.dll seuls à la racine ; aucun INI/README embarqué. 230 anciennes archives inchangées, 231 au total. Les 250 anciennes valeurs restent identiques ; deux clés ajoutées, **252** au total : Features.StrifeCounterEvade=1 et Values.SynergySwapMode=0. Les 55 actions Hotkeys et les raccourcis sont conservés. L'INI personnel n'est pas touché.

Lien public et sélection de V1.166 par le classement updater vérifiés. Nouveau ZIP récupéré une seule fois par Git ; pas de téléchargement HTTP supplémentaire ni d'ancienne archive. Defender : aucune menace détectée, protection inchangée. Main/archive/v1.43 intacts. [CI](evidence/V1_166_CI_CHECK.json) · [ZIP](evidence/V1_166_PACKAGE_CHECK.json) · [Defender](evidence/V1_166_DEFENDER_CHECK.json).

## Essai regroupé

1. Stoneskin sur Strife, Native/Abyssal : envers et bouts de l'écharpe, feu et restauration après expiration.
2. Quatre chevaux : marcher/sprinter dans le vide, s'arrêter, sauter, revenir au sol ; sabots rythmés et sans doublon au sol.
3. Contre-esquive achetée : Vanilla puis OFF, esquive au bon moment ; clone absent sur OFF, achats et dash inchangés.
4. Synergie pleine, Strife puis War : appui court normal ; appui long dans les trois modes. Vérifier qui lance, fin complète, jauge consommée une fois, échange seulement dans Inverted puis retour Vanilla.

La coop complète reste à vérifier en dernier comme convenu. Aucune nouvelle mesure FPS annoncée. Rollback : remettre les deux fichiers du ZIP V1.164 validé de main, en conservant l'INI. Suite : retours du lot, puis apparences utiles et nettoyage démontré des ajouts récents.

« Silhouette d'ombre » est la ruée vers le sol depuis les airs ; son niveau II ajoute une ruée à la réception. Contre-esquive est le clone créé par une esquive au bon moment ; niveau II : +20 % de dégâts pour ce clone. Ce sont les descriptions françaises du jeu, distinctes de Shadow Clone, le pouvoir de Courroux.
