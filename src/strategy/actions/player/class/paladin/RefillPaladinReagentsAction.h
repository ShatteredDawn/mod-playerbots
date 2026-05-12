/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <cstdint>
#include <string>

#include "Action.h"
#include "PlayerbotAI.h"
#include "abstract/AbstractRefillReagentAction.h"
#include "class/paladin/definition/enum/PaladinReagents.enum.h"

class RefillPaladinReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillPaladinReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_paladin_reagents") {}

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
		if (!this->hasEnoughSymbolsOfDivinity())
		{
			return true;
		}

		if (!this->hasEnoughSymbolsOfKings())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughSymbolsOfDivinity())
		{
			result = result && this->refillReagent(uint32(PaladinReagentsEnum::SYMBOL_OF_DIVINITY));
		}

		if (!this->hasEnoughSymbolsOfKings())
		{
			result = result && this->refillReagent(uint32_t(PaladinReagentsEnum::SYMBOL_OF_KINGS));
		}

		return result;
	}

private:

	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 29;
	}

	[[nodiscard]] bool hasEnoughSymbolsOfKings() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 52,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(PaladinReagentsEnum::SYMBOL_OF_KINGS)
			}
		);
	}

	[[nodiscard]] bool hasEnoughSymbolsOfDivinity() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 20,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(PaladinReagentsEnum::SYMBOL_OF_DIVINITY)
			}
		);
	}
};
