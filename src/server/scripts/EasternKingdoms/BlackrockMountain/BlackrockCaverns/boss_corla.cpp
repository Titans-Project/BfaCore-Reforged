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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_caverns.h"

enum Text
{
    YELL_AGGRO                     = 0,
    YELL_KILL                      = 1,
    YELL_EVOLVED_ZEALOT            = 2,
    YELL_DEATH                     = 3,
    EMOTE_EVOLVED_ZEALOT           = 4
};

enum Spells
{
    SPELL_EVOLUTION                = 75610,
    SPELL_DRAIN_ESSENSE            = 75645,
    SPELL_SHADOW_POWER             = 35322,
    SPELL_KNEELING_IN_SUPPLICATION = 75608,
    SPELL_GRAVITY_STRIKE           = 76561,
    SPELL_GRIEVOUS_WHIRL           = 93658,
    SPELL_NETHERBEAM               = 75677,
    SPELL_FORCE_BLAST              = 76522,
    SPELL_SHADOW_STRIKE            = 66134,
    SPELL_TWILIGHT_EVOLUTION       = 75732,
    H_SPELL_SHADOW_POWER           = 39193
};

enum Events
{
    // Out of combat events
    EVENT_DRAIN_ESSENSE            = 1,
    EVENT_STOP_DRAIN_ESSENSE       = 2,
    EVENT_EVOLUTION                = 3,
    EVENT_FORCE_BLAST              = 4,
    EVENT_GRAVITY_STRIKE           = 5,
    EVENT_GRIEVOUS_WHIRL           = 6,
    EVENT_SHADOW_STRIKE            = 7
    // Combat events
};

enum Actions
{
    ACTION_TRIGGER_START_CHANNELING = 1,
    ACTION_TRIGGER_STOP_CHANNELING  = 2
};

class boss_corla : public CreatureScript
{
    public:
        boss_corla(): CreatureScript("boss_corla") { }

        struct boss_corlaAI : public BossAI
        {
            boss_corlaAI(Creature* creature) : BossAI(creature, DATA_CORLA) { }

            void Reset() override
            {
                _Reset();
                combatPhase = false;
                events.ScheduleEvent(EVENT_DRAIN_ESSENSE, 2000);
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();
                Talk(YELL_AGGRO);
                events.Reset();
                combatPhase = true;
            }

            void KilledUnit(Unit* who) override
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    Talk(YELL_KILL);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                Talk(YELL_DEATH);
            }

            void UpdateAI(uint32 diff) override
            {
                events.Update(diff);

                if (!combatPhase)
                {
                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                            case EVENT_DRAIN_ESSENSE:
                                DoCast(me, SPELL_DRAIN_ESSENSE);
                                events.ScheduleEvent(EVENT_STOP_DRAIN_ESSENSE, 15000);
                                break;
                            case EVENT_STOP_DRAIN_ESSENSE:
                                me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                                events.ScheduleEvent(EVENT_EVOLUTION, 2000);
                                break;
                            case EVENT_EVOLUTION:
                                DoCast(me, SPELL_EVOLUTION);
                                events.ScheduleEvent(EVENT_DRAIN_ESSENSE, 2000);
                                break;
                            default:
                                break;
                        }
                    }
                    return;
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool combatPhase = false;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBlackrockCavernsAI<boss_corlaAI>(creature);
        }
};

