# V1.149 — livraison debug, 3 novembre 2026

**CONSTRUIT / CONTRÔLÉ.** Base V1.148 dont le correctif de transition Moloch avec sous-titres est VALIDÉ par l'utilisateur. Menu déplacé, application GPU précoce et état du filtre de visée contrôlés en CI ; les deux derniers changements restent EXPÉRIMENTAUX en jeu. Aucune installation par l'agent.

[ZIP cumulatif](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/c4867cdbe5be3395ae443cc26154f85484c06e0b/downloads/DarksidersGenesis_V1.149_SUBTITLE_SYSTEM_MENU_TEST.zip) — [périmètre et essais](V1_149_SYSTEM_GPU_AIM_STATE_TEST.md).

## Inclus

- **Gameplay → System and startup → Subtitle crash fix**. Même clé, même défaut, même garde RHI validé ; seule sa présentation/log change. [Explication préparée pour Nexus](NEXUS_SUBTITLE_CRASH_FIX.md), sans publication sur Nexus.
- GPU : sauvegarde immédiate du choix dans l'INI du mod, application de r.GraphicsAdapter par le proxy importé avant la configuration UE, suppression de l'écriture ASI tardive. Deux binaires actualisés nécessaires, l'updater les remplace ensemble. Valeur0..4 et défaut0 inchangés. Hypothèse du cache qui écrase l'écriture tardive à confirmer par le redémarrage réel.
- Trait/arc : un cache Other périmé pouvait empêcher une vérification après replay de commandes. Invalidation avant/après ExecuteCommandList et après ClearState, premier dessin inconnu vérifié, autres dessins ordinaires toujours rapides. Les deux CRC restent identiques, aucun ciblage/munition/contrôle modifié. Le défaut Moloch lui-même n'est pas annoncé validé réparé avant essai.

## Contrôles réels

[GitHub Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/41683169464) : **77/77 suites, aucun avertissement de compilation C/CS/LNK/D détecté**. Deux tests ajoutés : fichiers temporaires GPU et invalidation du cache de shader. Ils ne remplacent pas un démarrage ou un contrôle du rendu en jeu. Compilation uniquement GitHub.

ZIP 2356974 octets, ASI/dxgi.dll uniquement à la racine, AMD64/CRC vérifiés, sans compression et sans INI/README. SHA256 `736CCAC7125AAAC9C380C96D9808A415CEC64EEDE293F06BD7DDACEEA224BC10`. Téléchargement public HTTP200 et sélection de l'updater debug vérifiés ; interface et installation en jeu non observées. Defender : aucune détection et protections inchangées.

**213 anciennes archives identiques**,214 au total. Main/archive/tag v1.43 intacts. 246 clés et 55 identifiants Hotkeys conservés, aucun défaut ni paramètre ajouté. INI personnel, raccourcis, sauvegardes et PAK non modifiés par l'agent ; capture externe VM_READ uniquement, handles fermés. [CI](evidence/V1_149_CI_CHECK.json), [package](evidence/V1_149_PACKAGE_CHECK.json), [updater](evidence/V1_149_UPDATER_CHECK.json).

Source compilée `6f333ed36ab9b9a7584eb85a140ef59c72d85c9c`, archive immuable `c4867cdbe5be3395ae443cc26154f85484c06e0b`.

## À tester

1. Emplacement et état conservé de Subtitle crash fix ; passage Moloch avec les sous-titres activés pour non-régression.
2. Choisir le GPU voulu dans le menu, fermer complètement puis relancer soi-même. Vérifier le choix conservé, r.GraphicsAdapter dans Engine.ini et le véritable adaptateur dans ProjectMayhem.log. Le journal du mod doit indiquer prepared=N puis earlyPrepared=N ; une session déjà ouverte ne peut pas changer de GPU.
3. Trait/arc à Moloch en Hide Everywhere puis TPS Only ; conserver ensuite son réglage personnel. Contrôler aussi les effets de tirs.

Pose après changement de munition, Stoneskin intermittent et dash aérien restent ouverts. Pour ce dernier, le lien natif automatique d'atterrissage a été identifié et la règle stomp/direction est consignée ; aucun correctif de mouvement n'est livré ici. [Audit des sujets restants](V1_149_OPEN_REPAIRS_AUDIT.md).

Repli V1.148 disponible, archive conservée. Aucun passage main ni publication Nexus.
