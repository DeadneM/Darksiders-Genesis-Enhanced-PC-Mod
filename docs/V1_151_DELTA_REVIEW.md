# Revue du delta V1.151 préparé — 4 novembre 2026

Source locale sur debug, base livrée V1.150. Protection Moloch validée conservée. V1.150 ressources reste expérimentale ; les deux expériences GPU/trait-arc V1.149 sont rejetées. Aucune build, CI, publication ou installation du lot V1.151. Ce document concerne le delta en préparation, pas un nouvel audit exhaustif du mod.

## Passe 1 : transitions et effets de bord

- Sortie de pose : chemin normal AIM → Aim_End → ClearAimMode identifié ; absence de ce chemin dans les échantillons de marche après dash, puis retour après le tir confirmé par l'utilisateur. Le mode avant le tir n'a pas été capturé : hypothèse testable, pas cause définitivement prouvée.
- Essai source : réutiliser la fonction native pure SetAimMode, entièrement vérifiée, une fois après le seul dash observé terminé. Refuser capacité active, AIM/FIRE, pause, cinématique, roue, monture/ultime, propriétaire inconnu et verrouillage de cible. Protéger l'appel contre la réentrance. Aucun montage stoppé, pas de fausse action AIM ou de tir, pas d'écriture directe dans le personnage. Le raccourci et les joysticks restent réaffectables.
- GPU : écriture uniquement après fermeture et sauvegarde native finale ; relire le choix final du mod. Ajouter un refus si le jeu redémarre après la copie de sauvegarde et avant l'écriture. La vérification de processus réduit le risque de redémarrage simultané sans rendre cette opération atomique avec un lancement indépendant du jeu.
- Helper : ne pas tronquer un EXE déjà lancé et réutiliser seulement les octets embarqués identiques. Ne pas conserver la référence GetModuleHandleEx d'ExtractHelper, qui n'avait pas de FreeLibrary correspondant. Le pin explicite de durée de vie des tâches existantes est conservé.

## Passe 2 : contrats et comparaison des chemins

- Empreintes complètes AirDash36/185 et setter30 dans un seul DashScriptGuard.h ; opcodes et constantes signés, références résolues vérifiées séparément. Test de mutation du setter ajouté à la suite existante prévue ; aucune suite compilée ici.
- Script du setter : exactement une affectation de propriété, retour et fin. Les deux références AimMode doivent viser la même propriété de la classe Strife ; Mode doit être l'unique paramètre enfant, un octet à l'offset0. Un contrat différent conserve le comportement natif sans recherche répétée à chaque image.
- Le corps du chemin de reprise AIM maintenu est identique au précédent après retrait de la seule branche nouvelle AIM relâché. Les hooks TPS/look et les modules de projectiles, bras OFF, sauts, coop, Léviathan, bouclier et Stoneskin restent identiques à HEAD.
- 246 valeurs INI conservées, seule clé RenderCrashGuard renommée en MolochBossCrashFix, avec migration du choix personnel et défaut ON pour une nouvelle configuration. 55 actions Hotkeys conservées.
- Ancien writer GPU du proxy, export/header et test abandonnés retirés ; les deux hooks expérimentaux de shader V1.149 et leur politique/test retirés. Ensuite, à la demande de build, les 168 empreintes natives des deux matériaux capturés ont été extraites et leur absence de partage avec les autres entrées natives vérifiée. Aucun hash inventé ; le nouveau résultat graphique à Moloch reste à tester. Voir V1_151_AIM_MATERIAL_SHADERS.md.

## Critères de réussite au prochain lot construit

| Sujet | Vérification en jeu nécessaire |
|---|---|
| Pose Strife | Visée + dash/stomp puis relâchement : marche/course normale sans tirer pour débloquer ; visée maintenue toujours fonctionnelle. Tester modèles Native et une main OFF. |
| Dash/stomp Strife | Sans direction maintenue à la réception : pas d'enchaînement directionnel ; direction maintenue : dash natif. Impact, dégâts et contrôle conservés. War inchangé. |
| GPU | Modifier le choix, fermer le jeu, lire Engine.ini et gpu-result.txt, relancer : choix conservé. Aucun Engine.ini personnel modifié pendant cette préparation. |
| Moloch Boss Crash Fix | Nouveau nom menu/INI, défaut ON ; sous-titres et protection validée inchangés. |
| Ressources V1.150 | Après chargement/changement d'armure : pierre/feu Stoneskin et effets de cœur disponibles. Rendu et coût en jeu restent non validés. |
| Trait/arc Moloch | Vérifier le nouveau catalogue : TPS Only masque les deux en TPS, vue native les conserve ; Hide Everywhere masque les deux, Never les restaure. Autres effets/décor conservés. |

La pose bloquée après changement de munition sans dash reste ouverte. Coop en dernier. La build cumulative a été demandée après cette revue ; les résultats de compilation et de package seront consignés dans le reçu de livraison. Les contrôles statiques seuls ne remplacent pas les tests C++/C# sur GitHub ni le jeu.
