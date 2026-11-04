# V1.151 — lot cumulatif à tester

Base livrée V1.150 ; garde Moloch précédemment validé conservé. Tout nouveau comportement reste **EXPÉRIMENTAL** jusqu'au retour utilisateur. Compilation uniquement sur GitHub ; pas d'installation automatique. ZIP prévu : deux binaires à la racine, sans INI/README. Garder son INI, ses raccourcis, son PAK et ses sauvegardes.

1. **Trait/arc Moloch.** TPS Only : les deux disparaissent en TPS et reviennent en vue native. Hide Everywhere : absents dans les deux vues. Never : présents. Tirs, effets des pouvoirs et décor conservés. Refaire un essai dans un autre niveau. [Preuve et périmètre](V1_151_AIM_MATERIAL_SHADERS.md).
2. **Dash/stomp Strife.** Après un saut : sans direction maintenue à la réception, stomp sans dash directionnel supplémentaire ; avec une direction maintenue, enchaînement natif. Vérifier impact/dégâts et reprise de contrôle. War utilise son chemin natif inchangé.
3. **Pose après visée + dash/stomp.** Relâcher visée/tir et marcher : retour normal sans tirer pour débloquer. Puis tenir réellement la visée pendant le dash : elle doit continuer à fonctionner. Essayer deux pistolets Native puis une main OFF. Correction limitée au dash terminé ; changement de munition seul encore ouvert.
4. **GPU après fermeture.** Choisir l'adaptateur, fermer complètement le jeu, attendre quelques secondes puis vérifier Engine.ini et relancer. Le helper lit le dernier choix sauvegardé et écrit seulement après arrêt. Résultat dans gpu-result.txt du dossier d'état de l'updater ; sauvegarde Engine.ini.DGE-GPU.bak. Aucun rechargement UE forcé.
5. **Moloch Boss Crash Fix.** Nouveau nom dans Gameplay → System and startup ; clé Features.MolochBossCrashFix, défaut ON pour une nouvelle configuration. Migration de l'ancien choix explicite préservée. Sous-titres et protection validée inchangés.
6. **Effets intermittents V1.150 conservés.** Stoneskin Strife après chargement/changement d'armure, dont Abyssal : pierre/feu puis restauration. Effets de cœur après rechargement. Bibliothèque et reprise ciblée restent à valider, aucun gain FPS annoncé.

Contrôles de régression utiles : menus/roues, bras OFF et rangement, monture/ultime, munitions/pouvoirs et updater. Coop en dernier comme convenu. Ne pas confondre les suites CI avec une validation en jeu. Repli V1.150 conservé ; branches main/archive et anciennes archives protégées.
