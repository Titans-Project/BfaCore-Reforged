
UPDATE creature_template SET scriptname = 'boss_aspix' WHERE entry = 133944;
UPDATE creature_template SET scriptname = 'boss_tzane' WHERE entry = 132701;
UPDATE creature_template SET scriptname = 'boss_jiarak' WHERE entry = 132253;
UPDATE creature_template SET scriptname = 'boss_adderis' WHERE entry = 133379;
UPDATE creature_template SET scriptname = 'boss_nizuao' WHERE entry = 71954;
UPDATE creature_template SET scriptname = 'worldboss_yu_lon' WHERE entry = 71955;
UPDATE creature_template SET scriptname = 'npc_throne_of_thunder_gurubashi_berserker' WHERE entry = 69905;
UPDATE creature_template SET scriptname = 'boss_tortos' WHERE entry = 67977;
UPDATE creature_template SET scriptname = 'boss_ra_den' WHERE entry = 69473;
UPDATE creature_template SET scriptname = 'boss_primordius' WHERE entry = 69017;
UPDATE creature_template SET scriptname = 'boss_megaera' WHERE entry = 68065;
UPDATE creature_template SET scriptname = 'boss_lei_shen' WHERE entry = 68397;
UPDATE creature_template SET scriptname = 'boss_jinrokh_the_breaker' WHERE entry = 69465;
UPDATE creature_template SET scriptname = 'boss_blood_princess_thalena' WHERE entry = 102431;
UPDATE creature_template SET scriptname = 'boss_anubesset' WHERE entry = 102246;
UPDATE creature_template SET scriptname = 'boss_mind_flayer_kaarhj' WHERE entry = 101950;
UPDATE creature_template SET scriptname = 'boss_saelorn' WHERE entry = 102387;
UPDATE creature_template SET scriptname = 'boss_dunegorger_kraulok' WHERE entry = 138794;

UPDATE creature_template SET scriptname = 'boss_ji_kun' WHERE entry = 69712;
UPDATE creature_template SET scriptname = 'boss_horridon' WHERE entry = 68476;

UPDATE creature_template SET scriptname = 'boss_sul_the_sandcrawler' WHERE entry = 69078;


UPDATE creature_template SET scriptname = 'boss_kazra_jin' WHERE entry = 69134;
UPDATE creature_template SET scriptname = 'boss_frost_king_malakk' WHERE entry = 69131;
UPDATE creature_template SET scriptname = 'boss_high_priestess_mar_li' WHERE entry = 69132;
UPDATE creature_template SET scriptname = 'boss_spirit_of_gara_jal_council' WHERE entry = 69135;
UPDATE creature_template set ScriptName = 'boss_sha_of_fear' where entry = 60999;
UPDATE creature_template SET scriptname = 'npc_inquisitor_baleful' WHERE entry = 93105;
UPDATE creature_template SET scriptname = 'npc_jace_tyranna_fight' WHERE entry = 97959;
UPDATE creature_template SET scriptname = 'boss_fel_lord_betrug' WHERE entry = 102446;
UPDATE creature_template SET scriptname = 'boss_millificent_manastorm' WHERE entry = 101976;
UPDATE creature_template SET scriptname = 'boss_aluriel' WHERE entry = 107699;


UPDATE creature_template SET scriptname = 'boss_chronomatic_anomaly' WHERE entry = 104415;
UPDATE creature_template SET scriptname = 'boss_shivermaw' WHERE entry = 101951;
UPDATE creature_template SET scriptname = 'boss_festerface' WHERE entry = 101995;
UPDATE creature_template SET scriptname = 'boss_attumen_new' WHERE entry = 114262;
UPDATE creature_template SET scriptname = 'boss_trilliax' WHERE entry = 104288;
UPDATE creature_template SET scriptname = 'boss_skorpyron' WHERE entry = 102263;
UPDATE creature_template SET scriptname = 'boss_curator_new' WHERE entry = 114247;
UPDATE creature_template SET scriptname = 'boss_maiden_of_virtue_new' WHERE entry = 113971;

SET @MANA_DEVOURER := 116494;

UPDATE `creature_template` SET 
`ScriptName` = 'boss_mana_devourer', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`unit_flags2` = 0,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@MANA_DEVOURER, 11649401, 11649402);

UPDATE `creature_template` SET 
`flags_extra` = 128
WHERE `entry` IN (98080, 98081, 98082);



DELETE FROM `spell_script_names` WHERE `spell_id` IN (227297, 227524, 227451);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(227297, 'spell_devourer_coalesce_power'),
(227524, 'spell_devourer_energy_void'),
(227451, 'spell_devourer_mana_restore');

DELETE FROM `areatrigger_template` WHERE `id` IN (8791, 9077, 8754);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8791, 0, 2, 10, 10, 0, 0, 0, 0, 'at_kara_energy_void', 23420),
(9077, 0, 0, 0.5, 0.5, 0, 0, 0, 0, '', 23420),
(8754, 0, 4, 3, 3, 0, 0, 0, 0, 'at_kara_loose_mana', 23420);

UPDATE `instance_template` SET `script` = 'instance_new_karazhan' WHERE `map` = 1651;

DELETE FROM `creature_model_info` WHERE `DisplayID` = 73157;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`) VALUES
(73157, 8.012421, 10, 2);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN (8791, 9077, 8754);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8791, 8791, 0, 2294, 0, 0, 85, 0, 120000, 23420), -- SpellId: 227523
(9077, 9077, 0, 0, 0, 0, 0, 0, 10000, 23420), -- SpellId : 230212
(8754, 8754, 0, 0, 0, 0, 0, 375109, 0, 23420); -- SpellId: 227296



UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_midnight', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`InhabitType` = 1,
`VehicleId` = 4990,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (114264, 11426401, 11426402);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_intagible_presence',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16,
`unit_flags` = 256,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (114315, 11431501, 11431502);


DELETE FROM `spell_script_names` WHERE `spell_id` IN (227338, 227404, 227365);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(227338, 'spell_attumen_riderless'),
(227404, 'spell_attumen_intagible_presence'),
(227365, 'spell_attumen_spectral_charge');

DELETE FROM `areatrigger_template` WHERE `id` IN (8762);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8762, 0, 0, 1, 1, 0, 0, 0, 0, 'at_kara_spectral_charge', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN  (8762);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8762, 8762, 2271, 0, 0, 0, 0, 9000, 9000, 23420);

DELETE FROM `creature_text` WHERE `CreatureID` = @ATTUMEN;
REPLACE INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
(@ATTUMEN, '0', '0', 'Ah, faithful Midnight. For many years, our countless hunts provided game for the master\'s table.', '14', '100', '58644', 'ATTUMEN - INTRO'),
(@ATTUMEN, '1', '0', 'These days we hunt for sport... and the glory of the kill!', '14', '100', '58644', 'ATTUMEN - AGGRO'),
(@ATTUMEN, '1', '1', 'Well, well! Another group of thieves trying to steal my horse!', '14', '100', '58645', 'ATTUMEN - AGGRO'),
(@ATTUMEN, '2', '0', 'My quarry never escapes!', '14', '100', '58646', 'ATTUMEN - INTAGIBLE PRESENCE'),
(@ATTUMEN, '3', '0', 'Rise up, Midnight! Stamp them into dust!', '14', '100', '58639', 'ATTUMEN - MIGHTY_STOMP'),
(@ATTUMEN, '4', '0', 'Time to face my quarry toe-to-toe!', '14', '100', '58638', 'ATTUMEN - FOOT_FIGHT'),
(@ATTUMEN, '5', '0', 'We ride, Midnight! To victory!', '14', '100', '58640', 'ATTUMEN - RIDE_MIDNIGHT'),
(@ATTUMEN, '6', '0', 'Form ranks! Charge!', '14', '100', '58642', 'ATTUMEN - SPECTRAL_CHARGE'),
(@ATTUMEN, '7', '0', 'My trophy collection grows.', '14', '100', '58647', 'ATTUMEN - KILL'),
(@ATTUMEN, '7', '1', 'Easy prey!', '14', '100', '58648', 'ATTUMEN - KILL'),
(@ATTUMEN, '8', '0', 'Well done, Midnight!', '14', '100', '58641', 'ATTUMEN - KILL_MIDNIGHT'),
(@ATTUMEN, '9', '0', 'They had no chance! I kill for a living!', '14', '100', '58641', 'ATTUMEN - WIPE'),
(@ATTUMEN, '10', '0', 'I... have become... the hunted...', '14', '100', '58641', 'ATTUMEN - DEATH');



DELETE FROM `spell_script_names` WHERE `spell_id` IN (227508, 227817, 227823);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(227508, 'spell_maiden_mass_repetance'),
(227817, 'spell_maiden_holy_bulwark'),
(227823, 'spell_maiden_holy_wrath');



DELETE FROM `areatrigger_template` WHERE `id` = 8812;
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8812, 0, 2, 5, 5, 0, 0, 0, 0, 'at_sacred_ground', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` = 8812;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8812, 8812, 0, 2537, 0, 0, 86, 0, 155000, 23420);

DELETE FROM `creature_text` WHERE `CreatureID` = @MAIDEN;
REPLACE INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
(@MAIDEN, '0', '0', 'And so begins your purification!', '14', '100', '58644', 'MAIDEN - AGGRO'),
(@MAIDEN, '0', '1', 'The stench of corruption is upon you! ', '14', '100', '58645', 'MAIDEN - AGGRO'),
(@MAIDEN, '1', '0', 'All shall be sanctified!', '14', '100', '58646', 'MAIDEN - HOLY_BOLT'),
(@MAIDEN, '2', '0', 'By fire be cleansed!', '14', '100', '58639', 'MAIDEN - HOLY_SHOCK'),
(@MAIDEN, '3', '0', 'The righteous have nothing to fear!', '14', '100', '58638', 'MAIDEN - SACRED_GROUND'),
(@MAIDEN, '3', '1', 'Step into the light, mortals!', '14', '100', '58640', 'MAIDEN - SACRED_GROUND'),
(@MAIDEN, '4', '0', 'Hold, mortals! Open your hearts to virtue!', '14', '100', '58642', 'MAIDEN - MASS_REPETANCE'),
(@MAIDEN, '5', '0', 'Renounce your greed and perversion!', '14', '100', '58647', 'MAIDEN - HOLY_BULWARK'),
(@MAIDEN, '6', '0', 'Repent... or DIE!', '14', '100', '58648', 'MAIDEN - HOLY_WRATH'),
(@MAIDEN, '7', '0', 'Purified!', '14', '100', '58641', 'MAIDEN - KILL'),
(@MAIDEN, '7', '1', 'A teachable moment.', '14', '100', '58641', 'MAIDEN - KILL'),
(@MAIDEN, '8', '0', 'The lesson is complete.', '14', '100', '58641', 'MAIDEN - WIPE'),
(@MAIDEN, '8', '1', 'A pure mind is an orderly mind.', '14', '100', '58641', 'MAIDEN - WIPE'),
(@MAIDEN, '9', '0', 'I will... never... relent...', '14', '100', '58641', 'MAIDEN - DEATH'),
(@MAIDEN, '9', '1', 'You are all... as corrupt... as the Guardian...', '14', '100', '58641', 'MAIDEN - DEATH');

SET @MANA_DEVOURER := 116494;

UPDATE `creature_template` SET 
`ScriptName` = 'boss_mana_devourer', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`unit_flags2` = 0,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@MANA_DEVOURER, 11649401, 11649402);

UPDATE `creature_template` SET 
`flags_extra` = 128
WHERE `entry` IN (98080, 98081, 98082);



DELETE FROM `spell_script_names` WHERE `spell_id` IN (227297, 227524, 227451);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(227297, 'spell_devourer_coalesce_power'),
(227524, 'spell_devourer_energy_void'),
(227451, 'spell_devourer_mana_restore');

DELETE FROM `areatrigger_template` WHERE `id` IN (8791, 9077, 8754);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8791, 0, 2, 10, 10, 0, 0, 0, 0, 'at_kara_energy_void', 23420),
(9077, 0, 0, 0.5, 0.5, 0, 0, 0, 0, '', 23420),
(8754, 0, 4, 3, 3, 0, 0, 0, 0, 'at_kara_loose_mana', 23420);

UPDATE `instance_template` SET `script` = 'instance_new_karazhan' WHERE `map` = 1651;

