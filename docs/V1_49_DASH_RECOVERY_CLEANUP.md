# V1.49 — Action Recovery dash et nettoyage ASI TEST

## Base et objectif

Base : V1.48 installée, acceptée par l'utilisateur hors les six réparations documentées ; dépôt debug `4161ac46c01d23e90ab867c4c913c2341e676c57`. Demande du 17 octobre : conserver un ASI propre, reprendre Action Recovery et revérifier les changements. Les coffres restent différés. Cette version isole **la fin du dash et le retrait de code sans consommateur** ; les corrections de roues/trajectoires ne sont pas ajoutées au même essai.

**SOURCE PRÉPARÉE / EXPÉRIMENTAL.** Une fonctionnalité réelle est branchée sur le lifecycle natif, mais son gain en jeu, la compilation et la livraison restent à contrôler au moment de cette préparation. Le simple rétablissement d'une case n'est pas présenté comme une réparation validée.

Objectif : après la décélération utile d'un dash à pied, demander la fin normale native pour rendre les commandes plus tôt. Réussite utilisateur : intervalle troisième → quatrième dash Strife réduit, sans raccourcir le déplacement utile, glissement, état de mouvement bloqué ou invulnérabilité persistante ; OFF restitue le timing natif. Comparer aussi War et P2. L'option fonctionne indépendamment du mode TPS et des touches choisies dans le jeu.

## Point natif nouvellement vérifié

EXE supporté SHA256 `9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54`.

- Inventaire historique des captures : **979 observations** de dash au vtable `27D31B8`. Son Tick `+258 → 5B72B0` appelle la mise à jour native puis le virtuel de déplacement `+2E0 → 5B93F0` ; sa procédure de nettoyage `+2A8 → 59C110` est distincte. Cela ne mesure pas les deux timers en jeu.
- **5B0F40** démarre explicitement une phase de décélération en écrivant la même durée float dans `ability+224` et `+228`. **5B93F0** décrémente 224 avec le dt natif, puis applique une vitesse proportionnelle à `max(224/228,0)` lorsque la durée est positive. Il ne faut ni accélérer ce Tick ni modifier sa courbe pour traiter la récupération.
- Le Tick natif `5B72B0` appelle déjà **EndAbility(false)=59B080** dans son propre lifecycle. `59B2D0` passe l'action à l'état5 et appelle sa procédure de fin virtuelle. Pour la variante observée, **59C110** arrête le montage par la fonction native, retire le drapeau temporaire `actor+AAC`, restaure le mouvement via le virtuel natif et remet à zéro la vélocité du dash. La version test utilise cette même procédure complète ; elle ne force pas un drapeau MOVE ou une rotation du personnage.
- Six signatures de 16 octets contrôlées dans l'EXE : Tick, début de décélération, déplacement, EndAbility, nettoyage et getter de classe de dash. La garde tient compte des préfixes REX présents dans les octets réels ; le texte désassemblé seul ne suffit pas à recopier une signature.

**Limite importante :** les anciennes captures de position ne contenaient pas les timers224/228 ni l'appui du quatrième dash. Le moment de fin de cette décélération par rapport à la fin du montage et le gain réel de contrôle restent à constater dans l'essai. Si aucune phase positive n'est observée, la nouvelle logique laisse l'action native se terminer. Aucun gain de millisecondes n'est promis.

## Intervention V1.49

Nouveau module [ActionRecoveryFeature.cpp](../src/ActionRecoveryFeature.cpp), un seul hook de Tick :

