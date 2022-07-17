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

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "Containers.h"
#include "DynamicObject.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "SpellPackets.h"
#include "Unit.h"

enum DruidSpells
{
    SPELL_DRUID_THRASH_BEAR_PERIODIC_DAMAGE = 192090,
    SPELL_DRUID_MAUL = 6807,
    SPELL_DRUID_BLESSING_OF_THE_ANCIENTS = 202360,
    SPELL_DRUID_BLESSING_OF_ELUNE = 202737,
    SPELL_DRUID_BLESSING_OF_ANSHE = 202739,
    SPELL_DRUID_STARLORD_DUMMY = 202345,
    SPELL_DRUID_STARLORD_SOLAR = 202416,
    SPELL_DRUID_STARLORD_LUNAR = 202423,
    SPELL_DRUID_GLYPH_OF_STARS = 114301,
    SPELL_DRUID_CHOSEN_OF_ELUNE = 102560,
    SPELL_DRUID_BLUE_COLOR = 108268,
    SPELL_DRUID_SHADOWY_GHOST = 165114,
    SPELL_DRUID_GORE = 210706,
    SPELL_DRUID_YSERA_GIFT = 145108,
    SPELL_DRUID_YSERA_GIFT_CASTER_HEAL = 145109,
    SPELL_DRUID_YSERA_GIFT_RAID_HEAL = 145110,
    SPELL_DRUID_REJUVENATION = 774,
    SPELL_DRUID_HEALING_TOUCH = 5185,
    SPELL_DRUID_SWIFTMEND = 18562,
    SPELL_DRUID_TRAVEL_FORM = 783,
    SPELL_DRUID_FELINE_SWIFTNESS = 131768,
    SPELL_DRUID_SHRED = 5221,
    SPELL_DRUID_RAKE = 1822,
    SPELL_DRUID_RIP = 1079,
    SPELL_DRUID_FEROCIOUS_BITE = 22568,
    SPELL_DRUID_MOONFIRE_CAT = 155625,
    SPELL_DRUID_SWIPE_CAT = 106785,
    SPELL_DRUID_SABERTOOTH = 202031,
    SPELL_DRUID_FORMS_TRINKET_BEAR = 37340,
    SPELL_DRUID_FORMS_TRINKET_CAT = 37341,
    SPELL_DRUID_FORMS_TRINKET_MOONKIN = 37343,
    SPELL_DRUID_FORMS_TRINKET_NONE = 37344,
    SPELL_DRUID_FORMS_TRINKET_TREE = 37342,
    SPELL_DRUID_GLYPH_OF_INNERVATE = 54833,
    SPELL_DRUID_INCREASED_MOONFIRE_DURATION = 38414,
    SPELL_DRUID_GLYPH_OF_STARFIRE = 54846,
    SPELL_DRUID_ITEM_T8_BALANCE_RELIC = 64950,
    SPELL_DRUID_STAMPEDE = 81022,
    SPELL_DRUID_STAMPEDE_BAER_RANK_1 = 81016,
    SPELL_DRUID_STAMPEDE_CAT_RANK_1 = 81021,
    SPELL_DRUID_STAMPEDE_CAT_STATE = 109881,
    SPELL_DRUID_FERAL_CHARGE_BEAR = 16979,
    SPELL_DRUID_FERAL_CHARGE_CAT = 49376,
    SPELL_DRUID_GLYPH_OF_TYPHOON = 62135,
    SPELL_DRUID_EXHILARATE = 28742,
    SPELL_DRUID_INFUSION = 37238,
    SPELL_DRUID_BLESSING_OF_THE_CLAW = 28750,
    SPELL_DRUID_BLESSING_OF_REMULOS = 40445,
    SPELL_DRUID_BLESSING_OF_ELUNE_40446 = 40446,
    SPELL_DRUID_BLESSING_OF_CENARIUS = 40452,
    SPELL_DRUID_LANGUISH = 71023,
    SPELL_DRUID_REJUVENATION_T10_PROC = 70691,
    SPELL_DRUID_SHOOTING_STARS = 93400,
    SPELL_DRUID_SOLAR_ECLIPSE = 48517,
    SPELL_DRUID_SOLAR_ECLIPSE_MARKER = 67483, // Will make the yellow arrow on eclipse bar point to the yellow side (solar)
    SPELL_DRUID_LUNAR_ECLIPSE = 48518,
    SPELL_DRUID_LUNAR_ECLIPSE_MARKER = 67484, // Will make the yellow arrow on eclipse bar point to the blue side (lunar)
    SPELL_DRUID_WRATH = 5176,
    SPELL_DRUID_EUPHORIA = 152222,
    SPELL_DRUID_STARFIRE = 2912,
    SPELL_DRUID_SOLAR_BEAM = 78675,
    SPELL_DRUID_SOLAR_BEAM_SILENCE = 81261,
    SPELL_DRU_REGROWTH = 8936,
    SPELL_DRU_BLOODTALONS = 155672,
    SPELL_DRU_BLOODTALONS_TRIGGERED = 145152,
    SPELL_DRU_MOMENT_OF_CLARITY = 155577,
    SPELL_DRU_CLEARCASTING = 16870,
    SPELL_DRUID_OMEN_OF_CLARITY = 113043,
    SPELL_ZANDALARI_TROLL_DRUID_SABERTUSK_CAT_SHAPESHIFT = 287362,
    SPELL_ZANDALARI_PTERRODAX_FLY_SHAPESHIFT = 265524,
    SPELL_ZANDALARI_TRAVEL_FORM_RAPTOR_SHAPESHIFT = 271899,
    SPELL_ZANDALARI_BEAR_FORM_ANKYLODON_SHAPESHIFT = 262551,
    MODEL_ZANDALARI_BEAR_FORM = 84869,
    //SPELL_ZANDALARI_TROLL_AQUATIC_FORM = ?
    SPELL_FERAL_FRENZY_MAIN = 274837,
    SPELL_FERAL_FRENZY_BLEED = 274838,
    SPELL_DRU_ECLIPSE = 279619,
    SPELL_DRU_SOLAR_EMPOWEREMENT = 164545,
    SPELL_DRU_LUNAR_EMPOWEREMENT = 164547,
    SPELL_DRU_STARLORD = 203245,
    SPELL_DRU_STARLORD_BUFF = 279709,
    //
    SPELL_DRU_ABUNDANCE = 207383,
    SPELL_DRU_ABUNDANCE_BUFF = 207640,
    SPELL_DRU_SOUL_OF_THE_FOREST_RESTO = 158478,
    SPELL_DRU_SOUL_OF_THE_FOREST_RESTO_BUFF = 114108,
    SPELL_DRU_CULTIVATION = 200390,
    SPELL_DRU_CULTIVATION_HEAL = 200389,
    SPELL_DRU_GERMINATION = 155675,
    SPELL_DRU_GERMINATION_HOT = 155777,
    SPELL_DRU_GLYPH_OF_REJUVENATION = 17076,
    SPELL_DRU_GLYPH_OF_REJUVENATION_EFFECT = 96206,
    SPELL_DRU_SPRING_BLOSSOMS = 207385,
    SPELL_DRU_SPRING_BLOSSOMS_HEAL = 207386,
    SPELL_DRU_PHOTOSYNTHESIS = 274902,
    SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS = 274906,
    SPELL_DRU_LUNAR_BEAM_DAMAGE_HEAL = 204069,
    SPELL_DRU_URSOL_VORTEX_PULL = 118283,
    SPELL_DRU_MASS_ENTANGLEMENT = 102359,
    SPELL_DRU_GALACTIC_GUARDIAN = 203964,
    SPELL_DRU_GALACTIC_GAURDIAN_MOD_MOONFIRE = 213708,
    SPELL_DRU_PREDATOR = 202021,
    SPELL_DRU_TIGER_FURY = 5217,
};

enum ShapeshiftFormSpells
{
    SPELL_DRUID_BEAR_FORM = 5487,
    SPELL_DRUID_CAT_FORM = 768,
    SPELL_DRUID_MOONKIN_FORM = 24858,
    SPELL_DRUID_INCARNATION_TREE_OF_LIFE = 33891,
    SPELL_DRUID_INCARNATION_KING_OF_JUNGLE = 102543
};

enum SoulOfTheForestSpells
{
    SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO_TALENT = 158478,
    SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO = 114108
};

enum EfflorescenceSpells
{
    SPELL_DRUID_EFFLORESCENCE_HEAL = 81269,
    SPELL_DRUID_EFFLORESCENCE_DUMMY = 81262
};

enum GoreSpells
{
    SPELL_DRUID_THRASH = 106832,
    SPELL_DRUID_MOONFIRE = 8921,
    SPELL_DRUID_SWIPE = 213764
};

// 210706 - Gore 7.3.5
class spell_dru_gore : public AuraScript
{
    PrepareAuraScript(spell_dru_gore);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        bool _spellCanProc = (eventInfo.GetSpellInfo()->Id == SPELL_DRUID_THRASH || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_MAUL || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_MOONFIRE || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_SWIPE);
        return (eventInfo.GetHitMask() & PROC_HIT_NORMAL) && _spellCanProc;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_dru_gore::CheckProc);
    }
};

// Maul (Bear Form) - 6807
class spell_dru_maul_bear : public SpellScript
{
    PrepareSpellScript(spell_dru_maul_bear);

