/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Converts every remaining MyISAM table in the world database to InnoDB.
--
-- The stock BfaCore world dump ships every table as MyISAM. On this setup that
-- engine scanned roughly 8x slower than InnoDB and, unlike InnoDB, does not
-- benefit at all from innodb_buffer_pool_size - it always reads through the OS
-- file cache. That made it the single largest contributor to worldserver
-- startup time (creature loot templates: 51.6s -> 7.8s after conversion;
-- creature_template_locale: 31.8s -> 4.2s; full "World initialized" time:
-- 7m13s -> 1m47s together with the other startup fixes in this same batch -
-- see sql/updates/hotfixes/2026_08_25_hotfixes_index_locale_tables.sql and
-- sql/updates/world/2026_08_25_world_scriptname_indexes.sql).
--
-- ROW_FORMAT=DYNAMIC is forced because some MyISAM tables were created with
-- ROW_FORMAT=FIXED, which InnoDB does not support (errors out with
-- "Table storage engine doesn't have this option" otherwise).
-- sql_mode is relaxed for the duration of this script because a handful of
-- legacy columns have zero-date/invalid defaults that MyISAM tolerated but
-- InnoDB's stricter default sql_mode rejects on ALTER TABLE.
--
-- Idempotent: only tables still on MyISAM are touched, so re-running this
-- file (or applying it to a database that was already converted) is a no-op.
--
-- Not undone automatically: back up the world database before applying this
-- to a production server, e.g.
--   mysqldump -uroot -p --single-transaction=false --quick bfa_world > bfa_world_backup.sql
-- (single-transaction must be off while tables are still MyISAM, which does
-- not support transactional snapshots).

SET SESSION sql_mode = '';

DROP PROCEDURE IF EXISTS `convert_myisam_to_innodb`;

DELIMITER $$
CREATE PROCEDURE `convert_myisam_to_innodb`()
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE tbl VARCHAR(64);
    DECLARE cur CURSOR FOR
        SELECT t.TABLE_NAME
        FROM information_schema.TABLES t
        WHERE t.TABLE_SCHEMA = DATABASE()
          AND t.ENGINE = 'MyISAM'
        ORDER BY t.DATA_LENGTH + t.INDEX_LENGTH;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

    OPEN cur;
    read_loop: LOOP
        FETCH cur INTO tbl;
        IF done THEN
            LEAVE read_loop;
        END IF;

        SET @ddl = CONCAT('ALTER TABLE `', tbl, '` ROW_FORMAT=DYNAMIC ENGINE=InnoDB');
        PREPARE stmt FROM @ddl;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END LOOP;
    CLOSE cur;
END$$
DELIMITER ;

CALL `convert_myisam_to_innodb`();
DROP PROCEDURE `convert_myisam_to_innodb`;
