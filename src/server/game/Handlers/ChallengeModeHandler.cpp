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

#include "ChallengeModePackets.h"
#include "InstanceScript.h"
#include "Item.h"
#include "Log.h"
#include "Player.h"
#include "WorldSession.h"
#include "ChallengeModeMgr.h"

void WorldSession::HandleRequestLeaders(WorldPackets::ChallengeMode::RequestLeaders& packet)
{
    WorldPackets::ChallengeMode::RequestLeadersResult result;
    result.MapID = packet.MapId;
    result.ChallengeID = packet.ChallengeID;

    result.LastGuildUpdate = time(nullptr);
    result.LastRealmUpdate = time(nullptr);

    if (auto bestGuild = sChallengeModeMgr->BestGuildChallenge(_player->GetGuildId(), packet.ChallengeID))
    {
        for (auto itr = bestGuild->member.begin(); itr != bestGuild->member.end(); ++itr)
        {
            WorldPackets::ChallengeMode::ModeAttempt guildLeaders;
            guildLeaders.InstanceRealmAddress = GetVirtualRealmAddress();
            guildLeaders.AttemptID = bestGuild->ID;
            guildLeaders.CompletionTime = bestGuild->RecordTime;
            guildLeaders.CompletionDate = bestGuild->Date;
            guildLeaders.MedalEarned = bestGuild->ChallengeLevel;

            for (auto const& v : bestGuild->member)
            {
                WorldPackets::ChallengeMode::ModeAttempt::Member memberData;
                memberData.VirtualRealmAddress = GetVirtualRealmAddress();
                memberData.NativeRealmAddress = GetVirtualRealmAddress();
                memberData.Guid = v.guid;
                memberData.SpecializationID = v.specId;
                guildLeaders.Members.emplace_back(memberData);
            }

            result.GuildLeaders.emplace_back(guildLeaders);
        }
    }

    if (ChallengeData* bestServer = sChallengeModeMgr->BestServerChallenge(packet.ChallengeID))
    {
        WorldPackets::ChallengeMode::ModeAttempt realmLeaders;
        realmLeaders.InstanceRealmAddress = GetVirtualRealmAddress();
        realmLeaders.AttemptID = bestServer->ID;
        realmLeaders.CompletionTime = bestServer->RecordTime;
        realmLeaders.CompletionDate = bestServer->Date;
        realmLeaders.MedalEarned = bestServer->ChallengeLevel;

        for (auto const& v : bestServer->member)
        {
            WorldPackets::ChallengeMode::ModeAttempt::Member memberData;
            memberData.VirtualRealmAddress = GetVirtualRealmAddress();
            memberData.NativeRealmAddress = GetVirtualRealmAddress();
            memberData.Guid = v.guid;
            memberData.SpecializationID = v.specId;
            realmLeaders.Members.emplace_back(memberData);
        }
        result.RealmLeaders.push_back(realmLeaders);
    }

    SendPacket(result.Write());
}

void WorldSession::HandleGetChallengeModeRewards(WorldPackets::ChallengeMode::Misc& /*packet*/)
{
   // WorldPackets::ChallengeMode::Rewards rewards;
    //SendPacket(rewards.Write());
}

void WorldSession::HandleChallengeModeRequestMapStats(WorldPackets::ChallengeMode::Misc& /*packet*/)
{
    WorldPackets::ChallengeMode::AllMapStats stats;
    if (ChallengeByMap* last = sChallengeModeMgr->LastForMember(_player->GetGUID()))
    {
        for (auto const& v : *last)
        {
            WorldPackets::ChallengeMode::ChallengeModeMap modeMap;
            modeMap.ChallengeID = v.second->ChallengeID;
            modeMap.BestMedalDate = v.second->Date;
            modeMap.MapId = v.second->MapID;
            modeMap.CompletedChallengeLevel = v.second->ChallengeLevel;

            modeMap.LastCompletionMilliseconds = v.second->RecordTime;
            if (ChallengeData* _lastData = sChallengeModeMgr->BestForMemberMap(_player->GetGUID(), v.second->ChallengeID))
                modeMap.BestCompletionMilliseconds = _lastData->RecordTime;
            else
                modeMap.BestCompletionMilliseconds = v.second->RecordTime;

            modeMap.Affixes = v.second->Affixes;

            for (auto const& z : v.second->member)
                modeMap.BestSpecID.push_back(z.specId);

            stats.ChallengeModeMaps.push_back(modeMap);
        }
    }

    SendPacket(stats.Write());
}