    void OnHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_maul_bear::OnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnEffectHitTarget += SpellEffectFn(spell_dru_maul_bear::OnHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Thrash (Bear Form) - 77758
class spell_dru_thrash_bear : public SpellScript
{
    PrepareSpellScript(spell_dru_thrash_bear);

    void OnHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        caster->CastSpell(target, SPELL_DRUID_THRASH_BEAR_PERIODIC_DAMAGE, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_thrash_bear::OnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Thrash (Bear Form) periodic damage aura - 192090
class aura_dru_thrash_bear : public AuraScript
{
    PrepareAuraScript(aura_dru_thrash_bear);

    void OnTick(AuraEffect const* auraEff)
    {
        if (Unit* caster = auraEff->GetCaster())
        {
            if (Player* player = caster->ToPlayer())
            {
                if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
                {
                    int32 dmg = player->m_unitData->AttackPower * 0.605f;
                    dmg = (dmg * GetStackAmount()) / 5;
                    aurEff->SetDamage(dmg);
                }
            }
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_dru_thrash_bear::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// Blessing of the Ancients - 202360
class spell_dru_blessing_of_the_ancients : public SpellScript
{
    PrepareSpellScript(spell_dru_blessing_of_the_ancients);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        uint32 removeAura = GetCaster()->HasAura(SPELL_DRUID_BLESSING_OF_ELUNE) ? SPELL_DRUID_BLESSING_OF_ELUNE : SPELL_DRUID_BLESSING_OF_ANSHE;
        uint32 addAura = GetCaster()->HasAura(SPELL_DRUID_BLESSING_OF_ELUNE) ? SPELL_DRUID_BLESSING_OF_ANSHE : SPELL_DRUID_BLESSING_OF_ELUNE;

        GetCaster()->RemoveAurasDueToSpell(removeAura);
        GetCaster()->CastSpell(nullptr, addAura, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_blessing_of_the_ancients::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Solar empowerment - 164545
class aura_dru_solar_empowerment : public AuraScript
{
    PrepareAuraScript(aura_dru_solar_empowerment);

    void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (GetTarget()->HasAura(SPELL_DRUID_STARLORD_DUMMY))
            GetTarget()->CastSpell(nullptr, SPELL_DRUID_STARLORD_SOLAR, true);
    }

    void OnRemove(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STARLORD_SOLAR);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_dru_solar_empowerment::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_dru_solar_empowerment::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

// Lunar empowerment - 164547
class aura_dru_lunar_empowerment : public AuraScript
{
    PrepareAuraScript(aura_dru_lunar_empowerment);

    void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (GetTarget()->HasAura(SPELL_DRUID_STARLORD_DUMMY))
            GetTarget()->CastSpell(nullptr, SPELL_DRUID_STARLORD_LUNAR, true);
    }

    void OnRemove(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STARLORD_LUNAR);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_dru_lunar_empowerment::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_dru_lunar_empowerment::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};
//7.3.2.25549 END

// Efflorescence - 145205
class spell_dru_efflorescence : public SpellScript
{
    PrepareSpellScript(spell_dru_efflorescence);

    enum eCreature
    {
        NPC_EFFLORESCENCE = 47649
    };

    void HandleSummon()
    {
        if (Unit* caster = GetCaster())
            if (Creature* efflorescence = caster->GetSummonedCreatureByEntry(NPC_EFFLORESCENCE))
                efflorescence->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_efflorescence::HandleSummon);
    }
};

// Efflorescence (Aura) - 81262
class spell_dru_efflorescence_aura : public AuraScript
{
    PrepareAuraScript(spell_dru_efflorescence_aura);

    void HandleHeal(AuraEffect const* /*aurEff*/)
    {
        if (GetCaster() && GetCaster()->GetOwner())
        {
            GetCaster()->GetOwner()->CastSpell(GetCaster()->GetPosition(), SPELL_DRUID_EFFLORESCENCE_HEAL, true, nullptr, nullptr, GetCaster()->GetOwnerGUID());
            std::list<Player*> playerList;
            GetCaster()->GetPlayerListInGrid(playerList, 11.2f);
            for (auto& targets : playerList)
            {
                if (GetCaster()->GetOwner()->HasAura(SPELL_DRU_SPRING_BLOSSOMS))
                    if (!targets->HasAura(SPELL_DRU_SPRING_BLOSSOMS_HEAL))
                        GetCaster()->GetOwner()->CastSpell(targets, SPELL_DRU_SPRING_BLOSSOMS_HEAL, true, nullptr, nullptr, GetCaster()->GetOwnerGUID());
            }
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_efflorescence_aura::HandleHeal, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// Efflorescence (Heal) - 81269
class spell_dru_efflorescence_heal : public SpellScript
{
    PrepareSpellScript(spell_dru_efflorescence_heal);

    void SortTargets(std::list<WorldObject*>& targets)
    {
        targets.sort(Trinity::HealthPctOrderPred());

        if (targets.size() > 3)
            targets.resize(3);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_efflorescence_heal::SortTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
    }
};

// 159286 Primal Fury 
// @Version : 7.1.0.22908
class spell_dru_primal_fury : public SpellScriptLoader
{
public:
    spell_dru_primal_fury() : SpellScriptLoader("spell_dru_primal_fury") { }

    class spell_dru_primal_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_primal_fury_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            bool _spellCanProc = (eventInfo.GetSpellInfo()->Id == SPELL_DRUID_SHRED || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_RAKE || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_SWIPE_CAT || eventInfo.GetSpellInfo()->Id == SPELL_DRUID_MOONFIRE_CAT);

            if ((eventInfo.GetHitMask() & PROC_HIT_CRITICAL) && _spellCanProc)
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_primal_fury_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_primal_fury_AuraScript();
    }
};

enum PredatorySwiftnessSpells
{
    SPELL_DRUID_PREDATORY_SWIFTNESS = 16974,
    SPELL_DRUID_PREDATORY_SWIFTNESS_AURA = 69369
};

// Predatory Swiftness - 16974
// @Called : Maim - 22570, Ferocious Bite - 22568, Rip - 1079, Savage Roar - 1079
// @Version : 7.1.0.22908
class spell_dru_predatory_swiftness : public SpellScriptLoader
{
public:
    spell_dru_predatory_swiftness() : SpellScriptLoader("spell_dru_predatory_swiftness") { }

    class spell_dru_predatory_swiftness_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_predatory_swiftness_SpellScript);

        uint8 _cp;

        bool Load() override
        {
            _cp = GetCaster()->GetPower(POWER_COMBO_POINTS);
            return true;
        }

        SpellCastResult CheckCast()
        {
            if (GetCaster())
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (!GetCaster()->ToPlayer()->GetPower(POWER_COMBO_POINTS))
                    return SPELL_FAILED_NO_COMBO_POINTS;
            }
            else
                return SPELL_FAILED_DONT_REPORT;

            return SPELL_CAST_OK;
        }

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (player->HasAura(SPELL_DRUID_PREDATORY_SWIFTNESS) && roll_chance_i(20 * _cp))
                    player->CastSpell(player, SPELL_DRUID_PREDATORY_SWIFTNESS_AURA, true);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_predatory_swiftness_SpellScript::CheckCast);
            AfterHit += SpellHitFn(spell_dru_predatory_swiftness_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_predatory_swiftness_SpellScript();
    }
};

// Predatory Swiftness (Aura) - 69369
// @Called : Entangling Roots - 339, Rebirth - 20484, Regrowth - 8936
// @Version : 7.1.0.22908
class spell_dru_predatory_swiftness_aura : public SpellScriptLoader
{
public:
    spell_dru_predatory_swiftness_aura() : SpellScriptLoader("spell_dru_predatory_swiftness_aura") { }

    class spell_dru_predatory_swiftness_aura_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_predatory_swiftness_aura_SpellScript);

        void HandleAfterHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->HasAura(SPELL_DRUID_PREDATORY_SWIFTNESS_AURA))
                    player->RemoveAurasDueToSpell(SPELL_DRUID_PREDATORY_SWIFTNESS_AURA);
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_dru_predatory_swiftness_aura_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_predatory_swiftness_aura_SpellScript();
    }
};

enum LifebloomSpells
{
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL = 33778
};

//33763
class spell_dru_lifebloom : public AuraScript
{
    PrepareAuraScript(spell_dru_lifebloom);

    void OnTick(AuraEffect const* /*aurEff*/)
    {
        if (GetCaster()->HasAura(SPELL_DRUID_OMEN_OF_CLARITY))
            if (roll_chance_f(4))
                GetCaster()->CastSpell(nullptr, SPELL_DRU_CLEARCASTING, true);
    }

    void AfterRemove(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
        if (removeMode != AURA_REMOVE_BY_EXPIRE && removeMode != AURA_REMOVE_BY_ENEMY_SPELL)
            return;

        Unit* caster = GetCaster();
        if (!caster)
            return;

        caster->CastSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_lifebloom::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 106951 Berserk - 
// @Version : 7.1.0.22908
class spell_dru_berserk : public SpellScriptLoader
{
public:
    spell_dru_berserk() : SpellScriptLoader("spell_dru_berserk") { }

    class spell_dru_berserk_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_berserk_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() != FORM_CAT_FORM)
                return SPELL_FAILED_ONLY_SHAPESHIFT;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_berserk_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_berserk_SpellScript();
    }
};

enum SunfireSpells
{
    SPELL_DRUID_SUNFIRE_DAMAGE = 164815
};

// 93402 Sunfire - 
// @Version : 7.1.0.22908
class spell_dru_sunfire : public SpellScriptLoader
{
public:
    spell_dru_sunfire() : SpellScriptLoader("spell_dru_sunfire") {}

    class spell_dru_sunfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_sunfire_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster)
                return;

            caster->CastSpell(target, SPELL_DRUID_SUNFIRE_DAMAGE, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_sunfire_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_sunfire_SpellScript();
    }
};

enum BalanceAffinitySpells
{
    SPELL_DRUID_STARSURGE = 78674,
    SPELL_DRUID_SUNFIRE = 93402,
    SPELL_DRUID_LUNAR_STRIKE = 194153,
    SPELL_DRUID_SOLAR_WRATH = 190984
};

// 197488 Balance Affinity (Feral, Guardian) - 
// @Version : 7.1.0.22908
class spell_dru_balance_affinity_dps : public SpellScriptLoader
{
public:
    spell_dru_balance_affinity_dps() : SpellScriptLoader("spell_dru_balance_affinity_dps") {}

    class spell_dru_balance_affinity_dps_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_balance_affinity_dps_AuraScript);

        void LearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->AddTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->AddTemporarySpell(SPELL_DRUID_STARSURGE);
                player->AddTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
                player->AddTemporarySpell(SPELL_DRUID_SOLAR_WRATH);
                player->AddTemporarySpell(SPELL_DRUID_SUNFIRE);
            }
        }

        void UnlearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->RemoveTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->RemoveTemporarySpell(SPELL_DRUID_STARSURGE);
                player->RemoveTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
                player->RemoveTemporarySpell(SPELL_DRUID_SOLAR_WRATH);
                player->RemoveTemporarySpell(SPELL_DRUID_SUNFIRE);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_balance_affinity_dps_AuraScript::UnlearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_dru_balance_affinity_dps_AuraScript::LearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_balance_affinity_dps_AuraScript();
    }
};

// 197632 Balance Affinity (Restoration) - 
// @Version : 7.1.0.22908
class spell_dru_balance_affinity_resto : public SpellScriptLoader
{
public:
    spell_dru_balance_affinity_resto() : SpellScriptLoader("spell_dru_balance_affinity_resto") {}

    class spell_dru_balance_affinity_resto_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_balance_affinity_resto_AuraScript);

        void LearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->AddTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->AddTemporarySpell(SPELL_DRUID_STARSURGE);
                player->AddTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
            }
        }

        void UnlearnSpells(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                player->RemoveTemporarySpell(SPELL_DRUID_MOONKIN_FORM);
                player->RemoveTemporarySpell(SPELL_DRUID_STARSURGE);
                player->RemoveTemporarySpell(SPELL_DRUID_LUNAR_STRIKE);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_balance_affinity_resto_AuraScript::UnlearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_dru_balance_affinity_resto_AuraScript::LearnSpells, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_balance_affinity_resto_AuraScript();
    }
};

// Ferocious Bite - 22568
class spell_dru_ferocious_bite : public SpellScript
{
    PrepareSpellScript(spell_dru_ferocious_bite);

    void HandleOnTakePower(SpellPowerCost& powerCost)
    {
        if (powerCost.Power == POWER_COMBO_POINTS)
            m_comboPoints = powerCost.Amount;
    }

    void CalcDmg(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            int32 dmg = GetHitDamage();

            // Calculate damage per Combo Points
            float multiplier = (float)m_comboPoints / (float)caster->GetMaxPower(POWER_COMBO_POINTS);
            int32 newdmg = CalculatePct(dmg, multiplier * 100.f);

            // Calculate additional consumed Energy (up to 25 Energy) and increase damage based on Energy consumed (up to 100%)
            int32 energyConsumed = caster->GetPower(POWER_ENERGY) > 25 ? 25 : caster->GetPower(POWER_ENERGY);
            AddPct(newdmg, energyConsumed * (100.0f / 25.0f));

            SetHitDamage(newdmg);

            // If caster's target is below 25% health or the caster have Sabertooth talent,
            // refresh the duration of caster's Rip on the target
            if (Unit* target = GetHitUnit())
                if (target->HasAuraState(AURA_STATE_HEALTHLESS_25_PERCENT) || caster->HasAura(SPELL_DRUID_SABERTOOTH))
                    if (Aura* rip = target->GetAura(SPELL_DRUID_RIP, caster->GetGUID()))
                        rip->RefreshDuration();
        }
    }

    void Register() override
    {
        OnTakePower += SpellOnTakePowerFn(spell_dru_ferocious_bite::HandleOnTakePower);
        OnEffectHitTarget += SpellEffectFn(spell_dru_ferocious_bite::CalcDmg, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

private:
    int32 m_comboPoints = 0;
};

// 1850 Dash - 
// @Version : 7.1.0.22908
class spell_dru_dash : public SpellScriptLoader
{
public:
    spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

    class spell_dru_dash_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_dash_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // do not set speed if not in cat form
            if (GetUnitOwner()->GetShapeshiftForm() != FORM_CAT_FORM)
                amount = 0;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_dash_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_dash_AuraScript();
    }
};

enum SavageRoarSpells
{
    SPELL_DRUID_SAVAGE_ROAR = 62071
};

// 52610 Savage Roar - 
// @Version : 7.1.0.22908
class spell_dru_savage_roar : public SpellScriptLoader
{
public:
    spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

    class spell_dru_savage_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_roar_SpellScript);

        int32 m_ComboPoint = 0;
        int32 m_OldDuration = 0;

        SpellCastResult CheckCast()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster->GetShapeshiftForm() != FORM_CAT_FORM)
                return SPELL_FAILED_ONLY_SHAPESHIFT;

            return SPELL_CAST_OK;
        }

        void HandleOnPrepare()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            m_ComboPoint = caster->GetPower(POWER_COMBO_POINTS);

            if (Aura* savageRoar = caster->GetAura(GetSpellInfo()->Id))
                m_OldDuration = savageRoar->GetDuration();
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            int32 bonusDuration = m_ComboPoint * 6 * IN_MILLISECONDS;
            int32 maxDuration = GetSpellInfo()->GetMaxDuration();
            int32 newDuration = m_OldDuration + GetSpellInfo()->GetDuration() + bonusDuration;

            if (Aura* savageRoar = caster->GetAura(GetSpellInfo()->Id))
            {
                /// Can't be more then 130% of max duration
                if (newDuration > maxDuration)
                    newDuration = maxDuration;

                savageRoar->SetDuration(newDuration);
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
            OnPrepare += SpellOnPrepareFn(spell_dru_savage_roar_SpellScript::HandleOnPrepare);
            AfterHit += SpellHitFn(spell_dru_savage_roar_SpellScript::HandleAfterHit);
        }
    };

    class spell_dru_savage_roar_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_savage_roar_AuraScript);

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->CastSpell(target, SPELL_DRUID_SAVAGE_ROAR, true, nullptr, aurEff, GetCasterGUID());
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_roar_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_savage_roar_AuraScript();
    }
};

enum SurvivalInstinctsSpells
{
    SPELL_DRUID_SURVIVAL_INSTINCTS = 50322
};

// 61336 Survival Instincts - 
// @Version : 7.1.0.22908
class spell_dru_survival_instincts : public SpellScriptLoader
{
public:
    spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

    class spell_dru_survival_instincts_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

        void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->CastSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, true);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_survival_instincts_AuraScript();
    }
};

// 40121 Swift Flight Form (Passive) - 
// @Version : 7.1.0.22908
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
public:
    spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

    class spell_dru_swift_flight_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* caster = GetCaster()->ToPlayer())
                if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                    amount = 310;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_swift_flight_passive_AuraScript();
    }
};

// Wild growth - 48438
// @Edit : Soul of the Forest - 114108, Tree of Life - 33891
// @Version : 7.1.0.22908
class spell_dru_wild_growth : public SpellScriptLoader
{
public:
    spell_dru_wild_growth() : SpellScriptLoader("spell_dru_wild_growth") { }

    class spell_dru_wild_growth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_growth_SpellScript);

        bool m_HasSoulOfTheForest = false;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            uint8 maxTargets = GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints + 1;
            if (!maxTargets)
                return;

            // If Tree of Life is activated we increase 2 targets
            if (caster->HasAura(SPELL_DRUID_INCARNATION_TREE_OF_LIFE))
                maxTargets += 2;

            if (targets.size() > maxTargets)
            {
                targets.sort(Trinity::HealthPctOrderPred());
                targets.resize(maxTargets);
            }
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            // If soul of the forest is activated we increase the heal by 50%
            if (AuraEffect* wildGrowth = target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0, caster->GetGUID()))
            {
                if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO) || m_HasSoulOfTheForest)
                {
                    uint8 SoulofTheForestBonus = GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints;
                    if (!SoulofTheForestBonus)
                        return;

                    wildGrowth->SetAmount(wildGrowth->GetAmount() + CalculatePct(wildGrowth->GetAmount(), SoulofTheForestBonus));
                    m_HasSoulOfTheForest = true;
                    caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                }
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_dru_wild_growth_SpellScript::HandleAfterHit);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_growth_SpellScript();
    }
};

// 102560 Incarnation : Chosen of Elune - 
// @Version : 7.1.0.22908
class spell_dru_incarnation_chosen_of_elune : public SpellScriptLoader
{
public:
    spell_dru_incarnation_chosen_of_elune() : SpellScriptLoader("spell_dru_incarnation_chosen_of_elune") { }

    class spell_dru_incarnation_chosen_of_elune_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_incarnation_chosen_of_elune_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->HasAura(SPELL_DRUID_MOONKIN_FORM))
                    player->CastSpell(player, SPELL_DRUID_MOONKIN_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_incarnation_chosen_of_elune_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_incarnation_chosen_of_elune_SpellScript();
    }
};

// 102543 Incarnation : King of the Jungle - 
// @Version : 7.1.0.22908
class spell_dru_incarnation_king_of_the_jungle : public SpellScriptLoader
{
public:
    spell_dru_incarnation_king_of_the_jungle() : SpellScriptLoader("spell_dru_incarnation_king_of_the_jungle") { }

