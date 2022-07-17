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

#include "ChallengeModeMgr.h"
#include "Containers.h"
#include "DB2Stores.h"
#include "GameTables.h"
#include "Item.h"
#include "LootPackets.h"
#include "InstanceScript.h"
#include "ChallengeModePackets.h"

ChallengeModeMgr* ChallengeModeMgr::instance()
{
    static ChallengeModeMgr instance;
    return &instance;
}

MapChallengeModeEntry const* ChallengeModeMgr::GetMapChallengeModeEntry(uint32 mapId)
{
    for (uint32 i = 0; i < sMapChallengeModeStore.GetNumRows(); ++i)
        if (MapChallengeModeEntry const* challengeModeEntry = sMapChallengeModeStore.LookupEntry(i))
            if (challengeModeEntry->MapID == mapId)
                return challengeModeEntry;

    return nullptr;
}

bool ChallengeMember::operator<(const ChallengeMember& i) const
{
    return guid.GetCounter() > i.guid.GetCounter();
}

uint32 ChallengeModeMgr::GetChallengeLevel() const
{
    return std::min(_challengeLevel, sWorld->getIntConfig(CONFIG_CHALLENGE_LEVEL_LIMIT));
}

bool ChallengeModeMgr::HasOploteLoot(ObjectGuid const& guid)
{
    return Trinity::Containers::MapGetValuePtr(_oploteWeekLoot, guid);
}

OploteLoot* ChallengeModeMgr::GetOploteLoot(ObjectGuid const& guid)
{
    return Trinity::Containers::MapGetValuePtr(_oploteWeekLoot, guid);
}

uint32 ChallengeModeMgr::GetCAForLoot(ChallengeModeMgr* const challenge, uint32 goEntry)
{
    if (!challenge)
        return 0;

    switch (challenge->_mapID)
    {
    case 1754: //freehold 
    {
        // Lesser Dungeons
        switch (challenge->GetChallengeLevel())
        {
        case 0:
        case 1:
            return 0;
        case 2:
        case 3:
            return 147398;
        case 4:
        case 5:
        case 6:
            return 147399;
        case 7:
        case 8:
        case 9:
            return 147400;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return 147401;
        case 15:
        default:
            return 147719;
        }
    }
    case 1771: //tol dagor             
    case 1841: //underrot              
    case 1762: //king´s rest             
    case 2097: //op mechagon           
    case 1822: //siege of boralus  
    case 1862: //waycrest manor     
    case 1864: //shrine of the storm   
    {
        // Regular Dungeons
        switch (challenge->GetChallengeLevel())
        {
        case 0:
        case 1:
            return 0;
        case 2:
        case 3:
            return 147402;
        case 4:
        case 5:
        case 6:
            return 147403;
        case 7:
        case 8:
        case 9:
            return 147404;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return 147405;
        case 15:
        default:
            return 147718;
        }
    }
    case 1877: //temple of serthraliss 
    case 1594: //the motherlode!!      
    {
        // Greater Dungeons
        switch (challenge->GetChallengeLevel())
        {
        case 0:
        case 1:
            return 0;
        case 2:
        case 3:
            return 147406;
        case 4:
        case 5:
        case 6:
            return 147407;
        case 7:
        case 8:
        case 9:
            return 147408;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return 147409;
        case 15:
        default:
            return 147720;
        }
    }
    default:
        break;
    }

    return 0;
}

uint32 ChallengeModeMgr::GetBigCAForLoot(ChallengeModeMgr* const challenge, uint32 goEntry, uint32& count)
{
    if (!challenge || challenge->GetChallengeLevel() <= 10)
        return 0;

    if (challenge->GetChallengeLevel() >= 15)
        count = challenge->GetChallengeLevel() - 15;
    else
        count = challenge->GetChallengeLevel() - 10;

    switch (challenge->_mapID)
    {
    case 1754: //freehold 
    {
        // Lesser Dungeons
        return 147808; // Lesser Adept's Spoils
    }
    case 1771: //tol dagor            
    case 1841: //underrot             
    case 1762: //king´s rest          
    case 2097: //op mechagon          
    case 1864: //shrine of the storm  
    case 1822: //siege of boralus  
    case 1862: //waycrest manor     
    {
        // Regular Dungeons
        return 147809; // Adept's Spoils
    }
    case 1877: //temple of serthraliss 
    case 1594: //the motherlode!!      
    {
        // Greater Dungeons
        return 147810; // Greater Adept's Spoils
    }
    default:
        break;
    }

    return 0;
}

