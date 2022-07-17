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

/* ScriptData
SDName: Silithus
SD%Complete: 100
SDCategory: Silithus
EndScriptData */

/* ContentData
A Pawn on the Eternal Board - creatures, gameobjects and defines
quest_a_pawn_on_the_eternal_board
npc_qiraj_war_spawn : Adds that are summoned in the Qiraj gates battle.
npc_anachronos_the_ancient : Creature that controls the event.
npc_anachronos_quest_trigger: controls the spawning of the BG War mobs.
go_crystalline_tear : GameObject that begins the event and hands out quest
TO DO: get correct spell IDs and timings for spells cast upon dragon transformations
TO DO: Dragons should use the HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF) after transformation,
       but for some unknown reason it doesn't work.
go_wind_stone
EndContentData */

#include "ScriptMgr.h"
#include "CreatureAIImpl.h"
#include "Conversation.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "TemporarySummon.h"
#include "Log.h"
#include "Util.h"
#include "PhasingHandler.h"

/*#####
# Quest: A Pawn on the Eternal Board
#####*/

enum EternalBoard
{
    QUEST_A_PAWN_ON_THE_ETERNAL_BOARD = 8519,

    FACTION_HOSTILE                   = 14,
    FACTION_FRIENDLY                  = 35,

    EVENT_AREA_RADIUS                 = 65,     // 65yds
    EVENT_COOLDOWN                    = 500000, // in ms. appears after event completed or failed (should be = Adds despawn time)

    NPC_ANACHRONOS                    = 15381,
    NPC_FANDRAL_STAGHELM              = 15382,
    NPC_ARYGOS                        = 15380,
    NPC_MERITHRA_OF_THE_DREAM         = 15378,
    NPC_CAELESTRASZ                   = 15379,

    ANACHRONOS_SAY_1                  = 0,
    ANACHRONOS_SAY_2                  = 1,
    ANACHRONOS_SAY_3                  = 2,
    ANACHRONOS_SAY_4                  = 3,
    ANACHRONOS_SAY_5                  = 4,
    ANACHRONOS_SAY_6                  = 5,
    ANACHRONOS_SAY_7                  = 6,
    ANACHRONOS_SAY_8                  = 7,
    ANACHRONOS_SAY_9                  = 8,
    ANACHRONOS_SAY_10                 = 9,
    ANACHRONOS_EMOTE_1                = 10,
    ANACHRONOS_EMOTE_2                = 11,
    ANACHRONOS_EMOTE_3                = 12,

    FANDRAL_SAY_1                     = 0,
    FANDRAL_SAY_2                     = 1,
    FANDRAL_SAY_3                     = 2,
    FANDRAL_SAY_4                     = 3,
    FANDRAL_SAY_5                     = 4,
    FANDRAL_SAY_6                     = 5,
    FANDRAL_EMOTE_1                   = 6,
    FANDRAL_EMOTE_2                   = 7,

    CAELESTRASZ_SAY_1                 = 0,
    CAELESTRASZ_SAY_2                 = 1,
    CAELESTRASZ_YELL_1                = 2,

    ARYGOS_SAY_1                      = 0,
    ARYGOS_YELL_1                     = 1,
    ARYGOS_EMOTE_1                    = 2,

    MERITHRA_SAY_1                    = 0,
    MERITHRA_SAY_2                    = 1,
    MERITHRA_YELL_1                   = 2,
    MERITHRA_EMOTE_1                  = 3,

    GO_GATE_OF_AHN_QIRAJ              = 176146,
    GO_GLYPH_OF_AHN_QIRAJ             = 176148,
    GO_ROOTS_OF_AHN_QIRAJ             = 176147
};

struct QuestCinematic
{
    int32 TextId;
    uint32 Creature, Timer;
};

// Creature 0 - Anachronos, 1 - Fandral, 2 - Arygos, 3 - Merithra, 4 - Caelestrasz
static QuestCinematic EventAnim[]=
{
    {ANACHRONOS_SAY_1,  0, 2000},
    {FANDRAL_SAY_1,     1, 4000},
    {MERITHRA_EMOTE_1,  3, 500},
    {MERITHRA_SAY_1,    3, 500},
    {ARYGOS_EMOTE_1,    2, 2000},
    {CAELESTRASZ_SAY_1, 4, 8000},
    {MERITHRA_SAY_2,    3, 6000},
    {0, 3, 2000},
    {MERITHRA_YELL_1,   3, 2500},
    {0, 3, 3000},                   //Morph
    {0, 3, 4000},                   //EmoteLiftoff
    {0, 3, 4000},                   // spell
    {0, 3, 1250},                   //fly
    {0, 3, 250},                    //remove flags
    {ARYGOS_SAY_1, 2, 3000},
    {0, 3, 2000},
    {ARYGOS_YELL_1, 2, 3000},
    {0, 3, 3000},                   //Morph
    {0, 3, 4000},                   //EmoteLiftoff
    {0, 3, 4000},                   // spell
    {0, 3, 1000},                   //fly
    {0, 3, 1000},                   //remove flags
    {CAELESTRASZ_SAY_2, 4, 5000},
    {0, 3, 3000},
    {CAELESTRASZ_YELL_1, 4, 3000},
    {0, 3, 3000},                   //Morph
    {0, 3, 4000},                   //EmoteLiftoff
    {0, 3, 2500},                   // spell
    {ANACHRONOS_SAY_2, 0, 2000},
    {0, 3, 250},                    //fly
    {0, 3, 25},                     //remove flags
    {FANDRAL_SAY_2, 1, 3000},
    {ANACHRONOS_SAY_3, 0, 10000},   //Both run through the armies
    {0, 3, 2000},                   // Sands will stop
    {0, 3, 8000},                   // Summon Gate
    {ANACHRONOS_SAY_4, 0, 4000},
    {0, 0, 2000},                   //spell 1-> Arcane cosmetic (Mobs freeze)
    {0, 0, 5000},                   //Spell 2-> Arcane long cosmetic (barrier appears) (Barrier -> Glyphs)
    {0, 0, 7000},                   //BarrieR
    {0, 0, 4000},                   //Glyphs
    {ANACHRONOS_SAY_5, 0, 2000},
    {0, 0, 4000},                   // Roots
    {FANDRAL_SAY_3, 1, 3000},       //Root Text
    {FANDRAL_EMOTE_1, 1, 3000},     //falls knee
    {ANACHRONOS_SAY_6, 0, 3000},
    {ANACHRONOS_SAY_7, 0, 3000},
    {ANACHRONOS_SAY_8, 0, 8000},
    {ANACHRONOS_EMOTE_1, 0, 1000},  //Give Scepter
    {FANDRAL_SAY_4, 1, 3000},
    {FANDRAL_SAY_5, 1, 3000},       //->Equip hammer~Scepter, throw it at door
    {FANDRAL_EMOTE_2, 1, 3000},     //Throw hammer at door.
    {ANACHRONOS_SAY_9, 0, 3000},
    {FANDRAL_SAY_6, 1, 3000},       //fandral goes away
    {ANACHRONOS_EMOTE_2, 0, 3000},
    {ANACHRONOS_EMOTE_3, 0, 3000},
    {0, 0, 2000},
    {0, 0, 2000},
    {0, 0, 4000},
    {ANACHRONOS_SAY_10, 0, 3000},
    {0, 0, 2000},
    {0, 0, 3000},
    {0, 0, 15000},
    {0, 0, 5000},
    {0, 0, 3500},
    {0, 0, 5000},
    {0, 0, 3500},
    {0, 0, 5000},
    {0, 0, 0}
};

