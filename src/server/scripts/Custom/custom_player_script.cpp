/*
* Copyright (C) 2021 BfaCore Reforged
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

#include "Conversation.h"
#include "Creature.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "Chat.h"
#include "SharedDefines.h"
#include "GameEventMgr.h"


class playerscript_recruiter : public PlayerScript
{
public:
    playerscript_recruiter() : PlayerScript("playerscript_recruiter") {}

    void OnLevelChanged(Player* player, uint8 /*oldLevel*/) override
    {
        if (player->getLevel() != MAX_LEVEL)
            return;

        QueryResult result = LoginDatabase.PQuery("SELECT recruiter, recruiter_rewarded FROM account WHERE id = %u", player->GetSession()->GetAccountId());
        if (!result)
            return;

        Field* fields           = result->Fetch();
        uint32 recruiter        = fields[0].GetUInt32();
        bool recruiterRewarded  = fields[1].GetBool();

        if (recruiterRewarded)
            return;

        result = CharacterDatabase.PQuery("SELECT guid, NAME FROM characters WHERE account = %u ORDER BY totaltime DESC LIMIT 1", recruiter);
        if (!result)
            return;

        fields = result->Fetch();
        uint64 recruiterCharacterGUID = fields[0].GetUInt64();

        if (!recruiterCharacterGUID)
            return;

        result = LoginDatabase.PQuery("SELECT COUNT(*) FROM account WHERE recruiter = %u AND recruiter_rewarded = 1", recruiter);
        if (!result)
            return;

        fields = result->Fetch();
        uint32 recruiterRewardCount = fields[0].GetUInt32();
        uint32 rewardItem = 0;

        switch (++recruiterRewardCount)
        {
            case 1: rewardItem = 54860;     break; // X-53 Touring Rocket
            case 2: rewardItem = 37719;     break; // Swift Zhevra
            case 5: rewardItem = 106246;    break; // Emerald Hippogryph
            default: break;
        }

        if (rewardItem)
        {
            CharacterDatabase.PExecute("INSERT INTO character_shop (guid, type, itemId, itemCount) VALUES (" UI64FMTD ", 0, %u, 1)", recruiterCharacterGUID, rewardItem);
            LoginDatabase.PExecute("UPDATE account SET recruiter_rewarded = 1 WHERE id = %u", player->GetSession()->GetAccountId());
        }
    }
};

// TODO : this script is temp fix,
// remove it when legion start quests are properly fixed
class OnLegionArrival : public PlayerScript
{
public:
    OnLegionArrival() : PlayerScript("OnLegionArrival") { }

    enum
    {
        QUEST_THE_LEGION_RETURNS_A = 40519,
        QUEST_THE_LEGION_RETURNS_H = 43926,
        QUEST_BLINK_OF_AN_EYE = 44663,
        QUEST_KHADGARS_DISCOVERY = 44555,

        SPELL_LEGION_LAUNCH_PATCH_QUEST_LAUNCH = 258792,
        SPELL_MAGE_LEARN_GUARDIAN_HALL_TP = 204287,
        SPELL_WAR_LEARN_JUMP_TO_SKYHOLD = 192084,
        SPELL_DRUID_CLASS_HALL_TP = 204874,
        SPELL_CREATE_CLASS_HALL_ALLIANCE = 185506,
        SPELL_CREATE_CLASS_HALL_HORDE = 192191,

        CONVERSATION_KHADGAR_BLINK_OF_EYE = 3827,
    };

