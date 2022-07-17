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
#include "MotionMaster.h"
#include "PassiveAI.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"
#include "PhasingHandler.h"

/*####
## npc_valkyr_battle_maiden
####*/

enum Spells_VBM
{
    SPELL_REVIVE                = 51918
};

enum Says_VBM
{
    WHISPER_REVIVE              = 0
};

class npc_valkyr_battle_maiden : public CreatureScript
{
public:
    npc_valkyr_battle_maiden() : CreatureScript("npc_valkyr_battle_maiden") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_valkyr_battle_maidenAI(creature);
    }

    struct npc_valkyr_battle_maidenAI : public PassiveAI
    {
        npc_valkyr_battle_maidenAI(Creature* creature) : PassiveAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            FlyBackTimer = 500;
            phase = 0;
            x = 0.f;
            y = 0.f;
            z = 0.f;
        }

        uint32 FlyBackTimer;
        float x, y, z;
        uint32 phase;

        void Reset() override
        {
            me->setActive(true);
            me->SetVisible(false);
            me->AddUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
            me->SetCanFly(true);

            me->GetPosition(x, y, z);
            z += 4.0f;
            x -= 3.5f;
            y -= 5.0f;
            me->GetMotionMaster()->Clear(false);
            me->SetPosition(x, y, z, 0.0f);
        }

        void UpdateAI(uint32 diff) override
        {
            if (FlyBackTimer <= diff)
            {
                Player* player = nullptr;
                if (me->IsSummon())
                    if (Unit* summoner = me->ToTempSummon()->GetSummoner())
                        player = summoner->ToPlayer();

                if (!player)
                    phase = 3;

                switch (phase)
                {
                    case 0:
                        me->SetWalk(false);
                        me->HandleEmoteCommand(EMOTE_STATE_FLYGRABCLOSED);
                        FlyBackTimer = 500;
                        break;
                    case 1:
                        player->GetClosePoint(x, y, z, me->GetObjectSize());
                        z += 2.5f;
                        x -= 2.0f;
                        y -= 1.5f;
                        me->GetMotionMaster()->MovePoint(0, x, y, z);
                        me->SetTarget(player->GetGUID());
                        me->SetVisible(true);
                        FlyBackTimer = 4500;
                        break;
                    case 2:
                        if (!player->IsResurrectRequested())
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_CUSTOM_SPELL_01);
                            DoCast(player, SPELL_REVIVE, true);
                            Talk(WHISPER_REVIVE, player);
                        }
                        FlyBackTimer = 5000;
                        break;
                    case 3:
                        me->SetVisible(false);
                        FlyBackTimer = 3000;
                        break;
                    case 4:
                        me->DisappearAndDie();
                        break;
                    default:
                        //Nothing To DO
                        break;
                }
                ++phase;
            } else FlyBackTimer-=diff;
        }
    };

};

/* EbonHold phasehandler */
class phasehandler_ebon_hold : public PlayerScript
{
public:
    phasehandler_ebon_hold() : PlayerScript("phasehandler_ebon_hold") { }


    uint32 PLAYER_CHAPTER                  = 1; /* starts from chapter 1, value updated by quest progress */

    uint32 PHASE_CHAPTER1                  = 169;
    uint32 PHASE_CHAPTER2                  = 171;
    uint32 PHASE_CHAPTER3                  = 175;
    uint32 PHASE_CHAPTER4                  = 176;
    uint32 PHASE_EBON_HOLD                 = 553; // active on all chapters, only disabled on realm of shadows
    uint32 PHASE_REALM_OF_SHADOWS          = 174;
    uint32 PHASE_CHAPTERS_COMPLETE         = 177;

    uint32 QUEST_WILL_OF_THE_LICH_KING     = 12714;
    uint32 QUEST_SCARLET_APOCALYPSE        = 12778;
    uint32 QUEST_THE_LICH_KINGS_COMMAND    = 12800;
    uint32 QUEST_THE_BATTLE_FOR_EBON_HOLD  = 13166;

    uint32 SPELL_REALM_OF_SHADOWS          = 52693;

    uint32 AREA_EBON_HOLD_EAST_KINGD       = 4281;

    uint32 MAP_EBON_HOLD                   = 609;
    uint32 MAP_EASTERN_KINGDOMS            = 0;


