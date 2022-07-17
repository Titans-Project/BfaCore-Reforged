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

#ifndef Garrison_h__
#define Garrison_h__

#include "Define.h"
#include "DatabaseEnvFwd.h"
#include "GarrisonAI.h"
#include "GarrisonPackets.h"
#include "GarrisonMgr.h"
#include "Optional.h"
#include <unordered_map>

class ClassHall;
class GameObject;
class GarrisonAI;
class Map;
class Player;
class WarCampaign;
class WodGarrison;
struct GarrSiteLevelEntry;

class TC_GAME_API Garrison
{
public:
    struct Follower
    {
        uint32 GetItemLevel() const;
        bool HasAbility(uint32 garrAbilityId) const;
        void EarnXP(Player* owner, uint32 xp);
        uint32 _EarnXP(uint32 xp);
        uint32 GetRequiredLevelUpXP() const;
        GarrFollowerEntry const* GetEntry() const;
        bool IsShipyard() const;
        bool IsGarrison() const;
        /// Sends follower update
        void SendFollowerUpdate(WorldSession* session) const;
        /// Sends follower update
        void SendFollowerUpdate(Player* player) const;

        WorldPackets::Garrison::GarrisonFollower PacketInfo;
    };

    struct Mission
    {
        WorldPackets::Garrison::GarrisonMission PacketInfo;
        std::vector<WorldPackets::Garrison::GarrisonMissionReward> Rewards;
        std::vector<WorldPackets::Garrison::GarrisonMissionReward> BonusRewards;
        bool CanStartMission = true;
    };

    struct WorkOrder
    {
        uint32 DatabaseID;      ///< Building DB ID
        uint32 PlotInstanceID;  ///< Plot instance ID
        uint32 ShipmentID;      ///< Shipment ID (CharShipment.db2)
        uint32 CreationTime;    ///< Timestamp of creation
        uint32 CompleteTime;    ///< Timestamp of completion
    };

    explicit Garrison(Player* owner);
    virtual ~Garrison() {};

    Player* GetOwner() const { return _owner; }

    virtual bool LoadFromDB();
    virtual void SaveToDB(CharacterDatabaseTransaction& trans);
    void DeleteFromDB(CharacterDatabaseTransaction& trans);
    static void DeleteFromDB(CharacterDatabaseTransaction& trans, ObjectGuid::LowType guid, GarrisonType garrType);

    virtual bool Create(uint32 garrSiteId);
    void Update(uint32 const diff);
    virtual void Delete();

    virtual void Enter();
    virtual void Leave();

    uint32 GetScriptId() const;
    void AI_Initialize();
    void AI_Destroy();
    GarrisonAI* AI() { return _ai.get(); }

    virtual bool IsAllowedArea(uint32 areaID) const { return false; }

    GarrisonFactionIndex GetFaction() const;
    GarrisonType GetType() const { return _garrisonType; }
    void SetSiteLevel(GarrSiteLevelEntry const* siteLevel);
    GarrSiteLevelEntry const* GetSiteLevel() const { return _siteLevel; }

    // Followers
    void AddFollower(uint32 garrFollowerId);
    Follower* GetFollower(uint64 dbId);
    std::unordered_map<uint64 /*dbId*/, Garrison::Follower> const& GetFollowers() const { return _followers; }
    uint32 GetActiveFollowersCount() const;
    uint32 GetAverageFollowerILevel() const;
    uint32 GetMaxFollowerLevel() const;
    /// Change follower activation state
    void ChangeFollowerActivationState(uint64 followerDBID, bool active);
    /// Get num follower activation remaining
    uint32 GetNumFollowerActivationsRemaining() const;

    uint32 GetFollowerActivationLimit() const { return _followerActivationsRemainingToday; }
    void ResetFollowerActivationLimit() { _followerActivationsRemainingToday = 1; }

    template<typename Predicate>
    uint32 CountFollowers(Predicate&& predicate) const
    {
        uint32 count = 0;
        for (auto itr = _followers.begin(); itr != _followers.end(); ++itr)
            if (predicate(itr->second))
                ++count;

        return count;
    }