DELETE FROM `creature_model_info` WHERE `DisplayID` = 73157;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`) VALUES
(73157, 8.012421, 10, 2);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN (8791, 9077, 8754);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8791, 8791, 0, 2294, 0, 0, 85, 0, 120000, 23420), -- SpellId: 227523
(9077, 9077, 0, 0, 0, 0, 0, 0, 10000, 23420), -- SpellId : 230212
(8754, 8754, 0, 0, 0, 0, 0, 375109, 0, 23420); -- SpellId: 227296

SET @MOROES := 114312;

UPDATE `creature_template` SET 
`ScriptName` = 'boss_moroes', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`unit_flags2` = 0,
`InhabitType` = 1,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@MOROES, 11431201, 11431202);

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_moroes_guest', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16
WHERE `entry` IN (114316, 114317, 114318, 114319, 114320, 114321);



UPDATE `creature_template` SET 
`ScriptName` = '', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`flags_extra` = 128
WHERE `entry` IN (114327);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (227736, 227872, 227672);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(227736, 'spell_moroes_vanish'),
(227872, 'spell_moroes_ghastly_purge'),
(227672, 'spell_kara_will_breaker');

DELETE FROM `creature_template_addon` WHERE `entry` = 114327;
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES
(114327, '227472');

DELETE FROM `areatrigger_template` WHERE `id` IN (8782);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8782, 0, 4, 5, 5, 0, 0, 0, 0, 'at_kara_whirling_edge', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN (8782);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8782, 8782, 0, 0, 0, 0, 0, 0, 0, 23420); -- SpellId: 227472

DELETE FROM `creature_text` WHERE `CreatureID` = @MOROES;
INSERT INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
(@MOROES, '0', '1', 'The master has not been himself for quite some time. Perhaps a feast will brighten his spirit.', '14', '100', '58641', 'MOROES - INTRO'),
(@MOROES, '1', '0', 'I don\'t believe the master is expecting you!', '14', '100', '58644', 'MORES - AGGRO'),
(@MOROES, '1', '1', 'I do so detest party crashers!', '14', '100', '58644', 'MOROES - AGGRO'),
(@MOROES, '1', '2', 'I\'ll drag you out by your scruff!', '14', '100', '58645', 'MOROES - AGGRO'),
(@MOROES, '1', '3', 'May we see your invitation?', '14', '100', '58646', 'MOROES - AGGRO'),
(@MOROES, '2', '0', 'May I take your jacket?', '14', '100', '58639', 'MOROES - COAT_JACKET'),
(@MOROES, '2', '1', 'Mind your manners!', '14', '100', '58638', 'MOROES - COAT_JACKET'),
(@MOROES, '3', '0', 'I insist you dress for dinner!', '14', '100', '58640', 'MOROES - VANISH'),
(@MOROES, '4', '0', 'Stop troubling the master\'s guests!', '14', '100', '58642', 'MOROES - GHASTLY'),
(@MOROES, '5', '0', 'Nothing a good sprinkling of sawdust can\'t handle.', '14', '100', '58647', 'MOROES - KILL'),
(@MOROES, '5', '1', 'I\'ll have the cleaning crew take care of this... mess.', '14', '100', '58648', 'MOROES - KILL'),
(@MOROES, '5', '2', 'Hm! Seems that one had enough for the evening.', '14', '100', '58641', 'MOROES - KILL'),
(@MOROES, '5', '3', 'Stop being such a nuisance!', '14', '100', '58641', 'MOROES - KILL'),
(@MOROES, '6', '0', 'Perhaps your party would be more comfortable at the children\'s table.', '14', '100', '58641', 'MOROES - WIPE'),
(@MOROES, '6', '1', 'The guards really must do a better job of keeping out the ruffians.', '14', '100', '58641', 'MOROES - WIPE'),
(@MOROES, '7', '0', 'Seems I\'ll be... retiring... for... the evening...', '14', '100', '58641', 'MOROES - DEATH'),
(@MOROES, '7', '1', 'Master... I hope... I served you... well...', '14', '100', '58641', 'MOROES - DEATH'),
(@MOROES, '7', '2', 'I fear that I am currently... indisposed...', '14', '100', '58641', 'MOROES - DEATH'),
(@MOROES, '7', '3', 'I will just need... a moment...', '14', '100', '58641', 'MOROES - DEATH');

SET @NIGHTBANE := 114895;
SET @ARCHANAGOS := 115213;
SET @IMAGE_MEDIVH := 115038;

UPDATE `creature_template` SET 
`ScriptName` = 'boss_nightbane', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`unit_flags2` = 0,
`InhabitType` = 4,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@NIGHTBANE, 11489501, 11489502);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_bonecurse',
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`InhabitType` = 4
WHERE `entry` IN (114903, 11490301, 11490302);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_restless_bones',
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16
WHERE `entry` IN (114906, 11490601, 11490602);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_image_of_medivh',
`npcflag` = 1
WHERE `entry` IN (115038, 11503801, 11503802);

UPDATE `gameobject_template` SET 
`ScriptName` = 'go_door_entrance'
WHERE `entry` = 259690;

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_soul_fragment',
`npcflag` = `npcflag` |16777216,
`unit_flags` = 768
WHERE `entry` IN (115103, 115113, 115101, 115013, 115105);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (228862, 228796, 228829, 228835);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(228862, 'spell_nightbane_charred_earth'),
(228796, 'spell_nightbane_ignite_soul'),
(228829, 'spell_nightbane_burning_bones'),
(228835, 'spell_kara_absorb_vitality');

DELETE FROM `spell_target_position` WHERE `ID` IN (232406);
INSERT INTO `spell_target_position` (`ID`, `EffectIndex`, `MapID`, `PositionX`, `PositionY`, `PositionZ`, `VerifiedBuild`) VALUES
(232406, 0, 1651, -11049.805, -1980.708, 101.889, 23360);

DELETE FROM `areatrigger_template` WHERE `id` IN (8905, 9175);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8905, 0, 0, 10, 10, 0, 0, 0, 0, 'at_kara_charred_earth', 24461),
(9175, 4, 0, 10, 10, 4, 4, 0.3, 0.3, '', 24461);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN (8905, 9175);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8905, 8905, 0, 0, 0, 0, 0, 0, 60000, 24461), -- SpellId : 228864
(9175, 9175, 0, 0, 0, 0, 93, 0, 60000, 24461); -- SpellId : 228864

DELETE FROM `creature_text` WHERE `CreatureID` IN (@NIGHTBANE, @ARCHANAGOS, @IMAGE_MEDIVH);
INSERT INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
(@NIGHTBANE, '0', '0', 'Learn the meaning of torment!', '14', '100', '77532', 'NIGHTBANE - AGGRO'),
(@NIGHTBANE, '0', '1', 'Come forth and burn! ', '14', '100', '77534', 'NIGHTBANE - AGGRO'),
(@NIGHTBANE, '1', '0', 'Burn from the inside!', '14', '100', '77523', 'NIGHTBANE - IGNITE_SOUL'),
(@NIGHTBANE, '1', '1', 'Your soul betrays your allies!', '14', '100', '77522', 'NIGHTBANE - IGNITE_SOUL'),
(@NIGHTBANE, '1', '2', 'Your lifeblood is my weapon!', '14', '100', '77524', 'NIGHTBANE - IGNITE_SOUL'),
(@NIGHTBANE, '2', '0', 'I\'ve toyed with you long enough!', '14', '100', '77545', 'NIGHTBANE - PHASE_AIR'),
(@NIGHTBANE, '2', '1', 'Miserable vermin. I shall exterminate you from the air!', '14', '100', '77528', 'NIGHTBANE - PHASE_AIR'),
(@NIGHTBANE, '3', '0', 'Enough! I will finish you myself!', '14', '100', '77529', 'NIGHTBANE - PHASE_3'),
(@NIGHTBANE, '3', '1', 'I will end you!', '14', '100', '77546', 'NIGHTBANE - PHASE_3'),
(@NIGHTBANE, '3', '2', 'Time to end this!', '14', '100', '77544', 'NIGHTBANE - PHASE_3'),
(@NIGHTBANE, '4', '0', 'Run, cowards!', '14', '100', '77525', 'NIGHTBANE - BELLOWING_ROAR'),
(@NIGHTBANE, '4', '1', 'Weak-willed mortals!', '14', '100', '77526', 'NIGHTBANE - BELLOWING_ROAR'),
(@NIGHTBANE, '4', '2', 'Your courage falters!', '14', '100', '77527', 'NIGHTBANE - BELLOWING_ROAR'),
(@NIGHTBANE, '5', '0', 'I\'ll reduce you to ashes!', '14', '100', '77533', 'NIGHTBANE - KILL'),
(@NIGHTBANE, '5', '1', 'Kindling for the fire!', '14', '100', '77535', 'NIGHTBANE - KILL'),
(@NIGHTBANE, '5', '2', 'Pathetic!', '14', '100', '77536', 'NIGHTBANE - KILL'),
(@NIGHTBANE, '5', '3', 'Who\'s next?!', '14', '100', '77537', 'NIGHTBANE - KILL'),
(@NIGHTBANE, '6', '0', 'Mortals never learn.', '14', '100', '77539', 'NIGHTBANE - WIPE'),
(@NIGHTBANE, '6', '1', 'Pitiful... as expected.', '14', '100', '77538', 'NIGHTBANE - WIPE'),
(@NIGHTBANE, '6', '2', 'You called me here only to die at my feet?', '14', '100', '77540', 'NIGHTBANE - WIPE'),
(@NIGHTBANE, '7', '0', 'It is... never... over...', '14', '100', '77541', 'NIGHTBANE - DEATH'),
(@NIGHTBANE, '7', '1', 'I will... never... be free...', '14', '100', '77542', 'NIGHTBANE - DEATH'),
(@NIGHTBANE, '8', '0', 'Am I damned to answer the summons of mortals for all eternity?', '14', '100', '77530', 'NIGHTBANE - DEATH'),
(@NIGHTBANE, '9', '0', 'So be it! You will suffer for your insolence!', '14', '100', '77531', 'NIGHTBANE - DEATH'),
(@NIGHTBANE, '10', '0', 'The strange chill of a dark presence winds through the air', '41', '100', '0', 'NIGHTBANE - START TIMER'),
(@IMAGE_MEDIVH, '0', '0', 'You\'ve got my attention, dragon. You\'ll find I\'m not as easily scared as the villagers below.', '14', '100', '58641', 'IMAGE_MEDIVH_REPLY'),
(@IMAGE_MEDIVH, '1', '0', 'You dare challenge me at my own dwelling? Your arrogance is astounding, even for a dragon.', '14', '100', '58641', 'IMAGE_MEDIVH_REPLY'),
(@IMAGE_MEDIVH, '2', '0', 'I do not know what you speak of, dragon... but I will not be bullied by this display of insolence. I\'ll leave Karazhan when it suits me!', '14', '100', '58641', 'IMAGE_MEDIVH_REPLY'),
(@IMAGE_MEDIVH, '3', '0', 'He should not have angered me. I must go... recover my strength now...', '14', '100', '58641', 'IMAGE_MEDIVH_REPLY'),
(@ARCHANAGOS, '0', '0', 'Your dabbling in the arcane has gone too far, Medivh. You\'ve attracted the attention of powers beyond your understanding. You must leave Karazhan at once!', '14', '100', '58641', 'IMAGE_OF_ARCANAGOS'),
(@ARCHANAGOS, '1', '0', 'A dark power seeks to use you, Medivh! If you stay, dire days will follow. You must hurry, we don\'t have much time!', '14', '100', '77690', 'IMAGE_OF_ARCANAGOS'),
(@ARCHANAGOS, '2', '0', 'You leave me no alternative. I will stop you by force if you won\'t listen to reason.', '14', '100', '58641', 'IMAGE_OF_ARCANAGOS'),
(@ARCHANAGOS, '3', '0', 'What have you done, wizard? This cannot be! I\'m burning from... within!', '14', '100', '58641', 'IMAGE_OF_ARCANAGOS');

SET @ELFYRA         := 114284;
SET @GALINDRE       := 114251;
SET @COGGLESTON     := 114328;
SET @LUMINORE       := 114329;
SET @BABLET         := 114330;
SET @MRS_CAULDRON   := 114522;
SET @TOE_KNEE       := 114261;
SET @MRRGRIA        := 114260;
SET @BARNES         := 114339;

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_barnes',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 35,
`npcflag` = 1
WHERE `entry` IN (@BARNES, 11433901, 11433902);

UPDATE `creature_template` SET 
`ScriptName` = 'boss_opera_toe_knee', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@TOE_KNEE, 11426101, 11426102);

UPDATE `creature_template` SET 
`ScriptName` = 'boss_opera_mrrgria', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@MRRGRIA, 11426001, 11426002);

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_gang_ruffian', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16
WHERE `entry` IN (114265, 11426501, 11426502);

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_shoreline_speaker', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16
WHERE `entry` IN (114266, 11426601, 11426602);

UPDATE `creature_template` SET 
`ScriptName` = 'boss_opera_elfyra', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@ELFYRA, 11428401, 11428402);

UPDATE `creature_template` SET
`ScriptName` = 'boss_opera_galyndre',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@GALINDRE, 11432801, 11432802);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_winged_assistant',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16
WHERE `entry` IN (114324, 11432401, 11432402);

UPDATE `creature_template` SET
`ScriptName` = 'boss_opera_coggleston',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@COGGLESTON, 11432801, 11432802);

UPDATE `creature_template` SET
`ScriptName` = 'boss_opera_luminore',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@LUMINORE, 11432901, 11432902);

UPDATE `creature_template` SET
`ScriptName` = 'boss_opera_bablet',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16,
`speed_walk` = 0.25,
`speed_run` = 0.5,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@BABLET, 11433001, 11433002);

UPDATE `creature_template` SET
`ScriptName` = 'boss_opera_mrs_cauldrons',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@MRS_CAULDRON, 11452201, 11452202);

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_flashing_forks',
`minLevel` = 112,
`maxLevel` = 112,
`faction` = 16
WHERE `entry` IN (116574, 11657401, 11657402);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (231869, 227776, 227447, 227477, 228221, 227449, 227453, 227777, 227783);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(231869, 'spell_opera_wikket_bond'),
(227776, 'spell_opera_magic_magnificient'),
(227447, 'spell_opera_defy_gravity'),
(227477, 'spell_opera_summon_assistants'),
(228221, 'spell_opera_severe_dusting'),
(227449, 'spell_opera_dashing_gale'),
(227453, 'spell_opera_dashing_gale_area'),
(227777, 'spell_opera_thunder_ritual'),
(227783, 'spell_opera_wash_away');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (228011, 227568, 227319, 227453);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(228011, 228013, 1, 'Soup Spray - Mrs Cauldrons'),
(227568, 227567, 1, 'Burning Leg Sweep - Toe Knee'),
(227319, 227325, 0, 'Poisonous Shank - Gang Ruffian');



DELETE FROM `areatrigger_template` WHERE `id` IN (8843, 8763, 8847, 8781, 8811, 8764);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8843, 4, 2, 2, 2, 20, 20, 1.5, 1.5, 'at_kara_burning_blaze', 23420),
(8763, 0, 4, 3, 3, 0, 0, 0, 0, 'at_kara_defy_gravity', 23420),
(8847, 4, 4, 1, 1, 20, 20, 3, 3, '', 23420),
(8781, 0, 0, 3, 3, 0, 0, 0, 0, 'at_kara_dashing_gale', 23420),
(8811, 1, 0, 3, 5, 3, 3, 5, 3, 'at_kara_wash_away', 23420),
(8764, 0, 0, 3, 3, 0, 0, 0, 0, 'at_kara_bubble_blast', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN (8843, 8763, 8847, 8781, 8811, 8764);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8843, 8843, 0, 2564, 0, 0, 0, 0, 60000, 23420), -- SpellId : 228197
(8763, 8763, 0, 0, 0, 0, 0, 0, 180000, 23420), -- SpellId: 227472
(8847, 8847, 0, 0, 0, 0, 0, 0, 0, 23420), -- SpellId : 228213
(8781, 8781, 0, 0, 0, 0, 0, 0, 60000, 23420), -- SpellId: 227471
(8811, 8811, 0, 0, 0, 0, 0, 0, 18000, 23420), -- SpellId: 227790
(8764, 8764, 0, 0, 0, 0, 0, 0, 20000, 23420); -- SpellId: 227420



DELETE FROM `creature_text` WHERE `CreatureID` IN (@ELFYRA, @GALINDRE, @BARNES, @TOE_KNEE, @MRRGRIA, @COGGLESTON, @LUMINORE, @BABLET, @MRS_CAULDRON);
INSERT INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
-- Elfyra --
(@ELFYRA, '0', '0', 'We all deserve a chance to fly! Hey, do you really think the Wizard will de-greenify me?', '14', '100', '77153', 'ELFYRA - INTRO'),
(@ELFYRA, '1', '0', 'I don\'t have any friends... well, apart from you, Galindre.', '14', '100', '77154', 'ELFYRA - INTRO'),
(@ELFYRA, '2', '1', 'Together, we\'re unlimited!', '14', '100', '77146', 'ELFYRA - AGGRO'),
(@ELFYRA, '3', '0', 'Sorry to bring you down!', '14', '100', '77147', 'ELFYRA - DEFY_GRAVITY'),
(@ELFYRA, '3', '1', 'You deserve a chance to fly!', '14', '100', '77148', 'ELFYRA - DEFY_GRAVITY'),
(@ELFYRA, '4', '0', 'Come see my minions, and let them be heard!', '14', '100', '77149', 'ELFYRA - SUMMON_ASSISTANTS'),
(@ELFYRA, '4', '1', 'Elekkamomen otunbuntu rotu kerlaramen!', '14', '100', '77150', 'ELFYRA - SUMMON_ASSISTANTS'),
(@ELFYRA, '5', '0', 'For the first time, I feel... wikket.', '14', '100', '77151', 'ELFYRA - KILL'),
(@ELFYRA, '5', '1', 'That was quite the commotion!', '14', '100', '77152', 'ELFYRA - KILL'),
(@ELFYRA, '6', '0', 'How beautifully... tragic.', '14', '100', '77145', 'ELFYRA - DEATH'),
-- Galindre --
(@GALINDRE, '0', '0', 'Elfyra, come down from there! You\'re fluttering about like one of those horrible monkey creatures!', '14', '100', '77161', 'GALINDRE - INTRO'),
(@GALINDRE, '1', '0', 'Of course, dear! Though that old frock of yours... Oh, look! Some new friends have come to pay us a visit.', '14', '100', '77162', 'GALINDRE - INTRO'),
(@GALINDRE, '2', '0', 'If you\'re my friend, you\'ll make the hozen get rid of these interlopers... then we\'re off to see the Wizard!', '14', '100', '77163', 'GALINDRE - INTRO'),
(@GALINDRE, '3', '0', 'Follow my lead and you\'ll be popular!', '14', '100', '77158', 'GALINDRE - AGGRO'),
(@GALINDRE, '4', '0', 'This place could stand to be Galindre-fied!', '14', '100', '77157', 'GALINDRE - MAGIC_MAGNIFICIENT'),
(@GALINDRE, '4', '1', 'Now for something most magnificent!', '14', '100', '77156', 'GALINDRE - MAGIC_MAGNIFICIENT'),
(@GALINDRE, '5', '0', 'That one needed a makeover.', '14', '100', '77159', 'GALINDRE - KILL'),
(@GALINDRE, '5', '1', 'Have you misplaced your wikket mind?', '14', '100', '77160', 'GALINDRE - KILL'),
-- Toe Knee --
(@TOE_KNEE, '0', '0', 'So ya wanna rumble, do ya?', '14', '100', '77136', 'TOE_KNEE - AGGRO'),
(@TOE_KNEE, '0', '1', 'Let\'s dance!', '14', '100', '77137', 'TOE_KNEE - AGGRO'),
(@TOE_KNEE, '1', '0', 'It\'s you and me against the world, baby!', '14', '100', '77135', 'TOE_KNEE - INTRO'),
(@TOE_KNEE, '2', '0', 'Wanna go for a spin?', '14', '100', '77132', 'TOE_KNEE - DASHING_GALE'),
(@TOE_KNEE, '3', '0', 'Now that\'s what I call a hot foot!', '14', '100', '77133', 'TOE_KNEE - BURNING_LEG_SWEEP'),
(@TOE_KNEE, '4', '0', 'Beat it!', '14', '100', '77138', 'TOE_KNEE - KILL'),
(@TOE_KNEE, '4', '1', 'Go on! Scram!', '14', '100', '77139', 'TOE_KNEE - KILL'),
(@TOE_KNEE, '5', '0', 'A gang that don\'t own the fields, is nothing!', '14', '100', '77140', 'TOE_KNEE - WIPE'),
(@TOE_KNEE, '5', '1', 'I dug you punks a early grave!', '14', '100', '77141', 'TOE_KNEE - WIPE'),
(@TOE_KNEE, '6', '0', 'I\'ll never... stop saying... Mrrgria...?', '14', '100', '77142', 'TOE_KNEE - DEATH'),
(@TOE_KNEE, '6', '1', 'There\'s a... place for us... somewhere...', '14', '100', '77143', 'TOE_KNEE - DEATH'),
-- Coggleston --
(@COGGLESTON, '0', '0', 'Luminore! Mrs. Cauldrons! Babblet! Let us dispose with these interlopers!', '14', '100', '77088', 'COGGLESTON - AGGRO'),
(@COGGLESTON, '1', '0', 'Quickly now! Get rid of them!', '14', '100', '77094', 'COGGLESTON - KARA_KAZHAM'),
(@COGGLESTON, '2', '0', 'Delightful! One less guest for dinner!', '14', '100', '77089', 'COGGLESTON - KILL'),
(@COGGLESTON, '3', '0', 'Never stand in the way of true love!', '14', '100', '77090', 'COGGLESTON - WIPE'),
(@COGGLESTON, '4', '0', 'It seems... my rose... has... wilted...', '14', '100', '77091', 'COGGLESTON - DEATH'),
(@COGGLESTON, '5', '0', 'Oh no, no, no... this won\'t do! You outsiders will ruin our final chance to break the curse!', '14', '100', '77105', 'COGGLESTON - CONVERSATION'),
(@COGGLESTON, '6', '0', 'It\'s working! It\'s working!', '14', '100', '77106', 'COGGLESTON - CONVERSATION'),
-- Luminore --
(@LUMINORE, '0', '0', 'Zee true love, eet burnses!', '14', '100', '77095', 'LUMINORE - HEAT_WAVE'),
(@LUMINORE, '1', '0', 'The candle! Mine!', '14', '100', '77092', 'LUMINORE - KILL'),
(@LUMINORE, '2', '0', 'No... take... zee... candle...', '14', '100', '77093', 'LUMINORE - DEATH'),
-- Bablet --
(@BABLET, '0', '0', 'That will teach you to flirt with my boyfriend!', '14', '100', '77099', 'BABLET - KILL'),
(@BABLET, '1', '0', 'Only beauty... can kill... a brute...', '14', '100', '77100', 'BABLET - DEATH'),
-- Mrs Cauldrons --
(@MRS_CAULDRON, '0', '0', 'Hope I don\'t spoil your appetite, dearies!', '14', '100', '77096', 'MRS_CAULDRON - SOUP_SPRAY'),
(@MRS_CAULDRON, '1', '0', 'That didn\'t go very well at all, did it?', '14', '100', '77098', 'MRS_CAULDRON - KILL'),
(@MRS_CAULDRON, '2', '0', 'Guess I\'m... sleeping in... the cupboard.', '14', '100', '77097', 'MRS_CAULDRON - DEATH');



SET @SHADE_MEDIVH := 114350;

UPDATE `creature_template` SET 
`ScriptName` = 'boss_shade_of_medivh', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`unit_flags2` = 0,
`InhabitType` = 1,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@SHADE_MEDIVH, 11435001, 11435002);

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_guardian_image', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`InhabitType` = 1,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (114675, 11467501, 11467502);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (228334, 227806, 228269);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(228269, 'spell_shade_medivh_flame_wreath'),
(227806, 'spell_shade_medivh_ceaseless_winter'),
(228334, 'spell_shade_medivh_guardian_image');



DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 227641;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(227641, 227644, 1, 'Piercing Missiles - Shade of Medivh');

DELETE FROM `spell_target_position` WHERE `ID` = 227779;
INSERT INTO `spell_target_position` (`ID`, `EffectIndex`, `MapID`, `PositionX`, `PositionY`, `PositionZ`, `VerifiedBuild`) VALUES
(227779, 0, 1651, -4599.05, -2518.14, 2876.59, 23360),
(227779, 1, 1651, -4599.05, -2518.14, 2876.59, 23360);

DELETE FROM `areatrigger_template` WHERE `id` IN (8809, 8848, 8851);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8809, 4, 0, 40, 40, 1, 1, 0.3, 0.3, '', 23420),
(8848, 4, 0, 40, 40, 15, 15, 0.3, 0.3, 'at_kara_ceaseless_winter', 23420),
(8851, 0, 0, 4, 4, 0, 0, 0, 0, 'at_kara_flame_wreath', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN  (8809, 8848, 8851);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8809, 8809, 0, 0, 2285, 0, 89, 0, 20000, 23420), -- SpellId : 227779
(8848, 8848, 0, 0, 2307, 0, 0, 0, 20000, 23420), -- SpellId : 227779
(8851, 8851, 2618, 0, 0, 0, 0, 0, 20000, 23420); -- SpellId : 228257

DELETE FROM `creature_text` WHERE `CreatureID` = @SHADE_MEDIVH;
INSERT INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
(@SHADE_MEDIVH, '0', '0', 'This is who I am. I was tainted from birth, polluted from before my conception, a bad seed grown to bear bitter fruit.', '14', '100', '58644', 'CURATOR - INTRO'),
(@SHADE_MEDIVH, '1', '0', 'Hello, champions. I\'ve been expecting you.', '14', '100', '58644', 'SHADE_MEDIVH - AGGRO'),
(@SHADE_MEDIVH, '1', '1', 'You\'re just like the others, aren\'t you?', '14', '100', '77558', 'SHADE_MEDIVH - AGGRO'),
(@SHADE_MEDIVH, '2', '0', 'A basic spell... but effective', '14', '100', '77549', 'SHADE_MEDIVH - PIERCING_MISSILES'),
(@SHADE_MEDIVH, '2', '1', 'You grow weaker... I grow stronger!', '14', '100', '77564', 'SHADE_MEDIVH - PIERCING_MISSILES'),
(@SHADE_MEDIVH, '3', '0', 'The inherent nature of fire...', '14', '100', '77561', 'SHADE_MEDIVH - EVOCATION'),
(@SHADE_MEDIVH, '3', '1', 'Burn... just like that dragon!', '14', '100', '77570', 'SHADE_MEDIVH - EVOCATION'),
(@SHADE_MEDIVH, '4', '0', 'A memory, frozen in time!', '14', '100', '77560', 'SHADE_MEDIVH - EVOCATION_END'),
(@SHADE_MEDIVH, '5', '0', 'Keep still. Let it end.', '14', '100', '77555', 'SHADE_MEDIVH - KILL'),
(@SHADE_MEDIVH, '5', '1', 'Winter is here!', '14', '100', '77572', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '6', '0', 'I suggest staying put.', '14', '100', '77566', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '6', '1', 'It burns, burns, burns!', '14', '100', '77550', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '7', '0', 'Carrion for the birds...', '14', '100', '77570', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '7', '1', 'So many pieces of me...', '14', '100', '77565', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '8', '0', 'Just. Gets. Easier.', '14', '100', '77557', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '8', '1', 'A fitting end for a thief!', '14', '100', '58641', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '8', '2', 'All goes as planned.', '14', '100', '58641', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '8', '3', 'The tower needed a new decoration.', '14', '100', '77571', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '9', '0', 'You have no idea what it\'s like to do the things I\'ve done. Harsh things. Necessary things.', '14', '100', '77557', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '9', '1', 'Once you get used to it, you can kill friends as easily as anyone else.', '14', '100', '58641', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '10', '0', 'I fought it... for as long as... I could...', '14', '100', '58641', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '10', '1', 'So... it comes... to this...', '14', '100', '58641', 'SHADE_MEDIVH - DEATH'),
(@SHADE_MEDIVH, '10', '2', 'Atiesh... will be... splintered...', '14', '100', '77569', 'SHADE_MEDIVH - DEATH');

DELETE FROM `creature_template_addon` WHERE `entry` = 114247;

UPDATE `creature_template` SET `faction` = 35 WHERE `entry` IN (114262, 11426201, 11426202);

UPDATE `creature_template` SET 
`ScriptName` = 'npc_kara_volatile_energy', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`unit_class` = 2,
`unit_flags2` = `unit_flags2` |2048,
`InhabitType` = 8,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` = 114249;

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (234416, 227256);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(234416, 227267, 1, 'Summon Volatile Energy - The Curator'),
(227256, 227257, 0, 'Overload - The Curator');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (227270, 227269, 227254, 227257, 227278);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(227269, 'spell_kara_arc_lightning'),
(227270, 'spell_kara_arc_lightning_dmg'),
(227254, 'spell_curator_evocation'),
(227257, 'spell_curator_overload'),
(227278, 'spell_curator_power_discharge');

