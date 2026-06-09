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
#include "class/rogue/definition/enum/RogueReagents.enum.h"

class RefillRogueReagentsAction : public AbstractRefillReagentAction
{
public:
    RefillRogueReagentsAction(PlayerbotAI* botAI) : AbstractRefillReagentAction(botAI, "refill_rogue_reagents") {}

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
		if (!this->hasEnoughFlashPowder())
		{
			return true;
		}

		return false;
	}

    bool Execute(Event event) override
	{
		bool result = true;

		if (!this->hasEnoughFlashPowder())
		{
			result = result && this->refillReagent(uint32(RogueReagentsEnum::FLASH_POWDER));
		}

		return result;
	}

private:

	// @TODO: It might be a good idea to check if the bot has any spell requiring the reagent.
	// This would allow people to customise their spells. Niche use.
	[[nodiscard]] bool isHighLevelEnough() const noexcept
	{
		return this->bot->GetLevel() > 21;
	}

	[[nodiscard]] bool hasEnoughFlashPowder() const noexcept
	{
		return this->hasEnoughReagent(
			{
				.minimumLevel = 22,
				.maximumLevel = 0,
				.itemTemplateId = uint32_t(RogueReagentsEnum::FLASH_POWDER)
			}
		);
	}
};
