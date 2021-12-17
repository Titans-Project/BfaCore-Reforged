/*
**************************
*    BfaCore Reforged    *
**************************
*/

DELETE FROM spell_script_names WHERE ScriptName = 'spell_dh_felblade';
DELETE FROM spell_script_names WHERE spell_id = '232893';
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('232893', 'spell_dh_felblade');