//Cordinates for Spawns
Position const SpawnLocation[] =
{
    {-8085.0f, 1528.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1526.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8085.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8085.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8080.0f, 1424.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8085.0f, 1422.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    // 2 waves of warriors
    {-8082.0f, 1528.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1525.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1526.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1527.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8082.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8082.0f, 1523.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1521.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1528.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1519.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1526.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8082.0f, 1524.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1522.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1520.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8078.0f, 1518.0f, 2.61f, 3.141592f}, //Kaldorei Infantry
    {-8082.0f, 1516.0f, 2.61f, 3.141592f}, //Kaldorei Infantry

    {-8088.0f, 1510.0f, 2.61f, 0.0f},      //Anubisath Conqueror
    {-8084.0f, 1520.0f, 2.61f, 0.0f},      //Anubisath Conqueror
    {-8088.0f, 1530.0f, 2.61f, 0.0f},      //Anubisath Conqueror

    {-8080.0f, 1513.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8082.0f, 1523.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8085.0f, 1518.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8082.0f, 1516.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8085.0f, 1520.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8080.0f, 1528.0f, 2.61f, 0.0f},      //Qiraj Wasp

    {-8082.0f, 1513.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8079.0f, 1523.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8080.0f, 1531.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8079.0f, 1516.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8082.0f, 1520.0f, 2.61f, 0.0f},      //Qiraj Wasp
    {-8080.0f, 1518.0f, 2.61f, 0.0f},      //Qiraj Wasp

    {-8081.0f, 1514.0f, 2.61f, 0.0f},      //Qiraj Tank
    {-8081.0f, 1520.0f, 2.61f, 0.0f},      //Qiraj Tank
    {-8081.0f, 1526.0f, 2.61f, 0.0f},      //Qiraj Tank
    {-8081.0f, 1512.0f, 2.61f, 0.0f},      //Qiraj Tank
    {-8082.0f, 1520.0f, 2.61f, 0.0f},      //Qiraj Tank
    {-8081.0f, 1528.0f, 2.61f, 0.0f},      //Qiraj Tank

    {-8082.0f, 1513.0f, 2.61f, 3.141592f}, //Anubisath Conqueror
    {-8082.0f, 1520.0f, 2.61f, 3.141592f}, //Anubisath Conqueror
    {-8082.0f, 1527.0f, 2.61f, 3.141592f}, //Anubisath Conqueror
};

struct WaveData
{
    uint8 SpawnCount, UsedSpawnPoint;
    uint32 CreatureId, SpawnTimer, YellTimer, DespTimer;
    int32 WaveTextId;
};

static WaveData WavesInfo[5] =
{
    {30,  0, 15423, 0, 0, 24000, 0},    // Kaldorei Soldier
    { 3, 35, 15424, 0, 0, 24000, 0},    // Anubisath Conqueror
    {12, 38, 15414, 0, 0, 24000, 0},    // Qiraji Wasps
    { 6, 50, 15422, 0, 0, 24000, 0},    // Qiraji Tanks
    {15, 15, 15423, 0, 0, 24000, 0}     // Kaldorei Soldier

};

struct SpawnSpells
{
    uint32 Timer1, Timer2, SpellId;
};

static SpawnSpells SpawnCast[4] =
{
    {100000, 2000, 33652},   // Stoned
    {38500, 300000, 28528},  // Poison Cloud
    {58000, 300000, 35871},  // Frost Debuff (need correct spell)
    {80950, 300000, 42075},  // Fire Explosion (need correct spell however this one looks cool)
};

/*#####
# npc_anachronos_the_ancient
######*/

enum AnachronosTheAncient
{
    NPC_QIRAJI_WASP                  = 15414,
    NPC_QIRAJI_TANK                  = 15422,
    NPC_KALDOREI_INFANTRY            = 15423,
    NPC_ANUBISATH_CONQUEROR          = 15424,
    SPELL_ARCANE_CHANNELING          = 23017,
    SPELL_NOXIOUS_BREATH             = 24818,
    SPELL_GREEN_DRAGON_TRANSFORM_DND = 25105,
    SPELL_RED_DRAGON_TRANSFORM_DND   = 25106,
    SPELL_BLUE_DRAGON_TRANSFORM_DND  = 25107,
    SPELL_TIME_STOP                  = 25158,
    SPELL_CALL_PRISMATIC_BARRIER     = 25159,
    SPELL_CALL_GLYPHS_OF_WARDING     = 25166,
    SPELL_CALL_ANCIENTS              = 25167,
    SPELL_THROW_HAMMER               = 33806,
    SPELL_FROST_BREATH               = 50505,
    SPELL_FLAME_BREATH               = 54293
};

class npc_anachronos_the_ancient : public CreatureScript
{
public:
    npc_anachronos_the_ancient() : CreatureScript("npc_anachronos_the_ancient") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_anachronos_the_ancientAI(creature);
    }

    struct npc_anachronos_the_ancientAI : public ScriptedAI
    {
        npc_anachronos_the_ancientAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            AnimationTimer = 1500;
            AnimationCount = 0;
            AnachronosQuestTriggerGUID.Clear();
            MerithraGUID.Clear();
            ArygosGUID.Clear();
            CaelestraszGUID.Clear();
            FandralGUID.Clear();
            PlayerGUID.Clear();
            eventEnd = false;
        }

        uint32 AnimationTimer;
        uint8 AnimationCount;

        ObjectGuid AnachronosQuestTriggerGUID;
        ObjectGuid MerithraGUID;
        ObjectGuid ArygosGUID;
        ObjectGuid CaelestraszGUID;
        ObjectGuid FandralGUID;
        ObjectGuid PlayerGUID;
        bool eventEnd;

        void Reset() override
        {
            Initialize();

            me->AddUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
        }

        void HandleAnimation()
        {
            Player* player = ObjectAccessor::GetPlayer(*me, PlayerGUID);
            if (!player)
                return;

            Creature* Fandral = player->FindNearestCreature(NPC_FANDRAL_STAGHELM, 100.0f);
            Creature* Arygos = player->FindNearestCreature(NPC_ARYGOS, 100.0f);
            Creature* Caelestrasz = player->FindNearestCreature(NPC_CAELESTRASZ, 100.0f);
            Creature* Merithra = player->FindNearestCreature(NPC_MERITHRA_OF_THE_DREAM, 100.0f);

            if (!Fandral || !Arygos || !Caelestrasz || !Merithra)
                return;

            AnimationTimer = EventAnim[AnimationCount].Timer;
            if (eventEnd == false)
            {
                switch (AnimationCount)
                {
                    case 0:
                        Talk(ANACHRONOS_SAY_1, Fandral);
                        break;
                    case 1:
                        Fandral->SetTarget(me->GetGUID());
                        Fandral->AI()->Talk(FANDRAL_SAY_1, me);
                        break;
                    case 2:
                        Fandral->SetTarget(ObjectGuid::Empty);
                        Merithra->AI()->Talk(MERITHRA_EMOTE_1);
                        break;
                    case 3:
                        Merithra->AI()->Talk(MERITHRA_SAY_1);
                        break;
                    case 4:
                        Arygos->AI()->Talk(ARYGOS_EMOTE_1);
                        break;
                    case 5:
                        Caelestrasz->SetTarget(Fandral->GetGUID());
                        Caelestrasz->AI()->Talk(CAELESTRASZ_SAY_1);
                        break;
                    case 6:
                        Merithra->AI()->Talk(MERITHRA_SAY_2);
                        break;
                    case 7:
                        Caelestrasz->SetTarget(ObjectGuid::Empty);
                        Merithra->GetMotionMaster()->MoveCharge(-8065, 1530, 2.61f, 10);
                        break;
                    case 8:
                        Merithra->AI()->Talk(MERITHRA_YELL_1);
                        break;
                    case 9:
                        Merithra->CastSpell(Merithra, SPELL_GREEN_DRAGON_TRANSFORM_DND, true);
                        break;
                    case 10:
                        Merithra->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                        Merithra->SetDisableGravity(true);
                        Merithra->GetMotionMaster()->MoveCharge(-8065, 1530, 6.61f, 3);
                        break;
                    case 11:
                        Merithra->CastSpell(Merithra, SPELL_NOXIOUS_BREATH, false);
                        break;
                    case 12:
                        Merithra->GetMotionMaster()->MoveCharge(-8100, 1530, 50, 42);
                        break;
                    case 13:
                        break;
                    case 14:
                        Arygos->AI()->Talk(ARYGOS_SAY_1);
                        Merithra->SetVisible(false);
                        break;
                    case 15:
                        Arygos->GetMotionMaster()->MoveCharge(-8065, 1530, 2.61f, 10);
                        Merithra->GetMotionMaster()->MoveCharge(-8034.535f, 1535.14f, 2.61f, 42);
                        break;
                    case 16:
                        Arygos->AI()->Talk(ARYGOS_YELL_1);
                        break;
                    case 17:
                        Arygos->CastSpell(Arygos, SPELL_BLUE_DRAGON_TRANSFORM_DND, true);
                        break;
                    case 18:
                        Arygos->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                        Arygos->SetDisableGravity(true);
                        Arygos->GetMotionMaster()->MoveCharge(-8065, 1530, 6.61f, 42);
                        break;
                    case 19:
                        Arygos->CastSpell(Arygos, SPELL_FROST_BREATH, false);
                        break;
                    case 20:
                        Arygos->GetMotionMaster()->MoveCharge(-8095, 1530, 50, 42);
                        break;
                    case 21:
                        break;
                    case 22:
                        Caelestrasz->AI()->Talk(CAELESTRASZ_SAY_2, Fandral);
                        break;
                    case 23:
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8065, 1530, 2.61f, 10);
                        Arygos->SetVisible(false);
                        Arygos->GetMotionMaster()->MoveCharge(-8034.535f, 1535.14f, 2.61f, 10);
                        break;
                    case 24:
                        Caelestrasz->AI()->Talk(CAELESTRASZ_YELL_1);
                        break;
                    case 25:
                        Caelestrasz->CastSpell(Caelestrasz, SPELL_RED_DRAGON_TRANSFORM_DND, true);
                        break;
                    case 26:
                        Caelestrasz->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                        Caelestrasz->SetDisableGravity(true);
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8065, 1530, 7.61f, 4);
                        break;
                    case 27:
                        Caelestrasz->CastSpell(Caelestrasz, SPELL_FLAME_BREATH, false);
                        break;
                    case 28:
                        Talk(ANACHRONOS_SAY_2, Fandral);
                        break;
                    case 29:
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8095, 1530, 50, 42);
                        Fandral->AI()->Talk(FANDRAL_SAY_2);
                        break;
                    case 30:
                        break;
                    case 31:
                        Talk(ANACHRONOS_SAY_3, Fandral);
                        break;
                    case 32:
                        Caelestrasz->SetVisible(false);
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8034.535f, 1535.14f, 2.61f, 42);
                        Fandral->GetMotionMaster()->MoveCharge(-8108, 1529, 2.77f, 8);
                        me->GetMotionMaster()->MoveCharge(-8113, 1525, 2.77f, 8);
                        break;//both run to the gate
                    case 33:
                        Talk(ANACHRONOS_SAY_4);
                        Caelestrasz->GetMotionMaster()->MoveCharge(-8050, 1473, 65, 15);
                        break; //Text: sands will stop
                    case 34:
                        DoCast(player, SPELL_ARCANE_CHANNELING, true);//Arcane Channeling
                        break;
                    case 35:
                        me->CastSpell(-8088, 1520.43f, 2.67f, SPELL_TIME_STOP, true);
                        break;
                    case 36:
                        DoCast(player, SPELL_CALL_PRISMATIC_BARRIER, true);
                        break;
                    case 37:
                        me->SummonGameObject(GO_GATE_OF_AHN_QIRAJ, Position(-8130.f, 1525.f, 17.5f, 0.f), QuaternionData(), 0);
                        break;
                    case 38:
                        DoCast(player, SPELL_CALL_GLYPHS_OF_WARDING, true);
                        me->SummonGameObject(GO_GLYPH_OF_AHN_QIRAJ, Position(-8130.f, 1525.f, 17.5f, 0.f), QuaternionData(), 0);
                        break;
                    case 39:
                        Talk(ANACHRONOS_SAY_5, Fandral);
                        break;
                    case 40:
                        Fandral->CastSpell(me, SPELL_CALL_ANCIENTS, true);
                        break;
                    case 41:
                        Fandral->SummonGameObject(GO_ROOTS_OF_AHN_QIRAJ, Position(-8130.f, 1525.f, 17.5f, 0.f), QuaternionData(), 0);
                        Fandral->AI()->Talk(FANDRAL_SAY_3);
                        break;
                    case 42:
                        me->CastStop();
                        Fandral->AI()->Talk(FANDRAL_EMOTE_1);
                        break;
                    case 43:
                        Fandral->CastStop();
                        break;
                    case 44:
                        Talk(ANACHRONOS_SAY_6);
                        break;
                    case 45:
                        Talk(ANACHRONOS_SAY_7);
                        break;
                    case 46:
                        Talk(ANACHRONOS_SAY_8);
                        me->GetMotionMaster()->MoveCharge(-8110, 1527, 2.77f, 4);
                        break;
                    case 47:
                        Talk(ANACHRONOS_EMOTE_1);
                        break;
                    case 48:
                        Fandral->AI()->Talk(FANDRAL_SAY_4, me);
                        break;
                    case 49:
                        Fandral->AI()->Talk(FANDRAL_SAY_5, me);
                        break;
                    case 50:
                        Fandral->AI()->Talk(FANDRAL_EMOTE_2);
                        Fandral->CastSpell(-8127, 1525, 17.5f, SPELL_THROW_HAMMER, true);
                        break;
                    case 51:
                    {
                        uint32 entries[4] = { NPC_KALDOREI_INFANTRY, NPC_ANUBISATH_CONQUEROR, NPC_QIRAJI_WASP, NPC_QIRAJI_TANK };
                        Unit* mob = nullptr;
                        for (uint8 i = 0; i < 4; ++i)
                        {
                            mob = player->FindNearestCreature(entries[i], 50);
                            while (mob)
                            {
                                mob->RemoveFromWorld();
                                mob = player->FindNearestCreature(NPC_KALDOREI_INFANTRY, 50);
                            }
                        }
                        break;
                    }
                    case 52:
                        Fandral->GetMotionMaster()->MoveCharge(-8028.75f, 1538.795f, 2.61f, 4);
                        Fandral->AI()->Talk(ANACHRONOS_SAY_9, me);
                        break;
                    case 53:
                        Fandral->AI()->Talk(FANDRAL_SAY_6);
                        break;
                    case 54:
                        Talk(ANACHRONOS_EMOTE_2);
                        break;
                    case 55:
                        Fandral->SetVisible(false);
                        break;
                    case 56:
                        Talk(ANACHRONOS_EMOTE_3);
                        me->GetMotionMaster()->MoveCharge(-8116, 1522, 3.65f, 4);
                        break;
                    case 57:
                        me->GetMotionMaster()->MoveCharge(-8116.7f, 1527, 3.7f, 4);
                        break;
                    case 58:
                        me->GetMotionMaster()->MoveCharge(-8112.67f, 1529.9f, 2.86f, 4);
                        break;
                    case 59:
                        me->GetMotionMaster()->MoveCharge(-8117.99f, 1532.24f, 3.94f, 4);
                        break;
                    case 60:
                        Talk(ANACHRONOS_SAY_10, player);
                        me->GetMotionMaster()->MoveCharge(-8113.46f, 1524.16f, 2.89f, 4);
                        break;
                    case 61:
                        me->GetMotionMaster()->MoveCharge(-8057.1f, 1470.32f, 2.61f, 6);
                        if (player->IsInRange(me, 0, 15))
                            player->GroupEventHappens(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD, me);
                        break;
                    case 62:
                        me->SetDisplayId(15500);
                        break;
                    case 63:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                        me->SetDisableGravity(true);
                        break;
                    case 64:
                        me->GetMotionMaster()->MoveCharge(-8000, 1400, 150, 9);
                        break;
                    case 65:
                        me->SetVisible(false);
                        if (Creature* AnachronosQuestTrigger = (ObjectAccessor::GetCreature(*me, AnachronosQuestTriggerGUID)))
                        {
                            Talk(ARYGOS_YELL_1);
                            AnachronosQuestTrigger->AI()->EnterEvadeMode();
                            eventEnd=true;
                        }
                        break;
                }
            }
            ++AnimationCount;
        }
        void UpdateAI(uint32 diff) override
        {
            if (AnimationTimer)
            {
                if (AnimationTimer <= diff)
                    HandleAnimation();
                else AnimationTimer -= diff;
            }
            if (AnimationCount < 65)
                me->CombatStop();
            if (AnimationCount == 65 || eventEnd)
                EnterEvadeMode();
        }
    };

};

