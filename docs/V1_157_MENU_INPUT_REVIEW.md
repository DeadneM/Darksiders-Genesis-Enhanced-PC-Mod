# V1.157 — clics du menu, actions et lifecycle natif

## Base et retour utilisateur

Base cumulative debug V1.156 (`b3669c9ce373d699cef7a725da8191ddb62f342d`), main V1.153 inchangée. V1.155 n'est pas validée : l'utilisateur confirme que certaines fonctions ne marchent toujours pas sous le menu et signale l'impossibilité de cliquer à la souris. Le dernier journal chargé confirme `1.155-menu-runtime-continuity-test`. **GPU après fermeture/redémarrage désormais VALIDÉ par l'utilisateur.** Coop/P2 reste reporté à la fin. Les corrections cheval/écharpe V1.156 sont reprises sans modification, toujours expérimentales en jeu.

Objectif du lot : recevoir normalement chaque clic du menu et conserver les règles de gameplay lorsque l'éditeur est visible. Critère de réussite : cocher/décocher et modifier les valeurs avec la souris sans fermer le menu, utiliser les F1–F12 configurées hors saisie/capture, conserver le dash/stomp natif modifié et retrouver exactement les commandes après fermeture et Alt-Tab.

## Causes établies et correction

1. ShouldSuppressNativeAim avait été étendu à Runtime dans V1.155. Le routage des boutons souris réutilise ce prédicat : lorsque WantCaptureMouse était faux, un DOWN de l'éditeur pouvait devenir une route de tir, puis son UP était retourné au jeu avant l'appel ImGui. Le menu pouvait donc perdre un relâchement. Le menu possède maintenant tous ses messages souris, même au bord/hors de sa fenêtre interne ; une route de tir ne peut pas commencer sous l'éditeur. Un UP antérieurement détenu par le jeu garde son équilibrage natif mais est également signalé à l'éditeur s'il s'est ouvert depuis. Les clics d'édition ne sont plus interprétés comme un tir physique du mod.

2. Le service AirDash recevait encore loadingAllowed, issu de Commands : le menu annulait les gestes différés bien que leur demande provienne de l'action native. Il reçoit maintenant Runtime, comme les autres reprises de mouvement. Les pauses, roues, cinématiques, décès et propriétaires continuent leurs vérifications natives ; aucune modification du dash validé hors éditeur.

3. ProcessInput retournait avant tous les raccourcis F1–F12 et TriggerAction répétait des refus menu dans plusieurs branches de ressources. Une règle unique permet ces raccourcis dans la fenêtre de jeu active, même avec l'éditeur visible, sauf pendant saisie de texte ou réaffectation de touches. Les appuis capturés sont consommés afin de ne pas se rejouer à la sortie. Les refus répétés sont retirés au profit du même contrôle de focus commun, sans changement des 55 actions ni de leurs affectations.

Les journaux montrent des callbacks de visée, de ressources et d'apparences entre Overlay OPEN et CLOSED : ce n'était pas l'arrêt absolu de tous les modules. La sauvegarde des sliders publie déjà les valeurs runtime immédiatement. Les commandes de navigation, raccourcis d'orbite/recentrage, capture de touches et souris de l'éditeur conservent leur propriété ; on ne fait pas tourner la caméra avec une souris utilisée pour les sliders. Aucun exemple utilisateur précis n'est nécessaire pour démontrer les trois refus ci-dessus.

## Contrôles et limites

Les contrôles existants de lecture manette imbriquée, fenêtres natives et dash sont étendus : aucun DOWN d'éditeur ne détient son UP comme tir ; un ancien DOWN de jeu garde son UP ; raccourcis autorisés sous l'éditeur mais refusés en saisie/Alt-Tab ; état de dash déjà demandé maintenu avec Runtime. La réception Win32/ImGui complète et les clics réels restent à valider dans le jeu, pas prouvés par ces fixtures. Aucun nouveau hook ou appel moteur pour le menu ; code cheval/écharpe conservé.

Deux relectures du nouveau périmètre : usages/callers, puis transitions DOWN/UP, ouverture/fermeture et restauration. Les cinq refus menu répétés du dispatcher et l'ancienne liste partielle de consommation F-keys sont remplacés par la même politique ; les buffers de routage restent nécessaires à l'équilibrage des anciens appuis. Aucun autre code supprimé sans preuve. Pas de compilation locale. CI Windows complète requise avant installation.

## Livraison et essai groupé

INI et raccourcis personnels conservés, sauvegarde avant installation demandée par l'utilisateur. ZIP cumulatif de deux binaires à la racine, sans INI/README. Attendre la fermeture du jeu avant remplacement, ne pas relancer automatiquement. Main/archive, anciennes archives et comportements validés sont conservés. Retour arrière : réinstaller la sauvegarde précédente, garder l'INI.

Essai : 1) clics checkbox/slider du menu ouvert et F-key hors saisie ; 2) ouvrir/fermer pendant un appui puis Alt-Tab sans clic/tir coincé ; 3) dash/stomp avec éditeur affiché si l'action arrive au jeu ; 4) premier cheval du chapitre 1 sans rappel, puis descendre/rappeler ; 5) Stoneskin écharpe avant/pendant/après sur deux armures. Coop à la fin. Le GPU est accepté, pas à redemander dans ce lot.

Statut à cette rédaction : EXPÉRIMENTAL, compilation et essais en jeu à venir. La réussite GitHub sera enregistrée séparément.
