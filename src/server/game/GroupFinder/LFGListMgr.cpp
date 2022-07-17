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

#include "Containers.h"
#include "Object.h"
#include "LFGListMgr.h"
#include "GroupMgr.h"
#include "LFGPackets.h"
#include "ObjectMgr.h"
#include "LfgListPackets.h"
#include "SocialMgr.h"
#include "SocialPackets.h"

LFGListMgr* LFGListMgr::instance()
{
    static LFGListMgr instance;
    return &instance;
}

LFGListMgr::LFGListMgr()
{
}

/// @TODO: Find more appropriate
bool LFGListMgr::CanInsert(LFGListEntry const* lfgEntry, Player* requester, bool sendError /* = false */) const
{
    if (!lfgEntry->GroupFinderActivityData)
    {
        if (sendError)
            SendLfgListJoinResult(lfgEntry, LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_NO_LFG_LIST_OBJECT, requester);

        return false;
    }

    if (!IsEligibleForQueue(requester) || lfgEntry->ApplicationGroup)
    {
        if (sendError)
            SendLfgListJoinResult(lfgEntry, LFGListStatus::LFG_LIST_STATUS_ERR_ALREADY_USING_LFG_LIST_LIST, requester);

        return false;
    }

    if (GetPlayerItemLevelForActivity(lfgEntry->GroupFinderActivityData, requester) < lfgEntry->ItemLevel)
    {
        if (sendError)
            SendLfgListJoinResult(lfgEntry, LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_INVALID_SLOT, requester);

        return false;
    }

    if (Group* group = requester->GetGroup())
    {
        if ((!group->isRaidGroup() || !group->IsAssistant(requester->GetGUID())) && !group->IsLeader(requester->GetGUID()))
        {
            if (sendError)
                SendLfgListJoinResult(lfgEntry, LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_NO_LFG_LIST_OBJECT, requester);

            return false;
        }
    }

    return true;
}

bool LFGListMgr::Insert(LFGListEntry* lfgEntry, Player* requester)
{
    if (!CanInsert(lfgEntry, requester, true)) {
        return false;
    }
    if (!IsEligibleForQueue(requester))
    {
        SendLfgListJoinResult(lfgEntry, LFGListStatus::LFG_LIST_STATUS_ERR_ALREADY_USING_LFG_LIST_LIST, requester);
        return false;
    }

    auto group = requester->GetGroup();
    if (group && group->isBGGroup())
        group = requester->GetOriginalGroup();

    if (!group)
    {
        group = new Group;
        if (!group->AddLeaderInvite(requester))
        {
            SendLfgListJoinResult(lfgEntry, LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_NO_LFG_LIST_OBJECT, requester);
            delete group;
            return false;
        }

        group->Create(requester);
        sGroupMgr->AddGroup(group);
    }

    std::lock_guard<std::recursive_mutex> _lock(m_lock);
    lfgEntry->ApplicationGroup = group;
    _lfgListQueue[group->GetGUID().GetCounter()] = lfgEntry;
    SendLFGListStatusUpdate(lfgEntry);
    return true;
}

bool LFGListMgr::IsEligibleForQueue(Player* requester) const
{
    if (!requester)
        return false;

    auto group = requester->GetGroup();
    if (group && group->isBGGroup())
        group = requester->GetOriginalGroup();

    return !IsGroupQueued(group);
}

bool LFGListMgr::IsGroupQueued(Group const* group) const
{
    return group && _lfgListQueue.find(group->GetGUID().GetCounter()) != _lfgListQueue.end();
}

void LFGListMgr::SendLFGListStatusUpdate(LFGListEntry* lfgEntry, WorldSession* worldSession /* = nullptr */, bool listed /* = true */, LFGListStatus debugStatus /*= LFGListStatus::None*/)
{
    if (!lfgEntry)
        return;

    WorldPackets::LfgList::LfgListUpdateStatus status;
    if (auto v = lfgEntry->ApplicationGroup)
    {
        status.ApplicationTicket.RequesterGuid = v->GetGUID();
        status.ApplicationTicket.Id = v->GetGUID().GetCounter();
        status.ApplicationTicket.Type = WorldPackets::LFG::RideType::LfgListApplication;
        status.ApplicationTicket.Time = lfgEntry->CreationTime;
    }

    status.ExpirationTime = lfgEntry->Timeout;
    status.Status = AsUnderlyingType(debugStatus != LFGListStatus::None ? debugStatus : LFGListStatus::Joined);
    status.Listed = listed;

    status.Request.ActivityID = lfgEntry->GroupFinderActivityData->ID;
    status.Request.ItemLevel = lfgEntry->ItemLevel;
    status.Request.HonorLevel = lfgEntry->HonorLevel;
    status.Request.GroupName = lfgEntry->GroupName;
    status.Request.Comment = lfgEntry->Comment;
    status.Request.VoiceChat = lfgEntry->VoiceChat;
    status.Request.AutoAccept = lfgEntry->AutoAccept;
    status.Request.QuestID = lfgEntry->QuestID;

    if (worldSession)
    {
        if (auto player = worldSession->GetPlayer())
            player->SendDirectMessage(status.Write());
    }
    else if (lfgEntry->ApplicationGroup)
        lfgEntry->ApplicationGroup->BroadcastPacket(status.Write(), false);
}