/*######
# npc_qiraj_war_spawn
######*/

enum QirajWarSpawn
{
    SPELL_STONED_CHANNEL_CAST_VISUAL = 15533,
    SPELL_SUMMON_POISON_CLOUD        = 24319,
    SPELL_STONED                     = 33652
};

class npc_qiraj_war_spawn : public CreatureScript
{
public:
    npc_qiraj_war_spawn() : CreatureScript("npc_qiraj_war_spawn") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_qiraj_war_spawnAI(creature);
    }

    struct npc_qiraj_war_spawnAI : public ScriptedAI
    {
        npc_qiraj_war_spawnAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
            SpellTimer1 = 0;
            SpellTimer2 = 0;
            SpellTimer3 = 0;
            SpellTimer4 = 0;
        }

        void Initialize()
        {
            MobGUID.Clear();
            PlayerGUID.Clear();
            Timers = false;
            hasTarget = false;
        }

        ObjectGuid MobGUID;
        ObjectGuid PlayerGUID;
        uint32 SpellTimer1, SpellTimer2, SpellTimer3, SpellTimer4;
        bool Timers;
        bool hasTarget;

        void Reset() override
        {
            Initialize();
        }

        void EnterCombat(Unit* /*who*/) override { }
        void JustDied(Unit* /*slayer*/) override;

        void UpdateAI(uint32 diff) override
        {
            if (!Timers)
            {
                if (me->GetEntry() == NPC_ANUBISATH_CONQUEROR || me->GetEntry() == NPC_QIRAJI_TANK || me->GetEntry() == NPC_QIRAJI_WASP) //all but Kaldorei Soldiers
                {
                    SpellTimer1 = SpawnCast[1].Timer1;
                    SpellTimer2 = SpawnCast[2].Timer1;
                    SpellTimer3 = SpawnCast[3].Timer1;
                }
                if (me->GetEntry() == NPC_KALDOREI_INFANTRY || me->GetEntry() == NPC_ANUBISATH_CONQUEROR || me->GetEntry() == NPC_QIRAJI_TANK || me->GetEntry() == NPC_QIRAJI_WASP)
                    SpellTimer4 = SpawnCast[0].Timer1;
                Timers = true;
            }
            if (me->GetEntry() == NPC_ANUBISATH_CONQUEROR || me->GetEntry() == NPC_QIRAJI_TANK || me->GetEntry() == NPC_QIRAJI_WASP)
            {
                if (SpellTimer1 <= diff)
                {
                    DoCast(me, SpawnCast[1].SpellId);
                    DoCast(me, SPELL_SUMMON_POISON_CLOUD);
                    SpellTimer1 = SpawnCast[1].Timer2;
                } else SpellTimer1 -= diff;
                if (SpellTimer2 <= diff)
                {
                    DoCast(me, SpawnCast[2].SpellId);
                    SpellTimer2 = SpawnCast[2].Timer2;
                } else SpellTimer2 -= diff;
                if (SpellTimer3 <= diff)
                {
                    DoCast(me, SpawnCast[3].SpellId);
                    SpellTimer3 = SpawnCast[3].Timer2;
                } else SpellTimer3 -= diff;
            }
            if (me->GetEntry() == NPC_KALDOREI_INFANTRY || me->GetEntry() == NPC_ANUBISATH_CONQUEROR || me->GetEntry() == NPC_QIRAJI_TANK || me->GetEntry() == NPC_QIRAJI_WASP)
            {
                if (SpellTimer4 <= diff)
                {
                    me->RemoveAllAttackers();
                    me->AttackStop();
                    DoCast(me, SPELL_STONED_CHANNEL_CAST_VISUAL);
                    SpellTimer4 = SpawnCast[0].Timer2;
                } else SpellTimer4 -= diff;
            }
            if (!hasTarget)
            {
                Unit* target = nullptr;
                if (me->GetEntry() == NPC_ANUBISATH_CONQUEROR || me->GetEntry() == NPC_QIRAJI_TANK || me->GetEntry() == NPC_QIRAJI_WASP)
                    target = me->FindNearestCreature(NPC_KALDOREI_INFANTRY, 20, true);
                if (me->GetEntry() == NPC_KALDOREI_INFANTRY)
                {
                    uint8 tar = urand(0, 2);

                    if (tar == 0)
                        target = me->FindNearestCreature(NPC_QIRAJI_TANK, 20, true);
                    else if (tar == 1)
                        target = me->FindNearestCreature(NPC_ANUBISATH_CONQUEROR, 20, true);
                    else if (tar == 2)
                        target = me->FindNearestCreature(NPC_QIRAJI_WASP, 20, true);
                }
                hasTarget = true;
                if (target)
                    AttackStart(target);
            }
            if (!(me->FindNearestCreature(NPC_CAELESTRASZ, 60)))
                DoCast(me, SPELL_STONED);

            if (!UpdateVictim())
            {
                hasTarget = false;
                return;
            }

            DoMeleeAttackIfReady();
        }
    };

};

/*#####
# npc_anachronos_quest_trigger
#####*/

class npc_anachronos_quest_trigger : public CreatureScript
{
public:
    npc_anachronos_quest_trigger() : CreatureScript("npc_anachronos_quest_trigger") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_anachronos_quest_triggerAI(creature);
    }

    struct npc_anachronos_quest_triggerAI : public ScriptedAI
    {
        npc_anachronos_quest_triggerAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            PlayerGUID.Clear();

            WaveTimer = 2000;
            AnnounceTimer = 1000;
            LiveCount = 0;
            WaveCount = 0;

            EventStarted = false;
            Announced = false;
            Failed = false;
        }

        ObjectGuid PlayerGUID;

        uint32 WaveTimer;
        uint32 AnnounceTimer;

        int8 LiveCount;
        uint8 WaveCount;

        bool EventStarted;
        bool Announced;
        bool Failed;

        void Reset() override
        {
            Initialize();

            me->SetVisible(false);
        }

        void SummonNextWave()
        {
            uint8 locIndex = WavesInfo[WaveCount].UsedSpawnPoint;
            uint8 count = locIndex + WavesInfo[WaveCount].SpawnCount;

            for (uint8 i = locIndex; i <= count; ++i)
            {
                uint32 desptimer = WavesInfo[WaveCount].DespTimer;

                if (Creature* spawn = me->SummonCreature(WavesInfo[WaveCount].CreatureId, SpawnLocation[i], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, desptimer))
                {
                    if (spawn->GetEntry() == NPC_KALDOREI_INFANTRY)
                        spawn->SetDisplayId(15427 + rand32() % 4);
                    if (i >= 30) WaveCount = 1;
                    if (i >= 33) WaveCount = 2;
                    if (i >= 45) WaveCount = 3;
                    if (i >= 51) WaveCount = 4;

                    if (WaveCount < 5) //1-4 Wave
                    {
                        if (npc_qiraj_war_spawn::npc_qiraj_war_spawnAI* spawnAI = CAST_AI(npc_qiraj_war_spawn::npc_qiraj_war_spawnAI, spawn->AI()))
                        {
                            spawnAI->MobGUID = me->GetGUID();
                            spawnAI->PlayerGUID = PlayerGUID;
                        }
                    }
                }
            }

            WaveTimer = WavesInfo[WaveCount].SpawnTimer;
            AnnounceTimer = WavesInfo[WaveCount].YellTimer;
        }

        void CheckEventFail()
        {
            Player* player = ObjectAccessor::GetPlayer(*me, PlayerGUID);
            if (!player)
                return;

            if (Group* EventGroup = player->GetGroup())
            {
                Player* groupMember = nullptr;

                uint8 GroupMemberCount = 0;
                uint8 DeadMemberCount = 0;
                uint8 FailedMemberCount = 0;

                Group::MemberSlotList const& members = EventGroup->GetMemberSlots();

                for (Group::member_citerator itr = members.begin(); itr!= members.end(); ++itr)
                {
                    groupMember = ObjectAccessor::GetPlayer(*me, itr->guid);
                    if (!groupMember)
                        continue;
                    if (!groupMember->IsWithinDistInMap(me, EVENT_AREA_RADIUS) && groupMember->GetQuestStatus(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD) == QUEST_STATUS_INCOMPLETE)
                    {
                         groupMember->FailQuest(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD);
                        ++FailedMemberCount;
                    }
                    ++GroupMemberCount;

                    if (groupMember->isDead())
                        ++DeadMemberCount;
                }

                if (GroupMemberCount == FailedMemberCount || !player->IsWithinDistInMap(me, EVENT_AREA_RADIUS))
                    Failed = true; //only so event can restart
            }
        }

        void LiveCounter()
        {
            --LiveCount;
            if (!LiveCount)
                Announced = false;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!PlayerGUID || !EventStarted)
                return;

            if (WaveCount < 4)
            {
                if (!Announced && AnnounceTimer <= diff)
                {
                    Talk(WavesInfo[WaveCount].WaveTextId);
                    Announced = true;
                } else AnnounceTimer -= diff;

                if (WaveTimer <= diff)
                    SummonNextWave();
                else WaveTimer -= diff;
            }
            CheckEventFail();
            if (WaveCount == 4 || Failed)
                EnterEvadeMode();
        };
    };

};