    class spell_dru_incarnation_king_of_the_jungle_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_incarnation_king_of_the_jungle_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->HasAura(SPELL_DRUID_CAT_FORM))
                    player->CastSpell(player, SPELL_DRUID_CAT_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_incarnation_king_of_the_jungle_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_incarnation_king_of_the_jungle_SpellScript();
    }
};

// 102558 Incarnation : Guardian of Ursoc - 
// @Version : 7.1.0.22908
class spell_dru_incarnation_guardian_of_ursoc : public SpellScriptLoader
{
public:
    spell_dru_incarnation_guardian_of_ursoc() : SpellScriptLoader("spell_dru_incarnation_guardian_of_ursoc") { }

    class spell_dru_incarnation_guardian_of_ursoc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_incarnation_guardian_of_ursoc_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->HasAura(SPELL_DRUID_BEAR_FORM))
                    player->CastSpell(player, SPELL_DRUID_BEAR_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_incarnation_guardian_of_ursoc_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_incarnation_guardian_of_ursoc_SpellScript();
    }
};

// 102383 Wild Charge (Moonkin) - 
// @Version : 7.1.0.22908
class spell_dru_wild_charge_moonkin : public SpellScriptLoader
{
public:
    spell_dru_wild_charge_moonkin() : SpellScriptLoader("spell_dru_wild_charge_moonkin") { }

    class spell_dru_wild_charge_moonkin_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_charge_moonkin_SpellScript);

        SpellCastResult CheckFight()
        {
            if (GetCaster())
            {
                if (!GetCaster()->IsInCombat())
                    return SPELL_FAILED_DONT_REPORT;
            }
            else
                return SPELL_FAILED_DONT_REPORT;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_charge_moonkin_SpellScript::CheckFight);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_charge_moonkin_SpellScript();
    }
};

enum MoonfireSpells
{
    SPELL_DRUID_MOONFIRE_DAMAGE = 164812
};

// 8921 Moonfire - 
// @Version : 8.3.0
class spell_dru_moonfire : public SpellScriptLoader
{
public:
    spell_dru_moonfire() : SpellScriptLoader("spell_dru_moonfire") { }

    class spell_dru_moonfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_moonfire_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();

            int32 damage = GetHitDamage();

            if (caster != target)
                if (caster->CastSpell(target, SPELL_DRUID_MOONFIRE_DAMAGE, true))
                    AddPct(damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_MOONFIRE_DAMAGE)->GetEffect(EFFECT_0)->BasePoints);

            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_moonfire_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_moonfire_SpellScript();
    }
};

enum CatFormSpells
{
    SPELL_DRUID_DASH = 1850,
    SPELL_DRUID_PROWL = 5215,
    SPELL_DRUID_FORM_CAT_INCREASE_SPEED = 113636,
    SPELL_DRUID_CATFORM_OVERRIDE = 48629,
    SPELL_DRUID_LUNAR_INSPIRATION = 155580,
    SPELL_DRUID_MOONFIRE_CAT_OVERRIDE = 155627
};

enum DruidFlamesSpells
{
    SPELL_DRUID_DRUID_OF_THE_FLAMES = 99245,
    SPELL_DRUID_BURNING_ESSENCE = 138927,
    MODEL_DRUID_OF_THE_FLAMES = 38150
};

// Cat Form - 768
// @Called : Cat Form (Speed) - 113636, Cat Form (Thrash/Swipe) - 48629, Dash - 1850, Lunar Inspiration - 155580
// @Version : 7.1.0.22908
class spell_dru_cat_form : public SpellScriptLoader
{
public:
    spell_dru_cat_form() : SpellScriptLoader("spell_dru_cat_form") { }

    class spell_dru_cat_form_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_cat_form_SpellScript);

        void OnActivate()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (!player->HasAura(SPELL_DRUID_FORM_CAT_INCREASE_SPEED))
                {
                    player->CastSpell(player, SPELL_DRUID_FORM_CAT_INCREASE_SPEED, true);
                    player->RemoveMovementImpairingAuras();
                }
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_cat_form_SpellScript::OnActivate);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_cat_form_SpellScript();
    }

    class spell_dru_cat_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_cat_form_AuraScript);

        void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* dash = caster->GetAura(SPELL_DRUID_DASH))
                    if (dash->GetEffect(0))
                        if (dash->GetEffect(0)->GetAmount() == 0)
                            dash->GetEffect(0)->SetAmount(70);

                if (caster->HasAura(SPELL_DRUID_LUNAR_INSPIRATION))
                    caster->CastSpell(caster, SPELL_DRUID_MOONFIRE_CAT_OVERRIDE, true);

                caster->CastSpell(caster, SPELL_DRUID_CATFORM_OVERRIDE, true);
            }
        }

        void AfterApply(const AuraEffect* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* caster = GetCaster())
                // Check if the caster have Burning Essence or Druid of the Flames aura
                if (caster->HasAura(SPELL_DRUID_BURNING_ESSENCE) || caster->HasAura(SPELL_DRUID_DRUID_OF_THE_FLAMES))
                    // Change the caster model to Druid of the Flames (Fire Cat Form)
                    caster->SetDisplayId(MODEL_DRUID_OF_THE_FLAMES);
        }

        void OnRemove(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* dash = caster->GetAura(SPELL_DRUID_DASH))
                    dash->GetEffect(0)->SetAmount(0);

                if (caster->HasAura(SPELL_DRUID_PROWL))
                    caster->RemoveAura(SPELL_DRUID_PROWL);

                if (caster->HasAura(SPELL_DRUID_LUNAR_INSPIRATION))
                    caster->RemoveAura(SPELL_DRUID_MOONFIRE_CAT_OVERRIDE);

                //This is trigger for insane bug, making resto druid unplayable, will keep it commented
                //caster->RemoveAura(SPELL_DRUID_CATFORM_OVERRIDE);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_cat_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_cat_form_AuraScript();
    }
};

enum BearFormSpells
{
    SPELL_DRUID_BEARFORM_OVERRIDE = 106829,
    SPELL_DRUID_STAMPEDING_ROAR = 106898,
    SPELL_DRUID_STAMPEDING_ROAR_BEAR_OVERRIDE = 106899
};

// Bear form - 5487
// @Called : Bear Form (Thrash/Swipe) - 106829, Bear Form (Stampeding Roar) - 106899
// @Version : 7.1.0.22908
class spell_dru_bear_form : public SpellScriptLoader
{
public:
    spell_dru_bear_form() : SpellScriptLoader("spell_dru_bear_form") {}

    class spell_dru_bear_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bear_form_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_DRUID_BEARFORM_OVERRIDE, true);

            if (caster->HasSpell(SPELL_DRUID_STAMPEDING_ROAR))
                caster->CastSpell(caster, SPELL_DRUID_STAMPEDING_ROAR_BEAR_OVERRIDE, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->RemoveAurasDueToSpell(SPELL_DRUID_BEARFORM_OVERRIDE);

            if (caster->HasSpell(SPELL_DRUID_STAMPEDING_ROAR))
                caster->RemoveAurasDueToSpell(SPELL_DRUID_STAMPEDING_ROAR_BEAR_OVERRIDE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_bear_form_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_bear_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_bear_form_AuraScript();
    }
};

enum SkullBashSpells
{
    SPELL_DRUID_SKULL_BASH_CHARGE = 221514,
    SPELL_DRUID_SKULL_BASH_INTERUPT = 93985
};

// Skull Bash - 106839
// @Version : 7.1.0.22908
class spell_dru_skull_bash : public SpellScriptLoader
{
public:
    spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

    class spell_dru_skull_bash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_skull_bash_SpellScript);

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
            {
                GetCaster()->CastSpell(target, SPELL_DRUID_SKULL_BASH_CHARGE, true);
                GetCaster()->CastSpell(target, SPELL_DRUID_SKULL_BASH_INTERUPT, true);
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_skull_bash_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_skull_bash_SpellScript();
    }
};

// Stampeding Roar - 106898
// @Version : 7.1.0.22908
class spell_dru_stampeding_roar : public SpellScriptLoader
{
public:
    spell_dru_stampeding_roar() : SpellScriptLoader("spell_dru_stampeding_roar") { }

    class spell_dru_stampeding_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_stampeding_roar_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (!caster->HasAura(SPELL_DRUID_BEAR_FORM) && !caster->HasAura(SPELL_DRUID_CAT_FORM))
                caster->CastSpell(caster, SPELL_DRUID_BEAR_FORM, true);

            if (Unit* target = GetHitUnit())
                target->RemoveMovementImpairingAuras();
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_stampeding_roar_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_stampeding_roar_SpellScript();
    }
};

// Activate Cat Form
// @Called : Dash - 1850, Prowl - 5215, Displacer Beast - 102280
// @Version : 7.1.0.22908
class spell_dru_activate_cat_form : public SpellScriptLoader
{
public:
    spell_dru_activate_cat_form() : SpellScriptLoader("spell_dru_activate_cat_form") { }

    class spell_dru_activate_cat_form_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_activate_cat_form_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (!caster->HasAura(SPELL_DRUID_CAT_FORM))
                caster->CastSpell(caster, SPELL_DRUID_CAT_FORM, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_activate_cat_form_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_activate_cat_form_SpellScript();
    }
};

// Infected wound - 48484
// @Version : 7.1.0.22908
class spell_dru_infected_wound : public SpellScriptLoader
{
public:
    spell_dru_infected_wound() : SpellScriptLoader("spell_dru_infected_wound") { }

    class spell_dru_infected_wound_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_infected_wound_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_DRUID_RAKE)
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_infected_wound_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_infected_wound_AuraScript();
    }
};

// Ysera's Gift - 145108
class spell_dru_ysera_gift : public AuraScript
{
    PrepareAuraScript(spell_dru_ysera_gift);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        int64 amount = CalculatePct(caster->GetMaxHealth(), aurEff->GetBaseAmount());
        CustomSpellValues values;
        values.AddSpellMod(SPELLVALUE_MAX_TARGETS, 1);
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, amount);

        if (caster->IsFullHealth())
            caster->CastCustomSpell(SPELL_DRUID_YSERA_GIFT_RAID_HEAL, values, caster, TRIGGERED_FULL_MASK);
        else
            caster->CastCustomSpell(SPELL_DRUID_YSERA_GIFT_CASTER_HEAL, values, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_ysera_gift::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

enum RakeSpells
{
    SPELL_DRUID_RAKE_STUN = 163505
};

// Rake - 1822
class spell_dru_rake : public SpellScript
{
    PrepareSpellScript(spell_dru_rake);

    bool Load() override
    {
        Unit* caster = GetCaster();
        if (caster->HasAuraType(SPELL_AURA_MOD_STEALTH))
            m_stealthed = true;

        return true;
    }

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        if (!caster || !target)
            return;

        // While stealthed or have Incarnation: King of the Jungle aura, deal 100% increased damage
        if (m_stealthed || caster->HasAura(SPELL_DRUID_INCARNATION_KING_OF_JUNGLE))
            SetHitDamage(GetHitDamage() * 2);

        // Only stun if the caster was in stealth
        if (m_stealthed)
            caster->CastSpell(target, SPELL_DRUID_RAKE_STUN, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_rake::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

private:
    bool m_stealthed = false;
};

enum MaimSpells
{
    SPELL_DRUID_MAIM_STUN = 203123
};

// Maim - 22570
class spell_dru_maim : public SpellScript
{
    PrepareSpellScript(spell_dru_maim);

    void HandleTakePower(SpellPowerCost& powerCost)
    {
        if (powerCost.Power == POWER_COMBO_POINTS)
            _usedComboPoints = powerCost.Amount;
    }

    void AfterCast()
    {
        Unit* target = GetExplTargetUnit();
        if (!target)
            return;

        GetCaster()->CastSpell(target, SPELL_DRUID_MAIM_STUN, true);

        if (Aura* maimStun = target->GetAura(SPELL_DRUID_MAIM_STUN, GetCaster()->GetGUID()))
            maimStun->SetDuration(_usedComboPoints * 1000);
    }

    void Register() override
    {
        OnTakePower += SpellOnTakePowerFn(spell_dru_maim::HandleTakePower);
        AfterHit += SpellHitFn(spell_dru_maim::AfterCast);
    }

private:
    int32 _usedComboPoints = 0;
};

// Rip - 1079
// @Version : 7.1.0.22908
class spell_dru_rip : public SpellScriptLoader
{
public:
    spell_dru_rip() : SpellScriptLoader("spell_dru_rip") {}

    class spell_dru_rip_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rip_AuraScript);

        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            int8 _cp = caster->GetPower(POWER_COMBO_POINTS) + 1;
            int32 dmg = aurEff->GetDamage() * _cp;

            if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
            {
                aurEff->SetDamage(dmg);
                GetAura()->SetNeedClientUpdateForTargets();
            }

            caster->SetPower(POWER_COMBO_POINTS, 0);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_rip_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rip_AuraScript();
    }
};

enum BloodTalonsSpells
{
    SPELL_BLOODTALONS = 155672,
    SPELL_BLOODTALONS_AURA = 145152
};

// Bloodtalons - 155672
// @Called : Regrowth - 8936
// @Version : 7.1.0.22908
class spell_dru_bloodtalons : public SpellScriptLoader
{
public:
    spell_dru_bloodtalons() : SpellScriptLoader("spell_dru_bloodtalons") { }

    class spell_dru_bloodtalons_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_bloodtalons_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            if (caster)
                return;

            if (caster->HasAura(SPELL_BLOODTALONS))
                caster->CastSpell(caster, SPELL_BLOODTALONS_AURA, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_bloodtalons_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_bloodtalons_SpellScript();
    }
};

enum DruidForms
{
    DRUID_AQUATIC_FORM = 1066,
    DRUID_FLIGHT_FORM = 33943,
    DRUID_STAG_FORM = 165961,
    DRUID_SWIFT_FLIGHT_FORM = 40120
};

// 783 - Travel Form (dummy)
class spell_dru_travel_form_dummy : public SpellScriptLoader
{
public:
    spell_dru_travel_form_dummy() : SpellScriptLoader("spell_dru_travel_form_dummy") { }