uint32 ChallengeModeMgr::GetCAForOplote(uint32 challengeLevel)
{
    switch (challengeLevel)
    {
        // Is bug???
    case 0:
    case 1:
        return 0;
    case 2:
    case 3:
        return 147551;
    case 4:
    case 5:
    case 6:
        return 147550;
    case 7:
    case 8:
    case 9:
        return 147549;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        return 147548;
    default: // 15+
        return 147721;
    }
}


uint32 ChallengeModeMgr::GetBigCAForOplote(uint32 challengeLevel, uint32& count)
{
    if (challengeLevel <= 10)
        return 0;

    if (challengeLevel >= 15)
        count = challengeLevel - 15;
    else
        count = challengeLevel - 10;

    return 147819;
}

bool ChallengeMember::operator==(const ChallengeMember& i) const
{
    return guid.GetCounter() == i.guid.GetCounter();
}


uint32 ChallengeModeMgr::GetDamageMultiplier(uint8 challengeLevel)
{
    if (GtChallengeModeDamage const* challengeDamage = sChallengeModeDamage.GetRow(challengeLevel))
        return uint32(100.f * (challengeDamage->Scalar - 1.f));

    return 1;
}

uint32 ChallengeModeMgr::GetHealthMultiplier(uint8 challengeLevel)
{
    if (GtChallengeModeHealth const* challengeHealth = sChallengeModeHealth.GetRow(challengeLevel))
        return uint32(100.f * (challengeHealth->Scalar - 1.f));

    return 1;
}

ChallengeData* ChallengeModeMgr::BestServerChallenge(uint16 ChallengeID)
{
   return Trinity::Containers::MapGetValuePtr(_bestForMap, ChallengeID);
}

InstanceScript* ChallengeModeMgr::GetInstanceScript() const
{
    return _instanceScript;
}

float ChallengeModeMgr::GetChanceItem(uint8 mode, uint32 challengeLevel)
{
    float base_chance = 200.0f;
    float add_chance = 0.0f;

    if (challengeLevel > 10)
        add_chance += (challengeLevel - 10) * 40.0f;

    switch (mode)
    {
    case CHALLENGE_TIMER_LEVEL_3: /// 3 chests + 3 levels
    case CHALLENGE_TIMER_LEVEL_2: /// 2 chests + 2 levels
    case CHALLENGE_TIMER_LEVEL_1: /// 1 chest + 1 level
        base_chance += 100.0f; // 300% is 3 items
        break;
    case CHALLENGE_NOT_IN_TIMER:  /// 0 chest
        base_chance += 0.0f; // 200% is 2 items
        break;
    default:
        break;
    }

    base_chance += add_chance;

    return base_chance;
}

bool ChallengeModeMgr::IsChest(uint32 goEntry)
{
    switch (goEntry)  
    {
    case 282736: // Tol Dagor
    case 282737: // Atal'Dazar
    case 288642: // King's Rest
    case 288644: // Siege of Boralus
    case 290544: // The MOTHERLODE!!
    case 290621: // Waycrest Manor
    case 290758: // Temple of Sertraliss
    case 290759: // The Underrot
    case 290761: // Shrine of the Storm
    case 282735: // Freehold
         return true;
    default:
        break;
    }

    return false;
}

uint32 ChallengeModeMgr::GetChallengeTimer()
{
    if (!_challengeTimer)
        return 0;

    return _challengeTimer / IN_MILLISECONDS;
}

void ChallengeModeMgr::CheckBestMapId(ChallengeData* challengeData)
{
    if (!challengeData)
        return;

    if (!_bestForMap[challengeData->ChallengeID] || _bestForMap[challengeData->ChallengeID]->RecordTime > challengeData->RecordTime)
        _bestForMap[challengeData->ChallengeID] = challengeData;
}

