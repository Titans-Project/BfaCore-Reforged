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

#include "Battleground.h"
#include "BattlegroundBEG.h"
#include "Player.h"
#include "WorldStatePackets.h"

enum ArenaObjectTypes
{
    OBJECT_DOOR_1 = 0,
    OBJECT_DOOR_2,
    OBJECT_BUFF_1,
    OBJECT_BUFF_2,

    OBJECT_MAX
};

enum ArenaObjects
{
    OBJECT_TYPE_DOOR_1 = 265571,
    OBJECT_TYPE_DOOR_2 = 265569,
    OBJECT_TYPE_BUFF_1 = 184663,
    OBJECT_TYPE_BUFF_2 = 184664,
};

BattlegroundBEG::BattlegroundBEG()
{
    BgObjects.resize(OBJECT_MAX);
}

BattlegroundBEG::~BattlegroundBEG() = default;

void BattlegroundBEG::StartingEventCloseDoors()
{
    for (uint32 i = OBJECT_DOOR_1; i <= OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);

    Arena::StartingEventCloseDoors();
}

void BattlegroundBEG::StartingEventOpenDoors()
{
    for (uint32 i = OBJECT_DOOR_1; i <= OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for (uint32 i = OBJECT_BUFF_1; i <= OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, ARENA_COUNTDOWN_MAX);

    Arena::StartingEventOpenDoors();
}

void BattlegroundBEG::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (trigger)
    {
    case 4536:
    case 4537:
        break;
    default:
        Battleground::HandleAreaTrigger(player, trigger, entered);
        break;
    }
}

void BattlegroundBEG::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(0xE1A, 1);
    Arena::FillInitialWorldStates(packet);
}

bool BattlegroundBEG::SetupBattleground()
{
    // Gates
    if (!AddObject(OBJECT_DOOR_1, OBJECT_TYPE_DOOR_1, 2776.371f, 6055.702f, -3.733995f, 2.217981f, 0.0f, 0.0f, 0.8952494f, 0.4455655f, RESPAWN_IMMEDIATELY)
        ||!AddObject(OBJECT_DOOR_2, OBJECT_TYPE_DOOR_2, 2797.014f, 5953.527f, -4.099238f, 5.410522f, 0.0f, 0.0f, -0.4226179f, 0.9063079f, RESPAWN_IMMEDIATELY)
   // Buffs
        ||!AddObject(OBJECT_BUFF_1, OBJECT_TYPE_BUFF_1, 2815.306f, 5972.267f, -4.452f, 2.24f, 0.0f, 0.0f, 0.40f, 0.91f, RESPAWN_IMMEDIATELY)
        ||!AddObject(OBJECT_BUFF_2, OBJECT_TYPE_BUFF_2, 2761.437f, 6038.106f, -3.49f, 5.57f, 0.0f, 0.0f, 0.40f, 0.91f, RESPAWN_IMMEDIATELY))
    {
        TC_LOG_ERROR("sql.sql", "BatteGroundBEG: Failed to spawn some object!");
        return false;
    }

    return true;
}
