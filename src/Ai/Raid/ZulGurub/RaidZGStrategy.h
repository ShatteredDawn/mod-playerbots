#pragma once

#include "Boss/HighPriestVenoxis/Phase1/HolyWrath/HighPriestVenoxisPhase1HolyWrathAction.h"
#include "Boss/HighPriestVenoxis/Phase1/HolyWrath/HighPriestVenoxisPhase1HolyWrathMultiplier.h"
#include "Boss/HighPriestVenoxis/Phase1/RazzashiCobrasDPSPriority/HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityAction.h"
#include "Boss/HighPriestVenoxis/Phase1/RazzashiCobrasPositioning/HighPriestVenoxisPhase1RazzashiCobrasPositioningAction.h"
#include "Boss/HighPriestVenoxis/Phase1/VenoxisPositioning/HighPriestVenoxisPhase1VenoxisPositioningAction.h"
#include "CreateNextAction.h"
#include "Strategy.h"
#include "Multiplier.h"
#include "Trash/GurubashiBatRider/GurubashiBatRiderUnstableConcoctionAction.h"
#include "ZulGurub/Trash/GurubashiBatRider/GurubashiBatRiderUnstableConcoctionMultiplier.h"

class RaidZGStrategy : public Strategy
{
public:
    RaidZGStrategy(PlayerbotAI* botAI) : Strategy(botAI) {}

    const std::string getName() override
    {
        return "zg";
    }

    void InitTriggers(std::vector<TriggerNode*>& triggers) override
    {
        // Trash mobs

        triggers.push_back(
            new TriggerNode(
                "gurubashi bat rider unstable concoction",
                {
                    CreateNextAction<GurubashiBatRiderUnstableConcoctionAction>(ACTION_EMERGENCY + 10.0f)
                }
            )
        );

        // High Priest Venoxis

        triggers.push_back(
            new TriggerNode(
                "high priest venoxis phase 1 venoxis positioning",
                {
                    CreateNextAction<HighPriestVenoxisPhase1VenoxisPositioningAction>(ACTION_EMERGENCY + 9.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "high priest venoxis phase 1 razzashi cobras positioning",
                {
                    CreateNextAction<HighPriestVenoxisPhase1RazzashiCobrasPositioningAction>(ACTION_EMERGENCY + 9.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "high priest venoxis phase 1 razzashi cobras dps priority",
                {
                    CreateNextAction<HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityAction>(ACTION_HIGH + 1.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "high priest venoxis phase 1 holy wrath",
                {
                    CreateNextAction<HighPriestVenoxisPhase1HolyWrathAction>(ACTION_EMERGENCY + 10.0f)
                }
            )
        );
    }

    void InitMultipliers(std::vector<Multiplier*>& multipliers) override
    {
        multipliers.push_back(new GurubashiBatRiderUnstableConcoctionMultiplier(this->botAI));
        multipliers.push_back(new HighPriestVenoxisPhase1HolyWrathMultiplier(this->botAI));
    }
};
