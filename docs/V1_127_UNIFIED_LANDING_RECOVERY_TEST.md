# V1.127 - reception lourde unifiee et bilan Recovery

28 octobre 2026. **CONSTRUITE / CONTROLEE ; CONDITIONNEMENT NON VALIDE EN JEU.** Base cumulative V1.126 ; son timing personnalise de reception lourde est maintenant accepte par l'utilisateur. Coffre/dash restent incertains. main reste sur V1.124 validee. Aucun jeu lance, build local, installation ou modification du profil personnel.

## Objectif et critere

Une seule case Heavy landing movement recovery avec le delai deja accepte, au lieu de deux cases dependantes. Reussite : conserver le ressenti V1.126 master+custom ON, timing personnel et raccourcis, marche/saut possibles apres reception, vues native/TPS. Delai par defaut0,15s (0,05..0,8), contact sol0,05s (0..0,25). Master OFF conserve le natif. Les constantes MOVE0,833373/0,813425 et tous les garde-fous sont ceux du chemin custom accepte. Pas de nouvelle mecanique de recuperation.

La cle HeavyLandingCustomTiming et son etat runtime sont retires ; une sauvegarde de reglages supprime seulement cette cle obsolete. HeavyLandingRecovery et les deux temps restent inchanges.218valeurs livrees identiques,55ID Hotkeys conserves. Les apparences et Leviathan confirme restent inchanges et separes. Geyser/Stoneskin/Caltrops, transferts, camera/visee et chemins XInput/RawInput/menus/roues byte-identiques.

## Ce que dit reellement le journal V1.126

Session du28octobre12:30..12:57,1287lignes/200423octets, hash dans la preuve statique. Six demandes coffre (12:41:29,12:43:55,12:45:06,12:46:18,12:47:29,12:56:55). Zero demande dash, bien que les deux options soient cochees dans le profil releve en fin de session. Huit messages reception lourde ; le budget etant huit, ce n'est pas un total exact des receptions.

**Une demande MOVE ne mesure pas le retour effectif du controle.** Le coffre a execute son chemin ; son gain reste non mesure. Le dash n'a emis aucune demande dans ce journal : aucune amelioration annoncee. Les logs precedents ne permettent pas de choisir entre absence de fin de montage observable, action deja terminee, fenetre native deja ouverte ou garde de mouvement non satisfaite. Ne pas supprimer ces gardes pour produire artificiellement un effet.

V1.127 ajoute des compteurs separes dash/coffre : suivi d'identite/helper/montage, lecture en cours, premier arret observe, mouvement non pret ou MOVE deja present a ce premier arret, demandes. Douze lignes d'observation maximum, plus deux bilans de fermeture. Pas de scan ni nouveau hook ; seulement des compteurs et lectures deja necessaires. Ils ne mesurent toujours pas une latence de mouvement. Les compteurs suivent des identites de suivi, pas necessairement toutes les activations natives d'une instance reutilisee. Les timers et l'arret de montage requis restent strictement identiques.

## Tir TPS souris : enquete ouverte

Retour confirme : souris TPS, Statique cite ; autres munitions et manette non verifies. Profil releve :3D aiming ON, pitch camera-12deg, correction de visee+12deg.

1. La cible commune vient du helper natif571E50, trace simple171E560, canal19 Projectile. Le DefaultEngine.ini original associe ce canal a GameTraceChannel6. Son profil Destructible utilise Overlap pour Projectile alors que Damage bloque par defaut ; une trace simple de blocage peut donc ignorer ce destructible et retenir une collision derriere. Ce n'est pas encore la preuve de l'objet exact du retour utilisateur. Changer globalement vers Damage pourrait accrocher le maillage du joueur ou d'autres surfaces : pas de remplacement aveugle.
2. Le traitement natif du faisceau61D410 a des chemins avec composant cible620 ou point cible628. Ces chemins precedent et contournent la branche de tir libre corrigee a61D5E5. Une cible native verrouillee constitue donc une autre piste distincte de la direction libre.

Precision utilisateur recue pendant la compilation : il s'agit bien d'une caisse ou d'un pot destructible. Les assets Statique originaux controles par index/hash et payload identique indiquent aussi4cibles secondaires natives (7en Hotstreak) dans1800unites UE, faisceau principal1200unites : ces chaines devront rester distinctes du choix de la cible initiale.

Le journal ne fournit ni identite de la cible ni ordre des collisions lors du defaut. **Aucun correctif de visee annonce/applique dans V1.127.** NativeTpsFeature/ProjectileAimFeature et les controles sont inchanges. Prochain releve : type d'objet exact, objet devant/ennemi derriere, comparer meme scene en3D ON/OFF et autre munition ; capture des collisions/cibles si le symptome persiste. Ne pas generaliser a la manette sans essai. [Preuves natives et limites](evidence/V1_127_MOUSE_TARGET_AUDIT.json).

## Deux passes et essai groupe

Passe1 : chemin complet UI/config/runtime, suppression du drapeau obsolete, nettoyage de cle a la sauvegarde, I/O des temps/raccourcis et restauration des fenetres. Passe2 : reconstruction independante du code V1.126 custom ON et comparaison hors commentaires/espaces : identique ; revue du predicat dash/mouvement, du compteur borne et des chemins inchanges. Pas de promesse de gain FPS. [Comparaison et hash du journal](evidence/V1_127_STATIC_REVIEW.json).

Essai : garder l'INI, verifier une seule case reception lourde avec tes valeurs ; deux chutes en maintenant le deplacement puis saut, vue native/TPS. Activer coffre seul puis dash seul, maintenir le deplacement jusqu'a la fin ; marche/saut/seconde interaction et munition speciale. Rapporter le ressenti, puis fermer normalement le jeu pour ecrire les deux bilans. Coop en dernier. Rollback : options Recovery OFF, ou ZIP V1.126 avec son INI sauvegarde. Toute amelioration coffre/dash reste EXPERIMENTALE jusqu'au retour en jeu.
