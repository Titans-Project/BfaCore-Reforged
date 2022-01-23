/*
**************************
*    BfaCore Reforged    *
**************************
*/


-- Fix starting area profession Trainer
DELETE FROM `gossip_menu_option` WHERE  `MenuId` IN (12180,12185,12186,12188,12189,12190,12191,12192,12193,12195,12196,12197,12198,12199) AND `OptionIndex` IN (0,1,2,3,4,5,6,7,8,9,10);
-- Enchanting
UPDATE `creature_trainer` SET `OptionIndex`='11' WHERE  `CreatureId`=47396 AND `MenuId`=12193 AND `OptionIndex`=2;
-- Herbalism
DELETE FROM `trainer_spell` WHERE  `TrainerId`=388 AND `SpellId`=2372;
-- Skinning
DELETE FROM `trainer_spell` WHERE  `TrainerId`=390 AND `SpellId`=8615;
-- Alchemy
DELETE FROM `trainer_spell` WHERE  `TrainerId`=59 AND `SpellId`=2275;
-- Engineering
DELETE FROM `trainer_spell` WHERE  `TrainerId`=102 AND `SpellId`=4039;
-- Inscription
DELETE FROM `trainer_spell` WHERE  `TrainerId`=387 AND `SpellId`=45375;

-- Fix trainer_spell
-- Herbalism
UPDATE `trainer_spell` SET `MoneyCost`='10' WHERE  `TrainerId`=388 AND `SpellId`=265820;
DELETE FROM `trainer_spell` WHERE  `TrainerId`=424 AND `SpellId` IN (2373, 265826);
INSERT INTO `trainer_spell` (`TrainerId`, `SpellId`, `MoneyCost`, `ReqSkillLine`, `ReqSkillRank`, `ReqAbility1`, `ReqAbility2`, `ReqAbility3`, `ReqLevel`, `VerifiedBuild`) VALUES
(424, 265826, 10, 2556, 1, 0, 0, 0, 5, 0);

-- Classic Inscription
DELETE FROM `trainer_spell` WHERE `SpellId` IN (50614,50606,50599,58486,57708,58476,59484,59486,59487,57709,59387,60336,50616,50607,50600,58487,57710,58478,59489,59490,59491,50617,57711,50608,50601,58488,58480,57712,59493,59494,59502,50618,57713,50609);
 INSERT INTO `trainer_spell` (`TrainerId`, `SpellId`, `MoneyCost`, `ReqSkillLine`, `ReqSkillRank`, `ReqAbility1`, `ReqAbility2`, `ReqAbility3`, `ReqLevel`, `VerifiedBuild`) VALUES
 (383, 50614, 750, 2514, 155, 0, 0, 0, 0, 35662),
 (383, 50606, 750, 2514, 160, 0, 0, 0, 0, 35662),
 (383, 50599, 750, 2514, 165, 0, 0, 0, 0, 35662),
 (383, 58486, 750, 2514, 170, 0, 0, 0, 0, 35662),
 (383, 57708, 750, 2514, 175, 0, 0, 0, 0, 35662),
 (383, 58476, 750, 2514, 175, 0, 0, 0, 0, 35662),
 (383, 59484, 750, 2514, 175, 0, 0, 0, 0, 35662),
 (383, 59486, 750, 2514, 175, 0, 0, 0, 0, 35662),
 (383, 59487, 750, 2514, 175, 0, 0, 0, 0, 35662),
 (383, 57709, 3000, 2514, 200, 0, 0, 0, 0, 35662),
 (383, 59387, 3000, 2514, 200, 0, 0, 0, 0, 35662),
 (383, 60336, 3000, 2514, 200, 0, 0, 0, 0, 35662),
 (383, 50616, 4500, 2514, 205, 0, 0, 0, 0, 35662),
 (383, 50607, 4500, 2514, 210, 0, 0, 0, 0, 35662),
 (383, 50600, 4500, 2514, 215, 0, 0, 0, 0, 35662),
 (383, 58487, 4500, 2514, 220, 0, 0, 0, 0, 35662),
 (383, 57710, 6000, 2514, 225, 0, 0, 0, 0, 35662),
 (383, 58478, 4500, 2514, 225, 0, 0, 0, 0, 35662),
 (383, 59489, 6000, 2514, 225, 0, 0, 0, 0, 35662),
 (383, 59490, 6000, 2514, 225, 0, 0, 0, 0, 35662),
 (383, 59491, 6000, 2514, 225, 0, 0, 0, 0, 35662),
 (383, 50617, 10000, 2514, 250, 0, 0, 0, 0, 35662),
 (383, 57711, 10000, 2514, 250, 0, 0, 0, 0, 35662),
 (383, 50608, 10000, 2514, 255, 0, 0, 0, 0, 35662),
 (383, 50601, 10000, 2514, 260, 0, 0, 0, 0, 35662),
 (383, 58488, 10000, 2514, 265, 0, 0, 0, 0, 35662),
 (383, 58480, 10000, 2514, 270, 0, 0, 0, 0, 35662),
 (383, 57712, 10000, 2514, 275, 0, 0, 0, 0, 35662),
 (383, 59493, 10000, 2514, 275, 0, 0, 0, 0, 35662),
 (383, 59494, 10000, 2514, 275, 0, 0, 0, 0, 35662),
 (383, 59502, 10000, 2514, 275, 0, 0, 0, 0, 35662),
 (383, 50618, 20000, 2514, 290, 0, 0, 0, 0, 35662),
 (383, 57713, 20000, 2514, 290, 0, 0, 0, 0, 35662),
 (383, 50609, 20000, 2514, 295, 0, 0, 0, 0, 35662);
 