    void OnQuestStatusChange(Player* player, uint32 /*questId*/)
    {
        if ((player->IsInAlliance() && player->GetQuestStatus(QUEST_THE_LEGION_RETURNS_A) == QUEST_STATUS_REWARDED) || (player->IsInHorde() && player->GetQuestStatus(QUEST_THE_LEGION_RETURNS_H) == QUEST_STATUS_REWARDED))
        {
            if (player->GetQuestStatus(QUEST_BLINK_OF_AN_EYE) == QUEST_STATUS_NONE)
            {
                Conversation::CreateConversation(CONVERSATION_KHADGAR_BLINK_OF_EYE, player, player->GetPosition(), { player->GetGUID() });

                if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_BLINK_OF_AN_EYE))
                    player->AddQuest(quest, nullptr);
            }
        }
    }

    void OnLogin(Player* player, bool firstLogin) override
    {
        // Can happen in recovery cases
        if (player->getClass()==CLASS_DEMON_HUNTER) // demon hunters legionarrival
        {  
            if ((player->getLevel() >= 100) && (player->getLevel() <= 109))
                if ((player->HasQuest(40373)) || (player->GetQuestStatus(40373) == QUEST_STATUS_REWARDED)) // quest "a new direction"
                    HandleLegionArrival(player);
        }
        else
        {
            if (player->getLevel() >= 100 && firstLogin) // other classes onlegionarrival
                HandleLegionArrival(player);
        }

        if (Map* map = sMapMgr->FindMap(0, 0))
            map->LoadGrid(-11099.8f, -2212.36f);

        if (Map* map = sMapMgr->FindMap(1669, 0))
        {
            map->LoadGrid(459.02f, 1450.02f);
            map->LoadGrid(4682.5f, 9851.57f);
            map->LoadGrid(-2624.08f, 8654.29f);
        }

    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel < 100 && player->getLevel() >= 100)
        {
            if (player->getClass()==CLASS_DEMON_HUNTER) // demon hunters legionarrival
            {  
                if ((player->HasQuest(40373)) || (player->GetQuestStatus(40373) == QUEST_STATUS_REWARDED)) // quest "a new direction"
                    HandleLegionArrival(player);
            }
            else
                HandleLegionArrival(player);
        }
    }

    void HandleLegionArrival(Player* player)
    {
        switch (player->getClass())
        {
        case CLASS_MAGE:
            player->CastSpell(player, SPELL_MAGE_LEARN_GUARDIAN_HALL_TP, true);
            break;
        case CLASS_WARRIOR:
            player->CastSpell(player, SPELL_WAR_LEARN_JUMP_TO_SKYHOLD, true);
            break;
        case CLASS_DRUID:
            player->CastSpell(player, SPELL_DRUID_CLASS_HALL_TP, true);
            break;
        case CLASS_HUNTER:
            player->m_taxi.SetTaximaskNode(1848); // Hunter Class Hall
            break;
        default:
            break;
        }

        ///QUEST_THE_LEGION_RETURNS
        player->CastSpell(player, SPELL_LEGION_LAUNCH_PATCH_QUEST_LAUNCH, true);

        if ((player->IsInAlliance() && player->GetQuestStatus(QUEST_THE_LEGION_RETURNS_A) == QUEST_STATUS_REWARDED) || (player->IsInHorde() && player->GetQuestStatus(QUEST_THE_LEGION_RETURNS_H) == QUEST_STATUS_REWARDED))
        {
            if (player->GetQuestStatus(QUEST_BLINK_OF_AN_EYE) == QUEST_STATUS_NONE)
            {
                Conversation::CreateConversation(CONVERSATION_KHADGAR_BLINK_OF_EYE, player, player->GetPosition(), { player->GetGUID() });

                if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_BLINK_OF_AN_EYE))
                    player->AddQuest(quest, nullptr);
            }
        }

        if (player->GetQuestStatus(QUEST_BLINK_OF_AN_EYE) == QUEST_STATUS_REWARDED)
            HandleGetAfterBlinkOfAnEye(player);
    }

    void OnQuestComplete(Player* player, Quest const* quest)
    {
        /* demon hunters legionarrival */
        if (player->getClass()==CLASS_DEMON_HUNTER)
            if ((quest->GetQuestId() == 40373) && (player->getLevel() >= 100)) // quest "a new direction"
                HandleLegionArrival(player);

        if (quest->GetQuestId() == QUEST_BLINK_OF_AN_EYE)
            HandleGetAfterBlinkOfAnEye(player);
    }

    void HandleGetAfterBlinkOfAnEye(Player* player)
    {
        if (player->GetQuestStatus(QUEST_KHADGARS_DISCOVERY) == QUEST_STATUS_NONE)
            if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_KHADGARS_DISCOVERY))
                player->AddQuest(quest, nullptr);

        if (uint32 artifact_knowledge = sWorld->getIntConfig(CONFIG_CURRENCY_START_ARTIFACT_KNOWLEDGE))
        {
            if (artifact_knowledge > 10)
                player->CompletedAchievement(10852);
            if (artifact_knowledge > 24)
                player->CompletedAchievement(10853);
        }

        player->CastSpell(player, player->IsInAlliance() ? SPELL_CREATE_CLASS_HALL_ALLIANCE : SPELL_CREATE_CLASS_HALL_HORDE, true);
        HandleGetFirstFollower(player);
    }

    void HandleGetFirstFollower(Player* player)
    {
        if (player->GetQuestStatus(QUEST_BLINK_OF_AN_EYE) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(QUEST_BLINK_OF_AN_EYE) == QUEST_STATUS_REWARDED)
        {
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
                player->CastSpell(player, 198182, true);
                break;
            case CLASS_PALADIN:
                player->CastSpell(player, 181009, true);
                break;
            case CLASS_HUNTER:
                player->CastSpell(player, 203376, true);
                break;
            case CLASS_ROGUE:
                player->CastSpell(player, 235468, true);
                break;
            case CLASS_PRIEST:
                player->CastSpell(player, 219764, true);
                break;
            case CLASS_DEATH_KNIGHT:
                player->CastSpell(player, 191521, true);
                break;
            case CLASS_SHAMAN:
                player->CastSpell(player, 211710, true);
                break;
            case CLASS_MAGE:
                player->CastSpell(player, 217305, true);
                break;
            case CLASS_WARLOCK:
                player->CastSpell(player, 201163, true);
                break;
            case CLASS_MONK:
                player->CastSpell(player, 234265, true);
                break;
            case CLASS_DRUID:
                player->CastSpell(player, 210357, true);
                break;
            case CLASS_DEMON_HUNTER:
                player->CastSpell(player, player->IsInAlliance() ? 188249 : 215133, true);
                break;
            default:
                break;
            }
        }
        player->CastSpell(player, 164608, true);
    }
};

