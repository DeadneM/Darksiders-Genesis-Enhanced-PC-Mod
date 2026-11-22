# V1.180 — requêtes HDR expérimentales, toutes cartes

Acquis : V1.178 validée pour les effets et V1.177 pour les réglages. V1.179 Graphics est CONSTRUITE/CONTRÔLÉE (86/86, zéro avertissement), pas encore validée visuellement. Base source de cette évolution : archive debug 24d2b70. Main V1.173 conservée.

## Décision utilisateur et correction de l'approche
DX11 peut présenter du HDR. L'observation d'une branche Intel sans implémentation spécifique dans ce jeu ne justifie pas une interdiction générale de la fonctionnalité. L'utilisateur veut les mêmes choix expérimentaux pour Intel/AMD/NVIDIA et testera lui-même. Suppression du classement NativePath/PathFor et de toute restriction par constructeur dans le menu. Les anciennes observations natives restent des preuves sur un chemin du jeu, pas une conclusion absolue sur la sortie possible.

Graphics garde ses cinq groupes déplacés et leurs noms. HDR propose Native (défaut), SDR (HDR off), HDR10 1000/2000 nits et scRGB 1000/2000 nits. Luminosité facultative du HUD natif 0–4, Native par défaut, disponible pour un profil HDR. Les profils 1000/2000 correspondent aux sorties existantes du moteur, pas à une calibration garantie de l'écran. Les métadonnées d'écran/GPU restent en lecture seule à l'ouverture/Refresh, sans boucle par image.

## Application et retour arrière
Deux préférences [HDR] OutputMode / UIBrightness. Le helper existant attend la fermeture complète, relit la demande finale et utilise la même transaction de propriété/sauvegarde/readback/rollback que Rendering quality. HDR10 demande r.AllowHDR=1, r.HDR.EnableHDROutput=1, OutputDevice=3/4 et ColorGamut=2 ; scRGB demande OutputDevice=5/6 et ColorGamut=0. SDR demande 0 aux quatre champs. HUD personnalisé : r.HDR.UI.CompositeMode=1 et r.HDR.UI.Level=valeur. Native restaure les anciennes valeurs de ces six CVars seulement si elles appartiennent toujours au mod ; éditions externes prioritaires. Les profils SDR/Native restaurent aussi le HUD antérieur, sans perdre la préférence facultative enregistrée.

Aucun patch de drapeau de capacité, détour d'API HDR, shader, ResizeBuffers forcé ou modification Windows. Même code pour toutes les marques, aucune qualification de constructeur exigée. Il s'agit de demandes au moteur au prochain lancement, pas d'une preuve de transport HDR, de metadata correcte, de gamut ou de rendu. Le comportement de l'overlay et du HUD doit être testé. Plein écran exclusif éventuellement nécessaire ; garder Windows HDR activé.

## Contrôles et limites
288 clés / 13 sections : les 286 anciennes valeurs identiques, deux ajouts Native. Tests prévus : défauts, bornes/NaN/mode invalide, Save/Load ; quatre profils et six CVars exacts, SDR, Native avec restauration des clés anciennes/absentes, idempotence et entrées invalides sur fichiers temporaires UTF8/UTF16. Les garde-fous processus/rollback/édition externe du helper existant restent testés. Code de mêlée/visibilité/pistolets intact ; lames tenues/rangées restent un travail ultérieur.

V1.179 : ZIP vérifié à deux binaires et lien HTTP200 ; l'analyse Defender demandée n'a pas été exécutée car le contrôle automatique d'autorisation a échoué pour quota, sans décision d'insécurité. Aucun contournement. Le développement local V1.180 continue ; compilation/livraison à confirmer selon disponibilité de ce contrôle.

## Essai groupé
Installer en conservant l'INI. Dans Graphics > HDR, essayer d'abord HDR10 1000 nits, fermer puis relancer, vérifier image/hautes lumières/couleurs et menus. Revenir Native avec le même cycle ; ensuite essayer scRGB 1000 nits si nécessaire. Éventuellement tester le plein écran exclusif. Vérifier l'état de sortie rapporté et la profondeur du buffer, sans les considérer seuls comme validation du HDR. Luminosité HUD Native puis 1.5 à comparer seulement si le HDR fonctionne. Retour souhaité : rendu correct/incorrect/inchangé pour chaque mode et type d'affichage.

Avant tout downgrade : choisir Native et fermer le jeu pour restaurer les six CVars ; un ancien helper ne comprend pas les nouveaux champs de propriété HDR. Sauvegarde Engine.ini.DGE-render.bak et journal Engine.ini.DGE-render-owned.json conservés. Aucun fichier du jeu/personnel modifié pendant cette préparation, aucune installation automatique. ZIP deux binaires sans INI/README.