    class spell_dru_travel_form_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_travel_form_dummy_SpellScript);

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SPELL_FAILED_CUSTOM_ERROR;

            if (player->GetSkillValue(SKILL_RIDING) < 75)
                return SPELL_FAILED_APPRENTICE_RIDING_REQUIREMENT;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(player->IsInWater() ? DRUID_AQUATIC_FORM : DRUID_STAG_FORM);
            return spellInfo->CheckLocation(player->GetMapId(), player->GetZoneId(), player->GetAreaId(), player);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_travel_form_dummy_SpellScript::CheckCast);
        }
    };

    class spell_dru_travel_form_dummy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_travel_form_dummy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ DRUID_STAG_FORM, DRUID_AQUATIC_FORM, DRUID_FLIGHT_FORM, DRUID_SWIFT_FLIGHT_FORM });
        }

        bool Load() override
        {
            return GetCaster()->IsPlayer();
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            uint32 triggeredSpellId;

            Player* player = GetTarget()->ToPlayer();
            if (player->IsInWater()) // Aquatic form
                triggeredSpellId = DRUID_AQUATIC_FORM;
            else if (player->GetSkillValue(SKILL_RIDING) >= 225 && CheckLocationForForm(DRUID_FLIGHT_FORM) == SPELL_CAST_OK) // Flight form
                triggeredSpellId = player->getLevel() >= 71 ? DRUID_SWIFT_FLIGHT_FORM : DRUID_FLIGHT_FORM;
            else // Stag form (riding skill already checked in CheckCast)
                triggeredSpellId = DRUID_STAG_FORM;

            player->AddAura(triggeredSpellId, player);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // No need to check remove mode, it's safe for auras to remove each other in AfterRemove hook.
            GetTarget()->RemoveAura(DRUID_STAG_FORM);
            GetTarget()->RemoveAura(DRUID_AQUATIC_FORM);
            GetTarget()->RemoveAura(DRUID_FLIGHT_FORM);
            GetTarget()->RemoveAura(DRUID_SWIFT_FLIGHT_FORM);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_travel_form_dummy_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_dummy_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

    private:
        // Outdoor check already passed - Travel Form (dummy) has SPELL_ATTR0_OUTDOORS_ONLY attribute.
        SpellCastResult CheckLocationForForm(uint32 spell)
        {
            Player* player = GetTarget()->ToPlayer();
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
            return spellInfo->CheckLocation(player->GetMapId(), player->GetZoneId(), player->GetAreaId(), player);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_travel_form_dummy_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_travel_form_dummy_AuraScript();
    }
};

// 1066 - Aquatic Form
// 33943 - Flight Form
// 40120 - Swift Flight Form
// 165961 - Stag Form
class spell_dru_travel_form : public SpellScriptLoader
{
public:
    spell_dru_travel_form() : SpellScriptLoader("spell_dru_travel_form") { }

    class spell_dru_travel_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_travel_form_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ DRUID_STAG_FORM, DRUID_AQUATIC_FORM, DRUID_FLIGHT_FORM, DRUID_SWIFT_FLIGHT_FORM });
        }

        bool Load() override
        {
            return GetCaster()->IsPlayer();
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // If it stays 0, it removes Travel Form dummy in AfterRemove.
            triggeredSpellId = 0;

            // We should only handle aura interrupts.
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_INTERRUPT)
                return;

            // Check what form is appropriate
            Player* player = GetTarget()->ToPlayer();
            if (player->IsInWater()) // Aquatic form
                triggeredSpellId = DRUID_AQUATIC_FORM;
            else if (player->GetSkillValue(SKILL_RIDING) >= 225 && CheckLocationForForm(DRUID_FLIGHT_FORM) == SPELL_CAST_OK) // Flight form
                triggeredSpellId = player->GetSkillValue(SKILL_RIDING) >= 300 ? DRUID_SWIFT_FLIGHT_FORM : DRUID_FLIGHT_FORM;
            else if (CheckLocationForForm(DRUID_STAG_FORM) == SPELL_CAST_OK) // Stag form
                triggeredSpellId = DRUID_STAG_FORM;

            // If chosen form is current aura, just don't remove it.
            if (triggeredSpellId == m_scriptSpellId)
                PreventDefaultAction();
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget()->ToPlayer();

            if (triggeredSpellId) // Apply new form
                player->AddAura(triggeredSpellId, player);
            else // If not set, simply remove Travel Form dummy
                player->RemoveAura(SPELL_DRUID_TRAVEL_FORM);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }

    private:
        SpellCastResult CheckLocationForForm(uint32 spell_id)
        {
            Player* player = GetTarget()->ToPlayer();
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id);

            if (!player->GetMap()->IsOutdoors(player->GetPhaseShift(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()))
                return SPELL_FAILED_ONLY_OUTDOORS;

            return spellInfo->CheckLocation(player->GetMapId(), player->GetZoneId(), player->GetAreaId(), player);
        }

        uint32 triggeredSpellId;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_travel_form_AuraScript();
    }
};

// 24858  Moonkin Form
// 102560 Chosen of Elune
class aura_dru_astral_form : public AuraScript
{
    PrepareAuraScript(aura_dru_astral_form);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_GLYPH_OF_STARS });
    }

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->HasAura(SPELL_DRUID_GLYPH_OF_STARS))
        {
            target->SetDisplayId(target->GetNativeDisplayId());
            target->AddAura(SPELL_DRUID_BLUE_COLOR, target);
            target->AddAura(SPELL_DRUID_SHADOWY_GHOST, target);
            target->CastSpell(target, sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_STARS)->GetEffect(EFFECT_0)->BasePoints, true);
        }
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->HasAura(SPELL_DRUID_MOONKIN_FORM) || target->HasAura(SPELL_DRUID_CHOSEN_OF_ELUNE))
            return;

        target->RemoveAura(sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_STARS)->GetEffect(EFFECT_0)->BasePoints);
        target->RemoveAura(SPELL_DRUID_BLUE_COLOR);
        target->RemoveAura(SPELL_DRUID_SHADOWY_GHOST);
    }

    void Register() override
    {
        switch (m_scriptSpellId)
        {
        case SPELL_DRUID_MOONKIN_FORM:
            AfterEffectApply += AuraEffectApplyFn(aura_dru_astral_form::AfterApply, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(aura_dru_astral_form::AfterRemove, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            break;
        case SPELL_DRUID_CHOSEN_OF_ELUNE:
            AfterEffectApply += AuraEffectApplyFn(aura_dru_astral_form::AfterApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(aura_dru_astral_form::AfterRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            break;
        default:
            break;
        }
    }
};

// 197492 - Restoration Affinity
class aura_dru_restoration_affinity : public AuraScript
{
    PrepareAuraScript(aura_dru_restoration_affinity);

    const std::vector<uint32> LearnedSpells =
    {
        SPELL_DRUID_YSERA_GIFT,
        SPELL_DRUID_REJUVENATION,
        SPELL_DRUID_HEALING_TOUCH,
        SPELL_DRUID_SWIFTMEND
    };

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->LearnSpell(spellId, false);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->RemoveSpell(spellId);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_dru_restoration_affinity::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_dru_restoration_affinity::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 202157 - Feral Affinity
class aura_dru_feral_affinity : public AuraScript
{
    PrepareAuraScript(aura_dru_feral_affinity);

    const std::vector<uint32> LearnedSpells =
    {
        SPELL_DRUID_FELINE_SWIFTNESS,
        SPELL_DRUID_SHRED,
        SPELL_DRUID_RAKE,
        SPELL_DRUID_RIP,
        SPELL_DRUID_FEROCIOUS_BITE,
        SPELL_DRUID_SWIPE_CAT
    };

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->LearnSpell(spellId, false);
    }

    void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            for (uint32 spellId : LearnedSpells)
                target->RemoveSpell(spellId);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_dru_feral_affinity::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(aura_dru_feral_affinity::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 22842 - Frenzied Regeneration
class aura_dru_frenzied_regeneration : public AuraScript
{
    PrepareAuraScript(aura_dru_frenzied_regeneration);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Aura* frenzied = GetCaster()->GetAura(22842))
            frenzied->GetMaxDuration();
        uint64 healAmount = CalculatePct(GetCaster()->GetDamageOverLastSeconds(5), 50);
        uint64 minHealAmount = CalculatePct(GetCaster()->GetMaxHealth(), 5);
        healAmount = std::max(healAmount, minHealAmount);
        amount = (int32)healAmount;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(aura_dru_frenzied_regeneration::CalculateAmount, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH);
    }
};

enum StarfallSpells
{
    SPELL_DRUID_STARFALL_DAMAGE = 191037,
    SPELL_DRUID_STELLAR_EMPOWERMENT = 197637
};

// Starfall - 191034
// AreaTriggerID - 9482
struct at_dru_starfall : AreaTriggerAI
{
    int32 timeInterval;

    at_dru_starfall(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
    {
        // How often should the action be executed
        areatrigger->SetPeriodicProcTimer(850);
    }

    void OnPeriodicProc() override
    {
        if (Unit* caster = at->GetCaster())
            for (ObjectGuid objguid : at->GetInsideUnits())
                if (Unit* unit = ObjectAccessor::GetUnit(*caster, objguid))
                    if (caster->IsValidAttackTarget(unit))
                    {
                        caster->CastSpell(unit, SPELL_DRUID_STARFALL_DAMAGE, true);
                        caster->CastSpell(unit, SPELL_DRUID_STELLAR_EMPOWERMENT, true);
                    }
    }
};



enum UrsolsVortexSpells
{
    SPELL_DRUID_URSOLS_VORTEX_SLOW = 127797,
    SPELL_DRUID_URSOLS_VORTEX_PULL = 118283
};



// Efflorescence
// NPC Id - 47649
class npc_dru_efflorescence : public ScriptedAI
{
public:
    npc_dru_efflorescence(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        me->CastSpell(me, SPELL_DRUID_EFFLORESCENCE_DUMMY, true);
        me->AddUnitFlag(UnitFlags(UNIT_FLAG_NON_ATTACKABLE));
        me->AddUnitFlag(UnitFlags(UNIT_FLAG_NOT_SELECTABLE));
        me->AddUnitFlag(UnitFlags(UNIT_FLAG_REMOVE_CLIENT_CONTROL));
        me->SetReactState(REACT_PASSIVE);
    }
};

// Charm Woodland Creature - 127757
class aura_dru_charm_woodland_creature : public AuraScript
{
    PrepareAuraScript(aura_dru_charm_woodland_creature);

    void OnApply(const AuraEffect* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        // Make targeted creature follow the player - Using pet's default dist and angle
        if (Unit* caster = GetCaster())
            if (Unit* target = GetTarget())
                target->GetMotionMaster()->MoveFollow(caster, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    void OnRemove(const AuraEffect* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* target = GetTarget())
            if (target->GetMotionMaster()->GetCurrentMovementGeneratorType() == FOLLOW_MOTION_TYPE)
                target->GetMotionMaster()->MovementExpired(true); // reset movement
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_dru_charm_woodland_creature::OnApply, EFFECT_0, SPELL_AURA_AOE_CHARM, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(aura_dru_charm_woodland_creature::OnRemove, EFFECT_0, SPELL_AURA_AOE_CHARM, AURA_EFFECT_HANDLE_REAL);
    }
};

// Swipe - 106785
class spell_dru_swipe : public SpellScript
{
    PrepareSpellScript(spell_dru_swipe);

    void HandleOnHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        int32 damage = GetHitDamage();
        int32 casterLevel = caster->GetLevelForTarget(caster);

        // This prevent awarding multiple Combo Points when multiple targets hit with Swipe AoE
        if (m_awardComboPoint)
            // Awards the caster 1 Combo Point (get value from the spell data)
            caster->ModifyPower(POWER_COMBO_POINTS, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE_CAT)->GetEffect(EFFECT_0)->BasePoints);

        // If caster is level >= 44 and the target is bleeding, deals 20% increased damage (get value from the spell data)
        if ((casterLevel >= 44) && target->HasAuraState(AURA_STATE_BLEEDING))
            AddPct(damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE_CAT)->GetEffect(EFFECT_1)->BasePoints);

        SetHitDamage(damage);

        m_awardComboPoint = false;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_swipe::HandleOnHit, EFFECT_1, SPELL_EFFECT_DUMMY);
    }

private:
    bool m_awardComboPoint = true;
};

// Brutal Slash - 202028
class spell_dru_brutal_slash : public SpellScript
{
    PrepareSpellScript(spell_dru_brutal_slash);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        // This prevent awarding multiple Combo Points when multiple targets hit with Brutal Slash AoE
        if (m_awardComboPoint)
            // Awards the caster 1 Combo Point (get value from the spell data)
            caster->ModifyPower(POWER_COMBO_POINTS, sSpellMgr->GetSpellInfo(SPELL_DRUID_SWIPE_CAT)->GetEffect(EFFECT_0)->BasePoints);

        m_awardComboPoint = false;
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dru_brutal_slash::HandleOnHit);
    }

private:
    bool m_awardComboPoint = true;
};

// Thrash (Cat Form) - 106830
class spell_dru_thrash_cat : public SpellScript
{
    PrepareSpellScript(spell_dru_thrash_cat);

    void HandleOnEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        // This prevent awarding multiple Combo Points when multiple targets hit with Thrash AoE
        if (m_awardComboPoint)
            // Awards the caster 1 Combo Point
            caster->ModifyPower(POWER_COMBO_POINTS, 1);

        m_awardComboPoint = false;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_thrash_cat::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

private:
    bool m_awardComboPoint = true;
};

// Shred - 5221
class spell_dru_shred : public SpellScript
{
    PrepareSpellScript(spell_dru_shred);

    bool Load() override
    {
        Unit* caster = GetCaster();

        if (caster->HasAuraType(SPELL_AURA_MOD_STEALTH))
            m_stealthed = true;

        if (caster->HasAura(SPELL_DRUID_INCARNATION_KING_OF_JUNGLE))
            m_incarnation = true;

        m_casterLevel = caster->GetLevelForTarget(caster);

        return true;
    }

    void HandleCritChance(Unit* /*victim*/, float& chance)
    {
        // If caster is level >= 56, While stealthed or have Incarnation: King of the Jungle aura,
        // Double the chance to critically strike
        if ((m_casterLevel >= 56) && (m_stealthed || m_incarnation))
            chance *= 2.0f;
    }

    void HandleOnHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        int32 damage = GetHitDamage();
        
        caster->ModifyPower(POWER_COMBO_POINTS, 1);

