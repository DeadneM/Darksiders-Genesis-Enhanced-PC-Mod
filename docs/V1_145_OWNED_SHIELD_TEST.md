# V1.145 - bouclier adapté et cycle natif

**CONSTRUIT / CONTRÔLÉ, EXPÉRIMENTAL en jeu.** Base V1.144 avec Stinger Native/Hidden VALIDÉ et conservé. Pour le Porte-bouclier de la Légion, Rampage/Chaos sont REJETÉS et retirés de la sélection ; leurs templates restent dans la bibliothèque pour les pouvoirs qui les utilisent déjà.

## Objectif et rendu

Native=0 et Hidden=1 restent inchangés. Belial barrier=2 remplace Rampage ; Dagon shield=3 remplace Chaos ; Borrower=4 est conservé ; Water sphere=5 est ajouté. Borrower, jugé prometteur par l’utilisateur, est réduit à 60 % de sa taille précédente. Facteurs d’attachement initiaux Belial 0,35, Dagon 0,25, Borrower 0,60, eau 0,35. Ces tailles et rendus sont EXPÉRIMENTAUX. Choix communs aux vues native et TPS, au même endroit sous Appearance après Leviathan. Les cibles doivent être chargées, sinon effet natif conservé. Aucune charge synchrone, duplication de particules ou changement du statut de protection.

Les trois nouveaux templates sont vérifiés dans le PAK et ne peuvent servir que de cibles : leurs apparitions natives sur ennemis restent inchangées. Le numéro Borrower=4 de l’INI est préservé. Aucune nouvelle clé : 229 clés et 55 ID Hotkeys conservés.

## Disparition et preuve native

Lecture statique du binaire exact : MayhemStatusEffect_DamageShield dérive du statut natif et ses trois variantes de cœur pointent vers P_CreatureCore_ShieldDemon. Le traitement de dégâts 0x6D0860 appelle l’arrêt 0x69FB90 quand la capacité est absorbée ; l’arrêt libère la référence de composant +0x78. Destinataire +0x68, source de particules dans Properties +0x58, instigateur +0x70 confirmés par code et métadonnées. Aucun appel ou hook ajouté à ces fonctions ; leurs signatures conditionnent le support du suivi.

Le nouveau suivi mémorise uniquement les composants créés pour ce remplacement, puis les lie au statut DamageShield du personnage local : liste native de statuts, type exact, destinataire, source, monde, identités complètes et référence de composant. Quand le jeu retire cette référence, le rendu privé est masqué avec la fonction de visibilité déjà vérifiée. Aucun calcul de points de bouclier ni timer de protection ajouté. Un coup unique avant la première observation est traité par l’attachement au personnage et son unique statut de cœur, après la fin de l’appel de création. En cas d’ambiguïté, pas de liaison arbitraire.

Le pool restaure notre ancien drapeau avant la politique Hidden du nouvel effet, notamment Stinger. Les références périmées ou templates changés ne reçoivent pas d’écriture. Table bornée à 32 composants ; capacité indisponible ou signatures absentes : remplacement refusé, source native conservée. Observation au plus toutes les 100 ms par joueur, sans polling supplémentaire quand aucun suivi n’est requis. La taille passe par l’interception SpawnEmitterAttached déjà utilisée et validée pour les effets de mêlée, avec source, aura et propriétaire locaux vérifiés ; aucun nouveau hook/API moteur. Effets des lames et Lava conservent leur chemin précédent.

## Stoneskin et limites

L’utilisateur précise que la peau pierre/feu manque aléatoirement : le lien avec le choix de bouclier n’est PAS établi. Le journal contient des refus de remplacer des matériaux déjà surchargés. Le garde conserve ces matériaux plutôt que de les écraser. Pas de correction de matériau sans identification des parents/propriétaires ; source StoneSkinVisual inchangée. Préflight VM_READ préparé mais NON lancé : l’utilisateur a demandé d’attendre. Stinger demeure validé et indépendant.

Pose bloquée après changement de munition : toujours ouverte, non reproduite au dernier essai ; pas de correctif spéculatif. Dash aérien reporté explicitement.

## Critères et essais

1. Borrower plus petit en TPS et vue native ; vidé par un coup ou plusieurs, son rendu doit disparaître avec le bouclier, puis revenir à la recharge. Vérifier que protection et délai restent natifs.
2. Tester Belial, Dagon et eau quand chargés : position, taille, continuité, lisibilité près de la caméra et fin du rendu. Leurs proportions sont à valider.
3. Stinger masqué/natif inchangé ; autres particules Stoneskin et matériau pierre/feu conservés quand le défaut intermittent ne survient pas. Si le défaut revient, conserver l’état pour un prochain relevé plutôt que réinitialiser des matériaux à l’aveugle.
4. Retour Native/Hidden, changement de zone, cheval/ultime et coop restent à valider pour ce nouveau suivi. Les fixtures P1/P2 ne constituent pas une validation coop en jeu.

ZIP attendu à deux fichiers racine ASI et dxgi.dll, sans INI/README. Build GitHub uniquement, aucun lancement ou remplacement de fichiers du jeu par l’agent. Préserver archives, main/archive, INI/raccourcis/PAK/sauvegardes. Repli : V1.144 en gardant son INI, Native/Hidden ou Borrower ; éviter les choix Rampage/Chaos rejetés.

## Livraison

71/71 contrôles Windows réussis, aucun avertissement détecté. ZIP et catalogue updater contrôlés, Defender sans menace détectée. [Reçu et lien direct](V1_145_DELIVERY_RECEIPT.md). Aucun gain/rendu en jeu déduit de ces contrôles.
