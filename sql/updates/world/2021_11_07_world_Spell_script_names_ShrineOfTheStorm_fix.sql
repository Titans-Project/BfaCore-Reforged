/*
**************************
*    BfaCore Reforged    *
**************************
*/


/* DELETE */
DELETE FROM `spell_script_names` WHERE `spell_id` IN (264144, 264477, 264560, 269131, 269242, 269289, 267299);


/* boss_aqusirr.cpp */
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('264144', 'bfa_spell_undertow');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('264477', 'bfa_spell_grasp_from_the_depths');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('264560', 'bfa_spell_choking_brine');

/* boss_lord_stormsong.cpp */
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('269131', 'bfa_spell_ancient_mindbender');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('269242', 'bfa_spell_surrender_to_the_void');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('269289', 'bfa_spell_disciple_of_the_volzith');

/* boss_volzith_the_whisperer.cpp */
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('267299', 'bfa_spell_call_the_abyss');
