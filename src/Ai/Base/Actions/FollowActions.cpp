/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "FollowActions.h"

#include <algorithm>
#include <cmath>
#include <array>

#include "AiObjectContext.h"
#include "Event.h"
#include "Formations.h"
#include "LastMovementValue.h"
#include "MotionMaster.h"
#include "PlayerbotAI.h"
#include "Position.h"
#include "ServerFacade.h"
#include "Transport.h"
#include "Map.h"

Transport* FollowAction::getTransportForPosTolerant(Map& map, WorldObject& ref, uint32_t phaseMask, float x, float y, float z)
{
    std::array<float, 4> const probes = { z, z + 0.5f, z + 1.5f, z - 0.5f };

    for (const float pz : probes)
    {
        Transport* const transport = map.GetTransportForPos(phaseMask, x, y, pz, &ref);

        if (transport == nullptr)
        {
            continue;
        }

        return transport;
    }

    return nullptr;
}

// Attempts to find a point on the leader's transport that is closer to the bot,
// by probing along the segment from master -> bot and returning the last point
// that is still detected as being on the expected transport.
BoardPointTransportResultStruct FollowAction::findBoardingPointOnTransport(
    Map& map, Transport& expectedTransport, WorldObject& ref,
    float masterX, float masterY, float masterZ,
    float botX, float botY, float botZ
)
{
    const uint32_t phaseMask = ref.GetPhaseMask();
    const Transport* const transport = this->getTransportForPosTolerant(map, ref, phaseMask, masterX, masterY, masterZ);

    // Ensure master is actually detected on that transport (tolerant).
    if (transport != &expectedTransport)
    {
        return {
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
            .found = false
        };
    }

    // The raycast in GetTransportForPos starts at (z + 2). Probe with a safe Z.
    const float probeZ = std::max(masterZ, botZ);

    // Adaptive step count: small platforms need tighter sampling.
    const float dx2 = botX - masterX;
    const float dy2 = botY - masterY;
    const float dist2d = std::sqrt(dx2 * dx2 + dy2 * dy2);
    const int32_t steps = std::clamp(int32_t(dist2d / 0.75f), 10, 28);

    const float dx = (botX - masterX) / float(steps);
    const float dy = (botY - masterY) / float(steps);

    // Master must actually be on the expected transport for this to work.
    if (map.GetTransportForPos(phaseMask, masterX, masterY, probeZ, &ref) != &expectedTransport)
    {
        return {
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
            .found = false
        };
    }

    BoardPointTransportResultStruct result{
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
        .found = false
    };

    float lastX = masterX;
    float lastY = masterY;

    for (int32_t i = 1; i <= steps; ++i)
    {
        const float px = masterX + dx * i;
        const float py = masterY + dy * i;

        const Transport* const t = this->getTransportForPosTolerant(map, ref, phaseMask, px, py, probeZ);

        if (t != &expectedTransport)
        {
            break;
        }

        lastX = px;
        lastY = py;
        result.found = true;
    }

    result.x = lastX;
    result.y = lastY;
    // keep deck-level Z to encourage stepping onto the platform/boat
    result.z = masterZ;

    return result;

    // outX = lastX;
    // outY = lastY;
    // // keep deck-level Z to encourage stepping onto the platform/boat
    // outZ = masterZ;

    // return true;
}

