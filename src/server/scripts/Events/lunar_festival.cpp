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

#include "Creature.h"
#include "GameEventMgr.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "Unit.h"

//26375
class spell_lunar_festival_invitation : public SpellScript
{
    PrepareSpellScript(spell_lunar_festival_invitation);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Player* player = caster->ToPlayer())
                player->TeleportTo(1, 7585.24f, -2214.63f, 472.17f, 5.54f);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_lunar_festival_invitation::HandleDummy, EFFECT_0, SPELL_EFFECT_CREATE_ITEM);
    }
};

void AddSC_event_lunar_festival()
{
    RegisterSpellScript(spell_lunar_festival_invitation);
};