void ChallengeModeMgr::CheckBestGuildMapId(ChallengeData* challengeData)
{
    if (!challengeData || !challengeData->GuildID)
        return;

    if (!m_GuildBest[challengeData->GuildID][challengeData->ChallengeID] || m_GuildBest[challengeData->GuildID][challengeData->ChallengeID]->RecordTime > challengeData->RecordTime)
        m_GuildBest[challengeData->GuildID][challengeData->ChallengeID] = challengeData;
}
void ChallengeModeMgr::SetChallengeMapData(ObjectGuid::LowType const& ID, ChallengeData* data)
{
    _challengeMap[ID] = data;
}

typedef std::set<ChallengeMember> ChallengeMemberList;

void ChallengeModeMgr::SendChallengeModeMapStatsUpdate(Player* player)
{
    MapChallengeModeEntry const* mapChallengeModeEntry = GetMapChallengeModeEntry(instances->GetId());
    if (!mapChallengeModeEntry)
        return;

    ChallengeByMap* bestMap = BestForMember(player->GetGUID());
    if (!bestMap)
        return;

    auto itr = bestMap->find(_mapID);
    if (itr == bestMap->end())
        return;

    ChallengeData* best = itr->second;
    if (!best)
        return;

    WorldPackets::ChallengeMode::ChallengeModeMapStatsUpdate update;
    update.Stats.MapId = _mapID;
    update.Stats.BestCompletionMilliseconds = best->RecordTime;
    update.Stats.LastCompletionMilliseconds = _challengeTimer;
    update.Stats.ChallengeID = mapChallengeModeEntry->ID;
    update.Stats.BestMedalDate = best->Date;
    update.Stats.Affixes = best->Affixes;

    ChallengeMemberList members = best->member;
    for (auto const& v : members)
        update.Stats.BestSpecID.push_back(v.specId);

    if (player)
        player->SendDirectMessage(update.Write());
}

void ChallengeModeMgr::SaveChallengeToDB(ChallengeData const* challengeData)
{
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHALLENGE);
    stmt->setUInt32(0, challengeData->ID);
    stmt->setUInt64(1, challengeData->GuildID);
    stmt->setUInt16(2, challengeData->MapID);
    stmt->setUInt32(3, challengeData->RecordTime);
    stmt->setUInt32(4, challengeData->Date);
    stmt->setUInt8(5, challengeData->ChallengeLevel);
    stmt->setUInt8(6, challengeData->TimerLevel);
    std::ostringstream affixesListIDs;
    for (uint16 affixe : challengeData->Affixes)
       if (affixe)
          affixesListIDs << affixe << ' ';
   stmt->setString(7, affixesListIDs.str());
   stmt->setUInt32(8, challengeData->ChestID);
    trans->Append(stmt);

    for (auto const& v : challengeData->member)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHALLENGE_MEMBER);
        stmt->setUInt32(0, challengeData->ID);
        stmt->setUInt64(1, v.guid.GetCounter());
        stmt->setUInt16(2, v.specId);
        stmt->setUInt32(3, v.ChallengeLevel);
        stmt->setUInt32(4, v.Date);
        stmt->setUInt32(5, v.ChestID);
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);
}

bool ChallengeModeMgr::CheckBestMemberMapId(ObjectGuid const& guid, ChallengeData* challengeData)
{
    bool isBest = false;
    if (!_challengesOfMember[guid][challengeData->ChallengeID] || _challengesOfMember[guid][challengeData->ChallengeID]->RecordTime > challengeData->RecordTime)
    {
        _challengesOfMember[guid][challengeData->ChallengeID] = challengeData;
        isBest = true;
    }

    if (!_lastForMember[guid][challengeData->ChallengeID] || _lastForMember[guid][challengeData->ChallengeID]->Date < challengeData->Date)
        _lastForMember[guid][challengeData->ChallengeID] = challengeData;

    _challengeWeekList[guid].insert(challengeData);

    return isBest;
}