void WorldSession::HandleChallengeModeStart(WorldPackets::ChallengeMode::StartRequest& start)
{
    GameObject* object = _player->GetGameObjectIfCanInteractWith(start.GobGUID, GAMEOBJECT_TYPE_KEYSTONE_RECEPTACLE);
    if (!object)
    {
        TC_LOG_DEBUG("network", "WORLD: HandleChallengeModeStart - %s not found or you can not interact with it.", start.GobGUID.ToString().c_str());
        return;
    }

    Item* key = _player->GetItemByPos(start.Bag, start.Slot);
    if (!key)
    {
        TC_LOG_DEBUG("network", "WORLD: HandleChallengeModeStart - item in Bag %u and Slot %u not found.", start.Bag, start.Slot);
        return;
    }

    if (key->GetTemplate()->GetClass() != ITEM_CLASS_REAGENT || key->GetTemplate()->GetSubClass() != ITEM_SUBCLASS_KEYSTONE)
    {
        TC_LOG_DEBUG("network", "WORLD: HandleChallengeModeStart - Tried to start a challenge with item %s which have class %u and subclass %u.",
            key->GetGUID().ToString().c_str(),
            key->GetTemplate()->GetClass(),
            key->GetTemplate()->GetSubClass());
        return;
    }

    uint32 challengeModeId      = key->GetModifier(ITEM_MODIFIER_CHALLENGE_MAP_CHALLENGE_MODE_ID);
    uint32 challengeModeLevel   = key->GetModifier(ITEM_MODIFIER_CHALLENGE_KEYSTONE_LEVEL);

    MapChallengeModeEntry const* entry = sMapChallengeModeStore.LookupEntry(challengeModeId);
    if (!entry || !challengeModeLevel || entry->MapID != _player->GetMapId())
    {
        TC_LOG_DEBUG("network", "WORLD: HandleChallengeModeStart - Tried to start a challenge with wrong challengeModeId %u and level %u.", challengeModeId, challengeModeLevel);
        return;
    }

    if (InstanceScript* instanceScript = _player->GetInstanceScript())
        instanceScript->StartChallengeMode(challengeModeLevel);

    // Blizzard do not delete the key at challenge start, will require mort research
    _player->DestroyItem(start.Bag, start.Slot, true);
}

void WorldSession::HandleChallengeModeRequestMapStatsOpcode(WorldPackets::ChallengeMode::RequestMapStats& request)
{
    uint32 ChallengeModeMapCount = _player->m_CompletedChallenges.size();
    WorldPacket packet(SMSG_MYTHIC_PLUS_ALL_MAP_STATS);
    packet << (uint32)ChallengeModeMapCount;
    if (ChallengeModeMapCount > 0)
    {
        for (auto l_ChallengeData : _player->m_CompletedChallenges)
        {
            CompletedChallenge l_CompletedChallenge = l_ChallengeData.second;

            packet << uint32(l_CompletedChallenge.MapID);
            packet << uint32(l_ChallengeData.first);
            packet << uint32(l_CompletedChallenge.BestCompletion);
            packet << uint32(l_CompletedChallenge.LastCompletion);
            packet << uint32(l_CompletedChallenge.BestMedal);
            packet << uint32(l_CompletedChallenge.BestMedalDate);

            uint32 l_SpecCount = 5;
            packet << uint32(l_SpecCount);

            for (uint32 I = 0; I < 3; ++I)
                packet << uint32(0);

            for (uint32 J = 0; J < l_SpecCount; ++J)
                packet << uint16(0);    ///pecID
        }
    }

    SendPacket(&packet);
}

void WorldSession::SendChallengeModeMapStatsUpdate(uint32 keyID)
{
    WorldPacket packet(6 * 4);

    for (auto l_ChallengeData : _player->m_CompletedChallenges)
    {
        if (l_ChallengeData.first == keyID)
        {
            CompletedChallenge l_CompletedChallenge = l_ChallengeData.second;

            packet << uint32(l_CompletedChallenge.MapID);
            packet << uint32(l_ChallengeData.first);
            packet << uint32(l_CompletedChallenge.BestCompletion);
            packet << uint32(l_CompletedChallenge.LastCompletion);
            packet << uint32(l_CompletedChallenge.BestMedal);
            packet << uint32(l_CompletedChallenge.BestMedalDate);

            uint32 l_SpecCount = 5;
            packet << uint32(l_SpecCount);

            for (uint32 I = 0; I < 3; ++I)
                packet << uint32(0);

            for (uint32 J = 0; J < l_SpecCount; ++J)
                packet << uint16(0);    ///pecID
        }
    }

    SendPacket(&packet);
}


void WorldSession::HandleResetChallengeMode(WorldPackets::ChallengeMode::ResetChallengeMode& /*packet*/)
{
    if (auto const& instanceScript = _player->GetInstanceScript())
        if (instanceScript->instance->isChallenge())
            instanceScript->ResetChallengeMode();
}

