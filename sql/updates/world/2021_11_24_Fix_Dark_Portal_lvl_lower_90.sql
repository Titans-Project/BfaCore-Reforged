/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Blasted Lands - Dark Portal to Hellfire Peninsula teleport (loc 7352 map 530)
-- Hellfire Peninsula - Dark Portal to Blasted Lands (loc 7353 map 530)

DELETE FROM `areatrigger_teleport` WHERE `id` IN (-69);
INSERT INTO `areatrigger_teleport` (`Id`,`PortLocID`,`Name`) VALUES
(-69,7352,'Dark Portal to Hellfire Peninsula');

DELETE FROM `areatrigger` WHERE `guid` IN (69);
INSERT INTO `areatrigger` VALUES
(69,100007,0,1,-11905.86,-3208.1584,-14.8805,'');

DELETE FROM `areatrigger_teleport` WHERE `id` IN (-70);
INSERT INTO `areatrigger_teleport` (`Id`,`PortLocID`,`Name`) VALUES
(-70,7348,'Dark Portal to Blasted Lands');

DELETE FROM `areatrigger` WHERE `guid` IN (70);
INSERT INTO `areatrigger` VALUES
(70,100007,530,1,-246.4892,895.9616,84.3497,'');