class mob_twilight_zealot : public CreatureScript
{
public:
    mob_twilight_zealot() : CreatureScript("mob_twilight_zealot") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_twilight_zealotAI(creature);
    }

    struct mob_twilight_zealotAI : public ScriptedAI
    {
        mob_twilight_zealotAI(Creature* creature) : ScriptedAI(creature), Intialized(false) {}

        bool Intialized;
        EventMap events;

        void Reset() override
        {
            events.Reset();
            Intialized = false;
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 Diff) override
        {
            if (!Intialized && !me->HasAura(SPELL_KNEELING_IN_SUPPLICATION))
            {
                events.RescheduleEvent(EVENT_FORCE_BLAST, 10000);
                events.RescheduleEvent(EVENT_GRAVITY_STRIKE, 22000);
                events.RescheduleEvent(EVENT_GRIEVOUS_WHIRL, 7000);
                events.RescheduleEvent(EVENT_SHADOW_STRIKE, 14000);

                Intialized = true;

                me->SetReactState(REACT_AGGRESSIVE);

                me->GetMotionMaster()->MoveChase(GetPlayerAtMinimumRange(1.0f));
                me->Attack(GetPlayerAtMinimumRange(1.0f), false);
            }
            if (!me->IsInCombat() || me->HasAura(SPELL_TWILIGHT_EVOLUTION))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FORCE_BLAST:
                    if (auto target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        DoCast(target, SPELL_FORCE_BLAST);

                    events.RescheduleEvent(EVENT_FORCE_BLAST, 10000);
                    break;
                case EVENT_GRAVITY_STRIKE:
                    DoCastVictim(SPELL_GRAVITY_STRIKE);
                    events.RescheduleEvent(EVENT_GRAVITY_STRIKE, 22000);
                    break;
                case EVENT_GRIEVOUS_WHIRL:
                    DoCastAOE(SPELL_GRIEVOUS_WHIRL);
                    events.RescheduleEvent(EVENT_GRIEVOUS_WHIRL, 7000);
                    break;
                case EVENT_SHADOW_STRIKE:
                    DoCastVictim(SPELL_SHADOW_STRIKE);
                    events.RescheduleEvent(EVENT_SHADOW_STRIKE, 14000);
                    break;
                default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class mob_corla_netheressence_trigger : public CreatureScript
{
public:
    mob_corla_netheressence_trigger() : CreatureScript("mob_corla_netheressence_trigger") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_corla_netheressence_triggerAI(creature);
    }

    struct mob_corla_netheressence_triggerAI : public ScriptedAI
    {
        mob_corla_netheressence_triggerAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        ObjectGuid zealotGuid;
        ObjectGuid channelTargetGuid;
        ObjectGuid lastTargetGuid;

        Map::PlayerList CharmedPlayerList;

        uint32 uiCheckPlayerIsBetween;
        uint32 uiNetherEssenceVisual;

        void Reset() override
        {
            me->AddUnitFlag(UnitFlags(UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_REMOVE_CLIENT_CONTROL | UNIT_FLAG_NOT_SELECTABLE));
        }

        void UpdateAI(uint32 diff) override
        {
            if (!channelTargetGuid || !zealotGuid)
                return;

            Unit* channelTarget = ObjectAccessor::GetUnit(*me, channelTargetGuid);
            Unit* zealot = ObjectAccessor::GetUnit(*me, zealotGuid);
            if (!channelTarget || !zealot)
                return;

            if (zealot->HasAura(SPELL_TWILIGHT_EVOLUTION) || zealot->isDead())
                return;

            if (uiCheckPlayerIsBetween <= diff)
            {
                channelTarget = zealot;
                Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (i->GetSource()->IsInBetween(me, zealot, 1.0f))
                            channelTarget = i->GetSource();
                }

                me->CastSpell(channelTarget, SPELL_NETHERBEAM, true);

                zealot->SetAuraStack(SPELL_EVOLUTION, channelTarget, channelTarget->GetAuraCount(SPELL_EVOLUTION) + 1);
               
                if (channelTarget->GetAuraCount(SPELL_EVOLUTION) == 100)
                {
                    if (channelTarget == zealot)
                        channelTarget->RemoveAllAuras();

                    zealot->CastSpell(channelTarget, SPELL_TWILIGHT_EVOLUTION, true);
                }

                uiCheckPlayerIsBetween = 175;
            }
            else uiCheckPlayerIsBetween -= diff;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            zealotGuid = summoner->GetGUID();
        }

        void DoAction(const int32 action) override
        {
            switch (action)
            {
            case ACTION_TRIGGER_START_CHANNELING:
                CharmedPlayerList.clearReferences();

                channelTargetGuid = zealotGuid;

                uiCheckPlayerIsBetween = 100;
                lastTargetGuid = me->GetGUID();
                break;

            case ACTION_TRIGGER_STOP_CHANNELING:
                me->RemoveAllAuras();
                lastTargetGuid.Clear();
                channelTargetGuid.Clear();
                break;
            }
        }
    };
};

void AddSC_boss_corla()
{
    new boss_corla();
    new mob_twilight_zealot();
    new mob_corla_netheressence_trigger();
}
