/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_WARRIORACTIONS_H
#define _PLAYERBOT_WARRIORACTIONS_H

#include "AiObject.h"
#include "AiObjectContext.h"
#include "GenericSpellActions.h"
#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "Player.h"
#include "PlayerbotAI.h"
#include "ReachTargetActions.h"

// stances
BUFF_ACTION(CastBattleStanceAction, "battle stance");
BUFF_ACTION(CastDefensiveStanceAction, "defensive stance");
BUFF_ACTION(CastBerserkerStanceAction, "berserker stance");

// shouts
BUFF_ACTION(CastBattleShoutAction, "battle shout");
MELEE_ACTION_U(CastBattleShoutTauntAction, "battle shout", CastSpellAction::isUseful());  // useful to rebuff
// DEBUFF_ACTION_R(CastDemoralizingShoutAction, "demoralizing shout", 8.0f);                 // low range debuff

class CastDemoralizingShoutAction : public CastMeleeDebuffSpellAction
{
public:
    CastDemoralizingShoutAction(PlayerbotAI* botAI)
        : CastMeleeDebuffSpellAction(botAI, "demoralizing shout") {}
};

class CastDemoralizingShoutWithoutLifeTimeCheckAction : public CastMeleeDebuffSpellAction
{
public:
    CastDemoralizingShoutWithoutLifeTimeCheckAction(PlayerbotAI* botAI)
        : CastMeleeDebuffSpellAction(botAI, "demoralizing shout", false, 0.0f)
    {
    }
};

class CastChallengingShoutAction : public CastMeleeSpellAction
{
public:
    CastChallengingShoutAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "challenging shout") {}

    bool isUseful() override
    {
        Unit* const target = this->GetTarget();

        if (target == nullptr)
        {
            return false;
        }

        const ObjectGuid targetTarget = target->GetTarget();

        if (targetTarget.IsEmpty())
        {
            return false;
        }

        if (targetTarget == this->bot->GetGUID())
        {
            return false;
        }

        Player* const playerTargetTarget = ObjectAccessor::FindPlayer(targetTarget);

        if (playerTargetTarget == nullptr)
        {
            return true;
        }


        Value<Unit*>* const rtiTargetValue = this->context->GetValue<Unit*>("rti target");

        // This is a normally impossible situation where the Value is not correctly instantiated.
        // It does not mean the value itself is empty.
        if (rtiTargetValue == nullptr)
        {
            return false;
        }

        const Unit* const rtiTarget = rtiTargetValue->Get();

        if (PlayerbotAI::IsMainTank(playerTargetTarget))
        {
            if (rtiTarget != nullptr && rtiTarget->GetGUID() == target->GetGUID())
            {
                return true;
            }

            return false;
        }

        if (PlayerbotAI::IsAssistTank(playerTargetTarget))
        {
            if (rtiTarget != nullptr && rtiTarget->GetGUID() == target->GetGUID())
            {
                return true;
            }

            return false;
        }

        return true;
    }
};

DEBUFF_ACTION_R(CastIntimidatingShoutAction, "intimidating shout", 8.0f);
// shouts 2.4.3
BUFF_ACTION(CastCommandingShoutAction, "commanding shout");

// arms
MELEE_ACTION(CastHeroicStrikeAction, "heroic strike");
REACH_ACTION(CastChargeAction, "charge", 8.0f);
DEBUFF_CHECKISOWNER_ACTION(CastRendAction, "rend");
// DEBUFF_ENEMY_ACTION(CastRendOnAttackerAction, "rend");

class CastRendOnAttackerAction : public CastDebuffSpellOnMeleeAttackerAction
{
public:
    CastRendOnAttackerAction(PlayerbotAI* botAI) : CastDebuffSpellOnMeleeAttackerAction(botAI, "rend") {}
};

MELEE_ACTION(CastThunderClapAction, "thunder clap");
SNARE_ACTION(CastThunderClapSnareAction, "thunder clap");
SNARE_ACTION(CastHamstringAction, "hamstring");
MELEE_ACTION(CastOverpowerAction, "overpower");

class CastMockingBlowAction : public CastMeleeSpellAction
{
public:
    CastMockingBlowAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "mocking blow") {}
};

// BUFF_ACTION(CastRetaliationAction, "retaliation");
// arms 3.3.5
SPELL_ACTION(CastHeroicThrowAction, "heroic throw");
SNARE_ACTION(CastHeroicThrowSnareAction, "heroic throw");
// DEBUFF_ACTION(CastShatteringThrowAction, "shattering throw");

// arms talents
MELEE_ACTION(CastMortalStrikeAction, "mortal strike");
BUFF_ACTION(CastSweepingStrikesAction, "sweeping strikes");
// arms talents 3.3.5
MELEE_ACTION(CastBladestormAction, "bladestorm");