-- Cataclysm inscription
DELETE FROM `trainer_spell` WHERE `SpellId` IN (69385,85785,86004,86005,86375,86401,86402,86403,86615,86609,86616,86640,86641,86642,86643,86648,86649,86652,86653,86654,89244,89368,89369,89370,89371,89372,89373,264501);
INSERT INTO `trainer_spell` (`TrainerId`, `SpellId`, `MoneyCost`, `ReqSkillLine`, `ReqSkillRank`, `ReqAbility1`, `ReqAbility2`, `ReqAbility3`, `ReqLevel`, `VerifiedBuild`) VALUES
(383, 69385, 120000, 2511, 15, 0, 0, 0, 0, 35662),
(383, 85785, 120000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 86004, 80000, 2511, 1, 0, 0, 0, 0, 35662),
(383, 86005, 90000, 2511, 25, 0, 0, 0, 0, 35662),
(383, 86375, 75000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 86401, 75000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 86402, 75000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 86403, 75000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 86609, 80000, 2511, 1, 0, 0, 0, 0, 35662),
(383, 86615, 200000, 2511, 75, 0, 0, 0, 0, 35662),
(383, 86616, 90000, 2511, 25, 0, 0, 0, 0, 35662),
(383, 86640, 90000, 2511, 25, 0, 0, 0, 0, 35662),
(383, 86641, 120000, 2511, 60, 0, 0, 0, 0, 35662),
(383, 86642, 120000, 2511, 60, 0, 0, 0, 0, 35662),
(383, 86643, 120000, 2511, 60, 0, 0, 0, 0, 35662),
(383, 86648, 90000, 2511, 30, 0, 0, 0, 0, 35662),
(383, 86649, 120000, 2511, 55, 0, 0, 0, 0, 35662),
(383, 86652, 150000, 2511, 65, 0, 0, 0, 0, 35662),
(383, 86653, 150000, 2511, 65, 0, 0, 0, 0, 35662),
(383, 86654, 95000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 89244, 100000, 2511, 50, 0, 0, 0, 0, 35662),
(383, 89368, 80000, 2511, 1, 0, 0, 0, 0, 35662),
(383, 89369, 80000, 2511, 15, 0, 0, 0, 0, 35662),
(383, 89370, 80000, 2511, 20, 0, 0, 0, 0, 35662),
(383, 89371, 80000, 2511, 5, 0, 0, 0, 0, 35662),
(383, 89372, 80000, 2511, 10, 0, 0, 0, 0, 35662),
(383, 89373, 80000, 2511, 1, 0, 0, 0, 0, 35662),
(383, 264501, 10, 2514, 1, 0, 0, 0, 5, 35662);

-- Fix trainers
-- Elise Brightletter
DELETE FROM `world_bfa`.`npc_trainer` WHERE  `ID`=30717;
UPDATE `world_bfa`.`creature_trainer` SET `TrainerId`='383' WHERE  `CreatureId`=30717;

-- Alchemist Mallory
DELETE FROM `npc_trainer` WHERE `ID`=1215;
DELETE FROM `creature_trainer` WHERE `CreatureId`=1215;
INSERT INTO `creature_trainer` (`CreatureId`, `TrainerId`) VALUES ('1215', '122');
UPDATE `creature_template` SET `npcflag`='80' WHERE  `entry`=1215;

-- Reyna Stonebranch
DELETE FROM `npc_trainer` WHERE `ID`=5137;
DELETE FROM `creature_trainer` WHERE `CreatureId`=5137;
INSERT INTO `creature_trainer` (`CreatureId`, `TrainerId`, `MenuId`) VALUES (5137, 424, 7691);

-- Herbalist Pomeroy
DELETE FROM `npc_trainer` WHERE `ID`=1218;
UPDATE `creature_trainer` SET `TrainerId`='424' WHERE  `CreatureId`=1218;

-- Adele Fielder
DELETE FROM `npc_trainer` WHERE `ID`=1632;
DELETE FROM `creature_trainer` WHERE `CreatureId`=1632;
INSERT INTO `creature_trainer` (`CreatureId`, `TrainerId`, `MenuId`, `OptionIndex`) VALUES (1632, 990, 4203, 0);
UPDATE `creature_template` SET `gossip_menu_id`='4203' WHERE  `entry`=1632;

-- Helen Peltskinner
DELETE FROM `npc_trainer` WHERE `ID`=6306;
DELETE FROM `creature_trainer` WHERE `CreatureId`=6306;
INSERT INTO `creature_trainer` (`CreatureId`, `TrainerId`, `MenuId`, `OptionIndex`) VALUES (6306, 989, 648, 0);
UPDATE `creature_template` SET `gossip_menu_id`='648' WHERE  `entry`=6306;
UPDATE `creature_template` SET `npcflag`='81' WHERE  `entry`=6306;

-- Smith Argus
UPDATE `creature_trainer` SET `TrainerId`='991' WHERE  `CreatureId`=514;

-- Tognus Flintfire
UPDATE `creature_trainer` SET `TrainerId`='991' WHERE `CreatureId`=1241;