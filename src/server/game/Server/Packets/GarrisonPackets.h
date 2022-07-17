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

#ifndef GarrisonPackets_h__
#define GarrisonPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "Position.h"
#include "PacketUtilities.h"
#include <list>
#include <unordered_set>
#include <vector>

struct GarrAbilityEntry;

namespace WorldPackets
{
    namespace Garrison
    {
        class GarrisonCreateResult final : public ServerPacket
        {
        public:
            GarrisonCreateResult() : ServerPacket(SMSG_GARRISON_CREATE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 GarrSiteLevelID = 0;
            uint32 Result = 0;
        };

        class GarrisonDeleteResult final : public ServerPacket
        {
        public:
            GarrisonDeleteResult() : ServerPacket(SMSG_GARRISON_DELETE_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            uint32 Result = 0;
            uint32 GarrSiteID = 0;
        };

        class GetGarrisonInfo final : public ClientPacket
        {
        public:
            GetGarrisonInfo(WorldPacket&& packet) : ClientPacket(CMSG_GET_GARRISON_INFO, std::move(packet)) { }

            void Read() override { }
        };

        class CreateShipment final : public ClientPacket
        {
        public:
            CreateShipment(WorldPacket&& packet) : ClientPacket(CMSG_CREATE_SHIPMENT, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 Count = 0;
        };

        class CreateShipmentResponse final : public ServerPacket
        {
        public:
            CreateShipmentResponse() : ServerPacket(SMSG_CREATE_SHIPMENT_RESPONSE, 8 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 Result = 0;
        };

        struct GarrisonPlotInfo
        {
            uint32 GarrPlotInstanceID = 0;
            TaggedPosition<Position::XYZO> PlotPos;
            uint32 PlotType = 0;
        };

        struct GarrisonBuildingInfo
        {
            uint32 GarrPlotInstanceID = 0;
            uint32 GarrBuildingID = 0;
            time_t TimeBuilt = time_t(0);
            uint32 CurrentGarSpecID = 0;
            time_t TimeSpecCooldown = time_t(2288912640);   // 06/07/1906 18:35:44 - another in the series of magic blizz dates
            bool Active = false;
        };

        struct GarrisonFollower
        {
            uint64 DbID = 0;
            uint32 GarrFollowerID = 0;
            uint32 Quality = 0;
            uint32 FollowerLevel = 0;
            uint32 ItemLevelWeapon = 0;
            uint32 ItemLevelArmor = 0;
            uint32 Xp = 0;
            uint32 Durability = 0;
            uint32 CurrentBuildingID = 0;
            uint32 CurrentMissionID = 0;
            std::list<GarrAbilityEntry const*> AbilityID;
            uint32 ZoneSupportSpellID = 0;
            uint32 FollowerStatus = 0;
            std::string CustomName;
        };

        struct GarrisonMission
        {
            uint64 DbID = 0;
            uint32 MissionRecID = 0;
            time_t OfferTime = time_t(0);
            uint32 OfferDuration = 0;
            time_t StartTime = time_t(2288912640);
            uint32 TravelDuration = 0;
            uint32 MissionDuration = 0;
            uint32 MissionState = 0;
            uint32 SuccessChance = 0;
            uint32 Unknown2 = 0;
        };

        struct GarrisonMissionReward
        {
            int32 ItemID = 0;
            uint32 ItemQuantity = 0;
            int32 CurrencyID = 0;
            uint32 CurrencyQuantity = 0;
            uint32 FollowerXP = 0;
            uint32 BonusAbilityID = 0;
            int32 Unknown = 0;
        };

        struct GarrisonMissionBonusAbility
        {
            uint32 GarrMssnBonusAbilityID = 0;
            time_t StartTime = time_t(0);
        };

        struct GarrisonTalent
        {
            int32 GarrTalentID = 0;
            int32 Rank = 0;
            time_t ResearchStartTime = time_t(0);
            int32 Flags = 0;
        };

        struct GarrisonInfo
        {
            int32 GarrTypeID = 0;
            uint32 GarrSiteID = 0;
            uint32 GarrSiteLevelID = 0;
            uint32 NumFollowerActivationsRemaining = 0;
            uint32 NumMissionsStartedToday = 0;   // might mean something else, but sending 0 here enables follower abilities "Increase success chance of the first mission of the day by %."
            std::vector<GarrisonPlotInfo*> Plots;
            std::vector<GarrisonBuildingInfo const*> Buildings;
            std::vector<GarrisonFollower const*> Followers;
            std::vector<GarrisonMission const*> Missions;
            std::vector<std::vector<GarrisonMissionReward>> MissionRewards;
            std::vector<std::vector<GarrisonMissionReward>> MissionOvermaxRewards;
            std::vector<GarrisonMissionBonusAbility const*> MissionAreaBonuses;
            std::vector<GarrisonTalent> Talents;
            std::vector<bool> CanStartMission;
            std::vector<int32> ArchivedMissions;
        };