// TODO : this script is temp fix
class On110Arrival : public PlayerScript
{
public:
    On110Arrival() : PlayerScript("On110Arrival") { }

    enum
    {
        QUEST_UNITING_THE_ISLES = 43341,
        QUEST_ARMIES_OF_LEGIONFALL = 46730,
        QUEST_WHISPERS_OF_A_FRIGHTENED_WORLD = 46206,

        SPELL_LEGION_7_2_PATCH_QUEST_LAUNCH = 239624,
    };

    void OnLogin(Player* player, bool firstLogin) override
    {
        // Can happen in recovery cases
        if (player->getLevel() >= 110 && firstLogin)
            Handle110Arrival(player);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel < 110 && player->getLevel() >= 110)
            Handle110Arrival(player);
    }

    void Handle110Arrival(Player* player)
    {
        if (player->GetQuestStatus(QUEST_UNITING_THE_ISLES) == QUEST_STATUS_NONE)
            if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_UNITING_THE_ISLES))
                player->AddQuest(quest, nullptr);
        if (player->GetQuestStatus(QUEST_ARMIES_OF_LEGIONFALL) == QUEST_STATUS_NONE)
            player->CastSpell(player, SPELL_LEGION_7_2_PATCH_QUEST_LAUNCH, true);
        //if (player->GetQuestStatus(QUEST_WHISPERS_OF_A_FRIGHTENED_WORLD) == QUEST_STATUS_NONE)
        //    player->CastSpell(player, SPELL_WHISPERS_OF_A_FRIGHTENED_WORLD_PUSH, true);
    }
};

