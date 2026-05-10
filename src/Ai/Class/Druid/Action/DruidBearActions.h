/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DRUIDBEARACTIONS_H
#define _PLAYERBOT_DRUIDBEARACTIONS_H

#include "AiObjectContext.h"
#include "GenericSpellActions.h"
#include "ReachTargetActions.h"

class PlayerbotAI;

class CastFeralChargeBearAction : public CastReachTargetSpellAction
{
public:
    CastFeralChargeBearAction(PlayerbotAI* botAI) : CastReachTargetSpellAction(botAI, "feral charge - bear", 1.5f) {}
};

class CastGrowlAction : public CastSpellAction
{
public:
    CastGrowlAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "growl") {}

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

class CastChallengingRoarAction : public CastMeleeDebuffSpellAction
{
public:
    CastChallengingRoarAction(PlayerbotAI* botAI) : CastMeleeDebuffSpellAction(botAI, "challenging roar") {}

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

class CastMaulAction : public CastMeleeSpellAction
{
public:
    CastMaulAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "maul") {}

    bool isUseful() override;
};

class CastBashAction : public CastMeleeSpellAction
{
public:
    CastBashAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "bash") {}
};

class CastSwipeAction : public CastMeleeSpellAction
{
public:
    CastSwipeAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "swipe") {}
};

class CastDemoralizingRoarAction : public CastMeleeDebuffSpellAction
{
public:
    CastDemoralizingRoarAction(PlayerbotAI* botAI) : CastMeleeDebuffSpellAction(botAI, "demoralizing roar") {}
};

class CastMangleBearAction : public CastMeleeSpellAction
{
public:
    CastMangleBearAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "mangle (bear)") {}
};

class CastSwipeBearAction : public CastMeleeSpellAction
{
public:
    CastSwipeBearAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "swipe (bear)") {}
};

class CastLacerateAction : public CastMeleeSpellAction
{
public:
    CastLacerateAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "lacerate") {}
};

class CastBashOnEnemyHealerAction : public CastSpellOnEnemyHealerAction
{
public:
    CastBashOnEnemyHealerAction(PlayerbotAI* botAI) : CastSpellOnEnemyHealerAction(botAI, "bash") {}
};

#endif