        struct FollowerSoftCapInfo
        {
            int32 GarrFollowerTypeID;
            uint32 Count;
        };

        class GetGarrisonInfoResult final : public ServerPacket
        {
        public:
            GetGarrisonInfoResult() : ServerPacket(SMSG_GET_GARRISON_INFO_RESULT) { }

            WorldPacket const* Write() override;

            uint32 FactionIndex = 0;
            std::vector<GarrisonInfo> Garrisons;
            std::vector<FollowerSoftCapInfo> FollowerSoftCaps;
        };

        struct GarrisonRemoteBuildingInfo
        {
            GarrisonRemoteBuildingInfo() : GarrPlotInstanceID(0), GarrBuildingID(0) { }
            GarrisonRemoteBuildingInfo(uint32 plotInstanceId, uint32 buildingId) : GarrPlotInstanceID(plotInstanceId), GarrBuildingID(buildingId) { }

            uint32 GarrPlotInstanceID;
            uint32 GarrBuildingID;
        };

        struct GarrisonRemoteSiteInfo
        {
            uint32 GarrSiteLevelID = 0;
            std::vector<GarrisonRemoteBuildingInfo> Buildings;
        };

        class GarrisonRemoteInfo final : public ServerPacket
        {
        public:
            GarrisonRemoteInfo() : ServerPacket(SMSG_GARRISON_REMOTE_INFO) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonRemoteSiteInfo> Sites;
        };

