/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "Player.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/shaman/definition/enum/ShamanReagents.enum.h"
#include "shaman/definition/enum/ShamanTotemRelic.enum.h"

// @TODO: Manage fish oil and shiny fish scales when auction house interaction is ready
class RefillShamanReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillShamanReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_shaman_reagents") {}

	bool isPossible() override
	{
		if (!this->isHighLevelEnough())
		{
			return false;
		}

		return this->bot->GetFreeInventorySpace() > 0;
	}

	bool isUseful() override
	{
		if (!this->hasEnoughEarthTotem())
		{
			return true;
		}

		if (!this->hasEnoughFireTotem())
		{
			return true;
		}

		if (!this->hasEnoughWaterTotem())
		{
			return true;
		}

		if (!this->hasEnoughAirTotem())
		{
			return true;
		}

		if (!this->hasEnoughAnkh())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughEarthTotem())
		{
			result = result && this->refillReagent(uint32_t(ShamanReagentsEnum::EARTH_TOTEM));
		}

		if (!this->hasEnoughFireTotem())
		{
			result = result && this->refillReagent(uint32_t(ShamanReagentsEnum::FIRE_TOTEM));
		}

		if (!this->hasEnoughWaterTotem())
		{
			result = result && this->refillReagent(uint32_t(ShamanReagentsEnum::WATER_TOTEM));
		}

		if (!this->hasEnoughAirTotem())
		{
			result = result && this->refillReagent(uint32_t(ShamanReagentsEnum::AIR_TOTEM));
		}

		if (!this->hasEnoughAnkh())
		{
			result = result && this->refillReagent(uint32_t(ShamanReagentsEnum::ANKH));
		}

		return result;
	}

private:
	static constexpr std::array<ShamanTotemRelicEnum, 71> RELIC_TOTEMS = {
		ShamanTotemRelicEnum::TOTEM_OF_THE_EARTHEN_RING,
		ShamanTotemRelicEnum::TOTEM_OF_RAGE,
		ShamanTotemRelicEnum::TOTEM_OF_REBIRTH,
		ShamanTotemRelicEnum::TOTEM_OF_FLOWING_WATER,
		ShamanTotemRelicEnum::TOTEM_OF_LIFE,
		ShamanTotemRelicEnum::TOTEM_OF_THE_STORM,
		ShamanTotemRelicEnum::TOTEM_OF_SUSTAINING,
		ShamanTotemRelicEnum::TOTEM_OF_LIGHTNING,
		ShamanTotemRelicEnum::TOTEM_OF_IMPACT_A,
		ShamanTotemRelicEnum::TOTEM_OF_IMPACT_H,
		ShamanTotemRelicEnum::TOTEM_OF_THE_THUNDERHEAD,
		ShamanTotemRelicEnum::TOTEM_OF_THE_PLAINS,
		ShamanTotemRelicEnum::STORMFURY_TOTEM,
		ShamanTotemRelicEnum::TOTEM_OF_THE_TUNDRA,
		ShamanTotemRelicEnum::BRUTAL_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::BRUTAL_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::BRUTAL_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::VENGEFUL_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::VENGEFUL_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::VENGEFUL_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_ANCESTRAL_GUIDANCE,
		ShamanTotemRelicEnum::MERCILESS_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::MERCILESS_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::MERCILESS_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_THE_MAELSTROM,
		ShamanTotemRelicEnum::TOTEM_OF_LIVING_WATER,
		ShamanTotemRelicEnum::STONEBREAKER_TOTEM,
		ShamanTotemRelicEnum::SKYCALL_TOTEM,
		ShamanTotemRelicEnum::TOTEM_OF_THE_VOID,
		ShamanTotemRelicEnum::TOTEM_OF_THE_ASTRAL_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_SPONTANEOUS_REGROWTH,
		ShamanTotemRelicEnum::TOTEM_OF_HEALING_RAINS,
		ShamanTotemRelicEnum::GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::TOTEM_OF_THE_PULSING_EARTH,
		ShamanTotemRelicEnum::TOTEM_OF_THE_BAY,
		ShamanTotemRelicEnum::VENTURE_CO_LIGHTNING_ROD,
		ShamanTotemRelicEnum::VENTURE_CO_FLAME_SLICER,
		ShamanTotemRelicEnum::WRATHFUL_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::WRATHFUL_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::WRATHFUL_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_THE_SURGING_SEA,
		ShamanTotemRelicEnum::TOTEM_OF_THE_AVALANCHE,
		ShamanTotemRelicEnum::RELENTLESS_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::RELENTLESS_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::RELENTLESS_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_ELECTRIFYING_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_CALMING_TIDES,
		ShamanTotemRelicEnum::TOTEM_OF_QUAKING_EARTH,
		ShamanTotemRelicEnum::FURIOUS_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::FURIOUS_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::FURIOUS_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_THE_DANCING_FLAME,
		ShamanTotemRelicEnum::STEAMCALLER_TOTEM,
		ShamanTotemRelicEnum::THUNDERFALL_TOTEM,
		ShamanTotemRelicEnum::TOTEM_OF_DUELING,
		ShamanTotemRelicEnum::TOTEM_OF_HEX,
		ShamanTotemRelicEnum::DEADLY_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::DEADLY_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::DEADLY_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::TOTEM_OF_MISERY,
		ShamanTotemRelicEnum::TOTEM_OF_THE_ELEMENTAL_PLANE,
		ShamanTotemRelicEnum::TOTEM_OF_SPLINTERING,
		ShamanTotemRelicEnum::TOTEM_OF_FOREST_GROWTH,
		ShamanTotemRelicEnum::SAVAGE_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::SAVAGE_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::SAVAGE_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
		ShamanTotemRelicEnum::HATEFUL_GLADIATOR_TOTEM_OF_SURVIVAL,
		ShamanTotemRelicEnum::HATEFUL_GLADIATOR_TOTEM_OF_INDOMITABILITY,
		ShamanTotemRelicEnum::HATEFUL_GLADIATOR_TOTEM_OF_THE_THIRD_WIND,
	};

	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 3;
	}

	[[nodiscard]] bool hasRelicTotemEquipped() const noexcept
	{
		const Item* const equippedRelic = this->bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);

		return equippedRelic == nullptr;
	}

	[[nodiscard]] bool hasEnoughEarthTotem() const noexcept
	{
		if (this->hasRelicTotemEquipped())
		{
			return true;
		}

		return this->hasEnoughReagent(
			{
				.minimumLevel = 4,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(ShamanReagentsEnum::EARTH_TOTEM)
			}
		);
	}

	[[nodiscard]] bool hasEnoughFireTotem() const noexcept
	{
		if (this->hasRelicTotemEquipped())
		{
			return true;
		}

		return this->hasEnoughReagent(
			{
				.minimumLevel = 10,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(ShamanReagentsEnum::FIRE_TOTEM)
			}
		);
	}

	[[nodiscard]] bool hasEnoughWaterTotem() const noexcept
	{
		if (this->hasRelicTotemEquipped())
		{
			return true;
		}

		return this->hasEnoughReagent(
			{
				.minimumLevel = 20,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(ShamanReagentsEnum::WATER_TOTEM)
			}
		);
	}

	[[nodiscard]] bool hasEnoughAirTotem() const noexcept
	{
		if (this->hasRelicTotemEquipped())
		{
			return true;
		}

		return this->hasEnoughReagent(
			{
				.minimumLevel = 30,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(ShamanReagentsEnum::AIR_TOTEM)
			}
		);
	}

	[[nodiscard]] bool hasEnoughAnkh() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 30,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(ShamanReagentsEnum::ANKH)
			}
		);
	}
};
