DELETE FROM `spell_script_names` WHERE `spell_id` IN ('6201', '6262');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(6201, "spell_warlock_create_healthstone"),
(6262, "spell_warlock_healthstone");
