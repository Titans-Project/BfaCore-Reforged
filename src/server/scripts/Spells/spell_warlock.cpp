/*
 * Copyright (C) 2022 BfaCore Reforged
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
* Scripts for spells with SPELLFAMILY_WARLOCK and SPELLFAMILY_GENERIC spells used by warlock players.
* Ordered alphabetically using scriptname.
* Scriptnames of files in this file should be prefixed with "spell_warl_".
*/

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "AreaTriggerTemplate.h"
#include "Creature.h"
#include "SpellPackets.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "CellImpl.h"
#include "DatabaseEnv.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Pet.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Unit.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "Spell.h"
#include "PetAI.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "PassiveAI.h"
#include "CombatAI.h"
#include "GridNotifiers.h"
#include "Item.h"
#include <G3D/Vector3.h>

enum WarlockSpells
{
    SPELL_WARLOCK_ABSOLUTE_CORRUPTION = 196103,
    SPELL_CHAOS_BOLT = 116858,
    SPELL_WARLOCK_FEL_LORD_CLEAVE = 213688,
    SPELL_WARLOCK_AFTERMATH_STUN = 85387,
    SPELL_WARLOCK_ETERNAL_STRUGGLE_PROC = 196304,
    SPELL_WARLOCK_AGONY = 980,
    SPELL_WARLOCK_ARCHIMONDES_VENGEANCE_COOLDOWN = 116405,
    SPELL_WARLOCK_ARCHIMONDES_VENGEANCE_DAMAGE = 124051,
    SPELL_WARLOCK_ARCHIMONDES_VENGEANCE_PASSIVE = 116403,
    SPELL_WARLOCK_BACKDRAFT = 117828,
    SPELL_WARLOCK_BACKDRAFT_AURA = 196406,
    SPELL_WARLOCK_WILD_IMP_SUMMON = 104317,
    SPELL_WARLOCK_ESSENCE_DRAIN = 221711,
    SPELL_WARLOCK_ESSENCE_DRAIN_DEBUFF = 221715,
    SPELL_WARLOCK_SOUL_SKIN = 218567,
    SPELL_WARLOCK_BANE_OF_DOOM_EFFECT = 18662,
    SPELL_WARLOCK_HOWL_OF_TERROR = 5484,
    SPELL_WARLOCK_BURNING_RUSH = 111400,
    SPELL_WARLOCK_GLYPH_OF_FELGUARD = 56246,
    SPELL_WARLOCK_CALL_DREADSTALKERS = 104316,
    SPELL_WARLOCK_TEAR_CHAOS_BARRAGE = 187394,
    SPELL_WARLOCK_TEAR_CHAOS_BOLT = 215279,
    SPELL_CORRUPTION_DOT = 146739,
    SPELL_ARENA_DAMPENING = 110310,
    SPELL_WARLOCK_SOUL_LINK_HEAL = 108447,
    SPELL_WARLOCK_LASERBEAM = 212529,
    SPELL_WARLOCK_CONTAGION = 196105,
    SPELL_SOULFIRE_ENERGIZE = 281490,
    SPELL_WARLOCK_SOUL_LINK_BUFF = 108446,
    SPELL_DEMONBOLT_ENERGIZE = 280127,
    SPELL_WARLOCK_TORMENTED_SOULS = 216695,
    SPELL_WARLOCK_DEMON_SKIN = 219272,
    SPELL_WARLOCK_IMMOLATION_TRIGGERED = 20153,
    SPELL_WARLOCK_INFERNAL_FURNACE = 211119,
    SPELL_WARLOCK_T14_BONUS = 123141,
    SPELL_WARLOCK_HARVEST_LIFE = 108371,
    SPELL_WARLOCK_IMMOLATION = 19483,
    SPELL_WARLOCK_HELLFIRE_DAMAGE = 5857,
    SPELL_WARLOCK_CURSE_OF_SHADOWS_DAMAGE = 236615,
    SPELL_WARLOCK_RAGING_SOUL = 148463,
    SPELL_WARLOCK_CONTAGION_DEBUFF = 233494,
    SPELL_WARLOCK_STOLEN_POWER = 211530,
    SPELL_WARLOCK_STOLEN_POWER_COUNTER = 211529,
    SPELL_WARLOCK_STOLEN_POWER_BUFF = 211583,
    SPELL_WARLOCK_CALL_DREADSTALKERS_SUMMON = 193331,
    SPELL_WARLOCK_SHARPENED_DREADFANGS = 211123,
    SPELL_WARLOCK_CORRUPTION_TRIGGERED = 146739,
    SPELL_WARLOCK_CHANNEL_DEMONFIRE_ACTIVATOR = 228312,
    SPELL_WARLOCK_PET_DOOMBOLT = 85692,
    SPELL_INFERNO_AURA = 270545,
    SPELL_WARLOCK_DEVOURER_OF_LIFE_PROC = 215165,
    SPELL_RAIN_OF_FIRE_ENERGIZE = 270548,
    SPELL_WARLOCK_T16_4P_TRIGGERED = 145164,
    SPELL_WARLOCK_PLANESWALKER = 196675,
    SPELL_WARLOCK_PLANESWALKER_BUFF = 196674,
    SPELL_WARLOCK_SOUL_EFFIGY_DAMAGE = 205260,
    SPELL_WARLOCK_SOUL_EFFIGY_VISUAL = 205277,
    SPELL_WARLOCK_CHANNEL_DEMONFIRE_DAMAGE = 196448,
    SPELL_WARLOCK_COMMAND_DEMON_OVERRIDER = 119904,
    SPELL_WARLOCK_THALKIES_DISCORD_DAMAGE = 211727,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT1 = 233490,
    SPELL_WARLOCK_PET_LESSER_INVISIBILITY = 7870,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT2 = 233496,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT3 = 233497,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT4 = 233498,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT5 = 233499,
    SPELL_WARLOCK_CONFLAGRATE = 17962,
    SPELL_WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE = 108685,
    SPELL_WARLOCK_CORRUPTION = 172,
    SPELL_WARLOCK_THALKIELS_CONSUMPTION_DAMAGE = 211715,
    SPELL_WARLOCK_CORRUPTION_DAMAGE = 146739,
    SPELL_WARLOCK_CREATE_HEALTHSTONE = 23517,
    SPELL_WARLOCK_CURSE_OF_DOOM_EFFECT = 18662,
    SPELL_WARLOCK_COMPOUNDING_HORROR = 199281,
    SPELL_WARLOCK_COMPOUNDING_HORROR_DAMAGE = 231489,
    SPELL_WARLOCK_DARK_REGENERATION = 108359,
    SPELL_WARLOCK_DARK_SOUL_INSTABILITY = 113858,
    SPELL_WARLOCK_DREADSTALKER_CHARGE = 194247,
    SPELL_WARLOCK_DARK_SOUL_KNOWLEDGE = 113861,
    SPELL_WARLOCK_DARK_SOUL_MISERY = 113860,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_RANK2 = 231791,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_ENERGIZE = 31117,
    SPELL_WARLOCK_DECIMATE_AURA = 108869,
    SPELL_WARLOCK_DEMON_SOUL_FELGUARD = 79452,
    SPELL_WARLOCK_DEMON_SOUL_FELHUNTER = 79460,
    SPELL_WARLOCK_DEMON_SOUL_IMP = 79459,
    SPELL_WARLOCK_T15_2P_BONUS = 138129,
    SPELL_WARLOCK_T15_2P_BONUS_TRIGGERED = 138483,
    SPELL_WARLOCK_DEMON_SOUL_SUCCUBUS = 79453,
    SPELL_WARLOCK_DEMON_SOUL_VOIDWALKER = 79454,
    SPELL_WARLOCK_DEMONBOLT = 157695,
    SPELL_WARLOCK_DEMONIC_CALL = 114925,
    SPELL_WARLOCK_DEMONIC_CALLING = 205145,
    SPELL_WARLOCK_DEMONIC_CALLING_TRIGGER = 205146,
    SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST = 62388,
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON = 48018,
    SPELL_WARLOCK_SOUL_LEECH_SHIELD = 108366,
    SPELL_WARLOCK_SHARPENED_DREADFANGS_BUFF = 215111,
    SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT = 48020,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD = 54508,
    SPELL_WARLOCK_DEADWIND_HARVERST = 216708,
    SPELL_WARLOCK_SOUL_EFFIGY_AURA = 205247,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER = 54509,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP = 54444,
    SPELL_WARLOCK_THE_EXPANDABLES_BUFF = 211218,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS = 54435,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER = 54443,
    SPELL_WARLOCK_DEMONIC_GATEWAY_PERIODIC_CHARGE = 113901,
    SPELL_WARLOCK_DEMONIC_GATEWAY_SUMMON_GREEN = 113886,
    SPELL_WARLOCK_DEMONIC_GATEWAY_SUMMON_PURPLE = 113890,
    SPELL_WARLOCK_DEMONIC_GATEWAY_JUMP_GREEN = 113896,
    SPELL_WARLOCK_DEMONIC_GATEWAY_JUMP_PURPLE = 120729,
    SPELL_WARLOCK_DEMONIC_LEAP_JUMP = 109163,
    SPELL_WARLOCK_DEMONSKIN = 219272,
    SPELL_WARLOCK_DOOM_DOUBLED = 218572,
    SPELL_WARLOCK_DEMONWRATH_AURA = 193440,
    SPELL_WARLOCK_DEMONWRATH_SOULSHARD = 194379,
    SPELL_WARLOCK_DESTRUCTION_PASSIVE = 137046,
    SPELL_WARLOCK_DEVOUR_MAGIC_HEAL = 19658,
    SPELL_WARLOCK_DISRUPTED_NETHER = 114736,
    SPELL_WARLOCK_DOOM = 603,
    SPELL_WARLOCK_DOOM_SOUL_SHARD = 193318,
    SPELL_WARLOCK_DRAIN_LIFE_HEAL = 89653,
    SPELL_WARLOCK_ERADICATION = 196412,
    SPELL_WARLOCK_ERADICATION_DEBUFF = 196414,
    SPELL_WARLOCK_EYE_LASER = 205231,
    SPELL_WARLOCK_FEAR = 5782,
    SPELL_WARLOCK_FEAR_BUFF = 118699,
    SPELL_WARLOCK_FEAR_EFFECT = 118699,
    SPELL_WARLOCK_FEL_FIREBOLT = 104318,
    SPELL_WARLOCK_FEL_SYNERGY_HEAL = 54181,
    SPELL_WARLOCK_FIRE_AND_BRIMSTONE = 196408,
    SPELL_WARLOCK_GLYPH_OF_CONFLAGRATE = 56235,
    SPELL_WARLOCK_GLYPH_OF_HEALTHSTONE = 56224,
    SPELL_WARLOCK_GLYPH_OF_DEMON_TRAINING = 56249,
    SPELL_WARLOCK_GLYPH_OF_FEAR = 56244,
    SPELL_WARLOCK_GLYPH_OF_FEAR_EFFECT = 130616,
    SPELL_WARLOCK_GLYPH_OF_SHADOWFLAME = 63311,
    SPELL_WARLOCK_SOUL_FLAME_PROC = 199581,
    SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE = 63106,
    SPELL_WARLOCK_GLYPH_OF_SOULWELL = 58094,
    SPELL_WARLOCK_GLYPH_OF_SOULWELL_VISUAL = 34145,
    SPELL_WARLOCK_GRIMOIRE_FELGUARD = 111898,
    SPELL_WARLOCK_GRIMOIRE_FELHUNTER = 111897,
    SPELL_WARLOCK_GRIMOIRE_IMP = 111859,
    SPELL_WARLOCK_GRIMOIRE_OF_SACRIFICE = 108503,
    SPELL_WARLOCK_SEED_OF_CORRUPTION = 27243,
    SPELL_WARLOCK_LORD_OF_THE_FLAMES_CD = 226802,
    SPELL_WARLOCK_LORD_OF_THE_FLAMES = 224103,
    SPELL_WARLOCK_LORD_OF_THE_FLAMES_SUMMON = 224105,
    SPELL_WARLOCK_GRIMOIRE_OF_SYNERGY_BUFF = 171982,
    SPELL_WARLOCK_GRIMOIRE_SUCCUBUS = 111896,
    SPELL_WARLOCK_HAND_OF_GULDAN = 105174,
    SPELL_WARLOCK_GRIMOIRE_VOIDWALKER = 111895,
    SPELL_WARLOCK_HAND_OF_DOOM = 196283,
    SPELL_WARLOCK_HAND_OF_GULDAN_DAMAGE = 86040,
    SPELL_WARLOCK_HAND_OF_GULDAN_SUMMON = 196282,
    SPELL_WARLOCK_HARVEST_LIFE_HEAL = 125314,
    SPELL_WARLOCK_HAUNT = 48181,
    SPELL_WARLOCK_SHADOWBURN = 17877,
    SPELL_WARLOCK_HAVOC = 80240,
    SPELL_WARLOCK_HEALTH_FUNNEL_HEAL = 217979,
    SPELL_WARLOCK_FIREBOLT_BONUS = 231795,
    SPELL_WARLOCK_ROT_AND_DECAY = 212371,
    SPELL_WARLOCK_IMMOLATE = 348,
    SPELL_WARLOCK_SOULBURN = 74434,
    SPELL_WARLOCK_SOULBURN_UNENDING_BREATH = 104242,
    SPELL_WARLOCK_SOULBURN_DEMONIC_CIRCLE = 79438,
    SPELL_WARLOCK_IMMOLATE_DOT = 157736,
    SPELL_WARLOCK_IMMOLATE_FIRE_AND_BRIMSTONE = 108686,
    SPELL_WARLOCK_IMMOLATE_PROC = 193541,
    SPELL_WARLOCK_WRATH_OF_CONSUMPTION_PROC = 199646,
    SPELL_WARLOCK_IMPENDING_DOOM = 196270,
    SPELL_WARLOCK_IMPENDING_DOOM_SUMMON = 196271,
    SPELL_WARLOCK_IMPLOSION_DAMAGE = 196278,
    SPELL_WARLOCK_IMPLOSION_JUMP = 205205,
    SPELL_NIGHTFALL_BUFF = 264571,
    SPELL_WARLOCK_IMPROVED_DREADSTALKERS = 196272,
    SPELL_WARLOCK_IMPROVED_SOUL_FIRE_PCT = 85383,
    SPELL_WARLOCK_IMPROVED_SOUL_FIRE_STATE = 85385,
    SPELL_WARLOCK_INCINERATE = 29722,
    SPELL_WARLOCK_ITEM_S12_TIER_4 = 131632,
    SPELL_WARLOCK_KIL_JAEDENS_CUNNING_PASSIVE = 108507,
    SPELL_WARLOCK_LIFE_TAP_ENERGIZE = 31818,
    SPELL_WARLOCK_LIFE_TAP_ENERGIZE_2 = 32553,
    SPELL_WARLOCK_METAMORPHOSIS = 103958,
    SPELL_WARLOCK_MOLTEN_CORE = 122355,
    SPELL_WARLOCK_MOLTEN_CORE_AURA = 122351,
    SPELL_WARLOCK_NETHER_TALENT = 91713,
    SPELL_WARLOCK_NETHER_WARD = 91711,
    SPELL_WARLOCK_DEMONIC_GATEWAY_VISUAL = 113900,
    SPELL_WARLOCK_NIGHTFALL = 108558,
    SPELL_WARLOCK_PHANTOMATIC_SINGULARITY = 205179,
    SPELL_WARLOCK_PHANTOMATIC_SINGULARITY_DAMAGE = 205246,
    SPELL_WARLOCK_SHADOWBOLT = 686,
    SPELL_WARLOCK_POWER_TRIP = 196605,
    SPELL_WARLOCK_POWER_TRIP_ENERGIZE = 216125,
    SPELL_WARLOCK_PYROCLASM = 123686,
    SPELL_WARLOCK_SEED_OF_CORRUPTION_DETONATION = 27285,
    SPELL_WARLOCK_INTERNAL_COMBUSTION_DMG = 266136,
    SPELL_WARLOCK_RAIN_OF_FIRE_DAMAGE = 42223,
    SPELL_WARLOCK_ROARING_BLAZE = 205184,
    SPELL_WARLOCK_SEED_OF_CURRUPTION = 27243,
    SPELL_WARLOCK_SEED_OF_CURRUPTION_DAMAGE = 27285,
    SPELL_WARLOCK_T16_4P_INTERNAL_CD = 145165,
    SPELL_WARLOCK_SHADOW_BOLT = 686,
    SPELL_WARLOCK_SHADOW_BOLT_SHOULSHARD = 194192,
    SPELL_WARLOCK_SHADOW_TRANCE = 17941,
    SPELL_WARLOCK_SHADOW_WARD = 6229,
    SPELL_WARLOCK_SHADOWBURN_ENERGIZE = 125882,
    SPELL_WARLOCK_SHADOWFLAME = 47960,
    SPELL_WARLOCK_SHADOWY_INSPIRATION = 196269,
    SPELL_WARLOCK_SHADOWY_INSPIRATION_EFFECT = 196606,
    SPELL_WARLOCK_SHIELD_OF_SHADOW = 115232,
    SPELL_WARLOCK_SOULSHATTER_ENERGIZE = 212921,
    SPELL_WARLOCK_SOULSHATTER_HASTE = 236471,
    SPELL_WARLOCK_SIPHON_LIFE_HEAL = 63106,
    SPELL_WARLOCK_CASTING_CIRCLE = 221703,
    SPELL_WARLOCK_SOUL_CONDUIT_REFUND = 215942,
    SPELL_WARLOCK_SOUL_LEECH = 228974,
    SPELL_WARLOCK_SOUL_LEECH_ABSORB = 108366,
    SPELL_WARLOCK_SOUL_LEECH_AURA = 108370,
    SPELL_WARLOCK_SOUL_LINK_DUMMY_AURA = 108446,
    SPELL_WARLOCK_SOULSHATTER = 32835,
    SPELL_WARLOCK_DIMENSIONAL_RIFT = 196586,
    SPELL_WARLOCK_SOULSNATCHER_PROC = 196234,
    SPELL_WARLOCK_SOULWELL_CREATE_HEALTHSTONE = 34130,
    SPELL_WARLOCK_METAMORPHOSIS_SPELL_REPLACEMENTS = 103965,
    SPELL_WARLOCK_METAMORPHOSIS_ADDITIONAL_AURA = 54879,
    SPELL_WARLOCK_METAMORPHOSIS_ADDITIONAL_AURA_2 = 54817,
    SPELL_WARLOCK_SOW_THE_SEEDS = 196226,
    SPELL_WARLOCK_SPAWN_PURPLE_DEMONIC_GATEWAY = 113890,
    SPELL_WARLOCK_SUMMON_DREADSTALKER = 193332,
    SPELL_WARLOCK_SUPPLANT_DEMONIC_COMMAND = 119904,
    SPELL_WARLOCK_THREATENING_PRESENCE = 112042,
    SPELL_WARLOCK_TWILIGHT_WARD_METAMORPHOSIS_S12 = 131624,
    SPELL_WARLOCK_TWILIGHT_WARD_S12 = 131623,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION = 30108,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DAMAGE_1 = 233490,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DAMAGE_2 = 233496,
    SPELL_WARLOCK_SOUL_FIRE = 6353,
    SPELL_WARLOCK_SOUL_FIRE_METAMORPHOSIS = 104027,
    SPELL_WARLOCK_SINGE_MAGIC = 212620,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DAMAGE_3 = 233497,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DAMAGE_4 = 233498,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DAMAGE_5 = 233499,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL = 196364,
    SPELL_WARLOCK_INTERNAL_COMBUSTION_TALENT_AURA = 266134,
    SPELL_WARLOCK_WRITHE_IN_AGONY = 196102,
    SPELL_WARLOCK_ERADICATION_AURA = 196412,
    SPELL_WARLOCK_FATAL_ECHOES = 199257,
    SPELL_WARLOCK_DOOM_ENERGIZE = 193318,
    SPELL_WARLOCK_ERADICATION_DEBUF = 196414,
    SPELL_WARLOCK_SWEET_SOULS = 199220,
    SPELL_WARLOCK_SWEET_SOULS_HEAL = 199221,
    SPELL_WARLOCK_PVP_4P_BONUS = 143395,
    SPELL_SHADOW_EMBRACE = 32388,
    SPELL_SHADOW_EMBRACE_TARGET_DEBUFF = 32390,
};

