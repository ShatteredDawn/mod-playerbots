#pragma once

#include "Spell.h"

#include "AiObjectContext.h"
#include "HunterActions.h"
#include "MageActions.h"
#include "PaladinActions.h"
#include "PriestActions.h"
#include "RogueActions.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"

#include "WarlockActions.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerGoToPoisonousCloudAction : public MovementAction
{
public:
    HakkarTheSoulflayerGoToPoisonousCloudAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer go to poisonous cloud"
    ) : MovementAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        if (this->botAI == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();
        const Unit* const poisonousCloud = sonOfHakkarAssistant.findPoisonousCloud(*this->bot);

        if (poisonousCloud == nullptr)
        {
            return false;
        }

        const Position poisonousCloudPosition = poisonousCloud->GetPosition();
        const float distanceToPoisonousCloud = this->bot->GetExactDist2d(poisonousCloudPosition.GetPositionX(), poisonousCloudPosition.GetPositionY());
        static constexpr float maxDistance = 5.0f;

        if (distanceToPoisonousCloud < maxDistance)
        {
            return false;
        }

        return this->MoveTo(MAP_ZUL_GURUB, poisonousCloudPosition.GetPositionX(), poisonousCloudPosition.GetPositionY(), poisonousCloudPosition.GetPositionZ());
    }
};
