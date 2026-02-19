#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "../../facade/HighPriestVenoxisFacade.h"

class HighPriestVenoxisPhase1HolyWrathTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1HolyWrathTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 holy wrath") {}

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

        if (this->botAI->IsMainTank(this->bot) == true)
        {
            return false;
        }

        if (HighPriestVenoxisFacade::IsInPhase1(*this->bot) == false)
        {
            return false;
        }

        if (HighPriestVenoxisFacade::IsAtSafeDistanceFromVenoxis(*this->bot) == true)
        {
            return false;
        }

        return true;
    }
};