        // If caster is level >= 56, While stealthed or have Incarnation: King of the Jungle aura,
        // deals 50% increased damage (get value from the spell data)
        if ((caster->HasAura(231057)) && (m_stealthed || m_incarnation))
            AddPct(damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_SHRED)->GetEffect(EFFECT_2)->BasePoints);

        // If caster is level >= 44 and the target is bleeding, deals 20% increased damage (get value from the spell data)
        if (caster->HasAura(231063) && target->HasAuraState(AURA_STATE_BLEEDING))
            AddPct(damage, sSpellMgr->GetSpellInfo(SPELL_DRUID_SHRED)->GetEffect(EFFECT_3)->BasePoints);

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnCalcCritChance += SpellOnCalcCritChanceFn(spell_dru_shred::HandleCritChance);
        OnHit += SpellHitFn(spell_dru_shred::HandleOnHit);
    }

private:
    bool m_stealthed = false;
    bool m_incarnation = false;
    int32 m_casterLevel;
};

// Sunfire DOT - 164815, Moonfire DOT - 164812
class spell_dru_moonfire_sunfire_dot : public SpellScriptLoader
{
public:
    spell_dru_moonfire_sunfire_dot() : SpellScriptLoader("spell_dru_moonfire_sunfire_dot") { }

    class spell_dru_moonfire_sunfire_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_moonfire_sunfire_dot_AuraScript);

        enum Spells {
            SPELL_DRU_SUNFIRE_DOT = 164815,
            SPELL_DRU_MOONFIRE_DOT = 164812,
            SPELL_DRU_SHOOTING_STARS = 202342,
            SPELL_DRU_SHOOTING_STARS_TRIGGERED = 202497,
        };

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_DRU_SUNFIRE_DOT,
                    SPELL_DRU_MOONFIRE_DOT,
                    SPELL_DRU_SHOOTING_STARS,
                    SPELL_DRU_SHOOTING_STARS_TRIGGERED,
                });
        }

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            SpellInfo const* shootingStars = sSpellMgr->AssertSpellInfo(SPELL_DRU_SHOOTING_STARS);

            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_DRU_SHOOTING_STARS))
                    if (roll_chance_i(shootingStars->GetEffect(EFFECT_0)->BasePoints))
                        caster->CastSpell(GetTarget(), SPELL_DRU_SHOOTING_STARS_TRIGGERED, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_moonfire_sunfire_dot_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_moonfire_sunfire_dot_AuraScript();
    }
};

// 8936 - Regrowth
class spell_dru_regrowth : public SpellScript
{
    PrepareSpellScript(spell_dru_regrowth);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
         {
            SPELL_DRU_REGROWTH,
            SPELL_DRU_BLOODTALONS,
            SPELL_DRU_BLOODTALONS_TRIGGERED,
            SPELL_DRU_MOMENT_OF_CLARITY,
            SPELL_DRU_CLEARCASTING,
         });
    }

    void HandleHealEffect(SpellEffIndex /*effIndex*/)
    {
        if (GetCaster()->HasAura(SPELL_DRU_BLOODTALONS))
            GetCaster()->AddAura(SPELL_DRU_BLOODTALONS_TRIGGERED, GetCaster());

        if (Aura* clearcasting = GetCaster()->GetAura(SPELL_DRU_CLEARCASTING))
            if (GetCaster()->HasAura(SPELL_DRU_MOMENT_OF_CLARITY))
            {
                int32 amount = clearcasting->GetEffect(EFFECT_0)->GetAmount();
                clearcasting->GetEffect(EFFECT_0)->SetAmount(amount - 1);
                if (amount == -102)
                    GetCaster()->RemoveAurasDueToSpell(SPELL_DRU_CLEARCASTING);
            }
            else
                GetCaster()->RemoveAurasDueToSpell(SPELL_DRU_CLEARCASTING);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dru_regrowth::HandleHealEffect, EFFECT_0, SPELL_EFFECT_HEAL);
    }
};

// 197721 - Flourish
class spell_dru_flourish : public SpellScriptLoader
{
public:
    spell_dru_flourish() : SpellScriptLoader("spell_dru_flourish") { }


    class spell_dru_flourish_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_flourish_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            std::list<AuraEffect*> const& auraEffects = GetHitUnit()->GetAuraEffectsByType(SPELL_AURA_PERIODIC_HEAL);

            for (AuraEffect* auraEffect : auraEffects)
                if (auraEffect->GetCasterGUID() == GetCaster()->GetGUID())
                    if (Aura* healAura = auraEffect->GetBase())
                        healAura->SetDuration(healAura->GetDuration() + GetEffectValue() * IN_MILLISECONDS);
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<WorldObject*> tempTargets;
            for (WorldObject* target : targets)
            {
                if (target->IsPlayer())
                    if (target->ToUnit()->HasAuraTypeWithCaster(SPELL_AURA_PERIODIC_HEAL, GetCaster()->GetGUID()))
                        tempTargets.push_back(target);
            }

            if (!tempTargets.empty())
            {
                targets.clear();
                for (WorldObject* target : tempTargets)
                    targets.push_back(target);

            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_flourish_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_flourish_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_flourish_SpellScript();
    }
};

// 18562 - swiftmend
class spell_dru_swiftmend : public SpellScriptLoader
{
public:
    spell_dru_swiftmend() : SpellScriptLoader("spell_dru_swiftmend") { }


    class spell_dru_swiftmend_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_swiftmend_SpellScript);

        enum Spells
        {
            SPELL_DRUID_SOUL_OF_THE_FOREST = 158478,
            SPELL_DRUID_SOUL_OF_THE_FOREST_TRIGGERED = 114108,
        };


        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST))
                    caster->AddAura(SPELL_DRUID_SOUL_OF_THE_FOREST_TRIGGERED, caster);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_swiftmend_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_swiftmend_SpellScript();
    }
};

// 102793 - Ursol's Vortex
class spell_dru_ursols_vortex : public SpellScriptLoader
{
public:
    spell_dru_ursols_vortex() : SpellScriptLoader("spell_dru_ursols_vortex") { }


    class spell_dru_ursols_vortex_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_ursols_vortex_SpellScript);

        enum Spells
        {
            SPELL_DRUID_URSOLS_VORTEX_SLOW = 127797,
        };


        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                caster->AddAura(SPELL_DRUID_URSOLS_VORTEX_SLOW, GetHitUnit());
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_ursols_vortex_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_ursols_vortex_SpellScript();
    }
};

// 203953 - Brambles
class spell_dru_brambles : public SpellScriptLoader
{
public:
    spell_dru_brambles() : SpellScriptLoader("spell_dru_brambles") { }

    class spell_dru_brambles_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_brambles_AuraScript);

        enum Spells
        {
            SPELL_DRUID_BRAMBLES_DAMAGE = 203958,
        };

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = std::min(uint32(CalculatePct(GetTarget()->GetTotalAttackPowerValue(BASE_ATTACK), 24)), dmgInfo.GetDamage());
            if (dmgInfo.GetAttacker())
                GetCaster()->CastCustomSpell(SPELL_DRUID_BRAMBLES_DAMAGE, SPELLVALUE_BASE_POINT0, absorbAmount, dmgInfo.GetAttacker(), true);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_brambles_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_brambles_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_brambles_AuraScript();
    }
};

// 22812 - Barkskin
class spell_dru_barkskin : public SpellScriptLoader
{
public:
    spell_dru_barkskin() : SpellScriptLoader("spell_dru_barkskin") { }

    class spell_dru_barkskin_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_barkskin_AuraScript);

        enum Spells {
            SPELL_DRUID_BRAMBLES = 203953,
            SPELL_DRUID_BRAMBLES_BARSKIN_DAMAGE = 213709,
        };

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_DRUID_BRAMBLES,
                    SPELL_DRUID_BRAMBLES_BARSKIN_DAMAGE,
                });
        }

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_DRUID_BRAMBLES))
                    caster->CastSpell(caster, SPELL_DRUID_BRAMBLES_BARSKIN_DAMAGE, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_barkskin_AuraScript::OnTick, EFFECT_4, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_barkskin_AuraScript();
    }
};

// 102351 - Cenarion Ward
class spell_dru_cenarion_ward : public SpellScriptLoader
{
public:
    spell_dru_cenarion_ward() : SpellScriptLoader("spell_dru_cenarion_ward") { }

    class spell_dru_cenarion_ward_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_cenarion_ward_AuraScript);

        enum Spells
        {
            SPELL_DRUID_CENARION_WARD_TRIGGERED = 102352,
        };

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_CENARION_WARD_TRIGGERED });
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!GetCaster() || !eventInfo.GetActionTarget())
                return;

            GetCaster()->CastSpell(eventInfo.GetActionTarget(), SPELL_DRUID_CENARION_WARD_TRIGGERED, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_cenarion_ward_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_cenarion_ward_AuraScript();
    }
};

// 102352 - Cenarion Ward
class spell_dru_cenarion_ward_hot : public SpellScriptLoader
{
public:
    spell_dru_cenarion_ward_hot() : SpellScriptLoader("spell_dru_cenarion_ward_hot") { }

    class spell_dru_cenarion_ward_hot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_cenarion_ward_hot_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster())
                return;

            amount = CalculatePct(GetCaster()->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_NATURE), 220) / 4;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_cenarion_ward_hot_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_cenarion_ward_hot_AuraScript();
    }
};

// 203964  - Galactic Guardian
class spell_dru_galactic_guardian : public SpellScriptLoader
{
public:
    spell_dru_galactic_guardian() : SpellScriptLoader("spell_dru_galactic_guardian") { }

    class spell_dru_galactic_guardian_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_galactic_guardian_AuraScript);

        enum Spells
        {
            SPELL_DRUID_GALACTIC_GUARDIAN_TRIGGERED = 213708,
        };

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_GALACTIC_GUARDIAN_TRIGGERED });
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!GetCaster() || !eventInfo.GetActionTarget())
                return;

            GetCaster()->CastSpell(eventInfo.GetActionTarget(), SPELL_DRUID_GALACTIC_GUARDIAN_TRIGGERED, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_galactic_guardian_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_galactic_guardian_AuraScript();
    }
};

// 155835 - Bristling Fur - CRASH!
/*class spell_dru_bristling_fur : public SpellScriptLoader
{
public:
    spell_dru_bristling_fur() : SpellScriptLoader("spell_dru_bristling_fur") { }

    class spell_dru_bristling_fur_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bristling_fur_AuraScript);

        enum Spells
        {
            SPELL_DRUID_BRISTLING_FUR_TRIGGERED = 204031,
        };

        bool Validate(SpellInfo const*) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_BRISTLING_FUR_TRIGGERED });
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!GetCaster() || !eventInfo.GetActionTarget())
                return;
            int32 bp = 100 * eventInfo.GetDamageInfo()->GetDamage() / GetCaster()->GetMaxHealth();
            GetCaster()->CastCustomSpell(SPELL_DRUID_BRISTLING_FUR_TRIGGERED, SPELLVALUE_BASE_POINT0, bp, eventInfo.GetActionTarget());
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_bristling_fur_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_bristling_fur_AuraScript();
    }
};*/

// 37336 - Druid Forms Trinket
class spell_dru_forms_trinket : public SpellScriptLoader
{
public:
    spell_dru_forms_trinket() : SpellScriptLoader("spell_dru_forms_trinket") { }

    class spell_dru_forms_trinket_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_forms_trinket_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_DRUID_FORMS_TRINKET_BEAR,
                    SPELL_DRUID_FORMS_TRINKET_CAT,
                    SPELL_DRUID_FORMS_TRINKET_MOONKIN,
                    SPELL_DRUID_FORMS_TRINKET_NONE,
                    SPELL_DRUID_FORMS_TRINKET_TREE
                });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActor();

            switch (target->GetShapeshiftForm())
            {
            case FORM_BEAR_FORM:
            case FORM_DIRE_BEAR_FORM:
            case FORM_CAT_FORM:
            case FORM_MOONKIN_FORM:
            case FORM_NONE:
            case FORM_TREE_OF_LIFE:
                return true;
            default:
                break;
            }

            return false;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* target = eventInfo.GetActor();
            uint32 triggerspell = 0;

            switch (target->GetShapeshiftForm())
            {
            case FORM_BEAR_FORM:
            case FORM_DIRE_BEAR_FORM:
                triggerspell = SPELL_DRUID_FORMS_TRINKET_BEAR;
                break;
            case FORM_CAT_FORM:
                triggerspell = SPELL_DRUID_FORMS_TRINKET_CAT;
                break;
            case FORM_MOONKIN_FORM:
                triggerspell = SPELL_DRUID_FORMS_TRINKET_MOONKIN;
                break;
            case FORM_NONE:
                triggerspell = SPELL_DRUID_FORMS_TRINKET_NONE;
                break;
            case FORM_TREE_OF_LIFE:
                triggerspell = SPELL_DRUID_FORMS_TRINKET_TREE;
                break;
            default:
                return;
            }

            target->CastSpell(target, triggerspell, true, nullptr, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_forms_trinket_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_forms_trinket_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_forms_trinket_AuraScript();
    }
};

// 54845 - Glyph of Starfire
class spell_dru_glyph_of_starfire_proc : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_starfire_proc() : SpellScriptLoader("spell_dru_glyph_of_starfire_proc") { }

    class spell_dru_glyph_of_starfire_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_starfire_proc_AuraScript);

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_GLYPH_OF_STARFIRE, true, nullptr, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_starfire_proc_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_starfire_proc_AuraScript();
    }
};

// 61391 - Typhoon
class spell_dru_typhoon : public SpellScriptLoader
{
public:
    spell_dru_typhoon() : SpellScriptLoader("spell_dru_typhoon") { }

    class spell_dru_typhoon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_typhoon_SpellScript);

        void HandleKnockBack(SpellEffIndex effIndex)
        {
            // Glyph of Typhoon
            if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_TYPHOON))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_typhoon_SpellScript::HandleKnockBack, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_typhoon_SpellScript();
    }
};

// 28744 - Regrowth
class spell_dru_t3_6p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t3_6p_bonus() : SpellScriptLoader("spell_dru_t3_6p_bonus") { }

    class spell_dru_t3_6p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t3_6p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_BLESSING_OF_THE_CLAW });
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            eventInfo.GetActor()->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_BLESSING_OF_THE_CLAW, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_t3_6p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t3_6p_bonus_AuraScript();
    }
};

