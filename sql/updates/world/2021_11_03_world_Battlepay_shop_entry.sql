/*
**************************
*    BfaCore Reforged    *
**************************
*/

ALTER TABLE `battlepay_shop_entry`
	CHANGE COLUMN `EntryID` `EntryID` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT FIRST,
	CHANGE COLUMN `VasServiceType` `Flags` INT(11) NULL DEFAULT '0' AFTER `Ordering`,
	CHANGE COLUMN `StoreDeliveryType` `BannerType` INT(11) NULL DEFAULT NULL AFTER `Flags`,
	ADD PRIMARY KEY (`EntryID`);