DELETE FROM `areatrigger_template` WHERE `id` = 8780;
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8780, 0, 0, 6, 6, 0, 0, 0, 0, 'at_kara_power_discharge', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` = 8780;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8780, 8780, 0, 0, 0, 0, 0, 0, 0, 23420); -- SpellId : 227289

DELETE FROM `creature_text` WHERE `CreatureID` = @CURATOR;
REPLACE INTO `creature_text`(`CreatureID`, `groupid`, `id`, `text`, `type`, `probability`, `sound`, `comment`) VALUES
(@CURATOR, '0', '0', 'Janitorial units: begin Menagerie decontamination.', '12', '100', '58644', 'CURATOR - INTRO'),
(@CURATOR, '1', '0', 'The Menagerie is for-- ERADICATE! ERADICATE!', '14', '100', '58644', 'CURATOR - AGGRO'),
(@CURATOR, '1', '1', 'Welcome to the-- TERMINATE INTRUDERS! ', '14', '100', '58645', 'CURATOR - AGGRO'),
(@CURATOR, '2', '0', 'Countermeasures deployed.', '12', '100', '58646', 'CURATOR - VOLATILE_ENERGY'),
(@CURATOR, '2', '1', 'DETERENT ACIVATED!', '12', '100', '58639', 'CURATOR - VOLATILE_ENERGY'),
(@CURATOR, '3', '0', 'Overload in progress. Prepare for-- PURGE! PURGE! PURGE!', '12', '100', '58638', 'CURATOR - EVOCATION'),
(@CURATOR, '3', '1', 'Attention guests: DIE! DIE! DIE!', '12', '100', '58640', 'CURATOR - EVOCATION'),
(@CURATOR, '4', '0', 'Deterrent protocols successful.', '12', '100', '58642', 'CURATOR - EVOCATION_END'),
(@CURATOR, '5', '0', 'EXTERMINATED!', '12', '100', '58647', 'CURATOR - KILL'),
(@CURATOR, '6', '0', 'Curator is no longer op... er... ation... al.', '14', '100', '58648', 'CURATOR - DEATH'),
(@CURATOR, '6', '1', 'System... failure... im... mi... nent...', '14', '100', '58641', 'CURATOR - DEATH');

SET @VIZADUUM := 114790;

UPDATE `creature_template` SET 
`ScriptName` = 'boss_vizaduum', 
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`InhabitType` = 4,
`unit_class` = 2,
`unit_flags2` = 0,
`mechanic_immune_mask` = `mechanic_immune_mask` |617299967
WHERE `entry` IN (@VIZADUUM, 11479001, 11479002);

UPDATE `creature_template` SET
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`InhabitType` = 4,
`flags_extra` = 128
WHERE `entry` IN (114913, 11491301, 11491302);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_fel_cannon',
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`speed_walk` = 0.725,
`speed_run` = 0.725,
`flags_extra` = 128
WHERE `entry` IN (115274, 11527401, 11527402);

UPDATE `creature_template` SET
`ScriptName` = 'npc_kara_invisible_vehicle',
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 35,
`InhabitType` = 4,
`speed_walk` = 4,
`speed_run` = 4,
`flags_extra` = 128,
`VehicleId` = 5009
WHERE `entry` IN (115361, 11536101, 11536102);

UPDATE `creature_template` SET
`ScriptName` = 'npc_stabilize_rift',
`minLevel` = 112, 
`maxLevel` = 112, 
`faction` = 16,
`InhabitType` = 4,
`flags_extra` = 128
WHERE `entry` IN (115789, 11578901, 11578902);

UPDATE `creature_template` SET
`faction` = 16,
`flags_extra` = 128,
`InhabitType` = 8
WHERE `entry` = 115493;

DELETE FROM `creature_template_addon` WHERE `entry` IN (115694, 114913);
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES
(115694, '229906');



DELETE FROM `spell_script_names` WHERE `spell_id` IN (229159, 229283, 229287, 229246, 229151, 230084);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(229159, 'spell_vizaduum_chaotic_shadows'),
(229283, 'spell_vizaduum_bombardment_charge'),
(229287, 'spell_vizaduum_bombardment_aura'),
(229246, 'spell_vizaduum_fel_beam_charge'),
(229151, 'spell_vizaduum_disintegrate'),
(230084, 'spell_vizaduum_stabilize_rift');

DELETE FROM `areatrigger_template` WHERE `id` IN (8939, 8940, 8941, 8942, 8943, 8944, 8945, 8946, 9009, 8963, 8962, 9054);
INSERT INTO `areatrigger_template` (`id`, `type`, `flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(8939, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8940, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8941, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8942, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8943, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8944, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8945, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(8946, 0, 0, 3.5, 3.5, 0, 0, 0, 0, 'at_kara_explosive_shadows', 23420),
(9009, 0, 0, 5, 5, 0, 0, 0, 0, 'at_kara_demonic_portal', 23420),
(8963, 0, 2, 5, 5, 0, 0, 0, 0, 'at_kara_fel_flames', 23420),
(8962, 0, 4, 5, 5, 0, 0, 0, 0, 'at_kara_fel_beam', 23420),
(9054, 0, 4, 12, 12, 0, 0, 0, 0, 'at_kara_soul_harvest', 23420);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId` IN (8939, 8940, 8941, 8942, 8943, 8944, 8945, 8946, 9009, 8962, 8963, 9054);
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(8939, 8939, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8940, 8940, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8941, 8941, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8942, 8942, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8943, 8943, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8944, 8944, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8945, 8945, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(8946, 8946, 0, 0, 2382, 0, 0, 30000, 30000, 23420), -- SpellId : 229160
(9009, 9009, 0, 0, 0, 0, 0, 0, 30000, 23420), -- SpellId : 229591
(8962, 8962, 0, 0, 0, 0, 98, 0, 8000, 23420), -- SpellId : 229244
(8963, 8963, 0, 0, 0, 0, 98, 0, 120000, 23420), -- SpellId : 229249
(9054, 9054, 0, 0, 0, 0, 0, 0, 0, 23420); -- SpellId : 229906

DELETE FROM `waypoint_data` WHERE `id` IN (11479010, 11479020);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`) VALUES
(11479010, 0, 3717.774, -2079.090, 850.9378 ),
(11479010, 1, 3905.002, -1978.016, 965.4493 ),
(11479010, 2, 3915.754, -1971.380, 930.000 ),
(11479020, 0, 3826.860, -2041.582, 932.946  ),
(11479020, 1, 3659.277, -2061.302, 973.476  ),
(11479020, 2, 3571.789, -2043.062, 1008.941 ),
(11479020, 3, 3470.271, -2010.690, 1045.000 );






UPDATE creature_template SET scriptname = 'npc_lulin_faded_image_of_niuzao' WHERE entry = 69777;
UPDATE creature_template SET scriptname = 'npc_lulin_star_1' WHERE entry = 70124;
UPDATE creature_template SET scriptname = 'npc_lulin_ice_comet' WHERE entry = 69596;
UPDATE creature_template SET scriptname = 'npc_lulin_ice_comet' WHERE entry = 69704;
UPDATE creature_template SET scriptname = 'npc_lulin_moon_lotus_2' WHERE entry = 69456;
UPDATE creature_template SET scriptname = 'npc_lulin_star' WHERE entry = 69477;
UPDATE creature_template SET scriptname = 'npc_lulin_moon_lotus' WHERE entry = 69340;
UPDATE creature_template SET scriptname = 'boss_suen' WHERE entry = 68904;
UPDATE creature_template SET scriptname = 'boss_lulin' WHERE entry = 68905;
UPDATE creature_template SET scriptname = 'boss_moroes_new' WHERE entry = 114312;
UPDATE creature_template SET scriptname = 'boss_nightbane_new' WHERE entry = 114895;
UPDATE creature_template SET scriptname = 'boss_sisters_of_the_moon' WHERE entry = 118523;
UPDATE creature_template SET scriptname = 'boss_sisters_of_the_moon' WHERE entry = 118374;
UPDATE creature_template SET scriptname = 'boss_sisters_of_the_moon' WHERE entry = 118518;
UPDATE creature_template SET scriptname = 'boss_jes_howlis' WHERE entry = 127484;
UPDATE creature_template SET scriptname = 'boss_knight_captain_valyri' WHERE entry = 130646;
UPDATE creature_template SET scriptname = 'boss_overseer_korgus' WHERE entry = 127503;
UPDATE creature_template SET scriptname = 'boss_sand_queen' WHERE entry = 134927;
UPDATE creature_template SET scriptname = 'boss_lei_shi' WHERE entry = 62983;

UPDATE creature_template SET scriptname = 'boss_ancient_regail' WHERE entry = 60585;
UPDATE creature_template SET scriptname = 'boss_ancient_asani' WHERE entry = 60586;
UPDATE creature_template SET scriptname = 'boss_protector_kaolan' WHERE entry = 60583;
UPDATE creature_template SET scriptname = 'boss_tsulong' WHERE entry = 62442;
UPDATE creature_template SET scriptname = 'boss_dark_animus' WHERE entry = 69427;
UPDATE creature_template SET scriptname = 'boss_durumu_the_forgotten' WHERE entry = 68036;
UPDATE creature_template SET scriptname = 'boss_iron_qon' WHERE entry = 68078;
UPDATE creature_template SET scriptname = 'npc_iron_qon_roshak' WHERE entry = 68079;