        class GarrisonPurchaseBuilding final : public ClientPacket
        {
        public:
            GarrisonPurchaseBuilding(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_PURCHASE_BUILDING, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 BuildingID = 0;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonPlaceBuildingResult final : public ServerPacket
        {
        public:
            GarrisonPlaceBuildingResult() : ServerPacket(SMSG_GARRISON_PLACE_BUILDING_RESULT) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 Result = 0;
            GarrisonBuildingInfo BuildingInfo;
            bool PlayActivationCinematic = false;
        };

        class GarrisonCancelConstruction final : public ClientPacket
        {
        public:
            GarrisonCancelConstruction(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_CANCEL_CONSTRUCTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 PlotInstanceID = 0;
        };

        class GarrisonBuildingRemoved final : public ServerPacket
        {
        public:
            GarrisonBuildingRemoved() : ServerPacket(SMSG_GARRISON_BUILDING_REMOVED, 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 Result = 0;
            uint32 GarrPlotInstanceID = 0;
            uint32 GarrBuildingID = 0;
        };

        class GarrisonLearnBlueprintResult final : public ServerPacket
        {
        public:
            GarrisonLearnBlueprintResult() : ServerPacket(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 BuildingID = 0;
            uint32 Result = 0;
        };

        class GarrisonUnlearnBlueprintResult final : public ServerPacket
        {
        public:
            GarrisonUnlearnBlueprintResult() : ServerPacket(SMSG_GARRISON_UNLEARN_BLUEPRINT_RESULT, 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            uint32 BuildingID = 0;
            uint32 Result = 0;
        };

        class GarrisonRequestBlueprintAndSpecializationData final : public ClientPacket
        {
        public:
            GarrisonRequestBlueprintAndSpecializationData(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonCheckUpgradeable final : public ClientPacket
        {
        public:
            GarrisonCheckUpgradeable(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_CHECK_UPGRADEABLE, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonCheckUpgradeableResult final : public ServerPacket
        {
        public:
            GarrisonCheckUpgradeableResult(bool upgradeable = false) : ServerPacket(SMSG_GARRISON_IS_UPGRADEABLE_RESULT, 4), IsUpgradeable(upgradeable) { }

            WorldPacket const* Write() override;

            bool IsUpgradeable = false;
        };

        class GarrisonUpgrade final : public ClientPacket
        {
        public:
            GarrisonUpgrade(WorldPacket&& packet) : ClientPacket(CMSG_UPGRADE_GARRISON, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
        };

        class GarrisonUpgradeResult final : public ServerPacket
        {
        public:
            GarrisonUpgradeResult() : ServerPacket(SMSG_GARRISON_UPGRADE_RESULT, 4) { }

            WorldPacket const* Write() override;
        };

        class GarrisonRequestBlueprintAndSpecializationDataResult final : public ServerPacket
        {
        public:
            GarrisonRequestBlueprintAndSpecializationDataResult() : ServerPacket(SMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA_RESULT, 400) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            std::unordered_set<uint32> const* SpecializationsKnown = nullptr;
            std::unordered_set<uint32> const* BlueprintsKnown = nullptr;
        };

        class GarrisonGetBuildingLandmarks final : public ClientPacket
        {
        public:
            GarrisonGetBuildingLandmarks(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_GET_BUILDING_LANDMARKS, std::move(packet)) { }

            void Read() override { }
        };

        class GarrisonOpenMissionNpcClient final : public ClientPacket
        {
        public:
            GarrisonOpenMissionNpcClient(WorldPacket&& packet) : ClientPacket(CMSG_OPEN_MISSION_NPC, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            int32 GarrTypeID = 0;
        };

        class TC_GAME_API ShowAdventureMap final : public ServerPacket
        {
        public:
            ShowAdventureMap(ObjectGuid guid, uint32 uiMapID) : ServerPacket(SMSG_ADVENTURE_MAP_OPEN_NPC, 20), UnitGUID(guid), UiMapID(uiMapID) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            uint32 UiMapID = 0;
        };

        class QueryAdventureMapPOI final : public ClientPacket
        {
        public:
            QueryAdventureMapPOI(WorldPacket&& packet) : ClientPacket(CMSG_ADVENTURE_MAP_POI_QUERY, std::move(packet)) { }

            void Read() override;

            uint32 ID = 0;
        };

        class QueryAdventureMapPOIResponse final : public ServerPacket
        {
        public:
            QueryAdventureMapPOIResponse() : ServerPacket(SMSG_PLAYER_IS_ADVENTURE_MAP_POI_VALID, 5) { }

            WorldPacket const* Write() override;

            uint32 ID = 0;
            bool Active = true;
        };

        struct GarrisonBuildingLandmark
        {
            GarrisonBuildingLandmark() : GarrBuildingPlotInstID(0) { }
            GarrisonBuildingLandmark(uint32 buildingPlotInstId, Position const& pos) : GarrBuildingPlotInstID(buildingPlotInstId), Pos(pos) { }

            uint32 GarrBuildingPlotInstID;
            TaggedPosition<Position::XYZ> Pos;
        };

        class GarrisonBuildingLandmarks final : public ServerPacket
        {
        public:
            GarrisonBuildingLandmarks() : ServerPacket(SMSG_GARRISON_BUILDING_LANDMARKS) { }

            WorldPacket const* Write() override;

            std::vector<GarrisonBuildingLandmark> Landmarks;
        };

        class GarrisonPlotPlaced final : public ServerPacket
        {
        public:
            GarrisonPlotPlaced() : ServerPacket(SMSG_GARRISON_PLOT_PLACED) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            GarrisonPlotInfo* PlotInfo = nullptr;
        };

        class GarrisonPlotRemoved final : public ServerPacket
        {
        public:
            GarrisonPlotRemoved() : ServerPacket(SMSG_GARRISON_PLOT_REMOVED, 4) { }

            WorldPacket const* Write() override;

            uint32 GarrPlotInstanceID = 0;
        };

        class GarrisonAddFollowerResult final : public ServerPacket
        {
        public:
            GarrisonAddFollowerResult() : ServerPacket(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 5 * 4 + 4) { }

            WorldPacket const* Write() override;

            int32 GarrTypeID = 0;
            GarrisonFollower Follower;
            uint32 Result = 0;
        };

        class GarrisonRemoveFollowerResult final : public ServerPacket
        {
        public:
            GarrisonRemoveFollowerResult() : ServerPacket(SMSG_GARRISON_REMOVE_FOLLOWER_RESULT, 8 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 FollowerDBID = 0;
            int32 GarrTypeID = 0;
            uint32 Result = 0;
            uint32 Destroyed = 0;
        };

        class GarrisonBuildingActivated final : public ServerPacket
        {
        public:
            GarrisonBuildingActivated() : ServerPacket(SMSG_GARRISON_BUILDING_ACTIVATED, 4) { }

            WorldPacket const* Write() override;

            uint32 GarrPlotInstanceID = 0;
        };

        class GarrisonOpenMissionNpc final : public ServerPacket
        {
        public:
            GarrisonOpenMissionNpc() : ServerPacket(SMSG_GARRISON_OPEN_MISSION_NPC, 4) { }

            WorldPacket const* Write() override;

            uint32 garrType = 3;
            uint32 result = 0;
            std::vector<uint32 /* dbID */> Missions;
            bool   unk4 = false;
            bool   preventXmlOpenMissionEvent = false;
        };

        class GarrisonAddMissionResult final : public ServerPacket
        {
        public:
            GarrisonAddMissionResult() : ServerPacket(SMSG_GARRISON_ADD_MISSION_RESULT, 4) { }

            uint32 GarrType = 0;
            uint32 Result = 0;
            uint8 State = 0;
            GarrisonMission Mission;

            std::vector<GarrisonMissionReward> Rewards;
            std::vector<GarrisonMissionReward> BonusRewards;

            bool Success = true;

            WorldPacket const* Write() override;
        };

        class GarrisonStartMission final : public ClientPacket
        {
        public:
            GarrisonStartMission(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_START_MISSION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionID = 0;
            std::vector<uint64 /* dbID */> Followers;
        };

        class GarrisonStartMissionResult final : public ServerPacket
        {
        public:
            GarrisonStartMissionResult() : ServerPacket(SMSG_GARRISON_START_MISSION_RESULT, 4) { }

            uint32 Result = 0;
            uint16 FailReason = 1;
            GarrisonMission Mission;
            std::vector<uint64 /* dbID */> Followers;

            WorldPacket const* Write() override;
        };

        class GarrisonSwapBuildings final : public ClientPacket
        {
        public:
            GarrisonSwapBuildings(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_SWAP_BUILDINGS, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 PlotId1 = 0;
            uint32 PlotId2 = 0;
        };

        class GarrisonCompleteMission final : public ClientPacket
        {
        public:
            GarrisonCompleteMission(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_COMPLETE_MISSION, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionID = 0;
        };

        class GarrisonCompleteMissionResult final : public ServerPacket
        {
        public:
            GarrisonCompleteMissionResult() : ServerPacket(SMSG_GARRISON_COMPLETE_MISSION_RESULT, 4) { }

            uint32 Result = 0;
            GarrisonMission Mission;
            std::map<uint64 /*followerDBID*/, uint32 /*unk*/> Followers;
            bool Succeed = false;

            WorldPacket const* Write() override;
        };

        class GarrisonMissionBonusRoll final : public ClientPacket
        {
        public:
            GarrisonMissionBonusRoll(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_MISSION_BONUS_ROLL, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 MissionID = 0;
        };

        class GarrisonMissionBonusRollResult final : public ServerPacket
        {
        public:
            GarrisonMissionBonusRollResult() : ServerPacket(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, 4) { }

            GarrisonMission Mission;
            uint32 Result = 0;

            WorldPacket const* Write() override;
        };

        class GarrisonFollowerChangeXP final : public ServerPacket
        {
        public:
            GarrisonFollowerChangeXP() : ServerPacket(SMSG_GARRISON_FOLLOWER_CHANGED_XP, 4) { }

            uint32 XP = 0;
            uint32 Unk = 0;
            GarrisonFollower OldFollower;
            GarrisonFollower NewFollower;

            WorldPacket const* Write() override;
        };

        class GarrisonGenerateRecruits final : public ClientPacket
        {
        public:
            GarrisonGenerateRecruits(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_GENERATE_RECRUITS, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 TraitID = 0;
            uint32 AbiltyID = 0;
        };

        class GarrisonOpenTalentNpc final : public ServerPacket
        {
        public:
            GarrisonOpenTalentNpc() : ServerPacket(SMSG_GARRISON_OPEN_TALENT_NPC, 4) { }

            ObjectGuid NpcGUID;

            WorldPacket const* Write() override;
        };

        class GarrisonOpenRecruitmentNpc final : public ServerPacket
        {
        public:
            GarrisonOpenRecruitmentNpc() : ServerPacket(SMSG_GARRISON_OPEN_RECRUITMENT_NPC, 4) { }

            ObjectGuid NpcGUID;
            uint32 Unk1 = 0;
            //uint32 Unk2 = 0;
            //uint32 Unk3 = 0;
            std::vector <GarrisonFollower> followers;
            bool CanRecruitFollower = false;
            bool Unk4 = false;
            WorldPacket const* Write() override;
        };

        class GarrisonRecruitsFollower final : public ClientPacket
        {
        public:
            GarrisonRecruitsFollower(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_RECRUIT_FOLLOWER, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 FollowerID = 0;
        };

        class GarrisonRecruitFollowerResult final : public ServerPacket
        {
        public:
            GarrisonRecruitFollowerResult() : ServerPacket(SMSG_GARRISON_RECRUIT_FOLLOWER_RESULT, 64) { }

            uint32 resultID = 0;
            std::vector <GarrisonFollower> followers;

            WorldPacket const* Write() override;
        };

        WorldPacket InsertGarrisonFollower(WorldPacket& worldPacke, WorldPackets::Garrison::GarrisonFollower follower);

        class GarrisonSetFollowerInactive final : public ClientPacket
        {
        public:
            GarrisonSetFollowerInactive(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_SET_FOLLOWER_INACTIVE, std::move(packet)) { }

            void Read() override;

            uint64  followerDBID = 0;
            bool    desActivate = false;
        };

        class GarrisonFollowerChangedStatus final : public ServerPacket
        {
        public:
            GarrisonFollowerChangedStatus() : ServerPacket(SMSG_GARRISON_FOLLOWER_CHANGED_STATUS) { }

            WorldPacket const* Write() override;

            uint32 resultID = 0;
            std::vector <GarrisonFollower> followers;
        };

        class GarrisonRequestShipmentInfo final : public ClientPacket
        {
        public:
            GarrisonRequestShipmentInfo(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REQUEST_SHIPMENT_INFO, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
        };

        class GarrisonOpenShipmentNpcFromGossip final : public ServerPacket
        {
        public:
            GarrisonOpenShipmentNpcFromGossip() : ServerPacket(SMSG_OPEN_SHIPMENT_NPC_FROM_GOSSIP, 4) { }

            ObjectGuid NpcGUID;
            uint32 ShipmentContainerID = 0;
            WorldPacket const* Write() override;
        };

        class GarrisonResearchTalent final : public ClientPacket
        {
        public:
            GarrisonResearchTalent(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_RESEARCH_TALENT, std::move(packet)) { }

            void Read() override;

            uint32 GarrTalentID = 0;
        };

        class GarrisonResearchTalentResult final : public ServerPacket
        {
        public:
            GarrisonResearchTalentResult() : ServerPacket(SMSG_GARRISON_FOLLOWER_CATEGORIES, 4) { }
            uint32 Result = 0;
            uint32 GarrTypeId = 0;
            uint32 GarrTalentID = 0;
            uint32 StartTime = 0;
            uint32 Unk1 = 0;
            WorldPacket const* Write() override;
        };

        class GarrisonRequestClassSpecCategoryInfo final : public ClientPacket
        {
        public:
            GarrisonRequestClassSpecCategoryInfo(WorldPacket&& packet) : ClientPacket(CMSG_GARRISON_REQUEST_CLASS_SPEC_CATEGORY_INFO, std::move(packet)) { }

            void Read() override;

            uint32 GarrFollowerTypeId = 0;
        };

        class GarrisonFollowerCategories final : public ServerPacket
        {
        public:
            GarrisonFollowerCategories() : ServerPacket(SMSG_GARRISON_FOLLOWER_CATEGORIES, 4) { }

            uint32 GarrFollowerTypeId = 0;
            uint32 CategoryInfoCount = 0;
            //FollowerCategoryInfo
            WorldPacket const* Write() override;
        };

        class GarrisonCreateShipment final : public ClientPacket
        {
        public:
            GarrisonCreateShipment(WorldPacket&& packet) : ClientPacket(CMSG_CREATE_SHIPMENT, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            uint32 Count = 1;
        };

        class GarrisonCreateShipmentResponse final : public ServerPacket
        {
        public:
            GarrisonCreateShipmentResponse() : ServerPacket(SMSG_CREATE_SHIPMENT_RESPONSE, 4) { }
            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 Result = 0;
            WorldPacket const* Write() override;
        };
        //TO DO
        class GarrisonGetShipmentsOfTypeResponse final : public ServerPacket
        {
        public:
            GarrisonGetShipmentsOfTypeResponse() : ServerPacket(SMSG_GET_SHIPMENTS_OF_TYPE_RESPONSE, 4) { }
            uint64 ShipmentID = 0;
            uint32 ShipmentRecID = 0;
            uint32 CreationTime;            ///TIME_T
            uint32 ShipmentDuration;        ///TIME_T

            int32 ContainerID;
            //std::vector<Shipment> Shipments;
            WorldPacket const* Write() override;
        };
        //TO DO
        class GarrisonCompleteShipmentResponse final : public ServerPacket
        {
        public:
            GarrisonCompleteShipmentResponse() : ServerPacket(SMSG_COMPLETE_SHIPMENT_RESPONSE, 4) { }
            uint64 ShipmentID = 0;
            uint32 Result = 0;

            WorldPacket const* Write() override;
        };
    }
}

#endif // GarrisonPackets_h__
