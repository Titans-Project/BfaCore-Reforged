ALTER TABLE `characters` 
ADD COLUMN `camp_x` int(10) NULL DEFAULT NULL AFTER `numRespecs`,
ADD COLUMN `camp_y` int(10) NULL DEFAULT NULL AFTER `camp_x`,
ADD COLUMN `camp_z` int(10) NULL DEFAULT NULL AFTER `camp_y`,
ADD COLUMN `camp_o` int(10) NULL DEFAULT NULL AFTER `camp_z`,
ADD COLUMN `camp_mapid` int(10) NULL DEFAULT NULL AFTER `camp_o`;