void npc_qiraj_war_spawn::npc_qiraj_war_spawnAI::JustDied(Unit* /*slayer*/)
{
    me->DespawnOrUnsummon();

    if (!MobGUID)
        return;

    if (Creature* mob = ObjectAccessor::GetCreature(*me, MobGUID))
        if (npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI* triggerAI = CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, mob->AI()))
            triggerAI->LiveCounter();

}

/*#####
# go_crystalline_tear
######*/

enum CrystallineTear
{
    ARYGOS_GNOME_FORM                  = 15418,
    CAELESTRASZ_NIGHT_ELF_FORM         = 15419,
    MERITHRA_NIGHT_ELF_FORM            = 15420,
    ANACHRONOS_QUEST_TRIGGER_INVISIBLE = 15454
};

class go_crystalline_tear : public GameObjectScript
{
public:
    go_crystalline_tear() : GameObjectScript("go_crystalline_tear") { }

    bool OnQuestAccept(Player* player, GameObject* go, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_A_PAWN_ON_THE_ETERNAL_BOARD)
        {
            if (Creature * trigger = go->FindNearestCreature(ANACHRONOS_QUEST_TRIGGER_INVISIBLE, 100))
            {
                Unit* Merithra = trigger->SummonCreature(NPC_MERITHRA_OF_THE_DREAM, -8034.535f, 1535.14f, 2.61f, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                Unit* Caelestrasz = trigger->SummonCreature(NPC_CAELESTRASZ, -8032.767f, 1533.148f, 2.61f, 1.5f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                Unit* Arygos = trigger->SummonCreature(NPC_ARYGOS, -8034.52f, 1537.843f, 2.61f, 5.7f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                /* Unit* Fandral = */ trigger->SummonCreature(NPC_FANDRAL_STAGHELM, -8028.462f, 1535.843f, 2.61f, 3.141592f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);
                Creature* Anachronos = trigger->SummonCreature(NPC_ANACHRONOS, -8028.75f, 1538.795f, 2.61f, 4, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 220000);

                if (Merithra)
                {
                    Merithra->SetNpcFlags(UNIT_NPC_FLAG_NONE);
                    Merithra->SetStandState(UNIT_STAND_STATE_STAND);
                    Merithra->SetDisplayId(MERITHRA_NIGHT_ELF_FORM);
                    Merithra->SetFaction(35);
                }

                if (Caelestrasz)
                {
                    Caelestrasz->SetNpcFlags(UNIT_NPC_FLAG_NONE);
                    Caelestrasz->SetStandState(UNIT_STAND_STATE_STAND);
                    Caelestrasz->SetDisplayId(CAELESTRASZ_NIGHT_ELF_FORM);
                    Caelestrasz->SetFaction(35);
                }

                if (Arygos)
                {
                    Arygos->SetNpcFlags(UNIT_NPC_FLAG_NONE);
                    Arygos->SetStandState(UNIT_STAND_STATE_STAND);
                    Arygos->SetDisplayId(ARYGOS_GNOME_FORM);
                    Arygos->SetFaction(35);
                }

                if (Anachronos)
                {
                    if (npc_anachronos_the_ancient::npc_anachronos_the_ancientAI * anachronosAI = CAST_AI(npc_anachronos_the_ancient::npc_anachronos_the_ancientAI, Anachronos->AI()))
                        anachronosAI->PlayerGUID = player->GetGUID();

                    if (npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI * triggerAI = CAST_AI(npc_anachronos_quest_trigger::npc_anachronos_quest_triggerAI, trigger->AI()))
                    {
                        triggerAI->Failed = false;
                        triggerAI->PlayerGUID = player->GetGUID();
                        triggerAI->EventStarted = true;
                        triggerAI->Announced = true;
                    }
                }
            }
        }
        return true;
    }

};

/*###
## go_wind_stone
###*/

enum WSSpells
{
    AURA_TWILIGHT_SET      = 24746,
    AURA_MEDALLION         = 24748,
    AURA_RING              = 24782,

    SPELL_TEMPLAR_RANDOM   = 24745,
    SPELL_TEMPLAR_FIRE     = 24747,
    SPELL_TEMPLAR_AIR      = 24757,
    SPELL_TEMPLAR_EARTH    = 24759,
    SPELL_TEMPLAR_WATER    = 24761,

    SPELL_DUKE_RANDOM      = 24762,
    SPELL_DUKE_FIRE        = 24766,
    SPELL_DUKE_AIR         = 24769,
    SPELL_DUKE_EARTH       = 24771,
    SPELL_DUKE_WATER       = 24773,

    SPELL_ROYAL_RANDOM     = 24785,
    SPELL_ROYAL_FIRE       = 24787,
    SPELL_ROYAL_AIR        = 24791,
    SPELL_ROYAL_EARTH      = 24792,
    SPELL_ROYAL_WATER      = 24793,

    SPELL_PUNISHMENT       = 24803,
    SPELL_SPAWN_IN         = 25035
};

enum WSGossip
{
    OPTION_ID_WS_RANDOM    = 0,
    OPTION_ID_1_CRIMSON    = 1,
    OPTION_ID_2_AZURE      = 2,
    OPTION_ID_3_EARTHEN    = 3,
    OPTION_ID_4_HOARY      = 4,
    OPTION_ID_1_CYNDERS    = 1,
    OPTION_ID_2_FATHOMS    = 2,
    OPTION_ID_3_SHARDS     = 3,
    OPTION_ID_4_ZEPHYRS    = 4,
    OPTION_ID_1_SKALDRENOX = 1,
    OPTION_ID_2_SKWOL      = 2,
    OPTION_ID_3_KAZUM      = 3,
    OPTION_ID_4_WHIRLAXIS  = 4,
    GOSSIP_ID_LESSER_WS    = 6540,
    GOSSIP_ID_WIND_STONE   = 6542,
    GOSSIP_ID_GREATER_WS   = 6543
};

enum WSCreatures
{
    NPC_TEMPLAR_FIRE       = 15209,
    NPC_TEMPLAR_WATER      = 15211,
    NPC_TEMPLAR_AIR        = 15212,
    NPC_TEMPLAR_EARTH      = 15307,

    NPC_DUKE_FIRE          = 15206,
    NPC_DUKE_WATER         = 15207,
    NPC_DUKE_EARTH         = 15208,
    NPC_DUKE_AIR           = 15220,

    NPC_ROYAL_FIRE         = 15203,
    NPC_ROYAL_AIR          = 15204,
    NPC_ROYAL_EARTH        = 15205,
    NPC_ROYAL_WATER        = 15305
};

enum WSItems
{
    ITEM_TEMPLAR_FIRE      = 20416,
    ITEM_TEMPLAR_AIR       = 20418,
    ITEM_TEMPLAR_EARTH     = 20419,
    ITEM_TEMPLAR_WATER     = 20420,

    ITEM_DUKE_FIRE         = 20432,
    ITEM_DUKE_AIR          = 20433,
    ITEM_DUKE_EARTH        = 20435,
    ITEM_DUKE_WATER        = 20436,

    ITEM_ROYAL_FIRE        = 20447,
    ITEM_ROYAL_AIR         = 20448,
    ITEM_ROYAL_EARTH       = 20449,
    ITEM_ROYAL_WATER       = 20450
};

enum WS
{
    TEMPLAR                = 0,
    DUKE                   = 1,
    ROYAL                  = 2,

    FIRE                   = 0x1,
    WATER                  = 0x2,
    EARTH                  = 0x4,
    AIR                    = 0x8
};

enum WSTexts
{
    SAY_TEMPLAR_AGGRO      = 0,
    SAY_DUKE_AGGRO         = 0,
    YELL_ROYAL_AGGRO       = 0
};

class go_wind_stone : public GameObjectScript
{
    public:
        go_wind_stone() : GameObjectScript("go_wind_stone") { }

    private:
        uint8 GetPlayerRank(Player* player) // For random summoning
        {
            bool setAura = player->HasAura(AURA_TWILIGHT_SET);
            bool medallionAura = player->HasAura(AURA_MEDALLION);
            bool ringAura = player->HasAura(AURA_RING);

            if (setAura && medallionAura && ringAura)
                return 3;
            else if (setAura && medallionAura)
                return 2;
            else if (setAura)
                return 1;
            else
                return 0;
        }

        uint8 GetItems(Player* player, WS type)
        {
            uint8 result = 0x0;

            switch (type)
            {
                case TEMPLAR:
                {
                    if (player->HasItemCount(ITEM_TEMPLAR_FIRE))
                        result |= FIRE;
                    if (player->HasItemCount(ITEM_TEMPLAR_WATER))
                        result |= WATER;
                    if (player->HasItemCount(ITEM_TEMPLAR_EARTH))
                        result |= EARTH;
                    if (player->HasItemCount(ITEM_TEMPLAR_AIR))
                        result |= AIR;
                    break;
                }
                case DUKE:
                {
                    if (player->HasItemCount(ITEM_DUKE_FIRE))
                        result |= FIRE;
                    if (player->HasItemCount(ITEM_DUKE_WATER))
                        result |= WATER;
                    if (player->HasItemCount(ITEM_DUKE_EARTH))
                        result |= EARTH;
                    if (player->HasItemCount(ITEM_DUKE_AIR))
                        result |= AIR;
                    break;
                }
                case ROYAL:
                {
                    if (player->HasItemCount(ITEM_ROYAL_FIRE))
                        result |= FIRE;
                    if (player->HasItemCount(ITEM_ROYAL_WATER))
                        result |= WATER;
                    if (player->HasItemCount(ITEM_ROYAL_EARTH))
                        result |= EARTH;
                    if (player->HasItemCount(ITEM_ROYAL_AIR))
                        result |= AIR;
                    break;
                }
                default:
                    break;
            }
            return result;
        }

        void SummonNPC(GameObject* go, Player* player, uint32 npc, uint32 spell)
        {
            go->CastSpell(player, spell);
            TempSummon* summons = go->SummonCreature(npc, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), player->GetOrientation() - float(M_PI), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10 * 60 * 1000);
            summons->CastSpell(summons, SPELL_SPAWN_IN, false);
            switch (summons->GetEntry())
            {
                case NPC_TEMPLAR_FIRE:
                case NPC_TEMPLAR_WATER:
                case NPC_TEMPLAR_AIR:
                case NPC_TEMPLAR_EARTH:
                    summons->AI()->Talk(SAY_TEMPLAR_AGGRO, player);
                    break;

                case NPC_DUKE_FIRE:
                case NPC_DUKE_WATER:
                case NPC_DUKE_EARTH:
                case NPC_DUKE_AIR:
                    summons->AI()->Talk(SAY_DUKE_AGGRO);
                    break;
                case NPC_ROYAL_FIRE:
                case NPC_ROYAL_AIR:
                case NPC_ROYAL_EARTH:
                case NPC_ROYAL_WATER:
                    summons->AI()->Talk(YELL_ROYAL_AGGRO);
                    break;
            }
            summons->RemoveUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
            summons->SendMeleeAttackStart(player);
            summons->CombatStart(player);
        }

    public:
        bool OnGossipHello(Player* player, GameObject* go) override
        {
            uint8 rank = GetPlayerRank(player);

            uint32 gossipId = go->GetGOInfo()->GetGossipMenuId();
            switch (gossipId)
            {
                case GOSSIP_ID_LESSER_WS:
                {
                    if (rank >= 1) // 1 or 2 or 3
                        AddGossipItemFor(player, GOSSIP_ID_LESSER_WS, OPTION_ID_WS_RANDOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    else
                    {
                        go->CastSpell(player, SPELL_PUNISHMENT);
                        break;
                    }

                    uint8 item = GetItems(player, TEMPLAR);
                    if (item & FIRE)
                        AddGossipItemFor(player, GOSSIP_ID_LESSER_WS, OPTION_ID_1_CRIMSON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    if (item & WATER)
                        AddGossipItemFor(player, GOSSIP_ID_LESSER_WS, OPTION_ID_2_AZURE,   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    if (item & EARTH)
                        AddGossipItemFor(player, GOSSIP_ID_LESSER_WS, OPTION_ID_3_EARTHEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                    if (item & AIR)
                        AddGossipItemFor(player, GOSSIP_ID_LESSER_WS, OPTION_ID_4_HOARY,   GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                    break;
                }
                case GOSSIP_ID_WIND_STONE:
                {
                    if (rank >= 2) // 2 or 3
                        AddGossipItemFor(player, GOSSIP_ID_WIND_STONE, OPTION_ID_WS_RANDOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    else
                    {
                        go->CastSpell(player, SPELL_PUNISHMENT);
                        break;
                    }

                    uint8 item = GetItems(player, DUKE);
                    if (item & FIRE)
                        AddGossipItemFor(player, GOSSIP_ID_WIND_STONE, OPTION_ID_1_CYNDERS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                    if (item & WATER)
                        AddGossipItemFor(player, GOSSIP_ID_WIND_STONE, OPTION_ID_2_FATHOMS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                    if (item & EARTH)
                        AddGossipItemFor(player, GOSSIP_ID_WIND_STONE, OPTION_ID_3_SHARDS,  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                    if (item & AIR)
                        AddGossipItemFor(player, GOSSIP_ID_WIND_STONE, OPTION_ID_4_ZEPHYRS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
                    break;
                }
                case GOSSIP_ID_GREATER_WS:
                {
                    if (rank == 3) // 3
                        AddGossipItemFor(player, GOSSIP_ID_GREATER_WS, OPTION_ID_WS_RANDOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                    else
                    {
                        go->CastSpell(player, SPELL_PUNISHMENT);
                        break;
                    }

                    uint8 item = GetItems(player, ROYAL);
                    if (item & FIRE)
                        AddGossipItemFor(player, GOSSIP_ID_GREATER_WS, OPTION_ID_1_SKALDRENOX, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                    if (item & WATER)
                        AddGossipItemFor(player, GOSSIP_ID_GREATER_WS, OPTION_ID_2_SKWOL,      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                    if (item & EARTH)
                        AddGossipItemFor(player, GOSSIP_ID_GREATER_WS, OPTION_ID_3_KAZUM,      GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                    if (item & AIR)
                        AddGossipItemFor(player, GOSSIP_ID_GREATER_WS, OPTION_ID_4_WHIRLAXIS,  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                    break;
                }
                default:
                    break;
            }

            SendGossipMenuFor(player, player->GetGossipTextId(gossipId, go), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* go, uint32 /*sender*/, uint32 action) override
        {
            ClearGossipMenuFor(player);
            player->PlayerTalkClass->SendCloseGossip();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    SummonNPC(go, player, RAND(NPC_TEMPLAR_WATER, NPC_TEMPLAR_FIRE, NPC_TEMPLAR_EARTH, NPC_TEMPLAR_AIR), SPELL_TEMPLAR_RANDOM);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    SummonNPC(go, player, NPC_TEMPLAR_FIRE, SPELL_TEMPLAR_FIRE);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    SummonNPC(go, player, NPC_TEMPLAR_WATER, SPELL_TEMPLAR_WATER);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    SummonNPC(go, player, NPC_TEMPLAR_EARTH, SPELL_TEMPLAR_EARTH);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    SummonNPC(go, player, NPC_TEMPLAR_AIR, SPELL_TEMPLAR_AIR);
                    break;

                case GOSSIP_ACTION_INFO_DEF + 6:
                    SummonNPC(go, player, RAND(NPC_DUKE_FIRE, NPC_DUKE_WATER, NPC_DUKE_EARTH, NPC_DUKE_AIR), SPELL_DUKE_RANDOM);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 7:
                    SummonNPC(go, player, NPC_DUKE_FIRE, SPELL_DUKE_FIRE);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 8:
                    SummonNPC(go, player, NPC_DUKE_WATER, SPELL_DUKE_WATER);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 9:
                    SummonNPC(go, player, NPC_DUKE_EARTH, SPELL_DUKE_EARTH);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 10:
                    SummonNPC(go, player, NPC_DUKE_AIR, SPELL_DUKE_AIR);
                    break;

                case GOSSIP_ACTION_INFO_DEF + 11:
                    SummonNPC(go, player, RAND(NPC_ROYAL_FIRE, NPC_ROYAL_AIR, NPC_ROYAL_EARTH, NPC_ROYAL_WATER), SPELL_ROYAL_RANDOM);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 12:
                    SummonNPC(go, player, NPC_ROYAL_FIRE, SPELL_ROYAL_FIRE);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 13:
                    SummonNPC(go, player, NPC_ROYAL_WATER, SPELL_ROYAL_WATER);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 14:
                    SummonNPC(go, player, NPC_ROYAL_EARTH, SPELL_ROYAL_EARTH);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 15:
                    SummonNPC(go, player, NPC_ROYAL_AIR, SPELL_ROYAL_AIR);
                    break;

                default:
                    break;
            }
            return true;
        }
};

// 130216 - Magni Bronzebeard
struct npc_magni_bronzebeard_silithus : public ScriptedAI
{
    npc_magni_bronzebeard_silithus(Creature* creature) : ScriptedAI(creature) { }

    void MoveInLineOfSight(Unit* unit) override
    {
        if (Player* player = unit->ToPlayer())
            if (player->GetDistance(me) < 20.0f)
                player->KilledMonsterCredit(me->GetEntry());
    }
};

// 136907 - Magni Bronzebeard
struct npc_magni_bronzebeard_heart_chamber : public ScriptedAI
{
    npc_magni_bronzebeard_heart_chamber(Creature* creature) : ScriptedAI(creature) { }

    void sGossipSelect(Player* player, uint32 /*menuId*/, uint32 /*gossipListId*/) override
    {       
        player->CastSpell(player, 268798, true);
    }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->ID == 52428)
            player->PlayConversation(9001);

        if (quest->ID == 51403 || quest->ID == 53031)
            player->PlayConversation(8689);
    }
};

//3046
class scene_get_heart_of_azeroth : public SceneScript
{
public:
    scene_get_heart_of_azeroth() : SceneScript("scene_get_heart_of_azeroth") { }

    void OnSceneStart(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/)
    {
        player->GetScheduler().Schedule(38s, [player] (TaskContext context)
        {
            player->RemoveAurasDueToSpell(268798);
            player->GetSceneMgr().CancelSceneBySceneId(3046);
        });
    }
};

// 293847 - Titan Console
struct go_azeroth_heart_chamber_titan_console : public GameObjectAI
{
    go_azeroth_heart_chamber_titan_console(GameObject* go) : GameObjectAI(go) { }

    bool GossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->CastSpell(player, 270537, true); // Azeroth Stabbed
        return false;
    }
};

enum WoundMending
{
    SPELL_AZERITE_EXPLOSION     = 271748,
    SPELL_MAIN                  = 278756,
};

// 141870 - Azerite Wound
struct npc_azeroth_heart_chamber_azerite_wound : public ScriptedAI
{
    npc_azeroth_heart_chamber_azerite_wound(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        me->GetScheduler().CancelAll();
        me->GetScheduler().Schedule(1s, [](TaskContext context)
        {
            GetContextUnit()->CastSpell(nullptr, SPELL_AZERITE_EXPLOSION, true);
            context.Repeat(0.5s, 2s);
        });
    }

    void SpellHit(Unit* caster, SpellInfo const* /*spell*/) override
    {
        if (Player* pCaster = caster->ToPlayer())
        {
            pCaster->KilledMonsterCredit(me->GetEntry());

            if (GameObject* rock = me->FindNearestGameObject(294030, 5.f))
                rock->SetAnimKitId(16292, false);

            if (Quest const* quest = sObjectMgr->GetQuestTemplate(52428))
            {
                if (pCaster->IsQuestObjectiveProgressComplete(quest))
                {
                    pCaster->RemoveAurasDueToSpell(275824);
                    pCaster->CastSpell(pCaster, 271184, true);
                }
            }

            me->GetScheduler().CancelAll();
            me->GetScheduler().Schedule(20s, [](TaskContext context)
            {
                if (GameObject* rock = GetContextUnit()->FindNearestGameObject(294030, 5.f))
                    rock->SetAnimKitId(2664, false);

                GetContextCreature()->AI()->Reset();
            });
        }
    }
};

// 267913 Heart of Azeroth
class spell_azeroth_heart_chamber_heart_of_azeroth : public SpellScript
{
    PrepareSpellScript(spell_azeroth_heart_chamber_heart_of_azeroth);

    void HandleAfterHit()
    {
        GetCaster()->CastSpell(nullptr, SPELL_MAIN, true);

        GetCaster()->GetScheduler().Schedule(500ms, [](TaskContext context)
        {
            GetContextUnit()->CastSpell(nullptr, SPELL_AZERITE_EXPLOSION, true);
            GetContextUnit()->ModifyPower(POWER_ALTERNATE_POWER, 1);

            if (GetContextUnit()->HasAura(SPELL_MAIN))
                context.Repeat();
        });
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_azeroth_heart_chamber_heart_of_azeroth::HandleAfterHit);
    }
};

enum TeleportPadEntries
{
    GO_TELEPORT_PAD_ALLIANCE            = 294313,
    GO_TELEPORT_PAD_HORDE               = 294538,
    GO_TELEPORT_PAD_SILITHUS            = 289522,

    QUEST_SPEAKER_IMPERATIVE_ALLIANCE   = 51403,
    QUEST_SPEAKER_IMPERATIVE_HORDE      = 53031,
};

// 294313/294538/289522 - Teleport Pad
struct go_azeroth_heart_chamber_teleport_pad : public GameObjectAI
{
    go_azeroth_heart_chamber_teleport_pad(GameObject* go) : GameObjectAI(go) { }

    bool IsNeverVisibleFor(WorldObject const* seer) override
    {
        Player const* player = seer->ToPlayer();
        if (!player)
            return false;

        if (player->HasQuest(QUEST_SPEAKER_IMPERATIVE_ALLIANCE) && go->GetEntry() != GO_TELEPORT_PAD_ALLIANCE)
            return true;
        else if (player->HasQuest(QUEST_SPEAKER_IMPERATIVE_HORDE) && go->GetEntry() != GO_TELEPORT_PAD_HORDE)
            return true;
        else if (!player->HasQuest(QUEST_SPEAKER_IMPERATIVE_ALLIANCE) && !player->HasQuest(QUEST_SPEAKER_IMPERATIVE_HORDE) && go->GetEntry() != GO_TELEPORT_PAD_SILITHUS)
            return true;

        return false;
    }

    bool GossipHello(Player* player, bool /*isUse*/) override
    {
        player->KilledMonsterCredit(140176);
        player->KilledMonsterCredit(142930);
        player->RemoveAura(274461);

        if (player->HasQuest(QUEST_SPEAKER_IMPERATIVE_HORDE))
            player->TeleportTo(1, 1776.500f, -4338.799f, -7.508f, 0.105f);

        if (player->HasQuest(QUEST_SPEAKER_IMPERATIVE_ALLIANCE))
            player->TeleportTo(0, -8292.400f, 1339.900f, 5.241f, 4.693f);
            
        return false;
    }
};

//289521
struct go_titan_translocator : public GameObjectAI
{
    go_titan_translocator(GameObject* go) : GameObjectAI(go) { }

    bool GossipHello(Player* player, bool /*isUse*/) override
    {
        player->KilledMonsterCredit(137581);
        player->KilledMonsterCredit(137606);
        player->TeleportTo(1929, -8265.099f, 1754.500f, 312.625f, 3.141f);
        return true;
    }
};

enum QUEST
{
    QUEST_UNITING_THE_ISLES = 43341,
    QUEST_ANTORUS_THE_BURNING_THRONE_THE_DEATH_OF_A_TITAN = 49015,
    ///  ALIANCE
    QUEST_SUMMONS_TO_STORMWIND = 50371,
    QUEST_GIFTS_OF_THE_FALLEN = 49976,
    QUEST_WITNESS_TO_THE_WOUND = 49981,
    QUEST_FREE_SAMPLES = 50047,
    QUEST_ITS_A_SABOTAGE = 50046,
    QUEST_THE_TWILIGHT_SURVIVOR = 50228,
    QUEST_LARVAE_BY_THE_DOZEN = 50227,
    QUEST_A_THE_BLOOD_OF_AZEROTH = 50364,
    QUEST_A_THE_SPEAKERS_PERSPECTIVE = 50049,
    QUEST_A_THE_SPEAKERS_CALL = 50056,
    QUEST_THE_POWER_IN_OUR_HANDS = 50057,
    ///  HORL
    QUEST_SUMMONS_TO_ORGRIMMAR = 49977,
    QUEST_A_RECENT_DISCOVERY = 50341,
    QUEST_H_WITNESS_TO_THE_WOUND = 49982,

    QUEST_H_THE_BLOOD_OF_AZEROTH = 50364,
    QUEST_H_THE_SPEAKERS_PERSPECTIVE = 50055,
    QUEST_LAZY_PROSPECTORS = 50053,
    QUEST_H_THE_TWILIGHT_SURVIVOR = 50232,
    QUEST_GIZMAX_GREASEBOLT = 132045,
    QUEST_THE_SOURCE_OF_POWER = 50230,
    QUEST_H_LARVAE_BY_THE_DOZEN = 50231,
    QUEST_KHADGARS_REQUEST = 50360,
    QUEST_THE_SPEAKERS_PERSPECTIVE = 50055,
};

enum NPC
{
    NPC_MASTER_MATHIAS_SHAW_132255 = 132255,
    NPC_ZIDORMI_128607 = 128607,
    NPC_NOLAN_SPEED_131963 = 131963,
    NPC_KELSEY_STEELSPARK_130030 = 130030,
    NPC_TAMMY_TINKSPINNER_132606 = 132606,
    NPC_KHARGUS_STONEMANTLE_132192 = 132192,
    NPC_ARCHMAGE_KHADGAR_130032 = 130032,
    NPC_MAGNI_BRONZEBEARD_130216 = 130216,

    NPC_NATHANOS_BLIGHTCALLER_132254 = 132254,
    NPC_PIXNI_RUSTBOMB_132040 = 132040,
    NPC_GIZMAX_GREASEBOLT_132045 = 132045,
    NPC_GROL_WARBLADE_132147 = 132147,
    NPC_LAZY_PROSPECTOR_133019 = 133019,
    NPC_ARCHMAGE_KHADGAR_130033 = 130033,
};

enum
{
    SCENE_GIFTS_OF_THE_FALLEN = 260706,
    KILLED_MONSTER_CREDIT_QUEST_GIFTS_OF_THE_FALLEN = 132274, //KILLED_MONSTER_CREDIT_QUEST_GIFTS_OF_THE_FALLEN

    SCENE_A_RECENT_DISCOVERY = 260707,
    KILLED_MONSTER_CREDIT_A_RECENT_DISCOVERY = 132290, //KILLED_MONSTER_CREDIT_A_RECENT_DISCOVERY
    PHASE_AFTER_QUEST = 10186,
    SCENE_THE_POWER_IN_OUR_HANDS = 263099,
};

class On110Silithus : public PlayerScript
{
public:
    On110Silithus() : PlayerScript("On110Silithus") { }

    void OnLogin(Player* player, bool firstLogin) override
    {
        // Can happen in recovery cases
        if (player->getLevel() >= 110)
            HandleSilithus(player);
    }

    //void OnUpdateZone(Player* player, Area* /*newZone*/, Area* /*oldZone*/)
  /*  {
        HandlePhase(player);
    }*/

  //  void OnUpdateArea(Player* player, Area* /*newArea*/, Area* /*oldArea*/)
  /*  {
        HandlePhase(player);
    }*/

    void HandleSilithus(Player* player)
    {
        if ((player->GetQuestStatus(QUEST_ANTORUS_THE_BURNING_THRONE_THE_DEATH_OF_A_TITAN) == QUEST_STATUS_COMPLETE) || (player->GetQuestStatus(QUEST_ANTORUS_THE_BURNING_THRONE_THE_DEATH_OF_A_TITAN) == QUEST_STATUS_REWARDED))
        {
            if (player->IsInAlliance())
            {
                if (player->GetQuestStatus(QUEST_SUMMONS_TO_STORMWIND) == QUEST_STATUS_NONE)
                    if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_SUMMONS_TO_STORMWIND))
                    {
                     //   Conversation::CreateConversation(6607, player, player->GetPosition(), { player->GetGUID() });
                        player->AddQuest(quest, nullptr);
                    }
                if ((player->GetQuestStatus(QUEST_A_THE_SPEAKERS_PERSPECTIVE) == QUEST_STATUS_REWARDED && player->GetQuestStatus(QUEST_A_THE_SPEAKERS_CALL) == QUEST_STATUS_NONE) || player->IsGameMaster())
                    if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_A_THE_SPEAKERS_CALL))
                    {
                      //  Conversation::CreateConversation(6615, player, player->GetPosition(), { player->GetGUID() });
                        player->AddQuest(quest, nullptr);
                    }
            }
            else
            {
                ///HORDE
                if (player->GetQuestStatus(QUEST_SUMMONS_TO_ORGRIMMAR) == QUEST_STATUS_NONE)
                    if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_SUMMONS_TO_ORGRIMMAR))
                    {
                     //   Conversation::CreateConversation(6609, player, player->GetPosition(), { player->GetGUID() });
                        player->AddQuest(quest, nullptr);
                    }
            }
        }
        HandlePhase(player);
    }

    void HandlePhase(Player* player)
    {
        if (player->GetQuestStatus(QUEST_A_THE_SPEAKERS_PERSPECTIVE) == QUEST_STATUS_REWARDED || player->GetQuestStatus(QUEST_H_THE_SPEAKERS_PERSPECTIVE) == QUEST_STATUS_REWARDED || !player->IsGameMaster())
        {
            if (player->GetZoneId() == 1377 && player->GetAreaId() == 9310)
                PhasingHandler::AddPhase(player, PHASE_AFTER_QUEST, true);
        }
    }
};

#define GOSSIP_MASTER_MATHIAS_SHAW "Tell me What Happend。"
struct npc_master_mathias_shaw_132255 : public ScriptedAI
{
    npc_master_mathias_shaw_132255(Creature* creature) : ScriptedAI(creature) { Initialize(); }
    bool Intr;
    std::set<ObjectGuid> pList;
    ObjectGuid   m_playerGUID;
    void Initialize()
    {
        Intr = false;
        m_playerGUID = ObjectGuid::Empty;
    }

    void AddPlayer()
    {
        if (!HasPlayer(m_playerGUID))
            pList.insert(m_playerGUID);
    }

    bool HasPlayer(ObjectGuid guid)
    {
        return (pList.find(guid) != pList.end());
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsInWorld())
            return;
        if (!me->IsWithinDist(who, 15.0f, false))
            return;

        Player* player = who->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (!player)
            return;
        if (!HasPlayer(player->GetGUID()) && player->HasQuest(QUEST_SUMMONS_TO_STORMWIND))
        {
            m_playerGUID = player->GetGUID();
            AddPlayer();
            me->AI()->Talk(0, player);
            m_playerGUID = ObjectGuid::Empty;
        }
    }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_WITNESS_TO_THE_WOUND)
            player->AddConversationDelayedTeleport(5000, 6608, 1817, -6467.526f, -219.9097f, 5.90872f, 2.209932f);
    }

    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)  override
    {
        if (quest->GetQuestId() == QUEST_SUMMONS_TO_STORMWIND)
            if (const Quest* quest_gift = sObjectMgr->GetQuestTemplate(QUEST_GIFTS_OF_THE_FALLEN))
                player->AddQuest(quest_gift, me);
    }
    ///22009	33712	0 ClientToServer: CMSG_TALK_TO_GOSSIP (0x3495) 
    void sGossipHello(Player* player)
    {
        if (player->HasQuest(QUEST_SUMMONS_TO_STORMWIND)) {
            player->ForceCompleteQuest(QUEST_SUMMONS_TO_STORMWIND);
            ClearGossipMenuFor(player);
            if (me->IsQuestGiver())
                player->PrepareQuestMenu(me->GetGUID()); /* return true*/
            SendGossipMenuFor(player, 33712, me->GetGUID());
        }
        else if (player->GetQuestStatus(QUEST_GIFTS_OF_THE_FALLEN) == QUEST_STATUS_INCOMPLETE)
        {//player->GetQuestStatus(QUEST_SUMMONS_TO_STORMWIND)==QUEST_STATUS_REWARDED &&
         //TC_LOG_ERROR("server.worldserver", "QUEST_GIFTS_OF_THE_FALLEN OnGossipHello ");
         //if (creature->IsQuestGiver())
         //    player->PrepareQuestMenu(creature->GetGUID()); /* return true*/
         //SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
            ClearGossipMenuFor(player);
            //AddGossipItemFor(player, GOSSIP_ICON_CHAT, (GOSSIP_MASTER_MATHIAS_SHAW), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            SendGossipMenuFor(player, 33712, me->GetGUID());
        }
        else
        {
            ClearGossipMenuFor(player);
            if (me->IsQuestGiver())
                player->PrepareQuestMenu(me->GetGUID()); /* return true*/
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
        }
    }

    void sGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId)
    {
        CloseGossipMenuFor(player);
        if (player->HasQuest(QUEST_GIFTS_OF_THE_FALLEN))
        {
            player->CastSpell(player, SCENE_GIFTS_OF_THE_FALLEN, true);
            player->KilledMonsterCredit(296253);
            player->KilledMonsterCredit(KILLED_MONSTER_CREDIT_QUEST_GIFTS_OF_THE_FALLEN); //KILLED_MONSTER_CREDIT_QUEST_GIFTS_OF_THE_FALLEN
            player->ForceCompleteQuest(QUEST_GIFTS_OF_THE_FALLEN);
        }
    }
};

enum
{
    AURA_TIME_TRAVELLING = 255152, ///262016 
    SPELL_BLACK = 129809,
};
#define GOSSIP_ZIDORMI_1 "Can you show me what Silithus was like before becoming traumatized in the world？"
#define GOSSIP_ZIDORMI_2 "Can you take me back now？"
struct npc_zidormi_128607 : public ScriptedAI
{
    npc_zidormi_128607(Creature* creature) : ScriptedAI(creature) { Initialize(); }