1. L'original exécute entièrement son pas de physique/animation avec le dt reçu, exactement une fois.
2. La logique vérifie classe de dash, virtuels de déplacement/nettoyage prouvés, état RUNNING, composant actif réciproque, personnage local vivant dans sa forme à pied, rôle autorité et mode de mouvement personnalisé du dash. Horse/Ultimate, interactions/coffres, autre variante, pause, cinématique, autre thread ou lecture invalide gardent le natif.
3. Chaque joueur possède une petite trace contenant seulement des identités, dates et valeurs, jamais un UObject relu depuis un cache. Une durée positive et un compteur restant **encore positif doivent avoir été observés pour cette activation**. Un zéro initial, un ancien compteur négatif, un acteur remplacé, une réactivation ou un échantillon expiré ne suffisent pas à autoriser une fin.
4. Une fois cette phase épuisée, le délai réglé écoulé et l'action encore active, le module demande **une seule** fin native `EndAbility(false)`. Il ne touche plus l'objet après cet appel. La procédure native conserve la responsabilité du montage, des effets et du mouvement.

Le module ne change ni les positions/vitesses/timers, ni les axes/boutons, ni l'animation principale tant que la phase de décélération reste active. La procédure de fin native peut raccourcir la fin visuelle du montage : c'est l'expérience demandée, à comparer en jeu. Aucun ancien hook de fondu V1.39 ni déblocage forcé V0.8A n'est réintroduit.

Journal borné par joueur : `DASH_DECELERATION` pour les huit premières phases positives observées, puis `DASH_FINISH_REQUEST` quand une demande est effectivement émise, elapsed, durée/reste de décélération et délai. Les compteurs distinguent phases observées et demandes natives ; **ils ne mesurent pas le gain de contrôle et ne valident pas le résultat visuel**. Les entrées normales ne créent pas de spam par frame.

## Menu, configuration et compatibilité

Gameplay → Player movement : **Action Recovery (dash)**, case ON/OFF ; **Recovery delay**, valeur 0–300 ms avec curseur/saisie/Default communs. Zéro attend uniquement l'épuisement observé de la décélération. Le réglage `ActionRecovery=1` et `ActionRecoveryDelayMs=0` déjà livrés sont conservés : l'activation existante prend maintenant un effet dans le module natif si les gardes passent. L'option est marquée expérimentale.

Hotkeys : l'action **ActionRecovery conserve l'ID3**, reprend le vrai basculement ON/OFF et sauvegarde. Les **48 IDs**, les anciens tokens et les12affectations livrées restent inchangés. Les nouvelles demandes de raccourci sont ignorées hors focus/menu ou si le module est indisponible. Aucun raccourci par défaut réaffecté, aucun bouton de dash imposé.

## Code retiré et contrôle de propreté

Retrait de cinq réglages sans consommateur, de leurs membres compilés, lectures, écritures et valeurs livrées :

- `ActionRecoveryMultiplier`, ancien réglage sans usage ;
- `DodgeEarlyUnlockMs`, ancien déblocage retiré ;
- `TPSChargedFirePadMask`, détecteur ignoré depuis V1.10 ;
- `TPSHideGroundAimFx`, ancien interrupteur du hook natif retiré ; le filtre shader actif reste piloté par `AimIndicatorHideMode` ;
- `TPSHorseStableAimCamera`, ancien interrupteur sans consommateur ; le mode actif `TPSMountedAimMode` est conservé.

**117 clés actives**, contre122dont ces cinq inactives. Toutes les valeurs actives restantes sont identiques à V1.48. Un INI personnel ancien conserve ses lignes inconnues lors d'une sauvegarde ; elles ne sont plus chargées ni générées par l'ASI. L'installation ne doit pas remplacer ce fichier. Les tests couvrent cette conservation, pas seulement un fichier neuf.

Retrait des deux atomiques de chord chargé écrites sans lecture, de leurs écritures, du compteur/rapport purement diagnostique et de la variable physique devenue inutile. **TPSChargeChordDown reste conservée** : sa garde de forme ultime a encore un usage comportemental. Les commentaires prétendant que le getter de visée ne modifiait aucun résultat, ainsi que d'autres descriptions devenues trompeuses, sont corrigés. Aucun nouveau détecteur de touche n'est ajouté à Recovery.