UPDATE creature_template SET scriptname = 'npc_iron_qon_quetzal' WHERE entry = 68080;
UPDATE creature_template SET scriptname = 'npc_iron_qon_damren' WHERE entry = 68081;
UPDATE creature_template SET scriptname = 'npc_iron_qon_spear' WHERE entry = 69159;
UPDATE creature_template SET scriptname = 'npc_iron_qon_spear_2' WHERE entry = 68480;
UPDATE creature_template SET scriptname = 'npc_iron_qon_lightning_cloud' WHERE entry = 70660;
UPDATE creature_template SET scriptname = 'npc_iron_qon_roshak' WHERE entry = 68079;
UPDATE creature_template SET scriptname = 'npc_iron_qon_roshak' WHERE entry = 68079;
UPDATE creature_template SET scriptname = 'npc_iron_qon_roshak' WHERE entry = 68079;
UPDATE creature_template SET scriptname = 'npc_iron_qon_roshak' WHERE entry = 68079;
UPDATE creature_template SET scriptname = 'npc_iron_qon_roshak' WHERE entry = 68079;
UPDATE `quest_template_addon` SET `ScriptName`='q_breaking_out' WHERE `ID`=38672;
UPDATE `quest_template_addon` SET `PrevQuestID` = 38672, `ScriptName` = 'q_fel_infusion' WHERE `ID` = 38689; -- Fel Infusion
REPLACE INTO spell_script_names VALUES (408, "spell_rog_kidney_shot");
UPDATE creature_template SET ScriptName = 'mob_return_to_the_terrace' WHERE entry = 65736;
UPDATE `quest_template_addon` SET `ScriptName` = 'q_frozen_in_time' WHERE `ID` = 39685; -- Frozen In Time test script
update `npc_spellclick_spells` set `cast_flags` = 1 where `npc_entry` in (103658, 103655, 96659, 99732, 99731, 99709);
update `creature_template` set `npcflag` = 3 where entry = 92980;

DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=1;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 1, 4446.45, -295.896, -235.339, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=2;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 2, 4456.69, -311.004, -207.667, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=3;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 3, 4460.65, -321.54, -187.735, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=4;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 4, 4454.37, -328.542, -165.568, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=5;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 5, 4447.92, -327.422, -153.965, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=6;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 6, 4448.48, -319.545, -144.887, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=7;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 7, 4454.76, -318.391, -127.121, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=8;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 8, 4455.59, -325.278, -107.721, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=9;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 9, 4449.19, -328.938, -85.2078, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=10;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 10, 4444.79, -325.208, -66.8808, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=11;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 11, 4446.59, -317.991, -27.7739, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=12;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 12, 4451.85, -316.278, -6.79571, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=13;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 13, 4454.85, -320.903, 38.564, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=14;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 14, 4454.28, -324.109, 68.72, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=15;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 15, 4451.35, -326.188, 91.9784, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=16;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 16, 4451.14, -327.823, 136.227, NULL);
DELETE FROM `script_waypoint` WHERE `entry`=99443 AND `pointid`=17;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `point_comment`) VALUES (99443, 17, 4451.24, -351.073, 129.361, NULL);

DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 1, 4325.83, -585.576, -281.745, '4325.832 Y: -585.5758 Z: -281.7451');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 2, 4325.83, -582.826, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 3, 4325.83, -582.826, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 4, 4325.83, -579.576, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');
DELETE FROM `waypoints` WHERE `entry`=92718 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (92718, 5, 4325.83, -576.826, -281.495, '4325.832 Y: -582.8258 Z: -281.4951');

DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 1, 4446.45, -295.896, -235.339, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 2, 4456.69, -311.004, -207.667, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 3, 4460.65, -321.54, -187.735, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 4, 4454.37, -328.542, -165.568, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 5, 4447.92, -327.422, -153.965, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 6, 4448.48, -319.545, -144.887, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=7;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 7, 4454.76, -318.391, -127.121, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=8;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 8, 4455.59, -325.278, -107.721, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=9;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 9, 4449.19, -328.938, -85.2078, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=10;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 10, 4444.79, -325.208, -66.8808, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=11;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 11, 4446.59, -317.991, -27.7739, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=12;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 12, 4451.85, -316.278, -6.79571, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=13;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 13, 4454.85, -320.903, 38.564, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=14;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 14, 4454.28, -324.109, 68.72, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=15;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 15, 4451.35, -326.188, 91.9784, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=16;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 16, 4451.14, -327.823, 136.227, NULL);
DELETE FROM `waypoints` WHERE `entry`=99443 AND `pointid`=17;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99443, 17, 4451.24, -351.073, 129.361, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 1, 4337.47, -590.45, -281.415, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 2, 4336.72, -590.7, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 3, 4334.97, -590.95, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 4, 4333.97, -590.95, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 5, 4332.72, -590.7, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 6, 4331.97, -590.2, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=7;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 7, 4331.97, -589.45, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99631 AND `pointid`=8;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99631, 8, 4332.22, -588.95, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 1, 4315.13, -590.317, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 2, 4316.88, -590.817, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 3, 4317.88, -590.817, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 4, 4318.88, -590.816, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 5, 4319.63, -590.067, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 6, 4320.13, -589.317, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=99632 AND `pointid`=7;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (99632, 7, 4319.88, -588.817, -281.665, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 1, 4331.29, -587.508, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 2, 4331.29, -586.508, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 3, 4331.29, -585.758, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 4, 4331.04, -584.758, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 5, 4330.54, -584.258, -281.862, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963100 AND `pointid`=6;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963100, 6, 4328.46, -583.66, -281.729, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963101 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963101, 1, 4330.08, -548.288, -281.75, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 1, 4319.15, -586.838, -281.673, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 2, 4320.9, -586.338, -281.673, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 3, 4321.9, -585.088, -281.673, NULL);
DELETE FROM `waypoints` WHERE `entry`=9963200 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (9963200, 4, 4323.64, -583.668, -281.753, NULL);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=133511 AND `spell_effect`=133509 AND `type`=0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES (133511, 133509, 0, 'Make the bar go up');

update `creature_template` set `KillCredit1` = 106241 where `entry` = 92290;
update `creature_template` set `KillCredit1` = 106241 where `entry` = 97632;

update `creature_template` set `VehicleId` = 4372 where `entry` = 99443;
update `creature_template` set `ScriptName` = "npc_vault_of_the_wardens_vampiric_felbat" where `entry` = 99443;
update `creature_template` set `KillCredit1` = 106254 where `entry` = 96682;

update `gameobject` set `state` = 1 where `id` = 244588;
update `creature` set `npcflag` = 3 where `guid` = 20542908;
update `creature_template` set `InhabitType` = 4 where `entry` = 99443;
update `creature_template` set `speed_walk` = 7.5 where `entry` = 99443;
update `creature_template` set `speed_run` = 7.5 where `entry` = 99443;

