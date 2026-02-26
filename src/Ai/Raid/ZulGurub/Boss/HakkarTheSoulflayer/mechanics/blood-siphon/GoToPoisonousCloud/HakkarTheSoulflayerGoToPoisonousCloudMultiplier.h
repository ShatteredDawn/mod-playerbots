#pragma once

#include "AiObjectContext.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"
#include "Multiplier.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerGoToPoisonousCloudMultiplier : public Multiplier
{
public:
    HakkarTheSoulflayerGoToPoisonousCloudMultiplier(PlayerbotAI* botAI) : Multiplier(botAI, "hakkar the soulflayer go to poisonous cloud") {}

    float GetValue(Action& action) override
    {
        if (this->bot == nullptr)
        {
            return 0.0f;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();

        if (hakkarTheSoulflayerAssistant.isInCombatWithHakkar(*this->bot) == false)
        {
            return 1.0f;
        }

        if (this->bot->HasAura(uint32_t(PoisonousCloudEnum::AURA_POISONOUS_BLOOD)) == true)
        {
            return 1.0f;
        }

        const SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();
        const Unit* const poisonousCloud = sonOfHakkarAssistant.findPoisonousCloud(*this->bot);

        if (poisonousCloud == nullptr)
        {
            return 1.0f;
        }

        const Position poisonousCloudPosition = poisonousCloud->GetPosition();
        const float distanceToPoisonousCloud = this->bot->GetExactDist2d(poisonousCloudPosition.GetPositionX(), poisonousCloudPosition.GetPositionY());
        static constexpr float maxDistance = 5.0f;

        if (distanceToPoisonousCloud < maxDistance)
        {
            return 1.0f;
        }

        const MovementAction* const movementAction = dynamic_cast<const MovementAction*>(&action);

        if (movementAction != nullptr)
        {
            return 1.0f;
        }

        return 0.0f;
    }
};
