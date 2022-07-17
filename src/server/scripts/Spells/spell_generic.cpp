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
 * Scripts for spells with SPELLFAMILY_GENERIC which cannot be included in AI script file
 * of creature using it or can't be bound to any player class.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_gen_"
 */

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "ScriptMgr.h"
#include "Battleground.h"
#include "CellImpl.h"
#include "DB2Stores.h"
#include "GameTime.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "InstanceScript.h"
#include "Item.h"
#include "LFGMgr.h"
#include "Log.h"
#include "NPCPackets.h"
#include "Pet.h"
#include "ReputationMgr.h"
#include "SkillDiscovery.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "DatabaseEnv.h"


//312372
class spell_back_camp : public SpellScript
{
    PrepareSpellScript(spell_back_camp);
	   
    void HandleTeleport()
    {
        Unit* caster = GetCaster();
        Player* player = caster->ToPlayer();
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_CAMP);
        stmt->setUInt64(0, player->GetGUID().GetCounter());
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        Field* fields = result->Fetch();
        float camp_x = fields[0].GetFloat();
        float camp_y = fields[1].GetFloat();
        float camp_z = fields[2].GetFloat();
        float camp_o = fields[3].GetFloat();
        int camp_mapid = fields[4].GetUInt16();

        player->TeleportTo(camp_mapid, camp_x, camp_y, camp_z, camp_o);
        player->RemoveMovementImpairingAuras();

        Player* gamer = GetCaster()->ToPlayer();
        int mapid = caster->GetMapId();

        // Tente: 292769
        // Sac: 276247
        // campfire: 301125

        while (caster->GetPositionX() == camp_x) 
		{
            uint32 spawntm = 300;
            uint32 objectId = atoul("292769");
            GameObject* tempGob = gamer->SummonGameObject(objectId, *gamer, QuaternionData::fromEulerAnglesZYX(gamer->GetOrientation(), 0.0f, 0.0f), spawntm);
            gamer->SetLastTargetedGO(tempGob->GetGUID().GetCounter());

            objectId = atoul("276247");
            tempGob = gamer->SummonGameObject(objectId, Position(camp_x + 2.0f, camp_y + 2.0f, camp_z, camp_o), QuaternionData::fromEulerAnglesZYX(gamer->GetOrientation(), 0.0f, 0.0f), spawntm);
            gamer->SetLastTargetedGO(tempGob->GetGUID().GetCounter());

            objectId = atoul("301125");
            tempGob = gamer->SummonGameObject(objectId, Position(camp_x + -2.0f, camp_y + -2.0f, camp_z, camp_o), QuaternionData::fromEulerAnglesZYX(gamer->GetOrientation(), 0.0f, 0.0f), spawntm);
            gamer->SetLastTargetedGO(tempGob->GetGUID().GetCounter());
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_back_camp::HandleTeleport);
    }

};

//312370
class spell_make_camp : public SpellScript
{
    PrepareSpellScript(spell_make_camp);

    void Oncast()
    {
        Unit* caster = GetCaster();
        float x = caster->GetPositionX();
        float y = caster->GetPositionY();
        float z = caster->GetPositionZ();
        float o = caster->GetOrientation();
        int m = caster->GetMapId();
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_CAMP);
        stmt->setFloat(0, x);
        stmt->setFloat(1, y);
        stmt->setFloat(2, z);
        stmt->setFloat(3, o);
        stmt->setUInt16(4, uint16(m));
        stmt->setUInt64(5, caster->GetGUID().GetCounter());
        CharacterDatabase.Execute(stmt);
        // Tente: 292769
        // Sac: 276247
        // campfire: 301125

        Player* gamer = GetCaster()->ToPlayer();
        uint32 spawntm = 300;
        uint32 objectId = atoul("292769");
        GameObject* tempGob = gamer->SummonGameObject(objectId, *gamer, QuaternionData::fromEulerAnglesZYX(gamer->GetOrientation(), 0.0f, 0.0f), spawntm);
        gamer->SetLastTargetedGO(tempGob->GetGUID().GetCounter());

        objectId = atoul("276247");
        tempGob = gamer->SummonGameObject(objectId, Position(x + 2.0f, y + 2.0f, z, o), QuaternionData::fromEulerAnglesZYX(gamer->GetOrientation(), 0.0f, 0.0f), spawntm);
        gamer->SetLastTargetedGO(tempGob->GetGUID().GetCounter());

        objectId = atoul("301125");
        tempGob = gamer->SummonGameObject(objectId, Position(x + -2.0f, y + -2.0f, z, o), QuaternionData::fromEulerAnglesZYX(gamer->GetOrientation(), 0.0f, 0.0f), spawntm);
        gamer->SetLastTargetedGO(tempGob->GetGUID().GetCounter());
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_make_camp::Oncast);
    }
};

class spell_gen_absorb0_hitlimit1 : public SpellScriptLoader
{
    public:
        spell_gen_absorb0_hitlimit1() : SpellScriptLoader("spell_gen_absorb0_hitlimit1") { }

        class spell_gen_absorb0_hitlimit1_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_absorb0_hitlimit1_AuraScript);

        public:
            spell_gen_absorb0_hitlimit1_AuraScript()
            {
                limit = 0;
            }

        private:
            uint32 limit;

            bool Load() override
            {
                // Max absorb stored in 1 dummy effect
                limit = GetSpellInfo()->GetEffect(EFFECT_1)->CalcValue();
                return true;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
            {
                absorbAmount = std::min(limit, absorbAmount);
            }

            void Register() override
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_gen_absorb0_hitlimit1_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_absorb0_hitlimit1_AuraScript();
        }
};

// 28764 - Adaptive Warding (Frostfire Regalia Set)
enum AdaptiveWarding
{
    SPELL_GEN_ADAPTIVE_WARDING_FIRE     = 28765,
    SPELL_GEN_ADAPTIVE_WARDING_NATURE   = 28768,
    SPELL_GEN_ADAPTIVE_WARDING_FROST    = 28766,
    SPELL_GEN_ADAPTIVE_WARDING_SHADOW   = 28769,
    SPELL_GEN_ADAPTIVE_WARDING_ARCANE   = 28770
};

//28764
class spell_gen_adaptive_warding : public AuraScript
{
    PrepareAuraScript(spell_gen_adaptive_warding);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_GEN_ADAPTIVE_WARDING_FIRE,
            SPELL_GEN_ADAPTIVE_WARDING_NATURE,
            SPELL_GEN_ADAPTIVE_WARDING_FROST,
            SPELL_GEN_ADAPTIVE_WARDING_SHADOW,
            SPELL_GEN_ADAPTIVE_WARDING_ARCANE
        });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetSpellInfo())
            return false;

        // find Mage Armor
        if (!GetTarget()->GetAuraEffect(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT, SPELLFAMILY_MAGE, flag128(0x10000000, 0x0, 0x0)))
            return false;

        switch (GetFirstSchoolInMask(eventInfo.GetSchoolMask()))
        {
            case SPELL_SCHOOL_NORMAL:
            case SPELL_SCHOOL_HOLY:
            return false;
        }

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        uint32 spellId = 0;
        switch (GetFirstSchoolInMask(eventInfo.GetSchoolMask()))
        {
        case SPELL_SCHOOL_FIRE:
            spellId = SPELL_GEN_ADAPTIVE_WARDING_FIRE;
            break;
        case SPELL_SCHOOL_NATURE:
            spellId = SPELL_GEN_ADAPTIVE_WARDING_NATURE;
            break;
        case SPELL_SCHOOL_FROST:
            spellId = SPELL_GEN_ADAPTIVE_WARDING_FROST;
            break;
        case SPELL_SCHOOL_SHADOW:
            spellId = SPELL_GEN_ADAPTIVE_WARDING_SHADOW;
            break;
        case SPELL_SCHOOL_ARCANE:
            spellId = SPELL_GEN_ADAPTIVE_WARDING_ARCANE;
            break;
        default:
        return;
        }
        GetTarget()->CastSpell(GetTarget(), spellId, true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_gen_adaptive_warding::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_gen_adaptive_warding::HandleProc, EFFECT_0, SPELL_AURA_MOD_STAT);
    }
};

class spell_gen_allow_cast_from_item_only : public SpellScriptLoader
{
    public:
        spell_gen_allow_cast_from_item_only() : SpellScriptLoader("spell_gen_allow_cast_from_item_only") { }

        class spell_gen_allow_cast_from_item_only_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_allow_cast_from_item_only_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (!GetCastItem())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_allow_cast_from_item_only_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_allow_cast_from_item_only_SpellScript();
        }
};

enum AnimalBloodPoolSpell
{
    SPELL_ANIMAL_BLOOD      = 46221,
    SPELL_SPAWN_BLOOD_POOL  = 63471
};

class spell_gen_animal_blood : public SpellScriptLoader
{
    public:
        spell_gen_animal_blood() : SpellScriptLoader("spell_gen_animal_blood") { }

        class spell_gen_animal_blood_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_animal_blood_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_SPAWN_BLOOD_POOL });
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Remove all auras with spell id 46221, except the one currently being applied
                while (Aura* aur = GetUnitOwner()->GetOwnedAura(SPELL_ANIMAL_BLOOD, ObjectGuid::Empty, ObjectGuid::Empty, 0, GetAura()))
                    GetUnitOwner()->RemoveOwnedAura(aur);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* owner = GetUnitOwner())
                    if (owner->IsInWater())
                        owner->CastSpell(owner, SPELL_SPAWN_BLOOD_POOL, true);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_animal_blood_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_animal_blood_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_animal_blood_AuraScript();
        }
};

// 41337 Aura of Anger
class spell_gen_aura_of_anger : public SpellScriptLoader
{
    public:
        spell_gen_aura_of_anger() : SpellScriptLoader("spell_gen_aura_of_anger") { }

        class spell_gen_aura_of_anger_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_aura_of_anger_AuraScript);

            void HandleEffectPeriodicUpdate(AuraEffect* aurEff)
            {
                if (AuraEffect* aurEff1 = aurEff->GetBase()->GetEffect(EFFECT_1))
                    aurEff1->ChangeAmount(aurEff1->GetAmount() + 5);
                aurEff->SetAmount(100 * aurEff->GetTickNumber());
            }

            void Register() override
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_gen_aura_of_anger_AuraScript::HandleEffectPeriodicUpdate, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_aura_of_anger_AuraScript();
        }
};

enum ServiceUniform
{
    // Spells
    SPELL_SERVICE_UNIFORM       = 71450,

    // Models
    MODEL_GOBLIN_MALE           = 31002,
    MODEL_GOBLIN_FEMALE         = 31003
};

class spell_gen_aura_service_uniform : public SpellScriptLoader
{
    public:
        spell_gen_aura_service_uniform() : SpellScriptLoader("spell_gen_aura_service_uniform") { }

        class spell_gen_aura_service_uniform_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_aura_service_uniform_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_SERVICE_UNIFORM });
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Apply model goblin
                Unit* target = GetTarget();
                if (target->GetTypeId() == TYPEID_PLAYER)
                {
                    if (target->getGender() == GENDER_MALE)
                        target->SetDisplayId(MODEL_GOBLIN_MALE);
                    else
                        target->SetDisplayId(MODEL_GOBLIN_FEMALE);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (target->GetTypeId() == TYPEID_PLAYER)
                    target->RestoreDisplayId();
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_aura_service_uniform_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_aura_service_uniform_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_aura_service_uniform_AuraScript();
        }
};

class spell_gen_av_drekthar_presence : public SpellScriptLoader
{
    public:
        spell_gen_av_drekthar_presence() : SpellScriptLoader("spell_gen_av_drekthar_presence") { }

        class spell_gen_av_drekthar_presence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_av_drekthar_presence_AuraScript);

            bool CheckAreaTarget(Unit* target)
            {
                switch (target->GetEntry())
                {
                    // alliance
                    case 14762: // Dun Baldar North Marshal
                    case 14763: // Dun Baldar South Marshal
                    case 14764: // Icewing Marshal
                    case 14765: // Stonehearth Marshal
                    case 11948: // Vandar Stormspike
                    // horde
                    case 14772: // East Frostwolf Warmaster
                    case 14776: // Tower Point Warmaster
                    case 14773: // Iceblood Warmaster
                    case 14777: // West Frostwolf Warmaster
                    case 11946: // Drek'thar
                        return true;
                    default:
                        return false;
                }
            }

            void Register() override
            {
                DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_gen_av_drekthar_presence_AuraScript::CheckAreaTarget);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_av_drekthar_presence_AuraScript();
        }
};

enum GenericBandage
{
    SPELL_RECENTLY_BANDAGED     = 11196
};

class spell_gen_bandage : public SpellScriptLoader
{
    public:
        spell_gen_bandage() : SpellScriptLoader("spell_gen_bandage") { }

        class spell_gen_bandage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_bandage_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_RECENTLY_BANDAGED });
            }

            SpellCastResult CheckCast()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (target->HasAura(SPELL_RECENTLY_BANDAGED))
                        return SPELL_FAILED_TARGET_AURASTATE;
                }
                return SPELL_CAST_OK;
            }

            void HandleScript()
            {
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, SPELL_RECENTLY_BANDAGED, true);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_bandage_SpellScript::CheckCast);
                AfterHit += SpellHitFn(spell_gen_bandage_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_bandage_SpellScript();
        }
};

// Blood Reserve - 64568
enum BloodReserve
{
    SPELL_GEN_BLOOD_RESERVE_AURA = 64568,
    SPELL_GEN_BLOOD_RESERVE_HEAL = 64569
};

class spell_gen_blood_reserve : public SpellScriptLoader
{
    public:
        spell_gen_blood_reserve() : SpellScriptLoader("spell_gen_blood_reserve") { }

        class spell_gen_blood_reserve_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_blood_reserve_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_GEN_BLOOD_RESERVE_HEAL });
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                if (DamageInfo* dmgInfo = eventInfo.GetDamageInfo())
                    if (Unit* caster = eventInfo.GetActionTarget())
                        if (caster->HealthBelowPctDamaged(35, dmgInfo->GetDamage()))
                            return true;

                return false;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* caster = eventInfo.GetActionTarget();
                caster->CastCustomSpell(SPELL_GEN_BLOOD_RESERVE_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), caster, TRIGGERED_FULL_MASK, nullptr, aurEff);
                caster->RemoveAura(SPELL_GEN_BLOOD_RESERVE_AURA);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_gen_blood_reserve_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_gen_blood_reserve_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_blood_reserve_AuraScript();
        }
};

enum Bonked
{
    SPELL_BONKED            = 62991,
    SPELL_FOAM_SWORD_DEFEAT = 62994,
    SPELL_ON_GUARD          = 62972
};

class spell_gen_bonked : public SpellScriptLoader
{
    public:
        spell_gen_bonked() : SpellScriptLoader("spell_gen_bonked") { }

        class spell_gen_bonked_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_bonked_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Player* target = GetHitPlayer())
                {
                    Aura const* aura = GetHitAura();
                    if (!(aura && aura->GetStackAmount() == 3))
                        return;

                    target->CastSpell(target, SPELL_FOAM_SWORD_DEFEAT, true);
                    target->RemoveAurasDueToSpell(SPELL_BONKED);

                    if (Aura const* auraOnGuard = target->GetAura(SPELL_ON_GUARD))
                        if (Item* item = target->GetItemByGuid(auraOnGuard->GetCastItemGUID()))
                            target->DestroyItemCount(item->GetEntry(), 1, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_bonked_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_bonked_SpellScript();
        }
};

/* DOCUMENTATION: Break-Shield spells
    Break-Shield spells can be classified in three groups:

        - Spells on vehicle bar used by players:
            + EFFECT_0: SCRIPT_EFFECT
            + EFFECT_1: NONE
            + EFFECT_2: NONE
        - Spells cast by players triggered by script:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: SCRIPT_EFFECT
            + EFFECT_2: FORCE_CAST
        - Spells cast by NPCs on players:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: SCRIPT_EFFECT
            + EFFECT_2: NONE

    In the following script we handle the SCRIPT_EFFECT for effIndex EFFECT_0 and EFFECT_1.
        - When handling EFFECT_0 we're in the "Spells on vehicle bar used by players" case
          and we'll trigger "Spells cast by players triggered by script"
        - When handling EFFECT_1 we're in the "Spells cast by players triggered by script"
          or "Spells cast by NPCs on players" so we'll search for the first defend layer and drop it.
*/

enum BreakShieldSpells
{
    SPELL_BREAK_SHIELD_DAMAGE_2K                 = 62626,
    SPELL_BREAK_SHIELD_DAMAGE_10K                = 64590,

    SPELL_BREAK_SHIELD_TRIGGER_FACTION_MOUNTS    = 62575, // Also on ToC5 mounts
    SPELL_BREAK_SHIELD_TRIGGER_CAMPAING_WARHORSE = 64595,
    SPELL_BREAK_SHIELD_TRIGGER_UNK               = 66480
};

class spell_gen_break_shield : public SpellScriptLoader
{
    public:
        spell_gen_break_shield(const char* name) : SpellScriptLoader(name) { }

        class spell_gen_break_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_break_shield_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ 62552, 62719, 64100, 66482 });
            }

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();

                switch (effIndex)
                {
                    case EFFECT_0: // On spells wich trigger the damaging spell (and also the visual)
                    {
                        uint32 spellId;

                        switch (GetSpellInfo()->Id)
                        {
                            case SPELL_BREAK_SHIELD_TRIGGER_UNK:
                            case SPELL_BREAK_SHIELD_TRIGGER_CAMPAING_WARHORSE:
                                spellId = SPELL_BREAK_SHIELD_DAMAGE_10K;
                                break;
                            case SPELL_BREAK_SHIELD_TRIGGER_FACTION_MOUNTS:
                                spellId = SPELL_BREAK_SHIELD_DAMAGE_2K;
                                break;
                            default:
                                return;
                        }

                        if (Unit* rider = GetCaster()->GetCharmer())
                            rider->CastSpell(target, spellId, false);
                        else
                            GetCaster()->CastSpell(target, spellId, false);
                        break;
                    }
                    case EFFECT_1: // On damaging spells, for removing a defend layer
                    {
                        Unit::AuraApplicationMap const& auras = target->GetAppliedAuras();
                        for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                        {
                            if (Aura* aura = itr->second->GetBase())
                            {
                                if (aura->GetId() == 62552 || aura->GetId() == 62719 || aura->GetId() == 64100 || aura->GetId() == 66482)
                                {
                                    aura->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                    // Remove dummys from rider (Necessary for updating visual shields)
                                    if (Unit* rider = target->GetCharmer())
                                        if (Aura* defend = rider->GetAura(aura->GetId()))
                                            defend->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_break_shield_SpellScript::HandleScriptEffect, EFFECT_FIRST_FOUND, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_break_shield_SpellScript();
        }
};

// 46394 Brutallus Burn
class spell_gen_burn_brutallus : public SpellScriptLoader
{
    public:
        spell_gen_burn_brutallus() : SpellScriptLoader("spell_gen_burn_brutallus") { }

        class spell_gen_burn_brutallus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_burn_brutallus_AuraScript);

            void HandleEffectPeriodicUpdate(AuraEffect* aurEff)
            {
                if (aurEff->GetTickNumber() % 11 == 0)
                    aurEff->SetAmount(aurEff->GetAmount() * 2);
            }

            void Register() override
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_gen_burn_brutallus_AuraScript::HandleEffectPeriodicUpdate, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_burn_brutallus_AuraScript();
        }
};

// 48750 - Burning Depths Necrolyte Image
class spell_gen_burning_depths_necrolyte_image : public SpellScriptLoader
{
    public:
        spell_gen_burning_depths_necrolyte_image() : SpellScriptLoader("spell_gen_burning_depths_necrolyte_image") { }

        class spell_gen_burning_depths_necrolyte_image_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_burning_depths_necrolyte_image_AuraScript);

            bool Validate(SpellInfo const* spellInfo) override
            {
                return ValidateSpellInfo({ uint32(spellInfo->GetEffect(EFFECT_2)->CalcValue()) });
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(GetTarget(), uint32(GetSpellInfo()->GetEffect(EFFECT_2)->CalcValue()));
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(uint32(GetSpellInfo()->GetEffect(EFFECT_2)->CalcValue()), GetCasterGUID());
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gen_burning_depths_necrolyte_image_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_burning_depths_necrolyte_image_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_burning_depths_necrolyte_image_AuraScript();
        }
};

enum CannibalizeSpells
{
    SPELL_CANNIBALIZE_TRIGGERED = 20578
};

class spell_gen_cannibalize : public SpellScriptLoader
{
    public:
        spell_gen_cannibalize() : SpellScriptLoader("spell_gen_cannibalize") { }

        class spell_gen_cannibalize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_cannibalize_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_CANNIBALIZE_TRIGGERED });
            }

            SpellCastResult CheckIfCorpseNear()
            {
                Unit* caster = GetCaster();
                float max_range = GetSpellInfo()->GetMaxRange(false);
                WorldObject* result = nullptr;
                // search for nearby enemy corpse in range
                Trinity::AnyDeadUnitSpellTargetInRangeCheck check(caster, max_range, GetSpellInfo(), TARGET_CHECK_ENEMY);
                Trinity::WorldObjectSearcher<Trinity::AnyDeadUnitSpellTargetInRangeCheck> searcher(caster, result, check);
                Cell::VisitWorldObjects(caster, searcher, max_range);
                if (!result)
                    Cell::VisitGridObjects(caster, searcher, max_range);
                if (!result)
                    return SPELL_FAILED_NO_EDIBLE_CORPSES;
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_CANNIBALIZE_TRIGGERED, false);
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_gen_cannibalize_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_gen_cannibalize_SpellScript::CheckIfCorpseNear);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_cannibalize_SpellScript();
        }
};

enum ChaosBlast
{
    SPELL_CHAOS_BLAST   = 37675
};

class spell_gen_chaos_blast : public SpellScriptLoader
{
    public:
        spell_gen_chaos_blast() : SpellScriptLoader("spell_gen_chaos_blast") { }

        class spell_gen_chaos_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_chaos_blast_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_CHAOS_BLAST });
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                int32 basepoints0 = 100;
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                    caster->CastCustomSpell(target, SPELL_CHAOS_BLAST, &basepoints0, nullptr, nullptr, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_chaos_blast_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_chaos_blast_SpellScript();
        }
};

enum Clone
{
    SPELL_NIGHTMARE_FIGMENT_MIRROR_IMAGE        = 57528
};

class spell_gen_clone : public SpellScriptLoader
{
    public:
        spell_gen_clone() : SpellScriptLoader("spell_gen_clone") { }

        class spell_gen_clone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_clone_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
            }

            void Register() override
            {
                if (m_scriptSpellId == SPELL_NIGHTMARE_FIGMENT_MIRROR_IMAGE)
                {
                    OnEffectHitTarget += SpellEffectFn(spell_gen_clone_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
                    OnEffectHitTarget += SpellEffectFn(spell_gen_clone_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_DUMMY);
                }
                else
                {
                    OnEffectHitTarget += SpellEffectFn(spell_gen_clone_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
                    OnEffectHitTarget += SpellEffectFn(spell_gen_clone_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_clone_SpellScript();
        }
};

enum CloneWeaponSpells
{
    SPELL_COPY_WEAPON_AURA       = 41054,
    SPELL_COPY_WEAPON_2_AURA     = 63418,
    SPELL_COPY_WEAPON_3_AURA     = 69893,

    SPELL_COPY_OFFHAND_AURA      = 45205,
    SPELL_COPY_OFFHAND_2_AURA    = 69896,

    SPELL_COPY_RANGED_AURA       = 57594
};

class spell_gen_clone_weapon : public SpellScriptLoader
{
    public:
        spell_gen_clone_weapon() : SpellScriptLoader("spell_gen_clone_weapon") { }

        class spell_gen_clone_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_clone_weapon_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_clone_weapon_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_clone_weapon_SpellScript();
        }
};

class spell_gen_clone_weapon_aura : public SpellScriptLoader
{
    public:
        spell_gen_clone_weapon_aura() : SpellScriptLoader("spell_gen_clone_weapon_aura") { }

        class spell_gen_clone_weapon_auraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_clone_weapon_auraScript);

        public:
            spell_gen_clone_weapon_auraScript()
            {
                prevItem = 0;
            }

        private:
            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_COPY_WEAPON_AURA,
                    SPELL_COPY_WEAPON_2_AURA,
                    SPELL_COPY_WEAPON_3_AURA,
                    SPELL_COPY_OFFHAND_AURA,
                    SPELL_COPY_OFFHAND_2_AURA,
                    SPELL_COPY_RANGED_AURA
                });
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();
                if (!caster)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_COPY_WEAPON_AURA:
                    case SPELL_COPY_WEAPON_2_AURA:
                    case SPELL_COPY_WEAPON_3_AURA:
                    {
                        prevItem = target->GetVirtualItemId(0);

                        if (Player* player = caster->ToPlayer())
                        {
                            if (Item* mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                                target->SetVirtualItem(0, mainItem->GetEntry());
                        }
                        else
                            target->SetVirtualItem(0, caster->GetVirtualItemId(0));
                        break;
                    }
                    case SPELL_COPY_OFFHAND_AURA:
                    case SPELL_COPY_OFFHAND_2_AURA:
                    {
                        prevItem = target->GetVirtualItemId(1);

                        if (Player* player = caster->ToPlayer())
                        {
                            if (Item* offItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                                target->SetVirtualItem(1, offItem->GetEntry());
                        }
                        else
                            target->SetVirtualItem(1, caster->GetVirtualItemId(1));
                        break;
                    }
                    case SPELL_COPY_RANGED_AURA:
                    {
                        prevItem = target->GetVirtualItemId(2);

                        if (Player* player = caster->ToPlayer())
                        {
                            if (Item* rangedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                                target->SetVirtualItem(2, rangedItem->GetEntry());
                        }
                        else
                            target->SetVirtualItem(2, caster->GetVirtualItemId(2));
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_COPY_WEAPON_AURA:
                    case SPELL_COPY_WEAPON_2_AURA:
                    case SPELL_COPY_WEAPON_3_AURA:
                        target->SetVirtualItem(0, prevItem);
                        break;
                    case SPELL_COPY_OFFHAND_AURA:
                    case SPELL_COPY_OFFHAND_2_AURA:
                        target->SetVirtualItem(1, prevItem);
                        break;
                    case SPELL_COPY_RANGED_AURA:
                        target->SetVirtualItem(2, prevItem);
                        break;
                    default:
                        break;
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_clone_weapon_auraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_clone_weapon_auraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }

        private:
            uint32 prevItem;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_clone_weapon_auraScript();
        }
};

class spell_gen_count_pct_from_max_hp : public SpellScriptLoader
{
    public:
        spell_gen_count_pct_from_max_hp(char const* name, int32 damagePct = 0) : SpellScriptLoader(name), _damagePct(damagePct) { }

        class spell_gen_count_pct_from_max_hp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_count_pct_from_max_hp_SpellScript);

        public:
            spell_gen_count_pct_from_max_hp_SpellScript(int32 damagePct) : SpellScript(), _damagePct(damagePct) { }

            void RecalculateDamage()
            {
                if (!_damagePct)
                    _damagePct = GetHitDamage();

                SetHitDamage(GetHitUnit()->CountPctFromMaxHealth(_damagePct));
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_gen_count_pct_from_max_hp_SpellScript::RecalculateDamage);
            }

        private:
            int32 _damagePct;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_count_pct_from_max_hp_SpellScript(_damagePct);
        }

    private:
        int32 _damagePct;
};