    std::set<ObjectGuid> pList;
    ObjectGuid   m_playerGUID;
    void Initialize()
    {
        m_playerGUID = ObjectGuid::Empty;
    }

    void AddPlayer()
    {
        if (!HasPlayer(m_playerGUID))
            pList.insert(m_playerGUID);
    }

    bool HasPlayer(ObjectGuid guid)
    {
        return (pList.find(guid) != pList.end());
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsInWorld())
            return;
        if (!me->IsWithinDist(who, 15.0f, false))
            return;

        Player* player = who->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (!player)
            return;

        if (!HasPlayer(player->GetGUID()) && me->GetMapId() == 1)
        {
            m_playerGUID = player->GetGUID();
            AddPlayer();
            me->AI()->Talk(0, player); /// 
            m_playerGUID = ObjectGuid::Empty;
        }
    }

    void sGossipHello(Player* player)
    {
        ClearGossipMenuFor(player);
        if (player->GetMapId() == 1)
        {
            ///GossipID: 21720   TextID : 33094  GossipOptions : 1    GossipText : 0
            //AddGossipItemFor(player, GOSSIP_ICON_CHAT, (GOSSIP_ZIDORMI_2), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            SendGossipMenuFor(player, 33094, me->GetGUID());
        }
        else
        {
            ///GossipID: 21720 TextID: 33093 GossipOptions: 1  GossipText: 0
            //AddGossipItemFor(player, GOSSIP_ICON_CHAT, (GOSSIP_ZIDORMI_1), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            SendGossipMenuFor(player, 33093, me->GetGUID());
        }
    }