enum WarlockSpellIcons
{
    WARLOCK_ICON_ID_IMPROVED_LIFE_TAP = 208,
    WARLOCK_ICON_ID_MANA_FEED = 1982
};

enum MiscSpells
{
    SPELL_GEN_REPLENISHMENT = 57669,
    SPELL_PRIEST_SHADOW_WORD_DEATH = 32409
};

enum eGatewaySpells
{
    SPELL_WARLOCK_DEMONIC_GATEWAY_ACTIVATE = 113902,
    SPELL_WARLOCK_DEMONIC_GATEWAY_DEBUFF = 113942,
    TeleportVisualGreen = 236762,
    TeleportVisualPurple = 236671
};

enum FreakzWarlockNPCs
{
    NPC_WARLOCK_DEMONIC_GATEWAY_PURPLE = 59271,
    NPC_WARLOCK_DEMONIC_GATEWAY_GREEN = 59262,
    // pets
    NPC_WARLOCK_PET_IMP = 416,
    NPC_WARLOCK_PET_FEL_IMP = 58959,
    NPC_WARLOCK_PET_VOIDWALKER = 1860,
    NPC_WARLOCK_PET_VOIDLORD = 58960,
    NPC_WARLOCK_PET_SUCCUBUS = 1863,
    NPC_WARLOCK_PET_SHIVARRA = 58963,
    NPC_WARLOCK_PET_FEL_HUNTER = 417,
    NPC_WARLOCK_PET_OBSERVER = 58964,
    NPC_WARLOCK_PET_FELGUARD = 17252,
    NPC_WARLOCK_PET_WRATHGUARD = 58965,
};

enum FreakzWarlockSpellVisuals
{
    VISUAL_MALEFIC_GRASP = 25955,
};

// Demonwrath damage - 193439
class spell_warl_demonwrath : public SpellScript
{
    PrepareSpellScript(spell_warl_demonwrath);

    void SelectTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        std::list<Creature*> pets;
        caster->GetCreatureListInGrid(pets, 100.0f);

        pets.remove_if([caster](Creature* creature)
        {
            if (creature == caster)
                return true;
            if (!creature->HasAura(SPELL_WARLOCK_DEMONWRATH_AURA))
                return true;
            if (creature->GetCreatureType() != CREATURE_TYPE_DEMON)
                return true;
            return false;
        });

        targets.remove_if([pets, caster](WorldObject* obj)
        {
            if (!obj->ToUnit())
                return true;
            if (!caster->IsValidAttackTarget(obj->ToUnit()))
                return true;
            bool inRange = false;
            for (Unit* pet : pets)
                if (pet->GetExactDist(obj) <= 10.0f)
                    inRange = true;

            return !inRange;
        });
    }

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (Aura* aur = caster->GetAura(SPELL_WARLOCK_DEMONIC_CALLING))
            if (AuraEffect* aurEff = aur->GetEffect(EFFECT_1))
                if (roll_chance_i(aurEff->GetBaseAmount()))
                    caster->CastSpell(caster, SPELL_WARLOCK_DEMONIC_CALLING_TRIGGER, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_demonwrath::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_demonwrath::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// Demonwrath periodic - 193440
class spell_warl_demonwrath_periodic : public AuraScript
{
    PrepareAuraScript(spell_warl_demonwrath_periodic);

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        int32 rollChance = GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints;
        if (roll_chance_i(rollChance))
            caster->CastSpell(caster, SPELL_WARLOCK_DEMONWRATH_SOULSHARD, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonwrath_periodic::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Demonbolt - 157695
class spell_warl_demonbolt : public SpellScript
{
    PrepareSpellScript(spell_warl_demonbolt);

    int32 _summons = 0;

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        int32 damage = GetHitDamage();
        AddPct(damage, _summons * 20);
        SetHitDamage(damage);
    }

    void CountSummons(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        for (WorldObject* wo : targets)
        {
            if (!wo->ToCreature())
                continue;
            if (wo->ToCreature()->GetOwner() != caster)
                continue;
            if (wo->ToCreature()->GetCreatureType() != CREATURE_TYPE_DEMON)
                continue;

            _summons++;
        }

        targets.clear();
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_demonbolt::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_demonbolt::CountSummons, EFFECT_2, TARGET_UNIT_CASTER_PET);
    }
};

// Immolate Dot - 157736
class spell_warlock_immolate_dot : public SpellScriptLoader
{
public:
    spell_warlock_immolate_dot() : SpellScriptLoader("spell_warlock_immolate_dot") {}

    class spell_warlock_immolate_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_immolate_dot_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Aura* aur = caster->GetAura(SPELL_WARLOCK_CHANNEL_DEMONFIRE_ACTIVATOR))
                aur->RefreshDuration();

            if (GetAura()->Variables.Exist("Spells.AffectedByRoaringBlaze"))
            {
                int32 damage = GetEffect(EFFECT_0)->GetDamage();
                AddPct(damage, 25);
                            
                GetEffect(EFFECT_0)->SetDamage(damage);
                GetAura()->SetNeedClientUpdateForTargets();
                
            }
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_WARLOCK_CHANNEL_DEMONFIRE_ACTIVATOR, true);

            GetAura()->Variables.Remove("Spells.AffectedByRoaringBlaze");
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            std::list<Unit*> enemies;
            Trinity::AnyUnfriendlyUnitInObjectRangeCheck check(caster, caster, 100.f);
            Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(caster, enemies, check);
            Cell::VisitAllObjects(caster, searcher, 100.f);
            enemies.remove_if(Trinity::UnitAuraCheck(false, SPELL_WARLOCK_IMMOLATE_DOT, caster->GetGUID()));
            if (enemies.empty())
                if (Aura* aur = caster->GetAura(SPELL_WARLOCK_CHANNEL_DEMONFIRE_ACTIVATOR))
                    aur->SetDuration(0);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warlock_immolate_dot_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_immolate_dot_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            AfterEffectRemove += AuraEffectRemoveFn(spell_warlock_immolate_dot_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_immolate_dot_AuraScript();
    }
};

// Immolate proc - 193541
class spell_warl_immolate_aura : public AuraScript
{
    PrepareAuraScript(spell_warl_immolate_aura);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_WARLOCK_IMMOLATE_DOT)
        {
            int32 rollChance = GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints;
            rollChance = GetCaster()->ModifyPower(POWER_SOUL_SHARDS, 2.5f);
            bool crit = (eventInfo.GetHitMask() & PROC_HIT_CRITICAL) != 0;
            return crit ? roll_chance_i(rollChance * 2) : roll_chance_i(rollChance);
        }
        return false;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_immolate_aura::CheckProc);
    }
};

// Shadow Bolt - 686
class spell_warl_shadow_bolt : public SpellScript
{
    PrepareSpellScript(spell_warl_shadow_bolt);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_SHADOW_BOLT, SPELL_WARLOCK_SHADOW_BOLT_SHOULSHARD });
    }

    void HandleOnCast()
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, SPELL_WARLOCK_SHADOW_BOLT_SHOULSHARD, true);

        if (caster->HasAura(SPELL_WARLOCK_SHADOWY_INSPIRATION_EFFECT))
            caster->RemoveAura(SPELL_WARLOCK_SHADOWY_INSPIRATION_EFFECT);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warl_shadow_bolt::HandleOnCast);
    }
};

// 710 - Banish
class spell_warl_banish : public SpellScript
{
    PrepareSpellScript(spell_warl_banish);

    void HandleBanish(SpellMissInfo missInfo)
    {
        if (missInfo != SPELL_MISS_IMMUNE)
            return;

        if (Unit* target = GetHitUnit())
        {
            // Casting Banish on a banished target will remove applied aura
            if (Aura * banishAura = target->GetAura(GetSpellInfo()->Id, GetCaster()->GetGUID()))
                banishAura->Remove();
        }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_warl_banish::HandleBanish);
    }
};

// 17962 - Conflagrate
class spell_warl_conflagrate : public SpellScript
{
    PrepareSpellScript(spell_warl_conflagrate);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_IMMOLATE });
    }

    void HandleHit(SpellEffIndex /*effindex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        if (caster->HasAura(SPELL_WARLOCK_BACKDRAFT_AURA))
            caster->CastSpell(caster, SPELL_WARLOCK_BACKDRAFT, true);

        caster->ModifyPower(POWER_SOUL_SHARDS, 7.5f);

        if (caster->HasAura(SPELL_WARLOCK_ROARING_BLAZE))
        {
            if (Aura* aur = target->GetAura(SPELL_WARLOCK_IMMOLATE_DOT, caster->GetGUID()))
            {
                if (AuraEffect* aurEff = aur->GetEffect(EFFECT_0))
                {
                    int32 damage = aurEff->GetDamage();
                    aurEff->SetDamage(AddPct(damage, 25));
                    aur->SetNeedClientUpdateForTargets();
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_conflagrate::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Create Healthstone - 23517
class spell_warl_create_healthstone : public SpellScript
{
    PrepareSpellScript(spell_warl_create_healthstone);

    void FilterTargets(WorldObject*& target)
    {
        if (!target)
            return;

        if (Player* player = target->ToPlayer())
        {
            if (Item* item = player->GetItemByEntry(5512))
            {
                item->SetSpellCharges(1, -3);
                target = nullptr;
            }
        }
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_warl_create_healthstone::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER);
    }
};

// 603 - Doom
class spell_warlock_doom : public SpellScriptLoader
{
public:
    spell_warlock_doom() : SpellScriptLoader("spell_warlock_doom") { }

    class spell_warlock_doom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_doom_AuraScript);

        void PeriodicTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_WARLOCK_DOOM_ENERGIZE, true);
            if (caster->HasAura(SPELL_WARLOCK_IMPENDING_DOOM))
                caster->CastSpell(GetTarget(), SPELL_WARLOCK_WILD_IMP_SUMMON, true);

            if (caster->HasAura(SPELL_WARLOCK_DOOM_DOUBLED) && roll_chance_i(25))
                GetEffect(EFFECT_0)->SetDamage(aurEff->GetDamage() * 2);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_doom_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_doom_AuraScript();
    }
};


// 48018 - Demonic Circle: Summon
/// Updated 4.3.4
class spell_warl_demonic_circle_summon : public AuraScript
{
    PrepareAuraScript(spell_warl_demonic_circle_summon);

    bool firstTick = true;

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
    {
        // If effect is removed by expire remove the summoned demonic circle too.
        if (!(mode & AURA_EFFECT_HANDLE_REAPPLY))
            GetTarget()->RemoveGameObject(GetId(), true);

        if (AuraApplication* aurApp = GetTarget()->GetAuraApplication(GetSpellInfo()->Id, GetTarget()->GetGUID()))
            aurApp->SendFakeAuraUpdate(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
    }

    void HandleDummyTick(AuraEffect const* /*aurEff*/)
    {
        if (GetTarget()->GetGameObject(GetId()))
        {
            Unit* target = GetTarget();
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT);

            if (!target || !spellInfo)
                return;

            GameObject* circle = target->GetGameObject(GetId());
            if (!circle)
            {
                if (firstTick)
                    GetAura()->SetDuration(0);

                return;
            }

            // Here we check if player is in demonic circle teleport range, if so add
            // WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST; allowing him to cast the WARLOCK_DEMONIC_CIRCLE_TELEPORT.
            // If not in range remove the WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST.
            if (AuraApplication* circleSummon = target->GetAuraApplication(GetSpellInfo()->Id, target->GetGUID()))
            {
                if (target->IsWithinDist(circle, spellInfo->GetMaxRange(true)))
                    circleSummon->SendFakeAuraUpdate(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, false);
                else if (target->HasVisibleAura(circleSummon))
                    circleSummon->SendFakeAuraUpdate(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
            }
            else
                target->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectApplyFn(spell_warl_demonic_circle_summon::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_circle_summon::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 48020 - Demonic Circle: Teleport
/// Updated 4.3.4
class spell_warl_demonic_circle_teleport : public AuraScript
{
    PrepareAuraScript(spell_warl_demonic_circle_teleport);

    void HandleTeleport(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
        {
            if (GameObject* circle = player->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON))
            {
                player->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());
                player->RemoveMovementImpairingAuras();
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_circle_teleport::HandleTeleport, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Demonic Empowerment - 193396
class spell_warl_demonic_empowerment : public SpellScript
{
    PrepareSpellScript(spell_warl_demonic_empowerment);

    void HandleTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        targets.remove_if([caster](WorldObject* target)
        {
            if (!target->ToCreature())
                return true;

            if (!caster->IsFriendlyTo(target->ToUnit()))
                return true;

            if (target->ToCreature()->GetCreatureType() != CREATURE_TYPE_DEMON)
                return true;

            return false;
        });
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (caster->HasAura(SPELL_WARLOCK_SHADOWY_INSPIRATION))
            caster->CastSpell(caster, SPELL_WARLOCK_SHADOWY_INSPIRATION_EFFECT, true);

        if (caster->HasAura(SPELL_WARLOCK_POWER_TRIP) && caster->IsInCombat() && roll_chance_i(50))
            caster->CastSpell(caster, SPELL_WARLOCK_POWER_TRIP_ENERGIZE, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_demonic_empowerment::HandleTargets, EFFECT_ALL, TARGET_UNIT_CASTER_PET);
        OnCast += SpellCastFn(spell_warl_demonic_empowerment::HandleCast);
    }
};

// 67518, 19505 - Devour Magic
class spell_warl_devour_magic : public SpellScript
{
    PrepareSpellScript(spell_warl_devour_magic);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_DEVOUR_MAGIC_HEAL });
    }

    void OnSuccessfulDispel(SpellEffIndex /*effIndex*/)
    {
        if (SpellEffectInfo const* effect = GetSpellInfo()->GetEffect(EFFECT_1))
        {
            Unit* caster = GetCaster();
            int32 heal_amount = effect->CalcValue(caster);

            caster->CastCustomSpell(caster, SPELL_WARLOCK_DEVOUR_MAGIC_HEAL, &heal_amount, nullptr, nullptr, true);

            // Glyph of Felhunter
            if (Unit* owner = caster->GetOwner())
                if (owner->GetAura(SPELL_WARLOCK_GLYPH_OF_DEMON_TRAINING))
                    owner->CastCustomSpell(owner, SPELL_WARLOCK_DEVOUR_MAGIC_HEAL, &heal_amount, nullptr, nullptr, true);
        }
    }

    void Register() override
    {
        OnEffectSuccessfulDispel += SpellEffectFn(spell_warl_devour_magic::OnSuccessfulDispel, EFFECT_0, SPELL_EFFECT_DISPEL);
    }
};

// 48181 - Haunt
class aura_warl_haunt : public AuraScript
{
    PrepareAuraScript(aura_warl_haunt);

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster || GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
            return;

        caster->GetSpellHistory()->ResetCooldown(SPELL_WARLOCK_HAUNT, true);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectApplyFn(aura_warl_haunt::HandleRemove, EFFECT_1, SPELL_AURA_MOD_SCHOOL_MASK_DAMAGE_FROM_CASTER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

// 755 - Health Funnel
/// Updated 4.3.4
class spell_warl_health_funnel : public AuraScript
{
    PrepareAuraScript(spell_warl_health_funnel);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_HEALTH_FUNNEL_HEAL });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* target = GetUnitOwner();
        Unit* caster = GetCaster();
        if (!target || !caster)
            return;

        CustomSpellValues values;
        int32 damage = caster->CountPctFromMaxHealth(4);
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage * 2);

        caster->CastCustomSpell(SPELL_WARLOCK_HEALTH_FUNNEL_HEAL, values, target, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_health_funnel::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 27243 - Seed of Corruption
class spell_warl_seed_of_corruption : public SpellScript
{
    PrepareSpellScript(spell_warl_seed_of_corruption);

    void HandleBeforeCast()
    {
        _maxAdditionalTargets = 0;

        if (Aura* aura = GetCaster()->GetAura(SPELL_WARLOCK_SOW_THE_SEEDS))
            _maxAdditionalTargets += aura->GetSpellEffectInfo(EFFECT_0)->BasePoints;
    }

    void CorrectTargets(std::list<WorldObject*>& targets)
    {
        std::list<WorldObject*> correctedTargets;
        for (WorldObject* obj : targets)
        {
            if (obj == GetExplTargetUnit())
            {
                correctedTargets.push_back(obj);
                continue;
            }

            if (_maxAdditionalTargets)
            {
                correctedTargets.push_back(obj);
                --_maxAdditionalTargets;
            }
        }

        targets = correctedTargets;
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_warl_seed_of_corruption::HandleBeforeCast);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption::CorrectTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption::CorrectTargets, EFFECT_2, TARGET_UNIT_DEST_AREA_ENEMY);
    }

private:
    uint8 _maxAdditionalTargets;
};

// 27243 - Seed of Corruption
class aura_warl_seed_of_corruption : public AuraScript
{
    PrepareAuraScript(aura_warl_seed_of_corruption);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_SEED_OF_CURRUPTION_DAMAGE });
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        aurEff->GetBase()->Remove(AURA_REMOVE_BY_EXPIRE);
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget()->GetPosition(), SPELL_WARLOCK_SEED_OF_CURRUPTION_DAMAGE, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_warl_seed_of_corruption::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 27285 - Seed of Corruption damage
class spell_warlock_seed_of_corruption_damage : public SpellScriptLoader
{
public:
    spell_warlock_seed_of_corruption_damage() : SpellScriptLoader("spell_warlock_seed_of_corruption_damage") { }

    class spell_warlock_seed_of_corruption_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_seed_of_corruption_damage_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            // chain explosion
            if (Aura* soc = target->GetAura(SPELL_WARLOCK_SEED_OF_CORRUPTION, caster->GetGUID()))
            {
                if (!soc->Variables.Exist("_alreadyDetonated"))
                {
                    soc->Variables.Set("_alreadyDetonated", 0);
                    soc->Remove();
                    caster->CastSpell(target, SPELL_WARLOCK_SEED_OF_CORRUPTION_DETONATION, true);
                }
            }

            caster->CastSpell(target, SPELL_WARLOCK_CORRUPTION_TRIGGERED, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_seed_of_corruption_damage_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_seed_of_corruption_damage_SpellScript();
    }
};

// Soul Leech aura - 228974
class spell_warl_soul_leech_aura : public AuraScript
{
    PrepareAuraScript(spell_warl_soul_leech_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_DEMONSKIN });
    }

    bool OnCheckProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return false;

        int32 basePoints = GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints;
        int32 absorb = ((eventInfo.GetDamageInfo() ? eventInfo.GetDamageInfo()->GetDamage() : 0) * basePoints) / 100.f;

        // Add remaining amount if already applied
        if (Aura* aur = caster->GetAura(SPELL_WARLOCK_SOUL_LEECH_ABSORB))
            if (AuraEffect* aurEff = aur->GetEffect(EFFECT_0))
                absorb += aurEff->GetAmount();

        // Cannot go over 15% (or 20% with Demonskin) max health
        int32 basePointNormal = GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints;
        int32 basePointDS = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONSKIN)->GetEffect(EFFECT_1)->BasePoints;
        int32 totalBP = caster->HasAura(SPELL_WARLOCK_DEMONSKIN) ? basePointDS : basePointNormal;
        int32 threshold = (caster->GetMaxHealth() * totalBP) / 100.f;
        absorb = std::min(absorb, threshold);

        caster->CastCustomSpell(SPELL_WARLOCK_SOUL_LEECH_ABSORB, SPELLVALUE_BASE_POINT0, absorb, caster, TRIGGERED_FULL_MASK);
        return true;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_soul_leech_aura::OnCheckProc);
    }
};