// 63845 - Create Lance
enum CreateLanceSpells
{
    SPELL_CREATE_LANCE_ALLIANCE = 63914,
    SPELL_CREATE_LANCE_HORDE    = 63919
};

class spell_gen_create_lance : public SpellScriptLoader
{
    public:
        spell_gen_create_lance() : SpellScriptLoader("spell_gen_create_lance") { }

        class spell_gen_create_lance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_create_lance_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_CREATE_LANCE_ALLIANCE, SPELL_CREATE_LANCE_HORDE });
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (Player* target = GetHitPlayer())
                {
                    if (target->GetTeam() == ALLIANCE)
                        GetCaster()->CastSpell(target, SPELL_CREATE_LANCE_ALLIANCE, true);
                    else
                        GetCaster()->CastSpell(target, SPELL_CREATE_LANCE_HORDE, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_create_lance_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_create_lance_SpellScript();
        }
};

class spell_gen_creature_permanent_feign_death : public SpellScriptLoader
{
    public:
        spell_gen_creature_permanent_feign_death() : SpellScriptLoader("spell_gen_creature_permanent_feign_death") { }

        class spell_gen_creature_permanent_feign_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_creature_permanent_feign_death_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->SetDynamicFlags(UNIT_DYNFLAG_DEAD);
                target->AddUnitFlag2(UNIT_FLAG2_FEIGN_DEATH);

                if (target->GetTypeId() == TYPEID_UNIT)
                    target->ToCreature()->SetReactState(REACT_PASSIVE);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->RemoveDynamicFlag(UNIT_DYNFLAG_DEAD);
                target->RemoveUnitFlag2(UNIT_FLAG2_FEIGN_DEATH);
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_creature_permanent_feign_death_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_creature_permanent_feign_death_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_creature_permanent_feign_death_AuraScript();
        }
};

enum DalaranDisguiseSpells
{
    SPELL_SUNREAVER_DISGUISE_TRIGGER       = 69672,
    SPELL_SUNREAVER_DISGUISE_FEMALE        = 70973,
    SPELL_SUNREAVER_DISGUISE_MALE          = 70974,

    SPELL_SILVER_COVENANT_DISGUISE_TRIGGER = 69673,
    SPELL_SILVER_COVENANT_DISGUISE_FEMALE  = 70971,
    SPELL_SILVER_COVENANT_DISGUISE_MALE    = 70972
};

class spell_gen_dalaran_disguise : public SpellScriptLoader
{
    public:
        spell_gen_dalaran_disguise(const char* name) : SpellScriptLoader(name) { }

        class spell_gen_dalaran_disguise_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_dalaran_disguise_SpellScript);

            bool Validate(SpellInfo const* spellInfo) override
            {
                switch (spellInfo->Id)
                {
                    case SPELL_SUNREAVER_DISGUISE_TRIGGER:
                        return ValidateSpellInfo({ SPELL_SUNREAVER_DISGUISE_FEMALE, SPELL_SUNREAVER_DISGUISE_MALE });
                    case SPELL_SILVER_COVENANT_DISGUISE_TRIGGER:
                        return ValidateSpellInfo({ SPELL_SILVER_COVENANT_DISGUISE_FEMALE, SPELL_SILVER_COVENANT_DISGUISE_MALE });
                }
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetHitPlayer())
                {
                    uint8 gender = player->getGender();

                    uint32 spellId = GetSpellInfo()->Id;

                    switch (spellId)
                    {
                        case SPELL_SUNREAVER_DISGUISE_TRIGGER:
                            spellId = gender == GENDER_FEMALE ? SPELL_SUNREAVER_DISGUISE_FEMALE : SPELL_SUNREAVER_DISGUISE_MALE;
                            break;
                        case SPELL_SILVER_COVENANT_DISGUISE_TRIGGER:
                            spellId = gender == GENDER_FEMALE ? SPELL_SILVER_COVENANT_DISGUISE_FEMALE : SPELL_SILVER_COVENANT_DISGUISE_MALE;
                            break;
                        default:
                            break;
                    }

                    GetCaster()->CastSpell(player, spellId, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_dalaran_disguise_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_dalaran_disguise_SpellScript();
        }
};

enum DefendVisuals
{
    SPELL_VISUAL_SHIELD_1 = 63130,
    SPELL_VISUAL_SHIELD_2 = 63131,
    SPELL_VISUAL_SHIELD_3 = 63132
};

class spell_gen_defend : public SpellScriptLoader
{
    public:
        spell_gen_defend() : SpellScriptLoader("spell_gen_defend") { }

        class spell_gen_defend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_defend_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_VISUAL_SHIELD_1, SPELL_VISUAL_SHIELD_2, SPELL_VISUAL_SHIELD_3 });
            }

            void RefreshVisualShields(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                {
                    Unit* target = GetTarget();

                    for (uint8 i = 0; i < GetSpellInfo()->StackAmount; ++i)
                        target->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1 + i);

                    target->CastSpell(target, SPELL_VISUAL_SHIELD_1 + GetAura()->GetStackAmount() - 1, true, nullptr, aurEff);
                }
                else
                    GetTarget()->RemoveAurasDueToSpell(GetId());
            }

            void RemoveVisualShields(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                for (uint8 i = 0; i < GetSpellInfo()->StackAmount; ++i)
                    GetTarget()->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1 + i);
            }

            void RemoveDummyFromDriver(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (TempSummon* vehicle = caster->ToTempSummon())
                        if (Unit* rider = vehicle->GetSummoner())
                            rider->RemoveAurasDueToSpell(GetId());
            }

            void Register() override
            {
                /*
                SpellInfo const* spell = sSpellMgr->AssertSpellInfo(m_scriptSpellId);

                // 6.x effects removed

                // Defend spells cast by NPCs (add visuals)
                if (spell->GetEffect(EFFECT_0)->ApplyAuraName == SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN)
                {
                    AfterEffectApply += AuraEffectApplyFn(spell_gen_defend_AuraScript::RefreshVisualShields, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    OnEffectRemove += AuraEffectRemoveFn(spell_gen_defend_AuraScript::RemoveVisualShields, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                }

                // Remove Defend spell from player when he dismounts
                if (spell->GetEffect(EFFECT_2)->ApplyAuraName == SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN)
                    OnEffectRemove += AuraEffectRemoveFn(spell_gen_defend_AuraScript::RemoveDummyFromDriver, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);

                // Defend spells cast by players (add/remove visuals)
                if (spell->GetEffect(EFFECT_1)->ApplyAuraName == SPELL_AURA_DUMMY)
                {
                    AfterEffectApply += AuraEffectApplyFn(spell_gen_defend_AuraScript::RefreshVisualShields, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    OnEffectRemove += AuraEffectRemoveFn(spell_gen_defend_AuraScript::RemoveVisualShields, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                }
                */
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_defend_AuraScript();
        }
};

class spell_gen_despawn_self : public SpellScriptLoader
{
    public:
        spell_gen_despawn_self() : SpellScriptLoader("spell_gen_despawn_self") { }

        class spell_gen_despawn_self_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_despawn_self_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_UNIT;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetEffectInfo()->IsEffect(SPELL_EFFECT_DUMMY) || GetEffectInfo()->IsEffect(SPELL_EFFECT_SCRIPT_EFFECT))
                    GetCaster()->ToCreature()->DespawnOrUnsummon();
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_despawn_self_SpellScript::HandleDummy, EFFECT_ALL, SPELL_EFFECT_ANY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_despawn_self_SpellScript();
        }
};

enum DivineStormSpell
{
    SPELL_DIVINE_STORM      = 53385,
};

// 70769 Divine Storm!
class spell_gen_divine_storm_cd_reset : public SpellScriptLoader
{
    public:
        spell_gen_divine_storm_cd_reset() : SpellScriptLoader("spell_gen_divine_storm_cd_reset") { }

        class spell_gen_divine_storm_cd_reset_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_divine_storm_cd_reset_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_DIVINE_STORM });
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->GetSpellHistory()->ResetCooldown(SPELL_DIVINE_STORM, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_divine_storm_cd_reset_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_divine_storm_cd_reset_SpellScript();
        }
};

class spell_gen_ds_flush_knockback : public SpellScriptLoader
{
    public:
        spell_gen_ds_flush_knockback() : SpellScriptLoader("spell_gen_ds_flush_knockback") { }

        class spell_gen_ds_flush_knockback_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_ds_flush_knockback_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                // Here the target is the water spout and determines the position where the player is knocked from
                if (Unit* target = GetHitUnit())
                {
                    if (Player* player = GetCaster()->ToPlayer())
                    {
                        float horizontalSpeed = 20.0f + (40.0f - GetCaster()->GetDistance(target));
                        float verticalSpeed = 8.0f;
                        // This method relies on the Dalaran Sewer map disposition and Water Spout position
                        // What we do is knock the player from a position exactly behind him and at the end of the pipe
                        player->KnockbackFrom(target->GetPositionX(), player->GetPositionY(), horizontalSpeed, verticalSpeed);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_ds_flush_knockback_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_ds_flush_knockback_SpellScript();
        }
};

class spell_gen_dungeon_credit : public SpellScriptLoader
{
    public:
        spell_gen_dungeon_credit() : SpellScriptLoader("spell_gen_dungeon_credit") { }

        class spell_gen_dungeon_credit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_dungeon_credit_SpellScript);

        public:
            spell_gen_dungeon_credit_SpellScript()
            {
                _handled = false;
            }

        private:
            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_UNIT;
            }

            void CreditEncounter()
            {
                // This hook is executed for every target, make sure we only credit instance once
                if (_handled)
                    return;

                _handled = true;
                Unit* caster = GetCaster();
                if (InstanceScript* instance = caster->GetInstanceScript())
                    instance->UpdateEncounterStateForSpellCast(GetSpellInfo()->Id, caster);
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_gen_dungeon_credit_SpellScript::CreditEncounter);
            }

        private:
            bool _handled;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_dungeon_credit_SpellScript();
        }
};

enum EluneCandle
{
    // Creatures
    NPC_OMEN                       = 15467,

    // Spells
    SPELL_ELUNE_CANDLE_OMEN_HEAD   = 26622,
    SPELL_ELUNE_CANDLE_OMEN_CHEST  = 26624,
    SPELL_ELUNE_CANDLE_OMEN_HAND_R = 26625,
    SPELL_ELUNE_CANDLE_OMEN_HAND_L = 26649,
    SPELL_ELUNE_CANDLE_NORMAL      = 26636
};

class spell_gen_elune_candle : public SpellScriptLoader
{
    public:
        spell_gen_elune_candle() : SpellScriptLoader("spell_gen_elune_candle") { }

        class spell_gen_elune_candle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_elune_candle_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_ELUNE_CANDLE_OMEN_HEAD,
                    SPELL_ELUNE_CANDLE_OMEN_CHEST,
                    SPELL_ELUNE_CANDLE_OMEN_HAND_R,
                    SPELL_ELUNE_CANDLE_OMEN_HAND_L,
                    SPELL_ELUNE_CANDLE_NORMAL
                });
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId = 0;

                if (GetHitUnit()->GetEntry() == NPC_OMEN)
                {
                    switch (urand(0, 3))
                    {
                        case 0:
                            spellId = SPELL_ELUNE_CANDLE_OMEN_HEAD;
                            break;
                        case 1:
                            spellId = SPELL_ELUNE_CANDLE_OMEN_CHEST;
                            break;
                        case 2:
                            spellId = SPELL_ELUNE_CANDLE_OMEN_HAND_R;
                            break;
                        case 3:
                            spellId = SPELL_ELUNE_CANDLE_OMEN_HAND_L;
                            break;
                    }
                }
                else
                    spellId = SPELL_ELUNE_CANDLE_NORMAL;

                GetCaster()->CastSpell(GetHitUnit(), spellId, true, nullptr);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_elune_candle_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_elune_candle_SpellScript();
        }
};

enum FishingSpells
{
    SPELL_FISHING_NO_FISHING_POLE   = 131476,
    SPELL_FISHING_WITH_POLE         = 131490
};

// 131474 - Fishing
class spell_gen_fishing : public SpellScriptLoader
{
public:
    spell_gen_fishing() : SpellScriptLoader("spell_gen_fishing") { }

    class spell_gen_fishing_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_fishing_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_FISHING_NO_FISHING_POLE, SPELL_FISHING_WITH_POLE });
        }

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            uint32 spellId;
            Item* mainHand = GetCaster()->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            if (!mainHand || mainHand->GetTemplate()->GetClass() != ITEM_CLASS_WEAPON || mainHand->GetTemplate()->GetSubClass() != ITEM_SUBCLASS_WEAPON_FISHING_POLE)
                spellId = SPELL_FISHING_NO_FISHING_POLE;
            else
                spellId = SPELL_FISHING_WITH_POLE;

            GetCaster()->CastSpell(GetCaster(), spellId, false);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_fishing_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_fishing_SpellScript();
    }
};

enum TransporterBackfires
{
    SPELL_TRANSPORTER_MALFUNCTION_POLYMORPH     = 23444,
    SPELL_TRANSPORTER_EVIL_TWIN                 = 23445,
    SPELL_TRANSPORTER_MALFUNCTION_MISS          = 36902
};

class spell_gen_gadgetzan_transporter_backfire : public SpellScriptLoader
{
    public:
        spell_gen_gadgetzan_transporter_backfire() : SpellScriptLoader("spell_gen_gadgetzan_transporter_backfire") { }

        class spell_gen_gadgetzan_transporter_backfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_gadgetzan_transporter_backfire_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_TRANSPORTER_MALFUNCTION_POLYMORPH,
                    SPELL_TRANSPORTER_EVIL_TWIN,
                    SPELL_TRANSPORTER_MALFUNCTION_MISS
                });
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                int32 r = irand(0, 119);
                if (r < 20)                           // Transporter Malfunction - 1/6 polymorph
                    caster->CastSpell(caster, SPELL_TRANSPORTER_MALFUNCTION_POLYMORPH, true);
                else if (r < 100)                     // Evil Twin               - 4/6 evil twin
                    caster->CastSpell(caster, SPELL_TRANSPORTER_EVIL_TWIN, true);
                else                                    // Transporter Malfunction - 1/6 miss the target
                    caster->CastSpell(caster, SPELL_TRANSPORTER_MALFUNCTION_MISS, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_gadgetzan_transporter_backfire_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_gadgetzan_transporter_backfire_SpellScript();
        }
};


class spell_gen_gift_of_naaru : public SpellScriptLoader
{
    public:
        spell_gen_gift_of_naaru() : SpellScriptLoader("spell_gen_gift_of_naaru") { }

        class spell_gen_gift_of_naaru_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_gift_of_naaru_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (!GetCaster())
                    return;

                float heal = 0.0f;
                switch (GetSpellInfo()->SpellFamilyName)
                {
                    case SPELLFAMILY_MAGE:
                    case SPELLFAMILY_WARLOCK:
                    case SPELLFAMILY_PRIEST:
                        heal = 1.885f * float(GetCaster()->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()));
                        break;
                    case SPELLFAMILY_PALADIN:
                    case SPELLFAMILY_SHAMAN:
                        heal = std::max(1.885f * float(GetCaster()->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask())), 1.1f * float(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK)));
                        break;
                    case SPELLFAMILY_WARRIOR:
                    case SPELLFAMILY_HUNTER:
                    case SPELLFAMILY_DEATHKNIGHT:
                        heal = 1.1f * float(std::max(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK)));
                        break;
                    case SPELLFAMILY_GENERIC:
                    default:
                        break;
                }

                int32 healTick = std::floor(heal / aurEff->GetTotalTicks());
                amount += int32(std::max(healTick, 0));
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_gift_of_naaru_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_gift_of_naaru_AuraScript();
        }
};

enum GnomishTransporter
{
    SPELL_TRANSPORTER_SUCCESS                   = 23441,
    SPELL_TRANSPORTER_FAILURE                   = 23446
};

class spell_gen_gnomish_transporter : public SpellScriptLoader
{
    public:
        spell_gen_gnomish_transporter() : SpellScriptLoader("spell_gen_gnomish_transporter") { }

        class spell_gen_gnomish_transporter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_gnomish_transporter_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_TRANSPORTER_SUCCESS, SPELL_TRANSPORTER_FAILURE });
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                GetCaster()->CastSpell(GetCaster(), roll_chance_i(50) ? SPELL_TRANSPORTER_SUCCESS : SPELL_TRANSPORTER_FAILURE, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_gnomish_transporter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_gnomish_transporter_SpellScript();
        }
};


enum Interrupt
{
    SPELL_GEN_THROW_INTERRUPT           = 32747
};

// 32748 - Deadly Throw Interrupt
// 44835 - Maim Interrupt
class spell_gen_interrupt : public SpellScriptLoader
{
    public:
        spell_gen_interrupt() : SpellScriptLoader("spell_gen_interrupt") { }

        class spell_gen_interrupt_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_interrupt_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_GEN_THROW_INTERRUPT });
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_GEN_THROW_INTERRUPT, true, nullptr, aurEff);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_gen_interrupt_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_interrupt_AuraScript();
        }
};

class spell_gen_increase_stats_buff : public SpellScriptLoader
{
    public:
        spell_gen_increase_stats_buff(char const* scriptName) : SpellScriptLoader(scriptName) { }

        class spell_gen_increase_stats_buff_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_increase_stats_buff_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit()->IsInRaidWith(GetCaster()))
                    GetCaster()->CastSpell(GetCaster(), GetEffectValue() + 1, true); // raid buff
                else
                    GetCaster()->CastSpell(GetHitUnit(), GetEffectValue(), true); // single-target buff
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_increase_stats_buff_SpellScript::HandleDummy, EFFECT_0, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_increase_stats_buff_SpellScript();
        }
};

enum GenericLifebloom
{
    SPELL_HEXLORD_MALACRASS_LIFEBLOOM_FINAL_HEAL        = 43422,
    SPELL_TUR_RAGEPAW_LIFEBLOOM_FINAL_HEAL              = 52552,
    SPELL_CENARION_SCOUT_LIFEBLOOM_FINAL_HEAL           = 53692,
    SPELL_TWISTED_VISAGE_LIFEBLOOM_FINAL_HEAL           = 57763,
    SPELL_FACTION_CHAMPIONS_DRU_LIFEBLOOM_FINAL_HEAL    = 66094
};

class spell_gen_lifebloom : public SpellScriptLoader
{
    public:
        spell_gen_lifebloom(const char* name, uint32 spellId) : SpellScriptLoader(name), _spellId(spellId) { }

        class spell_gen_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_lifebloom_AuraScript);

        public:
            spell_gen_lifebloom_AuraScript(uint32 spellId) : AuraScript(), _spellId(spellId) { }

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ _spellId });
            }

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE && GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
                    return;

                // final heal
                GetTarget()->CastSpell(GetTarget(), _spellId, true, nullptr, aurEff, GetCasterGUID());
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_lifebloom_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }

        private:
            uint32 _spellId;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_lifebloom_AuraScript(_spellId);
        }

    private:
        uint32 _spellId;
};

/* DOCUMENTATION: Charge spells
    Charge spells can be classified in four groups:

        - Spells on vehicle bar used by players:
            + EFFECT_0: SCRIPT_EFFECT
            + EFFECT_1: TRIGGER_SPELL
            + EFFECT_2: NONE
        - Spells cast by player's mounts triggered by script:
            + EFFECT_0: CHARGE
            + EFFECT_1: TRIGGER_SPELL
            + EFFECT_2: APPLY_AURA
        - Spells cast by players on the target triggered by script:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: SCRIPT_EFFECT
            + EFFECT_2: NONE
        - Spells cast by NPCs on players:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: CHARGE
            + EFFECT_2: SCRIPT_EFFECT

    In the following script we handle the SCRIPT_EFFECT and CHARGE
        - When handling SCRIPT_EFFECT:
            + EFFECT_0: Corresponds to "Spells on vehicle bar used by players" and we make player's mount cast
              the charge effect on the current target ("Spells cast by player's mounts triggered by script").
            + EFFECT_1 and EFFECT_2: Triggered when "Spells cast by player's mounts triggered by script" hits target,
              corresponding to "Spells cast by players on the target triggered by script" and "Spells cast by
              NPCs on players" and we check Defend layers and drop a charge of the first found.
        - When handling CHARGE:
            + Only launched for "Spells cast by player's mounts triggered by script", makes the player cast the
              damaging spell on target with a small chance of failing it.
*/

enum ChargeSpells
{
    SPELL_CHARGE_DAMAGE_8K5             = 62874,
    SPELL_CHARGE_DAMAGE_20K             = 68498,
    SPELL_CHARGE_DAMAGE_45K             = 64591,

    SPELL_CHARGE_CHARGING_EFFECT_8K5    = 63661,
    SPELL_CHARGE_CHARGING_EFFECT_20K_1  = 68284,
    SPELL_CHARGE_CHARGING_EFFECT_20K_2  = 68501,
    SPELL_CHARGE_CHARGING_EFFECT_45K_1  = 62563,
    SPELL_CHARGE_CHARGING_EFFECT_45K_2  = 66481,

    SPELL_CHARGE_TRIGGER_FACTION_MOUNTS = 62960,
    SPELL_CHARGE_TRIGGER_TRIAL_CHAMPION = 68282,

    SPELL_CHARGE_MISS_EFFECT            = 62977,
};

class spell_gen_mounted_charge: public SpellScriptLoader
{
    public:
        spell_gen_mounted_charge() : SpellScriptLoader("spell_gen_mounted_charge") { }

        class spell_gen_mounted_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_mounted_charge_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ 62552, 62719, 64100, 66482 });
            }

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();

                switch (effIndex)
                {
                    case EFFECT_0: // On spells wich trigger the damaging spell (and also the visual)
                    {
                        uint32 spellId;

                        switch (GetSpellInfo()->Id)
                        {
                            case SPELL_CHARGE_TRIGGER_TRIAL_CHAMPION:
                                spellId = SPELL_CHARGE_CHARGING_EFFECT_20K_1;
                                break;
                            case SPELL_CHARGE_TRIGGER_FACTION_MOUNTS:
                                spellId = SPELL_CHARGE_CHARGING_EFFECT_8K5;
                                break;
                            default:
                                return;
                        }

                        // If target isn't a training dummy there's a chance of failing the charge
                        if (!target->IsCharmedOwnedByPlayerOrPlayer() && roll_chance_f(12.5f))
                            spellId = SPELL_CHARGE_MISS_EFFECT;

                        if (Unit* vehicle = GetCaster()->GetVehicleBase())
                            vehicle->CastSpell(target, spellId, false);
                        else
                            GetCaster()->CastSpell(target, spellId, false);
                        break;
                    }
                    case EFFECT_1: // On damaging spells, for removing a defend layer
                    case EFFECT_2:
                    {
                        Unit::AuraApplicationMap const& auras = target->GetAppliedAuras();
                        for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                        {
                            if (Aura* aura = itr->second->GetBase())
                            {
                                if (aura->GetId() == 62552 || aura->GetId() == 62719 || aura->GetId() == 64100 || aura->GetId() == 66482)
                                {
                                    aura->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                    // Remove dummys from rider (Necessary for updating visual shields)
                                    if (Unit* rider = target->GetCharmer())
                                        if (Aura* defend = rider->GetAura(aura->GetId()))
                                            defend->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void HandleChargeEffect(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_CHARGE_CHARGING_EFFECT_8K5:
                        spellId = SPELL_CHARGE_DAMAGE_8K5;
                        break;
                    case SPELL_CHARGE_CHARGING_EFFECT_20K_1:
                    case SPELL_CHARGE_CHARGING_EFFECT_20K_2:
                        spellId = SPELL_CHARGE_DAMAGE_20K;
                        break;
                    case SPELL_CHARGE_CHARGING_EFFECT_45K_1:
                    case SPELL_CHARGE_CHARGING_EFFECT_45K_2:
                        spellId = SPELL_CHARGE_DAMAGE_45K;
                        break;
                    default:
                        return;
                }

                if (Unit* rider = GetCaster()->GetCharmer())
                    rider->CastSpell(GetHitUnit(), spellId, false);
                else
                    GetCaster()->CastSpell(GetHitUnit(), spellId, false);
            }

            void Register() override
            {
                SpellInfo const* spell = sSpellMgr->AssertSpellInfo(m_scriptSpellId);

                if (spell->HasEffect(DIFFICULTY_NONE, SPELL_EFFECT_SCRIPT_EFFECT))
                    OnEffectHitTarget += SpellEffectFn(spell_gen_mounted_charge_SpellScript::HandleScriptEffect, EFFECT_FIRST_FOUND, SPELL_EFFECT_SCRIPT_EFFECT);

                if (spell->GetEffect(EFFECT_0)->Effect == SPELL_EFFECT_CHARGE)
                    OnEffectHitTarget += SpellEffectFn(spell_gen_mounted_charge_SpellScript::HandleChargeEffect, EFFECT_0, SPELL_EFFECT_CHARGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_mounted_charge_SpellScript();
        }
};

enum MossCoveredFeet
{
    SPELL_FALL_DOWN = 6869
};

// 6870 Moss Covered Feet
// 31399 Moss Covered Feet
class spell_gen_moss_covered_feet : public SpellScriptLoader
{
public:
    spell_gen_moss_covered_feet() : SpellScriptLoader("spell_gen_moss_covered_feet") { }

    class spell_gen_moss_covered_feet_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_moss_covered_feet_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_FALL_DOWN });
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            eventInfo.GetActionTarget()->CastSpell((Unit*)nullptr, SPELL_FALL_DOWN, true, nullptr, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_gen_moss_covered_feet_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_moss_covered_feet_AuraScript();
    }
};

enum Netherbloom : uint32
{
    SPELL_NETHERBLOOM_POLLEN_1      = 28703
};

// 28702 - Netherbloom
class spell_gen_netherbloom : public SpellScriptLoader
{
    public:
        spell_gen_netherbloom() : SpellScriptLoader("spell_gen_netherbloom") { }

        class spell_gen_netherbloom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_netherbloom_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                for (uint8 i = 0; i < 5; ++i)
                    if (!ValidateSpellInfo({ SPELL_NETHERBLOOM_POLLEN_1 + i }))
                        return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (Unit* target = GetHitUnit())
                {
                    // 25% chance of casting a random buff
                    if (roll_chance_i(75))
                        return;

                    // triggered spells are 28703 to 28707
                    // Note: some sources say, that there was the possibility of
                    //       receiving a debuff. However, this seems to be removed by a patch.

                    // don't overwrite an existing aura
                    for (uint8 i = 0; i < 5; ++i)
                        if (target->HasAura(SPELL_NETHERBLOOM_POLLEN_1 + i))
                            return;

                    target->CastSpell(target, SPELL_NETHERBLOOM_POLLEN_1 + urand(0, 4), true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_netherbloom_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_netherbloom_SpellScript();
        }
};

enum NightmareVine
{
    SPELL_NIGHTMARE_POLLEN      = 28721
};

// 28720 - Nightmare Vine
class spell_gen_nightmare_vine : public SpellScriptLoader
{
    public:
        spell_gen_nightmare_vine() : SpellScriptLoader("spell_gen_nightmare_vine") { }

        class spell_gen_nightmare_vine_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_nightmare_vine_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_NIGHTMARE_POLLEN });
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (Unit* target = GetHitUnit())
                {
                    // 25% chance of casting Nightmare Pollen
                    if (roll_chance_i(25))
                        target->CastSpell(target, SPELL_NIGHTMARE_POLLEN, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_nightmare_vine_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_nightmare_vine_SpellScript();
        }
};

enum ObsidianArmor
{
    SPELL_GEN_OBSIDIAN_ARMOR_HOLY       = 27536,
    SPELL_GEN_OBSIDIAN_ARMOR_FIRE       = 27533,
    SPELL_GEN_OBSIDIAN_ARMOR_NATURE     = 27538,
    SPELL_GEN_OBSIDIAN_ARMOR_FROST      = 27534,
    SPELL_GEN_OBSIDIAN_ARMOR_SHADOW     = 27535,
    SPELL_GEN_OBSIDIAN_ARMOR_ARCANE     = 27540
};

// 27539 - Obsidian Armor
class spell_gen_obsidian_armor : public SpellScriptLoader
{
    public:
        spell_gen_obsidian_armor() : SpellScriptLoader("spell_gen_obsidian_armor") { }

        class spell_gen_obsidian_armor_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_obsidian_armor_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                {
                    SPELL_GEN_OBSIDIAN_ARMOR_HOLY,
                    SPELL_GEN_OBSIDIAN_ARMOR_FIRE,
                    SPELL_GEN_OBSIDIAN_ARMOR_NATURE,
                    SPELL_GEN_OBSIDIAN_ARMOR_FROST,
                    SPELL_GEN_OBSIDIAN_ARMOR_SHADOW,
                    SPELL_GEN_OBSIDIAN_ARMOR_ARCANE
                });
            }

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                if (!eventInfo.GetSpellInfo())
                    return false;

                if (GetFirstSchoolInMask(eventInfo.GetSchoolMask()) == SPELL_SCHOOL_NORMAL)
                    return false;

                return true;
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                uint32 spellId = 0;
                switch (GetFirstSchoolInMask(eventInfo.GetSchoolMask()))
                {
                    case SPELL_SCHOOL_HOLY:
                        spellId = SPELL_GEN_OBSIDIAN_ARMOR_HOLY;
                        break;
                    case SPELL_SCHOOL_FIRE:
                        spellId = SPELL_GEN_OBSIDIAN_ARMOR_FIRE;
                        break;
                    case SPELL_SCHOOL_NATURE:
                        spellId = SPELL_GEN_OBSIDIAN_ARMOR_NATURE;
                        break;
                    case SPELL_SCHOOL_FROST:
                        spellId = SPELL_GEN_OBSIDIAN_ARMOR_FROST;
                        break;
                    case SPELL_SCHOOL_SHADOW:
                        spellId = SPELL_GEN_OBSIDIAN_ARMOR_SHADOW;
                        break;
                    case SPELL_SCHOOL_ARCANE:
                        spellId = SPELL_GEN_OBSIDIAN_ARMOR_ARCANE;
                        break;
                    default:
                        return;
                }
                GetTarget()->CastSpell(GetTarget(), spellId, true, nullptr, aurEff);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_gen_obsidian_armor_AuraScript::CheckProc);
                OnEffectProc += AuraEffectProcFn(spell_gen_obsidian_armor_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_obsidian_armor_AuraScript();
        }
};