    void sGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId)
    {
        CloseGossipMenuFor(player);

        if (player->GetMapId() == 1)
        {
            player->RemoveAurasDueToSpell(AURA_TIME_TRAVELLING);
            player->CastSpell(player, SPELL_BLACK);
            player->SeamlessTeleportToMap(1817);
        }
        else
        {
            player->CastSpell(player, AURA_TIME_TRAVELLING, true);
            player->TeleportTo(1, -6467.526f, -219.9097f, 5.90872f, 2.209932f);
        }
    }
};

struct npc_nolan_speed_131963 : public ScriptedAI
{
    npc_nolan_speed_131963(Creature* creature) : ScriptedAI(creature) { }

    enum Texts
    {
        SAY_FIRST_LINE = 0, ///The order we received was to use all necessary means to obtain this mysterious mineral.
        SAY_SECOND_LINE = 1,///If Twilight's Hammer rises again, we can't bear it.The voidwalker must die！
        SAY_THIRD_LINE = 2 ///I think the situation of Silithus is already under our control. The next action will be continued by MI7. thanks again，$n。
    };

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_FREE_SAMPLES)
            Talk(SAY_FIRST_LINE, player);
        if (quest->GetQuestId() == QUEST_THE_TWILIGHT_SURVIVOR)
            Talk(SAY_SECOND_LINE, player);
    }

    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)  override
    {
        if (quest->GetQuestId() == QUEST_A_THE_BLOOD_OF_AZEROTH)
            Talk(SAY_THIRD_LINE, player);
    }

};

