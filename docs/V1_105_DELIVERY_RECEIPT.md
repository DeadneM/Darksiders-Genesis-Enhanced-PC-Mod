# V1.105 - archive intermediaire controlee

CONSTRUITE / CONTROLEE : [60/60 suites Windows x64](https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/actions/runs/40216282794), zero erreur et **un avertissement MSVC C6262** dans CopyLaterSounds : 18 828 octets de pile. [ZIP immuable conserve](https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/f0262b09301e903ef89f5be2ab4964f717aa0ce0/downloads/DarksidersGenesis_V1.105_RAMPAGE_NATIVE_RATE_FOREIGN_AUDIO_TEST.zip). NON VALIDE EN JEU.

Source `12dbb75bf1e1cbdbeac8d389054e0887d5edec1a`, archive `f0262b09301e903ef89f5be2ab4964f717aa0ce0`, SHA256 `ACE0FECDF412BB12290CC5760401BE1220D14F10B3FBF0A9639C0A8AF6C5FE25`, 1001523 octets. Quatre fichiers racine, CRC/AMD64, README/INI de la source compilee et HTTP 200 compares ; 169 archives precedentes intactes. 169 cles INI et 55 ID Hotkeys conserves.

La piste audio utilisait deux plans temporaires : Sounds et Presentation. V1.106 compacte le plan existant en place, sans ajouter d'etat, de timer ou de comportement. Nouveau numero pour ne jamais remplacer cette archive. L'execution du pouvoir et l'audibilite restent non validees. Aucune compilation ou installation locale. [Contrat V1.105](V1_105_RAMPAGE_NATIVE_RATE_FOREIGN_AUDIO_TEST.md).
