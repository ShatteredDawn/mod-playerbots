#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "../../facade/HighPriestVenoxisFacade.h"

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

        const std::vector<Unit*> razzashiCobras = HighPriestVenoxisFacade::FindRazzashiCobras(*this->bot);

        if (razzashiCobras.empty())
        {
            return false;
        }

        Unit* const firstCobra = razzashiCobras.front();

        this->Attack(firstCobra);

        return true;
    }
};
