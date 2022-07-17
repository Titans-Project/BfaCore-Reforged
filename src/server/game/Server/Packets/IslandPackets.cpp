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

#include "IslandPackets.h"

WorldPacket const* WorldPackets::Island::IslandOpenNpc::Write()
{
    _worldPacket << QueueNPCGuid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Island::IslandAzeriteXpGain::Write()
{
    _worldPacket << XpGain;
    _worldPacket << PlayerGuid;
    _worldPacket << SourceGuid;
    _worldPacket << SourceID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Island::IslandCompleted::Write()
{
    _worldPacket << MapID;
    _worldPacket << Winner;
    _worldPacket << DisplayInfos.size();
    
    return &_worldPacket;
}

void WorldPackets::Island::IslandOnQueue::Read()
{
    _worldPacket >> QueueNPCGuid;
    _worldPacket >> ActivityID;
}

