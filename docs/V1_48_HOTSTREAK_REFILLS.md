## Dernier état : acquis acceptés hors cinq réparations ouvertes

L'utilisateur accepte **le reste pour l'instant**. Les listes générales de tests et mentions de non-validation ci-dessous sont des reçus antérieurs. Les seules réparations actuellement ouvertes sont : **roue d'équipement coop P1/P2 ; Charge Shot manette surtout Hotstreak en reculant ; Lava normal en cloche manette P1 ; visée native après dash ; Action Recovery dash**, coffres différés. Les quatre groupes d'ajouts Hotstreak/ammo/Hotkeys restent explicitement validés.

[Nouvel audit complet, preuves et ordre des réparations](V1_48_OPEN_REPAIRS_AUDIT.md). Cette mise à jour est documentaire ; aucune nouvelle build ni installation, aucun nouveau correctif annoncé comme validé. Les états ci-dessous retracent l'historique.

## État courant après le retour utilisateur V1.48

Réduction Hotstreak/saisie, Unlimited Hotstreak, refill Hotstreak, refill ammo et quatre actions Hotkeys : **VALIDÉS PAR L'UTILISATEUR**. Les anciens reçus de construction ci-dessous restent historiques ; leurs mentions « validation requise » ne remplacent pas ce nouveau retour. P2/coop n'est pas précisé pour ces validations.

Lava normal en cloche, manette P1 : l'utilisateur rapporte un point d'arrivée presque aux pieds quand la caméra est horizontale, et une dépendance apparente au viseur/à l'angle caméra. **SYMPTÔME CONFIRMÉ PAR SON TEST ; CAUSE EXACTE INCONNUE.** La rotation stockée, la cible3D, les positions de départ/cible, la vitesse et la compensation de gravité doivent être observées ensemble. Le nouveau retour oriente l'enquête sur la construction du point3D et la cohérence du pitch/distance ; il ne prouve pas à lui seul un correctif ou une cause unique. Aucun nouveau hook de visée dans ce reçu.

### Vérifications encore attendues

1. **Menu** : présentation commune (dont Reduce ammo), curseur/saisie/Default, sauvegarde puis rechargement, lisibilité en redimensionnant la fenêtre.
2. **Échange solo** : choisir un angle, échanger Strife/War plusieurs fois sans déplacer la caméra ; vérifier que l'angle reste choisi. Respawn/changement de zone à comparer séparément.
3. **Caméras et réglages coop** : chaque vue suit son joueur après réapparition, cheval et formes ultimes ; recentrage dans chaque forme ; FOV coop sur les deux vues et FOV solo indépendant ; distance/offset cheval et ultime appliqués à la bonne forme.
4. **Commandes P2** : maintien/relâchement équipement, recentrage/appui long TPS, vue temporaire, combinaison cheval, interversion complète des sticks et sélection dans les trois roues ; utiliser les bindings configurés, sans imposer de touche particulière.
5. **Premiers tirs** : primaire/secondaire alignés avec la visée dès le premier projectile, sans départ latéral ; à pied, cheval et Strife ultime, souris/manette, P1/P2.
6. **Pause/roues/scènes** : TPS conservé dans pause et trois roues ; vue native dans une vraie scène puis retour TPS ; raccourci temporaire toujours fonctionnel, pas de faux déclenchement.
7. **Gameplay P2** : vitesse/dégâts/gains, ressources/refills et raccourcis agissent aussi sur P2, sans modifier la réserve de l'autre joueur à tort. Les nouvelles validations Hotstreak/ammo n'établissent pas à elles seules ce point.
8. **Santé/vol plané/sprint cheval** : refill santé et chacun des Unlimited ON puis OFF ; reprise normale des coûts/durées après désactivation. Tester les coûts en mission : le Courroux est natif gratuit dans le Néant.

### Travail restant et ordre proposé

1. Réparer la trajectoire du **Lava normal** à partir du point3D réel et du calcul balistique natif, en préservant le faisceau Hotstreak et la séparation souris/manette.
2. Confirmer ou reprendre les corrections cumulatives de caméra/commandes P2 et de premier tir selon les huit contrôles ci-dessus ; elles sont construites mais pas encore toutes validées par l'utilisateur.
3. Réparer le bug **visée native + dash** qui annule la visée et fait disparaître arc/trait.
4. Rendre **Action Recovery** réellement fonctionnel pour le dash, notamment l'intervalle entre le troisième et le quatrième dash Strife. Fonction toujours indisponible ; coffre différé.
5. Après stabilisation, reprendre le nettoyage/optimisation ciblé avec les acquis validés comme référence. Ne pas assimiler les audits ou tests isolés à une réparation en jeu.