class spell_gen_oracle_wolvar_reputation : public SpellScriptLoader
{
    public:
        spell_gen_oracle_wolvar_reputation() : SpellScriptLoader("spell_gen_oracle_wolvar_reputation") { }

        class spell_gen_oracle_wolvar_reputation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_oracle_wolvar_reputation_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                Player* player = GetCaster()->ToPlayer();
                uint32 factionId = GetEffectInfo(effIndex)->CalcValue();
                int32  repChange = GetEffectInfo(EFFECT_1)->CalcValue();

                FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);
                if (!factionEntry)
                    return;

                // Set rep to baserep + basepoints (expecting spillover for oposite faction -> become hated)
                // Not when player already has equal or higher rep with this faction
                if (player->GetReputationMgr().GetReputation(factionEntry) < repChange)
                    player->GetReputationMgr().SetReputation(factionEntry, repChange);

                // EFFECT_INDEX_2 most likely update at war state, we already handle this in SetReputation
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_gen_oracle_wolvar_reputation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_oracle_wolvar_reputation_SpellScript();
        }
};

enum OrcDisguiseSpells
{
    SPELL_ORC_DISGUISE_TRIGGER       = 45759,
    SPELL_ORC_DISGUISE_MALE          = 45760,
    SPELL_ORC_DISGUISE_FEMALE        = 45762
};

class spell_gen_orc_disguise : public SpellScriptLoader
{
    public:
        spell_gen_orc_disguise() : SpellScriptLoader("spell_gen_orc_disguise") { }

        class spell_gen_orc_disguise_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_orc_disguise_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_ORC_DISGUISE_TRIGGER, SPELL_ORC_DISGUISE_MALE, SPELL_ORC_DISGUISE_FEMALE });
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Player* target = GetHitPlayer())
                {
                    uint8 gender = target->getGender();
                    if (!gender)
                        caster->CastSpell(target, SPELL_ORC_DISGUISE_MALE, true);
                    else
                        caster->CastSpell(target, SPELL_ORC_DISGUISE_FEMALE, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_orc_disguise_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_orc_disguise_SpellScript();
        }
};

enum ParalyticPoison
{
    SPELL_PARALYSIS = 35202
};

// 35201 - Paralytic Poison
class spell_gen_paralytic_poison : public SpellScriptLoader
{
    public:
        spell_gen_paralytic_poison() : SpellScriptLoader("spell_gen_paralytic_poison") { }

        class spell_gen_paralytic_poison_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_paralytic_poison_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_PARALYSIS });
            }

            void HandleStun(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                GetTarget()->CastSpell((Unit*)nullptr, SPELL_PARALYSIS, true, nullptr, aurEff);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_paralytic_poison_AuraScript::HandleStun, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_paralytic_poison_AuraScript();
        }
};

class spell_gen_proc_below_pct_damaged : public SpellScriptLoader
{
    public:
        spell_gen_proc_below_pct_damaged(const char* name) : SpellScriptLoader(name) { }

        class spell_gen_proc_below_pct_damaged_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_proc_below_pct_damaged_AuraScript);

            bool CheckProc(ProcEventInfo& eventInfo)
            {
                DamageInfo* damageInfo = eventInfo.GetDamageInfo();
                if (!damageInfo || !damageInfo->GetDamage())
                    return false;

                int32 pct = GetSpellInfo()->GetEffect(EFFECT_0)->CalcValue();

                if (eventInfo.GetActionTarget()->HealthBelowPctDamaged(pct, damageInfo->GetDamage()))
                    return true;

                return false;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_gen_proc_below_pct_damaged_AuraScript::CheckProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_proc_below_pct_damaged_AuraScript();
        }
};

enum ParachuteSpells
{
    SPELL_PARACHUTE         = 45472,
    SPELL_PARACHUTE_BUFF    = 44795,
};

// 45472 Parachute
class spell_gen_parachute : public SpellScriptLoader
{
    public:
        spell_gen_parachute() : SpellScriptLoader("spell_gen_parachute") { }

        class spell_gen_parachute_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_parachute_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_PARACHUTE, SPELL_PARACHUTE_BUFF });
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    if (target->IsFalling())
                    {
                        target->RemoveAurasDueToSpell(SPELL_PARACHUTE);
                        target->CastSpell(target, SPELL_PARACHUTE_BUFF, true);
                    }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_parachute_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_parachute_AuraScript();
        }
};

enum PetSummoned
{
    NPC_DOOMGUARD       = 11859,
    NPC_INFERNAL        = 89,
    NPC_IMP             = 416
};

class spell_gen_pet_summoned : public SpellScriptLoader
{
    public:
        spell_gen_pet_summoned() : SpellScriptLoader("spell_gen_pet_summoned") { }

        class spell_gen_pet_summoned_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_pet_summoned_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                if (player->GetLastPetNumber())
                {
                    PetType newPetType = (player->getClass() == CLASS_HUNTER) ? HUNTER_PET : SUMMON_PET;
                    Pet* newPet = new Pet(player, newPetType);
                    if (newPet->LoadPetData(player, 0, player->GetLastPetNumber(), true))
                    {
                        // revive the pet if it is dead
                        if (newPet->getDeathState() == DEAD)
                            newPet->setDeathState(ALIVE);

                        newPet->SetFullHealth();
                        newPet->SetFullPower(newPet->GetPowerType());

                        switch (newPet->GetEntry())
                        {
                            case NPC_DOOMGUARD:
                            case NPC_INFERNAL:
                                newPet->SetEntry(NPC_IMP);
                                break;
                            default:
                                break;
                        }
                    }
                    else
                        delete newPet;
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_pet_summoned_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_pet_summoned_SpellScript();
        }
};

class spell_gen_profession_research : public SpellScriptLoader
{
    public:
        spell_gen_profession_research() : SpellScriptLoader("spell_gen_profession_research") { }

        class spell_gen_profession_research_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_profession_research_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            SpellCastResult CheckRequirement()
            {
                if (HasDiscoveredAllSpells(GetSpellInfo()->Id, GetCaster()->ToPlayer()))
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_NOTHING_TO_DISCOVER);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();
                uint32 spellId = GetSpellInfo()->Id;

                // learn random explicit discovery recipe (if any)
                if (uint32 discoveredSpellId = GetExplicitDiscoverySpell(spellId, caster))
                    caster->LearnSpell(discoveredSpellId, false);

                caster->UpdateCraftSkill(spellId);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_profession_research_SpellScript::CheckRequirement);
                OnEffectHitTarget += SpellEffectFn(spell_gen_profession_research_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_profession_research_SpellScript();
        }
};

enum TrinketSpells
{
    SPELL_PVP_TRINKET_ALLIANCE  = 97403,
    SPELL_PVP_TRINKET_HORDE     = 97404
};

class spell_gen_pvp_trinket : public SpellScriptLoader
{
    public:
        spell_gen_pvp_trinket() : SpellScriptLoader("spell_gen_pvp_trinket") { }

        class spell_gen_pvp_trinket_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_pvp_trinket_SpellScript);

            void TriggerAnimation()
            {
                Player* caster = GetCaster()->ToPlayer();

                switch (caster->GetTeam())
                {
                    case ALLIANCE:
                        caster->CastSpell(caster, SPELL_PVP_TRINKET_ALLIANCE, TRIGGERED_FULL_MASK);
                        break;
                    case HORDE:
                        caster->CastSpell(caster, SPELL_PVP_TRINKET_HORDE, TRIGGERED_FULL_MASK);
                        break;
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_gen_pvp_trinket_SpellScript::TriggerAnimation);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_pvp_trinket_SpellScript();
        }
};

class spell_gen_remove_flight_auras : public SpellScriptLoader
{
    public:
        spell_gen_remove_flight_auras() : SpellScriptLoader("spell_gen_remove_flight_auras") { }

        class spell_gen_remove_flight_auras_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_remove_flight_auras_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->RemoveAurasByType(SPELL_AURA_FLY);
                    target->RemoveAurasByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_remove_flight_auras_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_remove_flight_auras_SpellScript();
        }
};

enum Replenishment
{
    SPELL_REPLENISHMENT             = 57669,
    SPELL_INFINITE_REPLENISHMENT    = 61782
};

class ReplenishmentCheck
{
public:
    bool operator()(WorldObject* obj) const
    {
        if (Unit* target = obj->ToUnit())
            return target->GetPowerType() != POWER_MANA;

        return true;
    }
};

class spell_gen_replenishment : public SpellScriptLoader
{
    public:
        spell_gen_replenishment() : SpellScriptLoader("spell_gen_replenishment") { }

        class spell_gen_replenishment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_replenishment_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                // In arenas Replenishment may only affect the caster
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    if (caster->InArena())
                    {
                        targets.clear();
                        targets.push_back(caster);
                        return;
                    }
                }

                targets.remove_if(ReplenishmentCheck());

                uint8 const maxTargets = 10;

                if (targets.size() > maxTargets)
                {
                    targets.sort(Trinity::PowerPctOrderPred(POWER_MANA));
                    targets.resize(maxTargets);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_replenishment_SpellScript::RemoveInvalidTargets, EFFECT_ALL, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_replenishment_SpellScript();
        }

        class spell_gen_replenishment_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_replenishment_AuraScript);

            bool Load() override
            {
                return GetUnitOwner()->GetPowerType() == POWER_MANA;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                switch (GetSpellInfo()->Id)
                {
                    case SPELL_REPLENISHMENT:
                        amount = GetUnitOwner()->GetMaxPower(POWER_MANA) * 0.002f;
                        break;
                    case SPELL_INFINITE_REPLENISHMENT:
                        amount = GetUnitOwner()->GetMaxPower(POWER_MANA) * 0.0025f;
                        break;
                    default:
                        break;
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_replenishment_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_replenishment_AuraScript();
        }
};

enum RunningWildMountIds
{
    SPELL_ALTERED_FORM          = 97709
};

class spell_gen_running_wild : public SpellScriptLoader
{
    public:
        spell_gen_running_wild() : SpellScriptLoader("spell_gen_running_wild") { }

        class spell_gen_running_wild_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_running_wild_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                if (!sCreatureDisplayInfoStore.LookupEntry(DISPLAYID_HIDDEN_MOUNT))
                    return false;
                return true;
            }

            void HandleMount(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                PreventDefaultAction();

                target->Mount(DISPLAYID_HIDDEN_MOUNT, 0, 0);

                // cast speed aura
                if (MountCapabilityEntry const* mountCapability = sMountCapabilityStore.LookupEntry(aurEff->GetAmount()))
                    target->CastSpell(target, mountCapability->ModSpellAuraID, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_running_wild_AuraScript::HandleMount, EFFECT_1, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_gen_running_wild_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_running_wild_SpellScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                return ValidateSpellInfo({ SPELL_ALTERED_FORM });
            }

            bool Load() override
            {
                // Definitely not a good thing, but currently the only way to do something at cast start
                // Should be replaced as soon as possible with a new hook: BeforeCastStart
                GetCaster()->CastSpell(GetCaster(), SPELL_ALTERED_FORM, TRIGGERED_FULL_MASK);
                return false;
            }

            void Register() override
            {
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_running_wild_AuraScript();
        }

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_running_wild_SpellScript();
        }
};

class spell_gen_two_forms : public SpellScriptLoader
{
    public:
        spell_gen_two_forms() : SpellScriptLoader("spell_gen_two_forms") { }

        class spell_gen_two_forms_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_two_forms_SpellScript);

            SpellCastResult CheckCast()
            {
                if (GetCaster()->IsInCombat())
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_TRANSFORM);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                // Player cannot transform to human form if he is forced to be worgen for some reason (Darkflight)
                /*if (GetCaster()->GetAuraEffectsByType(SPELL_AURA_WORGEN_ALTERED_FORM).size() > 1)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_TRANSFORM);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }*/

                return SPELL_CAST_OK;
            }

            void HandleTransform(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();
                PreventHitDefaultEffect(effIndex);
                if (target->HasAuraType(SPELL_AURA_WORGEN_ALTERED_FORM))
                    target->RemoveAurasByType(SPELL_AURA_WORGEN_ALTERED_FORM);
                else    // Basepoints 1 for this aura control whether to trigger transform transition animation or not.
                    target->CastCustomSpell(SPELL_ALTERED_FORM, SPELLVALUE_BASE_POINT0, 1, target, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_two_forms_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_gen_two_forms_SpellScript::HandleTransform, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_two_forms_SpellScript();
        }
};

class spell_gen_darkflight : public SpellScriptLoader
{
    public:
        spell_gen_darkflight() : SpellScriptLoader("spell_gen_darkflight") { }

        class spell_gen_darkflight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_darkflight_SpellScript);

            void TriggerTransform()
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_ALTERED_FORM, TRIGGERED_FULL_MASK);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_gen_darkflight_SpellScript::TriggerTransform);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_darkflight_SpellScript();
        }
};
enum SeaforiumSpells
{
    SPELL_PLANT_CHARGES_CREDIT_ACHIEVEMENT  = 60937
};

class spell_gen_seaforium_blast : public SpellScriptLoader
{
    public:
        spell_gen_seaforium_blast() : SpellScriptLoader("spell_gen_seaforium_blast") { }

        class spell_gen_seaforium_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_seaforium_blast_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_PLANT_CHARGES_CREDIT_ACHIEVEMENT });
            }

            bool Load() override
            {
                // OriginalCaster is always available in Spell::prepare
                return GetOriginalCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void AchievementCredit(SpellEffIndex /*effIndex*/)
            {
                // but in effect handling OriginalCaster can become nullptr
                if (Unit* originalCaster = GetOriginalCaster())
                    if (GameObject* go = GetHitGObj())
                        if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
                            originalCaster->CastSpell(originalCaster, SPELL_PLANT_CHARGES_CREDIT_ACHIEVEMENT, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_seaforium_blast_SpellScript::AchievementCredit, EFFECT_1, SPELL_EFFECT_GAMEOBJECT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_seaforium_blast_SpellScript();
        }
};

enum SpectatorCheerTrigger
{
    EMOTE_ONE_SHOT_CHEER        = 4,
    EMOTE_ONE_SHOT_EXCLAMATION  = 5,
    EMOTE_ONE_SHOT_APPLAUD      = 21
};

uint8 const EmoteArray[3] = { EMOTE_ONE_SHOT_CHEER, EMOTE_ONE_SHOT_EXCLAMATION, EMOTE_ONE_SHOT_APPLAUD };

class spell_gen_spectator_cheer_trigger : public SpellScriptLoader
{
    public:
        spell_gen_spectator_cheer_trigger() : SpellScriptLoader("spell_gen_spectator_cheer_trigger") { }

        class spell_gen_spectator_cheer_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_spectator_cheer_trigger_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->HandleEmoteCommand(EmoteArray[urand(0, 2)]);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_spectator_cheer_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_spectator_cheer_trigger_SpellScript();
        }
};

class spell_gen_spirit_healer_res : public SpellScriptLoader
{
    public:
        spell_gen_spirit_healer_res(): SpellScriptLoader("spell_gen_spirit_healer_res") { }

        class spell_gen_spirit_healer_res_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_spirit_healer_res_SpellScript);

            bool Load() override
            {
                return GetOriginalCaster() && GetOriginalCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* originalCaster = GetOriginalCaster()->ToPlayer();
                if (Unit* target = GetHitUnit())
                {
                    WorldPackets::NPC::SpiritHealerConfirm spiritHealerConfirm;
                    spiritHealerConfirm.Unit = target->GetGUID();
                    originalCaster->SendDirectMessage(spiritHealerConfirm.Write());
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_spirit_healer_res_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_spirit_healer_res_SpellScript();
        }
};

enum SummonElemental
{
    SPELL_SUMMON_FIRE_ELEMENTAL  = 8985,
    SPELL_SUMMON_EARTH_ELEMENTAL = 19704
};

class spell_gen_summon_elemental : public SpellScriptLoader
{
    public:
        spell_gen_summon_elemental(const char* name, uint32 spellId) : SpellScriptLoader(name), _spellId(spellId) { }

        class spell_gen_summon_elemental_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_summon_elemental_AuraScript);

        public:
            spell_gen_summon_elemental_AuraScript(uint32 spellId) : AuraScript(), _spellId(spellId) { }

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ _spellId });
            }

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                    if (Unit* owner = GetCaster()->GetOwner())
                        owner->CastSpell(owner, _spellId, true);
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                    if (Unit* owner = GetCaster()->GetOwner())
                        if (owner->GetTypeId() == TYPEID_PLAYER) /// @todo this check is maybe wrong
                            owner->ToPlayer()->RemovePet(nullptr, PET_SAVE_DISMISS, true);
            }

            void Register() override
            {
                 AfterEffectApply += AuraEffectApplyFn(spell_gen_summon_elemental_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                 AfterEffectRemove += AuraEffectRemoveFn(spell_gen_summon_elemental_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }

        private:
            uint32 _spellId;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_summon_elemental_AuraScript(_spellId);
        }

    private:
        uint32 _spellId;
};

enum TournamentMountsSpells
{
    SPELL_LANCE_EQUIPPED     = 62853
};

class spell_gen_summon_tournament_mount : public SpellScriptLoader
{
    public:
        spell_gen_summon_tournament_mount() : SpellScriptLoader("spell_gen_summon_tournament_mount") { }

        class spell_gen_summon_tournament_mount_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_summon_tournament_mount_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_LANCE_EQUIPPED });
            }

            SpellCastResult CheckIfLanceEquiped()
            {
                if (GetCaster()->IsInDisallowedMountForm())
                    GetCaster()->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

                if (!GetCaster()->HasAura(SPELL_LANCE_EQUIPPED))
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_HAVE_LANCE_EQUIPPED);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_summon_tournament_mount_SpellScript::CheckIfLanceEquiped);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_summon_tournament_mount_SpellScript();
        }
};

// 41213, 43416, 69222, 73076 - Throw Shield
class spell_gen_throw_shield : public SpellScriptLoader
{
    public:
        spell_gen_throw_shield() : SpellScriptLoader("spell_gen_throw_shield") { }

        class spell_gen_throw_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_throw_shield_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_throw_shield_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_throw_shield_SpellScript();
        }
};

enum MountedDuelSpells
{
    SPELL_ON_TOURNAMENT_MOUNT = 63034,
    SPELL_MOUNTED_DUEL        = 62875
};

class spell_gen_tournament_duel : public SpellScriptLoader
{
    public:
        spell_gen_tournament_duel() : SpellScriptLoader("spell_gen_tournament_duel") { }

        class spell_gen_tournament_duel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_tournament_duel_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_ON_TOURNAMENT_MOUNT, SPELL_MOUNTED_DUEL });
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* rider = GetCaster()->GetCharmer())
                {
                    if (Player* playerTarget = GetHitPlayer())
                    {
                        if (playerTarget->HasAura(SPELL_ON_TOURNAMENT_MOUNT) && playerTarget->GetVehicleBase())
                            rider->CastSpell(playerTarget, SPELL_MOUNTED_DUEL, true);
                    }
                    else if (Unit* unitTarget = GetHitUnit())
                    {
                        if (unitTarget->GetCharmer() && unitTarget->GetCharmer()->GetTypeId() == TYPEID_PLAYER && unitTarget->GetCharmer()->HasAura(SPELL_ON_TOURNAMENT_MOUNT))
                            rider->CastSpell(unitTarget->GetCharmer(), SPELL_MOUNTED_DUEL, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_tournament_duel_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_tournament_duel_SpellScript();
        }
};

class spell_gen_tournament_pennant : public SpellScriptLoader
{
    public:
        spell_gen_tournament_pennant() : SpellScriptLoader("spell_gen_tournament_pennant") { }

        class spell_gen_tournament_pennant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_tournament_pennant_AuraScript);

            bool Load() override
            {
                return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (!caster->GetVehicleBase())
                        caster->RemoveAurasDueToSpell(GetId());
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_tournament_pennant_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_tournament_pennant_AuraScript();
        }
};

class spell_gen_trigger_exclude_caster_aura_spell : public SpellScriptLoader
{
    public:
        spell_gen_trigger_exclude_caster_aura_spell() : SpellScriptLoader("spell_gen_trigger_exclude_caster_aura_spell") { }

        class spell_gen_trigger_exclude_caster_aura_spell_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_trigger_exclude_caster_aura_spell_SpellScript);

            bool Validate(SpellInfo const* spellInfo) override
            {
                return ValidateSpellInfo({ spellInfo->ExcludeCasterAuraSpell });
            }

            void HandleTrigger()
            {
                // Blizz seems to just apply aura without bothering to cast
                GetCaster()->AddAura(GetSpellInfo()->ExcludeCasterAuraSpell, GetCaster());
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_gen_trigger_exclude_caster_aura_spell_SpellScript::HandleTrigger);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_trigger_exclude_caster_aura_spell_SpellScript();
        }
};

class spell_gen_trigger_exclude_target_aura_spell : public SpellScriptLoader
{
public:
    spell_gen_trigger_exclude_target_aura_spell() : SpellScriptLoader("spell_gen_trigger_exclude_target_aura_spell") { }

    class spell_gen_trigger_exclude_target_aura_spell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_trigger_exclude_target_aura_spell_SpellScript);

        bool Validate(SpellInfo const* spellInfo) override
        {
            if (spellInfo->ExcludeTargetAuraSpell)
                if (spellInfo->ExcludeTargetAuraSpell == spellInfo->Id)
                    return ValidateSpellInfo({ spellInfo->ExcludeTargetAuraSpell });

            return true;
        }

        void HandleTrigger()
        {
            if (!GetCaster())
                return;

            if (Unit* target = GetHitUnit())
                // Blizz seems to just apply aura without bothering to cast
                GetCaster()->AddAura(GetSpellInfo()->ExcludeTargetAuraSpell, target);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_gen_trigger_exclude_target_aura_spell_SpellScript::HandleTrigger);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_trigger_exclude_target_aura_spell_SpellScript();
    }
};

enum PvPTrinketTriggeredSpells
{
    SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER         = 72752,
    SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER_WOTF    = 72757
};

template <uint32 TriggeredSpellId>
class spell_pvp_trinket_wotf_shared_cd : public SpellScriptLoader
{
    public:
        spell_pvp_trinket_wotf_shared_cd(char const* ScriptName) : SpellScriptLoader(ScriptName) { }

        template <uint32 Triggered>
        class spell_pvp_trinket_wotf_shared_cd_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pvp_trinket_wotf_shared_cd_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ Triggered });
            }

            void HandleScript()
            {
                /*
                 * @workaround: PendingCast flag normally means 'triggered' spell, however
                 * if the spell is cast triggered, the core won't send SMSG_SPELL_GO packet
                 * so client never registers the cooldown (see Spell::IsNeedSendToClient)
                 *
                 * ServerToClient: SMSG_SPELL_GO (0x0132) Length: 42 ConnIdx: 0 Time: 07/19/2010 02:32:35.000 Number: 362675
                 * Caster GUID: Full: Player
                 * Caster Unit GUID: Full: Player
                 * Cast Count: 0
                 * Spell ID: 72752 (72752)
                 * Cast Flags: PendingCast, Unknown3, Unknown7 (265)
                 * Time: 3901468825
                 * Hit Count: 1
                 * [0] Hit GUID: Player
                 * Miss Count: 0
                 * Target Flags: Unit (2)
                 * Target GUID: 0x0
                */

                // Spell flags need further research, until then just cast not triggered
                GetCaster()->CastSpell((Unit*)nullptr, Triggered, false);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_pvp_trinket_wotf_shared_cd_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pvp_trinket_wotf_shared_cd_SpellScript<TriggeredSpellId>();
        }
};

