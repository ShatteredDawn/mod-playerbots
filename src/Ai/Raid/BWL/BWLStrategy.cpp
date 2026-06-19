#include "BWLStrategy.h"

#include "CreateNextAction.h"
#include "MageActions.h"
#include "BWLActions.h"
#include "Strategy.h"

void RaidBwlStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "often",
            {
                CreateNextAction<BwlOnyxiaScaleCloakAuraCheckAction>(ACTION_RAID)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "bwl suppression device",
            {
                CreateNextAction<BwlTurnOffSuppressionDeviceAction>(ACTION_RAID)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "bwl affliction bronze",
            {
                CreateNextAction<BwlUseHourglassSandAction>(ACTION_RAID)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "bwl wild magic",
            {
                CreateNextAction<CastIceBlockAction>(ACTION_RAID)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "bwl nefarian fear ward",
            {
                CreateNextAction<BwlNefarianFearWardAction>(ACTION_RAID)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "bwl death talon wyrmguard tank",
            {
                CreateNextAction<BwlDeathTalonWyrmguardTankMoveAwayAction>(ACTION_RAID)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "bwl death talon wyrmguard ranged",
            {
                CreateNextAction<BwlDeathTalonWyrmguardRangedMoveAwayAction>(ACTION_RAID)
            }
        )
    );
}
