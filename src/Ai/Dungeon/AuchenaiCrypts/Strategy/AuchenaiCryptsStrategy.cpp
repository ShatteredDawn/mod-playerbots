#include "AuchenaiCryptsActions.h"
#include "AuchenaiCryptsStrategy.h"
#include "AuchenaiCryptsMultipliers.h"
#include "CreateNextAction.h"

void TbcDungeonAuchenaiCryptsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // Shirrak The Dead Watcher
    triggers.push_back(
        new TriggerNode(
            "shirrak tank position boss",
            {
                CreateNextAction<ShirrakTankPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "shirrak flee focus fire",
            {
                CreateNextAction<ShirrakFleeFocusFireAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "shirrak ranged keep distance",
            {
                CreateNextAction<ShirrakRangedKeepDistanceAction>(ACTION_RAID + 1.0f)
            }
        )
    );
}

void TbcDungeonAuchenaiCryptsStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    multipliers.push_back(new ShirrakFleeFocusFireMultiplier(this->botAI));
}
