# V1.182 publiée sur debug

Base V1.181. La correspondance croisée des holsters à cheval a été validée par le joueur. Les corrections de transition, de lames rangées et de HDR de V1.182 restent expérimentales en jeu. [Diagnostic et changements](V1_182_REVIEW.md).

**Construite et contrôlée** sur Windows x64 Release. Les 86 tests passent, sans avertissement C++. La première suite a décelé une protection des pistolets trop large lorsqu’aucune monture n’était identifiée. Le périmètre a été resserré et la seconde suite couvre ce cas séparément. [GitHub Actions](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/45113579838).

Le ZIP vérifié contient les deux binaires à la racine. Les 246 archives précédentes et les 296 valeurs de référence sont conservées. Defender n’a rien détecté sur le ZIP ni sur les binaires. Source \`67c3fa0611053ae8b5bed7ae53775130f103863b\`, archive \`0233ee744d70fb298bf3439883dcba4f1597f9ba\`, SHA-256 \`CAAE52EEE76013F2EB858729169FA5B517B628B805D7478341E96314902D691C\`. [Téléchargement direct](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/0233ee744d70fb298bf3439883dcba4f1597f9ba/downloads/DarksidersGenesis_V1.182_HDR_STARTUP_STOWED_BLADES_TEST.zip). [Contrôles détaillés](evidence/V1_182_DELIVERY_CHECK.json).

## Essai groupé

1. Monter et descendre avec un pistolet masqué sur les deux chevaux de Strife. Le pistolet masqué doit rester invisible et le holster visible doit garder son côté.
2. Cocher **Show Strife blades when sheathed** et regarder les lames au repos. Vérifier les côtés en masquant une lame et le retour à l’état rangé après la mêlée à pied. Le journal \`Stowed blades V1.182\` indique si la copie a été attachée et enregistrée.
3. Choisir **HDR10 1000** avec **Match HDR profile**, puis fermer et relancer. Avec un ancien buffer forcé, le premier cycle applique le format de sortie. Au nouveau lancement, comparer les couleurs et les hautes lumières. Les lignes attendues sont un buffer 10 bits, sortie moteur 3, gamut 2, HDR ON et signalisation acceptée. En scRGB 1000, attendre un format flottant 16 bits, sortie 5 et gamut 0.

Le placement des lames, la disparition du pistolet fantôme et le rendu HDR demandent l’essai du joueur. Les lames tenues pendant la mêlée à cheval restent à étudier. Pour les réglages graphiques en direct, les lectures natives de la résolution interne et des reflets sont repérées. L’anisotropie demande une vérification du renouvellement des samplers. Aucune installation. Pour revenir à une version antérieure, choisir HDR Native et fermer le jeu avant de remplacer les binaires.