struct npc_kelsey_steelspark_130030 : public ScriptedAI
{
    npc_kelsey_steelspark_130030(Creature* creature) : ScriptedAI(creature) { }
    enum Texts
    {
        SAY_FIRST_LINE = 0, ///These insatiable goblins! I really want to see the look on their faces when they found the cutting machine was blown to pieces！ 
    };

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_ITS_A_SABOTAGE)
            Talk(SAY_FIRST_LINE, player);
    }
};

struct npc_tammy_tinkspinner_132606 : public ScriptedAI
{
    npc_tammy_tinkspinner_132606(Creature* creature) : ScriptedAI(creature) { }

    enum Texts
    {
        SAY_FIRST_LINE = 0, ///We must figure out how the Silithid Scorpion survived the impact of the blade of Sargeras！
        SAY_SECOND_LINE = 1,///Thanks to you, we are now one step closer to understanding the impact of this sword on Silithus！
    };

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_LARVAE_BY_THE_DOZEN)
            Talk(SAY_FIRST_LINE, player);
    }

    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)  override
    {
        if (quest->GetQuestId() == QUEST_LARVAE_BY_THE_DOZEN)
            Talk(SAY_SECOND_LINE, player);
    }
};

struct npc_khargus_stonemantle_132192 : public ScriptedAI
{
    npc_khargus_stonemantle_132192(Creature* creature) : ScriptedAI(creature) { }

    enum Texts
    {
        SAY_FIRST_LINE = 0, /// I have never seen anything like this in my life!What is this？
    };
    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)  override
    {
        if (quest->GetQuestId() == QUEST_FREE_SAMPLES)
            Talk(SAY_FIRST_LINE, player);
    }
};

enum
{
    POINT_JUMP = 0,
    POINT_DESPAWN = 1,

    CONVERSATION_TALK = 6623,
};

struct npc_archmage_khadgar_130032 : public ScriptedAI
{
    npc_archmage_khadgar_130032(Creature* creature) : ScriptedAI(creature) { }


    void UpdateAI(uint32 diff) override
    {
        _scheduler.Update(diff);
    }

    void MovementInform(uint32 /*type*/, uint32 id) override
    {
        switch (id)
        {
        case POINT_JUMP:
            _scheduler.Schedule(250ms, [this](TaskContext /*context*/)
            {
                // me->GetMotionMaster()->MoveJump({ 1107.84f, 7222.57f, 38.9725f, me->GetOrientation() }, me->GetSpeed(MOVE_RUN), 1.4125f, POINT_DESPAWN);
            });
            break;
        case POINT_DESPAWN:
            me->DisappearAndDie();
            break;
        default:
            break;
        }
    }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_A_THE_SPEAKERS_PERSPECTIVE)
            Conversation::CreateConversation(CONVERSATION_TALK, player, player->GetPosition(), { player->GetGUID() });
    }
private:
    TaskScheduler _scheduler;
};

enum
{
    NPC_ANGUS_STORMBREW = 132684, // angus - stormbrew
    GO_TWILIGHT_OUTHOUSE = 280948, //Twilight Outhouse
    QUEST_A_WEE_BIT_O_CLOTH = 50229,//a-wee-bit-o-cloth
    SPELL_EXPLOSION = 261618, //Explosion
    SPELL_ON_FIRE = 261604, //on-fire
    CONVERSATION_ON_QUEST_ACCEPT = 6723,
    CONVERSATION_ON_QUEST_COMPLETE = 6724,
};
class go_twilight_outhouse : public GameObjectScript
{
public:
    go_twilight_outhouse() : GameObjectScript("go_twilight_outhouse") { }

