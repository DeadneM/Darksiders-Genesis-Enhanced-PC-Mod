# V1.168 — livraison et installation debug

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ ; TAA EXPÉRIMENTAL, non validé en jeu.** Base main V1.167 intégralement validée. Source `8db34a0d8cddd5cc4970a97fb3400e4605de694c`, archive `d54aadb682df5a2395338dc5e3bd17f55248aea2`. Main reste `6795f47` ; aucune promotion stable de ce candidat.

## Changements et conservation

Camera → Temporal anti-aliasing and sharpness : personnalisation TAA/netteté OFF par défaut, poids 0,08 et netteté 1 comme valeurs d'essai. OFF restaure les valeurs capturées ; aucun changement de méthode AA ou de profondeur de champ. Journal de démarrage neutralisé pour ne plus supposer notre proxy. Aucun hook ou comportement de chargeur ajouté. Source et section machine .text du proxy identiques à V1.167 ; les empreintes du PE recompilé peuvent différer. [Contrats et limites](V1_168_TAA_LOADER_REVIEW.md).

Les 252 anciennes valeurs et les 55 actions Hotkeys sont conservées. Trois nouvelles clés facultatives, 255 valeurs dans le profil de référence. Installation des deux binaires uniquement, avec sauvegarde vérifiée et rollback prévu ; INI personnel et raccourcis, Engine.ini, EXE et inventaire PAK préservés. Aucun lancement du jeu par l'agent. La nouvelle option est générée par l'ASI ; aucun INI fourni/remplacé dans le paquet.

## Contrôles et téléchargement

[GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/43456874935) : **85/85 suites réussies**, aucun avertissement ni erreur C++ détecté dans le journal. Contrats CVar, propriété/restauration des copies jeu/rendu et persistance/bornes INI contrôlés ; cela ne prouve pas le rendu ni un gain FPS. Defender : scan du candidat terminé sans détection, protections conservées.

[ZIP cumulatif x64](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/d54aadb682df5a2395338dc5e3bd17f55248aea2/downloads/DarksidersGenesis_V1.168_OPTIONAL_TEMPORAL_AA_TEST.zip) : **2,485,486 octets**, exactement `DarksidersGenesisMod.asi` et `dxgi.dll` à la racine, sans INI/README. CRC/AMD64/empreintes contrôlés. SHA256 `7572BB6F2ADF9C7D9200578834CA3BDD9C3A3D244A8BA889C6AEC269653DFEB8`. Les 232 anciennes archives sont conservées. ZIP nouveau récupéré une seule fois via Git ; aucun ancien paquet retéléchargé. [Preuves](evidence/V1_168_DELIVERY_CHECK.json).

## Test demandé et suite

Activer Customize temporal AA, comparer OFF/ON dans la même scène : mouvement puis immobilité, vue native puis TPS, écharpe/flammes/contours/HUD. Tester les deux valeurs, puis OFF pour contrôler la restauration. Vérifier un chargement et une cinématique. La personnalisation est globale, cinématiques incluses. Si gênant, laisser OFF ; retour binaire V1.167 possible via la sauvegarde.

L'échec historique Ultimate Loader n'a toujours pas de cause établie. Tout essai d'un chargeur alternatif reste séparé du TAA ; ne pas empiler une correction spéculative ni changer la version main validée.
