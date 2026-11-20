# V1.174 — réglages d'image livrés sur debug

Base V1.173 confirmée par l'utilisateur (réactivité, coop et appel aérien). Main/release V1.173 protégées. **CONSTRUIT / CONTRÔLÉ, rendu EXPÉRIMENTAL**, à valider en jeu. Aucune installation automatique.

## Utilisation

Camera → Image effects :
- Customize motion blur décoché : **Native, défaut**. Coché : Amount 0–1, curseur et saisie manuelle ; 0 coupe l'effet.
- Customize chromatic aberration décoché : **Native, défaut**. Coché : Maximum amount 0–10, curseur et saisie ; 0 coupe l'effet. Il s'agit du plafond natif : il n'ajoute pas des franges absentes de la scène.

Native rend exactement la main aux réglages du jeu/de la scène. Les valeurs d'essai lors du passage en personnalisé sont 0.5 et 5 ; elles ne sont pas une prétendue valeur vanilla universelle. Les anciennes cases activées deviennent 0, les autres Native, sauf si une valeur explicite nouvelle existe. Pas de nouveau hook, délai, scan, écriture Engine.ini ou changement DOF/caméra/skins. Les contrôles restent indépendants de TAA, globaux aux deux joueurs et cinématiques. [Périmètre complet](V1_174_REVIEW.md).

## Vérification

- [GitHub Actions Windows x64 Release](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/44777704991) : **86/86 tests, zéro avertissement et zéro erreur C++**. Cas de valeurs intermédiaires, bornes/invalides, restauration game/render, écritures inchangées évitées, sauvegarde, migration et priorité des clés inclus.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/69fe6bef946cfa5306bcfbace9fee6966fa1836c/downloads/DarksidersGenesis_V1.174_NATIVE_IMAGE_CONTROLS_TEST.zip) : 2493678 octets ; SHA256 `9F8D9C92C362C31E3ABFFEE6BD9A5675388F490A616CB0D475F5FF267B4B88AC`.
- ASI + dxgi.dll AMD64 seuls à la racine ; CRC, empreintes, blob GitHub et HTTP200 contrôlés ; aucun INI/README.
- Defender : analyse des deux binaires terminée, aucune détection correspondante. Aucun lancement ou remplacement des fichiers du jeu.
- Les 259 anciennes clés/valeurs de référence sont inchangées ; deux nouvelles valeurs Native, soit261 clés. Les anciens flags restent compatibles pour rollback. 238 anciennes archives préservées,239 cumulées ; main/archive préservées. Un seul nouveau ZIP récupéré via Git.
- La boucle de génération constexpr du CRC est reformulée sans index : tests CRC inchangés, ancien avertissement C28020 absent de ce build. Aucun avertissement masqué.

## Test conseillé

Sur une scène montrant les effets, comparer Native / 0 / intensité intermédiaire puis retour Native pour chaque contrôle séparément. Vérifier la mémorisation après redémarrage et un chargement ; ne pas déduire un échec d'une scène ne contenant aucun effet. Conserver les autres réglages.

Pour le TPS gourmand : prochaine étape ciblée = identifier la passe GPU coûteuse avant un autre changement. Aucun gain FPS/GPU annoncé ici, aucun réglage diminué automatiquement, crash HUNG toujours non attribué. Rollback : les deux binaires V1.173 et l'INI personnel conservé. [Preuves](evidence/V1_174_DELIVERY_CHECK.json).
