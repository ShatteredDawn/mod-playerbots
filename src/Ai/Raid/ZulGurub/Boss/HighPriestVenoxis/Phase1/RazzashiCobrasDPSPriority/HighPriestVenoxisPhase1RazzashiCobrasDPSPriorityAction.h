#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityAction : public AttackAction
{
public:
    HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityAction(
        PlayerbotAI* botAI,
        const std::string name = "high priest venoxis phase 1 razzashi cobras dps priority"
    ) : AttackAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();

        const std::vector<Unit*> razzashiCobras = highPriestVenoxisAssistant.findRazzashiCobras(*this->bot);

        if (razzashiCobras.empty())
        {
            return false;
        }

        Unit* const firstCobra = razzashiCobras.front();

        this->Attack(firstCobra);

        return true;
    }
};
