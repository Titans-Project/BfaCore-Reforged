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

#include "the_motherlode.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "Creature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "Unit.h"

enum Spells
{
    SPELL_RAGING_GAZE_BEAM = 257582,
    SPELL_RAGING_GAZE_SELECTOR = 271526,
    SPELL_CALL_EARTHRAGER = 257593,
    SPELL_AZERITE_INFUSION_MISSILE = 271698,
    SPELL_JAGGED_CUT = 257543,
    SPELL_RESONANT_PULSE = 258622,
    SPELL_RESONANT_QUAKE = 258627,
    SPELL_TECTONIC_SMASH = 275907,

    SPELL_FRACKING_TOTEM_BUTTON = 257455,
    SPELL_FRACKING_TOTEM_CAST = 257480,
    SPELL_FRACKING_TOTEM_STUN = 257481,
    SPELL_FRACKING_TOTEM_SUMMON = 268204,

};

enum Events
{
    EVENT_FIXATE = 1,
    EVENT_AZERITE_INFUSION,
    EVENT_JAGGED_CUT,
    EVENT_CALL_EARTHRAGER,
    EVENT_RESONANT_PULSE,
    EVENT_RESONANT_QUAKE,
    EVENT_TECTONIC_SMASH,

    EVENT_TOTEM_TARGET,
};

enum Timers
{
    TIMER_AZERITE_INFUSION = 19600,
    TIMER_CALL_EARTHRAGER = 60000,
    TIMER_RESONANT_PULSE = 10600,
    TIMER_TECTONIC_SMASH = 5000,

    TIMER_TOTEM_TARGET = 2000,
};
enum Creatures
{
    BOSS_AZEROKK = 129227,
    NPC_EARTHRAGERS = 129802,
    NPC_FRACKING_TOTEM = 136500,
    NPC_FRACKING_TOTEM_PRE = 129804,
};

const Position centerPos = { 1215.50f, -3325.10f, 57.33f }; // 40y

const Position earthragerPos[4] =
{
    { 1233.36f, -3321.13f, 57.50f },
    { 1216.14f, -3340.71f, 55.34f },
    { 1199.50f, -3330.84f, 55.80f },
    { 1202.84f, -3315.90f, 55.58f },
};

const Position frackingPos[2] =
{
    { 1180.16f, -3300.33f, 56.83f },
    { 1181.85f, -3301.90f, 56.79f },
};

enum Texts
{
    SAY_AGGRO = 0,             // 0 You trespass in my domain!
    SAY_RESONANT_PULSE,        // 1 |TINTERFACE\ICONS\SPELL_SHAMAN_EARTHQUAKE.BLP:20|t Azerokk prepares to cast |cFFFF0000|Hspell:258622|h[Resonant Pulse]|h|r!
    SAY_RESONANT_PULSE_FINISH1,// 2 Shatter!
    SAY_KILL,                  // 3 The blood of Azeroth flows through you!
    SAY_RESONANT_PULSE_FINISH2,// 4 The ground will consume you!
    SAY_AZERITE_INFUSION,      // 5 Unleash your power!
    SAY_CALL_EARTHRAGER,       // 6 Arise!
    SAY_DEAD                   // 7 The wounds... must be mended...
};

// 129227
struct bfa_boss_azerokk : public BossAI {
    bfa_boss_azerokk(Creature* creature) : BossAI(creature, DATA_AZEROKK), summons(me)
    {
        instance = me->GetInstanceScript();
        SpawnAdd();
        me->SetPowerType(POWER_MANA);
    }

    EventMap events;
    InstanceScript* instance;
    SummonList summons;

    void JustSummoned(Creature* summon) override
    {
        summons.Summon(summon);

        switch (summon->GetEntry())
        {
        case NPC_EARTHRAGERS:
        {
            summon->SetCombatDistance(10.0f);
            break;
        }
        }
    }

    void SpawnAdd()
    {
        std::list<Creature*> reagersList = me->FindNearestCreatures(NPC_EARTHRAGERS, 30.0f);
        std::list<Creature*> totemList = me->FindNearestCreatures(NPC_FRACKING_TOTEM_PRE, 45.0f);

        for (auto reagers : reagersList)
            reagers->ForcedDespawn();

        for (auto totems : totemList)
            totems->ForcedDespawn();

        for (uint8 i = 0; i < 4; ++i)
            me->SummonCreature(NPC_EARTHRAGERS, earthragerPos[i], TEMPSUMMON_DEAD_DESPAWN);

        for (uint8 i = 0; i < 2; ++i)
            me->SummonCreature(NPC_FRACKING_TOTEM_PRE, frackingPos[i], TEMPSUMMON_MANUAL_DESPAWN);
    }