HandleMovingTransportsResultEnum FollowAction::handleMovingTransports(Player& master)
{
    Map* map = master.GetMap();

    if (map == nullptr)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    const float masterPositionX = master.GetPositionX();
    const float masterPositionY = master.GetPositionY();
    const float masterPositionZ = master.GetPositionZ();
    const float botPositionX = this->bot->GetPositionX();
    const float botPositionY = this->bot->GetPositionY();
    const float botPositionZ = this->bot->GetPositionZ();

    const uint32_t mapId = this->bot->GetMapId();
    Transport* transport = master.GetTransport();

    if (transport == nullptr)
    {
        transport = this->getTransportForPosTolerant(
            *map,
            master,
            master.GetPhaseMask(),
            masterPositionX,
            masterPositionY,
            masterPositionZ
        );
    }

    if (transport == nullptr)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    // Ignore static transports (elevators/trams): only keep boats/zeppelins here.
    if (transport->IsStaticTransport())
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    if (this->bot->GetTransport() == transport)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    const float botProbeZ = std::max(this->bot->GetPositionZ(), transport->GetPositionZ());

    const Transport* const botSurfaceTransport = this->getTransportForPosTolerant(
        *map,
        *this->bot,
        this->bot->GetPhaseMask(),
        botPositionX,
        botPositionY,
        botProbeZ
    );

    if (botSurfaceTransport == transport)
    {
        transport->AddPassenger(this->bot, true);
        this->bot->StopMovingOnCurrentPos();

        return HandleMovingTransportsResultEnum::TRUE;
    }

    const float boardingAssistDistance = 60.0f;
    const float dist2d = ServerFacade::instance().GetDistance2d(this->bot, &master);
    const bool inAssist = ServerFacade::instance().IsDistanceLessOrEqualThan(dist2d, boardingAssistDistance);

    if (!inAssist)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    float destX = masterPositionX;
    float destY = masterPositionY;
    float destZ = masterPositionZ;

    const BoardPointTransportResultStruct boardingPointResult = this->findBoardingPointOnTransport(
        *map,
        *transport,
        master,
        masterPositionX, masterPositionY, masterPositionZ,
        botPositionX, botPositionY, botPositionZ
    );

    if (boardingPointResult.found)
    {
        destX = boardingPointResult.x;
        destY = boardingPointResult.z;
        destZ = boardingPointResult.z;
    }

    const MovementPriority priority = this->botAI->GetState() == BOT_STATE_COMBAT
        ? MovementPriority::MOVEMENT_COMBAT
        : MovementPriority::MOVEMENT_NORMAL;

    const bool movingAllowed = this->IsMovingAllowed(mapId, destX, destY, destZ);

    if (!movingAllowed)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    const bool dupMove = this->IsDuplicateMove(mapId, destX, destY, destZ);

    if (dupMove)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    const bool waiting = this->IsWaitingForLastMove(priority);

    if (waiting)
    {
        return HandleMovingTransportsResultEnum::NONE;
    }

    if (this->bot->IsSitState())
    {
        this->bot->SetStandState(UNIT_STAND_STATE_STAND);
    }

    if (this->bot->IsNonMeleeSpellCast(true))
    {
        this->bot->CastStop();
        this->botAI->InterruptSpell();
    }

    MotionMaster* const motionMaster = this->bot->GetMotionMaster();

    if (motionMaster == nullptr)
    {
        return HandleMovingTransportsResultEnum::FALSE;
    }

    motionMaster->MovePoint(
        0,
        destX, destY, destZ,
        FORCED_MOVEMENT_NONE,
        0.0f,
        0.0f,
        false,
        false
    );

    const float distanceFromPoint = this->bot->GetExactDist(destX, destY, destZ);
    const float moveDelay = this->MoveDelay(distanceFromPoint);
    const float delay = std::clamp(moveDelay * 1000.0f, 0.0f, float(PlayerbotAIConfig::instance().maxWaitForMove));

    Value<LastMovement&>* const lastMovementValue = this->context->GetValue<LastMovement&>("last movement");

    if (lastMovementValue == nullptr)
    {
        return HandleMovingTransportsResultEnum::FALSE;
    }

    lastMovementValue->Get().Set(mapId, destX, destY, destZ, this->bot->GetOrientation(), delay, priority);

    this->ClearIdleState();

    return HandleMovingTransportsResultEnum::TRUE;
}

