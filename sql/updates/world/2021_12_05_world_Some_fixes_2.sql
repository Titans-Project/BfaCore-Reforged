/*
**************************
*    BfaCore Reforged    *
**************************
*/

-- Add scriptname for Lorna (36457)
UPDATE `creature_template` SET `ScriptName`='npc_lorna_crowley_36457' WHERE `entry`=36457;

-- Add SAI for Tyrande (7999)
DELETE FROM `smart_scripts` WHERE `source_type` = '0' AND `entryorguid` = '7999';
INSERT INTO `smart_scripts`(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(7999, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 4, 5885, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tyrande Whisperwind - On Aggro - Play Sound 5885'),
(7999, 0, 1, 0, 0, 0, 100, 0, 9000, 12000, 17000, 22000, 0, 11, 20690, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Tyrande Whisperwind - In Combat - Cast Moonfire'),
(7999, 0, 2, 0, 0, 0, 100, 0, 5000, 7000, 6000, 8000, 0, 11, 20691, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tyrande Whisperwind - In Combat - Cast Cleave'),
(7999, 0, 3, 0, 0, 0, 100, 0, 11000, 14000, 9000, 12000, 0, 11, 20688, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Tyrande Whisperwind - In Combat - Cast Searing Arrow'),
(7999, 0, 4, 0, 0, 0, 100, 0, 6000, 8000, 20000, 25000, 0, 11, 20687, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tyrande Whisperwind - In Combat - Cast Starfall');


-- Fix Spirit Healer
UPDATE `creature_template` SET `gossip_menu_id` = 0, `npcflag` = 16385, `InhabitType` = 4, unit_flags=768, unit_flags2=2048 WHERE `name` = 'Spirit Healer';

UPDATE creature_template_addon SET bytes1=0 WHERE entry IN
(29259,32537,39660,65183,72676,88148,92355,94070,112180,104689,115774,133783,6491,144412,126141,163311,163310,158175,173514);

-- Fix Dark Iron Invader
-- non attackable_1 & immunce to pc (player) & no select
update creature_template set unit_flags=33557378 where entry=41899;
-- non attackable_2
update creature_template set unit_flags2=65536 where entry=41899;
-- null
update creature_template set unit_flags3=0 where entry=41899;

-- Fix Toy : Transmorpher beacon (14959) (/scripts/Custom/custom_npcs.cpp)
UPDATE `creature_template` SET `npcflag`='7' WHERE `entry`='149596';
UPDATE `creature_template` SET `ScriptName`='npc_transmorpher_beacon' WHERE `entry`='149596';