-- Maiev Shadowsong 92718 SAI
SET @ENTRY := 92718;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,38672,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest '' Taken - Say Line 0"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,53,1,92718,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest Taken - Start Waypoint"),
(@ENTRY,0,2,0,58,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Waypoint Finished - Despawn Instant"),
(@ENTRY,0,3,0,19,0,100,0,40253,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest '' Taken - Run Script"),
(@ENTRY,0,4,0,19,0,100,0,38723,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Quest '' Taken - Run Script");

-- Actionlist SAI
SET @ENTRY := 9271800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,85,223661,0,0,0,0,0,7,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Script - Invoker Cast 'Spell Id223661'"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,85,187864,0,0,0,0,0,7,0,0,0,0,0,0,0,"Maiev Shadowsong 92718 - On Script - Invoker Cast 'Spell Id187864'"),
(@ENTRY,9,2,0,0,0,100,0,100000,100000,0,0,62,1468,0,0,0,0,0,7,0,0,0,4084.27,-298.11,-282.07,3.11803,"Maiev Shadowsong 92718 - On Script - Teleport");

-- Kayn's Cell 103658 SAI
SET @ENTRY := 103658;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,11,0,100,0,0,0,0,0,82,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Respawn - Add Npc Flag Spellclick"),
(@ENTRY,0,1,2,73,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Spellclick - Run Script"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Spellclick - Store Targetlist"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,128,0,1,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Respawn");

-- Actionlist SAI
SET @ENTRY := 10365800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,128,3761,1,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Script"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Script - Despawn Instant"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,45,0,1,0,0,0,0,19,99631,5,0,0,0,0,0,"Kayn's Cell 103658 - On Script - Set Data 0 1"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,100,1,0,0,0,0,0,19,99631,5,0,0,0,0,0,"Kayn's Cell 103658 - On Script - Send Target 1");

-- Kayn Sunfury 99631 SAI
SET @ENTRY := 99631;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,53,0,99631,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Data Set 0 1 - Start Waypoint"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Data Set 0 0 - Say Line 0"),
(@ENTRY,0,2,3,40,0,100,0,8,99631,0,0,17,16,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint 8 Reached - Set Emote State 16"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint 0 Reached - Run Script"),
(@ENTRY,0,4,6,58,0,100,0,0,@ENTRY*100+00,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Despawn Instant"),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,33,99326,0,0,0,0,0,12,1,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Quest Credit 'Kill Credit: Reunion Finished - Kayn 99326'"),
(@ENTRY,0,7,0,38,0,100,0,0,2,0,0,53,1,@ENTRY*100+01,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Data Set 0 2 - Start Waypoint"),
(@ENTRY,0,8,9,58,0,100,0,0,@ENTRY*100+01,0,0,45,0,1,0,0,0,0,19,92980,10,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Set Data 0 1"),
(@ENTRY,0,9,0,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Waypoint Finished - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 9963100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,53,0,9963100,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Script - Start Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Script - Set Emote State 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,33,112276,0,0,0,0,0,12,1,0,0,0,0,0,0,"Kayn Sunfury 99631 - On Script - Quest Credit 'Kill Credit: Kayn Picked up Weapons 112276'");

-- Altruis's Cell 103655 SAI
SET @ENTRY := 103655;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,3,11,0,100,0,0,0,0,0,82,16777216,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Respawn - Add Npc Flag Spellclick"),
(@ENTRY,0,1,2,73,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Spellclick - Run Script"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Spellclick - Store Targetlist"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,128,0,1,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Respawn");

-- Actionlist SAI
SET @ENTRY := 10365500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,128,3761,1,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn's Cell 103658 - On Script"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis's Cell 103655 - On Script - Despawn Instant"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,45,0,1,0,0,0,0,19,99632,20,0,0,0,0,0,"Altruis's Cell 103655 - On Script - Set Data 0 1"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,100,1,0,0,0,0,0,19,99632,20,0,0,0,0,0,"Altruis's Cell 103655 - On Script - Send Target 1");

-- Altruis the Sufferer 99632 SAI
SET @ENTRY := 99632;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,53,0,99632,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Data Set 0 1 - Start Waypoint"),
(@ENTRY,0,1,5,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Data Set 0 0 - Say Line 0"),
(@ENTRY,0,2,3,40,0,100,0,7,99632,0,0,17,16,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint 7 Reached - Set Emote State 16"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint 0 Reached - Run Script"),
(@ENTRY,0,4,6,58,0,100,0,0,@ENTRY*100+00,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint Finished - Despawn Instant"),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,33,112287,0,0,0,0,0,12,1,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint Finished - Quest Credit 'Kill Credit: Reunion Finished - Altruis 112287'"),
(@ENTRY,0,7,0,38,0,100,0,0,2,0,0,53,1,@ENTRY*100+01,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Data Set 0 2 - Start Waypoint"),
(@ENTRY,0,8,6,58,0,100,0,0,@ENTRY*100+01,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Waypoint Finished - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 9963200;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,53,0,9963200,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Script - Start Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Script - Set Emote State 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,33,112277,0,0,0,0,0,12,1,0,0,0,0,0,0,"Altruis the Sufferer 99632 - On Script - Quest Credit 'Kill Credit: Altruis Picked up Weapons 112277'");

-- Lever SAI
SET @ENTRY := 244923;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,1,20,0,100,0,38672,0,0,0,45,0,2,0,0,0,0,10,20542674,99631,0,0,0,0,0,"Lever - On Quest '' Finished - Set Data 0 2"),
(@ENTRY,1,1,0,61,0,100,0,0,0,0,0,45,0,2,0,0,0,0,10,20542658,99632,0,0,0,0,0,"Lever - On Quest Finished - Set Data 0 2");

-- Kayn Sunfury 92980 SAI
SET @ENTRY := 92980;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,0,1,0,0,80,0,@ENTRY*100+00,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92980 - On Data Set 0 1 - Run Script");

-- Actionlist SAI
SET @ENTRY := 9298000;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92980 - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,45,0,1,0,0,0,0,19,92986,10,0,0,0,0,0,"Kayn Sunfury 92980 - On Script - Set Data 0 1"),
(@ENTRY,9,2,0,0,0,100,0,2000,2000,0,0,45,0,1,0,0,0,0,19,96847,20,0,0,0,0,0,"Kayn Sunfury 92980 - On Script - Set Data 0 1");

-- Altruis the Sufferer 92986 SAI
SET @ENTRY := 92986;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,2,19,0,100,0,38689,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Quest '' Taken - Say Line 1"),
(@ENTRY,0,1,0,38,0,100,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Data Set 0 1 - Say Line 0"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,85,133508,0,0,0,0,0,7,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Quest Taken - Invoker Cast 'Spell Id133508'"),
(@ENTRY,0,3,0,20,0,100,0,38689,0,0,0,85,133510,0,0,0,0,0,16,0,0,0,0,0,0,0,"Altruis the Sufferer 92986 - On Quest '' Finished - Invoker Cast 'Spell Id133510'");

-- Drelanim Whisperwind 96847 SAI
SET @ENTRY := 96847;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Drelanim Whisperwind 96847 - On Data Set 0 1 - Say Line 0");

-- hackfix i got no idea how to trigger the quest credit else
-- Fel Shocktrooper 92776 SAI
SET @ENTRY := 92776;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,3,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,4,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,5,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,6,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,7,0,32,0,50,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,8,0,32,0,60,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,9,0,32,0,70,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,10,0,32,0,80,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'");

-- Savage Felstalker 92782 SAI
SET @ENTRY := 92782;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,3,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,4,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,5,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,6,0,32,0,100,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,7,0,32,0,50,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,8,0,32,0,60,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,9,0,32,0,70,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'"),
(@ENTRY,0,10,0,32,0,80,0,0,0,0,0,85,133511,66,0,0,0,0,7,0,0,0,0,0,0,0,"Fel Shocktrooper 92776 - On Damaged Between 0-0 - Invoker Cast 'Spell Id133511'");

-- Kayn Sunfury 92984 SAI
SET @ENTRY := 92984;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,19,33344,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 1 - Remove Flag Immune To NPC's"),
(@ENTRY,0,1,3,61,0,100,0,0,0,0,0,2,2804,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 0 - Set Faction 2804"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,92990,20,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 0 - Start Attacking"),
(@ENTRY,0,4,0,38,0,100,0,0,2,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Data Set 0 2 - Run Script"),
(@ENTRY,0,5,0,34,0,100,0,8,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury 92984 - On Reached Point 0 - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 9298400;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,5000,5000,0,0,69,0,0,0,0,0,0,10,20542908,92984,0,0,0,0,0,"Kayn Sunfury 92984 - On Script - Move To Closest Creature 'Kayn Sunfury'");

-- Altruis the Sufferer 92985 SAI
SET @ENTRY := 92985;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,0,1,0,0,19,33344,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 1 - Remove Flag Immune To NPC's"),
(@ENTRY,0,1,3,61,0,100,0,0,0,0,0,2,2804,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 0 - Set Faction 2804"),
(@ENTRY,0,2,0,11,0,100,0,0,0,0,0,81,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Respawn - Set Npc Flags Gossip & Questgiver"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,97632,20,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 0 - Start Attacking"),
(@ENTRY,0,4,0,38,0,100,0,0,2,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Data Set 0 2 - Run Script"),
(@ENTRY,0,5,0,34,0,100,0,8,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer 92985 - On Reached Point 0 - Despawn Instant");

-- Sledge 92990 SAI
SET @ENTRY := 92990;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,4,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,20542913,92984,0,0,0,0,0,"Sledge 92990 - On Aggro - Set Data 0 1"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,10,20542914,92985,0,0,0,0,0,"Sledge 92990 - On Aggro - Set Data 0 1"),
(@ENTRY,0,2,3,4,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Sledge 92990 - On Aggro - Store Targetlist"),
(@ENTRY,0,3,4,61,0,100,0,0,0,0,0,100,1,0,0,0,0,0,19,97632,20,0,0,0,0,0,"Sledge 92990 - On Aggro - Send Target 1"),
(@ENTRY,0,4,0,61,0,100,0,0,0,0,0,45,0,1,0,0,0,0,19,97632,20,0,0,0,0,0,"Sledge 92990 - On Aggro - Set Data 0 1"),
(@ENTRY,0,5,6,6,0,100,0,0,0,0,0,45,0,2,0,0,0,0,10,20542913,92984,0,0,0,0,0,"Sledge 92990 - On Just Died - Set Data 0 2"),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,45,0,2,0,0,0,0,10,20542914,92985,0,0,0,0,0,"Sledge 92990 - On Just Died - Set Data 0 2");

-- Actionlist SAI
SET @ENTRY := 9298500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,5000,5000,0,0,69,0,0,0,0,0,0,10,20542909,92985,0,0,0,0,0,"Altruis the Sufferer 92985 - On Script - Move To Closest Creature 'Altruis the Sufferer'");

-- Crusher 97632 SAI
SET @ENTRY := 97632;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,0,1,0,0,49,0,0,0,0,0,0,12,1,0,0,0,0,0,0,"Crusher 97632 - On Data Set 0 1 - Start Attacking");

-- Countermeasures 99732 SAI
SET @ENTRY := 99732;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,204588,0,0,0,33,99732,0,0,0,0,0,7,0,0,0,0,0,0,0,"Countermeasures 99709 - On Spellhit 'Spell Id204588' - Quest Credit 'Countermeasures 99709'");

-- Countermeasures 99731 SAI
SET @ENTRY := 99731;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,204588,0,0,0,33,99731,0,0,0,0,0,7,0,0,0,0,0,0,0,"Countermeasures 99709 - On Spellhit 'Spell Id204588' - Quest Credit 'Countermeasures 99709'");

-- Countermeasures 99709 SAI
SET @ENTRY := 99709;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,204588,0,0,0,33,99709,0,0,0,0,0,7,0,0,0,0,0,0,0,"Countermeasures 99709 - On Spellhit 'Spell Id204588' - Quest Credit 'Countermeasures 99709'");

DELETE FROM `gameobject` WHERE `guid` = 20375675;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(20375675, 243965, 1481, 7705, 7747, 0, 0, 0, 1813.36, 1543.43, 88.3732, 5.74205, 0, 0, 0.267278, -0.963619, 7200, 255, 1, 22423);

DELETE FROM `creature_queststarter` WHERE `id` IN (92718, 92980, 92986, 92984, 97643, 96675, 97644, 97978, 97296, 114562, 116704, 99254);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES  
(92718, 38672), -- breaking out
(92980, 38690), -- rise of the illidari
(92986, 38689), -- fel infusion
(92718, 38723), -- stop guldan (Havoc) (https://www.wowhead.com/search?q=stop+gul%27dan)
(92718, 40253), -- stop guldan (Vengeance)
(92984, 39682), -- grand theft felbat
(97643, 39685), -- frozen in time
(97643, 39684), -- beam me up
(96675, 39683), -- forged in fire (Havoc)
(96675, 40254), -- forged in fire (Vengeance)
(96675, 39686), -- all the way up
(97644, 40373), -- -- a new direction (https://www.wowhead.com/search?q=A+New+Direction)
(97644, 39694), -- between us and freedom (havoc H)
(97644, 39688), -- between us and freedom (havoc A)
(97644, 40255), -- between us and freedom (vengeance A)
(97644, 40256), -- between us and freedom (vengeance H)
(97978, 39689), -- Illidari we are leaving (A)
(97978, 39690), -- Illidari we are leaving (H)
(97296, 40976), -- Audience with Warchief
(97296, 39691), -- The Call of War
(114562, 44663), -- Blink of an Eye
(116704, 39047), -- call of the Illidari
(99254, 40816), -- Power To Survive (Altruis)
(99254, 41120), -- Making Arrangements
(99254, 41121), -- By Any Means
(99254, 41119); -- The Hunt

DELETE FROM `creature_questender` WHERE `quest` IN (40253, 38723);
INSERT INTO `creature_questender` (`id`,`quest`) VALUES (92984, 40253), (92984, 38723);

UPDATE `creature_template` SET `ScriptName` = 'npc_kayn_3' WHERE `entry` = 96665;
UPDATE `creature_template` SET `ScriptName` = 'npc_altruis' WHERE `entry` = 92986;
UPDATE `creature_template` SET `ScriptName` = 'npc_fel_infusion' WHERE `entry` = 92776;
UPDATE `creature_template` SET `ScriptName` = 'npc_fel_infusion' WHERE `entry` = 92782;
UPDATE `creature_template` SET `ScriptName` = 'npc_kayn_cell' WHERE `entry` = 103658;
UPDATE `creature_template` SET `ScriptName` = 'npc_altruis_cell' WHERE `entry` = 103655;
UPDATE `creature_template` SET `ScriptName` = 'npc_korvas_bloodthorn' WHERE `entry` = 97644;
UPDATE `creature_template` SET `ScriptName` = 'npc_khadgar' WHERE `entry` = 97978;
UPDATE `creature_template` SET `ScriptName` = 'npc_maiev_shadowsong' WHERE `entry` = 92718;
UPDATE `creature_template` SET `ScriptName` = 'npc_sledge' WHERE `entry` = 92990;
UPDATE `creature_template` SET `ScriptName` = 'npc_crusher' WHERE `entry` = 97632;
UPDATE `creature_template` SET `ScriptName` = 'npc_immolanth' WHERE `entry` = 96682;
UPDATE `creature_template` SET `ScriptName` = 'npc_vow_ashgolm' WHERE `entry` = 96681;
UPDATE `creature_template` SET `ScriptName` = 'npc_bastillax' WHERE `entry` = 96783;
UPDATE `creature_template` SET `ScriptName` = 'npc_legion_portal' WHERE `entry` = 99501;
UPDATE `creature_template` SET `ScriptName` = 'npc_cyana_immolanth_fight' WHERE `entry` = 96672;
UPDATE `creature_template` SET `ScriptName` = 'npc_khadgars_upgraded_servant' WHERE `entry` = 114562;

UPDATE `creature` SET `ScriptName` = 'npc_kayn_sledge_fight' WHERE `guid` = 20542913;
UPDATE `creature` SET `ScriptName` = 'npc_altruis_crusher_fight' WHERE `guid` = 20542914;

UPDATE `gameobject_template` SET `ScriptName` = 'go_reflective_mirror' WHERE `entry` = 244449;
UPDATE `gameobject_template` SET `ScriptName` = 'go_pool_of_judgements' WHERE `entry` = 244455;
UPDATE `gameobject_template` SET `ScriptName` = 'go_warden_ascent' WHERE `entry` = 244644;

UPDATE `creature_template` SET `speed_walk` = 1, `speed_run` = 1 WHERE `entry` = 99632;
UPDATE `creature_template` SET `speed_walk` = 1, `speed_run` = 1 WHERE `entry` = 99631;
UPDATE `creature_template` SET `npcflag` = 3 WHERE `entry` IN (92980, 92984);
UPDATE `creature_template` SET `npcflag` = 1 WHERE `entry` IN (103658, 103655);
UPDATE `creature_template` SET `HealthModifier` = 20 WHERE `entry` = 96682;

UPDATE `quest_template_addon` SET `PrevQuestID` = 38672 WHERE `ID` = 38690; -- Rise of the Illidari
UPDATE `quest_template_addon` SET `PrevQuestID` = 38672, `ScriptName` = 'q_fel_infusion' WHERE `ID` = 38689; -- Fel Infusion
UPDATE `quest_template_addon` SET `ScriptName` = 'q_frozen_in_time' WHERE `ID` = 39685; -- Frozen In Time test script
UPDATE `quest_template_addon` SET `PrevQuestID` = 38690, `NextQuestID` = 39682, `ExclusiveGroup` = 38723 WHERE `ID` = 38723; -- Stop Guldan
UPDATE `quest_template_addon` SET `PrevQuestID` = 38690, `NextQuestID` = 39682, `ExclusiveGroup` = 38723 WHERE `ID` = 40253; -- Stop Guldan

UPDATE `quest_template_addon` SET `PrevQuestID` = 39682, `NextQuestID` = 39686, `ExclusiveGroup` = -39685 WHERE `ID` = 39685; -- Frozen In Time
UPDATE `quest_template_addon` SET `PrevQuestID` = 39682, `NextQuestID` = 39686, `ExclusiveGroup` = -39685 WHERE `ID` = 39684; -- Beam Me Up

UPDATE `quest_template_addon` SET `PrevQuestID` = 39682, `NextQuestID` = 39686, `ExclusiveGroup` = 39683 WHERE `ID` = 39683; -- Forged In Fire
UPDATE `quest_template_addon` SET `PrevQuestID` = 39682, `NextQuestID` = 39686, `ExclusiveGroup` = 39683 WHERE `ID` = 40254; -- Forged In Fire

UPDATE `quest_template_addon` SET `PrevQuestID` = 39686 WHERE `ID` = 40373; -- A New Direction

UPDATE `quest_template_addon` SET `PrevQuestID` = 40373, `NextQuestID` = 39690, `ExclusiveGroup` = 39694 WHERE `ID` = 39694; -- between us and freedom
UPDATE `quest_template_addon` SET `PrevQuestID` = 40373, `NextQuestID` = 39689, `ExclusiveGroup` = 39694 WHERE `ID` = 39688; -- between us and freedom
UPDATE `quest_template_addon` SET `PrevQuestID` = 40373, `NextQuestID` = 39689, `ExclusiveGroup` = 39694 WHERE `ID` = 40255; -- between us and freedom
UPDATE `quest_template_addon` SET `PrevQuestID` = 40373, `NextQuestID` = 39690, `ExclusiveGroup` = 39694 WHERE `ID` = 40256; -- between us and freedom

UPDATE `creature_loot_template` SET `Chance` = 82, `QuestRequired` = 0, `LootMode` = 1, `GroupId` = 0, `MinCount` = 1, `MaxCount` = 1 WHERE `Entry` = 97370 AND `Item` = 133313;

DELETE FROM `spell_script_names` WHERE `spell_id` = 204588; -- activate countermeasures
DELETE FROM `npc_spellclick_spells` WHERE `spell_id` = 204588;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (204588, 'spell_activate_countermeasure');
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES (99709, 204588, 1, 0), (99731, 204588, 1, 0), (99732, 204588, 1, 0);

DELETE FROM `gameobject` WHERE `guid` = 210120188; -- Vault of Silence door
DELETE FROM `gameobject` WHERE `guid` = 20373116; -- small chest
DELETE FROM `gameobject` WHERE `guid` = 20373163; -- small chest
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnDifficulties`,`phaseUseFlags`,`PhaseId`,`PhaseGroup`,`terrainSwapMap`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`ScriptName`,`VerifiedBuild`) VALUES
(210120188, 244925, 1468, 7814, 7814, 0, 0, 0, 0, -1, 4325.66, -576.49, -281.783, 4.65537, -0, -0, -0.726975, 0.686664, 300, 255, 1, 0, '', 22423);
DELETE FROM `gameobject` WHERE `guid` IN (20373129, 20373134); -- Vault of Betrayer doors
UPDATE `gameobject` SET `state` = 1 WHERE `guid` IN (20373013, 20373001, 20373016, 20373071, 20373066, 20373082, 20372994, 20373064);
DELETE FROM `creature` WHERE `guid` IN (20542659, 20542665);

DELETE FROM `creature_text` WHERE `CreatureID` IN (92718, 99632, 99631, 96682);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(92718, 0, 0, 'I will need your help to stop Gul\'dan. Free the other Illidari, quickly!', 12, 0, 100, 0, 0, 57338, 0, 0, 'Maiev to Player'),
(99632, 0, 0, 'How many years were wasted in that cell?', 12, 0, 100, 0, 0, 57343, 0, 0, 'Altruis on freed'),
(99631, 0, 0, 'I am awake? How is this possible?', 12, 0, 100, 0, 0, 57294, 0, 0, 'Kayn on freed'),
(99631, 1, 0, 'Why did you let THAT one free? He opposed Lord Illidan!', 12, 0, 100, 0, 0, 57320, 0, 0, 'Kayn\'s response of Altruis freed'),
(99632, 1, 0, 'It has been a long time, Kayn. I still believe in the cause. But, I do not blindly follow anyone... even Illidan.', 12, 0, 100, 0, 0, 57285, 0, 0, 'Altruis to Kayn'),
(99631, 2, 0, 'Why would Maiev free us? There could only be one reason...', 12, 0, 100, 0, 0, 57387, 0, 0, 'Kayns to Player when quest rewarded'),
(96682, 0, 0, 'Join me, demon hunters. I will increase your power beyond imagining!', 12, 0, 100, 0, 0, 57358, 0, 0, 'Immolanth on 60 pct'),
(96682, 1, 0, 'The wardens would keep us rotting here forever. Why do you fight on their side?', 12, 0, 100, 0, 0, 57359, 0, 0, 'Immolanth on 20 pct');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=92718;
DELETE FROM `smart_scripts` WHERE `entryorguid`=92718 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(92718,0,0,0,19,0,100,0,38672,0,0,0,66,0,0,0,0,0,0,21,5,0,0,0,0,0,0,"Maiev - On Quest 'Breaking Out' Taken - Set Orientation Closest Player"),
(92718,0,1,0,19,0,100,0,38672,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Maiev - On Quest 'Breaking Out' Taken - Say Line 0");

UPDATE `quest_template_addon` SET `ScriptName`='q_breaking_out' WHERE `ID`=38672;
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=92718;

-- Wrath Lord Lekos SAI
SET @LORD_LEKOS := 97069;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@LORD_LEKOS;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@LORD_LEKOS AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@LORD_LEKOS,0,0,0,0,0,100,1,0,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Wrath-Lord Lekos - In Combat - Say Line 0 (No Repeat)"),
(@LORD_LEKOS,0,1,0,0,0,100,0,5000,8000,12000,15000,11,200918,0,0,0,0,0,2,0,0,0,0,0,0,0,"Wrath-Lord Lekos - In Combat - Cast 'Fel Shield Blast'"),
(@LORD_LEKOS,0,2,0,0,0,100,0,10000,10000,10000,25000,11,200929,0,0,0,0,0,2,0,0,0,0,0,0,0,"Wrath-Lord Lekos - In Combat - Cast 'Furious Roar'"),
(@LORD_LEKOS,0,3,0,2,0,100,0,0,40,22000,25000,11,200936,2,0,0,0,0,1,0,0,0,0,0,0,0,"Wrath-Lord Lekos - Between 0-40% Health - Cast 'Spiked Bulwark'");

DELETE FROM `creature_text` WHERE `CreatureID` = 97069;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(97069, 0, 0, 'Lord Gul\'dan will lead us to victory. You cannot stop us.', 12, 0, 100, 0, 0, 57312, 0, 0, 'Wrath Lord Lekos to Player'),
(97069, 0, 1, 'Your world is the last standing. It, too, will be purged.', 12, 0, 100, 0, 0, 57298, 0, 0, 'Wrath Lord Lekos to Player'),
(97069, 0, 2, 'All goes according to HIS plan.', 12, 0, 100, 0, 0, 57392, 0, 0, 'Wrath Lord on Death');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` =0 AND `SourceEntry` IN (38723, 40253, 39683, 40254, 39694, 39688, 40255, 40256) AND `ConditionValue1` IN (212612, 212613);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
('19', '0', '38723', '0', '0', '1', '0', '212612', '0', '0', '0', '0', '0', '', 'Allow Quest "Stop Guldan" 38723 only for Havoc DH'),
('19', '0', '40253', '0', '0', '1', '0', '212613', '0', '0', '0', '0', '0', '', 'Allow Quest "Stop Guldan" only for Vengeance DH'),
('19', '0', '39683', '0', '0', '1', '0', '212612', '0', '0', '0', '0', '0', '', 'Allow Quest "Forged In Fire" only for Havoc DH'),
('19', '0', '40254', '0', '0', '1', '0', '212613', '0', '0', '0', '0', '0', '', 'Allow Quest "Forged In Fire" only for Vengeance DH'),
('19', '0', '39694', '0', '0', '1', '0', '212612', '0', '0', '0', '0', '0', '', 'Allow Quest "Between Us And Freedom" only for Havoc DH'),
('19', '0', '39688', '0', '0', '1', '0', '212612', '0', '0', '0', '0', '0', '', 'Allow Quest "Between Us And Freedom" only for Havoc DH'),
('19', '0', '40255', '0', '0', '1', '0', '212613', '0', '0', '0', '0', '0', '', 'Allow Quest "Between Us And Freedom" only for Vengeance DH'),
('19', '0', '40256', '0', '0', '1', '0', '212613', '0', '0', '0', '0', '0', '', 'Allow Quest "Between Us And Freedom" only for Vengeance DH');

-- Kethrazor SAI
SET @KETHRAZOR := 96997;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@KETHRAZOR;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@KETHRAZOR AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@KETHRAZOR,0,0,0,0,0,100,1,0,0,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Kethrazor - In Combat - Say Line 0 (No Repeat)"),
(@KETHRAZOR,0,1,0,0,0,100,0,5000,5000,12000,15000,11,201038,0,0,0,0,0,2,0,0,0,0,0,0,0,"Kethrazor - In Combat - Cast 'Mind Spike'"),
(@KETHRAZOR,0,2,0,0,0,100,0,10000,10000,18000,25000,11,201033,0,0,0,0,0,2,0,0,0,0,0,0,0,"Kethrazor - In Combat - Cast 'Seeping Eyes of Kethrazor'"),
(@KETHRAZOR,0,3,0,0,0,100,0,20000,22000,30000,32000,11,200999,0,0,0,0,0,2,0,0,0,0,0,0,0,"Kethrazor - In Combat - Cast 'Explosive Eyes of Kethrazor'"),
(@KETHRAZOR,0,4,0,6,0,100,0,0,0,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Kethrazor - On Just Died - Say Line 1");

DELETE FROM `creature_text` WHERE `CreatureID` = 96997;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
('96997', '0', '0', 'Your efforts are fruitless, demon hunter. Lord Gul\'dan sees all.', '14', '0', '100', '0', '0', '57341', '0', '0', 'Kethrazor to Player'),
('96997', '1', '0', 'In the end, you will all succumb to the Legion...', '14', '0', '100', '0', '0', '57368', '0', '0', 'Kethrazor to Player');

-- Seeping Inquisitor Eye SAI
SET @SEEPING_EYE := 101907;
UPDATE `creature_template` SET `faction`=90, `AIName`="SmartAI" WHERE `entry`=@SEEPING_EYE;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@SEEPING_EYE AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@SEEPING_EYE,0,0,0,0,0,100,0,0,0,0,0,11,201035,0,0,0,0,0,2,0,0,0,0,0,0,0,"Seeping Inquisitor Eye - In Combat - Cast 'Some Aura'");

-- Explosive Inquisitor Eye SAI
SET @EXPLOSIVE_EYE := 101901;
UPDATE `creature_template` SET `faction`=90, `AIName`="SmartAI" WHERE `entry`=@EXPLOSIVE_EYE;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@EXPLOSIVE_EYE AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@EXPLOSIVE_EYE,0,0,0,0,0,100,1,0,0,0,0,11,201028,0,0,0,0,0,1,0,0,0,0,0,0,0,"Explosive Inquisitor Eye - In Combat - Cast 'Explosive Shadows' (No Repeat)"),
(@EXPLOSIVE_EYE,0,1,0,0,0,100,1,0,0,0,0,11,201015,0,0,0,0,0,1,0,0,0,0,0,0,0,"Explosive Inquisitor Eye - In Combat - Cast 'Fixate' (No Repeat)");

-- Glazer SAI
SET @GLAZER := 96680;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@GLAZER;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@GLAZER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@GLAZER,0,0,0,1,0,100,1,1000,1000,1000,1000,11,191915,2,0,0,0,0,1,0,0,0,0,0,0,0,"Glazer - Out of Combat - Cast 'Focusing' (No Repeat)"),
(@GLAZER,0,1,0,1,0,100,0,2000,2000,15000,17000,11,196460,2,0,0,0,0,1,0,0,0,0,0,0,0,"Glazer - Out of Combat - Cast 'Lingering Gaze' (No Repeat)"),
(@GLAZER,0,2,0,1,0,100,0,3000,4000,20000,21000,11,196462,2,0,0,0,0,1,0,0,0,0,0,0,0,"Glazer - Out of Combat - Cast 'Pulse'");

-- Altruis near Glazer SAI
SET @ALTRUIS := 102391;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ALTRUIS;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ALTRUIS AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ALTRUIS,0,0,0,10,0,100,1,1,30,0,0,1,0,10000,0,0,0,0,18,30,0,0,0,0,0,0,"Altruis - Within 1-30 Range Out of Combat LoS - Say Line 0 (No Repeat)"),
(@ALTRUIS,0,1,0,20,0,100,0,39684,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis - On Quest 'Beam Me Up' Completed - Say Line 1");

DELETE FROM `creature_text` WHERE `CreatureID` = @ALTRUIS;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@ALTRUIS,0,0,'$n, the creature\'s destructive beam is focused upon me. Redirect the final mirror to shatter its shield!',12,0,100,0,0,57332,0,0,'Altruis to Player'),
(@ALTRUIS,1,0,'The shield is broken! Quickly, everyone out while the creature is stunned!',12,0,100,0,0,57333,0,0,'Altruis to Player');

-- Mirana Starlight SAI
SET @MIRANA := 99451;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@MIRANA;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@MIRANA AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@MIRANA,0,0,0,0,0,100,0,0,0,3400,4700,11,195783,64,0,0,0,0,2,0,0,0,0,0,0,0,"Mirana - In Combat - Cast 'Fan of Blades'"),
(@MIRANA,0,1,0,0,0,100,0,5000,8000,12000,15000,11,191026,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mirana - In Combat - Cast 'Throwing Blade'");

-- Drelanin Whisperwind SAI
SET @DRELANIN := 96847;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@DRELANIN;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@DRELANIN AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@DRELANIN,0,0,0,10,0,100,1,1,15,0,0,1,0,10000,0,0,0,0,18,15,0,0,0,0,0,0,"Drelanin - Within 1-15 Range Out of Combat LoS - Say Line 0 (No Repeat)"),
(@DRELANIN,0,1,0,0,0,100,0,5000,8000,18000,22000,11,189469,0,0,0,0,0,2,0,0,0,0,0,0,0,"Drelanin - In Combat - Cast 'Turn Kick'"),
(@DRELANIN,0,2,0,0,0,100,0,2000,5000,12000,15000,11,195783,0,0,0,0,0,2,0,0,0,0,0,0,0,"Drelanin - In Combat - Cast 'Fan of Blades'");

DELETE FROM `creature_text` WHERE `CreatureID` = @DRELANIN;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@DRELANIN,0,0,'I hope Maiev was right about freeing you, demon hunter. Go on - I will hold the line.',12,0,100,0,0,57291,0,0,'Drelanin to Player');

