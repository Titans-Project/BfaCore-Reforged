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
#include "ScriptedGossip.h"
#include "Player.h"
#include "Vehicle.h"

enum Moira
{
    QUEST_SEIZE_AMBASSADOR = 26118
};

class npc_moira_thaurissan : public CreatureScript
{
public:
    npc_moira_thaurissan() : CreatureScript("npc_moira_thaurissan") { }

    enum Data
    {
        NPC_MURADIN         = 42928,
        NPC_WILDHAMMER      = 42131,
        ACTION_START_EVENT  = 1,
    };

    enum TalkData
    {
        TALK_0 = 0,
        TALK_1 = 1,
        TALK_2 = 2,
        TALK_3 = 3,
        TALK_4 = 4,
    };

    bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest) override
    {
        switch (quest->GetQuestId())
        {
            case QUEST_SEIZE_AMBASSADOR:
                creature->AI()->DoAction(ACTION_START_EVENT);
                break;
            default:
                break;
        }

        return true;
    }

    struct npc_moira_thaurissanAI : public ScriptedAI
    {
        npc_moira_thaurissanAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 SummonTimer;
        uint8 Phase;

        void Reset() override
        {
            Phase = 0;
            SummonTimer = 300;
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case ACTION_START_EVENT:
                    SummonTimer = 300;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (SummonTimer < diff)
            {
                switch (Phase)
                {
                    case 0:
                    {
                        Talk(TALK_0);
                        SummonTimer = 1500;
                        Phase++;
                        break;
                    }
                    case 1:
                    {
                        if (Creature* agent = me->FindNearestCreature(NPC_MURADIN, 25.0f, true))
                            agent->AI()->Talk(TALK_0);

                        SummonTimer = 2500;
                        Phase++;
                    }
                    case 2:
                    {
                        Talk(TALK_1);
                        SummonTimer = 3500;
                        Phase++;
                        break;
                    }
                    case 3:
                    {
                        if (Creature* agent = me->FindNearestCreature(NPC_MURADIN, 25.0f, true))
                            agent->AI()->Talk(TALK_1);

                        SummonTimer = 4500;
                        Phase++;
                    }
                    case 4:
                    {
                        Talk(TALK_2);
                        SummonTimer = 5500;
                        Phase++;
                        break;
                    }
                    case 5:
                    {
                        if (Creature* agent = me->FindNearestCreature(NPC_MURADIN, 25.0f, true))
                            agent->AI()->Talk(TALK_2);

                        SummonTimer = 6500;
                        Phase++;
                    }
                    case 6:
                    {
                        Talk(TALK_3);
                        SummonTimer = 7500;
                        Phase++;
                        break;
                    }
                    case 7:
                    {
                        if (Creature* agent = me->FindNearestCreature(NPC_WILDHAMMER, 25.0f, true))
                            agent->AI()->Talk(TALK_0);

                        SummonTimer = 8500;
                        Phase++;
                    }
                    case 8:
                    {
                        Talk(TALK_4);
                        Phase++;
                        break;
                    }
                    default:
                        break;
                }

            }
            else SummonTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_moira_thaurissanAI(creature);
    }
};

class npc_mountaineer_watch : public CreatureScript
{
public:
    npc_mountaineer_watch() : CreatureScript("npc_mountaineer_watch") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(313) == QUEST_STATUS_INCOMPLETE)
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Captain Tharran wants you to deploy your remote observation bots and withdraw to Kharanos.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            SendGossipMenuFor(player, creature->GetEntry(), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            switch (creature->GetEntry())
            {
            case 40991: // Mountaineer Dunstan
                creature->Whisper("Tell Captain Tharran that I'll be back in Kharanos as soon as I've verified that the bot is working correctly.", LANG_UNIVERSAL, 0);
                CloseGossipMenuFor(player);
                break;
            case 40994: // Mountaineer Lewin
                creature->Whisper("I can't wait to get out there and help in the fight against those trolls.", LANG_UNIVERSAL, 0);
                CloseGossipMenuFor(player);
                break;
            case 41056: // Mountaineer Valgrum
                creature->Whisper("We've been fighting nonstop since the cataclysm. It'll be nice to get a rest, if a brief one.", LANG_UNIVERSAL, 0);
                CloseGossipMenuFor(player);
                break;
            }

            float x, y, z;
            creature->GetClosePoint(x, y, z, creature->GetObjectSize() / 3, 5.0f);
            if (Creature* bot = creature->SummonCreature(41052, x, y, z, 0, TEMPSUMMON_MANUAL_DESPAWN)) // Summon Remote Observation Bot.
            {
                bot->GetMotionMaster()->MoveRandom(10.0f);
                bot->DespawnOrUnsummon(10000);
            }

            player->KilledMonsterCredit(creature->GetEntry());
            creature->DespawnOrUnsummon(6000);
        }

        return true;
    }
};

void AddSC_ironforge()
{
    new npc_moira_thaurissan();
    new npc_mountaineer_watch();
}