// TODO : this script is temp fix,
// remove it when lordaeron battle is properly fixed
class OnBfaArrival : public PlayerScript
{
public:
    OnBfaArrival() : PlayerScript("OnBfaArrival") { }

    enum
    {
        QUEST_DYING_WORLD_A                 = 52946,
        QUEST_DYING_WORLD_H                 = 53028,

        SPELL_CREATE_WAR_CAMPAIGN_H         = 273381,
        SPELL_CREATE_WAR_CAMPAIGN_A         = 273382,

        CONVERSATION_MAGNI_DYING_WORLD      = 9316,
    };

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (player->getLevel() >= 110)
            HandleBFAStart(player);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel < 110 && player->getLevel() >= 110)
            HandleBFAStart(player);
    }

    void HandleBFAStart(Player* player)
    {
        if (player->GetQuestStatus(QUEST_DYING_WORLD_A) == QUEST_STATUS_NONE &&
            player->GetQuestStatus(QUEST_DYING_WORLD_H) == QUEST_STATUS_NONE)
        {
            player->CastSpell(player, player->IsInAlliance() ? SPELL_CREATE_WAR_CAMPAIGN_A : SPELL_CREATE_WAR_CAMPAIGN_H, true);

            Conversation::CreateConversation(CONVERSATION_MAGNI_DYING_WORLD, player, player->GetPosition(), { player->GetGUID() });

            if (const Quest* quest = sObjectMgr->GetQuestTemplate(player->IsInAlliance() ? QUEST_DYING_WORLD_A : QUEST_DYING_WORLD_H))
                player->AddQuest(quest, nullptr);
        }
    }
};

// TODO : this script is temp fix,
// remove it when bfa starting is properly fixed
class On120Arrival : public PlayerScript
{
public:
    On120Arrival() : PlayerScript("On120Arrival") { }

    int QUEST_WOLFSOFFENSIVE_A = 56031;
    int QUEST_WARCHIEFSORDER_H = 56030;

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel < 120 && player->getLevel() >= 120)
            if (const Quest* quest = sObjectMgr->GetQuestTemplate(player->IsInAlliance() ? QUEST_WOLFSOFFENSIVE_A : QUEST_WARCHIEFSORDER_H))
                player->AddQuest(quest, nullptr);
    }
};

/* Worgen Running Wild spell */
class WorgenRunningWild : public PlayerScript
{
public:
    WorgenRunningWild() : PlayerScript("WorgenRunningWild") { }

    void OnLevelChanged(Player* player, uint8 /*oldLevel*/) override
    {
        if (player->getRace() == RACE_WORGEN && player->getLevel() == 20)
            player->CastSpell(player, SPELL_RUNNING_WILD_LEARN);
    }
};

/* Save all players on logout */
// fixes bug where players are rolled back to previous level on logout
class PlayerSavingOnLogoutFix : public PlayerScript
{
public:
    PlayerSavingOnLogoutFix() : PlayerScript("PlayerSavingOnLogoutFix") { }

    void OnLogout(Player* player) override
    {
        ObjectAccessor::SaveAllPlayers();
    }
};

class PlayerScript_Weekly_Spells : public PlayerScript
{
public:
	PlayerScript_Weekly_Spells() : PlayerScript("PlayerScript_Weekly_Spells") {}

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (!player)
            return;

        if (!sGameEventMgr->IsActiveEvent(130))
        {
            player->RemoveActiveQuest(sObjectMgr->GetQuestTemplate(44175), false);
            player->RemoveAura(225788);
        }
        else
            player->CastSpell(player, 225788, false);