    void Reset() override
    {
        BossAI::Reset();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        events.Reset();
        SpawnAdd();
    }

    void KilledUnit(Unit* /*unit*/) override
    {
        Talk(SAY_KILL);
    }

    void JustDied(Unit*) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        summons.DespawnAll();
        Talk(SAY_DEAD);
    }

    void EnterCombat(Unit*) /*override*/
    {
        Talk(SAY_AGGRO);
        PlayAnimKits();
        EnterInCombat();
        me->SetInMovement(false, 5000);

        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

        if (me->GetMap()->IsMythic() || me->GetMap()->IsHeroic())
            events.ScheduleEvent(EVENT_TECTONIC_SMASH, TIMER_TECTONIC_SMASH);

        events.ScheduleEvent(EVENT_CALL_EARTHRAGER, TIMER_CALL_EARTHRAGER);
        events.ScheduleEvent(EVENT_AZERITE_INFUSION, TIMER_AZERITE_INFUSION);
        events.ScheduleEvent(EVENT_RESONANT_PULSE, TIMER_RESONANT_PULSE);
    }

    void PlayAnimKits()
    {
        me->PlayOneShotAnimKitId(11428);
        me->PlayOneShotAnimKitId(625);
    }

    void EnterInCombat()
    {
        std::list<Creature*> reagersList = me->FindNearestCreatures(NPC_EARTHRAGERS, 30.0f);

        for (auto reager : reagersList)
            reager->SetInCombatWithZone();
    }

    void EnterEvadeMode(EvadeReason /*why*/)
    {
        BossAI::EnterEvadeMode();
        me->GetMotionMaster()->MoveTargetedHome();
        Reset();
    }

    void OnSpellFinished(SpellInfo const* spellInfo) /*override*/
    {
        switch (spellInfo->Id)
        {

        case SPELL_RESONANT_PULSE:
        {
            std::list<Creature*> ragers;
            me->GetCreatureListWithEntryInGrid(ragers, NPC_EARTHRAGERS, 100.0f);

            if (!ragers.empty())
                for (auto rager : ragers)
                    rager->CastSpell(rager->GetVictim(), SPELL_RESONANT_QUAKE);

            break;
        }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (CheckHomeDistToEvade(diff, 25.0f))
        {
            Reset();
            return;
        }

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_RESONANT_PULSE:
            {
                Talk(SAY_RESONANT_PULSE);
                me->CastSpell(me, SPELL_RESONANT_PULSE);
                events.ScheduleEvent(EVENT_RESONANT_PULSE, 33200);
                break;
            }
            case EVENT_AZERITE_INFUSION:
            {
                Talk(SAY_AZERITE_INFUSION);
                std::list<Creature*> clist = me->FindNearestCreatures(NPC_EARTHRAGERS, 100.0f);

                if (!clist.empty())
                {
                    for (auto toDelete : clist)
                        if (toDelete->HasAura(SPELL_FRACKING_TOTEM_STUN))
                            clist.remove(toDelete);

                    if (clist.size() > 1)
                        clist.resize(1);

                    for (auto reager : clist)
                        me->CastSpell(reager, SPELL_AZERITE_INFUSION_MISSILE);
                }

                events.ScheduleEvent(EVENT_AZERITE_INFUSION, 13000);
                break;
            }

            case EVENT_TECTONIC_SMASH:
            {
                me->CastSpell(me->GetVictim(), SPELL_TECTONIC_SMASH);
                events.ScheduleEvent(EVENT_TECTONIC_SMASH, 23000);
                break;
            }

            case EVENT_CALL_EARTHRAGER:
            {
                Talk(SAY_CALL_EARTHRAGER);
                me->CastSpell(me, SPELL_CALL_EARTHRAGER);
                events.ScheduleEvent(EVENT_CALL_EARTHRAGER, 60400);
                break;
            }

            default:
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

// 129802
struct bfa_npc_earthrager : public ScriptedAI {
    bfa_npc_earthrager(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_FIXATE, 2000);
        events.ScheduleEvent(EVENT_JAGGED_CUT, 3000);
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_FIXATE:
            {
                me->CastSpell(SELECT_TARGET_RANDOM, SPELL_RAGING_GAZE_SELECTOR);
                me->CastSpell(me->GetVictim(), SPELL_RAGING_GAZE_BEAM);

                break;
            }

            case EVENT_JAGGED_CUT:
            {
                if (Unit* target = me->GetVictim())
                    if (me->IsWithinMeleeRange(target))
                        me->CastSpell(target, SPELL_JAGGED_CUT, true);

                events.ScheduleEvent(EVENT_JAGGED_CUT, 3000);
                break;
            }
            default:
                break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

// Fracking Totem - 136500
struct bfa_npc_fracking_totem : public ScriptedAI {

    bfa_npc_fracking_totem(Creature* creature) : ScriptedAI(creature) { }

    bool canStun;

    void Reset() override
    {
        canStun = false;
    }

    void StunRager()
    {
        if (!canStun)
        {
            canStun = true;
            me->AddUnitFlag(UnitFlags(UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC));
            Player* invoker;

            if (me->IsSummon())
            {
                invoker = me->ToTempSummon()->GetSummoner()->ToPlayer();
                me->ToTempSummon()->InitStats(60000);
            }

            if (!invoker)
                return;

            Unit* rager = invoker->GetSelectedUnit();

            if (rager->GetEntry() == NPC_EARTHRAGERS && !rager->HasAura(SPELL_FRACKING_TOTEM_STUN))
            {
                SetInCombat(invoker, rager);
                me->CastSpell(rager, SPELL_FRACKING_TOTEM_STUN);
            }
        }
    }

    void SetInCombat(Player* invoker, Unit* earthrager)
    {
        earthrager->SetInCombatWith(invoker);
        earthrager->getThreatManager().addThreat(invoker, 999999.9f);
        invoker->SetInCombatWith(earthrager);
        me->SetInCombatWithZone();
    }

    void UpdateAI(uint32 diff) override
    {
        StunRager();
    }
};

// 257480
class bfa_spell_fracking_totem_summon : public SpellScript {

    PrepareSpellScript(bfa_spell_fracking_totem_summon);

    void HandleAfterCast()
    {
        if (!GetCaster())
            return;

        if (Player* player = GetCaster()->GetSpellModOwner())
        {
            int32 castTime = 0;
            Unit* target = GetExplTargetUnit();
            player->ModSpellCastTime(GetSpellInfo(), castTime);
            player->CastCustomSpell(target, SPELL_FRACKING_TOTEM_SUMMON, NULL, NULL, NULL, true, NULL, NULL, player->GetGUID());
        }
    }

    SpellCastResult CheckCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
            if (!caster->isMoving())
                return SPELL_CAST_OK;

        return SPELL_FAILED_MOVING;
    }

    void Register()
    {
        OnCheckCast += SpellCheckCastFn(bfa_spell_fracking_totem_summon::CheckCast);
        AfterCast += SpellCastFn(bfa_spell_fracking_totem_summon::HandleAfterCast);
    }
};

// Resonant Pulse - 258622
class bfa_spell_resonant_pulse : public SpellScript
{

    PrepareSpellScript(bfa_spell_resonant_pulse);

    void HandleAfterCast() {

        uint8 randm = urand(0, 1);

        if (!GetCaster())
            return;

        if (randm == 0)
        {
            GetCaster()->ToCreature()->AI()->Talk(SAY_RESONANT_PULSE_FINISH1);
        }
        else if (randm == 1)
        {
            GetCaster()->ToCreature()->AI()->Talk(SAY_RESONANT_PULSE_FINISH2);
        }

    }

    void Register()
    {
        AfterCast += SpellCastFn(bfa_spell_resonant_pulse::HandleAfterCast);
    }
};

// 129804
struct bfa_npc_fracking_totem_selector : public ScriptedAI
{
    bfa_npc_fracking_totem_selector(Creature* creature) : ScriptedAI(creature) { }

    void SpellHit(Unit* caster, SpellInfo const* spellInfo) override
    {
        if (spellInfo->Id == SPELL_FRACKING_TOTEM_BUTTON && caster->IsPlayer())
            me->DespawnOrUnsummon();
    }
};

void AddSC_boss_azerokk()
{
    RegisterCreatureAI(bfa_boss_azerokk);
    RegisterCreatureAI(bfa_npc_earthrager);
    RegisterCreatureAI(bfa_npc_fracking_totem);
    RegisterCreatureAI(bfa_npc_fracking_totem_selector);
    RegisterSpellScript(bfa_spell_fracking_totem_summon);
    RegisterSpellScript(bfa_spell_resonant_pulse);
}