// 29858 - Soulshatter
/// Updated 4.3.4
class spell_warl_soulshatter : public SpellScriptLoader
{
public:
    spell_warl_soulshatter() : SpellScriptLoader("spell_warl_soulshatter") { }

    class spell_warl_soulshatter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soulshatter_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOULSHATTER))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (target->CanHaveThreatList() && target->getThreatManager().getThreat(caster) > 0.0f)
                    caster->CastSpell(target, SPELL_WARLOCK_SOULSHATTER, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soulshatter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soulshatter_SpellScript();
    }
};

// 30108 - Unstable Affliction
class spell_warlock_unstable_affliction : public SpellScriptLoader
{
public:
    spell_warlock_unstable_affliction() : SpellScriptLoader("spell_warlock_unstable_affliction") { }

    class spell_warlock_unstable_affliction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_unstable_affliction_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            std::vector<int32> uaspells = { SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT5, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT4,
                                            SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT3, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT2,
                                            SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT1 };

            int32 spellToCast = 0;
            int32 minDuration = 10000;
            int32 lowestDurationSpell = 0;
            for (int32 spellId : uaspells)
            {
                if (Aura* ua = target->GetAura(spellId, caster->GetGUID()))
                {
                    if (ua->GetDuration() < minDuration)
                    {
                        minDuration = ua->GetDuration();
                        lowestDurationSpell = ua->GetSpellInfo()->Id;
                    }
                }
                else
                    spellToCast = spellId;
            }

            if (!spellToCast)
                caster->CastSpell(target, lowestDurationSpell, true);
            else
                caster->CastSpell(target, spellToCast, true);

            if (caster->HasAura(SPELL_WARLOCK_CONTAGION))
                caster->CastSpell(target, SPELL_WARLOCK_CONTAGION_DEBUFF, true);

            if (caster->HasAura(SPELL_WARLOCK_COMPOUNDING_HORROR))
                caster->CastSpell(target, SPELL_WARLOCK_COMPOUNDING_HORROR_DAMAGE, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_unstable_affliction_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_unstable_affliction_SpellScript();
    }
};

// 233490 - Unstable Affliction dispel
class spell_warlock_unstable_affliction_dispel : public SpellScriptLoader
{
public:
    spell_warlock_unstable_affliction_dispel() : SpellScriptLoader("spell_warlock_unstable_affliction_dispel") { }

    class spell_warlock_unstable_affliction_dispel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_unstable_affliction_dispel_AuraScript);

        void HandleDispel(DispelInfo* dispelInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* dispeller = dispelInfo->GetDispeller()) {
                int32 damage = GetAura()->GetEffect(EFFECT_0)->GetDamage() * 4;
                caster->CastCustomSpell(dispeller, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL, &damage, nullptr, nullptr, true);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            if (!caster || !target || !caster->ToPlayer())
                return;

            if (caster->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION_RANK2))
            {
                if (GetTargetApplication() && GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                {
                    if (caster->Variables.Exist("_uaLockout"))
                        return;

                    caster->CastSpell(caster, SPELL_WARLOCK_UNSTABLE_AFFLICTION_ENERGIZE, true);

                    caster->Variables.Set("_uaLockout", 0);
                    caster->GetScheduler().Schedule(100ms, [caster](TaskContext /*context*/)
                    {
                        caster->Variables.Remove("_uaLockout");
                    });
                }
            }

            // When Unstable Affliction expires, it has a 6% chance to reapply itself.
            if (GetTargetApplication() && GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                if (roll_chance_i(caster->GetAuraEffectAmount(SPELL_WARLOCK_FATAL_ECHOES, EFFECT_0)))
                    caster->GetScheduler().Schedule(100ms, [this, caster, target](TaskContext /*context*/)
                {
                    caster->CastSpell(target, GetSpellInfo()->Id, true);
                });
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warlock_unstable_affliction_dispel_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            AfterDispel += AuraDispelFn(spell_warlock_unstable_affliction_dispel_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_unstable_affliction_dispel_AuraScript();
    }
};

enum lifeTap
{
    SPELL_WARLOCK_LIFE_TAP = 1454,
    SPELL_WARLOCK_LIFE_TAP_GLYPH = 63320,
};

// Life Tap - 1454
class spell_warl_life_tap : public SpellScript
{
    PrepareSpellScript(spell_warl_life_tap);

    SpellCastResult CheckLife()
    {
        if (GetCaster()->GetHealthPct() > 15.0f || GetCaster()->HasAura(SPELL_WARLOCK_LIFE_TAP_GLYPH))
            return SPELL_CAST_OK;
        return SPELL_FAILED_FIZZLE;
    }

    void HandleOnHitTarget(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        if (!GetCaster()->HasAura(SPELL_WARLOCK_LIFE_TAP_GLYPH))
            GetCaster()->EnergizeBySpell(GetCaster(), SPELL_WARLOCK_LIFE_TAP, int32(GetCaster()->GetMaxHealth() * GetSpellInfo()->GetEffect(0)->BasePoints / 100), POWER_MANA);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_warl_life_tap::CheckLife);
        OnEffectHitTarget += SpellEffectFn(spell_warl_life_tap::HandleOnHitTarget, EFFECT_0, SPELL_EFFECT_ENERGIZE);
    }
};

// 980 - Agony
class spell_warlock_agony : public SpellScriptLoader
{
public:
    spell_warlock_agony() : SpellScriptLoader("spell_warlock_agony") {}

    class spell_warlock_agony_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_agony_AuraScript);

        void HandleDummyPeriodic(AuraEffect const* auraEffect)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            float soulShardAgonyTick = caster->Variables.GetValue<float>("SoulShardAgonyTick", frand(0.0f, 99.0f));
            soulShardAgonyTick += 16.0f;

            if (soulShardAgonyTick >= 100.0f)
            {
                soulShardAgonyTick = frand(0.0f, 99.0f);

                if (Player* player = GetCaster()->ToPlayer())
                    if (player->GetPower(POWER_SOUL_SHARDS) < player->GetMaxPower(POWER_SOUL_SHARDS))
                        player->SetPower(POWER_SOUL_SHARDS, player->GetPower(POWER_SOUL_SHARDS) + 10);
            }

            caster->Variables.Set("SoulShardAgonyTick", soulShardAgonyTick);

            // If we have more than maxStackAmount, dont do anything
            if (GetStackAmount() >= auraEffect->GetBase()->GetMaxStackAmount())
                return;

            SetStackAmount(GetStackAmount() + 1);
        }

        void OnRemove(const AuraEffect* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // If last agony removed, remove tick counter
            if (Unit* caster = GetCaster())
                if (!caster->GetOwnedAura(SPELL_WARLOCK_AGONY))
                    caster->Variables.Remove("SoulShardAgonyTick");
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_agony_AuraScript::HandleDummyPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectRemove += AuraEffectRemoveFn(spell_warlock_agony_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_agony_AuraScript();
    }
};

// Burning Rush - 111400
class spell_warl_burning_rush : public SpellScript
{
    PrepareSpellScript(spell_warl_burning_rush);

    bool _isRemove = false;

    SpellCastResult CheckCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        if (caster->HealthBelowPct(5))
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        return SPELL_CAST_OK;
    }

    void HandleBeforeCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (caster->HasAura(SPELL_WARLOCK_BURNING_RUSH))
            _isRemove = true;
    }

    void HandleHit()
    {
        if (_isRemove)
            GetCaster()->RemoveAurasDueToSpell(SPELL_WARLOCK_BURNING_RUSH);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_warl_burning_rush::HandleBeforeCast);
        AfterHit += SpellHitFn(spell_warl_burning_rush::HandleHit);
        OnCheckCast += SpellCheckCastFn(spell_warl_burning_rush::CheckCast);
    }
};

class aura_warl_burning_rush : public AuraScript
{
    PrepareAuraScript(aura_warl_burning_rush);

    void OnTick(const AuraEffect* /* aurEff */)
    {
        if (GetCaster())
        {
            // This way if the current tick takes you below 4%, next tick won't execute
            uint64 basepoints = GetCaster()->CountPctFromMaxHealth(4);
            if (GetCaster()->GetHealth() <= basepoints ||
                GetCaster()->GetHealth() - basepoints <= basepoints)
                GetAura()->SetDuration(0);
        }
    }
    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_warl_burning_rush::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
    }
};

// Chaos Bolt - 116858 and 215279
class spell_warl_chaos_bolt : public SpellScript
{
    PrepareSpellScript(spell_warl_chaos_bolt);

    void HandleCritChance(Unit* /*victim*/, float& chance)
    {
        chance = 100.f;
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        Player * caster = GetCaster()->ToPlayer();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        if (caster->HasAura(SPELL_WARLOCK_INTERNAL_COMBUSTION_TALENT_AURA)) {
            int32 maxDuration = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_INTERNAL_COMBUSTION_TALENT_AURA)->GetEffect(EFFECT_0)->BasePoints * 1000;
            if (AuraApplication * immolate = target->GetAuraApplication(SPELL_WARLOCK_IMMOLATE_DOT, caster->GetGUID())) {
                AuraEffect *pDamageEffect = immolate->GetBase()->GetEffect(EFFECT_0);
                int32 damageToRetain = pDamageEffect->GetDamage();
                if (immolate->GetBase()->GetDuration() < maxDuration) {
                    damageToRetain *= pDamageEffect->GetTotalTicks() - pDamageEffect->GetTickNumber();
                }
                else {
                    damageToRetain *= maxDuration / pDamageEffect->GetPeriod();
                }
                immolate->GetBase()->ModDuration(-maxDuration);
                caster->CastCustomSpell(target, SPELL_WARLOCK_INTERNAL_COMBUSTION_DMG, &damageToRetain, nullptr, nullptr, true);
            }
        }

        if (caster->HasAura(SPELL_WARLOCK_ERADICATION_AURA)) {
            caster->AddAura(SPELL_WARLOCK_ERADICATION_DEBUF, target);
        }
    }
    void Register() override
    {
        OnCalcCritChance += SpellOnCalcCritChanceFn(spell_warl_chaos_bolt::HandleCritChance);
        OnEffectHitTarget += SpellEffectFn(spell_warl_chaos_bolt::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_warl_conflagrate_aura : public SpellScript
{
    PrepareSpellScript(spell_warl_conflagrate_aura);

    void HandleOnHit()
    {
        if (Player* _player = GetCaster()->ToPlayer())
        {
            if (Unit* target = GetHitUnit())
            {
                if (!target->HasAura(SPELL_WARLOCK_IMMOLATE) && !_player->HasAura(SPELL_WARLOCK_GLYPH_OF_CONFLAGRATE))
                    if (target->GetAura(SPELL_WARLOCK_CONFLAGRATE))
                        target->RemoveAura(SPELL_WARLOCK_CONFLAGRATE);

                if (!target->HasAura(SPELL_WARLOCK_IMMOLATE_FIRE_AND_BRIMSTONE))
                    if (target->GetAura(SPELL_WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE))
                        target->RemoveAura(SPELL_WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE);
            }
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warl_conflagrate_aura::HandleOnHit);
    }
};

class spell_warl_dark_regeneration : public AuraScript
{
    PrepareAuraScript(spell_warl_dark_regeneration);

    void HandleApply(const AuraEffect* /*aurEff*/, AuraEffectHandleModes /* mode */)
    {
        if (GetTarget())
            if (Guardian* pet = GetTarget()->GetGuardianPet())
                pet->CastSpell(pet, SPELL_WARLOCK_DARK_REGENERATION, true);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_dark_regeneration::HandleApply, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warl_demonic_call : public SpellScript
{
    PrepareSpellScript(spell_warl_demonic_call);

    void HandleOnHit()
    {
        if (Player* _player = GetCaster()->ToPlayer())
        {
            if (GetHitUnit())
            {
                if (_player->HasAura(SPELL_WARLOCK_DEMONIC_CALL) && !_player->HasAura(SPELL_WARLOCK_DISRUPTED_NETHER))
                {
                    _player->CastSpell(_player, SPELL_WARLOCK_HAND_OF_GULDAN_SUMMON, true);
                    _player->RemoveAura(SPELL_WARLOCK_DEMONIC_CALL);
                }
            }
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warl_demonic_call::HandleOnHit);
    }
};

// Drain Soul - 198590
class spell_warl_drain_soul : public SpellScriptLoader
{
public:
    spell_warl_drain_soul() : SpellScriptLoader("spell_warl_drain_soul") { }

    class spell_warl_drain_soul_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_drain_soul_AuraScript);

        void HandleRemove(const AuraEffect* /*aurEff*/, AuraEffectHandleModes /* mode */)
        {
            if (GetCaster() && GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                GetCaster()->ModifyPower(POWER_SOUL_SHARDS, 10);
        }

        void HandleDummyPeriodic(AuraEffect const* /* auraEffect */)
        {
            Unit* target = GetTarget();
            Unit* caster = GetCaster();
            if (!caster || !target)
                return;

            uint32 DrainSoulData[4][3] =
            {
                { 146739,   EFFECT_0, 131740 }, // Corruption
                { 30108,    EFFECT_0, 131736 },
                { 27243,    EFFECT_0, 132566 },
                { 980,      EFFECT_0, 131737 },
            };

            for (uint8 i = 0; i < 4; i++)
            {
                if (target->GetAuraEffect(DrainSoulData[i][0], DrainSoulData[i][1], GetCaster()->GetGUID()))
                    caster->CastSpell(target, DrainSoulData[i][2], true);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_drain_soul_AuraScript::HandleDummyPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectRemove += AuraEffectApplyFn(spell_warl_drain_soul_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_drain_soul_AuraScript();
    }
};

class spell_warl_glyph_of_soulwell : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_soulwell() : SpellScriptLoader("spell_warl_glyph_of_soulwell") { }

    class spell_warl_glyph_of_soulwell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_glyph_of_soulwell_SpellScript);

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            if (!GetExplTargetDest())
                return;

            if (!GetCaster()->HasAura(SPELL_WARLOCK_GLYPH_OF_SOULWELL))
                return;

            GetCaster()->CastSpell(GetExplTargetDest()->GetPositionX(), GetExplTargetDest()->GetPositionY(), GetExplTargetDest()->GetPositionZ(), SPELL_WARLOCK_GLYPH_OF_SOULWELL_VISUAL, true);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_warl_glyph_of_soulwell_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_glyph_of_soulwell_SpellScript();
    }
};

#define ENTRY_IMP               416
#define ENTRY_VOIDWALKER        1860
#define ENTRY_SUCCUBUS          1863
#define ENTRY_FELHUNTER         417
#define ENTRY_FELGUARD          17252
#define ENTRY_WATER_ELEMENTAL   510
#define ENTRY_TREANT            1964
#define ENTRY_FIRE_ELEMENTAL    15438
#define ENTRY_GHOUL             26125
#define ENTRY_BLOODWORM         28017

// Hand of Gul'Dan - 105174
class spell_warl_hand_of_guldan : public SpellScript
{
    PrepareSpellScript(spell_warl_hand_of_guldan);

    void HandleOnHit()
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetHitUnit())
            {
                int32 nrofsummons = 1;
                nrofsummons += caster->GetPower(POWER_SOUL_SHARDS);
                if (nrofsummons > 4)
                nrofsummons = 4;

                int8 offsetX[4]{ 0, 0, 1, 1 };
                int8 offsetY[4]{ 0, 1, 0, 1 };

                for (int i = 0; i < nrofsummons; i++)
                caster->CastSpell(target->GetPositionX() + offsetX[i], target->GetPositionY() + offsetY[i], target->GetPositionZ(), 104317, true);
                caster->CastSpell(target, SPELL_WARLOCK_HAND_OF_GULDAN_DAMAGE, true);
            }
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warl_hand_of_guldan::HandleOnHit);
    }
};

// Hand of Guldan damage - 86040
class spell_warl_hand_of_guldan_damage : public SpellScriptLoader
{
public:
    spell_warl_hand_of_guldan_damage() : SpellScriptLoader("spell_warl_hand_of_guldan_damage") { }

    class spell_warl_hand_of_guldan_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_hand_of_guldan_damage_SpellScript);

    public:
        spell_warl_hand_of_guldan_damage_SpellScript()
        {
            soulshards = 1;
        }

    private:

        bool Load() override
        {
            soulshards += GetCaster()->GetPower(POWER_SOUL_SHARDS);
            if (soulshards > 4)
            {
                GetCaster()->SetPower(POWER_SOUL_SHARDS, 1);
                soulshards = 4;

            }
            else
                GetCaster()->SetPower(POWER_SOUL_SHARDS, 0);
            return true;
        }

        uint32 soulshards;

        void HandleOnHit(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    uint32 dmg = GetHitDamage();
                    SetHitDamage(dmg*soulshards);

                    if (caster->HasAura(SPELL_WARLOCK_HAND_OF_DOOM))
                        caster->CastSpell(target, SPELL_WARLOCK_DOOM, true);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_hand_of_guldan_damage_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_hand_of_guldan_damage_SpellScript();
    }
};

class spell_warl_shadow_bulwark : public SpellScriptLoader
{
public:
    spell_warl_shadow_bulwark() : SpellScriptLoader("spell_warl_shadow_bulwark") { }

    class spell_warl_shadow_bulwark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadow_bulwark_AuraScript);

        void CalculateAmount(const AuraEffect*, int32& amount, bool &)
        {
            if (Unit* caster = GetCaster())
                amount = caster->CountPctFromMaxHealth(amount);
        }
        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_shadow_bulwark_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_shadow_bulwark_AuraScript();
    }
};

class spell_warl_shadowburn : public SpellScriptLoader
{
public:
    spell_warl_shadowburn() : SpellScriptLoader("spell_warl_shadowburn") { }

    class spell_warl_shadowburn_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadowburn_AuraScript);

        void HandleRemove(const AuraEffect* /*aurEff*/, AuraEffectHandleModes /* mode */)
        {
            if (GetCaster())
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode == AURA_REMOVE_BY_DEATH)
                    GetCaster()->SetPower(POWER_SOUL_SHARDS, GetCaster()->GetPower(POWER_SOUL_SHARDS) + 50);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectApplyFn(spell_warl_shadowburn_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_shadowburn_AuraScript();
    }
};

class spell_warl_void_ray : public SpellScriptLoader
{
public:
    spell_warl_void_ray() : SpellScriptLoader("spell_warl_void_ray") { }

    class spell_warl_void_ray_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_void_ray_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Aura* corruption = target->GetAura(SPELL_WARLOCK_CORRUPTION, _player->GetGUID()))
                    {
                        corruption->SetDuration(corruption->GetDuration() + 4000);
                        corruption->SetNeedClientUpdateForTargets();
                    }
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_warl_void_ray_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_void_ray_SpellScript();
    }
};

// Immolate - 348
class spell_warl_immolate : public SpellScriptLoader
{
public:
    spell_warl_immolate() : SpellScriptLoader("spell_warl_immolate") { }

    class spell_warl_immolate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_immolate_SpellScript);

        void HandleOnHit()
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_WARLOCK_IMMOLATE_DOT, true);
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_CHANNEL_DEMONFIRE_ACTIVATOR, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_warl_immolate_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_immolate_SpellScript();
    }
};