void ChallengeModeMgr::HitTimer()
{
    if (GetChallengeTimer() < _chestTimers[2])
        _rewardLevel = CHALLENGE_TIMER_LEVEL_3; /// 3 chests + 3 levels
    else if (GetChallengeTimer() < _chestTimers[1])
        _rewardLevel = CHALLENGE_TIMER_LEVEL_2; /// 2 chests + 2 levels
    else if (GetChallengeTimer() < _chestTimers[0])
        _rewardLevel = CHALLENGE_TIMER_LEVEL_1; /// 1 chest + 1 level
    else
        _rewardLevel = CHALLENGE_NOT_IN_TIMER;

    if (!_map)
        return;

    for (auto const& guid : _challengers)
        _countItems[guid] = 0;

    switch (_rewardLevel)
    {
        case CHALLENGE_TIMER_LEVEL_3: /// 3 chests + 3 levels
        case CHALLENGE_TIMER_LEVEL_2: /// 2 chests + 2 levels
        case CHALLENGE_TIMER_LEVEL_1: /// 1 chest + 1 level
        case CHALLENGE_NOT_IN_TIMER:  /// 0 chest
        {
            if (InstanceScript* script = GetInstanceScript())
            {
                if (GameObject* chest = _map->GetGameObject(script->_challengeChest))
                    chest->SetRespawnTime(7 * DAY);

                float _chance = GetChanceItem(_rewardLevel, _challengeLevel);
                auto countItems = int32(_chance / 100.0f);
                _chance -= countItems * 100.0f;

                if (roll_chance_f(_chance))
                    countItems++;

                while (countItems > 0)
                {
                    auto _tempList = _challengers;
                    Trinity::Containers::RandomResize(_tempList, countItems);

                    for (auto const& guid : _tempList)
                    {
                        countItems--;
                        _countItems[guid] += 1;
                    }
                }
            }
        }
        default:
            break;
    }
}

void ChallengeModeMgr::ResetGo()
{
   // if (InstanceScript* script = GetInstanceScript())
     //   for (ObjectGuid guid : script->_challengeDoorGuids)
           // if (GameObject* challengeDoor = sObjectAccessor->FindGameObject(guid))
             //   challengeDoor->SetGoState(GO_STATE_READY);
   //
   //if (InstanceScript* script = GetInstanceScript())
   //    if (GameObject* challengeOrb = _map->GetGameObject(script->_challengeOrbGuid))
   //    {
   //        challengeOrb->SetGoState(GO_STATE_READY);
   //        challengeOrb->RemoveFlag(GO_FLAG_NODESPAWN);
   //    }
}

ChallengeData* ChallengeModeMgr::BestGuildChallenge(ObjectGuid::LowType const& GuildID, uint16 ChallengeID)
{
    if (!GuildID)
        return nullptr;

    auto itr = m_GuildBest.find(GuildID);
    if (itr == m_GuildBest.end())
        return nullptr;

    return Trinity::Containers::MapGetValuePtr(itr->second, ChallengeID);
}
ChallengeByMap* ChallengeModeMgr::BestForMember(ObjectGuid const& guid)
{
   return Trinity::Containers::MapGetValuePtr(_challengesOfMember, guid);
}

ChallengeByMap* ChallengeModeMgr::LastForMember(ObjectGuid const& guid)
{
    return Trinity::Containers::MapGetValuePtr(_lastForMember, guid);
}

ChallengeData* ChallengeModeMgr::LastForMemberMap(ObjectGuid const& guid, uint32 ChallengeID)
{
    if (ChallengeByMap* _lastResalt = LastForMember(guid))
    {
        auto itr = _lastResalt->find(ChallengeID);
        if (itr != _lastResalt->end())
            return itr->second;
    }

    return nullptr;
}

ChallengeData* ChallengeModeMgr::BestForMemberMap(ObjectGuid const& guid, uint32 ChallengeID)
{
    if (ChallengeByMap* _lastResalt = BestForMember(guid))
    {
        auto itr = _lastResalt->find(ChallengeID);
        if (itr != _lastResalt->end())
            return itr->second;
    }

    return nullptr;
}


uint32 ChallengeModeMgr::GetRandomChallengeId(uint32 flags/* = 4*/)
{
    std::vector<uint32> challenges;

    for (uint32 i = 0; i < sMapChallengeModeStore.GetNumRows(); ++i)
        if (MapChallengeModeEntry const* challengeModeEntry = sMapChallengeModeStore.LookupEntry(i))
            if (challengeModeEntry->Flags & flags &&
                (challengeModeEntry->ID == 244 || 
				 challengeModeEntry->ID == 245 || 
				 challengeModeEntry->ID == 249 ||
				 challengeModeEntry->ID == 252 ||
				 challengeModeEntry->ID == 353 ||
				 challengeModeEntry->ID == 250 ||
				 challengeModeEntry->ID == 247 ||
				 challengeModeEntry->ID == 248 ||
				 challengeModeEntry->ID == 246  )) // Temp fix, only doable dungeons here
                        challenges.push_back(challengeModeEntry->ID);

        if (challenges.empty())
            return 0;
   
        return Trinity::Containers::SelectRandomContainerElement(challenges);
}

