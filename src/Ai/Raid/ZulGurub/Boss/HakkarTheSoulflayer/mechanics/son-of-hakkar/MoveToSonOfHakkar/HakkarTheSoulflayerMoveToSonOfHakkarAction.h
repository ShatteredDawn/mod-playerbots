#pragma once

#include "HunterActions.h"
#include "MageActions.h"
#include "PaladinActions.h"
#include "PriestActions.h"
#include "RogueActions.h"
#include "Spell.h"
#include "AiObjectContext.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"

#include "WarlockActions.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"


class HakkarTheSoulflayerMoveToSonOfHakkarAction : public MovementAction
{
public:
    HakkarTheSoulflayerMoveToSonOfHakkarAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer move to son of hakkar"
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
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        Creature* const sonOfHakkar = sonOfHakkarAssistant.findAnyAliveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return false;
        }

        sonOfHakkarAssistant.setActiveSonOfHakkar(sonOfHakkar);

        const bool meleeBot = PlayerbotAI::IsMelee(this->bot);
        const float maxDistance = meleeBot ? 5.0f : 20.0f;

        return this->ReachCombatTo(sonOfHakkar, maxDistance);
    }
};
