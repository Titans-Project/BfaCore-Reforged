#include "ScriptMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "InstanceScript.h"
#include "Map.h"
#include "Player.h"
#include "scholomance.h"

Position const GandlingLoc = { 180.7712f, -5.428603f, 75.57024f, 1.291544f };

ObjectGuid GateKirtonosGUID;
ObjectGuid GateGandlingGUID;
ObjectGuid GateMiliciaGUID;
ObjectGuid GateTheolenGUID;
ObjectGuid GatePolkeltGUID;
ObjectGuid GateRavenianGUID;
ObjectGuid GateBarovGUID;
ObjectGuid GateIlluciaGUID;
ObjectGuid BrazierOfTheHeraldGUID;

struct instance_scholomance : public InstanceScript
{
    instance_scholomance(InstanceMap* map) : InstanceScript(map)
    {
        SetHeaders(DataHeader);
        SetBossNumber(EncounterCount);
    }

    void OnGameObjectCreate(GameObject* go) override
    {
        switch (go->GetEntry())
        {
        case GO_GATE_KIRTONOS:
            GateKirtonosGUID = go->GetGUID();
            break;
        case GO_GATE_GANDLING:
            GateGandlingGUID = go->GetGUID();
            break;
        case GO_GATE_MALICIA:
            GateMiliciaGUID = go->GetGUID();
            break;
        case GO_GATE_THEOLEN:
            GateTheolenGUID = go->GetGUID();
            break;
        case GO_GATE_POLKELT:
            GatePolkeltGUID = go->GetGUID();
            break;
        case GO_GATE_RAVENIAN:
            GateRavenianGUID = go->GetGUID();
            break;
        case GO_GATE_BAROV:
            GateBarovGUID = go->GetGUID();
            break;
        case GO_GATE_ILLUCIA:
            GateIlluciaGUID = go->GetGUID();
            break;
        case GO_BRAZIER_OF_THE_HERALD:
            BrazierOfTheHeraldGUID = go->GetGUID();
            break;
        default:
            break;
        }
    }

    bool SetBossState(uint32 type, EncounterState state) override
    {
        if (!InstanceScript::SetBossState(type, state))
            return false;

        switch (type)
        {
        case DATA_LORDALEXEIBAROV:
        case DATA_DOCTORTHEOLENKRASTINOV:
        case DATA_THERAVENIAN:
        case DATA_LOREKEEPERPOLKELT:
        case DATA_INSTRUCTORMALICIA:
        case DATA_LADYILLUCIABAROV:
            CheckToSpawnGandling();
            break;
        default:
            break;
        }

        return true;
    }

    ObjectGuid GetGuidData(uint32 type) const override
    {
        switch (type)
        {
        case GO_GATE_KIRTONOS:
            return GateKirtonosGUID;
        case GO_GATE_GANDLING:
            return GateGandlingGUID;
        case GO_GATE_MALICIA:
            return GateMiliciaGUID;
        case GO_GATE_THEOLEN:
            return GateTheolenGUID;
        case GO_GATE_POLKELT:
            return GatePolkeltGUID;
        case GO_GATE_RAVENIAN:
            return GateRavenianGUID;
        case GO_GATE_BAROV:
            return GateBarovGUID;
        case GO_GATE_ILLUCIA:
            return GateIlluciaGUID;
        case GO_BRAZIER_OF_THE_HERALD:
            return BrazierOfTheHeraldGUID;
        default:
            break;
        }

        return ObjectGuid::Empty;
    }

    bool CheckPreBosses(uint32 bossId) const
    {
        switch (bossId)
        {
        case DATA_DARKMASTERGANDLING:
            if (GetBossState(DATA_LORDALEXEIBAROV) != DONE)
                return false;
            if (GetBossState(DATA_DOCTORTHEOLENKRASTINOV) != DONE)
                return false;
            if (GetBossState(DATA_THERAVENIAN) != DONE)
                return false;
            if (GetBossState(DATA_LOREKEEPERPOLKELT) != DONE)
                return false;
            if (GetBossState(DATA_INSTRUCTORMALICIA) != DONE)
                return false;
            if (GetBossState(DATA_LADYILLUCIABAROV) != DONE)
                return false;
            if (GetBossState(DATA_DARKMASTERGANDLING) == DONE)
                return false;
            break;
        default:
            break;
        }

        return true;
    }

    void CheckToSpawnGandling()
    {
        if (CheckPreBosses(DATA_DARKMASTERGANDLING))
            instance->SummonCreature(NPC_DARKMASTER_GANDLING, GandlingLoc);
    }

    void ReadSaveDataMore(std::istringstream& /*data*/) override
    {
        CheckToSpawnGandling();
    }
};

void AddSC_instance_scholomance()
{
    RegisterInstanceScript(instance_scholomance, 1007);
}