// 28719 - Healing Touch
class spell_dru_t3_8p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t3_8p_bonus() : SpellScriptLoader("spell_dru_t3_8p_bonus") { }

    class spell_dru_t3_8p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t3_8p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_EXHILARATE });
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Spell const* spell = eventInfo.GetProcSpell();
            if (!spell)
                return;

            Unit* caster = eventInfo.GetActor();
            std::vector<SpellPowerCost> const& costs = spell->GetPowerCost();
            auto m = std::find_if(costs.begin(), costs.end(), [](SpellPowerCost const& cost) { return cost.Power == POWER_MANA; });
            if (m == costs.end())
                return;

            int32 amount = CalculatePct(m->Amount, aurEff->GetAmount());
            caster->CastCustomSpell(SPELL_DRUID_EXHILARATE, SPELLVALUE_BASE_POINT0, amount, (Unit*)nullptr, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_t3_8p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t3_8p_bonus_AuraScript();
    }
};

// 37288 - Mana Restore
// 37295 - Mana Restore
class spell_dru_t4_2p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t4_2p_bonus() : SpellScriptLoader("spell_dru_t4_2p_bonus") { }

    class spell_dru_t4_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t4_2p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_INFUSION });
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            eventInfo.GetActor()->CastSpell((Unit*)nullptr, SPELL_DRUID_INFUSION, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_t4_2p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t4_2p_bonus_AuraScript();
    }
};

// 40442 - Druid Tier 6 Trinket
class spell_dru_item_t6_trinket : public SpellScriptLoader
{
public:
    spell_dru_item_t6_trinket() : SpellScriptLoader("spell_dru_item_t6_trinket") { }

    class spell_dru_item_t6_trinket_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_item_t6_trinket_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({
                SPELL_DRUID_BLESSING_OF_REMULOS,
                SPELL_DRUID_BLESSING_OF_ELUNE_40446,
                SPELL_DRUID_BLESSING_OF_CENARIUS
                });
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return;

            uint32 spellId;
            int32 chance;

            // Starfire
            if (spellInfo->SpellFamilyFlags[0] & 0x00000004)
            {
                spellId = SPELL_DRUID_BLESSING_OF_REMULOS;
                chance = 25;
            }
            // Rejuvenation
            else if (spellInfo->SpellFamilyFlags[0] & 0x00000010)
            {
                spellId = SPELL_DRUID_BLESSING_OF_ELUNE_40446;
                chance = 25;
            }
            // Mangle (Bear) and Mangle (Cat)
            else if (spellInfo->SpellFamilyFlags[1] & 0x00000440)
            {
                spellId = SPELL_DRUID_BLESSING_OF_CENARIUS;
                chance = 40;
            }
            else
                return;

            if (roll_chance_i(chance))
                eventInfo.GetActor()->CastSpell((Unit*)nullptr, spellId, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_item_t6_trinket_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_item_t6_trinket_AuraScript();
    }
};

// 70723 - Item - Druid T10 Balance 4P Bonus
class spell_dru_t10_balance_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t10_balance_4p_bonus() : SpellScriptLoader("spell_dru_t10_balance_4p_bonus") { }

    class spell_dru_t10_balance_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t10_balance_4p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_LANGUISH });
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo || !damageInfo->GetDamage())
                return;

            Unit* caster = eventInfo.GetActor();
            Unit* target = eventInfo.GetProcTarget();

            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(SPELL_DRUID_LANGUISH);
            int32 amount = CalculatePct(static_cast<int32>(damageInfo->GetDamage()), aurEff->GetAmount());
            amount /= spellInfo->GetMaxTicks(DIFFICULTY_NONE);
            // Add remaining ticks to damage done
            amount += target->GetRemainingPeriodicAmount(caster->GetGUID(), SPELL_DRUID_LANGUISH, SPELL_AURA_PERIODIC_DAMAGE);

            caster->CastCustomSpell(SPELL_DRUID_LANGUISH, SPELLVALUE_BASE_POINT0, amount, target, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_t10_balance_4p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t10_balance_4p_bonus_AuraScript();
    }
};

// 70691 - Item T10 Restoration 4P Bonus
class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

    class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster()->ToPlayer()->GetGroup())
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
            else
            {
                targets.remove(GetExplTargetUnit());
                std::list<Unit*> tempTargets;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->GetTypeId() == TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                        tempTargets.push_back((*itr)->ToUnit());

                if (tempTargets.empty())
                {
                    targets.clear();
                    FinishCast(SPELL_FAILED_DONT_REPORT);
                    return;
                }

                Unit* target = Trinity::Containers::SelectRandomContainerElement(tempTargets);
                targets.clear();
                targets.push_back(target);
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_t10_restoration_4p_bonus_SpellScript();
    }
};

// 70664 - Druid T10 Restoration 4P Bonus (Rejuvenation)
class spell_dru_t10_restoration_4p_bonus_dummy : public SpellScriptLoader
{
public:
    spell_dru_t10_restoration_4p_bonus_dummy() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus_dummy") { }

    class spell_dru_t10_restoration_4p_bonus_dummy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t10_restoration_4p_bonus_dummy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_REJUVENATION_T10_PROC });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || spellInfo->Id == SPELL_DRUID_REJUVENATION_T10_PROC)
                return false;

            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!healInfo || !healInfo->GetHeal())
                return false;

            Player* caster = eventInfo.GetActor()->ToPlayer();
            if (!caster)
                return false;

            return caster->GetGroup() || caster != eventInfo.GetProcTarget();
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 amount = static_cast<int32>(eventInfo.GetHealInfo()->GetHeal());
            eventInfo.GetActor()->CastCustomSpell(SPELL_DRUID_REJUVENATION_T10_PROC, SPELLVALUE_BASE_POINT0, amount, (Unit*)nullptr, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_t10_restoration_4p_bonus_dummy_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_t10_restoration_4p_bonus_dummy_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t10_restoration_4p_bonus_dummy_AuraScript();
    }
};

// 5176 - Wrath, 2912 - Starfire and 78674 - Starsurge
class spell_dru_eclipse : public SpellScriptLoader
{
public:
    spell_dru_eclipse() : SpellScriptLoader("spell_dru_eclipse") { }

    class spell_dru_eclipse_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_eclipse_SpellScript);

        void HandleAfterHit()
        {
            if (Player* _plr = GetCaster()->ToPlayer())
            {
                if (GetSpellInfo()->Id == SPELL_DRUID_STARSURGE)
                {
                    int32 casttime = _plr->GetCurrentSpellCastTime(SPELL_DRUID_STARSURGE);
                    if (casttime != 0)
                    {
                        // If caster has Shooting Stars we need to reset a cooldown
                        if (_plr->HasAura(SPELL_DRUID_SHOOTING_STARS))
                            _plr->GetSpellHistory()->ResetCooldown(SPELL_DRUID_STARSURGE);
                    }
                }
            }
        }

        void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if ((!_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) || !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE)))
                    {
                        switch (GetSpellInfo()->Id)
                        {
                        case SPELL_DRUID_WRATH:
                        {
                            int32 eclipse = 15; // 15 Lunar energy

                                                // The last eclipse was Lunar, so wrath shouldn't give energy
                            if (_player->GetLastEclipsePower() == SPELL_DRUID_LUNAR_ECLIPSE)
                                eclipse = 0;

                            if (_player->HasAura(SPELL_DRUID_EUPHORIA) && !_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                eclipse *= 2;

                            _player->SetEclipsePower(int32(_player->GetEclipsePower() - eclipse));

                            // Your crits with Wrath also increase sunfire duration by 2s
                            if (GetSpell()->IsCritForTarget(target))
                            {
                                if (Aura* aura = target->GetAura(SPELL_DRUID_SUNFIRE))
                                {
                                    aura->SetDuration(aura->GetDuration() + 2000);
                                    if (aura->GetMaxDuration() < aura->GetDuration())
                                        aura->SetMaxDuration(aura->GetDuration());
                                }
                            }

                            break;
                        }
                        case SPELL_DRUID_STARFIRE:
                        {
                            int32 eclipse = 20; // 20 Solar energy

                            // The last eclipse was Solar, so Starfire shouldn't give energy
                            if (_player->GetLastEclipsePower() == SPELL_DRUID_SOLAR_ECLIPSE)
                                eclipse = 0;

                            if (_player->HasAura(SPELL_DRUID_EUPHORIA) && !_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                eclipse *= 2;

                            _player->SetEclipsePower(int32(_player->GetEclipsePower() + eclipse));

                            // Your crits with Starfire also increase moonfire duration by 2s
                            if (GetSpell()->IsCritForTarget(target))
                            {
                                if (Aura* aura = target->GetAura(SPELL_DRUID_MOONFIRE))
                                {
                                    aura->SetDuration(aura->GetDuration() + 2000);
                                    if (aura->GetMaxDuration() < aura->GetDuration())
                                        aura->SetMaxDuration(aura->GetDuration());
                                }
                            }

                            break;
                        }

                        case SPELL_DRUID_STARSURGE:
                        {
                            int32 eclipse = 20; // 20 Solar or Lunar energy

                            if (_player->HasAura(SPELL_DRUID_EUPHORIA) && !_player->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && !_player->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                                eclipse *= 2;

                            // Give Solar energy just if our last eclipse was lunar else, give Lunar energy
                            if (_player->GetLastEclipsePower() == SPELL_DRUID_LUNAR_ECLIPSE)
                                _player->SetEclipsePower(int32(_player->GetEclipsePower() + eclipse));
                            else
                                _player->SetEclipsePower(int32(_player->GetEclipsePower() - eclipse));


                            // Your crits with Starsurge also increase sunfire duration by 2s
                            if (GetSpell()->IsCritForTarget(target))
                            {
                                if (Aura* aura = target->GetAura(SPELL_DRUID_SUNFIRE))
                                {
                                    aura->SetDuration(aura->GetDuration() + 2000);
                                    if (aura->GetMaxDuration() < aura->GetDuration())
                                        aura->SetMaxDuration(aura->GetDuration());
                                }
                            }
                            // Your crits with Starsurge also increase moonfire duration by 2s
                            if (GetSpell()->IsCritForTarget(target))
                            {
                                if (Aura* aura = target->GetAura(SPELL_DRUID_MOONFIRE))

                                {
                                    aura->SetDuration(aura->GetDuration() + 2000);
                                    if (aura->GetMaxDuration() < aura->GetDuration())
                                        aura->SetMaxDuration(aura->GetDuration());
                                }
                            }

                            break;
                        }
                        }
                        // Now check if we have lunar or solar eclipse and give it with effects
                        _player->SendEclipse();
                    }
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_dru_eclipse_SpellScript::HandleAfterCast);
            AfterHit += SpellHitFn(spell_dru_eclipse_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_eclipse_SpellScript();
    }
};

// 202808 - Primal Vitality
class spell_dru_primal_vitality : public SpellScriptLoader
{
public:
    spell_dru_primal_vitality() : SpellScriptLoader("spell_dru_primal_vitality") { }

    class spell_dru_primal_vitality_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_primal_vitality_AuraScript);

        enum
        {
            SPELL_DRUID_PRIMAL_VITALITY_PASSIVE = 202808,
            SPELL_DRUID_PRIMAL_VITALITY_EFFECT = 202812,
            SPELL_DRUID_PROWL = 5215
        };

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetSpellInfo())
                return false;

            if (!eventInfo.GetDamageInfo())
                return false;

            if (eventInfo.GetSpellInfo()->Id != SPELL_DRUID_PROWL)
                return false;

            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (Unit* target = eventInfo.GetProcTarget())
            {
                if (!target->HasAura(SPELL_DRUID_PRIMAL_VITALITY_EFFECT))
                    target->AddAura(SPELL_DRUID_PRIMAL_VITALITY_EFFECT, target);

            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_primal_vitality_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_primal_vitality_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_primal_vitality_AuraScript();
    }
};

// Nourish - 203374 <- wrong spell id + needs a rework as far as I can see, but I have no single clue about druids in this exp -- Damieninabox
class spell_dru_nourish : public SpellScriptLoader
{
public:
    spell_dru_nourish() : SpellScriptLoader("spell_dru_nourish") { }

private:
    class spell_dru_nourish_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_nourish_SpellScript);

        enum
        {
            SPELL_DRUID_NOURISH_PASSIVE = 203374,
            SPELL_DRUID_REJUVENATION = 774
        };

        void HandleOnHit()
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (caster->HasAura(SPELL_DRUID_NOURISH_PASSIVE))
                        caster->CastSpell(target, SPELL_DRUID_REJUVENATION, true);
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_nourish_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_nourish_SpellScript();
    }
};

// Called by Ironfur - 192081
// Den Mother - 201522
class spell_dru_denmother : public SpellScriptLoader
{
public:
    spell_dru_denmother() : SpellScriptLoader("spell_dru_denmother") { }

private:
    class spell_dru_denmother_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_denmother_SpellScript);

        enum
        {
            SPELL_DRUID_DEN_MOTHER = 201522,
            SPELL_DRUID_DEN_MOTHER_IRONFUR = 201629,
        };

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (_player->HasAura(SPELL_DRUID_DEN_MOTHER))
                {
                    std::list<Unit*> validTargets;
                    std::list<Unit*> groupList;

                    _player->GetPartyMembers(groupList);

                    if (groupList.empty())
                        return;

                    for (auto itr : groupList)
                    {
                        if ((itr->GetGUID() != _player->GetGUID()) && (itr->IsInRange(_player, 0, 50, true)))
                            validTargets.push_back(itr->ToUnit());
                    }

                    if (validTargets.empty())
                        return;

                    validTargets.sort(Trinity::HealthPctOrderPred());
                    auto const lowTarget = validTargets.front();

                    _player->CastSpell(lowTarget, 201629, true);
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_denmother_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_denmother_SpellScript();
    }
};

// Overgrowth - 203651
class spell_dru_overgrowth : public SpellScriptLoader
{
public:
    spell_dru_overgrowth() : SpellScriptLoader("spell_dru_overgrowth") { }

    class spell_dru_overgrowth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_overgrowth_SpellScript);

        enum
        {
            SPELL_DRUID_REJUVENATION = 774,
            SPELL_DRUID_WILD_GROWTH = 48438,
            SPELL_DRUID_LIFE_BLOOM = 33763,
            SPELL_DRUID_REGROWTH = 8936
        };

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    caster->AddAura(SPELL_DRUID_REJUVENATION, target);
                    caster->AddAura(SPELL_DRUID_WILD_GROWTH, target);
                    caster->AddAura(SPELL_DRUID_LIFE_BLOOM, target);
                    caster->AddAura(SPELL_DRUID_REGROWTH, target);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_overgrowth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_overgrowth_SpellScript();
    }
};

// 221514 - Skull Bash Charge
class spell_dru_skull_bash_charge : public SpellScriptLoader
{
public:
    spell_dru_skull_bash_charge() : SpellScriptLoader("spell_dru_skull_bash_charge") { }

