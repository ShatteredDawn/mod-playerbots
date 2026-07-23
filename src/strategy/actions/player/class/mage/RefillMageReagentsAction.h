/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/mage/definition/enum/MageReagents.enum.h"

class RefillMageReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillMageReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_mage_reagents") {}

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
		if (!this->hasEnoughRuneOfTeleportation())
		{
			return true;
		}

		if (!this->hasEnoughRuneOfPortals())
		{
			return true;
		}

		if (!this->hasEnoughArcanePowder())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughRuneOfTeleportation())
		{
			result = result && this->refillReagent(uint32_t(MageReagentsEnum::RUNE_OF_TELEPORTATION));
		}

		if (!this->hasEnoughRuneOfPortals())
		{
			result = result && this->refillReagent(uint32_t(MageReagentsEnum::RUNE_OF_PORTALS));
		}

		if (!this->hasEnoughArcanePowder())
		{
			result = result && this->refillReagent(uint32_t(MageReagentsEnum::ARCANE_POWDER));
		}

		return result;
	}

private:
	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 19;
	}

	[[nodiscard]] bool hasEnoughRuneOfTeleportation() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 20,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(MageReagentsEnum::RUNE_OF_TELEPORTATION)
			}
		);
	}

	[[nodiscard]] bool hasEnoughRuneOfPortals() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 40,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(MageReagentsEnum::RUNE_OF_PORTALS)
			}
		);
	}

	[[nodiscard]] bool hasEnoughArcanePowder() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 56,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(MageReagentsEnum::ARCANE_POWDER)
			}
		);
	}
};
