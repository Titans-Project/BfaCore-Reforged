/*
**************************
*    BfaCore Reforged    *
**************************
*/


/* DELETE */
DELETE FROM `spell_script_names` WHERE `spell_id` IN (257028, 256038, 256105, 257092, 257612, 257608);


/* boss_knight_captain_valyri.cpp */
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('257028', 'bfa_spell_fuselighter');


/* boss_overseer_korgus.cpp */
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('256038', 'bfa_spell_generic_deadeye');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('256105', 'bfa_spell_explosive_burst');


/* boss_the_sand_queen.cpp */
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('257092', 'bfa_spell_sand_trap');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('257612', 'bfa_spell_upheaval_target_selector');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('257608', 'bfa_spell_upheaval_2');
