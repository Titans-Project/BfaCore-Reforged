/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Fix Captain Rodgers (66292)
UPDATE `gossip_menu_option` SET `OptionText`='I am ready to depart.' WHERE (`MenuId`='66292') AND (`OptionIndex`='0');
UPDATE `creature_template` SET `ScriptName`='npc_captain_rodgers_66292' WHERE (`entry`='66292');
DELETE FROM smart_scripts WHERE entryorguid = 66292;

-- Spawn World Boss Humongris (108879)
DELETE FROM `creature` WHERE (`id` = 108879);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(3530000000003055, 108879, 1220, 0, 0, '0', 0, 0, 0, -1, 0, 0, 2435.59, 8254.55, 44.3088, 2.99236, 300, 0, 0, 10740, 0, 0, 0, 0, 0, 0, 0, '', 0);

-- Fix Quest : Decontamination (27635)
UPDATE `quest_template` SET `QuestType`='0' WHERE (`ID`='27635');

-- Fix Brother Alen (133951)
UPDATE `creature_template` SET `unit_flags`='0', `unit_flags2`='0', `unit_flags3`='0' WHERE (`entry`='133951');

-- Fix General_nazgrim (55054)
UPDATE `creature_template` SET `gossip_menu_id`='14988', `ScriptName`='npc_general_nazgrim_55054' WHERE (`entry`='55054');
REPLACE INTO `gossip_menu` (`MenuId`, `TextId`, `VerifiedBuild`) VALUES ('14988', '14988', '0');
REPLACE INTO `gossip_menu_option` (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild`) VALUES ('14988', '0', '0', 'I\'m ready to go, General.', '0', '0', '3', '0');
UPDATE `gossip_menu_option` SET `OptionType`='1', `OptionNpcFlag`='1' WHERE (`MenuId`='14988') AND (`OptionIndex`='0');