SET @JACE := 96653;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@JACE;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@JACE AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@JACE,0,0,0,10,0,100,1,1,30,0,0,1,0,10000,0,0,0,0,18,30,0,0,0,0,0,0,"Jace Darkweaver - Within 1-30 Range Out of Combat LoS - Say Line 0 (No Repeat)");

DELETE FROM `creature_text` WHERE `CreatureID` = @JACE;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@JACE,0,0,'Kayn and Altruis have already gone ahead. Take this felbat and join them.',12,0,100,0,0,57299,0,0,'Jace Darkweaver to Player');

SET @KORVAS_1 := 97643;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@KORVAS_1;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@KORVAS_1 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@KORVAS_1,0,0,0,10,0,100,1,1,15,0,0,1,0,10000,0,0,0,0,18,15,0,0,0,0,0,0,"Korvas Bloodthorn - Within 1-15 Range Out of Combat LoS - Say Line 0 (No Repeat)"),
(@KORVAS_1,0,1,0,19,0,100,0,39685,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Korvas Bloodthorn - On Quest 'Frozen In Time' Taken - Say Line 1"),
(@KORVAS_1,0,2,1,52,0,100,0,1,@KORVAS_1,0,0,1,2,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Korvas Bloodthorn - On Text 1 Over - Say Line 2");

DELETE FROM `creature_text` WHERE `CreatureID` = @KORVAS_1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@KORVAS_1,0,0,'This is sheer madness! The Wardens have lost control of their prison',12,0,100,0,0,57323,0,0,'Korvas Bloodthorn to Player'),
(@KORVAS_1,1,0,'Aside from us, the most powerful prisoners were sealed on this level. WERE being the operative word.',12,0,100,0,0,57324,0,0,'Korvas Bloodthorn to Player'),
(@KORVAS_1,2,0,'We cannot afford to lose Kayn and Altruis here. Waste no time and help them, quickly!',12,0,100,0,0,57397,0,0,'Korvas Bloodthorn to Player');

-- Cyana near Immolanth SAI
SET @CYANA := 96672;
DELETE FROM `creature_text` WHERE `CreatureID` = @CYANA;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@CYANA,0,0,'That was too close for my liking. $n, the demon\'s power is yours.',12,0,100,0,0,57357,0,0,'Cyana to Player');

-- Kayn near Ashgolm SAI
SET @KAYN_1 := 102393;
DELETE FROM `creature_text` WHERE `CreatureID` = @KAYN_1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@KAYN_1,0,0,'Well done! The monster is contained. Let\'s meet up with the others.',12,0,100,0,0,55229,0,0,'Kayn to Player');

SET @ALLARI_1 := 96675;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ALLARI_1;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ALLARI_1 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ALLARI_1,0,0,0,19,0,100,0,39686,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Allari the Souleater - On Quest 'All the way Up' Taken - Say Line 0");

DELETE FROM `creature_text` WHERE `CreatureID` = @ALLARI_1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@ALLARI_1,0,0,'The elevator will be here shortly. The others are waiting for you above.',12,0,100,0,0,57356,0,0,'Allari the Souleater to Player');

