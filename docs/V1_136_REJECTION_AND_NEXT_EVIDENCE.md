# V1.136 — rejet du correctif et régression des curseurs

31 octobre 2026. **REJETÉ PAR L'UTILISATEUR pour la visée.** Le verrouillage excessif initial persiste ; l'utilisateur précise que les curseurs n'ont plus d'effet. Il confirme avoir coché Adjust native acquisition pendant l'essai. Aucun nouveau défaut de direction n'est déduit de cette précision, ni aucune régression des autres fonctions validées.

## Ce qui est réellement établi

La session V1.136 du31octobre20:12:48..20:14:47 a été sauvegardée en référence privée avant écrasement éventuel par une nouvelle session. Le code était initialisé.485requêtes locales sont attribuées, aucune modulation d'angles n'est comptée, aucun point n'est récupéré par l'observateur synchrone prévu. La recherche2D native a été acceptée435fois et le repli3D41fois. Aucune application de direction libre de faisceau n'est comptée. Ces nombres ne mesurent pas des impacts ni la cause du verrouillage.

L'observateur66A620/59D3BD était censé conserver le point réellement consommé par le calcul natif du tir. **Ce chemin n'a jamais participé dans la session relevée.** Retirer le mécanisme précédent sans avoir prouvé l'existence de ce remplacement pendant les vrais tirs était une erreur d'approche de V1.136. Les fixtures simulaient expressément cet appel : elles validaient la mécanique de portée, pas son utilisation réelle dans le moteur.70tests réussis ne validaient donc pas cette substitution fonctionnelle.

L'affectation620 et le point628 du faisceau restent natifs en V1.136. Restaurer la recherche2D n'a pas démontré une correction du chemin verrouillé qui contourne la branche libre. Il ne faut pas annoncer cette étape comme une réparation accomplie ni ajouter un nouveau hook sur la seule base de l'ancien schéma supposé.

## Transmission des valeurs : constat incomplet

Les lignes disponibles ne montrent que100%, avec0requête modulée. L'INI final est OFF/Global0. **L'utilisateur confirme pourtant la case cochée pendant l'essai** ; la sauvegarde précédant l'installation était aussi ON/Global100. L'état final ne doit pas être utilisé pour lui attribuer une option oubliée. Les valeurs successives et les cibles ne sont pas corrélées dans ce journal.

La revue source ne trouve pas de liaison manquante menu/config/runtime : Save publie immédiatement les valeurs, Snapshot contient les champs et Publish écrit les atomiques lues par Percent. Ces fichiers sont inchangés depuis V1.135. Un état non observé, un contexte non attribué ou une consommation du choix en dehors des sorties surveillées restent à distinguer ; aucune de ces pistes n'est déclarée cause unique.

L'interface affiche les curseurs même caseOFF, contrairement à beaucoup d'autres options. C'est une ambiguïté possible de présentation, mais pas l'explication retenue du retour utilisateur confirmé.

## Suite concrète, sans nouvelle modification du jeu

Un lecteur externe en lecture seule est préparé pour relever simultanément les paramètres sauvegardés, les candidats des finders natifs, les vecteurs de regard, la cible faible sélectionnée du personnage et la vraie cible620/point628/direction270/longueur du faisceau principal. Le lecteur ne lance pas le jeu, ne fait aucun appel moteur et ne modifie aucune mémoire. Il ne sera exécuté qu'après le signal prêt de l'utilisateur, puis les essais seront groupés0/100/200 et Standard/Statique.

V1.135 et sa sauvegarde restent disponibles comme référence précédente où l'effet des réglages semblait perceptible ; son guidage par défaut contesté n'en fait pas une base Statique validée. Aucun rollback automatique, installation, nouveau build ou hook n'est effectué pendant ce diagnostic. L'installation reste V1.136 jusqu'à une action autorisée distincte. main/archive et anciennes archives intacts.

[Rejet utilisateur, empreintes et compteurs](evidence/V1_136_USER_REJECTION.json). Les reçus CI/package/install de V1.136 restent historiques et ne constituent pas une validation en jeu.