enum FriendOrFowl
{
    SPELL_TURKEY_VENGEANCE      = 25285
};

class spell_gen_turkey_marker : public SpellScriptLoader
{
    public:
        spell_gen_turkey_marker() : SpellScriptLoader("spell_gen_turkey_marker") { }

        class spell_gen_turkey_marker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_turkey_marker_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // store stack apply times, so we can pop them while they expire
                _applyTimes.push_back(GameTime::GetGameTimeMS());
                Unit* target = GetTarget();

                // on stack 15 cast the achievement crediting spell
                if (GetStackAmount() >= 15)
                    target->CastSpell(target, SPELL_TURKEY_VENGEANCE, true, nullptr, aurEff, GetCasterGUID());
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (_applyTimes.empty())
                    return;

                // pop stack if it expired for us
                if (_applyTimes.front() + GetMaxDuration() < GameTime::GetGameTimeMS())
                    ModStackAmount(-1, AURA_REMOVE_BY_EXPIRE);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gen_turkey_marker_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_turkey_marker_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }

            std::list<uint32> _applyTimes;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_turkey_marker_AuraScript();
        }
};

enum FoamSword
{
    ITEM_FOAM_SWORD_GREEN   = 45061,
    ITEM_FOAM_SWORD_PINK    = 45176,
    ITEM_FOAM_SWORD_BLUE    = 45177,
    ITEM_FOAM_SWORD_RED     = 45178,
    ITEM_FOAM_SWORD_YELLOW  = 45179
};

class spell_gen_upper_deck_create_foam_sword : public SpellScriptLoader
{
    public:
        spell_gen_upper_deck_create_foam_sword() : SpellScriptLoader("spell_gen_upper_deck_create_foam_sword") { }

        class spell_gen_upper_deck_create_foam_sword_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_upper_deck_create_foam_sword_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (Player* player = GetHitPlayer())
                {
                    static uint32 const itemId[5] = { ITEM_FOAM_SWORD_GREEN, ITEM_FOAM_SWORD_PINK, ITEM_FOAM_SWORD_BLUE, ITEM_FOAM_SWORD_RED, ITEM_FOAM_SWORD_YELLOW };
                    // player can only have one of these items
                    for (uint8 i = 0; i < 5; ++i)
                    {
                        if (player->HasItemCount(itemId[i], 1, true))
                            return;
                    }

                    CreateItem(effIndex, itemId[urand(0, 4)], ItemContext::NONE);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_upper_deck_create_foam_sword_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_upper_deck_create_foam_sword_SpellScript();
        }
};

enum VampiricTouch
{
    SPELL_VAMPIRIC_TOUCH_HEAL   = 52724
};

// 52723 - Vampiric Touch
// 60501 - Vampiric Touch
class spell_gen_vampiric_touch : public SpellScriptLoader
{
    public:
        spell_gen_vampiric_touch() : SpellScriptLoader("spell_gen_vampiric_touch") { }

        class spell_gen_vampiric_touch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_vampiric_touch_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_VAMPIRIC_TOUCH_HEAL });
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                DamageInfo* damageInfo = eventInfo.GetDamageInfo();
                if (!damageInfo || !damageInfo->GetDamage())
                    return;

                Unit* caster = eventInfo.GetActor();
                int32 bp = damageInfo->GetDamage() / 2;
                caster->CastCustomSpell(SPELL_VAMPIRIC_TOUCH_HEAL, SPELLVALUE_BASE_POINT0, bp, caster, true, nullptr, aurEff);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_gen_vampiric_touch_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_vampiric_touch_AuraScript();
        }
};

enum VehicleScaling
{
    SPELL_GEAR_SCALING      = 66668
};

class spell_gen_vehicle_scaling : public SpellScriptLoader
{
    public:
        spell_gen_vehicle_scaling() : SpellScriptLoader("spell_gen_vehicle_scaling") { }

        class spell_gen_vehicle_scaling_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_vehicle_scaling_AuraScript);

            bool Load() override
            {
                return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                float factor;
                uint16 baseItemLevel;

                /// @todo Reserach coeffs for different vehicles
                switch (GetId())
                {
                    case SPELL_GEAR_SCALING:
                        factor = 1.0f;
                        baseItemLevel = 205;
                        break;
                    default:
                        factor = 1.0f;
                        baseItemLevel = 170;
                        break;
                }

                float avgILvl = caster->ToPlayer()->GetAverageItemLevelEquipped();
                if (avgILvl < baseItemLevel)
                    return;                     /// @todo Research possibility of scaling down

                amount = uint16((avgILvl - baseItemLevel) * factor);
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_vehicle_scaling_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_HEALING_PCT);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_vehicle_scaling_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_vehicle_scaling_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_vehicle_scaling_AuraScript();
        }
};

enum VendorBarkTrigger
{
    NPC_AMPHITHEATER_VENDOR     = 30098,
    SAY_AMPHITHEATER_VENDOR     = 0
};

class spell_gen_vendor_bark_trigger : public SpellScriptLoader
{
    public:
        spell_gen_vendor_bark_trigger() : SpellScriptLoader("spell_gen_vendor_bark_trigger") { }

        class spell_gen_vendor_bark_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_vendor_bark_trigger_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Creature* vendor = GetCaster()->ToCreature())
                    if (vendor->GetEntry() == NPC_AMPHITHEATER_VENDOR)
                        vendor->AI()->Talk(SAY_AMPHITHEATER_VENDOR);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_vendor_bark_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_vendor_bark_trigger_SpellScript();
        }

};

class spell_gen_wg_water : public SpellScriptLoader
{
    public:
        spell_gen_wg_water() : SpellScriptLoader("spell_gen_wg_water") { }

        class spell_gen_wg_water_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_wg_water_SpellScript);

            SpellCastResult CheckCast()
            {
                if (!GetSpellInfo()->CheckTargetCreatureType(GetCaster()))
                    return SPELL_FAILED_DONT_REPORT;
                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_wg_water_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_wg_water_SpellScript();
        }
};

enum WhisperGulchYoggSaronWhisper
{
    SPELL_YOGG_SARON_WHISPER_DUMMY  = 29072
};

class spell_gen_whisper_gulch_yogg_saron_whisper : public SpellScriptLoader
{
    public:
        spell_gen_whisper_gulch_yogg_saron_whisper() : SpellScriptLoader("spell_gen_whisper_gulch_yogg_saron_whisper") { }

        class spell_gen_whisper_gulch_yogg_saron_whisper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_whisper_gulch_yogg_saron_whisper_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_YOGG_SARON_WHISPER_DUMMY });
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell((Unit*)nullptr, SPELL_YOGG_SARON_WHISPER_DUMMY, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_whisper_gulch_yogg_saron_whisper_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_whisper_gulch_yogg_saron_whisper_AuraScript();
        }
};

class spell_gen_eject_all_passengers : public SpellScriptLoader
{
    public:
        spell_gen_eject_all_passengers() : SpellScriptLoader("spell_gen_eject_all_passengers") { }

        class spell_gen_eject_all_passengers_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_eject_all_passengers_SpellScript);

            void RemoveVehicleAuras()
            {
                if (Vehicle* vehicle = GetHitUnit()->GetVehicleKit())
                    vehicle->RemoveAllPassengers();
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_gen_eject_all_passengers_SpellScript::RemoveVehicleAuras);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_eject_all_passengers_SpellScript();
        }
};

class spell_gen_eject_passenger : public SpellScriptLoader
{
    public:
        spell_gen_eject_passenger() : SpellScriptLoader("spell_gen_eject_passenger") { }

        class spell_gen_eject_passenger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_eject_passenger_SpellScript);

            bool Validate(SpellInfo const* spellInfo) override
            {
                SpellEffectInfo const* effect = spellInfo->GetEffect(EFFECT_0);
                if (!effect || effect->CalcValue() < 1)
                    return false;
                return true;
            }

            void EjectPassenger(SpellEffIndex /*effIndex*/)
            {
                if (Vehicle* vehicle = GetHitUnit()->GetVehicleKit())
                {
                    if (Unit* passenger = vehicle->GetPassenger(GetEffectValue() - 1))
                        passenger->ExitVehicle();
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_eject_passenger_SpellScript::EjectPassenger, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_eject_passenger_SpellScript();
        }
};

enum GMFreeze
{
    SPELL_GM_FREEZE = 9454
};

class spell_gen_gm_freeze : public SpellScriptLoader
{
    public:
        spell_gen_gm_freeze() : SpellScriptLoader("spell_gen_gm_freeze") { }

        class spell_gen_gm_freeze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_gm_freeze_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_GM_FREEZE });
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Do what was done before to the target in HandleFreezeCommand
                if (Player* player = GetTarget()->ToPlayer())
                {
                    // stop combat + make player unattackable + duel stop + stop some spells
                    player->SetFaction(35);
                    player->CombatStop();
                    if (player->IsNonMeleeSpellCast(true))
                        player->InterruptNonMeleeSpells(true);
                    player->AddUnitFlag(UNIT_FLAG_NON_ATTACKABLE);

                    // if player class = hunter || warlock remove pet if alive
                    if ((player->getClass() == CLASS_HUNTER) || (player->getClass() == CLASS_WARLOCK))
                    {
                        if (Pet* pet = player->GetPet())
                        {
                            pet->SavePetToDB(PET_SAVE_CURRENT_STATE);
                            // not let dismiss dead pet
                            if (pet->IsAlive())
                                player->RemovePet(pet, PET_SAVE_DISMISS);
                        }
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Do what was done before to the target in HandleUnfreezeCommand
                if (Player* player = GetTarget()->ToPlayer())
                {
                    // Reset player faction + allow combat + allow duels
                    player->setFactionForRace(player->getRace());
                    player->RemoveUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
                    // save player
                    player->SaveToDB();
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_gm_freeze_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_gm_freeze_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_gm_freeze_AuraScript();
        }
};

class spell_gen_stand : public SpellScriptLoader
{
public:
    spell_gen_stand() : SpellScriptLoader("spell_gen_stand") { }

    class spell_gen_stand_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_stand_SpellScript);

        void HandleScript(SpellEffIndex /*eff*/)
        {
            Creature* target = GetHitCreature();
            if (!target)
                return;

            target->SetStandState(UNIT_STAND_STATE_STAND);
            target->HandleEmoteCommand(EMOTE_STATE_NONE);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_stand_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_stand_SpellScript();
    }
};

enum RequiredMixologySpells
{
    SPELL_MIXOLOGY                      = 53042,
    // Flasks
    SPELL_FLASK_OF_THE_FROST_WYRM       = 53755,
    SPELL_FLASK_OF_STONEBLOOD           = 53758,
    SPELL_FLASK_OF_ENDLESS_RAGE         = 53760,
    SPELL_FLASK_OF_PURE_MOJO            = 54212,
    SPELL_LESSER_FLASK_OF_RESISTANCE    = 62380,
    SPELL_LESSER_FLASK_OF_TOUGHNESS     = 53752,
    SPELL_FLASK_OF_BLINDING_LIGHT       = 28521,
    SPELL_FLASK_OF_CHROMATIC_WONDER     = 42735,
    SPELL_FLASK_OF_FORTIFICATION        = 28518,
    SPELL_FLASK_OF_MIGHTY_RESTORATION   = 28519,
    SPELL_FLASK_OF_PURE_DEATH           = 28540,
    SPELL_FLASK_OF_RELENTLESS_ASSAULT   = 28520,
    SPELL_FLASK_OF_CHROMATIC_RESISTANCE = 17629,
    SPELL_FLASK_OF_DISTILLED_WISDOM     = 17627,
    SPELL_FLASK_OF_SUPREME_POWER        = 17628,
    SPELL_FLASK_OF_THE_TITANS           = 17626,
    // Elixirs
    SPELL_ELIXIR_OF_MIGHTY_AGILITY      = 28497,
    SPELL_ELIXIR_OF_ACCURACY            = 60340,
    SPELL_ELIXIR_OF_DEADLY_STRIKES      = 60341,
    SPELL_ELIXIR_OF_MIGHTY_DEFENSE      = 60343,
    SPELL_ELIXIR_OF_EXPERTISE           = 60344,
    SPELL_ELIXIR_OF_ARMOR_PIERCING      = 60345,
    SPELL_ELIXIR_OF_LIGHTNING_SPEED     = 60346,
    SPELL_ELIXIR_OF_MIGHTY_FORTITUDE    = 53751,
    SPELL_ELIXIR_OF_MIGHTY_MAGEBLOOD    = 53764,
    SPELL_ELIXIR_OF_MIGHTY_STRENGTH     = 53748,
    SPELL_ELIXIR_OF_MIGHTY_TOUGHTS      = 60347,
    SPELL_ELIXIR_OF_PROTECTION          = 53763,
    SPELL_ELIXIR_OF_SPIRIT              = 53747,
    SPELL_GURUS_ELIXIR                  = 53749,
    SPELL_SHADOWPOWER_ELIXIR            = 33721,
    SPELL_WRATH_ELIXIR                  = 53746,
    SPELL_ELIXIR_OF_EMPOWERMENT         = 28514,
    SPELL_ELIXIR_OF_MAJOR_MAGEBLOOD     = 28509,
    SPELL_ELIXIR_OF_MAJOR_SHADOW_POWER  = 28503,
    SPELL_ELIXIR_OF_MAJOR_DEFENSE       = 28502,
    SPELL_FEL_STRENGTH_ELIXIR           = 38954,
    SPELL_ELIXIR_OF_IRONSKIN            = 39628,
    SPELL_ELIXIR_OF_MAJOR_AGILITY       = 54494,
    SPELL_ELIXIR_OF_DRAENIC_WISDOM      = 39627,
    SPELL_ELIXIR_OF_MAJOR_FIREPOWER     = 28501,
    SPELL_ELIXIR_OF_MAJOR_FROST_POWER   = 28493,
    SPELL_EARTHEN_ELIXIR                = 39626,
    SPELL_ELIXIR_OF_MASTERY             = 33726,
    SPELL_ELIXIR_OF_HEALING_POWER       = 28491,
    SPELL_ELIXIR_OF_MAJOR_FORTITUDE     = 39625,
    SPELL_ELIXIR_OF_MAJOR_STRENGTH      = 28490,
    SPELL_ADEPTS_ELIXIR                 = 54452,
    SPELL_ONSLAUGHT_ELIXIR              = 33720,
    SPELL_MIGHTY_TROLLS_BLOOD_ELIXIR    = 24361,
    SPELL_GREATER_ARCANE_ELIXIR         = 17539,
    SPELL_ELIXIR_OF_THE_MONGOOSE        = 17538,
    SPELL_ELIXIR_OF_BRUTE_FORCE         = 17537,
    SPELL_ELIXIR_OF_SAGES               = 17535,
    SPELL_ELIXIR_OF_SUPERIOR_DEFENSE    = 11348,
    SPELL_ELIXIR_OF_DEMONSLAYING        = 11406,
    SPELL_ELIXIR_OF_GREATER_FIREPOWER   = 26276,
    SPELL_ELIXIR_OF_SHADOW_POWER        = 11474,
    SPELL_MAGEBLOOD_ELIXIR              = 24363,
    SPELL_ELIXIR_OF_GIANTS              = 11405,
    SPELL_ELIXIR_OF_GREATER_AGILITY     = 11334,
    SPELL_ARCANE_ELIXIR                 = 11390,
    SPELL_ELIXIR_OF_GREATER_INTELLECT   = 11396,
    SPELL_ELIXIR_OF_GREATER_DEFENSE     = 11349,
    SPELL_ELIXIR_OF_FROST_POWER         = 21920,
    SPELL_ELIXIR_OF_AGILITY             = 11328,
    SPELL_MAJOR_TROLLS_BLLOOD_ELIXIR    =  3223,
    SPELL_ELIXIR_OF_FORTITUDE           =  3593,
    SPELL_ELIXIR_OF_OGRES_STRENGTH      =  3164,
    SPELL_ELIXIR_OF_FIREPOWER           =  7844,
    SPELL_ELIXIR_OF_LESSER_AGILITY      =  3160,
    SPELL_ELIXIR_OF_DEFENSE             =  3220,
    SPELL_STRONG_TROLLS_BLOOD_ELIXIR    =  3222,
    SPELL_ELIXIR_OF_MINOR_ACCURACY      = 63729,
    SPELL_ELIXIR_OF_WISDOM              =  3166,
    SPELL_ELIXIR_OF_GIANTH_GROWTH       =  8212,
    SPELL_ELIXIR_OF_MINOR_AGILITY       =  2374,
    SPELL_ELIXIR_OF_MINOR_FORTITUDE     =  2378,
    SPELL_WEAK_TROLLS_BLOOD_ELIXIR      =  3219,
    SPELL_ELIXIR_OF_LIONS_STRENGTH      =  2367,
    SPELL_ELIXIR_OF_MINOR_DEFENSE       =   673
};

class spell_gen_mixology_bonus : public SpellScriptLoader
{
public:
    spell_gen_mixology_bonus() : SpellScriptLoader("spell_gen_mixology_bonus") { }

    class spell_gen_mixology_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_mixology_bonus_AuraScript);

    public:
        spell_gen_mixology_bonus_AuraScript()
        {
            bonus = 0;
        }

    private:
        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_MIXOLOGY });
        }

        bool Load() override
        {
            return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void SetBonusValueForEffect(SpellEffIndex effIndex, int32 value, AuraEffect const* aurEff)
        {
            if (aurEff->GetEffIndex() == uint32(effIndex))
                bonus = value;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (GetCaster()->HasAura(SPELL_MIXOLOGY) && GetCaster()->HasSpell(GetSpellInfo()->GetEffect(EFFECT_0)->TriggerSpell))
            {
                switch (GetId())
                {
                    case SPELL_WEAK_TROLLS_BLOOD_ELIXIR:
                    case SPELL_MAGEBLOOD_ELIXIR:
                        bonus = amount;
                        break;
                    case SPELL_ELIXIR_OF_FROST_POWER:
                    case SPELL_LESSER_FLASK_OF_TOUGHNESS:
                    case SPELL_LESSER_FLASK_OF_RESISTANCE:
                        bonus = CalculatePct(amount, 80);
                        break;
                    case SPELL_ELIXIR_OF_MINOR_DEFENSE:
                    case SPELL_ELIXIR_OF_LIONS_STRENGTH:
                    case SPELL_ELIXIR_OF_MINOR_AGILITY:
                    case SPELL_MAJOR_TROLLS_BLLOOD_ELIXIR:
                    case SPELL_ELIXIR_OF_SHADOW_POWER:
                    case SPELL_ELIXIR_OF_BRUTE_FORCE:
                    case SPELL_MIGHTY_TROLLS_BLOOD_ELIXIR:
                    case SPELL_ELIXIR_OF_GREATER_FIREPOWER:
                    case SPELL_ONSLAUGHT_ELIXIR:
                    case SPELL_EARTHEN_ELIXIR:
                    case SPELL_ELIXIR_OF_MAJOR_AGILITY:
                    case SPELL_FLASK_OF_THE_TITANS:
                    case SPELL_FLASK_OF_RELENTLESS_ASSAULT:
                    case SPELL_FLASK_OF_STONEBLOOD:
                    case SPELL_ELIXIR_OF_MINOR_ACCURACY:
                        bonus = CalculatePct(amount, 50);
                        break;
                    case SPELL_ELIXIR_OF_PROTECTION:
                        bonus = 280;
                        break;
                    case SPELL_ELIXIR_OF_MAJOR_DEFENSE:
                        bonus = 200;
                        break;
                    case SPELL_ELIXIR_OF_GREATER_DEFENSE:
                    case SPELL_ELIXIR_OF_SUPERIOR_DEFENSE:
                        bonus = 140;
                        break;
                    case SPELL_ELIXIR_OF_FORTITUDE:
                        bonus = 100;
                        break;
                    case SPELL_FLASK_OF_ENDLESS_RAGE:
                        bonus = 82;
                        break;
                    case SPELL_ELIXIR_OF_DEFENSE:
                        bonus = 70;
                        break;
                    case SPELL_ELIXIR_OF_DEMONSLAYING:
                        bonus = 50;
                        break;
                    case SPELL_FLASK_OF_THE_FROST_WYRM:
                        bonus = 47;
                        break;
                    case SPELL_WRATH_ELIXIR:
                        bonus = 32;
                        break;
                    case SPELL_ELIXIR_OF_MAJOR_FROST_POWER:
                    case SPELL_ELIXIR_OF_MAJOR_FIREPOWER:
                    case SPELL_ELIXIR_OF_MAJOR_SHADOW_POWER:
                        bonus = 29;
                        break;
                    case SPELL_ELIXIR_OF_MIGHTY_TOUGHTS:
                        bonus = 27;
                        break;
                    case SPELL_FLASK_OF_SUPREME_POWER:
                    case SPELL_FLASK_OF_BLINDING_LIGHT:
                    case SPELL_FLASK_OF_PURE_DEATH:
                    case SPELL_SHADOWPOWER_ELIXIR:
                        bonus = 23;
                        break;
                    case SPELL_ELIXIR_OF_MIGHTY_AGILITY:
                    case SPELL_FLASK_OF_DISTILLED_WISDOM:
                    case SPELL_ELIXIR_OF_SPIRIT:
                    case SPELL_ELIXIR_OF_MIGHTY_STRENGTH:
                    case SPELL_FLASK_OF_PURE_MOJO:
                    case SPELL_ELIXIR_OF_ACCURACY:
                    case SPELL_ELIXIR_OF_DEADLY_STRIKES:
                    case SPELL_ELIXIR_OF_MIGHTY_DEFENSE:
                    case SPELL_ELIXIR_OF_EXPERTISE:
                    case SPELL_ELIXIR_OF_ARMOR_PIERCING:
                    case SPELL_ELIXIR_OF_LIGHTNING_SPEED:
                        bonus = 20;
                        break;
                    case SPELL_FLASK_OF_CHROMATIC_RESISTANCE:
                        bonus = 17;
                        break;
                    case SPELL_ELIXIR_OF_MINOR_FORTITUDE:
                    case SPELL_ELIXIR_OF_MAJOR_STRENGTH:
                        bonus = 15;
                        break;
                    case SPELL_FLASK_OF_MIGHTY_RESTORATION:
                        bonus = 13;
                        break;
                    case SPELL_ARCANE_ELIXIR:
                        bonus = 12;
                        break;
                    case SPELL_ELIXIR_OF_GREATER_AGILITY:
                    case SPELL_ELIXIR_OF_GIANTS:
                        bonus = 11;
                        break;
                    case SPELL_ELIXIR_OF_AGILITY:
                    case SPELL_ELIXIR_OF_GREATER_INTELLECT:
                    case SPELL_ELIXIR_OF_SAGES:
                    case SPELL_ELIXIR_OF_IRONSKIN:
                    case SPELL_ELIXIR_OF_MIGHTY_MAGEBLOOD:
                        bonus = 10;
                        break;
                    case SPELL_ELIXIR_OF_HEALING_POWER:
                        bonus = 9;
                        break;
                    case SPELL_ELIXIR_OF_DRAENIC_WISDOM:
                    case SPELL_GURUS_ELIXIR:
                        bonus = 8;
                        break;
                    case SPELL_ELIXIR_OF_FIREPOWER:
                    case SPELL_ELIXIR_OF_MAJOR_MAGEBLOOD:
                    case SPELL_ELIXIR_OF_MASTERY:
                        bonus = 6;
                        break;
                    case SPELL_ELIXIR_OF_LESSER_AGILITY:
                    case SPELL_ELIXIR_OF_OGRES_STRENGTH:
                    case SPELL_ELIXIR_OF_WISDOM:
                    case SPELL_ELIXIR_OF_THE_MONGOOSE:
                        bonus = 5;
                        break;
                    case SPELL_STRONG_TROLLS_BLOOD_ELIXIR:
                    case SPELL_FLASK_OF_CHROMATIC_WONDER:
                        bonus = 4;
                        break;
                    case SPELL_ELIXIR_OF_EMPOWERMENT:
                        bonus = -10;
                        break;
                    case SPELL_ADEPTS_ELIXIR:
                        SetBonusValueForEffect(EFFECT_0, 13, aurEff);
                        SetBonusValueForEffect(EFFECT_1, 13, aurEff);
                        SetBonusValueForEffect(EFFECT_2, 8, aurEff);
                        break;
                    case SPELL_ELIXIR_OF_MIGHTY_FORTITUDE:
                        SetBonusValueForEffect(EFFECT_0, 160, aurEff);
                        break;
                    case SPELL_ELIXIR_OF_MAJOR_FORTITUDE:
                        SetBonusValueForEffect(EFFECT_0, 116, aurEff);
                        SetBonusValueForEffect(EFFECT_1, 6, aurEff);
                        break;
                    case SPELL_FEL_STRENGTH_ELIXIR:
                        SetBonusValueForEffect(EFFECT_0, 40, aurEff);
                        SetBonusValueForEffect(EFFECT_1, 40, aurEff);
                        break;
                    case SPELL_FLASK_OF_FORTIFICATION:
                        SetBonusValueForEffect(EFFECT_0, 210, aurEff);
                        SetBonusValueForEffect(EFFECT_1, 5, aurEff);
                        break;
                    case SPELL_GREATER_ARCANE_ELIXIR:
                        SetBonusValueForEffect(EFFECT_0, 19, aurEff);
                        SetBonusValueForEffect(EFFECT_1, 19, aurEff);
                        SetBonusValueForEffect(EFFECT_2, 5, aurEff);
                        break;
                    case SPELL_ELIXIR_OF_GIANTH_GROWTH:
                        SetBonusValueForEffect(EFFECT_0, 5, aurEff);
                        break;
                    default:
                        TC_LOG_ERROR("spells", "SpellId %u couldn't be processed in spell_gen_mixology_bonus", GetId());
                        break;
                }
                amount += bonus;
            }
        }

        int32 bonus;

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_mixology_bonus_AuraScript::CalculateAmount, EFFECT_ALL, SPELL_AURA_ANY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_mixology_bonus_AuraScript();
    }
};

enum LandmineKnockbackAchievement
{
    SPELL_LANDMINE_KNOCKBACK_ACHIEVEMENT = 57064
};

class spell_gen_landmine_knockback_achievement : public SpellScriptLoader
{
public:
    spell_gen_landmine_knockback_achievement() : SpellScriptLoader("spell_gen_landmine_knockback_achievement") { }

    class spell_gen_landmine_knockback_achievement_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_landmine_knockback_achievement_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (Player* target = GetHitPlayer())
            {
                Aura const* aura = GetHitAura();
                if (!aura || aura->GetStackAmount() < 10)
                    return;

                target->CastSpell(target, SPELL_LANDMINE_KNOCKBACK_ACHIEVEMENT, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_landmine_knockback_achievement_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_landmine_knockback_achievement_SpellScript();
    }
};

// 34098 - ClearAllDebuffs
class spell_gen_clear_debuffs : public SpellScriptLoader
{
    public:
        spell_gen_clear_debuffs() : SpellScriptLoader("spell_gen_clear_debuffs") { }

