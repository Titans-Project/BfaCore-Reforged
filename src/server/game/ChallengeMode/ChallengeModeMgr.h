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

#ifndef _CHALLENGEMODEMGR_H
#define _CHALLENGEMODEMGR_H
struct ChallengeMember
{
    ObjectGuid guid;
    uint16 specId;
    uint32 Date;                    /// time when recorde done
    uint32 ChallengeLevel;          /// 2-15 but blizzard store it as uint32? rly?
    uint32 ChestID;

    bool operator <(const ChallengeMember& i) const;
    bool operator ==(const ChallengeMember& i) const;
};
typedef std::set<ChallengeMember> ChallengeMemberList;

struct ChallengeData
{
    std::array<uint32, 3> Affixes;  /// key modifiers
    ObjectGuid::LowType GuildID;    /// is it guild group
    ObjectGuid::LowType ID;         /// challenge id
    uint32 RecordTime;              /// time taken for complite challenge
    uint32 Date;                    /// time when recorde done
    uint32 ChallengeLevel;          /// 2-15 but blizzard store it as uint32? rly?
    uint32 ChestID;
    uint16 MapID;
    uint16 ChallengeID;
    uint8  TimerLevel;              /// like 0 - not in time; 1-2-3 'timer deal' by db2 data 1-2-3 chests

    ChallengeMemberList member;
};

struct OploteLoot
{
    ObjectGuid guid;
    uint32 Date;
    uint32 ChallengeLevel;
    std::set<uint32> chestListID;
    bool needSave = true;
};

typedef std::unordered_map<uint16 /*ChallengeID*/, ChallengeData*> ChallengeByMap;
typedef std::unordered_map<ObjectGuid::LowType /*ID*/, ChallengeData*> ChallengeMap;
typedef std::unordered_map<ObjectGuid /*MemberGUID*/, ChallengeByMap> ChallengesOfMember;
typedef std::unordered_map<ObjectGuid::LowType /*guild*/, ChallengeByMap> GuildBestRecord;
typedef std::unordered_map<ObjectGuid /*MemberGUID*/, std::set<ChallengeData*>> ChallengeWeekListMap;
typedef std::unordered_map<ObjectGuid /*MemberGUID*/, OploteLoot> OploteLootMap;

struct MapChallengeModeEntry;


static const std::list<uint32> ChallengeChestList = { 282736,282737,288642,288644,290544,290621,290758,290759,290761,282735};

class TC_GAME_API ChallengeModeMgr
{
public:
    static ChallengeModeMgr* instance();
    InstanceMap* instances;
    MapChallengeModeEntry const* GetMapChallengeModeEntry(uint32 mapId);
    void ResetGo();
    ChallengeData* BestServerChallenge(uint16 ChallengeID);
    ChallengeData* BestGuildChallenge(ObjectGuid::LowType const& guildId, uint16 ChallengeID);
    ChallengeByMap* BestForMember(ObjectGuid const& guid);
    ChallengeByMap* LastForMember(ObjectGuid const& guid);
    ChallengeData* LastForMemberMap(ObjectGuid const& guid, uint32 ChallengeID);
    ChallengeData* BestForMemberMap(ObjectGuid const& guid, uint32 ChallengeID);
    void SendChallengeModeMapStatsUpdate(Player* player);
    bool CheckBestMemberMapId(ObjectGuid const& guid, ChallengeData* challengeData);
    void SaveChallengeToDB(ChallengeData const* challengeData);
   
    void CheckBestMapId(ChallengeData* challengeData);
    InstanceScript* GetInstanceScript() const;
    void SetChallengeMapData(ObjectGuid::LowType const& ID, ChallengeData* data);
    void CheckBestGuildMapId(ChallengeData* challengeData);
    uint32 GetDamageMultiplier(uint8 challengeLevel);
    uint32 GetHealthMultiplier(uint8 challengeLevel);
    void HitTimer();
    bool HasOploteLoot(ObjectGuid const& guid);
    OploteLoot* GetOploteLoot(ObjectGuid const& guid);
    static uint32 GetCAForLoot(ChallengeModeMgr* const challenge, uint32 goEntry);
    static uint32 GetBigCAForLoot(ChallengeModeMgr* const challenge, uint32 goEntry, uint32& count);
    static uint32 GetCAForOplote(uint32 challengeLevel);
    static uint32 GetBigCAForOplote(uint32 challengeLevel, uint32& count);
    uint32 GetChallengeLevel() const;
    uint32 GetRandomChallengeId(uint32 flags = 4);
    static float GetChanceItem(uint8 mode, uint32 challengeLevel);
    uint32 GetChallengeTimer();
    std::vector<int32> GetBonusListIdsForRewards(uint32 baseItemIlevel, uint8 challengeLevel);
    void Reward(Player* player, uint8 challengeLevel);
    GuidUnorderedSet _challengers;
    InstanceScript* _instanceScript;
    static bool IsChest(uint32 goEntry);
    void OnGameObjectCreateForScript(GameObject* /*go*/) {}
    void OnGameObjectRemoveForScript(GameObject* /*go*/) {}
    bool _complete;


protected:


    std::map<ObjectGuid, uint8> _countItems;
    uint16 _chestTimers[3];
    uint8 _rewardLevel;
  
    ChallengeMap _challengeMap;
    ChallengesOfMember _lastForMember;
    uint32 _challengeTimer;
    ChallengesOfMember _challengesOfMember;
    ChallengeByMap _bestForMap;
    uint32 _mapID;
    uint32 _challengeLevel;
    Map* _map;
    GuildBestRecord m_GuildBest;
    ChallengeWeekListMap _challengeWeekList;
    OploteLootMap _oploteWeekLoot;
    uint32 _deathCount = 0;
};

#define sChallengeModeMgr ChallengeModeMgr::instance()

#endif
