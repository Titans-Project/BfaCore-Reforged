/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Speeds up ObjectMgr::LoadScriptNames at server startup.
--
-- That loader runs a UNION of "SELECT DISTINCT(ScriptName) FROM <table>" over
-- 21 tables. None of them had an index on ScriptName, so the biggest ones
-- (creature: 464k rows, gameobject: 171k, creature_template: 140k) were fully
-- scanned and deduplicated through a temporary table - measured at ~16 seconds
-- of the startup. With an index, DISTINCT becomes a loose index scan
-- (measured: 14740 ms -> 155 ms on the three biggest tables).
-- These tables are only written by content updates, so the extra index has no
-- runtime write cost. Idempotent: existing indexes are kept.

DROP PROCEDURE IF EXISTS `add_scriptname_indexes`;

DELIMITER $$
CREATE PROCEDURE `add_scriptname_indexes`()
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE tbl VARCHAR(64);
    DECLARE cur CURSOR FOR
        SELECT c.TABLE_NAME
        FROM information_schema.COLUMNS c
        WHERE c.TABLE_SCHEMA = DATABASE()
          AND c.COLUMN_NAME = 'ScriptName'
          AND c.TABLE_NAME IN ('creature', 'gameobject', 'creature_template', 'gameobject_template', 'quest_template_addon')
          AND NOT EXISTS (SELECT 1 FROM information_schema.STATISTICS s
                          WHERE s.TABLE_SCHEMA = c.TABLE_SCHEMA AND s.TABLE_NAME = c.TABLE_NAME
                            AND s.INDEX_NAME = 'idx_scriptname');
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

    OPEN cur;
    read_loop: LOOP
        FETCH cur INTO tbl;
        IF done THEN
            LEAVE read_loop;
        END IF;

        SET @ddl = CONCAT('ALTER TABLE `', tbl, '` ADD INDEX `idx_scriptname` (`ScriptName`)');
        PREPARE stmt FROM @ddl;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END LOOP;
    CLOSE cur;
END$$
DELIMITER ;

CALL `add_scriptname_indexes`();
DROP PROCEDURE `add_scriptname_indexes`;
