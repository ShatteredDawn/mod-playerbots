#pragma once

#include "AiObjectContext.h"
#include "HighPriestVenoxisPhase1HolyWrathTrigger.h"
#include "Trash/GurubashiBatRider/GurubashiBatRiderUnstableConcoctionTrigger.h"
#include "Boss/HighPriestVenoxis/Phase1/VenoxisPositioning/HighPriestVenoxisPhase1VenoxisPositioningTrigger.h"
#include "Boss/HighPriestVenoxis/Phase1/RazzashiCobrasPositioning/HighPriestVenoxisPhase1RazzashiCobrasPositioningTrigger.h"
#include "Boss/HighPriestVenoxis/Phase1/RazzashiCobrasDPSPriority/HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityTrigger.h"

class RaidZGTriggerContext : public NamedObjectContext<Trigger>
{
public:
    RaidZGTriggerContext()
    {
        // Trash
        creators["gurubashi bat rider unstable concoction"] = &RaidZGTriggerContext::gurubashiBatRiderUnstableConcoction;
        creators["high priest venoxis phase 1 holy wrath"] = &RaidZGTriggerContext::highPriestVenoxisPhase1HolyWrath;
        creators["high priest venoxis phase 1 venoxis positioning"] = &RaidZGTriggerContext::highPriestVenoxisPhase1VenoxisPositioning;
        creators["high priest venoxis phase 1 razzashi cobras positioning"] = &RaidZGTriggerContext::highPriestVenoxisPhase1RazzashiCobrasPositioning;
        creators["high priest venoxis phase 1 razzashi cobras dps priority"] = &RaidZGTriggerContext::highPriestVenoxisPhase1RazzashiCobrasDPSPriority;
    }

private:
    // Trash
    static Trigger* gurubashiBatRiderUnstableConcoction(PlayerbotAI* botAI)
    {
        return new GurubashiBatRiderUnstableConcoctionTrigger(botAI);
    }

    // High Priest Venoxis
    static Trigger* highPriestVenoxisPhase1HolyWrath(PlayerbotAI* botAI)
    {
        return new HighPriestVenoxisPhase1HolyWrathTrigger(botAI);
    }

    static Trigger* highPriestVenoxisPhase1VenoxisPositioning(PlayerbotAI* botAI)
    {
        return new HighPriestVenoxisPhase1VenoxisPositioningTrigger(botAI);
    }

    static Trigger* highPriestVenoxisPhase1RazzashiCobrasPositioning(PlayerbotAI* botAI)
    {
        return new HighPriestVenoxisPhase1RazzashiCobrasPositioningTrigger(botAI);
    }

    static Trigger* highPriestVenoxisPhase1RazzashiCobrasDPSPriority(PlayerbotAI* botAI)
    {
        return new HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityTrigger(botAI);
    }
};