// Create Healthstone (Soulwell) - 34130
class spell_warl_create_healthstone_soulwell : public SpellScript
{
    PrepareSpellScript(spell_warl_create_healthstone_soulwell);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOULWELL_CREATE_HEALTHSTONE))
            return false;
        return true;
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastSpell(GetCaster(), 23517, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_create_healthstone_soulwell::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Call Dreadstalkers - 104316
class spell_warlock_call_dreadstalkers : public SpellScriptLoader
{
public:
    spell_warlock_call_dreadstalkers() : SpellScriptLoader("spell_warlock_call_dreadstalkers") {}

    class spell_warlock_call_dreadstalkers_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_call_dreadstalkers_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            for (int32 i = 0; i < GetEffectValue(); ++i)
                caster->CastSpell(caster, SPELL_WARLOCK_CALL_DREADSTALKERS_SUMMON, true);
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!caster || !target)
                return;

            std::list<Creature*> dreadstalkers;
            caster->GetCreatureListWithEntryInGrid(dreadstalkers, 98035);
            for (Creature* dreadstalker : dreadstalkers)
            {
                if (dreadstalker->GetOwner() == caster)
                {
                    dreadstalker->SetLevel(caster->getLevel());
                    dreadstalker->SetMaxHealth(caster->GetMaxHealth() / 3);
                    dreadstalker->SetHealth(caster->GetHealth() / 3);
                    dreadstalker->AI()->AttackStart(target);
                    dreadstalker->AddThreat(target, 9999999.f);
                }
            }

            if (uint32 impsToSummon = caster->GetAuraEffectAmount(SPELL_WARLOCK_IMPROVED_DREADSTALKERS, EFFECT_0))
                for (uint32 i = 0; i < impsToSummon; ++i)
                    caster->CastSpell(target->GetRandomNearPosition(3.f), SPELL_WARLOCK_WILD_IMP_SUMMON, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_call_dreadstalkers_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            AfterCast += SpellCastFn(spell_warlock_call_dreadstalkers_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_call_dreadstalkers_SpellScript();
    }
};

//5782 - Fear
class spell_warl_fear : public SpellScriptLoader
{
public:
    spell_warl_fear() : SpellScriptLoader("spell_warl_fear") {}

    class spell_warl_fear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fear_SpellScript);

        bool  Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_FEAR))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_FEAR_BUFF))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetExplTargetUnit();
            if (!target)
                return;

            caster->CastSpell(target, SPELL_WARLOCK_FEAR_BUFF, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_fear_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_fear_SpellScript();
    }
};

//204730 - Fear (effect)
class spell_warl_fear_buff : public SpellScriptLoader
{
public:
    spell_warl_fear_buff() : SpellScriptLoader("spell_warl_fear_buff") {}

    class spell_warl_fear_buff_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fear_buff_SpellScript);

        bool  Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_FEAR_BUFF))
                return false;
            return true;
        }

        void HandleAfterHit()
        {
            if (Aura* aura = GetHitAura())
            {
                aura->SetMaxDuration(20 * IN_MILLISECONDS);
                aura->SetDuration(20 * IN_MILLISECONDS);
                aura->RefreshDuration();
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_warl_fear_buff_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_fear_buff_SpellScript();
    }
};

class spell_warl_dark_pact : public AuraScript
{
    PrepareAuraScript(spell_warl_dark_pact);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
        {
            int32 sacrifiedHealth = 0;

            if (Guardian* guardian = caster->GetGuardianPet())
            {
                sacrifiedHealth = guardian->CountPctFromCurHealth(GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints);
                guardian->ModifyHealth(-sacrifiedHealth);
            }
            else
            {
                sacrifiedHealth = caster->CountPctFromCurHealth(GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints);
                caster->ModifyHealth(-sacrifiedHealth);
            }

            amount = CalculatePct(sacrifiedHealth, aurEff->GetBaseAmount());
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_dark_pact::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
    }
};

//80240 - Havoc
class spell_warl_havoc : public SpellScriptLoader
{
public:
    spell_warl_havoc() : SpellScriptLoader("spell_warl_havoc") { }

    class spell_warl_havoc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_havoc_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* victim = procInfo.GetActionTarget())
            {
                if (Unit* target = procInfo.GetProcTarget())
                {
                    if (victim != target)
                    {
                        if (SpellInfo const* spellInfo = aurEff->GetSpellInfo())
                        {
                            uint32 dmg = procInfo.GetDamageInfo()->GetDamage();
                            SpellNonMeleeDamage spell(caster, target, SPELL_WARLOCK_HAVOC, spellInfo->GetSpellXSpellVisualId(caster), SPELL_SCHOOL_MASK_SHADOW);
                            spell.damage = dmg;
                            spell.cleanDamage = spell.damage;
                            caster->DealSpellDamage(&spell, false);
                            caster->SendSpellNonMeleeDamageLog(&spell);
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warl_havoc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_havoc_AuraScript();
    }
};

//146739 - Corruption
class spell_warl_corruption_effect : public AuraScript
{
    PrepareAuraScript(spell_warl_corruption_effect);

    bool  Validate(SpellInfo const* /*spellInfo*/) override
    {
        if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_ABSOLUTE_CORRUPTION))
            return false;
        return true;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        Unit* caster = GetCaster();
        if (!target || !caster)
            return;

        //If the target is a player, only cast for the time said in ABSOLUTE_CORRUPTION
        if (caster->HasAura(SPELL_WARLOCK_ABSOLUTE_CORRUPTION))
            GetAura()->SetDuration(target->GetTypeId() == TYPEID_PLAYER ? sSpellMgr->GetSpellInfo(SPELL_WARLOCK_ABSOLUTE_CORRUPTION)->GetEffect(EFFECT_0)->BasePoints * IN_MILLISECONDS : 60 * 60 * IN_MILLISECONDS); //If not player, 1 hour
    }

    /*
    Removes the aura if the caster is null, far away or dead.
    */
    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* target = GetTarget();
        Unit* caster = GetCaster();
        if (!target)
            return;

        if (!caster)
        {
            target->RemoveAura(SPELL_WARLOCK_CORRUPTION_DAMAGE);
            return;
        }

        if (caster->isDead())
            target->RemoveAura(SPELL_WARLOCK_CORRUPTION_DAMAGE);

        if (!caster->IsInRange(target, 0, 80))
            target->RemoveAura(SPELL_WARLOCK_CORRUPTION_DAMAGE);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_corruption_effect::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_corruption_effect::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};


// Demonic Gateway - 111771
class spell_warl_demonic_gateway : public SpellScriptLoader
{
public:
    spell_warl_demonic_gateway() : SpellScriptLoader("spell_warl_demonic_gateway") { }

    class spell_warl_demonic_gateway_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demonic_gateway_SpellScript);

        void HandleLaunch(SpellEffIndex /*effIndex*/)
        {
            Unit * caster = GetCaster();

            // despawn all other gateways
            std::list<Creature*> targets1, targets2;
            caster->GetCreatureListWithEntryInGrid(targets1, NPC_WARLOCK_DEMONIC_GATEWAY_GREEN, 200.0f);
            caster->GetCreatureListWithEntryInGrid(targets2, NPC_WARLOCK_DEMONIC_GATEWAY_PURPLE, 200.0f);
            targets1.insert(targets1.end(), targets2.begin(), targets2.end());
            for (auto target : targets1)
            {
                if (target->GetOwnerGUID() != caster->GetGUID())
                    continue;
                target->DespawnOrUnsummon(1); // despawn at next tick
            }

            if (WorldLocation const* dest = GetExplTargetDest()) {
                caster->CastSpell(caster, SPELL_WARLOCK_DEMONIC_GATEWAY_SUMMON_PURPLE, true);
                caster->CastSpell(*dest, SPELL_WARLOCK_DEMONIC_GATEWAY_SUMMON_GREEN, true);
            }
        }

        SpellCastResult CheckRequirement()
        {
            // don't allow during Arena Preparation
            if (GetCaster()->HasAura(SPELL_ARENA_PREPARATION))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            // check if player can reach the location
            Spell * spell = GetSpell();
            if (spell->m_targets.HasDst())
            {
                Position pos;
                pos = spell->m_targets.GetDst()->_position.GetPosition();
                Unit * caster = GetCaster();

                if (caster->GetPositionZ() + 6.0f < pos.GetPositionZ() ||
                    caster->GetPositionZ() - 6.0f > pos.GetPositionZ())
                    return SPELL_FAILED_NOPATH;
            }

            return SPELL_CAST_OK;
        }

        void HandleVisual(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            WorldLocation const* dest = GetExplTargetDest();
            if (!caster || !dest)
                return;

            Position pos = dest->GetPosition();

            caster->SendPlaySpellVisual(pos, 20.f, 63644);
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_warl_demonic_gateway_SpellScript::HandleVisual, EFFECT_0, SPELL_EFFECT_SUMMON);
            OnEffectLaunch += SpellEffectFn(spell_warl_demonic_gateway_SpellScript::HandleLaunch, EFFECT_1, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_warl_demonic_gateway_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_demonic_gateway_SpellScript();
    }
};

class npc_warl_demonic_gateway : public CreatureScript
{
public:
    npc_warl_demonic_gateway() : CreatureScript("npc_warl_demonic_gateway") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_warl_demonic_gatewayAI(creature);
    }

    struct npc_warl_demonic_gatewayAI : public CreatureAI
    {
        npc_warl_demonic_gatewayAI(Creature* creature) : CreatureAI(creature) { }

        EventMap events;
        bool firstTick = true;

        void UpdateAI(uint32 /*diff*/) override
        {
            if (firstTick)
            {
                me->CastSpell(me, SPELL_WARLOCK_DEMONIC_GATEWAY_VISUAL, true);

                me->SetInteractSpellId(SPELL_WARLOCK_DEMONIC_GATEWAY_ACTIVATE);
                me->AddUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
                me->AddNpcFlag(UNIT_NPC_FLAG_SPELLCLICK);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetControlled(true, UNIT_STATE_ROOT);

                firstTick = false;
            }
        }

        void OnSpellClick(Unit* player, bool& /*result*/) override
        {
            // don't allow using the gateway while having specific auras
            uint32 aurasToCheck[4] = { 121164, 121175, 121176, 121177 }; // Orbs of Power @ Temple of Kotmogu
            for (auto auraToCheck : aurasToCheck)
                if (player->HasAura(auraToCheck))
                    return;

            TeleportTarget(player, true);
            return;
        }

        void TeleportTarget(Unit * target, bool allowAnywhere)
        {
            Unit * owner = me->GetOwner();
            if (!owner)
                return;

            // only if target stepped through the portal
            if (!allowAnywhere && me->GetDistance2d(target) > 1.0f)
                return;
            // check if target wasn't recently teleported
            if (target->HasAura(SPELL_WARLOCK_DEMONIC_GATEWAY_DEBUFF))
                return;
            // only if in same party
            if (!target->IsInRaidWith(owner))
                return;
            // not allowed while CC'ed
            if (!target->CanFreeMove())
                return;

            uint32 otherGateway = me->GetEntry() == NPC_WARLOCK_DEMONIC_GATEWAY_GREEN ? NPC_WARLOCK_DEMONIC_GATEWAY_PURPLE : NPC_WARLOCK_DEMONIC_GATEWAY_GREEN;
            uint32 teleportSpell = me->GetEntry() == NPC_WARLOCK_DEMONIC_GATEWAY_GREEN ? SPELL_WARLOCK_DEMONIC_GATEWAY_JUMP_GREEN : SPELL_WARLOCK_DEMONIC_GATEWAY_JUMP_PURPLE;
            std::list<Creature*> gateways;
            me->GetCreatureListWithEntryInGrid(gateways, otherGateway, 100.0f);
            for (auto gateway : gateways)
            {
                if (gateway->GetOwnerGUID() != me->GetOwnerGUID())
                    continue;

                target->CastSpell(gateway, teleportSpell, true);
                if (target->HasAura(SPELL_WARLOCK_PLANESWALKER))
                    target->CastSpell(target, SPELL_WARLOCK_PLANESWALKER_BUFF, true);
                // Item - Warlock PvP Set 4P Bonus: "Your allies can use your Demonic Gateway again 15 sec sooner"
                if (int32 amount = owner->GetAuraEffectAmount(SPELL_WARLOCK_PVP_4P_BONUS, EFFECT_0))
                    if (Aura * aura = target->GetAura(SPELL_WARLOCK_DEMONIC_GATEWAY_DEBUFF))
                        aura->SetDuration(aura->GetDuration() - amount * IN_MILLISECONDS);
                break;
            }
        }
    };
};

enum eCommandDemonSpells
{
    SPELL_IMP_CAUTERIZE_MASTER = 119899,
    SPELL_VOIDWALKER_SUFFERING = 17735,
    SPELL_FELHUNTER_SPELL_LOCK = 19647,
    SPELL_SUCCUBUS_WHIPLASH = 6360,
    SPELL_FELGUARD_FELSTORM = 89751,
    SPELL_INFERNAL_METEOR_STRIKE = 171017,
    SPELL_DOOMGUARD_SHADOW_LOCK = 171138
};

// Cauterize Master - 119905
class spell_warl_cauterize_master : public SpellScriptLoader
{
public:
    spell_warl_cauterize_master() : SpellScriptLoader("spell_warl_cauterize_master") {}

    class spell_warl_cauterize_master_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_cauterize_master_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_IMP_CAUTERIZE_MASTER))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return;

            /*if (pet->GetEntry() != PET_ENTRY_IMP)
                return;*/

            pet->CastSpell(caster, SPELL_IMP_CAUTERIZE_MASTER, true);
            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 30 * IN_MILLISECONDS);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_cauterize_master_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_warl_cauterize_master_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_cauterize_master_SpellScript();
    }
};

// Suffering - 119907
class spell_warl_suffering : public SpellScriptLoader
{
public:
    spell_warl_suffering() : SpellScriptLoader("spell_warl_suffering") {}

    class spell_warl_suffering_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_suffering_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_VOIDWALKER_SUFFERING))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet || !target)
                return;

            /*if (pet->GetEntry() != PET_ENTRY_VOIDWALKER)
                return;*/

            pet->CastSpell(target, SPELL_VOIDWALKER_SUFFERING, true);
            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 10 * IN_MILLISECONDS);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_suffering_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_warl_suffering_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_suffering_SpellScript();
    }
};

// Spell Lock - 119910
class spell_warl_spell_lock : public SpellScriptLoader
{
public:
    spell_warl_spell_lock() : SpellScriptLoader("spell_warl_spell_lock") {}

    class spell_warl_spell_lock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_spell_lock_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_FELHUNTER_SPELL_LOCK))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet || !target)
                return;

            /*if (pet->GetEntry() != PET_ENTRY_FELHUNTER)
                return;*/

            pet->CastSpell(target, SPELL_FELHUNTER_SPELL_LOCK, true);
            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 24 * IN_MILLISECONDS);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_spell_lock_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_warl_spell_lock_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_spell_lock_SpellScript();
    }
};

// Whiplash - 119909
class spell_warl_whiplash : public SpellScriptLoader
{
public:
    spell_warl_whiplash() : SpellScriptLoader("spell_warl_whiplash") {}

    class spell_warl_whiplash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_whiplash_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            WorldLocation const* dest = GetExplTargetDest();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet || !dest)
                return;

            /*if (pet->GetEntry() != PET_ENTRY_SUCCUBUS)
                return;*/

            pet->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), SPELL_SUCCUBUS_WHIPLASH, true);
            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 25 * IN_MILLISECONDS);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_SUCCUBUS_WHIPLASH))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_whiplash_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_warl_whiplash_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_whiplash_SpellScript();
    }
};

// Felstorm - 119914
class spell_warl_felstorm : public SpellScriptLoader
{
public:
    spell_warl_felstorm() : SpellScriptLoader("spell_warl_felstorm") {}

    class spell_warl_felstorm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_felstorm_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 45 * IN_MILLISECONDS);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_FELGUARD_FELSTORM))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_felstorm_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_warl_felstorm_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_felstorm_SpellScript();
    }
};

// Meteor Strike - 171152
class spell_warl_meteor_strike : public SpellScriptLoader
{
public:
    spell_warl_meteor_strike() : SpellScriptLoader("spell_warl_meteor_strike") {}

    class spell_warl_meteor_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_meteor_strike_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return;

            /*if (pet->GetEntry() != PET_ENTRY_INFERNAL)
                return;*/

            pet->CastSpell(pet, SPELL_INFERNAL_METEOR_STRIKE, true);

            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 60 * IN_MILLISECONDS);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_INFERNAL_METEOR_STRIKE))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_meteor_strike_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_warl_meteor_strike_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_meteor_strike_SpellScript();
    }
};

// Shadow Lock - 171140
class spell_warl_shadow_lock : public SpellScriptLoader
{
public:
    spell_warl_shadow_lock() : SpellScriptLoader("spell_warl_shadow_lock") {}

    class spell_warl_shadow_lock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_shadow_lock_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet || !target)
                return;

            /*if (pet->GetEntry() != PET_ENTRY_DOOMGUARD)
                return;*/

            pet->CastSpell(target, SPELL_DOOMGUARD_SHADOW_LOCK, true);

            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(GetSpellInfo()->Id, 24 * IN_MILLISECONDS);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Guardian* pet = caster->GetGuardianPet();
            if (!caster || !pet)
                return SPELL_FAILED_DONT_REPORT;

            if (pet->GetSpellHistory()->HasCooldown(SPELL_DOOMGUARD_SHADOW_LOCK))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_shadow_lock_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_warl_shadow_lock_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_shadow_lock_SpellScript();
    }
};

// Soul Leach appliers - 137046, 137044, 137043
class spell_warl_soul_leach_applier : public SpellScriptLoader
{
public:
    spell_warl_soul_leach_applier() : SpellScriptLoader("spell_warl_soul_leach_applier") {}

    class spell_warl_soul_leach_applier_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_leach_applier_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_WARLOCK_SOUL_LEECH, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_warl_soul_leach_applier_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soul_leach_applier_SpellScript();
    }
};


//219272 - Demon Skin
class spell_warl_demon_skin : public SpellScriptLoader
{
public:
    spell_warl_demon_skin() : SpellScriptLoader("spell_warl_demon_skin") { }

    class spell_warl_demon_skin_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demon_skin_AuraScript);

    void PeriodicTick(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        float absorb = (aurEff->GetAmount() / 10.f) * caster->GetMaxHealth() / 100.f;

        // Add remaining amount if already applied
        AuraEffect* soulLeechShield = caster->GetAuraEffect(SPELL_WARLOCK_SOUL_LEECH_SHIELD, EFFECT_0);
        if (soulLeechShield)
            absorb += soulLeechShield->GetAmount();

        AddPct(absorb, caster->GetAuraEffectAmount(SPELL_ARENA_DAMPENING, EFFECT_0));

        float threshold = caster->CountPctFromMaxHealth(GetEffect(EFFECT_1)->GetAmount());
        absorb = std::min(absorb, threshold);

        if (soulLeechShield)
            soulLeechShield->SetAmount(absorb);
        else
            caster->CastCustomSpell(SPELL_WARLOCK_SOUL_LEECH_SHIELD, SPELLVALUE_BASE_POINT0, absorb, caster, TRIGGERED_FULL_MASK);
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (Aura* aur = caster->GetAura(SPELL_WARLOCK_SOUL_LEECH_SHIELD))
        {
            aur->SetMaxDuration(15000);
            aur->RefreshDuration();
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_demon_skin_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demon_skin_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

AuraScript* GetAuraScript() const override
{
    return new spell_warl_demon_skin_AuraScript();
}
};

// Cataclysm - 152108
class spell_warl_cataclysm : public SpellScriptLoader
{
public:
    spell_warl_cataclysm() : SpellScriptLoader("spell_warl_cataclysm") {}

    class spell_warl_cataclysm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_cataclysm_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;
            if (!caster->ToPlayer())
                return;

            if (GetCaster()->ToPlayer()->GetPrimarySpecialization() == TALENT_SPEC_WARLOCK_DESTRUCTION)
                caster->CastSpell(target, SPELL_WARLOCK_IMMOLATE_DOT, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_cataclysm_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_cataclysm_SpellScript();
    }
};

// Channel Demonfire - 196447
class spell_warl_channel_demonfire : public SpellScriptLoader
{
public:
    spell_warl_channel_demonfire() : SpellScriptLoader("spell_warl_channel_demonfire") {}

    class spell_warl_channel_demonfire_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_channel_demonfire_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            std::list<Unit*> enemies;
            Trinity::AnyUnfriendlyUnitInObjectRangeCheck check(caster, caster, 100.f);
            Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(caster, enemies, check);
            Cell::VisitAllObjects(caster, searcher, 100.f);
            enemies.remove_if(Trinity::UnitAuraCheck(false, SPELL_WARLOCK_IMMOLATE_DOT, caster->GetGUID()));
            if (enemies.empty())
                return;

            Unit* target = Trinity::Containers::SelectRandomContainerElement(enemies);
            caster->CastSpell(target, SPELL_WARLOCK_CHANNEL_DEMONFIRE_DAMAGE, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_channel_demonfire_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_channel_demonfire_AuraScript();
    }
};

// Soul Conduit - 215941
class spell_warl_soul_conduit : public SpellScriptLoader
{
public:
    spell_warl_soul_conduit() : SpellScriptLoader("spell_warl_soul_conduit") {}

    class spell_warl_soul_conduit_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_conduit_AuraScript);

        int32 refund = 0;

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return false;
            if (eventInfo.GetActor() && eventInfo.GetActor() != caster)
                return false;

            if (Spell const* spell = eventInfo.GetProcSpell())
            {
                std::vector<SpellPowerCost> const& costs = spell->GetPowerCost();
                auto costData = std::find_if(costs.begin(), costs.end(), [](SpellPowerCost const& cost) { return cost.Power == POWER_MANA && cost.Amount > 0; });
                if (costData == costs.end())
                    return false;

                refund = costData->Amount;
                return true;
            }

            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (roll_chance_i(GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints))
                caster->CastCustomSpell(SPELL_WARLOCK_SOUL_CONDUIT_REFUND, SPELLVALUE_BASE_POINT0, refund, caster, TRIGGERED_FULL_MASK);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_soul_conduit_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warl_soul_conduit_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_soul_conduit_AuraScript();
    }
};

