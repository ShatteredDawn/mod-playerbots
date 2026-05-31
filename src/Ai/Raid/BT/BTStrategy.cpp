/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "BTStrategy.h"

#include "BTActions.h"
#include "BTMultipliers.h"
#include "CreateNextAction.h"

void RaidBlackTempleStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // General
    triggers.push_back(
        new TriggerNode(
            "black temple bot is not in combat",
            {
                CreateNextAction<BlackTempleEraseTimersAndTrackersAction>(ACTION_EMERGENCY + 11.0f)
            }
        )
    );

    // High Warlord Naj'entus
    triggers.push_back(
        new TriggerNode(
            "high warlord naj'entus pulling boss",
            {
                CreateNextAction<HighWarlordNajentusMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high warlord naj'entus boss engaged by tanks",
            {
                CreateNextAction<HighWarlordNajentusTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high warlord naj'entus casts needle spines",
            {
                CreateNextAction<HighWarlordNajentusDisperseRangedAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high warlord naj'entus player is impaled",
            {
                CreateNextAction<HighWarlordNajentusRemoveImpalingSpineAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high warlord naj'entus boss has tidal shield",
            {
                CreateNextAction<HighWarlordNajentusThrowImpalingSpineAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    // Supremus
    triggers.push_back(
        new TriggerNode(
            "supremus pulling boss or changing phase",
            {
                CreateNextAction<SupremusMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "supremus boss engaged by ranged",
            {
                CreateNextAction<SupremusDisperseRangedAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "supremus boss is fixated on bot",
            {
                CreateNextAction<SupremusKiteBossAction>(ACTION_EMERGENCY + 7.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "supremus volcano is nearby",
            {
                CreateNextAction<SupremusMoveAwayFromVolcanosAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "supremus need to manage phase timer",
            {
                CreateNextAction<SupremusManagePhaseTimerAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    // Shade of Akama
    triggers.push_back(
        new TriggerNode(
            "shade of akama killing channelers starts phase 2",
            {
                CreateNextAction<ShadeOfAkamaMeleeDpsPrioritizeChannelersAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    // Teron Gorefiend
    triggers.push_back(
        new TriggerNode(
            "teron gorefiend pulling boss",
            {
                CreateNextAction<TeronGorefiendMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "teron gorefiend boss engaged by tanks",
            {
                CreateNextAction<TeronGorefiendTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "teron gorefiend boss engaged by ranged",
            {
                CreateNextAction<TeronGorefiendPositionRangedOnBalconyAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "teron gorefiend boss is casting shadow of death",
            {
                CreateNextAction<TeronGorefiendAvoidShadowOfDeathAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "teron gorefiend bot has shadow of death",
            {
                CreateNextAction<TeronGorefiendMoveToCornerToDieAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "teron gorefiend bot transformed into vengeful spirit",
            {
                CreateNextAction<TeronGorefiendControlAndDestroyShadowyConstructsAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    // Gurtogg Bloodboil
    triggers.push_back(
        new TriggerNode(
            "gurtogg bloodboil pulling boss",
            {
                CreateNextAction<GurtoggBloodboilMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "gurtogg bloodboil boss engaged by tanks",
            {
                CreateNextAction<GurtoggBloodboilTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "gurtogg bloodboil boss casts bloodboil",
            {
                CreateNextAction<GurtoggBloodboilRotateRangedGroupsAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "gurtogg bloodboil bot has fel rage",
            {
                CreateNextAction<GurtoggBloodboilRangedMoveAwayFromEnragedPlayerAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "gurtogg bloodboil need to manage phase timer",
            {
                CreateNextAction<GurtoggBloodboilManagePhaseTimerAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    // Reliquary of Souls
    triggers.push_back(
        new TriggerNode(
            "reliquary of souls aggro resets upon phase change",
            {
                CreateNextAction<ReliquaryOfSoulsMisdirectBossToMainTankAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "reliquary of souls essence of suffering fixates on closest target",
            {
                CreateNextAction<ReliquaryOfSoulsAdjustDistanceFromSufferingAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "reliquary of souls essence of suffering disables healing",
            {
                CreateNextAction<ReliquaryOfSoulsHealersDpsSufferingAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "reliquary of souls essence of desire has rune shield",
            {
                CreateNextAction<ReliquaryOfSoulsSpellstealRuneShieldAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "reliquary of souls essence of desire casting deaden",
            {
                CreateNextAction<ReliquaryOfSoulsSpellReflectDeadenAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    // Mother Shahraz
    triggers.push_back(
        new TriggerNode(
            "mother shahraz pulling boss",
            {
                CreateNextAction<MotherShahrazMisdirectBossToMainTankAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "mother shahraz boss engaged by tanks",
            {
                CreateNextAction<MotherShahrazTanksPositionBossUnderPillarAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "mother shahraz tanks are positioning boss",
            {
                CreateNextAction<MotherShahrazMeleeDpsWaitAtSafePositionAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "mother shahraz sinister beam knocks back players",
            {
                CreateNextAction<MotherShahrazPositionRangedUnderPillarAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "mother shahraz bots are linked by fatal attraction",
            {
                CreateNextAction<MotherShahrazRunAwayToBreakFatalAttractionAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    // Illidari Council
    triggers.push_back(
        new TriggerNode(
            "illidari council pulling bosses",
            {
                CreateNextAction<IllidariCouncilMisdirectBossesToTanksAction>(ACTION_RAID + 4.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council gathios engaged by main tank",
            {
                CreateNextAction<IllidariCouncilMainTankPositionGathiosAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council gathios casting judgement of command",
            {
                CreateNextAction<IllidariCouncilMainTankReflectJudgementOfCommandAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council malande engaged by first assist tank",
            {
                CreateNextAction<IllidariCouncilFirstAssistTankFocusMalandeAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council darkshadow engaged by second assist tank",
            {
                CreateNextAction<IllidariCouncilSecondAssistTankPositionDarkshadowAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council zerevor engaged by mage tank",
            {
                CreateNextAction<IllidariCouncilMageTankPositionZerevorAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council mage tank needs dedicated healer",
            {
                CreateNextAction<IllidariCouncilPositionMageTankHealerAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council zerevor casts dangerous aoes",
            {
                CreateNextAction<IllidariCouncilDisperseRangedAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council pets screw up the pull",
            {
                CreateNextAction<IllidariCouncilCommandPetsToAttackGathiosAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council determining dps assignments",
            {
                CreateNextAction<IllidariCouncilAssignDpsTargetsAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidari council need to manage dps timer",
            {
                CreateNextAction<IllidariCouncilManageDpsTimerAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    // Illidan Stormrage <The Betrayer>
    triggers.push_back(
        new TriggerNode(
            "illidan stormrage tank needs aggro",
            {
                CreateNextAction<IllidanStormrageMisdirectToTankAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage boss casts flame crash in front of main tank",
            {
                CreateNextAction<IllidanStormrageMainTankRepositionBossAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage bot has parasitic shadowfiend",
            {
                CreateNextAction<IllidanStormrageIsolateBotWithParasiteAction>(ACTION_RAID + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage parasitic shadowfiends run wild",
            {
                CreateNextAction<IllidanStormrageSetEarthbindTotemAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage boss summoned flames of azzinoth",
            {
                CreateNextAction<IllidanStormrageAssistTanksHandleFlamesOfAzzinothAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage pets die to fire",
            {
                CreateNextAction<IllidanStormrageControlPetAggressionAction>(ACTION_RAID + 4.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage grate is safe from flames",
            {
                CreateNextAction<IllidanStormragePositionAboveGrateAction>(ACTION_EMERGENCY + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage bot struck by dark barrage",
            {
                CreateNextAction<IllidanStormrageRemoveDarkBarrageAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage boss is preparing to land",
            {
                CreateNextAction<IllidanStormrageMoveAwayFromLandingPointAction>(ACTION_EMERGENCY + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage boss deals splash damage",
            {
                CreateNextAction<IllidanStormrageDisperseRangedAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage this expansion hates melee",
            {
                CreateNextAction<IllidanStormrageMeleeGoSomewhereToNotDieAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage boss transforms into demon",
            {
                CreateNextAction<IllidanStormrageWarlockTankHandleDemonBossAction>(ACTION_EMERGENCY + 9.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage boss spawns adds",
            {
            CreateNextAction<IllidanStormrageDpsPrioritizeAddsAction>(ACTION_EMERGENCY + 1.0f)
        }
    )
);

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage maiev placed shadow trap",
            {
                CreateNextAction<IllidanStormrageUseShadowTrapAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage need to manage dps timer and rti",
            {
                CreateNextAction<IllidanStormrageManageDpsTimerAndRtiAction>(ACTION_EMERGENCY + 11.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage need to clear hazards between phases",
            {
                CreateNextAction<IllidanStormrageDestroyHazardsAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "illidan stormrage cheat",
            {
                CreateNextAction<IllidanStormrageHandleAddsCheatAction>(ACTION_EMERGENCY + 10.0f)
            }
        )
    );
}

void RaidBlackTempleStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    // High Warlord Naj'entus
    multipliers.push_back(new HighWarlordNajentusDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new HighWarlordNajentusDisableCombatFormationMoveMultiplier(botAI));

    // Supremus
    multipliers.push_back(new SupremusDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new SupremusFocusOnAvoidanceInPhase2Multiplier(botAI));
    multipliers.push_back(new SupremusHitboxIsBuggedMultiplier(botAI));

    // Teron Gorefiend
    multipliers.push_back(new TeronGorefiendDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new TeronGorefiendControlMovementMultiplier(botAI));
    multipliers.push_back(new TeronGorefiendMarkedBotOnlyMoveToDieMultiplier(botAI));
    multipliers.push_back(new TeronGorefiendSpiritsAttackOnlyShadowyConstructsMultiplier(botAI));
    multipliers.push_back(new TeronGorefiendDisableAttackingConstructsMultiplier(botAI));

    // Gurtogg Bloodboil
    multipliers.push_back(new GurtoggBloodboilDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new GurtoggBloodboilControlMovementMultiplier(botAI));

    // Reliquary of Souls
    multipliers.push_back(new ReliquaryOfSoulsDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new ReliquaryOfSoulsDontWasteHealingMultiplier(botAI));

    // Mother Shahraz
    multipliers.push_back(new MotherShahrazDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new MotherShahrazControlMovementMultiplier(botAI));
    multipliers.push_back(new MotherShahrazBotsWithFatalAttractionOnlyRunAwayMultiplier(botAI));

    // Illidari Council
    multipliers.push_back(new IllidariCouncilDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new IllidariCouncilDisableTankActionsMultiplier(botAI));
    multipliers.push_back(new IllidariCouncilControlMovementMultiplier(botAI));
    multipliers.push_back(new IllidariCouncilControlMisdirectionMultiplier(botAI));
    multipliers.push_back(new IllidariCouncilDisableArcaneShotOnZerevorMultiplier(botAI));
    multipliers.push_back(new IllidariCouncilDisableIceBlockMultiplier(botAI));
    multipliers.push_back(new IllidariCouncilWaitForDpsMultiplier(botAI));

    // Illidan Stormrage <The Betrayer>
    multipliers.push_back(new IllidanStormrageDelayDpsCooldownsMultiplier(botAI));
    multipliers.push_back(new IllidanStormrageControlTankActionsMultiplier(botAI));
    multipliers.push_back(new IllidanStormrageDisableDefaultTargetingMultiplier(botAI));
    multipliers.push_back(new IllidanStormrageControlNonTankMovementMultiplier(botAI));
    multipliers.push_back(new IllidanStormrageUseEarthbindTotemMultiplier(botAI));
    multipliers.push_back(new IllidanStormrageWaitForDpsMultiplier(botAI));
}
