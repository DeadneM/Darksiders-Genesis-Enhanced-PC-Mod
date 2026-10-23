# V1.89 — reçu du transfert expérimental Geyser

23 octobre 2026. Base expressément validée V1.87 `c20319f93413f849a41b10fdfd96f7842a172ac6` ; base cumulative V1.88 `4309438f8a87d95ef33ab14fe0a4bdc0f4dda245`, dont le prototype Geyser reste expérimental. Correctif réticule V1.87, bras War, Léviathan et autres acquis validés conservés. Branche debug seulement ; installation manuelle, conserver son INI, ses raccourcis, son PAK et ses sauvegardes.

- Sources compilées : `8459be89bfb2418ce8c80de37be02e7a8cb12f45` ; archive : `8507ca9ffb9fbc24a62ea6d13146245ae13c335e`.
- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40009032079) : run 40009032079, job 120604303321, success, **53/53 suites**, zéro erreur et zéro avertissement MSVC, analyse native activée. Les avertissements de dépendances/outillage sont consignés séparément, sans masquer d’avertissement de l’ASI.
- [ZIP public HTTP200 vérifié](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/8507ca9ffb9fbc24a62ea6d13146245ae13c335e/downloads/DarksidersGenesis_V1.89_STRIFE_GEYSER_CLEAN_TEST.zip) : **917067 octets**, SHA256 `EC5BDFC59EB764F7744ED43203D10B85CF99F33BA915A1484C64648795A6BB21` ; contenu distant identique au local, CRC valides et binaires AMD64.

| Fichier racine | Octets | SHA256 |
| --- | ---: | --- |
| `DarksidersGenesisMod.asi` | 1673728 | `7C27644DDE96AE3869BA7A99202AC914D426EB31B510369F3FDE8577173423B4` |
| `DarksidersGenesisMod.ini` | 4691 | `99E3EECAD438A6735DD4389D544535FF798BB7F9E9EF0E4E243BC40CC4F13524` |
| `dxgi.dll` | 155136 | `C76F5E49E7697BD8303DBFE8AA15E48C28D21676EC64218CE69626BBAA487171` |
| `README.md` | 45022 | `028E8D9AEF3576DA3D4D125BF5F813626BAF2F6AE5340B2F23DBDBD6895E485A` |

## Contrôles et limites

V1.88 intermédiaire est archivée avec son avertissement C6386 déclaré. V1.89 centralise les accès au marqueur propriétaire : seules deux adresses constantes P1/P2 sont retournées, toute autre valeur donne null. Test -1/0/1/2 ajouté. Aucun avertissement désactivé et aucun changement du calcul de pouvoir. Le contrôle final ci-dessous remplace cet état intermédiaire, sans réécrire son ZIP.

Nouvelle suite ForeignMontage : ABI constructeur neuf arguments, flags/outer/template, quatre montages reconnus, événement SpawnBlast natif à son temps exact, squelette compatible, tableaux de copie distincts, source inchangée, rejet des alias/événements doubles/branches/marqueurs/notifications inattendues, gardes de thread et arrêt avant allocation sur contrat inconnu. ForeignWrath : Stoneskin existant, coût/déblocage natifs, sélection Geyser indépendante, taux d’origine, arrêt du transfert après un échec, compagnon de mêlée non joué terminé une seule fois sur le propriétaire exact. WrathSlots : choix 4 accepté sur Strife, Native et pouvoirs propres préservés, configuration et 55 IDs Hotkeys inchangés.

Lecture seule du jeu : les quatre montages ont des tableaux de branchement/marqueurs vides ; Strife_Idle ne porte aucune notification. Signatures de construction, package transitoire et squelette comparées à l’exécutable exact. Le moteur gère l’allocation de la copie, son temps et sa fin ; aucun nouvel hook, aucune minuterie. **Ces contrôles ne prouvent pas l’apparition de Geyser ni ses dégâts dans le jeu.** [Contrat et limites](V1_89_STRIFE_GEYSER_CLEAN_TEST.md).

Geyser → Strife reste EXPÉRIMENTAL : pose de repos compatible, pas de geste retargeté ni de rising slash de War, sons/particules propres au montage non transférés. Si le contrat échoue au cast, celui-ci finit nativement et Geyser est bloqué jusqu’au redémarrage, sans affecter les autres pouvoirs. La protection Stoneskin existante est inchangée ; son aspect doit être comparé au natif, l’utilisateur n’étant plus certain qu’un cristal existait avant. Rampage → Strife et les trois pouvoirs Strife → War restent ouverts.

## Préservation, essai et repli

**168 clés INI et leurs défauts inchangés**, 55 ID Hotkeys inchangés, **153 anciennes archives intactes** (154 ZIP au total), **117 anciens fichiers source inchangés**. Aucun changement de caméra, projectile, manette/XInput/roue, bras/holster, effets de mêlée ou Léviathan ; ancien cast Stoneskin strictement conservé. README/INI du ZIP correspondent aux sources compilées ; le reçu complète le README de préparation sans réécrire l’archive.

Essayer Strife à pied avec un emplacement débloqué sur Geyser : attaque et dégâts, Courroux, fin du cast en vue native/TPS, puis les trois emplacements et P2 si disponible. Revenir à Native, vérifier les trois pouvoirs Strife et le Geyser propre de War. Courte régression réticule/menus, bras War, Léviathan et armes masquées. En cas d’échec, revenir à Native et conserver le journal ; ne pas répéter un cast en échec. Nouveau transfert non validé par l’utilisateur à la livraison.

Rollback : Native dans le choix, ou ASI V1.87 validé conservé avec son INI personnel. Aucune installation/relance automatique ni modification des fichiers du jeu, du PAK ou des sauvegardes. Fichier de reprise non suivi préservé. main `2bc3122c1ae07b7333798462f85c1d9fd3a739f7`, archive `10e0f220b85022bc9269e0cb9390a811a3b1cf89`, v1.43 `902861eb1ff34c14dc5ded092581b360767da817` vérifiés intacts. Suite : retour Geyser, puis adaptation des quatre transferts manquants ; réception lourde incertaine et autres FX P2 reportés.
