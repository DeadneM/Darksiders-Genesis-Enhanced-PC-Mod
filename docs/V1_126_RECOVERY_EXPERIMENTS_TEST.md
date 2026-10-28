# V1.126 - essais Recovery et effets de surface Tremor

28 octobre 2026. **CONSTRUITE / CONTROLEE ; NOUVEAUTES NON VALIDEES EN JEU.** Base cumulative V1.125 controlee, issue de V1.124 validee et conservee sur main. L'utilisateur demande de garder les apparences actuelles, chercher seulement des ajouts, conserver le Leviathan confirme a part, et autorise des parametres Recovery experimentaux. Suite uniquement debug ; pas de lancement, installation ou build local.

## Objectifs et criteres

1. Exposer des temps de reception lourde sans changer le profil accepte quand les nouveaux essais sont OFF. Comparer reprise de mouvement, geste/impact et saut ensuite sur les deux personnages.
2. Tester une fenetre MOVE native apres fin d'animation sur les seuls coffres/dashs identifies. Succes : meme animation/loot/mouvement utile, reprise plus rapide quand une action reste active apres son montage. Echec possible sans effet si aucun etat compatible n'existe : diagnostic READY n'est pas preuve de resultat.
3. Ajouter un seul groupe visuel Tremor avec quatre assets natifs verifies ; les43anciens groupes/choix restent equivalents et Leviathan reste separe. Succes visuel a confirmer sur un nouvel effet, sans changement de degats, comportement ou surface physique.

## Reglages du menu

Gameplay -> Player movement -> Recovery experiments, groupe ferme par defaut. Toutes les nouvelles cases OFF par defaut ; le profil de reception lourde existant garde sa propre case.

| Reglage | Defaut/plage | Action |
| --- | --- | --- |
| Custom heavy landing timing | OFF ;0,150s, plage0,050..0,800s | Requiert Heavy landing movement recovery existant. Delai apres debut d'ability avant fenetre native MOVE. A l'arret, le moteur garde la reception ; deplacement maintenu peut l'interrompre. |
| Continuous ground contact |0,050s, plage0..0,250s | Stabilite au sol requise dans le seul profil custom. Custom OFF conserve0,15s/50ms et bornes historiques acceptes exactement. |
| Ground dash after-animation recovery | OFF | Suit seulement les classes natives ground dash Strife Level1/2/3, Aim/Fire Level1/2/3 et War. Aucun bouton suppose. |
| Chest after-animation recovery | OFF | Seulement ChestInteract Strife/War et leurs quatre montages Chest/En_Chest natifs ; autres interactions intactes. |
| Wait after animation stops |0s, plage0..1s | Partage dash/coffres, ajoute une attente apres une confirmation de30ms d'arret natif. Ce n'est pas un multiplicateur global d'animation. |

Le custom utilise les vrais notifies MOVE cuits0,833373s Strife/0,813425s War pour la propriete/restauration ; le chemin custom OFF conserve les seuils de la version acceptee. Pas de retouche des gestes Geyser/Stoneskin/Caltrops.

## Chemin natif des essais apres animation

Aucun nouveau hook. Observeur local joueur existant, game thread/focus/menus/cinematique/formes controles. Native IsPlaying676620 doit avoir ete observe vrai sur le meme helper/montage, puis faux ;0 avant lancement n'est jamais une fin. Identites faibles acteur/ability/helper/montage, meme monde/generation, possession reciproque et ability courante en sommet de pile, role local3, sante positive et etat RUNNING sans fin en attente requis.

Coffre : helpers MayhemMontagePlayer et AnimMontage veritables, noms et packages exacts. Dash : seules les references directionnelles originales200/208/210/218 de l'ability peuvent servir de montage. Coffre : Walking sans custom. Dash : Walking sans custom ou mode6/custom5, et compteurs224/228 finis/valides dans les deux cas ; Walking seul ne prouve pas que son mouvement utile est fini. Ni timer, vitesse, root motion, loot, flags d'entree/visée, AllowedActionsFlags ou animation n'est ecrit.

