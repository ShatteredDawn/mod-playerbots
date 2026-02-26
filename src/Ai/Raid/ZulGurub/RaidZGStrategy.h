#pragma once

#include "Boss/HighPriestVenoxis/Phase1/HolyWrath/HighPriestVenoxisPhase1HolyWrathAction.h"
#include "Boss/HighPriestVenoxis/Phase1/HolyWrath/HighPriestVenoxisPhase1HolyWrathMultiplier.h"
#include "Boss/GrilekTheWanderer/mechanics/Avatar/GrilekTheWandererAvatarAction.h"
#include "Boss/GrilekTheWanderer/mechanics/Avatar/GrilekTheWandererAvatarMultiplier.h"
#include "Boss/HighPriestVenoxis/Phase1/RazzashiCobrasDPSPriority/HighPriestVenoxisPhase1RazzashiCobrasDPSPriorityAction.h"
#include "Boss/HighPriestVenoxis/Phase1/RazzashiCobrasPositioning/HighPriestVenoxisPhase1RazzashiCobrasPositioningAction.h"
#include "Boss/HighPriestVenoxis/Phase1/VenoxisPositioning/HighPriestVenoxisPhase1VenoxisPositioningAction.h"
#include "CreateNextAction.h"
#include "HakkarTheSoulflayer/mechanics/son-of-hakkar/BringBackSonOfHakkar/HakkarTheSoulflayerBringBackSonOfHakkarAction.h"
#include "HakkarTheSoulflayer/mechanics/cause-insanity/CauseInsanity/HakkarTheSoulflayerCauseInsanityAction.h"
#include "HakkarTheSoulflayer/mechanics/threat/ExcessiveThreat/HakkarTheSoulflayerExcessiveThreatAction.h"
#include "HakkarTheSoulflayer/mechanics/blood-siphon/GoToPoisonousCloud/HakkarTheSoulflayerGoToPoisonousCloudAction.h"
#include "HakkarTheSoulflayer/mechanics/blood-siphon/GoToPoisonousCloud/HakkarTheSoulflayerGoToPoisonousCloudMultiplier.h"
#include "HakkarTheSoulflayer/mechanics/positioning/HakkarPositioning/HakkarTheSoulflayerHakkarPositioningAction.h"
#include "HakkarTheSoulflayer/mechanics/son-of-hakkar/MoveToSonOfHakkar/HakkarTheSoulflayerMoveToSonOfHakkarAction.h"
#include "HakkarTheSoulflayer/mechanics/blood-siphon/PoisonousBlood/HakkarTheSoulflayerPoisonousBloodMultiplier.h"
#include "HakkarTheSoulflayer/mechanics/son-of-hakkar/PullSonOfHakkar/HakkarTheSoulflayerPullSonOfHakkarAction.h"
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

        // Gri'lek The Wanderer

        triggers.push_back(
            new TriggerNode(
                "grilek the wanderer avatar",
                {
                    CreateNextAction<GrilekTheWandererAvatarAction>(ACTION_EMERGENCY + 10.0f)
                }
            )
        );

        // Hakkar the Soulflayer
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer hakkar positioning",
                {
                    CreateNextAction<HakkarTheSoulflayerHakkarPositioningAction>(ACTION_EMERGENCY + 10.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer cause insanity",
                {
                    CreateNextAction<HakkarTheSoulflayerCauseInsanityAction>(ACTION_EMERGENCY + 9.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer excessive threat",
                {
                    CreateNextAction<HakkarTheSoulflayerExcessiveThreatAction>(ACTION_EMERGENCY + 1.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer go to poisonous cloud",
                {
                    CreateNextAction<HakkarTheSoulflayerGoToPoisonousCloudAction>(ACTION_EMERGENCY + 10.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer move to son of hakkar",
                {
                    CreateNextAction<HakkarTheSoulflayerMoveToSonOfHakkarAction>(ACTION_EMERGENCY + 9.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer pull son of hakkar",
                {
                    CreateNextAction<HakkarTheSoulflayerPullSonOfHakkarAction>(ACTION_EMERGENCY + 10.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hakkar the soulflayer bring back son of hakkar",
                {
                    CreateNextAction<HakkarTheSoulflayerBringBackSonOfHakkarAction>(ACTION_EMERGENCY + 11.0f)
                }
            )
        );
    }

    void InitMultipliers(std::vector<Multiplier*>& multipliers) override
    {
        // Trash mobs
        multipliers.push_back(new GurubashiBatRiderUnstableConcoctionMultiplier(this->botAI));

        // High Priest Venoxis
        multipliers.push_back(new HighPriestVenoxisPhase1HolyWrathMultiplier(this->botAI));

        // Gri'lek The Wanderer
        multipliers.push_back(new GrilekTheWandererAvatarMultiplier(this->botAI));

        // Hakkar the Soulflayer
        multipliers.push_back(new HakkarTheSoulflayerPoisonousBloodMultiplier(this->botAI));
        multipliers.push_back(new HakkarTheSoulflayerGoToPoisonousCloudMultiplier(this->botAI));
        // multipliers.push_back(new HakkarTheSoulflayerThreatManagementMultiplier(this->botAI));
    }
};