SET @KORVAS_2 := 97644;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@KORVAS_2;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@KORVAS_2 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@KORVAS_2,0,0,0,10,0,100,1,1,15,0,0,1,0,10000,0,0,0,0,18,15,0,0,0,0,0,0,"Korvas Bloodthorn - Within 1-15 Range Out of Combat LoS - Say Line 0 (No Repeat)"),
(@KORVAS_2,0,1,0,19,0,100,0,39694,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Korvas Bloodthorn - On Quest 'Between Us And Freedom - Say Line 1"),
(@KORVAS_2,0,2,0,19,0,100,0,39688,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Korvas Bloodthorn - On Quest 'Between Us And Freedom - Say Line 1"),
(@KORVAS_2,0,3,0,19,0,100,0,40255,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Korvas Bloodthorn - On Quest 'Between Us And Freedom - Say Line 1"),
(@KORVAS_2,0,4,0,19,0,100,0,40256,0,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Korvas Bloodthorn - On Quest 'Between Us And Freedom - Say Line 1");

DELETE FROM `creature_text` WHERE `CreatureID` = @KORVAS_2;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@KORVAS_2,0,0,'Stop this foolish bickering. We do not have time for this!',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player'),
(@KORVAS_2,1,0,'Let\'s keep moving. The others are in the chamber just ahead.',12,0,100,0,0,57361,0,0,'Korvas Bloodthorn to Player');

SET @KHADGAR_1 := 97978;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@KHADGAR_1;
DELETE FROM `creature_text` WHERE `CreatureID` = @KHADGAR_1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(@KHADGAR_1,0,0,'I am Arhmage Khadgar, leader of the Kirin Tor.',12,0,100,0,0,57302,0,0,'Khadgar to Player'),
(@KHADGAR_1,1,0,'Demon Hunters, Azeroth needs your help.',12,0,100,0,0,57364,0,0,'Khadgar to Player');

-- Stop Guldan Scene
DELETE FROM `scene_template` WHERE `SceneId`=1016 AND `ScriptPackageID`=1423;
INSERT INTO `scene_template` (`SceneId`,`Flags`,`ScriptPackageID`,`ScriptName`) VALUES
(1016,26,1423,'scene_guldan_stealing_illidan_corpse');

DELETE FROM `gameobject_template` WHERE `entry` IN (246559, 246560, 246562, 246561, 246557, 246556, 246555, 246558);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `RequiredLevel`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES
('246559','3','33263','Small Treasure Chest','','Opening','','1','57','246559','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64310','0','0','98','','','22423'),
('246560','3','33263','Small Treasure Chest','','Opening','','1','57','246560','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64311','0','0','98','','','22423'),
('246562','3','33263','Small Treasure Chest','','Opening','','1','57','246562','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64313','0','0','98','','','22423'),
('246561','3','33263','Small Treasure Chest','','Opening','','1','57','246561','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64312','0','0','98','','','22423'),
('246557','3','33263','Small Treasure Chest','','Opening','','1','57','246557','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64308','0','0','98','','','22423'),
('246556','3','33263','Small Treasure Chest','','Opening','','1','57','246556','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64307','0','0','98','','','22423'),
('246555','3','33263','Small Treasure Chest','','Opening','','1','57','246555','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64306','0','0','98','','','22423'),
('246558','3','33263','Small Treasure Chest','','Opening','','1','57','246558','1','0','0','0','0','0','0','0','0','0','0','1','21400','0','0','0','110','3','0','0','0','0','0','0','0','0','0','0','64309','0','0','98','','','22423');

DELETE FROM `gameobject_loot_template` WHERE `entry` IN (246559, 246560, 246562, 246561, 246557, 246556, 246555, 246558);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `reference`, `chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
('246560', '129196', '0', '100', '0', '1', '1', '2', '3', 'Legion Heartstone'),
('246560', '129210', '0', '99', '0', '1', '0', '2', '2', 'Fel Crystal Fragments'),
('246562', '129210', '0', '100', '0', '1', '0', '2', '3', 'Fel Crystal Fragments'),
('246559', '129196', '0', '100', '0', '1', '0', '2', '4', 'Legion Heartstone'),
('246561', '129196', '0', '100', '0', '1', '1', '2', '3', 'Legion Heartstone'),
('246561', '129210', '0', '99', '0', '1', '0', '2', '2', 'Fel Crystal Fragments'),
('246557', '129196', '0', '100', '0', '1', '1', '2', '3', 'Legion Heartstone'),
('246557', '129210', '0', '99', '0', '1', '0', '2', '2', 'Fel Crystal Fragments'),
('246555', '129210', '0', '100', '0', '1', '0', '2', '3', 'Fel Crystal Fragments'),
('246558', '129196', '0', '100', '0', '1', '1', '2', '3', 'Legion Heartstone'),
('246558', '129210', '0', '99', '0', '1', '0', '2', '2', 'Fel Crystal Fragments'),
('246556', '129210', '0', '100', '0', '1', '0', '2', '3', 'Fel Crystal Fragments');

UPDATE `gameobject_template` SET `name`='Small Treasure Chest', `castBarCaption`='Opening' WHERE `entry` IN (244689, 246309, 246353);
UPDATE `creature` SET `spawntimesecs`=120 WHERE `id`=92776 AND `map`=1468;
UPDATE `creature` SET `spawntimesecs`=120 WHERE `id`=92782 AND `map`=1468;

DELETE FROM `graveyard_zone` WHERE `GhostZone` IN (7871, 7866, 7864, 7819, 7873);
INSERT INTO `graveyard_zone` (`ID`, `GhostZone`, `Faction`, `Comment`) VALUES
(5135, 7871, 0, 'TombOfThePenitent'),
(5136, 7866, 0, 'TheDemonWard'),
(5137, 7864, 0, 'ChamberOfNight'),
(5344, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 1'),
(5345, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 2'),
(5346, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 3'),
(5347, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 4'),
(5348, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 5'),
(5349, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 6'),
(5350, 7819, 0, 'VaultOfTheBetrayer - Corpse Catcher 7'),
(5356, 7873, 0, 'VaultOfSilence');

SET @CGUID := 280000000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+250 AND @CGUID+260;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES
(@CGUID+250, 39660, 1468, 7814, 7871, 0, 0, 0, 0, 0, 4330.025, -315.0139, -283.7572, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: TombOfThePenitent - Difficulty: 0)
(@CGUID+251, 39660, 1468, 7814, 7873, 0, 0, 0, 0, 0, 4326.024, -598.0903, -281.8333, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfSilence - Difficulty: 0)
(@CGUID+252, 39660, 1468, 7814, 7866, 0, 0, 0, 0, 0, 4442.008, -392.8559, 125.8984, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: TheDemonWard - Difficulty: 0)
(@CGUID+253, 39660, 1468, 7814, 7864, 0, 0, 0, 0, 0, 4184.598, -442.408, 269.8069, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: ChamberOfNight - Difficulty: 0)
(@CGUID+254, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4079.568, -326.8594, -281.1126, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher1 - Difficulty: 0)
(@CGUID+255, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4050.474, -328.2865, -281.46, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher2 - Difficulty: 0)
(@CGUID+256, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4021.382, -327.4861, -281.1126, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher3 - Difficulty: 0)
(@CGUID+257, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4020.778, -297.4115, -281.46, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher4 - Difficulty: 0)
(@CGUID+258, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4023.035, -270.283, -281.1126, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher5 - Difficulty: 0)
(@CGUID+259, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4050.985, -268.4618, -281.4599, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549), -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher6 - Difficulty: 0)
(@CGUID+260, 39660, 1468, 7814, 7819, 0, 0, 0, 0, 0, 4079.085, -269.4063, -281.1126, 5.090357, 120, 0, 0, 0, 0, 0, 0, 0, 0, 25549); -- Spirit Healer (Area: VotW: VaultOfTheBetrayer - Catcher7 - Difficulty: 0)
UPDATE `creature` SET `npcflag`=16384 WHERE `guid` BETWEEN @CGUID+250 AND @CGUID+260;

UPDATE `creature` SET `npcflag`=0 WHERE `guid`=20542913;

-- Altruis the Sufferer 3 SAI
SET @ALRTUIS_FEL_INFUSION := 92986;
UPDATE `creature` SET `spawntimesecs`=15 WHERE `guid`=20542724 AND `id`=@ALRTUIS_FEL_INFUSION;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ALRTUIS_FEL_INFUSION;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ALRTUIS_FEL_INFUSION AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ALRTUIS_FEL_INFUSION,0,0,0,10,0,100,1,1,15,25000,25000,1,0,10000,0,0,0,0,18,15,0,0,0,0,0,0,"Altruis the Sufferer - Within 1-15 Range Out of Combat LoS - Say Line 1 (No Repeat)"),
(@ALRTUIS_FEL_INFUSION,0,1,0,19,0,100,0,38689,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer - On Quest 'Fel Infusion' Taken - Say Line 0"),
(@ALRTUIS_FEL_INFUSION,0,2,2,52,0,100,0,0,@ALRTUIS_FEL_INFUSION,0,0,12,@ALRTUIS_FEL_INFUSION*100+00,6,0,0,0,0,8,0,0,0,4321.75,-547.861,-281.497,1.68118,"Altruis the Sufferer - On Text 0 Over - Summon Creature 'Altruis the Sufferer'"),
(@ALRTUIS_FEL_INFUSION,0,3,0,61,0,100,0,0,@ALRTUIS_FEL_INFUSION,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Altruis the Sufferer - On Text 0 Over - Despawn Instant");

DELETE FROM `creature_text` WHERE `CreatureID` = 92986;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
('92986', '0', '0', 'Time for answers later. There are demons to kill', '12', '0', '100', '0', '0', '57339', '0', '0', 'Altruis the Sufferer to Player'),
('92986', '0', '1', 'The Legion is here.', '12', '0', '100', '0', '0', '57388', '0', '0', 'Altruis the Sufferer to Player');

-- Altruis the Sufferer Summon SAI
SET @ALTRUIS_2_SUMMON := 9298600; -- 909992
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ALTRUIS_2_SUMMON;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ALTRUIS_2_SUMMON AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ALTRUIS_2_SUMMON,0,0,0,61,0,100,0,0,0,0,0,53,1,9298600,0,0,0,0,1,0,0,0,0,0,0,0,'Altruis the Sufferer - On Reset - Start Waypoint'),
(@ALTRUIS_2_SUMMON,0,1,0,54,0,100,0,0,0,0,0,53,1,9298600,0,0,0,0,1,0,0,0,0,0,0,0,'Altruis the Sufferer - Just Spawned - Start WP'),
(@ALTRUIS_2_SUMMON,0,2,0,40,0,100,0,4,9298600,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Altruis the Sufferer - On Waypoint 4 Reached - Despawn Instant');

DELETE FROM `creature_template` WHERE `entry`=@ALTRUIS_2_SUMMON;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `femaleName`, `subname`, `TitleAlt`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `HealthScalingExpansion`, `RequiredExpansion`, `VignetteID`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES 
(@ALTRUIS_2_SUMMON,'0','0','0','0','0','Altruis the Sufferer','','Illidari',NULL,NULL,'0','100','100','5','0','0','2839','2','1','1.14286','1','1','0','1000','2000','1','1','1','33536','2048','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','SmartAI','0','3','1','10','1','5','1','1','1','1','0','0','1','0','0','','25549');

DELETE FROM `creature_template_addon` WHERE `entry`=@ALTRUIS_2_SUMMON;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `auras`) VALUES 
(@ALTRUIS_2_SUMMON, '0', '0', '0', '1', '0', '0', '0', '0', NULL);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=@ALTRUIS_2_SUMMON;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`, `VerifiedBuild`) VALUES 
(@ALTRUIS_2_SUMMON, '1', '128360', '0', '0', '128370', '0', '0', '0', '0', '0', '0');

DELETE FROM `waypoints` WHERE `entry`=9298600;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(9298600, 1, 4323.121, -541.476, -282.968, 'Altruis move'),
(9298600, 2, 4323.689, -538.507, -283.827, 'Altruis move'),
(9298600, 3, 4323.789, -525.309, -287.583, 'Altruis move'),
(9298600, 4, 4322.914, -516.714, -287.720, 'Altruis Despawn');

-- Kayn Sunfury 3 SAI
SET @KAYN_RISE := 92980;
UPDATE `creature` SET `spawntimesecs`=15 WHERE `guid`=20542693 AND `id`=@KAYN_RISE;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@KAYN_RISE;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@KAYN_RISE AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@KAYN_RISE,0,0,0,19,0,100,0,38690,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury - On Quest 'Rise of Illidari' Taken - Say Line 0"),
(@KAYN_RISE,0,1,2,52,0,100,0,0,@KAYN_RISE,0,0,12,@KAYN_RISE*100+00,6,0,0,0,0,8,0,0,0,4330.08,-548.288,-281.751,1.67951,"Kayn Sunfury - On Text 0 Over - Summon Creature 'Kayn Sunfury'"),
(@KAYN_RISE,0,2,0,61,0,100,0,0,@KAYN_RISE,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury - On Text 0 Over - Despawn Instant");

DELETE FROM `creature_text` WHERE `CreatureID` = 92980;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
('92980', '0', '0', 'The demons aim to destroy our world. We need to release our alies.', '12', '0', '100', '0', '0', '55352', '0', '0', 'Kayn Sunfury to Player');

-- Kayn Sunfury Summon SAI
SET @KAYN_2_SUMMON := 9298000;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@KAYN_2_SUMMON;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@KAYN_2_SUMMON AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@KAYN_2_SUMMON,0,0,0,61,0,100,0,0,0,0,0,53,1,9298000,0,0,0,0,1,0,0,0,0,0,0,0,'Kayn Sunfury - On Reset - Start WP'),
(@KAYN_2_SUMMON,0,1,0,54,0,100,0,0,0,0,0,53,1,9298000,0,0,0,0,1,0,0,0,0,0,0,0,'Kayn Sunfury - Just Spawned - Start WP'),
(@KAYN_2_SUMMON,0,2,0,40,0,100,0,4,9298000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Kayn Sunfury - On Waypoint 4 Reached - Despawn Instant');

DELETE FROM `creature_template` WHERE `entry`=@KAYN_2_SUMMON;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`,`name`, `femaleName`, `subname`, `TitleAlt`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `HealthScalingExpansion`, `RequiredExpansion`, `VignetteID`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `HealthModifier`, `HealthModifierExtra`, `ManaModifier`, `ManaModifierExtra`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES 
(@KAYN_2_SUMMON,'0','0','0','0','0','Kayn Sunfury','','Illidari',NULL,NULL,'0','100','100','5','0','0','2839','3','1','1.14286','1','1','0','1000','2000','1','1','1','32832','2048','0','0','0','0','0','0','7','4096','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','SmartAI','0','3','1','10','1','5','1','1','1','1','0','0','1','0','0','','25549');

DELETE FROM `creature_template_addon` WHERE `entry`=@KAYN_2_SUMMON;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `auras`) VALUES 
(@KAYN_2_SUMMON, '0', '0', '0', '1', '0', '0', '0', '0', NULL);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=@KAYN_2_SUMMON;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`, `VerifiedBuild`) VALUES 
(@KAYN_2_SUMMON, '1', '128359', '0', '0', '128371', '0', '0', '0', '0', '0', '0');

DELETE FROM `waypoints` WHERE `entry`=9298000;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(9298000, 1, 4328.037, -540.687, -283.179, 'Kayn move'),
(9298000, 2, 4328.187, -529.856, -286.617, 'Kayn move'),
(9298000, 3, 4328.747, -518.079, -287.721, 'Kayn move'),
(9298000, 4, 4332.642, -506.923, -288.638, 'Kayn Despawn');

UPDATE `creature_template` SET `unit_flags`=768, `npcflag`=1 WHERE `entry`=99501;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=99501;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(99501, 202064, 1, 0);

SET @CGUID := 280000000;
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+261 AND @CGUID+268;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(@CGUID+261,96656,1468,0,0,0,0,0,0,-1,0,1,4392.36,-513.713,-281.484,2.7597,300,0,0,1266732,0,0,0,0,0,0,0,'',25549),
(@CGUID+262,96656,1468,0,0,0,0,0,0,-1,0,1,4361.92,-529.656,-274.719,2.26568,300,0,0,1304724,0,0,0,0,0,0,0,'',25549),
(@CGUID+263,96656,1468,0,0,0,0,0,0,-1,0,1,4305.08,-559.763,-281.487,1.12057,300,0,0,1266732,0,0,0,0,0,0,0,'',25549),
(@CGUID+264,96656,1468,0,0,0,0,0,0,-1,0,1,4346.46,-427.373,-281.485,4.42082,300,0,0,1266732,0,0,0,0,0,0,0,'',25549),
(@CGUID+265,96656,1468,0,0,0,0,0,0,-1,0,1,4280.47,-457.641,-287.566,4.61166,300,0,0,1266732,0,0,0,0,0,0,0,'',25549),
(@CGUID+266,96656,1468,0,0,0,0,0,0,-1,0,1,4280.5,-528.708,-274.754,1.61223,300,0,0,1266732,0,0,0,0,0,0,0,'',25549),
(@CGUID+267,96656,1468,0,0,0,0,0,0,-1,0,1,4289.63,-457.4,-274.719,5.52037,300,0,0,1304724,0,0,0,0,0,0,0,'',25549),
(@CGUID+268,96656,1468,0,0,0,0,0,0,-1,0,1,4362.25,-457.504,-274.719,3.95586,300,0,0,1266732,0,0,0,0,0,0,0,'',25549);

UPDATE `creature` SET `spawntimesecs`=60 WHERE `guid` IN (20542822, 20542748, 20542855, 20542737);
UPDATE `creature` SET `spawntimesecs`=30 WHERE `guid` IN (20542615, 20542616);
UPDATE `creature_template` SET `unit_flags`=384 WHERE (entry = 100717);

UPDATE `creature_template_addon` SET `path_id`=99631 WHERE `entry`=99631;
UPDATE `creature_template` SET `MovementType`=2 WHERE `entry`=99631;
DELETE FROM `waypoints` WHERE `entry`=99631;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(99631,1,4334.12,-590.217,-281.922,''),
(99631,2,4331.97,-589.663,-281.922,''),
(99631,3,4331.98,-589.172,-281.922,''),
(99631,4,4332.24,-588.301,-281.922,''),
(99631,5,4332.14,-587.541,-281.922,''),
(99631,6,4331.09,-584.03,-281.925,''),
(99631,7,4330.39,-583.545,-281.924,''),
(99631,8,4328.46,-583.66,-281.841,'');
DELETE FROM `waypoint_data` WHERE `id`=99631;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(99631,1,4334.12,-590.217,-281.922,0,0,0,0,100,0),
(99631,2,4331.97,-589.663,-281.922,0,0,0,0,100,0),
(99631,3,4331.98,-589.172,-281.922,0,0,0,0,100,0),
(99631,4,4332.24,-588.301,-281.922,0,0,0,0,100,0),
(99631,5,4332.14,-587.541,-281.922,0,0,0,0,100,0),
(99631,6,4331.09,-584.03,-281.925,0,0,0,0,100,0),
(99631,7,4330.39,-583.545,-281.924,0,0,0,0,100,0),
(99631,8,4328.46,-583.66,-281.841,0,0,0,0,100,0);

UPDATE `creature` SET `PhaseId` = 993 WHERE `guid` = 20542908;
UPDATE `creature` SET `PhaseId` = 993 WHERE `guid` = 20542909;
UPDATE `creature` SET `PhaseId` = 543 WHERE `guid` = 20542912;
UPDATE `creature` SET `PhaseId` = 543 WHERE `guid` = 20542915;
UPDATE `creature` SET `PhaseId` = 543 WHERE `guid` = 20542913;
UPDATE `creature` SET `PhaseId` = 543 WHERE `guid` = 20542914;

DELETE FROM `creature_text` WHERE `CreatureID` = 92985;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(92985,0,0,'Just like old times, eh Kayn?',12,0,100,0,0,57329,0,0,'Altruis the Sufferer to Kayn Sunfury'),
(92985,1,0,'After ten-thousand years of imprisonment, Illidan succumbed to the fel energy within. When he was freed, he was not in his right mind.',12,0,100,0,0,57284,0,0,'Altruis the Sufferer to Kayn Sunfury'),
(92985,2,0,'You followed Illidan blindly, like a dog. Even when he lost his way.',12,0,100,0,0,57326,0,0,'Altruis the Sufferer to Kayn Sunfury'),
(92985,3,0,'I fought as hard as the rest of you! But I did not murder thousands of innocents.',12,0,100,0,0,57386,0,0,'Altruis the Sufferer to Kayn Sunfury'),
(92985,4,0,'Fool! You would trust the warden? She hates us almost as much as the Legion.',12,0,100,0,0,57389,0,0,'Altruis the Sufferer to Kayn Sunfury');

DELETE FROM `creature_text` WHERE `CreatureID` = 92984;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(92984,0,0,'Do not speak to me of old times, Altruis. You betrayed Illidan. You betrayed us all!',12,0,100,0,0,57345,0,0,'Kayn Sunfury to Altruis the Sufferer'),
(92984,1,0,'He was fighting it, as we all do. He needed us to have faith in him, Altruis.',12,0,100,0,0,57325,0,0,'Kayn Sunfury to Altruis the Sufferer'),
(92984,2,0,'He made the hard choices. He sacrificed EVERYTHING. What did you give?',12,0,100,0,0,57321,0,0,'Kayn Sunfury to Altruis the Sufferer'),
(92984,3,0,'You heard the Warden. We are the only ones who can defeat the Legion. We MUST survive this place.',12,0,100,0,0,57322,0,0,'Kayn Sunfury to Altruis the Sufferer'),
(92984,4,0,'You trust no one and believe in nothing, Altruis. You are a leader with no followers.',12,0,100,0,0,57390,0,0,'Kayn Sunfury to Altruis the Sufferer');

UPDATE `creature_template` SET `MovementType`=2 WHERE `entry`=92984;
UPDATE `creature_template` SET `MovementType`=2 WHERE `entry`=92985;

-- UPDATE `creature` SET `ScriptName`='npc_altruis_sufferer_4' WHERE `guid`=20542908;
-- Overlord Saurfang in Orgri
DELETE FROM `creature` WHERE `guid`=280000275;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(280000275,100636,1,1637,5170,0,0,1178,0,-1,0,1,1606.07,-4376.37,21.8468,3.63639,300,0,0,1305,0,0,1,0,0,0,0,'npc_lord_saurfang',25549);
-- khadgar in Orgri
DELETE FROM `creature` WHERE `guid`=280000276;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(280000276,97296,1,1637,5170,0,0,1178,0,-1,0,1,1465.72,-4419.58,25.45,0.172787,120,0,0,9145554,5,0,2,0,0,0,0,'',25549);

DELETE FROM `creature_text` WHERE `CreatureID`=100636;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(100636,0,0,'I don\'t like the looks of them, mage. They smell of demon filth.',12,0,100,0,0,57915,0,0,'Lord Saurfang to Player');

DELETE FROM `npc_text` WHERE `ID` IN (30560, 30561, 30562, 30563);
INSERT INTO `npc_text` (`ID`, `Probability0`, `Probability1`, `Probability2`, `Probability3`, `Probability4`, `Probability5`, `Probability6`, `Probability7`, `BroadcastTextId0`, `BroadcastTextId1`, `BroadcastTextId2`, `BroadcastTextId3`, `BroadcastTextId4`, `BroadcastTextId5`, `BroadcastTextId6`, `BroadcastTextId7`, `VerifiedBuild`) VALUES
(30560,1,0,0,0,0,0,0,0,147324,0,0,0,0,0,0,0,25549),
(30561,1,0,0,0,0,0,0,0,147325,0,0,0,0,0,0,0,25549),
(30562,1,0,0,0,0,0,0,0,147326,0,0,0,0,0,0,0,25549),
(30563,1,0,0,0,0,0,0,0,147327,0,0,0,0,0,0,0,25549);

DELETE FROM `gossip_menu` WHERE `MenuID` IN (20460, 20461, 20462, 20463);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(20460, 30560),
(20461, 30561),
(20462, 30562),
(20463, 30563);

UPDATE `creature_template` SET `minlevel`=100, `maxlevel`=100, `gossip_menu_id`=20460 WHERE `entry`=100636;
UPDATE `creature_template` SET `gossip_menu_id`=20461 WHERE `entry`=101035;
-- Alari the Souleater in Durotar Funeral
DELETE FROM `creature` WHERE `guid`=280000277;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(280000277,100873,1,14,4982,0,0,1180,0,-1,0,0,1272.81,-4383.71,28.735,3.48624,120,0,0,870,0,0,3,0,0,0,0,'',25549);
-- Lady Sylvana Windrunner in Durotar Funeral
DELETE FROM `creature` WHERE `guid`=280000278;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(280000278,101035,1,14,4982,0,0,1180,0,-1,0,0,1244.27,-4381.93,28.2979,4.74574,120,0,0,1075122944,0,0,3,0,0,0,0,'npc_lady_sylvana_funeral',25549);
-- Faction Acquisition quests
UPDATE `quest_template_addon` SET `PrevQuestID`=40976 WHERE `ID`=40982; -- Second Sight
UPDATE `quest_template_addon` SET `PrevQuestID`=40982 WHERE `ID`=40983; -- Demons Among Them
UPDATE `quest_template_addon` SET `PrevQuestID`=40983 WHERE `ID`=41002; -- A Weapon of the Horde
-- other quests to not mess up with the DH chain
UPDATE `quest_template_addon` SET `PrevQuestID`=40522 WHERE `ID`=40607; -- Demons Among Us
UPDATE `quest_template_addon` SET `PrevQuestID`=40607 WHERE `ID`=40605; -- Keep Your Friends Close
-- Felblade Spawns
DELETE FROM `creature` WHERE `guid` BETWEEN 280000301 AND 280000320;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(280000301,101104,1,14,4982,0,0,1181,0,-1,0,0,1234.2,-4423.7,22.5085,1.24287,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000302,101104,1,14,4982,0,0,1181,0,-1,0,0,1266.75,-4428.12,26.6402,2.11152,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000303,101104,1,14,4982,0,0,1181,0,-1,0,0,1294,-4389.82,26.2693,3.29276,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000304,101104,1,14,4982,0,0,1181,0,-1,0,0,1208.96,-4404.49,23.1639,0.0105786,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000305,101104,1,14,4982,0,0,1181,0,-1,0,0,1308.13,-4427.31,24.7053,2.7202,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000306,102543,1,14,4982,0,0,1181,0,-1,0,1,1264.68,-4398.66,26.3237,2.01098,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000307,102543,1,14,4982,0,0,1181,0,-1,0,1,1258.96,-4401.78,25.8567,1.77694,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000308,102543,1,14,4982,0,0,1181,0,-1,0,1,1263.19,-4407.53,25.7372,1.65755,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000309,102543,1,14,4982,0,0,1181,0,-1,0,1,1249.8,-4409.17,24.6836,2.19241,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000310,102543,1,14,4982,0,0,1181,0,-1,0,1,1242.62,-4413.61,23.726,1.96779,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000311,102543,1,14,4982,0,0,1181,0,-1,0,1,1248.65,-4419.28,23.8528,2.08481,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000312,102543,1,14,4982,0,0,1181,0,-1,0,1,1226.22,-4414.39,22.6062,1.0363,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000313,102543,1,14,4982,0,0,1181,0,-1,0,1,1218.39,-4409.72,22.9143,0.719791,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000314,102543,1,14,4982,0,0,1181,0,-1,0,1,1216.52,-4417.56,22.0563,0.7677,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000315,102543,1,14,4982,0,0,1181,0,-1,0,1,1244.4,-4390.51,28.2295,2.15628,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000316,102543,1,14,4982,0,0,1181,0,-1,0,1,1296.29,-4410.06,26.5713,2.26624,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000317,102543,1,14,4982,0,0,1181,0,-1,0,1,1293.49,-4434.14,27.1493,1.90103,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000318,102543,1,14,4982,0,0,1181,0,-1,0,1,1258.61,-4437.65,26.8017,1.70625,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000319,102543,1,14,4982,0,0,1181,0,-1,0,1,1227.55,-4448.21,24.2946,1.55939,120,0,0,174,0,0,0,0,0,0,0,'',25549),
(280000320,102543,1,14,4982,0,0,1181,0,-1,0,1,1224.24,-4394.29,23.6983,0.350657,120,0,0,174,0,0,0,0,0,0,0,'',25549);
UPDATE `creature_template` SET `KillCredit1`=101105, `minlevel`=99, `maxlevel`=99, `faction`=954, `unit_flags`=32768, `unit_flags2`=0 WHERE `entry` IN (102543, 101104);
UPDATE `creature_template_addon` SET `bytes1` = '0' , `auras` = '' WHERE `entry` = '102543';
UPDATE `creature_template` SET `faction` = '1786' WHERE `entry` = '102543';
UPDATE `creature_template` SET `unit_flags2` = '35653632' WHERE `entry` = '102543';
UPDATE `creature_template` SET `HealthScalingExpansion` = '5' , `lootid` = '98486' , `HealthModifier` = '1' WHERE `entry` = '102543';
UPDATE `scene_template` SET `ScriptName`='scene_demons_among_them_horde' WHERE `SceneId`=1453;
UPDATE `creature_template` SET `npcflag` = 2 WHERE `entry`=114562; 

DELETE FROM `creature_text` WHERE `CreatureID` = 116704;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `Comment`) VALUES
(116704,0,0,'I need to speak with you.',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player'),
(116704,0,1,'Altruis and the others await you at Krasus\' Landing. I\'ll meet you there.',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player'),
(116704,0,2,'Kayn and the others await you at Krasus\' Landing. I\'ll meet you there.',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player'),
(116704,0,3,'Every moment we sit idle, the Legion gains strength.',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player'),
(116704,0,4,'In Lord Illidan\'s absence, you are the ranking member of our order. We can\'t make this decision without you.',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player'),
(116704,0,5,'Let me know when you\'re ready.',12,0,100,0,0,57295,0,0,'Korvas Bloodthorn to Player');

UPDATE `creature_template` SET `npcflag`=2, `AIName`="", `ScriptName`='npc_korvas_bloodthorn_summon', `VerifiedBuild`=25549 WHERE `entry`=116704;
DELETE FROM `creature_equip_template` WHERE `CreatureID`=116704;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `AppearanceModID1`, `ItemVisual1`, `ItemID2`, `AppearanceModID2`, `ItemVisual2`, `ItemID3`, `AppearanceModID3`, `ItemVisual3`, `VerifiedBuild`) VALUES 
(116704,1,128359,0,0,128371,0,0,0,0,0,25549);

UPDATE `creature_template` SET `npcflag`=3, `gossip_menu_id`=20463, `ScriptName`='npc_altruis_sufferer_artifact', `VerifiedBuild`=25549 WHERE `entry`=99254;

UPDATE `quest_template_addon` SET `PrevQuestID`=39047 WHERE `ID`=40816; -- The Power To Survive (Altruis)
UPDATE `quest_template_addon` SET `PrevQuestID`=39261 WHERE `ID`=40814; -- The Power To Survive (Kayn)
UPDATE `quest_template_addon` SET `PrevQuestID`=41120 WHERE `ID`=41121; -- By Any Means
UPDATE `quest_template_addon` SET `PrevQuestID`=41121 WHERE `ID`=41119; -- The Hunt

DELETE FROM `playerchoice_response` WHERE `ChoiceId`=255;
INSERT INTO `playerchoice_response` (`ChoiceId`, `ResponseId`, `Index`, `ChoiceArtFileId`, `Header`, `Answer`, `Description`, `Confirmation`, `VerifiedBuild`) VALUES
(255, 640, 2, 0, 'Vengeance', 'Select', 'The mighty Aldrachi were one of the few to stand against the Burning Legion. It took Sargeras to fell their greatest champion, seizing his warblades in the process. Now the traitorous Illidari Carla, servant of Kil\'Jaeden, wields the infamous warblades. The more souls she claims, the more powerful she becomes.\n\n            |cFF000000|Hitem:128832|h[Aldrachi Warblades]|h|r', 'CONFIRM_ARTIFACT_CHOICE', 25549),
(255, 641, 1, 0, 'Havoc', 'Select', 'These glaives belong to the former demon hunter Varedis Felsoul, who was slain defending the Black Temple many years ago. The Burning Legion\'s master, Kil\'jaeden the Deceiver, raised Varedis in the Twisting Nether and imbued his glaives with a piece of the demon lord\'s chaotic power.\n\n            |cFF000000|Hitem:127829|h[Twinblades of the Deceiver]|h|r', 'CONFIRM_ARTIFACT_CHOICE', 25549);

DELETE FROM `spell_target_position` WHERE `ID`=192757;
INSERT INTO `spell_target_position` (`ID`, `EffectIndex`, `MapID`, `PositionX`, `PositionY`, `PositionZ`, `VerifiedBuild`) VALUES
(192757, 0, 0, -8544.392578, 462.887299, 104.472054, 22566);

-- Delete Reward From Artifact Quests (Temporarly, only for the implemented ones)
UPDATE `quest_template` SET `RewardAmount1` = 0 WHERE `ID` IN (40817, 40818);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=19 AND `SourceEntry` IN (41120,41803);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19,0,41120,0,0,14,0,40818,0,0,0,0,0,"","Quest 'Making Arrangements' can only be taken if quest 'Aldrachi Warblades Chosen' is not taken"),
(19,0,41803,0,0,28,0,40817,0,0,1,0,0,"","Quest 'Asking A Favor' can only be taken if quest 'Twinblades of the Deceiver Chosen' is not completed");

DELETE FROM `creature` WHERE `guid`=280000321;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES
(280000321,97296,0,1519,5390,0,0,175,0,-1,0,1,-8540.17,462.07,104.71,5.338177,120,0,0,9145554,5,0,2,0,0,0,0,'',25549);

DELETE FROM `spell_area` WHERE `area` IN (5390);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `quest_start_status`, `quest_end_status`) VALUES
(57569, 5390, 39689, 39691, 64, 64);


REPLACE INTO spell_script_names VALUES (119607, "spell_monk_renewing_mist_jump ");
REPLACE INTO spell_script_names VALUES (119611, "spell_monk_renewing_mist_hot");
REPLACE INTO spell_script_names VALUES (119898, "spell_warlock_command_demon");
REPLACE INTO spell_script_names VALUES (6262, "spell_warlock_healthstone_heal");
REPLACE INTO spell_script_names VALUES (691, "spell_warlock_summon_demon");
REPLACE INTO spell_script_names VALUES (688, "spell_warlock_summon_demon");
REPLACE INTO spell_script_names VALUES (697, "spell_warlock_summon_demon");
REPLACE INTO spell_script_names VALUES (712, "spell_warlock_summon_demon");
REPLACE INTO spell_script_names VALUES (30146, "spell_warlock_summon_demon");
REPLACE INTO spell_script_names VALUES (112870, "spell_warlock_summon_demon");

REPLACE INTO spell_script_names VALUES (104759, "aura_warlock_soul_shard_driver");
REPLACE INTO spell_script_names VALUES (193940, "spell_spawn_aegis");
REPLACE INTO spell_script_names VALUES (202341, "spell_anubesset_impale");
REPLACE INTO spell_script_names VALUES (201863, "spell_anubesset_call_of_swarm");
REPLACE INTO spell_script_names VALUES (202485, "spell_anubesset_summon_blistering_ooze");
REPLACE INTO spell_script_names VALUES (203057, "spell_thalena_shroud_of_sorrow_dmg");
REPLACE INTO spell_script_names VALUES (202659, "spell_thalena_blood_swarn");
REPLACE INTO spell_script_names VALUES (202779, "spell_thalena_frenzied_bloodthirst");
REPLACE INTO spell_script_names VALUES (202676, "spell_thalena_vampiric_kiss");
REPLACE INTO spell_script_names VALUES (202805, "spell_thalena_vampiric_kiss_player");
REPLACE INTO spell_script_names VALUES (203381, "spell_thalena_blood_call");
REPLACE INTO spell_script_names VALUES (261718, "spell_terror_wall_visual_back");
REPLACE INTO spell_script_names VALUES (261560, "spell_terror_wall_visual");



REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (204588, 'spell_activate_countermeasure');




UPDATE creature_template SET scriptname = 'npc_orb_of_swirling' WHERE entry = 128088;


REPLACE INTO spell_script_names VALUES (261552, "spell_terror_wall");
REPLACE INTO spell_script_names VALUES (261600, "spell_coalseced_essence");
REPLACE INTO spell_script_names VALUES (203622, "spell_betrug_chaotic_energy");



REPLACE INTO spell_script_names VALUES (202361, "spell_betrug_execution");
REPLACE INTO spell_script_names VALUES (202328, "spell_betrug_mighty_slash");
REPLACE INTO spell_script_names VALUES (202339, "spell_betrug_mighty_slash_jump");
REPLACE INTO spell_script_names VALUES (210879, "spell_betrug_seed_of_destruction");


REPLACE INTO spell_script_names VALUES (219984, "spell_chronomatic_anomaly_burst_of_time_area");
REPLACE INTO spell_script_names VALUES (206610, "spell_chronomatic_anomaly_time_release_area");
REPLACE INTO spell_script_names VALUES (206609, "spell_chronomatic_anomaly_time_release_absorb");
REPLACE INTO spell_script_names VALUES (206607, "spell_chronomatic_anomaly_chronometric_particles");
REPLACE INTO spell_script_names VALUES (212115, "spell_chronomatic_anomaly_temporal_smash_dest");
REPLACE INTO spell_script_names VALUES (212109, "spell_chronomatic_anomaly_temporal_smash");


REPLACE INTO spell_script_names VALUES (206615, "spell_chronomatic_anomaly_time_bomb_damage");
REPLACE INTO spell_script_names VALUES (206617, "spell_chronomatic_anomaly_time_bomb_periodic");
REPLACE INTO spell_script_names VALUES (206618, "spell_chronomatic_anomaly_time_bomb_area");

REPLACE INTO spell_script_names VALUES (137483, "spell_lulin_chilled");
REPLACE INTO spell_script_names VALUES (137474, "spell_suen_blazing_radiance_aura_2");


REPLACE INTO spell_script_names VALUES (137475, "spell_lulin_blazing_radiance_dmg_2");
REPLACE INTO spell_script_names VALUES (137531, "spell_lulin_tidal_force");
REPLACE INTO spell_script_names VALUES (138799, "spell_lulin_ice_comet_aoe");
REPLACE INTO spell_script_names VALUES (137411, "spell_suen_blazing_radiance_aura");
REPLACE INTO spell_script_names VALUES (137404, "spell_suen_tears_of_the_sun");
REPLACE INTO spell_script_names VALUES (137187, "spell_lulin_disipate");
REPLACE INTO spell_script_names VALUES (136768, "spell_lulin_cosmic_barrage_summon_1");
REPLACE INTO spell_script_names VALUES (136752, "spell_lulin_cosmic_barrage");
REPLACE INTO spell_script_names VALUES (136714, "spell_lulin_moon_lotus_summon");
REPLACE INTO spell_script_names VALUES (136690, "spell_lulin_moon_lotus");
REPLACE INTO spell_script_names VALUES (137105, "spell_suen_disipate");



REPLACE INTO spell_script_names VALUES (204277, "spell_skorpyron_power");
REPLACE INTO spell_script_names VALUES (205200, "spell_skorpyron_arcanoslash_periodic");
REPLACE INTO spell_script_names VALUES (204275, "spell_skorpyron_arcanoslash_damage");
REPLACE INTO spell_script_names VALUES (204531, "spell_skorpyron_arcane_tether_periodic_damage");
REPLACE INTO spell_script_names VALUES (210074, "spell_skorpyron_shockwave_damage");
REPLACE INTO spell_script_names VALUES (204471, "spell_skorpyron_focused_blast");
REPLACE INTO spell_script_names VALUES (210172, "spell_skorpyron_call_of_the_scorpid");
REPLACE INTO spell_script_names VALUES (204371, "spell_skorpyron_call_of_the_scorpid_pack_select");
REPLACE INTO spell_script_names VALUES (214966, "spell_skorpyron_arcane_tether_periodic_visual");
REPLACE INTO spell_script_names VALUES (211659, "spell_skorpyron_arcane_tether_damage");


REPLACE INTO spell_script_names VALUES (212494, "spell_gen_aluriel_annihilate");
REPLACE INTO spell_script_names VALUES (212587, "spell_gen_mark_of_frost");
REPLACE INTO spell_script_names VALUES (212531, "spell_gen_target_mark_of_forst");
REPLACE INTO spell_script_names VALUES (212735, "spell_gen_aluriel_detonate");
REPLACE INTO spell_script_names VALUES (213853, "spell_gen_aluriel_animate");

REPLACE INTO spell_script_names VALUES (201960, "spell_shivermaw_ice_bomb");
REPLACE INTO spell_script_names VALUES (201672, "spell_shivermaw_relentless_storm");
REPLACE INTO spell_script_names VALUES (201848, "spell_shivermaw_relentless_storm_missile");
REPLACE INTO spell_script_names VALUES (210504, "spell_saelorn_toxic_blood");
REPLACE INTO spell_script_names VALUES (202306, "spell_vha_creeping_slaughter");
REPLACE INTO spell_script_names VALUES (201495, "spell_vha_recongealing");
REPLACE INTO spell_script_names VALUES (201598, "spell_festerface_congealing_vomit");
REPLACE INTO spell_script_names VALUES (201369, "spell_vha_rocket_chicken_rocket");
REPLACE INTO spell_script_names VALUES (201356, "spell_vha_swarn_rockets");
REPLACE INTO spell_script_names VALUES (197776, "spell_xakal_fel_fissure");
