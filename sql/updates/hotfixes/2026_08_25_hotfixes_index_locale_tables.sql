/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Speeds up DB2 data store initialization at server startup.
--
-- DB2DatabaseLoader::LoadStrings runs, for every store that has localized string
-- fields and for every installed locale:
--     SELECT ... FROM <table>_locale WHERE (`VerifiedBuild` > 0) = ? AND locale = ?
--
-- The only index on these tables is the primary key, which starts with `ID`, so
-- `locale` could not be resolved by an index and every one of those queries ran a
-- full table scan. On broadcast_text_locale (1.9M rows / 330 MB) each scan took
-- ~8 seconds, and the loader issues 20 of them (10 locales x 2 for the custom
-- flag) - including 10 that match no row at all and scanned the whole table to
-- return nothing.
--
-- Indexing (locale, VerifiedBuild) lets MySQL resolve both predicates from the
-- index (`Using index condition`). Measured on broadcast_text_locale:
-- 8506 ms -> 254 ms per query.
--
-- Applied to every *_locale table that has both columns, so future hotfix data
-- growth cannot reintroduce the problem. Idempotent: existing indexes are kept.

DROP PROCEDURE IF EXISTS `add_locale_indexes`;

DELIMITER $$
CREATE PROCEDURE `add_locale_indexes`()
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE tbl VARCHAR(64);
    DECLARE cur CURSOR FOR
        SELECT t.TABLE_NAME
        FROM information_schema.TABLES t
        WHERE t.TABLE_SCHEMA = DATABASE()
          AND t.TABLE_NAME LIKE '%\_locale'
          AND EXISTS (SELECT 1 FROM information_schema.COLUMNS c
                      WHERE c.TABLE_SCHEMA = t.TABLE_SCHEMA AND c.TABLE_NAME = t.TABLE_NAME
                        AND c.COLUMN_NAME = 'locale')
          AND EXISTS (SELECT 1 FROM information_schema.COLUMNS c
                      WHERE c.TABLE_SCHEMA = t.TABLE_SCHEMA AND c.TABLE_NAME = t.TABLE_NAME
                        AND c.COLUMN_NAME = 'VerifiedBuild')
          AND NOT EXISTS (SELECT 1 FROM information_schema.STATISTICS s
                          WHERE s.TABLE_SCHEMA = t.TABLE_SCHEMA AND s.TABLE_NAME = t.TABLE_NAME
                            AND s.INDEX_NAME = 'idx_locale_verified');
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

    OPEN cur;
    read_loop: LOOP
        FETCH cur INTO tbl;
        IF done THEN
            LEAVE read_loop;
        END IF;

        SET @ddl = CONCAT('ALTER TABLE `', tbl, '` ADD INDEX `idx_locale_verified` (`locale`, `VerifiedBuild`)');
        PREPARE stmt FROM @ddl;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END LOOP;
    CLOSE cur;
END$$
DELIMITER ;

CALL `add_locale_indexes`();
DROP PROCEDURE `add_locale_indexes`;