        class spell_gen_clear_debuffs_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_clear_debuffs_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->RemoveOwnedAuras([](Aura const* aura)
                    {
                        SpellInfo const* spellInfo = aura->GetSpellInfo();
                        return !spellInfo->IsPositive() && !spellInfo->IsPassive();
                    });
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_clear_debuffs_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_clear_debuffs_SpellScript();
        }
};

// Spell 67039
class spell_gen_pony_mount_check : public SpellScriptLoader
{
    public:
        spell_gen_pony_mount_check() : SpellScriptLoader("spell_gen_pony_mount_check") { }

        enum PonySpells
        {
            //ACHIEV_PONY_UP = 3736,
            MOUNT_PONY = 29736
        };

        class spell_gen_pony_mount_check_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_pony_mount_check_AuraScript);

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->GetOwner())
                        //if(!caster->GetOwner()->ToPlayer()->HasAchieved(ACHIEV_PONY_UP))
                        {
                            if (caster->GetOwner()->IsMounted())
                            {
                                caster->Mount(MOUNT_PONY);
                                caster->SetSpeed(MOVE_RUN, 2.0f);
                            }
                            else if (caster->IsMounted())
                            {
                                caster->Dismount();
                                caster->SetSpeed(MOVE_RUN, 1.0f);
                            }
                        }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_pony_mount_check_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_pony_mount_check_AuraScript();
        }
};

class spell_gen_shroud_of_death : public SpellScriptLoader
{
public:
    spell_gen_shroud_of_death() : SpellScriptLoader("spell_gen_shroud_of_death") { }

    class spell_gen_shroud_of_death_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_shroud_of_death_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            PreventDefaultAction();
            GetUnitOwner()->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
            GetUnitOwner()->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            PreventDefaultAction();
            GetUnitOwner()->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
            GetUnitOwner()->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_gen_shroud_of_death_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_gen_shroud_of_death_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_shroud_of_death_AuraScript();
    }
};

// 169869 - Transformation Sickness
class spell_gen_decimatus_transformation_sickness : public SpellScriptLoader
{
public:
    spell_gen_decimatus_transformation_sickness() : SpellScriptLoader("spell_gen_decimatus_transformation_sickness") { }

    class spell_gen_decimatus_transformation_sickness_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_decimatus_transformation_sickness_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
             if (Unit* target = GetHitUnit())
                 target->SetHealth(target->CountPctFromMaxHealth(25));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_decimatus_transformation_sickness_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_decimatus_transformation_sickness_SpellScript();
    }
};

// 189491 - Summon Towering Infernal.
class spell_gen_anetheron_summon_towering_infernal : public SpellScriptLoader
{
    public:
        spell_gen_anetheron_summon_towering_infernal() : SpellScriptLoader("spell_gen_anetheron_summon_towering_infernal") { }

        class spell_gen_anetheron_summon_towering_infernal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_anetheron_summon_towering_infernal_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
               GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_anetheron_summon_towering_infernal_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_anetheron_summon_towering_infernal_SpellScript();
        }
};

enum KazrogalHellfireMark
{
    SPELL_MARK_OF_KAZROGAL_HELLFIRE = 189512,
    SPELL_MARK_OF_KAZROGAL_DAMAGE_HELLFIRE = 189515
};

class MarkTargetHellfireFilter
{
    public:
        bool operator()(WorldObject* target) const
        {
            if (Unit* unit = target->ToUnit())
                return unit->GetPowerType() != POWER_MANA;
            return false;
        }
};

class spell_gen_mark_of_kazrogal_hellfire : public SpellScriptLoader
{
    public:
        spell_gen_mark_of_kazrogal_hellfire() : SpellScriptLoader("spell_gen_mark_of_kazrogal_hellfire") { }

        class spell_gen_mark_of_kazrogal_hellfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_mark_of_kazrogal_hellfire_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(MarkTargetHellfireFilter());
            }

            void Register() override
            {
                 OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_mark_of_kazrogal_hellfire_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        class spell_gen_mark_of_kazrogal_hellfire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_mark_of_kazrogal_hellfire_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                return ValidateSpellInfo({ SPELL_MARK_OF_KAZROGAL_DAMAGE_HELLFIRE });
            }

            void OnPeriodic(AuraEffect const* aurEff)
            {
                Unit* target = GetTarget();

                if (target->GetPower(POWER_MANA) == 0)
                {
                    target->CastSpell(target, SPELL_MARK_OF_KAZROGAL_DAMAGE_HELLFIRE, true, nullptr, aurEff);
                    // Remove aura
                    SetDuration(0);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_mark_of_kazrogal_hellfire_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_POWER_BURN);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_mark_of_kazrogal_hellfire_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_mark_of_kazrogal_hellfire_AuraScript();
        }
};

class spell_gen_azgalor_rain_of_fire_hellfire_citadel : public SpellScriptLoader
{
    public:
        spell_gen_azgalor_rain_of_fire_hellfire_citadel() : SpellScriptLoader("spell_gen_azgalor_rain_of_fire_hellfire_citadel") { }

        class spell_gen_azgalor_rain_of_fire_hellfire_citadel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_azgalor_rain_of_fire_hellfire_citadel_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
               GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_azgalor_rain_of_fire_hellfire_citadel_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_azgalor_rain_of_fire_hellfire_citadel_SpellScript();
        }
};

enum AuraProcRemoveSpells
{
    SPELL_FACE_RAGE         = 99947,
    SPELL_IMPATIENT_MIND    = 187213
};

// 99947 - Face Rage
class spell_gen_face_rage : public SpellScriptLoader
{
    public:
        spell_gen_face_rage() : SpellScriptLoader("spell_gen_face_rage") { }

        class spell_gen_face_rage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_face_rage_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                return ValidateSpellInfo({ SPELL_FACE_RAGE });
            }

            void OnRemove(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(GetSpellInfo()->GetEffect(EFFECT_2)->TriggerSpell);
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_face_rage_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_face_rage_AuraScript();
        }
};

// 187213 - Impatient Mind
class spell_gen_impatient_mind : public SpellScriptLoader
{
    public:
        spell_gen_impatient_mind() : SpellScriptLoader("spell_gen_impatient_mind") { }

        class spell_gen_impatient_mind_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_impatient_mind_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                return ValidateSpellInfo({ SPELL_IMPATIENT_MIND });
            }

            void OnRemove(AuraEffect const* effect, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(effect->GetSpellEffectInfo()->TriggerSpell);
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_impatient_mind_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_impatient_mind_AuraScript();
        }
};

enum EredarBloodmage
{
    SPELL_BLOOD_SIPHON_DAMAGE  = 235232,
    SPELL_BLOOD_SIPHON_HEAL    = 235262
};

// 235222
class spell_gen_eredar_bloodmage_blood_siphon : public AuraScript
{
    PrepareAuraScript(spell_gen_eredar_bloodmage_blood_siphon);

    void HandleEffectPeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();
        GetCaster()->CastSpell(GetTarget(), aurEff->GetAmount(), true);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_eredar_bloodmage_blood_siphon::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 235232 - Blood Siphon (damage)
class spell_gen_eredar_bloodmage_blood_siphon_damage : public SpellScript
{
    PrepareSpellScript(spell_gen_eredar_bloodmage_blood_siphon_damage);

    void CalculateDamage(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        if (!target)
            return;

        int32 healthPctDmg = target->CountPctFromCurHealth(GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints);
        SetHitDamage(healthPctDmg);
        target->CastCustomSpell(GetCaster(), SPELL_BLOOD_SIPHON_HEAL, &healthPctDmg, 0, 0, true);

    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_eredar_bloodmage_blood_siphon_damage::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 83958 - Mobile Bank
class spell_gen_mobile_bank : public SpellScript
{
    PrepareSpellScript(spell_gen_mobile_bank);

    enum
    {
        GOB_MOBILE_BANK = 206602
    };

    void SpawnChest(SpellEffIndex /*effIndex*/)
    {
        if (GetCaster()->IsPlayer() && GetCaster()->ToPlayer()->GetGuildId())
            GetCaster()->SummonGameObject(GOB_MOBILE_BANK, GetCaster()->GetPositionWithDistInFront(2.f), QuaternionData::fromEulerAnglesZYX(GetCaster()->GetOrientation() - float(M_PI), 0.f, 0.f), 5 * MINUTE * IN_MILLISECONDS);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_mobile_bank::SpawnChest, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Arcane Pulse (Nightborne racial) - 260364
class spell_arcane_pulse : public SpellScript
{
    PrepareSpellScript(spell_arcane_pulse);

    void HandleDamage(SpellEffIndex /*effIndex*/)
    {
        float damage = GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK) * 2.f;

        if (!damage)
            damage = float(GetCaster()->GetTotalSpellPowerValue(SPELL_SCHOOL_MASK_ALL, false)) * 0.75f;

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_arcane_pulse::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

enum SpatialRiftSpells
{
    SPELL_SPATIAL_RIFT_AT           = 256948,
    SPELL_SPATIAL_RIFT_TELEPORT     = 257034,
    SPELL_SPATIAL_RIFT_DESPAWN_AT   = 257040
};

// Spatial Rift teleport (Void Elf racial) - 257040
class spell_spatial_rift_despawn : public SpellScript
{
    PrepareSpellScript(spell_spatial_rift_despawn);

    void OnDespawnAreaTrigger(SpellEffIndex /*effIndex*/)
    {
        if (AreaTrigger* at = GetCaster()->GetAreaTrigger(SPELL_SPATIAL_RIFT_AT))
        {
            GetCaster()->CastSpell(at->GetPosition(), SPELL_SPATIAL_RIFT_TELEPORT, true);
            at->SetDuration(0);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_spatial_rift_despawn::OnDespawnAreaTrigger, EFFECT_1, SPELL_EFFECT_REMOVE_AURA);
    }
};

// Light's Judgement - 256893  (Lightforged Draenei Racial)
class spell_light_judgement : public SpellScript
{
    PrepareSpellScript(spell_light_judgement);

    void HandleDamage(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            SetHitDamage(6.25f * caster->m_unitData->AttackPower);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_light_judgement::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Light's Reckoning - 255652 (Lightforged Draenei Racial)
class playerscript_light_reckoning : public PlayerScript
{
public:
    playerscript_light_reckoning() : PlayerScript("playerscript_light_reckoning") { }

    enum
    {
        SPELL_LIGHT_RECKONING = 255652
    };

    void OnDeath(Player* player) override
    {
        if (player->HasAura(SPELL_LIGHT_RECKONING))
            if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_LIGHT_RECKONING))
                if (SpellEffectInfo const* effectInfo = info->GetEffect(EFFECT_0))
                    player->CastSpell(player, effectInfo->TriggerSpell, true);
    }
};

// 214027 - Adaptation
class spell_pvp_honor_adaptation : public SpellScriptLoader
{
public:
    spell_pvp_honor_adaptation() : SpellScriptLoader("spell_pvp_honor_adaptation") { }

    enum AdaptionSpells
    {
        SPELL_ADAPTATION_DEBUFF = 195901,
        SPELL_PVP_TRINKET = 42292
    };

    class spell_pvp_honor_adaptation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pvp_honor_adaptation_AuraScript);

        bool Validate(SpellInfo const* spellInfo) override
        {
            return spellInfo->GetEffect(EFFECT_0) && ValidateSpellInfo(
                {
                    SPELL_ADAPTATION_DEBUFF,
                    SPELL_PVP_TRINKET
                });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetActionTarget())
                return false;

            if (eventInfo.GetActionTarget()->HasAura(SPELL_ADAPTATION_DEBUFF))
                return false;

            auto isLossOfControlSpell = [](SpellInfo const* spellInfo) -> bool
            {
                return (spellInfo->GetAllEffectsMechanicMask() & IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK) != 0;
            };

            if (!eventInfo.GetSpellInfo() || !isLossOfControlSpell(eventInfo.GetSpellInfo()))
                return false;

            int32 durationLimit = GetEffect(EFFECT_0)->GetAmount() * IN_MILLISECONDS;

            /// @workaround: we need to check the entire spell instead of the auras on the unit
            auto hasLossOfControlAuras = [&]() -> bool
            {
                Unit::AuraApplicationMap const& appliedAuras = eventInfo.GetActionTarget()->GetAppliedAuras();
                for (auto itr = appliedAuras.begin(); itr != appliedAuras.end(); ++itr)
                {
                    Aura const* aura = itr->second->GetBase();

                    if (aura->GetDuration() < durationLimit)
                        continue;

                    if (isLossOfControlSpell(aura->GetSpellInfo()))
                        return true;
                }

                return false;
            };

            return hasLossOfControlAuras();
        }

        void Proc(ProcEventInfo& /*eventInfo*/)
        {
            Unit* target = GetTarget();
            target->CastSpell(target, SPELL_ADAPTATION_DEBUFF, true);
            target->CastSpell(target, SPELL_PVP_TRINKET, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_pvp_honor_adaptation_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_pvp_honor_adaptation_AuraScript::Proc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pvp_honor_adaptation_AuraScript();
    }
};

// 42292 - PvP Trinket
// 195710 - Honorable Medallion
// 208683 - Gladiator's Medallion
class spell_pvp_honor_medallion : public SpellScriptLoader
{
public:
    spell_pvp_honor_medallion() : SpellScriptLoader("spell_pvp_honor_medallion") { }

    enum MedallionDummys
    {
        SPELL_GENERIC_PVP_TRINKET_DUMMY_ALLIANCE = 97403,
        SPELL_GENERIC_PVP_TRINKET_DUMMY_HORDE = 97404,

        VISUAL_KIT_ACTIVE_TRINKET = 701
    };

    class spell_pvp_honor_medallion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pvp_honor_medallion_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo(
                {
                    SPELL_GENERIC_PVP_TRINKET_DUMMY_ALLIANCE,
                    SPELL_GENERIC_PVP_TRINKET_DUMMY_HORDE
                });
        }

        void TriggerDummy()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->GetTeamId() == TEAM_ALLIANCE)
                    player->CastSpell(player, SPELL_GENERIC_PVP_TRINKET_DUMMY_ALLIANCE, true);
                else
                    player->CastSpell(player, SPELL_GENERIC_PVP_TRINKET_DUMMY_HORDE, true);

                // send to caster only
                player->SendPlaySpellVisualKit(VISUAL_KIT_ACTIVE_TRINKET, 0, 0);
            }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_pvp_honor_medallion_SpellScript::TriggerDummy);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pvp_honor_medallion_SpellScript();
    }
};

enum GilneasPrison
{
    SPELL_SUMMON_RAVENOUS_WORGEN_1 = 66836,
    SPELL_SUMMON_RAVENOUS_WORGEN_2 = 66925,

    NPC_WORGEN_RUNT                = 35456,
};

Position const WorgenRuntHousePos[] =
{
    // House Roof
    { -1729.345f, 1526.495f, 55.47962f, 6.188943f },
    { -1709.63f, 1527.464f, 56.86086f, 3.258752f },
    { -1717.75f, 1513.727f, 55.47941f, 4.704845f },
    { -1724.719f, 1526.731f, 55.66177f, 6.138319f },
    { -1713.974f, 1526.625f, 56.21981f, 3.306195f },
    { -1718.104f, 1524.071f, 55.81641f, 4.709816f },
    { -1718.262f, 1518.557f, 55.55954f, 4.726997f },
    // Cathdral Roof
    { -1618.054f, 1489.644f, 68.45153f, 3.593639f },
    { -1625.62f, 1487.033f, 71.27762f, 3.531424f },
    { -1638.569f, 1489.736f, 68.55273f, 4.548815f },
    { -1630.399f, 1481.66f, 71.41516f, 3.484555f },
    { -1622.424f, 1483.882f, 67.67381f, 3.404875f },
    { -1634.344f, 1491.3f, 70.10101f, 4.6248f },
    { -1631.979f, 1491.585f, 71.11481f, 4.032866f },
    { -1627.273f, 1499.689f, 68.89395f, 4.251452f },
    { -1622.665f, 1489.818f, 71.03797f, 3.776179f },
};

class spell_gen_gilneas_prison_periodic_dummy : public SpellScriptLoader
{
    public:
        spell_gen_gilneas_prison_periodic_dummy() : SpellScriptLoader("spell_gen_gilneas_prison_periodic_dummy") { }

        class spell_gen_gilneas_prison_periodic_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_gilneas_prison_periodic_dummy_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_SUMMON_RAVENOUS_WORGEN_1, // House roof
                        SPELL_SUMMON_RAVENOUS_WORGEN_2, // Cathedral roof
                    });
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    switch (RAND(0, 1))
                    {
                        case 0:
                            caster->CastSpell(caster, SPELL_SUMMON_RAVENOUS_WORGEN_1, true);
                            for (uint8 i = 0; i < 7; i++)
                                if (Creature* runt = caster->SummonCreature(NPC_WORGEN_RUNT, WorgenRuntHousePos[i]))
                                    runt->AI()->DoAction(i);
                            break;
                        case 1:
                            caster->CastSpell(caster, SPELL_SUMMON_RAVENOUS_WORGEN_2, true);
                            for (uint8 i = 7; i < 16; i++)
                                if (Creature* runt = caster->SummonCreature(NPC_WORGEN_RUNT, WorgenRuntHousePos[i]))
                                    runt->AI()->DoAction(i);
                            if (RAND(0, 1) == 1)
                                for (uint8 i = 0; i < RAND(1, 3); i++)
                                    if (Creature* runt = caster->SummonCreature(NPC_WORGEN_RUNT, WorgenRuntHousePos[i]))
                                        runt->AI()->DoAction(i);
                            break;
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_gilneas_prison_periodic_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_gilneas_prison_periodic_dummy_SpellScript();
        }
};

enum ThrowTorch
{
    CREDIT_ROUND_UP_WORGEN  = 35582,
    SPELL_THROW_TORCH       = 67063
};

class spell_gen_throw_torch : public SpellScriptLoader
{
    public:
        spell_gen_throw_torch() : SpellScriptLoader("spell_gen_throw_torch") { }

        class spell_gen_throw_torch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_throw_torch_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo( { SPELL_THROW_TORCH });
            }

            void HandleBeforeHit(SpellMissInfo missInfo)
            {
                if (missInfo != SPELL_MISS_NONE)
                    return;

                if (Player* player = GetCaster()->ToPlayer())
                    if (GetHitUnit() && !GetHitUnit()->HasAura(SPELL_THROW_TORCH))
                        player->KilledMonsterCredit(CREDIT_ROUND_UP_WORGEN);
            }

            void Register() override
            {
                BeforeHit += BeforeSpellHitFn(spell_gen_throw_torch_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_throw_torch_SpellScript();
        }
};

class spell_sha_cloud : public SpellScriptLoader
{
public:
    spell_sha_cloud() : SpellScriptLoader("spell_sha_cloud") { }

    class spell_sha_cloud_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_cloud_SpellScript);

        void HandleOnCast()
        {
            if (Unit* caster = GetCaster())
                caster->GetMotionMaster()->MoveJump(1273.716f, 1039.498f, 434.867f, 20.0f, 15.0f, GetSpellInfo()->Id, 0.966f, 0);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_sha_cloud_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_sha_cloud_SpellScript();
    }
};

class spell_endurance_of_niuzao : public SpellScriptLoader
{
public:
    spell_endurance_of_niuzao() : SpellScriptLoader("spell_endurance_of_niuzao") { }

    class spell_endurance_of_niuzao_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_endurance_of_niuzao_AuraScript);

        void AfterAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, float& absorbAmount)
        {
            if (Unit* owner = GetUnitOwner())
            {
                if (Player* plr = owner->ToPlayer())
                   // if (dmgInfo.GetAbsorb() < owner->GetHealth() || owner->HasAura(148010) || !plr->isInTankSpec())
                    {
                        dmgInfo.AbsorbDamage(-(int32(absorbAmount)));
                       // if (SpellInfo const* info = aurEff->GetSpellInfo())
                           // if (aurEff->GetAmount() != info->Effects[EFFECT_0]->BasePoints)
                            //    aurEff->SetAmount(info->Effects[EFFECT_0]->BasePoints);
                    }
                    else
                    {
                        owner->AddAura(148010, owner);
                        owner->AddAura(148958, owner);
                    }
            }
        }

        void Register() override
        {
           // AfterEffectAbsorb += AuraEffectAbsorbFn(spell_endurance_of_niuzao_AuraScript::AfterAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_endurance_of_niuzao_AuraScript();
    }
};

// Feast (pet ability) - 159953
class spell_gen_feast : public SpellScriptLoader
{
public:
    spell_gen_feast() : SpellScriptLoader("spell_gen_feast") { }

    class spell_gen_feast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_feast_SpellScript);

        SpellCastResult CheckIfCorpseNear()
        {
            Unit* caster = GetCaster();
            float max_range = GetSpellInfo()->GetMaxRange(false);
            WorldObject* result = nullptr;
            // search for nearby enemy corpse in range
            Trinity::AnyDeadUnitSpellTargetInRangeCheck check(caster, max_range, GetSpellInfo(), TARGET_CHECK_ENEMY);
            Trinity::WorldObjectSearcher<Trinity::AnyDeadUnitSpellTargetInRangeCheck> searcher(caster, result, check);
            //caster->GetMap()->VisitFirstFound(caster->m_positionX, caster->m_positionY, max_range, searcher);
            if (!result)
                return SPELL_FAILED_NO_EDIBLE_CORPSES;
            return SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, 159954, false);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_gen_feast_SpellScript::CheckIfCorpseNear);
            OnEffectHit += SpellEffectFn(spell_gen_feast_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_feast_SpellScript();
    }
};

// 24750 Trick
enum TrickSpells
{
    SPELL_PIRATE_COSTUME_MALE = 24708,
    SPELL_PIRATE_COSTUME_FEMALE = 24709,
    SPELL_NINJA_COSTUME_MALE = 24710,
    SPELL_NINJA_COSTUME_FEMALE = 24711,
    SPELL_LEPER_GNOME_COSTUME_MALE = 24712,
    SPELL_LEPER_GNOME_COSTUME_FEMALE = 24713,
    SPELL_SKELETON_COSTUME = 24723,
    SPELL_GHOST_COSTUME_MALE = 24735,
    SPELL_GHOST_COSTUME_FEMALE = 24736,
    SPELL_TRICK_BUFF = 24753,
};

class spell_gen_trick : public SpellScriptLoader
{
public:
    spell_gen_trick() : SpellScriptLoader("spell_gen_trick") {}

