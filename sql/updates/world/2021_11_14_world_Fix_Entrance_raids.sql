/*
**************************
*    BfaCore Reforged    *
**************************
*/ 

/* Uldir : Entrance + Exit */ 
REPLACE INTO `areatrigger` (`guid`, `id`, `map_id`, `spawn_mask`, `position_x`, `position_y`, `position_z`, `ScriptName`) VALUES ('111', '100007', '1642', '1', '1331', '618', '-168', '');
REPLACE INTO `areatrigger_teleport` (`ID`, `PortLocID`, `Name`) VALUES ('-111', '6552', 'Uldir - Entrance Target');
REPLACE INTO `areatrigger_teleport` (`ID`, `PortLocID`, `Name`) VALUES ('-112', '6551', 'Uldir - Exit Target');
REPLACE INTO `areatrigger` (`guid`, `id`, `map_id`, `spawn_mask`, `position_x`, `position_y`, `position_z`, `ScriptName`) VALUES ('112', '100007', '1861', '245760', '-374', '-255', '1113', '');

/* Argus (Raid) : Entrance */
REPLACE INTO `areatrigger` (`guid`, `id`, `map_id`, `spawn_mask`, `position_x`, `position_y`, `position_z`, `ScriptName`) VALUES ('79', '100007', '1669', '1', '-3207.5', '9414.35', '-174.367', '');
REPLACE INTO `areatrigger_teleport` (`ID`, `PortLocID`, `Name`) VALUES ('-110', '6161', '7.3 Argus Raid - Entrance');

/* TheMotherlode : npc_mine_cart_13677 */
UPDATE `creature_template` SET `ScriptName`='npc_mine_cart_136776' WHERE (`entry`='136776');

