/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/warlock/definition/enum/WarlockReagents.enum.h"

class RefillWarlockReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillWarlockReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_warlock_reagents") {}

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
		if (!this->hasEnoughInfernalStones())
		{
			return true;
		}

		if (!this->hasEnoughDemonicFigures())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughInfernalStones())
		{
			result = result && this->refillReagent(uint32_t(WarlockReagentsEnum::INFERNAL_STONE));
		}

		if (!this->hasEnoughDemonicFigures())
		{
			result = result && this->refillReagent(uint32_t(WarlockReagentsEnum::DEMONIC_FIGURE));
		}

		return result;
	}

private:
	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 54;
	}

	[[nodiscard]] bool hasEnoughInfernalStones() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 50,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(WarlockReagentsEnum::INFERNAL_STONE)
			}
		);
	}

	[[nodiscard]] bool hasEnoughDemonicFigures() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 60,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(WarlockReagentsEnum::DEMONIC_FIGURE)
			}
		);
	}
};
