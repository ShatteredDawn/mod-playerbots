#pragma once

#include "HakkarTheSoulflayer/definition/enum/PoisonousCloudEnum.h"
#include "HakkarTheSoulflayer/definition/enum/SonOfHakkarEnum.h"
#include "PlayerbotAI.h"
#include "RaidBossHelpers.h"
#include "Unit.h"
#include "Player.h"
#include "Spell.h"

#include "GridNotifiers.h"
// Necessary due to a poor implementation by AC.
#include "GridNotifiersImpl.h"

#include "Cell.h"
#include "CellImpl.h"

#include "PlayerbotMgr.h"
#include "PlayerbotAIConfig.h"
#include "assistant/son-of-hakkar/memory/SonOfHakkarMemory.h"

class SonOfHakkarAssistant
{
private:
    SonOfHakkarMemory memory{};

public:
    SonOfHakkarAssistant() = default;
    ~SonOfHakkarAssistant() = default;

    void setSonOfHakkarKilledForCurrentBloodSiphon(bool value) noexcept
    {
        this->memory.setSonOfHakkarKilledForCurrentBloodSiphon(value);
    }

    [[nodiscard]] Unit* findPoisonousCloud(Player& bot) const noexcept
    {
        return bot.FindNearestCreature(uint32_t(PoisonousCloudEnum::ENTRY), PlayerbotAIConfig::instance().sightDistance, true);
    }

    void setActiveSonOfHakkar(Creature* sonOfHakkar) noexcept
    {
        this->memory.setActiveSonOfHakkar(sonOfHakkar);
    }

    void initialiseDesignatedPuller(Player& player) noexcept
    {
        const Group* const group = player.GetGroup();

        if (group == nullptr)
        {
            return;
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

            if (player->IsAlive() == false)
            {
                member = member->next();

                continue;
            }

            if (PlayerbotAI::IsRanged(player) == false || PlayerbotAI::IsDps(player) == false)
            {
                member = member->next();

                continue;
            }

            MarkTargetWithTriangle(player, player);
            this->memory.setSonOfHakkarDesignatedPuller(player);

            PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(player);

            botAI->SayToRaid("I will attract the Son of Hakkar when the time comes!");

            break;
        }
    }

    void initialiseSonsOfHakkar(Player& player) noexcept
    {
        this->memory.setSonOfHakkarKilledForCurrentBloodSiphon(false);

        Creature* const leftSonOfHakkar = this->findSonOfHakkar(player, uint32_t(SonOfHakkarEnum::LEFT_SON_OF_HAKKAR_GUID));

        this->memory.setLeftSonOfHakkar(leftSonOfHakkar);

        MarkTargetWithCross(&player, leftSonOfHakkar);

        Creature* const rightSonOfHakkar = this->findSonOfHakkar(player, uint32_t(SonOfHakkarEnum::RIGHT_SON_OF_HAKKAR_GUID));

        this->memory.setRightSonOfHakkar(rightSonOfHakkar);

        MarkTargetWithStar(&player, rightSonOfHakkar);
    }

    [[nodiscard]] Creature* findSonOfHakkar(Player& player, uint32_t dbGuid) const
    {
        Map* map = player.GetMap();

        if (map == nullptr)
        {
            return nullptr;
        }

        const std::unordered_multimap<ObjectGuid::LowType, Creature*> store = map->GetCreatureBySpawnIdStore();
        const std::unordered_multimap<ObjectGuid::LowType, Creature*>::const_iterator it = store.find(dbGuid);
        const std::unordered_multimap<ObjectGuid::LowType, Creature*>::const_iterator end = store.end();

        if (it == end)
        {
            return nullptr;
        }

        Creature* sonOfHakkar = it->second;

        if (sonOfHakkar == nullptr)
        {
            return nullptr;
        }

        if (!sonOfHakkar->IsAlive())
        {
            return nullptr;
        }

         if (sonOfHakkar->GetEntry() != uint32_t(SonOfHakkarEnum::ENTRY))
        {
            return nullptr;
        }

        return sonOfHakkar;
    }

    [[nodiscard]] Creature* findAnyAliveSonOfHakkar() const noexcept
    {
        Creature* const leftSonOfHakkar = this->memory.getLeftSonOfHakkar();

        if (leftSonOfHakkar != nullptr && leftSonOfHakkar->IsAlive())
        {
            return leftSonOfHakkar;
        }

        Creature* const rightSonOfHakkar = this->memory.getRightSonOfHakkar();

        if (rightSonOfHakkar != nullptr && rightSonOfHakkar->IsAlive())
        {
            return rightSonOfHakkar;
        }

        return nullptr;
    }

    [[nodiscard]] float getDistanceToActiveSonOfHakkar(Player& bot) const noexcept
    {
        const Unit* const sonOfHakkar = this->memory.getActiveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return 0.0f;
        }

        return bot.GetExactDist(
            sonOfHakkar->GetPositionX(),
            sonOfHakkar->GetPositionY(),
            sonOfHakkar->GetPositionZ()
        );
    }

    [[nodiscard]] const SonOfHakkarMemory& getMemory() const noexcept
    {
        return this->memory;
    }

    [[nodiscard]] SonOfHakkarMemory& getMemory() noexcept
    {
        return this->memory;
    }
};
