/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "MagStrategy.h"
#include "CreateNextAction.h"
#include "MagMultipliers.h"
#include "MagActions.h"

void RaidMagtheridonStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "magtheridon first three channelers engaged by main tank",
            {
                CreateNextAction<MagtheridonMainTankAttackFirstThreeChannelersAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon nw channeler engaged by first assist tank",
            {
                CreateNextAction<MagtheridonFirstAssistTankAttackNWChannelerAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon ne channeler engaged by second assist tank",
            {
                CreateNextAction<MagtheridonSecondAssistTankAttackNEChannelerAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon pulling west and east channelers",
            {
                CreateNextAction<MagtheridonMisdirectHellfireChannelersToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon determining kill order",
            {
                CreateNextAction<MagtheridonAssignDpsPriorityAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon burning abyssal spawned",
            {
                CreateNextAction<MagtheridonWarlockCcBurningAbyssalAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon boss engaged by main tank",
            {
                CreateNextAction<MagtheridonMainTankPositionBossAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon boss engaged by ranged",
            {
                CreateNextAction<MagtheridonSpreadRangedAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon standing in debris",
            {
                CreateNextAction<MagtheridonMoveOutOfDebrisAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon incoming blast nova",
            {
                CreateNextAction<MagtheridonUseManticronCubeAction>(ACTION_EMERGENCY + 9.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon need to manage timers and assignments",
            {
                CreateNextAction<MagtheridonManageTimersAndAssignmentsAction>(ACTION_EMERGENCY + 11.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "magtheridon bot is not in combat",
            {
                CreateNextAction<MagtheridonEraseTimersAndTrackersAction>(ACTION_EMERGENCY + 12.0f)
            }
        )
    );
}

void RaidMagtheridonStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    multipliers.push_back(new MagtheridonUseManticronCubeMultiplier(botAI));
    multipliers.push_back(new MagtheridonWaitToAttackMultiplier(botAI));
    multipliers.push_back(new MagtheridonControlTankActionsMultiplier(botAI));
    multipliers.push_back(new MagtheridonDebrisDangerMultiplier(botAI));
}
