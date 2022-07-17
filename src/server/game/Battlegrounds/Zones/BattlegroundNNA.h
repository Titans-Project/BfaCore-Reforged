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

#ifndef __BATTLEGROUNDNNA_H
#define __BATTLEGROUNDNNA_H

#include "Arena.h"

enum BattlegroundNNAObjectTypes
{
    BG_NNA_OBJECT_DOOR_1 = 0,
    BG_NNA_OBJECT_DOOR_2 = 1,
    BG_NNA_OBJECT_BUFF_1 = 2,
    BG_NNA_OBJECT_BUFF_2 = 3,
    BG_NNA_OBJECT_MAX = 4
};

enum BattlegroundNNAObjects
{
    BG_NNA_OBJECT_TYPE_DOOR_1 = 260527,
    BG_NNA_OBJECT_TYPE_DOOR_2 = 260528,
    BG_NNA_OBJECT_TYPE_BUFF_1 = 184663,
    BG_NNA_OBJECT_TYPE_BUFF_2 = 184664
};

class BattlegroundNNA : public Arena
{
public:
    BattlegroundNNA();

    /* inherited from BattlegroundClass */
    void StartingEventCloseDoors() override;
    void StartingEventOpenDoors() override;

    void HandleAreaTrigger(Player* source, uint32 trigger, bool entered) override;
    bool SetupBattleground() override;
    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
};

#endif
