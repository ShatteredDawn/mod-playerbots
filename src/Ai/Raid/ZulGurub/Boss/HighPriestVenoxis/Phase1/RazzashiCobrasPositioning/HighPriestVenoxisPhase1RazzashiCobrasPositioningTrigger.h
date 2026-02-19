#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "../../facade/HighPriestVenoxisFacade.h"

class HighPriestVenoxisPhase1RazzashiCobrasPositioningTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1RazzashiCobrasPositioningTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 razzashi cobras positioning") {}

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

        if (this->botAI->IsTank(this->bot) == false)
        {
            return false;
        }

        if (this->botAI->IsMainTank(this->bot) == true)
        {
            return false;
        }

        if (HighPriestVenoxisFacade::IsInCombatWithVenoxis(*this->bot) == false)
        {
            return false;
        }

        const std::vector<Unit*> razzashiCobras = HighPriestVenoxisFacade::FindRazzashiCobras(*this->bot);

        if (razzashiCobras.empty())
        {
            return false;
        }

        Unit* const firstCobra = razzashiCobras.front();

        if (firstCobra == nullptr)
        {
            return false;
        }

        if (firstCobra->IsAlive() == false)
        {
            return false;
        }

        const Position cobrasIdealPosition = HighPriestVenoxisFacade::GetRazzashiCobrasPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(cobrasIdealPosition.GetPositionX(), cobrasIdealPosition.GetPositionY());
        static constexpr float maxDistance = HighPriestVenoxisFacade::GetRazzashiCobrasMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        return true;
    }
};
