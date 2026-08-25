# Optimisation du démarrage de worldserver

Mesuré le 2026-08-25 sur ce dépôt : **7 min 13 s → 1 min 47 s** (-75 %) jusqu'à
`World initialized in ...`, sans erreur nouvelle. Détail des postes et des
pièges de mesure : mémoire projet `bfacore_demarrage_optimisation.md`.

Ce correctif combine des changements versionnés dans ce dépôt (code C++,
migrations SQL) et des changements **hors dépôt**, sur l'installation MySQL
et le fichier `worldserver.conf` de chaque machine — ceux-là doivent être
refaits manuellement sur toute nouvelle installation ou tout autre poste de
build.

## 1. Migrations SQL (automatiques)

Trois fichiers sous `sql/updates/` :

- `sql/updates/world/2026_08_25_world_convert_myisam_to_innodb.sql`
  — convertit toutes les tables encore en MyISAM de `bfa_world` vers InnoDB.
  **Le plus gros du gain** (loot, localisation) vient de là. Idempotent.
- `sql/updates/world/2026_08_25_world_scriptname_indexes.sql`
  — index sur `ScriptName` pour `LoadScriptNames`. Idempotent.
- `sql/updates/hotfixes/2026_08_25_hotfixes_index_locale_tables.sql`
  — index `(locale, VerifiedBuild)` sur les tables `*_locale` de la base hotfixes.
  Idempotent.

Ces trois fichiers s'appliquent tout seuls au prochain démarrage de
`worldserver.exe` si l'auto-updater est actif (`Updates.AutoSetup = 1` dans
`worldserver.conf`, actif par défaut). Pour les appliquer à la main :

```bash
mysql -uroot -p bfa_world    < sql/updates/world/2026_08_25_world_convert_myisam_to_innodb.sql
mysql -uroot -p bfa_world    < sql/updates/world/2026_08_25_world_scriptname_indexes.sql
mysql -uroot -p bfa_hotfixes < sql/updates/hotfixes/2026_08_25_hotfixes_index_locale_tables.sql
```

**Avant la conversion InnoDB sur un serveur en production**, sauvegarder
`bfa_world` (MyISAM ne supporte pas les snapshots transactionnels, d'où
`--single-transaction=false`) :

```bash
mysqldump -uroot -p --single-transaction=false --quick bfa_world > bfa_world_backup.sql
```

## 2. Config `worldserver.conf` (manuel, par poste)

Nouvelle option `LoadLocales` (voir `worldserver.conf.dist`) : restreint les
locales chargées en mémoire à celles réellement servies par le realm. Vide
par défaut (comportement inchangé — toutes les locales trouvées dans `dbc/`).

```
LoadLocales = "enUS frFR"
```

Adapter la liste aux locales effectivement utilisées. Ne pas oublier la
locale par défaut (`DBC.Locale`) — elle est de toute façon toujours chargée
même si absente de cette liste.

## 3. `innodb_buffer_pool_size` (manuel, admin, par machine)

Le `my.ini` de l'installation MySQL locale (pas dans ce dépôt) est resté au
défaut usine `128M`, insuffisant pour les ~1,5 Go de données BfA. Passé à
`4G` le 2026-08-25 :

- `C:\ProgramData\MySQL\MySQL Server 8.4\my.ini`, section `[mysqld]`
- `innodb_buffer_pool_size=128M` → `innodb_buffer_pool_size=4G`
- Nécessite les droits administrateur pour éditer ce fichier.
- `SET GLOBAL innodb_buffer_pool_size = ...` change la valeur à chaud sans
  redémarrer MySQL, mais **ne persiste pas** — sans l'édition du fichier,
  la valeur retombe à 128M au prochain redémarrage du service MySQL.
- Ajuster à la RAM disponible de la machine cible plutôt que recopier 4G
  aveuglément ; viser un peu au-dessus de la taille totale des bases BfA
  (`SELECT SUM(data_length+index_length) FROM information_schema.tables
  WHERE table_schema IN ('bfa_world','bfa_hotfixes','bfa_characters','bfa_auth')`).

## Code modifié (dans ce dépôt, s'applique tout seul à la recompilation)

- `src/server/shared/DataStores/DB2Store.cpp` — évite de parser un fichier
  `.db2` en entier quand sa structure n'a aucune chaîne localisée.
- `src/server/game/DataStores/DB2Stores.cpp` — option `LoadLocales` pour les
  DB2 (fichiers + hotfix DB) + instrumentation de démarrage (niveau INFO :
  `DB2 load breakdown`, `DB2 profile <store> [<locale>]` pour tout store
  prenant plus de 200 ms).
- `src/server/shared/DataStores/DB2DatabaseLoader.cpp` — même instrumentation
  côté requêtes/hotfix.
- `src/server/game/Globals/ObjectMgr.cpp` — applique `LoadLocales` aux 10
  chargeurs de tables `*_locale` du monde (créatures, gameobjects, quêtes,
  pages, gossip, points d'intérêt).