// Soul Harvest - 196098
class spell_warl_soul_harvest : public SpellScriptLoader
{
public:
    spell_warl_soul_harvest() : SpellScriptLoader("spell_warl_soul_harvest") {}

    class spell_warl_soul_harvest_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_harvest_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;
            if (!caster->ToPlayer())
                return;

            std::list<Unit*> enemies;
            caster->GetAttackableUnitListInRange(enemies, 100.0f);
            int32 spellId = 0;
            switch (caster->ToPlayer()->GetSpecializationId())
            {
            case TALENT_SPEC_WARLOCK_AFFLICTION:
                spellId = SPELL_WARLOCK_AGONY;
                break;
            case TALENT_SPEC_WARLOCK_DEMONOLOGY:
                spellId = SPELL_WARLOCK_DOOM;
                break;
            case TALENT_SPEC_WARLOCK_DESTRUCTION:
                spellId = SPELL_WARLOCK_IMMOLATE_DOT;
                break;
            }
            enemies.remove_if(Trinity::UnitAuraCheck(false, spellId, caster->GetGUID()));
            int32 mod = std::min((int)enemies.size(), 15);
            int32 duration = GetAura()->GetMaxDuration() + 2 * mod * IN_MILLISECONDS;

            GetAura()->SetMaxDuration(duration);
            GetAura()->SetDuration(duration);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warl_soul_harvest_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_soul_harvest_AuraScript();
    }
};

// Grimoire of Service summons - 111859, 111895, 111896, 111897, 111898
class spell_warl_grimoire_of_service : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_service() : SpellScriptLoader("spell_warl_grimoire_of_service") {}

    class spell_warl_grimoire_of_service_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_grimoire_of_service_SpellScript);

        enum eServiceSpells
        {
            SPELL_IMP_SINGE_MAGIC = 89808,
            SPELL_VOIDWALKER_SUFFERING = 17735,
            SPELL_SUCCUBUS_SEDUCTION = 6358,
            SPELL_FELHUNTER_SPELL_LOCK = 19647,
            SPELL_FELGUARD_AXE_TOSS = 89766
        };

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(eServiceSpells::SPELL_FELGUARD_AXE_TOSS) ||
                !sSpellMgr->GetSpellInfo(eServiceSpells::SPELL_FELHUNTER_SPELL_LOCK) ||
                !sSpellMgr->GetSpellInfo(eServiceSpells::SPELL_IMP_SINGE_MAGIC) ||
                !sSpellMgr->GetSpellInfo(eServiceSpells::SPELL_SUCCUBUS_SEDUCTION) ||
                !sSpellMgr->GetSpellInfo(eServiceSpells::SPELL_VOIDWALKER_SUFFERING))
                return false;
            return true;
        }

        void HandleSummon(Creature* creature)
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!caster || !creature || !target)
                return;

            switch (GetSpellInfo()->Id)
            {
            case SPELL_WARLOCK_GRIMOIRE_IMP: // Imp
                creature->CastSpell(caster, eServiceSpells::SPELL_IMP_SINGE_MAGIC, true);
                break;
            case SPELL_WARLOCK_GRIMOIRE_VOIDWALKER: // Voidwalker
                creature->CastSpell(target, eServiceSpells::SPELL_VOIDWALKER_SUFFERING, true);
                break;
            case SPELL_WARLOCK_GRIMOIRE_SUCCUBUS: // Succubus
                creature->CastSpell(target, eServiceSpells::SPELL_SUCCUBUS_SEDUCTION, true);
                break;
            case SPELL_WARLOCK_GRIMOIRE_FELHUNTER: // Felhunter
                creature->CastSpell(target, eServiceSpells::SPELL_FELHUNTER_SPELL_LOCK, true);
                break;
            case SPELL_WARLOCK_GRIMOIRE_FELGUARD: // Felguard
                creature->CastSpell(target, eServiceSpells::SPELL_FELGUARD_AXE_TOSS, true);
                break;
            }
        }

        void Register() override
        {
            OnEffectSummon += SpellOnEffectSummonFn(spell_warl_grimoire_of_service_SpellScript::HandleSummon);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_grimoire_of_service_SpellScript();
    }
};

class ImplosionDamageEvent : public BasicEvent
{
public:
    ImplosionDamageEvent(Unit* caster, Unit* target) : _caster(caster), _target(target) { }

    bool Execute(uint64 /*execTime*/, uint32 /*diff*/) override
    {
        if (_caster && _target)
        {
            _caster->CastSpell(_target, SPELL_WARLOCK_IMPLOSION_DAMAGE, true);
            _target->ToCreature()->DisappearAndDie();
        }
        return true;
    }
private:
    Unit* _caster;
    Unit* _target;
};

// 196277 - Implosion
class spell_warl_implosion : public SpellScriptLoader
{
public:
    spell_warl_implosion() : SpellScriptLoader("spell_warl_implosion") { }

    class spell_warl_implosion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_implosion_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            std::list<Creature*> imps;
            caster->GetCreatureListWithEntryInGrid(imps, 55659); // Wild Imps
            for (Creature* imp : imps)
            {
                if (imp->ToTempSummon()->GetSummoner() == caster)
                {
                    imp->Variables.Set("ForceUpdateTimers", true);
                    imp->CastSpell(target, SPELL_WARLOCK_IMPLOSION_JUMP, true);
                    imp->GetMotionMaster()->MoveJump(*target, 300.f, 1.f, EVENT_JUMP);
                    ObjectGuid casterGuid = caster->GetGUID();
                    caster->GetScheduler().Schedule(500ms, [imp, casterGuid](TaskContext /*context*/)
                    {
                        imp->CastSpell(imp, SPELL_WARLOCK_IMPLOSION_DAMAGE, true, nullptr, nullptr, casterGuid);
                        imp->DisappearAndDie();
                    });
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_implosion_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_implosion_SpellScript();
    }
};
// Grimoire of Synergy - 171975
class spell_warl_grimoire_of_synergy : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_synergy() : SpellScriptLoader("spell_warl_grimoire_of_synergy") {}

    class spell_warl_grimoire_of_synergy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_grimoire_of_synergy_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;
            if (Player* player = caster->ToPlayer())
            {
                player->AddAura(GetSpellInfo()->Id, player);
                if (Guardian* pet = player->GetGuardianPet())
                    player->AddAura(GetSpellInfo()->Id, pet);
            }

        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_warl_grimoire_of_synergy_SpellScript::HandleCast);
        }
    };

    class spell_warl_grimoire_of_synergy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_grimoire_of_synergy_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* actor = eventInfo.GetActor();
            if (!actor)
                return false;
            if (actor->IsPet() || actor->IsGuardian())
            {
                Unit* owner = actor->GetOwner();
                if (!owner)
                    return false;
                if (roll_chance_i(10))
                    owner->CastSpell(owner, SPELL_WARLOCK_GRIMOIRE_OF_SYNERGY_BUFF, true);
                return true;
            }
            if (Player* player = actor->ToPlayer())
            {
                Guardian* guardian = player->GetGuardianPet();
                if (!guardian)
                    return false;
                if (roll_chance_i(10))
                    player->CastSpell(guardian, SPELL_WARLOCK_GRIMOIRE_OF_SYNERGY_BUFF, true);
                return true;
            }
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_grimoire_of_synergy_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_grimoire_of_synergy_AuraScript();
    }

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_grimoire_of_synergy_SpellScript();
    }
};

// Eye Laser - 205231
class spell_warl_eye_laser : public SpellScriptLoader
{
public:
    spell_warl_eye_laser() : SpellScriptLoader("spell_warl_eye_laser") {}

    class spell_warl_eye_laser_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_eye_laser_SpellScript);

        void HandleTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetOriginalCaster();
            if (!caster)
                return;
            targets.clear();
            Trinity::AllWorldObjectsInRange check(caster, 100.f);
            Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> search(caster, targets, check);
            Cell::VisitAllObjects(caster, search, 100.f);
            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_WARLOCK_DOOM, caster->GetGUID()));
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_eye_laser_SpellScript::HandleTargets, EFFECT_0, TARGET_UNIT_TARGET_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_eye_laser_SpellScript();
    }
};

// Darkglare - 103673
class npc_pet_warlock_darkglare : public CreatureScript
{
public:
    npc_pet_warlock_darkglare() : CreatureScript("npc_pet_warlock_darkglare") {}

    struct npc_pet_warlock_darkglare_PetAI : public PetAI
    {
        npc_pet_warlock_darkglare_PetAI(Creature* creature) : PetAI(creature) {}

        void UpdateAI(uint32 /*diff*/) override
        {
            Unit* owner = me->GetOwner();
            if (!owner)
                return;

            std::list<Unit*> targets;
            owner->GetAttackableUnitListInRange(targets, 100.0f);
            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_WARLOCK_DOOM, owner->GetGUID()));
            if (!targets.empty())
                me->CastSpell(targets.front(), SPELL_WARLOCK_EYE_LASER, false, nullptr, nullptr, owner->GetGUID());
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_pet_warlock_darkglare_PetAI(creature);
    }
};

// Wild Imp - 99739
struct npc_pet_warlock_wild_imp : public PetAI
{
    npc_pet_warlock_wild_imp(Creature* creature) : PetAI(creature) 
    {
        if (Unit* owner = me->GetOwner())
        {
            me->SetLevel(owner->getLevel());
            me->SetMaxHealth(owner->GetMaxHealth() / 3);
            me->SetHealth(owner->GetHealth() / 3);
        }
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        Unit* owner = me->GetOwner();
        if (!owner)
            return;

        Unit* target = GetTarget();
        ObjectGuid newtargetGUID = owner->GetTarget();
        if (newtargetGUID.IsEmpty() || newtargetGUID == _targetGUID)
        {
            CastSpellOnTarget(owner, target);
            return;
        }

        if (Unit* newTarget = ObjectAccessor::GetUnit(*me, newtargetGUID))
            if (target != newTarget && me->IsValidAttackTarget(newTarget))
                target = newTarget;

        CastSpellOnTarget(owner, target);
    }

private:
    Unit* GetTarget() const
    {
        return ObjectAccessor::GetUnit(*me, _targetGUID);
    }

    void CastSpellOnTarget(Unit* owner, Unit* target)
    {
        if (target && me->IsValidAttackTarget(target))
        {
            _targetGUID = target->GetGUID();
            me->CastSpell(target, SPELL_WARLOCK_FEL_FIREBOLT, false, nullptr, nullptr, owner->GetGUID());
        }
    }

    ObjectGuid _targetGUID;
};

// Demonic Calling - 205145
class spell_warl_demonic_calling : public SpellScriptLoader
{
public:
    spell_warl_demonic_calling() : SpellScriptLoader("spell_warl_demonic_calling") {}

    class spell_warl_demonic_calling_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_calling_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_CALLING_TRIGGER))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return false;
            if (eventInfo.GetSpellInfo() && (eventInfo.GetSpellInfo()->Id == SPELL_WARLOCK_DEMONBOLT || eventInfo.GetSpellInfo()->Id == SPELL_WARLOCK_SHADOW_BOLT) && roll_chance_i(20))
                caster->CastSpell(caster, SPELL_WARLOCK_DEMONIC_CALLING_TRIGGER, true);
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_demonic_calling_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_demonic_calling_AuraScript();
    }
};

// Chaotic Energies - 77220
class spell_warl_chaotic_energies : public SpellScriptLoader
{
public:
    spell_warl_chaotic_energies() : SpellScriptLoader("spell_warl_chaotic_energies") {}

    class spell_warl_chaotic_energies_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_chaotic_energies_AuraScript);

        void CalcAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void HandleAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
        {

        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_chaotic_energies_AuraScript::CalcAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_warl_chaotic_energies_AuraScript::HandleAbsorb, EFFECT_2);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_chaotic_energies_AuraScript();
    }
};

// Eradication - 196414
class spell_warl_eradication : public SpellScriptLoader
{
public:
    spell_warl_eradication() : SpellScriptLoader("spell_warl_eradication") {}

    class spell_warl_eradication_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_eradication_AuraScript);

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_eradication_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_eradication_AuraScript();
    }
};

// 205179
class aura_warl_phantomatic_singularity : public AuraScript
{
    PrepareAuraScript(aura_warl_phantomatic_singularity);

    void OnTick(const AuraEffect* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget()->GetPosition(), SPELL_WARLOCK_PHANTOMATIC_SINGULARITY_DAMAGE, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_warl_phantomatic_singularity::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_LEECH);
    }
};

// Grimoire of Service - 108501
class spell_warl_grimoire_of_service_aura : public AuraScript
{
    PrepareAuraScript(spell_warl_grimoire_of_service_aura);

    void Handlearn(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            player->LearnSpell(SPELL_WARLOCK_GRIMOIRE_IMP, false);
            player->LearnSpell(SPELL_WARLOCK_GRIMOIRE_VOIDWALKER, false);
            player->LearnSpell(SPELL_WARLOCK_GRIMOIRE_SUCCUBUS, false);
            player->LearnSpell(SPELL_WARLOCK_GRIMOIRE_FELHUNTER, false);
            if (player->GetSpecializationId() == TALENT_SPEC_WARLOCK_DEMONOLOGY)
                player->LearnSpell(SPELL_WARLOCK_GRIMOIRE_FELGUARD, false);
        }
    }
    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            player->RemoveSpell(SPELL_WARLOCK_GRIMOIRE_IMP, false, false);
            player->RemoveSpell(SPELL_WARLOCK_GRIMOIRE_VOIDWALKER, false, false);
            player->RemoveSpell(SPELL_WARLOCK_GRIMOIRE_SUCCUBUS, false, false);
            player->RemoveSpell(SPELL_WARLOCK_GRIMOIRE_FELHUNTER, false, false);
            player->RemoveSpell(SPELL_WARLOCK_GRIMOIRE_FELGUARD, false, false);
        }
    }
    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_grimoire_of_service_aura::Handlearn, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectApplyFn(spell_warl_grimoire_of_service_aura::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Incinerate - 29722
class spell_warl_incinerate : public SpellScript
{
    PrepareSpellScript(spell_warl_incinerate);

    void HandleOnHitMainTarget(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->ModifyPower(POWER_SOUL_SHARDS, 5.0f);
    }

    void HandleOnHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            if (!GetCaster()->HasAura(SPELL_WARLOCK_FIRE_AND_BRIMSTONE))
                if (target != GetExplTargetUnit())
                    PreventHitDamage();
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_incinerate::HandleOnHitMainTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnEffectHitTarget += SpellEffectFn(spell_warl_incinerate::HandleOnHitTarget, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Dreadstalker - 98035
class npc_warlock_dreadstalker : public CreatureScript
{
public:
    npc_warlock_dreadstalker() : CreatureScript("npc_warlock_dreadstalker") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_warlock_dreadstalkerAI(creature);
    }

    struct npc_warlock_dreadstalkerAI : public ScriptedAI
    {
        npc_warlock_dreadstalkerAI(Creature* creature) : ScriptedAI(creature) {}

        bool firstTick = true;

        void UpdateAI(uint32 /*diff*/) override
        {
            if (firstTick)
            {
                Unit* owner = me->GetOwner();
                if (!me->GetOwner() || !me->GetOwner()->ToPlayer())
                    return;

                me->SetMaxHealth(owner->CountPctFromMaxHealth(40));
                me->SetHealth(me->GetMaxHealth());

                if (Unit* target = owner->ToPlayer()->GetSelectedUnit())
                    me->CastSpell(target, SPELL_WARLOCK_DREADSTALKER_CHARGE, true);

                firstTick = false;

                me->CastCustomSpell(SPELL_WARLOCK_SHARPENED_DREADFANGS_BUFF, SPELLVALUE_BASE_POINT0, owner->GetAuraEffectAmount(SPELL_WARLOCK_SHARPENED_DREADFANGS, EFFECT_0), me, true);
            }

            UpdateVictim();
            DoMeleeAttackIfReady();
        }
    };
};

// 6262 - Healthstone
class spell_warl_healthstone_heal : public SpellScript
{
    PrepareSpellScript(spell_warl_healthstone_heal);

    void HandleOnHit()
    {
        // Glyph of Healthstone
        if (GetCaster()->HasAura(SPELL_WARLOCK_GLYPH_OF_HEALTHSTONE))
        {
            PreventHitHeal();
            return;
        }

        Unit* caster = GetCaster();
        if (!caster || !caster->ToPlayer())
            return;

        std::list<Unit*> allies;
        GetCaster()->GetFriendlyUnitListInRange(allies, 100.f);

        for (Unit* unit : allies)
            if (uint32 pct = unit->GetAuraEffectAmount(SPELL_WARLOCK_SWEET_SOULS, EFFECT_0))
                unit->CastCustomSpell(SPELL_WARLOCK_SWEET_SOULS_HEAL, SPELLVALUE_BASE_POINT0, pct, unit, true);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warl_healthstone_heal::HandleOnHit);
    }
};

// 264106 - Deathbolt
class spell_warl_deathbolt : public SpellScriptLoader
{
public:
    spell_warl_deathbolt() : SpellScriptLoader("spell_warl_deathbolt") { }

    class spell_warl_deathbolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_deathbolt_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            SetHitDamage(CalculateDamage());
        }

        int32 CalculateDamage()
        {
            int32 damage = 0;
            Unit::AuraApplicationMap const& auras = GetHitUnit()->GetAppliedAuras();
            for (auto i = auras.begin(); i != auras.end(); ++i)
            {
                SpellInfo const* spell = i->second->GetBase()->GetSpellInfo();

                if (spell->SpellFamilyName == uint32(SPELLFAMILY_WARLOCK) && spell->SpellFamilyFlags & flag128(502, 8110, 300000, 0)) // out of Mastery : Potent Afflictions
                {
                    AuraEffectVector effects = i->second->GetBase()->GetAuraEffects();
                    for (AuraEffectVector::const_iterator iter = effects.begin(); iter != effects.end(); ++iter)
                    {
                        if ((*iter)->GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
                        {
                            int32 valToUse = 0;

                            if (spell->Id == SPELL_CORRUPTION_DOT)
                                valToUse = (*iter)->GetRemainingDamage(GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints * 1000);
                            else
                                valToUse = (*iter)->GetRemainingDamage();

                            damage += valToUse * GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints / 100;
                        }
                    }
                }
            }

            return damage;
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_deathbolt_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_deathbolt_SpellScript();
    }
};

// 107024 - Fel Lord
class npc_warl_fel_lord : public CreatureScript
{
public:
    npc_warl_fel_lord() : CreatureScript("npc_warl_fel_lord") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_warl_fel_lordAI(creature);
    }

    struct npc_warl_fel_lordAI : public CreatureAI
    {
        npc_warl_fel_lordAI(Creature* creature) : CreatureAI(creature) { }

        void Reset() override
        {
            Unit* owner = me->GetOwner();
            if (!owner)
                return;

            me->SetMaxHealth(owner->GetMaxHealth());
            me->SetHealth(me->GetMaxHealth());
            me->SetControlled(true, UNIT_STATE_ROOT);
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            me->CastSpell(me, SPELL_WARLOCK_FEL_LORD_CLEAVE, false);
        }
    };
};

