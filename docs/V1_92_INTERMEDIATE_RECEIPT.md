# V1.92 — intermédiaire conservé, remplacé par V1.93

Run 40057951291, job 120765002088 : étapes compilation, 54 suites, package et archivage réussies ; conclusion globale cancelled, annulation arrivée après publication. Sources réellement compilées **616105597138f4a7012b2c48bd1be6ff6a14369e**, confirmées dans le log de checkout. Archive publiée au commit **13e70e35a81d32c607e97076142cdd5dd5a23751**, dont le parent dac4d9d ne doit pas être confondu avec les sources compilées du ZIP.

Cette archive précède la correction de réentrance par instance dac4d9d. Elle n’est pas le candidat final ; V1.93 inclut cette correction et empêche désormais la publication d’un ZIP si les sources ont changé pendant la compilation. V1.92 reste immuable, aucun résultat en jeu supposé. [Mesures et SHA256](evidence/V1_92_INTERMEDIATE_CHECKS.json). [Candidat final et essais différés](V1_93_DELIVERY_RECEIPT.md).