    bool OnQuestAccept(Player* player, GameObject* /*go*/, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_A_WEE_BIT_O_CLOTH)
            Conversation::CreateConversation(CONVERSATION_ON_QUEST_ACCEPT, player, player->GetPosition(), { player->GetGUID() });
        return true;
    }

    bool OnQuestReward(Player* player, GameObject* /*go*/, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_A_WEE_BIT_O_CLOTH)
            Conversation::CreateConversation(CONVERSATION_ON_QUEST_COMPLETE, player, player->GetPosition(), { player->GetGUID() });
        return true;
    }
};

///HORE
struct npc_pixni_rustbomb_132040 : public ScriptedAI
{
    npc_pixni_rustbomb_132040(Creature* creature) : ScriptedAI(creature) { }
    enum Texts
    {
        SAY_FIRST_LINE = 0, ///Don’t be afraid to get your hands dirty and smash the heads of these alien scorpions.！
        SAY_SECOND_LINE = 1,///This research can help us understand the power of that giant sword！
    };

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_THE_SOURCE_OF_POWER)
            Talk(SAY_FIRST_LINE, player);
    }

    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)  override
    {
        if (quest->GetQuestId() == QUEST_THE_SOURCE_OF_POWER)
            Talk(SAY_SECOND_LINE, player);
    }
};

///
struct npc_gizmax_greasebolt_132045 : public ScriptedAI
{
    npc_gizmax_greasebolt_132045(Creature* creature) : ScriptedAI(creature) { }
    enum
    {
        SAY_FIRST_LINE = 0,
    };
    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_GIZMAX_GREASEBOLT)
            Talk(SAY_FIRST_LINE, player);
    }
};

struct npc_grol_warblade_132147 : public ScriptedAI
{
    npc_grol_warblade_132147(Creature* creature) : ScriptedAI(creature) { }
    enum Texts
    {
        SAY_FIRST_LINE = 0, ///Go over there and let those lazy prospectors know their weight！
        SAY_SECOND_LINE = 1,///We cannot let this madman gather the survivors together! Kill him and bring me his emblem！
        SAY_THIRD_LINE = 2, /// Silithus situation is under control
    };

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_LAZY_PROSPECTORS)
            Talk(SAY_FIRST_LINE, player);
        if (quest->GetQuestId() == QUEST_H_THE_TWILIGHT_SURVIVOR)
            Talk(SAY_SECOND_LINE, player);
    }

    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)  override
    {
        if (quest->GetQuestId() == QUEST_H_THE_BLOOD_OF_AZEROTH)
            Talk(SAY_THIRD_LINE, player);
    }
};

struct npc_lazy_prospector_133019 : public ScriptedAI
{
    npc_lazy_prospector_133019(Creature* creature) : ScriptedAI(creature) { }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        {
            if (player->GetQuestStatus(QUEST_LAZY_PROSPECTORS) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(me->GetEntry());
                me->DespawnOrUnsummon(5000);
                Position runPos = me->GetPosition();
                GetPositionWithDistInFront(me, 15.f, runPos);
                me->SetSpeed(MOVE_RUN, 1.8f);
                me->GetMotionMaster()->MovePoint(1, runPos);
                ///TALK RANDOM 0-5
                Talk(urand(0, 5), player);
                me->GetMotionMaster()->Clear();
            }
        }
    }
};

struct npc_archmage_khadgar_130033 : public ScriptedAI
{
    npc_archmage_khadgar_130033(Creature* creature) : ScriptedAI(creature) { }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_THE_SPEAKERS_PERSPECTIVE)
            Conversation::CreateConversation(CONVERSATION_TALK, player, player->GetPosition(), { player->GetGUID() });
    }
};

///-- PHASE 10401
#define GOSSIP_NATHANOS_BLIGHTCALLER "Tell me what happened at the party"
struct npc_nathanos_blightcaller_132254 : public ScriptedAI
{
    npc_nathanos_blightcaller_132254(Creature* creature) : ScriptedAI(creature) { Initialize(); }

    std::set<ObjectGuid> pList;
    ObjectGuid   m_playerGUID;
    void Initialize()
    {
        m_playerGUID = ObjectGuid::Empty;
    }

    void AddPlayer()
    {
        if (!HasPlayer(m_playerGUID))
            pList.insert(m_playerGUID);
    }

    bool HasPlayer(ObjectGuid guid)
    {
        return (pList.find(guid) != pList.end());
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsInWorld())
            return;
        if (!me->IsWithinDist(who, 15.0f, false))
            return;

        Player* player = who->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (!player)
            return;
        if (!HasPlayer(player->GetGUID()) && player->HasQuest(QUEST_SUMMONS_TO_ORGRIMMAR))
        {
            m_playerGUID = player->GetGUID();
            AddPlayer();
            //me->AI()->Talk(0, player);
            m_playerGUID = ObjectGuid::Empty;
        }
    }

    void sGossipHello(Player* player)
    {
        if (player->HasQuest(QUEST_SUMMONS_TO_ORGRIMMAR)) {
            player->ForceCompleteQuest(QUEST_SUMMONS_TO_ORGRIMMAR);
            ClearGossipMenuFor(player);
            if (me->IsQuestGiver())
                player->PrepareQuestMenu(me->GetGUID()); /* return true*/
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
        }
        else if (player->GetQuestStatus(QUEST_A_RECENT_DISCOVERY) == QUEST_STATUS_INCOMPLETE)
        {
            ClearGossipMenuFor(player);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, (GOSSIP_NATHANOS_BLIGHTCALLER), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
        }
        else
        {
            ClearGossipMenuFor(player);
            if (me->IsQuestGiver())
                player->PrepareQuestMenu(me->GetGUID()); /* return true*/
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
        }
    }
    void sGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId)
    {
        CloseGossipMenuFor(player);

        if (player->HasQuest(QUEST_A_RECENT_DISCOVERY))
        {
            TC_LOG_ERROR("server.worldserver", " sGossipSelect 2");
            player->CastSpell(player, SCENE_A_RECENT_DISCOVERY, true);
        }
    }

    void sQuestAccept(Player* player, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_H_WITNESS_TO_THE_WOUND)
            player->AddConversationDelayedTeleport(5000, 6610, 1817, -6467.526f, -219.9097f, 5.90872f, 2.209932f);
    }

    void sQuestReward(Player* player, Quest const* quest, uint32 /*opt*/)
    {
        if (quest->GetQuestId() == QUEST_SUMMONS_TO_ORGRIMMAR)
            if (const Quest* quest_gift = sObjectMgr->GetQuestTemplate(QUEST_A_RECENT_DISCOVERY))
                player->AddQuest(quest_gift, me);
    }
};

struct npc_laurette_royston_132995 : public ScriptedAI
{
    npc_laurette_royston_132995(Creature* creature) : ScriptedAI(creature) { }

    void sGossipHello(Player* player)
    {
        if (player->GetQuestStatus(QUEST_A_THE_SPEAKERS_CALL) == QUEST_STATUS_INCOMPLETE || player->IsGameMaster())
        {
            ClearGossipMenuFor(player);
            //AddGossipItemFor(player, GOSSIP_ICON_CHAT, string_To_UTF8(GOSSIP_NATHANOS_BLIGHTCALLER), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            AddGossipItemFor(player, 22129, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());
        }
    }

    void sGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId)
    {
        CloseGossipMenuFor(player);
        if (player->HasQuest(QUEST_A_THE_SPEAKERS_CALL))
        {
            ///taxipath tele to
            player->CastSpell(player, 262048, true);
            PhasingHandler::AddPhase(player, PHASE_AFTER_QUEST, true);
            player->TeleportTo(1817, -7482.873f, 1822.149f, 1076.7f, 3.63f);
            ///player -> cast 46598 to Vehicle Entry: 133274
        }
    }
};

class scene_the_power_in_our_hands : public SceneScript
{
public:
    scene_the_power_in_our_hands() : SceneScript("scene_the_power_in_our_hands") { }

    void OnSceneEnd(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) override
    {
        player->KilledMonsterCredit(133395);

        //player->ForceCompleteQuest(SCENE_THE_POWER_IN_OUR_HANDS);
        ///achievementID 12071
        ///TO DO add full power to AP
    }
};

struct npc_kaylie_benson_133254 : public ScriptedAI
{
    npc_kaylie_benson_133254(Creature* creature) : ScriptedAI(creature) { }

    void sGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId)
    {
        CloseGossipMenuFor(player);
        PhasingHandler::RemovePhase(player, PHASE_AFTER_QUEST, true);
        player->TeleportTo(1817, -6419.63f, 180.85f, 8.52f, 4.419f);
    }
};

void AddSC_silithus()
{
    new On110Silithus();
    new go_crystalline_tear();
    new npc_anachronos_quest_trigger();
    new npc_anachronos_the_ancient();
    new npc_qiraj_war_spawn();
    new go_wind_stone();
    RegisterCreatureAI(npc_magni_bronzebeard_silithus);
    RegisterCreatureAI(npc_magni_bronzebeard_heart_chamber);
    RegisterSceneScript(scene_get_heart_of_azeroth);
    RegisterGameObjectAI(go_azeroth_heart_chamber_titan_console);
    RegisterCreatureAI(npc_azeroth_heart_chamber_azerite_wound);
    RegisterSpellScript(spell_azeroth_heart_chamber_heart_of_azeroth);
    RegisterGameObjectAI(go_azeroth_heart_chamber_teleport_pad);
    RegisterCreatureAI(npc_master_mathias_shaw_132255);
    RegisterCreatureAI(npc_zidormi_128607);
    RegisterCreatureAI(npc_nolan_speed_131963);
    RegisterCreatureAI(npc_kelsey_steelspark_130030);
    RegisterCreatureAI(npc_tammy_tinkspinner_132606);
    RegisterCreatureAI(npc_khargus_stonemantle_132192);
    RegisterCreatureAI(npc_archmage_khadgar_130032);
    new go_twilight_outhouse();
    RegisterCreatureAI(npc_pixni_rustbomb_132040);
    RegisterCreatureAI(npc_gizmax_greasebolt_132045);
    RegisterCreatureAI(npc_grol_warblade_132147);
    RegisterCreatureAI(npc_lazy_prospector_133019);
    RegisterCreatureAI(npc_archmage_khadgar_130033);
    RegisterCreatureAI(npc_nathanos_blightcaller_132254);
    RegisterCreatureAI(npc_laurette_royston_132995);
    new scene_the_power_in_our_hands();
    RegisterCreatureAI(npc_kaylie_benson_133254);
    RegisterGameObjectAI(go_titan_translocator);
}
