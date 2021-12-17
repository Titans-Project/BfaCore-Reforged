/*
**************************
*    BfaCore Reforged    *
**************************
*/

REPLACE INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES (10714, 0, 0, 8, 8, 0, 0, 0, 0, 'at_dh_sigil_of_silence', 28153);
REPLACE INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES (11023, 0, 0, 8, 8, 0, 0, 0, 0, 'at_dh_sigil_of_misery', 27481);
REPLACE INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `AnimId`, `AnimKitId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES (6351, 11023, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27481);
REPLACE INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `AnimId`, `AnimKitId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES (6027, 10714, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 32750);
REPLACE INTO `spell_custom_attr` (`entry`, `attributes`) VALUES ('178740', '4096');