// 199471 - Soul Flame
class spell_warlock_artifact_soul_flame : public SpellScriptLoader
{
public:
    spell_warlock_artifact_soul_flame() : SpellScriptLoader("spell_warlock_artifact_soul_flame") { }

    class spell_warlock_artifact_soul_flame_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_soul_flame_AuraScript);

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            Unit* caster = GetCaster();
            if (!caster || !target)
                return;

            Position p = target->GetPosition();
            caster->GetScheduler().Schedule(300ms, [caster, p](TaskContext /*context*/)
            {
                caster->CastSpell(p, SPELL_WARLOCK_SOUL_FLAME_PROC, true);
            });
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_soul_flame_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_soul_flame_AuraScript();
    }
};

// 104773 - Unending Resolve
class spell_warlock_unending_resolve : public SpellScriptLoader
{
public:
    spell_warlock_unending_resolve() : SpellScriptLoader("spell_warlock_unending_resolve") { }

    class spell_warlock_unending_resolve_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_unending_resolve_AuraScript);

        void PreventEffectIfCastingCircle(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster || caster->ToPlayer())
                return;
            Player* pCaster = caster->ToPlayer();
            if (!pCaster)
                return;

            if (pCaster->HasSpell(SPELL_WARLOCK_CASTING_CIRCLE))
                PreventDefaultAction();
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_warlock_unending_resolve_AuraScript::PreventEffectIfCastingCircle, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warlock_unending_resolve_AuraScript::PreventEffectIfCastingCircle, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_warlock_unending_resolve_AuraScript::PreventEffectIfCastingCircle, EFFECT_3, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warlock_unending_resolve_AuraScript::PreventEffectIfCastingCircle, EFFECT_3, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_unending_resolve_AuraScript();
    }
};

// 3026 - Use Soulstone
class spell_warlock_use_soulstone : public SpellScriptLoader
{
public:
    spell_warlock_use_soulstone() : SpellScriptLoader("spell_warlock_use_soulstone") { }

    class spell_warlock_use_soulstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_use_soulstone_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Player * player = GetCaster()->ToPlayer();
            if (!player)
                return;
            Unit * originalCaster = GetOriginalCaster();

            // already have one active request
            if (player->IsResurrectRequested())
                return;

            int32 healthPct = GetSpellInfo()->GetEffect(EFFECT_1)->CalcValue(originalCaster);
            int32 manaPct = GetSpellInfo()->GetEffect(EFFECT_0)->CalcValue(originalCaster);

            uint32 health = player->CountPctFromMaxHealth(healthPct);
            uint32 mana = 0;
            if (player->GetMaxPower(POWER_MANA) > 0)
                mana = CalculatePct(player->GetMaxPower(POWER_MANA), manaPct);

            player->ResurrectPlayer(0.0f);
            player->SetHealth(health);
            player->SetPower(POWER_MANA, mana);
            player->SetPower(POWER_RAGE, 0);
            player->SetPower(POWER_ENERGY, player->GetMaxPower(POWER_ENERGY));
            player->SetPower(POWER_FOCUS, 0);
            player->SpawnCorpseBones();
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_warlock_use_soulstone_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SELF_RESURRECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_use_soulstone_SpellScript();
    }
};

// 5697 - Unending Breath
class spell_warlock_unending_breath : public SpellScriptLoader
{
public:
    spell_warlock_unending_breath() : SpellScriptLoader("spell_warlock_unending_breath") { }

    class spell_warlock_unending_breath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_unending_breath_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit * caster = GetCaster();
            if (Unit * target = GetHitUnit())
                if (caster->HasAura(SPELL_WARLOCK_SOULBURN))
                    caster->CastSpell(target, SPELL_WARLOCK_SOULBURN_UNENDING_BREATH, true);
        }

        void Register() override
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_warlock_unending_breath_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_unending_breath_SpellScript();
    }
};

// 145072 - Item - Warlock T16 2P Bonus
class spell_warlock_t16_demo_2p : public SpellScriptLoader
{
public:
    spell_warlock_t16_demo_2p() : SpellScriptLoader("spell_warlock_t16_demo_2p") { }

    class spell_warlock_t16_demo_2p_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_t16_demo_2p_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            uint32 procSpellId = 0;
            if (auto spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo())
                procSpellId = spellInfo->Id;
            uint32 chance = 0;
            uint32 triggeredSpellId = 0;
            switch (procSpellId)
            {
            case SPELL_WARLOCK_CONFLAGRATE:
            case SPELL_WARLOCK_CONFLAGRATE_FIRE_AND_BRIMSTONE:
                chance = aurEff->GetSpellInfo()->GetEffect(EFFECT_3)->BasePoints;
                triggeredSpellId = 145075; // Destructive Influence
                break;
            case SPELL_WARLOCK_UNSTABLE_AFFLICTION:
                chance = aurEff->GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints;
                triggeredSpellId = 145082; // Empowered Grasp
                break;
            case SPELL_WARLOCK_SOUL_FIRE:
            case SPELL_WARLOCK_SOUL_FIRE_METAMORPHOSIS:
                chance = aurEff->GetSpellInfo()->GetEffect(EFFECT_3)->BasePoints;
                triggeredSpellId = 145085; // Fiery Wrath
                break;
            default:
                return;
            }
            if (!roll_chance_i(chance))
                return;
            Unit * caster = GetUnitOwner();
            caster->CastSpell(caster, triggeredSpellId, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_t16_demo_2p_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_t16_demo_2p_AuraScript();
    }
};

// 145091 - Item - Warlock T16 4P Bonus
class spell_warlock_t16_4p : public SpellScriptLoader
{
public:
    spell_warlock_t16_4p() : SpellScriptLoader("spell_warlock_t16_4p") { }

    class spell_warlock_t16_4p_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_t16_4p_AuraScript);

        void PeriodicTick(AuraEffect const* aurEffConst)
        {
            // "When a Burning Ember fills up, your critical strike chance is increased by 15% for 5 seconds"
            Player * caster = GetUnitOwner()->ToPlayer();
            if (!caster || caster->HasAura(SPELL_WARLOCK_T16_4P_INTERNAL_CD))
                return;
            // allow only in Destruction
            if (caster->GetSpecializationId() != TALENT_SPEC_WARLOCK_DESTRUCTION)
                return;

            AuraEffect * aurEff = const_cast<AuraEffect*>(aurEffConst);
            uint32 currentPower = caster->GetPower(POWER_BURNING_EMBERS) / 10;
            uint32 oldPower = aurEff->GetAmount();
            if (currentPower > oldPower)
            {
                caster->CastSpell(caster, SPELL_WARLOCK_T16_4P_TRIGGERED, true);
                caster->CastSpell(caster, SPELL_WARLOCK_T16_4P_INTERNAL_CD, true);
            }
            aurEff->SetAmount(currentPower);
        }

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetDamageInfo())
                return;

            Unit* caster = GetUnitOwner();
            Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
            if (!caster || !victim)
                return;
            // "Shadow Bolt and Touch of Chaos have a 8% chance to also cast Hand of Gul'dan at the target"
            caster->CastSpell(victim, SPELL_WARLOCK_HAND_OF_GULDAN, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_t16_4p_AuraScript::PeriodicTick, EFFECT_31, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectProc += AuraEffectProcFn(spell_warlock_t16_4p_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_t16_4p_AuraScript();
    }
};

// Called by Dark Soul - 77801 ( Generic ), 113858 ( Instability ), 113860 ( Misery ), 113861 ( Knowledge )
class spell_warlock_t15_2p_bonus : public SpellScriptLoader
{
public:
    spell_warlock_t15_2p_bonus() : SpellScriptLoader("spell_warlock_t15_2p_bonus") { }

    class spell_warlock_t15_2p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_t15_2p_bonus_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->HasAura(SPELL_WARLOCK_T15_2P_BONUS)) // Check if caster has bonus aura
                    caster->AddAura(SPELL_WARLOCK_T15_2P_BONUS_TRIGGERED, caster);
            }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_warlock_t15_2p_bonus_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_t15_2p_bonus_SpellScript();
    }
};

// 264178 - Demonbolt
class spell_warlock_demonbolt_new : public SpellScriptLoader
{
public:
    spell_warlock_demonbolt_new() : SpellScriptLoader("spell_warlock_demonbolt_new") { }

    class spell_warlock_demonbolt_new_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_demonbolt_new_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster())
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_DEMONBOLT_ENERGIZE, true);
                GetCaster()->CastSpell(GetCaster(), SPELL_DEMONBOLT_ENERGIZE, true);
            }
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_warlock_demonbolt_new_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_demonbolt_new_SpellScript();
    }
};

// 212619 - Call Felhunter
class spell_warlock_call_felhunter : public SpellScriptLoader
{
public:
    spell_warlock_call_felhunter() : SpellScriptLoader("spell_warlock_call_felhunter") { }

    class spell_warlock_call_felhunter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_call_felhunter_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->ToPlayer())
                return SPELL_FAILED_BAD_TARGETS;

            if (caster->ToPlayer()->GetPet() && caster->ToPlayer()->GetPet()->GetEntry() == 417)
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warlock_call_felhunter_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_call_felhunter_SpellScript();
    }
};

// 5740 - Rain of Fire
// MiscId - 5420
struct at_warlock_rain_of_fire : AreaTriggerAI
{
    at_warlock_rain_of_fire(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    void OnUpdate(uint32 diff) override
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        int32 timer = at->Variables.GetValue<int32>("Spells.RainOfFireTimer") + diff;
        if (timer < 1000)
        {
            at->Variables.Set("Spells.RainOfFireTimer", timer);
            return;
        }

        for (ObjectGuid guid : at->GetInsideUnits())
            if (Unit* unit = ObjectAccessor::GetUnit(*caster, guid))
                if (caster->IsValidAttackTarget(unit))
                    caster->CastSpell(unit, SPELL_WARLOCK_RAIN_OF_FIRE_DAMAGE, true);

        at->Variables.Set<int32>("Spells.RainOfFireTimer", int32(timer - 1000));
    }
};

// 211729 - Thal'kiel's Discord
// MiscId - 6913
struct at_warlock_artifact_thalkiels_discord : AreaTriggerAI
{
    at_warlock_artifact_thalkiels_discord(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    void OnUpdate(uint32 diff) override
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        int32 timer = at->Variables.GetValue<int32>("_timer") + diff;
        if (timer >= 1300)
        {
            at->Variables.Set<int32>("_timer", 0);
            caster->CastSpell(*at, SPELL_WARLOCK_THALKIES_DISCORD_DAMAGE, true);
        }
        else
            at->Variables.Set("_timer", timer);
    }
};

// 42223 - Rain of fire damage
class spell_warlock_rain_of_fire_damage : public SpellScriptLoader
{
public:
    spell_warlock_rain_of_fire_damage() : SpellScriptLoader("spell_warlock_rain_of_fire_damage") { }

    class spell_warlock_rain_of_fire_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_rain_of_fire_damage_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_INFERNO_AURA) && roll_chance_i(sSpellMgr->GetSpellInfo(SPELL_INFERNO_AURA)->GetEffect(EFFECT_0)->BasePoints))
                caster->CastSpell(caster, SPELL_RAIN_OF_FIRE_ENERGIZE, true);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_warlock_rain_of_fire_damage_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_rain_of_fire_damage_SpellScript();
    }
};

// 126 - Eye of Kilrogg
class spell_warlock_eye_of_kilrogg : public SpellScriptLoader
{
public:
    spell_warlock_eye_of_kilrogg() : SpellScriptLoader("spell_warlock_eye_of_kilrogg") { }

    class spell_warlock_eye_of_kilrogg_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_eye_of_kilrogg_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->ToPlayer())
                return;

            if (caster->ToPlayer()->GetPet())
            {
                caster->GetScheduler().Schedule(250ms, [caster](TaskContext /*context*/)
                {
                    caster->ToPlayer()->PetSpellInitialize();
                });
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warlock_eye_of_kilrogg_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY_DETECT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_eye_of_kilrogg_AuraScript();
    }
};

// Doomguard - 11859, Terrorguard - 59000
class npc_warlock_doomguard : public CreatureScript
{
public:
    npc_warlock_doomguard() : CreatureScript("npc_warlock_doomguard") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_warlock_doomguardAI(creature);
    }

    struct npc_warlock_doomguardAI : public ScriptedAI
    {
        npc_warlock_doomguardAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        float maxDistance;

        void Reset()
        {
            me->SetClass(CLASS_ROGUE);
            me->SetPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 200);
            me->SetPower(POWER_ENERGY, 200);

            events.Reset();
            events.ScheduleEvent(1, 300);

            me->SetControlled(true, UNIT_STATE_ROOT);
            maxDistance = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_PET_DOOMBOLT)->RangeEntry->RangeMax[0];
        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();
            if (Unit * owner = me->GetOwner())
                if (Unit * victim = owner->GetVictim())
                    me->Attack(victim, false);

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case 1:
                    if (!me->GetVictim())
                    {
                        me->SetControlled(false, UNIT_STATE_ROOT);
                        events.ScheduleEvent(eventId, 100);
                        return;
                    }
                    me->SetControlled(true, UNIT_STATE_ROOT);
                    me->CastSpell(me->GetVictim(), SPELL_WARLOCK_PET_DOOMBOLT, false, nullptr, nullptr, me->GetOwnerGUID());
                    events.ScheduleEvent(eventId, 3100);
                    break;
                }
            }
        }
    };
};

// Reap Souls - 216698
class spell_warlock_artifact_reap_souls : public SpellScriptLoader
{
public:
    spell_warlock_artifact_reap_souls() : SpellScriptLoader("spell_warlock_artifact_reap_souls") {}

    class spell_warlock_artifact_reap_souls_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_artifact_reap_souls_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_WARLOCK_DEADWIND_HARVERST, true);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return SPELL_FAILED_DONT_REPORT;

            if (!caster->HasAura(SPELL_WARLOCK_TORMENTED_SOULS))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warlock_artifact_reap_souls_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_warlock_artifact_reap_souls_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_artifact_reap_souls_SpellScript();
    }
};

// 212356 - Soulshatter
class spell_warlock_soul_shatter : public SpellScriptLoader
{
public:
    spell_warlock_soul_shatter() : SpellScriptLoader("spell_warlock_soul_shatter") { }

    class spell_warlock_soul_shatter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_soul_shatter_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            unitList.remove_if([caster](WorldObject* obj)
            {
                Unit* target = obj->ToUnit();
                if (!target)
                    return true;

                return !(target->HasAura(SPELL_WARLOCK_AGONY, caster->GetGUID()) || target->HasAura(SPELL_WARLOCK_CORRUPTION_TRIGGERED, caster->GetGUID()) ||
                    target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT1, caster->GetGUID()) || target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT2, caster->GetGUID()) ||
                    target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT3, caster->GetGUID()) || target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT4, caster->GetGUID()) ||
                    target->HasAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT5, caster->GetGUID()));
            });

            unitList.sort(Trinity::ObjectDistanceOrderPred(caster));
            if (unitList.size() > 5)
                unitList.resize(5);
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            // Apparently does 10% dmg no matter what.
            int32 maxHealthPctPossible = GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints;
            //int32 healthPctFromDot = (int32) ((float) RemoveDotsAndAggregateDamage(caster, target) / (float) target->CountPctFromMaxHealth(maxHealthPctPossible));

            //if (healthPctFromDot < maxHealthPctPossible)
            //{
            //    SetHitDamage(healthPctFromDot);
            //} else {
            SetHitDamage(maxHealthPctPossible);
            //}
            caster->CastSpell(caster, SPELL_WARLOCK_SOULSHATTER_ENERGIZE, true);
            caster->CastSpell(caster, SPELL_WARLOCK_SOULSHATTER_HASTE, true);
        }

        int32 RemoveDotsAndAggregateDamage(Unit* caster, Unit* target)
        {
            std::vector<uint32> dots{ SPELL_WARLOCK_AGONY, SPELL_WARLOCK_CORRUPTION_TRIGGERED, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT1, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT2,
                                      SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT3, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT4, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT5 };
            int32 dotDamage = 0;
            for (uint32 dot : dots)
            {
                //if (AuraEffect* aurEff = target->GetAuraEffect(dot, SPELL_AURA_PERIODIC_DAMAGE, caster->GetGUID()))
                //    dotDamage += aurEff->GetRemainingDamage(24000);
                target->RemoveAurasDueToSpell(dot, caster->GetGUID());
            }
            return dotDamage;
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warlock_soul_shatter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_warlock_soul_shatter_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_soul_shatter_SpellScript();
    }
};

// 108446 - Soul Link
class spell_warlock_soul_link : public SpellScriptLoader
{
public:
    spell_warlock_soul_link() : SpellScriptLoader("spell_warlock_soul_link") { }

    class spell_warlock_soul_link_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_soul_link_AuraScript);

        void HandleSplit(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& splitAmount)
        {
            Unit* pet = GetUnitOwner();
            if (!pet)
                return;

            Unit* owner = pet->GetOwner();
            if (!owner)
                return;

            if (owner->HasAura(SPELL_WARLOCK_SOUL_SKIN) && owner->HealthBelowPct(35))
                splitAmount *= 2;
        }

        void Register() override
        {
            OnEffectSplit += AuraEffectSplitFn(spell_warlock_soul_link_AuraScript::HandleSplit, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_soul_link_AuraScript();
    }
};

// Thal'kiel's Consumption - 211714
class spell_warlock_artifact_thalkiels_consumption : public SpellScriptLoader
{
public:
    spell_warlock_artifact_thalkiels_consumption() : SpellScriptLoader("spell_warlock_artifact_thalkiels_consumption") {}

    class spell_warlock_artifact_thalkiels_consumption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_artifact_thalkiels_consumption_SpellScript);

        int32 damage = 0;

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster)
                return;

            caster->CastCustomSpell(SPELL_WARLOCK_THALKIELS_CONSUMPTION_DAMAGE, SPELLVALUE_BASE_POINT0, damage, target, TRIGGERED_FULL_MASK);
        }

        void SaveDamage(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                // Remove non-unit (should not happen, better be safe though) and Players (only caster)
                if (!target->ToUnit() || target->ToPlayer())
                    return true;
                // Remove Gateways
                if (target->ToCreature()->GetCreatureType() != CREATURE_TYPE_DEMON)
                    return true;

                return false;
            });

            int32 basePoints = GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints;
            for (WorldObject* pet : targets)
                damage += pet->ToUnit()->CountPctFromMaxHealth(basePoints);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_artifact_thalkiels_consumption_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warlock_artifact_thalkiels_consumption_SpellScript::SaveDamage, EFFECT_1, TARGET_UNIT_CASTER_PET);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_artifact_thalkiels_consumption_SpellScript();
    }
};

// Raging Soul handled via Dark Soul: Instability (113858) \ Knowledge (113861) \ Misery (113860)
class spell_warlock_4p_t14_pve : public SpellScriptLoader
{
public:
    spell_warlock_4p_t14_pve() : SpellScriptLoader("spell_warlock_4p_t14_pve") { }

