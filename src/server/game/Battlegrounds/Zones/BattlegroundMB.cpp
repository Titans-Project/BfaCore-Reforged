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

#include "BattlegroundMB.h"
#include "Player.h"
#include "WorldStatePackets.h"

BattlegroundMB::BattlegroundMB()
{
    BgObjects.resize(BG_MB_OBJECT_MAX);
}

void BattlegroundMB::StartingEventCloseDoors()
{
    for (uint32 i = BG_MB_OBJECT_DOOR_1; i <= BG_MB_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundMB::StartingEventOpenDoors()
{
    for (uint32 i = BG_MB_OBJECT_DOOR_1; i <= BG_MB_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for (uint32 i = BG_MB_OBJECT_BUFF_1; i <= BG_MB_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 60);
}

void BattlegroundMB::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
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

void BattlegroundMB::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(0xE1A, 1);
    Arena::FillInitialWorldStates(packet);
}

bool BattlegroundMB::SetupBattleground()
{
    /// Door is custom need snifer
    if (!AddObject(BG_MB_OBJECT_DOOR_1, BG_MB_OBJECT_TYPE_DOOR_1, -1947.074707f, 1355.202515f, 34.755638f, 3.816283f, 0.0f, 0.0f, 0.0f, RESPAWN_IMMEDIATELY)
         || !AddObject(BG_MB_OBJECT_DOOR_2, BG_MB_OBJECT_TYPE_DOOR_2, -1961.725952f, 1354.591675f, 34.430664f, 4.683993f, 0.0f, 0.0f, 0.0f, RESPAWN_IMMEDIATELY)

        /// Buffs
        || !AddObject(BG_MB_OBJECT_BUFF_1, BG_MB_OBJECT_TYPE_BUFF_1, -1962.612305f, 1245.579956f, 34.431129f, 1.652806f, 0.0f, 0.0f, 0.0f, 120)
        || !AddObject(BG_MB_OBJECT_BUFF_2, BG_MB_OBJECT_TYPE_BUFF_2, -1962.612305f, 1245.579956f, 34.431129f, 1.652806f, 0.0f, 0.70068f, -0.0f, 120))
    {
        TC_LOG_ERROR("sql.sql", "BatteGroundMB: Failed to spawn some object!");
        return false;
    }

    return true;
}