    class spell_gen_trick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_trick_SpellScript);
        bool Validate(SpellInfo const* /*SpellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PIRATE_COSTUME_MALE) || !sSpellMgr->GetSpellInfo(SPELL_PIRATE_COSTUME_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_NINJA_COSTUME_MALE)
                || !sSpellMgr->GetSpellInfo(SPELL_NINJA_COSTUME_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_LEPER_GNOME_COSTUME_MALE) || !sSpellMgr->GetSpellInfo(SPELL_LEPER_GNOME_COSTUME_FEMALE)
                || !sSpellMgr->GetSpellInfo(SPELL_SKELETON_COSTUME) || !sSpellMgr->GetSpellInfo(SPELL_GHOST_COSTUME_MALE) || !sSpellMgr->GetSpellInfo(SPELL_GHOST_COSTUME_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_TRICK_BUFF))
                return false;
            return true;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Player* target = GetHitPlayer())
            {
                uint8 gender = target->getGender();
                uint32 spellId = SPELL_TRICK_BUFF;
                switch (urand(0, 5))
                {
                case 1:
                    spellId = gender ? SPELL_LEPER_GNOME_COSTUME_FEMALE : SPELL_LEPER_GNOME_COSTUME_MALE;
                    break;
                case 2:
                    spellId = gender ? SPELL_PIRATE_COSTUME_FEMALE : SPELL_PIRATE_COSTUME_MALE;
                    break;
                case 3:
                    spellId = gender ? SPELL_GHOST_COSTUME_FEMALE : SPELL_GHOST_COSTUME_MALE;
                    break;
                case 4:
                    spellId = gender ? SPELL_NINJA_COSTUME_FEMALE : SPELL_NINJA_COSTUME_MALE;
                    break;
                case 5:
                    spellId = SPELL_SKELETON_COSTUME;
                    break;
                default:
                    break;
                }

                caster->CastSpell(target, spellId, true, nullptr);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_trick_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_trick_SpellScript();
    }
};

// 24751 Trick or Treat
enum TrickOrTreatSpells
{
    SPELL_TRICK = 24714,
    SPELL_TREAT = 24715,
    SPELL_TRICKED_OR_TREATED = 24755,
    SPELL_TRICKY_TREAT_SPEED = 42919,
    SPELL_TRICKY_TREAT_TRIGGER = 42965,
    SPELL_UPSET_TUMMY = 42966
};

class spell_gen_trick_or_treat : public SpellScriptLoader
{
public:
    spell_gen_trick_or_treat() : SpellScriptLoader("spell_gen_trick_or_treat") {}

    class spell_gen_trick_or_treat_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_trick_or_treat_SpellScript);

        bool Validate(SpellInfo const* /*SpellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_TRICK) || !sSpellMgr->GetSpellInfo(SPELL_TREAT) || !sSpellMgr->GetSpellInfo(SPELL_TRICKED_OR_TREATED))
                return false;
            return true;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Player* target = GetHitPlayer())
            {
                caster->CastSpell(target, roll_chance_i(50) ? SPELL_TRICK : SPELL_TREAT, true, nullptr);
                caster->CastSpell(target, SPELL_TRICKED_OR_TREATED, true, nullptr);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_trick_or_treat_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_trick_or_treat_SpellScript();
    }
};

enum parachuteIC
{
    SPELL_PARACHUTE_IC = 66657,
};

class spell_gen_parachute_ic : public SpellScriptLoader
{
public:
    spell_gen_parachute_ic() : SpellScriptLoader("spell_gen_parachute_ic") { }

    class spell_gen_parachute_ic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_parachute_ic_AuraScript);

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
        {
            if (Player* target = GetTarget()->ToPlayer())
                //if (target->m_movementInfo.fall.fallTime > 2000)
                    target->CastSpell(target, SPELL_PARACHUTE_IC, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_parachute_ic_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_parachute_ic_AuraScript();
    }
};


class spell_gen_gunship_portal : public SpellScriptLoader
{
public:
    spell_gen_gunship_portal() : SpellScriptLoader("spell_gen_gunship_portal") { }

    class spell_gen_gunship_portal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_gunship_portal_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (Battleground* bg = caster->GetBattleground())
              //  if (bg->GetTypeID(true) == MS::Battlegrounds::BattlegroundTypeId::BattlegroundIsleOfConquest)
                    bg->DoAction(1, caster->GetGUID());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_gunship_portal_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_gunship_portal_SpellScript();
    }
};

enum MagicRoosterSpells
{
    SPELL_MAGIC_ROOSTER_NORMAL = 66122,
    SPELL_MAGIC_ROOSTER_DRAENEI_MALE = 66123,
    SPELL_MAGIC_ROOSTER_TAUREN_MALE = 66124,
};

class spell_gen_magic_rooster : public SpellScriptLoader
{
public:
    spell_gen_magic_rooster() : SpellScriptLoader("spell_gen_magic_rooster") { }

    class spell_gen_magic_rooster_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_magic_rooster_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Player* target = GetHitPlayer())
            {
                // prevent client crashes from stacking mounts
                target->RemoveAurasByType(SPELL_AURA_MOUNTED);

                uint32 spellId = SPELL_MAGIC_ROOSTER_NORMAL;
                switch (target->getRace())
                {
                case RACE_DRAENEI:
                    if (target->getGender() == GENDER_MALE)
                        spellId = SPELL_MAGIC_ROOSTER_DRAENEI_MALE;
                    break;
                case RACE_TAUREN:
                    if (target->getGender() == GENDER_MALE)
                        spellId = SPELL_MAGIC_ROOSTER_TAUREN_MALE;
                    break;
                default:
                    break;
                }

                target->CastSpell(target, spellId, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_magic_rooster_SpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_magic_rooster_SpellScript();
    }
};

enum Launch
{
    SPELL_LAUNCH_NO_FALLING_DAMAGE = 66251
};

class spell_gen_launch : public SpellScriptLoader
{
public:
    spell_gen_launch() : SpellScriptLoader("spell_gen_launch") {}

    class spell_gen_launch_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_launch_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (Player* player = GetHitPlayer())
                player->AddAura(SPELL_LAUNCH_NO_FALLING_DAMAGE, player); // prevents falling damage
        }

        void Launch()
        {
            WorldLocation const* const position = GetExplTargetDest();

            if (Player* player = GetHitPlayer())
            {
                player->ExitVehicle();

                // A better research is needed
                // There is no spell for this, the following calculation was based on void Spell::CalculateJumpSpeeds

                float speedZ = 10.0f;
                float dist = position->GetExactDist2d(player->GetPositionX(), player->GetPositionY());
                float speedXY = dist;

                player->GetMotionMaster()->MoveJump(position->GetPositionX(), position->GetPositionY(), position->GetPositionZ(), speedXY, speedZ);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_launch_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_FORCE_CAST);
            AfterHit += SpellHitFn(spell_gen_launch_SpellScript::Launch);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_launch_SpellScript();
    }
};


enum DummyTrigger
{
    SPELL_PERSISTANT_SHIELD_TRIGGERED = 26470,
    SPELL_PERSISTANT_SHIELD = 26467,
};

class spell_gen_dummy_trigger : public SpellScriptLoader
{
public:
    spell_gen_dummy_trigger() : SpellScriptLoader("spell_gen_dummy_trigger") { }

    class spell_gen_dummy_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_dummy_trigger_SpellScript);

        bool Validate(SpellInfo const* /*SpellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PERSISTANT_SHIELD_TRIGGERED) || !sSpellMgr->GetSpellInfo(SPELL_PERSISTANT_SHIELD))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            float damage = GetEffectValue();
            Unit* caster = GetCaster();
           // if (Unit* target = GetHitUnit())
              //  if (SpellInfo const* triggeredByAuraSpell = GetTriggeringSpell())
                  //  if (triggeredByAuraSpell->Id == SPELL_PERSISTANT_SHIELD_TRIGGERED)
                        //caster->CastCustomSpell(target, SPELL_PERSISTANT_SHIELD_TRIGGERED, &damage, nullptr, nullptr, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_dummy_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_dummy_trigger_SpellScript();
    }

};


// 201632
class spell_legion_hearty_feast : public AuraScript
{
    PrepareAuraScript(spell_legion_hearty_feast);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (auto caster = GetUnitOwner())
            caster->CastSpell(caster, 192002, true);
    }

    void OnTick(AuraEffect const* /*aurEff*/)
    {
        if (auto caster = GetUnitOwner())
        {
            if (auto plr = caster->ToPlayer())
            {
                uint32 spellId = 0;

                //switch (plr->GetSpecType())
                {
              //  case DPS_INTELLECT:
               // case HEALER_INTELLECT:
                    spellId = 201636;
                  //  break;
             //   case DPS_STRENGH:
                    spellId = 201634;
                //    break;
               // case DPS_AGILITY:
                    spellId = 201635;
               //     break;
               // case TANK_AGILITY:
              //  case TANK_STRENGH:
                    spellId = 201637;
                //    break;
              //  default:
                //    break;
                }
                caster->CastSpell(caster, spellId, true);
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_legion_hearty_feast::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_legion_hearty_feast::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};


// 201644
class spell_legion_food_table : public AuraScript
{
    PrepareAuraScript(spell_legion_food_table);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (auto caster = GetUnitOwner())
        {
            caster->CastSpell(caster, 192002, true);
        }
    }

    void OnTick(AuraEffect const* /*aurEff*/)
    {
        if (auto caster = GetUnitOwner())
        {
            if (auto plr = caster->ToPlayer())
            {
                uint32 spellId = 0;

              //  switch (plr->GetSpecType())
                {
              //  case DPS_INTELLECT:
             //   case HEALER_INTELLECT:
                    spellId = 201640;
               //     break;
             //   case DPS_STRENGH:
                    spellId = 201638;
              //      break;
             //   case DPS_AGILITY:
                    spellId = 201639;
               //     break;
              //  case TANK_AGILITY:
              //  case TANK_STRENGH:
                    spellId = 201641;
               //     break;
              //  default:
              //      break;
                }

                caster->CastSpell(caster, spellId, true);
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_legion_food_table::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_legion_food_table::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};


//176888
class spell_gen_hearthstone_board : public SpellScript
{
    PrepareSpellScript(spell_gen_hearthstone_board);

    void HandleOnCast()
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        std::string text;

        if (auto entry = sBroadcastTextStore.LookupEntry(90913))
        {
            text = DB2Manager::GetBroadcastTextValue(entry, player->GetSession()->GetSessionDbLocaleIndex());
            player->TextEmote(text);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_gen_hearthstone_board::HandleOnCast);
    }
};


//176890
class spell_gen_winning_hand : public AuraScript
{
    PrepareAuraScript(spell_gen_winning_hand);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        std::string text;

        if (auto entry = sBroadcastTextStore.LookupEntry(90915))
        {
            text = DB2Manager::GetBroadcastTextValue(entry, player->GetSession()->GetSessionDbLocaleIndex());
            player->TextEmote(text);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_gen_winning_hand::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

//217668
class spell_request_the_master_call_on_you : public AuraScript
{
    PrepareAuraScript(spell_request_the_master_call_on_you);

    std::string text;
    uint32 replics[27] = { 121859, 121860, 121861, 121862, 121863, 121864, 121865, 121866, 121867, 121868, 121869, 121870, 121871, 121872, 121873, 121874, 121875, 121876, 121877, 121878, 121879, 121880, 121881, 121882, 121883, 121884, 121885 };

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        if (auto entry = sBroadcastTextStore.LookupEntry(replics[urand(0, 26)]))
        {
            text = DB2Manager::GetBroadcastTextValue(entry, player->GetSession()->GetSessionDbLocaleIndex());
            //player->BossWhisper(text, LANG_UNIVERSAL, player->GetGUID());
        }
    }

    void OnTick(AuraEffect const* /*aurEff*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            return;

        if (auto entry = sBroadcastTextStore.LookupEntry(replics[urand(0, 26)]))
        {
            text = DB2Manager::GetBroadcastTextValue(entry, player->GetSession()->GetSessionDbLocaleIndex());
         //   player->BossWhisper(text, LANG_UNIVERSAL, player->GetGUID());
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_request_the_master_call_on_you::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_request_the_master_call_on_you::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

//128018
class spell_searing_gaze_of_the_dook : public SpellScript
{
    PrepareSpellScript(spell_searing_gaze_of_the_dook);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();
        if (!target || !caster)
            return;

        caster->AddAura(128054, caster);
        caster->CastSpell(target, 127902, false);
        target->CastSpell(caster, 127903, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_searing_gaze_of_the_dook::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

//127902
class spell_searing_gaze_of_the_dook_hit : public SpellScript
{
    PrepareSpellScript(spell_searing_gaze_of_the_dook_hit);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit()->ToCreature();
        Unit* caster = GetCaster();
        if (!target || !caster)
            return;

        target->CastSpell(target, 127917, true);
        target->CastSpell(target, 127919, true);
        target->CastSpell(target, 127920, true);
        caster->CastSpell(target, 128058, true);
        target->RemoveAurasDueToSpell(128024);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_searing_gaze_of_the_dook_hit::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

//128058
class spell_searing_gaze_of_the_dook_despawn : public AuraScript
{
    PrepareAuraScript(spell_searing_gaze_of_the_dook_despawn);

    void Tick(AuraEffect const* aurEff)
    {
        Unit* target = GetTarget()->ToCreature();
        if (!target)
            return;

        target->ToCreature()->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_searing_gaze_of_the_dook_despawn::Tick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};


// 246293
class spell_set_up_board : public SpellScript
{
    PrepareSpellScript(spell_set_up_board);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster)
            return;

        if (caster->HasAura(246351))
            if (auto sum = caster->FindNearestCreature(123794, 2.0f))
                sum->GetAI()->DoAction(3);

        if (caster->HasAura(246289))
            if (auto sum = caster->FindNearestCreature(123793, 2.0f))
                sum->GetAI()->DoAction(1);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_set_up_board::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

//246290
class spell_deck_o_matic : public SpellScript
{
    PrepareSpellScript(spell_deck_o_matic);

    void HandleOnCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (caster->HasAura(246351))
            if (auto sum = caster->FindNearestCreature(123794, 2.0f))
                sum->GetAI()->DoAction(1);

        if (caster->HasAura(246289))
            if (auto sum = caster->FindNearestCreature(123793, 2.0f))
                sum->GetAI()->DoAction(1);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_deck_o_matic::HandleOnCast);
    }
};

//246292
class spell_improved_card_pack : public SpellScript
{
    PrepareSpellScript(spell_improved_card_pack);

    void HandleOnCast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (caster->HasAura(246351))
            if (auto sum = caster->FindNearestCreature(123794, 2.0f))
                sum->GetAI()->DoAction(2);

        if (caster->HasAura(246289))
            if (auto sum = caster->FindNearestCreature(123793, 2.0f))
                sum->GetAI()->DoAction(2);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_improved_card_pack::HandleOnCast);
    }
};

//228704
class spell_kaldorei_light_globe : public SpellScript
{
    PrepareSpellScript(spell_kaldorei_light_globe);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        if (!target)
            return;

        if (target->HasAura(228705))
            target->RemoveAura(228705);
        else
            target->AddAura(228705, target);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_kaldorei_light_globe::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

//246823
class spell_throw_sphere_catch : public SpellScript
{
    PrepareSpellScript(spell_throw_sphere_catch);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();
        if (!target || !caster)
            return;

        uint32 rand_spell[3] = { 246810, 246811, 246812 };
        target->CastSpell(caster, rand_spell[urand(0, 2)], false);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_throw_sphere_catch::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 246351
class spell_hearthstation_horde : public AuraScript
{
    PrepareAuraScript(spell_hearthstation_horde);

    ObjectGuid guid;

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster)
            return;

        Position pos;
        caster->GetFirstCollisionPosition(1.4f, 0.0f);
        if (auto sum = caster->SummonCreature(123794, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), caster->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
            guid = sum->GetGUID();
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster)
            return;

       // if (auto sum = Unit::GetCreature(*caster, guid))
         //   sum->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_hearthstation_horde::OnApply, EFFECT_1, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_hearthstation_horde::OnRemove, EFFECT_1, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
    }
};

//246289
class spell_hearthstation_alliance : public AuraScript
{
    PrepareAuraScript(spell_hearthstation_alliance);

    ObjectGuid guid;

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster)
            return;

        Position pos;
        caster->GetFirstCollisionPosition(1.4f, 0.0f);
        if (auto sum = caster->SummonCreature(123793, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), caster->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
            guid = sum->GetGUID();
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (!caster)
            return;

    //    if (auto sum = Unit::GetCreature(*caster, guid))
        //    sum->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_hearthstation_alliance::OnApply, EFFECT_1, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_hearthstation_alliance::OnRemove, EFFECT_1, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
    }
};

// 99424
class spell_q29347_crayfish : public AuraScript
{
    PrepareAuraScript(spell_q29347_crayfish);

    uint32 m_timer = 2000;

    void OnUpdate(uint32 diff, AuraEffect* aurEff)
    {
        auto caster = GetCaster();
        if (!caster)
            return;

        if (m_timer <= diff)
        {
            m_timer = 2000;
            if (caster->FindNearestCreature(1355, 3.f, true))
            {
                caster->RemoveAura(99424);
                if (auto plr = caster->ToPlayer())
                    plr->KilledMonsterCredit(53551);
            }
        }
        else
            m_timer -= diff;
    }

    void Register() override
    {
       // OnEffectUpdate += AuraEffectUpdateFn(spell_q29347_crayfish::OnUpdate, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID);
    }
};

// 63520
class spell_q13698_saw : public SpellScript
{
    PrepareSpellScript(spell_q13698_saw);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        auto caster = GetCaster();
        if (!caster)
            return;

        if (auto target = GetHitGObj())
        {
            if (target->GetEntry() == 194482 && target->GetGoState() != GO_STATE_ACTIVE)
                if (auto veh = caster->GetVehicleKit())
                    if (auto pass = veh->GetPassenger(0))
                        if (auto plr = pass->ToPlayer())
                            plr->KillCreditGO(194482, ObjectGuid::Empty);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_q13698_saw::HandleDummy, EFFECT_0, SPELL_EFFECT_ACTIVATE_OBJECT);
    }
};

// 63517
class spell_q13698_shredder_aura : public AuraScript
{
    PrepareAuraScript(spell_q13698_shredder_aura);

    uint32 m_timer = 2000;

    void OnUpdate(uint32 diff, AuraEffect* aurEff)
    {
        auto caster = GetCaster();
        if (!caster)
            return;

        if (m_timer <= diff)
        {
            m_timer = 1000;
            if (auto plr = caster->ToPlayer())
            {
                if (plr->FindNearestCreature(17287, 6.f))
                {
                    uint32 count = plr->GetQuestObjectiveData(13698, int8(194482));
                    if (count >= uint32(8))
                    {
                        plr->KilledMonsterCredit(33709);
                        if (auto veh = plr->GetVehicleBase())
                        {
                            if (auto cre = veh->ToCreature())
                            {
                                cre->DespawnOrUnsummon(500);
                                plr->RemoveAura(65317);
                            }
                        }
                    }
                }
            }
        }
        else
            m_timer -= diff;
    }

    void Register() override
    {
       // OnEffectUpdate += AuraEffectUpdateFn(spell_q13698_shredder_aura::OnUpdate, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE);
    }
};

// 99435
class spell_q29347_bait : public AuraScript
{
    PrepareAuraScript(spell_q29347_bait);

    uint32 m_timer = 2000;

    void OnUpdate(uint32 diff, AuraEffect* aurEff)
    {
        auto caster = GetCaster();
        if (!caster)
            return;

        if (m_timer <= diff)
        {
            m_timer = 2000;
           // if (caster->IsInWater() && caster->GetAreaID() == 212)
            {
                caster->CastSpell(caster, 99424, true);
                caster->RemoveAura(99435);
                uint8 seat = 0;
                for (uint8 i = 0; i < 7; ++i)
                {
                    if (auto cre = caster->SummonCreature(71004, caster->GetPosition(), TEMPSUMMON_CORPSE_DESPAWN, 300000))
                    {
                        //cre->AddPlayerInPersonnalVisibilityList(caster->GetGUID());
                        cre->EnterVehicle(caster, seat);
                        ++seat;
                    }
                }
            }
        }
        else
            m_timer -= diff;
    }

    void Register() override
    {
      //  OnEffectUpdate += AuraEffectUpdateFn(spell_q29347_bait::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_gen_love_seat : public SpellScript
{
    PrepareSpellScript(spell_gen_love_seat);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        if (Unit* target = GetHitUnit())
        {
            if (target->HasAura(56687))
                caster->CastSpell(target, 63345, true);
            else if (target->HasAura(63345))
                caster->CastSpell(target, 56687, true);

            if (!target->HasAura(56687))
                caster->CastSpell(target, 63345, true);
            else if (!target->HasAura(63345))
                caster->CastSpell(target, 56687, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_gen_love_seat::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_gen_dream_funnel : public SpellScriptLoader
{
public:
    spell_gen_dream_funnel() : SpellScriptLoader("spell_gen_dream_funnel") { }

    class spell_gen_dream_funnel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_dream_funnel_AuraScript);

        void HandleEffectCalcAmount(AuraEffect const* /*aurEff*/, float& amount, bool& canBeRecalculated)
        {
            if (GetCaster())
                amount = GetCaster()->GetMaxHealth() * 0.05f;

            canBeRecalculated = false;
        }

        void Register() override
        {
           // DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_dream_funnel_AuraScript::HandleEffectCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
         //   DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_dream_funnel_AuraScript::HandleEffectCalcAmount, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_dream_funnel_AuraScript();
    }
};


enum Mounts
{
    // Magic Broom
    SPELL_MAGIC_BROOM_60 = 42680,
    SPELL_MAGIC_BROOM_100 = 42683,
    SPELL_MAGIC_BROOM_150 = 42667,
    SPELL_MAGIC_BROOM_280 = 42668,

    // Headless Horseman's Mount
    SPELL_HEADLESS_HORSEMAN_MOUNT_60 = 51621,
    SPELL_HEADLESS_HORSEMAN_MOUNT_100 = 48024,
    SPELL_HEADLESS_HORSEMAN_MOUNT_150 = 51617,
    SPELL_HEADLESS_HORSEMAN_MOUNT_280 = 48023,

    // Winged Steed of the Ebon Blade
    SPELL_WINGED_STEED_150 = 54726,
    SPELL_WINGED_STEED_280 = 54727,

    // Big Love Rocket
    SPELL_BIG_LOVE_ROCKET_0 = 71343,
    SPELL_BIG_LOVE_ROCKET_60 = 71344,
    SPELL_BIG_LOVE_ROCKET_100 = 71345,
    SPELL_BIG_LOVE_ROCKET_150 = 71346,
    SPELL_BIG_LOVE_ROCKET_310 = 71347,

    // Invincible
    SPELL_INVINCIBLE_60 = 72281,
    SPELL_INVINCIBLE_100 = 72282,
    SPELL_INVINCIBLE_150 = 72283,
    SPELL_INVINCIBLE_310 = 72284,

    // Blazing Hippogryph
    SPELL_BLAZING_HIPPOGRYPH_150 = 74854,
    SPELL_BLAZING_HIPPOGRYPH_280 = 74855,

    // Celestial Steed
    SPELL_CELESTIAL_STEED_60 = 75619,
    SPELL_CELESTIAL_STEED_100 = 75620,
    SPELL_CELESTIAL_STEED_150 = 75617,
    SPELL_CELESTIAL_STEED_280 = 75618,
    SPELL_CELESTIAL_STEED_310 = 76153,

    // X-53 Touring Rocket
    SPELL_X53_TOURING_ROCKET_150 = 75957,
    SPELL_X53_TOURING_ROCKET_280 = 75972,
    SPELL_X53_TOURING_ROCKET_310 = 76154,
};

class spell_gen_mount : public SpellScriptLoader
{
public:
    spell_gen_mount(const char* name, uint32 mount0 = 0, uint32 mount60 = 0, uint32 mount100 = 0, uint32 mount150 = 0, uint32 mount280 = 0, uint32 mount310 = 0) : SpellScriptLoader(name),
        _mount0(mount0), _mount60(mount60), _mount100(mount100), _mount150(mount150), _mount280(mount280), _mount310(mount310) { }

    class spell_gen_mount_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_mount_SpellScript);

    public:
        spell_gen_mount_SpellScript(uint32 mount0, uint32 mount60, uint32 mount100, uint32 mount150, uint32 mount280, uint32 mount310) : SpellScript(),
            _mount0(mount0), _mount60(mount60), _mount100(mount100), _mount150(mount150), _mount280(mount280), _mount310(mount310) { }

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (_mount0 && !sSpellMgr->GetSpellInfo(_mount0))
                return false;
            if (_mount60 && !sSpellMgr->GetSpellInfo(_mount60))
                return false;
            if (_mount100 && !sSpellMgr->GetSpellInfo(_mount100))
                return false;
            if (_mount150 && !sSpellMgr->GetSpellInfo(_mount150))
                return false;
            if (_mount280 && !sSpellMgr->GetSpellInfo(_mount280))
                return false;
            if (_mount310 && !sSpellMgr->GetSpellInfo(_mount310))
                return false;
            return true;
        }

        void HandleMount(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            if (Player* target = GetHitPlayer())
            {
                // Prevent stacking of mounts and client crashes upon dismounting
                target->RemoveAurasByType(SPELL_AURA_MOUNTED, ObjectGuid::Empty, GetHitAura());

                // Triggered spell id dependent on riding skill and zone
                bool canFly = false;
                //uint32 map = sDB2Manager.GetVirtualMapForMapAndZone(target->GetMapId(), target->GetCurrentZoneID());
               // if (map == 530 || map == 571)
                    canFly = true;

                float x, y, z;
                target->GetPosition(x, y, z);
                uint32 areaFlag = 0; //target->GetBaseMap()->GetAreaFlag(x, y, z);
                AreaTableEntry const* area = sAreaTableStore.LookupEntry(areaFlag);
                if (!area || (canFly && (area->Flags[0] & AREA_FLAG_NO_FLY_ZONE)))
                    canFly = false;

                uint32 mount = 0;
                switch (target->GetBaseSkillValue(SKILL_RIDING))
                {
                case 0:
                    mount = _mount0;
                    break;
                case 75:
                    mount = _mount60;
                    break;
                case 150:
                    mount = _mount100;
                    break;
                case 225:
                    if (canFly)
                        mount = _mount150;
                    else
                        mount = _mount100;
                    break;
                case 300:
                    if (canFly)
                        mount = _mount280;
                    else
                        mount = _mount100;
                    break;
                case 375:
                    if (canFly)
                        mount = _mount310;
                    else
                        mount = _mount100;
                    break;
                default:
                    break;
                }

                if (mount)
                {
                    PreventHitAura();
                    target->CastSpell(target, mount, true);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_mount_SpellScript::HandleMount, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
        }

    private:
        uint32 _mount0;
        uint32 _mount60;
        uint32 _mount100;
        uint32 _mount150;
        uint32 _mount280;
        uint32 _mount310;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_mount_SpellScript(_mount0, _mount60, _mount100, _mount150, _mount280, _mount310);
    }

private:
    uint32 _mount0;
    uint32 _mount60;
    uint32 _mount100;
    uint32 _mount150;
    uint32 _mount280;
    uint32 _mount310;
};


// Gobelin Gumbo - 42760
class spell_gen_gobelin_gumbo : public SpellScriptLoader
{
public:
    spell_gen_gobelin_gumbo() : SpellScriptLoader("spell_gen_gobelin_gumbo") { }

    class spell_gen_gobelin_gumbo_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_gobelin_gumbo_AuraScript);

        uint32 burpTimer;

        bool Load() override
        {
            burpTimer = 10000;
            return true;
        }

        void OnUpdate(uint32 diff, AuraEffect* aurEff)
        {
            if (GetCaster())
            {
                if (burpTimer <= diff)
                {
                    burpTimer = 10000;
                   // if (roll_chance_i(30))
                    //    GetCaster()->CastSpell(GetCaster(), GOBELING_GUMBO_BURP, true);
                }
                else
                    burpTimer -= diff;
            }
        }

        void Register() override
        {
           // OnEffectUpdate += AuraEffectUpdateFn(spell_gen_gobelin_gumbo_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_gobelin_gumbo_AuraScript();
    }
};

enum eBrewfestSpeedSpells
{
    SPELL_BREWFEST_RAM = 43880,
    SPELL_RAM_FATIGUE = 43052,
    SPELL_SPEED_RAM_GALLOP = 42994,
    SPELL_SPEED_RAM_CANTER = 42993,
    SPELL_SPEED_RAM_TROT = 42992,
    SPELL_SPEED_RAM_NORMAL = 43310,
    SPELL_SPEED_RAM_EXHAUSED = 43332
};

class spell_brewfest_speed : public SpellScriptLoader
{
public:
    spell_brewfest_speed() : SpellScriptLoader("spell_brewfest_speed") {}

    class spell_brewfest_speed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_brewfest_speed_AuraScript);
            bool Validate(SpellInfo const * /*SpellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_RAM_FATIGUE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_BREWFEST_RAM))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_GALLOP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_CANTER))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_TROT))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_NORMAL))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_GALLOP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SPEED_RAM_EXHAUSED))
                return false;
            return true;
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (GetId() == SPELL_SPEED_RAM_EXHAUSED)
                return;
            Player* pCaster = GetCaster()->ToPlayer();
            if (!pCaster)
                return;
            int i;
            switch (GetId())
            {
            case SPELL_SPEED_RAM_GALLOP:
                for (i = 0; i < 5; i++)
                    pCaster->AddAura(SPELL_RAM_FATIGUE, pCaster);
                break;
            case SPELL_SPEED_RAM_CANTER:
                pCaster->AddAura(SPELL_RAM_FATIGUE, pCaster);
                break;
            case SPELL_SPEED_RAM_TROT:
                if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                    if (pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() <= 2)
                        pCaster->RemoveAura(SPELL_RAM_FATIGUE);
                    else
                        pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-2);
                break;
            case SPELL_SPEED_RAM_NORMAL:
                if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                    if (pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() <= 4)
                        pCaster->RemoveAura(SPELL_RAM_FATIGUE);
                    else
                        pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-4);
                break;
            }
            if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                if (pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() >= 100)
                    pCaster->CastSpell(pCaster, SPELL_SPEED_RAM_EXHAUSED, false);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* pCaster = GetCaster()->ToPlayer();
            if (!pCaster)
                return;
            if (!pCaster->HasAura(SPELL_BREWFEST_RAM))
                return;
            if (GetId() == SPELL_SPEED_RAM_EXHAUSED)
            {
                if (pCaster->HasAura(SPELL_RAM_FATIGUE))
                    pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-15);
            }
            else if (!pCaster->HasAura(SPELL_RAM_FATIGUE) || pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() < 100)
                switch (GetId())
                {
                case SPELL_SPEED_RAM_GALLOP:
                    if (!pCaster->HasAura(SPELL_SPEED_RAM_EXHAUSED))
                        pCaster->CastSpell(pCaster, SPELL_SPEED_RAM_CANTER, false);
                    break;
                case SPELL_SPEED_RAM_CANTER:
                    if (!pCaster->HasAura(SPELL_SPEED_RAM_GALLOP))
                        pCaster->CastSpell(pCaster, SPELL_SPEED_RAM_TROT, false);
                    break;
                case SPELL_SPEED_RAM_TROT:
                    if (!pCaster->HasAura(SPELL_SPEED_RAM_CANTER))
                        pCaster->CastSpell(pCaster, SPELL_SPEED_RAM_NORMAL, false);
                    break;
                }
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* pCaster = GetCaster()->ToPlayer();
            if (!pCaster)
                return;
            switch (GetId())
            {
            case SPELL_SPEED_RAM_GALLOP:
                pCaster->GetAura(SPELL_SPEED_RAM_GALLOP)->SetDuration(4000);
                break;
            case SPELL_SPEED_RAM_CANTER:
                pCaster->GetAura(SPELL_SPEED_RAM_CANTER)->SetDuration(4000);
                break;
            case SPELL_SPEED_RAM_TROT:
                pCaster->GetAura(SPELL_SPEED_RAM_TROT)->SetDuration(4000);
                break;
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_brewfest_speed_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_brewfest_speed_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_brewfest_speed_AuraScript::HandleEffectRemove, EFFECT_2, SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_brewfest_speed_AuraScript();
    }
};

class spell_gen_tricky_treat : public SpellScriptLoader
{
public:
    spell_gen_tricky_treat() : SpellScriptLoader("spell_gen_tricky_treat") {}

    class spell_gen_tricky_treat_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_tricky_treat_SpellScript);

        bool Validate(SpellInfo const* /*SpellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_TRICKY_TREAT_SPEED))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_TRICKY_TREAT_TRIGGER))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_UPSET_TUMMY))
                return false;
            return true;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_TRICKY_TREAT_TRIGGER) && caster->GetAuraCount(SPELL_TRICKY_TREAT_SPEED) > 3 && roll_chance_i(33))
                    caster->CastSpell(caster, SPELL_UPSET_TUMMY, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_tricky_treat_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_tricky_treat_SpellScript();
    }
};

class spell_gen_leviroth_self_impale : public SpellScriptLoader
{
public:
    spell_gen_leviroth_self_impale() : SpellScriptLoader("spell_gen_leviroth_self_impale") { }

    class spell_gen_leviroth_self_impale_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_leviroth_self_impale_SpellScript);

        void HandleDamageCalc(SpellEffIndex /*effIndex*/)
        {
            uint32 bp = GetCaster()->CountPctFromCurHealth(93);
            SetHitDamage(bp);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_leviroth_self_impale_SpellScript::HandleDamageCalc, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_leviroth_self_impale_SpellScript();
    }
};

// spell 147655 - ?????? ??????? ???????
class spell_gulp_frog_toxin : public SpellScriptLoader
{
public:
    spell_gulp_frog_toxin() : SpellScriptLoader("spell_gulp_frog_toxin") { }

    class spell_gulp_frog_toxinAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gulp_frog_toxinAuraScript);

        uint32 lastStack;

        bool Load() override
        {
            lastStack = 0;
            return true;
        }

        void OnStackChange(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                if (lastStack == 10)
                    target->CastSpell(target, 147656, false);

            lastStack = GetStackAmount();
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_gulp_frog_toxinAuraScript::OnStackChange, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    // function which creates AuraScript
    AuraScript* GetAuraScript() const override
    {
        return new spell_gulp_frog_toxinAuraScript();
    }
};

// spell 147280 - Time-Lost Wisdom
class spell_time_lost_wisdom : public SpellScriptLoader
{
public:
    spell_time_lost_wisdom() : SpellScriptLoader("spell_time_lost_wisdom") { }

    class spell_time_lost_wisdom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_time_lost_wisdom_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                switch (urand(0, 4))
                {
                case 0: target->CastSpell(target, 147281, true); break;
                case 1: target->CastSpell(target, 147282, true); break;
                case 2: target->CastSpell(target, 147283, true); break;
                case 3: target->CastSpell(target, 147284, true); break;
                case 4: target->CastSpell(target, 147285, true); break;
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_time_lost_wisdom_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_time_lost_wisdom_SpellScript();
    }
};

// spell  98507 - Brutal Assault
class spell_gen_brutal_assault : public SpellScriptLoader
{
public:
    spell_gen_brutal_assault() : SpellScriptLoader("spell_gen_brutal_assault") { }

    class spell_gen_brutal_assaultAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_brutal_assaultAuraScript);

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
           // int32 amount = aurEff->GetOldBaseAmount() + aurEff->GetAmount();
          //  if (AuraEffect* aurEffSelf = GetEffect(EFFECT_0))
               // aurEffSelf->SetAmount(amount);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_gen_brutal_assaultAuraScript::OnApply, EFFECT_0, SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    // function which creates AuraScript
    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_brutal_assaultAuraScript();
    }
};