    class spell_warlock_4p_t14_pve_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_4p_t14_pve_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->HasAura(SPELL_WARLOCK_T14_BONUS))
                    caster->CastSpell(caster, SPELL_WARLOCK_RAGING_SOUL, true);
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_warlock_4p_t14_pve_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_4p_t14_pve_SpellScript();
    }
};

class npc_warlock_infernal : public CreatureScript
{
public:
    npc_warlock_infernal() : CreatureScript("npc_warlock_infernal") { }

    struct npc_warlock_infernalAI : public ScriptedAI
    {
        npc_warlock_infernalAI(Creature *c) : ScriptedAI(c)
        {
        }

        Position spawnPos;

        void Reset() override
        {
            spawnPos = me->GetPosition();

            // if we leave default state (ASSIST) it will passively be controlled by warlock
            me->SetReactState(REACT_PASSIVE);

            // melee damage
            if (Unit * owner = me->GetOwner())
                if (Player * player = owner->ToPlayer())
                {
                    bool isLordSummon = me->GetEntry() == 108452;

                    uint32 spellPower = player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
                    uint32 dmg = CalculatePct(spellPower, isLordSummon ? 30 : 50);
                    uint32 diff = CalculatePct(dmg, 10);
                   
                    me->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, dmg - diff);
                    me->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, dmg + diff);
                   

                    if (isLordSummon)
                        return;

                    if (player->HasAura(SPELL_WARLOCK_LORD_OF_THE_FLAMES) && !player->HasAura(SPELL_WARLOCK_LORD_OF_THE_FLAMES_CD))
                    {
                        std::vector<float> angleOffsets{ float(M_PI) / 2.f, float(M_PI), 3.f * float(M_PI) / 2.f };
                        for (uint32 i = 0; i < 3; ++i)
                            player->CastSpell(me, SPELL_WARLOCK_LORD_OF_THE_FLAMES_SUMMON, true);

                        player->CastSpell(player, SPELL_WARLOCK_LORD_OF_THE_FLAMES_CD, true);
                    }
                }
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            if (!me->HasAura(SPELL_WARLOCK_IMMOLATION))
                DoCast(SPELL_WARLOCK_IMMOLATION);

            // "The Infernal deals strong area of effect damage, and will be drawn to attack targets near the impact point"
            if (!me->GetVictim())
            {
                std::list<Unit*> targets;
                Unit* preferredTarget = nullptr;
                float prefferedDist = 100.0f;
                me->GetAttackableUnitListInRange(targets, 100.0f);
                for (auto target : targets)
                {
                    float dist = target->GetDistance2d(spawnPos.GetPositionX(), spawnPos.GetPositionY());
                    if (dist < prefferedDist)
                    {
                        preferredTarget = target;
                        prefferedDist = dist;
                    }
                }
                if (preferredTarget)
                    me->AI()->AttackStart(preferredTarget);
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_warlock_infernalAI(creature);
    }
};

// 234153 - Drain Life
class spell_warlock_drain_life : public AuraScript
{
    PrepareAuraScript(spell_warlock_drain_life);

    void PeriodicTick(AuraEffect const* /*aurEff*/)
    {
        Unit * caster = GetCaster();
        if (!caster)
            return;
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_drain_life::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_LEECH);
    }
};

// 63106 - Siphon Life @ Glyph of Siphon Life
class spell_warlock_siphon_life : public SpellScriptLoader
{
public:
    spell_warlock_siphon_life() : SpellScriptLoader("spell_warlock_siphon_life") { }

    class spell_warlock_siphon_life_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_siphon_life_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            Unit * caster = GetCaster();
            uint32 heal = caster->SpellHealingBonusDone(caster, GetSpellInfo(), caster->CountPctFromMaxHealth(GetSpellInfo()->GetEffect(effIndex)->BasePoints), HEAL, GetEffectInfo());
            heal /= 100; // 0.5%
            heal = caster->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, HEAL, GetEffectInfo());
            SetHitHeal(heal);
            PreventHitDefaultEffect(effIndex);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_siphon_life_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_siphon_life_SpellScript();
    }
};

// 108416 - Dark Pact
class spell_warlock_dark_pact : public SpellScriptLoader
{
public:
    spell_warlock_dark_pact() : SpellScriptLoader("spell_warlock_dark_pact") { }

    class spell_warlock_dark_pact_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_dark_pact_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool &)
        {
            int32 sacrifiedHealth = GetCaster()->CountPctFromCurHealth(GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints);
            GetCaster()->ModifyHealth(-sacrifiedHealth);
            amount = CalculatePct(sacrifiedHealth, GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warlock_dark_pact_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_dark_pact_AuraScript();
    }
};

// 19483 - Immolation
class spell_warlock_infernal_immolation : public AuraScript
{
    PrepareAuraScript(spell_warlock_infernal_immolation);

    void PeriodicTick(AuraEffect const* /*aurEff*/)
    {
        PreventDefaultAction();
        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->CastSpell(caster, SPELL_WARLOCK_IMMOLATION_TRIGGERED, true, nullptr, nullptr, caster->GetOwnerGUID());
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_infernal_immolation::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// 30146, 112870 - Summon Felguard, Summon Wrathguard
class spell_warlock_glyph_of_felguard : public SpellScriptLoader
{
public:
    spell_warlock_glyph_of_felguard() : SpellScriptLoader("spell_warlock_glyph_of_felguard") { }

    class spell_warlock_glyph_of_felguard_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_glyph_of_felguard_SpellScript);

        void HandleAfterHit()
        {
            if (Player* caster = GetCaster()->ToPlayer())
            {
                if (!caster->HasAura(SPELL_WARLOCK_GLYPH_OF_FELGUARD))
                    return;

                uint32 itemEntry = 0;
                for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
                    if (Item* pItem = caster->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                        if (const ItemTemplate* itemplate = pItem->GetTemplate())
                            if (itemplate->GetClass() == ITEM_CLASS_WEAPON && (
                                itemplate->GetSubClass() == ITEM_SUBCLASS_WEAPON_SWORD2 ||
                                itemplate->GetSubClass() == ITEM_SUBCLASS_WEAPON_AXE2 ||
                                itemplate->GetSubClass() == ITEM_SUBCLASS_WEAPON_AXE2 ||
                                itemplate->GetSubClass() == ITEM_SUBCLASS_WEAPON_MACE2 ||
                                itemplate->GetSubClass() == ITEM_SUBCLASS_WEAPON_POLEARM))
                            {
                                itemEntry = itemplate->GetId();
                                break;
                            }

                if (Pet* pet = ObjectAccessor::GetPet(*caster, caster->GetPetGUID()))
                {
                    for (uint8 i = 0; i < 3; ++i)
                        pet->SetVirtualItem(i, 0);

                    pet->SetVirtualItem(0, itemEntry);
                }
            }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_warlock_glyph_of_felguard_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_glyph_of_felguard_SpellScript();
    }
};

// 205178 - Soul Effigy target
class spell_warlock_soul_effigy_target : public SpellScriptLoader
{
public:
    spell_warlock_soul_effigy_target() : SpellScriptLoader("spell_warlock_soul_effigy_target") { }

    class spell_warlock_soul_effigy_target_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_soul_effigy_target_AuraScript);

        void PeriodicTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (!caster->Variables.Exist("Spells.SoulEffigyGuid"))
            {
                Remove();
                return;
            }

            ObjectGuid const guid = caster->Variables.GetValue<ObjectGuid>("Spells.SoulEffigyGuid", ObjectGuid::Empty);
            if (!ObjectAccessor::GetUnit(*caster, guid))
                Remove();
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            ObjectGuid const guid = caster->Variables.GetValue<ObjectGuid>("Spells.SoulEffigyGuid", ObjectGuid::Empty);

            if (Unit* effigy = ObjectAccessor::GetUnit(*caster, guid))
                effigy->ToTempSummon()->DespawnOrUnsummon();
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            caster->Variables.Set("Spells.SoulEffigyTargetGuid", target->GetGUID());
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_warlock_soul_effigy_target_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_warlock_soul_effigy_target_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_soul_effigy_target_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_soul_effigy_target_AuraScript();
    }
};

// 196301 - Devourer of Life
class spell_warlock_artifact_devourer_of_life : public SpellScriptLoader
{
public:
    spell_warlock_artifact_devourer_of_life() : SpellScriptLoader("spell_warlock_artifact_devourer_of_life") { }

    class spell_warlock_artifact_devourer_of_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_devourer_of_life_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (roll_chance_i(aurEff->GetAmount()))
                caster->CastSpell(caster, SPELL_WARLOCK_DEVOURER_OF_LIFE_PROC, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_devourer_of_life_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_devourer_of_life_AuraScript();
    }
};

// 212623 - Singe Magic
class spell_warlock_singe_magic : public SpellScriptLoader
{
public:
    spell_warlock_singe_magic() : SpellScriptLoader("spell_warlock_singe_magic") { }

    class spell_warlock_singe_magic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_singe_magic_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            if (Pet* pet = caster->ToPlayer()->GetPet())
                pet->CastCustomSpell(SPELL_WARLOCK_SINGE_MAGIC, SPELLVALUE_BASE_POINT0, GetEffectInfo(EFFECT_0)->BasePoints, target, true);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->ToPlayer())
                return SPELL_FAILED_BAD_TARGETS;

            if (caster->ToPlayer()->GetPet() && caster->ToPlayer()->GetPet()->GetEntry() == 416)
                return SPELL_CAST_OK;

            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warlock_singe_magic_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_warlock_singe_magic_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_singe_magic_SpellScript();
    }
};

// 108370 - Soul Leech
class spell_warlock_soul_leech : public SpellScriptLoader
{
public:
    spell_warlock_soul_leech() : SpellScriptLoader("spell_warlock_soul_leech") { }

    class spell_warlock_soul_leech_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_soul_leech_AuraScript);

        void OnProc(AuraEffect const*  aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* secondaryTarget = nullptr;
            if (Player* player = caster->ToPlayer())
                secondaryTarget = player->GetPet();
            if (Pet* pet = caster->ToPet())
            {
                secondaryTarget = pet->GetOwner();
                if (!secondaryTarget)
                    return;
            }

            Unit* targets[2] = { caster, secondaryTarget };
            for (Unit* target : targets)
            {
                if (target)
                {
                    int32 finalAmount = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
                    int32 soulLinkHeal = finalAmount; // save value for soul link
                    // add old amount
                    if (Aura * aura = target->GetAura(SPELL_WARLOCK_SOUL_LEECH_SHIELD))
                        finalAmount += aura->GetEffect(EFFECT_0)->GetAmount();

                    AddPct(finalAmount, caster->GetAuraEffectAmount(SPELL_ARENA_DAMPENING, EFFECT_0));
                    // max 15% of HP
                    int32 maxHealthPct = GetEffect(EFFECT_1)->GetAmount();
                    if (uint32 demonskinBonus = caster->GetAuraEffectAmount(SPELL_WARLOCK_DEMON_SKIN, EFFECT_1))
                        maxHealthPct = demonskinBonus;
                    finalAmount = std::min<uint32>(finalAmount, CalculatePct(target->GetMaxHealth(), maxHealthPct));
                    target->CastCustomSpell(target, SPELL_WARLOCK_SOUL_LEECH_SHIELD, &finalAmount, nullptr, nullptr, true);

                    if (target->ToPlayer() && target->HasAura(SPELL_WARLOCK_SOUL_LINK_BUFF))
                    {
                        int32 playerHeal = CalculatePct(soulLinkHeal, target->GetAura(SPELL_WARLOCK_SOUL_LINK_BUFF)->GetEffect(EFFECT_1)->GetAmount());
                        int32 petHeal = CalculatePct(soulLinkHeal, target->GetAura(SPELL_WARLOCK_SOUL_LINK_BUFF)->GetEffect(EFFECT_2)->GetAmount());
                        target->CastCustomSpell(target, SPELL_WARLOCK_SOUL_LINK_HEAL, &playerHeal, &petHeal, nullptr, true);
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_soul_leech_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_soul_leech_AuraScript();
    }
};

// 211219 - The Expendables
class spell_warlock_artifact_the_expendables : public SpellScriptLoader
{
public:
    spell_warlock_artifact_the_expendables() : SpellScriptLoader("spell_warlock_artifact_the_expendables") { }

    class spell_warlock_artifact_the_expendables_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_the_expendables_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->ToPlayer())
                return;

            Player* player = caster->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!player)
                return;

            for (Unit* unit : player->m_Controlled)
                player->CastSpell(unit, SPELL_WARLOCK_THE_EXPANDABLES_BUFF, true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warlock_artifact_the_expendables_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_the_expendables_AuraScript();
    }
};

// 103679 - Soul Effigy
class npc_warlock_soul_effigy : public CreatureScript
{
public:
    npc_warlock_soul_effigy() : CreatureScript("npc_warlock_soul_effigy") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_warlock_soul_effigyAI(creature);
    }

    struct npc_warlock_soul_effigyAI : public CreatureAI
    {
        npc_warlock_soul_effigyAI(Creature* creature) : CreatureAI(creature) { }

        void Reset() override
        {
            me->SetControlled(true, UNIT_STATE_ROOT);
            me->CastSpell(me, SPELL_WARLOCK_SOUL_EFFIGY_AURA, true);
        }

        void UpdateAI(uint32 /*diff*/) override { }
    };
};

// 6353 - Soul Fire
class spell_warlock_soul_fire : public SpellScriptLoader
{
public:
    spell_warlock_soul_fire() : SpellScriptLoader("spell_warlock_soul_fire") { }

    class spell_warlock_soul_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_soul_fire_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster())
                GetCaster()->ModifyPower(POWER_SOUL_SHARDS, + 40);

            //TODO: Improve it later
            GetCaster()->GetSpellHistory()->ModifyCooldown(SPELL_WARLOCK_SOUL_FIRE, -2000);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_warlock_soul_fire_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_soul_fire_SpellScript();
    }
};

// 212580 - Eye of the Observer
class spell_warlock_eye_of_the_observer : public SpellScriptLoader
{
public:
    spell_warlock_eye_of_the_observer() : SpellScriptLoader("spell_warlock_eye_of_the_observer") { }

    class spell_warlock_eye_of_the_observer_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_eye_of_the_observer_AuraScript);

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            Unit* actor = eventInfo.GetActor();
            if (!caster || !actor)
                return;

            caster->CastCustomSpell(SPELL_WARLOCK_LASERBEAM, SPELLVALUE_BASE_POINT0, actor->CountPctFromMaxHealth(5), actor, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_eye_of_the_observer_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_eye_of_the_observer_AuraScript();
    }
};

// 199472 - Wrath of Consumption
class spell_warlock_artifact_wrath_of_consumption : public SpellScriptLoader
{
public:
    spell_warlock_artifact_wrath_of_consumption() : SpellScriptLoader("spell_warlock_artifact_wrath_of_consumption") { }

    class spell_warlock_artifact_wrath_of_consumption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_wrath_of_consumption_AuraScript);

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_WARLOCK_WRATH_OF_CONSUMPTION_PROC, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_wrath_of_consumption_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_wrath_of_consumption_AuraScript();
    }
};

// 104318 - Fel Firebolt @ Wild Imp
class spell_warlock_fel_firebolt_wild_imp : public SpellScriptLoader
{
public:
    spell_warlock_fel_firebolt_wild_imp() : SpellScriptLoader("spell_warlock_fel_firebolt_wild_imp") { }

    class spell_warlock_fel_firebolt_wild_imp_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_fel_firebolt_wild_imp_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            // "Increases damage dealt by your Wild Imps' Firebolt by 10%."
            if (Unit * owner = GetCaster()->GetOwner())
            {
                if (uint32 pct = owner->GetAuraEffectAmount(SPELL_WARLOCK_INFERNAL_FURNACE, EFFECT_0))
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), pct));

                if (owner->HasAura(SPELL_WARLOCK_STOLEN_POWER))
                {
                    if (Aura* aur = owner->AddAura(SPELL_WARLOCK_STOLEN_POWER_COUNTER, owner))
                    {
                        if (aur->GetStackAmount() == 100)
                        {
                            owner->CastSpell(owner, SPELL_WARLOCK_STOLEN_POWER_BUFF, true);
                            aur->Remove();
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_fel_firebolt_wild_imp_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_fel_firebolt_wild_imp_SpellScript();
    }
};

// 234877 - Curse of Shadows
class spell_warlock_curse_of_shadows : public SpellScriptLoader
{
public:
    spell_warlock_curse_of_shadows() : SpellScriptLoader("spell_warlock_curse_of_shadows") { }

    class spell_warlock_curse_of_shadows_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_curse_of_shadows_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo();
            if (!spellInfo || !(spellInfo->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                return;

            int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            caster->CastCustomSpell(SPELL_WARLOCK_CURSE_OF_SHADOWS_DAMAGE, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetActionTarget(), true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_curse_of_shadows_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_curse_of_shadows_AuraScript();
    }
};

// 108558 - Nightfall
class spell_warlock_nightfall : public SpellScriptLoader
{
public:
    spell_warlock_nightfall() : SpellScriptLoader("spell_warlock_nightfall") { }

    class spell_warlock_nightfall_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_nightfall_AuraScript);

        void OnAuraProc(ProcEventInfo& /*eventInfo*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_NIGHTFALL_BUFF, true);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return (&eventInfo)->GetSpellInfo()->Id == SPELL_CORRUPTION_DOT;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warlock_nightfall_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_warlock_nightfall_AuraScript::OnAuraProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_nightfall_AuraScript();
    }
};

// Dimensional Rift - 196586
class spell_warlock_artifact_dimensional_rift : public SpellScriptLoader
{
public:
    spell_warlock_artifact_dimensional_rift() : SpellScriptLoader("spell_warlock_artifact_dimensional_rift") {}

    class spell_warlock_artifact_dimensional_rift_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_artifact_dimensional_rift_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            //green //green //purple
            std::vector<uint32> spellVisualIds = { 219117, 219117, 219107 };
            // Chaos Tear  //Chaos Portal  //Shadowy Tear
            std::vector<uint32> summonIds = { 108493,        108493,          99887 };
            // Durations must be longer, because if the npc gets destroyed before the last projectile hits
            // it won't deal any damage.
            std::vector<uint32> durations = { 7000, 4500, 16000 };
            uint32 id = std::rand() % 3;
            Position pos = caster->GetPosition();
            // Portals appear in a random point, in a distance between 4-8yards
            caster->MovePosition(pos, (float)(std::rand() % 5) + 4.f, (float)rand_norm() * static_cast<float>(2 * M_PI));
            if (Creature* rift = caster->SummonCreature(summonIds[id], pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, durations[id]))
            {
                rift->CastSpell(rift, spellVisualIds[id], true);
                rift->SetOwnerGUID(caster->GetGUID());
                // We cannot really use me->GetVictim() inside of the AI, since the target
                // for portal is locked, it doesn't change no matter what. So we set it like this
                rift->SetTarget(target->GetGUID());
                // We use same ID and script for Chaos Portal and Chaos Tear as there are no more NPCs for this spell
                rift->SetArmor(id, 0);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_artifact_dimensional_rift_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_artifact_dimensional_rift_SpellScript();
    }
};

// 3110 - Firebolt
class spell_warlock_imp_firebolt : public SpellScriptLoader
{
public:
    spell_warlock_imp_firebolt() : SpellScriptLoader("spell_warlock_imp_firebolt") { }

    class spell_warlock_imp_firebolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_imp_firebolt_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !caster->GetOwner() || !target)
                return;

            Unit* owner = caster->GetOwner();
            int32 damage = GetHitDamage();
            if (target->HasAura(SPELL_WARLOCK_IMMOLATE_DOT, owner->GetGUID()))
                AddPct(damage, owner->GetAuraEffectAmount(SPELL_WARLOCK_FIREBOLT_BONUS, EFFECT_0));

            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_imp_firebolt_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_imp_firebolt_SpellScript();
    }
};

// Summon Darkglare - 205180
class spell_warlock_summon_darkglare : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_darkglare);

    void HandleOnHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
        {
            Player::AuraEffectList effectList = target->GetAuraEffectsByTypes({ SPELL_AURA_PERIODIC_DAMAGE, SPELL_AURA_PERIODIC_DAMAGE_PERCENT }, GetCaster()->GetGUID());

            for (AuraEffect* effect : effectList)
                if (Aura* aura = effect->GetBase())
                    aura->ModDuration(GetEffectInfo(EFFECT_1)->CalcValue() * IN_MILLISECONDS);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warlock_summon_darkglare::HandleOnHitTarget, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 196412 - Eradication
class spell_warlock_eradication : public SpellScriptLoader
{
public:
    spell_warlock_eradication() : SpellScriptLoader("spell_warlock_eradication") { }

    class spell_warlock_eradication_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_eradication_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return (&eventInfo)->GetSpellInfo()->Id == SPELL_CHAOS_BOLT;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warlock_eradication_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_eradication_AuraScript();
    }
};

// 205247 - Soul Effigy aura
class spell_warlock_soul_effigy_aura : public SpellScriptLoader
{
public:
    spell_warlock_soul_effigy_aura() : SpellScriptLoader("spell_warlock_soul_effigy_aura") { }

    class spell_warlock_soul_effigy_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_soul_effigy_aura_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* owner = caster->ToTempSummon()->GetSummoner();
            if (!owner)
                return;

            if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->IsPositive())
                return;

            int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            if (!damage)
                return;

            ObjectGuid const guid = owner->Variables.GetValue<ObjectGuid>("Spells.SoulEffigyTargetGuid", ObjectGuid::Empty);

            if (Unit* target = ObjectAccessor::GetUnit(*owner, guid))
            {
                caster->CastSpell(target, SPELL_WARLOCK_SOUL_EFFIGY_VISUAL, true, nullptr, nullptr, owner->GetGUID());
                ObjectGuid targetGuid = target->GetGUID();
                ObjectGuid ownerGuid = owner->GetGUID();
                caster->GetScheduler().Schedule(750ms, [caster, targetGuid, damage, ownerGuid](TaskContext /*context*/)
                {
                    Unit* target = ObjectAccessor::GetUnit(*caster, targetGuid);
                    Unit* owner = ObjectAccessor::GetUnit(*caster, ownerGuid);
                    if (!target || !owner)
                        return;

                    caster->CastCustomSpell(SPELL_WARLOCK_SOUL_EFFIGY_DAMAGE, SPELLVALUE_BASE_POINT0, damage, target, true, nullptr, nullptr, owner->GetGUID());
                });
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_soul_effigy_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_soul_effigy_aura_AuraScript();
    }
};

