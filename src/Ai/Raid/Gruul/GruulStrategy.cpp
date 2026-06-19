#include "GruulStrategy.h"
#include "CreateNextAction.h"
#include "GruulActions.h"
#include "GruulMultipliers.h"

void RaidGruulsLairStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // High King Maulgar
    triggers.push_back(
        new TriggerNode(
            "high king maulgar boss engaged by main tank",
            {
                CreateNextAction<HighKingMaulgarMainTankAttackMaulgarAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar olm engaged by first assist tank",
            {
                CreateNextAction<HighKingMaulgarFirstAssistTankAttackOlmAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar blindeye engaged by second assist tank",
            {
                CreateNextAction<HighKingMaulgarSecondAssistTankAttackBlindeyeAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar krosh engaged by mage tank",
            {
                CreateNextAction<HighKingMaulgarMageTankAttackKroshAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar kiggler engaged by moonkin tank",
            {
                CreateNextAction<HighKingMaulgarMoonkinTankAttackKigglerAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar determining kill order",
            {
                CreateNextAction<HighKingMaulgarAssignDPSPriorityAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar boss channeling whirlwind",
            {
                CreateNextAction<HighKingMaulgarRunAwayFromWhirlwindAction>(ACTION_EMERGENCY + 7.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar krosh casts blast wave",
            {
                CreateNextAction<HighKingMaulgarMoveAwayFromBlastNovaDangerAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar wild fel stalker spawned",
            {
                CreateNextAction<HighKingMaulgarBanishFelStalkerAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "high king maulgar pulling ogre council",
            {
                CreateNextAction<HighKingMaulgarMisdirectOgresToTanksAction>(ACTION_RAID + 2.0f)
            }
        )
    );

    // Gruul the Dragonkiller
    triggers.push_back(
        new TriggerNode(
            "gruul the dragonkiller boss engaged by main tank",
            {
                CreateNextAction<GruulTheDragonkillerTanksPositionBossAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "gruul the dragonkiller boss engaged by range",
            {
                CreateNextAction<GruulTheDragonkillerSpreadRangedAction>(ACTION_RAID + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "gruul the dragonkiller incoming shatter",
            {
                CreateNextAction<GruulTheDragonkillerShatterSpreadAction>(ACTION_EMERGENCY + 6.0f)
            }
        )
    );
}

void RaidGruulsLairStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    // High King Maulgar
    multipliers.push_back(new HighKingMaulgarDelayBloodlustAndHeroismMultiplier(botAI));
    multipliers.push_back(new HighKingMaulgarControlTankActionsMultiplier(botAI));
    multipliers.push_back(new HighKingMaulgarAvoidWhirlwindMultiplier(botAI));
    multipliers.push_back(new HighKingMaulgarDisableArcaneShotOnKroshMultiplier(botAI));
    multipliers.push_back(new HighKingMaulgarDisableMageTankAoeMultiplier(botAI));

    // Gruul the Dragonkiller
    multipliers.push_back(new GruulTheDragonkillerDelayBloodlustAndHeroismMultiplier(botAI));
    multipliers.push_back(new GruulTheDragonkillerControlTankMovementMultiplier(botAI));
    multipliers.push_back(new GruulTheDragonkillerStaySpreadForShatterMultiplier(botAI));
}
