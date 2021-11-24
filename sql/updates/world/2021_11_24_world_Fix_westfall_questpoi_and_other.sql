/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Westfall

-- Quest POI
DELETE FROM `quest_poi` WHERE `QuestID` = '26266'; 
INSERT INTO `quest_poi`(`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (26266, 0, 0, -1, 0, 0, 0, 52, 0, 1, 0, 0, 0, 0, 35662); 
INSERT INTO `quest_poi`(`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (26266, 0, 1, 0, 265062, 57988, 0, 52, 0, 1, 0, 0, 0, 0, 35662); 
DELETE FROM `quest_poi_points` WHERE `QuestID` = '26266'; 
INSERT INTO `quest_poi_points`(`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (26266, 1, 0, -10499, 1023, 35662);
INSERT INTO `quest_poi_points`(`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (26266, 0, 0, -10112, 1042, 35662);

DELETE FROM `quest_poi` WHERE `QuestID` = '26322';
DELETE FROM `quest_poi_points` WHERE `QuestID` = '26322';
INSERT INTO `quest_poi`(`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (26322, 0, 0, -1, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 24330);
INSERT INTO `quest_poi`(`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (26322, 0, 1, 0, 0, 0, 0, 52, 0, 0, 0, 0, 0, 0, 24330);
INSERT INTO `quest_poi_points`(`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (26322, 0, 0, -8363, 232, 24330);
INSERT INTO `quest_poi_points`(`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (26322, 1, 0, -10533, 1021, 24330);
--

DELETE FROM `quest_poi` WHERE `QuestID` = '26289';
DELETE FROM `quest_poi_points` WHERE `QuestID` = '26289';
INSERT INTO `quest_poi`(`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (26289, 0, 0, -1, 0, 0, 0, 39, 0, 1, 0, 0, 0, 0, 24330);
INSERT INTO `quest_poi_points`(`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (26289, 0, 0, -11042, 625, 24330);

-- Fix "Homeless Stormwind Citizen" for quest: Feeding the Hungry and the Hopeless (26271)
DELETE FROM `creature_template` WHERE `entry` IN (42386, 42384);
INSERT INTO `creature_template`(`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `femaleName`, `subname`, `TitleAlt`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `HealthScalingExpansion`, `RequiredExpansion`, `VignetteID`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `FadeRegionRadius`, `WidgetSetID`, `WidgetSetUnitConditionID`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES (42384, 0, 0, 0, 42625, 42414, 'Homeless Stormwind Citizen', '', '', NULL, '', 0, 9, 10, 0, 0, 0, 7, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 0, 0, 1, 33024, 2048, 0, 0, 0, 0, 0, 0, 7, 0, 0, 42384, 0, 0, 0, 0, 0, 0, 0, 0, 80383, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, '', 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_hungry_hobo', 35662);
INSERT INTO `creature_template`(`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `femaleName`, `subname`, `TitleAlt`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `HealthScalingExpansion`, `RequiredExpansion`, `VignetteID`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `FadeRegionRadius`, `WidgetSetID`, `WidgetSetUnitConditionID`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES (42386, 0, 0, 0, 42625, 42617, 'Homeless Stormwind Citizen', '', '', NULL, '', 0, 9, 10, 0, 0, 0, 7, 0, 1, 1.14286, 1, 0, 0, 2000, 2000, 0, 0, 1, 33024, 2048, 0, 0, 0, 0, 0, 0, 7, 0, 0, 42386, 0, 0, 0, 0, 0, 0, 0, 0, 80382, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, '', 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_hungry_hobo', 35662);
DELETE FROM `creature_template_addon` WHERE `entry` IN (42386, 42384);
DELETE FROM `creature_addon` WHERE `guid` IN (214595, 214569, 214460, 214566, 214597, 214627, 214617, 214567, 214467, 214619, 214633, 214629, 214574, 214612, 214464, 214462, 214599, 214601, 214459, 214628, 214468, 214461, 214613, 214630);

UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = '42617';

UPDATE `creature_template` SET `ScriptName` = 'npc_westplains_drifter' WHERE `entry` = '42391';


-- Fix thugs faction for quest : Lou's Parting Thoughts (26232)
UPDATE `creature_template` SET `difficulty_entry_1` = 0, `difficulty_entry_2` = 0, `difficulty_entry_3` = 0, `KillCredit1` = 42417, `KillCredit2` = 0, `name` = 'Thug', `femaleName` = '', `subname` = '', `TitleAlt` = NULL, `IconName` = '', `gossip_menu_id` = 0, `minlevel` = 11, `maxlevel` = 11, `HealthScalingExpansion` = 0, `RequiredExpansion` = 0, `VignetteID` = 0, `faction` = 1892, `npcflag` = 0, `speed_walk` = 1, `speed_run` = 1.14286, `scale` = 1, `rank` = 0, `dmgschool` = 0, `BaseAttackTime` = 2000, `RangeAttackTime` = 2000, `BaseVariance` = 0, `RangeVariance` = 0, `unit_class` = 1, `unit_flags` = 512, `unit_flags2` = 2048, `unit_flags3` = 0, `dynamicflags` = 4, `family` = 0, `trainer_type` = 0, `trainer_class` = 0, `trainer_race` = 0, `type` = 7, `type_flags` = 0, `type_flags2` = 0, `lootid` = 0, `pickpocketloot` = 0, `skinloot` = 0, `resistance1` = 0, `resistance2` = 0, `resistance3` = 0, `resistance4` = 0, `resistance5` = 0, `resistance6` = 0, `spell1` = 0, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0, `VehicleId` = 0, `mingold` = 0, `maxgold` = 0, `AIName` = '', `MovementType` = 0, `InhabitType` = 3, `HoverHeight` = 1, `HealthModifier` = 0.3, `HealthModifierExtra` = 1, `ManaModifier` = 1, `ManaModifierExtra` = 1, `ArmorModifier` = 1, `DamageModifier` = 1, `ExperienceModifier` = 1, `RacialLeader` = 0, `movementId` = 0, `FadeRegionRadius` = 0, `WidgetSetID` = 0, `WidgetSetUnitConditionID` = 0, `RegenHealth` = 1, `mechanic_immune_mask` = 0, `flags_extra` = 0, `ScriptName` = '', `VerifiedBuild` = 35662 WHERE `entry` = 42387;

-- Fix dead Two-Shoed Lou
UPDATE `creature_template` SET `difficulty_entry_1` = 0, `difficulty_entry_2` = 0, `difficulty_entry_3` = 0, `KillCredit1` = 0, `KillCredit2` = 0, `name` = 'Two-Shoed Lou', `femaleName` = '', `subname` = '', `TitleAlt` = NULL, `IconName` = '', `gossip_menu_id` = 0, `minlevel` = 12, `maxlevel` = 12, `HealthScalingExpansion` = 0, `RequiredExpansion` = 0, `VignetteID` = 0, `faction` = 1892, `npcflag` = 0, `speed_walk` = 1, `speed_run` = 1.14286, `scale` = 1, `rank` = 0, `dmgschool` = 0, `BaseAttackTime` = 2000, `RangeAttackTime` = 2000, `BaseVariance` = 0, `RangeVariance` = 0, `unit_class` = 1, `unit_flags` = 512, `unit_flags2` = 2048, `unit_flags3` = 0, `dynamicflags` = 32, `family` = 0, `trainer_type` = 0, `trainer_class` = 0, `trainer_race` = 0, `type` = 7, `type_flags` = 0, `type_flags2` = 0, `lootid` = 0, `pickpocketloot` = 0, `skinloot` = 0, `resistance1` = 0, `resistance2` = 0, `resistance3` = 0, `resistance4` = 0, `resistance5` = 0, `resistance6` = 0, `spell1` = 0, `spell2` = 0, `spell3` = 0, `spell4` = 0, `spell5` = 0, `spell6` = 0, `spell7` = 0, `spell8` = 0, `VehicleId` = 0, `mingold` = 0, `maxgold` = 0, `AIName` = '', `MovementType` = 0, `InhabitType` = 3, `HoverHeight` = 1, `HealthModifier` = 1, `HealthModifierExtra` = 1, `ManaModifier` = 1, `ManaModifierExtra` = 1, `ArmorModifier` = 1, `DamageModifier` = 1, `ExperienceModifier` = 1, `RacialLeader` = 0, `movementId` = 0, `FadeRegionRadius` = 0, `WidgetSetID` = 0, `WidgetSetUnitConditionID` = 0, `RegenHealth` = 1, `mechanic_immune_mask` = 0, `flags_extra` = 0, `ScriptName` = '', `VerifiedBuild` = 35662 WHERE `entry` = 42560;

-- Spawn Two-Shoed Lou + phase
DELETE FROM `creature` WHERE `guid` = '3530203330003073';
INSERT INTO `creature`(`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES (3530203330003073, 42560, 0, 40, 109, '0', 0, 171, 0, -1, 0, 0, -9854.94, 1277.08, 40.8754, 6.12867, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, '', 0);
DELETE FROM `creature_addon` WHERE `guid` = '3530203330003073';
INSERT INTO `creature_addon`(`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES (3530203330003073, 0, 0, 7, 1, 0, 0, 0, 0, 0, '0');

-- Fix phases and auras for other npcs
UPDATE `creature` SET `PhaseId` = '171' WHERE `guid` IN (10678111, 10678112, 10678114, 10678115, 10678113, 10678110);
UPDATE `creature_addon` SET `auras` = '0' WHERE `guid` IN (10678111, 10678112, 10678114, 10678115, 10678113, 10678110);

-- Add boots object
DELETE FROM `gameobject` WHERE `guid` = '3440000000150';
INSERT INTO `gameobject`(`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES (3440000000150, 301562, 0, 0, 0, '0', 0, 171, 0, -1, -9853.61, 1276.96, 40.8862, 6.17121, -0, -0, -0.055958, 0.998433, 300, 255, 1, 0, '', 0);
