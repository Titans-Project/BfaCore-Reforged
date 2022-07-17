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

#include "Player.h"
#include "ScriptMgr.h"
#include "CombatAI.h"
#include "Creature.h"
#include "CreatureGroups.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Scenario.h"
#include "WorldStatePackets.h"
#include "MiscPackets.h"
#include "IslandPackets.h"
#include "WorldSession.h"

enum sv_events
{

};

enum sv_gos
{

};

enum sv_conversations
{

};

struct scenario_verdant_wilds : public InstanceScript
{
    
}

protected:
    EventMap events;
};

void AddSC_expedition_verdant_wilds()
{
    RegisterInstanceScript(expedition_verdant_wilds, 1882);
}

