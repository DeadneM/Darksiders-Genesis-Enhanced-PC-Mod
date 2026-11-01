# Statique : verrouillage et comparaison sans ASI

1er novembre 2026. **DIAGNOSTIC CONTRÔLÉ ; réparation Statique non acquise.** Aucun changement C++ ni nouveau build. V1.138 installée manuellement et identifiée par le journal et les deux empreintes du package.

## Résultat du nouveau relevé

Le code natif possède deux cibles distinctes : le verrouillage forcé (`E90`) et la sélection automatique (`E98`). Le getter `66A5B0` donne priorité au premier, puis au second. Le dispatcher natif active/efface le premier par les actions 62/63 ; ce sont des fonctions réaffectables, pas une raison de corriger une touche particulière.

Le relevé utile comporte **5 290 échantillons sur 538,23 secondes**, dont 147 rejetés lors de transitions/lectures non valides et 610 concernant War hors périmètre. **475 échantillons de faisceau, 185 avec cible affectée ; aucun verrouillage forcé dans les échantillons Strife valides.** Toutes les cibles affectées observées correspondent à la sélection automatique native. Le verrouillage forcé n'explique donc pas ces tirs ; ne pas ajouter un effacement forcé ou un nouveau hook pour cette hypothèse.

Le premier relevé de trois minutes avait manqué les tirs : Strife au repos puis monture. Ses absences de cible ne sont pas une preuve. La capture prolongée a correctement repris le personnage après son remplacement. Les fichiers détaillés restent privés ; [agrégats et limites](evidence/STATIC_HARDLOCK_BASELINE_2026_11_01.json).

L'utilisateur confirme le même défaut en « Vanilla Aim » et avec 3D aiming ON/OFF. La capture inclut aussi la vue native, **toujours avec ASI chargé** : ce n'est pas un test sans mod. Les profils commun/Statique lus hors appel restent `7500 / 90 / 45 / 0.75 / 1`. Le multiplicateur de l'EXE est `pi/360` : le champ horizontal 45 désigne un cône total de 45°, soit ±22,5°. La restauration transactionnelle des angles impose de ne pas interpréter une lecture hors appel comme un échec du curseur.

Les positions/directions ont changé entre les essais. Les proportions de cibles ne mesurent pas la force de l'aide ; aucun résultat d'impact n'est déduit des seules directions. Les essais 0/26 % du dernier passage n'ont pas enregistré de faisceau ; 50 % en a enregistré 55 dont 8 affectés, sans constituer un A/B contrôlé.

## Changement d'approche

Les corrections de trajectoire TPS sont refusées par leurs fournisseurs de rayon en mode « Vanilla Aim ». L'ajustement OFF appelle directement le sélecteur original. Comme le défaut persiste dans ces conditions et dans la vue native, la prochaine séparation utile est **mod complètement déchargé / mod chargé**. Ne pas livrer un nouveau filtre de contacts : les précédents ont cassé la portée et les chaînes.

Limitation à garder pour la suite des curseurs : en mode par munition, le profil commun DualPistols reste à 100 %. Cela limite la couverture de ce mode sur une sélection issue du profil commun ; ce point n'explique pas les essais ajustement OFF et ne doit pas être présenté comme la cause initiale.

## Installation temporaire pour le test natif

Après demande et fermeture vérifiée du jeu, **ASI et `dxgi.dll` V1.138 ont été sauvegardés et renommés temporairement**. Leur empreinte reste celle du package ; aucun fichier remplacé, aucun lancement par l'agent. INI, raccourcis, sauvegardes et PAK conservés. Le chargement du mod est donc désactivé jusqu'à restauration.

Le test demandé : même munition Statique, même caisse/pot et ennemi latéral, observer si l'attraction est identique, plus faible ou absente. Après fermeture du jeu, rétablir **exactement les deux fichiers V1.138** avec contrôle de leurs empreintes ; ne jamais restaurer automatiquement l'INI de sauvegarde. Les détails et l'état sont dans le reçu privé `audit-v131/native-baseline-backups/V1.138-20261101-121109/`.

L'inventaire récursif ne trouve aucun PAK supplémentaire ; le contenu intégral du PAK principal n'a pas été revérifié contre Steam. Le résultat sans ASI est **en attente**, pas annoncé validé. Les branches main/archive, anciennes archives et le défaut intermittent Stoneskin restent inchangés.