    // Missions
    void AddMission(uint32 garrMissionId);
    Mission* GetMission(uint64 dbId);
    Mission* GetMissionByID(uint32 ID);
    void DeleteMission(uint64 dbId);
    std::unordered_map<uint64 /*dbId*/, Garrison::Mission> const& GetMissions() const { return _missions; }
    std::vector<Follower*> GetMissionFollowers(uint32 garrMissionId);
    bool HasMission(uint32 garrMissionId) const;
    void StartMission(uint32 garrMissionId, std::vector<uint64 /*DbID*/> Followers);
    void SendStartMissionResult(bool success, Garrison::Mission* mission = nullptr, std::vector<uint64 /*DbID*/>* Followers = nullptr);
    void CompleteMission(uint32 garrMissionId);
    void CalculateMissonBonusRoll(uint32 garrMissionId);
    void RewardMission(Mission* mission, bool withOvermaxReward);
    uint32 GetRandomRewardId() const;

    std::pair<std::vector<GarrMissionEntry const*>, std::vector<double>> GetAvailableMissions() const;
    void GenerateMissions();

    // WorkOrders
    std::unordered_map<uint64 /*dbId*/, Garrison::WorkOrder> const& GetWorkOrders()const { return _workorders; }
    Garrison::WorkOrder* GetWorkOrder(uint64 dbId);
    /// Get building max work order
    uint32 GetBuildingMaxWorkOrder(uint32 plotInstanceID) const;
    /// Get in progress work order count
    uint32 GetWorkOrderCount(uint32 plotInstanceID) const;
    /// Get in progress work order count
    std::vector<WorkOrder> GetBuildingWorkOrders(uint32 plotInstanceID) const;
    /// Start new work order
    uint64 StartWorkOrder(uint32 plotInstanceID, uint32 shipmentID);
    /// Delete work order
    void DeleteWorkOrder(uint64 dbId);

    bool IsWodGarrison() const { return GetType() == GARRISON_TYPE_GARRISON; }
    WodGarrison* ToWodGarrison() { if (IsWodGarrison()) return reinterpret_cast<WodGarrison*>(this); else return nullptr; }
    WodGarrison const* ToWodGarrison() const { if (IsWodGarrison()) return reinterpret_cast<WodGarrison const*>(this); else return nullptr; }

    bool IsClassHall() const { return GetType() == GARRISON_TYPE_CLASS_HALL; }
    ClassHall* ToClassHall() { if (IsClassHall()) return reinterpret_cast<ClassHall*>(this); else return nullptr; }
    ClassHall const* ToClassHall() const { if (IsClassHall()) return reinterpret_cast<ClassHall const*>(this); else return nullptr; }

    bool IsWarCampaign() const { return GetType() == GARRISON_TYPE_WAR_CAMPAIGN; }
    WarCampaign* ToWarCampaign() { if (IsWarCampaign()) return reinterpret_cast<WarCampaign*>(this); else return nullptr; }
    WarCampaign const* ToWarCampaign() const { if (IsWarCampaign()) return reinterpret_cast<WarCampaign const*>(this); else return nullptr; }

protected:
    GarrisonType _garrisonType;

    Map* FindMap() const;

    Player* _owner;
    GarrSiteLevelEntry const* _siteLevel;
    std::unique_ptr<GarrisonAI> _ai;
    IntervalTimer _timers[GUPDATE_COUNT];

    std::unordered_map<uint64 /*dbId*/, Garrison::Follower> _followers;
    std::unordered_set<uint32> _followerIds;
    uint32 _followerActivationsRemainingToday;
    uint32 m_NumFollowerActivation;
    uint32 m_NumFollowerActivationRegenTimestamp;

    std::unordered_map<uint64 /*dbId*/, Garrison::Mission> _missions;
    std::vector<Garrison::Mission> m_Missions;
    std::unordered_set<uint32> _missionIds;
    std::unordered_map<uint64 /*dbId*/, Garrison::WorkOrder> _workorders;
    std::unordered_set<uint32> _workorderIds;
};

#endif // Garrison_h__