bool LFGListMgr::Remove(ObjectGuid::LowType Guid, Player* requester /* = nullptr */, bool disband /* = true */)
{
    auto itr = _lfgListQueue.find(Guid);
    if (itr == _lfgListQueue.end())
        return false;

    auto group = itr->second->ApplicationGroup;
    if (!group)
        return false;

    if (requester && ((!group->isRaidGroup() || !group->IsAssistant(requester->GetGUID())) && !group->IsLeader(requester->GetGUID())))
        return false;

    for (auto itr2 = itr->second->ApplicationsContainer.begin(); itr2 != itr->second->ApplicationsContainer.end();)
    {
        ChangeApplicantStatus(&itr2->second, LFGListApplicationStatus::Cancelled);
        itr2 = itr->second->ApplicationsContainer.begin();
    }

    std::lock_guard<std::recursive_mutex> _lock(m_lock);
    auto entry = itr->second;
    _lfgListQueue.erase(itr);
    SendLFGListStatusUpdate(entry, nullptr, false);
    delete entry;

    if (disband && group->GetMembersCount() < 2)
        group->Disband();

    return true;
}

void LFGListMgr::PlayerAddedToGroup(Player* player, Group* group)
{
    SendLFGListStatusUpdate(GetEntrybyGuid(group->GetGUID().GetCounter()), nullptr, true);
}

void LFGListMgr::PlayerRemoveFromGroup(Player* player, Group* group)
{
    SendLFGListStatusUpdate(GetEntrybyGuid(group->GetGUID().GetCounter()), nullptr, false);
}

std::list<LFGListEntry const*> LFGListMgr::GetFilteredList(uint32 activityCategory, uint32 /*activitySubCategory*/, std::string filterString, Player* player)
{
    std::list<LFGListEntry const*> lfgFiltered;

    for (auto& itr : _lfgListQueue)
    {
        auto listEntry = itr.second;
        if (listEntry->GroupFinderActivityData->GroupFinderCategoryID != activityCategory)
            continue;

        if (filterString.length() && listEntry->GroupName.length())
        {
            auto upperName = listEntry->GroupName;
            std::transform(upperName.begin(), upperName.end(), upperName.begin(), toupper);
            std::transform(filterString.begin(), filterString.end(), filterString.begin(), toupper);

            if (upperName.find(filterString) == std::string::npos)
                continue;
        }

        if (CanQueueFor(itr.second, player, false) != LFGListStatus::None)
            continue;

        lfgFiltered.push_back(listEntry);
    }
    return lfgFiltered;
}

LFGListEntry* LFGListMgr::GetEntrybyGuid(ObjectGuid::LowType Guid)
{
    return Trinity::Containers::MapGetValuePtr(_lfgListQueue, Guid);
}

LFGListEntry* LFGListMgr::GetEntryByApplicant(WorldPackets::LFG::RideTicket applicant)
{
    for (const auto& pair : _lfgListQueue)
        if (auto result = pair.second->GetApplicant(applicant.Id))
            if (result->ApplicationTime == applicant.Time)
                return  pair.second;

    return nullptr;
}

