# V1.133 - visibilite de l'aide TPS

CONSTRUITE / CONTROLEE, EXPERIMENTALE ; NON VALIDEE EN JEU. Base acceptee V1.130. Le retour V1.131 et la remise finale du curseur a zero ne constituent pas une validation de l'aide positive. Aucune compilation locale ni lancement du jeu ; installation ensuite demandee et verifiee le 30 octobre 2026.

## Livraison verifiee

Source `c43d937f1602bdcc7bd23d97eb93bfaf80449836` ; archive `7f3b9ec2cc71ef997543cfe5d2751a867190db6f`. [GitHub Actions Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40959653744) : 68/68 suites, zero avertissement/erreur MSVC. [ZIP cumulatif quatre fichiers a la racine](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/7f3b9ec2cc71ef997543cfe5d2751a867190db6f/downloads/DarksidersGenesis_V1.133_AIM_ASSIST_TARGET_DEPTH_TEST.zip), 2426622 octets, SHA256 `EABB728BF99CB83A158A6F835D46D710494ECB7810F3B3479D75D1FE56F4E833`.197 anciens ZIP Git identiques,198 au total. CRC/methode0, AMD64, INI/README compares a la source compilee. Updater debug selectionne et telecharge cette meme archive. Defender ZIP et contenu sans menace, protections actives, aucun reglage modifie.

## Correction et limites

V1.131 exigeait un hit BLOQUANT sur l'ennemi pour autoriser l'aide. Le canal natif Projectile peut traverser Pawn/CharacterMesh, donc cette exigence peut refuser une cible pourtant visible. V1.133 controle plutot l'absence d'une surface bloquante avant cette cible. Une requete indisponible ou une identite expiree reste refusee. Le point vise garde aussi la profondeur finie de l'ennemi, plutot que le decor/ciel derriere lui ; cela sert a la convergence depuis le canon et au solveur Lava. La couverture plus proche du rayon partiel reste prioritaire. Ce changement corrige une incoherence prouvee dans le contrat natif ; la derniere session ne prouve pas que c'etait son seul blocage. Le journal compte maintenant les valeurs utilisees et les raisons de refus. Il ne mesure pas les impacts.

Cone6degres, priorite du rayon original aux collisions proches, interpolation et contact Statique V1.130 inchanges. Pas de nouveau hook, canal/profil modifie ou scan d'acteurs.227 valeurs INI et55ID Hotkeys, controles/cameras/poses UI, modeles et physique Lava conserves. [Deux passes comparees et preuve native](evidence/V1_133_STATIC_REVIEW.json), [contrat et essai](V1_133_AIM_ASSIST_TARGET_DEPTH_TEST.md).

## A tester

Garder son INI. Global0/50/100 sur un ennemi juste a cote du centre, d'abord Standard et Statique, puis autres munitions et Hotstreak. Per ammunition : un type0 et un type100. Tester l'absence d'attraction par un ennemi tres lateral ou derriere un obstacle, puis retour0 et main vide pause/roues. Fermer normalement le jeu pour ecrire les compteurs. Coop en dernier. V1.133 ensuite installee sur demande, INI personnel conserve. Rollback zero ou archive V1.130 validee. main/archive et profil personnel preserves.

[CI](evidence/V1_133_CI_CHECK.json), [ZIP](evidence/V1_133_PACKAGE_CHECK.json), [updater](evidence/V1_133_UPDATER_CHECK.json), [scan](evidence/V1_133_DEFENDER_CHECK.json).

## Installation locale demandee le 30 octobre 2026

V1.133 installee depuis le ZIP controle, jeu ferme, apres sauvegarde V1.131 `before-V1.133-20261030-194838-8270be80`. ASI/DXGI/README remplaces puis relus ; anciennes copies ASI/DXGI/README/INI/journal sauvegardees et verifiees. INI personnel identique octet pour octet et raccourcis conserves, EXE original identique, aucun PAK ecrit (noms/tailles/dates UTC inchanges). Jeu non lance ; l'aide reste EXPERIMENTALE, a valider par comparaison Global0/100 puis lecture du journal. [Preuve](evidence/V1_133_INSTALL_CHECK.json).
