# Statique - audit du fonctionnement natif apres V1.133

30 octobre 2026. AUDIT CONTROLE ; REPARATION NON CONSTRUITE. Aucun nouveau build, installation, lancement ou capture memoire. V1.133 reste installee. Le retour utilisateur sur sa portee fait rouvrir le filtre Statique V1.130, sans remettre en cause les autres acquis confirmes.

## Constat du journal

Session terminee a19:56:39 : aide ajoutee appliquee5166fois en P1 ;2637affectations Statique controlees,2637supprimees,0acceptee. INI finalGlobal100. Les compteurs prouvent que les calculs ont tourne et que le filtre a retire toutes ces cibles ; ils ne mesurent ni impacts ni distance perdue. [Preuves](evidence/V1_134_STATIC_NATIVE_AUDIT.json).

## Comportement natif

Le Tick61D410 possede un chemin avec composant cible620 : longueur desiree640 calculee depuis la distance au composant. Sans composant et sans point cible explicite628, le tir libre utilise BeamMaxLength5AC. Les assets originaux verifies donnent1200unites de jeu pour BeamMaxLength,1800pour MaxDistanceFromOwner de la chaine,4cibles configurees normalement et7en Hotstreak. Ce sont des valeurs serialisees de base, pas une mesure de portee effective ni un nombre garanti de rebonds sur une scene.

L'affectation native617B90 ne change pas seulement une direction : quand la cible change, elle retire l'association precedente, detruit les faisceaux secondaires referencés et remet leur liste a zero, puis stocke la nouvelle cible. La creation de chaine61FC90 exige un composant cible et les donnees de chaine. Retirer la cible pour ne permettre que le tir libre change donc le fonctionnement natif et peut reduire la portee ressentie ou perturber les rebonds, sans aucune ecriture de BeamMaxLength.

## Erreur de notre approche

Le filtre PrimaryStatic/AssignTarget V1.130 exigeait un contact bloquant deja confirme sur le rayon camera et sur le volume du faisceau. Il retire en amont la cible qui autorise justement le chemin natif vise/chaine. Le contrat de collision Projectile Overlap/Ignore renforce ce refus. Les faisceaux secondaires n'etaient pas interceptes directement, mais cela ne les preservait pas des effets du setter natif appele avec une cible nulle. La precedente affirmation de preservation des chaines et de la portee etait donc trop forte.

V1.131..133 ajoutent un autre recentrage vers la cible native, apres ce filtre. Ce pourcentage n'est pas une proportion du fonctionnement vanilla ; zero conserve le filtre V1.130 et100ajoute notre correction. L'avoir conserve comme base validee a preserve une erreur structurelle. Les deux passes - source/journal puis Tick/setter/chaine/assets - convergent sur cette erreur. La distance exacte perdue et la causalite complete du retour visuel ne sont pas mesurees.

## Decision pour la suite

Revenir au choix de cible et au cycle du faisceau natifs ; retirer le filtre force-tir-libre et le recentrage ajoute quand le remplacement est pret, sans toucher aux correctifs de pose/main vide, controles/cameras et autres fonctions validees. Ne pas restaurer artificiellement la portee en augmentant1200, ni presenter un tir libre allonge comme du natif.

L'option demandee doit modifier l'accrochage natif : OFF conserve le jeu ;100% est la reference native ; en dessous, acquisition plus restrictive ; au-dessus, plus permissive. Global et valeurs par munition restent souhaites. Portee physique, degats et nombre/distance des chaines restent independants. Les champs distance/angles/score sont visibles dans le selecteur6C3520, mais leurs types/proprietaires par arme et leur restauration doivent etre verifies avant ecriture. Aucun nouvel offset brut n'est applique dans cet audit. Il ne suffit pas de changer le texte du curseur actuel.

## Parametres retrouves avant implementation

Les champs sont maintenant nommes par les metadonnees du binaire : MayhemTargetingProperties (0x30octets), SearchRadius a0, VerticalSearchAngle a4, HorizontalSearchAngle a8. Le selecteur6C3520 lit ces trois champs depuis helper+28/+2C/+30.66A620 calcule le point natif depuis Socket_Projectile_Impact ou les bornes du modele : ne pas substituer arbitrairement la racine/pieds du personnage.

Les assets originaux ont ete relus depuis le PAK et compares aux references privees, y compris empreintes du flux et du payload. Statique declare SearchRadius16500 et angle horizontal30 ; le profil partage DualPistols declare7500 et45. Standard et Charge declarent1200 et30 ; Beam declare1200,35horizontal et80vertical. Plusieurs types pointent vers TargetingPropertiesOverrideAbility=DualPistols : ces declarations ne prouvent pas a elles seules quel helper est actif a un instant donne. [Contrat et matrice](evidence/NATIVE_AIM_PARAMETER_CONTRACT_2026_10_30.json).

La politique0..200%,100%=natif, est preparee hors de l'ASI. Branchement au selecteur et remplacement du filtre/ancien recentrage attendent la confirmation en lecture seule du lien helper/capacite/arme. Le jeu etait ferme lors de la demande ; le signal de preparation utilisateur est attendu. Aucune nouvelle fonction n'est annoncee construite, installee ou validee.