void LFGListMgr::OnPlayerApplyForGroup(Player* player, WorldPackets::LFG::RideTicket const* applicationTicket, uint32 activityID, std::string comment, uint8 role)
{
    if (!sGroupFinderActivityStore[activityID])
        return;

    auto entry = GetEntrybyGuid(applicationTicket->Id);
    if (!entry)
        return;

    LFGListEntry::LFGListApplicationEntry application(player->GetGUID().GetCounter(), entry);
    application.RoleMask = role;
    application.Comment = comment;

    if (entry->ApplicationsContainer.find(application.ID) != entry->ApplicationsContainer.end())
        return;

    entry->ApplicationsContainer.insert(std::make_pair(application.ID, application));

    auto applicationEntry = &entry->ApplicationsContainer.find(application.ID)->second;

    applicationEntry->Status = CanQueueFor(applicationEntry->m_Owner, player);

    if (applicationEntry->Status != LFGListStatus::None)
        ChangeApplicantStatus(applicationEntry, LFGListApplicationStatus::Failed);
    else
    {
        if (entry->AutoAccept)
        {
            if (!entry->ApplicationGroup->isRaidGroup() && GetMemeberCountInGroupIncludingInvite(entry) == 5)
                ChangeApplicantStatus(applicationEntry, LFGListApplicationStatus::Applied); // Handled clientside -- OnAccept = function(self, applicantID) ConvertToRaid(); C_LFGList.InviteApplicant(applicantID) end,
            else
                ChangeApplicantStatus(applicationEntry, LFGListApplicationStatus::Invited);
        }
        else
            ChangeApplicantStatus(applicationEntry, LFGListApplicationStatus::Applied);
    }
}

LFGListEntry::LFGListApplicationEntry* LFGListMgr::GetApplicationByID(uint32 id)
{
    for (auto& group : _lfgListQueue)
        for (auto& applicant : group.second->ApplicationsContainer)
            if (applicant.first == id)
                return &applicant.second;

    return nullptr;
}

void LFGListMgr::ChangeApplicantStatus(LFGListEntry::LFGListApplicationEntry* application, LFGListApplicationStatus status, bool notify /* = true */)
{
    if (!application || application->ApplicationStatus == status)
        return;

    bool remove = false;
    auto player = application->GetPlayer();
    auto const& listEntry = application->m_Owner;

    application->ApplicationStatus = status;

    switch (status)
    {
    case LFGListApplicationStatus::Invited:
        if (!listEntry->ApplicationGroup->isRaidGroup() && GetMemeberCountInGroupIncludingInvite(listEntry) >= 5 || player && CanQueueFor(listEntry, player) != LFGListStatus::None)
            break;
    case LFGListApplicationStatus::Applied:
        application->ResetTimeout();
        listEntry->ResetTimeout();

        if (notify && player)
            SendLfgListApplyForGroupResult(listEntry, application, player);
        break;
    case LFGListApplicationStatus::InviteDeclined:
    case LFGListApplicationStatus::Declined:
    case LFGListApplicationStatus::Cancelled:
    case LFGListApplicationStatus::Timeout:
    case LFGListApplicationStatus::Failed:
    case LFGListApplicationStatus::DeclinedFull:
    case LFGListApplicationStatus::DeclinedDelisted:
        application->Listed = false;
        remove = true;
        break;
    case LFGListApplicationStatus::InviteAccepted:
        if (!listEntry->ApplicationGroup->isRaidGroup() && GetMemeberCountInGroupIncludingInvite(listEntry) >= 5 || CanQueueFor(listEntry, player) != LFGListStatus::None)
            break;

        application->Listed = false;
        remove = true;
        listEntry->ApplicationGroup->AddMember(player);
        listEntry->ApplicationGroup->SetLfgRoles(player->GetGUID(), application->RoleMask);
        break;
    default:
        break;
    }

    listEntry->BroadcastApplicantUpdate(application);

    if (remove)
        listEntry->ApplicationsContainer.erase(listEntry->ApplicationsContainer.find(application->ID));

    AutoInviteApplicantsIfPossible(listEntry);
}

void LFGListMgr::RemoveAllApplicationsByPlayer(uint32 playerGUID, bool notify /* = false */)
{
    //@TODO totally wrong
    while (auto application = GetApplicationByID(playerGUID))
    {
        ChangeApplicantStatus(application, LFGListApplicationStatus::DeclinedDelisted, notify);
    }
}

uint8 LFGListMgr::GetApplicationCountByPlayer(ObjectGuid::LowType guid) const
{
    uint8 counter = 0;
    for (auto& group : _lfgListQueue)
        for (auto& applicant : group.second->ApplicationsContainer)
            if (applicant.second.PlayerGuid == guid)
                ++counter;

    return counter;
}

void LFGListMgr::Update(uint32 const diff)
{
    std::lock_guard<std::recursive_mutex> _lock(m_lock);
    for (auto itr = _lfgListQueue.begin(); itr != _lfgListQueue.end();)
    {
        if (!itr->second) // Prevent crash
        {
            _lfgListQueue.erase(itr);
            itr = _lfgListQueue.begin();
            continue;
        }
        if (!itr->second->Update(diff))
        {
            Remove((*itr).second->GetID());
            itr = _lfgListQueue.begin();
        }
        else
            ++itr;
    }
}