Ce reçu modifie uniquement la documentation. Pas de changement d'ASI/INI installé, de valeurs par défaut, de Hotkeys, de release, d'archive ou de branche stable.

## V1.48 — construite, contrôlée et installée : Hotstreak / refills (16 octobre 2026)

**CONSTRUIT / CONTRÔLÉ / INSTALLÉ / EXPÉRIMENTAL — validation utilisateur en jeu requise.** Base cumulative V1.47 construite mais non installée, acquis validés V1.43 et suivants conservés. Source `b7b90d29314ce00229a0737004426cbb0375b28d`, archive debug `550b7784b4f1341521bc8f7113250e5177207232`. [CI Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/38964259848) **SUCCESS,22/22 tests** ; huit avertissements historiques, aucun nouveau. Les tests des vrais services P1/P2 couvrent les nouveaux refills, le diviseur, Unlimited ON/OFF, les resets et les générations. Cela ne vaut pas validation en jeu.

Ajouts : **Reduce Hotstreak decrease**0–10 avec saisie manuelle/Default, **Unlimited Hotstreak**, **Fill Hotstreak**, **Fill ammunition**, quatre nouvelles actions Hotkeys appendues44–47 sans remplacer les anciens bindings. Réduction et Unlimited Hotstreak OFF par défaut ;0/1=natif,10=un dixième. Le module natif conserve délais/reset/activation et le plafond du Hotstreak ; les refills font une seule demande fraîche par joueur et contrôlent la relecture. Les119anciennes clés/valeurs et44actions, le calcul validé de consommation des munitions,27fichiers protégés et13fonctions centrales de visée/RawInput/XInput ont été comparés et conservés. V1.47 cumulative : présentation commune de toutes valeurs, échange Strife/War basé sur les types natifs1/2 et commandes d'équipement/TPS P2 avec états distincts, encore à vérifier visuellement.

[ZIP cumulatif V1.48 — téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/debug/downloads/DarksidersGenesis_V1.48_HOTSTREAK_REFILLS_TEST.zip), **843312 octets**, SHA256 `6EDA1124513CC88903F8F0DFCC611BE7624024D831CD7FAB5E1043A5F6E1CE90`. Exactement ASI/dxgi.dll/INI/README à la racine, CRC et binaires AMD64, identité1.48 et122clés contrôlés ; empreintes CI/dépôt/téléchargement public identiques. **111 anciennes archives intactes**, main`2bc3122`, archive`10e0f22`, tag/release publiquev1.43`902861e`/id414771221 vérifiés inchangés. Aucune nouvelle release publique créée. Le README du ZIP immuable reste celui de la préparation.

Installation après vérification élevée des processus : le jeu était complètement fermé. Sauvegarde des quatre fichiers et du log : `C:\Users\deadn\.codex\.chatgpt-projects\g-p-6ac1496f81ec8191b55c0e4db2eefebd\audit-v131\install-backup-before-v148-20261016-224804`. ASI et dxgi installés conformes au ZIP ; **INI personnel et raccourcis préservés octet pour octet**, SHA256 avant/après `89647F8748749404EC06725B4EEC792BBC3CBA68B03C1639A9E7CBEDFF988A95`. Les nouvelles clés manquantes utilisent leurs défauts compilés OFF sans remplacer le fichier personnel. Le README installé est ensuite actualisé avec ce reçu. Aucun lancement automatique ni écriture externe de mémoire.

**Lava : l'utilisateur confirme le projectile normal en cloche, manette P1.** Son calcul natif balistique et sa rotation en cache ont été identifiés, mais **aucun correctif Lava n'est annoncé** ; la prochaine capture doit comparer cible3D, cache, entrée au relâchement et caméra. Le faisceau Hotstreak n'est pas signalé. **Visée native après dash NON RÉSOLUE ; Action Recovery INDISPONIBLE.** Test prioritaire : nouveaux refills/Hotkeys, diviseurs1/2/10 puis Unlimited ON/OFF en mission ; ensuite menu/échanges solo/équipement P2 et récapitulatif V1.46–47. [Preuves, limites, critères et rollback](V1_48_HOTSTREAK_REFILLS.md).

# V1.48 — Hotstreak et remplissage des munitions TEST

## Base, objectif et statuts

Base debug V1.47 `876dae6` / source `c23fb2e`, construite avec21/21 tests mais NON INSTALLÉE/NON VALIDÉE. La dernière session utilisateur était V1.46, dont les sauts d'angle et commandes P2 sont encore rejetés. V1.47 corrige ces causes sur preuve native, à retester. Les acquis solo V1.43 et options validées ensuite sont conservés. Aucun statut global VALIDÉ n'est ajouté aux V1.46–48.

