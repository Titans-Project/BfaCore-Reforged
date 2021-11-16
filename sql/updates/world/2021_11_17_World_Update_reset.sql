/*
**************************
*    BfaCore Reforged    *
**************************
*/

DROP TABLE `updates`;

CREATE TABLE `updates` (
	`name` VARCHAR(200) NOT NULL COMMENT 'filename with extension of the update.' COLLATE 'utf8_general_ci',
	`hash` CHAR(40) NULL DEFAULT '' COMMENT 'sha1 hash of the sql file.' COLLATE 'utf8_general_ci',
	`state` ENUM('RELEASED','ARCHIVED') NOT NULL DEFAULT 'RELEASED' COMMENT 'defines if an update is released or archived.' COLLATE 'utf8_general_ci',
	`timestamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'timestamp when the query was applied.',
	`speed` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'time the query takes to apply in ms.',
	PRIMARY KEY (`name`) USING BTREE
)
COMMENT='List of all applied updates in this database.'
COLLATE='utf8_general_ci'
ENGINE=MyISAM
;