std::vector<int32> ChallengeModeMgr::GetBonusListIdsForRewards(uint32 baseItemIlevel, uint8 challengeLevel)
{
    if (challengeLevel < 2)
        return {};

    std::vector<std::pair<int32, uint32>> bonusDescriptionByChallengeLevel =
    {
        { 3410, 5  },   // Mythic 2
        { 3411, 5  },   // Mythic 3
        { 3412, 10 },   // Mythic 4
        { 3413, 15 },   // Mythic 5
        { 3414, 20 },   // Mythic 6
        { 3415, 20 },   // Mythic 7
        { 3416, 25 },   // Mythic 8
        { 3417, 25 },   // Mythic 9
        { 3418, 30 },   // Mythic 10
        { 3509, 35 },   // Mythic 11
        { 3510, 40 },   // Mythic 12
        { 3534, 45 },   // Mythic 13
        { 3535, 50 },   // Mythic 14
        { 3535, 55 },   // Mythic 15
    };

    const uint32 baseMythicIlevel = 885;
    std::pair<int32, uint32> bonusAndDeltaPair = bonusDescriptionByChallengeLevel[challengeLevel < 15 ? (challengeLevel - 2): 13];
    return { bonusAndDeltaPair.first, (int32)sDB2Manager.GetItemBonusListForItemLevelDelta(baseMythicIlevel - baseItemIlevel + bonusAndDeltaPair.second) };
}

void ChallengeModeMgr::Reward(Player* player, uint8 challengeLevel)
{
    if (!GetMapChallengeModeEntry(player->GetMapId()))
        return;

    JournalInstanceEntry const* journalInstance  = sDB2Manager.GetJournalInstanceByMapId(player->GetMapId());
    if (!journalInstance)
        return;

    auto encounters = sDB2Manager.GetJournalEncounterByJournalInstanceId(journalInstance->ID);
    if (!encounters)
        return;

    std::vector<JournalEncounterItemEntry const*> items;
    for (auto encounter : *encounters)
        if (std::vector<JournalEncounterItemEntry const*> const* journalItems = sDB2Manager.GetJournalItemsByEncounter(encounter->ID))
            items.insert(items.end(), journalItems->begin(), journalItems->end());

    if (items.empty())
        return;

    std::vector<ItemTemplate const*> stuffLoots;
    for (JournalEncounterItemEntry const* journalEncounterItem : items)
    {
        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(journalEncounterItem->ItemID);
        if (!itemTemplate)
            continue;

        if (!itemTemplate->IsUsableByLootSpecialization(player, false))
            continue;

        if (itemTemplate->GetInventoryType() != INVTYPE_NON_EQUIP)
            stuffLoots.push_back(itemTemplate);
    }

    ItemTemplate const* randomStuffItem = Trinity::Containers::SelectRandomContainerElement(stuffLoots);
    if (!randomStuffItem)
        return;

    uint32 itemId = randomStuffItem->GetId();
    ItemPosCountVec dest;
    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1);
    if (msg != EQUIP_ERR_OK)
    {
        player->SendEquipError(msg, nullptr, nullptr, itemId);
        return;
    }

    std::vector<int32> bonusListIds = GetBonusListIdsForRewards(randomStuffItem->GetBaseItemLevel(), challengeLevel);
    Item* pItem = player->StoreNewItem(dest, itemId, true, GenerateItemRandomBonusListId(itemId), GuidSet(), ItemContext::NONE, bonusListIds);
    player->SendNewItem(pItem, 1, true, false, true);

    WorldPackets::Loot::DisplayToast displayToast;
    displayToast.EntityId = itemId;
    displayToast.ToastType = TOAST_ITEM;
    displayToast.Quantity = 1;
    displayToast.RandomPropertiesID = pItem->GetItemRandomBonusListId();
    displayToast.ToastMethod = TOAST_METHOD_POPUP;
    displayToast.bonusListIDs = pItem->m_itemData->BonusListIDs;
    player->SendDirectMessage(displayToast.Write());
}