Objectif V1.48 : trois commandes Hotstreak (diviseur, Unlimited, refill) et un refill ammo, dans la présentation commune avec quatre actions assignables. Réussite : en mission, réduction mesurable à1/2 puis1/10, Unlimited ON maintient la réserve et OFF rend la baisse normale ; remplir une fois n'active pas Unlimited ; les plafonds et coûts/reset normaux restent cohérents ; P1/P2 ne partagent ni réserve ni requête. **SOURCE PRÉPARÉE/EXPÉRIMENTAL. Compiler, contrôler le ZIP et tester en jeu restent nécessaires.**

## Preuves natives Hotstreak

EXE retail SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`. Audit hors processus :

- `AddJuice=660260` (déjà validé pour les gains), `RemoveJuice=681A40`, `TryConsumeJuice=68BA20`, Tick composant `68AF70`.
- Getter de classe `78F980`, slot `380DB38` ; constructeur joueur `658695` appelle ce getter, construit le composant et le stocke à `actor+CB8` (`6586BF`). Le composant doit réciproquement appartenir au joueur par `component+E8` et appartenir à cette classe native.
- Réserve `actor+EA0`, classe native `3810268`, rôle autorité3 ; niveau float `398`, plafond natif `6A9280 = [470]*[39C]`. Strife est le type1 prouvé par la capture V1.47 ; War2 est exclu.
- `RemoveJuice` retire une quantité float via setter `6BA400`, puis gère fin du Hotstreak et effets. Le hook appelle toujours son original, en réduisant uniquement le montant positif/finis des appels prouvés : Tick retourne à `68B007` (baisse active/inactive après délais natifs), TryConsume à `68BA89`, coût de combat à `67E81E`, thunk réfléchi RemoveJuice à `793580`.
- Les appels natifs de reset `678929/678A8E` (retours `67892E/678A93`) restent originaux, ainsi que tout appel inconnu. Gains, activation, timers et drapeaux du Hotstreak ne sont pas réécrits. La persistance du mode actif est laissée au lifecycle natif.
- Fill/Unlimited appellent le trampoline original AddJuice avec `maximum-current`, sans multiplier ce refill par le réglage de gains (y compris si ce dernier vaut0). Le cap et les effets de remplissage restent natifs. Un joueur mort/non local/War/classe inconnue ne peut pas être rempli.
- Sept signatures exactes16octets sont exigées avant le nouveau hook RemoveJuice. EXE non conforme ou échec d'installation : fonction indisponible, original conservé.

## Munitions et requêtes

Fill ammunition réutilise les proxies natifs déjà prouvés pour Unlimited ammo, leur setter/plafond et les tableaux P1/P2 distincts. Aucun hook supplémentaire. Le calcul validé de crédits fractionnaires de réduction est identique. Les sentinelles natives négatives de munitions infinies ne changent pas ; aucun ammo/item non possédé n'est créé.

Les deux nouveaux remplissages partagent une petite politique de requête : métadonnées fraîches1,5s, une demande par acteur/génération,5s pour l'exécuter, pause/cinéma différés, respawn/identité différente/fermeture annulent. Aucune lecture de pointeur Unreal sur le thread d'interface. Relecture de la réserve après le setter/AddJuice avant de signaler Applied. Chaque joueur reçoit son propre résultat visible.

## Configuration et compatibilité

Ajouts seulement : `[Features] ReduceHotstreakDecrease=0`, `UnlimitedHotstreak=0` ; `[Values] HotstreakDecreaseDivisor=2.000`. Diviseur0–10, saisie manuelle et Default.0/1=natif ;2=moitié ;10=un dixième ; Unlimited prioritaire. Les119anciennes clés et leurs valeurs livrées sont inchangées. Actions appendues44ReduceHotstreak,45UnlimitedHotstreak,46RefillHotstreak,47RefillAmmo ; précédents0–43 et les12bindings livrés inchangés. Les quatre nouvelles actions sont neutralisées hors focus et pendant l'édition du menu. Le fichier INI personnel ne doit pas être remplacé à l'installation.

## Audit Lava : preuve, inférence et inconnues

Signalement utilisateur confirmé : **projectile normal en cloche, manette,P1** ; le faisceau Hotstreak n'est pas signalé. La session V1.46 journalise821appels de la base des pistolets,554corrections P1 et aucun P2, mais son quota de logs initiaux ne permet pas d'identifier un tir Lava. Aucun impact réel n'est mesuré.

Lecture seule des fichiers du jeu : index du pak v8 de86641entrées, SHA1 de l'index vérifié ; extraction locale bornée des seuls assets Lava utiles avec tailles, blocs Zlib et SHA1 des payloads contrôlés. Aucun asset du jeu n'est ajouté au dépôt ou au ZIP. Le Blueprint Lava normal dérive de `MayhemPlayerLavaShotProjectileAbility` ; sa variante Juiced utilise `MayhemBeamProjectile` et doit être examinée séparément.

Classe native Lava `7BABA0`, constructeur `7BB280`, vtable `27D37B0`, parent `26CA918`. Son Tick `5B7580` appelle le parent puis `5B7DC0`. Ce dernier :

1. calcule la rotation initiale avec `66A2B0` (rotation joueur `672760` et direction depuis la bouche `663F80`) ;
2. conserve le Rotator à `ability+20C/214` ;
3. lit la cible native et ses positions3D, la vitesse/gravité du projectile ; ajuste son pitch avec `6647A0` et sa vitesse avec `664800` ;
4. fournit la rotation stockée au spawn via le virtuel `+2C0`, implémentation `7BBF20`.

**PROUVÉ : ce retour final est distinct de `6A9F20/7CFA3B`, corrigé pour les pistolets depuis V1.46.** Le pitch et la vitesse sont couplés au point3D pour l'arc. **INFÉRÉ :** l'ancienne direction/cible ou l'instant de cache/release peut expliquer une sortie incohérente ; la condition exacte du bug n'est pas démontrée. Remplacer seulement le yaw ne corrigerait pas le calcul balistique global.

**INCONNU :** valeurs réelles au dernier Tick/préparation/spawn, état de l'entrée lors du release et cible native choisie ; quelles variantes normale/Juiced sont affectées. Prochaine étape : capture lecture seule synchronisée action/entrée, cible3D, cache20C/214, vitesse218 et caméra du bon joueur pendant un tir Lava normal puis Juiced. Il faut tester après V1.47 séparément le maintien d'angle, sans confondre ce correctif avec celui du tir. V1.48 ne modifie aucun hook de visée et n'annonce pas Lava réparé.

## Contrôles préparés

Nouveau test réel ConfigStore : save/reload/anciens bindings,122clés via parité complète existante, bornes/NaN, IDs appendus ; politique de déplétion et lifecycle de refill pause/respawn/expiration. Le test des vrais services gameplay est étendu aux refills ammo/Hotstreak P1/P2, sentinel-1, original AddJuice, réduction, priorité Unlimited, OFF et exclusion des resets/anciens composants. Les21suites antérieures restent présentes (22suites au total attendues). Une compilation ou un test isolé réussi ne validera pas l'interface ou le gameplay.

## Tests utilisateur et TODO cumulatifs

1. Hotstreak Strife en mission : refill à100%, attendre/consommer, diviseurs1/2/10 ; Unlimited ON puis OFF ; changer War/Strife et vérifier absence de jauge artificielle pour War. Refaire si Strife est P2.
2. Ammo : vider plusieurs réserves spéciales, Fill une fois/menu puis Hotkeys ; plafonds atteints, baisse ensuite normale ; réserve native infinie conservée. Refaire P2.
3. V1.47 : comparer présentation de toutes valeurs, saisie/Default/reload et taille de fenêtre ; angle stable lors de4–5échanges solo ; maintien/release équipement P2, bindings reconfigurés, recentrage/appui long, vue temporaire, roues et cheval.
4. V1.46 : vues coop indépendantes après respawn/cheval/ultimes, FOV coop/solo, premier primaire/secondaire pied/cheval/Strife ultime, souris/manette ; pause/roues TPS, entrée/retour scène natifs ; gameplay/refills P2 ; Unlimited health/glide/sprint ON puis OFF. Courroux à tester en mission car gratuit dans le Néant.
5. **Lava encore non réparé**, distinguer normal/Hotstreak, native/TPS et instant de relâchement ; capture suivante ci-dessus. **Visée native après dash NON RÉSOLUE ; Action Recovery INDISPONIBLE** : mesurer aussi l'instant de l'appui et le quatrième dash, pas seulement la position ; coffre différé. Aucun déblocage/mouvement forcé ajouté.

## Livraison et rollback

Branche debug uniquement ; main/archive/release publiquev1.43 et111anciennes archives protégées. Nouveau ZIP prévu `DarksidersGenesis_V1.48_HOTSTREAK_REFILLS_TEST.zip`, exactement ASI/dxgi.dll/INI/README à la racine. Contrôler CI, AMD64, identité embarquée, hashes CI/dépôt/téléchargement direct avant annoncer ces validations. Si le contrôle automatique d'autorisation reste indisponible, indiquer précisément l'opération non exécutée ; aucun contournement.

Installation seulement jeu effectivement fermé, sauvegarde des quatre fichiers + log et conservation fraîche de l'INI octet pour octet. Rollback des seuls ASI/dxgi/README vers V1.46 installée ou V1.47 construite avec l'INI personnel conservé ; désactiver les deux nouvelles options avant comparaison. Aucun lancement automatique et aucune écriture de mémoire externe.