bool FollowAction::Execute(Event)
{
    // Transport handling for moving transports only (boats/zeppelins).
    Player* master = this->botAI->GetMaster();

    if (master != nullptr && master->IsInWorld() && bot->IsInWorld() && bot->GetMapId() == master->GetMapId())
    {
        const HandleMovingTransportsResultEnum result = this->handleMovingTransports(*master);

        if (result != HandleMovingTransportsResultEnum::NONE)
        {
            return result == HandleMovingTransportsResultEnum::TRUE;
        }
    }
    // end unified transport handling

    Value<Formation*>* const formationValue = this->context->GetValue<Formation*>("formation");

    if (formationValue == nullptr)
    {
        return false;
    }

    Formation* const formation = formationValue->Get();

    const std::string target = formation->GetTargetName();

    if (target.empty())
    {
        const WorldLocation loc = formation->GetLocation();

        if (Formation::IsNullLocation(loc))
        {
            return false;
        }

        const MovementPriority priority = this->botAI->GetState() == BOT_STATE_COMBAT ? MovementPriority::MOVEMENT_COMBAT : MovementPriority::MOVEMENT_NORMAL;

        return this->MoveTo(
            loc.GetMapId(),
            loc.GetPositionX(),
            loc.GetPositionY(),
            loc.GetPositionZ(),
            false,
            false,
            false,
            true,
            priority,
            true
        );
    }

    Value<Unit*>* const targetValue = this->context->GetValue<Unit*>(target);

    if (targetValue == nullptr)
    {
        return false;
    }

    Unit* const targetUnit = targetValue->Get();

    if (targetUnit == nullptr)
    {
        return false;
    }

    return this->Follow(targetUnit);

    // This section has been commented out because it was forcing the pet to
    // follow the bot on every "follow" action tick, overriding any attack or
    // stay commands that might have been issued by the player.
    // if (Pet* pet = bot->GetPet())
    // {
    //     botAI->PetFollow();
    // }
    // if (moved)
    // botAI->SetNextCheckDelay(sPlayerbotAIConfig.reactDelay);

}

Unit* FollowAction::getResolvedFollowTarget(const std::string& target)
{
    if (target.empty())
    {
        Value<Unit*>* const groupLeaderValue = this->context->GetValue<Unit*>("group leader");

        if (groupLeaderValue == nullptr)
        {
            return nullptr;
        }

        return groupLeaderValue->Get();
    }

    Value<Unit*>* const targetValue = this->context->GetValue<Unit*>(target);

    if (targetValue == nullptr)
    {
        return nullptr;
    }

    return targetValue->Get();
}

bool FollowAction::isUseful()
{
    // move from group takes priority over follow as it's added and removed automatically
    // (without removing/adding follow)
    if (
        this->botAI->HasStrategy("move from group", BOT_STATE_COMBAT)
        || this->botAI->HasStrategy("move from group", BOT_STATE_NON_COMBAT)
    )
    {
        return false;
    }

    if (this->bot->GetCurrentSpell(CURRENT_CHANNELED_SPELL) != nullptr)
    {
        return false;
    }

    Value<Formation*>* const formationValue = this->context->GetValue<Formation*>("formation");

    if (formationValue == nullptr)
    {
        return false;
    }

    Formation* const formation = formationValue->Get();

    if (formation == nullptr)
    {
        return false;
    }

    const std::string target = formation->GetTargetName();
    const Unit* followTarget = this->getResolvedFollowTarget(target);

    if (followTarget == nullptr)
    {
        followTarget = this->botAI->GetMaster();
    }

    if (followTarget == nullptr)
    {
        Value<Unit*>* const groupLeaderValue = this->context->GetValue<Unit*>("group leader");

        if (groupLeaderValue != nullptr)
        {
            const Unit* const groupLeader = groupLeaderValue->Get();

            if (groupLeader != nullptr)
            {
                followTarget = groupLeader;
            }
        }
    }

    if (followTarget != nullptr)
    {
        if (followTarget->HasUnitState(UNIT_STATE_IN_FLIGHT))
        {
            return false;
        }

        if (!this->CanDeadFollow(followTarget))
        {
            return false;
        }

        if (followTarget->GetGUID() == this->bot->GetGUID())
        {
            return false;
        }
    }

    const float distance = this->getDistanceToTarget(*formation, target);

    if (this->botAI->HasStrategy("master fishing", BOT_STATE_NON_COMBAT))
    {
        return ServerFacade::instance().IsDistanceGreaterThan(distance, PlayerbotAIConfig::instance().fishingDistanceFromMaster);
    }

    return ServerFacade::instance().IsDistanceGreaterThan(distance, formation->GetMaxDistance());
}

