# V1.151 — livraison cumulative debug

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu.** Base livrée V1.150 ; protection Moloch précédemment VALIDÉE conservée. [ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/375dd3d029798570413320954f33c038f3b16487/downloads/DarksidersGenesis_V1.151_GPU_EXIT_AIR_LANDING_MOLOCH_TEST.zip) — [liste des essais](V1_151_TEST_PLAN.md).

## Modifications

- Trait/arc : les deux CRC historiques appartiennent aux matériaux natifs VFX_beam_aimArc et VFX_beam_aimBeam. Leurs 168 shaders pixel sont maintenant reconnus par CRC, taille et empreinte complète ; aucun CRC cible partagé dans les 40163 entrées natives contrôlées. Aucun nouveau hook, calcul par dessin ou changement de visée. Les modes existants sont conservés. [Provenance et limites](V1_151_AIM_MATERIAL_SHADERS.md).
- Dash/stomp Strife : filtrage du seul enchaînement après atterrissage lorsque MOVE natif n'est pas maintenu, scripts complets et propriétaire contrôlés. Nettoyage natif conservé, aucun mode de mouvement, bouton ou curseur de script forcé. War conserve sa réception native. Impact/dégâts et comportement final à valider.
- Pose Strife après dash : une fois le dash fini, sans capacité active, AIM/FIRE ou verrouillage, essayer le setter natif SetAimMode(0) si le mode est encore 1. Contrat complet, classe/propriétés et paramètre contrôlés ; appel unique protégé contre réentrance. La valeur avant tir manquait à la capture : correction expérimentale, pas une cause définitivement prouvée. Changement de munition sans dash encore ouvert.
- GPU : ancien writer du proxy et écritures Engine.ini en session retirés ; helper intégré attend la fermeture, relit le dernier choix, sauvegarde et écrit r.GraphicsAdapter. Refus si jeu relancé avant écriture, résultat gpu-result.txt. Aucun rechargement de configuration UE forcé. Redémarrage simultané indépendant non atomique ; comportement réel à tester.
- Moloch Boss Crash Fix dans Gameplay → System and startup ; défaut ON, Features.MolochBossCrashFix. Migration de l'ancien choix explicite préservée. Condition et garde validés inchangés.

Stoneskin/ressources V1.150 conservés, toujours à confirmer après chargement/changement d'armure. Visée TPS, projectiles, XInput, bras OFF, holsters, montures/ultimes, sauts, capacités/effets validés et raccourcis préservés. Les deux expériences V1.149 abandonnées et leurs tests/code sans usage ont été retirés. [Revue du delta](V1_151_DELTA_REVIEW.md).

## Contrôles réels

[GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41869863644) : **78/78 suites réussies, aucun avertissement C/C++/C#/linker détecté sur le log final**. La première tentative a été arrêtée avant tout package : symbole RadialActive manquant dans le support de test et avertissement d'index joueur ; fixture complétée et bornes explicites ajoutées, puis recompilation complète réussie. Aucun build local.

ZIP **2399470 octets**, deux modules AMD64 à la racine, CRC et absence de compression contrôlés. Sans INI/README ; le profil personnel reste conservé. SHA256 `E0CED166F21022FC9DD56FEE9E7F9D5B85B5A641179E5EC4FB8E83B536D79C45`. Téléchargement immuable HTTP 200 et règles de sélection de l'updater debug vérifiés. Defender sans détection, protections inchangées. Installation/interface du jeu non observées.

**215 anciennes archives identiques, 216 au total.** 246 valeurs INI conservées hormis la seule clé renommée demandée ; 55 ID Hotkeys identiques. Branches main/archive et tag v1.43 vérifiés inchangés. Aucun INI personnel, PAK, sauvegarde ou binaire installé modifié ; les deux changements locaux préexistants hors tâche sont laissés de côté.

Source finale `02f9eac97a90e09106604d582311c9b52ff6b020`, archive immuable `375dd3d029798570413320954f33c038f3b16487`. [CI](evidence/V1_151_CI_CHECK.json), [package](evidence/V1_151_PACKAGE_CHECK.json), [updater](evidence/V1_151_UPDATER_CHECK.json), [catalogue natif](evidence/V1_151_AIM_SHADER_CATALOG.json).

## Test et suite

Priorité au trait ET à l'arc dans Moloch, puis dash/stomp sans/avec direction, pose après relâchement, GPU après fermeture et ressources après rechargement. [Procédure courte](V1_151_TEST_PLAN.md). Le catalogue ne prouve pas le rendu du niveau ; aucun succès graphique, dégâts, FPS ou gameplay nouveau annoncé sans retour utilisateur. Coop en dernier. Repli V1.150 conservé, aucune promotion main ni publication Nexus.