Apres la grace choisie et30ms de confirmation, ouverture unique de MOVE via5A9D60, la meme fonction deja utilisee par HeavyLanding ; le jeu gere la demande reaffectable, son interruption et sa fin. Aucun Finish, StopMontage, SetMovementMode, touche simulee ou patch d'ability global. Une fenetre deja native n'est jamais revendiquee. OFF/perte de contexte restaure uniquement une fenetre acquise, avec les API natives et identites/monde/proprietaires recontroles ; monde change/objet expire/fin deja engagee => pas d'appel sur un ancien objet. Shutdown ne dereference pas les objets sur son thread.

Limites : un helper/montage deja libere ou une ability deja terminee n'est pas eligible. L'essai ne raccourcit pas le mouvement utile ni une animation encore active. Un temps mort situe ailleurs peut donc rester identique. En coop les contextes sont independants dans les fixtures, mais l'essai en jeu reste differe.

## Apparences conservees et ajout

Appearance -> Native effect library -> Tools -> Tremor surface impact : Native / Gold debris / Metal debris / Stone debris / Water splash / Hidden. Quatre ParticleSystem exportes du dossier natif PlayerGear/TremorGauntlet/Ground, index PAK et SHA1 controles ; ne pas les presenter comme de nouvelles ameliorations du gantelet. [Preuves](evidence/V1_126_TREMOR_APPEARANCE.json).

Groupe ajoute en fin d'enum/familles pour conserver les anciens indices/valeurs.44familles,455assets,99roles. Les98anciennes correspondances sont comparees par nom/package et conservent leur sens apres tri des quatre nouveaux assets. Renderer/fabrique inchanges : seulement les nouvelles particules reconnues, cible chargee, sinon rendu natif. Aucun chargement synchrone ou nouveau scan de demarrage. Les composants persistants gardent les limites existantes ; aucun ancien groupe supprime ou corrige d'office.

LeviathanVisualFeature et son reglage confirme sont byte-identiques. Stinger reste egalement sur son chemin natif deja accepte ; il n'est pas ajoute en doublon a la bibliotheque. Les212valeurs INI precedentes et55ID Hotkeys sont conserves ; sept cles ajoutees,219au total. INI personnel conserve par l'updater.

## Controles et essai groupe

66suites Windows x64 attendues. Nouvelle suite RecoveryTail : OFF, observation play/stop, grace, mouvement dash termine, cas inconnus/menus/focus/thread/proprietaire/package, identite recyclee, monde change, fenetre native preexistante, restauration, P2 separe et I/O runtime. Tous les anciens tests HeavyLanding conserves, avec profils custom/delai/stabilite/restauration additionnels ; bibliotheque teste chaque role/variante y compris les quatre nouveaux assets. [Revue statique](evidence/V1_126_STATIC_REVIEW.json). Fixtures ne prouvent ni reprise en jeu ni rendu.

Essai utilisateur apres livraison GitHub :

1. Nouvelles cases OFF : regression courte deplacement/tir/dash et reception lourde actuelle ; Leviathan et apparences personnelles inchanges.
2. Activer Heavy landing recovery + Custom timing : comparer0,15 puis0,30s (contact0,05s), maintenir deplacement apres une chute ; marcher/sauter ensuite, Strife puis War. Les valeurs tres courtes peuvent couper davantage la pose de reception.
3. Activer seulement Chest after-animation : attente0 puis0,15s, maintenir deplacement en ouvrant un coffre ; verifier loot, deplacement/saut et seconde interaction. Activer avant de commencer l'action.
4. Activer seulement Ground dash after-animation : deplacement maintenu pendant la chaine Strife, puis avec visee/tir ; distance et munitions restent natives. Comparer0 puis0,15s. Si aucun effet, le journal permettra de distinguer demande appliquee et cas non eligible ; ne pas valider sur READY seul.
5. Tremor : provoquer un nouvel impact de surface, comparer une cible chargee, revenir Native, verifier comportement inchangé. L'option n'agit ni sur tous les effets du gantelet ni sur la geometrie du sol.

Rollback : desactiver les trois nouvelles cases et mettre Tremor surface impact sur Native, ou reprendre le ZIP V1.124/main en gardant l'INI personnel. Aucune promotion main de l'essai, anciennes archives conservees.
