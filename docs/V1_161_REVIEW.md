# V1.161 — première activation Stoneskin sur Strife

Base V1.160 `7c429b778266e41bb5ebd2324e7bc605685aae2f`. Retour utilisateur : saut VALIDÉ, raccourci OK ; sur trois Stoneskin, la première explosion manque. Coop déjà acceptée. Main reste V1.159.

Journal local V1.160 du 11 novembre : cast 15:24:49.663, premiers matériaux acceptés 15:24:49.916–918, aucune demande d'explosion correspondante. Casts 15:24:51.753 et 15:24:53.831 suivis d'une demande acceptée respectivement à .757 et .834. Aucun refus de création enregistré. Ces faits ne démontrent pas le rendu d'un composant ; ils montrent que la reprise après refus de V1.160 ne suffit pas au premier lancement.

Le chemin actuel efface le ticket sur une lecture initiale sans statut, même avant la première apparition de celui-ci. V1.161 conserve brièvement ce ticket en attendant le statut natif (une seconde au maximum, cadence existante de 50 ms pour ce ticket). Une absence après un statut déjà observé reste une expiration immédiate. Ni statut, ni matériaux, ni effet ne sont inventés pendant l'attente. Un statut trop tardif ne ressuscite pas un ticket expiré, même après une interruption d'observation.

Les bornes de reprise V1.160 après création refusée restent identiques. Propriétaire/monde/forme doivent rester valides. Même explosion, geste, peau/écharpe, cendres, aura et dégâts ; aucun nouveau hook/API. Mayhem/Rampage et défaut D-pad bas conservés sans modification. Le lien causal entre publication tardive et premier échec visuel reste à confirmer par le prochain essai.

Tests supplémentaires : première liste vide puis statut présent, liste initiale illisible, arrivée Molten tardive, délai dépassé, interruption de service, monde/forme perdus. Compilation Windows uniquement GitHub. **PRÉPARÉ / EXPÉRIMENTAL**, non validé en jeu à cette étape. Aucun téléchargement d'ancien fichier ni nouveau relevé mémoire.

Objectif de l'essai : après lancement ou chargement, vérifier la toute première explosion Stoneskin sur Strife, puis deux activations suivantes. Saut/raccourci/coop restent acceptés ; pas de retest général demandé. ZIP cumulatif deux binaires, INI personnel conservé, rollback V1.160 local disponible.