// spell  110310 - Dampening for arena
class spell_gen_dampening : public SpellScriptLoader
{
public:
    spell_gen_dampening() : SpellScriptLoader("spell_gen_dampening") { }

    class spell_gen_dampeningAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_dampeningAuraScript);

        int32 amount = 0;

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
               // if (player->GetBattleground() && player->GetBattleground()->GetJoinType() == MS::Battlegrounds::JoinType::Arena2v2)
                {
                    amount = 20;
                    const_cast<AuraEffect*>(aurEff)->ChangeAmount(amount);
                }
            }
        }

        void HandlePeriodicTick(AuraEffect const* aurEff)
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            if (AuraEffect* aurEff0 = aurEff->GetBase()->GetEffect(EFFECT_0))
            {
                amount = aurEff0->GetAmount() + 1;
                if (amount >= 100)
                    return;

                aurEff0->ChangeAmount(amount);

                float bp0 = -amount;
              //  player->CastCustomSpell(player, 74410, &bp0, &bp0, &bp0, true);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_gen_dampeningAuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_dampeningAuraScript::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_dampeningAuraScript();
    }
};

// spell  121176 - Orb of Power
class spell_gen_orb_of_power : public SpellScriptLoader
{
public:
    spell_gen_orb_of_power() : SpellScriptLoader("spell_gen_orb_of_power") { }

    class spell_gen_orb_of_powerAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_orb_of_powerAuraScript);

        uint8 ticks = 0;

        void HandleEffectPeriodicUpdate(AuraEffect* aurEff)
        {
            if (AuraEffect* aurEff0 = aurEff->GetBase()->GetEffect(EFFECT_0))
                aurEff0->SetAmount(aurEff0->GetAmount() + aurEff0->GetBaseAmount());
            if (AuraEffect* aurEff1 = aurEff->GetBase()->GetEffect(EFFECT_1))
                aurEff1->SetAmount(aurEff1->GetAmount() + aurEff1->GetBaseAmount());
            if (AuraEffect* aurEff2 = aurEff->GetBase()->GetEffect(EFFECT_2))
            {
                int32 amount = aurEff2->GetAmount() + aurEff2->GetBaseAmount();

                if (AuraApplication * aurApp = GetAura()->GetApplicationOfTarget(GetCasterGUID()))
                {
                    aurEff2->HandleEffect(aurApp, AURA_EFFECT_HANDLE_REAL, false);
                    aurEff2->SetAmount(amount);
                    aurEff2->HandleEffect(aurApp, AURA_EFFECT_HANDLE_REAL, true);
                }
            }

            if (Player* caster = GetCaster()->ToPlayer())
            {
                if (caster->GetPositionX() > 1749 && caster->GetPositionX() < 1816 && caster->GetPositionY() > 1287 && caster->GetPositionY() < 1379)
                {
                    ticks++;

                  //  if (ticks == 6)
                        //caster->UpdateAchievementCriteria(CRITERIA_TYPE_BE_SPELL_TARGET, 128768); // Credit Achieve - Powerball
                }
                else
                    ticks = 0;
            }
        }

        void Register() override
        {
            OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_gen_orb_of_powerAuraScript::HandleEffectPeriodicUpdate, EFFECT_3, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    // function which creates AuraScript
    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_orb_of_powerAuraScript();
    }
};

enum
{
    SPELL_MAGE_TEMPORAL_DISPLACEMENT = 80354,
    HUNTER_SPELL_INSANITY = 95809,
    HUNTER_SPELL_FATIGUED = 160455,
    SPELL_SHAMAN_SATED = 57724,
    SPELL_SHAMAN_EXHAUSTED = 57723,

};
// Drums of Rage - 146555, 178207
class spell_gen_drums_of_rage : public SpellScriptLoader
{
public:
    spell_gen_drums_of_rage() : SpellScriptLoader("spell_gen_drums_of_rage") { }

    class spell_gen_drums_of_rage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_drums_of_rage_SpellScript);

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            std::list<WorldObject*> removeList;

            for (auto itr : targets)
            {
                if (Unit* unit = itr->ToUnit())
                {
                    if (unit->HasAura(HUNTER_SPELL_INSANITY) || unit->HasAura(HUNTER_SPELL_FATIGUED) || unit->HasAura(SPELL_SHAMAN_EXHAUSTED) || unit->HasAura(SPELL_SHAMAN_SATED) || unit->HasAura(SPELL_MAGE_TEMPORAL_DISPLACEMENT))
                    {
                        removeList.push_back(itr);
                        continue;
                    }
                }
            }

            if (!removeList.empty())
            {
                for (auto itr : removeList)
                    targets.remove(itr);
            }
        }

        void ApplyDebuff()
        {
            if (Player* caster = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (caster->GetTeam() == HORDE)
                        target->CastSpell(target, SPELL_SHAMAN_SATED, true);
                    else
                        target->CastSpell(target, SPELL_SHAMAN_EXHAUSTED, true);
                }
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_drums_of_rage_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_drums_of_rage_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            AfterHit += SpellHitFn(spell_gen_drums_of_rage_SpellScript::ApplyDebuff);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_drums_of_rage_SpellScript();
    }
};

// Guild Battle Standard
class spell_gen_battle_guild_standart : public SpellScriptLoader
{
public:
    spell_gen_battle_guild_standart() : SpellScriptLoader("spell_gen_battle_guild_standart") { }

    class spell_gen_battle_guild_standart_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_battle_guild_standart_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            //Updated by CyberBrest
            if (Creature* pStandart = GetCaster()->ToCreature())
            {
                Unit* owner = pStandart->GetOwner();
                if (!owner)
                {
                    targets.clear();
                    return;
                }

                Player* player = owner->ToPlayer();
                if (!player)
                {
                    targets.clear();
                    return;
                }

                //ObjectGuid guildID = player->GetGuildGUID();
              //  if (!guildID)
                {
                    targets.clear();
                    return;
                }

               // targets.remove_if(GuildCheck(pStandart->GetGUID(), guildID));
            }
            else
                targets.clear();
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_battle_guild_standart_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_battle_guild_standart_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_battle_guild_standart_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ALLY);
        }

    private:
        class GuildCheck
        {
        public:
            GuildCheck(ObjectGuid casterGUID, ObjectGuid guildId) : _casterGUID(casterGUID), _guildId(guildId) {}

            bool operator()(WorldObject* unit)
            {
                if (!_guildId)
                    return true;

                if (unit->GetTypeId() != TYPEID_PLAYER)
                    return true;

              //  if (unit->ToPlayer()->GetGuildGUID() != _guildId)
                 //   return true;

                if (Aura* const aur = unit->ToPlayer()->GetAura(90216))
                    if (aur->GetCasterGUID() != _casterGUID)
                        return true;

                if (Aura* const aur = unit->ToPlayer()->GetAura(90708))
                    if (aur->GetCasterGUID() != _casterGUID)
                        return true;

                return false;
            }

        private:
            ObjectGuid _casterGUID;
            ObjectGuid _guildId;
        };
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_battle_guild_standart_SpellScript();
    }

    class spell_gen_battle_guild_standart_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_battle_guild_standart_AuraScript);


        void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
        {
            if (Unit* pStandart = GetCaster())
            {
                float bp0 = 0.f;
                float bp1 = 0.f;

                switch (pStandart->GetEntry())
                {
                case 48636: // 5%, horde
                    bp0 = 5;
                    break;
                case 48637: // 10%, horde
                    bp0 = 10;
                    break;
                case 48638: // 15%, horde
                    bp0 = 15;
                    break;
                case 48115: // 5%, alliance
                    bp1 = 5;
                    break;
                case 48633: // 10%, alliance
                    bp1 = 10;
                    break;
                case 48634: // 15%, alliance
                    bp1 = 15;
                    break;
                }
             //   if (bp0)
                //    pStandart->CastCustomSpell(pStandart, 90708, &bp0, &bp0, &bp0, true);

               // if (bp1)
                  //  pStandart->CastCustomSpell(pStandart, 90216, &bp1, &bp1, &bp1, true);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_battle_guild_standart_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_battle_guild_standart_AuraScript();
    }
};

class spell_gen_cooking_way : public SpellScriptLoader
{
public:
    spell_gen_cooking_way() : SpellScriptLoader("spell_gen_cooking_way") { }

    class spell_gen_cooking_way_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_cooking_way_SpellScript);

        void HandleAfterCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

          //  uint32 skillid = GetSpellInfo()->Effects[1]->MiscValue;
          //  caster->SetSkill(skillid, GetSpellInfo()->Effects[1]->CalcValue(), 525, 600);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_gen_cooking_way_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_cooking_way_SpellScript();
    }
};


//43681
class spell_gen_bg_inactive : public SpellScriptLoader
{
public:
    spell_gen_bg_inactive() : SpellScriptLoader("spell_gen_bg_inactive") {}

    class spell_gen_bg_inactive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_bg_inactive_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                if (player->GetMap()->IsBattleground())
                    player->LeaveBattleground();
            }
        }

        void Register() override
        {
           // AfterEffectRemove += AuraEffectRemoveFn(spell_gen_bg_inactive_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SPELL_AURA_MOD_HONOR_POINTS_GAIN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_bg_inactive_AuraScript();
    }
};

class spell_gen_bounce_achievement : public SpellScriptLoader
{
public:
    spell_gen_bounce_achievement() : SpellScriptLoader("spell_gen_bounce_achievement") { }

    class spell_gen_bounce_achievement_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_bounce_achievement_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
          //  if (Player* pCaster = GetCaster()->ToPlayer())
                //pCaster->GetAchievementMgr()->ResetAchievementCriteria(CRITERIA_TYPE_BE_SPELL_TARGET, 95529);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_gen_bounce_achievement_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_bounce_achievement_AuraScript();
    }
};

//193378, 193379, 193380, 193381, 193382, 193383, 193384, 205139, 205695, 205712, 205713, 205714
class spell_gen_herbalism_trap : public SpellScriptLoader
{
public:
    spell_gen_herbalism_trap() : SpellScriptLoader("spell_gen_herbalism_trap") { }

    class spell_gen_herbalism_trap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_herbalism_trap_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

          //  switch (RoundingFloatValue(GetSpellInfo()->Effects[effIndex]->BasePoints))
            {
          //  case 201702: //Legion Herbalist Learning
            {
                //if (!caster->HasSpell(195114)) //Check Legion Herbalist skill
                 //   caster->CastSpell(caster, GetSpellInfo()->Effects[effIndex]->BasePoints, true);
               // break;
            }
          //  default:
         //       break;
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_herbalism_trap_SpellScript::HandleDummy, EFFECT_ALL, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_herbalism_trap_SpellScript();
    }
};

//185496, 185528, 185529, 185768, 185769, 185770, 204918, 204981, 205009, 205060, 205802, 205804, 205805
class spell_gen_mining_trap : public SpellScriptLoader
{
public:
    spell_gen_mining_trap() : SpellScriptLoader("spell_gen_mining_trap") { }

    class spell_gen_mining_trap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_mining_trap_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

          //  switch (RoundingFloatValue(GetSpellInfo()->Effects[effIndex]->BasePoints))
            {
            //case 201706: //Legion Mining Learning
            {
                //if (!caster->HasSpell(195122)) //Check Legion Mining skill
                   // caster->CastSpell(caster, GetSpellInfo()->Effects[effIndex]->BasePoints, true);
             //   break;
            }
           // default:
              //  break;
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_mining_trap_SpellScript::HandleDummy, EFFECT_ALL, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_mining_trap_SpellScript();
    }
};

//8613
class spell_gen_learn_legion_skinning : public SpellScriptLoader
{
public:
    spell_gen_learn_legion_skinning() : SpellScriptLoader("spell_gen_learn_legion_skinning") { }

    class spell_gen_learn_legion_skinning_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_learn_legion_skinning_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

            if (!caster->HasSpell(195125)) //Check Legion Skinning skill
            {
                if (Creature* creature = GetHitUnit()->ToCreature())
                    if (creature->GetCreatureTemplate()->RequiredExpansion == EXPANSION_LEGION)
                        caster->CastSpell(caster, 201707, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_learn_legion_skinning_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SKINNING);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_learn_legion_skinning_SpellScript();
    }
};

//! 192190
//! triggered from spells: 210908, 192190 at quest complete.
//! for example SELECT * FROM `quest_template` WHERE `RewardDisplaySpell` = 210908
class spell_gen_create_artefact : public SpellScriptLoader
{
public:
    spell_gen_create_artefact() : SpellScriptLoader("spell_gen_create_artefact") { }

    class spell_gen_create_artefact_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_create_artefact_SpellScript);

        enum sp
        {
            __HORDE = 192191,
            __ALLIANCE = 185506,
        };
        void HandleDummy(SpellEffIndex effIndex)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

            caster->CastSpell(caster, caster->GetTeam() == HORDE ? __HORDE : __ALLIANCE, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_create_artefact_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_create_artefact_SpellScript();
    }
};

// SPELL_PRINCIPLES_OF_WAR - 197912
class spell_principles_of_war : public SpellScriptLoader
{
public:
    spell_principles_of_war() : SpellScriptLoader("spell_principles_of_war") {}

    class spell_principles_of_war_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_principles_of_war_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, float& amount, bool& canBeRecalculated)
        {
            auto caster = GetCaster();
            if (!caster)
                return;

            auto player = caster->ToPlayer();
            if (!player)
                return;

            canBeRecalculated = true;

           // float itemLevel = player->GetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL);
           // if (itemLevel > 1000.f)
            //    itemLevel = 1000.f;

        //    if (itemLevel < 800.f)
            {
                amount = 0;
                return;
            }

         //   amount = RoundingFloatValue((itemLevel - 800.f) * 0.1f);
        }

        void Register() override
        {
          //  DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_principles_of_war_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_principles_of_war_AuraScript();
    }
};

// Blessing of the Light - 220058, 220887, 220792, 220110
class spell_gen_absorb_if_health : public SpellScriptLoader
{
public:
    spell_gen_absorb_if_health() : SpellScriptLoader("spell_gen_absorb_if_health") { }

    class spell_gen_absorb_if_health_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_absorb_if_health_AuraScript);

        void CalculateAmount(AuraEffect const* /*AuraEffect**/, float& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, float & absorbAmount)
        {
            Unit* target = GetCaster();

            if (dmgInfo.GetDamage() < target->GetHealth())
                return;
            int32 health = GetSpellInfo()->Id == 220110 ? 220113 : 220073;
            int32 knock = GetSpellInfo()->Id == 220110 ? 220111 : 220103;

            target->CastSpell(target, health, true);
            target->CastSpell(target, knock, true);
            absorbAmount = dmgInfo.GetDamage();
            GetAura()->SetDuration(250);
        }

        void Register() override
        {
            //DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_absorb_if_health_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
           // OnEffectAbsorb += AuraEffectAbsorbFn(spell_gen_absorb_if_health_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_absorb_if_health_AuraScript();
    }
};

//51640
class spell_gen_taunt_flag_targeting : public SpellScriptLoader
{
public:
    spell_gen_taunt_flag_targeting() : SpellScriptLoader("spell_gen_taunt_flag_targeting") { }

    class spell_gen_taunt_flag_targeting_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_taunt_flag_targeting_SpellScript);

        SpellCastResult CheckIfCorpseNear()
        {
            Unit* caster = GetCaster();
            float max_range = GetSpellInfo()->GetMaxRange(false);
            WorldObject* result = nullptr;
            // search for nearby enemy corpse in range
            Trinity::AnyDeadUnitSpellTargetInRangeCheck check(caster, max_range, GetSpellInfo(), TARGET_CHECK_ENEMY);
            Trinity::WorldObjectSearcher<Trinity::AnyDeadUnitSpellTargetInRangeCheck> searcher(caster, result, check);
            //caster->GetMap()->VisitFirstFound(caster->m_positionX, caster->m_positionY, max_range, searcher);
            if (!result || !result->ToPlayer())
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
            return SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), 51657, true);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_gen_taunt_flag_targeting_SpellScript::CheckIfCorpseNear);
            OnEffectHitTarget += SpellEffectFn(spell_gen_taunt_flag_targeting_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_taunt_flag_targeting_SpellScript();
    }
};

//52410 BG: Strand of the Ancients
class spell_sa_place_seaforium_charge : public SpellScriptLoader
{
public:
    spell_sa_place_seaforium_charge() : SpellScriptLoader("spell_sa_place_seaforium_charge") {}

    class spell_sa_place_seaforium_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sa_place_seaforium_charge_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->GetTeam() == HORDE)
                    player->CastSpell(player, 226090, true);
                else
                    player->CastSpell(player, 226094, true);
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_sa_place_seaforium_charge_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_sa_place_seaforium_charge_SpellScript();
    }
};

//Hidden Appearance Unlock
class spell_hidden_appearance_unlock : public SpellScriptLoader
{
public:
    spell_hidden_appearance_unlock() : SpellScriptLoader("spell_hidden_appearance_unlock") {}

    class spell_hidden_appearance_unlock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hidden_appearance_unlock_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
              //  Quest const* quest = sQuestDataStore->GetQuestTemplate(34905);
               // if (!quest)
                    return;

                uint16 logSlot = player->FindQuestSlot(34905);
                if (logSlot < MAX_QUEST_LOG_SIZE)
                    player->AreaExploredOrEventHappens(34905);
               // else if (player->CanTakeQuest(quest, false))    // never rewarded before
                    player->CompleteQuest(34905);             // quest not in log - for internal use
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hidden_appearance_unlock_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hidden_appearance_unlock_SpellScript();
    }
};

//! 213170 spell
class spell_class_hall_panel : public SpellScriptLoader
{
public:
    spell_class_hall_panel() : SpellScriptLoader("spell_class_hall_panel") { }

    class spell_class_hall_panel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_class_hall_panel_AuraScript);

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (target->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = target->ToPlayer();
            if (!player)
                return;

          //  Garrison* garrison = player->GetGarrisonPtr();
//            if ((!garrison || !garrison->HasGarrison(GARRISON_TYPE_CLASS_ORDER)) && player->getLevel() >= 100)
            {
                if (player->GetTeam() == HORDE)
                {
                    // if (GetQuestRewardStatus(40518))
                    player->CastSpell(player, 192191, true);
                }
                else // if (GetQuestRewardStatus(42740))
                    player->CastSpell(player, 185506, true);
            }

            if (target->GetTypeId() == TYPEID_UNIT)
                target->ToCreature()->SetReactState(REACT_PASSIVE);
        }

        void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->RemoveUnitFlag(UnitFlags(UNIT_FLAG2_FEIGN_DEATH));
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_class_hall_panel_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_class_hall_panel_AuraScript();
    }
};

enum TournamentPennantSpells
{
    SPELL_PENNANT_STORMWIND_ASPIRANT = 62595,
    SPELL_PENNANT_STORMWIND_VALIANT = 62596,
    SPELL_PENNANT_STORMWIND_CHAMPION = 62594,
    SPELL_PENNANT_GNOMEREGAN_ASPIRANT = 63394,
    SPELL_PENNANT_GNOMEREGAN_VALIANT = 63395,
    SPELL_PENNANT_GNOMEREGAN_CHAMPION = 63396,
    SPELL_PENNANT_SEN_JIN_ASPIRANT = 63397,
    SPELL_PENNANT_SEN_JIN_VALIANT = 63398,
    SPELL_PENNANT_SEN_JIN_CHAMPION = 63399,
    SPELL_PENNANT_SILVERMOON_ASPIRANT = 63401,
    SPELL_PENNANT_SILVERMOON_VALIANT = 63402,
    SPELL_PENNANT_SILVERMOON_CHAMPION = 63403,
    SPELL_PENNANT_DARNASSUS_ASPIRANT = 63404,
    SPELL_PENNANT_DARNASSUS_VALIANT = 63405,
    SPELL_PENNANT_DARNASSUS_CHAMPION = 63406,
    SPELL_PENNANT_EXODAR_ASPIRANT = 63421,
    SPELL_PENNANT_EXODAR_VALIANT = 63422,
    SPELL_PENNANT_EXODAR_CHAMPION = 63423,
    SPELL_PENNANT_IRONFORGE_ASPIRANT = 63425,
    SPELL_PENNANT_IRONFORGE_VALIANT = 63426,
    SPELL_PENNANT_IRONFORGE_CHAMPION = 63427,
    SPELL_PENNANT_UNDERCITY_ASPIRANT = 63428,
    SPELL_PENNANT_UNDERCITY_VALIANT = 63429,
    SPELL_PENNANT_UNDERCITY_CHAMPION = 63430,
    SPELL_PENNANT_ORGRIMMAR_ASPIRANT = 63431,
    SPELL_PENNANT_ORGRIMMAR_VALIANT = 63432,
    SPELL_PENNANT_ORGRIMMAR_CHAMPION = 63433,
    SPELL_PENNANT_THUNDER_BLUFF_ASPIRANT = 63434,
    SPELL_PENNANT_THUNDER_BLUFF_VALIANT = 63435,
    SPELL_PENNANT_THUNDER_BLUFF_CHAMPION = 63436,
    SPELL_PENNANT_ARGENT_CRUSADE_ASPIRANT = 63606,
    SPELL_PENNANT_ARGENT_CRUSADE_VALIANT = 63500,
    SPELL_PENNANT_ARGENT_CRUSADE_CHAMPION = 63501,
    SPELL_PENNANT_EBON_BLADE_ASPIRANT = 63607,
    SPELL_PENNANT_EBON_BLADE_VALIANT = 63608,
    SPELL_PENNANT_EBON_BLADE_CHAMPION = 63609
};

enum TournamentMounts
{
    NPC_STORMWIND_STEED = 33217,
    NPC_IRONFORGE_RAM = 33316,
    NPC_GNOMEREGAN_MECHANOSTRIDER = 33317,
    NPC_EXODAR_ELEKK = 33318,
    NPC_DARNASSIAN_NIGHTSABER = 33319,
    NPC_ORGRIMMAR_WOLF = 33320,
    NPC_DARK_SPEAR_RAPTOR = 33321,
    NPC_THUNDER_BLUFF_KODO = 33322,
    NPC_SILVERMOON_HAWKSTRIDER = 33323,
    NPC_FORSAKEN_WARHORSE = 33324,
    NPC_ARGENT_WARHORSE = 33782,
    NPC_ARGENT_STEED_ASPIRANT = 33845,
    NPC_ARGENT_HAWKSTRIDER_ASPIRANT = 33844
};

enum TournamentQuestsAchievements
{
    ACHIEVEMENT_CHAMPION_STORMWIND = 2781,
    ACHIEVEMENT_CHAMPION_DARNASSUS = 2777,
    ACHIEVEMENT_CHAMPION_IRONFORGE = 2780,
    ACHIEVEMENT_CHAMPION_GNOMEREGAN = 2779,
    ACHIEVEMENT_CHAMPION_THE_EXODAR = 2778,
    ACHIEVEMENT_CHAMPION_ORGRIMMAR = 2783,
    ACHIEVEMENT_CHAMPION_SEN_JIN = 2784,
    ACHIEVEMENT_CHAMPION_THUNDER_BLUFF = 2786,
    ACHIEVEMENT_CHAMPION_UNDERCITY = 2787,
    ACHIEVEMENT_CHAMPION_SILVERMOON = 2785,
    ACHIEVEMENT_ARGENT_VALOR = 2758,
    ACHIEVEMENT_CHAMPION_ALLIANCE = 2782,
    ACHIEVEMENT_CHAMPION_HORDE = 2788,

