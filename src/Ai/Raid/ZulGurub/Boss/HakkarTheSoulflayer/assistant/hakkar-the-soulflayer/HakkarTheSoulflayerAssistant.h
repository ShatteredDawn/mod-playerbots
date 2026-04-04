#pragma once

#include "PlayerbotAI.h"
#include "ThreatManager.h"
#include "Unit.h"
#include "Player.h"
#include "Spell.h"
#include "Position.h"

#include "GridNotifiers.h"
// Necessary due to a poor implementation by AC.
#include "GridNotifiersImpl.h"

#include "PlayerbotMgr.h"
#include "PlayerbotAIConfig.h"

#include "../definition/enum/HakkarTheSoulflayerEnum.h"
#include "../../../../../../domain/core/raid/assistant/tank-assistant/memory/RaidTankAssistantMemory.h"
#include "raid/assistant/tank-assistant/RaidTankAssistant.h"
#include "Ai/Raid/ZulGurub/Boss/HakkarTheSoulflayer/assistant/hakkar-the-soulflayer/memory/HakkarTheSoulflayerMemory.h"
#include "domain/core/raid/leader/definition/struct/RaidLeaderContextStruct.h"

class HakkarTheSoulflayerAssistant
{
private:
    HakkarTheSoulflayerMemory memory{};

public:
    HakkarTheSoulflayerAssistant() = default;
    ~HakkarTheSoulflayerAssistant() = default;

    void restartBloodSiphonWatch() noexcept
    {
        this->memory.getBloodSiphonWatch().reset();
        this->memory.getBloodSiphonWatch().start();
    }

    [[nodiscard]] constexpr float getHakkarMaxPositionDistance() const noexcept
    {
        return 4.0f;
    }

    [[nodiscard]] Position getHakkarPosition() const noexcept
    {
        return Position{
            -11787.951f,
            -1655.4058f,
            53.128f,
        };
    }

    [[nodiscard]] bool hasExcessiveThreat(Player& bot, RaidLeaderContextStruct context) const noexcept
    {
        if (this->isInCombatWithHakkar(bot) == false)
        {
            return false;
        }

        if (PlayerbotAI::IsTank(&bot) == true)
        {
            return false;
        }

        Unit* const hakkar = this->findActiveBoss(bot);

        if (hakkar == nullptr)
        {
            return false;
        }

        const RaidTankAssistantMemory& raidTankMemory = context.raidTankAssistant.getMemory();

        Player* const mainTank = raidTankMemory.getMainTank();

        if (mainTank == nullptr)
        {
            return false;
        }

        ThreatManager& threatMgr = hakkar->GetThreatMgr();

        const float botThreat = threatMgr.GetThreat(&bot);
        const float mainTankThreat = threatMgr.GetThreat(mainTank);

        if (mainTankThreat <= 0.0f)
        {
            return false;
        }

        Player* const secondTank = raidTankMemory.getSecondTank();

        if (secondTank == nullptr)
        {
            return false;
        }

        const float secondTankThreat = threatMgr.GetThreat(secondTank);

        if (secondTankThreat <= 0.0f)
        {
            return false;
        }

        const float secondThreatPct = (botThreat / secondTankThreat) * 100.0f;

        return secondThreatPct >= float(HakkarTheSoulflayerEnum::NON_TANK_THREAT_THRESHOLD);
    }

    [[nodiscard]] Player* findSecondInThreat(Player& bot) const noexcept
    {
        const Unit* const hakkar = this->findActiveBoss(bot);

        if (hakkar == nullptr)
        {
            return nullptr;
        }

        if (hakkar->IsAlive() == false)
        {
            return nullptr;
        }

        if (hakkar->IsInCombat() == false)
        {
            return nullptr;
        }

        const ThreatManager& threatMgr = hakkar->GetThreatMgr();

        if (threatMgr.GetThreatListSize() < 2)
        {
            return nullptr;
        }

        const Acore::IteratorPair<ThreatManager::ThreatListIterator> threatList = hakkar->GetThreatMgr().GetSortedThreatList();

        uint8_t index = 0;

        for (const ThreatReference* const threatRef : threatList)
        {
            if (threatRef == nullptr)
            {
                continue;
            }

            if (index == 2)
            {
                Unit* const threatUnit = threatRef->GetVictim();

                if (threatUnit == nullptr)
                {
                    return nullptr;
                }

                Player* const playerThreatUnit = dynamic_cast<Player*>(threatUnit);

                return playerThreatUnit;
            }

            ++index;
        }

        return nullptr;
    }

    [[nodiscard]] Player* findInsaneAlly(Player& bot) const noexcept
    {
        if (this->isInCombatWithHakkar(bot) == false)
        {
            return nullptr;
        }

        const Group* const group = bot.GetGroup();

        if (group == nullptr)
        {
            return nullptr;
        }

        const GroupReference* member = group->GetFirstMember();

        while (member != nullptr)
        {
            Player* const player = member->GetSource();

            if (player == nullptr)
            {
                member = member->next();

                continue;
            }

            const bool affectedByInsanity = player->HasAura(uint32_t(HakkarTheSoulflayerEnum::SPELL_CAUSE_INSANITY));

            if (affectedByInsanity)
            {
                return player;
            }

            member = member->next();
        }

        return nullptr;
    }

    [[nodiscard]] bool isInCombatWithHakkar(Player& bot) const noexcept
    {
        const Unit* const hakkar = this->findActiveBoss(bot);

        if (hakkar == nullptr)
        {
            return false;
        }

        return bot.IsInCombat() && hakkar->IsInCombat();
    }

    [[nodiscard]] Unit* findActiveBoss(Player& bot) const noexcept
    {
        if (!bot.IsInCombat())
        {
            return nullptr;
        }

        return bot.FindNearestCreature(uint32_t(HakkarTheSoulflayerEnum::ENTRY), PlayerbotAIConfig::instance().sightDistance, true);
    };

    [[nodiscard]] const HakkarTheSoulflayerMemory& getMemory() const noexcept
    {
        return this->memory;
    }
};
