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

Signalement utilisateur : **manette,P1**. La session V1.46 journalise821appels de la base des pistolets,554corrections P1 et aucun P2, mais son quota de logs initiaux ne permet pas d'identifier un tir Lava. Aucun impact réel n'est mesuré.

Lecture seule des fichiers du jeu : index du pak v8 de86641entrées, SHA1 de l'index vérifié ; extraction locale bornée des seuls assets Lava utiles avec tailles, blocs Zlib et SHA1 des payloads contrôlés. Aucun asset du jeu n'est ajouté au dépôt ou au ZIP. Le Blueprint Lava normal dérive de `MayhemPlayerLavaShotProjectileAbility` ; sa variante Juiced utilise `MayhemBeamProjectile` et doit être examinée séparément.

Classe native Lava `7BABA0`, constructeur `7BB280`, vtable `27D37B0`, parent `26CA918`. Son Tick `5B7580` appelle le parent puis `5B7DC0`. Ce dernier :

1. calcule la rotation initiale avec `66A2B0` (rotation joueur `672760` et direction depuis la bouche `663F80`) ;
2. conserve le Rotator à `ability+20C/214` ;
3. lit la cible native et ses positions3D, la vitesse/gravitée du projectile ; ajuste son pitch avec `6647A0` et sa vitesse avec `664800` ;
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
