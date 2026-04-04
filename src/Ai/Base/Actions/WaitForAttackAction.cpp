/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "WaitForAttackAction.h"

#include <algorithm>
#include <cctype>

#include "AiObjectContext.h"
#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "PlayerbotAI.h"
#include "PlayerbotTextMgr.h"
#include "ServerFacade.h"
#include "TravelMgr.h"
#include "WaitForAttackStrategy.h"

WorldPosition WaitForAttackKeepSafeDistanceAction::GetBestPoint(
    AiObjectContext& context,
    Player& bot,
    Unit& target,
    float minDistance,
    float maxDistance
) const
{
    WorldPosition botPosition{bot};
    WorldPosition targetPosition{target};

    const int8_t startDir = urand(0, 1) * 2 - 1;
    const float radiansIncrement = (5.0f / 180.0f) * float(M_PI);
    const float startAngle = targetPosition.getAngleTo(botPosition) + frand(0.0f, radiansIncrement) * startDir;
    const float distance = frand(minDistance, maxDistance);

    Value<GuidVector>* const enemiesValue = this->context->GetValue<GuidVector>("possible targets no los");

    if (enemiesValue == nullptr)
    {
        return WorldPosition{};
    }

    const GuidVector enemies = enemiesValue->Get();

    for (float tryAngle = 0.0f; tryAngle < float(M_PI); tryAngle += radiansIncrement)
    {
        for (int8_t tryDir = -1; tryAngle && tryDir < 1; tryDir += 2)
        {
            float pointAngle = startAngle + tryAngle * startDir * tryDir;

            float x = targetPosition.GetPositionX() + distance * cos(pointAngle);
            float y = targetPosition.GetPositionY() + distance * sin(pointAngle);
            float z = targetPosition.GetPositionZ() + 1.0f;

            WorldPosition point{targetPosition.GetMapId(), x, y, z};
            point.setZ(point.getHeight());

            // Check line of sight to target
            if (!target.IsWithinLOS(
                point.GetPositionX(),
                point.GetPositionY(),
                point.GetPositionZ() + bot.GetCollisionHeight())
            )
            {
                continue;
            }

            // Check if enemies are close to this point
            bool enemyClose = false;

            for (const ObjectGuid& enemyGUID : enemies)
            {
                const Unit* const enemy = ObjectAccessor::GetUnit(bot, enemyGUID);

                if (enemy == nullptr)
                {
                    continue;
                }

                if (!enemy->IsWithinLOSInMap(this->bot) && enemy->IsHostileTo(this->bot))
                {
                    continue;
                }

                const float enemyAttackRange = enemy->GetCombatReach() + ATTACK_DISTANCE;
                WorldPosition enemyPos{enemy};

                if (enemyPos.sqDistance(point) <= (enemyAttackRange * enemyAttackRange))
                {
                    enemyClose = true;

                    break;
                }
            }

            if (enemyClose)
                continue;

            // Check if bot can path to this point
            if (!botPosition.canPathTo(point, this->bot))
            {
                continue;
            }

            return point;
        }
    }

    return botPosition;
}


bool WaitForAttackKeepSafeDistanceAction::Execute(Event)
{
    if (this->bot == nullptr)
    {
        return false;
    }

    Value<Unit*>* const currentTargetValue = this->context->GetValue<Unit*>("current target");

    if (currentTargetValue == nullptr)
    {
        return false;
    }

    Unit* target = currentTargetValue->Get();

    if (target == nullptr)
    {
        return false;
    }

    // If our target is moving towards a stationary unit, use that unit as anchor
    if (!target->IsStopped())
    {
        const ObjectGuid targetGuid = target->GetTarget();

        if (!targetGuid.IsEmpty())
        {
            Unit* const targetsTarget = ObjectAccessor::GetUnit(*target, targetGuid);

            if (targetsTarget != nullptr && targetsTarget->IsStopped())
            {
                target = targetsTarget;
            }
        }
    }

    if (target->IsAlive())
    {
        const float safeDistance = std::max(
            target->GetCombatReach() + ATTACK_DISTANCE,
            WaitForAttackStrategy::GetSafeDistance());
        const float safeDistanceThreshold = WaitForAttackStrategy::GetSafeDistanceThreshold();

        const WorldPosition bestPoint = this->GetBestPoint(*context, *this->bot, *target,
            safeDistance - safeDistanceThreshold, safeDistance);

        if (bestPoint)
        {
            return this->MoveTo(
                bestPoint.GetMapId(),
                bestPoint.GetPositionX(),
                bestPoint.GetPositionY(),
                bestPoint.GetPositionZ()
            );
        }
    }

    return false;
}

bool SetWaitForAttackTimeAction::Execute(Event event)
{
    std::string newTimeStr = event.getParam();

    if (newTimeStr.empty())
    {
        std::string const text = PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "wait_for_attack_provide_time",
            "Please provide a time to set (in seconds)",
            std::map<std::string, std::string>());
        botAI->TellMaster(text);
        return false;
    }

    if (!std::all_of(newTimeStr.begin(), newTimeStr.end(), ::isdigit))
    {
        std::string const text = PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "wait_for_attack_invalid_time",
            "Please provide valid time to set (in seconds) between 0 and 99",
            std::map<std::string, std::string>());
        botAI->TellMaster(text);
        return false;
    }

    int newTime = std::stoi(newTimeStr);
    if (newTime < 0 || newTime > 99)
    {
        std::string const text = PlayerbotTextMgr::instance().GetBotTextOrDefault(
            "wait_for_attack_invalid_time",
            "Please provide valid time to set (in seconds) between 0 and 99",
            std::map<std::string, std::string>());
        botAI->TellMaster(text);
        return false;
    }

    context->GetValue<uint8>("wait for attack time")->Set(static_cast<uint8>(newTime));

    std::map<std::string, std::string> placeholders;
    placeholders["%new_time"] = std::to_string(newTime);
    std::string const text = PlayerbotTextMgr::instance().GetBotTextOrDefault(
        "wait_for_attack_time_set",
        "Wait for attack time set to %new_time seconds",
        placeholders);
    botAI->TellMaster(text);
    return true;
}