    class spell_dru_skull_bash_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_skull_bash_charge_SpellScript);

        void HandleCharge(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            if (!GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), 93985, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_skull_bash_charge_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_skull_bash_charge_SpellScript();
    }
};

// 157982 - Tranquility Heal
class spell_dru_tranquility_heal : public SpellScriptLoader
{
public:
    spell_dru_tranquility_heal() : SpellScriptLoader("spell_dru_tranquility_heal") { }

    class spell_dru_tranquility_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_tranquility_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (Unit* caster = GetCaster())
            {
                uint32 heal = CalculatePct(caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_NATURE), 180);
                SetHitHeal(heal);
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_dru_tranquility_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_tranquility_heal_SpellScript();
    }
};

// 200389 - Cultivation
class spell_dru_cultivation : public SpellScriptLoader
{
public:
    spell_dru_cultivation() : SpellScriptLoader("spell_dru_cultivation") { }

    class spell_dru_cultivation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_cultivation_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!GetCaster())
                return;

            amount = CalculatePct(GetCaster()->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_NATURE), 60);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_cultivation_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_cultivation_AuraScript();
    }
};

// 774 - Rejuvenation
class spell_dru_rejuvenation : public SpellScriptLoader
{
public:
    spell_dru_rejuvenation() : SpellScriptLoader("spell_dru_rejuvenation") { }

    class spell_dru_rejuvenation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rejuvenation_AuraScript);

        void HandleCalculateAmount(AuraEffect const* /*p_AurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {                
                if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO) && !caster->HasAura(SPELL_DRUID_REJUVENATION))
                {
                    amount *= 2;
                    caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                }
            }
        }

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            Unit* caster = GetCaster();

            if (!caster)
                return;

            if (AuraEffect* GlyphOfRejuvenation = caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_REJUVENATION, EFFECT_0))
            {
                GlyphOfRejuvenation->SetAmount(GlyphOfRejuvenation->GetAmount() + 1);
                if (GlyphOfRejuvenation->GetAmount() >= 3)
                    caster->CastSpell(caster, SPELL_DRU_GLYPH_OF_REJUVENATION_EFFECT, true);
            }
        }

        void OnPeriodic(AuraEffect const* AurEff)
        {
            if (Unit* target = GetTarget())
                if (GetCaster()->HasAura(SPELL_DRU_CULTIVATION) && !target->HasAura(SPELL_DRU_CULTIVATION_HEAL) && target->HealthBelowPct(60))
                    GetCaster()->CastSpell(target, SPELL_DRU_CULTIVATION_HEAL, true);
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            Unit* caster = GetCaster();

            if (!caster)
                return;

            if (AuraEffect* l_GlyphOfRejuvenation = caster->GetAuraEffect(SPELL_DRU_GLYPH_OF_REJUVENATION, EFFECT_0))
            {
                l_GlyphOfRejuvenation->SetAmount(l_GlyphOfRejuvenation->GetAmount() - 1);
                if (l_GlyphOfRejuvenation->GetAmount() < 3)
                    caster->RemoveAura(SPELL_DRU_GLYPH_OF_REJUVENATION_EFFECT);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_rejuvenation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_rejuvenation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rejuvenation_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_rejuvenation_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rejuvenation_AuraScript();
    }

    class spell_dru_rejuvenation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_rejuvenation_SpellScript);

        int32 m_RejuvenationAura = 0;
        int32 m_RejuvenationAuraAmount = 0;

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetHitUnit();
            if (!target)
                return;

            Aura* RejuvenationAura = target->GetAura(SPELL_DRUID_REJUVENATION, caster->GetGUID());

            if (RejuvenationAura && m_RejuvenationAura > 0)
                RejuvenationAura->SetDuration(m_RejuvenationAura);

            if (AuraEffect* NewRejuvenationAuraEffect = target->GetAuraEffect(SPELL_DRUID_REJUVENATION, EFFECT_0))
            {
                if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
                {
                    NewRejuvenationAuraEffect->SetAmount(NewRejuvenationAuraEffect->GetAmount() * 2);
                    caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
                }
            }
        }

        void HandleBeforeHit(SpellMissInfo missInfo)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO))
            {
                //NewRejuvenationAuraEffect->SetAmount(NewRejuvenationAuraEffect->GetAmount() * 2);
                SetHitHeal(GetHitHeal() * 2);
                //caster->RemoveAura(SPELL_DRUID_SOUL_OF_THE_FOREST_RESTO);
            }

            if (caster->HasAura(SPELL_DRU_GERMINATION) && target->HasAura(SPELL_DRUID_REJUVENATION, caster->GetGUID()))
            {
                Aura* RejuvenationAura = target->GetAura(SPELL_DRUID_REJUVENATION, caster->GetGUID());
                if (!RejuvenationAura)
                    return;

                if (!target->HasAura(SPELL_DRU_GERMINATION_HOT, caster->GetGUID()))
                {
                    caster->CastSpell(target, SPELL_DRU_GERMINATION_HOT, true);
                    m_RejuvenationAura = RejuvenationAura->GetDuration();
                }
                else
                {
                    Aura* GerminationAura = target->GetAura(SPELL_DRU_GERMINATION_HOT, caster->GetGUID());
                    Aura* RejuvenationAura = target->GetAura(SPELL_DRUID_REJUVENATION, caster->GetGUID());
                    if (GerminationAura && RejuvenationAura)
                    {
                        int32 GerminationDuration = GerminationAura->GetDuration();
                        int32 RejuvenationDuration = RejuvenationAura->GetDuration();
                        if (GerminationDuration > RejuvenationDuration)
                        {
                            caster->AddAura(SPELL_DRUID_REJUVENATION, target);
                        }
                        else
                        {
                            caster->CastSpell(target, SPELL_DRU_GERMINATION_HOT, true);
                            m_RejuvenationAura = RejuvenationDuration;
                        }
                    }
                }
            }
        }

        void Register()
        {
            BeforeHit += BeforeSpellHitFn(spell_dru_rejuvenation_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_dru_rejuvenation_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dru_rejuvenation_SpellScript();
    }
};

// 58180 - Infected Wounds
class spell_dru_infected_wounds : public SpellScriptLoader
{
public:
    spell_dru_infected_wounds() : SpellScriptLoader("spell_dru_infected_wounds") { }


    class spell_dru_infected_wounds_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_infected_wounds_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (GetCaster()->HasAura(GetSpellInfo()->Id))
                GetCaster()->RemoveAurasDueToSpell(GetSpellInfo()->Id);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_infected_wounds_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_infected_wounds_SpellScript();
    }
};

// 190984 - Solar Wrath | 194153 - Lunar Strike
class spell_dru_blessing_of_elune : public SpellScript
{
    PrepareSpellScript(spell_dru_blessing_of_elune);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();

        if (!caster)
            return;

        uint32 power = GetHitDamage();

        if (Aura* aura = caster->GetAura(202737))
            if (AuraEffect* aurEff = aura->GetEffect(EFFECT_0))
                power += CalculatePct(power, aurEff->GetAmount());

        SetHitDamage(power);
    }

    void Register()
    {
        OnHit += SpellHitFn(spell_dru_blessing_of_elune::HandleOnHit);
    }
};

// 208253 - Essence of G'Hanir
class spell_dru_essence_of_ghanir : public SpellScriptLoader
{
public:
    spell_dru_essence_of_ghanir() : SpellScriptLoader("spell_dru_essence_of_ghanir") { }

    class spell_dru_essence_of_ghanir_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_essence_of_ghanir_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SpellModOp(aurEff->GetMiscValue());
                spellMod->type = SPELLMOD_PCT;
                spellMod->spellId = GetId();
                spellMod->mask = GetSpellInfo()->GetEffect(aurEff->GetEffIndex())->SpellClassMask;
            }

            spellMod->value = aurEff->GetAmount();
        }

        void Register() override
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_essence_of_ghanir_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_essence_of_ghanir_AuraScript();
    }
};

// 200851 - Rage of the Sleeper
class spell_dru_rage_of_the_sleeper : public SpellScriptLoader
{
public:
    spell_dru_rage_of_the_sleeper() : SpellScriptLoader("spell_dru_rage_of_the_sleeper") { }

    class spell_dru_rage_of_the_sleeper_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rage_of_the_sleeper_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = caster->GetVictim())
                    caster->CastSpell(target, 219432, true);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rage_of_the_sleeper_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_rage_of_the_sleeper_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_rage_of_the_sleeper_AuraScript();
    }
};

// 194153 Lunar Strike
class spell_druid_lunar_strike : public SpellScript
{
    PrepareSpellScript(spell_druid_lunar_strike);

    enum Spells
    {
        SPELL_DRUID_LUNAR_STRIKE = 194153,
        SPELL_DRUID_WARRIOR_OF_ELUNE = 202425,
        SPELL_DRUID_NATURES_BALANCE = 202430,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_MOONFIRE_DAMAGE, SPELL_DRUID_WARRIOR_OF_ELUNE, SPELL_DRUID_LUNAR_STRIKE, SPELL_DRUID_NATURES_BALANCE });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        Unit* explTarget = GetExplTargetUnit();
        Unit* currentTarget = GetHitUnit();

        if (!explTarget || !currentTarget)
            return;

        if (currentTarget != explTarget)
            SetHitDamage(GetHitDamage() * GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints / 100);

        if (GetCaster()->HasAura(SPELL_DRUID_NATURES_BALANCE))
            if (Aura* moonfireDOT = currentTarget->GetAura(SPELL_DRUID_MOONFIRE_DAMAGE, GetCaster()->GetGUID()))
            {
                int32 duration = moonfireDOT->GetDuration();
                int32 newDuration = duration + 6 * IN_MILLISECONDS;

                if (newDuration > moonfireDOT->GetMaxDuration())
                    moonfireDOT->SetMaxDuration(newDuration);

                moonfireDOT->SetDuration(newDuration);
            }

        if (GetCaster() && roll_chance_f(20) && GetCaster()->HasAura(SPELL_DRU_ECLIPSE))
            GetCaster()->CastSpell(nullptr, SPELL_DRU_SOLAR_EMPOWEREMENT, true);
    }

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        if (Aura* WarriorOfElune = GetCaster()->GetAura(SPELL_DRUID_WARRIOR_OF_ELUNE))
        {
            int32 amount = WarriorOfElune->GetEffect(EFFECT_0)->GetAmount();
            WarriorOfElune->GetEffect(EFFECT_0)->SetAmount(amount - 1);
            if (amount == -102)
                GetCaster()->RemoveAurasDueToSpell(SPELL_DRUID_WARRIOR_OF_ELUNE);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_druid_lunar_strike::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnEffectHit += SpellEffectFn(spell_druid_lunar_strike::HandleHit, EFFECT_1, SPELL_EFFECT_ENERGIZE);
    }
};

// 190984 Solar Wrath
class spell_druid_solar_wrath : public SpellScript
{
    PrepareSpellScript(spell_druid_solar_wrath);

    enum Spells
    {
        SPELL_DRUID_SOLAR_WRATH = 190984,
        SPELL_DRUID_NATURES_BALANCE = 202430,
        SPELL_DRUID_SUNFIRE_DOT = 164815,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_SUNFIRE_DOT, SPELL_DRUID_SOLAR_WRATH, SPELL_DRUID_NATURES_BALANCE });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            if (GetCaster()->HasAura(SPELL_DRUID_NATURES_BALANCE))
                if (Aura* sunfireDOT = target->GetAura(SPELL_DRUID_SUNFIRE_DOT, GetCaster()->GetGUID()))
                {
                    int32 duration = sunfireDOT->GetDuration();
                    int32 newDuration = duration + 4 * IN_MILLISECONDS;

                    if (newDuration > sunfireDOT->GetMaxDuration())
                        sunfireDOT->SetMaxDuration(newDuration);

                    sunfireDOT->SetDuration(newDuration);
                }
        if (GetCaster() && roll_chance_f(20) && GetCaster()->HasAura(SPELL_DRU_ECLIPSE))
            GetCaster()->CastSpell(nullptr, SPELL_DRU_LUNAR_EMPOWEREMENT, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_druid_solar_wrath::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 203975 Earthwarden
class spell_druid_earthwarden_triggered : public AuraScript
{
    PrepareAuraScript(spell_druid_earthwarden_triggered);

    enum Spells
    {
        SPELL_DRUID_EARTHWARDEN = 203974,
        SPELL_DRUID_EARTHWARDEN_TRIGGERED = 203975,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_EARTHWARDEN, SPELL_DRUID_EARTHWARDEN_TRIGGERED });
    }

    void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* auraEffect, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        if (dmgInfo.GetDamageType() == DIRECT_DAMAGE)
        {
            SpellInfo const* earthwarden = sSpellMgr->AssertSpellInfo(SPELL_DRUID_EARTHWARDEN);

            absorbAmount = CalculatePct(dmgInfo.GetDamage(), earthwarden->GetEffect(EFFECT_0)->BasePoints);
            GetCaster()->RemoveAurasDueToSpell(SPELL_DRUID_EARTHWARDEN_TRIGGERED);
        }
    }

    void Register()
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_druid_earthwarden_triggered::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_druid_earthwarden_triggered::Absorb, EFFECT_0);
    }
};

// 203974 Earthwarden
class spell_druid_earthwarden : public AuraScript
{
    PrepareAuraScript(spell_druid_earthwarden);

    enum Spells
    {
        SPELL_DRUID_EARTHWARDEN = 203974,
        SPELL_DRUID_EARTHWARDEN_TRIGGERED = 203975,
        SPELL_DRUID_TRASH = 77758,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_EARTHWARDEN, SPELL_DRUID_EARTHWARDEN_TRIGGERED, SPELL_DRUID_TRASH });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetCaster()->IsPlayer())
            if (eventInfo.GetDamageInfo()->GetSpellInfo())
                return eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_DRUID_TRASH;
        return false;
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (!GetCaster()->ToPlayer()->GetSpellHistory()->HasCooldown(SPELL_DRUID_EARTHWARDEN))
            GetCaster()->AddAura(SPELL_DRUID_EARTHWARDEN_TRIGGERED, GetCaster());
        GetCaster()->ToPlayer()->GetSpellHistory()->AddCooldown(SPELL_DRUID_EARTHWARDEN, 0, std::chrono::milliseconds(500));
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(spell_druid_earthwarden::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_druid_earthwarden::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 80313 Pulverize
class spell_druid_pulverize : public SpellScript
{
    PrepareSpellScript(spell_druid_pulverize);

    enum Spells
    {
        SPELL_DRUID_PULVERIZE = 80313,
        SPELL_DRUID_TRASH_DOT_TWO_STACKS_MARKER = 158790,
        SPELL_DRUID_PULVERIZE_DAMAGE_REDUCTION_BUFF = 158792,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_PULVERIZE, SPELL_DRUID_TRASH_DOT_TWO_STACKS_MARKER });
    }

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
        {
            target->RemoveAurasDueToSpell(SPELL_DRUID_TRASH_DOT_TWO_STACKS_MARKER);
            GetCaster()->CastSpell(target, SPELL_DRUID_PULVERIZE_DAMAGE_REDUCTION_BUFF, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_druid_pulverize::HandleHitTarget, EFFECT_2, SPELL_EFFECT_DUMMY);
    }
};

// 204053 Rend and Tear
class spell_druid_rend_and_tear : public AuraScript
{
    PrepareAuraScript(spell_druid_rend_and_tear);

    enum Spells
    {
        SPELL_DRUID_REND_AND_TEAR = 204053,
        SPELL_DRUID_TRASH_DOT = 192090,
    };

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DRUID_REND_AND_TEAR, SPELL_DRUID_TRASH_DOT });
    }

    void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* auraEffect, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* caster = GetCaster();
        Unit* attacker = dmgInfo.GetAttacker();
        absorbAmount = 0;

        if (!caster || !attacker || !HasEffect(EFFECT_1))
            return;

        if (caster->GetShapeshiftForm() == FORM_BEAR_FORM)
            if (Aura* trashDOT = attacker->GetAura(SPELL_DRUID_TRASH_DOT, caster->GetGUID()))
                absorbAmount = CalculatePct(dmgInfo.GetDamage(), trashDOT->GetStackAmount() * GetSpellInfo()->GetEffect(EFFECT_1)->BasePoints);
    }

    void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
    {
        if (!spellMod)
            return;

        spellMod->value = GetCaster()->GetShapeshiftForm() == FORM_BEAR_FORM ? aurEff->GetAmount() : 0;
    }

    void Register()
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_druid_rend_and_tear::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_druid_rend_and_tear::Absorb, EFFECT_0);
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_druid_rend_and_tear::HandleEffectCalcSpellMod, EFFECT_1, SPELL_AURA_ADD_FLAT_MODIFIER);
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_druid_rend_and_tear::HandleEffectCalcSpellMod, EFFECT_2, SPELL_AURA_ADD_FLAT_MODIFIER);
    }
};

//Incarnation: Tree of Life 33891
class incarnation_tree_of_life : public SpellScript
{
    PrepareSpellScript(incarnation_tree_of_life);

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();
        if (Aura* tree = caster->GetAura(33891))
        {
            tree->SetDuration(30000, true);
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(incarnation_tree_of_life::HandleAfterCast);
    }
};

//274837
class spell_feral_frenzy : public SpellScript
{
    PrepareSpellScript(spell_feral_frenzy);

    void HandleOnHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();

        if (!caster || !target)
            return;

        this->strikes = 0;

        int32 strikeDamage = 100 / 20 + caster->m_unitData->AttackPower;

        caster->GetScheduler().Schedule(50ms, [caster, target, strikeDamage, this] (TaskContext context)
        {
           if (caster->GetDistance2d(target) <= 5.0f)
           {
                caster->DealDamage(target, strikeDamage, 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, 0, true);
                strikes++;

                if (this->strikes < 5)                
                    context.Repeat(200ms);

                else if (this->strikes == 5)
                {
                    caster->CastSpell(target, SPELL_FERAL_FRENZY_BLEED, true);
                    int32 bleedDamage = 100 / 10 + caster->m_unitData->AttackPower;
                    caster->DealDamage(target, bleedDamage, 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, 0, true);
                }
           }
        });
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_feral_frenzy::HandleOnHit);
    }

private:
    uint8 strikes;
};

class feral_spells : public PlayerScript
{
public:
    feral_spells() : PlayerScript("feral_spells") { }

    void OnLogin(Player* player, bool) override
    {
        if (player->GetSpecializationId() != TALENT_SPEC_DRUID_CAT)
            return;

        if (player->getLevel() >= 5 && !player->HasSpell(SPELL_DRUID_SHRED))
            player->LearnSpell(SPELL_DRUID_SHRED, false, true);

        if (player->getLevel() >= 20 && !player->HasSpell(SPELL_DRUID_RIP))
            player->LearnSpell(SPELL_DRUID_RIP, false, true);

        if (player->getLevel() >= 12 && !player->HasSpell(SPELL_DRUID_RAKE))
            player->LearnSpell(SPELL_DRUID_RAKE, false, true);

        if (player->getLevel() >= 7 && !player->HasSpell(SPELL_DRUID_FEROCIOUS_BITE))
            player->LearnSpell(SPELL_DRUID_FEROCIOUS_BITE, false, true);
    }
};

//78674
class spell_dru_starsurge : public SpellScript
{
    PrepareSpellScript(spell_dru_starsurge);

    void HandleOnHit()
    {
        if (GetCaster())
            if (GetCaster()->GetAuraCount(SPELL_DRU_STARLORD_BUFF) < 3)
                GetCaster()->CastSpell(nullptr, SPELL_DRU_STARLORD_BUFF, true);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dru_starsurge::HandleOnHit);
    }
};

//191034
class spell_dru_starfall : public SpellScript
{
    PrepareSpellScript(spell_dru_starfall);

    void HandleOnHit()
    {
        if (GetCaster())
            if (GetCaster()->GetAuraCount(SPELL_DRU_STARLORD_BUFF) < 3)
                GetCaster()->CastSpell(nullptr, SPELL_DRU_STARLORD_BUFF, true);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_dru_starfall::HandleOnHit);
    }
};

//252216
class spell_dru_tiger_dash : public SpellScript
{
    PrepareSpellScript(spell_dru_tiger_dash);

    void OnActivate()
    {
        if (Player* player = GetCaster()->ToPlayer())
            player->CastSpell(player, SPELL_DRUID_CAT_FORM, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_tiger_dash::OnActivate);
    }    
};

//274902
class spell_dru_photosynthesis : public AuraScript
{
    PrepareAuraScript(spell_dru_photosynthesis);

    void OnApply(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster()->HasAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS))
            GetCaster()->AddAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS);
    }

    void OnRemove(const AuraEffect* /* aurEff */, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster()->HasAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS))
            GetCaster()->RemoveAura(SPELL_DRU_PHOTOSYNTHESIS_MOD_HEAL_TICKS);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_dru_photosynthesis::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_dru_photosynthesis::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

