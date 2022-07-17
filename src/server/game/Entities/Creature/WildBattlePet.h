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

#ifndef _WILDBATTLEPET_HEADER_
#define _WILDBATTLEPET_HEADER_

#include "Common.h"
#include "Timer.h"
#include "PetBattle.h"
#include "BattlePet.h"

#define WILDBATTLEPETMGR_UPDATE_INTERVAL 6000
#define WILDBATTLEPET_RESPAWN_WHEN_NOT_DEFEATED 10

class Creature;

struct WildBattlePetPool
{
    std::set<Creature*> ToBeReplaced;
    std::set<Creature*> Replaced;
    std::map<ObjectGuid, ObjectGuid> ReplacedRelation;
};

struct WildPetPoolTemplate
{
    uint32 Species{};
    uint32 BattlePetEntry{};
    uint32 CreatureEntry{};
    uint32 Max{};
    uint32 RespawnTime{};
    uint32 MinLevel{};
    uint32 MaxLevel{};
};

class WildBattlePetZonePools
{
public:
    void LoadPoolTemplate(Field* fields);
    std::map<uint32, WildPetPoolTemplate> m_Templates;
};

class WildBattlePetMgr
{
public:
    static WildBattlePetMgr* Instance();

    WildBattlePetMgr();

    void Load();

    void Populate(WildPetPoolTemplate* wTemplate, WildBattlePetPool* pTemplate);
    void Depopulate(WildBattlePetPool* pTemplate);

    void ReplaceCreature(Creature* creature, WildPetPoolTemplate* wTemplate, WildBattlePetPool* pTemplate);
    void EnableWildBattle(Creature* creature);
    void UnreplaceCreature(Creature* creature);

    bool IsWildPet(Creature* creature);
    std::shared_ptr<BattlePetInstance> GetWildBattlePet(Creature* creature);

    void EnterInBattle(Creature* creature);
    void LeaveBattle(Creature* creature, bool p_Defeated);

    WildPetPoolTemplate* GetWildPetTemplate(uint32 mapId, uint32 zoneId, uint32 entry);
    bool IsBattlePet(uint32 entry);

private:
    ///        map              zone          pools
    std::vector<std::map<uint32, WildBattlePetZonePools>> m_PoolsByMap;
    std::set<uint32> m_battlePetSetEntry;
};

#define sWildBattlePetMgr WildBattlePetMgr::Instance()


#endif
