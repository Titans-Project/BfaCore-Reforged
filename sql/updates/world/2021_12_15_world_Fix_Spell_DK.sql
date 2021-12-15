/*
**************************
*    BfaCore Reforged    *
**************************
*/

DELETE FROM spell_script_names WHERE spell_id = 43265;
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('253593', 'aura_inexorable_assault');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('207230', 'spell_dk_frostscythe');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('108194', 'spell_dk_asphyxiate');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('194311', 'spell_dk_festering_wound_damage');
REPLACE INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES ('9225', '0', '0', '8', '8', '0', '0', '0', '0', 'at_dk_death_and_decay', '31478');
REPLACE INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `AnimId`, `AnimKitId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES ('4485', '9225', '0', '0', '0', '0', '0', '0', '0', '0', '10000', '32750');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('207289', 'spell_dk_unholy_frenzy');


