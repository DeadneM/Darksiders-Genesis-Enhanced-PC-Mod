# Statique : guidage restant à 100 % dans V1.135

30 octobre 2026. **AUDIT CONTRÔLÉ ; aucune réparation construite.** Retour utilisateur : les réglages semblent agir, mais Statique paraît plus guidée que le jeu d'origine même par défaut. Cela ne valide pas une équivalence native. L'ASI installée correspond maintenant à V1.135 (SHA256 vérifié) ; installation faite hors de cette intervention. Aucun fichier du jeu ni INI modifié ici.

## Première passe : ce qui reste actif

Dans `NativeAimAssistFeature.cpp`, OFF retourne simplement le pourcentage 100. `SelectTarget` remplace toujours l'argument de direction par `ray.direction`, celle du TPS, et mémorise le choix du moteur. `Target` peut encore restituer son point socket/bornes à OFF ou100. Dans `ProjectileAimFeature.cpp`, ce point passe AVANT `TraceCameraTarget` et réoriente entièrement le consommateur vers lui. Ce n'est pas seulement un réglage des angles natifs.

Le cache est accepté jusqu'à250ms, avec contrôles d'identité/joueur/génération/arme et de position devant le canon, mais sans nouvelle vérification angulaire ou d'obstacle à cet endroit. Hors mode par munition activé, le finder commun peut servir de repli lorsque le choix spécifique n'est plus attribuable. Ces mécanismes peuvent favoriser une cible éloignée du réticule ou maintenir brièvement une ancienne direction. Cela décrit une possibilité démontrée par le code, pas la mesure de son incidence dans l'essai.

À100, les angles ne sont effectivement pas écrits. Aucun changement supplémentaire des constantes de rayon, poids de sélection, portée physique, dégâts ou chaînes n'a été trouvé dans ce module. Le problème potentiel est l'autorité qui choisit la direction finale, pas un multiplicateur caché de dégâts ou de portée.

## Deuxième passe : comparaison avec les consommateurs natifs

Le binaire d'origine appelle6C3520 à5B9BEA avec une direction issue de672760 puis8A8F10 et publie le choix sur le personnage àE98. Lors de la création du faisceau (69818F..6981CB) et de sa mise à jour (5B9FD0..5B9FFF), le jeu affecte déjà le composant de cible via617B90. Son sélecteur vérifie les angles avant le classement et le bonus de cible actuelle (6C39C8..6C39DE). Il existe donc bien un accrochage natif, qu'il ne faut pas confondre avec notre ajout.

La substitution partagée de V1.135 peut aussi diriger un consommateur libre vers un acteur mémorisé, notamment le retour forward du faisceau à61D5E5 lorsqu'il n'a pas de cible attachée. Elle n'est pas conditionnée à l'usage effectif de cette cible par ce consommateur natif. Le retrait du mauvais filtre Statique reste justifié ; ce filtre ne doit pas être réintroduit pour masquer ce deuxième problème.

**Les deux passes concordent : « 100 % = natif » était une formulation trop large. Cela garantit les valeurs angulaires natives, pas le comportement complet du tir.** L'ajout de direction/point est une explication crédible du guidage ressenti, pas encore une cause unique prouvée par comparaison en jeu.

## Journal et limites

La session V1.135 terminée à22:21:40 compte1172requêtes,231ajustements et311utilisations du point sélectionné, P1 uniquement. Les lignes observées passent par200%,0%,100%, avec des angles natifs restaurés90/45 et un rayon7500. L'INI final est Global100. Les311utilisations regroupent les munitions et les réglages de la session : ce ne sont pas311impacts Statique à100. Aucune nouvelle capture mémoire ni comparaison sans mod n'a été faite.

## Suite ciblée

Retirer l'autorité ajoutée qui recentre les consommateurs vers un point mémorisé et revoir le remplacement systématique de direction ; conserver la base TPS validée et laisser chaque consommateur natif appliquer son propre choix. Vérifier d'abord une vraie référence OFF/100, puis seulement le réglage des angles. Ne pas ajouter un autre filtre de contact, un cône arbitraire ou modifier la longueur du faisceau. Une correction demande un nouveau build isolé et un essai : cet audit n'en annonce aucun.

[Preuve bornée et compteurs](evidence/V1_135_DEFAULT_STATIC_AUDIT.json). Aucun nouveau test de compilation : le code source est inchangé ; les69contrôles de la livraison précédente ne prouvaient pas l'équivalence avec le jeu d'origine.
