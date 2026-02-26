#pragma once

#include "../../../../domain/core/raid/leader/BaseRaidLeader.h"

#include "Ai/Raid/ZulGurub/Boss/HighPriestVenoxis/assistant/HighPriestVenoxisAssistant.h"
#include "Ai/Raid/ZulGurub/Boss/HakkarTheSoulflayer/assistant/hakkar-the-soulflayer/HakkarTheSoulflayerAssistant.h"
#include "Ai/Raid/ZulGurub/Boss/HakkarTheSoulflayer/assistant/son-of-hakkar/SonOfHakkarAssistant.h"
#include "GrilekTheWanderer/assistant/GrilekTheWandererAssistant.h"

class ZulGurubRaidLeader : public BaseRaidLeader
{
private:
    HighPriestVenoxisAssistant highPriestVenoxisAssistant{};
    GrilekTheWandererAssistant grilekTheWandererAssistant{};
    HakkarTheSoulflayerAssistant hakkarTheSoulflayerAssistant{};
    SonOfHakkarAssistant sonOfHakkarAssistant{};

public:
    ZulGurubRaidLeader() = default;
    ~ZulGurubRaidLeader() = default;

    [[nodiscard]] const HighPriestVenoxisAssistant& getHighPriestVenoxisAssistant() const noexcept
    {
        return this->highPriestVenoxisAssistant;
    }

	[[nodiscard]] const GrilekTheWandererAssistant& getGrilekTheWandererAssistant() const noexcept
	{
		return this->grilekTheWandererAssistant;
	}

    [[nodiscard]] HakkarTheSoulflayerAssistant& getHakkarTheSoulflayerAssistant() noexcept
    {
        return this->hakkarTheSoulflayerAssistant;
    }

    [[nodiscard]] const HakkarTheSoulflayerAssistant& getHakkarTheSoulflayerAssistant() const noexcept
    {
        return this->hakkarTheSoulflayerAssistant;
    }

    [[nodiscard]] SonOfHakkarAssistant& getSonOfHakkarAssistant() noexcept
    {
        return this->sonOfHakkarAssistant;
    }

    [[nodiscard]] const SonOfHakkarAssistant& getSonOfHakkarAssistant() const noexcept
    {
        return this->sonOfHakkarAssistant;
    }

    [[nodiscard]] bool isInInstance(Player& player) const noexcept
    {
        return player.GetMapId() == MAP_ZUL_GURUB;
    }
};
