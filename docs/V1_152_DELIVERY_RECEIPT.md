# V1.152 sur debug

Le lot reprend V1.151 et son filtre de visée Moloch validé. Avec Strife à pied, relâcher Dash avant 180 ms lance le stomp. Maintenir la commande lance un dash vers l'avant, limité par le moteur à environ 0,24 s. Le geste conserve l'action Dash réaffectable. Sa fin évite le deuxième dash automatique au sol.

Le correctif de visée couvre les interruptions qui laissent Strife en pose AIM après la fin d'une action. Stoneskin accepte les matériaux dynamiques natifs de l'armure et restaure leurs paramètres à la fin. [Périmètre et preuves](V1_152_LIFECYCLE_PREPARATION.md).

**CONSTRUIT et CONTRÔLÉ.** GitHub Windows x64 a passé les 81 tests et l'analyse du code. Le ZIP contient DarksidersGenesisMod.asi et dxgi.dll à la racine. Les empreintes, les CRC, le téléchargement public et la sélection du ZIP par les règles du canal debug ont été vérifiés. Les 216 ZIP précédents sont conservés. Les références main, archive et v1.43 ont été vérifiées. [Contrôle GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41966356644), [preuve du ZIP](evidence/V1_152_PACKAGE_CHECK.json).

Source `2dd6ef891dd48dd221d66d21dbfeb2ca80452ec7`. Archive `635e422dacff9a04381b13e6a9e909cb9639d14c`. SHA256 `4F587ABE1CCD624487636C0804E20D12E5A4A78A354DC990B1D12E4CD57B1510`.

Defender n'a rien détecté dans le ZIP ni dans les deux modules extraits, avec les signatures 1.463.119.0 et les protections actives.

**EXPÉRIMENTAL en jeu.** Avec Strife en mission, comparer appui court et maintenu en l'air dans les deux vues, avec et sans déplacement. Vérifier la pose, le saut et la reprise du déplacement après le geste, puis essayer près d'une paroi. Ensuite, vérifier une interruption de visée par un changement de munition ou un stomp, et Stoneskin après changement d'armure et plus tard dans la mission. La persistance GPU après fermeture reste à confirmer. V1.151 fournit le repli.

[Télécharger le ZIP V1.152](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/635e422dacff9a04381b13e6a9e909cb9639d14c/downloads/DarksidersGenesis_V1.152_AIR_GESTURE_RANGED_MATERIALS_TEST.zip).