        if (!sGameEventMgr->IsActiveEvent(131))
        {
            player->RemoveActiveQuest(sObjectMgr->GetQuestTemplate(44173), false);
            player->RemoveAura(186403);
        }
        else
            player->CastSpell(player, 186403, false);

        if (!sGameEventMgr->IsActiveEvent(132))
        {
            player->RemoveActiveQuest(sObjectMgr->GetQuestTemplate(44174), false);
            player->RemoveAura(186406);
        }
        else
            player->CastSpell(player, 186406, false);

        if (!sGameEventMgr->IsActiveEvent(133))
        {
            player->RemoveActiveQuest(sObjectMgr->GetQuestTemplate(44172), false);
            player->RemoveAura(186401);
        }
        else
            player->CastSpell(player, 186401, false);

        if (!sGameEventMgr->IsActiveEvent(134))
        {
            player->RemoveActiveQuest(sObjectMgr->GetQuestTemplate(44171), false);
            player->RemoveAura(225787);
        }
        else
            player->CastSpell(player, 225787, false);

    }

    void OnMapChanged(Player* player) override
    {
        if (!player)
            return;

        if (!sGameEventMgr->IsActiveEvent(130))
            player->RemoveAura(225788);
        if (!sGameEventMgr->IsActiveEvent(131))
            player->RemoveAura(186403);
        if (!sGameEventMgr->IsActiveEvent(132))
            player->RemoveAura(186406);
        if (!sGameEventMgr->IsActiveEvent(133))
            player->RemoveAura(186401);
        if (!sGameEventMgr->IsActiveEvent(134))
            player->RemoveAura(225787);

    }
};

class player_level_rewards : public PlayerScript
{
public:
    player_level_rewards() : PlayerScript("player_level_rewards") {}

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel <= 119 && player->getLevel() >= 120)
        { 
            switch (player->getRace()) // Heritage Armor
            {
                case RACE_VOID_ELF:
                    if (player->GetQuestStatus(49928) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(49928))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_LIGHTFORGED_DRAENEI:
                    if (player->GetQuestStatus(49782) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(49782))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_NIGHTBORNE:
                    if (player->GetQuestStatus(49784) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(49784))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_HIGHMOUNTAIN_TAUREN:
                    if (player->GetQuestStatus(49783) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(49783))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_DARK_IRON_DWARF:
                    if (player->GetQuestStatus(51483) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(51483))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_MAGHAR_ORC:
                    if (player->GetQuestStatus(51484) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(51484))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_ZANDALARI_TROLL:
                    if (player->GetQuestStatus(53721) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(53721))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_KUL_TIRAN:
                    if (player->GetQuestStatus(53722) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(53722))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_VULPERA:
                    if (player->GetQuestStatus(58435) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(58435))
                            player->AddQuest(quest, nullptr);
                    break;
                case RACE_MECHAGNOME:
                    if (player->GetQuestStatus(58436) == QUEST_STATUS_NONE)
                        if (const Quest * quest = sObjectMgr->GetQuestTemplate(58436))
                            player->AddQuest(quest, nullptr);
                    break;
                default:
                    break;
            }
        }
    }
};


void AddSC_custom_player_script()
{
    RegisterPlayerScript(playerscript_recruiter);
    //RegisterPlayerScript(OnLegionArrival);          // TEMP FIX! Quest 40519 and 43926 - "legion returns". remove it when legion start quests are properly fixed
    //RegisterPlayerScript(On110Arrival);             // TEMP FIX! Quest 43341 - "uniting the isles".
    RegisterPlayerScript(OnBfaArrival);             // TEMP FIX! remove it when lordaeron battle is properly fixed.
    RegisterPlayerScript(On120Arrival);             // TEMP FIX! remove it when bfa starting is properly fixed.
    RegisterPlayerScript(WorgenRunningWild);
    RegisterPlayerScript(PlayerSavingOnLogoutFix);
	new PlayerScript_Weekly_Spells();
	RegisterPlayerScript(player_level_rewards);

}

