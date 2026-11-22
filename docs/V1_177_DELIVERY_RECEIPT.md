# V1.177 — qualité de rendu livrée sur debug

**CONSTRUIT / CONTRÔLÉ ; résultat visuel EXPÉRIMENTAL.** Base V1.176, stable main V1.173 inchangée. Aucune installation et aucune écriture dans le véritable Engine.ini pendant le développement.

Dans Camera > Rendering quality (restart required), sept options Native par défaut : anisotropie 1–16, résolution interne 25–200 %, ombres qualité 0–5 / taille maximale 64–8192 / distance 0–5, visibilité 0–5, SSR qualité 0–4. Les bornes sont des plages d'essai, pas des maxima universels. Tous les anciens réglages et le catalogue V1.176 restent conservés.

Le helper existant applique les choix au fichier Engine.ini après fermeture complète ; ils prennent effet au lancement suivant. Retour Native également après fermeture/redémarrage : restauration de la valeur antérieure seulement si elle appartient encore au mod. Une édition externe est préservée. Sauvegarde .DGE-render.bak et journal .DGE-render-owned.json ; ne pas effacer ce journal avant restauration. Aucune nouvelle boucle/hook en jeu ; les modifications de ces préférences sont sauvegardées immédiatement depuis le menu. [Détails et retour arrière](V1_177_REVIEW.md).

- [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45006971348) : **86/86 tests, zéro avertissement/erreur C++**, Windows x64 Release. Tests helper sur fichiers temporaires UTF8/UTF16 : garde processus fermé, sept CVars, sauvegarde exacte, idempotence, Native avec clé absente/existante, édition externe, valeurs invalides, journal corrompu et rollback après interruption. Ce ne sont pas des essais du rendu réel.
- Premier run45005960013 : compilation réussie, test helper échoué lors de la suppression finale du dossier (fixtures non retirées). Nettoyage ciblé corrigé, aucun code de production changé entre les deux runs et aucun ZIP du run échoué.
- Source `f5417d74296e018cdce2a065c2828e4d28feba90` ; archive `4d21fadeac379915e5c024544255e24fc5ac9ff5`.
- [ZIP direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/4d21fadeac379915e5c024544255e24fc5ac9ff5/downloads/DarksidersGenesis_V1.177_RENDER_QUALITY_RESTART_TEST.zip) : 2509550 octets, SHA256 `0547ABE0BCE35853BF5DAFD989346377E5AEE25FCE6A829B56864696A1B54F4E`.
- Deux binaires AMD64 à la racine sans INI/README, CRC/empreintes/blob distant/lien HTTP200 vérifiés. Defender ZIP et binaires : aucune menace détectée, protections intactes. 241 anciennes archives préservées, 242 cumulées. Un seul nouveau ZIP récupéré par Git, aucune ancienne archive téléchargée.
- 279 anciennes valeurs de référence identiques ; sept Native ajoutées, total 286 / 12 sections. INI personnel, raccourcis, main/archive et tag stable conservés. Aucun build local, aucune installation automatique.

## Test court
Installer manuellement en conservant l'INI ou passer par l'updater debug. Essayer d'abord la résolution interne à 75 %, fermer complètement et relancer dans la même scène ; remettre Native et refaire ce cycle pour vérifier le retour. Ensuite anisotropie 16 sur sol vu en biais, ombres à 2 / 1024 / 0.5, visibilité 0.5, SSR 0, par comparaison adaptée à chaque effet. Les limites des textures/LOD/passes du jeu peuvent restreindre certains résultats. Aucun gain GPU garanti.

Pour revenir aux anciens binaires : remettre les nouveaux contrôles Native et fermer le jeu avant remplacement ; le changement d'ASI seul n'annule pas des valeurs Engine.ini déjà appliquées. [Preuves](evidence/V1_177_DELIVERY_CHECK.json).
