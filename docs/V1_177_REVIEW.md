# V1.177 — qualité de rendu, application au redémarrage

Base : debug V1.176 e14fbb6, fonctions retenues par retour utilisateur (quatre réglages post-traitement conditionnels documentés séparément). Stable main V1.173 intacte.

Objectif : sept choix supplémentaires, Native par défaut, sans travail ni écriture mémoire récurrente pendant le jeu : anisotropie 1–16 ; résolution interne 25–200 % ; qualité ombres 0–5 ; taille maximale ombres 64–8192 ; distance ombres 0–5 ; distance visibilité 0–5 ; qualité SSR 0–4. Bornes de test explicites, pas maxima universels ; 0 n'est pas valide pour tout. Les limites des assets, du renderer et des presets du jeu s'appliquent encore.

Sept noms et références natives vérifiés hors jeu dans l'EXE supporté. Pas de preuve de gain GPU ni du résultat visuel. Catalogue dans RenderQualitySettings.h ; choix UI enregistrés avec saisie manuelle. Section Camera > Rendering quality (restart required). Pas de nouveau hook, scan ou temporisation en jeu ; pas de forçage des profils DOF ou des dix-sept effets V1.176.

## Application et retour Native
Le helper embarqué existant attend la fermeture du jeu (mécanisme GPU conservé), relit les choix finaux du mod INI puis écrit uniquement les sept CVars autorisés dans SystemSettings d'Engine.ini. Le nouveau lancement les charge nativement ; pas de tentative de recréation improvisée des samplers d'anisotropie en session. Un redémarrage est donc requis, y compris pour revenir Native.

Aucun nouveau choix ne modifie Engine.ini tant qu'il est Native et sans propriété antérieure. Une fois personnalisé, un journal adjacent .DGE-render-owned.json conserve la présence/valeur précédente et la dernière écriture du mod. Native restaure seulement une valeur encore identique à la nôtre ; les éditions externes sont conservées. Une clé initialement absente est supprimée lors de sa restauration, une clé existante est rétablie. Sauvegarde exacte Engine.ini.DGE-render.bak avant les changements. Journal écrit atomiquement avant les modifications et validé avant tout accès ; garde processus fermé, lecture de contrôle, rollback sur erreur si le jeu est toujours fermé. Aucun fichier de sauvegarde du jeu touché. Le mécanisme GPU existant reste inchangé ; seul son helper sert aussi ces préférences.

Ne pas annoncer des mises à jour visuelles immédiates : les CVars existent, mais leur utilisation effective dépend encore du moteur. Le lancement du helper exige toujours l'EXE exact et les contrôles existants. Aucun helper lancé, aucune écriture du véritable Engine.ini pendant le développement de cette version.

## Contrôles
Tests prévus : bornes et choix entiers, Native/NaN, defaults INI et Save/Load ; helper sur fichiers temporaires UTF8 et UTF16, sept CVars, conservation des autres clés/commentaires, aucune écriture au jeu ouvert ou avec tous les choix Native, idempotence, retour Native, clé absente/existante, édition externe, entrée invalide, journal corrompu, rollback après interruption. 279 anciennes valeurs de référence à préserver ; 7 Native ajoutées, total 286 / 12 sections.

Statut PRÉPARÉ / EXPÉRIMENTAL avant CI. Builds uniquement GitHub, ZIP deux binaires racine sans INI/README. Installation personnelle non touchée ; ne pas promouvoir main.

## Essai groupé
1. Installer en gardant l'INI. Choisir anisotropie 16, résolution 75 %, ombres qualité 2/taille 1024/distance 0.5, visibilité 0.5, SSR 0, fermer puis relancer. Résolution doit changer nettement ; comparer surfaces obliques, ombres et reflets dans une scène adaptée.
2. Remettre Native, fermer puis relancer : retour à la configuration antérieure. L'anisotropie seule peut être peu perceptible selon les textures/angles ; distances ne créent pas de LOD absents. Vérifier les réglages GPU existants et le rendu V1.176 inchangés.
3. Vérifier éventuellement 100/125 % puis Native et les réglages individuellement ; éviter d'empiler les valeurs les plus coûteuses pendant le premier essai.

Rollback : revenir Native et fermer le jeu avant de remettre les anciens binaires. Remplacer seulement l'ASI ne retire pas automatiquement des valeurs déjà écrites dans Engine.ini. La sauvegarde et le journal servent au retour contrôlé ; ne pas effacer le journal avant restauration.