Contrôle source : **57 autres fichiers existants protégés**, conformes à la base, dont les modules de tir, ciblage natif souris, coop, propriété, jauges/gains, munitions/Hotstreak, santé/potions, cheval, glide, warnings et updater. La caméra principale n'a qu'une correction de commentaire. Les fonctions de visée/RawInput/axes sont revérifiées séparément après suppression de télémétrie. Cela ne constitue pas une preuve d'absence absolue de tout code mort dans le dépôt.

## Vérification et limites

Nouveaux tests du **vrai module** avec frontières natives simulées : physique originale avant la demande, dt conservé, phase positive puis épuisée, absence de départ sur zéro/vieux timer, délai, OFF/ON, une demande, réactivation/respawn/expiration, P1/P2 indépendants, native déjà terminée, variante inconnue, pause/cinéma, Horse/Ultimate, acteur non autorité, composant invalide et valeurs non finies. Test ConfigStore réel : ancien token/ID, publication de l'activation/délai, bornes/NaN et conservation des lignes personnelles retirées du code.

Les22suites V1.48 restent présentes ; **24suites attendues** avec ces deux ajouts. La CI Windows x64 et son analyse doivent confirmer compilation et tests. Contrôler ensuite quatre fichiers à la racine du ZIP, AMD64, version1.49,117clés, absence des cinq noms de réglages retirés dans l'ASI, empreintes CI/dépôt/téléchargement et conservation des112ZIPantérieurs. Ces contrôles ne valident pas la distance, le ressenti ou les transitions réelles du dash.

Première CI `38998637969`, source `0fb9f17` : compilation réussie, test natif DashRecovery réussi et22/24suites réussies. Deux tests de configuration ont décelé une ligne de sauvegarde orpheline `TPSChargedFirePadMask`, qui recréait une clé et pouvait remplacer sa valeur personnelle par celle du buffer précédent. Cette ligne a été supprimée avant toute création de paquet ou installation ; le contrôle statique couvre désormais aussi les noms de clés indépendamment de leur casse. Les tests restent exigeants, sans assouplissement de l'attente de conservation/parité. Huit avertissements MSVC historiques, aucun dans le nouveau module Recovery.

Les autres réparations restent ouvertes : roue d'équipement coop P1/P2, Charge Shot/Hotstreak en reculant, Graviton et cohérence du pitch 3D, Lava normal balistique, visée native après dash. Le pitch par défaut -12° et les chemins de tir ne changent pas ici. Les acquis V1.48, y compris refills/Hotkeys, sont conservés. Coffre différé.

## Livraison prévue, test et rollback

Branche **debug** uniquement. Nouveau ZIP cumulatif `DarksidersGenesis_V1.49_DASH_RECOVERY_CLEANUP_TEST.zip`, ASI/dxgi.dll/INI/README à la racine ; nouveau numéro obligatoire pour conserver V1.48 immuable. Pas de promotion de main/archive ni de remplacement de la release publique v1.43.

Installation seulement jeu effectivement fermé, sauvegarde ASI/dxgi/README/INI/log, conservation fraîche de l'INI et contrôle des empreintes installées. Aucun lancement automatique ni écriture externe de mémoire.

Test ciblé : en mission, Strife à pied, comparer OFF puis ON délai0 : trois dashs enchaînés, puis tentative du quatrième ; répéter en maintenant le déplacement puis en attaquant à la sortie. Vérifier distance, arrêt, changement de direction, reprise d'attaque et absence d'invulnérabilité persistante. Comparer ensuite War, P2, modes natif/TPS et une sortie de menu. Délai150/300 ms doit ajouter une attente après décélération, OFF doit restituer le natif. Conserver le journal pour savoir si une demande native a été émise.

Retour arrière immédiat : désactiver Action Recovery ; si nécessaire réinstaller les binaires/README du ZIP V1.48 conservé, en gardant l'INI personnel courant. Une option retirée du code n'a aucun effet à réactiver dans une vieille ligne INI ; revenir aux binaires correspondants est le rollback de code.
