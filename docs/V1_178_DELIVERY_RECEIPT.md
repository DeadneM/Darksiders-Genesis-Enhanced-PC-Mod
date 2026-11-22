# V1.178 — mêlée montée livrée sur debug

**CONSTRUIT / CONTRÔLÉ ; rendu EXPÉRIMENTAL.** Base V1.177 validée, main V1.173 conservée. Aucune installation automatique.

Arcs de Strife étendus aux attaques à cheval gauche/droite : mêmes 40 %, origine sur la main masquée vérifiée dans le squelette monté. Chemins attaché/détaché, quaternion, rotation, durée, sons, dégâts et callbacks natifs conservés. Aucun nouveau hook/scan par image. Classificateur à pied et fonction Lava conservés. [Périmètre et preuves](V1_178_REVIEW.md).

- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45035011517) : **86/86 tests, zéro avertissement/erreur C++**, Windows x64 Release. Test mêlée ciblé réussi avant build complet, puis dans la suite entière. Fixtures : modes de lames, deux attaques, P1/P2 simulés, indices montés différents, frame attachée et origine décalée, mauvais propriétaires/assets/os. Rendu/coop réels encore à tester.
- Run45034346720 rejeté : fixture de socket à pied involontairement modifiée, 85/86 tests ; correction du test sans modification de production entre les deux runs. Aucun ZIP du run échoué. Contrôle ciblé placé avant le build complet pour économiser les relances.
- Source `0a468033fd189747a7093679acdec7c170877db2` ; archive `9debf5b95150dea1e2b6db04ff3e3c09632e761c`.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/9debf5b95150dea1e2b6db04ff3e3c09632e761c/downloads/DarksidersGenesis_V1.178_MOUNTED_MELEE_SCOPE_TEST.zip) : 2514158 octets, SHA256 `685097DF39FAAE9C39E9FA89F381C62FD5713B37C2C979E3BBA75419A98C6C80`.
- Deux binaires AMD64 seuls à la racine, sans INI/README ; CRC/empreintes/blob distant/HTTP200 contrôlés. Defender ZIP et binaires : aucune menace détectée, protections intactes.
- 242 anciennes archives préservées, 243 cumulées. Les 286 valeurs de référence sont identiques. INI personnel, raccourcis, Engine.ini, main/archive et tag stable intacts. Un seul nouveau ZIP récupéré par Git, aucun ancien fichier téléchargé ; seulement huit exports manquants lus depuis le PAK local. Aucun build local ni capture mémoire.

## Essai court
Strife à cheval : attaques des deux côtés avec Native, gauche OFF, droite OFF puis les deux OFF. Vérifier arc réduit depuis la main masquée, côté armé inchangé, sons/dégâts/animation normaux. Descendre puis comparer la mêlée à pied. Deux skins de monture si possible ; P2 à confirmer séparément lorsque l'utilisateur sera prêt.

## HDR
Variables présentes, mais branche Intel D3D11 sans implémentation native HDR dans le chemin étudié. Aucun forçage de sortie/tonemapper ajouté. Cela ne décrit pas les capacités physiques de la carte ou de l'écran ; D3D12 et solutions externes restent une étude distincte. [Analyse](HDR_NATIVE_BACKEND_REVIEW.md) · [Instructions recoupées](evidence/HDR_NATIVE_VENDOR_CHECK.json).

Rollback : remettre les deux binaires V1.177, INI conservé. Aucun nouveau réglage persistant créé par ce lot. [Contrôles](evidence/V1_178_DELIVERY_CHECK.json).
