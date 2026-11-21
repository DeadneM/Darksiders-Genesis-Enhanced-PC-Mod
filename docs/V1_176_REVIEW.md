# V1.176 — catalogue de post-traitement

Base : debug 2575c38 (V1.175), main V1.173 conservée. Objectif : choix Native/personnalisé pour les familles disponibles et correction des contrats avant tout constat visuel.

## Correction démontrée
V1.175 associait SceneFringeIntensity au bit voisin : PP+5/0x20. REJETÉ. La métadonnée bool pointe sur son setter à +48, pas -8 ; le setter exact OR [rcx+5],0x10 est à RVA 1DD3150. Association recoupée avec les contrats déjà connus DOF et MotionBlurAmount. V1.176 utilise 0x10. Les anciens succès CI ne démontraient pas ce contrat ni son rendu.

## Catalogue
17 paramètres supplémentaires : bloom/dirt, lens flares (intensité/seuil), vignette, grain/jitter, AO (intensité/rayon/puissance), SSR (intensité/qualité/roughness), exposition EV, cubemap, LUT, maximum du motion blur. Tous Native (-1) par défaut. Valeurs personnalisées >=0 ; plafonds exploratoires par paramètre, pas des maxima universels du moteur. Exposition 0 = neutre, pas désactivation. Les assets natifs restent nécessaires pour dirt/cubemap/LUT ; les options du moteur peuvent désactiver certaines passes.

Inventaire hors jeu : 119 champs float candidats. Ce nombre ne signifie pas 119 réglages disponibles : champs signés, dénominateurs strictement positifs, fonctions RTX absentes du rendu DX11, textures/vecteurs restent hors catalogue tant que leur domaine et leur utilité ne sont pas prouvés. Ne pas forcer ces paramètres à zéro.

Amount motion blur et netteté montent à 10, poids TAA min 0. Maximum blur 0–100 (% écran) relie aussi le CVar final natif r.MotionBlur.Max, indépendamment vérifié. Aucun nouveau hook, changement de qualité, profil DOF, délai ou fréquence. Chemin Native sans boucle sur le catalogue. Publication atomique ; contrats vérifiés une fois sur l'exécutable supporté, sortie partielle native conservée. Diagnostic borné de sortie caméra, explicitement pas une observation du rendu final.

## Contrôle et limites
Tests : conservation Native octet par octet, champs/flags exacts isolés, limites/NaN, transitions partielles, contrats absents, publication/sauvegarde/rechargement. 262 anciennes clés du fichier de référence à conserver ; 17 clés PostProcess ajoutées, total 279. INI personnel et installation intacts. CI à exécuter sur GitHub ; aucun build local. Statut PRÉPARÉ / EXPÉRIMENTAL, jamais validation visuelle.

## Test groupé
Chromatique Native/0/1/5 puis Native, en observant les bords contrastés. Motion Amount 0/2 avec Max 5/20 puis Native, en mouvement. Ensuite un seul groupe à la fois : Native/0/valeur d'essai/Native (bloom, grain, vignette, AO, SSR, exposition). Vérifier retour Native, transitions et profils native/TPS. Une absence d'effet peut dépendre des passes/assets du niveau et doit être rapportée ; aucun gain GPU annoncé.

Rollback : réinstaller les deux binaires de V1.175 en gardant l'INI (nouvelles clés ignorées), ou désactiver les nouveaux réglages pour retrouver Native. V1.175 conserve toutefois son défaut chromatique ; V1.173 reste la base stable publiée. Archives anciennes, main/archive et raccourcis inchangés.