// fury
MELEE_ACTION(CastCleaveAction, "cleave");
MELEE_ACTION(CastExecuteAction, "execute");
REACH_ACTION(CastInterceptAction, "intercept", 8.0f);
ENEMY_HEALER_ACTION(CastInterceptOnEnemyHealerAction, "intercept");
SNARE_ACTION(CastInterceptOnSnareTargetAction, "intercept");
MELEE_ACTION(CastSlamAction, "slam");
class CastBerserkerRageAction : public CastSpellAction
{
public:
    CastBerserkerRageAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "berserker rage") {}

    std::string const GetTargetName() override { return "self target"; }
    bool isPossible() override;
    bool isUseful() override;
};
MELEE_ACTION(CastWhirlwindAction, "whirlwind");
MELEE_ACTION(CastPummelAction, "pummel");
ENEMY_HEALER_ACTION(CastPummelOnEnemyHealerAction, "pummel");
// fury 2.4.3
MELEE_ACTION(CastVictoryRushAction, "victory rush");
// fury 3.3.5
BUFF_ACTION(CastEnragedRegenerationAction, "enraged regeneration");
BUFF_ACTION(CastHeroicFuryAction, "heroic fury");

// fury talents
BUFF_ACTION(CastDeathWishAction, "death wish");
BUFF_ACTION(CastRecklessnessAction, "recklessness");
MELEE_ACTION(CastBloodthirstAction, "bloodthirst");
DEBUFF_ACTION_R(CastPiercingHowlAction, "piercing howl", 8.0f);
// fury talents 2.4.3
BUFF_ACTION(CastRampageAction, "rampage");

// protection
class CastTauntAction : public CastMeleeSpellAction
{
public:
    CastTauntAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "taunt") {}

    bool isUseful() override
    {
        Unit* const target = this->GetTarget();

        if (target == nullptr)
        {
            return false;
        }

        const ObjectGuid targetTarget = target->GetTarget();

        if (targetTarget.IsEmpty())
        {
            return false;
        }

        if (targetTarget == this->bot->GetGUID())
        {
            return false;
        }

        Player* const playerTargetTarget = ObjectAccessor::FindPlayer(targetTarget);

        if (playerTargetTarget == nullptr)
        {
            return true;
        }


        Value<Unit*>* const rtiTargetValue = this->context->GetValue<Unit*>("rti target");

        // This is a normally impossible situation where the Value is not correctly instantiated.
        // It does not mean the value itself is empty.
        if (rtiTargetValue == nullptr)
        {
            return false;
        }

        const Unit* const rtiTarget = rtiTargetValue->Get();

        if (PlayerbotAI::IsMainTank(playerTargetTarget))
        {
            if (rtiTarget != nullptr && rtiTarget->GetGUID() == target->GetGUID())
            {
                return true;
            }

            return false;
        }

        if (PlayerbotAI::IsAssistTank(playerTargetTarget))
        {
            if (rtiTarget != nullptr && rtiTarget->GetGUID() == target->GetGUID())
            {
                return true;
            }

            return false;
        }

        return true;
    }
};

SNARE_ACTION(CastTauntOnSnareTargetAction, "taunt");
BUFF_ACTION(CastBloodrageAction, "bloodrage");
MELEE_ACTION(CastShieldBashAction, "shield bash");
ENEMY_HEALER_ACTION(CastShieldBashOnEnemyHealerAction, "shield bash");
MELEE_ACTION(CastRevengeAction, "revenge");
BUFF_ACTION(CastShieldBlockAction, "shield block");
DEBUFF_ACTION_U(CastDisarmAction, "disarm",
                GetTarget() && GetTarget()->IsPlayer() ? !botAI->IsRanged((Player*)GetTarget())
                                                       : CastDebuffSpellAction::isUseful());
DEBUFF_ENEMY_ACTION(CastDisarmOnAttackerAction, "disarm");
BUFF_ACTION(CastShieldWallAction, "shield wall");
// protection 2.4.3
PROTECT_ACTION(CastInterveneAction, "intervene");
BUFF_ACTION(CastSpellReflectionAction, "spell reflection");

// protection talents
BUFF_ACTION(CastLastStandAction, "last stand");
MELEE_ACTION(CastShieldSlamAction, "shield slam");
MELEE_ACTION(CastConcussionBlowAction, "concussion blow");
// protection talents 2.4.3
MELEE_ACTION(CastDevastateAction, "devastate");
// protection talents 3.3.5
DEBUFF_ACTION_R(CastShockwaveAction, "shockwave", 8.0f);
SNARE_ACTION(CastShockwaveSnareAction, "shockwave");

class CastSunderArmorAction : public CastDebuffSpellAction
{
public:
    CastSunderArmorAction(PlayerbotAI* botAI) : CastDebuffSpellAction(botAI, "sunder armor")
    {
        range = ATTACK_DISTANCE;
    }

    bool isUseful() override;
};

class CastVigilanceAction : public BuffOnPartyAction
{
public:
    CastVigilanceAction(PlayerbotAI* botAI) : BuffOnPartyAction(botAI, "vigilance") {}

    Unit* GetTarget() override;
    bool Execute(Event event) override;
};

class CastRetaliationAction : public CastBuffSpellAction
{
public:
    CastRetaliationAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "retaliation") {}

    bool isUseful() override;
};

class CastShatteringThrowAction : public CastSpellAction
{
public:
    CastShatteringThrowAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "shattering throw") {}

    Unit* GetTarget() override;
    bool isUseful() override;
    bool isPossible() override;
    bool Execute(Event event) override;
};

#endif
