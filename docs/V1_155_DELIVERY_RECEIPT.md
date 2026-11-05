# V1.155 — continuité du gameplay sous le menu du mod

Base : V1.154 sur debug, poussières de dash validées par l'utilisateur sur Strife à pied après clarification dash/stomp. Main reste sur V1.153 acceptée. **CONSTRUIT et CONTRÔLÉ** sur GitHub Windows x64 : 81/81 tests réussis, aucun avertissement de compilation détecté. Le comportement du nouveau correctif reste **EXPÉRIMENTAL en jeu**. [GitHub](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/42068449279).

## Correction

L'ancien test menu ouvert servait à la fois à bloquer la navigation de jeu pendant l'édition et à suspendre des règles de gameplay. GameplayMenuPolicy sépare maintenant le maintien des règles (fenêtre active, non réduite, hors fermeture) et les nouveaux appuis/commandes réservés au jeu. Les chemins de visée TPS et de manette restent actifs sous l'éditeur. Les reprises de pose et de mouvement ne dépendent plus de l'autorisation de chargement d'assets. L'éditeur lit les manettes avec PhysicalRead, pour conserver ses axes physiques ; P2 conserve ses états de gameplay avec capture distincte des commandes de navigation.

La rotation de caméra par l'éditeur, les raccourcis/rebindings et les nouveaux gestes différés restent isolés. Les deltas souris de navigation ne sont pas rejoués à la fermeture. Alt-Tab, réduction, pause native, roues et cinématiques conservent leurs conditions propres. Aucun nouveau hook ni nouvelle touche. [Périmètre et contrôles](V1_155_MENU_RUNTIME_REVIEW.md).

L'échec du premier contrôle 80/81 était celui du scénario ajouté : il avait relâché le tir avant l'ancienne assertion de tir P2. Le scénario est rétabli avec un delta nul, puis les 81 tests complets ont réussi. Pas de ZIP produit par ce premier passage.

## Livraison

[Télécharger V1.155](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/1771ab1aeeceaf0321d66ed0e4ae0eb685baa087/downloads/DarksidersGenesis_V1.155_MENU_RUNTIME_CONTINUITY_TEST.zip). Source `041e5a7003b00901504268b1770b174bba04a7b7`, archive `1771ab1aeeceaf0321d66ed0e4ae0eb685baa087`. ZIP de 2420462 octets, SHA256 `2DB2814B4AC501F22A5FCA448B407E3A2C29875309113A72B8276D1D116C9EA2`. Deux modules AMD64 à la racine : DarksidersGenesisMod.asi et dxgi.dll ; aucun INI ou README.

Les 219 anciennes archives sont conservées ; les 248 valeurs et les 55 actions sont inchangées. Les sources des apparences, shaders et matériaux Stoneskin sont identiques à V1.154. Le téléchargement public, les CRC, les règles de sélection de l'updater debug et les références main/archive/v1.43 ont été vérifiés. Defender ne trouve aucune menace dans le ZIP et les modules, sans désactivation de protection. [Preuve du ZIP](evidence/V1_155_PACKAGE_CHECK.json) · [CI](evidence/V1_155_CI_CHECK.json) · [Defender](evidence/V1_155_DEFENDER_CHECK.json).

Pas de compilation locale, pas d'installation par l'agent. La seule observation en jeu dans ce lot concerne V1.154 : 1 124 échantillons en lecture seule, aucune erreur enregistrée, huit templates de poussière actifs/visibles, Hidden masqué et Native restitué, confirmés par l'utilisateur. Elle ne valide pas V1.155 ou la coop.

## Essai et suite

Comparer les comportements de gameplay en ouvrant puis refermant le menu du mod. Vérifier la visée TPS et l'échange des sticks, le maintien des états et reprises de mouvement, puis la navigation de l'éditeur sans rotation ou recentrage déclenché par ses commandes. Vérifier la fermeture et Alt-Tab. Coop/P2 reste différé comme demandé ; ses contrôles automatisés ne remplacent pas cet essai.

Les flammes/particules natives du cheval à sa première apparition et la transparence de l'écharpe sous Stoneskin restent ouverts. Ils ne sont pas annoncés corrigés. [Constats et prochaine observation](V1_155_OPEN_NATIVE_VISUALS.md). Rollback : ZIP V1.154 ou V1.153, en gardant son INI et ses raccourcis.