    void OnUpdate(Player* player, uint32) override
    {
        /* final phase of ebon hold after all chapters are completed */
        if ((player->GetAreaId() == AREA_EBON_HOLD_EAST_KINGD) && (player->GetMapId() == MAP_EASTERN_KINGDOMS))
        {
            if ((player->GetQuestStatus(QUEST_THE_BATTLE_FOR_EBON_HOLD) == QUEST_STATUS_REWARDED) || (player->IsAlliedRace()))
            {
                if (!player->GetPhaseShift().HasPhase(PHASE_CHAPTERS_COMPLETE)) PhasingHandler::AddPhase(player, PHASE_CHAPTERS_COMPLETE, true);
            }
        }

        if (player->GetMapId() == MAP_EBON_HOLD)
        {
            /* Update current chapter of player */
            if (player->HasQuest(QUEST_THE_LICH_KINGS_COMMAND) || player->GetQuestStatus(QUEST_THE_LICH_KINGS_COMMAND) == QUEST_STATUS_REWARDED)    PLAYER_CHAPTER = 4;
            else if (player->HasQuest(QUEST_SCARLET_APOCALYPSE) || player->GetQuestStatus(QUEST_SCARLET_APOCALYPSE) == QUEST_STATUS_REWARDED)       PLAYER_CHAPTER = 3;
            else if (player->HasQuest(QUEST_WILL_OF_THE_LICH_KING) || player->GetQuestStatus(QUEST_WILL_OF_THE_LICH_KING) == QUEST_STATUS_REWARDED) PLAYER_CHAPTER = 2;
            else PLAYER_CHAPTER = 1;


            /* Handle Realm of Shadows phase shifts in scarlet enclave */
            if (player->HasAura(SPELL_REALM_OF_SHADOWS))
            {
                if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER1))       PhasingHandler::RemovePhase(player, PHASE_CHAPTER1, true);
                if (player->GetPhaseShift().HasPhase(PHASE_EBON_HOLD))      PhasingHandler::RemovePhase(player, PHASE_EBON_HOLD, true);
                if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER2))       PhasingHandler::RemovePhase(player, PHASE_CHAPTER2, true);
                if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER3))       PhasingHandler::RemovePhase(player, PHASE_CHAPTER3, true);
                if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER4))       PhasingHandler::RemovePhase(player, PHASE_CHAPTER4, true);
                if (!player->GetPhaseShift().HasPhase(PHASE_REALM_OF_SHADOWS)) PhasingHandler::AddPhase(player, PHASE_REALM_OF_SHADOWS, true);
            }


            /* Handle chapter phase shifts in scarlet enclave */
            if (!player->HasAura(SPELL_REALM_OF_SHADOWS))
            {
                /* Chapter 1 phase */
                if (PLAYER_CHAPTER == 1)
                {
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER2)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER2, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER3)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER3, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER4)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER4, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_CHAPTER1))   PhasingHandler::AddPhase(player, PHASE_CHAPTER1, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_EBON_HOLD))  PhasingHandler::AddPhase(player, PHASE_EBON_HOLD, true);
                }

                /* Chapter 2 phase */
                if (PLAYER_CHAPTER == 2)
                {
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER1)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER1, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER3)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER3, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER4)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER4, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_CHAPTER2))   PhasingHandler::AddPhase(player, PHASE_CHAPTER2, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_EBON_HOLD))  PhasingHandler::AddPhase(player, PHASE_EBON_HOLD, true);
                }

                /* Chapter 3 phase */
                if (PLAYER_CHAPTER == 3)
                {
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER1)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER1, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER2)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER2, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER4)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER4, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_CHAPTER3))   PhasingHandler::AddPhase(player, PHASE_CHAPTER3, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_EBON_HOLD))  PhasingHandler::AddPhase(player, PHASE_EBON_HOLD, true);
                }

                /* Chapter 4 phase */
                if (PLAYER_CHAPTER == 4)
                {
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER1)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER1, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER2)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER2, true);
                    if (player->GetPhaseShift().HasPhase(PHASE_CHAPTER3)) PhasingHandler::RemovePhase(player, PHASE_CHAPTER3, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_CHAPTER4))   PhasingHandler::AddPhase(player, PHASE_CHAPTER4, true);
                    if (!player->GetPhaseShift().HasPhase(PHASE_EBON_HOLD))  PhasingHandler::AddPhase(player, PHASE_EBON_HOLD, true);
                }
            }
        }
    }
};

void AddSC_the_scarlet_enclave()
{
    new npc_valkyr_battle_maiden();
    new phasehandler_ebon_hold();
}