void LFGListMgr::RemovePlayerDueToLogout(uint32 guidLow) ///< This is wrong, but cba to do it other way for now
{
    RemoveAllApplicationsByPlayer(guidLow);
}

void LFGListMgr::OnPlayerLogin(Player* player)
{
    if (auto group = player->GetGroup())
        if (auto entry = GetEntrybyGuid(group->GetGUID().GetCounter()))
            SendLFGListStatusUpdate(entry, player->GetSession());
}

LFGListStatus LFGListMgr::CanQueueFor(LFGListEntry* entry, Player* requestingPlayer, bool apply /* = true */)
{
    if (!requestingPlayer)
        return LFGListStatus::None;

    auto group = entry->ApplicationGroup;
    auto activity = entry->GroupFinderActivityData;
    auto iLvl = GetPlayerItemLevelForActivity(activity, requestingPlayer);

    if ((activity->MinGearLevelSuggestion && iLvl < activity->MinGearLevelSuggestion) || iLvl < entry->ItemLevel)
        return LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_INVALID_SLOT;   ///< Same as above, filtered out

    if ((activity->MaxPlayers && static_cast<int32>(group->GetMembersCount()) >= activity->MaxPlayers) || group->GetMembersCount() >= 40)
        return LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_TOO_MANY_MEMBERS;

    if (requestingPlayer->getLevel() < activity->MinLevel || (activity->MaxLevelSuggestion && requestingPlayer->getLevel() > activity->MaxLevelSuggestion))
        return LFGListStatus::LFG_LIST_STATUS_ERR_LFG_LIST_INVALID_SLOT;   ///< Filtered out

    if (apply)
        return LFGListStatus::None;

    return LFGListStatus::None;
}

bool LFGListMgr::IsActivityPvP(GroupFinderActivityEntry const* activity) const
{
    if (!activity)
        return false;

    switch (activity->GroupFinderCategoryID)
    {
    case LFG_LIST_ACTIVITY_CATEGORY_ARENA:
    case LFG_LIST_ACTIVITY_CATEGORY_ARENA_SKIRMISH:
    case LFG_LIST_ACTIVITY_CATEGORY_BATTLEGROUNDS:
    case LFG_LIST_ACTIVITY_CATEGORY_RATED_BATTLEGROUNDS:
    case LFG_LIST_ACTIVITY_CATEGORY_OUTDOOR_PVP:
        return true;
    default:
        return activity->ID == 17;    ///< Custom PvP
    }
}

float LFGListMgr::GetPlayerItemLevelForActivity(GroupFinderActivityEntry const* activity, Player* player) const
{
    if (player == nullptr)
        return 0.0f;

    return player->GetAverageItemLevelEquipped() + (IsActivityPvP(activity) ? PlayerAvgItemLevelOffsets::PLAYER_AVG_ITEM_LEVEL_PVP : PlayerAvgItemLevelOffsets::PLAYER_AVG_ITEM_LEVEL_NON_PVP);
}

float LFGListMgr::GetLowestItemLevelInGroup(LFGListEntry* entry) const
{
    auto minIlvl = 100000.0f;

    for (auto ref = entry->ApplicationGroup->GetFirstMember(); ref != nullptr; ref = ref->next())
        if (auto player = ref->GetSource())
            minIlvl = std::min(minIlvl, GetPlayerItemLevelForActivity(entry->GroupFinderActivityData, player));

    return minIlvl != 100000.0f ? minIlvl : 0.0f;
}

uint8 LFGListMgr::GetMemeberCountInGroupIncludingInvite(LFGListEntry* entry)
{
    return CountEntryApplicationsWithStatus(entry, LFGListApplicationStatus::InviteDeclined) + entry->ApplicationGroup->GetMembersCount();
}

uint8 LFGListMgr::CountEntryApplicationsWithStatus(LFGListEntry* entry, LFGListApplicationStatus status)
{
    return static_cast<uint8>(std::count_if(entry->ApplicationsContainer.begin(), entry->ApplicationsContainer.end(), [&](std::pair<uint32, LFGListEntry::LFGListApplicationEntry> const& itr)
    {
        return itr.second.ApplicationStatus == status;
    }));
}

void LFGListMgr::AutoInviteApplicantsIfPossible(LFGListEntry* entry)
{
    if (!entry->AutoAccept)
        return;

    if (!entry->ApplicationGroup->isRaidGroup() && GetMemeberCountInGroupIncludingInvite(entry) >= 5)
        return;

    for (auto& applicant : entry->ApplicationsContainer)
        if (CanQueueFor(entry, applicant.second.GetPlayer()) == LFGListStatus::None)
            ChangeApplicantStatus(&applicant.second, LFGListApplicationStatus::Invited);
}

