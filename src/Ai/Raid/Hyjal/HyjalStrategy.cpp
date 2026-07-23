/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "HyjalStrategy.h"
#include "CreateNextAction.h"
#include "HyjalActions.h"
#include "HyjalMultipliers.h"

void RaidHyjalSummitStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // General
    triggers.push_back(
        new TriggerNode(
            "hyjal summit bot is not in combat",
            {
                CreateNextAction<HyjalSummitEraseTrackersAction>(ACTION_EMERGENCY + 11.0f)
            }
        )
    );

    // Rage Winterchill
    triggers.push_back(
        new TriggerNode(
            "rage winterchill pulling boss",
            {
                CreateNextAction<RageWinterchillMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rage winterchill boss engaged by main tank",
            {
                CreateNextAction<RageWinterchillMainTankPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rage winterchill boss casts death and decay on ranged",
            {
                CreateNextAction<RageWinterchillSpreadRangedInCircleAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rage winterchill melee is standing in death and decay",
            {
                CreateNextAction<RageWinterchillMeleeGetOutOfDeathAndDecayAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    // Anetheron
    triggers.push_back(
        new TriggerNode(
            "anetheron pulling boss or infernal",
            {
                CreateNextAction<AnetheronMisdirectBossAndInfernalsToTanksAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "anetheron boss engaged by main tank",
            {
                CreateNextAction<AnetheronMainTankPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "anetheron boss casts carrion swarm",
            {
                CreateNextAction<AnetheronSpreadRangedInCircleAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "anetheron bot is targeted by infernal",
            {
                CreateNextAction<AnetheronBringInfernalToInfernalTankAction>(ACTION_EMERGENCY + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "anetheron infernals need to be kept away from raid",
            {
                CreateNextAction<AnetheronFirstAssistTankPickUpInfernalsAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "anetheron infernals continue to spawn",
            {
                CreateNextAction<AnetheronAssignDpsPriorityAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    // Kaz'rogal
    triggers.push_back(
        new TriggerNode(
            "kaz'rogal pulling boss",
            {
                CreateNextAction<KazrogalMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "kaz'rogal boss engaged by main tank",
            {
                CreateNextAction<KazrogalMainTankPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "kaz'rogal boss engaged by assist tanks",
            {
                CreateNextAction<KazrogalAssistTanksMoveInFrontOfBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "kaz'rogal low mana bots need escape path",
            {
                CreateNextAction<KazrogalSpreadRangedInArcAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "kaz'rogal bot is low on mana",
                {
                CreateNextAction<KazrogalLowManaBotTakeDefensiveMeasuresAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "kaz'rogal mark deals shadow damage",
            {
                CreateNextAction<KazrogalCastShadowProtectionSpellAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    // Azgalor
    triggers.push_back(
        new TriggerNode(
            "azgalor pulling boss",
            {
                CreateNextAction<AzgalorMisdirectBossToMainTankAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor boss engaged by main tank",
                {
                CreateNextAction<AzgalorMainTankPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor main tank is positioning boss",
                {
                CreateNextAction<AzgalorWaitAtSafePositionAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor boss engaged by ranged",
            {
                CreateNextAction<AzgalorDisperseRangedAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor boss casts rain of fire on melee",
            {
                CreateNextAction<AzgalorMeleeGetOutOfFireAndSwapTargetsAction>(ACTION_EMERGENCY + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor bot is doomed",
            {
                CreateNextAction<AzgalorMoveToDoomguardTankAction>(ACTION_EMERGENCY + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor doomguards must be controlled",
            {
                CreateNextAction<AzgalorFirstAssistTankPositionDoomguardAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "azgalor doomguards must die",
            {
                CreateNextAction<AzgalorRangedDpsPrioritizeDoomguardsAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    // Archimonde
    triggers.push_back(
        new TriggerNode(
            "archimonde pulling boss",
            {
                CreateNextAction<ArchimondeMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "archimonde boss engaged by main tank",
            {
                CreateNextAction<ArchimondeMoveBossToInitialPositionAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "archimonde boss casts fear",
            {
                CreateNextAction<ArchimondeCastFearImmunitySpellAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "archimonde boss casts air burst",
            {
                CreateNextAction<ArchimondeSpreadToAvoidAirBurstAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "archimonde boss summoned doomfire",
            {
                CreateNextAction<ArchimondeAvoidDoomfireAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "archimonde bot stood in doomfire",
            {
                CreateNextAction<ArchimondeRemoveDoomfireDotAction>(ACTION_EMERGENCY + 7.0f)
            }
        )
    );
}

void RaidHyjalSummitStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    // Trash
    multipliers.push_back(new HyjalSummitTimeBloodlustAndHeroismMultiplier(botAI));

    // Rage Winterchill
    multipliers.push_back(new RageWinterchillDisableCombatFormationMoveMultiplier(botAI));
    multipliers.push_back(new RageWinterchillMeleeControlAvoidanceMultiplier(botAI));

    // Anetheron
    multipliers.push_back(new AnetheronDisableTankActionsMultiplier(botAI));
    multipliers.push_back(new AnetheronDisableCombatFormationMoveMultiplier(botAI));
    multipliers.push_back(new AnetheronControlMisdirectionMultiplier(botAI));

    // Kaz'rogal
    multipliers.push_back(new KazrogalLowManaBotStayAwayFromGroupMultiplier(botAI));
    multipliers.push_back(new KazrogalKeepAspectOfTheViperActiveMultiplier(botAI));
    multipliers.push_back(new KazrogalControlMovementMultiplier(botAI));

    // Azgalor
    multipliers.push_back(new AzgalorDisableTankActionsMultiplier(botAI));
    multipliers.push_back(new AzgalorDoomedBotPrioritizePositioningMultiplier(botAI));
    multipliers.push_back(new AzgalorMeleeDpsControlAvoidanceMultiplier(botAI));

    // Archimonde
    multipliers.push_back(new ArchimondeDisableCombatFormationMoveMultiplier(botAI));
}
