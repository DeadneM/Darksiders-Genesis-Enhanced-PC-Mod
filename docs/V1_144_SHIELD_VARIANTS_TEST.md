# V1.144 - variantes de barrière et suivi de la pose

**CONSTRUIT / CONTRÔLÉ, variantes EXPÉRIMENTALES en jeu.** Base V1.143, masquage du cristal Stinger expressément VALIDÉ par l’utilisateur. Fire aura comme remplacement du Porte-bouclier de la Légion est REJETÉE et retirée de cette sélection. Le Stinger natif/masqué et le cache CPU Stoneskin restent dans leurs chemins précédents.

## Objectif et choix

Proposer trois véritables particules de bouclier dans le groupe Legion shieldbearer en bas d’Appearance, après Leviathan : Native=0, Hidden=1 inchangés, Rampage shield=2, Chaos shield=3, Sphere shield (Borrower)=4. Dans un ancien INI réglé sur Fire aura=2, le choix devient intentionnellement Rampage shield en réponse au retrait demandé. Aucun autre choix n’est remappé. Paramètres communs aux vues native et TPS.

Templates vérifiés en lecture seule dans le PAK : P_Rampage_Shield et P_Chaos_Rampage_Shield (déjà dans la bibliothèque), P_Borrower_Shield (ajout unique, ParticleSystem, sphère et anneau en espace local). Les modules de particules ont été examinés ; cela ne prouve pas un placement/échelle satisfaisant sur le cœur. Effets à tester. Certains templates empruntés ont des cycles courts ; la continuité visuelle doit aussi être vérifiée.

La sphère Borrower est uniquement une cible de remplacement. Son apparition native sur un ennemi passe intacte, même si le visuel du cœur est masqué ou remplacé. Le même garde empêche de signaler Hidden prêt quand seule la cible est chargée. Aucun nouveau hook, API moteur ou chargement synchrone. Les templates doivent déjà être chargés ; sinon le visuel natif est conservé avec avertissement dans le menu. N’affecte ni protection, régénération, dégâts, coût ou durée du statut. Aucun INI, raccourci ou nombre d’options de famille ajouté ; 229 clés et 55 ID Hotkeys conservés.

## Retour V1.143 et performance

Stinger crystal : VALIDÉ par l’utilisateur. Fire aura : REJETÉE seulement pour le bouclier ; le template partagé de Flamebrand reste dans sa famille d’origine.

Le dernier journal démarre bien V1.143. Cache : 1 182 181 succès, 22 défauts de cache. stoneskin-observe moyen : 278,08 µs, contre 1 025,51 µs dans le relevé V1.142 antérieur ; les scènes/personnages/états sont mélangés et non identiques. Baisse du coût échantillonné observée, pas de comparaison FPS/GPU ou validation de fluidité. Aucun nouveau changement de cet observateur dans V1.144.

## Pose et sujets différés

Précision utilisateur : tout le haut du corps reste en visée, y compris les deux bras lorsque deux pistolets sont visibles ; changement de munition reproduit, Stoneskin ne déclenche pas le défaut. Ne pas élargir le correctif du bras OFF à ce symptôme. Un lecteur externe VM_READ plus précis est préparé, incluant montages, positions/poids et états bruts des nœuds de graphe. Démarrage après signal prêt, comparaison demandée avant/après le tir qui rétablit la pose. Aucun correctif ou succès de pose annoncé sans cause établie. Dash aérien explicitement reporté.

## Critères de réussite et repli

Essayer les trois variantes quand elles sont chargées, à la prochaine apparition du bouclier, en vue normale et TPS. Vérifier orientation, position, taille, continuité, lisibilité près de la caméra, disparition avec le statut et retour Native/Hidden. La protection doit rester native. Stinger validé : vérifier son masquage et le retour Native sans affecter les autres visuels Stoneskin. P2 différé. Les variations de FPS seront rapportées séparément des contrôles de fixtures.

ZIP attendu : ASI et dxgi.dll seulement à la racine. Compilation exclusivement GitHub ; pas d’installation ni lancement du jeu par l’agent. INI personnel, raccourcis, PAK, sauvegardes, anciennes archives, main/archive conservés. Repli : ZIP V1.143, sans utiliser sa variante Fire aura rejetée ; garder l’INI.

## Livraison et dernier retour

L’utilisateur n’a pas réussi à reproduire de nouveau la pose bloquée ; les essais de reproduction sont arrêtés, sans déclarer le bug corrigé. Le lot passe 70/70 contrôles GitHub après correction du premier échec de compilation. [Reçu de livraison, preuves et ZIP](V1_144_DELIVERY_RECEIPT.md). Les nouvelles variantes sont à tester ; Stinger demeure validé.
