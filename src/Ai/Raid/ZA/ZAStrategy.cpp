/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ZAStrategy.h"
#include "CreateNextAction.h"
#include "ZA/ZAActions.h"
#include "ZAMultipliers.h"

void RaidZulAmanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // Trash
    triggers.push_back(
        new TriggerNode(
            "amani'shi medicine man summoned ward",
            {
                CreateNextAction<AmanishiMedicineManMarkWardAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    // Akil'zon <Eagle Avatar>
    triggers.push_back(
        new TriggerNode(
            "akil'zon pulling boss",
            {
                CreateNextAction<AkilzonMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "akil'zon boss engaged by main tank",
            {
                CreateNextAction<AkilzonTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "akil'zon boss casts static disruption",
            {
                CreateNextAction<AkilzonSpreadRangedAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "akil'zon electrical storm incoming",
            {
                CreateNextAction<AkilzonMoveToEyeOfTheStormAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "akil'zon bots need to prepare for electrical storm",
            {
                CreateNextAction<AkilzonManageElectricalStormTimerAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    // Nalorakk <Bear Avatar>
    triggers.push_back(
        new TriggerNode(
            "nalorakk pulling boss",
            {
                CreateNextAction<NalorakkMisdirectBossToMainTankAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "nalorakk boss switches forms",
            {
                CreateNextAction<NalorakkTanksPositionBossAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "nalorakk boss casts surge",
            {
                CreateNextAction<NalorakkSpreadRangedAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    // Jan'alai <Dragonhawk Avatar>
    triggers.push_back(
        new TriggerNode(
            "jan'alai pulling boss",
            {
                CreateNextAction<JanalaiMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "jan'alai boss engaged by main tank",
            {
                CreateNextAction<JanalaiTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "jan'alai boss casts flame breath",
            {
                CreateNextAction<JanalaiSpreadRangedInCircleAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "jan'alai boss summoning fire bombs",
            {
                CreateNextAction<JanalaiAvoidFireBombsAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "jan'alai amani'shi hatchers spawned",
            {
                CreateNextAction<JanalaiMarkAmanishiHatchersAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    // Halazzi <Lynx Avatar>
    triggers.push_back(
        new TriggerNode(
            "halazzi pulling boss",
            {
                CreateNextAction<HalazziMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "halazzi boss engaged by main tank",
            {
                CreateNextAction<HalazziMainTankPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "halazzi boss summons spirit lynx",
            {
                CreateNextAction<HalazziFirstAssistTankAttackSpiritLynxAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "halazzi determining dps target",
            {
                CreateNextAction<HalazziAssignDpsPriorityAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    // Hex Lord Malacrass
    triggers.push_back(
        new TriggerNode(
            "hex lord malacrass pulling boss",
            {
                CreateNextAction<HexLordMalacrassMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "hex lord malacrass determining kill order",
            {
                CreateNextAction<HexLordMalacrassAssignDpsPriorityAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "hex lord malacrass boss is channeling whirlwind",
            {
                CreateNextAction<HexLordMalacrassRunAwayFromWhirlwindAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "hex lord malacrass boss has spell reflection",
            {
                CreateNextAction<HexLordMalacrassCastersStopAttackingAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "hex lord malacrass boss placed freezing trap",
            {
                CreateNextAction<HexLordMalacrassMoveAwayFromFreezingTrapAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    // Zul'jin
    triggers.push_back(
        new TriggerNode(
            "zul'jin main tank needs aggro upon pull or phase change",
            {
                CreateNextAction<ZuljinMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "zul'jin boss engaged by main tank",
            {
                CreateNextAction<ZuljinTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "zul'jin boss is channeling whirlwind in troll form",
            {
                CreateNextAction<ZuljinRunAwayFromWhirlwindAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "zul'jin boss is summoning cyclones in eagle form",
            {
                CreateNextAction<ZuljinAvoidCyclonesAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "zul'jin boss casts aoe abilities in dragonhawk form",
            {
                CreateNextAction<ZuljinSpreadRangedAction>(ACTION_RAID + 1.0f)
            }
        )
    );
}

void RaidZulAmanStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    // Akil'zon <Eagle Avatar>
    multipliers.push_back(new AkilzonDisableCombatFormationMoveMultiplier(this->botAI));
    multipliers.push_back(new AkilzonStayInEyeOfTheStormMultiplier(this->botAI));

    // Nalorakk <Bear Avatar>
    multipliers.push_back(new NalorakkDisableTankActionsMultiplier(this->botAI));
    multipliers.push_back(new NalorakkControlMisdirectionMultiplier(this->botAI));

    // Jan'alai <Dragonhawk Avatar>
    multipliers.push_back(new JanalaiDisableTankActionsMultiplier(this->botAI));
    multipliers.push_back(new JanalaiDisableCombatFormationMoveMultiplier(this->botAI));
    multipliers.push_back(new JanalaiStayAwayFromFireBombsMultiplier(this->botAI));
    multipliers.push_back(new JanalaiDoNotCrowdControlHatchersMultiplier(this->botAI));
    multipliers.push_back(new JanalaiDelayBloodlustAndHeroismMultiplier(this->botAI));

    // Halazzi <Lynx Avatar>
    multipliers.push_back(new HalazziDisableTankActionsMultiplier(this->botAI));
    multipliers.push_back(new HalazziControlMisdirectionMultiplier(this->botAI));

    // Hex Lord Malacrass
    multipliers.push_back(new HexLordMalacrassAvoidWhirlwindMultiplier(this->botAI));
    multipliers.push_back(new HexLordMalacrassStopAttackingDuringSpellReflectionMultiplier(this->botAI));
    multipliers.push_back(new HexLordMalacrassDoNotDispelUnstableAfflictionMultiplier(this->botAI));

    // Zul'jin
    multipliers.push_back(new ZuljinDisableTankFaceMultiplier(this->botAI));
    multipliers.push_back(new ZuljinAvoidWhirlwindMultiplier(this->botAI));
    multipliers.push_back(new ZuljinDisableAvoidAoeMultiplier(this->botAI));
    multipliers.push_back(new ZuljinDelayBloodlustAndHeroismMultiplier(this->botAI));
}
