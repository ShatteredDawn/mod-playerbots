#pragma once

#include "Group.h"
#include "Map.h"
#include "HakkarTheSoulflayer/definition/enum/HakkarTheSoulflayerEnum.h"
#include "SonOfHakkarEnum.h"
#include "Unit.h"
#include "Player.h"
#include "UnitScript.h"
#include "PlayerbotAI.h"
#include "utility/StopWatch.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerScript : public UnitScript
{
public:
    HakkarTheSoulflayerScript() : UnitScript("HakkarTheSoulflayerScript") {}

    void OnUnitEnterCombat(Unit* hakkar, Unit* unit) override
    {
        if (hakkar == nullptr)
        {
            return;
        }

        if (hakkar->GetEntry() != uint32_t(HakkarTheSoulflayerEnum::ENTRY))
        {
            return;
        }

        const uint32_t instanceId = unit->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, unit->GetMapId());
        RaidTankAssistant& raidTankAssistant = raidLeader.getRaidTankAssistant();
        HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();
        SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        Player* const player = dynamic_cast<Player*>(unit);

        if (player == nullptr)
        {
            return;
        }

        raidTankAssistant.initialiseTanks(*player);
        hakkarTheSoulflayerAssistant.restartBloodSiphonWatch();
        sonOfHakkarAssistant.initialiseDesignatedPuller(*player);
        sonOfHakkarAssistant.initialiseSonsOfHakkar(*player);
    }

    void OnUnitDeath(Unit* unit, Unit*) override
    {
        if (unit == nullptr)
        {
            return;
        }

        if (unit->GetEntry() != uint32_t(SonOfHakkarEnum::ENTRY))
        {
            return;
        }

        const uint32_t instanceId = unit->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, unit->GetMapId());
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();
        SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        const SonOfHakkarMemory& memory = sonOfHakkarAssistant.getMemory();

        const Creature* const leftSonOfHakkar = memory.getLeftSonOfHakkar();
        const Creature* const rightSonOfHakkar = memory.getRightSonOfHakkar();

        if (leftSonOfHakkar == nullptr || rightSonOfHakkar == nullptr)
        {
            return;
        }

        const HakkarTheSoulflayerMemory& hakkarTheSoulflayerMemory = hakkarTheSoulflayerAssistant.getMemory();
        const bool isLeftSonOfHakkarKilled = unit->GetGUID() == leftSonOfHakkar->GetGUID();
        const bool isRightSonOfHakkarKilled = unit->GetGUID() == rightSonOfHakkar->GetGUID();
        const bool isKilledOnTimeForBloodSiphon = hakkarTheSoulflayerMemory.getBloodSiphonWatch().getElapsedSeconds() <= double(PoisonousCloudEnum::AURA_POISONOUS_BLOOD_DURATION);

        if ((isLeftSonOfHakkarKilled || isRightSonOfHakkarKilled) && isKilledOnTimeForBloodSiphon)
        {
            sonOfHakkarAssistant.setSonOfHakkarKilledForCurrentBloodSiphon(true);
        }
    }

    void OnAuraApply(Unit* unit, Aura* aura) noexcept override
    {
        if (unit == nullptr || aura == nullptr)
        {
            return;
        }

        if (aura->GetSpellInfo()->Id != uint32_t(HakkarTheSoulflayerEnum::SPELL_BLOOD_SIPHON))
        {
            return;
        }

        if (unit->GetEntry() != uint32_t(HakkarTheSoulflayerEnum::ENTRY))
        {
            return;
        }

        const uint32_t instanceId = unit->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, unit->GetMapId());
        HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();
        SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        sonOfHakkarAssistant.setSonOfHakkarKilledForCurrentBloodSiphon(false);
        sonOfHakkarAssistant.setActiveSonOfHakkar(nullptr);
        hakkarTheSoulflayerAssistant.restartBloodSiphonWatch();
    }

};
