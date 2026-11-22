# V1.178 — effets de mêlée Strife à cheval

Base : debug V1.177 aacec1b, validée par l'utilisateur. Défaut exclusivement à cheval ; chemin à pied, y compris deux lames OFF, à conserver. Main V1.173 intacte.

## Preuve et correction
L'ancien filtre exigeait Foot + Strife_Blueprint_C et des animations à pied ; le test excluait même explicitement Horse. Le chemin monté possède Despair et son mesh combinant cheval/cavalier. Réutilisation de la chaîne locale réciproque existante, rôle local et personnage Strife normal ; aucun choix de touche.

Index complet PAK réutilisé depuis le cache. Seuls huit fichiers manquants ont été extraits localement (quatre animations et leurs exports), aucun téléchargement ou export massif. Strife_Horse_Attack_L/R portent chacun deux AnimNotify_PlayParticleEffect ; montages sans notify additionnel. Asset P_Mayhem_Atk_L_01, package /Game/VFX/Particles/Characters/Horses/Mayhem/P_Mayhem_Atk_L_01 ; LocationOffset (0,0,50), socket VFX_Master_Attack_01. Les rotations natives diffèrent selon le côté et restent inchangées.

Extension du même filtre à ce seul asset/package, ces deux animations/packages, offset et socket exacts. Mains du cavalier retrouvées par nom dans le squelette monté (limite 512), indices seuls en cache et revalidés à chaque notification ; aucune position cachée, aucun scan par image. Le mesh/asset courant est relu et les noms sont vérifiés avant l'appel natif déjà utilisé. Chemin chaud après découverte : deux positions actuelles, pas de scan supplémentaire.

Arc ramené à 40 % et déplacé vers la main réelle la plus proche de l'origine de l'effet, uniquement si sa lame est masquée. Les deux lames OFF permettent de réduire les deux arcs. Même règle sur spawn monde ou attaché : pour le mode monté, sélectionner la main après l'offset de l'émetteur, pas depuis le socket parent seul. Frame du socket, quaternion, durée, rotation et options natives conservés. Aucun changement des blasts/dégâts/sons ni nouveau hook. Compteurs de diagnostic séparés pied/cheval, bornés à douze modifications chacun.

Séquence de classification Foot et requêtes de mains conservées ; PrepareLava et initialisation des hooks antérieurs inchangés hors texte de readiness. Politique Foot garde ses anciennes branches et valeurs enum. Le nouveau mode MountedHand ne change que la sélection montée. Plan privé toujours 128 octets.

## Contrôles et essai
Tests ajoutés : monté gauche/droite, gauche OFF/droite OFF/les deux OFF/Native, P1/P2 simulés, os montés à des indices différents de Foot, frame attachée et origine d'émetteur décalée, possession périmée, War, mauvais package/socket/offset, os manquant, squelette trop grand. Tests à pied existants conservés. Fixtures et CI ne valident pas le rendu réel ni la coop en jeu.

Statut PRÉPARÉ / EXPÉRIMENTAL avant GitHub Actions. Aucun changement d'INI/défaut/raccourci/rendu V1.177, aucune installation ou capture mémoire. ZIP deux binaires racine, sans INI/README.

Essai : Strife à cheval, attaquer des deux côtés avec Native, gauche OFF, droite OFF puis les deux OFF. Vérifier que l'arc réduit part de la main masquée, que le côté encore armé garde son effet, et que dégâts/sons/anim restent normaux. Descendre et vérifier les attaques à pied inchangées. Vérifier deux skins de monture si possible ; P2 reste à confirmer séparément en jeu.

Rollback : remettre les deux binaires V1.177, INI conservé. Aucun réglage supplémentaire écrit dans Engine.ini par ce lot.