void LFGListMgr::SendLfgListJoinResult(LFGListEntry const* entry, LFGListStatus status, Player* player) const
{
    auto group = entry->ApplicationGroup;
    if (!group)
        return;

    WorldPackets::LfgList::LfgListJoinResult result;
    result.ApplicationTicket.RequesterGuid = group->GetGUID();
    result.ApplicationTicket.Id = group->GetGUID().GetCounter();
    result.ApplicationTicket.Type = WorldPackets::LFG::RideType::LfgListApplication;
    result.ApplicationTicket.Time = entry->CreationTime;
    result.Status = AsUnderlyingType(status);
    result.Result = 0;

    player->SendDirectMessage(result.Write());
}

void LFGListMgr::SendLfgListApplyForGroupResult(LFGListEntry const* lfgEntry, LFGListEntry::LFGListApplicationEntry const* application, Player* player)
{
    if (!player)
        return;

    auto group = lfgEntry->ApplicationGroup;
    if (!group)
        return;

    WorldPackets::LfgList::LfgListApplyToGroupResponce responce;

    responce.ApplicantTicket.RequesterGuid = ObjectGuid::Create<HighGuid::Player>(application->PlayerGuid);
    responce.ApplicantTicket.Id = application->ID;
    responce.ApplicantTicket.Type = WorldPackets::LFG::RideType::LfgListApplicant;
    responce.ApplicantTicket.Time = application->ApplicationTime;

    responce.InviteExpireTimer = application->Timeout;
    responce.Status = AsUnderlyingType(LFGListStatus::Joined);
    responce.Role = application->RoleMask;
    responce.ApplicationStatus = AsUnderlyingType(application->ApplicationStatus);

    auto activityID = lfgEntry->GroupFinderActivityData->ID;

    responce.ApplicationTicket.RequesterGuid = group->GetGUID();
    responce.ApplicationTicket.Id = group->GetGUID().GetCounter();
    responce.ApplicationTicket.Type = WorldPackets::LFG::RideType::LfgListApplication;
    responce.ApplicationTicket.Time = lfgEntry->CreationTime;

    responce.SearchResult.ApplicationTicket.RequesterGuid = group->GetGUID();
    responce.SearchResult.ApplicationTicket.Id = group->GetGUID().GetCounter();
    responce.SearchResult.ApplicationTicket.Type = WorldPackets::LFG::RideType::LfgListApplication;
    responce.SearchResult.ApplicationTicket.Time = lfgEntry->CreationTime;
    responce.SearchResult.UnkGuid1 = group->GetLeaderGUID();
    responce.SearchResult.UnkGuid2 = group->GetLeaderGUID();
    responce.SearchResult.UnkGuid3 = group->GetLeaderGUID();
    responce.SearchResult.UnkGuid4 = group->GetLeaderGUID();
    responce.SearchResult.VirtualRealmAddress = GetVirtualRealmAddress();
    responce.SearchResult.CompletedEncounters = 0;
    responce.SearchResult.ResultID = 3;
    responce.SearchResult.Age = lfgEntry->CreationTime;
    responce.SearchResult.ApplicationStatus = AsUnderlyingType(LFGListApplicationStatus::None);
    responce.SearchResult.JoinRequest.ActivityID = activityID;
    responce.SearchResult.JoinRequest.ItemLevel = lfgEntry->ItemLevel;
    responce.SearchResult.JoinRequest.HonorLevel = lfgEntry->HonorLevel;
    responce.SearchResult.JoinRequest.GroupName = lfgEntry->GroupName;
    responce.SearchResult.JoinRequest.Comment = lfgEntry->Comment;
    responce.SearchResult.JoinRequest.VoiceChat = lfgEntry->VoiceChat;
    responce.SearchResult.JoinRequest.AutoAccept = lfgEntry->AutoAccept;
    responce.SearchResult.JoinRequest.QuestID = lfgEntry->QuestID;

    for (auto const& member : group->GetMemberSlots())
    {
        uint8 role = member.roles >= 2 ? std::log2(member.roles) - 1 : member.roles;
        responce.SearchResult.Members.emplace_back(member._class, member.roles);
    }
    for (auto const& member : lfgEntry->ApplicationsContainer)
        if (auto applicant = member.second.GetPlayer())
            responce.SearchResult.Members.emplace_back(applicant->getClass(), member.second.RoleMask);

    player->SendDirectMessage(responce.Write());
}
