/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "RSStrategy.h"

#include "ChooseTargetActions.h"
#include "CreateNextAction.h"
#include "MovementActions.h"
#include "RSActions.h"
#include "RSMultipliers.h"

void RaidRsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{

    triggers.push_back(
        new TriggerNode(
            "rs baltharus brand",
            {
                CreateNextAction<RsBaltharusBrandAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs baltharus tank position",
            {
                CreateNextAction<RsBaltharusTankPositionAction>(ACTION_RAID + 6.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs baltharus avoid front",
            {
                CreateNextAction<RsBaltharusAvoidFrontAction>(ACTION_RAID + 5.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs baltharus healer position",
            {
                CreateNextAction<RsBaltharusHealerPositionAction>(ACTION_RAID + 4.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs saviana conflagration",
            {
                CreateNextAction<RsSavianaConflagrationAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs saviana avoid front",
            {
                CreateNextAction<RsSavianaAvoidFrontAction>(ACTION_RAID + 6.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs saviana tank position",
            {
                CreateNextAction<RsSavianaTankPositionAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs saviana melee spread",
            {
                CreateNextAction<RsSavianaMeleeSpreadAction>(ACTION_RAID + 7.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs zarithrian adds",
            {
                CreateNextAction<RsZarithrianAddsAction>(ACTION_RAID + 5.0f),
                CreateNextAction<AttackRtiTargetAction>(ACTION_RAID + 4.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs zarithrian tank",
            {
                CreateNextAction<RsZarithrianTankAction>(ACTION_RAID + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs halion start position",
            {
                CreateNextAction<RsHalionStartPositionAction>(ACTION_RAID + 6.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion combustion",
            {
                CreateNextAction<RsHalionCombustionAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion meteor",
            {
                CreateNextAction<RsHalionMeteorAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion tank position",
            {
                CreateNextAction<RsHalionTankPositionAction>(ACTION_RAID + 6.0f),
                CreateNextAction<AttackRtiTargetAction>(ACTION_RAID + 5.0f)
        }
    )
);
    triggers.push_back(
        new TriggerNode(
            "rs halion avoid cones",
            {
                CreateNextAction<RsHalionAvoidConesAction>(ACTION_RAID + 5.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs halion adds",
            {
                CreateNextAction<RsHalionAddsAction>(ACTION_RAID + 6.0f),
                CreateNextAction<AttackRtiTargetAction>(ACTION_RAID + 5.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion add tank",
            {
                CreateNextAction<RsHalionAddTankAction>(ACTION_RAID + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs halion enter portal",
            {
                CreateNextAction<RsHalionEnterPortalAction>(ACTION_RAID + 8.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion cutter",
            {
                CreateNextAction<RsHalionCutterAction>(ACTION_RAID + 8.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion consumption",
            {
                CreateNextAction<RsHalionConsumptionAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion heal consumption",
            {
                CreateNextAction<RsHalionHealConsumptionAction>(ACTION_RAID + 7.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion p2 tank position",
            {
                CreateNextAction<RsHalionP2TankPositionAction>(ACTION_RAID + 6.0f),
                CreateNextAction<AttackRtiTargetAction>(ACTION_RAID + 5.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs halion p2 avoid cones",
            {
                CreateNextAction<RsHalionP2AvoidConesAction>(ACTION_RAID + 5.0f),
                CreateNextAction<AttackRtiTargetAction>(ACTION_RAID + 4.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rs trash main tank",
            {
                CreateNextAction<RsTrashMainTankAction>(ACTION_RAID + 6.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs trash assist tank",
            {
                CreateNextAction<RsTrashAssistTankAction>(ACTION_RAID + 6.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs trash ranged",
            {
                CreateNextAction<RsTrashRangedAction>(ACTION_RAID + 5.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs trash melee flank",
            {
                CreateNextAction<RearFlankAction>(ACTION_RAID + 5.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rs trash adds",
            {
                CreateNextAction<RsTrashAddsAction>(ACTION_RAID + 5.0f),
                CreateNextAction<AttackRtiTargetAction>(ACTION_RAID + 4.0f)
            }
        )
    );
}

void RaidRsStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    multipliers.push_back(
        new RsBaltharusBrandSafeMultiplier(botAI));
    multipliers.push_back(
        new RsSavianaBeaconMultiplier(botAI));
    multipliers.push_back(
        new RsSavianaMeleeSpreadMultiplier(botAI));
    multipliers.push_back(
        new RsZarithrianAddsMultiplier(botAI));
    multipliers.push_back(
        new RsZarithrianTankSwapMultiplier(botAI));
    multipliers.push_back(
        new RsHalionCombustionMultiplier(botAI));
    multipliers.push_back(
        new RsHalionMeteorMultiplier(botAI));
    multipliers.push_back(
        new RsHalionMeleeFlankMultiplier(botAI));
    multipliers.push_back(
        new RsHalionP2Multiplier(botAI));
    multipliers.push_back(
        new RsHalionHpBalanceMultiplier(botAI));
    multipliers.push_back(
        new RsHalionRealmIsolationMultiplier(botAI));
    multipliers.push_back(
        new RsTrashAddsMultiplier(botAI));
}