float FollowAction::getDistanceToTarget(Formation& formation, const std::string& target)
{
    if (target.empty())
    {
        const WorldLocation loc = formation.GetLocation();

        if (Formation::IsNullLocation(loc) || this->bot->GetMapId() != loc.GetMapId())
        {
            return 0.0f;
        }

        return this->bot->GetDistance(loc.GetPositionX(), loc.GetPositionY(), loc.GetPositionZ());
    }

    Value<float>* const distanceToTargetValue = this->context->GetValue<float>("distance", target);

    if (distanceToTargetValue == nullptr)
    {
        return 0.0f;
    }

    return distanceToTargetValue->Get();
}

bool FollowAction::CanDeadFollow(const Unit* const target) const noexcept
{
    // In battleground, wait for spirit healer
    if (this->bot->InBattleground() && !this->bot->IsAlive())
    {
        return false;
    }

    // Move to corpse when dead and player is alive or not a ghost.
    if (!this->bot->IsAlive() && (target->IsAlive() || !target->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST)))
    {
        return false;
    }

    return true;
}

bool FleeToGroupLeaderAction::Execute(Event)
{
    Value<Unit*>* const groupLeaderValue = this->context->GetValue<Unit*>("group leader");

    if (groupLeaderValue == nullptr)
    {
        return false;
    }

    Unit* const fTarget = groupLeaderValue->Get();

    const bool canFollow = this->Follow(fTarget);

    if (!canFollow)
    {
        return false;
    }

    const WorldPosition targetPos{fTarget};
    WorldPosition botPos{bot};

    const float distance = botPos.fDist(targetPos);

    if (distance < PlayerbotAIConfig::instance().reactDistance * 3)
    {
        if (!urand(0, 3))
        {
            this->botAI->TellMaster("I am close, wait for me!");
        }
        this->botAI->SetNextCheckDelay(3000);

        return true;
    }

    if (distance < 1000.0f)
    {
        if (!urand(0, 10))
        {
            this->botAI->TellMaster("I heading to your position.");
        }
        this->botAI->SetNextCheckDelay(3000);

        return true;
    }

    if (!urand(0, 20))
    {
        this->botAI->TellMaster("I am traveling to your position.");
    }

    this->botAI->SetNextCheckDelay(3000);

    return true;
}

bool FleeToGroupLeaderAction::isUseful()
{
    if (!this->botAI->GetGroupLeader())
    {
        return false;
    }

    if (this->botAI->GetGroupLeader() == this->bot)
    {
        return false;
    }

    Value<Unit*>* const currentTargetValue = this->context->GetValue<Unit*>("current target");

    if (currentTargetValue == nullptr)
    {
        return false;
    }

    const Unit* const target = currentTargetValue->Get();

    if (target != nullptr && this->botAI->GetGroupLeader()->GetTarget() == target->GetGUID())
    {
        return false;
    }

    if (!this->botAI->HasStrategy("follow", BOT_STATE_NON_COMBAT))
    {
        return false;
    }

    Value<Unit*>* const groupLeaderValue = this->context->GetValue<Unit*>("group leader");

    if (groupLeaderValue == nullptr)
    {
        return false;
    }

    const Unit* const followTarget = groupLeaderValue->Get();

    if (!this->CanDeadFollow(followTarget))
    {
        return false;
    }

    return true;
}