    QUEST_VALIANT_OF_STORMWIND = 13593,
    QUEST_A_VALIANT_OF_STORMWIND = 13684,
    QUEST_VALIANT_OF_DARNASSUS = 13706,
    QUEST_A_VALIANT_OF_DARNASSUS = 13689,
    QUEST_VALIANT_OF_IRONFORGE = 13703,
    QUEST_A_VALIANT_OF_IRONFORGE = 13685,
    QUEST_VALIANT_OF_GNOMEREGAN = 13704,
    QUEST_A_VALIANT_OF_GNOMEREGAN = 13688,
    QUEST_VALIANT_OF_THE_EXODAR = 13705,
    QUEST_A_VALIANT_OF_THE_EXODAR = 13690,
    QUEST_VALIANT_OF_ORGRIMMAR = 13707,
    QUEST_A_VALIANT_OF_ORGRIMMAR = 13691,
    QUEST_VALIANT_OF_SEN_JIN = 13708,
    QUEST_A_VALIANT_OF_SEN_JIN = 13693,
    QUEST_VALIANT_OF_THUNDER_BLUFF = 13709,
    QUEST_A_VALIANT_OF_THUNDER_BLUFF = 13694,
    QUEST_VALIANT_OF_UNDERCITY = 13710,
    QUEST_A_VALIANT_OF_UNDERCITY = 13695,
    QUEST_VALIANT_OF_SILVERMOON = 13711,
    QUEST_A_VALIANT_OF_SILVERMOON = 13696
};

class spell_gen_on_tournament_mount : public SpellScriptLoader
{
public:
    spell_gen_on_tournament_mount() : SpellScriptLoader("spell_gen_on_tournament_mount") { }

    class spell_gen_on_tournament_mount_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_on_tournament_mount_AuraScript);

    public:
        spell_gen_on_tournament_mount_AuraScript()
        {
            _pennantSpellId = 0;
        }

    private:
        uint32 _pennantSpellId;

        bool Load() override
        {
            return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* vehicle = caster->GetVehicleBase())
                {
                    _pennantSpellId = GetPennatSpellId(caster->ToPlayer(), vehicle);
                    caster->CastSpell(caster, _pennantSpellId, true);
                }
            }
        }

        void HandleRemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->RemoveAurasDueToSpell(_pennantSpellId);
        }

        uint32 GetPennatSpellId(Player* player, Unit* mount)
        {
            switch (mount->GetEntry())
            {
            case NPC_ARGENT_STEED_ASPIRANT:
            case NPC_STORMWIND_STEED:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_STORMWIND))
                    return SPELL_PENNANT_STORMWIND_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_STORMWIND) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_STORMWIND))
                    return SPELL_PENNANT_STORMWIND_VALIANT;
                else
                    return SPELL_PENNANT_STORMWIND_ASPIRANT;
            }
            case NPC_GNOMEREGAN_MECHANOSTRIDER:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_GNOMEREGAN))
                    return SPELL_PENNANT_GNOMEREGAN_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_GNOMEREGAN) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_GNOMEREGAN))
                    return SPELL_PENNANT_GNOMEREGAN_VALIANT;
                else
                    return SPELL_PENNANT_GNOMEREGAN_ASPIRANT;
            }
            case NPC_DARK_SPEAR_RAPTOR:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_SEN_JIN))
                    return SPELL_PENNANT_SEN_JIN_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_SEN_JIN) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_SEN_JIN))
                    return SPELL_PENNANT_SEN_JIN_VALIANT;
                else
                    return SPELL_PENNANT_SEN_JIN_ASPIRANT;
            }
            case NPC_ARGENT_HAWKSTRIDER_ASPIRANT:
            case NPC_SILVERMOON_HAWKSTRIDER:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_SILVERMOON))
                    return SPELL_PENNANT_SILVERMOON_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_SILVERMOON) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_SILVERMOON))
                    return SPELL_PENNANT_SILVERMOON_VALIANT;
                else
                    return SPELL_PENNANT_SILVERMOON_ASPIRANT;
            }
            case NPC_DARNASSIAN_NIGHTSABER:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_DARNASSUS))
                    return SPELL_PENNANT_DARNASSUS_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_DARNASSUS) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_DARNASSUS))
                    return SPELL_PENNANT_DARNASSUS_VALIANT;
                else
                    return SPELL_PENNANT_DARNASSUS_ASPIRANT;
            }
            case NPC_EXODAR_ELEKK:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_THE_EXODAR))
                    return SPELL_PENNANT_EXODAR_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_THE_EXODAR) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_THE_EXODAR))
                    return SPELL_PENNANT_EXODAR_VALIANT;
                else
                    return SPELL_PENNANT_EXODAR_ASPIRANT;
            }
            case NPC_IRONFORGE_RAM:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_IRONFORGE))
                    return SPELL_PENNANT_IRONFORGE_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_IRONFORGE) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_IRONFORGE))
                    return SPELL_PENNANT_IRONFORGE_VALIANT;
                else
                    return SPELL_PENNANT_IRONFORGE_ASPIRANT;
            }
            case NPC_FORSAKEN_WARHORSE:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_UNDERCITY))
                    return SPELL_PENNANT_UNDERCITY_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_UNDERCITY) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_UNDERCITY))
                    return SPELL_PENNANT_UNDERCITY_VALIANT;
                else
                    return SPELL_PENNANT_UNDERCITY_ASPIRANT;
            }
            case NPC_ORGRIMMAR_WOLF:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_ORGRIMMAR))
                    return SPELL_PENNANT_ORGRIMMAR_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_ORGRIMMAR) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_ORGRIMMAR))
                    return SPELL_PENNANT_ORGRIMMAR_VALIANT;
                else
                    return SPELL_PENNANT_ORGRIMMAR_ASPIRANT;
            }
            case NPC_THUNDER_BLUFF_KODO:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_THUNDER_BLUFF))
                    return SPELL_PENNANT_THUNDER_BLUFF_CHAMPION;
                else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_THUNDER_BLUFF) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_THUNDER_BLUFF))
                    return SPELL_PENNANT_THUNDER_BLUFF_VALIANT;
                else
                    return SPELL_PENNANT_THUNDER_BLUFF_ASPIRANT;
            }
            case NPC_ARGENT_WARHORSE:
            {
                if (player->HasAchieved(ACHIEVEMENT_CHAMPION_ALLIANCE) || player->HasAchieved(ACHIEVEMENT_CHAMPION_HORDE))
                    return player->getClass() == CLASS_DEATH_KNIGHT ? SPELL_PENNANT_EBON_BLADE_CHAMPION : SPELL_PENNANT_ARGENT_CRUSADE_CHAMPION;
                else if (player->HasAchieved(ACHIEVEMENT_ARGENT_VALOR))
                    return player->getClass() == CLASS_DEATH_KNIGHT ? SPELL_PENNANT_EBON_BLADE_VALIANT : SPELL_PENNANT_ARGENT_CRUSADE_VALIANT;
                else
                    return player->getClass() == CLASS_DEATH_KNIGHT ? SPELL_PENNANT_EBON_BLADE_ASPIRANT : SPELL_PENNANT_ARGENT_CRUSADE_ASPIRANT;
            }
            default:
                return 0;
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_gen_on_tournament_mount_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_gen_on_tournament_mount_AuraScript::HandleRemoveEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_on_tournament_mount_AuraScript();
    }
};

//312916
class spell_class_mecagnomo_emergency : public SpellScriptLoader
{
public:
    spell_class_mecagnomo_emergency() : SpellScriptLoader("spell_class_mecagnomo_emergency") { }

    class spell_class_mecagnomo_emergency_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_class_mecagnomo_emergency_AuraScript);


        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetCaster();

            uint32 triggerOnHealth = caster->CountPctFromMaxHealth(aurEff->GetAmount());
            uint32 currentHealth = caster->GetHealth();
            // Just falling below threshold
            if (currentHealth > triggerOnHealth && (currentHealth - caster->GetMaxHealth() * 25.0f / 100.0f) <= triggerOnHealth){
                caster->CastSpell(caster, 313010);
            }
            
            
        }

      
        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            caster->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
            return true;
           
        }
        
        
        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_class_mecagnomo_emergency_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_class_mecagnomo_emergency_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_class_mecagnomo_emergency_AuraScript();
    }
};

// 313015
class spell_class_mecagnomo_emergency2 : public SpellScriptLoader
{
public:
    spell_class_mecagnomo_emergency2() : SpellScriptLoader("spell_class_mecagnomo_emergency2") { }

    class spell_class_mecagnomo_emergency2_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_class_mecagnomo_emergency2_AuraScript);

           
        void HandleHit(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster()->HasAura(313010))
                PreventDefaultAction();
        }

       void Register() override
        {   
            OnEffectApply += AuraEffectApplyFn(spell_class_mecagnomo_emergency2_AuraScript::HandleHit, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_class_mecagnomo_emergency2_AuraScript();
    }
};
// 313010

class spell_class_mecagnomo_emergency3 : public SpellScriptLoader
{
public:
    spell_class_mecagnomo_emergency3() : SpellScriptLoader("spell_class_mecagnomo_emergency3") { }

    class spell_class_mecagnomo_emergency3_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_class_mecagnomo_emergency3_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            if (!GetCaster()->HasAura(313015))
                PreventHitDefaultEffect(effIndex);
        }

        void HandleHeal(SpellEffIndex effIndex)
        {

            Unit * caster = GetCaster();
            uint32 heal = caster->GetMaxHealth() * 25.0f / 100.0f;
            //caster->SpellHealingBonusDone(caster, GetSpellInfo(), caster->CountPctFromMaxHealth(GetSpellInfo()->GetEffect(effIndex)->BasePoints), HEAL, GetEffectInfo());
            heal = caster->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, HEAL, GetEffectInfo());
            SetHitHeal(heal);
            caster->CastSpell(caster, 313015, true);

            PreventHitDefaultEffect(effIndex);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_class_mecagnomo_emergency3_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL_PCT);
            OnEffectLaunch += SpellEffectFn(spell_class_mecagnomo_emergency3_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_class_mecagnomo_emergency3_SpellScript();
    }
};


//206150/challengers_might
class spell_challengers_might : public SpellScriptLoader
{
public:
    spell_challengers_might() : SpellScriptLoader("spell_challengers_might") { }

    enum eSpell
    {
        SPELL_ENRAGE = 228318,
    };

    class spell_challengers_might_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_challengers_might_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& p_EventInfo)
        {
            PreventDefaultAction();
            ;
            if (!GetUnitOwner())
                return;
            //??
            if (AuraEffect* aurEff1 = aurEff->GetBase()->GetEffect(EFFECT_2))
            {
                if (aurEff1->GetBaseAmount())
                {
                    if (!GetUnitOwner()->ToCreature()->IsDungeonBoss())
                    {
                        if (GetUnitOwner()->GetHealthPct() <= 30.f)
                            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_ENRAGE, true);
                        else
                            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_ENRAGE);
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_challengers_might_AuraScript::OnProc, EFFECT_2, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_challengers_might_AuraScript();
    }
};

struct npc_volcanic_plume_105877 : public ScriptedAI
{
    npc_volcanic_plume_105877(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        SetCombatMovement(false);
        me->SetReactState(REACT_PASSIVE);
        me->AddUnitState(UNIT_STATE_CANNOT_TURN);
        me->SetDisableGravity(true);
        me->SetFaction(16);
        me->SetDisplayId(16925);
        me->CastSpell(me, 209862, false);
        me->DespawnOrUnsummon(3000);
    }
};

class challange_player_instance_handler : public PlayerScript {
public:
    challange_player_instance_handler() : PlayerScript("challange_player_instance_handler")
    {  }

    void OnStartChallengeMode(Player* player, uint8 level, uint8 affix1, uint8 affix2, uint8 affix3)
    {
        isstarted = true;
        _affix1 = affix1;
        _affix2 = affix2;
        _affix3 = affix3;
    }

    bool CheckChallengeModeStarted(Player* player)
    {
        if (InstanceScript* instance = player->GetInstanceScript())
            isstarted = instance->IsChallengeModeStarted();
        else
            isstarted = false;
        return isstarted;
    }

    void OnTakeDamage(Player* player, uint32 /*damage*/, SpellSchoolMask schoolMask)
    {
        if (isstarted)
        {
            isstarted = CheckChallengeModeStarted(player);

            if (_affix2 == 4 && schoolMask == SPELL_SCHOOL_MASK_NORMAL)//necrotic
            {
                player->AddAura(209858);
            }
            if (_affix2 == 12)//grievous
            {
                if (player->GetHealthPct() < 90.f && !player->HasAura(240559) && player->IsInCombat())
                    player->CastSpell(player, 240559, true);

                if (player->GetHealthPct() > 90.f || player->GetHealthPct() < 60.f)
                    player->RemoveAurasDueToSpell(240559);
            }
        }
    }

    void OnUpdate(Player* player, uint32 diff)
    {
        if (checkTimer <= diff)
        {
            if (isstarted)
            {
                isstarted = CheckChallengeModeStarted(player);

                if (_affix2 == 3 && player->IsInCombat())//olcanic-plume
                {
                    player->CastSpell(player, 209861, true);//105877
                }
                if (_affix2 == 13 && player->IsInCombat())//explosive
                {
                    if (roll_chance_i(10))
                    {
                        if (Creature* explosives = player->SummonCreature(120651, player->GetNearPosition(10.f, 0.0f), TEMPSUMMON_TIMED_DESPAWN, 10000, 0, false))
                        {
                            explosives->CastSpell(explosives, 240446, true);
                        }
                    }
                }
                if (_affix2 == 14 && player->IsInCombat())//quaking
                {
                    player->AddAura(240447);
                }
            }
            checkTimer = 15000;
        }
        else checkTimer -= diff;

    }
    uint32 checkTimer = 10000;
    bool isstarted = false;
    uint8 _affix1;
    uint8 _affix2;
    uint8 _affix3;
};
//226489 / sanguine_ichor AT 12765
struct at_challenge_sanguine_ichor : AreaTriggerAI
{
    at_challenge_sanguine_ichor(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    void OnUnitEnter(Unit* unit) override
    {
        if (!unit)
            return;
        if (unit->IsPlayer())
            unit->CastSpell(unit, 226512, true);
        else
            unit->CastSpell(unit, 226510, true);
    }

    void OnUnitExit(Unit* unit) override
    {
        if (!unit)
            return;

        if (unit->HasAura(226512))
            unit->RemoveAura(226512);
        if (unit->HasAura(226510))
            unit->RemoveAura(226510);
    }

    void OnRemove() override
    {
        Unit* caster = at->GetCaster();

        if (!caster)
            return;

        for (auto guid : at->GetInsideUnits())
            if (Unit* unit = ObjectAccessor::GetUnit(*caster, guid))
            {
                if (unit->HasAura(226512))
                    unit->RemoveAura(226512);
                if (unit->HasAura(226510))
                    unit->RemoveAura(226510);
            }
    }
};

//274738
class spell_maghar_orc_racial_ancestors_call : public SpellScript
{
    PrepareSpellScript(spell_maghar_orc_racial_ancestors_call);

    void Oncast()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        uint32 RandomStats = urand(0, 3);

        switch (RandomStats)
        {   
        case 0:        
            //mastery
            caster->CastSpell(nullptr, 274741, true);
            break;

        case 1:
  
            //versatility
            caster->CastSpell(nullptr, 274742, true);
            break;
    
        case 2:    
            //haste
            caster->CastSpell(nullptr, 274740, true);
            break;
  
        case 3:     
            //crit
            caster->CastSpell(nullptr, 274739, true);    
            break;
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_maghar_orc_racial_ancestors_call::Oncast);
    }
};

void AddSC_generic_spell_scripts()
{
    new spell_challengers_might();
    RegisterCreatureAI(npc_volcanic_plume_105877);
    new challange_player_instance_handler();
    RegisterAreaTriggerAI(at_challenge_sanguine_ichor);
    new spell_class_mecagnomo_emergency();
    new spell_class_mecagnomo_emergency2();
    new spell_class_mecagnomo_emergency3();
    new spell_class_hall_panel();
    new spell_hidden_appearance_unlock();
    new spell_sa_place_seaforium_charge();
    new spell_gen_taunt_flag_targeting();
    new spell_gen_absorb_if_health();
    new spell_principles_of_war();
    new spell_gen_create_artefact();
    new spell_gen_herbalism_trap();
    new spell_gen_mining_trap();
    new spell_gen_learn_legion_skinning();
    new spell_gen_bounce_achievement();
    new spell_gen_bg_inactive();
    new spell_gen_cooking_way();
    new spell_gen_battle_guild_standart();
    new spell_gen_drums_of_rage();
    new spell_gen_orb_of_power();
    new spell_gen_dampening();
    new spell_gen_brutal_assault();
    new spell_time_lost_wisdom();
    new spell_gulp_frog_toxin();
    new spell_gen_leviroth_self_impale();
    new spell_gen_tricky_treat();
    new spell_brewfest_speed();
    new spell_gen_gobelin_gumbo();
    new spell_gen_mount("spell_magic_broom", 0, SPELL_MAGIC_BROOM_60, SPELL_MAGIC_BROOM_100, SPELL_MAGIC_BROOM_150, SPELL_MAGIC_BROOM_280);
    new spell_gen_mount("spell_headless_horseman_mount", 0, SPELL_HEADLESS_HORSEMAN_MOUNT_60, SPELL_HEADLESS_HORSEMAN_MOUNT_100, SPELL_HEADLESS_HORSEMAN_MOUNT_150, SPELL_HEADLESS_HORSEMAN_MOUNT_280);
    new spell_gen_mount("spell_big_love_rocket", SPELL_BIG_LOVE_ROCKET_0, SPELL_BIG_LOVE_ROCKET_60, SPELL_BIG_LOVE_ROCKET_100, SPELL_BIG_LOVE_ROCKET_150, SPELL_BIG_LOVE_ROCKET_310);
    new spell_gen_mount("spell_invincible", 0, SPELL_INVINCIBLE_60, SPELL_INVINCIBLE_100, SPELL_INVINCIBLE_150, SPELL_INVINCIBLE_310);
    new spell_gen_mount("spell_blazing_hippogryph", 0, 0, 0, SPELL_BLAZING_HIPPOGRYPH_150, SPELL_BLAZING_HIPPOGRYPH_280);
    new spell_gen_mount("spell_celestial_steed", 0, SPELL_CELESTIAL_STEED_60, SPELL_CELESTIAL_STEED_100, SPELL_CELESTIAL_STEED_150, SPELL_CELESTIAL_STEED_280, SPELL_CELESTIAL_STEED_310);
    new spell_gen_mount("spell_x53_touring_rocket", 0, 0, 0, SPELL_X53_TOURING_ROCKET_150, SPELL_X53_TOURING_ROCKET_280, SPELL_X53_TOURING_ROCKET_310);
    new spell_gen_dummy_trigger();
    new spell_gen_launch();
    new spell_gen_magic_rooster();
    new spell_gen_gunship_portal();
    new spell_gen_parachute_ic();
    new spell_gen_trick();
    new spell_gen_trick_or_treat();
    new spell_gen_feast();
    new spell_endurance_of_niuzao();
    new spell_sha_cloud();
    new spell_gen_absorb0_hitlimit1();
    RegisterAuraScript(spell_gen_adaptive_warding);
    new spell_gen_allow_cast_from_item_only();
    new spell_gen_animal_blood();
    new spell_gen_aura_of_anger();
    new spell_gen_aura_service_uniform();
    new spell_gen_av_drekthar_presence();
    new spell_gen_bandage();
    new spell_gen_blood_reserve();
    new spell_gen_bonked();
    new spell_gen_break_shield("spell_gen_break_shield");
    new spell_gen_break_shield("spell_gen_tournament_counterattack");
    new spell_gen_burn_brutallus();
    new spell_gen_burning_depths_necrolyte_image();
    new spell_gen_cannibalize();
    new spell_gen_chaos_blast();
    new spell_gen_clone();
    new spell_gen_clone_weapon();
    new spell_gen_clone_weapon_aura();
    new spell_gen_count_pct_from_max_hp("spell_gen_default_count_pct_from_max_hp");
    new spell_gen_count_pct_from_max_hp("spell_gen_50pct_count_pct_from_max_hp", 50);
    new spell_gen_create_lance();
    new spell_gen_creature_permanent_feign_death();
    new spell_gen_dalaran_disguise("spell_gen_sunreaver_disguise");
    new spell_gen_dalaran_disguise("spell_gen_silver_covenant_disguise");
    new spell_gen_defend();
    new spell_gen_despawn_self();
    new spell_gen_divine_storm_cd_reset();
    new spell_gen_ds_flush_knockback();
    new spell_gen_dungeon_credit();
    new spell_gen_elune_candle();
    new spell_gen_fishing();
    new spell_gen_gadgetzan_transporter_backfire();
    new spell_gen_gift_of_naaru();
    new spell_gen_gnomish_transporter();
    new spell_gen_increase_stats_buff("spell_pal_blessing_of_kings");
    new spell_gen_increase_stats_buff("spell_pal_blessing_of_might");
    new spell_gen_increase_stats_buff("spell_dru_mark_of_the_wild");
    new spell_gen_increase_stats_buff("spell_pri_power_word_fortitude");
    new spell_gen_interrupt();
    new spell_gen_lifebloom("spell_hexlord_lifebloom", SPELL_HEXLORD_MALACRASS_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_tur_ragepaw_lifebloom", SPELL_TUR_RAGEPAW_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_cenarion_scout_lifebloom", SPELL_CENARION_SCOUT_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_twisted_visage_lifebloom", SPELL_TWISTED_VISAGE_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_faction_champion_dru_lifebloom", SPELL_FACTION_CHAMPIONS_DRU_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_mounted_charge();
    new spell_gen_netherbloom();
    new spell_gen_obsidian_armor();
    new spell_gen_oracle_wolvar_reputation();
    new spell_gen_orc_disguise();
    new spell_gen_paralytic_poison();
    new spell_gen_proc_below_pct_damaged("spell_item_soul_harvesters_charm");
    new spell_gen_proc_below_pct_damaged("spell_item_commendation_of_kaelthas");
    new spell_gen_proc_below_pct_damaged("spell_item_corpse_tongue_coin");
    new spell_gen_proc_below_pct_damaged("spell_item_corpse_tongue_coin_heroic");
    new spell_gen_proc_below_pct_damaged("spell_item_petrified_twilight_scale");
    new spell_gen_proc_below_pct_damaged("spell_item_petrified_twilight_scale_heroic");
    new spell_gen_parachute();
    new spell_gen_pet_summoned();
    new spell_gen_profession_research();
    new spell_gen_pvp_trinket();
    new spell_gen_remove_flight_auras();
    new spell_gen_replenishment();
    // Running Wild
    new spell_gen_running_wild();
    new spell_gen_two_forms();
    new spell_gen_darkflight();
    new spell_gen_seaforium_blast();
    new spell_gen_spectator_cheer_trigger();
    new spell_gen_spirit_healer_res();
    new spell_gen_summon_elemental("spell_gen_summon_fire_elemental", SPELL_SUMMON_FIRE_ELEMENTAL);
    new spell_gen_summon_elemental("spell_gen_summon_earth_elemental", SPELL_SUMMON_EARTH_ELEMENTAL);
    new spell_gen_summon_tournament_mount();
    new spell_gen_throw_shield();
    new spell_gen_tournament_duel();
    new spell_gen_tournament_pennant();
    new spell_gen_trigger_exclude_caster_aura_spell();
    new spell_gen_trigger_exclude_target_aura_spell();
    new spell_pvp_trinket_wotf_shared_cd<SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER>("spell_pvp_trinket_shared_cd");
    new spell_pvp_trinket_wotf_shared_cd<SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER_WOTF>("spell_wotf_shared_cd");
    new spell_gen_turkey_marker();
    new spell_gen_upper_deck_create_foam_sword();
    new spell_gen_vampiric_touch();
    new spell_gen_vehicle_scaling();
    new spell_gen_vendor_bark_trigger();
    new spell_gen_wg_water();
    new spell_gen_whisper_gulch_yogg_saron_whisper();
    new spell_gen_eject_all_passengers();
    new spell_gen_eject_passenger();
    new spell_gen_gm_freeze();
    new spell_gen_stand();
    new spell_gen_mixology_bonus();
    new spell_gen_landmine_knockback_achievement();
    new spell_gen_clear_debuffs();
    new spell_gen_pony_mount_check();
    new spell_gen_shroud_of_death();
    new spell_gen_decimatus_transformation_sickness();
    new spell_gen_anetheron_summon_towering_infernal();
    new spell_gen_mark_of_kazrogal_hellfire();
    new spell_gen_azgalor_rain_of_fire_hellfire_citadel();
    new spell_gen_face_rage();
    new spell_gen_impatient_mind();
    RegisterAuraScript(spell_gen_eredar_bloodmage_blood_siphon);
    RegisterSpellScript(spell_gen_eredar_bloodmage_blood_siphon_damage);
    RegisterSpellScript(spell_gen_mobile_bank);
    RegisterSpellScript(spell_arcane_pulse);
    RegisterSpellScript(spell_spatial_rift_despawn);
    RegisterSpellScript(spell_light_judgement);
    RegisterAuraScript(spell_legion_hearty_feast);
    RegisterAuraScript(spell_legion_food_table);
    RegisterSpellScript(spell_gen_hearthstone_board);
    RegisterAuraScript(spell_gen_winning_hand);
    RegisterAuraScript(spell_request_the_master_call_on_you);
    RegisterSpellScript(spell_searing_gaze_of_the_dook);
    RegisterSpellScript(spell_searing_gaze_of_the_dook_hit);
    RegisterAuraScript(spell_searing_gaze_of_the_dook_despawn);
    RegisterSpellScript(spell_deck_o_matic);
    RegisterSpellScript(spell_improved_card_pack);
    RegisterSpellScript(spell_kaldorei_light_globe);
    RegisterSpellScript(spell_throw_sphere_catch);
    RegisterSpellScript(spell_set_up_board);
    RegisterAuraScript(spell_hearthstation_horde);
    RegisterAuraScript(spell_hearthstation_alliance);
    RegisterAuraScript(spell_q29347_crayfish);
    RegisterSpellScript(spell_q13698_saw);
    RegisterAuraScript(spell_q13698_shredder_aura);
    RegisterSpellScript(spell_gen_love_seat);
    RegisterAuraScript(spell_q29347_bait);
    new playerscript_light_reckoning();
    new spell_pvp_honor_adaptation();
    new spell_pvp_honor_medallion();
    new spell_gen_gilneas_prison_periodic_dummy();
    new spell_gen_dream_funnel();
    new spell_gen_throw_torch();
    new spell_gen_on_tournament_mount();
    RegisterSpellScript(spell_make_camp);
    RegisterSpellScript(spell_back_camp);
    RegisterSpellScript(spell_maghar_orc_racial_ancestors_call);
}
