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

#ifndef __BATTLEGROUNDMB_H
#define __BATTLEGROUNDMB_H

#include "Arena.h"

enum BattlegroundMBObjectTypes
{
    BG_MB_OBJECT_DOOR_1 = 0,
    BG_MB_OBJECT_DOOR_2 = 1,
    BG_MB_OBJECT_BUFF_1 = 2,
    BG_MB_OBJECT_BUFF_2 = 3,
    BG_MB_OBJECT_MAX = 4
};

enum BattlegroundMBObjects
{
    BG_MB_OBJECT_TYPE_DOOR_1 = 250431, // Esta es la puerta de la Horda
    BG_MB_OBJECT_TYPE_DOOR_2 = 250430, // Esta es la puerta de la Alianza


    BG_MB_OBJECT_TYPE_BUFF_1 = 184663, // Buff Originales Arenas Mugambala
    BG_MB_OBJECT_TYPE_BUFF_2 = 184664  // Buff 2 Originales Arenas Mugambala
};

class BattlegroundMB : public Arena
{
public:
    BattlegroundMB();

    /* BattlegroundClass */
    void StartingEventCloseDoors() override;
    void StartingEventOpenDoors() override;

    void HandleAreaTrigger(Player* source, uint32 trigger, bool entered) override;
    bool SetupBattleground() override;
    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
};

#endif
