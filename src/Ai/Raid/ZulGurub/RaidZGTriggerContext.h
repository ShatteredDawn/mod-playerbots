#pragma once

#include "AiObjectContext.h"
#include "BloodlordMandokirWhirlwindTrigger.h"
#include "GrilekTheWandererAvatarTrigger.h"
#include "HakkarTheSoulflayer/mechanics/son-of-hakkar/BringBackSonOfHakkar/HakkarTheSoulflayerBringBackSonOfHakkarTrigger.h"
#include "HakkarTheSoulflayer/mechanics/blood-siphon/GoToPoisonousCloud/HakkarTheSoulflayerGoToPoisonousCloudTrigger.h"
#include "HakkarTheSoulflayer/mechanics/cause-insanity/CauseInsanity/HakkarTheSoulflayerCauseInsanityTrigger.h"
#include "HakkarTheSoulflayer/mechanics/threat/ExcessiveThreat/HakkarTheSoulflayerExcessiveThreatTrigger.h"
#include "HakkarTheSoulflayer/mechanics/positioning/HakkarPositioning/HakkarTheSoulflayerHakkarPositioningTrigger.h"
#include "HakkarTheSoulflayer/mechanics/son-of-hakkar/MoveToSonOfHakkar/HakkarTheSoulflayerMoveToSonOfHakkarTrigger.h"
#include "HakkarTheSoulflayer/mechanics/son-of-hakkar/PullSonOfHakkar/HakkarTheSoulflayerPullSonOfHakkarTrigger.h"
#include "HighPriestThekal/Phase1/HealthBalance/HighPriestThekalHealthBalanceTrigger.h"
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

        // High Priest Venoxis
        creators["high priest venoxis phase 1 holy wrath"] = &RaidZGTriggerContext::highPriestVenoxisPhase1HolyWrath;
        creators["high priest venoxis phase 1 venoxis positioning"] = &RaidZGTriggerContext::highPriestVenoxisPhase1VenoxisPositioning;
        creators["high priest venoxis phase 1 razzashi cobras positioning"] = &RaidZGTriggerContext::highPriestVenoxisPhase1RazzashiCobrasPositioning;
        creators["high priest venoxis phase 1 razzashi cobras dps priority"] = &RaidZGTriggerContext::highPriestVenoxisPhase1RazzashiCobrasDPSPriority;

        // Bloodlord Mandokir
        creators["bloodlord mandokir whirlwind"] = &RaidZGTriggerContext::bloodlordMandokirWhirlwind;

        // Gri'lek the Wanderer
        creators["grilek the wanderer avatar"] = &RaidZGTriggerContext::grilekTheWandererAvatarTrigger;

        // High Priest Thekal
        creators["high priest thekal phase 1 health balance"] = &RaidZGTriggerContext::highPriestThekalHealthBalance;

        // Hakkar the Soulflayer
        creators["hakkar the soulflayer go to poisonous cloud"] = &RaidZGTriggerContext::hakkarTheSoulflayerGoToPoisonousCloud;
        creators["hakkar the soulflayer cause insanity"] = &RaidZGTriggerContext::hakkarTheSoulflayerCauseInsanity;
        creators["hakkar the soulflayer excessive threat"] = &RaidZGTriggerContext::hakkarTheSoulflayerExcessiveThreat;
        creators["hakkar the soulflayer pull son of hakkar"] = &RaidZGTriggerContext::hakkarTheSoulflayerPullSonOfHakkar;
        creators["hakkar the soulflayer bring back son of hakkar"] = &RaidZGTriggerContext::hakkarTheSoulflayerBringBackSonOfHakkar;
        creators["hakkar the soulflayer hakkar positioning"] = &RaidZGTriggerContext::hakkarTheSoulflayerHakkarPositioning;
        creators["hakkar the soulflayer move to son of hakkar"] = &RaidZGTriggerContext::hakkarTheSoulflayerMoveToSonOfHakkar;
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

    // Bloodlord Mandokir
    static Trigger* bloodlordMandokirWhirlwind(PlayerbotAI* botAI)
    {
        return new BloodlordMandokirWhirlwindTrigger(botAI);
    }

    // Gri'lek the Wanderer
    static Trigger* grilekTheWandererAvatarTrigger(PlayerbotAI* botAI)
    {
        return new GrilekTheWandererAvatarTrigger(botAI);
    }

    // High Priest Thekal
    static Trigger* highPriestThekalHealthBalance(PlayerbotAI* botAI)
    {
        return new HighPriestThekalHealthBalanceTrigger(botAI);
    }

    // Hakkar the Soulflayer
    static Trigger* hakkarTheSoulflayerGoToPoisonousCloud(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerGoToPoisonousCloudTrigger(botAI);
    }

    static Trigger* hakkarTheSoulflayerCauseInsanity(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerCauseInsanityTrigger(botAI);
    }

    static Trigger* hakkarTheSoulflayerExcessiveThreat(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerExcessiveThreatTrigger(botAI);
    }

    static Trigger* hakkarTheSoulflayerPullSonOfHakkar(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerPullSonOfHakkarTrigger(botAI);
    }

    static Trigger* hakkarTheSoulflayerBringBackSonOfHakkar(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerBringBackSonOfHakkarTrigger(botAI);
    }

    static Trigger* hakkarTheSoulflayerHakkarPositioning(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerHakkarPositioningTrigger(botAI);
    }

    static Trigger* hakkarTheSoulflayerMoveToSonOfHakkar(PlayerbotAI* botAI)
    {
        return new HakkarTheSoulflayerMoveToSonOfHakkarTrigger(botAI);
    }
};