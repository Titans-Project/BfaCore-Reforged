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
#pragma once

#include "LFGList.h"

class LFGListMgr
{
    public:
        static LFGListMgr* instance();

        LFGListMgr();

        bool Insert(LFGListEntry* lfgEntry, Player* requester);
        bool CanInsert(LFGListEntry const* lfgEntry, Player* requester, bool sendError = false) const;
        bool IsEligibleForQueue(Player* player) const;
        bool IsGroupQueued(Group const* group) const;
        void SendLFGListStatusUpdate(LFGListEntry* lfgEntry, WorldSession* worldSession = nullptr, bool listed = true, LFGListStatus debugStatus = LFGListStatus::None);
        bool Remove(ObjectGuid::LowType Guid, Player* requester = nullptr, bool disband = true);
        void PlayerAddedToGroup(Player* player, Group* group);
        void PlayerRemoveFromGroup(Player* player, Group* group);
        std::list<LFGListEntry const*> GetFilteredList(uint32 activityCategory, uint32 activitySubCategory, std::string filterString, Player* player);
        LFGListEntry* GetEntrybyGuid(ObjectGuid::LowType Guid);
        LFGListEntry* GetEntryByApplicant(WorldPackets::LFG::RideTicket applicant);
        void OnPlayerApplyForGroup(Player* player, WorldPackets::LFG::RideTicket const* applicationTicket, uint32 activityID, std::string comment, uint8 role);
        void ChangeApplicantStatus(LFGListEntry::LFGListApplicationEntry* application, LFGListApplicationStatus status, bool notify = true);
        LFGListEntry::LFGListApplicationEntry* GetApplicationByID(uint32 id);
        void Update(uint32 const diff);
        void RemovePlayerDueToLogout(uint32 guidLow);
        void OnPlayerLogin(Player* player);
        LFGListStatus CanQueueFor(LFGListEntry* entry, Player* requestingPlayer, bool apply = true);
        void RemoveAllApplicationsByPlayer(uint32 playerGUID, bool notify = false);
        bool IsActivityPvP(GroupFinderActivityEntry const* activity) const;
        float GetPlayerItemLevelForActivity(GroupFinderActivityEntry const* activity, Player* player) const;
        uint8 GetApplicationCountByPlayer(ObjectGuid::LowType guid) const;
        float GetLowestItemLevelInGroup(LFGListEntry* entry) const;
        uint8 GetMemeberCountInGroupIncludingInvite(LFGListEntry* entry);
        uint8 CountEntryApplicationsWithStatus(LFGListEntry* entry, LFGListApplicationStatus status);
        void AutoInviteApplicantsIfPossible(LFGListEntry* entry);

        void SendLfgListJoinResult(LFGListEntry const* entry, LFGListStatus status, Player* player) const;
        void SendLfgListApplyForGroupResult(LFGListEntry const* lfgEntry, LFGListEntry::LFGListApplicationEntry const* application, Player* player);
    private:
        std::map < ObjectGuid::LowType, LFGListEntry* > _lfgListQueue;
        std::recursive_mutex m_lock;
};

#define sLFGListMgr LFGListMgr::instance()
