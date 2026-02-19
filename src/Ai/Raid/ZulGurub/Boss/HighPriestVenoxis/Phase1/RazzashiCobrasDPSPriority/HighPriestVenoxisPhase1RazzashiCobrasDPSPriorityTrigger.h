#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "../../facade/HighPriestVenoxisFacade.h"

class HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 razzashi cobras dps priority") {}

    bool IsActive() override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        if (this->botAI == nullptr)
        {
            return false;
        }

        if (HighPriestVenoxisFacade::IsInCombatWithVenoxis(*this->bot) == false)
        {
            return false;
        }

        if (this->botAI->IsMainTank(this->bot) == true || this->botAI->IsHeal(this->bot) == true)
        {
            return false;
        }

        const std::vector<Unit*> razzashiCobras = HighPriestVenoxisFacade::FindRazzashiCobras(*this->bot);

        if (razzashiCobras.empty())
        {
            return false;
        }

        const Unit* const target = this->bot->GetVictim();

        if (target == razzashiCobras.front())
        {
            return false;
        }

        return true;
    }
};
