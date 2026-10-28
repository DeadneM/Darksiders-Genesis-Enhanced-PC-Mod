# V1.128 - lisibilite et presentation homogene du menu

28 octobre 2026. **CONSTRUITE / CONTROLEE ; RENDU COMPLET NON VALIDE EN JEU.** Base V1.127 installee sur demande avec sauvegarde V1.126 et INI conserve. L'utilisateur demande des espaces plus lisibles, des descriptions moins longues et une experience homogene ; retour general positif, pas de nouvelle validation specifique du gain coffre/dash supposee. Suite debug ; main reste V1.124.

## Objectif et critere

Lire les options sans paragraphes permanents, utiliser les memes controles et conserver une presentation utilisable avec la police personnalisee. Critere : Gameplay/Appearance/Camera/Hotkeys/Updates restent accessibles, descriptions courtes et details consultables, boutons Default et raccourcis operationnels, aucune valeur changee sans action utilisateur. Tests de jeu et visuels utilisateur encore necessaires.

## Changements de presentation

- Description courte sous l'option : premiere phrase, au plus100caracteres ; marque (?) si la suite est disponible au survol. Texte complet conserve dans l'aide. Une indication de survol commune aux onglets. Les avertissements d'indisponibilite, retours d'action et confirmations de mise a jour/rollback/reset restent visibles ; les trois confirmations gardent leurs handlers inchanges. Les lignes Ready repetitives disparaissent lorsque tout est disponible.
- Echelles coherentes : marges, ecarts, taille des champs, boutons et largeur des colonnes de raccourcis suivent la police du menu. Le style est restaure a la sortie, y compris si la fenetre est reduite/fermee ; aucun effet sur le HUD ou les autres fenetres ImGui. Les textes de statut s'adaptent a la largeur disponible.
- Curseur, saisie exacte et Default utilisent un seul composant, qui revient a la ligne si necessaire. Les listes de choix ont leur libelle au-dessus et une largeur adaptee. Les boutons associes peuvent passer dessous. Les identites des controles numeriques et les scopes de capture clavier sont preserves ; les choix conservent leurs donnees/handlers.
- Appearance : groupes fermes initialement Character models, Unlockable skins et Leviathan lightning ; bibliotheque d'effets conservee, Leviathan separe. Fermer un groupe ne desactive aucune option. Quelques sous-titres identiques au groupe sont retires. Les explications des transferts sont remises a jour sans changer les capacites.
- Heavy landing : le champ est correctement nomme Recovery delay (secondes). Ground stability time, sous Advanced landing settings, conserve la cle HeavyLandingGroundSeconds et sa valeur0,05s : temps minimum de contact continu avec le sol avant d'autoriser l'ouverture Recovery. Il filtre les contacts brefs ; ce n'est pas un nouveau correctif ni un temps ajoute apres l'animation.

## Conservation et deux revues

218valeurs INI et55ID Hotkeys inchanges ; pas de nouvelle cle/migration. Modules de jeu byte-identiques : Recovery, visee, camera/blur, RawInput/XInput, roues, armes/holsters, apparences et Leviathan, pouvoirs et FX. Seuls OverlayUi, un helper de presentation partage, BuildInfo et le branchement d'un test evoluent. V1.127 reste installee, pas de lancement, installation V1.128 ou compilation locale.

Passe1 : perimetre de fichiers, mutations config et commandes dans le meme ordre, INI identique. Passe2 :24appels numeriques litteraux compares (ID, adresse, plage et defaut), trois confirmations comparees hors espaces, etats/scopes/identites ImGui et restoration du style revus. Aucun retrait de code de jeu ou promesse FPS. [Preuve de revue](evidence/V1_128_STATIC_REVIEW.json).

Nouvelle suite OverlayLayout utilisant le vrai Dear ImGui :12combinaisons de largeur240/480/990 et police10/13/20/32 ; bornes des champs/boutons, clic reel Default apres retour a la ligne, absence de modification sans action et restoration du style. La suite precedente OverlayBindingScope reste active pour le conflit historique Zoom In/Clear. La matrice ne remplace pas l'observation du menu complet en jeu.

## Essai et suite

Conserver l'INI. Ouvrir Gameplay, Camera et Appearance ; comparer lisibilite, survol (?) et groupes. Tester taille de police habituelle et grande, redimensionner, saisir une valeur puis Default, ouvrir une liste. Verifier Hotkeys/clear d'une entree non utilisee et presence des confirmations Updates sans confirmer d'installation/reset. Pas de nouveau test de gameplay impose pour ce seul lot visuel ; continuer les essais Recovery V1.127 si souhaites.

Le tir TPS souris sur caisse/pot devant ennemi, parfois devie jusqu'au bord de l'ecran, reste OPEN. Aucun patch de selection/visee dans V1.128 ; autres munitions/manette non confirmees. Coffre/dash restent a mesurer ; coop en dernier. Rollback : ZIP V1.127 en gardant l'INI, valeurs de jeu identiques.