// 219415 - Dimension Ripper
class spell_warlock_artifact_dimension_ripper : public SpellScriptLoader
{
public:
    spell_warlock_artifact_dimension_ripper() : SpellScriptLoader("spell_warlock_artifact_dimension_ripper") { }

    class spell_warlock_artifact_dimension_ripper_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_dimension_ripper_AuraScript);

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->GetSpellHistory()->RestoreCharge(sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DIMENSIONAL_RIFT)->ChargeCategoryId);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_dimension_ripper_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_dimension_ripper_AuraScript();
    }
};

// 6358 - Seduction, 115268 - Mesmerize
class spell_warlock_seduction : public SpellScriptLoader
{
public:
    spell_warlock_seduction() : SpellScriptLoader("spell_warlock_seduction") { }

    class spell_warlock_seduction_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_seduction_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // Glyph of Demon Training
            Unit * target = GetTarget();
            Unit * caster = GetCaster();
            if (!caster)
                return;
            if (Unit * owner = caster->GetOwner())
            {
                if (owner->HasAura(SPELL_WARLOCK_GLYPH_OF_DEMON_TRAINING))
                {
                    target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                    target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                }
            }

            // remove invisibility from Succubus on successful cast
            caster->RemoveAura(SPELL_WARLOCK_PET_LESSER_INVISIBILITY);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_warlock_seduction_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_seduction_AuraScript();
    }
};

// 171017 - Meteor Strike
class spell_warlock_infernal_meteor_strike : public SpellScriptLoader
{
public:
    spell_warlock_infernal_meteor_strike() : SpellScriptLoader("spell_warlock_infernal_meteor_strike") { }

    class spell_warlock_infernal_meteor_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_infernal_meteor_strike_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->GetCharmerOrOwnerPlayerOrPlayerItself())
                if (player->HasAura(SPELL_WARLOCK_LORD_OF_THE_FLAMES) && !player->HasAura(SPELL_WARLOCK_LORD_OF_THE_FLAMES_CD))
                {
                    for (uint32 i = 0; i < 3; ++i)
                        player->CastSpell(caster, SPELL_WARLOCK_LORD_OF_THE_FLAMES_SUMMON, true);
                    player->CastSpell(player, SPELL_WARLOCK_LORD_OF_THE_FLAMES_CD, true);
                }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_warlock_infernal_meteor_strike_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_infernal_meteor_strike_SpellScript();
    }
};

// 111898 - Grimoire: Felguard
class spell_warlock_grimoire_felguard : public SpellScriptLoader
{
public:
    spell_warlock_grimoire_felguard() : SpellScriptLoader("spell_warlock_grimoire_felguard") { }

    class spell_warlock_grimoire_felguard_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_grimoire_felguard_SpellScript);

        SpellCastResult CheckRequirement()
        {
            Player * caster = GetCaster()->ToPlayer();
            if (!caster)
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
            // allow only in Demonology spec
            if (caster->GetSpecializationId() != TALENT_SPEC_WARLOCK_DEMONOLOGY)
                return SPELL_FAILED_NO_SPEC;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warlock_grimoire_felguard_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_grimoire_felguard_SpellScript();
    }
};

// 196305 - Eternal Struggle
class spell_warlock_artifact_eternal_struggle : public SpellScriptLoader
{
public:
    spell_warlock_artifact_eternal_struggle() : SpellScriptLoader("spell_warlock_artifact_eternal_struggle") { }

    class spell_warlock_artifact_eternal_struggle_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_eternal_struggle_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastCustomSpell(SPELL_WARLOCK_ETERNAL_STRUGGLE_PROC, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), caster, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_eternal_struggle_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_eternal_struggle_AuraScript();
    }
};

// 196236 - Soulsnatcher
class spell_warlock_artifact_soul_snatcher : public SpellScriptLoader
{
public:
    spell_warlock_artifact_soul_snatcher() : SpellScriptLoader("spell_warlock_artifact_soul_snatcher") { }

    class spell_warlock_artifact_soul_snatcher_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_soul_snatcher_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (roll_chance_i(aurEff->GetAmount()))
                caster->CastSpell(caster, SPELL_WARLOCK_SOULSNATCHER_PROC, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_soul_snatcher_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_soul_snatcher_AuraScript();
    }
};

// 211720 - Thal'kiel's Discord
class spell_warlock_artifact_thalkiels_discord : public SpellScriptLoader
{
public:
    spell_warlock_artifact_thalkiels_discord() : SpellScriptLoader("spell_warlock_artifact_thalkiels_discord") { }

    class spell_warlock_artifact_thalkiels_discord_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_artifact_thalkiels_discord_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            Unit* target = eventInfo.GetActionTarget();
            if (!caster || !target)
                return;

            if (!caster->IsValidAttackTarget(target))
                return;

            caster->CastSpell(target, aurEff->GetSpellEffectInfo()->TriggerSpell, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_artifact_thalkiels_discord_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_artifact_thalkiels_discord_AuraScript();
    }
};

// 117828 - Backdraft
class spell_warlock_backdraft : public SpellScriptLoader
{
public:
    spell_warlock_backdraft() : SpellScriptLoader("spell_warlock_backdraft") { }

    class spell_warlock_backdraft_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_backdraft_AuraScript);

        bool OnCheckProc(ProcEventInfo& /*eventInfo*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return false;

            if (caster->Variables.GetValue<time_t>("Spells.BackdraftCD", 0) > time(nullptr))
                return false;

            caster->Variables.Set<time_t>("Spells.BackdraftCD", time(nullptr) + 500);
            return true;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warlock_backdraft_AuraScript::OnCheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_backdraft_AuraScript();
    }
};

// 231489 - Compounding Horror
class spell_warlock_compounding_horror : public SpellScriptLoader
{
public:
    spell_warlock_compounding_horror() : SpellScriptLoader("spell_warlock_compounding_horror") { }

    class spell_warlock_compounding_horror_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_compounding_horror_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            int32 damage = GetHitDamage();
            int32 stacks = 0;
            if (Aura* aur = caster->GetAura(SPELL_WARLOCK_COMPOUNDING_HORROR))
                stacks = aur->GetStackAmount();

            SetHitDamage(damage * stacks);

            caster->RemoveAurasDueToSpell(SPELL_WARLOCK_COMPOUNDING_HORROR);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_compounding_horror_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_compounding_horror_SpellScript();
    }
};

// 233494 - Contagion
class spell_warlock_contagion : public SpellScriptLoader
{
public:
    spell_warlock_contagion() : SpellScriptLoader("spell_warlock_contagion") { }

    class spell_warlock_contagion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_contagion_AuraScript);

        void PeriodicTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            std::vector<int32> uaspells = { SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT5, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT4,
                SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT3, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT2,
                SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT1 };

            bool hasUa = false;
            for (int32 ua : uaspells)
                if (target->HasAura(ua, caster->GetGUID()))
                    hasUa = true;

            if (!hasUa)
                Remove();
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_contagion_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_MOD_SCHOOL_MASK_DAMAGE_FROM_CASTER);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_contagion_AuraScript();
    }
};

// 212282 - Cremation
class spell_warlock_cremation : public SpellScriptLoader
{
public:
    spell_warlock_cremation() : SpellScriptLoader("spell_warlock_cremation") { }

    class spell_warlock_cremation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_cremation_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();
            Unit* target = eventInfo.GetActionTarget();
            if (!caster || !target)
                return;

            switch (eventInfo.GetDamageInfo()->GetSpellInfo()->Id)
            {
            case SPELL_WARLOCK_SHADOWBURN:
            case SPELL_WARLOCK_CONFLAGRATE:
                caster->CastCustomSpell(GetSpellInfo()->GetEffect(EFFECT_0)->TriggerSpell, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), target, true);
                break;
            case SPELL_WARLOCK_INCINERATE:
                caster->CastSpell(target, SPELL_WARLOCK_IMMOLATE_DOT, true);
                break;
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warlock_cremation_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_cremation_AuraScript();
    }
};

// 85692 - Doom Bolt
class spell_warlock_doomguard_doom_bolt : public SpellScriptLoader
{
public:
    spell_warlock_doomguard_doom_bolt() : SpellScriptLoader("spell_warlock_doomguard_doom_bolt") { }

    class spell_warlock_doomguard_doom_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warlock_doomguard_doom_bolt_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            // "Deals 20% additional damage to targets below 20% health"
            if (GetHitUnit()->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
            {
                uint32 damage = GetHitDamage();
                AddPct(damage, GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints);
                SetHitDamage(damage);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warlock_doomguard_doom_bolt_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warlock_doomguard_doom_bolt_SpellScript();
    }
};

// 221711 - Essence Drain
// Called by Drain Soul (198590) and Drain Life (234153)
class spell_warlock_essence_drain : public SpellScriptLoader
{
public:
    spell_warlock_essence_drain() : SpellScriptLoader("spell_warlock_essence_drain") { }

    class spell_warlock_essence_drain_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warlock_essence_drain_AuraScript);

        void PeriodicTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            if (!caster || !target)
                return;

            if (caster->HasAura(SPELL_WARLOCK_ESSENCE_DRAIN))
                caster->CastSpell(target, SPELL_WARLOCK_ESSENCE_DRAIN_DEBUFF, true);

            if (uint32 durationBonus = caster->GetAuraEffectAmount(SPELL_WARLOCK_ROT_AND_DECAY, EFFECT_0))
            {
                std::vector<uint32> dots{ SPELL_WARLOCK_AGONY, SPELL_WARLOCK_CORRUPTION_TRIGGERED, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT1, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT2,
                    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT3, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT4, SPELL_WARLOCK_UNSTABLE_AFFLICTION_DOT5 };

                for (uint32 dot : dots)
                    if (Aura* aur = target->GetAura(dot, caster->GetGUID()))
                        aur->ModDuration(durationBonus);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warlock_essence_drain_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warlock_essence_drain_AuraScript();
    }
};

// 108415 - Soul Link 8.xx
class spell_warl_soul_link : public SpellScript
{
    PrepareSpellScript(spell_warl_soul_link);

    void HandleOnHit()
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* target = GetHitUnit())
            {
                if (!target->HasAura(SPELL_WARLOCK_SOUL_LINK_DUMMY_AURA))
                    caster->CastSpell(caster, SPELL_WARLOCK_SOUL_LINK_DUMMY_AURA, true);
            }
        }
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warl_soul_link::HandleOnHit);
    }
};

//232670
class spell_warr_shadowbolt_affliction : public SpellScript
{
    PrepareSpellScript(spell_warr_shadowbolt_affliction);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        if (caster->HasAura(SPELL_SHADOW_EMBRACE))
            caster->AddAura(SPELL_SHADOW_EMBRACE_TARGET_DEBUFF, target);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_warr_shadowbolt_affliction::HandleOnHit);
    }
};

void AddSC_warlock_spell_scripts()
{
    new spell_warl_demon_skin();
    RegisterSpellScript(spell_warlock_summon_darkglare);
    new spell_warlock_artifact_soul_snatcher();
    new spell_warlock_doomguard_doom_bolt();
    new spell_warlock_backdraft();
    new spell_warlock_contagion();
    new spell_warlock_essence_drain();
    new spell_warlock_compounding_horror();
    new spell_warlock_cremation();
    new spell_warlock_artifact_thalkiels_discord();
    new spell_warlock_grimoire_felguard();
    new spell_warlock_artifact_eternal_struggle();
    new spell_warlock_infernal_meteor_strike();
    new spell_warlock_nightfall();
    new spell_warlock_artifact_dimension_ripper();
    new spell_warlock_seduction();
    new spell_warlock_artifact_dimensional_rift();
    new spell_warlock_soul_effigy_aura();
    new spell_warlock_eradication();
    new spell_warlock_imp_firebolt();
    new spell_warlock_fel_firebolt_wild_imp();
    new spell_warlock_curse_of_shadows();
    new spell_warlock_eye_of_the_observer();
    new spell_warlock_singe_magic();
    new spell_warlock_artifact_wrath_of_consumption();
    new spell_warlock_soul_fire();
    new spell_warlock_soul_effigy_target();
    new spell_warlock_artifact_the_expendables();
    new spell_warlock_artifact_devourer_of_life();
    new spell_warlock_siphon_life();
    new spell_warlock_soul_leech();
    new spell_warlock_dark_pact();
    new spell_warlock_glyph_of_felguard();
    RegisterAuraScript(spell_warlock_infernal_immolation);
    RegisterAuraScript(spell_warlock_drain_life);
    new spell_warlock_4p_t14_pve();
    new spell_warlock_rain_of_fire_damage();
    new spell_warlock_soul_shatter();
    new spell_warlock_artifact_reap_souls();
    new spell_warlock_t16_4p();
    new spell_warlock_eye_of_kilrogg();
    new spell_warlock_soul_link();
    new spell_warlock_demonbolt_new();
    new spell_warlock_artifact_thalkiels_consumption();
    new spell_warlock_t15_2p_bonus();
    new spell_warlock_call_felhunter();
    new spell_warlock_doom();
    RegisterSpellScript(spell_warl_banish);
    RegisterSpellAndAuraScriptPair(spell_warl_burning_rush, aura_warl_burning_rush);
    RegisterSpellScript(spell_warl_chaos_bolt);
    RegisterSpellScript(spell_warl_conflagrate);
    RegisterSpellScript(spell_warl_conflagrate_aura);
    RegisterAuraScript(spell_warl_corruption_effect);
    RegisterSpellScript(spell_warl_create_healthstone);
    RegisterSpellScript(spell_warl_create_healthstone_soulwell);
    RegisterAuraScript(spell_warl_dark_pact);
    RegisterAuraScript(spell_warl_dark_regeneration);
    RegisterSpellScript(spell_warl_demonbolt);
    RegisterSpellScript(spell_warl_demonic_call);
    RegisterAuraScript(spell_warl_demonic_circle_summon);
    RegisterAuraScript(spell_warl_demonic_circle_teleport);
    RegisterSpellScript(spell_warl_demonic_empowerment);
    new spell_warl_demonic_gateway();
    new spell_warlock_t16_demo_2p();
    new spell_warlock_call_dreadstalkers();
    new spell_warlock_unending_breath();
    RegisterSpellScript(spell_warl_devour_magic);
    RegisterSpellScript(spell_warl_soul_link);
    new spell_warlock_unending_resolve();
    new spell_warlock_artifact_soul_flame();
    new spell_warl_drain_soul();
    new spell_warlock_use_soulstone();
    new spell_warl_fear();
    new spell_warl_deathbolt();
    new spell_warl_fear_buff();
    new spell_warl_glyph_of_soulwell();
    RegisterSpellScript(spell_warl_hand_of_guldan);
    new spell_warl_hand_of_guldan_damage();
    RegisterAuraScript(aura_warl_haunt);
    new spell_warl_havoc();
    RegisterAuraScript(spell_warl_health_funnel);
    RegisterSpellScript(spell_warl_healthstone_heal);
    new spell_warl_immolate();
    RegisterAuraScript(spell_warl_immolate_aura);
    RegisterSpellScript(spell_warl_life_tap);
    RegisterSpellAndAuraScriptPair(spell_warl_seed_of_corruption, aura_warl_seed_of_corruption);
    new spell_warlock_seed_of_corruption_damage();
    RegisterSpellScript(spell_warl_shadow_bolt);
    new spell_warl_shadow_bulwark();
    new spell_warl_shadowburn();
    new spell_warl_soul_leach_applier();
    RegisterAuraScript(spell_warl_soul_leech_aura);
    new spell_warl_soulshatter();
    new spell_warlock_unstable_affliction();
    new spell_warlock_unstable_affliction_dispel();
    new spell_warl_void_ray();
    new spell_warlock_agony();
    new spell_warl_cauterize_master();
    new spell_warl_suffering();
    new spell_warl_whiplash();
    new spell_warl_spell_lock();
    new spell_warl_felstorm();
    RegisterSpellScript(spell_warl_demonwrath);
    RegisterAuraScript(spell_warl_demonwrath_periodic);
    new spell_warl_meteor_strike();
    new spell_warl_shadow_lock();
    new spell_warl_cataclysm();
    new spell_warlock_immolate_dot();
    new spell_warl_channel_demonfire();
    new spell_warl_soul_conduit();
    new spell_warl_soul_harvest();
    new spell_warl_grimoire_of_service();
    new spell_warl_implosion();
    new spell_warl_grimoire_of_synergy();
    new spell_warl_eye_laser();
    new spell_warl_demonic_calling();
    new spell_warl_chaotic_energies();
    new spell_warl_eradication();
    RegisterAuraScript(aura_warl_phantomatic_singularity);
    RegisterAuraScript(spell_warl_grimoire_of_service_aura);
    RegisterSpellScript(spell_warl_incinerate);
    RegisterAreaTriggerAI(at_warlock_rain_of_fire);
    RegisterAreaTriggerAI(at_warlock_artifact_thalkiels_discord);
    new npc_warl_fel_lord();
    new npc_warlock_infernal();
    new npc_warlock_doomguard();
    new npc_warlock_soul_effigy();
    new npc_warl_demonic_gateway();
    new npc_warlock_dreadstalker();
    new npc_pet_warlock_darkglare();
    RegisterCreatureAI(npc_pet_warlock_wild_imp);
    RegisterSpellScript(spell_warr_shadowbolt_affliction);
}
