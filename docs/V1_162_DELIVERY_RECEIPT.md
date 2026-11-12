# V1.162 — marche aérienne des chevaux, reçu de livraison

**CONSTRUIT / CONTRÔLÉ / PUBLIÉ sur debug ; EXPÉRIMENTAL en jeu.** Base V1.161 `96388c7f9265fb13d4c16cdf513d5fa7cfb4287e` ; source `0539b6f5f5eacdf48d94cd7b4a6178dd3ed50e60` ; archive `772b831f6e67750e2e951fa299dfbe67c7211051`. Saut des quatre chevaux, raccourcis, formes et coop acceptés conservés ; main V1.159 inchangée.

Gameplay → Horse : **Walk in the air (experimental)**, OFF par défaut. Le réglage associé est le délai de chute après relâchement : 0–1 s, 0,2 s par défaut. Même action Saut réaffectable que le jeu, sans nouvelle touche imposée. Maintenir après un saut engage le mode au sommet ; réappuyer pendant une chute l'engage immédiatement. En mode acquis, relâcher donne un court délai avant chute ; réappuyer pendant ce délai redemande le saut natif. Relâcher avant acquisition laisse la chute normale. Collisions natives conservées, aucun noclip. La vitesse/pose de ce mode natif est à vérifier en jeu.

Lecture préalable : héritage et signatures natives vérifiés sur Despair sans appel moteur ni écriture par le lecteur externe. Dans l'ASI, le mode de mouvement natif Flying et l'arrêt de vitesse sont demandés sur le thread du jeu, uniquement sur monture locale réciproquement possédée. Retour Falling au relâchement/annulation, gardé par monde et identités faibles ; aucun appel tardif sur objet recyclé ou cheval transféré. Pas de nouveau hook ni chargement/recherche globale d'assets. Les sauts supplémentaires passent par DoJump déjà employé pour le saut validé.

GitHub Windows x64 : **83/83 suites réussies**, aucun avertissement C++ détecté. [Exécution](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43109770701). Contrats et transitions simulés : mauvaise méthode/paramètres, changements de possession pendant rappel, recyclage, OFF/focus/pause/menu, saut → sommet → maintien → nouveau saut, délai et configuration. Les avertissements externes CMake/Node sont consignés. Ces contrôles ne prouvent pas le fonctionnement physique en jeu ; aucun build local.

[ZIP V1.162](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/772b831f6e67750e2e951fa299dfbe67c7211051/downloads/DarksidersGenesis_V1.162_MOUNTED_AIR_WALK_TEST.zip) — 2445038 octets, SHA256 `79504228AF148D6B42DD8AF0FDE9258AD9EA38C6F4C4425C6EA9AA857D518F68`. Deux binaires AMD64 seuls à la racine, CRC vérifiés, aucun INI/README. 226 anciennes archives conservées. 248 anciennes valeurs identiques, deux nouvelles clés (OFF / 0.200), 55 actions Hotkeys inchangées. Lien HEAD 200 et sélection numérique de l'updater debug vérifiés ; ZIP récupéré une fois par Git, aucun paquet antérieur retéléchargé. Defender : aucune menace détectée, protections inchangées.

## Test groupé à faire

1. Activer la case dans Gameplay → Horse. Sauter en maintenant Saut, puis avancer dans le vide ; relâcher pour vérifier la chute après le court délai.
2. Refaire en relâchant Saut avant le sommet : chute normale ; réappuyer pendant la chute pour reprendre la marche aérienne.
3. Pendant le délai de relâchement d'une marche aérienne, réappuyer pour un autre saut. Vérifier une seconde alternance et une collision avec le décor.
4. Décocher en l'air : retour à la chute. Vérifier aussi pause/Alt-Tab et descente/remplacement du cheval, puis le saut normal avec la nouvelle option OFF. Tester les quatre apparences lorsque possible ; P2 seulement pour cette nouvelle fonctionnalité si disponible, sans remettre en attente la coop déjà acceptée.

**Non inclus** : envers d'écharpe Stoneskin toujours ouvert, pas de modification spéculative de matériau ou d'optimisation moteur. Les cinq candidats locaux supplémentaires ont été examinés : War_Cloth_Gold_Inst_A combine double-face et Emissive_Multi_Color_Scroll mais sa compatibilité avec les textures/UV et le feu de Strife n'est pas établie. Les variantes Abyssales Stoneskin ne fournissent pas d'override double face. Étude à poursuivre séparément.

Aucune installation ni relance du jeu effectuée ; INI personnel intact. Installation via l'updater debug ou le ZIP en gardant l'INI. Rollback : décocher l'option, ou V1.161 déjà disponible. [Périmètre](V1_162_REVIEW.md) · [Contrat lu](V1_162_NATIVE_MOUNT_CONTRACT.json) · [CI](evidence/V1_162_CI_CHECK.json) · [Paquet](evidence/V1_162_PACKAGE_CHECK.json) · [Defender](evidence/V1_162_DEFENDER_CHECK.json).
