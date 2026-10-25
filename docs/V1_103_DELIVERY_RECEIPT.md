# V1.103 - reception lourde par interruption native

**CONSTRUITE / CONTROLEE : [57/57 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40199153318), zero avertissement/erreur MSVC. [ZIP cumulatif immuable verifie](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/74cb774b480f1cfb5cd2ed6e8d8a15ccd0fd39ec/downloads/DarksidersGenesis_V1.103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.zip). Recuperation EXPERIMENTALE, non validee en jeu.**

Base `d123f417119e763724713d9859bb3e802b32cac6` : V1.102 Stoneskin et police approuves par l'utilisateur. Geyser/Clone/Caltrops V1.98 et World Ender War V1.100 conserves. Source compilee `100e2c34b3e108f93894ba1e1d18205bb65b097b` ; archive `74cb774b480f1cfb5cd2ed6e8d8a15ccd0fd39ec`.

L'ancienne ecriture de AllowPlayerMovement et de l'action MOVE est retiree. Le candidat appelle uniquement la demande native d'interruption MOVE de la reception lourde, apres l'impact et un sol stable. Le moteur decide de la sortie a la demande de mouvement ; pas de Finish force, nouvelle animation, touche imposee ou nouveau hook. La case existante reste OFF. Son descriptif precise la fin de recuperation native ; aucune nouvelle cle.

Verification native hors ligne : enum MOVE=1, NotifyBegin 0x5A85A9 vers 0x5A9D60, masques temporaire/effectif +0x120/+0xB0 et reconciliation 0x5B4739 vers setter 0x599310 ; signatures comparees a l'EXE exact. Fenetres d'origine Strife 0,668465 s et War 0,402459 s, effets d'impact anterieurs au seuil 0,15 s du candidat. Double revue du chemin natif puis des proprietaires/restaurations : l'ancien correctif de permissions ne reste pas superpose.

- 57 suites Windows reussies ; contrats HeavyLanding et conservation OFF/INI, proprietaires, package/rate, impact/sol, autres pouvoirs, fenetres natives, fin, instance recyclee, pause/roues/cinema/focus verifies par fixtures. Cela ne valide pas le mouvement/rendu du moteur en jeu.
- 191 autres fichiers source/tests/config compares a la base ; pas de changement des capacites validees, de la visee/camera, des entrees souris/XInput, des roues, de horse, des holsters, du flou ou de Stinger.
- 169 cles INI et 55 ID Hotkeys conserves ; 167 anciens ZIP compares par SHA256, 168 archives au total. main/archive/v1.43 verifies intacts.
- ZIP 976150 octets, SHA256 `2FF84AFDB1C505E218EC2AD7EAEBCB64B4325180C08E70B451F75C60E1D962F8` ; quatre fichiers racine, CRC valide, ASI/dxgi AMD64, README/INI compares a la source compilee. Lien HTTP 200 et octets identiques verifies.

Installation manuelle jeu ferme, conserver INI/raccourcis/PAK/sauvegardes. Aucun lancement ni installation du jeu par l'agent. [Contrat, limites, essais et repli](V1_103_HEAVY_LANDING_NATIVE_INTERRUPT_TEST.md).

Essais : Strife puis War, meme chute assez haute en native et TPS, OFF/ON ; maintenir le deplacement pour comparer la reprise ; sans deplacement conserver l'impact/animation ; seconde chute, puis dash et attaque. Remettre OFF en cas d'anomalie. Repli V1.102 avec la case OFF. Les temps compares reposent sur l'ability et les assets/rate d'origine ; leur synchronisation effective et le blend restent a tester.

Rampage Strife reste retire. Aucune erreur de package/squelette/DefaultSlot identifiee hors ligne ; l'etat exact au refus ancien n'est pas capture. READY sans CAST ne prouve pas un lancement. La prochaine etape est une capture ciblee du refus, pas la reactivation du montage rejete. P2 et effets P2 restent differes.
