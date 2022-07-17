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

/*
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_pri_".
 */

#include "ScriptMgr.h"
#include "PassiveAI.h"
#include "PetAI.h"
#include "ScriptedCreature.h"

enum PriestSpells
{
    SPELL_PRIEST_GLYPH_OF_SHADOWFIEND       = 58228,
    SPELL_PRIEST_SHADOWFIEND_DEATH          = 57989,
    SPELL_PRIEST_LIGHTWELL_CHARGES          = 59907
};

class npc_pet_pri_lightwell : public CreatureScript
{
    public:
        npc_pet_pri_lightwell() : CreatureScript("npc_pet_pri_lightwell") { }

        struct npc_pet_pri_lightwellAI : public PassiveAI
        {
            npc_pet_pri_lightwellAI(Creature* creature) : PassiveAI(creature)
            {
                DoCast(me, SPELL_PRIEST_LIGHTWELL_CHARGES, false);
            }

            void EnterEvadeMode(EvadeReason /*why*/) override
            {
                if (!me->IsAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_pri_lightwellAI(creature);
        }
};

//19668
struct npc_pet_pri_shadowfiend : public PetAI
{
    npc_pet_pri_shadowfiend(Creature* creature) : PetAI(creature) { }

    bool CanAIAttack(Unit const* target) const override
    {
        if (!target)
            return false;
        Unit* owner = me->GetOwner();
        if (owner && !target->IsInCombatWith(owner))
            return false;

        return PetAI::CanAIAttack(target);
    }

     void IsSummonedBy(Unit* summoner) override
     {
        if (summoner->HasAura(SPELL_PRIEST_GLYPH_OF_SHADOWFIEND))
            DoCastAOE(SPELL_PRIEST_SHADOWFIEND_DEATH);

         me->InitCharmInfo();
         me->SetReactState(REACT_DEFENSIVE);
         if (Unit* target = summoner->ToPlayer()->GetSelectedUnit())
             me->AI()->AttackStart(target);
     }
};

void AddSC_priest_pet_scripts()
{
    new npc_pet_pri_lightwell();
    RegisterCreatureAI(npc_pet_pri_shadowfiend);
}