//10682, 204066
struct at_dru_lunar_beam : AreaTriggerAI
{
    at_dru_lunar_beam(AreaTrigger* at) : AreaTriggerAI(at) { }

    void OnCreate() override
    {
        at->SetPeriodicProcTimer(1000);
    }

    void OnPeriodicProc() override
    {
        if (at->GetCaster())
            at->GetCaster()->CastSpell(at->GetPosition(), SPELL_DRU_LUNAR_BEAM_DAMAGE_HEAL, true);
    }
};

//3020, 102793
struct at_dru_ursol_vortex : AreaTriggerAI
{
    at_dru_ursol_vortex(AreaTrigger* at) : AreaTriggerAI(at) { }

private:
    bool pull;

    void OnCreate() override
    {
        pull = false;
    }

    void OnUnitEnter(Unit* target) override
    {
        if (target->IsValidAttackTarget(at->GetCaster()) && !pull)
        {
            pull = true;
            target->CastSpell(at->GetPosition(), SPELL_DRU_URSOL_VORTEX_PULL, true);
        }
    }
};

//102359
class spell_dru_mass_entanglement : public SpellScript
{
    PrepareSpellScript(spell_dru_mass_entanglement);

    void HandleCast()
    {
        std::list<Unit*> targetList;
        GetCaster()->GetAttackableUnitListInRange(targetList, 15.0f);
        if (targetList.size())
            for (auto& targets : targetList)
            {
                GetCaster()->AddAura(SPELL_DRU_MASS_ENTANGLEMENT, targets);
            }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_dru_mass_entanglement::HandleCast);
    }
};

class dru_predator : public PlayerScript
{
public:
    dru_predator() : PlayerScript("dru_predator") { }

    void OnPVPKill(Player* killer, Player* killed) 
    { 
        if (killer->getClass() == CLASS_DRUID)
            return;

        if (!killer->HasAura(SPELL_DRU_PREDATOR))
            return;

        if (killer->GetSpellHistory()->HasCooldown(SPELL_DRU_TIGER_FURY))
            killer->GetSpellHistory()->ResetCooldown(SPELL_DRU_TIGER_FURY);
    }

    void OnCreatureKill(Player* killer, Creature* killed) 
    { 
        if (killer->getClass() == CLASS_DRUID)
            return;

        if (!killer->HasAura(SPELL_DRU_PREDATOR))
            return;

        if (killer->GetSpellHistory()->HasCooldown(SPELL_DRU_TIGER_FURY))
            killer->GetSpellHistory()->ResetCooldown(SPELL_DRU_TIGER_FURY);
    }
};

void AddSC_druid_spell_scripts()
{
    RegisterSpellScript(spell_dru_efflorescence);
    RegisterAuraScript(spell_dru_efflorescence_aura);
    RegisterSpellScript(spell_dru_efflorescence_heal);
    new spell_dru_primal_fury();
    new spell_dru_predatory_swiftness();
    new spell_dru_predatory_swiftness_aura();
    RegisterAuraScript(spell_dru_lifebloom);
    new spell_dru_berserk();
    new spell_dru_sunfire();
    new spell_dru_balance_affinity_dps();
    new spell_dru_balance_affinity_resto();
    RegisterSpellScript(spell_dru_ferocious_bite);
    new spell_dru_dash();
    new spell_dru_savage_roar();
    new spell_dru_survival_instincts();
    new spell_dru_swift_flight_passive();
    new spell_dru_wild_growth();
    new spell_dru_incarnation_chosen_of_elune();
    new spell_dru_incarnation_king_of_the_jungle();
    new spell_dru_incarnation_guardian_of_ursoc();
    new spell_dru_wild_charge_moonkin();
    new spell_dru_moonfire();
    new spell_dru_cat_form();
    new spell_dru_bear_form();
    new spell_dru_skull_bash();
    new spell_dru_stampeding_roar();
    new spell_dru_activate_cat_form();
    new spell_dru_infected_wound();
    RegisterAuraScript(spell_dru_ysera_gift);
    RegisterSpellScript(spell_dru_rake);
    RegisterSpellScript(spell_dru_maim);
    new spell_dru_rip();
    new spell_dru_bloodtalons();
    new spell_dru_travel_form_dummy();
    new spell_dru_travel_form();
    RegisterAuraScript(aura_dru_charm_woodland_creature);
    RegisterSpellScript(spell_dru_swipe);
    RegisterSpellScript(spell_dru_brutal_slash);
    RegisterSpellScript(spell_dru_thrash_cat);
    RegisterSpellScript(spell_dru_shred);
    RegisterSpellScript(spell_dru_maul_bear);
    RegisterSpellScript(spell_dru_thrash_bear);
    RegisterAuraScript(aura_dru_thrash_bear);
    RegisterSpellScript(spell_dru_blessing_of_the_ancients);
    RegisterAuraScript(spell_dru_gore);
    RegisterAuraScript(aura_dru_solar_empowerment);
    RegisterAuraScript(aura_dru_lunar_empowerment);
    RegisterAuraScript(aura_dru_astral_form);
    RegisterAuraScript(aura_dru_restoration_affinity);
    RegisterAuraScript(aura_dru_feral_affinity);
    RegisterAuraScript(aura_dru_frenzied_regeneration);
    RegisterAreaTriggerAI(at_dru_starfall);
    RegisterCreatureAI(npc_dru_efflorescence);
    new spell_dru_moonfire_sunfire_dot();
    RegisterSpellScript(spell_dru_regrowth);
    new spell_dru_flourish();
    new spell_dru_swiftmend();
    new spell_dru_ursols_vortex();
    new spell_dru_brambles();
    new spell_dru_barkskin();
    new spell_dru_cenarion_ward();
    new spell_dru_cenarion_ward_hot();
    new spell_dru_galactic_guardian();
    //new spell_dru_bristling_fur();
    new spell_dru_forms_trinket();
    new spell_dru_glyph_of_starfire_proc();
    new spell_dru_typhoon();
    new spell_dru_t3_6p_bonus();
    new spell_dru_t3_8p_bonus();
    new spell_dru_t4_2p_bonus();
    new spell_dru_item_t6_trinket();
    new spell_dru_t10_balance_4p_bonus();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_t10_restoration_4p_bonus_dummy();
    //new spell_dru_eclipse();
    new spell_dru_primal_vitality();
    new spell_dru_nourish();
    new spell_dru_denmother();
    new spell_dru_overgrowth();
    new spell_dru_skull_bash_charge();
    new spell_dru_tranquility_heal();
    new spell_dru_cultivation();
    new spell_dru_rejuvenation();
    new spell_dru_infected_wounds();
    RegisterSpellScript(spell_dru_blessing_of_elune);
    new spell_dru_essence_of_ghanir();
    new spell_dru_rage_of_the_sleeper();
    RegisterSpellScript(spell_druid_lunar_strike);
    RegisterSpellScript(spell_druid_solar_wrath);
    RegisterAuraScript(spell_druid_earthwarden_triggered);
    RegisterAuraScript(spell_druid_earthwarden);
    RegisterSpellScript(spell_druid_pulverize);
    RegisterAuraScript(spell_druid_rend_and_tear);
    RegisterSpellScript(incarnation_tree_of_life);
    RegisterSpellScript(spell_feral_frenzy);
    RegisterPlayerScript(feral_spells);
    RegisterSpellScript(spell_dru_starsurge);
    RegisterSpellScript(spell_dru_starfall);
    RegisterSpellScript(spell_dru_tiger_dash);
    RegisterAuraScript(spell_dru_photosynthesis);
    RegisterAreaTriggerAI(at_dru_lunar_beam);
    RegisterAreaTriggerAI(at_dru_ursol_vortex);
    RegisterSpellScript(spell_dru_mass_entanglement);
    RegisterPlayerScript(dru_predator);
}
